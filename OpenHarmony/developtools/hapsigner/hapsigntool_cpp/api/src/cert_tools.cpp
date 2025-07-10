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
#include <cassert>
#include <string>
#include <numeric>
#include <unordered_map>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/conf.h>

#include "cert_tools.h"
#include "openssl/ec.h"
#include "openssl/obj_mac.h"
#include "openssl/asn1.h"
#include "signature_tools_log.h"
#include "constant.h"

namespace OHOS {
namespace SignatureTools {

static std::unordered_map<std::string, long> externDic{
    {"digitalSignature", X509v3_KU_DIGITAL_SIGNATURE},
    {"nonRepudiation", X509v3_KU_NON_REPUDIATION},
    {"keyEncipherment", X509v3_KU_KEY_ENCIPHERMENT},
    {"dataEncipherment", X509v3_KU_DATA_ENCIPHERMENT},
    {"keyAgreement", X509v3_KU_KEY_AGREEMENT},
    {"certificateSignature", X509v3_KU_KEY_CERT_SIGN},
    {"crlSignature", X509v3_KU_CRL_SIGN},
    {"encipherOnly", X509v3_KU_ENCIPHER_ONLY},
    {"decipherOnly", X509v3_KU_DECIPHER_ONLY},

};

static std::unordered_map<std::string, std::string> externKey{
    {"serverAuthentication", "1.3.6.1.5.5.7.3.1"},
    {"clientAuthentication",  "1.3.6.1.5.5.7.3.2"},
    {"codeSignature",  "1.3.6.1.5.5.7.3.3"},
    {"emailProtection",  "1.3.6.1.5.5.7.3.4"},
    {"smartCardLogin",  "1.3.6.1.5.5.7.3.5"},
    {"timestamp",  "1.3.6.1.5.5.7.3.8"},
    {"ocspSignature",  "1.3.6.1.5.5.7.3.9"},

};

bool CertTools::SaveCertTofile(const std::string& filename, X509* cert)
{
    BIO* certBio = BIO_new_file(filename.data(), "w");
    if (!certBio) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("BIO_new failed");
        return false;
    }

    if (PEM_write_bio_X509(certBio, cert) < 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("PEM_write_bio_X509 failed");
        BIO_free(certBio);
        return false;
    }
    BIO_free(certBio);
    return true;
}

bool CertTools::UpdateConstraint(Options* options)
{
    if (options->count(Options::BASIC_CONSTRAINTS)) {
        if (!String2Bool(options, Options::BASIC_CONSTRAINTS)) {
            return false;
        }
    } else {
        (*options)[Options::BASIC_CONSTRAINTS] = DEFAULT_BASIC_CONSTRAINTS;
    }

    if (options->count(Options::BASIC_CONSTRAINTS_CRITICAL)) {
        if (!String2Bool(options, Options::BASIC_CONSTRAINTS_CRITICAL)) {
            return false;
        }
    } else {
        (*options)[Options::BASIC_CONSTRAINTS_CRITICAL] = DEFAULT_BASIC_CONSTRAINTS_CRITICAL;
    }

    if (options->count(Options::BASIC_CONSTRAINTS_CA)) {
        if (!String2Bool(options, Options::BASIC_CONSTRAINTS_CA)) {
            return false;
        }
    } else {
        (*options)[Options::BASIC_CONSTRAINTS_CA] = DEFAULT_BASIC_CONSTRAINTS_CA;
    }
    return true;
}

bool CertTools::String2Bool(Options* options, const std::string& option)
{
    std::string val = options->GetString(option);
    if (val == "1" || val == "true" || val == "TRUE") {
        (*options)[option] = true;
    } else if (val == "0" || val == "false" || val == "FALSE") {
        (*options)[option] = false;
    } else {
        PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                            val + "is not valid value for " + "-" + option);
        return false;
    }
    return true;
}

bool CertTools::SetBisicConstraints(Options* options, X509* cert)
{
    if (!UpdateConstraint(options)) {
        return false;
    }

    bool basicCon = options->GetBool(Options::BASIC_CONSTRAINTS);
    if (basicCon) {
        bool basicConstraintsCritical = options->GetBool(Options::BASIC_CONSTRAINTS_CRITICAL);
        int critial = basicConstraintsCritical ? 1 : 0;
        bool basicConstraintsCa = options->GetBool(Options::BASIC_CONSTRAINTS_CA);
        std::string  ContainCa = basicConstraintsCa ? "CA:TRUE" : "CA:FALSE";
        std::string constraints = ContainCa + "," + "pathlen:" +
            std::to_string(options->GetInt(Options::BASIC_CONSTRAINTS_PATH_LEN));
        X509V3_CTX ctx;
        X509V3_set_ctx_nodb(&ctx);

        X509_EXTENSION* ext = X509V3_EXT_conf_nid(NULL, &ctx, NID_basic_constraints, constraints.c_str());
        if (X509_EXTENSION_set_critical(ext, critial) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to set  critical for extKeyUsage ");
            X509_EXTENSION_free(ext);
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            return false;
        }
        if (X509_add_ext(cert, ext, -1) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_add_ext failed");
            X509_EXTENSION_free(ext);
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            return false;
        }
        X509_EXTENSION_free(ext);
    }
    
    return true;
}

bool CertTools::SetBisicConstraintsPathLen(Options* options, X509* cert)
{
    std::string setOptions = "CA:TRUE, pathlen:" +
        std::to_string(options->GetInt(Options::BASIC_CONSTRAINTS_PATH_LEN));
    X509V3_CTX ctx;
    X509V3_set_ctx_nodb(&ctx);
    X509_EXTENSION* ext = X509V3_EXT_conf_nid(NULL, &ctx, NID_basic_constraints, setOptions.c_str());
    if (X509_EXTENSION_set_critical(ext, 1) == 0) {
        SIGNATURE_TOOLS_LOGE("failed to set  critical for extKeyUsage ");
        X509_EXTENSION_free(ext);
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        return false;
    }
    if (X509_add_ext(cert, ext, -1) == 0) {
        SIGNATURE_TOOLS_LOGE("X509_add_ext failed\n");
        X509_EXTENSION_free(ext);
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        return false;
    }
    X509_EXTENSION_free(ext);
    return true;
}

bool CertTools::SignForSubCert(X509* cert, X509_REQ* subcsr, X509_REQ* rootcsr, EVP_PKEY* caPrikey, Options* options)
{
    if (caPrikey == nullptr || rootcsr == nullptr || subcsr == nullptr) {
        SIGNATURE_TOOLS_LOGE("Sign failed because of caPrikey, roocsr or subcsr is nullptr");
        return false;
    }

    std::string signAlg = options->GetString(Options::SIGN_ALG);
    EVP_PKEY* pubKey = X509_REQ_get_pubkey(subcsr);
    if (pubKey == nullptr) {
        SIGNATURE_TOOLS_LOGE("X509_REQ_get_pubkey failed");
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        return false;
    }
    X509_NAME* issuerName = X509_REQ_get_subject_name(rootcsr);
    X509_NAME* subjectName = X509_REQ_get_subject_name(subcsr);
    do {
        if (X509_set_pubkey(cert, pubKey) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_set_pubkey failed");
            break;
        }
        if (X509_set_issuer_name(cert, issuerName) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_set_issuer_name failed");
            break;
        }
        if (X509_set_subject_name(cert, subjectName) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_set_subject_name failed");
            break;
        }
        if (!SignCert(cert, caPrikey, signAlg)) {
            break;
        }
        EVP_PKEY_free(pubKey);
        return true;
    } while (0);

    EVP_PKEY_free(pubKey);
    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    return false;
}

X509* CertTools::SignCsrGenerateCert(X509_REQ* rootcsr, X509_REQ* subcsr,
                                     EVP_PKEY* keyPair, Options* options)
{
    X509* cert = X509_new();
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to create X509 cert");
        return nullptr;
    }

    do {
        int validity = options->GetInt(Options::VALIDITY);
        if (!SetCertVersion(cert, DEFAULT_CERT_VERSION) ||
            !SetCertSerialNum(cert)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of set version or serial num failed");
            break;
        }
        if (!SetCertValidity(cert, validity)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of set validity failed");
            break;
        }
        if (!SetBisicConstraintsPathLen(options, cert) ||
            !SetKeyIdentifierExt(cert) ||
            !SetAuthorizeKeyIdentifierExt(cert) ||
            !SetKeyUsage(cert, options) ||
            !SignForSubCert(cert, subcsr, rootcsr, keyPair, options)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of other reasons");
            break;
        }
        return cert;
    } while (0);

    X509_free(cert);
    return nullptr;
}

bool CertTools::SetSubjectForCert(X509_REQ* certReq, X509* cert)
{
    if (certReq == nullptr) {
        SIGNATURE_TOOLS_LOGE("set subjcet failed because certReq is nullptr");
        return false;
    }

    do {
        if (X509_set_subject_name(cert, X509_REQ_get_subject_name(certReq)) != 1) {
            SIGNATURE_TOOLS_LOGE("X509_set_issuer_name failed");
            break;
        }
        if (X509_set_issuer_name(cert, X509_REQ_get_subject_name(certReq)) != 1) {
            SIGNATURE_TOOLS_LOGE("X509_set_issuer_name failed");
            break;
        }
        return true;
    } while (0);

    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    return false;
}

X509* CertTools::GenerateRootCertificate(EVP_PKEY* keyPair, X509_REQ* certReq, Options* options)
{
    X509* cert = X509_new();
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to create X509 cert");
        return nullptr;
    }
    do {
        int validity = options->GetInt(Options::VALIDITY);
        std::string signAlg = options->GetString(Options::SIGN_ALG);
        if (!SetCertVersion(cert, DEFAULT_CERT_VERSION) || !SetCertSerialNum(cert)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of set version or serial num failed");
            break;
        }
        if (!SetCertValidityStartAndEnd(cert, DEFAULT_START_VALIDITY, validity)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of set validity failed");
            break;
        }
        if (!SetBisicConstraintsPathLen(options, cert) ||
            !SetSubjectForCert(certReq, cert) ||
            !SetCertPublickKey(cert, certReq) ||
            !SetKeyIdentifierExt(cert) ||
            !SetKeyUsage(cert, options)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of other reasons");
            break;
        }
        if (!SignCert(cert, keyPair, signAlg)) {
            SIGNATURE_TOOLS_LOGE("failed to generate X509 cert cause of sign failed");
            break;
        }
        return cert;
    } while (0);

    X509_free(cert);
    return nullptr;
}

X509* CertTools::GenerateSubCert(EVP_PKEY* keyPair, X509_REQ* rootcsr, Options* options)
{
    std::unique_ptr<LocalizationAdapter> adapter = std::make_unique< LocalizationAdapter>(options);
    EVP_PKEY* subKey = nullptr;
    X509_REQ* subcsr = nullptr;
    X509* subCert = nullptr;
    subKey = adapter->GetAliasKey(false);
    if (subKey == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to get the keypair");
        return nullptr;
    }

    do {
        subcsr = CertTools::GenerateCsr(subKey, options->GetString(Options::SIGN_ALG),
                                        options->GetString(Options::SUBJECT));
        if (subcsr == nullptr) {
            SIGNATURE_TOOLS_LOGE("failed to generate csr");
            break;
        }
        subCert = SignCsrGenerateCert(rootcsr, subcsr, keyPair, options);
        if (subCert == nullptr) {
            SIGNATURE_TOOLS_LOGE("failed to generate the subCert");
            break;
        }
    } while (0);

    EVP_PKEY_free(subKey);
    X509_REQ_free(subcsr);
    return subCert;
}

bool CertTools::SetKeyUsage(X509* cert, Options* options)
{
    std::string keyUsage = options->GetString(Options::KEY_USAGE);
    ASN1_INTEGER* keyUsageInt = ASN1_INTEGER_new();
    long key = 0;
    if (keyUsage.empty()) {
        key = X509v3_KU_KEY_CERT_SIGN | X509v3_KU_CRL_SIGN;
        if (keyUsageInt == NULL || ASN1_INTEGER_set(keyUsageInt, key) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to set asn1_integer");
            ASN1_INTEGER_free(keyUsageInt);
            return false;
        }
        if (X509_add1_ext_i2d(cert, NID_key_usage, keyUsageInt, 0, X509V3_ADD_DEFAULT) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to add ext");
            ASN1_INTEGER_free(keyUsageInt);
            return false;
        }
    } else {
        bool keyUsageCritical = options->GetBool(Options::KEY_USAGE_CRITICAL);
        int crit = keyUsageCritical > 0 ? 1 : 0;
        std::vector<std::string> vecs = StringUtils::SplitString(keyUsage.c_str(), ',');
        key = std::accumulate(vecs.begin(), vecs.end(), key,
            [&](long key, const std::string& vec) { return key | externDic[vec]; });
        if (keyUsageInt == NULL || ASN1_INTEGER_set(keyUsageInt, key) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to set asn1_integer");
            ASN1_INTEGER_free(keyUsageInt);
            return false;
        }
        if (X509_add1_ext_i2d(cert, NID_key_usage, keyUsageInt, crit, X509V3_ADD_DEFAULT) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to add ext");
            ASN1_INTEGER_free(keyUsageInt);
            return false;
        }
    }
    ASN1_INTEGER_free(keyUsageInt);
    return true;
}

bool CertTools::SetkeyUsageExt(X509* cert, Options* options)
{
    X509_EXTENSION* ext = nullptr;
    bool keyUsageCritical = options->GetBool(Options::KEY_USAGE_CRITICAL);
    int crit = keyUsageCritical  ? 1 : 0;
    if (!options->GetString(Options::EXT_KEY_USAGE).empty()) {
        ext = X509V3_EXT_conf(NULL, NULL, NID_EXT_KEYUSAGE_CONST.c_str(),
                              externKey[options->GetString(Options::EXT_KEY_USAGE)].c_str());
        if (X509_EXTENSION_set_critical(ext, crit) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to set  critical for extKeyUsage ");
            X509_EXTENSION_free(ext);
            return false;
        }
        if (X509_add_ext(cert, ext, -1) == 0) {
            SIGNATURE_TOOLS_LOGE("failed to add extension");
            X509_EXTENSION_free(ext);
            return false;
        }
    }
    X509_EXTENSION_free(ext);
    return true;
}

bool CertTools::SetExpandedInformation(X509* cert, Options* options)
{
    if (!SetKeyUsage(cert, options) ||
        !SetkeyUsageExt(cert, options)) {
        SIGNATURE_TOOLS_LOGE("Failed to set expanded information ");
        return false;
    }
    return true;
}

bool CertTools::SetPubkeyAndSignCert(X509* cert, X509_REQ* issuercsr,
                                     X509_REQ* certReq, EVP_PKEY* keyPair, Options* options)
{
    do {
        if (X509_set_issuer_name(cert, X509_REQ_get_subject_name(issuercsr)) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_set_issuer_name failed");
            break;
        }
        if (X509_set_subject_name(cert, X509_REQ_get_subject_name(certReq)) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_set_subject_name failed");
            break;
        }
        if ((options->GetString(Options::SIGN_ALG)) == SIGN_ALG_SHA256) {
            if (X509_sign(cert, keyPair, EVP_sha256()) == 0) {
                SIGNATURE_TOOLS_LOGE("X509_sign failed");
                break;
            }
        } else {
            if (X509_sign(cert, keyPair, EVP_sha384()) == 0) {
                SIGNATURE_TOOLS_LOGE("X509_sign failed");
                break;
            }
        }
        return true;
    } while (0);

    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    return false;
}

X509* CertTools::GenerateCert(EVP_PKEY* keyPair, X509_REQ* certReq, Options* options)
{
    X509_REQ* issuercsr = CertTools::GenerateCsr(keyPair, options->GetString(Options::SIGN_ALG),
                                                 options->GetString(Options::ISSUER));
    if (issuercsr == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to generate the issuercsr");
        return nullptr;
    }

    X509* cert = X509_new();
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to create X509 cert");
        X509_REQ_free(issuercsr);
        return nullptr;
    }
    do {
        if (!SetCertVersion(cert, DEFAULT_CERT_VERSION) ||
            !SetCertSerialNum(cert) ||
            !SetKeyIdentifierExt(cert)) {
            SIGNATURE_TOOLS_LOGE("failed to set cert version, serial number or key identifier");
            break;
        }
        int validity = options->GetInt(Options::VALIDITY);
        if (!SetCertValidityStartAndEnd(cert, DEFAULT_START_VALIDITY, validity)) {
            SIGNATURE_TOOLS_LOGE("failed to set cert validity");
            break;
        }
        if (!SetBisicConstraints(options, cert) ||
            !SetCertPublickKey(cert, certReq) ||
            !SetExpandedInformation(cert, options) ||
            !SetPubkeyAndSignCert(cert, issuercsr, certReq, keyPair, options)) {
            SIGNATURE_TOOLS_LOGE("failed to generate cert cause of other reasons");
            break;
        }
        X509_REQ_free(issuercsr);
        return cert;
    } while (0);

    X509_REQ_free(issuercsr);
    X509_free(cert);
    return nullptr;
}

X509_REQ* CertTools::GenerateCsr(EVP_PKEY* evpPkey, std::string signAlgorithm, std::string subject)
{
    X509_NAME* name = nullptr;
    X509_REQ* req = X509_REQ_new();
    if (req == nullptr) {
        SIGNATURE_TOOLS_LOGE("X509_REQ_new failed");
        return nullptr;
    }
    do {
        if (X509_REQ_set_pubkey(req, evpPkey) == 0) {
            SIGNATURE_TOOLS_LOGE("X509_REQ_set_pubkey failed");
            break;
        }

        name = BuildDN(subject, req);
        if (name == nullptr) {
            SIGNATURE_TOOLS_LOGE("failed to add subject into cert");
            break;
        }

        if (signAlgorithm == SIGN_ALG_SHA256) {
            if (X509_REQ_sign(req, evpPkey, EVP_sha256()) == 0) {
                SIGNATURE_TOOLS_LOGE("X509_REQ_sign failed");
                break;
            }
        } else if (signAlgorithm == SIGN_ALG_SHA384) {
            if (X509_REQ_sign(req, evpPkey, EVP_sha384()) == 0) {
                SIGNATURE_TOOLS_LOGE("X509_REQ_sign failed");
                break;
            }
        } else {
            PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                                "Sign algorithm format error! Please check again.");
            break;
        }
        return req;
    } while (0);

    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    X509_REQ_free(req);
    return nullptr;
}

std::string CertTools::CsrToString(X509_REQ* csr)
{
    BIO* csrBio = BIO_new(BIO_s_mem());
    if (!csrBio) {
        return "";
    }
    if (!PEM_write_bio_X509_REQ(csrBio, csr)) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("PEM_write_bio_X509_REQ error");
        BIO_free(csrBio);
        return "";
    }
    BUF_MEM* data = nullptr;
    BIO_get_mem_ptr(csrBio, &data);
    if (!data) {
        BIO_free(csrBio);
        return "";
    }
    if (!data->data) {
        BIO_free(csrBio);
        return "";
    }
    std::string csrStr(data->data, data->length);
    BIO_free(csrBio);
    return csrStr;
}

X509* CertTools::ReadfileToX509(const std::string& filename)
{
    BIO* certBio = BIO_new_file(filename.c_str(), "rb");
    if (!certBio) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("BIO_new_file failed");
        BIO_free(certBio);
        return nullptr;
    }

    X509* cert = X509_new();
    if (cert == NULL) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("failed to create X509 cert");
        BIO_free(certBio);
        return nullptr;
    }
    if (!PEM_read_bio_X509(certBio, &cert, NULL, NULL)) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("PEM_read_bio_X509 failed");
        X509_free(cert);
        BIO_free(certBio);
        return nullptr;
    }
    BIO_free(certBio);

    return cert;
}

bool CertTools::SetCertVersion(X509* cert, int versionNum)
{
    if (X509_set_version(cert, versionNum) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set x509 cert version failed");
        return false;
    }
    return true;
}

bool CertTools::SetCertSerialNum(X509* cert)
{
    BIGNUM* bignum = BN_new();

    do {
        uint8_t serialNumberValue[RANDOM_SERIAL_NUMBER_LENGTH] = {0};
        if (!SerialNumberBuilder(serialNumberValue, sizeof(serialNumberValue))) {
            break;
        }
        if (!BN_bin2bn(serialNumberValue, sizeof(serialNumberValue), bignum)) {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            break;
        }
        if (BN_is_negative(bignum)) {
            BN_set_negative(bignum, 0); // Replace negative numbers with positive ones
        }
        if (!BN_to_ASN1_INTEGER(bignum, X509_get_serialNumber(cert))) {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            break;
        }
        BN_free(bignum);
        return true;
    } while (0);
    
    SIGNATURE_TOOLS_LOGE("set x509 cert serial number failed");
    BN_free(bignum);
    return false;
}

bool CertTools::SetCertIssuerName(X509* cert, X509_NAME* issuer)
{
    if (X509_set_issuer_name(cert, issuer) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set x509 cert issuer name failed");
        return false;
    }
    return true;
}

bool CertTools::SetCertSubjectName(X509* cert, X509_REQ* subjectCsr)
{
    X509_NAME* subject = nullptr;
    if (!(subject = X509_REQ_get_subject_name(subjectCsr))) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("get X509 cert subject name failed");
        return false;
    }
    if (X509_set_subject_name(cert, subject) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set X509 cert subject name failed");
        return false;
    }
    return true;
}

bool CertTools::SetCertValidityStartAndEnd(X509* cert, long vilidityStart, long vilidityEnd)
{
    if (X509_gmtime_adj(X509_getm_notBefore(cert), vilidityStart) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set cert vilidity start time failed");
        return false;
    }
    if (X509_gmtime_adj(X509_getm_notAfter(cert), vilidityEnd) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set cert vilidity end time failed");
        return false;
    }
    return true;
}

bool CertTools::SetCertPublickKey(X509* cert, X509_REQ* subjectCsr)
{
    EVP_PKEY* publicKey = X509_REQ_get_pubkey(subjectCsr);
    if (publicKey == nullptr) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("get the pubkey from csr failed");
        return false;
    }
    if (X509_set_pubkey(cert, publicKey) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        EVP_PKEY_free(publicKey);
        SIGNATURE_TOOLS_LOGE("set public key to cert failed");
        return false;
    }
    EVP_PKEY_free(publicKey);
    return true;
}

bool CertTools::SetBasicExt(X509* cert)
{
    X509_EXTENSION* basicExtension = X509V3_EXT_conf(NULL, NULL, NID_BASIC_CONST.c_str(),
                                                     DEFAULT_BASIC_EXTENSION.c_str());
    if (X509_add_ext(cert, basicExtension, -1) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set basicExtension information failed");
        X509_EXTENSION_free(basicExtension);
        return false;
    }
    X509_EXTENSION_free(basicExtension);
    return true;
}

bool CertTools::SetkeyUsageExt(X509* cert)
{
    X509_EXTENSION* keyUsageExtension = X509V3_EXT_conf(NULL, NULL, NID_KEYUSAGE_CONST.c_str(),
                                                        DEFAULT_KEYUSAGE_EXTENSION.c_str());
    if (X509_add_ext(cert, keyUsageExtension, -1) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set keyUsageExtension information failed");
        X509_EXTENSION_free(keyUsageExtension);
        return false;
    }
    X509_EXTENSION_free(keyUsageExtension);
    return true;
}

bool CertTools::SetKeyUsageEndExt(X509* cert)
{
    X509_EXTENSION* keyUsageEndExtension = X509V3_EXT_conf(NULL, NULL, NID_EXT_KEYUSAGE_CONST.c_str(),
                                                           DEFAULT_EXTEND_KEYUSAGE.c_str());
    if (X509_add_ext(cert, keyUsageEndExtension, -1) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set keyUsageEndExtension information failed");
        X509_EXTENSION_free(keyUsageEndExtension);
        return false;
    }
    X509_EXTENSION_free(keyUsageEndExtension);
    return true;
}

bool CertTools::SetKeyIdentifierExt(X509* cert)
{
    unsigned char digest[SHA256_DIGEST_LENGTH] = {0};
    unsigned int digestLen = 0;
    if (X509_pubkey_digest(cert, EVP_sha256(), digest, &digestLen) != 1) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("digest x509 cert public key failed");
        return false;
    }
    ASN1_OCTET_STRING* pubKeyDigestData = ASN1_OCTET_STRING_new();
    if (ASN1_OCTET_STRING_set(pubKeyDigestData, digest, digestLen) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set ANS1 pubKeyDigestData failed");
        ASN1_OCTET_STRING_free(pubKeyDigestData);
        return false;
    }

    X509_EXTENSION* subKeyIdentifierExtension = nullptr;
    /* function OBJ_nid2obj(NID_subject_key_identifier) return value is a global variable, so should not free it */
    subKeyIdentifierExtension = X509_EXTENSION_create_by_OBJ(NULL, OBJ_nid2obj(NID_subject_key_identifier),
                                                             0, pubKeyDigestData);
    if (X509_add_ext(cert, subKeyIdentifierExtension, -1) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set subKeyIdentifierExtension information failed");
        ASN1_OCTET_STRING_free(pubKeyDigestData);
        X509_EXTENSION_free(subKeyIdentifierExtension);
        return false;
    }
    ASN1_OCTET_STRING_free(pubKeyDigestData);
    X509_EXTENSION_free(subKeyIdentifierExtension);
    return true;
}

bool CertTools::SetAuthorizeKeyIdentifierExt(X509* cert)
{
    unsigned char key_id[] = { 0x73, 0x3a, 0x81, 0x87, 0x8f, 0x95, 0xc1, 0x94,
                               0xcf, 0xef, 0xab, 0x6f, 0x7f, 0x01, 0x52, 0x86,
                               0xa3, 0xc2, 0x01, 0xc2 };
    unsigned int key_id_len = sizeof(key_id);
    X509_EXTENSION* ext = nullptr;
    AUTHORITY_KEYID* akid = AUTHORITY_KEYID_new();
    akid->keyid = ASN1_OCTET_STRING_new();
    if (ASN1_OCTET_STRING_set(akid->keyid, key_id, key_id_len) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set ANS1 pubKeyDigestData failed");
        AUTHORITY_KEYID_free(akid);
        return false;
    }
    ext = X509V3_EXT_i2d(NID_authority_key_identifier, 1, akid);
    if (X509_add_ext(cert, ext, -1) == 0) {
        SIGNATURE_TOOLS_LOGE("Failed to add AKI extension to certificate");
        X509_EXTENSION_free(ext);
        AUTHORITY_KEYID_free(akid);
        return false;
    }

    X509_EXTENSION_free(ext);
    AUTHORITY_KEYID_free(akid);
    return true;
}

bool CertTools::SetSignCapacityExt(X509* cert, const char signCapacity[], int capacityLen)
{
    ASN1_OCTET_STRING* certSignCapacityData = ASN1_OCTET_STRING_new();
    if (ASN1_OCTET_STRING_set(certSignCapacityData,
        reinterpret_cast<const unsigned char*>(signCapacity), capacityLen) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("failed to set pubkey digst into ASN1 object");
        ASN1_OCTET_STRING_free(certSignCapacityData);
        return false;
    }
    // generate user-define Nid
    ASN1_OBJECT* nid = OBJ_txt2obj(X509_EXT_OID.c_str(), 1);
    X509_EXTENSION* certSignCapacityExt = X509_EXTENSION_create_by_OBJ(NULL, nid, 0, certSignCapacityData);

    if (X509_add_ext(cert, certSignCapacityExt, -1) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("set certSignCapacityExt information failed");
        ASN1_OBJECT_free(nid);
        X509_EXTENSION_free(certSignCapacityExt);
        ASN1_OCTET_STRING_free(certSignCapacityData);
        return false;
    }
    ASN1_OBJECT_free(nid);
    X509_EXTENSION_free(certSignCapacityExt);
    ASN1_OCTET_STRING_free(certSignCapacityData);
    return true;
}

bool CertTools::SignCert(X509* cert, EVP_PKEY* privateKey, std::string signAlg)
{
    const EVP_MD* alg = nullptr;
    if (signAlg == SIGN_ALG_SHA256) {
        /* in openssl this func return value is stack variable, so we not need to release it */
        alg = EVP_sha256();
    }
    if (signAlg == SIGN_ALG_SHA384) {
        alg = EVP_sha384();
    }
    if (X509_sign(cert, privateKey, alg) == 0) {
        VerifyHapOpensslUtils::GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("sign X509 cert failed");
        return false;
    }
    return true;
}

bool CertTools::SetCertValidity(X509* cert, int validity)
{
    if (!SetCertValidityStartAndEnd(cert, DEFAULT_START_VALIDITY, validity)) {
        return false;
    }
    return true;
}

bool CertTools::SerialNumberBuilder(uint8_t* serialNum, int length)
{
    if (RAND_bytes(serialNum, length) != 1) { // this function is thread safity
        SIGNATURE_TOOLS_LOGE("serial number build failed");
        return false;
    }
    return true;
}

X509* CertTools::GenerateEndCert(X509_REQ* csr, EVP_PKEY* issuerKeyPair, LocalizationAdapter& adapter,
                                 const char signCapacity[], int capacityLen)
{
    X509* cert = X509_new(); // in this function, should not release X509cert memory
    if (cert == nullptr) {
        SIGNATURE_TOOLS_LOGE("failed to create X509 cert");
        return nullptr;
    }
    X509_REQ* issuerReq = X509_REQ_new();
    if (issuerReq == nullptr) {
        SIGNATURE_TOOLS_LOGE("X509_REQ_new failed");
        adapter.AppAndProfileAssetsRealse({}, {}, {cert});
        return nullptr;
    }
    do {
        std::string issuerStr = adapter.options->GetString(adapter.options->ISSUER);
        int validity = adapter.options->GetInt(adapter.options->VALIDITY);
        std::string signAlg = adapter.options->GetString(adapter.options->SIGN_ALG);

        if (!SetCertVersion(cert, DEFAULT_CERT_VERSION) || !SetCertSerialNum(cert)) {
            SIGNATURE_TOOLS_LOGE("failed to set cert version or serial number");
            break;
        }
        if (!SetCertIssuerName(cert, BuildDN(issuerStr, issuerReq)) || !SetCertSubjectName(cert, csr)) {
            SIGNATURE_TOOLS_LOGE("failed to set cert issuer or subject name");
            break;
        }
        if (!SetCertValidity(cert, validity) || !SetCertPublickKey(cert, csr)) {
            SIGNATURE_TOOLS_LOGE("failed to set cert validity or public key");
            break;
        }
        if (!SetBasicExt(cert) || !SetkeyUsageExt(cert) || !SetKeyUsageEndExt(cert)) {
            SIGNATURE_TOOLS_LOGE("failed to set basic ext or key usage ext");
            break;
        }
        if (!SetKeyIdentifierExt(cert) || !SetSignCapacityExt(cert, signCapacity, capacityLen)) {
            SIGNATURE_TOOLS_LOGE("failed to set key identifier ext or sign capacity ext");
            break;
        }
        if (!SignCert(cert, issuerKeyPair, signAlg)) {
            SIGNATURE_TOOLS_LOGE("failed to sign cert");
            break;
        }
        adapter.AppAndProfileAssetsRealse({}, {issuerReq}, {});
        return cert; // return x509 assets
    } while (0);

    adapter.AppAndProfileAssetsRealse({}, {issuerReq}, {cert});
    return nullptr;
}

} // namespace SignatureTools
} // namespace OHOS

