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
#ifndef SIGNATURETOOLS_FSVERITY_DESCRIPTOR_H
#define SIGNATURETOOLS_FSVERITY_DESCRIPTOR_H

#include <stdint.h>
#include <vector>
#include <memory>

#include "byte_buffer.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
class FsVerityDescriptor {
public:
    static const uint8_t VERSION = 1;
    static const int PAGE_SIZE_4K = 4096;
    static const int FLAG_STORE_MERKLE_TREE_OFFSET = 0x1;
    static const int FS_VERITY_DESCRIPTOR_TYPE = 0x1;
    static const uint8_t CODE_SIGN_VERSION = 0x1;
    static const int DESCRIPTOR_SIZE = 256;
    static const int ROOT_HASH_FILED_SIZE = 64;
    static const int SALT_SIZE = 32;
    static const int RESERVED_SIZE_AFTER_FLAGS = 4;
    static const int RESERVED_SIZE_AFTER_TREE_OFFSET = 127;

    class Builder;
    FsVerityDescriptor();
    long GetFileSize()
    {
        return fileSize;
    }

    long GetMerkleTreeOffset()
    {
        return merkleTreeOffset;
    }

    int GetSignSize()
    {
        return signSize;
    }

    static FsVerityDescriptor FromByteArray(std::vector<int8_t>& bytes);
    void ToByteArray(std::vector<int8_t> &ret);
    void GetByteForGenerateDigest(std::vector<int8_t>& ret);
    
    FsVerityDescriptor(Builder& builder)
    {
        this->version = builder.version;
        this->fileSize = builder.fileSize;
        this->hashAlgorithm = builder.hashAlgorithm;
        this->log2BlockSize = builder.log2BlockSize;
        this->saltSize = builder.saltSize;
        this->signSize = builder.signSize;
        this->salt = builder.salt;
        this->rawRootHash = builder.rawRootHash;
        this->flags = builder.flags;
        this->merkleTreeOffset = builder.merkleTreeOffset;
        this->csVersion = builder.csVersion;
    }

    void WriteBytesWithSize(ByteBuffer* buffer, std::vector<int8_t>& src, int size);

public:
class Builder {
    friend class FsVerityDescriptor;
public:
    Builder& SetVersion(uint8_t version)
    {
        this->version = version;
        return *this;
    }

    Builder& SetFileSize(long fileSize)
    {
        this->fileSize = fileSize;
        return *this;
    }

    Builder& SetHashAlgorithm(uint8_t hashAlgorithm)
    {
        this->hashAlgorithm = hashAlgorithm;
        return *this;
    }

    Builder& SetLog2BlockSize(uint8_t log2BlockSize)
    {
        this->log2BlockSize = log2BlockSize;
        return *this;
    }

    Builder& SetSignSize(int signSize)
    {
        this->signSize = signSize;
        return *this;
    }

    Builder& SetSaltSize(uint8_t saltSize)
    {
        this->saltSize = saltSize;
        return *this;
    }

    Builder& SetSalt(const std::vector<int8_t>& salt)
    {
        this->salt = salt;
        return *this;
    }

    Builder& SetRawRootHash(const std::vector<int8_t>& rawRootHash)
    {
        this->rawRootHash = rawRootHash;
        return *this;
    }

    Builder& SetFlags(int flags)
    {
        this->flags = flags;
        return *this;
    }

    Builder& SetMerkleTreeOffset(long merkleTreeOffset)
    {
        this->merkleTreeOffset = merkleTreeOffset;
        return *this;
    }

    Builder& SetCsVersion(uint8_t csVersion)
    {
        this->csVersion = csVersion;
        return *this;
    }

    FsVerityDescriptor Build()
    {
        return FsVerityDescriptor(*this);
    }

private:
    uint8_t version = VERSION;
    long fileSize;
    uint8_t hashAlgorithm;
    uint8_t log2BlockSize;
    uint8_t saltSize;
    int signSize;
    std::vector<int8_t> salt;
    std::vector<int8_t> rawRootHash;
    std::int32_t flags;
    std::int64_t merkleTreeOffset;
    uint8_t csVersion;
};

private:
    uint8_t version;
    long fileSize;
    uint8_t hashAlgorithm;
    uint8_t log2BlockSize;
    uint8_t saltSize;
    int signSize;
    std::vector<int8_t> salt;
    std::vector<int8_t> rawRootHash;
    int flags;
    long merkleTreeOffset;
    uint8_t csVersion;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_FSVERITY_DESCRIPTOR_H