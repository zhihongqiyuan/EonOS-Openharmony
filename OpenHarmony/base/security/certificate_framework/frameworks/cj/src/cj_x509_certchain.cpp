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

#include "cj_x509_certchain.h"

CfResult parseAnchor(CjX509TrustAnchor *trustAnchors, uint32_t trustAnchorCnt, HcfX509TrustAnchorArray &ret);

HcfCertCRLCollectionArray *parseCRLCollections(HcfCertCrlCollection **certCRLCollections, uint32_t certCRLCollectionCnt,
                                               HcfCertCRLCollectionArray &ret);

HcfRevocationCheckParam *parseRevocation(const CjRevocationCheckParam *checkParam,
                                         HcfRevocationCheckParam &revocationCheckParam,
                                         HcfRevChkOpArray &revChkOption);

HcfKuArray *parseKeyUsage(const CjX509CertChainValidateParams *params, HcfKuArray &keyUsage);

void updateCertMatchParameters(HcfX509CertChainBuildParameters &hcfParams, const CjX509CertMatchParams *matchParams);

void updateValidateParameters(const CjX509CertChainValidateParams &validParams,
                              HcfX509CertChainBuildParameters &hcfParams);

SubAltNameArray *parseSubAltName(const CjX509CertMatchParams &matchParams, SubAltNameArray &subjectAlternativeNames);

int32_t FfiCertCjX509CertChainNewInstanceBlob(const CfEncodingBlob *blob, CjX509CertChain *returnObj)
{
    auto chain = static_cast<HcfCertChain *>(malloc(sizeof(HcfCertChain)));
    if (chain == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfCertChainCreate(blob, nullptr, &chain);
    if (errCode != CF_SUCCESS) {
        free(chain);
        return errCode;
    }
    returnObj->chain = chain;
    return CF_SUCCESS;
}

int32_t FfiCertCjX509CertChainNewInstanceArray(const HcfX509CertificateArray *inCerts, CjX509CertChain *returnObj)
{
    auto chain = static_cast<HcfCertChain *>(malloc(sizeof(HcfCertChain)));
    if (chain == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfCertChainCreate(nullptr, inCerts, &chain);
    if (errCode != CF_SUCCESS) {
        free(chain);
        return errCode;
    }
    returnObj->chain = chain;
    return CF_SUCCESS;
}

void FfiCertCjX509CertChainDeleteInstance(CjX509CertChain self)
{
    CfObjDestroy(self.chain);
}

CfResult FfiCertCjX509CertChainGetCertList(const CjX509CertChain self, HcfX509CertificateArray *out)
{
    return self.chain->getCertList(self.chain, out);
}

CfResult FfiCertCjX509CertChainValidate(const CjX509CertChain self,
                                        const CjX509CertChainValidateParams *params,
                                        CjX509CertChainValidateResult *result)
{
    HcfX509TrustAnchorArray anchors;
    CfResult errCode = parseAnchor(params->trustAnchors, params->trustAnchorCnt, anchors);
    if (errCode != CF_SUCCESS) {
        return errCode;
    }

    HcfCertCRLCollectionArray certCRLCollections;
    HcfCertCRLCollectionArray *certCRLCollectionsPtr = parseCRLCollections(
        params->certCRLCollections, params->certCRLCollectionCnt,
        certCRLCollections);

    HcfRevocationCheckParam revocationCheckParam;
    HcfRevChkOpArray revChkOption;
    HcfRevocationCheckParam *revocationCheckParamPtr = parseRevocation(params->revocationCheckParam,
                                                                       revocationCheckParam,
                                                                       revChkOption);

    HcfKuArray keyUsage;
    HcfKuArray *keyUsagePtr = parseKeyUsage(params, keyUsage);

    auto hcfParams = HcfX509CertChainValidateParams{
        .date = params->date,
        .trustAnchors = &anchors,
        .certCRLCollections = certCRLCollectionsPtr,
        .revocationCheckParam = revocationCheckParamPtr,
        .policy = params->policy,
        .sslHostname = params->sslHostname,
        .keyUsage = keyUsagePtr,
    };

    HcfX509CertChainValidateResult hcfResult;
    errCode = self.chain->validate(self.chain, &hcfParams, &hcfResult);

    for (uint32_t i = 0; i < anchors.count; ++i) {
        free(anchors.data[i]);
    }
    free(anchors.data);

    if (errCode == CF_SUCCESS) {
        result->trustAnchor.CAPubKey = hcfResult.trustAnchor->CAPubKey;
        result->trustAnchor.CACert = hcfResult.trustAnchor->CACert;
        result->trustAnchor.CASubject = hcfResult.trustAnchor->CASubject;
        result->trustAnchor.nameConstraints = hcfResult.trustAnchor->nameConstraints;
        result->entityCert = hcfResult.entityCert;
    }
    return errCode;
}

CfResult FfiCertCjX509CertChainToString(const CjX509CertChain self, CfBlob *out)
{
    return self.chain->toString(self.chain, out);
}

CfResult FfiCertCjX509CertChainHashCode(const CjX509CertChain self, CfBlob *out)
{
    return self.chain->hashCode(self.chain, out);
}

CfResult FfiCertBuildX509CertChain(const CjX509CertMatchParams &matchParams,
                                   const CjX509CertChainValidateParams &validParams,
                                   int32_t maxLength, CjX509CertChain *returnObj)
{
    HcfCertificate *certPtr = nullptr;
    if (matchParams.x509Cert != nullptr) {
        certPtr = &matchParams.x509Cert->base;
    }

    SubAltNameArray subjectAlternativeNames;
    SubAltNameArray *subjectAlternativeNamesPtr = parseSubAltName(matchParams, subjectAlternativeNames);

    HcfX509TrustAnchorArray anchors;
    CfResult errCode = parseAnchor(validParams.trustAnchors, validParams.trustAnchorCnt, anchors);
    if (errCode != CF_SUCCESS) {
        return errCode;
    }

    HcfCertCRLCollectionArray certCRLCollections;
    HcfCertCRLCollectionArray *certCRLCollectionsPtr = parseCRLCollections(
        validParams.certCRLCollections, validParams.certCRLCollectionCnt,
        certCRLCollections);

    HcfRevocationCheckParam revocationCheckParam;
    HcfRevChkOpArray revChkOption;
    HcfRevocationCheckParam *revocationCheckParamPtr = parseRevocation(validParams.revocationCheckParam,
                                                                       revocationCheckParam,
                                                                       revChkOption);

    HcfKuArray keyUsage;
    HcfKuArray *keyUsagePtr = parseKeyUsage(&validParams, keyUsage);

    HcfX509CertChainBuildParameters hcfParams = {};
    updateCertMatchParameters(hcfParams, &matchParams);
    hcfParams.certMatchParameters.x509Cert = certPtr;
    hcfParams.certMatchParameters.subjectAlternativeNames = subjectAlternativeNamesPtr;
    hcfParams.maxlength = maxLength;
    updateValidateParameters(validParams, hcfParams);
    hcfParams.validateParameters.trustAnchors = &anchors;
    hcfParams.validateParameters.certCRLCollections = certCRLCollectionsPtr;
    hcfParams.validateParameters.revocationCheckParam = revocationCheckParamPtr;
    hcfParams.validateParameters.keyUsage = keyUsagePtr;

    HcfX509CertChainBuildResult *buildResult = nullptr;
    errCode = HcfCertChainBuildResultCreate(&hcfParams, &buildResult);
    if (errCode != CF_SUCCESS) {
        return errCode;
    }
    returnObj->chain = buildResult->certChain;
    // release buildResult here
    // buildResult->certChain is allocated by CfMalloc and will be released by cangjie
    free(buildResult);
    return CF_SUCCESS;
}

SubAltNameArray *parseSubAltName(const CjX509CertMatchParams &matchParams, SubAltNameArray &subjectAlternativeNames)
{
    SubAltNameArray *subjectAlternativeNamesPtr = nullptr;
    if (matchParams.subjectAlternativeNameCnt > 0) {
        subjectAlternativeNames = SubAltNameArray{
            .data = matchParams.subjectAlternativeNames,
            .count = matchParams.subjectAlternativeNameCnt
        };
        subjectAlternativeNamesPtr = &subjectAlternativeNames;
    }
    return subjectAlternativeNamesPtr;
}

void updateValidateParameters(const CjX509CertChainValidateParams &validParams,
                              HcfX509CertChainBuildParameters &hcfParams)
{
    hcfParams.validateParameters = {
        .date = validParams.date,
        .policy = validParams.policy,
        .sslHostname = validParams.sslHostname,
    };
}

void updateCertMatchParameters(HcfX509CertChainBuildParameters &hcfParams, const CjX509CertMatchParams *matchParams)
{
    hcfParams.certMatchParameters.validDate = matchParams->validDate;
    hcfParams.certMatchParameters.issuer = matchParams->issuer;
    hcfParams.certMatchParameters.keyUsage = matchParams->keyUsage;
    hcfParams.certMatchParameters.serialNumber = matchParams->serialNumber;
    hcfParams.certMatchParameters.subject = matchParams->subject;
    hcfParams.certMatchParameters.publicKey = matchParams->publicKey;
    hcfParams.certMatchParameters.publicKeyAlgID = matchParams->publicKeyAlgID;
    hcfParams.certMatchParameters.matchAllSubjectAltNames = matchParams->matchAllSubjectAltNames;
    hcfParams.certMatchParameters.authorityKeyIdentifier = matchParams->authorityKeyIdentifier;
    hcfParams.certMatchParameters.minPathLenConstraint = matchParams->minPathLenConstraint;
    hcfParams.certMatchParameters.extendedKeyUsage = matchParams->extendedKeyUsage;
    hcfParams.certMatchParameters.nameConstraints = matchParams->nameConstraints;
    hcfParams.certMatchParameters.certPolicy = matchParams->certPolicy;
    hcfParams.certMatchParameters.privateKeyValid = matchParams->privateKeyValid;
    hcfParams.certMatchParameters.subjectKeyIdentifier = matchParams->subjectKeyIdentifier;
}

CfResult FfiCertCreateTrustAnchorWithKeyStore(const CfBlob *keyStore, const CfBlob *pwd,
                                              CjX509TrustAnchorArray *returnObj)
{
    HcfX509TrustAnchorArray *anchorArray = nullptr;
    const auto errCode = HcfCreateTrustAnchorWithKeyStore(keyStore, pwd, &anchorArray);
    if (errCode != CF_SUCCESS) {
        return errCode;
    }
    returnObj->count = anchorArray->count;
    returnObj->data = static_cast<CjX509TrustAnchor **>(malloc(sizeof(CjX509TrustAnchor *) * anchorArray->count));
    if (returnObj->data == nullptr) {
        free(anchorArray->data);
        return CF_ERR_MALLOC;
    }
    for (uint32_t i = 0; i < anchorArray->count; ++i) {
        const auto anchor = static_cast<CjX509TrustAnchor *>(malloc(sizeof(CjX509TrustAnchor)));
        if (anchor == nullptr) {
            for (uint32_t j = 0; j < i; j++) {
                free(returnObj->data[j]);
            }
            free(anchorArray->data);
            return CF_ERR_MALLOC;
        }
        anchor->CAPubKey = anchorArray->data[i]->CAPubKey;
        anchor->CACert = anchorArray->data[i]->CACert;
        anchor->CASubject = anchorArray->data[i]->CASubject;
        anchor->nameConstraints = anchorArray->data[i]->nameConstraints;
        returnObj->data[i] = anchor;
    }
    free(anchorArray->data);
    return CF_SUCCESS;
}

HcfCertCRLCollectionArray *parseCRLCollections(HcfCertCrlCollection **certCRLCollections,
                                               uint32_t certCRLCollectionCnt,
                                               HcfCertCRLCollectionArray &ret)
{
    HcfCertCRLCollectionArray *certCRLCollectionsPtr = nullptr;
    if (certCRLCollectionCnt != 0) {
        ret.data = certCRLCollections;
        ret.count = certCRLCollectionCnt;
        certCRLCollectionsPtr = &ret;
    }
    return certCRLCollectionsPtr;
}

CfResult parseAnchor(CjX509TrustAnchor *trustAnchors, uint32_t trustAnchorCnt, HcfX509TrustAnchorArray &ret)
{
    ret = HcfX509TrustAnchorArray{
        .data = static_cast<HcfX509TrustAnchor **>(malloc(sizeof(HcfX509TrustAnchor *) * (trustAnchorCnt))),
        .count = trustAnchorCnt,
    };
    if (ret.data == nullptr) {
        return CF_ERR_MALLOC;
    }
    for (uint32_t i = 0; i < trustAnchorCnt; ++i) {
        const auto item = static_cast<HcfX509TrustAnchor *>(malloc(sizeof(HcfX509TrustAnchor)));
        if (item == nullptr) {
            for (uint32_t j = 0; j < i; j++) {
                free(ret.data[j]);
            }
            free(ret.data);
            return CF_ERR_MALLOC;
        }
        item->CAPubKey = trustAnchors[i].CAPubKey;
        item->CACert = trustAnchors[i].CACert;
        item->CASubject = trustAnchors[i].CASubject;
        item->nameConstraints = trustAnchors[i].nameConstraints;
        ret.data[i] = item;
    }
    return CF_SUCCESS;
}

HcfKuArray *parseKeyUsage(const CjX509CertChainValidateParams *params, HcfKuArray &keyUsage)
{
    HcfKuArray *keyUsagePtr = nullptr;
    if (params->keyUsageCnt != 0) {
        keyUsage = HcfKuArray{
            .data = params->keyUsage,
            .count = params->keyUsageCnt,
        };
        keyUsagePtr = &keyUsage;
    }
    return keyUsagePtr;
}

HcfRevocationCheckParam *parseRevocation(const CjRevocationCheckParam *checkParam,
                                         HcfRevocationCheckParam &revocationCheckParam,
                                         HcfRevChkOpArray &revChkOption)
{
    HcfRevocationCheckParam *revocationCheckParamPtr = nullptr;
    HcfRevChkOpArray *revChkOptionPtr = nullptr;
    if (checkParam) {
        if (checkParam->optionCnt != 0) {
            revChkOption = HcfRevChkOpArray{
                .data = checkParam->options,
                .count = checkParam->optionCnt,
            };
            revChkOptionPtr = &revChkOption;
        }
        revocationCheckParam = HcfRevocationCheckParam{
            .ocspRequestExtension = checkParam->ocspRequestExtension,
            .ocspResponderURI = checkParam->ocspResponderURI,
            .ocspResponderCert = checkParam->ocspResponderCert,
            .ocspResponses = checkParam->ocspResponses,
            .crlDownloadURI = checkParam->crlDownloadURI,
            .options = revChkOptionPtr,
            .ocspDigest = checkParam->ocspDigest,
        };
        revocationCheckParamPtr = &revocationCheckParam;
    }
    return revocationCheckParamPtr;
}
