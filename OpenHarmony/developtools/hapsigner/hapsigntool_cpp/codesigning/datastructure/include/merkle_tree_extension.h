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
#ifndef SIGNATRUETOOLS_MERKLE_TREE_EXTENSION_H
#define SIGNATRUETOOLS_MERKLE_TREE_EXTENSION_H

#include <vector>
#include <string>

#include "extension.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {

class MerkleTreeExtension : public Extension {
public:
    static constexpr int32_t MERKLE_TREE_INLINED = 0x1;
    static constexpr int32_t MERKLE_TREE_EXTENSION_DATA_SIZE = 80;
    MerkleTreeExtension();
    MerkleTreeExtension(int64_t merkleTreeSize, int64_t merkleTreeOffset, const std::vector<int8_t> rootHash);
    virtual ~MerkleTreeExtension();
    static MerkleTreeExtension* FromByteArray(std::vector<int8_t>& bytes);
    virtual int32_t GetSize();
    virtual void ToByteArray(std::vector<int8_t>& ret);
    int64_t GetMerkleTreeSize();
    int64_t GetMerkleTreeOffset();
    void SetMerkleTreeOffset(int64_t offset);

private:
    static constexpr int32_t ROOT_HASH_SIZE = 64;
    static constexpr int32_t PAGE_SIZE_4K = 4096;
    int64_t merkleTreeSize;
    int64_t merkleTreeOffset;
    std::vector<int8_t> rootHash;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_MERKLE_TREE_EXTENSION_H
