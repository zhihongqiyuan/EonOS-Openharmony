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

#include "reminder_store.h"
#include "reminderstore_fuzzer.h"
#include "abs_shared_result_set.h"
#include "reminder_request_alarm.h"
#include "reminder_request_timer.h"
#include "reminder_store_strategy.h"
#include "reminder_request_calendar.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::ReminderStore reminderStore;
        int32_t oldVersion = fdp->ConsumeIntegral<int32_t>();
        int32_t id = fdp->ConsumeIntegral<int32_t>();
        bool value = fdp->ConsumeBool();
        reminderStore.Init();
        reminderStore.InitData();
        reminderStore.Delete(oldVersion);
        reminderStore.DeleteUser(oldVersion);
        reminderStore.Delete(stringData, oldVersion, oldVersion);
        reminderStore.DeleteBase(stringData);
        sptr<Notification::ReminderRequest> timer = new Notification::ReminderRequestTimer(id);
        sptr<Notification::ReminderRequest> alarm = new Notification::ReminderRequestAlarm(id);
        sptr<Notification::ReminderRequest> calendar = new Notification::ReminderRequestCalendar(id);
        NativeRdb::ValuesBucket bucket;
        Notification::ReminderStrategy::AppendValuesBucket(timer, bucket, value);
        std::shared_ptr<NativeRdb::ResultSet> result = std::make_shared<NativeRdb::AbsSharedResultSet>();
        std::shared_ptr<NativeRdb::ResultSet> baseResult = std::make_shared<NativeRdb::AbsSharedResultSet>();
        Notification::ReminderStrategy::RecoverFromOldVersion(timer, result);
        Notification::ReminderStrategy::RecoverFromDb(timer, result);
        Notification::ReminderStrategy::RecoverTimeFromOldVersion(timer, result);
        Notification::ReminderStrategy::RecoverIdFromOldVersion(timer, result);
        Notification::ReminderStrategy::RecoverContextFromOldVersion(timer, result);
        Notification::ReminderStrategy::RecoverTimeFromDb(timer, result);
        Notification::ReminderStrategy::RecoverIdFromDb(timer, result);
        Notification::ReminderStrategy::RecoverContextFromDb(timer, result);
        Notification::ReminderTimerStrategy::AppendValuesBucket(timer, bucket);
        Notification::ReminderTimerStrategy::RecoverFromOldVersion(timer, result);
        Notification::ReminderTimerStrategy::RecoverFromDb(timer, baseResult, result);
        Notification::ReminderAlarmStrategy::AppendValuesBucket(alarm, bucket);
        Notification::ReminderAlarmStrategy::RecoverFromOldVersion(alarm, result);
        Notification::ReminderAlarmStrategy::RecoverFromDb(alarm, baseResult, result);
        Notification::ReminderCalendarStrategy::AppendValuesBucket(calendar, bucket);
        Notification::ReminderCalendarStrategy::RecoverFromOldVersion(calendar, result);
        Notification::ReminderCalendarStrategy::RecoverFromDb(calendar, baseResult, result);
        Notification::ReminderCalendarStrategy::RecoverTime(calendar, result);
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
