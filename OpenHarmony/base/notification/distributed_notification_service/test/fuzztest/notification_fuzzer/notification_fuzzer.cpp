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

#define private public
#define protected public
#include "notification.h"
#undef private
#undef protected
#include "notification_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        sptr<Notification::NotificationRequest> request = new Notification::NotificationRequest();
        if (request != nullptr) {
            request->SetClassification(stringData);
        }
        Notification::Notification notification(stringData, request);
        notification.EnableLight();
        notification.EnableSound();
        notification.EnableVibrate();
        notification.GetBundleName();
        notification.GetCreateBundle();
        notification.GetLabel();
        notification.GetLedLightColor();
        notification.GetLockscreenVisibleness();
        notification.GetGroup();
        notification.GetId();
        notification.GetKey();
        notification.GetNotificationRequest();
        notification.GetPostTime();
        notification.GetSound();
        notification.GetUid();
        notification.GetPid();
        notification.IsUnremovable();
        notification.GetVibrationStyle();
        notification.IsGroup();
        notification.IsFloatingIcon();
        notification.GetRemindType();
        notification.IsRemoveAllowed();
        Parcel parcel;
        return notification.MarshallingBool(parcel);
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
