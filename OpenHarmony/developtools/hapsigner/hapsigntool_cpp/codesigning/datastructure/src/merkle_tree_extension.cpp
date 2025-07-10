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
#include "merkle_tree_extension.h"

namespace OHOS {
namespace SignatureTools {

MerkleTreeExtension::MerkleTreeExtension()
    : Extension(MERKLE_TREE_INLINED, MERKLE_TREE_EXTENSION_DATA_SIZE)
{
    merkleTreeSize = 0;
    merkleTreeOffset = 0;
}

MerkleTreeExtension::MerkleTreeExtension(int64_t merkleTreeSize, int64_t merkleTreeOffset,
    const std::vector<int8_t> rootHash) : Extension(MERKLE_TREE_INLINED, MERKLE_TREE_EXTENSION_DATA_SIZE)
{
    this->merkleTreeSize = merkleTreeSize;
    this->merkleTreeOffset = merkleTreeOffset;
    this->rootHash = rootHash;
}

MerkleTreeExtension::~MerkleTreeExtension()
{
}

int32_t MerkleTreeExtension::GetSize()
{
    return Extension::EXTENSION_HEADER_SIZE + MERKLE_TREE_EXTENSION_DATA_SIZE;
}

int64_t MerkleTreeExtension::GetMerkleTreeSize()
{
    return merkleTreeSize;
}

int64_t MerkleTreeExtension::GetMerkleTreeOffset()
{
    return merkleTreeOffset;
}

void MerkleTreeExtension::SetMerkleTreeOffset(int64_t offset)
{
    merkleTreeOffset = offset;
}

void MerkleTreeExtension::ToByteArray(std::vector<int8_t>& ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(
        Extension::EXTENSION_HEADER_SIZE + MERKLE_TREE_EXTENSION_DATA_SIZE));
    std::vector<int8_t> extByteArr;
    Extension::ToByteArray(extByteArr);
    bf->PutData(extByteArr.data(), extByteArr.size());
    bf->PutInt64(merkleTreeSize);
    bf->PutInt64(merkleTreeOffset);
    bf->PutData(rootHash.data(), rootHash.size());
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf->GetCapacity());
}

MerkleTreeExtension* MerkleTreeExtension::FromByteArray(std::vector<int8_t>& bytes)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(bytes.size()));
    bf->PutData(bytes.data(), bytes.size());
    bf->Flip();
    int64_t inMerkleTreeSize = 0;
    bool flag = bf->GetInt64(inMerkleTreeSize);
    if (!flag || (inMerkleTreeSize % PAGE_SIZE_4K != 0)) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The merkletree size must be an integer multiple of 4096");
        return nullptr;
    }
    int64_t inMerkleTreeOffset = 0;
    flag = bf->GetInt64(inMerkleTreeOffset);
    if (!flag || (inMerkleTreeOffset % PAGE_SIZE_4K != 0)) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The merkletree offset must be an integer multiple of 4096");
        return nullptr;
    }
    std::vector<int8_t> inRootHash(ROOT_HASH_SIZE);
    bf->GetByte(inRootHash.data(), inRootHash.size());
    return new MerkleTreeExtension(inMerkleTreeSize, inMerkleTreeOffset, inRootHash);
}

}
}