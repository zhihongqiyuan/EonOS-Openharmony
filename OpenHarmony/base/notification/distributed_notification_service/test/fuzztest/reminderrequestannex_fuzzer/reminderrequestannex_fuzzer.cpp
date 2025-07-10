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

#include "reminder_request.h"
#include "reminderrequestannex_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        int32_t reminderId = fdp->ConsumeIntegral<int32_t>();
        Notification::ReminderRequest reminderRequest(reminderId);
        uint8_t snoozeTimes = fdp->ConsumeIntegral<uint8_t>();
        reminderRequest.SetSnoozeTimes(snoozeTimes);
        uint8_t snooziTimes = fdp->ConsumeIntegral<uint8_t>();
        reminderRequest.SetSnoozeTimesDynamic(snooziTimes);
        uint64_t timeIntervalInSeconds = fdp->ConsumeIntegral<uint64_t>();
        reminderRequest.SetTimeInterval(timeIntervalInSeconds);
        reminderRequest.SetTitle(stringData);
        reminderRequest.SetTriggerTimeInMilli(timeIntervalInSeconds);
        std::shared_ptr< Notification::ReminderRequest::WantAgentInfo> wantAgentInfo =
            std::make_shared< Notification::ReminderRequest::WantAgentInfo>();
        reminderRequest.SetWantAgentInfo(wantAgentInfo);
        reminderRequest.ShouldShowImmediately();
        reminderRequest.GetActionButtons();
        reminderRequest.GetContent();
        reminderRequest.GetExpiredContent();
        reminderRequest.GetMaxScreenWantAgentInfo();
        reminderRequest.GetNotificationId();
        reminderRequest.GetReminderId();
        reminderRequest.GetReminderTimeInMilli();
        reminderRequest.SetReminderId(reminderId);
        reminderRequest.SetReminderTimeInMilli(timeIntervalInSeconds);
        uint64_t ringDurationInSeconds = fdp->ConsumeIntegral<uint64_t>();
        reminderRequest.SetRingDuration(ringDurationInSeconds);
        reminderRequest.GetSlotType();
        reminderRequest.GetSnoozeContent();
        reminderRequest.GetSnoozeTimes();
        reminderRequest.GetSnoozeTimesDynamic();
        reminderRequest.GetState();
        reminderRequest.GetTimeInterval();
        reminderRequest.GetTriggerTimeInMilli();
        reminderRequest.GetUserId();
        reminderRequest.GetUid();
        reminderRequest.GetWantAgentInfo();
        reminderRequest.GetReminderType();
        reminderRequest.GetRingDuration();
        reminderRequest.UpdateNextReminder();
        reminderRequest.SetNextTriggerTime();
        Parcel parcel;
        reminderRequest.InitServerObj();
        return reminderRequest.IsAlerting();
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
