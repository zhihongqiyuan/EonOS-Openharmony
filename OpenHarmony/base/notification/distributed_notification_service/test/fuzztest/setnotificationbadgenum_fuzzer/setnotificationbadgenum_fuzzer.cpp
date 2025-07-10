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

#include "setnotificationbadgenum_fuzzer.h"

#include "ans_dialog_host_client.h"
#include "notification_helper.h"
#include <fuzzer/FuzzedDataProvider.h>
namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fdp)
    {
        // test RequestEnableNotification function
        std::string deviceId = fdp->ConsumeRandomLengthString();
        sptr<IRemoteObject> callerToken = nullptr;
        sptr<Notification::AnsDialogHostClient> client = nullptr;
        Notification::AnsDialogHostClient::CreateIfNullptr(client);
        client = Notification::AnsDialogHostClient::GetInstance();
        Notification::NotificationHelper::RequestEnableNotification(deviceId, client, callerToken);
        // test HasNotificationPolicyAccessPermission function
        bool hasPermission = true;
        Notification::NotificationHelper::HasNotificationPolicyAccessPermission(hasPermission);
        // test SetNotificationBadgeNum function
        return Notification::NotificationHelper::SetNotificationBadgeNum(fdp->ConsumeIntegral<int32_t>()) == ERR_OK;
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
