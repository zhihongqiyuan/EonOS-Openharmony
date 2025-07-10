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

#include "event_handler.h"

#include <unistd.h>
#include <sys/syscall.h>
#include "event_handler_utils.h"
#include "event_logger.h"
#ifdef HAS_HICHECKER_NATIVE_PART
#include "hichecker.h"
#endif // HAS_HICHECKER_NATIVE_PART
#ifdef FFRT_USAGE_ENABLE
#include "ffrt_inner.h"
#endif // FFRT_USAGE_ENABLE
#include "parameters.h"
#include "thread_local_data.h"
#include "event_hitrace_meter_adapter.h"
#include "ffrt_descriptor_listener.h"

using namespace OHOS::HiviewDFX;
namespace OHOS {
namespace AppExecFwk {
namespace {
static constexpr int FFRT_SUCCESS = 0;
static constexpr int FFRT_ERROR = -1;
static constexpr int FFRT_TASK_REMOVE_FAIL = 1;
static constexpr uint64_t MILLISECONDS_TO_NANOSECONDS_RATIO = 1000000;
static const uint64_t PENDING_JOB_TIMEOUT[3] = {
    system::GetIntParameter("const.sys.notification.pending_higher_event_vip", 4),
    system::GetIntParameter("const.sys.notification.pending_higher_event_immediate", 40),
    system::GetIntParameter("const.sys.notification.pending_higher_event_high", 400)
};
DEFINE_EH_HILOG_LABEL("EventHandler");
}
thread_local std::weak_ptr<EventHandler> EventHandler::currentEventHandler;
thread_local int32_t EventHandler::currentEventPriority = -1;

std::shared_ptr<EventHandler> EventHandler::Current()
{
#ifdef FFRT_USAGE_ENABLE
    if (ffrt_this_task_get_id()) {
        auto handler = ffrt_get_current_queue_eventhandler();
        if (handler == nullptr) {
            HILOGW("Current handler is null.");
            return nullptr;
        }
        return *(reinterpret_cast<std::shared_ptr<EventHandler>*>(handler));
    } else {
        return currentEventHandler.lock();
    }
#else
    return currentEventHandler.lock();
#endif
}

EventHandler::EventHandler(const std::shared_ptr<EventRunner> &runner) : eventRunner_(runner)
{
    static std::atomic<uint64_t> handlerCount = 1;
    handlerId_ = std::to_string(handlerCount.load()) + "_" + std::to_string(GetTimeStamp());
    handlerCount.fetch_add(1);
    EH_LOGI_LIMIT("Create eventHandler %{public}s", handlerId_.c_str());
}

EventHandler::~EventHandler()
{
    HILOGI("~EventHandler enter %{public}s", handlerId_.c_str());
    if (eventRunner_) {
        HILOGD("eventRunner is alive");
        /*
         * This handler is finishing, need to remove all events belong to it.
         * But events only have weak pointer of this handler,
         * now weak pointer is invalid, so these events become orphans.
         */
#ifdef FFRT_USAGE_ENABLE
        if (eventRunner_->threadMode_ == ThreadMode::FFRT) {
            eventRunner_->GetEventQueue()->RemoveOrphanByHandlerId(handlerId_);
        } else {
            eventRunner_->GetEventQueue()->RemoveOrphan();
        }
#else
        eventRunner_->GetEventQueue()->RemoveOrphan();
#endif
    }
}

bool EventHandler::SendEvent(InnerEvent::Pointer &event, int64_t delayTime, Priority priority)
{
    if (!event) {
        HILOGE("Could not send an invalid event");
        return false;
    }

    InnerEvent::TimePoint now = InnerEvent::Clock::now();
    event->SetSendTime(now);
    event->SetSenderKernelThreadId(getproctid());
    event->SetEventUniqueId();
    if (delayTime > 0) {
        event->SetHandleTime(now + std::chrono::milliseconds(delayTime));
    } else {
        event->SetHandleTime(now);
    }
    event->SetOwnerId(handlerId_);
    event->SetDelayTime(delayTime);
    event->SetOwner(shared_from_this());
    // get traceId from event, if HiTraceChain::begin has been called, would get a valid trace id.
    auto traceId = event->GetOrCreateTraceId();
    // if traceId is valid, out put trace information
    if (AllowHiTraceOutPut(traceId, event->HasWaiter())) {
        HiTracePointerOutPut(traceId, event, "Send", HiTraceTracepointType::HITRACE_TP_CS);
    }
    HILOGD("Current event id is %{public}s .", (event->GetEventUniqueId()).c_str());
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before sending events");
        return false;
    }
    return eventRunner_->GetEventQueue()->Insert(event, priority);
}

bool EventHandler::PostTaskAtFront(const Callback &callback, const std::string &name, Priority priority,
    const Caller &caller)
{
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before posting events");
        return false;
    }

    auto event = InnerEvent::Get(callback, name, caller);
    if (!event) {
        HILOGE("Get an invalid event");
        return false;
    }

    event->SetDelayTime(0);
    event->SetOwnerId(handlerId_);
    InnerEvent::TimePoint now = InnerEvent::Clock::now();
    event->SetSendTime(now);
    event->SetHandleTime(now);
    event->SetSenderKernelThreadId(getproctid());
    event->SetEventUniqueId();
    event->SetOwner(shared_from_this());
    auto traceId = event->GetOrCreateTraceId();
    if (AllowHiTraceOutPut(traceId, event->HasWaiter())) {
        HiTracePointerOutPut(traceId, event, "Send", HiTraceTracepointType::HITRACE_TP_CS);
    }
    HILOGD("Current front event id is %{public}s .", (event->GetEventUniqueId()).c_str());
    eventRunner_->GetEventQueue()->Insert(event, priority, EventInsertType::AT_FRONT);
    return true;
}

bool EventHandler::SendTimingEvent(InnerEvent::Pointer &event, int64_t taskTime, Priority priority)
{
    InnerEvent::TimePoint nowSys = InnerEvent::Clock::now();
    auto epoch = nowSys.time_since_epoch();
    long nowSysTime = std::chrono::duration_cast<std::chrono::milliseconds>(epoch).count();
    int64_t delayTime = taskTime - nowSysTime;
    if (delayTime < 0) {
        HILOGW("SendTime is before now systime, change to 0 delaytime Event");
        return SendEvent(event, 0, priority);
    }

    return SendEvent(event, delayTime, priority);
}

bool EventHandler::SendSyncEvent(InnerEvent::Pointer &event, Priority priority)
{
    if ((!event) || (priority == Priority::IDLE)) {
        HILOGE("Could not send an invalid event or idle event");
        return false;
    }

    if ((!eventRunner_) || (!eventRunner_->IsRunning())) {
        HILOGE("MUST Set a running event runner before sending sync events");
        return false;
    }

    bool result = true;
#ifdef FFRT_USAGE_ENABLE
    if ((ffrt_this_task_get_id() && eventRunner_->threadMode_ == ThreadMode::FFRT) ||
        eventRunner_ == EventRunner::Current()) {
        DistributeEvent(event);
        return true;
    }

    // get traceId from event, if HiTraceChain::begin has been called, would get a valid trace id.
    auto spanId = event->GetOrCreateTraceId();

    if (eventRunner_->threadMode_ == ThreadMode::FFRT) {
        event->SetSendTime(InnerEvent::Clock::now());
        event->SetEventUniqueId();
        event->SetHandleTime(InnerEvent::Clock::now());
        event->SetOwnerId(handlerId_);
        event->SetDelayTime(0);
        event->SetOwner(shared_from_this());
        result = eventRunner_->GetEventQueue()->InsertSyncEvent(event, priority);
    } else {
        // Create waiter, used to block.
        auto waiter = event->CreateWaiter();
        // Send this event as normal one.
        if (!SendEvent(event, 0, priority)) {
            HILOGE("SendEvent is failed");
            return false;
        }
        // Wait until event is processed(recycled).
        waiter->Wait();
    }
#else
    // If send a sync event in same event runner, distribute here.
    if (eventRunner_ == EventRunner::Current()) {
        DistributeEvent(event);
        return true;
    }

    // get traceId from event, if HiTraceChain::begin has been called, would get a valid trace id.
    auto spanId = event->GetOrCreateTraceId();

    // Create waiter, used to block.
    auto waiter = event->CreateWaiter();
    // Send this event as normal one.
    if (!SendEvent(event, 0, priority)) {
        HILOGE("SendEvent is failed");
        return false;
    }
    // Wait until event is processed(recycled).
    waiter->Wait();
#endif
    if ((spanId) && (spanId->IsValid())) {
        HiTraceChain::Tracepoint(HiTraceTracepointType::HITRACE_TP_CR, *spanId, "event is processed");
    }

    return result;
}

void EventHandler::RemoveAllEvents()
{
    HILOGD("RemoveAllEvents enter");
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing all events");
        return;
    }

    eventRunner_->GetEventQueue()->Remove(shared_from_this());
}

void EventHandler::RemoveEvent(uint32_t innerEventId)
{
    HILOGD("RemoveEvent enter");
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing events by id");
        return;
    }

    eventRunner_->GetEventQueue()->Remove(shared_from_this(), innerEventId);
}

void EventHandler::RemoveEvent(uint32_t innerEventId, int64_t param)
{
    HILOGD("RemoveEvent -- enter");
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing events by id and param");
        return;
    }

    eventRunner_->GetEventQueue()->Remove(shared_from_this(), innerEventId, param);
}

void EventHandler::RemoveTask(const std::string &name)
{
    HILOGD("RemoveTask enter name %{public}s", name.c_str());
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing events by task name %{public}s", name.c_str());
        return;
    }

    eventRunner_->GetEventQueue()->Remove(shared_from_this(), name);
}

int EventHandler::RemoveTaskWithRet(const std::string &name)
{
    HILOGD("RemoveTaskWithRet enter");
    if (!eventRunner_) {
        HILOGE("MUST Ret Set event runner before removing events by task name");
        return FFRT_TASK_REMOVE_FAIL;
    }

    bool ret = eventRunner_->GetEventQueue()->Remove(shared_from_this(), name);
    return ret ? FFRT_SUCCESS : FFRT_TASK_REMOVE_FAIL;
}

ErrCode EventHandler::AddFileDescriptorListener(int32_t fileDescriptor, uint32_t events,
    const std::shared_ptr<FileDescriptorListener> &listener, const std::string &taskName)
{
    return AddFileDescriptorListener(fileDescriptor, events, listener, taskName, EventQueue::Priority::HIGH);
}

ErrCode EventHandler::AddFileDescriptorListener(int32_t fileDescriptor, uint32_t events,
    const std::shared_ptr<FileDescriptorListener> &listener, const std::string &taskName,
    EventQueue::Priority priority)
{
    HILOGD("enter");
    if ((fileDescriptor < 0) || ((events & FILE_DESCRIPTOR_EVENTS_MASK) == 0) || (!listener)) {
        HILOGE("%{public}d, %{public}u, %{public}s: Invalid parameter",
               fileDescriptor, events, listener ? "valid" : "null");
        return EVENT_HANDLER_ERR_INVALID_PARAM;
    }

    if (!eventRunner_) {
        HILOGE("MUST Set event runner before adding fd listener");
        return EVENT_HANDLER_ERR_NO_EVENT_RUNNER;
    }

    listener->SetOwner(shared_from_this());
    return eventRunner_->GetEventQueue()->AddFileDescriptorListener(fileDescriptor, events, listener, taskName,
        priority);
}

void EventHandler::RemoveAllFileDescriptorListeners()
{
    HILOGD("enter");
    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing all fd listener");
        return;
    }

    eventRunner_->GetEventQueue()->RemoveFileDescriptorListener(shared_from_this());
}

void EventHandler::RemoveFileDescriptorListener(int32_t fileDescriptor)
{
    HILOGD("enter");
    if (fileDescriptor < 0) {
        HILOGE("fd %{public}d: Invalid parameter", fileDescriptor);
        return;
    }

    if (!eventRunner_) {
        HILOGE("MUST Set event runner before removing fd listener by fd");
        return;
    }

    eventRunner_->GetEventQueue()->RemoveFileDescriptorListener(fileDescriptor);
}

void EventHandler::SetEventRunner(const std::shared_ptr<EventRunner> &runner)
{
    HILOGD("enter");
    if (eventRunner_ == runner) {
        return;
    }

    if (eventRunner_) {
        HILOGW("It is not recommended to change the event runner dynamically");

        // Remove all events and listeners from old event runner.
        RemoveAllEvents();
        RemoveAllFileDescriptorListeners();
    }

    // Switch event runner.
    eventRunner_ = runner;
    return;
}

void EventHandler::DeliveryTimeAction(const InnerEvent::Pointer &event, InnerEvent::TimePoint nowStart)
{
#ifdef HAS_HICHECKER_NATIVE_PART
    HILOGD("enter");
    if (!HiChecker::NeedCheckSlowEvent()) {
        return;
    }
    int64_t deliveryTimeout = eventRunner_->GetDeliveryTimeout();
    if (deliveryTimeout > 0) {
        std::string threadName = eventRunner_->GetRunnerThreadName();
        std::string eventName = GetEventName(event);
        int64_t threadId = getproctid();
        std::string threadIdCharacter = std::to_string(threadId);
        std::chrono::duration<double> deliveryTime = nowStart - event->GetSendTime();
        std::string deliveryTimeCharacter = std::to_string((deliveryTime).count());
        std::string deliveryTimeoutCharacter = std::to_string(deliveryTimeout);
        std::string handOutTag = "threadId: " + threadIdCharacter + "," + "threadName: " + threadName + "," +
            "eventName: " + eventName + "," + "deliveryTime: " + deliveryTimeCharacter + "," +
            "deliveryTimeout: " + deliveryTimeoutCharacter;
        if ((nowStart - std::chrono::milliseconds(deliveryTimeout)) > event->GetHandleTime()) {
            HiChecker::NotifySlowEvent(handOutTag);
            if (deliveryTimeoutCallback_) {
                deliveryTimeoutCallback_();
            }
        }
    }
#endif // HAS_HICHECKER_NATIVE_PART
}

void EventHandler::DistributeTimeAction(const InnerEvent::Pointer &event, InnerEvent::TimePoint nowStart)
{
#ifdef HAS_HICHECKER_NATIVE_PART
    HILOGD("enter");
    if (!HiChecker::NeedCheckSlowEvent()) {
        return;
    }
    int64_t distributeTimeout = eventRunner_->GetDistributeTimeout();
    if (distributeTimeout > 0) {
        std::string threadName = eventRunner_->GetRunnerThreadName();
        std::string eventName = GetEventName(event);
        int64_t threadId = getproctid();
        std::string threadIdCharacter = std::to_string(threadId);
        InnerEvent::TimePoint nowEnd = InnerEvent::Clock::now();
        std::chrono::duration<double> distributeTime = nowEnd - nowStart;
        std::string distributeTimeCharacter = std::to_string((distributeTime).count());
        std::string distributeTimeoutCharacter = std::to_string(distributeTimeout);
        std::string executeTag = "threadId: " + threadIdCharacter + "," + "threadName: " + threadName + "," +
            "eventName: " + eventName + "," + "distributeTime: " + distributeTimeCharacter + "," +
            "distributeTimeout: " + distributeTimeoutCharacter;
        if ((nowEnd - std::chrono::milliseconds(distributeTimeout)) > nowStart) {
            HiChecker::NotifySlowEvent(executeTag);
            if (distributeTimeoutCallback_) {
                distributeTimeoutCallback_();
            }
        }
    }
#endif // HAS_HICHECKER_NATIVE_PART
}

void EventHandler::DistributeTimeoutHandler(const InnerEvent::TimePoint& beginTime)
{
    int64_t distributeTimeout = EventRunner::GetMainEventRunner()->GetTimeout();
    if (distributeTimeout > 0) {
        InnerEvent::TimePoint endTime = InnerEvent::Clock::now();
        if ((endTime - std::chrono::milliseconds(distributeTimeout)) > beginTime &&
            EventRunner::distributeCallback_) {
            HILOGD("AppMainThread Callback.");
            auto diff = endTime - beginTime;
            int64_t durationTime = std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
            EventRunner::distributeCallback_(durationTime);
        }
    }
}

void EventHandler::DistributeEvent(const InnerEvent::Pointer &event) __attribute__((no_sanitize("cfi")))
{
    if (!event) {
        HILOGE("Could not distribute an invalid event");
        return;
    }

    currentEventHandler = shared_from_this();
    if (enableEventLog_) {
        auto now = InnerEvent::Clock::now();
        HILOGD("start at %{public}s, event %{public}s, CurrentQueueSize %{public}s",
            (InnerEvent::DumpTimeToString(now)).c_str(), (event->Dump()).c_str(),
            (eventRunner_->GetEventQueue()->DumpCurrentQueueSize()).c_str());
    }

    StartTraceAdapter(event);

    auto spanId = event->GetTraceId();
    auto traceId = HiTraceChain::GetId();
    bool allowTraceOutPut = AllowHiTraceOutPut(spanId, event->HasWaiter());
    if (allowTraceOutPut) {
        HiTraceChain::SetId(*spanId);
        HiTracePointerOutPut(spanId, event, "Receive", HiTraceTracepointType::HITRACE_TP_SR);
    }

    InnerEvent::TimePoint nowStart = InnerEvent::Clock::now();
    DeliveryTimeAction(event, nowStart);
    HILOGD("EventName: %{public}s, eventId: %{public}s, priority: %{public}d", GetEventName(event).c_str(),
        (event->GetEventUniqueId()).c_str(), event->GetEventPriority());

    SetCurrentEventPriority(event->GetEventPriority());
    std::string eventName = GetEventName(event);
    InnerEvent::TimePoint beginTime;
    bool isAppMainThread = EventRunner::IsAppMainThread();
    if (EventRunner::distributeBegin_ && isAppMainThread) {
        beginTime = EventRunner::distributeBegin_(eventName);
    }

    if (event->HasTask()) {
        // Call task callback directly if contains a task.
        HILOGD("excute event taskCallback");
        (event->GetTaskCallback())();
    } else {
        // Otherwise let developers to handle it.
        ProcessEvent(event);
    }

    if (EventRunner::distributeBegin_ && EventRunner::distributeEnd_ && isAppMainThread) {
        EventRunner::distributeEnd_(eventName, beginTime);
        DistributeTimeoutHandler(beginTime);
    }

    DistributeTimeAction(event, nowStart);

    if (allowTraceOutPut) {
        HiTraceChain::Tracepoint(HiTraceTracepointType::HITRACE_TP_SS, *spanId, "Event Distribute over");
        HiTraceChain::ClearId();
        if (traceId.IsValid()) {
            HiTraceChain::SetId(traceId);
        }
    }
    if (enableEventLog_) {
        auto now = InnerEvent::Clock::now();
        HILOGD("end: %{public}s", InnerEvent::DumpTimeToString(now).c_str());
    }
    FinishTraceAdapter();
}

bool EventHandler::HasPendingHigherEvent(int32_t priority)
{
    if (!eventRunner_ || !eventRunner_->GetEventQueue()) {
        return false;
    }
    if (priority < static_cast<int32_t>(AppExecFwk::EventQueue::Priority::VIP) ||
        priority > static_cast<int32_t>(AppExecFwk::EventQueue::Priority::IDLE)) {
        priority = GetCurrentEventPriority();
    }
    if (priority <= static_cast<int32_t>(AppExecFwk::EventQueue::Priority::VIP)) {
        return false;
    }
    InnerEvent::TimePoint now = InnerEvent::Clock::now();
    for (int i = priority - 1; i >= static_cast<int32_t>(AppExecFwk::EventQueue::Priority::VIP); --i) {
        auto eventHandleTime = eventRunner_->GetEventQueue()->GetQueueFirstEventHandleTime(i);
        if (eventHandleTime == UINT64_MAX) {
            continue;
        }
        if (priority == static_cast<int32_t>(AppExecFwk::EventQueue::Priority::IDLE)) {
            return true;
        }
        if (eventHandleTime + PENDING_JOB_TIMEOUT[i] * MILLISECONDS_TO_NANOSECONDS_RATIO <=
            static_cast<uint64_t>(now.time_since_epoch().count())) {
            return true;
        }
    }
    return false;
}

void EventHandler::SetCurrentEventPriority(int32_t priority)
{
    currentEventPriority = priority;
}

const int32_t &EventHandler::GetCurrentEventPriority()
{
    return currentEventPriority;
}

void EventHandler::Dump(Dumper &dumper)
{
    HILOGI("EventHandler start dumper!");
    auto now = std::chrono::system_clock::now();
    dumper.Dump(dumper.GetTag() + " EventHandler dump begin curTime: " +
        InnerEvent::DumpTimeToString(now) + LINE_SEPARATOR);
    if (eventRunner_ == nullptr) {
        dumper.Dump(dumper.GetTag() + " event runner uninitialized!" + LINE_SEPARATOR);
    } else {
        eventRunner_->Dump(dumper);
    }
    HILOGI("EventHandler end dumper!");
}

bool EventHandler::HasInnerEvent(uint32_t innerEventId)
{
    if (!eventRunner_) {
        HILOGE("event runner uninitialized!");
        return false;
    }
    return eventRunner_->GetEventQueue()->HasInnerEvent(shared_from_this(), innerEventId);
}

bool EventHandler::HasInnerEvent(int64_t param)
{
    if (!eventRunner_) {
        HILOGE("event runner uninitialized!");
        return false;
    }
    return eventRunner_->GetEventQueue()->HasInnerEvent(shared_from_this(), param);
}

std::string EventHandler::GetEventName(const InnerEvent::Pointer &event)
{
    std::string eventName;
    if (!event) {
        HILOGW("event is nullptr");
        return eventName;
    }

    if (event->HasTask()) {
        eventName = event->GetTaskName();
    } else {
        InnerEvent::EventId eventId = event->GetInnerEventIdEx();
        if (eventId.index() == TYPE_U32_INDEX) {
            eventName = std::to_string(std::get<uint32_t>(eventId));
        } else {
            eventName = std::get<std::string>(eventId);
        }
    }
    return eventName;
}

bool EventHandler::IsIdle()
{
    return eventRunner_->GetEventQueue()->IsIdle();
}

void EventHandler::ProcessEvent(const InnerEvent::Pointer &)
{}

void EventHandler::EnableEventLog(bool enableEventLog)
{
    enableEventLog_ = enableEventLog;
}

bool EventHandler::HasPreferEvent(int basePrio)
{
    return eventRunner_->GetEventQueue()->HasPreferEvent(basePrio);
}

PendingTaskInfo EventHandler::QueryPendingTaskInfo(int32_t fileDescriptor)
{
    if (!eventRunner_) {
        HILOGE("QueryPendingTaskInfo event runner uninitialized!");
        return PendingTaskInfo();
    }
    return eventRunner_->GetEventQueue()->QueryPendingTaskInfo(fileDescriptor);
}

void EventHandler::TaskCancelAndWait()
{
#ifdef FFRT_USAGE_ENABLE
    if (!eventRunner_) {
        HILOGE("CancelAndWait error,event runner is nullptr");
        return;
    }
    if (eventRunner_->threadMode_ == ThreadMode::FFRT) {
        eventRunner_->GetEventQueue()->CancelAndWait();
    }
#endif
}

extern "C" void* GetMainEventHandlerForFFRT()
{
    HILOGD("GetMainEventHandlerForFFRT enter");
    static std::shared_ptr<OHOS::AppExecFwk::EventHandler> mainHandler =
        std::make_shared<OHOS::AppExecFwk::EventHandler>(OHOS::AppExecFwk::EventRunner::GetMainEventRunner());
    if (mainHandler == nullptr) {
        HILOGW("GetMainEventHandlerForFFRT execute fail, mainHandler is nullptr");
        return nullptr;
    }
    return &mainHandler;
}

extern "C" void* GetCurrentEventHandlerForFFRT()
{
    HILOGD("GetCurrentEventHandlerForFFRT enter");
    thread_local std::shared_ptr<OHOS::AppExecFwk::EventHandler> currentHandler =
        std::make_shared<OHOS::AppExecFwk::EventHandler>(OHOS::AppExecFwk::EventRunner::Current());
    if (currentHandler == nullptr) {
        HILOGW("GetCurrentEventHandlerForFFRT execute fail, currentHandler is nullptr");
        return nullptr;
    }
    return &currentHandler;
}

extern "C" bool PostTaskByFFRT(void* handler, const std::function<void()>& callback, const TaskOptions &task)
{
    HILOGD("PostTaskByFFRT enter");
    if (handler == nullptr) {
        HILOGW("PostTaskByFFRT execute fail, handler is nullptr");
        return false;
    }
    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    Caller caller = {};
    caller.dfxName_ = task.dfxName_;
    return (*ptr)->PostTask(callback, std::to_string(task.taskId_), task.delayTime_, task.priority_, caller);
}

extern "C" bool PostSyncTaskByFFRT(void* handler, const std::function<void()>& callback,
    const std::string &name, EventQueue::Priority priority)
{
    HILOGD("PostSyncTaskByFFRT enter");
    if (handler == nullptr) {
        HILOGW("PostSyncTaskByFFRT execute fail, handler is nullptr");
        return false;
    }
    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    return (*ptr)->PostSyncTask(callback, name, priority);
}

extern "C" int RemoveTaskForFFRT(void* handler, const uintptr_t taskId)
{
    HILOGD("RemoveTaskForFFRT enter");
    if (handler == nullptr) {
        HILOGW("RemoveTaskForFFRT execute fail, handler is nullptr");
        return FFRT_TASK_REMOVE_FAIL;
    }
    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    return (*ptr)->RemoveTaskWithRet(std::to_string(taskId));
}

extern "C" void RemoveAllTaskForFFRT(void* handler)
{
    HILOGD("RemoveAllTaskForFFRT enter");
    if (handler == nullptr) {
        HILOGW("RemoveAllTaskForFFRT execute fail, handler is nullptr");
        return;
    }
    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    (*ptr)->RemoveAllEvents();
}

extern "C" int AddFdListenerByFFRT(void* handler, uint32_t fd, uint32_t event, void* data, ffrt_poller_cb cb)
{
    if (handler == nullptr) {
        HILOGW("AddFdListenerByFFRT execute fail, handler is nullptr");
        return FFRT_ERROR;
    }

    auto listener = std::make_shared<FfrtDescriptorListener>(event, data, cb);
    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    if ((*ptr) == nullptr) {
        HILOGW("AddFdListenerByFFRT execute failed");
        return FFRT_ERROR;
    }
    uint32_t innerEvent = FfrtDescriptorListener::ConvertEvents(event);
    std::string taskName = "FFRT_FD_" + std::to_string(fd);
    ErrCode result = (*ptr)->AddFileDescriptorListener(fd, innerEvent, listener, taskName);
    return (result == ERR_OK) ? FFRT_SUCCESS : FFRT_ERROR;
}

extern "C" int RemoveFdListenerByFFRT(void* handler, uint32_t fd)
{
    if (handler == nullptr) {
        HILOGW("RemoveFdListenerByFFRT execute fail, handler is nullptr");
        return FFRT_ERROR;
    }

    std::shared_ptr<EventHandler>* ptr = reinterpret_cast<std::shared_ptr<EventHandler>*>(handler);
    if ((*ptr) == nullptr) {
        HILOGW("RemoveFdListenerByFFRT execute failed");
        return FFRT_ERROR;
    }
    (*ptr)->RemoveFileDescriptorListener(fd);
    return FFRT_SUCCESS;
}
}  // namespace AppExecFwk
}  // namespace OHOS
