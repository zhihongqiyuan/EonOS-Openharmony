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

#ifndef CJX509CERTCHAIN_H
#define CJX509CERTCHAIN_H

#include "x509_cert_chain.h"

#include "cj_cert_common.h"
#include "cj_declare_macro.h"

#define FFI_DECLARE_HcfCertChain(FUNC, RETURN_TYPE, ...) \
FFI_DECLARE_API(CjX509CertChain, FUNC, RETURN_TYPE, ##__VA_ARGS__)

extern "C" {
typedef struct CjX509CertChain CjX509CertChain;

struct CjX509CertChain {
    HcfCertChain *chain;
};

typedef struct CjX509CertChainValidateParams CjX509CertChainValidateParams;

typedef struct CjX509TrustAnchor CjX509TrustAnchor;

typedef struct CjRevocationCheckParam CjRevocationCheckParam;

struct CjX509TrustAnchor {
    CfBlob *CAPubKey;
    HcfX509Certificate *CACert;
    CfBlob *CASubject;
    CfBlob *nameConstraints;
};


struct CjRevocationCheckParam {
    CfBlobArray *ocspRequestExtension;
    CfBlob *ocspResponderURI;
    HcfX509Certificate *ocspResponderCert;
    CfBlob *ocspResponses;
    CfBlob *crlDownloadURI;
    HcfRevChkOption *options;
    uint32_t optionCnt;
    CfBlob *ocspDigest;
};

struct CjX509CertChainValidateParams {
    CjX509TrustAnchor *trustAnchors;
    uint32_t trustAnchorCnt;
    CfBlob *date;
    HcfCertCrlCollection **certCRLCollections;
    uint32_t certCRLCollectionCnt;
    CjRevocationCheckParam *revocationCheckParam;
    HcfValPolicyType policy;
    CfBlob *sslHostname;
    HcfKeyUsageType *keyUsage;
    uint32_t keyUsageCnt;
};

typedef struct CjX509CertChainValidateResult CjX509CertChainValidateResult;

struct CjX509CertChainValidateResult {
    CjX509TrustAnchor trustAnchor;
    HcfX509Certificate *entityCert;
};

int32_t FfiCertCjX509CertChainNewInstanceBlob(const CfEncodingBlob *blob, CjX509CertChain *returnObj);
int32_t FfiCertCjX509CertChainNewInstanceArray(const HcfX509CertificateArray *inCerts, CjX509CertChain *returnObj);
void FfiCertCjX509CertChainDeleteInstance(CjX509CertChain self);
FFI_DECLARE_HcfCertChain(GetCertList, CfResult, HcfX509CertificateArray *out);
FFI_DECLARE_HcfCertChain(Validate, CfResult, const CjX509CertChainValidateParams *params,
                         CjX509CertChainValidateResult *result);
FFI_DECLARE_HcfCertChain(ToString, CfResult, CfBlob *out);
FFI_DECLARE_HcfCertChain(HashCode, CfResult, CfBlob *out);


CfResult FfiCertBuildX509CertChain(const CjX509CertMatchParams &matchParams,
                                   const CjX509CertChainValidateParams &validParams,
                                   int32_t maxLength, CjX509CertChain *returnObj);

typedef struct {
    CjX509TrustAnchor **data;
    uint32_t count;
} CjX509TrustAnchorArray;

CfResult FfiCertCreateTrustAnchorWithKeyStore(const CfBlob *keyStore, const CfBlob *pwd,
                                              CjX509TrustAnchorArray *returnObj);
}

#endif //CJX509CERTCHAIN_H
