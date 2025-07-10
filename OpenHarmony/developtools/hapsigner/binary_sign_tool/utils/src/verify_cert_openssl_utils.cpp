/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "verify_cert_openssl_utils.h"
#include <cinttypes>
#include <cmath>
#include <fstream>

#include "openssl/pem.h"
#include "openssl/sha.h"
#include "signature_tools_log.h"
#include "securec.h"
#include "verify_hap_openssl_utils.h"

namespace OHOS {
namespace SignatureTools {

const uint32_t VerifyCertOpensslUtils::MIN_CERT_CHAIN_LEN_NEED_VERIFY_CRL = 2;
const int32_t VerifyCertOpensslUtils::OPENSSL_READ_CRL_MAX_TIME = 1048576; // 1024 * 1024
const int32_t VerifyCertOpensslUtils::OPENSSL_READ_CRL_LEN_EACH_TIME = 1024;
const int32_t VerifyCertOpensslUtils::BASE64_ENCODE_LEN_OF_EACH_GROUP_DATA = 4;
const int32_t VerifyCertOpensslUtils::BASE64_ENCODE_PACKET_LEN = 3;

void VerifyCertOpensslUtils::GenerateCertSignFromCertStack(STACK_OF(X509)* stackCerts, CertSign& certVisitSign)
{
    if (stackCerts == nullptr) {
        return;
    }
    for (int32_t i = 0; i < sk_X509_num(stackCerts); i++) {
        X509* x509Cert = sk_X509_value(stackCerts, i);
        if (x509Cert == nullptr) {
            continue;
        }
        certVisitSign[x509Cert] = false;
    }
}

void VerifyCertOpensslUtils::ClearCertVisitSign(CertSign& certVisitSign)
{
    for (auto& certPair : certVisitSign) {
        certPair.second = false;
    }
}

bool VerifyCertOpensslUtils::GetCertsChain(CertChain& certsChain, CertSign& certVisitSign)
{
    if (certsChain.empty() || certVisitSign.empty()) {
        SIGNATURE_TOOLS_LOGE("input is invalid");
        return false;
    }
    X509* issuerCert;
    X509* cert = certsChain[0];
    while ((issuerCert = FindCertOfIssuer(cert, certVisitSign)) != nullptr) {
        certsChain.push_back(X509_dup(issuerCert));
        certVisitSign[issuerCert] = true;
        cert = issuerCert;
    }
    if (CertVerify(cert, cert) == false) {
        SIGNATURE_TOOLS_LOGE("CertVerify is invalid");
        return false;
    }
    {
        X509_NAME* aName = X509_get_issuer_name(cert);
        X509_NAME* bName = X509_get_subject_name(cert);
        if (aName == NULL || bName == NULL) {
            SIGNATURE_TOOLS_LOGE("NULL X509_NAME");
            return false;
        }
        if (X509_NAME_cmp(aName, bName) != 0) {
            SIGNATURE_TOOLS_LOGE("compare error!");
            return false;
        }
        return true;
    }
}

X509* VerifyCertOpensslUtils::FindCertOfIssuer(X509* cert, CertSign& certVisitSign)
{
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("input is invalid");
        return nullptr;
    }
    X509_NAME* signCertIssuerName = X509_get_issuer_name(cert);
    for (auto certPair : certVisitSign) {
        if (certPair.second) {
            continue;
        }
        X509* issuerCert = certPair.first;
        X509_NAME* issuerCertSubjectName = X509_get_subject_name(issuerCert);
        /* verify sign and issuer */
        if (X509NameCompare(issuerCertSubjectName, signCertIssuerName) &&
            CertVerify(cert, issuerCert)) {
            return issuerCert;
        }
    }
    return nullptr;
}

bool VerifyCertOpensslUtils::CertVerify(X509* cert, const X509* issuerCert)
{
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("input is invalid");
        return false;
    }
    EVP_PKEY* caPublicKey = X509_get0_pubkey(issuerCert);
    if (caPublicKey == nullptr) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("get pubkey from caCert failed");
        return false;
    }
    return X509_verify(cert, caPublicKey) > 0;
}

bool VerifyCertOpensslUtils::VerifyCertChainPeriodOfValidity(CertChain& certsChain,
                                                             const ASN1_TYPE* signTime)
{
    if (certsChain.empty()) {
        return false;
    }
    for (uint32_t i = 0; i < certsChain.size() - 1; i++) {
        if (certsChain[i] == nullptr) {
            SIGNATURE_TOOLS_LOGE("%dst cert is nullptr", i);
            return false;
        }
        const ASN1_TIME* notBefore = X509_get0_notBefore(certsChain[i]);
        const ASN1_TIME* notAfter = X509_get0_notAfter(certsChain[i]);
        if (!CheckSignTimeInValidPeriod(signTime, notBefore, notAfter)) {
            SIGNATURE_TOOLS_LOGE("%dst cert is not in period of validity", i);
            return false;
        }
    }
    return true;
}

bool VerifyCertOpensslUtils::CheckAsn1TimeIsValid(const ASN1_TIME* asn1Time)
{
    if (asn1Time == nullptr || asn1Time->data == nullptr) {
        return false;
    }
    return true;
}

bool VerifyCertOpensslUtils::CheckAsn1TypeIsValid(const ASN1_TYPE* asn1Type)
{
    if (asn1Type == nullptr || asn1Type->value.asn1_string == nullptr ||
        asn1Type->value.asn1_string->data == nullptr) {
        return false;
    }
    return true;
}

bool VerifyCertOpensslUtils::CheckSignTimeInValidPeriod(const ASN1_TYPE* signTime,
                                                        const ASN1_TIME* notBefore,
                                                        const ASN1_TIME* notAfter)
{
    if (!CheckAsn1TimeIsValid(notBefore) || !CheckAsn1TimeIsValid(notAfter)) {
        SIGNATURE_TOOLS_LOGE("no valid period");
        return false;
    }
    if (!CheckAsn1TypeIsValid(signTime)) {
        SIGNATURE_TOOLS_LOGE("signTime is invalid");
        return false;
    }
    if (ASN1_TIME_compare(notBefore, signTime->value.asn1_string) > 0 ||
        ASN1_TIME_compare(notAfter, signTime->value.asn1_string) < 0) {
        SIGNATURE_TOOLS_LOGE("Out of valid period, signTime: %s, "
                             "notBefore:%s, notAfter : %s",
                             signTime->value.asn1_string->data, notBefore->data, notAfter->data);
        return false;
    }
    SIGNATURE_TOOLS_LOGD("signTime type: %d, data: %s, "
                         "notBefore:%s, notAfter : %s",
                         signTime->type, signTime->value.asn1_string->data,
                         notBefore->data, notAfter->data);
    return true;
}

bool VerifyCertOpensslUtils::VerifyCrl(CertChain& certsChain, STACK_OF(X509_CRL)* crls,
                                       Pkcs7Context& pkcs7Context)
{
    if (certsChain.empty()) {
        SIGNATURE_TOOLS_LOGE("cert chain is null");
        return false;
    }
    /* get signed cert's issuer and then it will be used to find local crl */
    if (!GetIssuerFromX509(certsChain[0], pkcs7Context.certIssuer)) {
        SIGNATURE_TOOLS_LOGE("get issuer of signed cert failed");
        return false;
    }
    X509_CRL* targetCrl = GetCrlBySignedCertIssuer(crls, certsChain[0]);
    /* crl is optional */
    if (targetCrl != nullptr && certsChain.size() >= MIN_CERT_CHAIN_LEN_NEED_VERIFY_CRL) {
        /* if it include crl, it must be verified by ca cert */
        if (X509_CRL_verify(targetCrl, X509_get0_pubkey(certsChain[1])) <= 0) {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            SIGNATURE_TOOLS_LOGE("verify crlInPackage failed");
            return false;
        }
    }
    return true;
}

X509_CRL* VerifyCertOpensslUtils::GetCrlBySignedCertIssuer(STACK_OF(X509_CRL)* crls, const X509* cert)
{
    if (crls == nullptr || cert == nullptr) {
        return nullptr;
    }
    X509_NAME* certIssuerName = X509_get_issuer_name(cert);
    for (int32_t i = 0; i < sk_X509_CRL_num(crls); i++) {
        X509_CRL* x509Crl = sk_X509_CRL_value(crls, i);
        if (x509Crl == nullptr) {
            continue;
        }
        X509_NAME* crlIssuer = X509_CRL_get_issuer(x509Crl);
        if (X509NameCompare(crlIssuer, certIssuerName)) {
            return x509Crl;
        }
    }
    return nullptr;
}

bool VerifyCertOpensslUtils::X509NameCompare(const X509_NAME* a, const X509_NAME* b)
{
    if (a == nullptr || b == nullptr) {
        return false;
    }
    return X509_NAME_cmp(a, b) == 0;
}

bool VerifyCertOpensslUtils::GetSubjectFromX509(const X509* cert, std::string& subject)
{
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("cert is nullptr");
        return false;
    }
    X509_NAME* name = X509_get_subject_name(cert);
    subject = GetDnToString(name);
    SIGNATURE_TOOLS_LOGD("subject = %s", subject.c_str());
    return true;
}

bool VerifyCertOpensslUtils::GetIssuerFromX509(const X509* cert, std::string& issuer)
{
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("cert is nullptr");
        return false;
    }
    X509_NAME* name = X509_get_issuer_name(cert);
    issuer = GetDnToString(name);
    SIGNATURE_TOOLS_LOGD("cert issuer = %s", issuer.c_str());
    return true;
}

std::string VerifyCertOpensslUtils::GetDnToString(X509_NAME* x509Name)
{
    if (x509Name == nullptr) {
        return "";
    }
    std::string countryNameString;
    GetTextFromX509Name(x509Name, NID_countryName, countryNameString);
    std::string organizationName;
    GetTextFromX509Name(x509Name, NID_organizationName, organizationName);
    std::string organizationalUnitName;
    GetTextFromX509Name(x509Name, NID_organizationalUnitName, organizationalUnitName);
    std::string commonName;
    GetTextFromX509Name(x509Name, NID_commonName, commonName);
    return "C=" + countryNameString + ", O=" + organizationName + ", OU=" + organizationalUnitName +
        ", CN=" + commonName;
}

void VerifyCertOpensslUtils::GetTextFromX509Name(X509_NAME* name, int32_t nId, std::string& text)
{
    int32_t textLen = X509_NAME_get_text_by_NID(name, nId, nullptr, 0);
    if (textLen <= 0) {
        return;
    }
    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(textLen + 1);
    if (X509_NAME_get_text_by_NID(name, nId, buffer.get(), textLen + 1) != textLen) {
        return;
    }
    text = std::string(buffer.get());
}
} // namespace SignatureTools
} // namespace OHOS