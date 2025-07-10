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

#include "cj_x509_crl.h"

int32_t FfiCertCjX509CrlNewInstance(const CfEncodingBlob *blob, CjX509Crl *returnObj)
{
    auto crl = static_cast<HcfX509Crl *>(malloc(sizeof(HcfX509Crl)));
    if (crl == nullptr) {
        return CF_ERR_MALLOC;
    }
    const auto errCode = HcfX509CrlCreate(blob, &crl);
    if (errCode != CF_SUCCESS) {
        free(crl);
        return errCode;
    }
    returnObj->crl = crl;
    return CF_SUCCESS;
}

void FfiCertCjX509CrlDeleteInstance(CjX509Crl self)
{
    CfObjDestroy(self.crl);
}

bool FfiCertCjX509CrlIsRevoked(const CjX509Crl self, const HcfX509Certificate *cert)
{
    return self.crl->base.isRevoked(&self.crl->base, &cert->base);
}

const char *FfiCertCjX509CrlGetType(const CjX509Crl self)
{
    return self.crl->base.getType(&self.crl->base);
}

CfResult FfiCertCjX509CrlGetEncoded(const CjX509Crl self, CfEncodingBlob *encodedOut)
{
    return self.crl->getEncoded(self.crl, encodedOut);
}

CfResult FfiCertCjX509CrlVerify(const CjX509Crl self, HcfPubKey *pubKey)
{
    return self.crl->verify(self.crl, pubKey);
}

long FfiCertCjX509CrlGetVersion(const CjX509Crl self)
{
    return self.crl->getVersion(self.crl);
}

CfResult FfiCertCjX509CrlGetIssuerName(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getIssuerName(self.crl, out);
}

CfResult FfiCertCjX509CrlGetLastUpdate(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getLastUpdate(self.crl, out);
}

CfResult FfiCertCjX509CrlGetNextUpdate(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getNextUpdate(self.crl, out);
}

CfResult FfiCertCjX509CrlGetRevokedCert(const CjX509Crl self, const CfBlob *serialNumber,
                                        CjX509CrlEntry *entryOut)
{
    return self.crl->getRevokedCert(self.crl, serialNumber, &entryOut->crlEntry);
}

CfResult FfiCertCjX509CrlGetRevokedCertWithCert(const CjX509Crl self, HcfX509Certificate *cert,
                                                CjX509CrlEntry *entryOut)
{
    return self.crl->getRevokedCertWithCert(self.crl, cert, &entryOut->crlEntry);
}

CfResult FfiCertCjX509CrlGetRevokedCerts(const CjX509Crl self, CfArray *entrysOut)
{
    return self.crl->getRevokedCerts(self.crl, entrysOut);
}

CfResult FfiCertCjX509CrlGetTbsInfo(const CjX509Crl self, CfBlob *tbsCertListOut)
{
    return self.crl->getTbsInfo(self.crl, tbsCertListOut);
}

CfResult FfiCertCjX509CrlGetSignature(const CjX509Crl self, CfBlob *signature)
{
    return self.crl->getSignature(self.crl, signature);
}

CfResult FfiCertCjX509CrlGetSignatureAlgName(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getSignatureAlgName(self.crl, out);
}

CfResult FfiCertCjX509CrlGetSignatureAlgOid(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getSignatureAlgOid(self.crl, out);
}

CfResult FfiCertCjX509CrlGetSignatureAlgParams(const CjX509Crl self, CfBlob *sigAlgParamOut)
{
    return self.crl->getSignatureAlgParams(self.crl, sigAlgParamOut);
}

CfResult FfiCertCjX509CrlGetExtensions(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getExtensions(self.crl, out);
}

CfResult FfiCertCjX509CrlMatch(const CjX509Crl self, const CjX509CrlMatchParams *matchParams, bool *out)
{
    HcfCertificate *certPtr = nullptr;
    if (matchParams->x509Cert != nullptr) {
        certPtr = &matchParams->x509Cert->base;
    }
    const auto hcfParams = HcfX509CrlMatchParams{
        .issuer = matchParams->issuer,
        .x509Cert = certPtr,
        .updateDateTime = matchParams->updateDateTime,
        .maxCRL = matchParams->maxCRL,
        .minCRL = matchParams->minCRL,
    };
    return self.crl->match(self.crl, &hcfParams, out);
}

CfResult FfiCertCjX509CrlToString(const CjX509Crl self, CfBlob *out)
{
    return self.crl->toString(self.crl, out);
}

CfResult FfiCertCjX509CrlHashCode(const CjX509Crl self, CfBlob *out)
{
    return self.crl->hashCode(self.crl, out);
}

CfResult FfiCertCjX509CrlGetExtensionsObject(const CjX509Crl self, CfBlob *out)
{
    return self.crl->getExtensionsObject(self.crl, out);
}
