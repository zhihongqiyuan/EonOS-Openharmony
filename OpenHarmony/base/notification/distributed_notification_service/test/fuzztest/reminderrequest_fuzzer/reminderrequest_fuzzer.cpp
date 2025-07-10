/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include "reminder_request.h"
#include "reminderrequest_fuzzer.h"
#include <fuzzer/FuzzedDataProvider.h>

namespace OHOS {
    namespace {
        constexpr uint8_t ACTION_BUTTON_TYPE = 3;
        constexpr uint8_t SLOT_TYPE_NUM = 5;
    }
    bool DoSomethingInterestingWithMyAPI(FuzzedDataProvider* fdp)
    {
        std::string stringData = fdp->ConsumeRandomLengthString();
        int32_t reminderId = fdp->ConsumeIntegral<int32_t>();
        Notification::ReminderRequest reminderRequest(reminderId);
        reminderRequest.CanRemove();
        reminderRequest.CanShow();
        reminderRequest.Dump();
        uint8_t types = fdp->ConsumeIntegral<uint8_t>() % ACTION_BUTTON_TYPE;
        Notification::ReminderRequest::ActionButtonType type =
            Notification::ReminderRequest::ActionButtonType(types);
        reminderRequest.SetActionButton(stringData, type, stringData);
        reminderRequest.SetContent(stringData);
        reminderRequest.SetExpiredContent(stringData);
        bool enabled = fdp->ConsumeBool();
        reminderRequest.SetExpired(enabled);
        reminderRequest.InitReminderId();
        reminderRequest.InitUserId(reminderId);
        reminderRequest.InitUid(reminderId);
        reminderRequest.IsExpired();
        reminderRequest.IsShowing();
        reminderRequest.OnClose(enabled);
        reminderRequest.OnDateTimeChange();
        uint64_t oriTriggerTime = fdp->ConsumeIntegral<uint64_t>();
        uint64_t optTriggerTimes = fdp->ConsumeIntegral<uint64_t>();
        reminderRequest.HandleSysTimeChange(oriTriggerTime, optTriggerTimes);
        uint64_t oldZoneTriggerTime = fdp->ConsumeIntegral<uint64_t>();
        uint64_t newZoneTriggerTime = fdp->ConsumeIntegral<uint64_t>();
        uint64_t optTriggerTime = fdp->ConsumeIntegral<uint64_t>();
        reminderRequest.HandleTimeZoneChange(oldZoneTriggerTime, newZoneTriggerTime, optTriggerTime);
        reminderRequest.OnSameNotificationIdCovered();
        reminderRequest.OnShow(enabled, enabled, enabled);
        reminderRequest.OnShowFail();
        reminderRequest.OnSnooze();
        reminderRequest.OnStart();
        reminderRequest.OnStop();
        reminderRequest.OnTerminate();
        reminderRequest.OnTimeZoneChange();
        reminderRequest.StringSplit(stringData, stringData);
        std::shared_ptr< Notification::ReminderRequest::MaxScreenAgentInfo> maxScreenWantAgentInfo =
            std::make_shared< Notification::ReminderRequest::MaxScreenAgentInfo>();
        reminderRequest.SetMaxScreenWantAgentInfo(maxScreenWantAgentInfo);
        reminderRequest.SetNotificationId(reminderId);
        uint8_t typed = fdp->ConsumeIntegral<uint8_t>() % SLOT_TYPE_NUM;
        Notification::NotificationConstant::SlotType slotType =
            Notification::NotificationConstant::SlotType(typed);
        reminderRequest.SetSlotType(slotType);
        reminderRequest.SetSnoozeContent(stringData);
        return reminderRequest.ShouldShowImmediately();
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
