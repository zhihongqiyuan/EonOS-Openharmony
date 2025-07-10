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

#include "reminder_request_timer.h"
#include "reminder_datashare_helper.h"
#include "reminderdatasharehelper_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        int32_t id = fdp->ConsumeIntegral<int32_t>();
        std::string bundleName = fdp->ConsumeRandomLengthString();
        constexpr uint64_t seconds = 1200;
        sptr<Notification::ReminderRequest> reminder = new Notification::ReminderRequestTimer(seconds);

        Notification::ReminderDataShareHelper& helper = Notification::ReminderDataShareHelper::GetInstance();
        helper.RegisterObserver();
        helper.UnRegisterObserver();
        std::map<std::string, sptr<Notification::ReminderRequest>> reminders;
        helper.Query(reminders);
        helper.Update(id, id);
        helper.StartDataExtension(id);
        helper.SetUserId(id);
        helper.UpdateCalendarUid();
        helper.GetCacheReminders();
        helper.InsertCacheReminders(reminders);
        helper.OnDataInsertOrDelete();
        DataShare::DataShareObserver::ChangeInfo info;
        helper.OnDataUpdate(info);
        auto result = helper.CreateDataShareHelper();
        helper.ReleaseDataShareHelper(result);
        helper.GetColumns();
        helper.CreateReminder(info);
        helper.InitNormalInfo(reminder);
        return true;
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
