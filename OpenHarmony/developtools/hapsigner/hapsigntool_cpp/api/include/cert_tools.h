/*
 * Copyright (c) 2024-2024 Huawei Device Co., Ltd.
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
#ifndef SIGNATRUETOOLS_CERT_TOOLS_H
#define SIGNATRUETOOLS_CERT_TOOLS_H
#include <string>

#include "cert_dn_utils.h"
#include "openssl/x509v3.h"
#include "openssl/rand.h"
#include "localization_adapter.h"
#include "securec.h"

namespace OHOS {
namespace SignatureTools {

class CertTools {
public:
    static X509* GenerateRootCertificate(EVP_PKEY* keyPair, X509_REQ* certReq, Options* options);
    static X509* GenerateSubCert(EVP_PKEY* keyPair, X509_REQ* certReq, Options* options);
    static X509* GenerateCert(EVP_PKEY* keyPair, X509_REQ* certReq, Options* options);
    static bool SaveCertTofile(const std::string& filename, X509* cert);
    static X509_REQ* GenerateCsr(EVP_PKEY* evpPkey, std::string signAlgorithm, std::string subject);
    static X509* SignCsrGenerateCert(X509_REQ* rootcsr, X509_REQ* subcsr,
                                     EVP_PKEY* keyPair, Options* options);
    static std::string CsrToString(X509_REQ* csr);
    static X509* GenerateEndCert(X509_REQ* csr, EVP_PKEY* issuerKeyPair,
                                 LocalizationAdapter& adapter,
                                 const char signCapacity[], int capacityLen);
    static X509* ReadfileToX509(const std::string& filename);
    static bool SetBisicConstraints(Options* options, X509* cert);
    static bool SetBisicConstraintsPathLen(Options* options, X509* cert);
    static bool SetSubjectForCert(X509_REQ* certReq, X509* cert);
    static bool SignForSubCert(X509* cert, X509_REQ* csr, X509_REQ* caReq,
                               EVP_PKEY* caPrikey, Options* options);
    static bool SetKeyUsage(X509* cert, Options* options);
    static bool SetkeyUsageExt(X509* cert, Options* options);
    static bool SetCertValidity(X509* cert, int validity);
    static bool SerialNumberBuilder(uint8_t* serialNum, int length);
    static bool SetCertVersion(X509* cert, int versionNum);
    static bool SetCertSerialNum(X509* cert);
    static bool SetCertIssuerName(X509* cert, X509_NAME* issuer);
    static bool SetCertSubjectName(X509* cert, X509_REQ* subjectCsr);
    static bool SetCertValidityStartAndEnd(X509* cert, long vilidityStart, long vilidityEnd);
    static bool SetCertPublickKey(X509* cert, X509_REQ* subjectCsr);
    static bool SetBasicExt(X509* cert);
    static bool SetkeyUsageExt(X509* cert);
    static bool SetKeyUsageEndExt(X509* cert);
    static bool SetKeyIdentifierExt(X509* cert);
    static bool SetAuthorizeKeyIdentifierExt(X509* cert);
    static bool SetSignCapacityExt(X509* cert, const char signCapacity[], int capacityLen);
    static bool SignCert(X509* cert, EVP_PKEY* privateKey, std::string signAlg);
    static bool SetExpandedInformation(X509* cert, Options* options);
    static bool SetPubkeyAndSignCert(X509* cert, X509_REQ* issuercsr,
                                     X509_REQ* certReq, EVP_PKEY* keyPair, Options* options);
    static bool UpdateConstraint(Options* options);
    static bool String2Bool(Options* options, const std::string& option);
    CertTools() = default;
    ~CertTools() = default;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_CERT_TOOLS_H
