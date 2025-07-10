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

#ifndef BASE_EVENTHANDLER_INTERFACES_INNER_API_FFRT_DESCRIPTOR_LISTENER_H
#define BASE_EVENTHANDLER_INTERFACES_INNER_API_FFRT_DESCRIPTOR_LISTENER_H

#include "file_descriptor_listener.h"

#include <functional>
#include <sys/epoll.h>
#include <sys/eventfd.h>

#define LOCAL_API __attribute__((visibility ("hidden")))
namespace OHOS {
namespace AppExecFwk {
class FfrtDescriptorListener : public FileDescriptorListener {
public:
    FfrtDescriptorListener() {}
    FfrtDescriptorListener(uint32_t events, void *data, std::function<void(void *, uint32_t)> cbVal)
        : events_(events), data_(data), cb_(cbVal) {}

    ~FfrtDescriptorListener() {}

    LOCAL_API void OnReadable(int32_t fileDescriptor) override;

    LOCAL_API void OnWritable(int32_t fileDescriptor) override;

    LOCAL_API void OnShutdown(int32_t fileDescriptor) override;

    LOCAL_API void OnException(int32_t fileDescriptor) override;

    static uint32_t ConvertEvents(uint32_t events);
private:
    void InvokePollCb(uint32_t events)
    {
        if (cb_ != nullptr) {
            cb_(data_, events);
        }
    }
private:
    uint32_t events_ = 0;
    void* data_ = nullptr;
    std::function<void(void*, uint32_t)> cb_ = nullptr;
};
}
}
#endif  // #ifndef BASE_EVENTHANDLER_INTERFACES_INNER_API_EVENT_QUEUE_FFRT_H
