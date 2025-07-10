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

#ifndef CALENDAR_MANAGER_NAPI_H
#define CALENDAR_MANAGER_NAPI_H

#include <vector>

#include "napi/native_api.h"
#include "napi/native_common.h"
#include "napi/native_node_api.h"

#include "calendar_log.h"
#include "napi_util.h"
#include "napi_queue.h"
#include "calendar_napi.h"
#include "native_calendar_manager.h"
#include "napi_env.h"
#include "abs_shared_result_set.h"
#include "data_ability_helper.h"
#include "data_ability_predicates.h"
#include "values_bucket.h"

#include <ui_content.h>
#include "napi_base_context.h"
namespace OHOS::CalendarApi {
class CalendarManagerNapi {
public:
    struct EditEventContext : public ContextBase {
        string event;
        napi_value _jsContext;
        string caller;
        int32_t _sessionId = 0;
        Ace::UIContent *_uiContent = nullptr;
        napi_value id;
    };
    
    static napi_value New(napi_env env, napi_callback_info info);
    static napi_value CreateCalendar(napi_env env, napi_callback_info info);
    static napi_value DeleteCalendar(napi_env env, napi_callback_info info);
    static napi_value GetCalendar(napi_env env, napi_callback_info info);
    static napi_value GetAllCalendars(napi_env env, napi_callback_info info);
    static napi_value EditEvent(napi_env env, napi_callback_info info);
    static napi_value Init(napi_env env, napi_value exports);
private:
    static napi_value LaunchEditorPage(napi_env env, std::shared_ptr<EditEventContext> ctxt);
};

napi_value GetCalendarManager(napi_env env, napi_callback_info info);

}  // namespace Calendar::CalendarApi
#endif  //  CALENDAR_MANAGER_NAPI_H
