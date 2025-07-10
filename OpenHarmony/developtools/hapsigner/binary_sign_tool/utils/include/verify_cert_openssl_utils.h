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
#ifndef HAP_CERT_VERIFY_OPENSSL_UTILS_H
#define HAP_CERT_VERIFY_OPENSSL_UTILS_H

#include <string>
#include <unordered_map>

#include "export_define.h"
#include "byte_buffer.h"
#include "pkcs7_context.h"

namespace OHOS {
namespace SignatureTools {

using CertSign = std::unordered_map<X509*, bool>;

class VerifyCertOpensslUtils {
public:
    VerifyCertOpensslUtils() = delete;
    DLL_EXPORT static bool VerifyCrl(CertChain& certsChain, STACK_OF(X509_CRL)* crls,
                                    Pkcs7Context& pkcs7Context);

    DLL_EXPORT static bool X509NameCompare(const X509_NAME* a, const X509_NAME* b);
    DLL_EXPORT static X509* FindCertOfIssuer(X509* cert, CertSign& certVisitSign);
    DLL_EXPORT static std::string GetDnToString(X509_NAME* name);
    DLL_EXPORT static void GetTextFromX509Name(X509_NAME* name, int32_t nId, std::string& text);
    DLL_EXPORT static X509_CRL* GetCrlBySignedCertIssuer(STACK_OF(X509_CRL)* crls, const X509* cert);
    DLL_EXPORT static bool CheckSignTimeInValidPeriod(const ASN1_TYPE* signTime,
                                                    const ASN1_TIME* notBefore, const ASN1_TIME* notAfter);
    DLL_EXPORT static void GenerateCertSignFromCertStack(STACK_OF(X509)* certs, CertSign& certVisitSign);
    DLL_EXPORT static void ClearCertVisitSign(CertSign& certVisitSign);
    DLL_EXPORT static bool GetCertsChain(CertChain& certsChain, CertSign& certVisitSign);
    DLL_EXPORT static bool CertVerify(X509* cert, const X509* issuerCert);
    DLL_EXPORT static bool CheckAsn1TimeIsValid(const ASN1_TIME* asn1Time);
    DLL_EXPORT static bool CheckAsn1TypeIsValid(const ASN1_TYPE* asn1Type);
    DLL_EXPORT static bool GetSubjectFromX509(const X509* cert, std::string& subject);
    DLL_EXPORT static bool GetIssuerFromX509(const X509* cert, std::string& issuer);
    DLL_EXPORT static bool VerifyCertChainPeriodOfValidity(CertChain& certsChain,
                                                            const ASN1_TYPE* signTime);
private:
    static const int32_t OPENSSL_READ_CRL_LEN_EACH_TIME;
    static const int32_t BASE64_ENCODE_LEN_OF_EACH_GROUP_DATA;
    static const int32_t BASE64_ENCODE_PACKET_LEN;
    static const uint32_t MIN_CERT_CHAIN_LEN_NEED_VERIFY_CRL;
    static const int32_t OPENSSL_READ_CRL_MAX_TIME;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // HAP_CERT_VERIFY_OPENSSL_UTILS_H
