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
#include "notification_user_input.h"
#undef private
#undef protected
#include "notificationuserinput_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t ENABLE = 2;
        constexpr uint8_t INPUT_EDIT_TYPE = 3;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        Notification::NotificationUserInput notificationUserInput(stringData);
        AAFwk::Want want;
        uint8_t sources = fdp->ConsumeIntegral<uint8_t>() % ENABLE;
        Notification::NotificationConstant::InputsSource source =
            Notification::NotificationConstant::InputsSource(sources);
        notificationUserInput.SetInputsSource(want, source);
        notificationUserInput.GetInputsSource(want);
        std::shared_ptr<Notification::NotificationUserInput> input =
            std::make_shared<Notification::NotificationUserInput>();
        std::vector<std::shared_ptr<Notification::NotificationUserInput>> userInputs;
        userInputs.emplace_back(input);
        AAFwk::WantParams additional;
        notificationUserInput.AddInputsToWant(userInputs, want, additional);
        notificationUserInput.GetInputsFromWant(want);
        notificationUserInput.Create(stringData);
        notificationUserInput.GetInputKey();
        notificationUserInput.AddAdditionalData(additional);
        notificationUserInput.GetAdditionalData();
        uint8_t inputEditTypes = fdp->ConsumeIntegral<uint8_t>() % INPUT_EDIT_TYPE;
        Notification::NotificationConstant::InputEditType inputEditType =
            Notification::NotificationConstant::InputEditType(inputEditTypes);
        notificationUserInput.SetEditType(inputEditType);
        notificationUserInput.GetEditType();
        std::vector<std::string> options;
        options.emplace_back(stringData);
        notificationUserInput.SetOptions(options);
        notificationUserInput.GetOptions();
        bool doPermit = fdp->ConsumeBool();
        notificationUserInput.SetPermitMimeTypes(stringData, doPermit);
        notificationUserInput.GetPermitMimeTypes();
        notificationUserInput.IsMimeTypeOnly();
        notificationUserInput.SetTag(stringData);
        notificationUserInput.GetTag();
        notificationUserInput.SetPermitFreeFormInput(doPermit);
        notificationUserInput.IsPermitFreeFormInput();
        notificationUserInput.Dump();
        Parcel parcel;
        notificationUserInput.ReadFromParcel(parcel);
        return notificationUserInput.Marshalling(parcel);
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
