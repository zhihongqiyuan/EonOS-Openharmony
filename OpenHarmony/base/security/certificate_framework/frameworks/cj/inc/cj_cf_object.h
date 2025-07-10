/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CJCFOBJECT_H
#define CJCFOBJECT_H

#include "cf_api.h"
#include "cf_result.h"

#include "cj_declare_macro.h"

#define FFI_DECLARE_CfObject(FUNC, RETURN_TYPE, ...) \
    FFI_DECLARE_API(CjCfObject, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjCfObject CjCfObject;

struct CjCfObject {
    CfObject *cfObj;
};

FFI_DECLARE_INSTANCE(CjCfObject, const CfEncodingBlob *blob);
FFI_DECLARE_CfObject(GetEncoded, CfResult, CfBlob *out);
FFI_DECLARE_CfObject(GetOidList, CfResult, int32_t valueType, CfArray *out);
FFI_DECLARE_CfObject(GetEntry, CfResult, int32_t valueType, CfBlob *oid, CfBlob *out);
FFI_DECLARE_CfObject(CheckCA, CfResult, int32_t *out);
FFI_DECLARE_CfObject(HasUnsupportedCriticalExtension, CfResult, bool *out);
}

#endif //CJCFOBJECT_H
