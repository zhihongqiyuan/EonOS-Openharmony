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
#include <sstream>
#include <iomanip>
#include "calendar_log.h"
#include "native_util.h"
#include <ctime>
#include <numeric>

namespace OHOS::CalendarApi::Native {
const int MIN_DAY_OF_WEEK = 1;
const int MAX_DAY_OF_WEEK = 7;
const int MIN_MONTH_OF_YEAR = -12;
const int MAX_MONTH_OF_YEAR = 12;
const int MIN_DAY_OF_MONTH = -31;
const int MAX_DAY_OF_MONTH = 31;
const int MIN_DAY_OF_YEAR = -366;
const int MAX_DAY_OF_YEAR = 366;
const int MIN_WEEK_OF_YEAR = -53;
const int MAX_WEEK_OF_YEAR = 53;
const int MIN_WEEK_OF_MONTH = -5;
const int MAX_WEEK_OF_MONTH = 5;
void DumpCalendarAccount(const CalendarAccount &account)
{
    LOG_DEBUG("account.name:%{private}s", account.name.c_str());
    LOG_DEBUG("account.type:%{private}s", account.type.c_str());
    LOG_DEBUG("account.displayName:%{private}s", account.displayName.value_or("").c_str());
}

void DumpEvent(const Event &event)
{
    LOG_DEBUG("id       :%{private}d", event.id.value_or(-1));
    LOG_DEBUG("type     :%{private}d", event.type);
    LOG_DEBUG("title    :%{private}s", event.title.value_or("[null]").c_str());
    if (event.location) {
        auto location = event.location.value();
        LOG_DEBUG("location.location  :%{private}s", location.location.value_or("[null]").c_str());
        LOG_DEBUG("location.longitude :%{private}lf", location.longitude.value_or(-1));
        LOG_DEBUG("location.latitude  :%{private}lf", location.latitude.value_or(-1));
    } else {
        LOG_DEBUG("location [null]");
    }
    if (event.service) {
        auto service = event.service.value();
        LOG_DEBUG("service.type  :%{private}s", service.type.c_str());
        LOG_DEBUG("service.description :%{private}s", service.description.value_or("[null]").c_str());
        LOG_DEBUG("service.uri  :%{private}s", service.uri.c_str());
    } else {
        LOG_DEBUG("service [null]");
    }
    if (event.recurrenceRule.has_value()) {
        LOG_DEBUG("recurrenceRule.recurrenceFrequency: %{private}d", event.recurrenceRule.value().recurrenceFrequency);
    }
    LOG_DEBUG("startTime    :%{private}s", std::to_string(event.startTime).c_str());
    LOG_DEBUG("endTime      :%{private}s", std::to_string(event.endTime).c_str());
    LOG_DEBUG("isAllDay :%{private}d", event.isAllDay.value_or(0));

    for (const auto &attendee : event.attendees) {
        LOG_DEBUG("attendee.name   :%{private}s", attendee.name.c_str());
        LOG_DEBUG("attendee.email  :%{private}s", attendee.email.c_str());
    }

    LOG_DEBUG("timeZone     :%{private}s", event.timeZone.value_or("[null]").c_str());
    LOG_DEBUG("description  :%{private}s", event.description.value_or("[null]").c_str());
}

void BuildEventLocation(DataShare::DataShareValuesBucket &valuesBucket, const Event &event)
{
    if (!event.location) {
        return;
    }
    auto location = event.location.value();
    if (location.location) {
        valuesBucket.Put("eventLocation", location.location.value());
    } else {
        valuesBucket.Put("eventLocation", "");
    }
    if (location.longitude) {
        // longitude is string in db
        valuesBucket.Put("location_longitude", std::to_string(location.longitude.value()));
    } else {
        valuesBucket.Put("location_longitude", "");
    }
    if (location.latitude) {
        // latitude is string in db
        valuesBucket.Put("location_latitude", std::to_string(location.latitude.value()));
    } else {
        valuesBucket.Put("location_latitude", "");
    }
}

void BuildEventService(DataShare::DataShareValuesBucket &valuesBucket, const Event &event)
{
    if (!event.service.has_value()) {
        LOG_INFO("BuildEventService event.service has no value ");
        return;
    }
    const auto service = event.service.value();
    if (service.description) {
        valuesBucket.Put("service_description", service.description.value());
    } else {
        valuesBucket.Put("service_description", "");
    }
    valuesBucket.Put("service_type", service.type);
    valuesBucket.Put("service_cp_bz_uri", service.uri);
}

std::string GetUTCTime(const int64_t &timeValue)
{
    const int monOffset = 1;
    const int strLen = 2;
    const int baseYear = 1900;
    time_t expire = timeValue / 1000;
    std::tm expireTime = {0};
#ifdef WINDOWS_PLATFORM
    if (gmtime_s(&expireTime, &expire)) {
#else
    if (gmtime_r(&expire, &expireTime) == nullptr) {
#endif
        return {};
    }
    std::stringstream out;
    out << (expireTime.tm_year + baseYear);
    out << std::setfill('0') << std::setw(strLen) << expireTime.tm_mon + monOffset;
    out << std::setfill('0') << std::setw(strLen) << expireTime.tm_mday;
    out << "T";
    out << std::setfill('0') << std::setw(strLen) << expireTime.tm_hour;
    out << std::setfill('0') << std::setw(strLen) << expireTime.tm_min;
    out << std::setfill('0') << std::setw(strLen) << expireTime.tm_sec;
    out << "Z";

    return out.str();
}

std::string GetUTCTimes(const std::vector<int64_t> &timeValues)
{
    std::stringstream out;
    if (timeValues.size() == 0) {
        return out.str();
    }
    
    const auto timeLen = timeValues.size() - 1;
    if (timeLen == 0) {
        out << GetUTCTime(timeValues[0]);
        return out.str();
    }
    
    for (unsigned int i = 0; i <= timeLen; i++) {
        out << GetUTCTime(timeValues[i]);
        if (i != timeLen) {
            out << ",";
        }
    }

    return out.str();
}

std::string GetRule(const Event &event)
{
    const time_t now = event.startTime / 1000;
    const std::tm *time = std::localtime(&now);
    std::string rrule;
    RecurrenceType recurrenceFrequency = event.recurrenceRule.value().recurrenceFrequency;
    if (recurrenceFrequency == DAILY) {
        rrule = "FREQ=DAILY" + GetEventRRule(event) + ";WKST=SU";
    } else if (recurrenceFrequency == WEEKLY) {
        rrule = "FREQ=WEEKLY" + GetEventRRule(event) + ";WKST=SU;BYDAY=";
        if (time != nullptr) {
            rrule += GetWeeklyRule(event, *time);
        }
    } else if (recurrenceFrequency == MONTHLY) {
        rrule = "FREQ=MONTHLY" + GetEventRRule(event) + ";WKST=SU";
        if (time != nullptr) {
            rrule += GetMonthlyRule(event, *time);
        }
    } else if (recurrenceFrequency == YEARLY) {
        rrule = "FREQ=YEARLY" + GetEventRRule(event) + ";WKST=SU";
        if (time != nullptr) {
            rrule += GetYearlyRule(event, *time);
        }
    } else if (recurrenceFrequency == NORULE) {
        LOG_INFO(" recurrenceFrequency == NORULE");
        rrule = "";
    }

    return rrule;
}

std::string GetEventRRule(const Event &event)
{
    auto recurrenceRule = event.recurrenceRule.value();
    std::string rrule;
    if (recurrenceRule.expire.has_value() && recurrenceRule.expire.value() > 0) {
        rrule += ";UNTIL=" + GetUTCTime(event.recurrenceRule.value().expire.value());
    }
    if (recurrenceRule.count.has_value() && recurrenceRule.count.value() > 0) {
        rrule += ";COUNT=" + std::to_string(recurrenceRule.count.value());
    }
    if (recurrenceRule.interval.has_value() && recurrenceRule.interval.value() > 0) {
        rrule += ";INTERVAL=" + std::to_string(recurrenceRule.interval.value());
    }
    return rrule;
}

std::string GetWeeklyRule(const Event &event, const std::tm &time)
{
    std::string rrule;
    bool isHasSetData = false;
    auto rRuleValue = event.recurrenceRule.value();
    const std::vector<string> weekList = {"SU", "MO", "TU", "WE", "TH", "FR", "SA"};
    if (rRuleValue.daysOfWeek.has_value()) {
        isHasSetData = true;
        auto daysOfWeekList = rRuleValue.daysOfWeek.value();
        rrule = GetDaysOfWeekRule(MIN_DAY_OF_WEEK, MAX_DAY_OF_WEEK, daysOfWeekList);
    }
    if (isHasSetData == false) {
        if (time.tm_wday < MAX_DAY_OF_WEEK) {
            rrule = weekList[time.tm_wday];
        }
    }
    return rrule;
}

std::string GetMonthlyRule(const Event &event, const std::tm &time)
{
    bool isHasSetData = false;
    std::string rrule;
    auto rruleValue = event.recurrenceRule.value();
    if (rruleValue.daysOfWeek.has_value() && rruleValue.weeksOfMonth.has_value()) {
        isHasSetData = true;
        rrule += ";BYDAY=";
        rrule += GetDaysOfWeekMonthRule(rruleValue.daysOfWeek.value(), rruleValue.weeksOfMonth.value());
    }

    if (rruleValue.daysOfMonth.has_value()) {
        isHasSetData = true;
        rrule += ";BYMONTHDAY=";
        auto daysOfMonthList = rruleValue.daysOfMonth.value();
        rrule += GetRRuleSerial(MIN_DAY_OF_MONTH, MAX_DAY_OF_MONTH, daysOfMonthList);
    }
    if (isHasSetData == false) {
        rrule += ";BYMONTHDAY=";
        rrule += std::to_string(time.tm_mday);
    }
    return rrule;
}

std::string GetYearlyRule(const Event &event, const std::tm &time)
{
    const int monOffset = 1;
    bool isHasSetData = false;
    std::string rrule;
    auto rruleValue = event.recurrenceRule.value();
    if (rruleValue.daysOfYear.has_value()) {
        isHasSetData = true;
        std::string days = GetRRuleSerial(MIN_DAY_OF_YEAR, MAX_DAY_OF_YEAR, rruleValue.daysOfYear.value());
        rrule = ";BYYEARDAY=" + days;
    }
    if (rruleValue.weeksOfYear.has_value() && rruleValue.daysOfWeek.has_value()) {
        isHasSetData = true;
        auto weekOfYearList = rruleValue.weeksOfYear.value();
        std::string weeks = GetRRuleSerial(MIN_WEEK_OF_YEAR, MAX_WEEK_OF_YEAR, weekOfYearList);
        auto daysOfWeekList = rruleValue.daysOfWeek.value();
        std::string weekdays = GetDaysOfWeekRule(MIN_DAY_OF_WEEK, MAX_DAY_OF_WEEK, daysOfWeekList);
        rrule = ";BYWEEKNO=" + weeks + ";BYDAY=" + weekdays;
    }
    if (rruleValue.monthsOfYear.has_value() && rruleValue.daysOfMonth.has_value()) {
        isHasSetData = true;
        auto daysOfMonthList = rruleValue.daysOfMonth.value();
        auto monthsOfYearList = rruleValue.monthsOfYear.value();
        std::string days = GetRRuleSerial(MIN_DAY_OF_MONTH, MAX_DAY_OF_MONTH, daysOfMonthList);
        std::string months = GetRRuleSerial(MIN_MONTH_OF_YEAR, MAX_MONTH_OF_YEAR, monthsOfYearList);
        rrule = ";BYMONTHDAY=" + days + ";BYMONTH=" + months;
    }
    if (rruleValue.monthsOfYear.has_value() && rruleValue.weeksOfMonth.has_value() &&
        rruleValue.daysOfWeek.has_value()) {
        isHasSetData = true;
        auto monthsOfYearList = rruleValue.monthsOfYear.value();
        auto weeksOfMonthList = rruleValue.weeksOfMonth.value();
        auto daysOfWeekList = rruleValue.daysOfWeek.value();
        std::string months = GetRRuleSerial(MIN_MONTH_OF_YEAR, MAX_MONTH_OF_YEAR, monthsOfYearList);
        std::string daysOfWeekMonth = GetDaysOfWeekMonthRule(daysOfWeekList, weeksOfMonthList);
        rrule = ";BYDAY=" + daysOfWeekMonth + ";BYMONTH=" + months;
    }
    if (isHasSetData == false) {
        rrule += ";BYMONTHDAY=";
        rrule += std::to_string(time.tm_mday);
        rrule += ";BYMONTH=";
        rrule += std::to_string(time.tm_mon + monOffset);
    }
    return rrule;
}

std::string GetDaysOfWeekRule(int minValue, int maxValue, const std::vector<int64_t> &daysOfWeekList)
{
    std::string rrule;
    const std::vector<string> weekDayList = {"MO", "TU", "WE", "TH", "FR", "SA", "SU"};
    for (const auto &dayOfWeek : daysOfWeekList) {
        if (dayOfWeek <= maxValue && dayOfWeek >= minValue) {
            if (&dayOfWeek == &daysOfWeekList.back()) {
                rrule = rrule + weekDayList[dayOfWeek - 1];
                break;
            }
            rrule = rrule + weekDayList[dayOfWeek - 1] + ",";
        }
    }
    return rrule;
}

std::string GetDaysOfWeekMonthRule(
    const std::vector<int64_t> &daysOfWeekList, const std::vector<int64_t> &weeksOfMonthList)
{
    std::string rrule;
    const std::vector<string> weekDayList = {"MO", "TU", "WE", "TH", "FR", "SA", "SU"};
    auto daysLen = daysOfWeekList.size();
    for (size_t i = 0; i < daysLen; i++) {
        if (daysOfWeekList[i] >= MIN_DAY_OF_WEEK && daysOfWeekList[i] <= MAX_DAY_OF_WEEK &&
            weeksOfMonthList[i] >= MIN_WEEK_OF_MONTH && weeksOfMonthList[i] <= MAX_WEEK_OF_MONTH) {
            if (i == daysLen - 1) {
                rrule = rrule + std::to_string(weeksOfMonthList[i]) + weekDayList[daysOfWeekList[i] - 1];
                break;
            } else {
                rrule = rrule + std::to_string(weeksOfMonthList[i]) + weekDayList[daysOfWeekList[i] - 1] + ",";
            }
        }
    }
    return rrule;
}

std::string GetRRuleSerial(int minValue, int maxValue, const std::vector<int64_t> &serialList)
{
    std::string rrule;
    for (const auto &serial : serialList) {
        if (serial >= minValue && serial <= maxValue) {
            if (&serial == &serialList.back()) {
                rrule = rrule + std::to_string(serial);
                break;
            }
            rrule = rrule + std::to_string(serial) + ",";
        }
    }
    return rrule;
}

void BuildEventRecurrenceRule(DataShare::DataShareValuesBucket &valuesBucket, const Event &event)
{
    if (!event.recurrenceRule.has_value()) {
        return;
    }
    
    std::string rrule = GetRule(event);
    valuesBucket.Put("rrule", rrule);
    
    if (event.recurrenceRule.value().excludedDates.has_value()) {
        const auto excludedDateStr = GetUTCTimes(event.recurrenceRule.value().excludedDates.value());
        valuesBucket.Put("exdate", excludedDateStr);
    }
}

DataShare::DataShareValuesBucket BuildValueEvent(const Event &event, int calendarId, int channelId, bool isUpdate)
{
    DataShare::DataShareValuesBucket valuesBucket;
    valuesBucket.Put("calendar_id", calendarId);
    if (!isUpdate) {
        LOG_DEBUG("title %{private}s", event.title.value_or("").c_str());
        valuesBucket.Put("title", event.title.value_or(""));
    } else {
        if (event.title) {
            valuesBucket.Put("title", event.title.value());
        }
    }
    valuesBucket.Put("important_event_type", event.type);
    valuesBucket.Put("dtstart", event.startTime);
    valuesBucket.Put("dtend", event.endTime);
    valuesBucket.Put("channel_id", channelId);

    BuildEventLocation(valuesBucket, event);
    BuildEventService(valuesBucket, event);
    BuildEventRecurrenceRule(valuesBucket, event);

    LOG_DEBUG("description %{private}s", event.description.value_or("").c_str());

    if (event.description.has_value()) {
        valuesBucket.Put("description", event.description.value());
    }
    if (event.timeZone.has_value()) {
        valuesBucket.Put("eventTimezone", event.timeZone.value());
    }
    if (event.isAllDay.has_value()) {
        valuesBucket.Put("allDay", event.isAllDay.value());
    }
    if (event.identifier.has_value()) {
        valuesBucket.Put("identifier", event.identifier.value());
    }
    if (event.isLunar.has_value()) {
        valuesBucket.Put("event_calendar_type", event.isLunar.value());
    }
    return valuesBucket;
}

DataShare::DataShareValuesBucket BuildAttendeeValue(const Attendee &attendee, int eventId)
{
    DataShare::DataShareValuesBucket valuesBucket;
    valuesBucket.Put("event_id", eventId);
    valuesBucket.Put("attendeeName", attendee.name);
    LOG_DEBUG("attendeeName %{private}s", attendee.name.c_str());
    valuesBucket.Put("attendeeEmail", attendee.email);
    LOG_DEBUG("attendeeEmail %{private}s", attendee.email.c_str());
    if (attendee.role.has_value()) {
        valuesBucket.Put("attendeeRelationship", attendee.role.value());
    }
    if (attendee.status.has_value()) {
        valuesBucket.Put("attendeeStatus", attendee.status.value());
    }
    if (attendee.type.has_value()) {
        valuesBucket.Put("attendeeType", attendee.type.value());
    }

    return valuesBucket;
}

int GetValue(DataShareResultSetPtr &resultSet, string_view fieldName, int& out)
{
    int index = 0;
    auto ret = resultSet->GetColumnIndex(string(fieldName), index);
    if (ret != DataShare::E_OK) {
        return ret;
    }
    return resultSet->GetInt(index, out);
}

int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, std::string& out)
{
    return resultSet->GetString(index, out);
}

int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, int& out)
{
    return resultSet->GetInt(index, out);
}

int GetIndexValue(const DataShareResultSetPtr &resultSet, int index, int64_t& out)
{
    return resultSet->GetLong(index, out);
}

int GetValue(DataShareResultSetPtr &resultSet, string_view fieldName, std::string& out)
{
    int index = 0;
    auto fieldNameStr = string(fieldName);
    auto ret = resultSet->GetColumnIndex(fieldNameStr, index);
    if (ret != DataShare::E_OK) {
        LOG_WARN("GetValue [%{private}s] failed [%{private}d]", fieldNameStr.c_str(), ret);
        return ret;
    }
    return resultSet->GetString(index, out);
}

std::vector<std::shared_ptr<Calendar>> ResultSetToCalendars(DataShareResultSetPtr &resultSet)
{
    std::vector<std::shared_ptr<Calendar>> result;
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    LOG_INFO("GetRowCount is %{public}d", rowCount);
    if (rowCount == 0) {
        return result;
    }
    auto err = resultSet->GoToFirstRow();
    if (err != DataShare::E_OK) {
        LOG_INFO("Failed GoToFirstRow %{public}d", err);
        return result;
    }
    do {
        int idValue = -1;
        if (GetValue(resultSet, "_id", idValue) != DataShare::E_OK) {
            break;
        }
        LOG_DEBUG("id: %{private}d", idValue);
        std::string nameValue;
        if (GetValue(resultSet, "account_name", nameValue) != DataShare::E_OK) {
            break;
        }
        LOG_DEBUG("account_name: %{private}s", nameValue.c_str());
        std::string typeValue;
        if (GetValue(resultSet, "account_type", typeValue) != DataShare::E_OK) {
            break;
        }
        LOG_DEBUG("account_type: %{private}s", typeValue.c_str());

        std::string displayNameValue;
        GetValue(resultSet, "calendar_displayName", displayNameValue);
        LOG_DEBUG("calendar_displayName: %{private}s", displayNameValue.c_str());

        int canReminder = -1;
        GetValue(resultSet, "canReminder", canReminder);
        LOG_DEBUG("canReminder: %{private}d", canReminder);

        int colorValue = 0;
        GetValue(resultSet, "calendar_color", colorValue);
        CalendarAccount curAccount {nameValue, typeValue, displayNameValue};
        result.emplace_back(std::make_shared<Calendar>(curAccount, idValue));
    } while (resultSet->GoToNextRow() == DataShare::E_OK);
    return result;
}

std::optional<Location> ResultSetToLocation(DataShareResultSetPtr &resultSet)
{
    Location out;
    string value;
    auto ret = GetValue(resultSet, "eventLocation", value);
    out.location = std::make_optional<string>(value);
    ret = GetValue(resultSet, "location_longitude", value);
    double longitudeValue = -1;
    std::stringstream str2digit;
    str2digit << value;
    str2digit >> longitudeValue;
    if (longitudeValue != -1) {
        out.longitude = std::make_optional<double>(longitudeValue);
    }
    ret = GetValue(resultSet, "location_latitude", value);
    double latitudeValue = -1;
    str2digit.clear();
    str2digit << value;
    str2digit >> latitudeValue;
    if (latitudeValue != -1) {
        out.latitude = std::make_optional<double>(latitudeValue);
    }

    if (ret != DataShare::E_OK) {
        return std::nullopt;
    }
    return std::make_optional<Location>(out);
}

std::optional<EventService> ResultSetToEventService(DataShareResultSetPtr &resultSet)
{
    EventService out;
    string value;
    auto ret = GetValue(resultSet, "service_type", value);
    if (ret != DataShare::E_OK) {
        return std::nullopt;
    }
    const std::set<std::string> serviceType = {"Meeting", "Watching", "Repayment", "Live", "Shopping",
                                               "Trip", "Class", "SportsEvents", "SportsExercise"};
    if (serviceType.count(value)) {
        out.type = value;
    } else {
        return std::nullopt;
    }
    ret = GetValue(resultSet, "service_cp_bz_uri", value);
    if (ret != DataShare::E_OK) {
        return std::nullopt;
    }
    out.uri = value;
    ret = GetValue(resultSet, "service_description", value);
    if (ret == DataShare::E_OK) {
        out.description = std::make_optional<string>(value);
    }
    return std::make_optional<EventService>(out);
}

int StringToInt(const std::string &str)
{
    try {
        return std::stoi(str);
    } catch (std::exception &ex) {
        LOG_ERROR("StringToInt conversion fail, str: %{public}s", str.c_str());
        return 0;
    }
}

std::time_t TimeToUTC(const std::string &strTime)
{
    const int baseYear = 1900;
    const int offset = 2;
    const int yearOffset = 4;
    const int monBase = 4;
    const int dayBase = 6;
    const int hourBase = 9;
    const int minBase = 11;
    const int secBase = 13;
    const int monCount = 12;
    const int monRectify = 11;
    const int micSecond = 1000;
    const int timeStrLenMin = 8;
    const int timeStrLen = 15;

    std::tm expireTime = {0};
    if (strTime.size() < timeStrLenMin) {
        LOG_DEBUG("strTime length error");
        return 0;
    }
    expireTime.tm_year = StringToInt(strTime.substr(0, yearOffset)) - baseYear;
    expireTime.tm_mon = (StringToInt(strTime.substr(monBase, offset)) + monRectify) % monCount;
    expireTime.tm_mday = StringToInt(strTime.substr(dayBase, offset));
    if (strTime.find("T") != std::string::npos && strTime.length() >= timeStrLen) {
        expireTime.tm_hour = StringToInt(strTime.substr(hourBase, offset));
        expireTime.tm_min = StringToInt(strTime.substr(minBase, offset));
        expireTime.tm_sec = StringToInt(strTime.substr(secBase,  offset));
    } else {
        expireTime.tm_hour = 0;
        expireTime.tm_min = 0;
        expireTime.tm_sec = 0;
    }

    std::time_t utcTime = mktime(&expireTime) * micSecond; //精确到微秒

    return utcTime;
}

std::vector<std::string> SplitString(const std::string &str, const std::string &flag)
{
    std::vector<std::string> result;
    std::string::size_type pos1 = 0;
    std::string::size_type pos2 = str.find(flag);
    while (pos2 != std::string::npos) {
        result.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + flag.size();
        pos2 = str.find(flag, pos1);
    }
    if (pos1 != str.length()) {
        result.push_back(str.substr(pos1));
    }

    return result;
}

std::optional<vector<int64_t>> ResultSetToExcludedDates(DataShareResultSetPtr &resultSet)
{
    std::string value;
    auto ret = GetValue(resultSet, "exdate", value);
    if (ret != DataShare::E_OK) {
        return std::nullopt;
    }
    std::vector<string> strListExDate = SplitString(value, ",");

    std::vector<int64_t> excludedDates;
    for (const auto &str : strListExDate) {
        auto exDate = TimeToUTC(str);
        excludedDates.emplace_back(exDate);
    }

    return std::make_optional<vector<int64_t>>(excludedDates);
}

void ConvertRecurrenceFrequency(const std::string &frequency, RecurrenceRule &rule)
{
    if (frequency == "YEARLY") {
        rule.recurrenceFrequency = YEARLY;
        return;
    }
    if (frequency == "MONTHLY") {
        rule.recurrenceFrequency = MONTHLY;
        return;
    }
    if (frequency == "WEEKLY") {
        rule.recurrenceFrequency = WEEKLY;
        return;
    }
    if (frequency == "DAILY") {
        rule.recurrenceFrequency = DAILY;
    }
}

std::optional<RecurrenceRule> ResultSetToRecurrenceRule(DataShareResultSetPtr &resultSet)
{
    const int strListSize = 2;
    RecurrenceRule out;
    out.recurrenceFrequency = NORULE;
    std::string value;
    auto ret = GetValue(resultSet, "rrule", value);
    if (ret != DataShare::E_OK) {
        return std::nullopt;
    }
    std::map<std::string, std::string> ruleMap;
    std::vector<std::string> strListRule = SplitString(value, ";");
    for (const auto &str : strListRule) {
        std::vector<std::string> keyAndValue = SplitString(str, "=");
        if (keyAndValue.size() == strListSize) {
            ruleMap.insert(std::pair<std::string, std::string>(keyAndValue[0], keyAndValue[1]));
        }
    }
    SetRRuleValue(ruleMap, out);
    out.excludedDates = ResultSetToExcludedDates(resultSet);

    return std::make_optional<RecurrenceRule>(out);
}

void SetVecNum(std::optional<std::vector<int64_t>> &ruleVec, const std::string &ruleStr)
{
    std::vector<std::string> weekNumList = SplitString(ruleStr, ",");
    for (const auto &weekNum : weekNumList) {
        ruleVec->push_back(StringToInt(weekNum));
    }
}

void SetRRuleValue(const std::map<std::string, std::string> &ruleMap, RecurrenceRule &out)
{
    std::map<std::string, std::string>::const_iterator iter;
    for (iter = ruleMap.begin(); iter != ruleMap.end(); iter++) {
        if (iter->first == "FREQ") {
            ConvertRecurrenceFrequency(iter->second, out);
            continue;
        }
        if (iter->first == "COUNT") {
            out.count = std::make_optional<int64_t>(StringToInt(iter->second));
            continue;
        }
        if (iter->first == "INTERVAL") {
            out.interval = std::make_optional<int64_t>(StringToInt(iter->second));
            continue;
        }
        if (iter->first == "UNTIL") {
            out.expire = std::make_optional<int64_t>(TimeToUTC(iter->second));
        }
        if (iter->first == "BYDAY") {
            std::vector<std::string> weekDayList = SplitString(iter->second, ",");
            SetByDayOfRRule(weekDayList, out);
        }
        if (iter->first == "BYWEEKNO") {
            out.weeksOfYear = std::make_optional<std::vector<int64_t>>();
            SetVecNum(out.weeksOfYear, iter->second);
        }
        if (iter->first == "BYMONTHDAY") {
            out.daysOfMonth = std::make_optional<std::vector<int64_t>>();
            SetVecNum(out.daysOfMonth, iter->second);
        }
        if (iter->first == "BYYEARDAY") {
            out.daysOfYear = std::make_optional<std::vector<int64_t>>();
            SetVecNum(out.daysOfYear, iter->second);
        }
        if (iter->first == "BYMONTH") {
            out.monthsOfYear = std::make_optional<std::vector<int64_t>>();
            SetVecNum(out.monthsOfYear, iter->second);
        }
    }
}

void SetByDayOfRRule(const std::vector<std::string> &weekDayList, RecurrenceRule &out)
{
    const int weekStrLen = 2;
    const std::vector<string> dayOfWeekList = {"MO", "TU", "WE", "TH", "FR", "SA", "SU"};
    out.daysOfWeek = std::make_optional<vector<int64_t>>();
    out.weeksOfMonth = std::make_optional<vector<int64_t>>();
    for (const auto &weekday : weekDayList) {
        if (weekday.length() > weekStrLen) {
            std::string weekDayStr = weekday.substr(weekday.length() - weekStrLen, weekStrLen);
            std::string WeekNumStr = weekday.substr(0, weekday.length() - weekStrLen);
            auto it = std::find(dayOfWeekList.begin(), dayOfWeekList.end(), weekDayStr);
            if (it != dayOfWeekList.end()) {
                int dayNum = it - dayOfWeekList.begin();
                out.daysOfWeek->push_back(dayNum + 1);
                out.weeksOfMonth->push_back(StringToInt(WeekNumStr));
            }
        } else if (weekday.length() == weekStrLen) {
            auto it = std::find(dayOfWeekList.begin(), dayOfWeekList.end(), weekday);
            if (it != dayOfWeekList.end()) {
                int dayNum = it - dayOfWeekList.begin();
                out.daysOfWeek->push_back(dayNum + 1);
            }
        }
    }
}

void ResultSetToInstanceTime(Event &event, DataShareResultSetPtr &resultSet, const std::set<std::string>& columns)
{
    if (columns.count("instanceStartTime")) {
        GetValueOptional(resultSet, "begin", event.instanceStartTime);
    }

    if (columns.count("instanceEndTime")) {
        GetValueOptional(resultSet, "end", event.instanceEndTime);
    }
}

void ResultSetToEvent(Event &event, DataShareResultSetPtr &resultSet, const std::set<std::string>& columns)
{
    GetValueOptional(resultSet, "_id", event.id);
    if (columns.count("type")) {
        int type = 0;
        GetValue(resultSet, "important_event_type", type);
        event.type = static_cast<EventType>(type);
    }
    if (columns.count("title")) {
        GetValueOptional(resultSet, "title", event.title);
    }
    if (columns.count("startTime")) {
        LOG_DEBUG("TLQ get startTime");
        GetValue(resultSet, "dtstart", event.startTime);
    }
    if (columns.count("endTime")) {
        LOG_DEBUG("TLQ get endTime");
        GetValue(resultSet, "dtend", event.endTime);
    }
    if (columns.count("isAllDay")) {
        int isAllDay = 0;
        GetValue(resultSet, "allDay", isAllDay);
        event.isAllDay = static_cast<bool>(isAllDay);
    }
    if (columns.count("description")) {
        GetValueOptional(resultSet, "description", event.description);
    }
    if (columns.count("timeZone")) {
        GetValueOptional(resultSet, "eventTimezone", event.timeZone);
    }
    if (columns.count("location")) {
        event.location = ResultSetToLocation(resultSet);
    }
    if (columns.count("service")) {
        event.service = ResultSetToEventService(resultSet);
    }
    if (columns.count("recurrenceRule")) {
        event.recurrenceRule = ResultSetToRecurrenceRule(resultSet);
    }

    if (columns.count("identifier")) {
        GetValueOptional(resultSet, "identifier", event.identifier);
    }
    if (columns.count("isLunar")) {
        int isLunar = 0;
        GetValue(resultSet, "event_calendar_type", isLunar);
        event.isLunar = static_cast<bool>(isLunar);
    }
    ResultSetToInstanceTime(event, resultSet, columns);
}

int ResultSetToEvents(std::vector<Event> &events, DataShareResultSetPtr &resultSet,
    const std::set<std::string>& columns)
{
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    LOG_INFO("GetRowCount is %{public}d", rowCount);
    if (rowCount <= 0) {
        return -1;
    }
    auto err = resultSet->GoToFirstRow();
    if (err != DataShare::E_OK) {
        LOG_ERROR("Failed GoToFirstRow %{public}d", err);
        return -1;
    }
    do {
        Event event;
        ResultSetToEvent(event, resultSet, columns);
        events.emplace_back(event);
    } while (resultSet->GoToNextRow() == DataShare::E_OK);
    return 0;
}

void ResultSetToAttendeeStatus(Attendee &attendee, DataShareResultSetPtr &resultSet)
{
    int statusValue = 0;
    GetValue(resultSet, "attendeeStatus", statusValue);
    if (statusValue == UNKNOWN) {
        attendee.status = std::make_optional<AttendeeStatus>(UNKNOWN);
    } else if (statusValue == TENTATIVE) {
        attendee.status = std::make_optional<AttendeeStatus>(TENTATIVE);
    } else if (statusValue == ACCEPTED) {
        attendee.status = std::make_optional<AttendeeStatus>(ACCEPTED);
    } else if (statusValue == DECLINED) {
        attendee.status = std::make_optional<AttendeeStatus>(DECLINED);
    } else {
        attendee.status = std::make_optional<AttendeeStatus>(UNRESPONSIVE);
    }
}

void ResultSetToAttendeeType(Attendee &attendee, DataShareResultSetPtr &resultSet)
{
    int typeValue = 0;
    GetValue(resultSet, "attendeeType", typeValue);
    if (typeValue == REQUIRED) {
        attendee.type = std::make_optional<AttendeeType>(REQUIRED);
    } else if (typeValue == OPTIONAL) {
        attendee.type = std::make_optional<AttendeeType>(OPTIONAL);
    } else {
        attendee.type = std::make_optional<AttendeeType>(RESOURCE);
    }
}

int ResultSetToAttendees(std::vector<Attendee> &attendees, DataShareResultSetPtr &resultSet)
{
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    LOG_INFO("GetRowCount is %{public}d", rowCount);
    if (rowCount <= 0) {
        return -1;
    }
    auto err = resultSet->GoToFirstRow();
    if (err != DataShare::E_OK) {
        LOG_ERROR("Failed GoToFirstRow %{public}d", err);
        return -1;
    }
    int roleValue = 0;
    do {
        Attendee attendee;
        GetValue(resultSet, "attendeeName", attendee.name);
        GetValue(resultSet, "attendeeEmail", attendee.email);
        GetValue(resultSet, "attendeeRelationship",  roleValue);
        if (roleValue == PARTICIPANT) {
            attendee.role = std::make_optional<RoleType>(PARTICIPANT);
        } else if (roleValue == ORGANIZER) {
            attendee.role = std::make_optional<RoleType>(ORGANIZER);
        }

        ResultSetToAttendeeStatus(attendee, resultSet);
        ResultSetToAttendeeType(attendee, resultSet);
        attendees.emplace_back(attendee);
    } while (resultSet->GoToNextRow() == DataShare::E_OK);
    return 0;
}

std::string EventIdsToString(const std::vector<int> &ids) {
    if (ids.empty()) {
        return "";
    }
    return std::accumulate(std::next(ids.begin()), ids.end(), std::to_string(ids[0]),
        [](const std::string& a, int b) {
            std::stringstream ss;
            ss << a << ", " << b;
            return ss.str();
        }
    );
}

int ResultSetToReminders(std::vector<int> &reminders, DataShareResultSetPtr &resultSet)
{
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    LOG_INFO("GetRowCount is %{public}d", rowCount);
    if (rowCount <= 0) {
        return -1;
    }
    auto err = resultSet->GoToFirstRow();
    if (err != DataShare::E_OK) {
        LOG_ERROR("Failed GoToFirstRow %{public}d", err);
        return -1;
    }
    do {
        int minutes = 0;
        GetValue(resultSet, "minutes", minutes);
        reminders.emplace_back(minutes);
    } while (resultSet->GoToNextRow() == DataShare::E_OK);
    return 0;
}

void ResultSetToConfig(CalendarConfig &config, DataShareResultSetPtr &resultSet)
{
    int rowCount = 0;
    resultSet->GetRowCount(rowCount);
    LOG_INFO("GetRowCount is %{public}d", rowCount);
    auto err = resultSet->GoToFirstRow();
    if (err != DataShare::E_OK) {
        LOG_ERROR("Failed GoToFirstRow %{public}d", err);
    }
    do {
        int enableReminder = 0;
        std::int64_t color = 0;
        GetValue(resultSet, "canReminder", enableReminder);
        GetValue(resultSet, "calendar_color", color);
        config.enableReminder = static_cast<bool>(enableReminder);
        LOG_INFO("enableReminder is %{public}d", enableReminder);
        config.color = color;
    } while (resultSet->GoToNextRow() == DataShare::E_OK);
}

bool IsValidHexString(const std::string& colorStr)
{
    if (colorStr.empty()) {
        return false;
    }
    for (char ch : colorStr) {
        if ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
            continue;
        }
        return false;
    }
    return true;
}

bool ColorParse(const std::string& colorStr, variant<string, int64_t>& colorValue)
{
    if (colorStr.empty()) {
        LOG_ERROR("color string is empty");
        return false;
    }

    if (colorStr[0] != '#') { // start with '#'
        LOG_ERROR("color string not start with #");
        return false;
    }

    const int rgbLen = 7;
    const int argbLen = 9;
    if (colorStr.size() != rgbLen && colorStr.size() != argbLen) {
        LOG_ERROR("color string length is not 7 or 9");
        return false;
    }

    std::string colorStrSub = colorStr.substr(1);
    if (!IsValidHexString(colorStrSub)) {
        LOG_DEBUG("color string is not valid hex string");
        return false;
    }

    LOG_DEBUG("color string size is 7 or 9");
    try {
        colorValue.emplace<1>(std::stoll(colorStrSub, NULL, 16));  // 16 is convert hex string to number
    } catch (std::exception &ex) {
        LOG_ERROR("stoll fail %{public}s", ex.what());
        return false;
    }
    if (std::get_if<1>(&colorValue)) {
        LOG_DEBUG("colorStrSub -> colorValue colorValue:%{public}s", std::to_string(std::get<1>(colorValue)).c_str());
        return true;
    }
    LOG_DEBUG("color is null");
    return false;
}

void SetLocationFieldInfo(std::vector<string>& queryField)
{
    queryField.emplace_back("eventLocation");
    queryField.emplace_back("location_longitude");
    queryField.emplace_back("location_latitude");
}

void SetServiceFieldInfo(std::vector<string>& queryField)
{
    queryField.emplace_back("service_type");
    queryField.emplace_back("service_cp_bz_uri");
    queryField.emplace_back("service_description");
}

void FillFieldInfo(const std::string field, std::vector<string>& queryField, std::set<string>& resultSetField,
    const std::map<string, string> eventField)
{
    if (field == "location") {
            SetLocationFieldInfo(queryField);
            resultSetField.insert(field);
            return;
        }
        if (field == "service") {
            SetServiceFieldInfo(queryField);
            resultSetField.insert(field);
            return;
        }
        if (field == "attendee") {
            resultSetField.insert(field);
            return;
        }
        if (field == "reminderTime") {
            resultSetField.insert(field);
            return;
        }
        if (field == "identifier") {
            queryField.emplace_back("identifier");
            resultSetField.insert(field);
            return;
        }
        if (field == "recurrenceRule") {
            queryField.emplace_back("rrule");
            queryField.emplace_back("exdate");
            resultSetField.insert(field);
            return;
        }
        if (field == "isLunar") {
            queryField.emplace_back("event_calendar_type");
            resultSetField.insert(field);
            return;
        }
        if (field == "instanceStartTime") {
            queryField.emplace_back("begin");
            resultSetField.insert(field);
            return;
        }
        if (field == "instanceEndTime") {
            queryField.emplace_back("end");
            resultSetField.insert(field);
            return;
        }
        queryField.emplace_back(eventField.at(field));
        resultSetField.insert(field);
}

void SetFieldInfo(const std::vector<string>& eventKey, std::vector<string>& queryField,
    std::set<string>& resultSetField, const std::map<string, string> eventField)
{
    for (const auto& field : eventKey) {
        if (field == "id") {
            continue;
        }
        FillFieldInfo(field, queryField, resultSetField, eventField);
    }
}

void SetField(const std::vector<string>& eventKey, std::vector<string>& queryField, std::set<string>& resultSetField)
{
    const std::map<string, string> eventField = { { "id", "_id" },
                                                  { "type", "important_event_type" },
                                                  { "title", "title" },
                                                  { "startTime", "dtstart" },
                                                  { "endTime", "dtend" },
                                                  { "isAllDay", "allDay" },
                                                  { "timeZone", "eventTimezone" },
                                                  { "description", "description" }};
    SetFieldInfo(eventKey, queryField, resultSetField, eventField);
}
}