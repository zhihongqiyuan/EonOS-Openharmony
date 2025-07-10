/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include <napi/native_api.h>

#include "contacts_api.h"

extern const char _binary_contact_js_start[];
extern const char _binary_contact_js_end[];
extern const char _binary_contact_abc_start[];
extern const char _binary_contact_abc_end[];

namespace OHOS {
namespace ContactsApi {
static napi_value ModuleInit(napi_env env, napi_value exports)
{
    OHOS::ContactsApi::Init(env, exports);
    return exports;
}

extern "C" __attribute__((constructor)) void RegisterModule(void)
{
    napi_module module = {
        .nm_version = 1, // NAPI v1
        .nm_flags = 0, // normal
        .nm_filename = nullptr,
        .nm_register_func = ModuleInit,
        .nm_modname = "contact",
        .nm_priv = nullptr,
        .reserved = {}
    };
    napi_module_register(&module);
}

extern "C" __attribute__((visibility("default"))) void NAPI_contact_GetJSCode(const char** buf,
    int* bufLen)
{
    if (buf != nullptr) {
        *buf = _binary_contact_js_start;
    }

    if (bufLen != nullptr) {
        *bufLen = _binary_contact_js_end - _binary_contact_js_start;
    }
}

extern "C" __attribute__((visibility("default"))) void NAPI_contact_GetABCCode(const char** buf,
    int* bufLen)
{
    if (buf != nullptr) {
        *buf = _binary_contact_abc_start;
    }

    if (bufLen != nullptr) {
        *bufLen = _binary_contact_abc_end - _binary_contact_abc_start;
    }
}
} // namespace ContactsApi
} // namespace OHOS