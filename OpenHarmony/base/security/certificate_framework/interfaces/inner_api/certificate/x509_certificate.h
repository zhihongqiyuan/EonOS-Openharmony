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

#ifndef CF_X509_CERTIFICATE_H
#define CF_X509_CERTIFICATE_H

#include "certificate.h"
#include "cf_blob.h"
#include "cf_result.h"
#include "x509_cert_match_parameters.h"
#include "x509_distinguished_name.h"
#include "x509_csr.h"

typedef struct PrivateKeyInfo PrivateKeyInfo;
struct PrivateKeyInfo {
    CfEncodingBlob *privateKey;
    char *privateKeyPassword;
};

typedef struct HcfX509Certificate HcfX509Certificate;

struct HcfX509Certificate {
    /** HcfCX509Certificate inherit HcfCertificate. */
    HcfCertificate base;

    /** Check whether the certificate is valid at the given time.
     * time format： YYMMDDHHMMSSZ or YYYYMMDDHHMMSSZ
    */
    CfResult (*checkValidityWithDate)(HcfX509Certificate *self, const char *date);

    /** Get version number from certificate. */
    long (*getVersion)(HcfX509Certificate *self);

    /** Get serial number from certificate. */
    CfResult (*getSerialNumber)(HcfX509Certificate *self, CfBlob *out);

    /** Get issuer distinguished name from certificate. */
    CfResult (*getIssuerName)(HcfX509Certificate *self, CfBlob *out);

    /** Get issuer distinguished name Der Format from certificate. */
    CfResult (*getIssuerNameDer)(HcfX509Certificate *self, CfBlob *out);

    /** Get subject distinguished name from certificate. */
    CfResult (*getSubjectName)(HcfX509Certificate *self, CfBlob *out);

    /** Get subject distinguished name from certificate. */
    CfResult (*getSubjectNameDer)(HcfX509Certificate *self, CfBlob *out);

    /** Get the not before time within the validity period of the certificate.
     * time format： YYMMDDHHMMSSZ or YYYYMMDDHHMMSSZ
     */
    CfResult (*getNotBeforeTime)(HcfX509Certificate *self, CfBlob *outDate);

    /** Get the not after time within the validity period of the certificate.
     *  time format： YYMMDDHHMMSSZ or YYYYMMDDHHMMSSZ
     */
    CfResult (*getNotAfterTime)(HcfX509Certificate *self, CfBlob *outDate);

    /** Get signature value from certificate. */
    CfResult (*getSignature)(HcfX509Certificate *self, CfBlob *sigOut);

    /** Get signature algorithm name from certificate. */
    CfResult (*getSignatureAlgName)(HcfX509Certificate *self, CfBlob *outName);

    /** Get signature algorithm oid from certificate. */
    CfResult (*getSignatureAlgOid)(HcfX509Certificate *self, CfBlob *out);

    /** Get the DER encoded signature algorithm parameters from the signature algorithm of the certificate. */
    CfResult (*getSignatureAlgParams)(HcfX509Certificate *self, CfBlob *sigAlgParamsOut);

    /** Get a Boolean array representing the bits of keyuse extension.
     * The key usage extension defines the purpose of the key. */
    CfResult (*getKeyUsage)(HcfX509Certificate *self, CfBlob *boolArr);

    /** Get a const string list that represents the object identifier of the extkeyusage. */
    CfResult (*getExtKeyUsage)(HcfX509Certificate *self, CfArray *keyUsageOut);

    /** Get the path length of the certificate constraint from the key extensions(BasicConstraints).
     * The BasicConstraints identify whether the issuer of the certificate is CA and the depth of the cert chain.
     * Only when CA is set to true, pathLenConstraint is meaningful.
     */
    int32_t (*getBasicConstraints)(HcfX509Certificate *self);

    /** Get subject alternative name from certificate. */
    CfResult (*getSubjectAltNames)(HcfX509Certificate *self, CfArray *outName);

    /** Get issuer alternative name from certificate. */
    CfResult (*getIssuerAltNames)(HcfX509Certificate *self, CfArray *outName);

    /** Match the ceritificate with X509CertMatchParameters. */
    CfResult (*match)(HcfX509Certificate *self, const HcfX509CertMatchParams *matchParams, bool *out);

    /** Get CRL distribution points URI from certificate. */
    CfResult (*getCRLDistributionPointsURI)(HcfX509Certificate *self, CfArray *outURI);

    /** Get the string of ceritificate. */
    CfResult (*toString)(HcfX509Certificate *self, CfBlob *out);

    /** Get the hashCode of ceritificate. */
    CfResult (*hashCode)(HcfX509Certificate *self, CfBlob *out);

    /** Get the Entension Object of ceritificate. */
    CfResult (*getExtensionsObject)(HcfX509Certificate *self, CfBlob *out);

    /** Get subject distinguished name utf8 type from certificate. */
    CfResult (*getSubjectNameEx)(HcfX509Certificate *self, CfEncodinigType encodingType, CfBlob *out);

    /** Get issuer distinguished name utf8 type from certificate. */
    CfResult (*getIssuerNameEx)(HcfX509Certificate *self, CfEncodinigType encodingType, CfBlob *out);

    /** Get the string utf8 type of ceritificate. */
    CfResult (*toStringEx)(HcfX509Certificate *self, CfEncodinigType encodingType, CfBlob *out);
};

typedef struct HcfX509CertificateArray HcfX509CertificateArray;
struct HcfX509CertificateArray {
    HcfX509Certificate **data;
    uint32_t count;
};

#ifdef __cplusplus
extern "C" {
#endif

CfResult HcfX509CertificateCreate(const CfEncodingBlob *inStream, HcfX509Certificate **returnObj);
CfResult HcfX509CertificateGenCsr(PrivateKeyInfo *privateKey, const HcfGenCsrConf *conf, CfBlob *csrBlob);

#ifdef __cplusplus
}
#endif

#endif // CF_X509_CERTIFICATE_H

