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

#include "event_filter_napi.h"
#include "calendar_log.h"
#include "napi_util.h"
#include "napi_queue.h"

namespace OHOS::CalendarApi {

EventFilterNapi::EventFilterNapi()
{
}

napi_value EventFilterNapi::Constructor(napi_env env)
{
    napi_property_descriptor descriptor[] = {
        DECLARE_NAPI_STATIC_FUNCTION("filterById", EventFilterNapi::FilterById),
        DECLARE_NAPI_STATIC_FUNCTION("filterByTime", EventFilterNapi::FilterByTime),
        DECLARE_NAPI_STATIC_FUNCTION("filterByTitle", EventFilterNapi::FilterByTitle),
    };
    size_t count = sizeof(descriptor) / sizeof(descriptor[0]);
    return NapiUtil::DefineClass(env, "EventFilter", descriptor, count, EventFilterNapi::New);
}

napi_value EventFilterNapi::New(napi_env env, napi_callback_info info)
{
    LOG_DEBUG("EventFilterNapi new");
    auto ctxt = std::make_shared<ContextBase>();
    auto input = [env, ctxt](size_t argc, napi_value* argv) {
        CHECK_ARGS_RETURN_VOID(ctxt, argc >= 1, "invalid arguments!");
    };
    ctxt->GetCbInfoSync(env, info, input);
    NAPI_ASSERT(env, ctxt->status == napi_ok, "invalid arguments!");

    auto instance = new (std::nothrow) EventFilterNapi();
    NAPI_ASSERT(env, instance !=nullptr, "no memory for EventFilterNapi");

    auto finalize = [](napi_env env, void* data, void* hint) {
        LOG_DEBUG("EventFilterNapi finalize.");
        auto* instance = reinterpret_cast<EventFilterNapi*>(data);
        CHECK_RETURN_VOID(instance != nullptr, "finalize null!");
        delete instance;
    };
    if (napi_wrap(env, ctxt->self, instance, finalize, nullptr, nullptr) != napi_ok) {
        delete instance;
        GET_AND_THROW_LAST_ERROR(env);
        return nullptr;
    }
    return ctxt->self;
}

napi_status EventFilterNapi::ToJson(napi_env env, napi_value inner, EventFilterNapi*& out)
{
    LOG_DEBUG("EventFilterNapi::ToJson");
    return NapiUtil::Unwrap(env, inner, reinterpret_cast<void**>(&out), EventFilterNapi::Constructor(env));
}

void EventFilterNapi::SetNative(std::shared_ptr<Native::EventFilter>& eventFilter)
{
    eventFilter_ = eventFilter;
}

std::shared_ptr<Native::EventFilter>& EventFilterNapi::GetNative()
{
    return eventFilter_;
}

napi_value EventFilterNapi::FilterById(napi_env env, napi_callback_info info)
{
    LOG_INFO("FilterById");
    napi_value result = nullptr;
    napi_value thisVar = nullptr;
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr);
    NAPI_ASSERT(env, argc == 1, "requires 1 parameter");
    std::vector<int> ids;
    auto status = NapiUtil::GetValueArray(env, argv[0], ids);
    CHECK_RETURN(status == napi_ok, "GetValueArray failed", nullptr);
    EventFilterNapi *filter;
    status = napi_new_instance(env, EventFilterNapi::Constructor(env), argc, argv, &result);
    CHECK_RETURN(status == napi_ok, "napi_new_instance failed", result);
    CHECK_RETURN(result != nullptr, "napi_new_instance failed", result);
    status = napi_unwrap(env, result, reinterpret_cast<void**>(&filter));
    CHECK_RETURN(status == napi_ok, "napi_unwrap failed", nullptr);
    CHECK_RETURN(filter != nullptr, "filter is null!", result);
    auto nativeFilter = Native::FilterById(ids);
    CHECK_RETURN(nativeFilter != nullptr, "Native::FilterById failed!", result);
    filter->SetNative(nativeFilter);
    return result;
}


napi_value EventFilterNapi::FilterByTime(napi_env env, napi_callback_info info)
{
    LOG_INFO("FilterByTime");
    const int paramNumber = 2;
    napi_value result = nullptr;
    napi_value thisVar = nullptr;
    size_t argc = paramNumber;
    napi_value argv[paramNumber] = { 0 };
    napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr);
    NAPI_ASSERT(env, argc == paramNumber, "requires 2 parameter");
    int64_t start = 0;
    int64_t end = 0;
    napi_status status = NapiUtil::GetValue(env, argv[0], start);
    NAPI_ASSERT(env, status == napi_ok, "GetValue start failed");
    status = NapiUtil::GetValue(env, argv[1], end);
    NAPI_ASSERT(env, status == napi_ok, "GetValue end failed");
    EventFilterNapi *filter;
    status = napi_new_instance(env, EventFilterNapi::Constructor(env), argc, argv, &result);
    CHECK_RETURN(status == napi_ok, "napi_new_instance failed", result);
    CHECK_RETURN(result != nullptr, "napi_new_instance failed", result);
    status = napi_unwrap(env, result, reinterpret_cast<void**>(&filter));
    CHECK_RETURN(status == napi_ok, "napi_unwrap failed", nullptr);
    CHECK_RETURN(filter != nullptr, "filter is null!", result);
    auto nativeFilter = Native::FilterByTime(start, end);
    CHECK_RETURN(nativeFilter != nullptr, "Native::FilterById failed!", result);
    filter->SetNative(nativeFilter);
    return result;
}

napi_value EventFilterNapi::FilterByTitle(napi_env env, napi_callback_info info)
{
    LOG_INFO("FilterByTitle");
    napi_value result = nullptr;
    napi_value thisVar = nullptr;
    size_t argc = 1;
    napi_value argv[1] = { 0 };
    napi_get_cb_info(env, info, &argc, argv, &thisVar, nullptr);
    NAPI_ASSERT(env, argc == 1, "requires 1 parameter");
    string title;
    napi_status status = NapiUtil::GetValue(env, argv[0], title);
    NAPI_ASSERT(env, status == napi_ok, "GetValue failed");
    EventFilterNapi *filter;
    status = napi_new_instance(env, EventFilterNapi::Constructor(env), argc, argv, &result);
    CHECK_RETURN(status == napi_ok, "napi_new_instance failed", result);
    CHECK_RETURN(result != nullptr, "napi_new_instance failed", result);
    status = napi_unwrap(env, result, reinterpret_cast<void**>(&filter));
    CHECK_RETURN(status == napi_ok, "napi_unwrap failed", nullptr);
    CHECK_RETURN(filter != nullptr, "filter is null!", result);
    auto nativeFilter = Native::FilterByTitle(title);
    CHECK_RETURN(nativeFilter != nullptr, "Native::FilterById failed!", result);
    filter->SetNative(nativeFilter);
    return result;
}

napi_value EventFilterNapi::Init(napi_env env, napi_value exports)
{
    napi_set_named_property(env, exports, "EventFilter", EventFilterNapi::Constructor(env));
    return exports;
}

}