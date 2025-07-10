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

#ifndef SIGNATRUETOOLS_HAP_UTILS_H
#define SIGNATRUETOOLS_HAP_UTILS_H

#include <set>
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

#include "content_digest_algorithm.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class HapUtils {
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
};
} // namespace SignatureTools
} // namespace OHOS
#endif
