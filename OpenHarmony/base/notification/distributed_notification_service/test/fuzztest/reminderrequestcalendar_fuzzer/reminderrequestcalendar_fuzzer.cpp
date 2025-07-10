/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

#include "reminder_request_calendar.h"
#include "reminderrequestcalendar_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t MONTHS = 12;
        constexpr uint8_t DAYS = 31;
        constexpr uint16_t YEAR = 365;
        constexpr uint16_t HOURS = 24;
        constexpr uint16_t MINUTES = 60;
        constexpr uint16_t SECONDS = 60;
        constexpr uint8_t WEEK = 7;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        struct tm nowTime;
        uint8_t months = fdp->ConsumeIntegral<uint8_t>() % MONTHS + 1;
        uint8_t days = fdp->ConsumeIntegral<uint8_t>() % DAYS + 1;
        uint8_t weeks = fdp->ConsumeIntegral<uint8_t>() % WEEK + 1;
        std::vector<uint8_t> repeatMonths;
        std::vector<uint8_t> repeatDays;
        std::vector<uint8_t> daysOfWeek;
        daysOfWeek.push_back(weeks);
        repeatMonths.push_back(months);
        repeatDays.push_back(days);
        Notification::ReminderRequestCalendar reminderRequestCalendar(nowTime, repeatMonths, repeatDays, daysOfWeek);
        // test SetNextTriggerTime function
        reminderRequestCalendar.SetNextTriggerTime();
        // test InitDateTime function
        reminderRequestCalendar.InitDateTime();
        // test InitDateTime function
        reminderRequestCalendar.InitDateTime(nowTime);
        // test SetDay function
        bool enabled = fdp->ConsumeBool();
        reminderRequestCalendar.SetDay(days, enabled);
        // test SetMonth function
        reminderRequestCalendar.SetMonth(months, enabled);
        // test SetRepeatDaysOfWeek function
        reminderRequestCalendar.SetRepeatDaysOfWeek(enabled, daysOfWeek);
        // test SetRepeatMonths function
        reminderRequestCalendar.SetRepeatMonths(repeatMonths);
        // test SetRepeatDaysOfMonth function
        reminderRequestCalendar.SetRepeatDaysOfMonth(repeatDays);
        // test GetRepeatMonths function
        reminderRequestCalendar.GetRepeatMonths();
        // test GetRepeatDays function
        reminderRequestCalendar.GetRepeatDays();
        // test GetDaysOfMonth function
        uint16_t year = fdp->ConsumeIntegral<uint16_t>() % YEAR;
        reminderRequestCalendar.GetDaysOfMonth(year, months);
        // test GetNextDay function
        struct tm target;
        reminderRequestCalendar.GetNextDay(year, months, nowTime, target);
        // test GetNextTriggerTime function
        reminderRequestCalendar.GetNextTriggerTime();
        // test GetNextTriggerTimeAsRepeatReminder function
        struct tm tarTime;
        reminderRequestCalendar.GetNextTriggerTimeAsRepeatReminder(nowTime, tarTime);
        // test GetTimeInstantMilli function
        uint8_t hour = fdp->ConsumeIntegral<uint8_t>() % HOURS;
        uint8_t minute = fdp->ConsumeIntegral<uint8_t>() % MINUTES;
        uint8_t second = fdp->ConsumeIntegral<uint8_t>() % SECONDS;
        reminderRequestCalendar.GetTimeInstantMilli(year, months, days, hour, minute, second);
        // test IsRepeatReminder function
        reminderRequestCalendar.IsRepeatReminder();
        // test IsRepeatMonth function
        reminderRequestCalendar.IsRepeatMonth(months);
        // test IsRepeatDay function
        reminderRequestCalendar.IsRepeatDay(days);
        // test OnDateTimeChange function
        reminderRequestCalendar.OnDateTimeChange();
        // test OnTimeZoneChange function
        reminderRequestCalendar.OnTimeZoneChange();
        // test UpdateNextReminder function
        reminderRequestCalendar.UpdateNextReminder();
        // test PreGetNextTriggerTimeIgnoreSnooze function
        reminderRequestCalendar.PreGetNextTriggerTimeIgnoreSnooze(enabled, enabled);
        // test Unmarshalling function
        Parcel parcel;
        reminderRequestCalendar.Unmarshalling(parcel);
        reminderRequestCalendar.ReadFromParcel(parcel);
        return reminderRequestCalendar.Marshalling(parcel);
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    FuzzedDataProvider fdp(data, size);
    OHOS::DoSomethingInterestingWithMyAPI(&fdp);
    return 0;
}
