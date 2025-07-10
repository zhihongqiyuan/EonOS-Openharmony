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
#include "key_store_helper.h"
#include <cstring>
#include "openssl/err.h"
#include "constant.h"
#include "signature_tools_errno.h"

namespace OHOS {
namespace SignatureTools {
KeyStoreHelper::KeyStoreHelper()
{
    passWordStatus = true;
    keyPairPwdLen = 0;
    keyStorePwdLen = 0;
    publicKeyStatus = RET_FAILED;
    privateKeyStatus = RET_FAILED;
}

void KeyStoreHelper::SetPassWordStatus(bool status)
{
    passWordStatus = status;
}

void KeyStoreHelper::SetIsRegen(bool autoCreate)
{
    isRegen = autoCreate;
}

bool KeyStoreHelper::GetPassWordStatus()
{
    return passWordStatus;
}

void KeyStoreHelper::ResetKeyStatusvariable()
{
    publicKeyStatus = RET_FAILED;
    privateKeyStatus = RET_FAILED;
}

void KeyStoreHelper::ResePwdLenvariable()
{
    keyPairPwdLen = 0;
    keyStorePwdLen = 0;
}

void KeyStoreHelper::PrintAliasExistErrorMsg(const std::string& alias, const std::string& keyStorePath)
{
    if (!isRegen) {
        PrintErrorNumberMsg("KEY_ALIAS_ERROR", KEY_ALIAS_ERROR, "keyAlias: '"
                            + alias + "' is not exist in" + keyStorePath);
    }
}

void KeyStoreHelper::KeyPairFree(EC_GROUP* group, EC_KEY* pkey, const std::string& Message)
{
    if (!Message.empty()) {
        SIGNATURE_TOOLS_LOGE("%s", Message.c_str());
    }

    EC_GROUP_free(group);
    group = nullptr;

    EC_KEY_free(pkey);
    pkey = nullptr;
}

void KeyStoreHelper::KeyPairFree(BIGNUM* bnSerial, X509_NAME* issuerName, X509_NAME* subjectName,
                                 ASN1_INTEGER* ai, const std::string& Message)
{
    if (!Message.empty()) {
        SIGNATURE_TOOLS_LOGE("%s", Message.c_str());
    }

    BN_free(bnSerial);
    bnSerial = nullptr;

    ASN1_INTEGER_free(ai);
    ai = nullptr;

    X509_NAME_free(issuerName);
    issuerName = nullptr;

    X509_NAME_free(subjectName);
    subjectName = nullptr;
}

void KeyStoreHelper::KeyPairFree(X509* cert, PKCS12* p12, BIO* bioOut, const std::string& Message)
{
    if (!Message.empty()) {
        SIGNATURE_TOOLS_LOGE("%s", Message.c_str());
    }

    X509_free(cert);
    cert = nullptr;

    PKCS12_free(p12);
    p12 = nullptr;

    BIO_free_all(bioOut);
    bioOut = nullptr;
}

void KeyStoreHelper::KeyPairFree(STACK_OF(X509)* ocerts, STACK_OF(PKCS12_SAFEBAG)* bags, char* name)
{
    sk_X509_pop_free(ocerts, X509_free);
    ocerts = nullptr;

    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
    bags = nullptr;

    free(name);
    name = nullptr;
}

void KeyStoreHelper::KeyPairFree(STACK_OF(PKCS7)* safes, EVP_PKEY* publickey)
{
    sk_PKCS7_pop_free(safes, PKCS7_free);
    safes = nullptr;

    EVP_PKEY_free(publickey);
    publickey = nullptr;
}

void KeyStoreHelper::KeyPairFree(STACK_OF(PKCS12_SAFEBAG)* bags, PKCS8_PRIV_KEY_INFO* p8, char* name)
{
    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
    bags = nullptr;

    PKCS8_PRIV_KEY_INFO_free(p8);
    p8 = nullptr;

    free(name);
    name = nullptr;
}

bool KeyStoreHelper::InitX509(X509& cert, EVP_PKEY& evpPkey)
{
    BIGNUM* bnSerial = BN_new();
    X509_NAME* issuerName = X509_NAME_new();
    const EVP_MD* md = EVP_sha256();
    X509_NAME* subjectName = nullptr;
    if (!bnSerial || !issuerName || !md) {
        KeyPairFree(bnSerial, issuerName, subjectName, nullptr, "Failed to initialize the x509 info.");
        return false;
    }
    ASN1_INTEGER* ai = BN_to_ASN1_INTEGER(bnSerial, NULL);
    if (ai == NULL || issuerName == NULL) {
        KeyPairFree(bnSerial, issuerName, subjectName, ai, "Failed to initialize the x509 structure.");
        return false;
    }
    X509_set_serialNumber(&cert, ai);
    X509_gmtime_adj(X509_get_notBefore(&cert), 0);
    X509_gmtime_adj(X509_get_notAfter(&cert), (long)DEFAULT_VALIDITY_DAYS * ONE_DAY_TIME);
    if (!X509_NAME_add_entry_by_txt(issuerName, "C",
        MBSTRING_ASC, reinterpret_cast<const unsigned char*>("US"), -1, -1, 0)
        || !X509_NAME_add_entry_by_txt(issuerName, "O",
        MBSTRING_ASC, reinterpret_cast<const unsigned char*>("My Company"), -1, -1, 0)
        || !X509_NAME_add_entry_by_txt(issuerName, "CN",
        MBSTRING_ASC, reinterpret_cast<const unsigned char*>("My Issuer"), -1, -1, 0)) {
        KeyPairFree(bnSerial, issuerName, subjectName, ai,
                    "Failed to initialize the x509 structure.X509_NAME type");
        return false;
    }
    X509_set_issuer_name(&cert, issuerName);
    subjectName = X509_NAME_dup(issuerName);
    if (subjectName == NULL) {
        KeyPairFree(bnSerial, issuerName, subjectName, ai,
                    "Failed to initialize the x509 structure.X509_NAME type");
        return false;
    }
    X509_set_subject_name(&cert, subjectName);
    if (!X509_set_pubkey(&cert, &evpPkey)) {
        KeyPairFree(bnSerial, issuerName, subjectName, ai,
                    "Failed to initialize the x509 structure.X509_NAME type");
        return false;
    }
    X509_set_version(&cert, DEFAULT_CERT_VERSION);
    if (!X509_sign(&cert, &evpPkey, md)) {
        KeyPairFree(bnSerial, issuerName, subjectName, ai,
                    "Failed to initialize the x509 structure.X509_NAME type");
        return false;
    }
    KeyPairFree(bnSerial, issuerName, subjectName, ai, "");
    return true;
}

void KeyStoreHelper::SetPwdLenKeyStatus(char* pass, char* keyPass)
{
    ResePwdLenvariable();
    ResetKeyStatusvariable();

    if (pass != nullptr) {
        keyStorePwdLen = strlen(pass);
    }

    if (keyPass != nullptr) {
        keyPairPwdLen = strlen(keyPass);
    }
}

/*
* Function: Find the key pair in the PKCS12 structure by alias.
* Annotation: This function is to modify the openSSL interface "PKCS12_parse" and "find_friendly_name" functions,
*             To fit java generated p12 files.
* find_friendly_name interface: Look for aliases in PKCS12 structures, one key pair for each PKCS12 structure.
* PKCS12_parse interface: Parse the PKCS12 structure, and obtain the key pair,
*                         one PKCS12 structure corresponds to one key pair.
* FindKeyPair interface: Get the stack structure "STACK_OF(PKCS7)" of PKCS7 in PKCS12 structure, and then traverse
*                        the "STACK_OF(PKCS7)" structure to get the public key and the private key by alias.
**/
int KeyStoreHelper::FindKeyPair(PKCS12* p12, const std::string& alias, char* keyPwd,
                                char* keyStorePwd, EVP_PKEY** keyPiar, const std::string& keyStorePath)
{
    EVP_PKEY* publickey = nullptr;
    STACK_OF(PKCS7)* safes = nullptr;
    PKCS7* safe = nullptr;

    SetPwdLenKeyStatus(keyStorePwd, keyPwd);

    if ((safes = PKCS12_unpack_authsafes(p12)) == NULL) {
        sk_PKCS7_pop_free(safes, PKCS7_free);
        return RET_FAILED;
    }

    for (int n = 0; n < sk_PKCS7_num(safes); n++) {
        if ((publicKeyStatus == RET_OK) && (privateKeyStatus == RET_OK)) {
            break;
        }

        safe = sk_PKCS7_value(safes, n);
        if (OBJ_obj2nid(safe->type) == NID_pkcs7_encrypted) {
            if (publicKeyStatus != RET_OK) {
                publicKeyStatus = GetPublicKey(safe, alias, keyStorePwd, keyStorePwdLen, &publickey);
            }

            if (!GetPassWordStatus()) {
                KeyPairFree(safes, publickey);
                return RET_FAILED;
            }
        } else if (OBJ_obj2nid(safe->type) == NID_pkcs7_data && privateKeyStatus != RET_OK) {
            privateKeyStatus = GetPrivateKey(safe, alias, keyPwd, keyPairPwdLen, keyPiar);
            if (!GetPassWordStatus()) {
                KeyPairFree(safes, publickey);
                return RET_FAILED;
            }
        }
    }

    if (((publicKeyStatus == RET_OK) && (privateKeyStatus == RET_OK))
        && (publickey != nullptr) && (*keyPiar != nullptr)) {
        if (EVP_PKEY_copy_parameters(*keyPiar, publickey) != 1) {
            KeyPairFree(safes, publickey);
            SIGNATURE_TOOLS_LOGE("publickey and privatekey set EVP_PKEY struct failed");
            return RET_FAILED;
        }

        KeyPairFree(safes, publickey);
        return RET_OK;
    }
    PrintAliasExistErrorMsg(alias, keyStorePath);
    KeyPairFree(safes, publickey);
    return RET_FAILED;
}

int KeyStoreHelper::GetPublicKey(PKCS7* safe, const std::string& alias, char* pass, int passlen, EVP_PKEY** publickey)
{
    char* name = NULL;
    PKCS12_SAFEBAG* bag = nullptr;
    STACK_OF(PKCS12_SAFEBAG)* bags = nullptr;
    STACK_OF(X509)* ocerts = sk_X509_new_null();

    bags = PKCS12_unpack_p7encdata(safe, pass, passlen);
    if (bags == nullptr) {
        PrintErrorNumberMsg("KEY_PASSWORD_ERROR", KEY_PASSWORD_ERROR, "'" + alias + "' keypair password error");
        KeyPairFree(ocerts, bags, name);
        SetPassWordStatus(false);
        return RET_FAILED;
    }

    if (ParsePkcs12Safebags(bags, pass, passlen, ocerts) == RET_FAILED) {
        PrintErrorNumberMsg("KEY_PASSWORD_ERROR", KEY_PASSWORD_ERROR, "'" + alias + "' keypair password error");
        KeyPairFree(ocerts, bags, name);
        SetPassWordStatus(false);
        return RET_FAILED;
    }
    for (int i = 0; i < sk_X509_num(ocerts); i++) {
        bag = sk_PKCS12_SAFEBAG_value(bags, i);
        name = PKCS12_get_friendlyname(bag);
        if (strcmp(name, alias.c_str()) != 0) {
            continue;
        }
        X509* cert = sk_X509_value(ocerts, i);
        if (cert == nullptr) {
            KeyPairFree(ocerts, bags, name);
            return RET_FAILED;
        }
        *publickey = X509_get_pubkey(cert);
        if (*publickey != nullptr) {
            KeyPairFree(ocerts, bags, name);
            return RET_OK;
        }
    }

    KeyPairFree(ocerts, bags, name);
    return RET_FAILED;
}

int KeyStoreHelper::GetPrivateKey(PKCS7* safe, const std::string& alias, char* pass, int passlen, EVP_PKEY** keyPiar)
{
    STACK_OF(PKCS12_SAFEBAG)* bags = nullptr;
    PKCS12_SAFEBAG* bag = nullptr;
    PKCS8_PRIV_KEY_INFO* p8 = nullptr;
    char* name = NULL;

    bags = PKCS12_unpack_p7data(safe);
    for (int m = 0; m < sk_PKCS12_SAFEBAG_num(bags); m++) {
        bag = sk_PKCS12_SAFEBAG_value(bags, m);
        if (PKCS12_SAFEBAG_get_nid(bag) != NID_pkcs8ShroudedKeyBag) {
            continue;
        }
        name = PKCS12_get_friendlyname(bag);
        if (strcmp(name, alias.c_str()) != 0) {
            continue;
        }
        if ((p8 = PKCS12_decrypt_skey(bag, pass, passlen)) == NULL) {
            PrintErrorNumberMsg("KEY_PASSWORD_ERROR", KEY_PASSWORD_ERROR, "'" + alias
                                + "' keypair password error");
            KeyPairFree(bags, p8, name);
            SetPassWordStatus(false);
            return RET_FAILED;
        }
        *keyPiar = EVP_PKCS82PKEY(p8);
        if (*keyPiar == NULL) {
            KeyPairFree(bags, p8, name);
            return RET_FAILED;
        }

        KeyPairFree(bags, p8, name);
        return RET_OK;
    }

    KeyPairFree(bags, p8, name);
    return RET_FAILED;
}

int KeyStoreHelper::WriteKeyStore(EVP_PKEY* evpPkey, std::string& keyStorePath,
                                  char* keyStorePwd, std::string alias, char* keyPwd)
{
    X509* cert = X509_new();
    PKCS12* p12 = nullptr;
    BIO* bioOut = nullptr;

    if (evpPkey == nullptr) {
        KeyPairFree(cert, p12, bioOut, "The key pair pointer is null");
        return RET_FAILED;
    }

    if (!InitX509(*cert, *evpPkey)) {
        KeyPairFree(cert, p12, bioOut, "initialize x509 structure failed");
        return RET_FAILED;
    }

    if (CreatePKCS12(&p12, keyStorePath, keyStorePwd, keyPwd, alias.c_str(), evpPkey, cert) == RET_FAILED) {
        KeyPairFree(cert, p12, bioOut, "Create PKCS12 Structure Failed");
        return RET_FAILED;
    }

    bioOut = BIO_new_file(keyStorePath.c_str(), "wb");
    if (bioOut == nullptr) {
        KeyPairFree(cert, p12, bioOut, "Open file: '" + keyStorePath + "' failed");
        return RET_FAILED;
    }

    if (i2d_PKCS12_bio(bioOut, p12) != 1) {
        KeyPairFree(cert, p12, bioOut, "PKCS12 structure write File failure");
        return RET_FAILED;
    }

    KeyPairFree(cert, p12, bioOut, "");
    return RET_OK;
}

/*
* Function: Create a PKCS12 structured keystore, If the key stock is available, the key pair is appended.
* Annotation: This function is a modification of the OpenSSL "PKCS12_create" function, To fit java generated p12 files.
* PKCS12_create interface: A p12 file holds multiple PKCS12 structures, and a structure holds a key pair.
* CreatePKCS12 interface: A p12 file stores a PKCS12 structure,a PKCS12 structure stores multiplePKCS7 structures,
*                         and a PKCS7 structure corresponds to a key pair.
**/
int KeyStoreHelper::CreatePKCS12(PKCS12** p12, const std::string& charsStorePath, char* keyStorePwd,
                                 char* keyPwd, const std::string& charsAlias, EVP_PKEY* evpPkey, X509* cert)
{
    STACK_OF(PKCS7)* safes = nullptr;
    PKCS12* acceptP12 = nullptr;
    BIO* bioOut = BIO_new_file(charsStorePath.c_str(), "rb");
    if (bioOut != nullptr) {
        acceptP12 = d2i_PKCS12_bio(bioOut, NULL);
        if (acceptP12 == nullptr) {
            return RET_FAILED;
        }
        if (Pkcs12PasswordParse(acceptP12, keyStorePwd, charsStorePath) == RET_FAILED) {
            BIO_free_all(bioOut);
            return RET_FAILED;
        }
        safes = PKCS12_unpack_authsafes(acceptP12);
    }

    BIO_free_all(bioOut);
    if (keyStorePwd == nullptr) {
        *p12 = CreatePKCS12(keyStorePwd, keyPwd, charsAlias.c_str(), evpPkey, cert, 0, 0, 0, -1, 0, &safes);
    } else {
        *p12 = CreatePKCS12(keyStorePwd, keyPwd, charsAlias.c_str(), evpPkey, cert, 0, 0, 0, 0, 0, &safes);
    }

    sk_PKCS7_pop_free(safes, PKCS7_free);
    safes = nullptr;
    PKCS12_free(acceptP12);

    if (*p12 == nullptr) {
        return RET_FAILED;
    }
    return RET_OK;
}

int KeyStoreHelper::ReadKeyStore(std::string& keyStorePath, char* keyStorePwd, const std::string& alias,
                                 char* keyPwd, EVP_PKEY** evpPkey)
{
    X509* cert = nullptr;
    PKCS12* p12 = nullptr;
    BIO* bioOut = nullptr;

    bioOut = BIO_new_file(keyStorePath.c_str(), "rb");
    if (bioOut == nullptr) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        KeyPairFree(cert, p12, bioOut, "Open file: '" + keyStorePath + "' failed");
        return RET_FAILED;
    }

    p12 = d2i_PKCS12_bio(bioOut, NULL);
    if (Pkcs12PasswordParse(p12, keyStorePwd, keyStorePath) == RET_FAILED) {
        KeyPairFree(cert, p12, bioOut, "");
        SetPassWordStatus(false);
        return RET_FAILED;
    }
    int status = FindKeyPair(p12, alias, keyPwd, keyStorePwd, evpPkey, keyStorePath);
    if (status == RET_FAILED) {
        KeyPairFree(cert, p12, bioOut, "");
        return RET_FAILED;
    }

    KeyPairFree(cert, p12, bioOut, "");
    return RET_OK;
}

int KeyStoreHelper::Pkcs12PasswordParse(PKCS12* p12, const char* keyStorePwd, const std::string& keyStoreFile)
{
    if (p12 == NULL) {
        PrintErrorNumberMsg("KEYSTORE_STRUCTURE_ERROR", KEYSTORE_STRUCTURE_ERROR, "'" + keyStoreFile
                            + "' keystore is not a PKCS12 structure");
        return RET_FAILED;
    }

    if (keyStorePwd == NULL || *keyStorePwd == '\0') {
        if (!PKCS12_mac_present(p12) || PKCS12_verify_mac(p12, NULL, 0)) {
            keyStorePwd = NULL;
        } else if (PKCS12_verify_mac(p12, "", 0)) {
            keyStorePwd = "";
        } else {
            goto err;
        }
    } else if (!PKCS12_verify_mac(p12, keyStorePwd, -1)) {
        goto err;
    }

    return RET_OK;
err:
    PrintErrorNumberMsg("KEYSTORE_PASSWORD_ERROR", KEYSTORE_PASSWORD_ERROR, "keyStore password error");
    return RET_FAILED;
}

bool KeyStoreHelper::IsKeyStoreFileExist(std::string& keyStorePath)
{
    if (keyStorePath.empty()) {
        return false;
    }
    BIO* bioOut = nullptr;
    bioOut = BIO_new_file(keyStorePath.c_str(), "rb");
    if (bioOut == nullptr) {
        return false;
    }
    BIO_free(bioOut);
    return true;
}

EVP_PKEY* KeyStoreHelper::GenerateKeyPair(const std::string& algorithm, int keySize)
{
    if (algorithm.empty() || (0 == keySize)) {
        SIGNATURE_TOOLS_LOGI("keyAlg and keySize is nullptr!");
        return nullptr;
    }
    EC_GROUP* group = nullptr;
    EC_KEY* keyPair = EC_KEY_new();

    if (keySize == static_cast<int>(NIST_P_256)) {
        group = EC_GROUP_new_by_curve_name(NID_X9_62_prime256v1);
    } else if (keySize == static_cast<int>(NIST_P_384)) {
        group = EC_GROUP_new_by_curve_name(NID_secp384r1);
    } else {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        KeyPairFree(group, keyPair, "Algorithm length error");
        return nullptr;
    }
    if (!group) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        KeyPairFree(group, keyPair, "Elliptic curve encryption using P256 or P384 failed");
        return nullptr;
    }

    EC_KEY_set_group(keyPair, group);
    if (EC_KEY_generate_key(keyPair) != 1) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        KeyPairFree(group, keyPair, "Description Failed to generate an elliptic curve key pair");
        return nullptr;
    }
    if (EC_KEY_check_key(keyPair) != 1) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        KeyPairFree(group, keyPair, "Description Failed to generate an elliptic curve key pair");
        return nullptr;
    }
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_set1_EC_KEY(pkey, keyPair);
    KeyPairFree(group, keyPair, "");
    return pkey;
}

PKCS12* KeyStoreHelper::CreatePKCS12(const char* keyStorePwd, const char* keyPwd, const char* name, EVP_PKEY* pkey,
                                     X509* cert, int keyNid, int certNid, int iter,
                                     int macStatus, int keyType, STACK_OF(PKCS7)** safes)
{
    PKCS12* p12 = NULL;
    STACK_OF(PKCS12_SAFEBAG)* bags = NULL;
    unsigned char keyId[EVP_MAX_MD_SIZE];
    unsigned int keyIdLen = 0;
    PKCS12_SAFEBAG* bag = NULL;

    if (!certNid) {
        certNid = NID_PBE_CBC;
    }
    SetNidMac(keyNid, iter, macStatus);
    if (!pkey && !cert) {
        PKCS12err(PKCS12_F_PKCS12_CREATE, PKCS12_R_INVALID_NULL_ARGUMENT);
        return NULL;
    }

    if (!X509_check_private_key(cert, pkey)) {
        return NULL;
    }
    X509_digest(cert, EVP_sha384(), keyId, &keyIdLen);

    if (SetCertPkcs12(cert, bag, bags, keyId, keyIdLen, name, safes, certNid, iter, keyStorePwd) == RET_FAILED) {
        goto err;
    }

    if (SetPkeyPkcs12(pkey, bag, bags, name, safes, iter, keyPwd, keyType, keyNid, keyId, keyIdLen) == RET_FAILED) {
        goto err;
    }

    p12 = PKCS12_add_safes(*safes, 0);

    if (!p12) {
        goto err;
    }
    safes = NULL;
    if ((macStatus != -1) && !PKCS12_set_mac(p12, keyStorePwd, -1, NULL, 0, macStatus, NULL)) {
        goto err;
    }
    return p12;

err:
    sk_PKCS12_SAFEBAG_pop_free(bags, PKCS12_SAFEBAG_free);
    return NULL;
}

void KeyStoreHelper::SetNidMac(int& nidKey, int& iter, int& macStatus)
{
    if (!nidKey) {
        nidKey = NID_TRIPLEDES_CBC;
    }

    if (!iter) {
        iter = PKCS12_DEFAULT_ITER;
    }

    if (!macStatus) {
        macStatus = 1;
    }
}

int KeyStoreHelper::SetCertPkcs12(X509* cert, PKCS12_SAFEBAG* bag, STACK_OF(PKCS12_SAFEBAG)* certBags,
                                  unsigned char* keyId, unsigned int keyIdLen,
                                  const char* name, STACK_OF(PKCS7)** safes,
                                  int certNid, int iter, const char* keyStorePwd)
{
    if (cert) {
        bag = PKCS12_add_cert(&certBags, cert);
        if (name && !PKCS12_add_friendlyname(bag, name, -1)) {
            goto err;
        }

        if (keyIdLen && !PKCS12_add_localkeyid(bag, keyId, keyIdLen)) {
            goto err;
        }
    }

    if (certBags && !PKCS12_add_safe(safes, certBags, certNid, iter, keyStorePwd)) {
        goto err;
    }

    sk_PKCS12_SAFEBAG_pop_free(certBags, PKCS12_SAFEBAG_free);
    certBags = nullptr;
    return RET_OK;
err:
    sk_PKCS12_SAFEBAG_pop_free(certBags, PKCS12_SAFEBAG_free);
    certBags = nullptr;
    return RET_FAILED;
}

int KeyStoreHelper::SetPkeyPkcs12(EVP_PKEY* pkey, PKCS12_SAFEBAG* bag, STACK_OF(PKCS12_SAFEBAG)* keyBags,
                                  const char* name, STACK_OF(PKCS7)** safes, int iter, const char* keyPwd,
                                  int keyType, int keyNid, unsigned char* keyId, unsigned int keyIdLen)
{
    if (pkey) {
        bag = PKCS12_add_key(&keyBags, pkey, keyType, iter, keyNid, keyPwd);
        if (!bag) {
            return RET_FAILED;
        }

        if (GetAttrNid(bag, pkey, NID_ms_csp_name) == RET_FAILED) {
            goto err;
        }

        if (GetAttrNid(bag, pkey, NID_LocalKeySet) == RET_FAILED) {
            goto err;
        }

        if (name && !PKCS12_add_friendlyname(bag, name, -1)) {
            goto err;
        }

        if (keyIdLen && !PKCS12_add_localkeyid(bag, keyId, keyIdLen)) {
            goto err;
        }
    }
    if (keyBags && !PKCS12_add_safe(safes, keyBags, -1, 0, NULL)) {
        goto err;
    }

    sk_PKCS12_SAFEBAG_pop_free(keyBags, PKCS12_SAFEBAG_free);
    keyBags = nullptr;
    return RET_OK;
err:
    sk_PKCS12_SAFEBAG_pop_free(keyBags, PKCS12_SAFEBAG_free);
    keyBags = nullptr;
    return RET_FAILED;
}

int KeyStoreHelper::GetAttrNid(PKCS12_SAFEBAG* bag, EVP_PKEY* pkey, int nid)
{
    int idx;
    X509_ATTRIBUTE* attr;
    idx = EVP_PKEY_get_attr_by_NID(pkey, nid, -1);
    if (idx < 0) {
        return RET_OK;
    }
    attr = EVP_PKEY_get_attr(pkey, idx);
    STACK_OF(X509_ATTRIBUTE)* attrlib = const_cast<STACK_OF(X509_ATTRIBUTE)*>(PKCS12_SAFEBAG_get0_attrs(bag));
    if (!X509at_add1_attr(&attrlib, attr)) {
        return RET_FAILED;
    }
    return RET_OK;
}

int KeyStoreHelper::ParsePkcs12Safebag(PKCS12_SAFEBAG* bag, const char* pass, int passlen, STACK_OF(X509)* ocerts)
{
    X509* x509Cert = nullptr;
    const ASN1_TYPE* attr;
    ASN1_BMPSTRING* name = NULL;
    ASN1_OCTET_STRING* kid = NULL;
    if ((attr = PKCS12_SAFEBAG_get0_attr(bag, NID_friendlyName))) {
        name = attr->value.bmpstring;
    }

    if ((attr = PKCS12_SAFEBAG_get0_attr(bag, NID_localKeyID))) {
        kid = attr->value.octet_string;
    }

    if (PKCS12_SAFEBAG_get_nid(bag) != NID_certBag && PKCS12_SAFEBAG_get_bag_nid(bag) != NID_x509Certificate) {
        return RET_OK;
    }

    if ((x509Cert = PKCS12_SAFEBAG_get1_cert(bag)) == NULL) {
        return RET_FAILED;
    }

    if (kid && !X509_keyid_set1(x509Cert, kid->data, kid->length)) {
        goto err;
    }

    if (name) {
        int len;
        unsigned char* data;
        len = ASN1_STRING_to_UTF8(&data, name);
        if (!SetX509Alias(len, x509Cert, data)) {
            goto err;
        }
    }
    if (!sk_X509_push(ocerts, x509Cert)) {
        goto err;
    }

    return RET_OK;
err:
    X509_free(x509Cert);
    return RET_FAILED;
}

bool KeyStoreHelper::SetX509Alias(int len, X509* x509, unsigned char* data)
{
    if (len >= 0) {
        int r = X509_alias_set1(x509, data, len);
        OPENSSL_free(data);
        if (!r) {
            X509_free(x509);
            return false;
        }
    }
    return true;
}

int KeyStoreHelper::ParsePkcs12Safebags(const STACK_OF(PKCS12_SAFEBAG)* bags, const char* pass,
                                        int passlen, STACK_OF(X509)* ocerts)
{
    int i;
    for (i = 0; i < sk_PKCS12_SAFEBAG_num(bags); i++) {
        if (ParsePkcs12Safebag(sk_PKCS12_SAFEBAG_value(bags, i), pass, passlen, ocerts) == RET_FAILED)
            return RET_FAILED;
    }
    return RET_OK;
}
} // namespace SignatureTools
} // namespace OHOS