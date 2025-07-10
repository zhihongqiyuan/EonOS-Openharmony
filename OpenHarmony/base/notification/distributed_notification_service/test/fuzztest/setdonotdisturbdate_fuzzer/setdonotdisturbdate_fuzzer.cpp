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

#include "setdonotdisturbdate_fuzzer.h"

#include "notification_helper.h"
#include <fuzzer/FuzzedDataProvider.h>
namespace OHOS {
    namespace {
        constexpr uint8_t SLOT_TYPE_NUM = 5;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fdp)
    {
        int32_t userId = fdp->ConsumeIntegral<int32_t>();
        uint32_t type = fdp->ConsumeIntegral<uint32_t>();
        Notification::NotificationDoNotDisturbDate disturb;
        Notification::NotificationConstant::DoNotDisturbType disturbType =
            Notification::NotificationConstant::DoNotDisturbType(type);
        disturb.SetDoNotDisturbType(disturbType);
        // test SetDoNotDisturbDate function
        Notification::NotificationHelper::SetDoNotDisturbDate(userId, disturb);
        // test GetDoNotDisturbDate function
        Notification::NotificationHelper::GetDoNotDisturbDate(userId, disturb);
        // test SetEnabledForBundleSlot function
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::NotificationBundleOption bundleOption;
        bundleOption.SetBundleName(stringData);
        bundleOption.SetUid(userId);
        uint8_t types = fdp->ConsumeIntegral<uint8_t>() % SLOT_TYPE_NUM;
        Notification::NotificationConstant::SlotType slotType = Notification::NotificationConstant::SlotType(types);
        bool enabled = fdp->ConsumeBool();
        Notification::NotificationHelper::SetEnabledForBundleSlot(bundleOption, slotType, enabled, false);
        // test GetEnabledForBundleSlot function
        Notification::NotificationHelper::GetEnabledForBundleSlot(bundleOption, slotType, enabled);
        // test SetSyncNotificationEnabledWithoutApp function
        Notification::NotificationHelper::SetSyncNotificationEnabledWithoutApp(userId, enabled);
        // test GetSyncNotificationEnabledWithoutApp function
        Notification::NotificationHelper::GetSyncNotificationEnabledWithoutApp(userId, enabled);
        // test RemoveNotifications function
        return Notification::NotificationHelper::RemoveNotifications(userId);
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
