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

#include "elf_sign_block.h"

namespace OHOS {
namespace SignatureTools {

ElfSignBlock::ElfSignBlock()
{
    type = MERKLE_TREE_INLINED;
}

ElfSignBlock::ElfSignBlock(int32_t paddingSize, const std::vector<int8_t> &merkleTreeData,
                           const FsVerityDescriptorWithSign &descriptorWithSign)
{
    std::vector<int8_t> inMerkleTreeData;
    if (!merkleTreeData.empty()) {
        inMerkleTreeData = merkleTreeData;
    }
    treeLength = paddingSize + inMerkleTreeData.size();
    merkleTreeWithPadding.resize(treeLength);
    std::copy(inMerkleTreeData.begin(), inMerkleTreeData.end(), merkleTreeWithPadding.begin() + paddingSize);
    this->descriptorWithSign = descriptorWithSign;
}

int32_t ElfSignBlock::Size()
{
    int tmpVariable = 2;
    return FsVerityDescriptorWithSign::INTEGER_BYTES * tmpVariable
        + merkleTreeWithPadding.size() + descriptorWithSign.Size();
}

std::vector<int8_t>& ElfSignBlock::GetMerkleTreeWithPadding()
{
    return merkleTreeWithPadding;
}

int64_t ElfSignBlock::GetDataSize()
{
    return descriptorWithSign.GetFsVerityDescriptor().GetFileSize();
}

int64_t ElfSignBlock::GetTreeOffset()
{
    return descriptorWithSign.GetFsVerityDescriptor().GetMerkleTreeOffset();
}

std::vector<int8_t>& ElfSignBlock::GetSignature()
{
    return descriptorWithSign.GetSignature();
}

void ElfSignBlock::ToByteArray(std::vector<int8_t>& ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(Size());
    bf->PutInt32(type);
    bf->PutInt32(merkleTreeWithPadding.size());
    bf->PutData(merkleTreeWithPadding.data(), merkleTreeWithPadding.size());
    std::vector<int8_t> descriptorWithSignArr;
    descriptorWithSign.ToByteArray(descriptorWithSignArr);
    bf->PutData(descriptorWithSignArr.data(), descriptorWithSignArr.size());
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf->GetLimit());
}

bool ElfSignBlock::FromByteArray(std::vector<int8_t>& bytes, ElfSignBlock& elfSignBlock)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(bytes.size());
    bf->PutData(bytes.data(), bytes.size());
    bf->Flip();
    int32_t inTreeType = 0;
    bf->GetInt32(inTreeType);
    if (MERKLE_TREE_INLINED != inTreeType) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The merkle tree type of elf signature block is incorrect");
        return false;
    }
    int32_t inTreeLength = 0;
    bf->GetInt32(inTreeLength);
    std::vector<int8_t> treeWithPadding(inTreeLength);
    bf->GetByte(treeWithPadding.data(), treeWithPadding.size());
    int32_t inFsdType = 0;
    bf->GetInt32(inFsdType);
    if (FsVerityDescriptor::FS_VERITY_DESCRIPTOR_TYPE != inFsdType) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The FS descriptor type of elf signature block is incorrect");
        return false;
    }
    int32_t inFsdLength = 0;
    int tmpVariable = 2;
    bf->GetInt32(inFsdLength);
    if (bytes.size() != FsVerityDescriptorWithSign::INTEGER_BYTES * tmpVariable + inTreeLength +
        FsVerityDescriptorWithSign::INTEGER_BYTES * tmpVariable + inFsdLength) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The signature length of the elf signature block is incorrect");
        return false;
    }
    std::vector<int8_t> fsdArray(FsVerityDescriptor::DESCRIPTOR_SIZE);
    bf->GetByte(fsdArray.data(), fsdArray.size());
    FsVerityDescriptor fsd = FsVerityDescriptor::FromByteArray(fsdArray);
    if (inFsdLength != fsd.GetSignSize() + FsVerityDescriptor::DESCRIPTOR_SIZE) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The signed size of the elf signature block is incorrect");
        return false;
    }
    std::vector<int8_t> inSignature(inFsdLength - FsVerityDescriptor::DESCRIPTOR_SIZE);
    bf->GetByte(inSignature.data(), inSignature.size());
    FsVerityDescriptorWithSign fsVerityDescriptorWithSign(inFsdType, inFsdLength, fsd, inSignature);
    elfSignBlock.type = inTreeType;
    elfSignBlock.treeLength = inTreeLength;
    elfSignBlock.merkleTreeWithPadding = treeWithPadding;
    elfSignBlock.descriptorWithSign = fsVerityDescriptorWithSign;
    return true;
}

int32_t ElfSignBlock::ComputeMerkleTreePaddingLength(int64_t signBlockOffset)
{
    int tmpVariable = 2;
    return (int32_t)(PAGE_SIZE_4K - (signBlockOffset + FsVerityDescriptorWithSign::INTEGER_BYTES * tmpVariable)
                     % PAGE_SIZE_4K) % PAGE_SIZE_4K;
}

}
}