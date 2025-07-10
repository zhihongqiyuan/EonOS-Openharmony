/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "event_runner.h"

#include <condition_variable>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

#include <unistd.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

#include "event_handler.h"
#include "event_queue_base.h"
#include "event_inner_runner.h"
#include "event_logger.h"
#include "securec.h"
#include "singleton.h"
#ifdef FFRT_USAGE_ENABLE
#include "event_queue_ffrt.h"
#include "ffrt_inner.h"
#endif  // FFRT_USAGE_ENABLE


namespace OHOS {
namespace AppExecFwk {
namespace {
const char *g_crashEmptyDumpInfo = "Current Event Caller is empty. Nothing to dump";
const int CRASH_BUF_MIN_LEN = 2;
constexpr int64_t MIN_APP_UID = 20000;
thread_local static Caller g_currentEventCaller = {};
thread_local static std::string g_currentEventName = {};

DEFINE_EH_HILOG_LABEL("EventRunner");

// Invoke system call to set name of current thread.
inline void SystemCallSetThreadName(const std::string &name)
{
    if (prctl(PR_SET_NAME, name.c_str()) < 0) {
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to set thread name, %{public}s", errmsg);
    }
    HILOGD("thread name is %{public}s", name.c_str());
}

// Help to calculate hash code of object.
template<typename T>
inline size_t CalculateHashCode(const T &obj)
{
    std::hash<T> calculateHashCode;
    return calculateHashCode(obj);
}

// Thread collector is used to reclaim thread that needs to finish running.
class ThreadCollector : public DelayedRefSingleton<ThreadCollector> {
    DECLARE_DELAYED_REF_SINGLETON(ThreadCollector);

public:
    DISALLOW_COPY_AND_MOVE(ThreadCollector);

    using ExitFunction = std::function<void()>;

    void ReclaimCurrentThread()
    {
        // Get id of current thread.
        auto threadId = std::this_thread::get_id();
        HILOGD("Thread id: %{public}zu", CalculateHashCode(threadId));

        {
            // Add thread id to list and notify to reclaim.
            std::lock_guard<std::mutex> lock(collectorLock_);
            if (destroying_) {
                HILOGI("Thread collector is destroying");
                return;
            }

            reclaims_.emplace_back(threadId);
            if (isWaiting_) {
                condition_.notify_one();
            }
        }

        if (threadLock_.try_lock()) {
            if ((!thread_) && (needCreateThread_)) {
                // Start daemon thread to collect finished threads, if not exist.
                thread_ = std::make_unique<std::thread>(&ThreadCollector::Run, this);
            }
            threadLock_.unlock();
        }
    }

    bool Deposit(std::unique_ptr<std::thread> &thread, const ExitFunction &threadExit)
    {
        if ((!thread) || (!thread->joinable()) || (!threadExit)) {
            auto threadState = thread ? (thread->joinable() ? "active" : "finished") : "null";
            HILOGE("%{public}s, %{public}s: Invalid parameter", threadState, threadExit ? "valid" : "null");
            return false;
        }

        auto threadId = thread->get_id();
        HILOGD("New thread id: %{public}zu", CalculateHashCode(threadId));
        // Save these information into map.
        std::lock_guard<std::mutex> lock(collectorLock_);
        if (destroying_) {
            HILOGW("Collector thread is destroying");
            return false;
        }
        // Save thread object and its exit callback.
        depositMap_.emplace(threadId,
            ThreadExitInfo {
                .thread = std::move(thread),
                .threadExit = threadExit,
            });
        return true;
    }

private:
    DEFINE_EH_HILOG_LABEL("ThreadCollector");

    struct ThreadExitInfo {
        std::unique_ptr<std::thread> thread;
        ExitFunction threadExit;
    };

    inline void ReclaimAll()
    {
        HILOGD("enter");
        std::unique_lock<std::mutex> lock(collectorLock_);
        // All thread deposited need to stop one by one.
        while (!depositMap_.empty()) {
            DoReclaimLocked(lock, depositMap_.begin());
        }
    }

    void Stop()
    {
        HILOGD("enter");
        {
            // Stop the collect thread, while destruction of collector.
            std::lock_guard<std::mutex> lock(collectorLock_);
            destroying_ = true;
            if (isWaiting_) {
                condition_.notify_all();
            }
        }

        {
            std::lock_guard<std::mutex> lock(threadLock_);
            if ((thread_) && (thread_->joinable())) {
                // Wait utils collect thread finished, if exists.
                thread_->join();
            }
            needCreateThread_ = false;
        }

        ReclaimAll();
    }

    void DoReclaimLocked(std::unique_lock<std::mutex> &lock,
        std::unordered_map<std::thread::id, ThreadExitInfo>::iterator it, bool needCallExit = true)
    {
        if (it == depositMap_.end()) {
            return;
        }

        // Remove thread information from map.
        auto threadId = it->first;
        auto exitInfo = std::move(it->second);
        (void)depositMap_.erase(it);

        // Unlock, because stopping thread maybe spend lot of time, it should be out of the lock.
        lock.unlock();

        size_t hashThreadId = CalculateHashCode(threadId);
        HILOGD("Thread id: %{public}zu", hashThreadId);
        if (needCallExit) {
            // Call exit callback to stop loop in thread.
            exitInfo.threadExit();
        }
        // Wait until thread finished.
        exitInfo.thread->join();
        HILOGD("Done, thread id: %{public}zu", hashThreadId);

        // Lock again.
        lock.lock();
    }

    void Run()
    {
        HILOGD("Collector thread is started");

        std::unique_lock<std::mutex> lock(collectorLock_);
        while (!destroying_) {
            // Reclaim threads in list one by one.
            while (!reclaims_.empty()) {
                auto threadId = reclaims_.back();
                reclaims_.pop_back();
                DoReclaimLocked(lock, depositMap_.find(threadId), false);
            }

            // Maybe stop running while doing reclaim, so check before waiting.
            if (destroying_) {
                break;
            }

            isWaiting_ = true;
            condition_.wait(lock);
            isWaiting_ = false;
        }

        HILOGD("Collector thread is stopped");
    }

    std::mutex collectorLock_;
    std::condition_variable condition_;
    bool isWaiting_ {false};
    bool destroying_ {false};
    std::vector<std::thread::id> reclaims_;
    std::unordered_map<std::thread::id, ThreadExitInfo> depositMap_;

    std::mutex threadLock_;
    // Thread for collector
    std::unique_ptr<std::thread> thread_;
    bool needCreateThread_ {true};

    // Avatar of thread collector, used to stop collector at the specified opportunity.
    class Avatar {
    public:
        DISALLOW_COPY_AND_MOVE(Avatar);

        Avatar() = default;
        ~Avatar()
        {
            HILOGD("enter");
            if (avatarEnabled_) {
                GetInstance().avatarDestructed_ = true;
                GetInstance().Stop();
            }
        }

        static inline void Disable()
        {
            avatarEnabled_ = false;
        }
    };

    // Mark whether avatar is destructed.
    volatile bool avatarDestructed_ {false};
    // Mark whether avatar is enabled.
    static volatile bool avatarEnabled_;
    static Avatar avatar_;
};

ThreadCollector::ThreadCollector()
    : collectorLock_(), condition_(), reclaims_(), depositMap_(), threadLock_(), thread_(nullptr)
{
    // Thread collector is created, so enable avatar.
    HILOGD("enter");
    avatarEnabled_ = true;
}

ThreadCollector::~ThreadCollector()
{
    // If avatar is not destructed, stop collector by itself.
    HILOGD("enter");
    if (!avatarDestructed_) {
        avatar_.Disable();
        Stop();
    }
}

typedef void(*ThreadInfoCallback)(char *buf, size_t len, void *ucontext);
extern "C" void SetThreadInfoCallback(ThreadInfoCallback func) __attribute__((weak));

class EventRunnerImpl final : public EventInnerRunner {
public:
    explicit EventRunnerImpl(const std::shared_ptr<EventRunner> &runner) : EventInnerRunner(runner)
    {
        HILOGD("enter");
        queue_ = std::make_shared<EventQueueBase>();
    }

    ~EventRunnerImpl() final
    {
        HILOGD("enter");
        queue_->RemoveAll();
    }
    DISALLOW_COPY_AND_MOVE(EventRunnerImpl);

    static void ThreadMain(const std::weak_ptr<EventRunnerImpl> &wp)
    {
        HILOGD("enter");
        if (SetThreadInfoCallback != nullptr) {
            SetThreadInfoCallback(CrashCallback);
        }
        std::shared_ptr<EventRunnerImpl> inner = wp.lock();
        if (inner) {
            HILOGD("Start running for thread '%{public}s'", inner->threadName_.c_str());

            // Call system call to modify thread name.
            SystemCallSetThreadName(inner->threadName_);

            // Enter event loop.
            inner->Run();

            HILOGD("Stopped running for thread '%{public}s'", inner->threadName_.c_str());
        } else {
            EH_LOGW_LIMIT("EventRunner has been released just after its creation");
        }

        // Reclaim current thread.
        ThreadCollector::GetInstance().ReclaimCurrentThread();
    }

    void Run() final
    {
        HILOGD("enter");
        // Prepare to start event loop.
        queue_->Prepare();

        // Make sure instance of 'EventRunner' exists.
        if (owner_.expired()) {
            return;
        }

        threadId_ = std::this_thread::get_id();
        kernelThreadId_ = getproctid();

        // Save old event runner.
        std::weak_ptr<EventRunner> oldRunner = currentEventRunner;
        // Set current event runner into thread local data.
        currentEventRunner = owner_;

        // Start event looper.
        if (runningMode_ == Mode::NO_WAIT) {
            NoWaitModeLoop();
        } else {
            DefaultModeLoop();
        }

        // Restore current event runner.
        currentEventRunner = oldRunner;
    }

    void Stop() final
    {
        HILOGD("enter");
        queue_->Finish();
    }

    void NoWaitModeLoop()
    {
        // handler event queue
        InnerEvent::TimePoint nextWakeTime = InnerEvent::TimePoint::max();
        for (auto event = queue_->GetExpiredEvent(nextWakeTime); event; event =
            queue_->GetExpiredEvent(nextWakeTime)) {
            ExecuteEventHandler(event);
        }
        // wait for file descriptor
        queue_->CheckFileDescriptorEvent();
        //  handler file descriptor event
        for (auto event = queue_->GetExpiredEvent(nextWakeTime); event; event =
            queue_->GetExpiredEvent(nextWakeTime)) {
            ExecuteEventHandler(event);
        }
    }

    inline void DefaultModeLoop()
    {
        // Default running loop
        for (auto event = queue_->GetEvent(); event; event = queue_->GetEvent()) {
            ExecuteEventHandler(event);
        }
    }

    void RecordDispatchEventId(InnerEvent::Pointer &event)
    {
        std::shared_ptr<Logger> logging = logger_;
        if (logging != nullptr) {
            if (!event->HasTask()) {
                InnerEvent::EventId eventId = event->GetInnerEventIdEx();
                if (eventId.index() == TYPE_U32_INDEX) {
                    logging->Log(
                        "Dispatching to handler event id = " + std::to_string(std::get<uint32_t>(eventId)));
                } else {
                    logging->Log("Dispatching to handler event id = " + std::get<std::string>(eventId));
                }
            } else {
                logging->Log("Dispatching to handler event task name = " + event->GetTaskName());
            }
        }
    }

    void ExecuteEventHandler(InnerEvent::Pointer &event)
    {
        std::shared_ptr<EventHandler> handler = event->GetOwner();
        // Make sure owner of the event exists.
        if (handler) {
            RecordDispatchEventId(event);
            SetCurrentEventInfo(event);
            queue_->PushHistoryQueueBeforeDistribute(event);
            handler->DistributeEvent(event);
            queue_->PushHistoryQueueAfterDistribute();
            ClearCurrentEventInfo();
#ifdef NOTIFICATIONG_SMART_GC
            queue_->NotifyObserverVipDone(event);
#endif
        } else {
            HILOGW("Invalid event handler.");
        }
        // Release event manually, otherwise event will be released until next event coming.
        event.reset();
    }

    inline bool Attach(std::unique_ptr<std::thread> &thread)
    {
        auto exitThread = [queue = queue_]() { queue->Finish(); };

        return ThreadCollector::GetInstance().Deposit(thread, exitThread);
    }

    inline void SetThreadName(const std::string &threadName)
    {
        static std::atomic<uint32_t> idGenerator(1);

        if (threadName.empty()) {
            // Generate a default name
            threadName_ = "EventRunner#";
            threadName_ += std::to_string(idGenerator++);
        } else {
            threadName_ = threadName;
        }
    }

    inline void SetRunningMode(const Mode runningMode)
    {
        runningMode_ = runningMode;
    }

private:
    DEFINE_EH_HILOG_LABEL("EventRunnerImpl");

    static void CrashCallback(char *buf, size_t len, void *ucontext);

    void SetCurrentEventInfo(const InnerEvent::Pointer &event)
    {
        g_currentEventCaller = event->GetCaller();
        if (event->HasTask()) {
            g_currentEventName = event->GetTaskName();
        } else {
            InnerEvent::EventId eventId = event->GetInnerEventIdEx();
            if (eventId.index() == TYPE_U32_INDEX) {
                g_currentEventName = std::to_string(std::get<uint32_t>(eventId));
            } else {
                g_currentEventName = std::get<std::string>(eventId);
            }
        }
    }

    inline void ClearCurrentEventInfo()
    {
        g_currentEventCaller = {};
        g_currentEventName.clear();
    }
};
}  // unnamed namespace

void EventRunnerImpl::CrashCallback(char *buf, size_t len, void *ucontext)
{
    if (len < CRASH_BUF_MIN_LEN) {
        return;
    }
    if (memset_s(buf, len, 0x00, len) != EOK) {
        HILOGE("memset_s failed");
        return;
    }

    if (!g_currentEventName.empty()) {
        const char* file = g_currentEventCaller.file_.c_str();
        const char* func = g_currentEventCaller.func_.c_str();
        const char* eventName = g_currentEventName.c_str();
        int line = g_currentEventCaller.line_;
        if (snprintf_s(buf, len, len - 1, "Current Event Caller info: [%s(%s:%d)]. EventName is '%s'",
            file, func, line, eventName) < 0) {
            HILOGE("snprintf_s failed");
            return;
        }
        return;
    }

    if (memcpy_s(buf, len - 1, g_crashEmptyDumpInfo, len - 1) != EOK) {
        HILOGE("memcpy_s failed");
        return;
    }
}

EventInnerRunner::EventInnerRunner(const std::shared_ptr<EventRunner> &runner)
    : queue_(nullptr), owner_(runner), logger_(nullptr), threadName_(""), threadId_()
{
    HILOGD("enter");
}

std::shared_ptr<EventRunner> EventInnerRunner::GetCurrentEventRunner()
{
    const std::weak_ptr<EventRunner> &wp = currentEventRunner;
    return wp.lock();
}

ThreadLocalData<std::weak_ptr<EventRunner>> EventInnerRunner::currentEventRunner;

namespace {
volatile bool ThreadCollector::avatarEnabled_ = false;

/*
 * All event runners are relied on 'currentEventRunner', so make sure destruction of 'currentEventRunner'
 * should after all event runners finished. All event runners finished in destruction of 'ThreadCollector::Avatar',
 * so instance of 'ThreadCollector::Avatar' MUST defined after 'currentEventRunner'.
 */
ThreadCollector::Avatar ThreadCollector::avatar_;
}  // unnamed namespace

std::shared_ptr<EventRunner> EventRunner::mainRunner_;
EventRunner::DistributeBeginTime EventRunner::distributeBegin_ = nullptr;
EventRunner::DistributeEndTime EventRunner::distributeEnd_ = nullptr;
EventRunner::CallbackTime EventRunner::distributeCallback_ = nullptr;

std::shared_ptr<EventRunner> EventRunner::Create(bool inNewThread)
{
    HILOGD("inNewThread is %{public}d", inNewThread);
    if (inNewThread) {
        EH_LOGI_LIMIT("EventRunner created");
        return Create(std::string(), Mode::DEFAULT, ThreadMode::NEW_THREAD);
    }

    // Constructor of 'EventRunner' is private, could not use 'std::make_shared' to construct it.
    std::shared_ptr<EventRunner> sp(new (std::nothrow) EventRunner(false, Mode::DEFAULT));
    if (sp == nullptr) {
        HILOGI("Failed to create EventRunner Instance");
        return nullptr;
    }
    auto innerRunner = std::make_shared<EventRunnerImpl>(sp);
    innerRunner->SetRunningMode(Mode::DEFAULT);
    sp->innerRunner_ = innerRunner;
    sp->queue_ = innerRunner->GetEventQueue();
    sp->threadMode_ = ThreadMode::NEW_THREAD;
    sp->queue_->SetIoWaiter(false);
    return sp;
}

std::shared_ptr<EventRunner> EventRunner::Create(bool inNewThread, ThreadMode threadMode)
{
    HILOGD("inNewThread is %{public}d %{public}d", inNewThread, threadMode);
    if (inNewThread) {
        EH_LOGI_LIMIT("EventRunner created");
        return Create(std::string(), Mode::DEFAULT, threadMode);
    }

    // Constructor of 'EventRunner' is private, could not use 'std::make_shared' to construct it.
    std::shared_ptr<EventRunner> sp(new (std::nothrow) EventRunner(false, Mode::DEFAULT));
    if (sp == nullptr) {
        HILOGW("Failed to create EventRunner Instance");
        return nullptr;
    }
    auto innerRunner = std::make_shared<EventRunnerImpl>(sp);
    innerRunner->SetRunningMode(Mode::DEFAULT);
    sp->innerRunner_ = innerRunner;
    sp->queue_ = innerRunner->GetEventQueue();
    sp->threadMode_ = ThreadMode::NEW_THREAD;
    sp->queue_->SetIoWaiter(false);
    return sp;
}

std::shared_ptr<EventRunner> EventRunner::Create(const std::string &threadName, Mode mode, ThreadMode threadMode)
{
    HILOGD("threadName is %{public}s %{public}d %{public}d", threadName.c_str(), mode, threadMode);
    // Constructor of 'EventRunner' is private, could not use 'std::make_shared' to construct it.
    std::shared_ptr<EventRunner> sp(new EventRunner(true, mode));
    auto innerRunner = std::make_shared<EventRunnerImpl>(sp);
    innerRunner->SetRunningMode(mode);
    sp->innerRunner_ = innerRunner;
    innerRunner->SetThreadName(threadName);

#ifdef FFRT_USAGE_ENABLE
    if (threadMode == ThreadMode::FFRT && mode == Mode::DEFAULT) {
        sp->threadMode_ = ThreadMode::FFRT;
        sp->queue_ = std::make_shared<EventQueueFFRT>();
        sp->queue_->SetIoWaiter(true);
    } else {
        sp->threadMode_ = ThreadMode::NEW_THREAD;
        sp->queue_ = innerRunner->GetEventQueue();
        sp->queue_->SetIoWaiter(false);
    }
    if (threadMode == ThreadMode::FFRT || mode == Mode::NO_WAIT) {
        return sp;
    }
#else
    sp->threadMode_ = ThreadMode::NEW_THREAD;
    sp->queue_ = innerRunner->GetEventQueue();
    sp->queue_->SetIoWaiter(false);
    if (mode == Mode::NO_WAIT) {
        return sp;
    }
#endif

    // Start new thread
    auto thread =
        std::make_unique<std::thread>(EventRunnerImpl::ThreadMain, std::weak_ptr<EventRunnerImpl>(innerRunner));
    if (!innerRunner->Attach(thread)) {
        HILOGW("Failed to attach thread, maybe process is exiting");
        innerRunner->Stop();
        thread->join();
    }

    return sp;
}

std::shared_ptr<EventRunner> EventRunner::Current()
{
#ifdef FFRT_USAGE_ENABLE
    if (ffrt_this_task_get_id()) {
        auto handler = ffrt_get_current_queue_eventhandler();
        if (handler == nullptr) {
            HILOGW("Current handler is null.");
            return nullptr;
        }
        auto sharedHandler =  *(reinterpret_cast<std::shared_ptr<EventHandler>*>(handler));
        return sharedHandler->GetEventRunner();
    } else {
        auto runner = EventInnerRunner::GetCurrentEventRunner();
        if (runner) {
            return runner;
        }
        return nullptr;
    }
#else
    auto runner = EventInnerRunner::GetCurrentEventRunner();
    if (runner) {
        return runner;
    }
    return nullptr;
#endif
}

EventRunner::EventRunner(bool deposit, Mode runningMode) : deposit_(deposit), runningMode_(runningMode)
{
    runnerId_ = std::to_string(GetTimeStamp());
    HILOGD("deposit_ is %{public}d %{public}d", deposit_, runningMode_);
}

std::string EventRunner::GetRunnerThreadName() const
{
    return innerRunner_->GetThreadName();
}

EventRunner::~EventRunner()
{
    HILOGI("~EventRunner deposit_ is %{public}d %{public}s", deposit_, runnerId_.c_str());
    if (deposit_ && innerRunner_ != nullptr) {
        innerRunner_->Stop();
    }
}

void EventRunner::StartRunningForNoWait()
{
    auto innerRunner = std::static_pointer_cast<EventRunnerImpl>(innerRunner_);
    auto thread =
        std::make_unique<std::thread>(EventRunnerImpl::ThreadMain, std::weak_ptr<EventRunnerImpl>(innerRunner));
    if (!innerRunner->Attach(thread)) {
        HILOGW("Failed to attach thread for no wait, maybe process is exiting");
        innerRunner->Stop();
        thread->join();
    }
}

ErrCode EventRunner::Run()
{
    HILOGD("enter");
    if (deposit_ && runningMode_ == Mode::DEFAULT) {
        HILOGE("Do not call, if event runner is deposited");
        return EVENT_HANDLER_ERR_RUNNER_NO_PERMIT;
    }

    // Avoid more than one thread to start this runner.
    if (running_.exchange(true)) {
        HILOGW("Already running");
        return EVENT_HANDLER_ERR_RUNNER_ALREADY;
    }

    runnerId_ += "_" + std::to_string(getproctid());
    if (deposit_ && runningMode_ == Mode::NO_WAIT) {
        // Start new thread for NO_WAIT Mode
        StartRunningForNoWait();
    }  else {
        // Entry event loop.
        innerRunner_->Run();
    }

    running_.store(false);
    return ERR_OK;
}

ErrCode EventRunner::Stop()
{
    HILOGD("enter");
    if (deposit_) {
        EH_LOGE_LIMIT("Stop: Do not call, if event runner is deposited");
        return EVENT_HANDLER_ERR_RUNNER_NO_PERMIT;
    }

    if (running_.load()) {
        innerRunner_->Stop();
    } else {
        HILOGW("Stop: Already stopped");
    }

    return ERR_OK;
}

void EventRunner::Dump(Dumper &dumper)
{
    if (!IsRunning()) {
        dumper.Dump(dumper.GetTag() + " Event runner is not running" + LINE_SEPARATOR);
        return;
    }

    if (queue_ == nullptr) {
        dumper.Dump(dumper.GetTag() + " Queue is nullLINE_SEPARATOR" + LINE_SEPARATOR);
        return;
    }

    dumper.Dump(dumper.GetTag() + " Event runner (" + "Thread name = " + innerRunner_->GetThreadName() +
                ", Thread ID = " + std::to_string(GetKernelThreadId()) + ") is running" + LINE_SEPARATOR);
    queue_->Dump(dumper);
}

void EventRunner::DumpRunnerInfo(std::string& runnerInfo)
{
    if (!IsRunning()) {
        runnerInfo = "        Event runner is not running" + LINE_SEPARATOR;
    }

    if (queue_ == nullptr) {
        runnerInfo = "        Queue is null" + LINE_SEPARATOR;
        return;
    }

    std::string queueInfo;
    queue_->DumpQueueInfo(queueInfo);
    runnerInfo += queueInfo;
}

void EventRunner::SetLogger(const std::shared_ptr<Logger> &logger)
{
    innerRunner_->SetLogger(logger);
}

std::shared_ptr<EventQueue> EventRunner::GetCurrentEventQueue()
{
#ifdef FFRT_USAGE_ENABLE
    if (ffrt_this_task_get_id()) {
        auto handler = ffrt_get_current_queue_eventhandler();
        if (handler == nullptr) {
            HILOGW("Current handler is null.");
            return nullptr;
        }
        auto sharedHandler =  *(reinterpret_cast<std::shared_ptr<EventHandler>*>(handler));
        return sharedHandler->GetEventRunner()->GetEventQueue();
    } else {
        auto runner = EventRunner::Current();
        if (!runner) {
            HILOGE("current runner is nullptr");
            return nullptr;
        }
        return runner->queue_;
    }
#else
    auto runner = EventRunner::Current();
    if (!runner) {
        HILOGE("current runner is nullptr");
        return nullptr;
    }
    return runner->queue_;
#endif
}

uint64_t EventRunner::GetThreadId()
{
    std::thread::id tid = innerRunner_->GetThreadId();
    std::stringstream buf;
    buf << tid;
    std::string stid = buf.str();
    return std::stoull(stid);
}

uint64_t EventRunner::GetKernelThreadId()
{
    return innerRunner_->GetKernelThreadId();
}

bool EventRunner::IsCurrentRunnerThread()
{
#ifdef FFRT_USAGE_ENABLE
    if (ffrt_this_task_get_id()) {
        auto handler = ffrt_get_current_queue_eventhandler();
        if (handler == nullptr) {
            HILOGW("Current handler is null.");
            return false;
        }
        auto sharedHandler =  *(reinterpret_cast<std::shared_ptr<EventHandler>*>(handler));
        return queue_ == sharedHandler->GetEventRunner()->GetEventQueue();
    } else {
        return std::this_thread::get_id() == innerRunner_->GetThreadId();
    }
#else
    return std::this_thread::get_id() == innerRunner_->GetThreadId();
#endif
}

std::shared_ptr<EventRunner> EventRunner::GetMainEventRunner()
{
    if (!mainRunner_) {
        mainRunner_ = Create(false);
        if (!mainRunner_) {
            HILOGE("mainRunner_ create fail");
        }
    }

    return mainRunner_;
}

bool EventRunner::IsAppMainThread()
{
    static int pid = -1;
    static int uid = -1;

    if (pid == -1) {
        pid = getpid();
    }
    if (uid == -1) {
        uid = getuid();
    }
    if (pid == gettid() && uid >= MIN_APP_UID) {
        return true;
    }
    return false;
}

void EventRunner::SetMainLooperWatcher(const DistributeBeginTime begin,
    const DistributeEndTime end)
{
    if ((begin != nullptr && end != nullptr) || (begin == nullptr && end == nullptr)) {
        distributeBegin_ = begin;
        distributeEnd_ = end;
    } else {
        HILOGE("SetMainLooperWatcher Error, invaild parameter");
    }
}
}  // namespace AppExecFwk
}  // namespace OHOS
