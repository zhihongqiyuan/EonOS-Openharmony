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

#ifndef SIGNATRUETOOLS_VERIFY_ELF_H
#define SIGNATRUETOOLS_VERIFY_ELF_H

#include <string>
#include <vector>

#include "options.h"
#include "block_data.h"
#include "signing_block.h"
#include "pkcs7_context.h"
#include "sign_block_info.h"

namespace OHOS {
namespace SignatureTools {
class VerifyElf {
public:
    static const int8_t SIGNATURE_BLOCK;
    static const int8_t PROFILE_NOSIGNED_BLOCK;
    static const int8_t PROFILE_SIGNED_BLOCK;
    static const int8_t KEY_ROTATION_BLOCK;
    static const int8_t CODESIGNING_BLOCK_TYPE;

public:
    bool Verify(Options* options);
    static bool CheckParams(Options* options);
    static bool CheckSignFile(const std::string& signedFile);
    static bool GetSignBlockData(std::vector<int8_t>& bytes, BlockData& blockData,
        const std::string fileType);
    static bool GetSignBlockInfo(const std::string& file, SignBlockInfo& signBlockInfo,
        const std::string fileType);
    static bool GetFileDigest(std::vector<int8_t>& fileBytes, const std::vector<int8_t>& signatrue,
        SignBlockInfo& signBlockInfo);
    static bool GetRawContent(const std::vector<int8_t>& contentVec, std::string& rawContent);
    static bool VerifyP7b(std::unordered_map<int8_t, SigningBlock>& signBlockMap, Options* options,
        Pkcs7Context& pkcs7Context, std::vector<int8_t>& profileVec, std::string& profileJson);

private:
    bool VerifyElfFile(const std::string& elfFile, std::vector<int8_t>& profileVec,
        Options* options, Pkcs7Context& pkcs7Context);
    static bool CheckMagicAndVersion(std::vector<int8_t>& bytes, int64_t& offset,
        const std::string fileType);
    static void GetElfSignBlock(std::vector<int8_t>& bytes, BlockData& blockData,
        std::unordered_map<int8_t, SigningBlock>& signBlockMap);
    static void GetBinSignBlock(std::vector<int8_t>& bytes, BlockData& blockData,
        std::unordered_map<int8_t, SigningBlock>& signBlockMap);
    static bool GenerateFileDigest(std::vector<int8_t>& fileBytes, SignBlockInfo& signBlockInfo);
};
} // namespace SignatureTools
} // namespace OHOS
#endif