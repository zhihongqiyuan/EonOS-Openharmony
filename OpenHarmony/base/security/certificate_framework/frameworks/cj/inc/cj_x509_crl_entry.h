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

#ifndef CJX509CRLENTRY_H
#define CJX509CRLENTRY_H

#include "x509_crl_entry.h"

#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfX509CrlEntry(FUNC, RETURN_TYPE, ...) \
FFI_DECLARE_API(CjX509CrlEntry, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjX509CrlEntry CjX509CrlEntry;

struct CjX509CrlEntry {
    HcfX509CrlEntry *crlEntry;
};

FFI_DECLARE_INSTANCE(CjX509CrlEntry, HcfX509CrlEntry *crlEntry);
FFI_DECLARE_HcfX509CrlEntry(GetEncoded, CfResult, CfEncodingBlob *encodedOut);
FFI_DECLARE_HcfX509CrlEntry(GetSerialNumber, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509CrlEntry(GetCertIssuer, CfResult, CfBlob *encodedOut);
FFI_DECLARE_HcfX509CrlEntry(GetRevocationDate, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509CrlEntry(GetExtensions, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509CrlEntry(HasExtensions, CfResult, bool *out);
FFI_DECLARE_HcfX509CrlEntry(ToString, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509CrlEntry(HashCode, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509CrlEntry(GetExtensionsObject, CfResult, CfBlob *out);
}

#endif //CJX509CRLENTRY_H
