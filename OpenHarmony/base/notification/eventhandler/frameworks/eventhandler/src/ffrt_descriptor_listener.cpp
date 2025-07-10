/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "ffrt_descriptor_listener.h"
#include "event_logger.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
    DEFINE_EH_HILOG_LABEL("FfrtDescriptorListener");
}
void FfrtDescriptorListener::OnReadable(int32_t fileDescriptor)
{
    if (fileDescriptor < 0) {
        HILOGW("OnReadable bad fd events %{public}d.", events_);
        return;
    }
    InvokePollCb(EPOLLIN);
}

void FfrtDescriptorListener::OnWritable(int32_t fileDescriptor)
{
    if (fileDescriptor < 0) {
        HILOGW("OnWritable bad fd");
        return;
    }
    InvokePollCb(EPOLLOUT);
}

void FfrtDescriptorListener::OnShutdown(int32_t fileDescriptor)
{
    if (fileDescriptor < 0 && (events_ & EPOLLHUP)) {
        HILOGW("OnShutdown bad fd");
        return;
    }

    InvokePollCb(EPOLLHUP);
}

void FfrtDescriptorListener::OnException(int32_t fileDescriptor)
{
    if (fileDescriptor < 0 && (events_ & EPOLLERR)) {
        HILOGW("OnException bad fd");
        return;
    }

    InvokePollCb(EPOLLERR);
}

uint32_t FfrtDescriptorListener::ConvertEvents(uint32_t events)
{
    uint32_t epollEvents = 0;
    if ((events & EPOLLIN) != 0) {
        epollEvents |= FILE_DESCRIPTOR_INPUT_EVENT;
    }

    if ((events & EPOLLOUT) != 0) {
        epollEvents |= FILE_DESCRIPTOR_OUTPUT_EVENT;
    }
    return epollEvents;
}
}}
