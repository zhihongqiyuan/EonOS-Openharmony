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

#ifndef X509_CERT_CHAIN_VALIDATE_PARAMETERS_H
#define X509_CERT_CHAIN_VALIDATE_PARAMETERS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "cert_crl_collection.h"
#include "cf_blob.h"
#include "x509_trust_anchor.h"

typedef enum {
    VALIDATION_POLICY_TYPE_X509 = 0,
    VALIDATION_POLICY_TYPE_SSL = 1,
} HcfValPolicyType;

typedef enum {
    REVOCATION_CHECK_OPTION_PREFER_OCSP = 0,
    REVOCATION_CHECK_OPTION_ACCESS_NETWORK = 1,
    REVOCATION_CHECK_OPTION_FALLBACK_NO_PREFER = 2,
    REVOCATION_CHECK_OPTION_FALLBACK_LOCAL = 3,
} HcfRevChkOption;

typedef enum {
    KEYUSAGE_DIGITAL_SIGNATURE = 0,
    KEYUSAGE_NON_REPUDIATION = 1,
    KEYUSAGE_KEY_ENCIPHERMENT = 2,
    KEYUSAGE_DATA_ENCIPHERMENT = 3,
    KEYUSAGE_KEY_AGREEMENT = 4,
    KEYUSAGE_KEY_CERT_SIGN = 5,
    KEYUSAGE_CRL_SIGN = 6,
    KEYUSAGE_ENCIPHER_ONLY = 7,
    KEYUSAGE_DECIPHER_ONLY = 8,
} HcfKeyUsageType;

typedef struct {
    HcfKeyUsageType *data;
    uint32_t count;
} HcfKuArray;

typedef struct {
    HcfRevChkOption *data;
    uint32_t count;
} HcfRevChkOpArray;

typedef struct HcfRevocationCheckParam HcfRevocationCheckParam;
struct HcfRevocationCheckParam {
    CfBlobArray *ocspRequestExtension;
    CfBlob *ocspResponderURI;
    HcfX509Certificate *ocspResponderCert;
    CfBlob *ocspResponses;
    CfBlob *crlDownloadURI;
    HcfRevChkOpArray *options;
    CfBlob *ocspDigest;
};

typedef struct HcfX509CertChainValidateParams HcfX509CertChainValidateParams;
struct HcfX509CertChainValidateParams {
    CfBlob *date;                                  // string
    HcfX509TrustAnchorArray *trustAnchors;         // Array<X509TrustAnchor>
    HcfCertCRLCollectionArray *certCRLCollections; // Array<CertCRLCollection>;
    HcfRevocationCheckParam *revocationCheckParam;
    HcfValPolicyType policy;
    CfBlob *sslHostname;
    HcfKuArray *keyUsage;
};

#endif // X509_CERT_CHAIN_VALIDATE_PARAMETERS_H
