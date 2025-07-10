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

#ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_EPOLL_IO_WAITER_H
#define BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_EPOLL_IO_WAITER_H

#include <atomic>
#include <map>
#include <mutex>

#include <sys/epoll.h>
#include "io_waiter.h"
#include "nocopyable.h"
#include "event_queue.h"
#include "file_descriptor_listener.h"

#define LOCAL_API __attribute__((visibility ("hidden")))
namespace OHOS {
namespace AppExecFwk {
class EventHandler;

// Use epoll to listen file descriptor.
class EpollIoWaiter final : public IoWaiter {
public:
    EpollIoWaiter() = default;
    ~EpollIoWaiter() final;
    DISALLOW_COPY_AND_MOVE(EpollIoWaiter);

    /**
     * Initialize epoll.
     *
     * @return True if succeeded.
     */
    LOCAL_API bool Init();

    LOCAL_API bool WaitFor(std::unique_lock<std::mutex> &lock, int64_t nanoseconds) final;

    LOCAL_API void NotifyOne() final;
    LOCAL_API void NotifyAll() final;
    bool SupportListeningFileDescriptor() const final;

    LOCAL_API bool AddFileDescriptor(int32_t fileDescriptor, uint32_t events, const std::string &taskName,
        const std::shared_ptr<FileDescriptorListener>& listener, EventQueue::Priority priority) final;
    void RemoveFileDescriptor(int32_t fileDescriptor) final;

    LOCAL_API void SetFileDescriptorEventCallback(const FileDescriptorEventCallback &callback) final;
    LOCAL_API void InsertFileDescriptorMap(int32_t fileDescriptor, const std::string& taskName,
        EventQueue::Priority priority, const std::shared_ptr<FileDescriptorListener>& listener);
    LOCAL_API void EraseFileDescriptorMap(int32_t fileDescriptor);
    LOCAL_API std::shared_ptr<FileDescriptorInfo> GetFileDescriptorMap(int32_t fileDescriptor) override;
private:
    LOCAL_API void DrainAwakenPipe() const;

    // File descriptor for epoll.
    int32_t epollFd_{-1};
    // File descriptor used to wake up epoll.
    int32_t awakenFd_{-1};
    std::mutex fileDescriptorMapLock;
    FileDescriptorEventCallback callback_;
    std::atomic<int32_t> waitingCount_{0};
    std::map<int32_t, std::shared_ptr<FileDescriptorInfo>> fileDescriptorMap_;
};
}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // #ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_EPOLL_IO_WAITER_H
