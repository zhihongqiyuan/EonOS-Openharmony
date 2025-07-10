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

#include "cj_cert_crl_collection.h"

int32_t FfiCertCjCertCrlCollectionNewInstance(const HcfX509CertificateArray *inCerts, const HcfX509CrlArray *inCrls,
                                              CjCertCrlCollection *returnObj)
{
    auto collection = static_cast<HcfCertCrlCollection *>(malloc(sizeof(HcfCertCrlCollection)));
    if (collection == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfCertCrlCollectionCreate(inCerts, inCrls, &collection);
    if (errCode != CF_SUCCESS) {
        free(collection);
        return errCode;
    }
    returnObj->collection = collection;
    return CF_SUCCESS;
}

void FfiCertCjCertCrlCollectionDeleteInstance(CjCertCrlCollection self)
{
    CfObjDestroy(self.collection);
}

CfResult FfiCertCjCertCrlCollectionSelectCerts(const CjCertCrlCollection self,
                                               const CjX509CertMatchParams *matchParams,
                                               HcfX509CertificateArray *retCerts)
{
    SubAltNameArray *subjectAlternativeNamesPtr = nullptr;
    SubAltNameArray subjectAlternativeNames;
    if (matchParams->subjectAlternativeNameCnt != 0) {
        subjectAlternativeNames = SubAltNameArray{
            .data = matchParams->subjectAlternativeNames,
            .count = matchParams->subjectAlternativeNameCnt
        };
        subjectAlternativeNamesPtr = &subjectAlternativeNames;
    }

    HcfCertificate *certPtr = nullptr;
    if (matchParams->x509Cert) {
        certPtr = &matchParams->x509Cert->base;
    }

    const auto params = HcfX509CertMatchParams{
        certPtr,
        matchParams->validDate,
        matchParams->issuer,
        matchParams->keyUsage,
        matchParams->serialNumber,
        matchParams->subject,
        matchParams->publicKey,
        matchParams->publicKeyAlgID,
        subjectAlternativeNamesPtr,
        matchParams->matchAllSubjectAltNames,
        matchParams->authorityKeyIdentifier,
        matchParams->minPathLenConstraint,
        matchParams->extendedKeyUsage,
        matchParams->nameConstraints,
        matchParams->certPolicy,
        matchParams->privateKeyValid,
        matchParams->subjectKeyIdentifier,
    };

    return self.collection->selectCerts(self.collection, &params, retCerts);
}

CfResult FfiCertCjCertCrlCollectionSelectCRLs(const CjCertCrlCollection self,
                                              const CjX509CrlMatchParams *matchParams,
                                              HcfX509CrlArray *retCrls)
{
    HcfCertificate *certPtr = nullptr;
    if (matchParams->x509Cert) {
        certPtr = &matchParams->x509Cert->base;
    }

    const auto hcfParams = HcfX509CrlMatchParams{
        .issuer = matchParams->issuer,
        .x509Cert = certPtr,
        .updateDateTime = matchParams->updateDateTime,
        .maxCRL = matchParams->maxCRL,
        .minCRL = matchParams->minCRL,
    };
    return self.collection->selectCRLs(self.collection, &hcfParams, retCrls);
}
