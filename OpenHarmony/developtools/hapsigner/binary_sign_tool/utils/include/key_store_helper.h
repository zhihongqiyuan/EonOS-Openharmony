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
#ifndef SIGNATRUETOOLS_KEYSTOREHELPER_H
#define SIGNATRUETOOLS_KEYSTOREHELPER_H
#include <string>
#include "openssl/ssl.h"
#include "openssl/pem.h"
#include "openssl/evp.h"
#include "openssl/pkcs12.h"
#include "openssl/pkcs7.h"
#include "signature_tools_log.h"
#include "verify_hap_openssl_utils.h"

namespace OHOS {
namespace SignatureTools {
class KeyStoreHelper {
public:
    KeyStoreHelper();
    ~KeyStoreHelper() = default;
    int WriteKeyStore(EVP_PKEY* evpPkey, std::string& keyStorePath, char* keyStorePwd,
                    std::string alias, char* keyPwd);
    int ReadKeyStore(std::string& keyStorePath, char* keyStorePwd, const std::string& alias,
                    char* keyPwd, EVP_PKEY** evpPkey);
    int GetPublicKey(PKCS7* safe, const std::string& alias, char* pass, int passlen, EVP_PKEY** publickey);
    int GetPrivateKey(PKCS7* safe, const std::string& alias, char* pass, int passlen, EVP_PKEY** keyPiar);
    int SetCertPkcs12(X509* cert, PKCS12_SAFEBAG* bag, STACK_OF(PKCS12_SAFEBAG)* certBags, unsigned char* keyId,
                    unsigned int keyIdLen, const char* name, STACK_OF(PKCS7)** safes,
                    int certNid, int iter, const char* keyStorePwd);
    int SetPkeyPkcs12(EVP_PKEY* pkey, PKCS12_SAFEBAG* bag, STACK_OF(PKCS12_SAFEBAG)* keyBags,
                    const char* name, STACK_OF(PKCS7)** safes, int iter, const char* keyPwd,
                    int keyType, int keyNid, unsigned char* keyId, unsigned int keyIdLen);
    int GetAttrNid(PKCS12_SAFEBAG* bag, EVP_PKEY* pkey, int nid);
    int FindKeyPair(PKCS12* p12, const std::string& alias, char* keyPwd, char* keyStorePwd,
                    EVP_PKEY** keyPiar, const std::string& keyStorePath);
    int ParsePkcs12Safebag(PKCS12_SAFEBAG* bag, const char* pass, int passlen, STACK_OF(X509)* ocerts);
    int ParsePkcs12Safebags(const STACK_OF(PKCS12_SAFEBAG)* bags, const char* pass,
                            int passlen, STACK_OF(X509)* ocerts);
    int Pkcs12PasswordParse(PKCS12* p12, const char* keyStorePwd, const std::string& keyStoreFile);
    int CreatePKCS12(PKCS12** p12, const std::string& charsStorePath, char* keyStorePwd,
                    char* keyPwd, const std::string& charsAlias, EVP_PKEY* evpPkey, X509* cert);

    bool InitX509(X509& cert, EVP_PKEY& evpPkey);
    bool IsKeyStoreFileExist(std::string& keyStorePath);
    bool SetX509Alias(int len, X509* x509, unsigned char* data);
    bool GetPassWordStatus();

    void SetNidMac(int& nidKey, int& iter, int& macStatus);
    void SetPassWordStatus(bool status);
    void SetIsRegen(bool autoCreate);

    EVP_PKEY* GenerateKeyPair(const std::string& algorithm, int keySize);
    PKCS12* CreatePKCS12(const char* keyStorePwd, const char* keyPwd, const char* name, EVP_PKEY* pkey, X509* cert,
                        int keyNid, int certNid, int iter, int macStatus, int keyType, STACK_OF(PKCS7)** safes);

private:
    void KeyPairFree(EC_GROUP* group, EC_KEY* pkey, const std::string& Message);
    void KeyPairFree(BIGNUM* bnSerial, X509_NAME* issuerName, X509_NAME* subjectName,
                    ASN1_INTEGER* ai, const std::string& Message);
    void KeyPairFree(X509* cert, PKCS12* p12, BIO* bioOut, const std::string& Message);
    void KeyPairFree(STACK_OF(X509)* ocerts, STACK_OF(PKCS12_SAFEBAG)* bags, char* name);
    void KeyPairFree(STACK_OF(PKCS7)* safes, EVP_PKEY* publickey);
    void KeyPairFree(STACK_OF(PKCS12_SAFEBAG)* bags, PKCS8_PRIV_KEY_INFO* p8, char* name);
    void ResetKeyStatusvariable();
    void ResePwdLenvariable();
    void PrintAliasExistErrorMsg(const std::string& alias, const std::string& keyStorePath);
    void SetPwdLenKeyStatus(char* pass, char* keyPass);

private:
    const int NID_PBE_CBC = 149;
    const int NID_TRIPLEDES_CBC = 146;
    
    bool passWordStatus;
    bool isRegen;

    int keyStorePwdLen;
    int keyPairPwdLen;
    int publicKeyStatus;
    int privateKeyStatus;
};
} // namespace SignatureTools
} // namespace OHOS
#endif
