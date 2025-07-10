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

#ifndef SIGNATRUETOOLS_HAP_UTILS_H
#define SIGNATRUETOOLS_HAP_UTILS_H

#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

#include "content_digest_algorithm.h"
#include "signing_block.h"
#include "zip_data_input.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class HapUtils {
public:
    class HapSignBlockInfo {
    public:
        virtual ~HapSignBlockInfo()
        {
        }
        HapSignBlockInfo(int64_t offset, int version, ByteBuffer contentByteBuffer);

        virtual int GetVersion();
        virtual ByteBuffer GetContent();
        virtual int64_t GetOffset();

    private:
        const int64_t offset;
        const int version;
        ByteBuffer const content;
    };

public:
    static constexpr int HAP_SIGNATURE_SCHEME_V1_BLOCK_ID = 0x20000000;
    static constexpr int HAP_PROOF_OF_ROTATION_BLOCK_ID = 0x20000001;
    static constexpr int HAP_PROFILE_BLOCK_ID = 0x20000002;
    static constexpr int HAP_PROPERTY_BLOCK_ID = 0x20000003;
    static constexpr int HAP_CODE_SIGN_BLOCK_ID = 0x30000001;
    static constexpr int CONTENT_DIGESTED_CHUNK_MAX_SIZE_BYTES = 1024 * 1024;
    static constexpr int CONTENT_VERSION = 2;
    static constexpr int BIT_SIZE = 8;
    static constexpr int HALF_BIT_SIZE = 4;
    static constexpr int INT_SIZE = 4;
    static constexpr int BLOCK_NUMBER = 1;
    static constexpr int HAP_SIGN_SCHEME_V2_BLOCK_VERSION = 2;
    static constexpr int HAP_SIGN_SCHEME_V3_BLOCK_VERSION = 3;
    static constexpr int64_t HAP_SIG_BLOCK_MAGIC_LO_V2 = 0x2067695320504148LL;
    static constexpr int64_t HAP_SIG_BLOCK_MAGIC_HI_V2 = 0x3234206b636f6c42LL;
    static constexpr int64_t HAP_SIG_BLOCK_MAGIC_LO_V3 = 0x676973207061683cLL;
    static constexpr int64_t HAP_SIG_BLOCK_MAGIC_HI_V3 = 0x3e6b636f6c62206eLL;
    static constexpr int HAP_SIG_BLOCK_HEADER_SIZE = 32;
    static constexpr int HAP_SIG_BLOCK_MIN_SIZE = HAP_SIG_BLOCK_HEADER_SIZE;
    static constexpr int BLOCK_SIZE = 8;

public:
    static std::string GetAppIdentifier(const std::string& profileContent);
    static std::pair<std::string, std::string> ParseAppIdentifier(const std::string& profileContent);
    static std::vector<int8_t> GetHapSigningBlockMagic(int compatibleVersion);
    static int GetHapSigningBlockVersion(int compatibleVersion);
    static bool ReadFileToByteBuffer(const std::string& file, ByteBuffer& buffer);

private:
    static const int32_t MAX_APP_ID_LEN = 32;
    static const std::string HAP_DEBUG_OWNER_ID;
    static std::set<int> HAP_SIGNATURE_OPTIONAL_BLOCK_IDS;
    static constexpr int MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3 = 8;
    static const std::vector<int8_t> HAP_SIGNING_BLOCK_MAGIC_V2;
    static const std::vector<int8_t> HAP_SIGNING_BLOCK_MAGIC_V3;
    static constexpr int8_t ZIP_FIRST_LEVEL_CHUNK_PREFIX = 0x5a;
    static const int8_t ZIP_SECOND_LEVEL_CHUNK_PREFIX = static_cast<int8_t>(0xa5);
    static const int DIGEST_PRIFIX_LENGTH = 5;
    static constexpr int BUFFER_LENGTH = 4096;
    static const std::string HEX_CHAR_ARRAY;

    class StaticConstructor {
    public:
        StaticConstructor();
    };

    static HapUtils::StaticConstructor staticConstructor;
};
} // namespace SignatureTools
} // namespace OHOS
#endif
