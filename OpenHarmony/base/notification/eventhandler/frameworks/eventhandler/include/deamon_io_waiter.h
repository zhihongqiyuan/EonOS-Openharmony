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

#ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_DEAMON_IO_WAITER_H
#define BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_DEAMON_IO_WAITER_H

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
class DeamonIoWaiter {
public:
    DeamonIoWaiter() = default;
    ~DeamonIoWaiter();
    static DeamonIoWaiter& GetInstance();
    DISALLOW_COPY_AND_MOVE(DeamonIoWaiter);

    /**
     * Initialize epoll.
     *
     * @return True if succeeded.
     */
    bool Init();

    void NotifyOne();
    void NotifyAll();
    void StopEpollIoWaiter();
    void StartEpollIoWaiter();
    bool SupportListeningFileDescriptor() const;

    LOCAL_API bool AddFileDescriptor(int32_t fileDescriptor, uint32_t events, const std::string &taskName,
        const std::shared_ptr<FileDescriptorListener>& listener, EventQueue::Priority priority);
    LOCAL_API void RemoveFileDescriptor(int32_t fileDescriptor);

    LOCAL_API void InsertFileDescriptorMap(int32_t fileDescriptor, const std::string& taskName,
        EventQueue::Priority priority, const std::shared_ptr<FileDescriptorListener>& listener);
    LOCAL_API void EraseFileDescriptorMap(int32_t fileDescriptor);
    LOCAL_API std::shared_ptr<FileDescriptorInfo> GetFileDescriptorMap(int32_t fileDescriptor);
    LOCAL_API void HandleFileDescriptorEvent(int32_t fileDescriptor, uint32_t events);
private:
    LOCAL_API void EpollWaitFor();
    LOCAL_API void DrainAwakenPipe() const;
    LOCAL_API void HandleEpollEvents(struct epoll_event *epollEvents, int32_t eventsCount);

    // File descriptor for epoll.
    int32_t epollFd_{-1};
    // File descriptor used to wake up epoll.
    int32_t awakenFd_{-1};
    std::mutex fileDescriptorMapLock;
    std::atomic<int32_t> waitingCount_{0};
    std::atomic<bool> running_ = false;
    std::atomic<bool> isFinished_ = false;
    std::unique_ptr<std::thread> epollThread_;
    std::map<int32_t, std::shared_ptr<FileDescriptorInfo>> fileDescriptorMap_;
};
}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // #ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_DEAMON_IO_WAITER_H
