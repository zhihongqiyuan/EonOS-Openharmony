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

#ifndef CJX500DISTINGUISHEDNAME_H
#define CJX500DISTINGUISHEDNAME_H

#include "x509_distinguished_name.h"

#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfX509DistinguishedName(FUNC, RETURN_TYPE, ...) \
FFI_DECLARE_API(CjX500DistinguishedName, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjX500DistinguishedName CjX500DistinguishedName;

struct CjX500DistinguishedName {
    HcfX509DistinguishedName *name;
};

FFI_DECLARE_INSTANCE(CjX500DistinguishedName, const CfBlob *blob, bool bString);
FFI_DECLARE_HcfX509DistinguishedName(GetEncoded, CfResult, CfEncodingBlob *out);
FFI_DECLARE_HcfX509DistinguishedName(GetName, CfResult, CfBlob *type, CfBlob *out, CfArray *outArr);
}

#endif //CJX500DISTINGUISHEDNAME_H
