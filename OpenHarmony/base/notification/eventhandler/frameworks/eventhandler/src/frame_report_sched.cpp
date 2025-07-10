/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

#include "frame_report_sched.h"
 
#include <dlfcn.h>
#include <unistd.h>
 
#include "event_logger.h"
 
namespace OHOS {
namespace AppExecFwk {
namespace {
DEFINE_EH_HILOG_LABEL("FrameReportSched");
const std::string FRAME_AWARE_SO_PATH = "libframe_ui_intf.z.so";
constexpr int RS_UID = 1003;
} // namespace
FrameReport& FrameReport::GetInstance()
{
    static FrameReport instance;
    return instance;
}
 
FrameReport::FrameReport()
{
    LoadLibrary();
    uid_ = getuid();
}
 
FrameReport::~FrameReport()
{
    CloseLibrary();
}
 
void FrameReport::LoadLibrary()
{
    if (!frameSchedSoLoaded_) {
        frameSchedHandle_ = dlopen(FRAME_AWARE_SO_PATH.c_str(), RTLD_LAZY);
        if (frameSchedHandle_ == nullptr) {
            HILOGE("[LoadLibrary] dlopen libframe_ui_intf.so failed!"
                " error = %{public}s\n", dlerror());
            return;
        }
        frameSchedSoLoaded_ = true;
    }
    HILOGD("[LoadLibrary] dlopen libframe_ui_intf.so success");
    reportSchedEventFunc_ = (ReportSchedEventFunc)LoadSymbol("ReportSchedEvent");
    return;
}
 
void FrameReport::CloseLibrary()
{
    if (dlclose(frameSchedHandle_) != 0) {
        HILOGE("[CloseLibrary] libframe_ui_intf.so failed!\n");
        return;
    }
    frameSchedHandle_ = nullptr;
    frameSchedSoLoaded_ = false;
}
 
void* FrameReport::LoadSymbol(const char* symName)
{
    if (frameSchedHandle_ == nullptr) {
        HILOGE("[loadSymbol]libframe_ui_intf.so not loaded.\n");
        return nullptr;
    }
    void *funcSym = dlsym(frameSchedHandle_, symName);
    if (funcSym == nullptr) {
        HILOGE("[loadSymbol] Get %{public}s symbol failed: %{public}s\n", symName, dlerror());
        return nullptr;
    }
    return funcSym;
}
 
void FrameReport::ReportSchedEvent(FrameSchedEvent event, const std::unordered_map<std::string, std::string> &payload)
{
    if (!frameSchedSoLoaded_) {
        HILOGD("[ReportSchedEvent] libframe_ui_intf.so is closed");
        return;
    }
    if (reportSchedEventFunc_ == nullptr) {
        HILOGD("[ReportSchedEvent] reportSchedEventFunc_ is nullptr");
        return;
    }
    if (uid_ == RS_UID) {
        HILOGD("[ReportSchedEvent] uid_ == RS_UID");
        return;
    }
    reportSchedEventFunc_(event, payload);
}
}  // namespace AppExecFwk
}  // namespace OHOS