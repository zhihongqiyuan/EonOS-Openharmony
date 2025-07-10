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
#ifndef SIGNATRUETOOLS_SIGNTOOLSERVICELMPL_H
#define SIGNATRUETOOLS_SIGNTOOLSERVICELMPL_H

#include "options.h"
#include "file_utils.h"
#include "cert_tools.h"
#include "localization_adapter.h"
#include "signature_tools_log.h"
#include "service_api.h"

namespace OHOS {
namespace SignatureTools {

class SignToolServiceImpl : public ServiceApi {
public:
    static int GetProvisionContent(const std::string& input, std::string& ret);
    
    SignToolServiceImpl() = default;
    virtual ~SignToolServiceImpl() = default;

    bool GenerateCA(Options* options)override;
    bool GenerateRootCertToFile(Options* options, EVP_PKEY* rootKey);
    bool GenerateSubCertToFile(Options* options, EVP_PKEY* rootKey);
    bool OutputModeOfCert(X509* cert, Options* options);
    bool GenerateCert(Options* options)override;
    bool GenerateKeyStore(Options* options)override;
    bool GenerateCsr(Options* options)override;
    bool OutputString(std::string content, std::string file);
    bool GenerateAppCert(Options* option)override;
    bool GenerateProfileCert(Options* options)override;
    bool GetAndOutPutCert(LocalizationAdapter& adapter, X509* cert);
    bool SignProfile(Options* options)override;
    bool SignHap(Options* options)override;
    bool VerifyProfile(Options* options)override;
    bool OutPutCertChain(std::vector<X509*>& certs, const std::string& outPutPath);
    bool OutPutCert(X509* cert, const std::string& outPutPath);
    bool PrintX509CertFromMemory(X509* cert);
    bool PrintX509CertChainFromMemory(std::vector<X509*> certs);
    bool VerifyHapSigner(Options* option)override;
    bool X509CertVerify(X509* cert, EVP_PKEY* privateKey);
    X509_REQ* GetCsr(EVP_PKEY* keyPair, std::string signAlg, std::string subject);
    int HandleIssuerKeyAliasEmpty(Options* options);
    int HandleIsserKeyAliasNotEmpty(Options* options);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGNTOOLSERVICELMPL_H
