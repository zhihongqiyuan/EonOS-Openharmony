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
#ifndef OHOS_CALENDAR_NAPI_UTIL_H
#define OHOS_CALENDAR_NAPI_UTIL_H
#include <cstdint>
#include <map>
#include <variant>
#include "calendar_define.h"
#include "event_filter_napi.h"
#include "napi/native_api.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"

namespace OHOS::CalendarApi::NapiUtil {
    napi_status GetValue(napi_env env, napi_value in, napi_value& out);
    napi_status SetValue(napi_env env, napi_value in, napi_value& out);
    /* napi_value <-> bool */
    napi_status GetValue(napi_env env, napi_value in, bool& out);
    napi_status SetValue(napi_env env, const bool& in, napi_value& out);

    /* napi_value <-> int32_t */
    napi_status GetValue(napi_env env, napi_value in, int32_t& out);
    napi_status SetValue(napi_env env, const int32_t& in, napi_value& out);

    /* napi_value <-> uint32_t */
    napi_status GetValue(napi_env env, napi_value in, uint32_t& out);
    napi_status SetValue(napi_env env, const uint32_t& in, napi_value& out);

    /* napi_value <-> int64_t */
    napi_status GetValue(napi_env env, napi_value in, int64_t& out);
    napi_status SetValue(napi_env env, const int64_t& in, napi_value& out);

    /* napi_value <-> uint64_t */
    napi_status GetValue(napi_env env, napi_value in, uint64_t& out);
    napi_status SetValue(napi_env env, const uint64_t& in, napi_value& out);

    /* napi_value <-> double */
    napi_status GetValue(napi_env env, napi_value in, double& out);
    napi_status SetValue(napi_env env, const double& in, napi_value& out);

    /* napi_value <-> std::string */
    napi_status GetValue(napi_env env, napi_value in, std::string& out);
    napi_status SetValue(napi_env env, const std::string& in, napi_value& out);

    /* napi_value <-> std::vector<std::string> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<std::string>& out);
    napi_status SetValue(napi_env env, const std::vector<std::string>& in, napi_value& out);

    /* napi_value <-> std::vector<uint8_t> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<uint8_t>& out);
    napi_status SetValue(napi_env env, const std::vector<uint8_t>& in, napi_value& out);

    /* napi_value <-> std::vector<int32_t> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<int32_t>& out);
    napi_status SetValue(napi_env env, const std::vector<int32_t>& in, napi_value& out);

    /* napi_value <-> std::vector<uint32_t> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<uint32_t>& out);
    napi_status SetValue(napi_env env, const std::vector<uint32_t>& in, napi_value& out);

    /* napi_value <-> std::vector<int64_t> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<int64_t>& out);
    napi_status SetValue(napi_env env, const std::vector<int64_t>& in, napi_value& out);

    /* napi_value <-> std::vector<double> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<double>& out);
    napi_status SetValue(napi_env env, const std::vector<double>& in, napi_value& out);

    /* napi_value <-> CalendarAccount */
    napi_status GetValue(napi_env env, napi_value in, CalendarAccount& out);
    napi_status SetValue(napi_env env, const CalendarAccount& in, napi_value& out);

    /* napi_value <-> CalendarConfig */
    napi_status GetValue(napi_env env, napi_value in, CalendarConfig& out);
    napi_status SetValue(napi_env env, const CalendarConfig& in, napi_value& out);

    /* napi_value <-> Location */
    napi_status GetValue(napi_env env, napi_value in, Location& out);
    napi_status SetValue(napi_env env, const Location& in, napi_value& out);

    /* napi_value <-> RecurrenceRule */
    napi_status GetValue(napi_env env, napi_value in, RecurrenceRule& out);
    napi_status SetValue(napi_env env, const RecurrenceRule& in, napi_value& out);

    /* napi_value <-> Attendee */
    napi_status GetValue(napi_env env, napi_value in, Attendee& out);
    napi_status SetValue(napi_env env, const Attendee& in, napi_value& out);

    /* napi_value <-> std::vector<Attendee> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<Attendee>& out);
    napi_status SetValue(napi_env env, const std::vector<Attendee>& in, napi_value& out);

    /* napi_value <-> EventService */
    napi_status GetValue(napi_env env, napi_value in, EventService& out);
    napi_status SetValue(napi_env env, const EventService& in, napi_value& out);

    /* napi_value <-> EventFilter */
    napi_status GetValue(napi_env env, napi_value in, EventFilterNapi*& out);
    napi_status SetValue(napi_env env, const EventFilterNapi& in, napi_value& out);

    /* napi_value <-> EventType */
    napi_status GetValue(napi_env env, napi_value in, EventType& out);
    napi_status SetValue(napi_env env, const EventType& in, napi_value& out);

    /* napi_value <-> Event */
    napi_status GetValue(napi_env env, napi_value in, Event& out);
    napi_status SetValue(napi_env env, const Event& in, napi_value& out);

    /* napi_value <-> std::vector<Event> */
    napi_status GetValue(napi_env env, napi_value in, std::vector<Event>& out);
    napi_status SetValue(napi_env env, const std::vector<Event>& in, napi_value& out);

    /* AttendeeStatus -> napi_value */
    napi_status SetAttendeeStatus(napi_env env, const Attendee& in, napi_value& out);

    /* AttendeeType -> napi_value */
    napi_status SetAttendeeType(napi_env env, const Attendee& in, napi_value& out);

    /* napi_get_named_property wrapper */
    template <typename T>
    napi_status GetNamedProperty(napi_env env, napi_value in, const std::string& prop, T& value)
    {
        bool hasProp = false;
        napi_status status = napi_has_named_property(env, in, prop.c_str(), &hasProp);
        if ((status == napi_ok) && hasProp) {
            napi_value inner = nullptr;
            status = napi_get_named_property(env, in, prop.c_str(), &inner);
            if ((status == napi_ok) && (inner != nullptr)) {
                return GetValue(env, inner, value);
            }
        }
        return napi_invalid_arg;
    };

    template <typename T>
    napi_status SetNamedProperty(napi_env env, const string& prop, const T&nativeValue, napi_value& out)
    {
        napi_value value = nullptr;
        auto status = SetValue(env, nativeValue, value);
        if (status != napi_ok) {
            return status;
        }
        return napi_set_named_property(env, out, prop.c_str(), value);
    }

    template <typename T>
    void SetNamedPropertyOptional(napi_env env, const string& prop, const std::optional<T>&nativeValue, napi_value& out)
    {
        if (!nativeValue) {
            return;
        }
        napi_value value = nullptr;
        auto status = SetValue(env, nativeValue.value(), value);
        if (status != napi_ok) {
            return;
        }
        napi_set_named_property(env, out, prop.c_str(), value);
    }

    /* napi_get_named_property wrapper */
    template <typename T>
    void GetNamedPropertyOptional(napi_env env, napi_value in, const std::string& prop, std::optional<T>& out)
    {
        bool hasProp = false;
        napi_status status = napi_has_named_property(env, in, prop.c_str(), &hasProp);
        if ((status != napi_ok) || !hasProp) {
            out = std::nullopt;
            return;
        }
        napi_value inner = nullptr;
        status = napi_get_named_property(env, in, prop.c_str(), &inner);
        if ((status != napi_ok) || (inner == nullptr)) {
            out = std::nullopt;
            return;
        }
        T value;
        status = GetValue(env, inner, value);
        if (status != napi_ok) {
            out = std::nullopt;
            return;
        }
        out = value;
    };

    /* napi_value <-> std::vector<T> */
    template <typename T>
    napi_status SetValueArray(napi_env env, const std::vector<T>& in, napi_value& out)
    {
        napi_status status = napi_create_array_with_length(env, in.size(), &out);
        if (status != napi_ok) {
            return status;
        }
        int index = 0;
        for (auto& item : in) {
            napi_value element = nullptr;
            SetValue(env, item, element);
            status = napi_set_element(env, out, index++, element);
            if (status != napi_ok) {
                return status;
            }
        }
        return status;
    }

    /* napi_value <-> std::vector<T> */
    template <typename T>
    napi_status GetValueArray(napi_env env, napi_value in, std::vector<T>& out)
    {
        out.clear();
        bool isArray = false;
        napi_is_array(env, in, &isArray);
        if (!isArray) {
            return napi_invalid_arg;
        }

        uint32_t length = 0;
        napi_status status = napi_get_array_length(env, in, &length);
        if (status != napi_ok || length <= 0) {
            return napi_invalid_arg;
        }
        for (uint32_t i = 0; i < length; i++) {
            napi_value item = nullptr;
            status = napi_get_element(env, in, i, &item);
            if (item == nullptr || status != napi_ok) {
                return napi_invalid_arg;
            }
            T outItem;
            GetValue(env, item, outItem);
            out.push_back(outItem);
        }
        return status;
    }

    /* napi_define_class  wrapper */
    napi_value DefineClass(napi_env env, const std::string& name,
        const napi_property_descriptor* properties, size_t count, napi_callback newcb);

    /* napi_new_instance  wrapper */
    napi_ref NewWithRef(napi_env env, size_t argc, napi_value* argv, void** out, napi_value constructor);

    /* napi_unwrap with napi_instanceof */
    napi_status Unwrap(napi_env env, napi_value in, void** out, napi_value constructor);

    bool Equals(napi_env env, napi_value value, napi_ref copy);

} // namespace OHOS::CalendarApi::NapiUtil
#endif // OHOS_CALENDAR_NAPI_UTIL_H
