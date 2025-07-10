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
#include <unistd.h>

#include "sign_elf.h"
#include "string_utils.h"
#include "code_signing.h"
#include "param_constants.h"
#include "block_head.h"
#include "sign_head.h"
#include "signature_block_types.h"
#include "signature_block_tags.h"

namespace OHOS {
namespace SignatureTools {

int SignElf::blockNum = 0;
const std::string SignElf::CODESIGN_OFF = "0";

bool SignElf::Sign(SignerConfig& signerConfig, std::map<std::string, std::string>& signParams)
{
    std::string inputFile = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    std::string tmpFile;
    bool checkAlignFileBy4kBytesFlag = AlignFileBy4kBytes(inputFile, tmpFile);
    if (!checkAlignFileBy4kBytesFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] AlignFileBy4kBytes error");
        remove(tmpFile.c_str());
        return false;
    }
    std::string outputFile = signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE);
    std::string profileSigned = signParams.at(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    bool checkWriteBlockDataToFileFlag = WriteBlockDataToFile(signerConfig, tmpFile,
                                                              outputFile, profileSigned, signParams);
    if (!checkWriteBlockDataToFileFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] WriteBlockDataToFile error");
        remove(tmpFile.c_str());
        return false;
    }
    bool checkWriteSignHeadDataToOutputFileFlag = WriteSignHeadDataToOutputFile(tmpFile, outputFile, blockNum);
    if (!checkWriteSignHeadDataToOutputFileFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] WriteSignHeadDataToOutputFile error");
        remove(tmpFile.c_str());
        return false;
    }
    return (remove(tmpFile.c_str()) == 0);
    ;
}

bool SignElf::AlignFileBy4kBytes(const std::string& inputFile, std::string& tmpFile)
{
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto timeStamp = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    tmpFile = "tmpFile" + std::to_string(timeStamp);
    std::ofstream output(tmpFile);
    bool checkSpaceFlag = FileUtils::IsSpaceEnough(std::string("./"), FileUtils::GetFileLen(inputFile));
    if (!checkSpaceFlag) {
        char currentPath[FILE_PATH_LENGTH] = { 0 };
        getcwd(currentPath, FILE_PATH_LENGTH);
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] The available space of the current directory: "
                            + std::string(currentPath) + " is insufficient. Please check");
        return false;
    }
    bool checkOutputFlag = output.is_open();
    if (!checkOutputFlag) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] open file: " + tmpFile + "failed");
        return false;
    }
    std::ifstream input(inputFile);
    bool checkInputFlag = input.is_open();
    if (!checkInputFlag) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] open file: " + inputFile + "failed");
        return false;
    }
    char buffer[FILE_BUFFER_BLOCK];
    std::streamsize bytesRead;
    int64_t outputLength = 0;
    while ((bytesRead = input.read(buffer, sizeof(buffer)).gcount()) > 0) {
        output.write(buffer, bytesRead);
        if (!output) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] write data to " + tmpFile + "failed");
            return false;
        }
        outputLength += bytesRead;
    }
    int64_t addLength = PAGE_SIZE - (outputLength % PAGE_SIZE);
    std::vector<char> bytes(addLength, 0);
    output.write(bytes.data(), addLength);
    return true;
}

bool SignElf::WriteBlockDataToFile(SignerConfig& signerConfig,
                                   const std::string &inputFile, std::string& outputFile,
                                   const std::string& profileSigned,
                                   const std::map<std::string, std::string>& signParams)
{
    std::string proFile;
    if (signParams.find(ParamConstants::PARAM_BASIC_PROFILE) != signParams.end()) {
        proFile = signParams.at(ParamConstants::PARAM_BASIC_PROFILE);
    }
    std::list<SignBlockData> signDataList;
    int64_t binFileLen = FileUtils::GetFileLen(inputFile);
    bool checkFlag = binFileLen < 0 || IsLongOverflowInteger(binFileLen);
    if (checkFlag) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "[SignElf] The length exceeds the maximum limit.");
        return false;
    }
    bool checkIsEmptyFlag = StringUtils::IsEmpty(proFile);
    if (!checkIsEmptyFlag) {
        signDataList.push_front(GenerateProfileSignByte(proFile, profileSigned));
    }
    blockNum = signDataList.size() + 1;
    SignBlockData* codeSign = nullptr;
    bool checkGenerateCodeSignByteFlag = !GenerateCodeSignByte(signerConfig, signParams, inputFile, blockNum,
                                                               binFileLen, &codeSign) || !codeSign;
    if (checkGenerateCodeSignByteFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] generate code sign byte error.");
        if (codeSign) {
            delete codeSign;
        }
        return false;
    }
    signDataList.push_front(*codeSign);
    blockNum = signDataList.size();
    bool checkGenerateSignBlockHeadFlag = GenerateSignBlockHead(signDataList);
    if (!checkGenerateSignBlockHeadFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf]  generate sign block head error.");
        delete codeSign;
        return false;
    }
    delete codeSign;
    return WriteSignedElf(inputFile, signDataList, outputFile);
}

bool SignElf::WriteSignedElf(const std::string &inputFile, std::list<SignBlockData>& signBlockList,
                             std::string &outputFile)
{
    std::ifstream fileInputStream(inputFile, std::ios::binary);
    std::ofstream fileOutputStream(outputFile, std::ios::binary);
    bool checkFlag = !fileInputStream.is_open();
    if (checkFlag) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] open file: " + inputFile + "failed");
        return false;
    }
    checkFlag = !fileOutputStream.is_open();
    if (checkFlag) {
        fileInputStream.close();
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "[SignElf] open file: " + outputFile + "failed");
        return false;
    }
    char buffer[FILE_BUFFER_BLOCK];
    while (!fileInputStream.eof()) {
        fileInputStream.read(buffer, sizeof(buffer));
        fileOutputStream.write(buffer, fileInputStream.gcount());
    }
    bool writeFlag = WriteSignBlockData(signBlockList, fileOutputStream);
    if (!writeFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] write signBlockList to file error");
        fileInputStream.close();
        fileOutputStream.close();
        return false;
    }
    fileInputStream.close();
    fileOutputStream.close();
    return true;
}

bool SignElf::WriteSignBlockData(std::list<SignBlockData>& signBlockList, std::ofstream& fileOutputStream)
{
    for (auto& signBlockData : signBlockList) {
        bool checkWriteByteToOutFileFlag = FileUtils::WriteByteToOutFile(signBlockData.GetBlockHead(),
                                                                         fileOutputStream);
        if (!checkWriteByteToOutFileFlag) {
            SIGNATURE_TOOLS_LOGE("[SignElf] write data to file error");
            return false;
        }
    }
    for (auto& signBlockData : signBlockList) {
        bool isSuccess;
        bool checkFlag = signBlockData.GetByte();
        if (checkFlag) {
            isSuccess = FileUtils::WriteByteToOutFile(signBlockData.GetSignData(), fileOutputStream);
        } else {
            std::ifstream InputSignFileStream(signBlockData.GetSignFile(), std::ios::binary);
            bool checkFileFlag = !InputSignFileStream.is_open();
            if (checkFileFlag) {
                PrintErrorNumberMsg("IO_ERROR", IO_ERROR,
                                    "[SignElf] open file: " + signBlockData.GetSignFile() + "failed");
                return false;
            }
            int result = FileUtils::WriteInputToOutPut(InputSignFileStream, fileOutputStream,
                                                       (long)signBlockData.GetLen());
            isSuccess = (result == 0 ? true : false);
        }
        if (!isSuccess) {
            PrintErrorNumberMsg("IO_ERROR", IO_ERROR, "write data to file error");
            return false;
        }
    }
    return true;
}

bool SignElf::GenerateSignBlockHead(std::list<SignBlockData>& signDataList)
{
    int64_t offset = BlockHead::GetElfBlockLen() * signDataList.size();
    for (std::list<SignBlockData>::iterator it = signDataList.begin(); it != signDataList.end(); ++it) {
        std::vector<int8_t> tmp = BlockHead::GetBlockHeadLittleEndian(it->GetType(),
            SignatureBlockTags::DEFAULT, it->GetLen(), offset);
        it->SetBlockHead(tmp);
        offset += it->GetLen();
        bool checkIsLongOverflowIntegerFlag = IsLongOverflowInteger(offset);
        if (checkIsLongOverflowIntegerFlag) {
            PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "[SignElf] The length exceeds the maximum limit.");
            return false;
        }
    }
    return true;
}

SignBlockData SignElf::GenerateProfileSignByte(std::string profileFile, std::string profileSigned)
{
    int64_t profileDataLen = FileUtils::GetFileLen(profileFile);
    if (profileDataLen < 0 || IsLongOverflowShort(profileDataLen)) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "[SignElf] The length exceeds the maximum limit.");
    }
    char isSigned = SignatureBlockTypes::GetProfileBlockTypes(profileSigned);
    return SignBlockData(profileFile, isSigned);
}

bool SignElf::GenerateCodeSignByte(SignerConfig& signerConfig, const std::map<std::string, std::string> &signParams,
                                   const std::string &inputFile, const int blockNum, const long binFileLen,
                                   SignBlockData** codeSign)
{
    if (signParams.at(ParamConstants::PARAM_SIGN_CODE) == CODESIGN_OFF) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "[SignElf] check pamams signCode = 0 error.");
        return false;
    }
    CodeSigning codeSigning(&signerConfig);
    long offset = binFileLen + (long)BlockHead::GetElfBlockLen() * blockNum;
    std::string profileContent;
    if (signParams.find(ParamConstants::PARAM_PROFILE_JSON_CONTENT) != signParams.end()) {
        profileContent = signParams.at(ParamConstants::PARAM_PROFILE_JSON_CONTENT);
    }
    std::vector<int8_t> codesignData;
    bool checkGetElfCodeSignBlockFlag = codeSigning.GetElfCodeSignBlock(inputFile, offset,
                                                                        signParams.at(ParamConstants::PARAM_IN_FORM),
                                                                        profileContent, codesignData);
    if (!checkGetElfCodeSignBlockFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] get elf code sign block error.");
        return false;
    }
    *codeSign = new SignBlockData(codesignData, CODESIGN_BLOCK_TYPE);
    return true;
}

bool SignElf::WriteSignHeadDataToOutputFile(const std::string& inputFile, const std::string& outputFile,
                                            const int blockNum)
{
    int64_t size = FileUtils::GetFileLen(outputFile) - FileUtils::GetFileLen(inputFile);
    if (IsLongOverflowInteger(size)) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR,
                            "[SignElf] The length exceeds the maximum limit.");
        return false;
    }
    SignHead signHeadData;
    std::vector<int8_t> signHeadByte = signHeadData.GetSignHeadLittleEndian((int)size, blockNum);
    std::ofstream fileOutputStream(outputFile, std::ios::app | std::ios::binary);
    return FileUtils::WriteByteToOutFile(signHeadByte, fileOutputStream);
}

bool SignElf::IsLongOverflowInteger(const int64_t num)
{
    return (num - (num & 0xffffffffL)) != 0;
}

bool SignElf::IsLongOverflowShort(const int64_t num)
{
    return (num - (num & 0xffffL)) != 0;
}

} // namespace SignatureTools
} // namespace OHOS