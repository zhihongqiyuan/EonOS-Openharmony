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

#ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_IO_WAITER_H
#define BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_IO_WAITER_H

#include <cerrno>
#include <cstdint>
#include <functional>
#include <mutex>
#include "event_queue.h"
#include "file_descriptor_listener.h"

#include "nocopyable.h"

#define LOCAL_API __attribute__((visibility ("hidden")))
namespace OHOS {
namespace AppExecFwk {

class FileDescriptorInfo {
public:
    DISALLOW_COPY_AND_MOVE(FileDescriptorInfo);
    FileDescriptorInfo() {}
    FileDescriptorInfo(std::string taskName, EventQueue::Priority priority,
        std::shared_ptr<FileDescriptorListener>listener): taskName_(taskName), priority_(priority),
        listener_(listener) {}
    std::string taskName_;
    EventQueue::Priority priority_;
    std::shared_ptr<FileDescriptorListener> listener_;
};

// Interface of IO waiter
class IoWaiter {
public:
    using FileDescriptorEventCallback = std::function<void(int32_t, uint32_t, const std::string&,
        EventQueue::Priority priority)>;

    IoWaiter() = default;
    virtual ~IoWaiter() = default;
    DISALLOW_COPY_AND_MOVE(IoWaiter);

    /**
     * Wait until IO event coming or timed out.
     *
     * @param lock An unique lock which must be locked by the current thread.
     * @param nanoseconds Nanoseconds for time out, negative value indicate waiting forever.
     * @return True if succeeded.
     */
    LOCAL_API virtual bool WaitFor(std::unique_lock<std::mutex> &lock, int64_t nanoseconds) = 0;

    /**
     * Unblocks one of the waiting threads.
     */
    LOCAL_API virtual void NotifyOne() = 0;

    /**
     * Unblocks all of the waiting threads.
     */
    LOCAL_API virtual void NotifyAll() = 0;

    /**
     * Check whether this waiter support listening file descriptor.
     *
     * @return True if supported.
     */
    LOCAL_API virtual bool SupportListeningFileDescriptor() const = 0;

    /**
     * Add file descriptor.
     *
     * @param fileDescriptor File descriptor which need to listen.
     * @param events Events from file descriptor, such as input, output.
     * @return True if succeeded.
     */
    LOCAL_API virtual bool AddFileDescriptor(int32_t fileDescriptor, uint32_t events, const std::string &taskName,
        const std::shared_ptr<FileDescriptorListener>& listener, EventQueue::Priority priority) = 0;

    /**
     * Remove file descriptor.
     *
     * @param fileDescriptor File descriptor which need to remove.
     */
    LOCAL_API virtual void RemoveFileDescriptor(int32_t fileDescriptor) = 0;

    /**
     * Set callback to handle events from file descriptors, such as readable, writable and so on.
     *
     * @param callback Callback function to handle events from file descriptors.
     */
    LOCAL_API virtual void SetFileDescriptorEventCallback(const FileDescriptorEventCallback &callback) = 0;

    LOCAL_API virtual std::shared_ptr<FileDescriptorInfo> GetFileDescriptorMap(int32_t fileDescriptor)
        { return nullptr; }
};
}  // namespace AppExecFwk
}  // namespace OHOS

#endif  // #ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_IO_WAITER_H
