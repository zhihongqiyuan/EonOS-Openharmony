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

#ifndef SIGNATRUETOOLS_SIGH_ELF_H
#define SIGNATRUETOOLS_SIGH_ELF_H

#include <list>
#include <string>
#include <vector>
#include <elfio.hpp>

#include "signer_config.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class SignElf {
public:
    static const char CODESIGN_BLOCK_TYPE = 3;
    static bool Sign(SignerConfig& signerConfig, std::map<std::string, std::string> &signParams);

private:
    static int blockNum;
    static constexpr int FILE_PATH_LENGTH = 256;
    static constexpr int PAGE_SIZE = 4096;
    static constexpr int FILE_BUFFER_BLOCK = 16384;
    static const std::string profileSec;
    static const std::string permissionSec;

    static bool loadModule(std::map<std::string, std::string>& signParams, std::string& moduleContent);
    static bool loadProfileAndSign(SignerConfig& signerConfig, std::map<std::string, std::string>& signParams,
                            std::string& p7b);
    static bool isExecElf(ELFIO::elfio& reader);
    static bool WriteCodeSignBlock(SignerConfig& signerConfig, const std::map<std::string, std::string>& signParams,
                                    long secOffset);
    static bool WriteSection(ELFIO::elfio& reader, const std::string& content, const std::string& secName);
    static bool WriteSecDataToFile(ELFIO::elfio& reader, SignerConfig& signerConfig,
                                   std::map<std::string, std::string>& signParams);
};
} // namespace SignatureTools
} // namespace OHOS
#endif
