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

#include "epoll_io_waiter.h"

#include <chrono>

#include <mutex>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <unistd.h>

#include "event_handler_utils.h"
#include "event_logger.h"
#include "file_descriptor_listener.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const size_t MAX_EPOLL_EVENTS_SIZE = 8;
DEFINE_EH_HILOG_LABEL("EpollIoWaiter");

inline int32_t EpollCtrl(int32_t epollFd, int32_t operation, int32_t fileDescriptor, uint32_t epollEvents)
{
    struct epoll_event epollEvent = {
        .events = epollEvents,
        .data = {.fd = fileDescriptor},
    };

    return epoll_ctl(epollFd, operation, fileDescriptor, &epollEvent);
}
}  // unnamed namespace

EpollIoWaiter::~EpollIoWaiter()
{
    HILOGD("enter");
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

bool EpollIoWaiter::Init()
{
    HILOGD("enter");
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

bool EpollIoWaiter::WaitFor(std::unique_lock<std::mutex> &lock, int64_t nanoseconds)
{
    if (epollFd_ < 0) {
        HILOGE("MUST initialized before waiting");
        return false;
    }

    // Increasment of waiting count MUST be done before unlock.
    ++waitingCount_;
    lock.unlock();

    // Block on epoll_wait outside of the lock.
    struct epoll_event epollEvents[MAX_EPOLL_EVENTS_SIZE];
    int32_t retVal = epoll_wait(epollFd_, epollEvents, MAX_EPOLL_EVENTS_SIZE, NanosecondsToTimeout(nanoseconds));
    // Decrease waiting count after block at once.
    --waitingCount_;
    if (waitingCount_ < 0) {
        HILOGE("WaitingCount_ become negative: %{public}d", waitingCount_.load());
    }

    bool result = true;
    if (retVal < 0) {
        if (errno != EINTR && errno != EINVAL) {
            char errmsg[MAX_ERRORMSG_LEN] = {0};
            GetLastErr(errmsg, MAX_ERRORMSG_LEN);
            HILOGE("Failed to wait epoll, %{public}s", errmsg);
            result = false;
        }
    } else {
        for (int32_t i = 0; i < retVal; ++i) {
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
            auto fileDescriptorInfo = GetFileDescriptorMap(epollEvents[i].data.fd);
            if (callback_ && fileDescriptorInfo != nullptr) {
                callback_(epollEvents[i].data.fd, events, fileDescriptorInfo->taskName_,
                    fileDescriptorInfo->priority_);
            }
        }
    }

    lock.lock();
    return result;
}

void EpollIoWaiter::NotifyOne()
{
    // Epoll only support wake up all waiting thread.
    NotifyAll();
}

void EpollIoWaiter::NotifyAll()
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

bool EpollIoWaiter::SupportListeningFileDescriptor() const
{
    return true;
}

bool EpollIoWaiter::AddFileDescriptor(int32_t fileDescriptor, uint32_t events, const std::string &taskName,
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
    if (EpollCtrl(epollFd_, EPOLL_CTL_ADD, fileDescriptor, epollEvents) < 0) {
        RemoveFileDescriptor(fileDescriptor);
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to add file descriptor into epoll, %{public}s", errmsg);
        return false;
    }
    return true;
}

void EpollIoWaiter::RemoveFileDescriptor(int32_t fileDescriptor)
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

void EpollIoWaiter::DrainAwakenPipe() const
{
    uint64_t value = 0;
    ssize_t retVal = read(awakenFd_, &value, sizeof(value));
    if (retVal < 0) {
        char errmsg[MAX_ERRORMSG_LEN] = {0};
        GetLastErr(errmsg, MAX_ERRORMSG_LEN);
        HILOGE("Failed to read data from awaken pipe, %{public}s", errmsg);
    }
}

void EpollIoWaiter::SetFileDescriptorEventCallback(const IoWaiter::FileDescriptorEventCallback &callback)
{
    callback_ = callback;
}

void EpollIoWaiter::InsertFileDescriptorMap(int32_t fileDescriptor, const std::string& taskName,
    EventQueue::Priority priority, const std::shared_ptr<FileDescriptorListener>& listener)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    std::shared_ptr<FileDescriptorInfo> fileDescriptorInfo =
        std::make_shared<FileDescriptorInfo>(taskName, priority, listener);
    fileDescriptorMap_.emplace(fileDescriptor, fileDescriptorInfo);
}

void EpollIoWaiter::EraseFileDescriptorMap(int32_t fileDescriptor)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    fileDescriptorMap_.erase(fileDescriptor);
}

std::shared_ptr<FileDescriptorInfo> EpollIoWaiter::GetFileDescriptorMap(int32_t fileDescriptor)
{
    std::lock_guard<std::mutex> lock(fileDescriptorMapLock);
    auto it = fileDescriptorMap_.find(fileDescriptor);
    if (it == fileDescriptorMap_.end()) {
        HILOGW("EpollIoWaiter get file descriptor failed %{public}d", fileDescriptor);
        return nullptr;
    }
    return it->second;
}
}  // namespace AppExecFwk
}  // namespace OHOS
