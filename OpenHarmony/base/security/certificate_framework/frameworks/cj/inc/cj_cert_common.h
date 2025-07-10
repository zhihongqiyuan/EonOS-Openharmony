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

#ifndef CJ_CERT_COMMON_H
#define CJ_CERT_COMMON_H

#include "x509_certificate.h"

typedef struct CjX509CertMatchParams CjX509CertMatchParams;

struct CjX509CertMatchParams {
    HcfX509Certificate *x509Cert;
    CfBlob *validDate;
    CfBlob *issuer;
    CfBlob *keyUsage;
    CfBlob *serialNumber;
    CfBlob *subject;
    CfBlob *publicKey;
    CfBlob *publicKeyAlgID;
    SubjectAlternaiveNameData *subjectAlternativeNames;
    uint32_t subjectAlternativeNameCnt;
    bool matchAllSubjectAltNames;
    CfBlob *authorityKeyIdentifier;
    int32_t minPathLenConstraint;
    CfArray *extendedKeyUsage;
    CfBlob *nameConstraints;
    CfArray *certPolicy;
    CfBlob *privateKeyValid;
    CfBlob *subjectKeyIdentifier;
};

struct CjX509CrlMatchParams {
    CfBlobArray *issuer;
    HcfX509Certificate *x509Cert;
    CfBlob *updateDateTime;
    CfBlob *maxCRL;
    CfBlob *minCRL;
};

typedef struct HcfPubKey HcfPubKey;
struct HcfPubKey {
};

extern "C" void FfiCertDestroy(void *obj);

#endif //CJ_CERT_COMMON_H
