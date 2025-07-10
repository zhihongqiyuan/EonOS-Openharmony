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

#include "reminder_request_alarm.h"
#include "reminderrequestalarm_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t HOUR = 24;
        constexpr uint8_t MINUTE = 60;
        constexpr uint8_t WEEK = 7;
        constexpr uint8_t ENABLE = 2;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        uint8_t hour = fdp->ConsumeIntegral<uint8_t>() % HOUR;
        uint8_t minute = fdp->ConsumeIntegral<uint8_t>() % MINUTE;
        uint8_t week = fdp->ConsumeIntegral<uint8_t>() % WEEK;
        std::vector<uint8_t> daysOfWeek;
        daysOfWeek.push_back(week);
        auto rrc = std::make_shared<Notification::ReminderRequestAlarm>(hour, minute, daysOfWeek);
        // test SetRepeatDaysOfWeek function
        bool enabled = fdp->ConsumeBool();
        rrc->SetRepeatDaysOfWeek(enabled, daysOfWeek);
        // test GetDaysOfWeek function
        rrc->GetDaysOfWeek();
        // test CheckParamValid function
        rrc->CheckParamValid();
        // test IsRepeatReminder function
        rrc->IsRepeatReminder();
        // test PreGetNextTriggerTimeIgnoreSnooze function
        rrc->PreGetNextTriggerTimeIgnoreSnooze(enabled, enabled);
        // test GetNextTriggerTime function
        rrc->GetNextTriggerTime(enabled);
        // test GetNextDaysOfWeek function
        time_t now;
        (void)time(&now);  // unit is seconds.
        time_t target = static_cast<time_t>(fdp->ConsumeIntegral<uint8_t>() % ENABLE);
        rrc->GetNextDaysOfWeek(now, target);
        // test GetHour function
        rrc->GetHour();
        // test GetMinute function
        rrc->GetMinute();
        // test GetRepeatDaysOfWeek function
        rrc->GetRepeatDaysOfWeek();
        // test OnDateTimeChange function
        rrc->OnDateTimeChange();
        // test OnTimeZoneChange function
        rrc->OnTimeZoneChange();
        // test UpdateNextReminder function
        rrc->UpdateNextReminder();
        // test Unmarshalling function
        Parcel parcel;
        rrc->Unmarshalling(parcel);
        rrc->ReadFromParcel(parcel);
        return rrc->Marshalling(parcel);
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
