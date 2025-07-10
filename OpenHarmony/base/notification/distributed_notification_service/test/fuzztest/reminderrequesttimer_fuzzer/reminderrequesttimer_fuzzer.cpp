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
#include "reminderrequesttimer_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        uint64_t countDownTimeInSeconds = fdp->ConsumeIntegral<uint64_t>();
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::ReminderRequestTimer reminderRequestTimer(countDownTimeInSeconds);
        reminderRequestTimer.GetInitInfo();
        bool enabled = fdp->ConsumeBool();
        reminderRequestTimer.PreGetNextTriggerTimeIgnoreSnooze(enabled, enabled);
        reminderRequestTimer.OnDateTimeChange();
        reminderRequestTimer.OnTimeZoneChange();
        reminderRequestTimer.UpdateNextReminder();
        reminderRequestTimer.CheckParamsValid(countDownTimeInSeconds);
        reminderRequestTimer.UpdateTimeInfo(stringData);
        Parcel parcel;
        reminderRequestTimer.Unmarshalling(parcel);
        reminderRequestTimer.Marshalling(parcel);
        return reminderRequestTimer.ReadFromParcel(parcel);
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
