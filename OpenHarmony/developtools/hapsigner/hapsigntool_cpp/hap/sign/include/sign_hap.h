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
#ifndef SIGNATRUETOOLS_SIGH_HAP_H
#define SIGNATRUETOOLS_SIGH_HAP_H
#include <limits.h>
#include <unordered_map>
#include <vector>
#include <memory>

#include "hap_utils.h"
#include "signer_config.h"
#include "hap_signer_block_utils.h"
#include "securec.h"

namespace OHOS {
namespace SignatureTools {
class SignHap {
public:
    static bool Sign(DataSource* contents[], int32_t len, SignerConfig& config,
                     std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result);
    static bool ComputeDigests(const DigestParameter& digestParam, DataSource* contents[], int32_t len,
                               const std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result);

private:
    static constexpr int INT_SIZE = 4;
    static constexpr int CONTENT_VERSION = 2;
    static constexpr int BLOCK_NUMBER = 1;
    static constexpr int CONTENT_NUBER = 3;
    static constexpr int STORED_ENTRY_SO_ALIGNMENT = 4096;
    static constexpr int BUFFER_LENGTH = 4096;
    static constexpr int BLOCK_COUNT = 4;
    static constexpr int BLOCK_MAGIC = 16;
    static constexpr int BLOCK_VERSION = 4;
    static constexpr long INIT_OFFSET_LEN = 4L;
    static constexpr int OPTIONAL_TYPE_SIZE = 4;
    static constexpr int OPTIONAL_LENGTH_SIZE = 4;
    static constexpr int OPTIONAL_OFFSET_SIZE = 4;
    static void ExtractedResult(std::vector<OptionalBlock>& optionalBlocks, ByteBuffer& result,
                                std::unordered_map<int, int>& typeAndOffsetMap);
    static bool GenerateHapSigningBlock(const std::string& hapSignatureSchemeBlock,
                                        std::vector<OptionalBlock>& optionalBlocks,
                                        int compatibleVersion, ByteBuffer& result);
    static void EncodeListOfPairsToByteArray(const DigestParameter& digestParam,
                                             const std::vector<std::pair<int32_t, ByteBuffer>>& contentDigests,
                                             ByteBuffer& result);
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGH_HAP_H
