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

#ifndef CJX509CERTIFICATE_H
#define CJX509CERTIFICATE_H

#include "cf_api.h"
#include "x509_certificate.h"

#include "cj_cert_common.h"
#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfX509Certificate(FUNC, RETURN_TYPE, ...) \
    FFI_DECLARE_API(CjX509Certificate, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjX509Certificate CjX509Certificate;

struct CjX509Certificate {
    HcfX509Certificate *cert;
    CfObject *cfObj;
};

FFI_DECLARE_INSTANCE(CjX509Certificate, const CfEncodingBlob *blob);
CfResult FfiCertCjX509CertificateFromHcfCert(HcfX509Certificate *hcfCert, CjX509Certificate *returnObj);
FFI_DECLARE_HcfX509Certificate(Verify, CfResult, HcfPubKey *pubKey);
FFI_DECLARE_HcfX509Certificate(GetEncoded, CfResult, CfEncodingBlob *encodedByte);
FFI_DECLARE_HcfX509Certificate(GetPublicKey, CfResult, HcfPubKey **retPubKey);
FFI_DECLARE_HcfX509Certificate(CheckValidityWithDate, CfResult, const char *date);
FFI_DECLARE_HcfX509Certificate(GetVersion, long);
FFI_DECLARE_HcfX509Certificate(GetSerialNumber, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetIssuerName, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetSubjectName, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetNotBeforeTime, CfResult, CfBlob *outDate);
FFI_DECLARE_HcfX509Certificate(GetNotAfterTime, CfResult, CfBlob *outDate);
FFI_DECLARE_HcfX509Certificate(GetSignature, CfResult, CfBlob *sigOut);
FFI_DECLARE_HcfX509Certificate(GetSignatureAlgName, CfResult, CfBlob *outName);
FFI_DECLARE_HcfX509Certificate(GetSignatureAlgOid, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetSignatureAlgParams, CfResult, CfBlob *sigAlgParamsOut);
FFI_DECLARE_HcfX509Certificate(GetKeyUsage, CfResult, CfBlob *boolArr);
FFI_DECLARE_HcfX509Certificate(GetExtKeyUsage, CfResult, CfArray *keyUsageOut);
FFI_DECLARE_HcfX509Certificate(GetBasicConstraints, int32_t);
FFI_DECLARE_HcfX509Certificate(GetSubjectAltNames, CfResult, CfArray *outName);
FFI_DECLARE_HcfX509Certificate(GetIssuerAltNames, CfResult, CfArray *outName);
FFI_DECLARE_HcfX509Certificate(Match, CfResult, const CjX509CertMatchParams *matchParams, bool *out);
FFI_DECLARE_HcfX509Certificate(GetCRLDistributionPointsURI, CfResult, CfArray *outURI);
FFI_DECLARE_HcfX509Certificate(ToString, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(HashCode, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetExtensionsObject, CfResult, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetSubjectNameEx, CfResult, const CfEncodinigType encodingType, CfBlob *out);
FFI_DECLARE_HcfX509Certificate(GetItem, CfResult, const int32_t itemType, CfBlob *out);
}

#endif //CJX509CERTIFICATE_H
