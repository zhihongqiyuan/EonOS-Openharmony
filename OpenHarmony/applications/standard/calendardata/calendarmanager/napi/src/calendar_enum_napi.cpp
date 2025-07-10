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

#include "calendar_enum_napi.h"
#include <map>
#include <vector>
#include <string_view>
#include "calendar_define.h"
#include "napi_queue.h"


namespace OHOS::CalendarApi {
struct JsEnumInt {
    std::string_view enumName;
    int32_t enumInt;
};

struct JsEnumString {
    std::string_view enumName;
    std::string_view enumString;
};


static const std::vector<struct JsEnumInt> g_eventType = {
    { "IMPORTANT", EventType::Important },
    { "NORMAL", EventType::Normal },
};

static const std::vector<struct JsEnumInt> g_recurrenceFrequency = {
    { "YEARLY", RecurrenceType::YEARLY },
    { "MONTHLY", RecurrenceType::MONTHLY },
    { "WEEKLY", RecurrenceType::WEEKLY },
    { "DAILY", RecurrenceType::DAILY },
};

static const std::vector<struct JsEnumString> g_calendarTypeKey = {
    { "LOCAL", "local" },
    { "EMAIL", "email" },
    { "BIRTHDAY", "birthday" },
    { "CALDAV", "caldav" },
    { "SUBSCRIBED", "subscribed" },
};

static const std::vector<struct JsEnumString> g_serviceType = {
    { "MEETING", "Meeting" },
    { "WATCHING", "Watching" },
    { "REPAYMENT", "Repayment" },
    { "LIVE", "Live" },
    { "SHOPPING", "Shopping" },
    { "TRIP", "Trip" },
    { "CLASS", "Class" },
    { "SPORTS_EVENTS", "SportsEvents" },
    { "SPORTS_EXERCISE", "SportsExercise" },
};

static const std::vector<struct JsEnumString> g_attendeeRole = {
    { "ORGANIZER", "organizer" },
    { "PARTICIPANT", "participant" },
};

static const std::vector<struct JsEnumInt> g_attendeeStatus = {
    {"UNKNOWN", AttendeeStatus::UNKNOWN},
    {"TENTATIVE", AttendeeStatus::TENTATIVE},
    {"ACCEPTED", AttendeeStatus::ACCEPTED},
    {"DECLINED", AttendeeStatus::DECLINED},
    {"UNRESPONSIVE", AttendeeStatus::UNRESPONSIVE},
};

static const std::vector<struct JsEnumInt> g_attendeeType = {
    {"REQUIRED", AttendeeType::REQUIRED},
    {"OPTIONAL", AttendeeType::OPTIONAL},
    {"RESOURCE", AttendeeType::RESOURCE},
};

static const std::map<std::string_view, const std::vector<struct JsEnumInt>&> g_intEnumClassMap = {
    { "EventType", g_eventType},
    { "RecurrenceFrequency", g_recurrenceFrequency},
    {"AttendeeStatus", g_attendeeStatus},
    {"AttendeeType", g_attendeeType}
};

static const std::map<std::string_view, const std::vector<struct JsEnumString>&> g_stringEnumClassMap = {
    { "CalendarType", g_calendarTypeKey },
    { "ServiceType", g_serviceType },
    { "AttendeeRole", g_attendeeRole },
};

napi_value CalendarEnumNapi::JsEnumIntInit(napi_env env, napi_value exports)
{
    for (const auto &enumClass : g_intEnumClassMap) {
        auto &enumClassName = enumClass.first;
        auto &enumItemVec = enumClass.second;
        int32_t vecSize = static_cast<int32_t>(enumItemVec.size());
        std::vector<napi_value> value;
        value.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            napi_create_int32(env, enumItemVec[index].enumInt, &value[index]);
        }

        std::vector<napi_property_descriptor> property;
        property.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            property[index] = napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(
                enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor,
            nullptr, property.size(), property.data(), &result);
        CHECK_RETURN(status == napi_ok, "Failed to define enum", nullptr);

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        CHECK_RETURN(status == napi_ok, "Failed to set result", nullptr);
    }
    return exports;
}

napi_value CalendarEnumNapi::JsEnumStringInit(napi_env env, napi_value exports)
{
    for (auto it = g_stringEnumClassMap.begin(); it != g_stringEnumClassMap.end(); it++) {
        auto &enumClassName = it->first;
        auto &enumItemVec = it->second;
        int32_t vecSize = static_cast<int32_t>(enumItemVec.size());
        std::vector<napi_value> value;
        value.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            napi_create_string_utf8(env, enumItemVec[index].enumString.data(), NAPI_AUTO_LENGTH, &value[index]);
        }

        std::vector<napi_property_descriptor> property;
        property.resize(vecSize);
        for (int32_t index = 0; index < vecSize; ++index) {
            property[index] = napi_property_descriptor DECLARE_NAPI_STATIC_PROPERTY(
                enumItemVec[index].enumName.data(), value[index]);
        }

        auto constructor = [](napi_env env, napi_callback_info info) {
            napi_value jsThis = nullptr;
            napi_get_cb_info(env, info, nullptr, nullptr, &jsThis, nullptr);
            return jsThis;
        };

        napi_value result = nullptr;
        napi_status status = napi_define_class(env, enumClassName.data(), NAPI_AUTO_LENGTH, constructor,
            nullptr, property.size(), property.data(), &result);
        CHECK_RETURN(status == napi_ok, "Failed to define enum", nullptr);

        status = napi_set_named_property(env, exports, enumClassName.data(), result);
        CHECK_RETURN(status == napi_ok, "Failed to set result", nullptr);
    }
    return exports;
}

napi_value CalendarEnumNapi::Init(napi_env env, napi_value exports)
{
    JsEnumIntInit(env, exports);
    JsEnumStringInit(env, exports);
    return exports;
}
} // namespace CalendarApi