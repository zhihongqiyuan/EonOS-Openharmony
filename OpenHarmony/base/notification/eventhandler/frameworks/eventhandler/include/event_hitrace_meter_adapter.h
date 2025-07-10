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
#ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_HITRACE_METER_H
#define BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_HITRACE_METER_H

#ifdef EH_HITRACE_METER_ENABLE
#include <dlfcn.h>
#include "inner_event.h"

#define LOCAL_API __attribute__((visibility ("hidden")))
namespace OHOS {
namespace AppExecFwk {
constexpr uint64_t HITRACE_TAG_NOTIFICATION = (1ULL << 40); // Notification module tag.
bool IsTagEnabled(uint64_t tag);
void StartTrace(uint64_t label, const std::string& value, float limit = -1);
void FinishTrace(uint64_t label);
using IsTagEnabledType = decltype(IsTagEnabled)*;
using StartTraceType = decltype(StartTrace)*;
using FinishTraceType = decltype(FinishTrace)*;

#if (defined(__aarch64__) || defined(__x86_64__))
static const std::string TRACE_LIB_PATH = "/system/lib64/chipset-pub-sdk/libhitrace_meter.so";
#else
static const std::string TRACE_LIB_PATH = "/system/lib/chipset-pub-sdk/libhitrace_meter.so";
#endif

class TraceAdapter {
public:
    TraceAdapter()
    {
        Load();
    }

    ~TraceAdapter()
    {
    }

    static TraceAdapter* Instance()
    {
        static TraceAdapter instance;
        return &instance;
    }

    IsTagEnabledType IsTagEnabled = nullptr;
    StartTraceType StartTrace = nullptr;
    FinishTraceType FinishTrace = nullptr;
private:
    LOCAL_API void Load()
    {
        if (handle != nullptr) {
            HILOGD("%{public}s is already dlopened.", TRACE_LIB_PATH.c_str());
            return;
        }

        handle = dlopen(TRACE_LIB_PATH.c_str(), RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            HILOGE("dlopen %{public}s failed.", TRACE_LIB_PATH.c_str());
            return;
        }

        IsTagEnabled = reinterpret_cast<IsTagEnabledType>(dlsym(handle, "IsTagEnabled"));
        if (IsTagEnabled == nullptr) {
            HILOGE("get symbol IsTagEnabled failed.");
            return;
        }

        StartTrace = reinterpret_cast<StartTraceType>(dlsym(handle, "StartTrace"));
        if (StartTrace == nullptr) {
            HILOGE("get symbol StartTrace failed.");
            return;
        }

        FinishTrace = reinterpret_cast<FinishTraceType>(dlsym(handle, "FinishTrace"));
        if (FinishTrace == nullptr) {
            HILOGE("get symbol FinishTrace failed.");
            return;
        }
    }

    DEFINE_EH_HILOG_LABEL("EventHiTraceAdapter");
    void* handle = nullptr;
};

LOCAL_API static inline void StartTraceAdapter(const InnerEvent::Pointer &event)
{
    if (TraceAdapter::Instance()->IsTagEnabled && TraceAdapter::Instance()->StartTrace) {
        if (TraceAdapter::Instance()->IsTagEnabled(HITRACE_TAG_NOTIFICATION)) {
            TraceAdapter::Instance()->StartTrace(HITRACE_TAG_NOTIFICATION, event->TraceInfo(), -1);
        }
    }
}
LOCAL_API static inline void StartTraceObserver(ObserverTrace &observer)
{
    if (TraceAdapter::Instance()->IsTagEnabled && TraceAdapter::Instance()->StartTrace) {
        if (TraceAdapter::Instance()->IsTagEnabled(HITRACE_TAG_NOTIFICATION)) {
            TraceAdapter::Instance()->StartTrace(HITRACE_TAG_NOTIFICATION, observer.getTraceInfo(), -1);
        }
    }
}
LOCAL_API static inline void FinishTraceAdapter()
{
    if (TraceAdapter::Instance()->FinishTrace) {
        TraceAdapter::Instance()->FinishTrace(HITRACE_TAG_NOTIFICATION);
    }
}
}}
#else
namespace OHOS {
namespace AppExecFwk {
static inline void StartTraceAdapter(const InnerEvent::Pointer &event)
{
}
static inline void StartTraceObserver(ObserverTrace &observer)
{
}
static inline void FinishTraceAdapter()
{
}
}}
#endif
#endif // BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_HITRACE_METER_H
