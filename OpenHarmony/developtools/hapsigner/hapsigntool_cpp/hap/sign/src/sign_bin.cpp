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

#include "sign_bin.h"
#include "param_constants.h"
#include "file_utils.h"
#include "block_head.h"
#include "signature_block_types.h"
#include "signature_block_tags.h"
#include "hash_utils.h"
#include "sign_content_info.h"
#include "sign_head.h"
#include "bc_pkcs7_generator.h"
#include "params.h"

namespace OHOS {
namespace SignatureTools {

bool SignBin::Sign(SignerConfig& signerConfig, const std::map<std::string, std::string>& signParams)
{
    /* 1. Make block head, write to output file. */
    std::string signCode = signParams.at(ParamConstants::PARAM_SIGN_CODE);
    if (ParamConstants::ENABLE_SIGN_CODE == signCode) {
        SIGNATURE_TOOLS_LOGW("can not sign bin with codesign.\n");
    }
    std::string inputFile = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    std::string outputFile = signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE);
    std::string profileFile = signParams.at(ParamConstants::PARAM_BASIC_PROFILE);
    std::string profileSigned = signParams.at(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    bool writeBlockOk = WriteBlockDataToFile(inputFile, outputFile, profileFile, profileSigned);
    if (!writeBlockOk) {
        SIGNATURE_TOOLS_LOGE("The block head data made failed.");
        FileUtils::DelDir(outputFile);
        return false;
    }
    /* 2. Make sign data, and append write to output file */
    std::string signAlg = signParams.at(ParamConstants::PARAM_BASIC_SIGANTURE_ALG);
    bool writeSignDataOk = WriteSignDataToOutputFile(signerConfig, outputFile, signAlg);
    if (!writeSignDataOk) {
        SIGNATURE_TOOLS_LOGE("The sign data made failed.");
        FileUtils::DelDir(outputFile);
        return false;
    }
    /* 3. Make sign head data, and write to output file */
    bool writeSignHeadDataOk = WriteSignHeadDataToOutputFile(inputFile, outputFile);
    if (!writeSignHeadDataOk) {
        SIGNATURE_TOOLS_LOGE("The sign head data made failed.");
        FileUtils::DelDir(outputFile);
        return false;
    }
    return true;
}

bool SignBin::WriteBlockDataToFile(const std::string& inputFile, const std::string& outputFile,
                                   const std::string& profileFile, const std::string& profileSigned)
{
    int64_t binFileLen = FileUtils::GetFileLen(inputFile);
    int64_t profileDataLen = FileUtils::GetFileLen(profileFile);
    bool isValid = CheckBinAndProfileLengthIsValid(binFileLen, profileDataLen);
    if (!isValid) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "file length is invalid, binFileLen: " + std::to_string(binFileLen) +
                            "lld, profileDataLen: " + std::to_string(profileDataLen) + "lld");
        return false;
    }
    int64_t offset = binFileLen + BlockHead::GetBlockLen() + BlockHead::GetBlockLen();
    bool isOver = IsLongOverflowInteger(offset);
    if (isOver) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
            "The profile block head offset is overflow interger range in sign bin, offset: "
            + std::to_string(offset));
        return false;
    }
    char isSigned = SignatureBlockTypes::GetProfileBlockTypes(profileSigned);
    std::string proBlockByte =
        BlockHead::GetBlockHead(isSigned, SignatureBlockTags::DEFAULT, (short)profileDataLen, (int)offset);
    offset += profileDataLen;
    isOver = IsLongOverflowInteger(offset);
    if (isOver) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
            "The profile block head offset is overflow interger range in sign bin, offset: "
            + std::to_string(offset));
        return false;
    }
    std::string signBlockByte = BlockHead::GetBlockHead(
        SignatureBlockTypes::SIGNATURE_BLOCK, SignatureBlockTags::DEFAULT, (short)0, (int)offset);
    return WriteSignedBin(inputFile, proBlockByte, signBlockByte, profileFile, outputFile);
}

std::vector<int8_t> SignBin::GenerateFileDigest(const std::string& outputFile,
                                                const std::string& signAlg)
{
    SignatureAlgorithmHelper signatureAlgorithmClass;
    bool getAlgOk = Params::GetSignatureAlgorithm(signAlg, signatureAlgorithmClass);
    if (!getAlgOk) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "[SignHap] get Signature Algorithm failed.");
        return std::vector<int8_t>();
    }
    std::string alg = signatureAlgorithmClass.m_contentDigestAlgorithm.GetDigestAlgorithm();
    std::vector<int8_t> data = HashUtils::GetFileDigest(outputFile, alg);
    if (data.empty()) {
        SIGNATURE_TOOLS_LOGE("GetFileDigest failed.");
        return std::vector<int8_t>();
    }
    SignContentInfo contentInfo;
    contentInfo.AddContentHashData(0, SignatureBlockTags::HASH_ROOT_4K, HashUtils::GetHashAlgsId(alg),
                                   data.size(), data);
    return contentInfo.GetByteContent();
}

bool SignBin::WriteSignDataToOutputFile(SignerConfig& SignerConfig, const std::string& outputFile,
                                        const std::string& signAlg)
{
    std::vector<int8_t> dig = GenerateFileDigest(outputFile, signAlg);
    if (dig.empty()) {
        SIGNATURE_TOOLS_LOGE("generateSignature verity digest is null.");
        return false;
    }
    std::string retStr;
    std::string signedData(dig.begin(), dig.end());
    std::unique_ptr<Pkcs7Generator> generator = std::make_unique<BCPkcs7Generator>();
    if (generator->GenerateSignedData(signedData, &SignerConfig, retStr)) {
        SIGNATURE_TOOLS_LOGE("failed to GenerateSignedData!");
        return false;
    }
    bool writeByteToOutFile = FileUtils::AppendWriteByteToFile(retStr, outputFile);
    if (!writeByteToOutFile) {
        SIGNATURE_TOOLS_LOGE("write signedData to outputFile failed!");
        return false;
    }
    return true;
}

bool SignBin::WriteSignHeadDataToOutputFile(const std::string& inputFile, const std::string& outputFile)
{
    int64_t size = FileUtils::GetFileLen(outputFile) - FileUtils::GetFileLen(inputFile) + SignHead::SIGN_HEAD_LEN;
    bool isOver = IsLongOverflowInteger(size);
    if (isOver) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
            "File size is Overflow integer range, size: " + std::to_string(size));
        return false;
    }
    SignHead signHeadData;
    std::vector<int8_t> signHeadByte = signHeadData.GetSignHead(size);
    if (signHeadByte.empty()) {
        SIGNATURE_TOOLS_LOGE("Failed to get sign head data!");
        return false;
    }
    bool writeByteToOutFile =
        FileUtils::AppendWriteByteToFile(std::string(signHeadByte.begin(), signHeadByte.end()), outputFile);
    if (!writeByteToOutFile) {
        SIGNATURE_TOOLS_LOGE("Failed to WriteByteToOutFile!");
        return false;
    }
    return true;
}

bool SignBin::CheckBinAndProfileLengthIsValid(int64_t binFileLen, int64_t profileDataLen)
{
    return binFileLen != -1 && profileDataLen != -1 && !IsLongOverflowShort(profileDataLen);
}

bool SignBin::IsLongOverflowInteger(int64_t num)
{
    return (num - (num & 0xffffffffL)) != 0;
}

bool SignBin::IsLongOverflowShort(int64_t num)
{
    return (num - (num & 0xffff)) != 0;
}

bool SignBin::WriteSignedBin(const std::string& inputFile, const std::string& proBlockByte,
                             const std::string& signBlockByte, const std::string& profileFile,
                             const std::string& outputFile)
{
    // 1. write the input file to the output file.
    bool writeInputOk = FileUtils::WriteInputToOutPut(inputFile, outputFile);
    // 2. append write profile block head to the output file.
    bool appendProfileHeadOk = FileUtils::AppendWriteByteToFile(proBlockByte, outputFile);
    // 3. append write sign block head to the output file.
    bool appendBlockHeadOk = FileUtils::AppendWriteByteToFile(signBlockByte, outputFile);
    // 4. write profile src file to the output file.
    bool appendProfileSrcOk = FileUtils::AppendWriteFileToFile(profileFile, outputFile);
    if (!writeInputOk || !appendProfileHeadOk || !appendBlockHeadOk || !appendProfileSrcOk) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Failed to write signed bin");
        return false;
    }
    return true;
}

} // namespace SignatureTools
} // namespace OHOS