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

#ifndef SIGNATRUETOOLS_LOCALIIZATION_ADAPTER_H
#define SIGNATRUETOOLS_LOCALIIZATION_ADAPTER_H

#include <memory>
#include <string>

#include "openssl/ssl.h"
#include "openssl/pem.h"
#include "openssl/err.h"
#include "options.h"
#include "key_store_helper.h"
#include "signature_tools_log.h"
#include "digest_common.h"
namespace OHOS {
namespace SignatureTools {
class LocalizationAdapter {
public:
    LocalizationAdapter() = default;
    LocalizationAdapter(Options* options);
    ~LocalizationAdapter() = default;

    int IsAliasExist(const std::string& alias);
    int GetKeyPair(bool autoCreate, EVP_PKEY** keyPair);
    int IssuerKeyStoreFile(EVP_PKEY** keyPair, bool autoCreate);
    int KeyStoreFile(EVP_PKEY** keyPair, bool autoCreate);

    void ResetPwd();
    void SetIssuerKeyStoreFile(bool issuerKeyStoreFile);
    void AppAndProfileAssetsRealse(std::initializer_list<EVP_PKEY*> keys,
                                   std::initializer_list<X509_REQ*> reqs,
                                   std::initializer_list<X509*> certs);

    bool IsOutFormChain();
    bool IsRemoteSigner();

    const std::string GetSignAlg() const;
    const std::string GetOutFile();
    const std::string GetInFile();
    
    Options* GetOptions();
    EVP_PKEY* GetAliasKey(bool autoCreate);
    EVP_PKEY* GetIssuerKeyByAlias();
    X509* GetSubCaCertFile();
    X509* GetCaCertFile();
    STACK_OF(X509*) GetSignCertChain();
    std::vector<X509*> GetCertsFromFile(std::string& certPath, const std::string& logTitle);

private:
    void ResetChars(char* chars);

public:
    Options* options;
    std::unique_ptr<KeyStoreHelper> keyStoreHelper;

private:
    static constexpr int MIN_CERT_CHAIN_SIZE = 2;
    static constexpr int MAX_CERT_CHAIN_SIZE = 3;
    bool isIssuerKeyStoreFile;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_LOCALIIZATION_ADAPTER_H
