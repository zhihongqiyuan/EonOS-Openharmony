/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#include <VirtualScreen.h>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <new>
#include "CommandLineFactory.h"
#include "CommandLineInterface.h"
#include "CommandParser.h"
#include "CppTimerManager.h"
#include "CrashHandler.h"
#include "Interrupter.h"
#include "JsAppImpl.h"
#include "ModelManager.h"
#include "PreviewerEngineLog.h"
#include "SharedData.h"
#include "TimerTaskHandler.h"
#include "TraceTool.h"
#include "VirtualScreenImpl.h"
#include "jsi.h"

static const int NOTIFY_INTERVAL_TIME = 1000; // Unit millisecond

static void InitSettings()
{
    // Setting the Simulator Model
    ModelManager::SetCurrentDevice(CommandParser::GetInstance().GetDeviceType());
    VirtualScreenImpl::GetInstance().InitResolution();
    VirtualScreenImpl::GetInstance().InitFrameCountTimer();
}

static void ApplyConfig()
{
    std::string liteConfigArgs = CommandParser::GetInstance().GetConfigPath();
    if (liteConfigArgs.empty()) {
        ELOG("No persistent properties path found.");
    }
    CommandLineInterface::GetInstance().ReadAndApplyConfig(liteConfigArgs);
}

static void DataChangeCheck()
{
    SharedDataManager::CheckTick();
}

static void InitSharedData()
{
    // The brightness ranges from 1 to 255. The default value is 255.
    SharedData<uint8_t>(SharedDataType::BRIGHTNESS_VALUE, 255, 1, 255);
    SharedData<uint8_t>(SharedDataType::BRIGHTNESS_MODE, (uint8_t)BrightnessMode::MANUAL,
                        (uint8_t)BrightnessMode::MANUAL, (uint8_t)BrightnessMode::AUTO);
    SharedData<bool>(SharedDataType::KEEP_SCREEN_ON, true);
    SharedData<uint8_t>(SharedDataType::BATTERY_STATUS, (uint8_t)ChargeState::NOCHARGE,
                        (uint8_t)ChargeState::NOCHARGE, (uint8_t)ChargeState::CHARGING);
    // Battery level range: 0.0–1.0; default: 1.0
    SharedData<double>(SharedDataType::BATTERY_LEVEL, 1.0, 0.0, 1.0);
    // Heart rate range: 0 to 255. The default value is 80.
    SharedData<uint8_t>(SharedDataType::HEARTBEAT_VALUE, 80, 0, 255);
    // The value ranges from 0 to 999999. The default value is 0.
    SharedData<uint32_t>(SharedDataType::SUMSTEP_VALUE, 0, 0, 999999);
    // The volume ranges from 0.0 to 1.0. The default value is 1.0.
    SharedData<double>(SharedDataType::VOLUME_VALUE, 1.0, 0.0, 1.0);
    // The atmospheric pressure ranges from 0 to 999900. The default value is 101325.
    SharedData<uint32_t>(SharedDataType::PRESSURE_VALUE, 101325, 0, 999900);
    SharedData<bool>(SharedDataType::WEARING_STATE, true);
    SharedData<std::string>(SharedDataType::LANGUAGE, "zh-CN");
    // The value ranges from 180 to 180. The default value is 0.
    SharedData<double>(SharedDataType::LONGITUDE, 0, -180, 180);
    // The value ranges from -90 to 90. The default value is 0.
    SharedData<double>(SharedDataType::LATITUDE, 0, -90, 90);
}

static void NewHandler()
{
    ELOG("Custom new handler: memory allocation failed.");
}

static void SendJsHeapData()
{
    OHOS::ACELite::JSHeapStatus status;
    OHOS::ACELite::JSI::GetJSHeapStatus(status);
    CommandLineInterface::GetInstance().SendJSHeapMemory(status.totalBytes, status.allocBytes, status.peakAllocBytes);
}

int main(int argc, char* argv[])
{
    ILOG("ThinPreviewer enter the main function.");
    std::set_new_handler(NewHandler);
    // thin device global exception handler
    auto thinCrashHandler = std::make_unique<CrashHandler>();
    thinCrashHandler->InitExceptionHandler();
    // Creating a Main Thread Timer Manager
    CppTimerManager& manager = CppTimerManager::GetTimerManager();
    // Parsing User Commands
    CommandParser& parser = CommandParser::GetInstance();
    int ret = parser.ParseArgs(argc, argv);
    if (ret >= 0) {
        return ret;
    }
    InitSharedData();
    InitSettings();
    if (parser.IsSet("s")) {
        CommandLineInterface::GetInstance().Init(parser.Value("s"));
    }
    ApplyConfig();
    JsAppImpl::GetInstance().InitJsApp();
    TraceTool::GetInstance().HandleTrace("Enter the main function");
    CppTimer dataCheckTimer(DataChangeCheck);
    manager.AddCppTimer(dataCheckTimer);
    dataCheckTimer.Start(100); // 100ms Timer polling period
    CppTimer jsHeapSendTimer(SendJsHeapData);
    if (parser.IsSendJSHeap()) {
        manager.AddCppTimer(jsHeapSendTimer);
        jsHeapSendTimer.Start(NOTIFY_INTERVAL_TIME); // 1000ms Timer polling period
    }
    // Registering and monitoring the changes of the brightness and volume
    std::thread::id curThreadId = std::this_thread::get_id();
    SharedData<uint8_t>::AppendNotify(SharedDataType::BRIGHTNESS_VALUE,
                                      TimerTaskHandler::CheckBrightnessValueChanged, curThreadId);
    while (!Interrupter::IsInterrupt()) {
        CommandLineInterface::GetInstance().ProcessCommand();
        manager.RunTimerTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    JsAppImpl::GetInstance().Stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // sleep 500 ms
    return 0;
}
