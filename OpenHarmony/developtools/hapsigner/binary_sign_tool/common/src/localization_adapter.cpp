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
#include <string>
#include <cstring>

#include "localization_adapter.h"
#include "constant.h"
#include <openssl/evp.h>
#include <openssl/types.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>

namespace OHOS {
namespace SignatureTools {

LocalizationAdapter::LocalizationAdapter(Options* options)
{
    this->options = options;
    this->keyStoreHelper = std::make_unique<KeyStoreHelper>();
    this->isIssuerKeyStoreFile = false;
}

int LocalizationAdapter::IsAliasExist(const std::string& alias)
{
    std::string keyStoreFile = options->GetString(Options::KEY_STORE_FILE);
    if (!keyStoreHelper->IsKeyStoreFileExist(keyStoreFile)) {
        return RET_FAILED;
    }

    EVP_PKEY* keyPair = nullptr;
    char* keyStorePwd = options->GetChars(Options::KEY_STORE_RIGHTS);
    char* keyPwd = options->GetChars(Options::KEY_RIGHTS);
    keyStoreHelper->SetIsRegen(true);
    int status = keyStoreHelper->ReadKeyStore(keyStoreFile, keyStorePwd, alias, keyPwd, &keyPair);
    EVP_PKEY_free(keyPair);
    if (status == RET_OK) {
        return RET_OK;
    }

    return RET_FAILED;
}

void LocalizationAdapter::ResetPwd()
{
    char* keyRights = options->GetChars(Options::KEY_RIGHTS);
    if (keyRights != nullptr) {
        ResetChars(keyRights);
    }
    char* keyStoreRights = options->GetChars(Options::KEY_STORE_RIGHTS);
    if (keyStoreRights != nullptr) {
        ResetChars(keyStoreRights);
    }
    char* issuerKeyRights = options->GetChars(Options::ISSUER_KEY_RIGHTS);
    if (issuerKeyRights != nullptr) {
        ResetChars(issuerKeyRights);
    }
    char* issuerKeyStoreRights = options->GetChars(Options::ISSUER_KEY_STORE_RIGHTS);
    if (issuerKeyStoreRights != nullptr) {
        ResetChars(issuerKeyStoreRights);
    }
}

void LocalizationAdapter::ResetChars(char* chars)
{
    if (chars == NULL) {
        return;
    }
    for (size_t i = 0; i < strlen(chars); i++) {
        chars[i] = 0;
    }
}

EVP_PKEY* LocalizationAdapter::GetAliasKey(bool autoCreate)
{
    EVP_PKEY* keyPair = nullptr;
    if (keyStoreHelper == nullptr) {
        keyStoreHelper = std::make_unique<KeyStoreHelper>();
    }

    int status = GetKeyPair(autoCreate, &keyPair);
    if (status == RET_FAILED) {
        EVP_PKEY_free(keyPair);
        return nullptr;
    }

    return keyPair;
}

int LocalizationAdapter::GetKeyPair(bool autoCreate, EVP_PKEY** keyPair)
{
    keyStoreHelper->SetPassWordStatus(true);
    keyStoreHelper->SetIsRegen(autoCreate);

    int status = RET_FAILED;
    if (isIssuerKeyStoreFile) {
        status = IssuerKeyStoreFile(keyPair, autoCreate);
    } else {
        status = KeyStoreFile(keyPair, autoCreate);
    }
    isIssuerKeyStoreFile = false;
    return status;
}

int LocalizationAdapter::KeyStoreFile(EVP_PKEY** keyPair, bool autoCreate)
{
    std::string keyStorePath = "";
    keyStorePath = options->GetString(Options::KEY_STORE_FILE);
    char* keyStorePwd = options->GetChars(Options::KEY_STORE_RIGHTS);
    char* keyPwd = options->GetChars(Options::KEY_RIGHTS);
    std::string keyAlias = options->GetString(Options::KEY_ALIAS);
    bool fileStatus = keyStoreHelper->IsKeyStoreFileExist(keyStorePath);
    if (fileStatus) {
        int status = keyStoreHelper->ReadKeyStore(keyStorePath, keyStorePwd, keyAlias, keyPwd, keyPair);
        if (status == RET_OK) {
            return RET_OK;
        }
        
        if (!keyStoreHelper->GetPassWordStatus()) {
                autoCreate = false;
        }
    }
    if (autoCreate) {
        std::string keyAlg = options->GetString(Options::KEY_ALG);
        int keySize = options->GetInt(Options::KEY_SIZE);
        *keyPair = keyStoreHelper->GenerateKeyPair(keyAlg, keySize);
        int status = keyStoreHelper->WriteKeyStore(*keyPair, keyStorePath, keyStorePwd, keyAlias, keyPwd);
        if (status == RET_OK) {
            PrintMsg("Remind: generate new keypair ,the keyalias is " + keyAlias + " !");
            return RET_OK;
        }
    }

    return RET_FAILED;
}

int LocalizationAdapter::IssuerKeyStoreFile(EVP_PKEY** keyPair, bool autoCreate)
{
    std::string keyStore = options->GetString(Options::ISSUER_KEY_STORE_FILE);
    char* keyStorePwd = options->GetChars(Options::ISSUER_KEY_STORE_RIGHTS);
    std::string keyAlias = options->GetString(Options::ISSUER_KEY_ALIAS);
    char* keyPwd = options->GetChars(Options::ISSUER_KEY_RIGHTS);

    if (keyStore.empty()) {
        keyStore = options->GetString(Options::KEY_STORE_FILE);
        keyStorePwd = options->GetChars(Options::KEY_STORE_RIGHTS);
    }

    bool fileStatus = keyStoreHelper->IsKeyStoreFileExist(keyStore);
    if (fileStatus) {
        int status = keyStoreHelper->ReadKeyStore(keyStore, keyStorePwd, keyAlias, keyPwd, keyPair);
        if (status == RET_OK) {
            return RET_OK;
        }

        if (!keyStoreHelper->GetPassWordStatus()) {
                autoCreate = false;
        }
    }

    if (!fileStatus && !keyStore.empty() && !autoCreate) {
        PrintErrorNumberMsg("KEY_ALIAS_ERROR", KEY_ALIAS_ERROR, "keyAlias: '"
                            + keyAlias + "' is not exist in" + keyStore);
    }

    if (autoCreate) {
        std::string keyAlg = options->GetString(Options::KEY_ALG);
        int keySize = options->GetInt(Options::KEY_SIZE);
        *keyPair = keyStoreHelper->GenerateKeyPair(keyAlg, keySize);
        if (keyStore.empty()) {
            return keyStoreHelper->WriteKeyStore(*keyPair, keyStore, keyStorePwd, keyAlias, keyPwd);
        }
    }

    return RET_FAILED;
}

void LocalizationAdapter::SetIssuerKeyStoreFile(bool issuerKeyStoreFile)
{
    this->isIssuerKeyStoreFile = issuerKeyStoreFile;
}

STACK_OF(X509)* LocalizationAdapter::GetSignCertChain()
{
    STACK_OF(X509)* certificates = NULL;
    std::string certPath = options->GetString(Options::PROFILE_CERT_FILE);
    if (certPath.empty()) {
        certPath = options->GetString(Options::APP_CERT_FILE);
    }
    certificates = sk_X509_new(NULL);
    if (certificates == NULL) {
        SIGNATURE_TOOLS_LOGE("sk_X509_new failed");
        return  NULL;
    }
    std::vector<X509*> certs = GetCertsFromFile(certPath, Options::PROFILE_CERT_FILE);
    for (int i = 0; i < static_cast<int>(certs.size()); i++) {
        sk_X509_push(certificates, certs[i]);
    }
    if (sk_X509_num(certificates) < MIN_CERT_CHAIN_SIZE || sk_X509_num(certificates) > MAX_CERT_CHAIN_SIZE) {
        SIGNATURE_TOOLS_LOGE("Profile cert '%s' must a cert chain", certPath.c_str());
        goto err;
    }
    return certificates;
err:
    sk_X509_pop_free(certificates, X509_free);
    return NULL;
}

EVP_PKEY* LocalizationAdapter::GetIssuerKeyByAlias()
{
    return GetAliasKey(false);
}

bool LocalizationAdapter::IsOutFormChain()
{
    std::string checkStr = OUT_FORM_CERT_CHAIN;
    std::string outForm = options->GetString(Options::OUT_FORM, checkStr);
    if (outForm.compare(OUT_FORM_CERT_CHAIN) == 0) {
        return true;
    }
    return false;
}

X509* LocalizationAdapter::GetSubCaCertFile()
{
    std::string certPath = options->GetString(Options::SUB_CA_CERT_FILE);
    return GetCertsFromFile(certPath, Options::SUB_CA_CERT_FILE).at(0);
}

const std::string LocalizationAdapter::GetSignAlg() const
{
    return options->GetString(Options::SIGN_ALG);
}

X509* LocalizationAdapter::GetCaCertFile()
{
    std::string certPath = options->GetString(Options::CA_CERT_FILE);
    return GetCertsFromFile(certPath, Options::CA_CERT_FILE).at(0);
}

const std::string LocalizationAdapter::GetOutFile()
{
    return options->GetString(Options::OUT_FILE);
}

std::vector<X509*> LocalizationAdapter::GetCertsFromFile(std::string& certPath, const std::string& logTitle)
{
    SIGNATURE_TOOLS_LOGD("outPutPath = %s , logTitle = %s", certPath.c_str(), logTitle.c_str());
    std::vector<X509*> certs;
    if (certPath.empty()) {
        SIGNATURE_TOOLS_LOGE("cert path not exist!");
        return certs;
    }
    // Read And Get Cert
    BIO* bio = BIO_new_file(certPath.c_str(), "rb");
    if (!bio) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "open file:" + certPath + "failed");
        DigestCommon::GetOpensslErrorMessage();
        BIO_free(bio);
        return certs;
    }
    X509* cert = nullptr;
    while ((cert = PEM_read_bio_X509(bio, NULL, NULL, NULL)) != nullptr) {
        certs.emplace_back(cert);
    }
    BIO_free(bio);
    return certs;
}

const std::string LocalizationAdapter::GetInFile()
{
    return options->GetString(Options::IN_FILE);
}

bool LocalizationAdapter::IsRemoteSigner()
{
    std::string mode = options->GetString(Options::MODE, LOCAL_SIGN);
    return mode == REMOTE_SIGN;
}

Options* LocalizationAdapter::GetOptions()
{
    return options;
}

void LocalizationAdapter::AppAndProfileAssetsRealse(std::initializer_list<EVP_PKEY*> keys,
                                                    std::initializer_list<X509_REQ*> reqs,
                                                    std::initializer_list<X509*> certs)
{
    for (auto cert : certs) {
        if (cert) {
            X509_free(cert);
            cert = nullptr;
        }
    }
    for (auto req : reqs) {
        if (req) {
            X509_REQ_free(req);
            req = nullptr;
        }
    }
    for (auto key : keys) {
        if (key) {
            EVP_PKEY_free(key);
            key = nullptr;
        }
    }
}

} // namespace SignatureTools
} // namespace OHOS

