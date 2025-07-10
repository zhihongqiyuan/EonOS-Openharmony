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
#include "sign_provider.h"
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <cstdio>
#include <cinttypes>
#include <algorithm>

#include "nlohmann/json.hpp"
#include "string_utils.h"
#include "file_utils.h"
#include "sign_elf.h"
#include "params.h"
#include "constant.h"

using namespace nlohmann;
namespace OHOS {
namespace SignatureTools {
std::vector<std::string> SignProvider::VALID_SIGN_ALG_NAME = {
    ParamConstants::HAP_SIG_ALGORITHM_SHA256_ECDSA,
    ParamConstants::HAP_SIG_ALGORITHM_SHA384_ECDSA,
    ParamConstants::HAP_SIG_ALGORITHM_SHA512_ECDSA
};

bool SignProvider::InitSigerConfig(SignerConfig& signerConfig, STACK_OF(X509)* publicCerts, Options* options)
{
    std::optional<X509_CRL*> crl = GetCrl();
    if (!CreateSignerConfigs(publicCerts, crl, options, signerConfig)) {
        SIGNATURE_TOOLS_LOGE("[Sign] create Signer Configs failed");
        return false;
    }
    return true;
}

bool SignProvider::SignElf(Options* options)
{
    STACK_OF(X509)* publicCerts = nullptr;
    int ret = GetX509Certificates(options, &publicCerts);
    if (ret != RET_OK) {
        sk_X509_pop_free(publicCerts, X509_free);
        SIGNATURE_TOOLS_LOGE("[SignElf] get X509 Certificates failed! errorCode:%d", ret);
        return false;
    }
    SignerConfig signerConfig;
    if (!InitSigerConfig(signerConfig, publicCerts, options)) {
        SIGNATURE_TOOLS_LOGE("SignElf] create Signer Configs failed");
        return false;
    }

    if (!profileContent.empty()) {
        signParams.insert(std::make_pair(ParamConstants::PARAM_PROFILE_JSON_CONTENT, profileContent));
    }

    if (!SignElf::Sign(signerConfig, signParams)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] sign elf failed");
        return false;
    }

    return true;
}

bool SignProvider::CreateSignerConfigs(STACK_OF(X509)* certificates, const std::optional<X509_CRL*>& crl,
                                       Options* options, SignerConfig& inOut)
{
    inOut.FillParameters(signParams);
    inOut.SetCertificates(certificates);
    inOut.SetOptions(options);
    std::vector<SignatureAlgorithmHelper> signatureAlgorithms;
    SignatureAlgorithmHelper alg;
    // Since CheckParmaAndInitConfig has already validated all parameters, it is possible to directly use at
    if (!Params::GetSignatureAlgorithm(signParams.at(ParamConstants::PARAM_BASIC_SIGANTURE_ALG),
                                       alg)) {
        SIGNATURE_TOOLS_LOGE("[Sign] get Signature Algorithm failed");
        return false;
    }
    signatureAlgorithms.push_back(alg);
    inOut.SetSignatureAlgorithms(signatureAlgorithms);
    if (crl.has_value()) {
        SIGNATURE_TOOLS_LOGE("not support crl");
        return false;
    }
    return true;
}

int SignProvider::LoadOptionalBlocks()
{
    int ret = RET_OK;
    if (auto property = signParams.find(ParamConstants::PARAM_BASIC_PROPERTY);
        property != signParams.end()) {
        if ((ret = LoadOptionalBlock(property->second, HapUtils::HAP_PROPERTY_BLOCK_ID)) != RET_OK)
            return ret;
    }
    if (auto profile = signParams.find(ParamConstants::PARAM_BASIC_PROFILE); profile != signParams.end()) {
        if ((ret = LoadOptionalBlock(profile->second, HapUtils::HAP_PROFILE_BLOCK_ID)) != RET_OK)
            return ret;
    }
    if (auto proofOfRotation = signParams.find(ParamConstants::PARAM_BASIC_PROOF);
        proofOfRotation != signParams.end()) {
        if ((LoadOptionalBlock(proofOfRotation->second, HapUtils::HAP_PROOF_OF_ROTATION_BLOCK_ID)) != RET_OK)
            return ret;
    }
    return ret;
}

int SignProvider::LoadOptionalBlock(const std::string& file, int type)
{
    if (file.empty())
        return RET_OK;
    if (!CheckFile(file)) {
        SIGNATURE_TOOLS_LOGE("check file failed. Invalid file: %s, file type: %d",
                             file.c_str(), type);
        return FILE_NOT_FOUND;
    }
    ByteBuffer optionalBlockBuffer;
    if (!FileUtils::ReadFileToByteBuffer(file, optionalBlockBuffer))
        return IO_ERROR;
    if (optionalBlockBuffer.GetCapacity() == 0) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, file + " is empty!");
        return IO_ERROR;
    }
    optionalBlocks.push_back({type, optionalBlockBuffer});
    return RET_OK;
}

std::optional<X509_CRL*> SignProvider::GetCrl()
{
    return std::nullopt;
}

bool SignProvider::CheckFile(const std::string& filePath)
{
    if (filePath.empty()) {
        PrintErrorNumberMsg("FILE_NOT_FOUND", FILE_NOT_FOUND, "file name is null.");
        return false;
    }
    if (!std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath)) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, filePath + " not exist or can not read!");
        return false;
    }
    return true;
}

int SignProvider::GetX509Certificates(Options* options, STACK_OF(X509)** X509Vec)
{
    int ret = RET_OK;
    // 1.check the parameters
    if (!CheckParams(options)) {
        SIGNATURE_TOOLS_LOGE("Check Params failed please check");
        return COMMAND_ERROR;
    }
    // 2.get x509 verify certificate
    ret = GetPublicCerts(options, X509Vec);
    if (ret != RET_OK) {
        SIGNATURE_TOOLS_LOGE("Get Public Certs please check");
        return ret;
    }
    // 3. load optionalBlocks
    ret = LoadOptionalBlocks();
    if (ret != RET_OK) {
        SIGNATURE_TOOLS_LOGE("Load Optional Blocks please check");
        return ret;
    }
    std::string profileFile = options->GetString(Options::PROFILE_FILE);
    if (FileUtils::IsEmpty(profileFile)) {
        return ret;
    }
    // 4. check Profile Valid
    if ((ret = CheckProfileValid(*X509Vec)) < 0) {
        SIGNATURE_TOOLS_LOGE("profile check error");
        sk_X509_pop_free(*X509Vec, X509_free);
        *X509Vec = nullptr;
        return ret;
    }
    return ret;
}

int SignProvider::GetPublicCerts(Options* options, STACK_OF(X509)** ret)
{
    std::string appCertFileName = options->GetString(Options::APP_CERT_FILE);
    if (appCertFileName.empty()) {
        SIGNATURE_TOOLS_LOGI("appCertFile param can not find,may be is RemoteSigner");
        return RET_OK;
    }
    return GetCertificateChainFromFile(appCertFileName, ret);
}

int SignProvider::GetCertificateChainFromFile(const std::string& certChianFile, STACK_OF(X509)** ret)
{
    return GetCertListFromFile(certChianFile, ret);
}

int SignProvider::GetCertListFromFile(const std::string& certsFile, STACK_OF(X509)** ret)
{
    X509* cert = nullptr;
    *ret = sk_X509_new(nullptr);
    if (*ret == nullptr) {
        PrintErrorNumberMsg("RET_FAILED", RET_FAILED, "get CertList FromFile [sk_X509_new] failed");
        return RET_FAILED;
    }
    BIO* certBio = BIO_new_file(certsFile.c_str(), "rb");
    if (!certBio) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            std::string("get CertList from file ") + certsFile + " failed");
        sk_X509_free(*ret);
        return IO_ERROR;
    }
    while (1) {
        cert = PEM_read_bio_X509(certBio, NULL, NULL, NULL);
        if (cert == nullptr)
            break;
        sk_X509_push(*ret, cert);
    }
    BIO_free(certBio);
    return RET_OK;
}

bool SignProvider::SetSignParams(Options* options, std::unordered_set<std::string>& paramSet)
{
    for (auto it = options->begin(); it != options->end(); it++) {
        if (paramSet.find(it->first) != paramSet.end()) {
            size_t size = it->first.size();
            std::string str = it->first.substr(size - 3);
            if (str != "Pwd") {
                this->signParams.insert(std::make_pair(it->first, options->GetString(it->first)));
            }
        }
    }
    return true;
}

bool SignProvider::CheckParams(Options* options)
{
    std::vector<std::string> paramFileds;
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_ALIGNMENT);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_SIGANTURE_ALG);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_INPUT_FILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_OUTPUT_FILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PRIVATE_KEY);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROFILE);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROOF);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROPERTY);
    paramFileds.emplace_back(ParamConstants::PARAM_REMOTE_SERVER);
    paramFileds.emplace_back(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    paramFileds.emplace_back(ParamConstants::PARAM_LOCAL_PUBLIC_CERT);
    paramFileds.emplace_back(ParamConstants::PARAM_SIGN_CODE);
    paramFileds.emplace_back(ParamConstants::PARAM_MODULE_FILE);
    paramFileds.emplace_back(ParamConstants::PARAM_AD_HOC);

    std::unordered_set<std::string> paramSet = Params::InitParamField(paramFileds);
    for (auto it = options->begin(); it != options->end(); it++) {
        if (paramSet.find(it->first) != paramSet.end()) {
            signParams.insert(std::make_pair(it->first, options->GetString(it->first)));
        }
    }

    if (signParams.find(ParamConstants::PARAM_BASIC_PROFILE_SIGNED) == signParams.end()
        || signParams.at(ParamConstants::PARAM_BASIC_PROFILE_SIGNED).empty()) {
        signParams[ParamConstants::PARAM_BASIC_PROFILE_SIGNED] = DEFAULT_PROFILE_SIGNED_1;
    }
    if (signParams.find(ParamConstants::PARAM_AD_HOC) == signParams.end()
        || signParams.at(ParamConstants::PARAM_AD_HOC).empty()) {
        signParams[ParamConstants::PARAM_AD_HOC] = ParamConstants::AD_HOC_TYPE_0;
    }
    if (!CheckSignatureAlg()) {
        SIGNATURE_TOOLS_LOGE("signAlg Parameter is not support");
        return false;
    }
    CheckSignAlignment();
    return true;
}

bool SignProvider::CheckSignatureAlg()
{
    std::string signAlg = signParams[ParamConstants::PARAM_BASIC_SIGANTURE_ALG];
    // Remove leading spaces
    size_t start = signAlg.find_first_not_of(" ");
    if (start != std::string::npos) {
        signAlg = signAlg.substr(start);
    }
    // Remove trailing spaces
    size_t end = signAlg.find_last_not_of(" ");
    if (end != std::string::npos) {
        signAlg.resize(end + 1);
    }
    for (auto it = VALID_SIGN_ALG_NAME.begin(); it != VALID_SIGN_ALG_NAME.end(); it++) {
        if (StringUtils::CaseCompare(*it, signAlg)) {
            return true;
        }
    }
    PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                        "signAlg Parameter is only support [SHA256withECDSA / SHA384withECDSA]");
    return false;
}

void SignProvider::CheckSignAlignment()
{
    if (signParams.find(ParamConstants::PARAM_BASIC_ALIGNMENT) == signParams.end()) {
        signParams.insert(std::make_pair(ParamConstants::PARAM_BASIC_ALIGNMENT, ParamConstants::ALIGNMENT));
    }
}

X509* SignProvider::GetCertificate(const std::string& certificate)const
{
    BIO* in = BIO_new_mem_buf(certificate.data(), certificate.size());
    if (!in) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "bio new error ,get ceritificate from base64 certificate failed");
        return NULL;
    }
    X509* cert = NULL;
    cert = PEM_read_bio_X509(in, NULL, NULL, NULL);
    if (!cert) {
        BIO_free(in);
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR,
                            "read no cert from base64 certificate failed");
        return NULL;
    }
    BIO_free(in);
    return cert;
}

std::string SignProvider::GetCertificateCN(X509* cert)const
{
    X509_NAME* name = NULL;
    int len = 0;
    std::string ret;
    if (cert == NULL) {
        return "";
    }
    name = X509_get_subject_name(cert);
    if (!name) {
        SIGNATURE_TOOLS_LOGE("name is NULL,get X509_NAME from cert failed");
    }
    len = X509_NAME_get_text_by_NID(name, NID_countryName, NULL, 0);
    if (len <= 0) {
        return "";
    }
    ret.resize(len + 1);
    if (X509_NAME_get_text_by_NID(name, NID_countryName, &ret[0], len + 1) != len) {
        return "";
    }
    return ret;
}

std::string SignProvider::FindProfileFromOptionalBlocks()const
{
    std::string profile;
    for (const OptionalBlock& optionalBlock : optionalBlocks) {
        if (optionalBlock.optionalType == HapUtils::HAP_PROFILE_BLOCK_ID) {
            profile = std::string(optionalBlock.optionalBlockValue.GetBufferPtr(),
                                  optionalBlock.optionalBlockValue.GetCapacity());
        }
    }
    return profile;
}

int SignProvider::CheckProfileValid(STACK_OF(X509)* inputCerts)
{
    std::string profile = FindProfileFromOptionalBlocks();
    std::map<std::string, std::string>::const_iterator ite =
        signParams.find(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    if (ite == signParams.end()) {
        PrintErrorNumberMsg("INVALIDPARAM_ERROR", INVALIDPARAM_ERROR,
                            "find PARAM_BASIC_PROFILE_SIGNED failed");
        return INVALIDPARAM_ERROR;
    }
    bool isProfileWithoutSign = (DEFAULT_PROFILE_SIGNED_0 == ite->second);
    if (!isProfileWithoutSign) {
        PKCS7Data p7Data;
        if (p7Data.Parse(profile) < 0) {
            SIGNATURE_TOOLS_LOGE("Parse profile error.");
            return PARSE_ERROR;
        }
        if (p7Data.Verify() < 0) {
            SIGNATURE_TOOLS_LOGE("Verify profile pkcs7 failed! Profile is invalid.");
            return VERIFY_ERROR;
        }
        profileContent.clear();
        if (p7Data.GetContent(profileContent) < 0) {
            SIGNATURE_TOOLS_LOGE("get content data failed");
            return  INVALIDPARAM_ERROR;
        }
    } else {
        profileContent = profile;
    }

    ProfileInfo info;
    if (ParseProvision(profileContent, info) != PROVISION_OK) {
        SIGNATURE_TOOLS_LOGE("parse provision error");
        return PARSE_ERROR;
    }
    if (CheckProfileInfo(info, inputCerts) < 0) {
        SIGNATURE_TOOLS_LOGE("Check Profile Info error");
        return RET_FAILED;
    }
    return 0;
}

int SignProvider::CheckProfileInfo(const ProfileInfo& info, STACK_OF(X509)* inputCerts)const
{
    X509* certInProfile = NULL;
    if (info.type == ProvisionType::RELEASE) {
        certInProfile = GetCertificate(info.bundleInfo.distributionCertificate);
    } else if (info.type == ProvisionType::DEBUG) {
        certInProfile = GetCertificate(info.bundleInfo.developmentCertificate);
    } else {
        PrintErrorNumberMsg("NOT_SUPPORT_ERROR", NOT_SUPPORT_ERROR, "Unsupported profile type!");
        return NOT_SUPPORT_ERROR;
    }
    if (sk_X509_num(inputCerts) > 0 && !CheckInputCertMatchWithProfile(sk_X509_value(inputCerts, 0),
                                                                       certInProfile)) {
        X509_free(certInProfile);
        SIGNATURE_TOOLS_LOGE("input certificates do not match with profile!");
        return RET_FAILED;
    }
    std::string cn = GetCertificateCN(certInProfile);
    X509_free(certInProfile);
    SIGNATURE_TOOLS_LOGI("certificate in profile: %s", cn.c_str());
    if (cn.empty()) {
        PrintErrorNumberMsg("CERTIFICATE_ERROR", CERTIFICATE_ERROR, "Common name of certificate is empty!");
        return CERTIFICATE_ERROR;
    }
    return 0;
}

bool SignProvider::CheckInputCertMatchWithProfile(X509* inputCert, X509* certInProfile)const
{
    return true;
}
} // namespace SignatureTools
} // namespace OHOS