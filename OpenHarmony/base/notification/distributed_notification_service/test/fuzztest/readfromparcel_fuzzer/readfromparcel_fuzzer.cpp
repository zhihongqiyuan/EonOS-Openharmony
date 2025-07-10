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

#include <cstdint>
#define private public
#define protected public
#include "notification.h"
#undef private
#undef protected
#include "readfromparcel_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t SOURCE_TYPE = 3;
        constexpr uint8_t SLOT_VISIBLENESS_TYPE_NUM = 4;
        constexpr uint8_t SLOT_TYPE_NUM = 5;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        sptr<Notification::NotificationRequest> request = new Notification::NotificationRequest();
        if (request != nullptr) {
            request->SetClassification(stringData);
        }
        Notification::Notification notification(request);
        Parcel parcel;
        notification.MarshallingString(parcel);
        notification.MarshallingInt32(parcel);
        notification.MarshallingInt64(parcel);
        notification.MarshallingParcelable(parcel);
        notification.Marshalling(parcel);
        notification.ReadFromParcelBool(parcel);
        notification.ReadFromParcelString(parcel);
        notification.ReadFromParcelInt32(parcel);
        notification.ReadFromParcelInt64(parcel);
        notification.ReadFromParcelParcelable(parcel);
        notification.Unmarshalling(parcel);
        bool enabled = fdp->ConsumeBool();
        notification.SetEnableSound(enabled);
        notification.SetEnableLight(enabled);
        notification.SetEnableVibration(enabled);
        int32_t color = fdp->ConsumeIntegral<int32_t>();
        notification.SetLedLightColor(color);
        uint8_t visibleness = fdp->ConsumeIntegral<uint8_t>() % SLOT_VISIBLENESS_TYPE_NUM;
        Notification::NotificationConstant::VisiblenessType visiblenessType =
            Notification::NotificationConstant::VisiblenessType(visibleness);
        notification.SetLockScreenVisbleness(visiblenessType);
        int64_t time = 2;
        notification.SetPostTime(time);
        std::vector<int64_t> style;
        style.emplace_back(time);
        notification.SetVibrationStyle(style);
        int32_t remindType = static_cast<int32_t>(fdp->ConsumeIntegral<uint8_t>() % SLOT_TYPE_NUM);
        Notification::NotificationConstant::RemindType remind =
            Notification::NotificationConstant::RemindType(remindType);
        notification.SetRemindType(remind);
        notification.SetRemoveAllowed(enabled);
        int32_t source = static_cast<int32_t>(fdp->ConsumeIntegral<uint8_t>() % SOURCE_TYPE);
        Notification::NotificationConstant::SourceType sourceType =
            Notification::NotificationConstant::SourceType(source);
        notification.SetSourceType(sourceType);
        notification.Dump();
        return notification.ReadFromParcel(parcel);
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
