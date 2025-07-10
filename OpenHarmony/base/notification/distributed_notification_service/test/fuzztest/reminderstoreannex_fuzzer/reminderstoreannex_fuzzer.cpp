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
#include "reminderstoreannex_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        Notification::ReminderStore reminderStore;
        // test GetReminders function
        std::string queryCondition = fdp->ConsumeRandomLengthString();
        reminderStore.GetReminders(queryCondition);
        // test GetAllValidReminders function
        reminderStore.GetAllValidReminders();
        // test Query function
        reminderStore.Query(queryCondition);
        // test GetInt32Val function
        std::shared_ptr<NativeRdb::ResultSet> resultSet = std::make_shared<NativeRdb::AbsSharedResultSet>();
        std::string name = fdp->ConsumeRandomLengthString();
        int32_t value = fdp->ConsumeIntegral<int32_t>();
        reminderStore.GetInt32Val(resultSet, name, value);
        std::string value1 = fdp->ConsumeRandomLengthString();
        reminderStore.GetStringVal(resultSet, name, value1);
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
