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

#ifndef CF_X509CRL_H
#define CF_X509CRL_H

#include "cf_blob.h"
#include "crl.h"
#include "x509_certificate.h"
#include "x509_crl_entry.h"
#include "x509_crl_match_parameters.h"
#include "x509_distinguished_name.h"

typedef struct HcfX509Crl HcfX509Crl;

struct HcfX509Crl {
    /** HcfX509Crl inherit HcfCrl. */
    HcfCrl base;

    /** Get the der coding format. */
    CfResult (*getEncoded)(HcfX509Crl *self, CfEncodingBlob *encodedOut);

    /** Use the public key to verify the signature of CRL. */
    CfResult (*verify)(HcfX509Crl *self, void *key);

    /** Get version number from CRL. */
    long (*getVersion)(HcfX509Crl *self);

    /** Get the issuer name from CRL. Issuer means the entity that signs and publishes the CRL. */
    CfResult (*getIssuerName)(HcfX509Crl *self, CfBlob *out);

    /** Get the issuer name Der from CRL. Issuer means the entity that signs and publishes the CRL. */
    CfResult (*getIssuerNameDer)(HcfX509Crl *self, CfBlob *out);

    /** Get lastUpdate value from CRL. */
    CfResult (*getLastUpdate)(HcfX509Crl *self, CfBlob *out);

    /** Get nextUpdate value from CRL. */
    CfResult (*getNextUpdate)(HcfX509Crl *self, CfBlob *out);

    /** This method can be used to find CRL entries in indirect CRLs. */
    CfResult (*getRevokedCert)(HcfX509Crl *self, const CfBlob *serialNumber, HcfX509CrlEntry **entryOut);

    /** This method can be used to find CRL entries in indirect cert. */
    CfResult (*getRevokedCertWithCert)(HcfX509Crl *self, HcfX509Certificate *cert,
        HcfX509CrlEntry **entryOut);

    /** Get all entries in this CRL. */
    CfResult (*getRevokedCerts)(HcfX509Crl *self, CfArray *entrysOut);

    /** Get the CRL information encoded by Der from this CRL. */
    CfResult (*getTbsInfo)(HcfX509Crl *self, CfBlob *tbsCertListOut);

    /** Get signature value from CRL. */
    CfResult (*getSignature)(HcfX509Crl *self, CfBlob *signature);

    /** Get the signature algorithm name of the CRL signature algorithm. */
    CfResult (*getSignatureAlgName)(HcfX509Crl *self, CfBlob *out);

    /** Get the signature algorithm oid string from CRL. */
    CfResult (*getSignatureAlgOid)(HcfX509Crl *self, CfBlob *out);

    /** Get the der encoded signature algorithm parameters from the CRL signature algorithm. */
    CfResult (*getSignatureAlgParams)(HcfX509Crl *self, CfBlob *sigAlgParamOut);

    /** Get all the extensions in CRL. */
    CfResult (*getExtensions)(HcfX509Crl *self, CfBlob *out);

    /** Match the crl with X509CrlMatchParameters. */
    CfResult (*match)(HcfX509Crl *self, const HcfX509CrlMatchParams *matchParams, bool *out);

    /** Get the string of crl. */
    CfResult (*toString)(HcfX509Crl *self, CfBlob *out);

    /** Get the hashCode of crl. */
    CfResult (*hashCode)(HcfX509Crl *self, CfBlob *out);

    /** Get the Entension Object of crl. */
    CfResult (*getExtensionsObject)(HcfX509Crl *self, CfBlob *out);

    /** Get the issuer name utf8 type from CRL. Issuer means the entity that signs and publishes the CRL. */
    CfResult (*getIssuerNameEx)(HcfX509Crl *self, CfEncodinigType encodingType, CfBlob *out);

    /** Get the string utf8 type of crl. */
    CfResult (*toStringEx)(HcfX509Crl *self, CfEncodinigType encodingType, CfBlob *out);
};

typedef struct HcfX509CrlArray HcfX509CrlArray;
struct HcfX509CrlArray {
    HcfX509Crl **data;
    uint32_t count;
};

#ifdef __cplusplus
extern "C" {
#endif

CfResult HcfX509CrlCreate(const CfEncodingBlob *inStream, HcfX509Crl **returnObj);

#ifdef __cplusplus
}
#endif

#endif // CF_X509CRL_H