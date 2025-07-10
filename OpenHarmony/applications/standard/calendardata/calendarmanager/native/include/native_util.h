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

#ifndef CALENDAR_NATIVE_UTILS_H
#define CALENDAR_NATIVE_UTILS_H

#include <string>
#include <string_view>
#include <vector>
#include <map>

#include "calendar_define.h"
#include "datashare_result_set.h"
#include "datashare_values_bucket.h"
#include "native_calendar.h"

namespace OHOS::CalendarApi::Native {
    using DataShareResultSetPtr = std::shared_ptr<DataShare::DataShareResultSet>;
    void DumpCalendarAccount(const CalendarAccount &account);
    void DumpEvent(const Event &event);
    DataShare::DataShareValuesBucket BuildValueEvent(const Event &event, int calendarId, int channelId, bool isUpdate);
    DataShare::DataShareValuesBucket BuildAttendeeValue(const Attendee &attendee, int eventId);
    int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, int& out);
    int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, std::string& out);
    int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, int64_t& out);
    void SetRRuleValue(const std::map<std::string, std::string> &ruleMap, RecurrenceRule &out);
    void SetByDayOfRRule(const std::vector<std::string> &weekDayList, RecurrenceRule &out);
    std::string GetDaysOfWeekRule(int minValue, int maxValue, const std::vector<int64_t> &daysOfWeekList);
    std::string GetDaysOfWeekMonthRule(
        const std::vector<int64_t> &daysOfWeekList, const std::vector<int64_t> &weeksOfMonthList);
    std::string GetRRuleSerial(int minValue, int maxValue, const std::vector<int64_t> &serialList);
    std::string GetWeeklyRule(const Event &event, const std::tm &time);
    std::string GetMonthlyRule(const Event &event, const std::tm &time);
    std::string GetYearlyRule(const Event &event, const std::tm &time);
    std::string GetEventRRule(const Event &event);

    std::vector<std::shared_ptr<Calendar>> ResultSetToCalendars(DataShareResultSetPtr &resultSet);
    int ResultSetToEvents(std::vector<Event> &events,
        DataShareResultSetPtr &resultSet, const std::set<std::string>& columns);
    int ResultSetToAttendees(std::vector<Attendee> &attendees, DataShareResultSetPtr &resultSet);
    int ResultSetToReminders(std::vector<int> &reminders, DataShareResultSetPtr &resultSet);
    void ResultSetToAttendeeStatus(Attendee &attendee, DataShareResultSetPtr &resultSet);
    void ResultSetToAttendeeType(Attendee &attendee, DataShareResultSetPtr &resultSet);
    void ResultSetToConfig(CalendarConfig &config, DataShareResultSetPtr &resultSet);
    void SetField(const std::vector<string>& eventKey,
        std::vector<string>& queryField, std::set<string>& resultSetField);

    bool ColorParse(const std::string& colorStr, variant<string, int64_t>& colorValue);

    std::string GetUTCTime(const int64_t &timeValue);
    std::string GetUTCTimes(const std::vector<int64_t> &timeValues);
    std::string GetRule(const Event &event);
    std::time_t TimeToUTC(const std::string &strTime);
    std::string EventIdsToString(const std::vector<int> &ids);
    template<typename T>
    int GetValue(DataShareResultSetPtr &resultSet, string_view fieldName, T& out)
    {
        int index = 0;
        auto fieldNameStr = string(fieldName);
        auto ret = resultSet->GetColumnIndex(fieldNameStr, index);
        if (ret != DataShare::E_OK) {
            return ret;
        }
        return GetIndexValue(resultSet, index, out);
    }

    template<typename T>
    int GetValueOptional(DataShareResultSetPtr &resultSet, string_view fieldName, std::optional<T>& out)
    {
        out = std::nullopt;
        int index = 0;
        auto fieldNameStr = string(fieldName);
        auto ret = resultSet->GetColumnIndex(string(fieldName), index);
        if (ret != DataShare::E_OK) {
            return ret;
        }
        T value;
        ret = GetIndexValue(resultSet, index, value);
        if (ret != DataShare::E_OK) {
            return ret;
        }
        out = value;
        return ret;
    }
}
 // namespace OHOS::Calendar::NativeUtils

#endif /* CALENDAR_NATIVE_UTILS_H */