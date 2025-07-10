
/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_FRAME_REPORT_SCHED_H
#define BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_FRAME_REPORT_SCHED_H

#include <string>
#include <unordered_map>
 
namespace OHOS {
namespace AppExecFwk {
enum class FrameSchedEvent {
    SCHED_EVENT_BASE = 0,
    INIT = 1,
    UI_FLUSH_BEGIN = 20001,
    UI_EVENT_HANDLE_BEGIN = 20005,
    SCHED_EVENT_MAX,
};
 
using ReportSchedEventFunc = void (*)(FrameSchedEvent, const std::unordered_map<std::string, std::string>&);
 
class FrameReport final {
public:
    static FrameReport& GetInstance();
    void ReportSchedEvent(FrameSchedEvent event, const std::unordered_map<std::string, std::string>& payload);
    
private:
    FrameReport();
    ~FrameReport();
    void LoadLibrary();
    void CloseLibrary();
    void* LoadSymbol(const char* symName);
 
    void* frameSchedHandle_ = nullptr;
    bool frameSchedSoLoaded_ = false;
    int uid_ = 0;
    ReportSchedEventFunc reportSchedEventFunc_ = nullptr;
};
}  // namespace AppExecFwk
}  // namespace OHOS
 
#endif  // #ifndef BASE_EVENTHANDLER_FRAMEWORKS_EVENTHANDLER_INCLUDE_FRAME_REPORT_SCHED_H