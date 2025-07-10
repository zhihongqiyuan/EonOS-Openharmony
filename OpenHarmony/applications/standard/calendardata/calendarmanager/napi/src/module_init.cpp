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

#include "module_init.h"
#include "calendar_napi.h"
#include "calendar_manager_napi.h"
#include "calendar_enum_napi.h"
#include "event_filter_napi.h"
#include "calendar_log.h"

namespace OHOS::CalendarApi {
napi_value ModuleInit(napi_env env, napi_value exports)
{
    CalendarNapi::Init(env, exports);
    CalendarManagerNapi::Init(env, exports);
    EventFilterNapi::Init(env, exports);
    CalendarEnumNapi::Init(env, exports);
    LOG_INFO("napi_module Init end...");
    return exports;
}
}