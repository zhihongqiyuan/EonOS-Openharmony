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
#ifndef SIGNATURETOOLS_VERIFY_CODE_SIGNATURE_H
#define SIGNATURETOOLS_VERIFY_CODE_SIGNATURE_H

#include <vector>
#include <string>
#include <fstream>
#include <contrib/minizip/unzip.h>

#include "signature_tools_log.h"
#include "code_sign_block.h"
#include "hap_utils.h"
#include "merkle_tree_extension.h"
#include "native_lib_info_segment.h"
#include "code_signing.h"
#include "fs_verity_generator.h"
#include "cms_utils.h"

namespace OHOS {
namespace SignatureTools {
class VerifyCodeSignature {
public:
    static bool VerifyHap(std::string file, int64_t offset, int64_t length,
                          std::string fileFormat, std::string profileContent);
    static bool VerifyElf(std::string file, int64_t offset, int64_t length,
                          std::string fileFormat, std::string profileContent);
    static bool VerifyNativeLib(CodeSignBlock& csb, std::string& file, unzFile& zFile,
                                std::pair<std::string, std::string>& pairResult);
    static bool VerifyCodeSign(std::string file, std::pair<std::string,
                               std::string>& pairResult, CodeSignBlock& csb);
    static bool VerifySingleFile(std::istream& input, int64_t length, std::vector<int8_t> signature,
                                 int64_t merkleTreeOffset, std::vector<int8_t> inMerkleTreeBytes);
    static bool AreVectorsEqual(const std::vector<int8_t>& vec1, const std::vector<int8_t>& vec2);

private:
    static bool GenerateCodeSignBlock(const std::string& file, int64_t offset, int64_t length,
                                      CodeSignBlock& csb);
    static bool ParseSegmentHead(CodeSignBlock& csb, std::ifstream& signedHap,
                                 std::vector<char>& merkleTreeBytes, int32_t& fileReadOffset);
    static bool ParseMerkleTree(CodeSignBlock& csb, int32_t readOffset, std::ifstream& signedHap,
                                int64_t computedTreeOffset);
    static int64_t GetAlignmentAddr(int64_t alignment, int64_t input);
    static std::pair<std::vector<int8_t>, std::vector<int8_t>> GenerateFsVerityDigest(std::istream& inputStream,
                                                                                      int64_t size,
                                                                                      int64_t merkleTreeOffset);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_CMS_UTILS_H