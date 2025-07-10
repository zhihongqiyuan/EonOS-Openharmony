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

#ifndef CJX509CRL_H
#define CJX509CRL_H

#include "cj_x509_crl_entry.h"
#include "x509_crl.h"

#include "cj_declare_macro.h"
#include "cj_cert_common.h"

#define FFI_DECLARE_HcfX509Crl(FUNC, RETURN_TYPE, ...) \
    FFI_DECLARE_API(CjX509Crl, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjX509Crl CjX509Crl;

struct CjX509Crl {
    HcfX509Crl *crl;
};

FFI_DECLARE_INSTANCE(CjX509Crl, const CfEncodingBlob *blob);
FFI_DECLARE_HcfX509Crl(IsRevoked, bool, const HcfX509Certificate *cert);
FFI_DECLARE_HcfX509Crl(GetType, const char *);
FFI_DECLARE_HcfX509Crl(GetEncoded, CfResult, CfEncodingBlob *encodedOut);
FFI_DECLARE_HcfX509Crl(Verify, CfResult, HcfPubKey *pubKey);
FFI_DECLARE_HcfX509Crl(GetVersion, long);
FFI_DECLARE_HcfX509Crl(GetIssuerName, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetLastUpdate, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetNextUpdate, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetRevokedCert, CfResult, const CfBlob *serialNumber, CjX509CrlEntry *entryOut);
FFI_DECLARE_HcfX509Crl(GetRevokedCertWithCert, CfResult, HcfX509Certificate *cert, CjX509CrlEntry *entryOut);
FFI_DECLARE_HcfX509Crl(GetRevokedCerts, CfResult, CfArray *entrysOut);
FFI_DECLARE_HcfX509Crl(GetTbsInfo, CfResult, CfBlob *tbsCertListOut);
FFI_DECLARE_HcfX509Crl(GetSignature, CfResult, CfBlob *signature);
FFI_DECLARE_HcfX509Crl(GetSignatureAlgName, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetSignatureAlgOid, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetSignatureAlgParams, CfResult, CfBlob *sigAlgParamOut);
FFI_DECLARE_HcfX509Crl(GetExtensions, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(Match, CfResult, const CjX509CrlMatchParams *matchParams, bool *out);
FFI_DECLARE_HcfX509Crl(ToString, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(HashCode, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Crl(GetExtensionsObject, CfResult, CfBlob *out);
}


#endif //CJX509CRL_H
