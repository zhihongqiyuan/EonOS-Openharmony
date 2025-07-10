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
#include "notification_action_button.h"
#undef private
#undef protected
#include "notificationrequest_fuzzer.h"
#include "notification_request.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t FLAG_STATUS = 11;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider *fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        int32_t notificationId = fdp->ConsumeIntegral<int32_t>();
        Notification::NotificationRequest request(notificationId);
        request.IsInProgress();
        bool enabled = fdp->ConsumeBool();
        request.SetInProgress(enabled);
        request.IsUnremovable();
        request.SetUnremovable(enabled);
        request.GetBadgeNumber();
        request.GetNotificationId();
        std::shared_ptr<AbilityRuntime::WantAgent::WantAgent> wantAgent = nullptr;
        request.SetWantAgent(wantAgent);
        request.GetWantAgent();
        request.SetRemovalWantAgent(wantAgent);
        request.GetRemovalWantAgent();
        request.GetMaxScreenWantAgent();
        std::shared_ptr<AAFwk::WantParams> extras = nullptr;
        request.SetAdditionalData(extras);
        request.GetAdditionalData();
        request.GetDeliveryTime();
        request.IsShowDeliveryTime();
        request.SetShowDeliveryTime(enabled);
        // make NotificationActionButton paramter
        std::shared_ptr<Notification::NotificationActionButton> actionButton =
            std::make_shared<Notification::NotificationActionButton>();
        // make semanticActionButton paramter
        int32_t semanticAction = fdp->ConsumeIntegral<int32_t>() % FLAG_STATUS;
        Notification::NotificationConstant::SemanticActionButton semanticActionButton =
            Notification::NotificationConstant::SemanticActionButton(semanticAction);
        actionButton->SetSemanticActionButton(semanticActionButton);
        actionButton->SetAutoCreatedReplies(enabled);
        actionButton->SetContextDependent(enabled);
        request.AddActionButton(actionButton);
        request.ClearActionButtons();
        request.IsPermitSystemGeneratedContextualActionButtons();
        request.SetPermitSystemGeneratedContextualActionButtons(enabled);
        return request.IsAgentNotification();
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
