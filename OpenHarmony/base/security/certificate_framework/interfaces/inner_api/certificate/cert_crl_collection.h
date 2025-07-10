/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef CERT_CRL_COLLECTION_H
#define CERT_CRL_COLLECTION_H

#include <stddef.h>
#include <stdint.h>
#include "cf_blob.h"
#include "cf_object_base.h"
#include "cf_result.h"
#include "x509_cert_match_parameters.h"
#include "x509_crl_match_parameters.h"
#include "x509_certificate.h"
#include "x509_crl.h"

#define MAX_LEN_OF_CERT_CRL_ARR 1024

typedef struct HcfCertCrlCollection HcfCertCrlCollection;

struct HcfCertCrlCollection {
    struct CfObjectBase base;

    /** Get certs list which match HcfX509CertMatchParams. */
    CfResult (*selectCerts)(
        HcfCertCrlCollection *self, const HcfX509CertMatchParams *matchParams, HcfX509CertificateArray *retCerts);

    /** Get CRLs list which match HcfX509CrlMatchParams. */
    CfResult (*selectCRLs)(
        HcfCertCrlCollection *self, const HcfX509CrlMatchParams *matchParams, HcfX509CrlArray *retCrls);

    /** Get CRLs list (without clone) in cert crl collection. */
    CfResult (*getCRLs)(HcfCertCrlCollection *self, HcfX509CrlArray **retCrls);
};

typedef struct HcfCertCRLCollectionArray HcfCertCRLCollectionArray;
struct HcfCertCRLCollectionArray {
    HcfCertCrlCollection **data;
    uint32_t count;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Generate cert crl collection instance.
 */
CfResult HcfCertCrlCollectionCreate(
    const HcfX509CertificateArray *inCerts, const HcfX509CrlArray *inCrls, HcfCertCrlCollection **out);

#ifdef __cplusplus
}
#endif

#endif // CERT_CRL_COLLECTION_H
