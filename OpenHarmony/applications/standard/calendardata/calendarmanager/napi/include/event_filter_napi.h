/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef EVENT_FILTER_NAPI_H
#define EVENT_FILTER_NAPI_H

#include "napi/native_api.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"
#include "event_filter.h"

namespace OHOS::CalendarApi {
class EventFilterNapi {
public:
    EventFilterNapi();
    static napi_value Init(napi_env env, napi_value exports);
    static napi_value Constructor(napi_env env);
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_status ToJson(napi_env env, napi_value inner, EventFilterNapi*& out);

    void SetNative(std::shared_ptr<Native::EventFilter>& eventFilter);
    std::shared_ptr<Native::EventFilter>& GetNative();

private:
    static napi_value FilterById(napi_env env, napi_callback_info info);
    static napi_value FilterByTime(napi_env env, napi_callback_info info);
    static napi_value FilterByTitle(napi_env env, napi_callback_info info);

    static napi_value Construct(napi_env env, napi_callback_info info);
    static void Destructor(napi_env env, void *nativeObject, void *finalize_hint);
private:
    std::shared_ptr<Native::EventFilter> eventFilter_ = nullptr;
};

} // OHOS::CalendarApi
#endif // EVENT_FILTER_NAPI_H