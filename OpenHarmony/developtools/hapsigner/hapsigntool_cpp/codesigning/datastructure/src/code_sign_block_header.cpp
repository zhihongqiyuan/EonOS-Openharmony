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
#include "code_sign_block_header.h"

namespace OHOS {
namespace SignatureTools {

CodeSignBlockHeader::CodeSignBlockHeader()
{
    magic = MAGIC_NUM;
    version = CODE_SIGNING_VERSION;
    reserved = std::vector<int8_t>(RESERVED_BYTE_ARRAY_LENGTH);
}

CodeSignBlockHeader::CodeSignBlockHeader(Builder* builder)
{
    this->magic = builder->magic;
    this->version = builder->version;
    this->blockSize = builder->blockSize;
    this->segmentNum = builder->segmentNum;
    this->flags = builder->flags;
    this->reserved = builder->reserved;
}

CodeSignBlockHeader::~CodeSignBlockHeader()
{
}

void CodeSignBlockHeader::SetSegmentNum(int num)
{
    segmentNum = num;
}

int CodeSignBlockHeader::GetSegmentNum()
{
    return segmentNum;
}

void CodeSignBlockHeader::SetBlockSize(int64_t size)
{
    blockSize = static_cast<int>(size);
}

int CodeSignBlockHeader::GetBlockSize()
{
    return blockSize;
}

void CodeSignBlockHeader::SetFlags(int flags)
{
    this->flags = flags;
}

void CodeSignBlockHeader::ToByteArray(std::vector<int8_t>& ret)
{
    ByteBuffer bf(Size());
    bf.PutInt64(magic);
    bf.PutInt32(version);
    bf.PutInt32(blockSize);
    bf.PutInt32(segmentNum);
    bf.PutInt32(flags);
    bf.PutData(reinterpret_cast<const char*>(reserved.data()), reserved.size());
    ret = std::vector<int8_t>(bf.GetBufferPtr(), bf.GetBufferPtr() + bf.GetPosition());
}

CodeSignBlockHeader* CodeSignBlockHeader::FromByteArray(const std::vector<int8_t>& bytes)
{
    if (bytes.size() != Size()) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "The size of code signature block is incorrect.");
        return nullptr;
    }
    ByteBuffer bf(bytes.size());
    bf.PutData(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    bf.Flip();
    int64_t inMagic;
    if (!bf.GetInt64(inMagic) || inMagic != MAGIC_NUM) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The magic number in the code signature block header is incorrect.");
        return nullptr;
    }
    int inVersion;
    bf.GetInt32(inVersion);
    if (inVersion != CODE_SIGNING_VERSION) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The code signing version in the code signature block header is incorrect.");
        return nullptr;
    }
    int inBlockSize;
    bf.GetInt32(inBlockSize);
    int inSegmentNum;
    bf.GetInt32(inSegmentNum);
    if (inSegmentNum != SEGMENT_NUM) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The segment number in the code signature block header is incorrect.");
        return nullptr;
    }
    int inFlags;
    bf.GetInt32(inFlags);
    if (inFlags < 0 || inFlags >(FLAG_MERKLE_TREE_INLINED + FLAG_NATIVE_LIB_INCLUDED)) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "The flag in the code signature block header is incorrect.");
        return nullptr;
    }
    std::vector<int8_t> inReserved(RESERVED_BYTE_ARRAY_LENGTH);
    bf.GetByte(inReserved.data(), RESERVED_BYTE_ARRAY_LENGTH);
    CodeSignBlockHeader::Builder* tempVar = new(std::nothrow) CodeSignBlockHeader::Builder();
    if (tempVar == nullptr) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "create  CodeSignBlockHeader::Builder failed");
        return nullptr;
    }
    CodeSignBlockHeader* codeSignBlockHeader = tempVar->SetMagic(inMagic)->SetVersion(inVersion)->
        SetBlockSize(inBlockSize)->SetSegmentNum(inSegmentNum)->
        SetFlags(inFlags)->SetReserved(inReserved)->Build();
    delete tempVar;
    return codeSignBlockHeader;
}

int CodeSignBlockHeader::Size()
{
    return MAGIC_BYTE_ARRAY_LENGTH + MAGIC_BYTE_LENGTH * MAGIC_BYTE_LENGTH + RESERVED_BYTE_ARRAY_LENGTH;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetMagic(int64_t magic)
{
    this->magic = magic;
    return this;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetVersion(int version)
{
    this->version = version;
    return this;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetBlockSize(int blockSize)
{
    this->blockSize = blockSize;
    return this;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetSegmentNum(int segmentNum)
{
    this->segmentNum = segmentNum;
    return this;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetFlags(int flags)
{
    this->flags = flags;
    return this;
}

CodeSignBlockHeader::Builder* CodeSignBlockHeader::Builder::SetReserved(const std::vector<int8_t>& reserved)
{
    this->reserved = reserved;
    return this;
}

CodeSignBlockHeader* CodeSignBlockHeader::Builder::Build()
{
    return new CodeSignBlockHeader(this);
}

CodeSignBlockHeader::Builder::~Builder()
{
}

}
}