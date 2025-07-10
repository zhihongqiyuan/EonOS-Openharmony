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
#include <iostream>
#include <openssl/x509_vfy.h>

#include "signer_factory.h"
#include "localization_adapter.h"
#include "signer_config.h"

namespace OHOS {
namespace SignatureTools {
SignerConfig::SignerConfig() : options(nullptr),
    certificates(nullptr),
    x509CRLs(nullptr),
    signer(nullptr),
    compatibleVersion(0)
{
}

SignerConfig::~SignerConfig()
{
    if (certificates) {
        sk_X509_pop_free(certificates, X509_free);
    }
    certificates = NULL;

    if (x509CRLs) {
        sk_X509_CRL_pop_free(x509CRLs, X509_CRL_free);
    }
    x509CRLs = NULL;
}

Options* SignerConfig::GetOptions() const
{
    return options;
}

void SignerConfig::SetOptions(Options* optionsParam)
{
    options = optionsParam;
}

STACK_OF(X509)* SignerConfig::GetCertificates() const
{
    if (IsInputCertChainNotEmpty() || signer == nullptr) {
        return certificates;
    }
    return signer->GetCertificates();
}

void SignerConfig::SetCertificates(STACK_OF(X509)* certificatesParam)
{
    certificates = certificatesParam;
}

STACK_OF(X509_CRL)* SignerConfig::GetX509CRLs() const
{
    if (IsInputCertChainNotEmpty() || IsInputCrlNotEmpty() || signer == nullptr) {
        return x509CRLs;
    }
    return signer->GetCrls();
}

void SignerConfig::SetX509CRLs(STACK_OF(X509_CRL)* crls)
{
    x509CRLs = crls;
}

std::vector<SignatureAlgorithmHelper> SignerConfig::GetSignatureAlgorithms() const
{
    return signatureAlgorithms;
}

void SignerConfig::SetSignatureAlgorithms(const std::vector<SignatureAlgorithmHelper>& signatureAlgorithmsParam)
{
    signatureAlgorithms = signatureAlgorithmsParam;
}

const std::map<std::string, std::string>& SignerConfig::GetSignParamMap() const
{
    return signParamMap;
}

void SignerConfig::FillParameters(const std::map<std::string, std::string>& params)
{
    signParamMap = params;
}

std::shared_ptr<Signer> SignerConfig::GetSigner()
{
    if (signer == nullptr) {
        SignerFactory factory;
        LocalizationAdapter adapter(options);
        signer = factory.GetSigner(adapter);
    }
    return signer;
}

int SignerConfig::GetCompatibleVersion() const
{
    return compatibleVersion;
}

void SignerConfig::SetCompatibleVersion(int compatibleVersionParam)
{
    compatibleVersion = compatibleVersionParam;
}

bool SignerConfig::IsInputCertChainNotEmpty() const
{
    return certificates != nullptr;
}

bool SignerConfig::IsInputCrlNotEmpty() const
{
    return x509CRLs != nullptr;
}
} // namespace SignatureTools
} // namespace OHOS