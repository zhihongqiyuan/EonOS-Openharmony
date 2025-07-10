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
#include "code_sign_block.h"
#include "merkle_tree_extension.h"

namespace OHOS {
namespace SignatureTools {

CodeSignBlock::CodeSignBlock()
{
}

CodeSignBlock::~CodeSignBlock()
{
}

void CodeSignBlock::AddOneMerkleTree(const std::string& key, const std::vector<int8_t>& merkleTree)
{
    if (key.empty()) {
        return;
    }
    if (merkleTreeMap.find(key) == merkleTreeMap.end()) {
        if (merkleTree.empty()) {
            merkleTreeMap.insert(std::make_pair(key, std::vector<int8_t>(0)));
        } else {
            merkleTreeMap.insert(std::make_pair(key, merkleTree));
        }
    }
}

std::vector<int8_t> CodeSignBlock::GetOneMerkleTreeByFileName(const std::string& key)
{
    if (key.empty()) {
        return std::vector<int8_t>();
    }
    return merkleTreeMap[key];
}

void CodeSignBlock::SetCodeSignBlockFlag()
{
    int flags = CodeSignBlockHeader::FLAG_MERKLE_TREE_INLINED;
    if (nativeLibInfoSegment.GetSectionNum() != 0) {
        flags |= CodeSignBlockHeader::FLAG_NATIVE_LIB_INCLUDED;
    }
    codeSignBlockHeader.SetFlags(flags);
}

void CodeSignBlock::SetSegmentNum()
{
    codeSignBlockHeader.SetSegmentNum(static_cast<int>(segmentHeaderList.size()));
}

void CodeSignBlock::AddToSegmentList(const SegmentHeader& sh)
{
    segmentHeaderList.push_back(sh);
}

std::vector<SegmentHeader>& CodeSignBlock::GetSegmentHeaderList()
{
    return segmentHeaderList;
}

void CodeSignBlock::SetSegmentHeaders()
{
    // fs-verity info segment
    SegmentHeader tempVar(SegmentHeader::CSB_FSVERITY_INFO_SEG, FsVerityInfoSegment::FS_VERITY_INFO_SEGMENT_SIZE);
    segmentHeaderList.push_back(tempVar);
    // hap info segment
    SegmentHeader tempVar2(SegmentHeader::CSB_HAP_META_SEG, hapInfoSegment.GetSize());
    segmentHeaderList.push_back(tempVar2);
    // native lib info segment
    SegmentHeader tempVar3(SegmentHeader::CSB_NATIVE_LIB_INFO_SEG, nativeLibInfoSegment.Size());
    segmentHeaderList.push_back(tempVar3);
}

CodeSignBlockHeader& CodeSignBlock::GetCodeSignBlockHeader()
{
    return codeSignBlockHeader;
}

void CodeSignBlock::SetCodeSignBlockHeader(const CodeSignBlockHeader& csbHeader)
{
    codeSignBlockHeader = csbHeader;
}

void CodeSignBlock::SetFsVerityInfoSegment(const FsVerityInfoSegment& fsVeritySeg)
{
    fsVerityInfoSegment = fsVeritySeg;
}

HapInfoSegment& CodeSignBlock::GetHapInfoSegment()
{
    return hapInfoSegment;
}

void CodeSignBlock::SetHapInfoSegment(const HapInfoSegment& hapSeg)
{
    hapInfoSegment = hapSeg;
}

NativeLibInfoSegment& CodeSignBlock::GetSoInfoSegment()
{
    return nativeLibInfoSegment;
}

void CodeSignBlock::SetSoInfoSegment(const NativeLibInfoSegment& soSeg)
{
    nativeLibInfoSegment = soSeg;
}

void CodeSignBlock::ToByteArray(std::vector<int8_t>& ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>
        (ByteBuffer(codeSignBlockHeader.GetBlockSize()));
    std::vector<int8_t> codeSignBlockHeaderVec;
    codeSignBlockHeader.ToByteArray(codeSignBlockHeaderVec);
    bf->PutData(codeSignBlockHeaderVec.data(), codeSignBlockHeaderVec.size());
    for (auto &sh : segmentHeaderList) {
        std::vector<int8_t> shVec;
        sh.ToByteArray(shVec);
        bf->PutData(shVec.data(), shVec.size());
    }
    bf->PutData(zeroPadding.data(), zeroPadding.size());
    // Hap merkle tree
    bf->PutData(merkleTreeMap["Hap"].data(), merkleTreeMap["Hap"].size());
    std::vector<int8_t> fsVerityInfoSegmentVec;
    fsVerityInfoSegment.ToByteArray(fsVerityInfoSegmentVec);
    bf->PutData(fsVerityInfoSegmentVec.data(), fsVerityInfoSegmentVec.size());
    std::vector<int8_t> hapInfoSegmentVec;
    hapInfoSegment.ToByteArray(hapInfoSegmentVec);
    bf->PutData(hapInfoSegmentVec.data(), hapInfoSegmentVec.size());
    std::vector<int8_t> nativeLibInfoSegmentVec;
    nativeLibInfoSegment.ToByteArray(nativeLibInfoSegmentVec);
    bf->PutData(nativeLibInfoSegmentVec.data(), nativeLibInfoSegmentVec.size());
    
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf->GetPosition());
}

void CodeSignBlock::ComputeSegmentOffset()
{
    // 1) the first segment is placed after merkle tree
    int segmentOffset = CodeSignBlockHeader::Size()
        + segmentHeaderList.size() * SegmentHeader::SEGMENT_HEADER_LENGTH
        + zeroPadding.size() + GetOneMerkleTreeByFileName("Hap").size();
    for (std::vector<SegmentHeader>::size_type i = 0; i < segmentHeaderList.size(); i++) {
        segmentHeaderList[i].SetSegmentOffset(static_cast<int32_t>(segmentOffset));
        segmentOffset += segmentHeaderList[i].GetSegmentSize();
    }
}

int64_t CodeSignBlock::ComputeMerkleTreeOffset(int64_t codeSignBlockOffset)
{
    int64_t sizeWithoutMerkleTree = CodeSignBlockHeader::Size()
        + SEGMENT_HEADER_COUNT * SegmentHeader::SEGMENT_HEADER_LENGTH;
    // add code sign block offset while computing align position for merkle tree
    int64_t residual = (codeSignBlockOffset + sizeWithoutMerkleTree) % PAGE_SIZE_4K;
    if (residual == 0) {
        zeroPadding = std::vector<int8_t>(0);
    } else {
        zeroPadding = std::vector<int8_t>(static_cast<int>(PAGE_SIZE_4K - residual));
    }
    return codeSignBlockOffset + sizeWithoutMerkleTree + zeroPadding.size();
}

void CodeSignBlock::GenerateCodeSignBlockByte(int64_t fsvTreeOffset, std::vector<int8_t>& ret)
{
    // 1) compute overall block size without merkle tree
    int64_t csbSize = CodeSignBlockHeader::Size()
        + static_cast<int64_t>(segmentHeaderList.size()) * SegmentHeader::SEGMENT_HEADER_LENGTH
        + zeroPadding.size()
        + GetOneMerkleTreeByFileName("Hap").size()
        + fsVerityInfoSegment.Size()
        + hapInfoSegment.GetSize()
        + nativeLibInfoSegment.Size();
    Extension* ext = hapInfoSegment.GetSignInfo().GetExtensionByType(MerkleTreeExtension::MERKLE_TREE_INLINED);
    if (ext != nullptr) {
        MerkleTreeExtension* merkleTreeExtension = static_cast<MerkleTreeExtension*>(ext);
        merkleTreeExtension->SetMerkleTreeOffset(fsvTreeOffset);
    }
    codeSignBlockHeader.SetBlockSize(csbSize);
    // 2) generate byte array of complete code sign block
    ToByteArray(ret);
}

}
}