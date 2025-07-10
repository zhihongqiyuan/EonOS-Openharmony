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
#include "fs_verity_descriptor.h"

namespace OHOS {
namespace SignatureTools {
FsVerityDescriptor::FsVerityDescriptor()
{}

FsVerityDescriptor FsVerityDescriptor::FromByteArray(std::vector<int8_t>& bytes)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(bytes.size()));
    bf->PutData(bytes.data(), bytes.size());
    // after put, rewind is mandatory before get
    bf->Flip();
    std::unique_ptr<FsVerityDescriptor::Builder> builder =
        std::make_unique<FsVerityDescriptor::Builder>(FsVerityDescriptor::Builder());
    int8_t inFsVersion;
    bf->GetInt8(inFsVersion);
    if (FsVerityDescriptor::VERSION != inFsVersion) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The FS descriptor version of FsVerityDescriptor is incorrect");
        return builder->Build();
    }
    int8_t inFsHashAlgorithm;
    bf->GetInt8(inFsHashAlgorithm);
    int8_t inLog2BlockSize;
    bf->GetInt8(inLog2BlockSize);
    builder->SetVersion(inFsVersion).SetHashAlgorithm(inFsHashAlgorithm).SetLog2BlockSize(inLog2BlockSize);
    int8_t inSaltSize;
    bf->GetInt8(inSaltSize);
    int32_t inSignSize;
    bf->GetInt32(inSignSize);
    int64_t inDataSize;
    bf->GetInt64(inDataSize);
    char inRootHashArr[FsVerityDescriptor::ROOT_HASH_FILED_SIZE];
    bf->GetData(inRootHashArr, FsVerityDescriptor::ROOT_HASH_FILED_SIZE);
    std::vector<int8_t> inRootHash(inRootHashArr, inRootHashArr + FsVerityDescriptor::ROOT_HASH_FILED_SIZE);
    builder->SetSaltSize(inSaltSize).SetSignSize(inSignSize).SetFileSize(inDataSize).SetRawRootHash(inRootHash);
    char inSaltArr[FsVerityDescriptor::SALT_SIZE];
    bf->GetData(inSaltArr, FsVerityDescriptor::SALT_SIZE);
    std::vector<int8_t> inSalt(inSaltArr, inSaltArr + FsVerityDescriptor::SALT_SIZE);
    int32_t inFlags;
    bf->GetInt32(inFlags);
    bf->SetPosition(bf->GetPosition() + RESERVED_SIZE_AFTER_FLAGS);
    int64_t inTreeOffset;
    bf->GetInt64(inTreeOffset);
    if (inTreeOffset % PAGE_SIZE_4K != 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                            "The offset of merkle tree of FsVerityDescriptor is incorrect");
        return builder->Build();
    }
    int8_t reserve[FsVerityDescriptor::RESERVED_SIZE_AFTER_TREE_OFFSET];
    bf->GetByte(reserve, sizeof(reserve));
    int8_t inCsVersion;
    bf->GetInt8(inCsVersion);
    builder->SetSalt(inSalt).SetFlags(inFlags).SetMerkleTreeOffset(inTreeOffset).SetCsVersion(inCsVersion);
    return builder->Build();
}

void FsVerityDescriptor::ToByteArray(std::vector<int8_t> &ret)
{
    std::unique_ptr<ByteBuffer> buffer = std::make_unique<ByteBuffer>(ByteBuffer(DESCRIPTOR_SIZE));
    buffer->PutByte(VERSION);
    buffer->PutByte(hashAlgorithm);
    buffer->PutByte(log2BlockSize);
    if (saltSize > SALT_SIZE) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Salt is too long");
        ret = std::vector<int8_t>();
        return;
    }
    buffer->PutByte(saltSize);
    buffer->PutInt32(signSize);
    buffer->PutInt64(fileSize);
    WriteBytesWithSize(buffer.get(), rawRootHash, ROOT_HASH_FILED_SIZE);
    WriteBytesWithSize(buffer.get(), salt, SALT_SIZE);
    buffer->PutInt32(flags);
    std::vector<int8_t> emptyVector;
    WriteBytesWithSize(buffer.get(), emptyVector, RESERVED_SIZE_AFTER_FLAGS);
    buffer->PutInt64(merkleTreeOffset);
    WriteBytesWithSize(buffer.get(), emptyVector, RESERVED_SIZE_AFTER_TREE_OFFSET);
    buffer->PutByte(csVersion);
    buffer->Flip();
    char dataArr[DESCRIPTOR_SIZE] = { 0 };
    buffer->GetData(dataArr, DESCRIPTOR_SIZE);
    ret = std::vector<int8_t>(dataArr, dataArr + DESCRIPTOR_SIZE);
    return;
}

void FsVerityDescriptor::GetByteForGenerateDigest(std::vector<int8_t> &ret)
{
    std::unique_ptr<ByteBuffer> buffer = std::make_unique<ByteBuffer>(ByteBuffer(DESCRIPTOR_SIZE));
    buffer->PutByte(CODE_SIGN_VERSION);
    buffer->PutByte(hashAlgorithm);
    buffer->PutByte(log2BlockSize);
    if (saltSize > SALT_SIZE) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "Salt is too long");
        ret = std::vector<int8_t>();
        return;
    }
    buffer->PutByte(saltSize);
    buffer->PutInt32(0);
    buffer->PutInt64(fileSize);
    WriteBytesWithSize(buffer.get(), rawRootHash, ROOT_HASH_FILED_SIZE);
    WriteBytesWithSize(buffer.get(), salt, SALT_SIZE);
    buffer->PutInt32(flags);
    std::vector<int8_t> emptyVector;
    WriteBytesWithSize(buffer.get(), emptyVector, RESERVED_SIZE_AFTER_FLAGS);
    buffer->PutInt64(merkleTreeOffset);
    buffer->Flip();
    char dataArr[DESCRIPTOR_SIZE] = { 0 };
    buffer->GetData(dataArr, DESCRIPTOR_SIZE);
    ret = std::vector<int8_t>(dataArr, dataArr + DESCRIPTOR_SIZE);
    return;
}

void FsVerityDescriptor::WriteBytesWithSize(ByteBuffer* buffer, std::vector<int8_t>& src, int size)
{
    int pos = buffer->GetPosition();
    if (!src.empty()) {
        if (src.size() > size) {
            buffer->PutData(0, src.data(), src.size());
        } else {
            buffer->PutData(src.data(), src.size());
        }
    }
    buffer->SetPosition(pos + size);
}
} // namespace SignatureTools
} // namespace OHOS