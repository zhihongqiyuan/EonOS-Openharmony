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

#include "sign_elf.h"
#include <filesystem>
#include <unistd.h>

#include "file_utils.h"
#include "string_utils.h"
#include "constant.h"
#include "param_constants.h"
#include "profile_sign_tool.h"

namespace OHOS {
namespace SignatureTools {

int SignElf::blockNum = 0;
const std::string SignElf::profileSec = ".profile";
const std::string SignElf::permissionSec = ".permission";

bool SignElf::Sign(SignerConfig& signerConfig, std::map<std::string, std::string>& signParams)
{
    std::string inputFile = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    ELFIO::elfio elfReader;
    if (!elfReader.load(inputFile)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to load input ELF file");
        return false;
    }
    bool writeProfilFlag = WriteSecDataToFile(elfReader, signerConfig, signParams);
    if (!writeProfilFlag) {
        SIGNATURE_TOOLS_LOGE("[SignElf] WriteSecDataToFile error");
        return false;
    }
    std::string outputFile = signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE);
    std::string tmpOutputFilePath = outputFile;
    if (outputFile == inputFile) {
        tmpOutputFilePath = "tmp-signed-elf";
    }
    if (!elfReader.save(tmpOutputFilePath)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to save out ELF file");
        return false;
    }
    return FileUtils::CopyTmpFileAndDel(tmpOutputFilePath, outputFile);
}

bool SignElf::loadModule(std::map<std::string, std::string>& signParams, std::string& moduleContent)
{
    if (signParams.find(ParamConstants::PARAM_MODULE_FILE) != signParams.end()) {
        std::string modulefilePath = signParams.at(ParamConstants::PARAM_MODULE_FILE);
        if (FileUtils::ReadFile(modulefilePath, moduleContent) < 0) {
            SIGNATURE_TOOLS_LOGE("[SignElf] Failed to open module file");
            return false;
        }
    } else {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to find module file");
        return false;
    }
    return true;
}

bool SignElf::loadProfileAndSign(SignerConfig& signerConfig, std::map<std::string, std::string>& signParams,
                                 std::string& p7b)
{
    std::string profileContent;
    if (signParams.find(ParamConstants::PARAM_BASIC_PROFILE) != signParams.end()) {
        std::string profilefilePath = signParams.at(ParamConstants::PARAM_BASIC_PROFILE);
        if (FileUtils::ReadFile(profilefilePath, profileContent) < 0) {
            SIGNATURE_TOOLS_LOGE("[SignElf] Failed to open profile file");
            return false;
        }
    } else {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to find profile file");
        return false;
    }
    std::string profileSigned = signParams.at(ParamConstants::PARAM_BASIC_PROFILE_SIGNED);
    if (profileSigned == DEFAULT_PROFILE_SIGNED_0) {
        std::string alg = signParams.at(ParamConstants::PARAM_BASIC_SIGANTURE_ALG);
        if (ProfileSignTool::SignProfile(profileContent, signerConfig.GetSigner(), alg, p7b) < 0) {
            SIGNATURE_TOOLS_LOGE("[SignElf] SignProfile error");
            return false;
        }
    } else {
        p7b = profileContent;
    }
    return true;
}

bool SignElf::isExecElf(ELFIO::elfio& reader)
{
    ELFIO::Elf64_Half eType = reader.get_type();
    if (eType == ELFIO::ET_EXEC) {
        return true;
    }
    if (eType == ELFIO::ET_DYN && reader.get_entry() > 0) {
        return true;
    }
    return false;
}

bool SignElf::WriteCodeSignBlock(SignerConfig& signerConfig, const std::map<std::string, std::string>& signParams,
                                 long secOffset)
{
    std::string inputFile = signParams.at(ParamConstants::PARAM_BASIC_INPUT_FILE);
    std::string outputFile = signParams.at(ParamConstants::PARAM_BASIC_OUTPUT_FILE);

    ELFIO::elfio reader;
    if (!reader.load(inputFile)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to load input ELF file");
        return false;
    }

    // Create .codesign section with 4K size
    ELFIO::section* codesignSec = reader.sections.add(".codesign");
    if (!codesignSec) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to create .codesign section");
        return false;
    }

    codesignSec->set_type(ELFIO::SHT_PROGBITS);
    codesignSec->set_flags(ELFIO::SHF_ALLOC);
    codesignSec->set_addr_align(1);

    // Allocate 4K of data
    const size_t codesignSize = 4096;
    std::vector<char> codesignData(codesignSize, 0);

    codesignSec->set_data(codesignData.data(), codesignData.size());

    // Save the modified ELF file
    if (!reader.save(outputFile)) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to save output ELF file");
        return false;
    }

    // Get the offset of the .codesign section
    ELFIO::Elf64_Off sectionOffset = codesignSec->get_offset();
    SIGNATURE_TOOLS_LOGI("[SignElf] .codesign section offset: %ld", sectionOffset);
    secOffset = sectionOffset;
    return true;
}

bool SignElf::WriteSection(ELFIO::elfio& reader, const std::string& content, const std::string& secName)
{
    ELFIO::section* sec = reader.sections[secName];
    if (sec) {
        SIGNATURE_TOOLS_LOGE("[SignElf] %s section already exists", secName.c_str());
        return false;
    }
    sec = reader.sections.add(secName);
    if (!sec) {
        SIGNATURE_TOOLS_LOGE("[SignElf] Failed to create %s section", secName.c_str());
        return false;
    }
    sec->set_type(ELFIO::SHT_PROGBITS);
    sec->set_flags(ELFIO::SHF_ALLOC);
    sec->set_addr_align(1);
    sec->set_data(content);
    return true;
}

bool SignElf::WriteSecDataToFile(ELFIO::elfio& reader, SignerConfig& signerConfig,
                                 std::map<std::string, std::string>& signParams)
{
    // check elf bin or so
    if (!isExecElf(reader)) {
        return true;
    }
    std::string p7b;
    if (!loadProfileAndSign(signerConfig, signParams, p7b)) {
        return false;
    }
    if (!WriteSection(reader, p7b, profileSec)) {
        return false;
    }
    PrintMsg("add profile to ELF file success");
    std::string moduleContent;
    if (!loadModule(signParams, moduleContent)) {
        return false;
    }
    if (!WriteSection(reader, moduleContent, permissionSec)) {
        return false;
    }
    PrintMsg("add permission to ELF file success");
    return true;
}
} // namespace SignatureTools
} // namespace OHOS