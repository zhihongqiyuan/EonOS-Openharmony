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
#ifndef SIGNATRUETOOLS_ELF_SIGN_BLOCK_H
#define SIGNATRUETOOLS_ELF_SIGN_BLOCK_H

#include <vector>
#include <string>

#include "fs_verity_descriptor_with_sign.h"

namespace OHOS {
namespace SignatureTools {

class ElfSignBlock {
public:
    static constexpr int32_t PAGE_SIZE_4K = 4096;
    static constexpr int32_t MERKLE_TREE_INLINED = 0x2;

public:
    ElfSignBlock();
    ElfSignBlock(int32_t paddingSize, const std::vector<int8_t> &merkleTreeData,
                 const FsVerityDescriptorWithSign &descriptorWithSign);
    int32_t Size();
    std::vector<int8_t>& GetMerkleTreeWithPadding();
    int64_t GetDataSize();
    int64_t GetTreeOffset();
    std::vector<int8_t>& GetSignature();
    void ToByteArray(std::vector<int8_t>& ret);
    static bool FromByteArray(std::vector<int8_t>& bytes, ElfSignBlock& elfSignBlock);
    static int32_t ComputeMerkleTreePaddingLength(int64_t signBlockOffset);

private:
    int32_t type = MERKLE_TREE_INLINED;
    int32_t treeLength;
    std::vector<int8_t> merkleTreeWithPadding;
    FsVerityDescriptorWithSign descriptorWithSign;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ELF_SIGN_BLOCK_H