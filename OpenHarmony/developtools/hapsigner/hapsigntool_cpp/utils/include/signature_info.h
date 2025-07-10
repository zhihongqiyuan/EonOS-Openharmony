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
#ifndef SIGNERTOOLS_SIGNATURE_INFO_H
#define SIGNERTOOLS_SIGNATURE_INFO_H

#include <vector>
#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
struct OptionalBlock {
    int32_t optionalType = 0;
    ByteBuffer optionalBlockValue;
};
struct SignatureInfo {
    ByteBuffer hapSignatureBlock;
    int64_t hapSigningBlockOffset;
    int64_t hapCentralDirOffset;
    int64_t hapEocdOffset;
    ByteBuffer hapEocd;
    std::vector<OptionalBlock> optionBlocks;
    int32_t version = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // HAP_SIGNATURE_INFO_H
