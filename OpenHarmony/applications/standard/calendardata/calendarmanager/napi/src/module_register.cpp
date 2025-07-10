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

#include <pthread.h>
#include <unistd.h>

#include "module_init.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"
#include "calendar_log.h"

extern const char _binary_editor_js_start[];
extern const char _binary_editor_js_end[];
extern const char _binary_editor_abc_start[];
extern const char _binary_editor_abc_end[];

namespace OHOS::CalendarApi {
EXTERN_C_START
/*
 * function for module exports
 */
static napi_value Init(napi_env env, napi_value exports)
{
    LOG_INFO("napi_module Init start...");
    exports = ModuleInit(env, exports);
    return exports;
}
EXTERN_C_END

/*
 * Module define
 */
static napi_module _module = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "calendarManager",
    .nm_priv = ((void *)0),
    .reserved = {0},
};

/*
 * Module register function
 */
extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module_register(&_module);
}
}  // namespace OHOS::CalendarApi

extern "C" __attribute__((visibility("default"))) void NAPI_calendarManager_GetJSCode(const char** buf,
    int* bufLen)
{
    if (buf != nullptr) {
        *buf = _binary_editor_js_start;
    }

    if (bufLen != nullptr) {
        *bufLen = _binary_editor_js_end - _binary_editor_js_start;
    }
}

extern "C" __attribute__((visibility("default"))) void NAPI_calendarManager_GetABCCode(const char** buf,
    int* bufLen)
{
    if (buf != nullptr) {
        *buf = _binary_editor_abc_start;
    }

    if (bufLen != nullptr) {
        *bufLen = _binary_editor_abc_end - _binary_editor_abc_start;
    }
}