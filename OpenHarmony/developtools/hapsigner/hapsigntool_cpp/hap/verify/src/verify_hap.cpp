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
#include <climits>
#include <cstdlib>
#include <regex>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "securec.h"
#include "hap_signer_block_utils.h"
#include "signature_info.h"
#include "options.h"
#include "openssl/pem.h"
#include "pkcs7_data.h"
#include "hap_utils.h"
#include "string_utils.h"
#include "verify_code_signature.h"
#include "param_constants.h"
#include "file_utils.h"
#include "nlohmann/json.hpp"
#include "verify_hap.h"

using namespace nlohmann;
namespace OHOS {
namespace SignatureTools {
const int32_t VerifyHap::HEX_PRINT_LENGTH = 3;
const int32_t VerifyHap::DIGEST_BLOCK_LEN_OFFSET = 8;
const int32_t VerifyHap::DIGEST_ALGORITHM_OFFSET = 12;
const int32_t VerifyHap::DIGEST_LEN_OFFSET = 16;
const int32_t VerifyHap::DIGEST_OFFSET_IN_CONTENT = 20;
const std::string VerifyHap::HAP_APP_PATTERN = "[^]*.hap$";
const std::string VerifyHap::HQF_APP_PATTERN = "[^]*.hqf$";
const std::string VerifyHap::HSP_APP_PATTERN = "[^]*.hsp$";
const std::string VerifyHap::APP_APP_PATTERN = "[^]*.app$";
static constexpr int ZIP_HEAD_OF_SUBSIGNING_BLOCK_LENGTH = 12;

VerifyHap::VerifyHap() : isPrintCert(true)
{
}

VerifyHap::VerifyHap(bool printCert)
{
    isPrintCert = printCert;
}

void VerifyHap::setIsPrintCert(bool printCert)
{
    isPrintCert = printCert;
}

bool VerifyHap::HapOutPutPkcs7(PKCS7* p7, const std::string& outPutPath)
{
    std::string p7bContent = StringUtils::Pkcs7ToString(p7);
    if (p7bContent.empty()) {
        SIGNATURE_TOOLS_LOGE("p7b to string failed!\n");
        return false;
    }
    if (FileUtils::Write(p7bContent, outPutPath) < 0) {
        SIGNATURE_TOOLS_LOGE("p7b write to file falied!\n");
        return false;
    }
    return true;
}

bool VerifyHap::outputOptionalBlocks(const std::string& outputProfileFile, const std::string& outputProofFile,
                                     const std::string& outputPropertyFile,
                                     const std::vector<OptionalBlock>& optionBlocks)
{
    for (auto& optionBlock : optionBlocks) {
        if (optionBlock.optionalType == HapUtils::HAP_PROFILE_BLOCK_ID) {
            if (!writeOptionalBytesToFile(optionBlock, outputProfileFile)) {
                return false;
            }
        } else if (optionBlock.optionalType == HapUtils::HAP_PROPERTY_BLOCK_ID) {
            if (!writeOptionalBytesToFile(optionBlock, outputPropertyFile)) {
                return false;
            }
        } else if (optionBlock.optionalType == HapUtils::HAP_PROOF_OF_ROTATION_BLOCK_ID) {
            if (!writeOptionalBytesToFile(optionBlock, outputProofFile)) {
                return false;
            }
        } else {
            SIGNATURE_TOOLS_LOGE("Unsupported Block Id: %d", optionBlock.optionalType);
            return false;
        }
    }
    return true;
}
bool VerifyHap::writeOptionalBytesToFile(const OptionalBlock& optionalBlock, const std::string& path)
{
    if (path.empty()) {
        return true;
    }
    std::string optionBlockString(optionalBlock.optionalBlockValue.GetBufferPtr(),
                          optionalBlock.optionalBlockValue.GetCapacity());
    if (FileUtils::Write(optionBlockString, path) < 0) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "write optional bytes to file:" + path + " falied!");
        return false;
    }
    return true;
}

bool VerifyHap::HapOutPutCertChain(std::vector<X509*>& certs, const std::string& outPutPath)
{
    if (isPrintCert) {
        if (!PrintCertChainToCmd(certs)) {
            SIGNATURE_TOOLS_LOGE("print cert chain to cmd failed\n");
            return false;
        }
    }
    VerifyHapOpensslUtils::GetOpensslErrorMessage();
    SIGNATURE_TOOLS_LOGD("outPutPath = %s", outPutPath.c_str());
    std::vector<std::string> certStr;
    for (auto& cert : certs) {
        certStr.emplace_back(StringUtils::SubjectToString(cert));
        certStr.emplace_back(StringUtils::x509CertToString(cert));
    }
    std::string outPutCertChainContent = std::accumulate(certStr.begin(), certStr.end(), std::string(),
        [](std::string sum, const std::string& certstr) { return sum + certstr; });
    if (FileUtils::Write(outPutCertChainContent, outPutPath) < 0) {
        SIGNATURE_TOOLS_LOGE("certChain write to file falied!\n");
        return false;
    }
    return true;
}

bool VerifyHap::PrintCertChainToCmd(std::vector<X509*>& certChain)
{
    BIO* outFd = BIO_new_fp(stdout, BIO_NOCLOSE);
    if (!outFd) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "The stdout stream may have errors");
        return false;
    }
    uint64_t format = XN_FLAG_SEP_COMMA_PLUS; // Print according to RFC2253
    uint64_t content = X509_FLAG_NO_EXTENSIONS | X509_FLAG_NO_ATTRIBUTES | X509_FLAG_NO_HEADER | X509_FLAG_NO_SIGDUMP;
    int num = 0;
    for (auto& cert : certChain) {
        PrintMsg("+++++++++++++++++++++++++++++++++certificate #" + std::to_string(num) +
                 "+++++++++++++++++++++++++++++++++++++");
        if (!X509_print_ex(outFd, cert, format, content)) {
            VerifyHapOpensslUtils::GetOpensslErrorMessage();
            SIGNATURE_TOOLS_LOGE("print x509 cert to cmd failed");
            BIO_free(outFd);
            return false;
        }
        ++num;
    }
    BIO_free(outFd);
    return true;
}

int32_t VerifyHap::Verify(const std::string& filePath, Options* options)
{
    SIGNATURE_TOOLS_LOGD("Start Verify");
    std::string standardFilePath;
    if (!CheckFilePath(filePath, standardFilePath)) {
        SIGNATURE_TOOLS_LOGE("Check file path%s failed", filePath.c_str());
        return IO_ERROR;
    }
    RandomAccessFile hapFile;
    if (!hapFile.Init(standardFilePath)) {
        SIGNATURE_TOOLS_LOGE("%s init failed", standardFilePath.c_str());
        return ZIP_ERROR;
    }
    int32_t resultCode = Verify(hapFile, options, filePath);
    if (resultCode != RET_OK) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, standardFilePath + " verify failed");
    }
    return resultCode;
}

bool VerifyHap::CheckFilePath(const std::string& filePath, std::string& standardFilePath)
{
    char path[PATH_MAX] = { 0x00 };
    if (filePath.size() > PATH_MAX || realpath(filePath.c_str(), path) == nullptr) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                            filePath + " does not exist or is over " + std::to_string(PATH_MAX) + " chars");
        return false;
    }
    standardFilePath = std::string(path);
    std::string standardFilePathTmp = std::string(path);
    std::transform(standardFilePathTmp.begin(), standardFilePathTmp.end(), standardFilePathTmp.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    bool ret = (!std::regex_match(standardFilePathTmp, std::regex(HAP_APP_PATTERN)) &&
                !std::regex_match(standardFilePathTmp, std::regex(HSP_APP_PATTERN)) &&
                !std::regex_match(standardFilePathTmp, std::regex(APP_APP_PATTERN)) &&
                !std::regex_match(standardFilePathTmp, std::regex(HQF_APP_PATTERN)));
    if (ret) {
        PrintErrorNumberMsg("COMMAND_PARAM_ERROR", COMMAND_PARAM_ERROR,
                            "only support format is [hap, hqf, hsp, app]");
        return false;
    }
    return true;
}

int32_t VerifyHap::Verify(RandomAccessFile& hapFile, Options* options, const std::string& filePath)
{
    SignatureInfo hapSignInfo;
    if (!HapSignerBlockUtils::FindHapSignature(hapFile, hapSignInfo)) {
        return ZIP_ERROR;
    }

    if (CheckCodeSign(filePath, hapSignInfo.optionBlocks) == false) {
        SIGNATURE_TOOLS_LOGE("check coode sign failed\n");
        return VERIFY_ERROR;
    }

    Pkcs7Context pkcs7Context;
    if (!VerifyAppPkcs7(pkcs7Context, hapSignInfo.hapSignatureBlock)) {
        return PARSE_ERROR;
    }

    if (!GetDigestAndAlgorithm(pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("Get digest failed");
        return PARSE_ERROR;
    }

    STACK_OF(X509_CRL)* x509Crl = nullptr;
    if (!VerifyHapOpensslUtils::GetCrlStack(pkcs7Context.p7, x509Crl)) {
        SIGNATURE_TOOLS_LOGE("Get Crl stack failed");
        return PARSE_ERROR;
    }

    if (!VerifyCertOpensslUtils::VerifyCrl(pkcs7Context.certChain[0], x509Crl, pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("Verify Crl stack failed");
        return VERIFY_ERROR;
    }

    if (!HapSignerBlockUtils::VerifyHapIntegrity(pkcs7Context, hapFile, hapSignInfo)) {
        SIGNATURE_TOOLS_LOGE("Verify Integrity failed");
        return VERIFY_ERROR;
    }
    if (!HapOutPutCertChain(pkcs7Context.certChain[0],
        options->GetString(Options::OUT_CERT_CHAIN))) {
        SIGNATURE_TOOLS_LOGE("out put cert chain failed");
        return IO_ERROR;
    }

    if (!outputOptionalBlocks(options->GetString(ParamConstants::PARAM_VERIFY_PROFILE_FILE),
                              options->GetString(ParamConstants::PARAM_VERIFY_PROOF_FILE),
                              options->GetString(ParamConstants::PARAM_VERIFY_PROPERTY_FILE),
                              hapSignInfo.optionBlocks)) {
        SIGNATURE_TOOLS_LOGE("output Optional Blocks failed");
        return IO_ERROR;
    }
    return RET_OK;
}

bool VerifyHap::CheckCodeSign(const std::string& hapFilePath,
                              const std::vector<OptionalBlock>& optionalBlocks)const
{
    std::unordered_map<int, ByteBuffer> map;
    for (const OptionalBlock& block : optionalBlocks) {
        map.emplace(block.optionalType, block.optionalBlockValue);
    }
    bool codeSignFlag = map.find(HapUtils::HAP_PROPERTY_BLOCK_ID) != map.end() &&
        map[HapUtils::HAP_PROPERTY_BLOCK_ID].GetCapacity() > 0;
    if (codeSignFlag) {
        ByteBuffer propertyBlockArray = map[HapUtils::HAP_PROPERTY_BLOCK_ID];
        std::vector<std::string> fileNameArray = StringUtils::SplitString(hapFilePath, '.');
        if (fileNameArray.size() < ParamConstants::FILE_NAME_MIN_LENGTH) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "ZIP64 format not supported.");
            return false;
        }

        if (propertyBlockArray.GetCapacity() < ZIP_HEAD_OF_SUBSIGNING_BLOCK_LENGTH)
            return false;
        uint32_t blockType;
        propertyBlockArray.GetUInt32(OFFSET_ZERO, blockType);
        uint32_t blockLength;
        propertyBlockArray.GetUInt32(OFFSET_FOUR, blockLength);
        uint32_t blockOffset;
        propertyBlockArray.GetUInt32(OFFSET_EIGHT, blockOffset);

        if (blockType != HapUtils::HAP_CODE_SIGN_BLOCK_ID) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "code sign data not exist in hap " + hapFilePath);
            return false;
        }
        auto ite = map.find(HapUtils::HAP_PROFILE_BLOCK_ID);
        if (ite == map.end())
            return false;
        ByteBuffer profileArray = ite->second;
        std::string profileArray_(profileArray.GetBufferPtr(), profileArray.GetCapacity());
        std::string profileContent;
        if (GetProfileContent(profileArray_, profileContent) < 0) {
            SIGNATURE_TOOLS_LOGE("get profile content failed, file: %s", hapFilePath.c_str());
            return false;
        }
        std::string suffix = fileNameArray[fileNameArray.size() - 1];
        bool isCodeSign = VerifyCodeSignature::VerifyHap(hapFilePath, blockOffset, blockLength,
                                                         suffix, profileContent);
        if (!isCodeSign) {
            SIGNATURE_TOOLS_LOGE("verify codesign failed, file: %s", hapFilePath.c_str());
            return false;
        }
        SIGNATURE_TOOLS_LOGI("verify codesign success.");
        return true;
    }
    SIGNATURE_TOOLS_LOGI("can not find codesign block.");
    return true;
}

int VerifyHap::GetProfileContent(const std::string profile, std::string& ret)
{
    json obj = json::parse(profile, nullptr, false);
    if (!obj.is_discarded() && obj.is_structured()) {
        ret = profile;
        return 0;
    }
    PKCS7Data p7Data;
    if (p7Data.Parse(profile) < 0) {
        ret = profile;
        return -1;
    }
    if (p7Data.Verify() < 0) {
        PrintErrorNumberMsg("PKCS7_VERIFY_ERROR", VERIFY_ERROR,
                            "Verify profile pkcs7 failed! Profile is invalid");
        ret = profile;
        return -1;
    }
    if (p7Data.GetContent(ret) < 0) {
        PrintErrorNumberMsg("PKCS7_VERIFY_ERROR", VERIFY_ERROR,
                            "Check profile failed, signed profile content is not byte array");
        ret = profile;
        return -1;
    }
    return 0;
}

bool VerifyHap::VerifyAppPkcs7(Pkcs7Context& pkcs7Context, const ByteBuffer& hapSignatureBlock)
{
    const unsigned char* pkcs7Block = reinterpret_cast<const unsigned char*>(hapSignatureBlock.GetBufferPtr());
    uint32_t pkcs7Len = static_cast<unsigned int>(hapSignatureBlock.GetCapacity());
    if (!VerifyHapOpensslUtils::ParsePkcs7Package(pkcs7Block, pkcs7Len, pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("parse pkcs7 failed");
        return false;
    }
    if (!VerifyHapOpensslUtils::GetCertChains(pkcs7Context.p7, pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("GetCertChains from pkcs7 failed");
        return false;
    }
    if (!VerifyHapOpensslUtils::VerifyPkcs7(pkcs7Context)) {
        SIGNATURE_TOOLS_LOGE("verify signature failed");
        return false;
    }
    return true;
}

bool VerifyHap::GetDigestAndAlgorithm(Pkcs7Context& digest)
{
    /*
     * contentinfo format:
     * int: version
     * int: block number
     * digest blocks:
     * each digest block format:
     * int: length of sizeof(digestblock) - 4
     * int: Algorithm ID
     * int: length of digest
     * byte[]: digest
     */
     /* length of sizeof(digestblock - 4) */
    int32_t digestBlockLen;
    if (!digest.content.GetInt32(DIGEST_BLOCK_LEN_OFFSET, digestBlockLen)) {
        SIGNATURE_TOOLS_LOGE("get digestBlockLen failed");
        return false;
    }
    /* Algorithm ID */
    if (!digest.content.GetInt32(DIGEST_ALGORITHM_OFFSET, digest.digestAlgorithm)) {
        SIGNATURE_TOOLS_LOGE("get digestAlgorithm failed");
        return false;
    }
    /* length of digest */
    int32_t digestlen;
    if (!digest.content.GetInt32(DIGEST_LEN_OFFSET, digestlen)) {
        SIGNATURE_TOOLS_LOGE("get digestlen failed");
        return false;
    }
    int32_t sum = sizeof(digestlen) + sizeof(digest.digestAlgorithm) + digestlen;
    if (sum != digestBlockLen) {
        SIGNATURE_TOOLS_LOGE("digestBlockLen: %d is not equal to sum: %d",
                             digestBlockLen, sum);
        return false;
    }
    /* set position to the digest start point */
    digest.content.SetPosition(DIGEST_OFFSET_IN_CONTENT);
    /* set limit to the digest end point */
    digest.content.SetLimit(DIGEST_OFFSET_IN_CONTENT + digestlen);
    digest.content.Slice();
    return true;
}

int32_t VerifyHap::WriteVerifyOutput(Pkcs7Context& pkcs7Context, std::vector<int8_t>& profile, Options* options)
{
    if (pkcs7Context.certChain.size() > 0) {
        bool flag = VerifyHap::HapOutPutCertChain(pkcs7Context.certChain[0],
            options->GetString(Options::OUT_CERT_CHAIN));
        if (!flag) {
            SIGNATURE_TOOLS_LOGE("out put cert chain failed");
            return IO_ERROR;
        }
    }
    if (pkcs7Context.p7 == nullptr) {
        std::string p7bContent(profile.begin(), profile.end());
        bool writeFlag = FileUtils::Write(p7bContent, options->GetString(Options::OUT_PROFILE)) < 0;
        if (writeFlag) {
            SIGNATURE_TOOLS_LOGE("p7b write to file falied!\n");
            return IO_ERROR;
        }
        return RET_OK;
    }
    bool pkcs7flag = VerifyHap::HapOutPutPkcs7(pkcs7Context.p7, options->GetString(Options::OUT_PROFILE));
    if (!pkcs7flag) {
        SIGNATURE_TOOLS_LOGE("out put p7b failed");
        return IO_ERROR;
    }
    return RET_OK;
}
} // namespace SignatureTools
} // namespace OHOS