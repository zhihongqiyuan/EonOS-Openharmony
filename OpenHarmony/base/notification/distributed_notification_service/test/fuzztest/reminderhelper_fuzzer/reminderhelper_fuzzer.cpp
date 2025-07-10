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

#include "reminder_helper.h"
#include "reminderhelper_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::ReminderRequest reminder;
        reminder.SetContent(stringData);
        reminder.SetExpiredContent(stringData);
        int32_t reminderId2 = 0;
        Notification::ReminderHelper::PublishReminder(reminder, reminderId2);
        int32_t reminderId = fdp->ConsumeIntegral<int32_t>();
        Notification::ReminderHelper::CancelReminder(reminderId);
        Notification::ReminderRequestAdaptation valid;
        std::vector<Notification::ReminderRequestAdaptation> validReminders;
        validReminders.emplace_back(valid);
        Notification::ReminderHelper::GetValidReminders(validReminders);
        uint64_t excludeDate = fdp->ConsumeIntegral<uint64_t>();
        Notification::ReminderHelper::AddExcludeDate(reminderId, excludeDate);
        Notification::ReminderHelper::DelExcludeDates(reminderId);
        std::vector<int64_t> dates;
        Notification::ReminderHelper::GetExcludeDates(reminderId, dates);
        return Notification::ReminderHelper::CancelAllReminders();
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
