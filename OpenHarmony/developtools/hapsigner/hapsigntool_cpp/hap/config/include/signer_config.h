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
#ifndef SIGNATRUETOOLS_SIGNER_CONFIG_H
#define SIGNATRUETOOLS_SIGNER_CONFIG_H
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "options.h"
#include "openssl/x509.h"
#include "signature_algorithm_helper.h"
#include "signer.h"

namespace OHOS {
namespace SignatureTools {
class SignerConfig {
public:
    SignerConfig();
    ~SignerConfig();
    Options* GetOptions() const;
    void SetOptions(Options* optionsParam);
    STACK_OF(X509)* GetCertificates() const;
    void SetCertificates(STACK_OF(X509)* certificatesParam);
    STACK_OF(X509_CRL)* GetX509CRLs() const;
    void SetX509CRLs(STACK_OF(X509_CRL)* crls);
    std::vector<SignatureAlgorithmHelper> GetSignatureAlgorithms() const;
    void SetSignatureAlgorithms(const std::vector<SignatureAlgorithmHelper>& signatureAlgorithmsParam);
    const std::map<std::string, std::string>& GetSignParamMap() const;
    void FillParameters(const std::map<std::string, std::string>& params);
    std::shared_ptr<Signer> GetSigner();
    int GetCompatibleVersion() const;
    void SetCompatibleVersion(int compatibleVersionParam);

private:
    bool IsInputCertChainNotEmpty() const;
    bool IsInputCrlNotEmpty() const;
    Options* options;
    STACK_OF(X509)* certificates;
    STACK_OF(X509_CRL)* x509CRLs;
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    std::map<std::string, std::string> signParamMap;
    std::shared_ptr<Signer> signer;
    int compatibleVersion;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGNER_CONFIG_H
