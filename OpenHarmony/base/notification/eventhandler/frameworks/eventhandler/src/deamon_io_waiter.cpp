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

#include "deamon_io_waiter.h"

#include <chrono>

#include <mutex>
#include <sys/eventfd.h>
#include <unistd.h>

#include "event_handler_utils.h"
#include "event_logger.h"
#include "event_handler.h"
#include "frame_report_sched.h"
#ifdef RES_SCHED_ENABLE
#include "res_type.h"
#include "res_sched_client.h"
#endif

namespace OHOS {
namespace AppExecFwk {
namespace {
const size_t MAX_EPOLL_EVENTS_SIZE = 8;
DEFINE_EH_HILOG_LABEL("DeamonIoWaiter");

inline int32_t EpollCtrl(int32_t epollFd, int32_t operation, int32_t fileDescriptor, uint32_t epollEvents)
{
    struct epoll_event epollEvent = {
        .events = epollEvents,
        .data = {.fd = fileDescriptor},
    };

    return epoll_ctl(epollFd, operation, fileDescriptor, &epollEvent);
}
}  // unnamed namespace

DeamonIoWaiter::~DeamonIoWaiter()
{
    HILOGD("enter");
    if (!isFinished_) {
        StopEpollIoWaiter();
    }
    // Close all valid file descriptors.
    if (epollFd_ >= 0) {
        close(epollFd_);
        epollFd_ = -1;
    }

    if (awakenFd_ >= 0) {
        close(awakenFd_);
        awakenFd_ = -1;
    }
}

DeamonIoWaiter& DeamonIoWaiter::GetInstance()
{
    static DeamonIoWaiter DeamonIoWaiter;
    return DeamonIoWaiter;
}

bool DeamonIoWaiter::Init()
{
    HILOGD("enter");
    if (running_.load() == true) {
        return true;
    }
    if (epollFd_ >= 0) {
        HILOGE("Already initialized");
        return true;
    }

    int32_t epollFd = -1;
    int32_t awakenFd = -1;

    do {
        epollFd = epoll_create(MAX_EPOLL_EVENTS_SIZE);
        if (epollFd < 0) {
            char errmsg[MAX_ERRORMSG_LEN] = {0};
            GetLastErr(errmsg, MAX_ERRORMSG_LEN);
            HILOGE("Failed to create epoll, %{public}s", errmsg);
            break;
        }

        awakenFd = eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
        if (awakenFd < 0) {
            char errmsg[MAX_ERRORMSG_LEN] = {0};
            GetLastErr(errmsg, MAX_ERRORMSG_LEN);
            HILOGE("Failed to create event fd, %{public}s", errmsg);
            break;
        }

        // Add readable file descriptor of pipe, used to wake up blocked thread.
        if (EpollCtrl(epollFd, EPOLL_CTL_ADD, awakenFd, EPOLLIN | EPOLLET) < 0) {
            char errmsg[MAX_ERRORMSG_LEN] = {0};
            GetLastErr(errmsg, MAX_ERRORMSG_LEN);
            HILOGE("Failed to add awaken file descriptor into epoll, %{public}s", errmsg);
            break;
        }

        // Prepare epoll successfully.
        epollFd_ = epollFd;
        awakenFd_ = awakenFd;

        return true;
    } while (0);

    // If any error happened, close all valid file descriptors.
    if (epollFd >= 0) {
        close(epollFd);
    }

    if (awakenFd >= 0) {
        close(awakenFd);
    }

    return false;
}

void DeamonIoWaiter::StartEpollIoWaiter()
{
    if (running_.exchange(true)) {
        return;
    }
    auto task = std::bind(&DeamonIoWaiter::EpollWaitFor, this);
    epollThread_ = std::make_unique<std::thread>(task);
}

void DeamonIoWaiter::StopEpollIoWaiter()
{
    isFinished_ = true;
    NotifyAll();
    if (epollThread_ != nullptr && epollThread_->joinable()) {
        epollThread_->join();
    }
    running_.store(false);
}

static void PostTaskForVsync(const std::shared_ptr<FileDescriptorInfo> &fileDescriptorInfo,
    const std::shared_ptr<EventHandler> &handler, const InnerEvent::Callback &cb,
    uint32_t delayTime)
{
    auto runner = handler->GetEventRunner();
    if (!runner) {
        return;
    }
    auto event = InnerEvent::Get(cb, fileDescriptorInfo->taskName_);
    if (!event) {
        return;
    }

    event->MarkVsyncTask();
    auto task = []() { EventRunner::Current()->GetEventQueue()->SetBarrierMode(true); };
    handler->PostTask(task, "BarrierEvent", delayTime, fileDescriptorInfo->priority_);
    FrameReport::GetInstance().ReportSchedEvent(FrameSchedEvent::UI_EVENT_HANDLE_BEGIN, {});
    if (!handler->SendEvent(event, delayTime, fileDescriptorInfo->priority_)) {
        handler->RemoveTask("BarrierEvent");
        runner->GetEventQueue()->SetBarrierMode(false);
    }
}

void DeamonIoWaiter::HandleFileDescriptorEvent(int32_t fileDescriptor, uint32_t events)
    __attribute__((no_sanitize("cfi")))
{
    auto fileDescriptorInfo = GetFileDescriptorMap(fileDescriptor);
    if (fileDescriptorInfo != nullptr && fileDescriptorInfo->listener_ != nullptr) {
        auto handler = fileDescriptorInfo->listener_->GetOwner();
        if (!handler) {
            HILOGW("Owner of listener is released %{public}d.", fileDescriptor);
            return;
        }

        auto listener = fileDescriptorInfo->listener_;
        bool isVsyncTask = handler->GetEventRunner() && listener->IsVsyncListener();
        std::weak_ptr<FileDescriptorListener> wp = fileDescriptorInfo->listener_;
        auto f = [fileDescriptor, events, wp, isVsyncTask]() {
            if (isVsyncTask) {
                auto queue = EventRunner::Current()->GetEventQueue();
                queue->HandleVsyncTaskNotify();
                queue->SetBarrierMode(false);
            }
            auto listener = wp.lock();
            if (!listener) {
                HILOGW("Listener is released");
                return;
            }

            if ((events & FILE_DESCRIPTOR_INPUT_EVENT) != 0) {
                listener->OnReadable(fileDescriptor);
            }

            if ((events & FILE_DESCRIPTOR_OUTPUT_EVENT) != 0) {
                listener->OnWritable(fileDescriptor);
            }

            if ((events & FILE_DESCRIPTOR_SHUTDOWN_EVENT) != 0) {
                listener->OnShutdown(fileDescriptor);
            }

            if ((events & FILE_DESCRIPTOR_EXCEPTION_EVENT) != 0) {
                listener->OnException(fileDescriptor);
            }
        };

        HILOGD("Post fd %{public}d, task %{public}s, priority %{public}d delay %{public}dms.", fileDescriptor,
            fileDescriptorInfo->taskName_.c_str(), fileDescriptorInfo->priority_, listener->GetDelayTime());
        // Post a high priority task to handle file descriptor events.
        if (isVsyncTask) {
            PostTaskForVsync(fileDescriptorInfo, handler, f, listener->GetDelayTime());
        } else {
            handler->PostTask(f, fileDescriptorInfo->taskName_, listener->GetDelayTime(),
                fileDescriptorInfo->priority_);
        }
    }
}

void DeamonIoWaiter::HandleEpollEvents(struct epoll_event *epollEvents, int32_t eventsCount)
{
    for (int32_t i = 0; i < eventsCount; ++i) {
        if (epollEvents[i].data.fd == awakenFd_) {
            // Drain awaken pipe, if woken up by it.
            DrainAwakenPipe();
            continue;
        }

        // Transform epoll events into file descriptor listener events.
        uint32_t events = 0;
        if ((epollEvents[i].events & EPOLLIN) != 0) {
            events |= FILE_DESCRIPTOR_INPUT_EVENT;
        }

        if ((epollEvents[i].events & EPOLLOUT) != 0) {
            events |= FILE_DESCRIPTOR_OUTPUT_EVENT;
        }

        if ((epollEvents[i].events & (EPOLLHUP)) != 0) {
            events |= FILE_DESCRIPTOR_SHUTDOWN_EVENT;
        }

        if ((epollEvents[i].events & (EPOLLERR)) != 0) {
            events |= FILE_DESCRIPTOR_EXCEPTION_EVENT;
        }
        HandleFileDescriptorEvent(epollEvents[i].data.fd, events);
    }
}

void DeamonIoWaiter::EpollWaitFor()
{
    if (epollFd_ < 0) {
        HILOGE("MUST initialized before waiting");
        return;
    }
    HILOGD("Epoll io waiter start polling.");
    pthread_setname_np(pthread_self(), "OS_EVENT_POLL");
#ifdef RES_SCHED_ENABLE
    std::unordered_map<std::string, std::string> payload {
        {"pid", std::to_string(getprocpid())}
    };
    uint32_t type = ResourceSchedule::ResType::RES_TYPE_REPORT_DISTRIBUTE_TID;
    int64_t value = getproctid();
    ResourceSchedule::ResSchedClient::GetInstance().ReportData(type, value, payload);
    HILOGD("Epoll io waiter set thread sched. pid: %{public}d, tid: %{public}d", getprocpid(), getproctid());
#endif
    while (!isFinished_) {
        // Increasment of waiting count MUST be done before unlock.
        ++waitingCount_;

        // Block on epoll_wait outside of the lock.
        struct epoll_event epollEvents[MAX_EPOLL_EVENTS_SIZE];
        int32_t retVal = epoll_wait(epollFd_, epollEvents, MAX_EPOLL_EVENTS_SIZE, -1);
        // Decrease waiting count after block at once.
        --waitingCount_;
        if (waitingCount_ < 0) {
            HILOGE("WaitingCount_ become negative: %{public}d", waitingCount_.load());
        }

        if (retVal < 0) {
            if (errno != EINTR && errno != EINVAL) {
                char errmsg[MAX_ERRORMSG_LEN] = {0};
                GetLastErr(errmsg, MAX_ERRORMSG_LEN);
                HILOGE("Failed to wait epoll, %{public}s", errmsg);
            }
        } else {
            HandleEpollEvents(epollEvents, retVal);
        }
    }
}

void DeamonIoWaiter::NotifyOne()
{
    // Epoll only support wake up all waiting thread.
    NotifyAll();
}

void DeamonIoWaiter::NotifyAll()
{
    if (awakenFd_ < 0) {
        HILOGE("MUST initialized before notifying");
        return;
    }

    // Not waiting, so nothing to do.
    if (waitingCount_.load() == 0) {
        return;
    }

    static const uint64_t increment = 1;
    ssize_t retVal = write(awakenFd_, &increment, sizeof(increment));
    if (retVal < 0) {
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to write data into awaken pipe, %{public}s", errmsg);
    }
}

bool DeamonIoWaiter::SupportListeningFileDescriptor() const
{
    return true;
}

bool DeamonIoWaiter::AddFileDescriptor(int32_t fileDescriptor, uint32_t events, const std::string &taskName,
    const std::shared_ptr<FileDescriptorListener>& listener, EventQueue::Priority priority)
{
    if ((fileDescriptor < 0) || ((events & FILE_DESCRIPTOR_EVENTS_MASK) == 0)) {
        HILOGE("%{public}d, %{public}u: Invalid parameter", fileDescriptor, events);
        return false;
    }

    if (epollFd_ < 0) {
        HILOGE("MUST initialized before adding fds");
        return false;
    }

    // Transform file descriptor listener events into epoll events.
    uint32_t epollEvents = 0;
    if ((events & FILE_DESCRIPTOR_INPUT_EVENT) != 0) {
        epollEvents |= EPOLLIN;
    }

    if ((events & FILE_DESCRIPTOR_OUTPUT_EVENT) != 0) {
        epollEvents |= EPOLLOUT;
    }

    InsertFileDescriptorMap(fileDescriptor, taskName, priority, listener);
    if (EpollCtrl(epollFd_, EPOLL_CTL_ADD, fileDescriptor, epollEvents | EPOLLET) < 0) {
        RemoveFileDescriptor(fileDescriptor);
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to add file descriptor into epoll, %{public}s", errmsg);
        return false;
    }
    HILOGD("DeamonIoWaiter add file %{public}d, %{public}s, %{public}d", fileDescriptor, taskName.c_str(), priority);
    return true;
}

void DeamonIoWaiter::RemoveFileDescriptor(int32_t fileDescriptor)
{
    if (fileDescriptor < 0) {
        HILOGE("Invalid param while removing fd: %{public}d", fileDescriptor);
        return;
    }

    if (epollFd_ < 0) {
        HILOGE("MUST initialized before removing fds");
        return;
    }

    if (EpollCtrl(epollFd_, EPOLL_CTL_DEL, fileDescriptor, 0) < 0) {
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to remove file descriptor from epoll, %{public}s", errmsg);
        return;
    }
    EraseFileDescriptorMap(fileDescriptor);
}

void DeamonIoWaiter::DrainAwakenPipe() const
{
    uint64_t value = 0;
    ssize_t retVal = read(awakenFd_, &value, sizeof(value));
    if (retVal < 0) {
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to read data from awaken pipe, %{public}s", errmsg);
    }
}

void DeamonIoWaiter::InsertFileDescriptorMap(int32_t fileDescriptor, const std::string& taskName,
    EventQueue::Priority priority, const std::shared_ptr<FileDescriptorListener>& listener)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    std::shared_ptr<FileDescriptorInfo> fileDescriptorInfo =
        std::make_shared<FileDescriptorInfo>(taskName, priority, listener);
    fileDescriptorMap_.emplace(fileDescriptor, fileDescriptorInfo);
}

void DeamonIoWaiter::EraseFileDescriptorMap(int32_t fileDescriptor)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    fileDescriptorMap_.erase(fileDescriptor);
}

std::shared_ptr<FileDescriptorInfo> DeamonIoWaiter::GetFileDescriptorMap(int32_t fileDescriptor)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    auto it = fileDescriptorMap_.find(fileDescriptor);
    if (it == fileDescriptorMap_.end()) {
        HILOGW("DeamonIoWaiter get file descriptor failed %{public}d", fileDescriptor);
        return nullptr;
    }
    return it->second;
}
}  // namespace AppExecFwk
}  // namespace OHOS
