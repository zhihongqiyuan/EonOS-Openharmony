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
#ifndef SIGNATRUETOOLS_SIGN_PROVIDER_H
#define SIGNATRUETOOLS_SIGN_PROVIDER_H

#include <sstream>
#include <set>
#include <vector>
#include <string>
#include <optional>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <iostream>
#include <utility>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>

#include "options.h"
#include "signature_tools_errno.h"
#include "signature_tools_log.h"
#include "signer_config.h"
#include "param_constants.h"
#include "byte_buffer.h"
#include "hap_utils.h"
#include "pkcs7_data.h"
#include "profile_verify.h"
#include "signature_info.h"

typedef std::tuple<std::shared_ptr<std::ifstream>, std::shared_ptr<std::ofstream>, std::string> fileIOTuple;
namespace OHOS {
namespace SignatureTools {
class SignProvider {
public:
    SignProvider() = default;
    virtual ~SignProvider() = default;
    bool Sign(Options* options);
    bool SignElf(Options* options);
    bool SetSignParams(Options* options, std::unordered_set<std::string>& paramSet);
    virtual std::optional<X509_CRL*> GetCrl();
    virtual bool CheckParams(Options* options);
    virtual bool CheckInputCertMatchWithProfile(X509* inputCert, X509* certInProfile)const;

protected:
    void CheckSignAlignment();
    X509* GetCertificate(const std::string& certificate)const;
    std::string GetCertificateCN(X509* cert)const;
    std::string FindProfileFromOptionalBlocks()const;
    int CheckProfileValid(STACK_OF(X509)* inputCerts);
    int CheckProfileInfo(const ProfileInfo& info, STACK_OF(X509)* inputCerts)const;
    int LoadOptionalBlocks();
    std::vector<OptionalBlock> optionalBlocks;
    std::map<std::string, std::string> signParams = std::map<std::string, std::string>();

private:
    bool InitSigerConfig(SignerConfig& signerConfig, STACK_OF(X509)* publicCerts, Options* options);

    bool CreateSignerConfigs(STACK_OF(X509)* certificates, const std::optional<X509_CRL*>& crl,
                             Options* options, SignerConfig&);

    bool CheckSignatureAlg();
    int LoadOptionalBlock(const std::string& file, int type);
    bool CheckFile(const std::string& filePath);

    int GetX509Certificates(Options* options, STACK_OF(X509)** ret);
    int GetPublicCerts(Options* options, STACK_OF(X509)** ret);
    int GetCertificateChainFromFile(const std::string& certChianFile, STACK_OF(X509)** ret);
    int GetCertListFromFile(const std::string& certsFile, STACK_OF(X509)** ret);

private:
    static std::vector<std::string> VALID_SIGN_ALG_NAME;
    static constexpr int FOUR_BYTE = 4;
    std::string profileContent;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGN_PROVIDER_H