/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

#ifndef CF_X509_CERT_CHAIN_H
#define CF_X509_CERT_CHAIN_H

#include <stddef.h>
#include <stdint.h>

#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"
#include "x509_cert_chain_validate_params.h"
#include "x509_cert_chain_validate_result.h"
#include "x509_certificate.h"

typedef struct HcfCertChain HcfCertChain;
struct HcfCertChain {
    struct CfObjectBase base;

    /** Get cert array. */
    CfResult (*getCertList)(HcfCertChain *self, HcfX509CertificateArray *out);

    /** Validate Cert Chain. */
    CfResult (*validate)(
        HcfCertChain *self, const HcfX509CertChainValidateParams *params, HcfX509CertChainValidateResult *result);

    /** Get the string of Cert Chain. */
    CfResult (*toString)(HcfCertChain *self, CfBlob *out);

    /** Get the hashCode of Cert Chain. */
    CfResult (*hashCode)(HcfCertChain *self, CfBlob *out);
};

typedef struct HcfX509CertChainBuildResult HcfX509CertChainBuildResult;
struct HcfX509CertChainBuildResult {
    CfObjectBase base;
    HcfCertChain *certChain;
    HcfX509CertChainValidateResult validateResult;
};

typedef struct HcfX509CertChainBuildParameters HcfX509CertChainBuildParameters;
struct HcfX509CertChainBuildParameters {
    HcfX509CertMatchParams certMatchParameters;
    int32_t maxlength;
    HcfX509CertChainValidateParams validateParameters;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate cert chain instance.
 */
CfResult HcfCertChainCreate(
    const CfEncodingBlob *inStream, const HcfX509CertificateArray *inCerts, HcfCertChain **returnObj);

CfResult HcfCertChainBuildResultCreate(
    const HcfX509CertChainBuildParameters *inParams, HcfX509CertChainBuildResult **returnObj);

CfResult HcfCreateTrustAnchorWithKeyStore(
    const CfBlob *keyStore, const CfBlob *pwd, HcfX509TrustAnchorArray **trustAnchorArray);

CfResult HcfParsePKCS12(const CfBlob *keyStore, const HcfParsePKCS12Conf *conf, HcfX509P12Collection **p12Collection);

#ifdef __cplusplus
}
#endif

#endif // CF_X509_CERT_CHAIN_H
