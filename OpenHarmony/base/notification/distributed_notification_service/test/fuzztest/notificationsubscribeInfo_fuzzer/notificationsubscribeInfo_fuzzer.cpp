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

#include "notification_subscribe_info.h"
#include "notificationsubscribeInfo_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::NotificationSubscribeInfo sotificationSubscribeInfo;
        sotificationSubscribeInfo.AddAppName(stringData);
        std::vector<std::string> appNames;
        appNames.emplace_back(stringData);
        sotificationSubscribeInfo.AddAppNames(appNames);
        sotificationSubscribeInfo.GetAppNames();
        int32_t userId = fdp->ConsumeIntegral<int32_t>();
        sotificationSubscribeInfo.AddAppUserId(userId);
        sotificationSubscribeInfo.GetAppUserId();
        sotificationSubscribeInfo.Dump();
        Parcel parcel;
        return sotificationSubscribeInfo.Marshalling(parcel);
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
