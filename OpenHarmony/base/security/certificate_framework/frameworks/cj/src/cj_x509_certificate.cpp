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

#include "cf_param.h"

#include "cj_x509_certificate.h"

int32_t FfiCertCjX509CertificateNewInstance(const CfEncodingBlob *blob, CjX509Certificate *returnObj)
{
    auto cert = static_cast<HcfX509Certificate *>(malloc(sizeof(HcfX509Certificate)));
    if (cert == nullptr) {
        return CF_ERR_MALLOC;
    }
    CfResult errCode = HcfX509CertificateCreate(blob, &cert);
    if (errCode != CF_SUCCESS) {
        free(cert);
        return errCode;
    }
    auto cfObj = static_cast<CfObject *>(malloc(sizeof(CfObject)));
    if (cfObj == nullptr) {
        free(cert);
        return CF_ERR_MALLOC;
    }
    errCode = CfResult(CfCreate(CF_OBJ_TYPE_CERT, blob, &cfObj));
    if (errCode != CF_SUCCESS) {
        CfObjDestroy(cert);
        free(cfObj);
        return errCode;
    }
    returnObj->cert = cert;
    returnObj->cfObj = cfObj;
    return CF_SUCCESS;
}

void FfiCertCjX509CertificateDeleteInstance(CjX509Certificate self)
{
    CfObjDestroy(self.cert);
    self.cfObj->destroy(&self.cfObj);
}

CfResult FfiCertCjX509CertificateFromHcfCert(HcfX509Certificate *hcfCert, CjX509Certificate *returnObj)
{
    CfEncodingBlob blob = {};
    CfResult errCode = hcfCert->base.getEncoded(&hcfCert->base, &blob);
    if (errCode != CF_SUCCESS) {
        return errCode;
    }
    auto cert = static_cast<HcfX509Certificate *>(malloc(sizeof(HcfX509Certificate)));
    if (cert == nullptr) {
        free(blob.data);
        return CF_ERR_MALLOC;
    }
    errCode = HcfX509CertificateCreate(&blob, &cert);
    if (errCode != CF_SUCCESS) {
        free(blob.data);
        free(cert);
        return errCode;
    }
    auto cfObj = static_cast<CfObject *>(malloc(sizeof(CfObject)));
    if (cfObj == nullptr) {
        free(blob.data);
        free(cert);
        return CF_ERR_MALLOC;
    }

    errCode = CfResult(CfCreate(CF_OBJ_TYPE_CERT, &blob, &cfObj));
    if (errCode != CF_SUCCESS) {
        free(blob.data);
        CfObjDestroy(cert);
        free(cfObj);
        return errCode;
    }
    returnObj->cert = cert;
    returnObj->cfObj = cfObj;
    free(blob.data);
    return errCode;
}

CfResult FfiCertCjX509CertificateVerify(const CjX509Certificate self, HcfPubKey *pubKey)
{
    return self.cert->base.verify(&self.cert->base, pubKey);
}

CfResult FfiCertCjX509CertificateGetEncoded(const CjX509Certificate self, CfEncodingBlob *encodedByte)
{
    return self.cert->base.getEncoded(&self.cert->base, encodedByte);
}

CfResult FfiCertCjX509CertificateGetPublicKey(const CjX509Certificate self, HcfPubKey **retPubKey)
{
    HcfPubKey *pubKey = nullptr;
    const CfResult errCode = self.cert->base.getPublicKey(&self.cert->base, (void **) &pubKey);
    if (errCode == CF_SUCCESS) {
        *retPubKey = pubKey;
    }
    return errCode;
}

CfResult FfiCertCjX509CertificateCheckValidityWithDate(const CjX509Certificate self, const char *date)
{
    return self.cert->checkValidityWithDate(self.cert, date);
}

long FfiCertCjX509CertificateGetVersion(const CjX509Certificate self)
{
    return self.cert->getVersion(self.cert);
}

CfResult FfiCertCjX509CertificateGetSerialNumber(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->getSerialNumber(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetIssuerName(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->getIssuerName(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetSubjectName(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->getSubjectName(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetNotBeforeTime(const CjX509Certificate self, CfBlob *outDate)
{
    return self.cert->getNotBeforeTime(self.cert, outDate);
}

CfResult FfiCertCjX509CertificateGetNotAfterTime(const CjX509Certificate self, CfBlob *outDate)
{
    return self.cert->getNotAfterTime(self.cert, outDate);
}

CfResult FfiCertCjX509CertificateGetSignature(const CjX509Certificate self, CfBlob *sigOut)
{
    return self.cert->getSignature(self.cert, sigOut);
}

CfResult FfiCertCjX509CertificateGetSignatureAlgName(const CjX509Certificate self, CfBlob *outName)
{
    return self.cert->getSignatureAlgName(self.cert, outName);
}

CfResult FfiCertCjX509CertificateGetSignatureAlgOid(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->getSignatureAlgOid(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetSignatureAlgParams(const CjX509Certificate self, CfBlob *sigAlgParamsOut)
{
    return self.cert->getSignatureAlgParams(self.cert, sigAlgParamsOut);
}

CfResult FfiCertCjX509CertificateGetKeyUsage(const CjX509Certificate self, CfBlob *boolArr)
{
    return self.cert->getKeyUsage(self.cert, boolArr);
}

CfResult FfiCertCjX509CertificateGetExtKeyUsage(const CjX509Certificate self, CfArray *keyUsageOut)
{
    return self.cert->getExtKeyUsage(self.cert, keyUsageOut);
}

int32_t FfiCertCjX509CertificateGetBasicConstraints(const CjX509Certificate self)
{
    return self.cert->getBasicConstraints(self.cert);
}

CfResult FfiCertCjX509CertificateGetSubjectAltNames(const CjX509Certificate self, CfArray *outName)
{
    return self.cert->getSubjectAltNames(self.cert, outName);
}

CfResult FfiCertCjX509CertificateGetIssuerAltNames(const CjX509Certificate self, CfArray *outName)
{
    return self.cert->getIssuerAltNames(self.cert, outName);
}

CfResult FfiCertCjX509CertificateMatch(const CjX509Certificate self, const CjX509CertMatchParams *matchParams,
                                       bool *out)
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

    return self.cert->match(self.cert, &params, out);
}

CfResult FfiCertCjX509CertificateGetCRLDistributionPointsURI(const CjX509Certificate self, CfArray *outURI)
{
    return self.cert->getCRLDistributionPointsURI(self.cert, outURI);
}

CfResult FfiCertCjX509CertificateToString(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->toString(self.cert, out);
}

CfResult FfiCertCjX509CertificateHashCode(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->hashCode(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetExtensionsObject(const CjX509Certificate self, CfBlob *out)
{
    return self.cert->getExtensionsObject(self.cert, out);
}

CfResult FfiCertCjX509CertificateGetSubjectNameEx(const CjX509Certificate self, const CfEncodinigType encodingType,
                                                  CfBlob *out)
{
    return self.cert->getSubjectNameEx(self.cert, encodingType, out);
}

CfResult FfiCertCjX509CertificateGetItem(const CjX509Certificate self, const int32_t itemType, CfBlob *out)
{
    CfParamSet *inParamSet = nullptr;
    int32_t ret;
    if ((ret = CfInitParamSet(&inParamSet)) != CF_SUCCESS) {
        return CfResult(ret);
    }

    const CfParam param[] = {
        CfParam{.tag = CF_TAG_GET_TYPE, .int32Param = CF_GET_TYPE_CERT_ITEM},
        CfParam{.tag = CF_TAG_PARAM0_INT32, .int32Param = itemType}
    };
    if ((ret = CfAddParams(inParamSet, param, sizeof(param) / sizeof(CfParam))) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParamSet *outParamSet = nullptr;
    if ((ret = self.cfObj->get(self.cfObj, inParamSet, &outParamSet)) != CF_SUCCESS) {
        CfFreeParamSet(&inParamSet);
        return CfResult(ret);
    }

    CfParam *resultParam = nullptr;
    ret = CfGetParam(outParamSet, CF_TAG_RESULT_BYTES, &resultParam);
    CfFreeParamSet(&inParamSet);
    CfFreeParamSet(&outParamSet);

    if (ret == CF_SUCCESS) {
        *out = resultParam->blob;
    }
    return CfResult(ret);
}
