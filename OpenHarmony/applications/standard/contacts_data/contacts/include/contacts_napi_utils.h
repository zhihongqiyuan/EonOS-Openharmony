/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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

#ifndef CONTACT_NAPI_UTILS_H
#define CONTACT_NAPI_UTILS_H

#include "napi/native_node_api.h"

#include "contacts_napi_common.h"

namespace OHOS {
namespace ContactsApi {
/**
 * contact NAPI utility class.
 */
class ContactsNapiUtils {
public:
    static napi_value ToInt32Value(napi_env env, int32_t value);
    static napi_value CreateClassConstructor(napi_env env, napi_callback_info info);
    static bool MatchValueType(napi_env env, napi_value value, napi_valuetype targetType);
    static bool MatchParameters(
        napi_env env, const napi_value parameters[], std::initializer_list<napi_valuetype> valueTypes);
    static napi_value CreateError(napi_env, int32_t errorCode);
    static napi_value CreateErrorByVerification(napi_env, int32_t errorCode);
};
} // namespace ContactsApi
} // namespace OHOS

#endif