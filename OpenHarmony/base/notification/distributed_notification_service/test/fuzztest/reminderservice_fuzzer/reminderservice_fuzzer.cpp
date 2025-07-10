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

#include <cstdint>
#include <fuzzer/FuzzedDataProvider.h>
#include <memory>
#include <string>
#include "reminder_agent_service.h"
#include "reminderservice_fuzzer.h"
#include "ans_permission_def.h"

constexpr uint8_t SLOT_TYPE_NUM = 5;

namespace OHOS {

    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fuzzData)
    {
        auto service = std::make_shared<Notification::ReminderAgentService>();
        std::string stringData = fuzzData->ConsumeRandomLengthString();
        int32_t userId = fuzzData->ConsumeIntegral<int32_t>();
        Notification::ReminderRequest reminder;
        int32_t reminderId2 = 0;
        service->PublishReminder(reminder, reminderId2);
        int32_t reminderId = fuzzData->ConsumeIntegral<int32_t>();
        service->CancelReminder(reminderId);
        std::vector<Notification::ReminderRequestAdaptation> reminders;
        service->GetValidReminders(reminders);
        service->CancelAllReminders();
        int64_t excludeDate = fuzzData->ConsumeIntegral<int64_t>();
        service->AddExcludeDate(reminderId, excludeDate);
        service->DelExcludeDates(reminderId);
        std::vector<int64_t> excludeDates;
        service->GetExcludeDates(reminderId, excludeDates);
        return true;
    }
}

/* Fuzzer entry point */
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    /* Run your code on data */
    FuzzedDataProvider fdp(data, size);
    std::vector<std::string> requestPermission = {
        OHOS::Notification::OHOS_PERMISSION_NOTIFICATION_CONTROLLER,
        OHOS::Notification::OHOS_PERMISSION_NOTIFICATION_AGENT_CONTROLLER,
        OHOS::Notification::OHOS_PERMISSION_SET_UNREMOVABLE_NOTIFICATION
    };
    SystemHapTokenGet(requestPermission);
    OHOS::DoSomethingInterestingWithMyAPI(&fdp);
    return 0;
}
