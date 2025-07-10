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
#include "sign_info.h"

namespace OHOS {
namespace SignatureTools {

SignInfo::SignInfo()
{
    saltSize = 0;
    sigSize = 0;
    flags = 0;
    dataSize = 0;
    salt = std::vector<int8_t>();
    extensionNum = 0;
    extensionOffset = 0;
    signature = std::vector<int8_t>();
    zeroPadding = std::vector<int8_t>();
}

SignInfo::SignInfo(int32_t saltSize,
                   int32_t flags,
                   int64_t dataSize,
                   const std::vector<int8_t>& salt,
                   const std::vector<int8_t>& sig)
{
    this->saltSize = saltSize;
    this->flags = flags;
    this->dataSize = dataSize;
    if (salt.empty()) {
        this->salt = std::vector<int8_t>(SALT_BUFFER_LENGTH, 0);
    } else {
        this->salt = salt;
    }
    signature = sig;
    sigSize = sig.empty() ? 0 : sig.size();
    // align for extension after signature
    zeroPadding = std::vector<int8_t>((SignInfo::SIGNATURE_ALIGNMENT
                                            - (sigSize % SignInfo::SIGNATURE_ALIGNMENT))
                                            % SignInfo::SIGNATURE_ALIGNMENT, 0);
    extensionNum = 0;
    extensionOffset = 0;
}

SignInfo::SignInfo(int32_t saltSize,
                   int32_t sigSize,
                   int32_t flags,
                   int64_t dataSize,
                   const std::vector<int8_t>& salt,
                   int32_t extensionNum,
                   int32_t extensionOffset,
                   const std::vector<int8_t>& signature,
                   const std::vector<int8_t>& zeroPadding,
                   const std::vector<MerkleTreeExtension*>& extensionList)
{
    this->saltSize = saltSize;
    this->sigSize = sigSize;
    this->flags = flags;
    this->dataSize = dataSize;
    this->salt = salt;
    this->extensionNum = extensionNum;
    this->extensionOffset = extensionOffset;
    this->signature = signature;
    this->zeroPadding = zeroPadding;
    this->extensionList = extensionList;
}

SignInfo::SignInfo(const SignInfo& other)
{
    this->saltSize = other.saltSize;
    this->sigSize = other.sigSize;
    this->flags = other.flags;
    this->dataSize = other.dataSize;
    this->salt = other.salt;
    this->extensionNum = other.extensionNum;
    this->extensionOffset = other.extensionOffset;
    this->signature = other.signature;
    this->zeroPadding = other.zeroPadding;
    for (MerkleTreeExtension* ext : other.extensionList) {
        MerkleTreeExtension* extTmp = new MerkleTreeExtension(*ext);
        this->extensionList.push_back(extTmp);
    }
}

SignInfo& SignInfo::operator=(const SignInfo& other)
{
    if (this == &other) {
        return *this;
    }
    this->saltSize = other.saltSize;
    this->sigSize = other.sigSize;
    this->flags = other.flags;
    this->dataSize = other.dataSize;
    this->salt = other.salt;
    this->extensionNum = other.extensionNum;
    this->extensionOffset = other.extensionOffset;
    this->signature = other.signature;
    this->zeroPadding = other.zeroPadding;
    for (MerkleTreeExtension* ext : other.extensionList) {
        MerkleTreeExtension* extTmp = new MerkleTreeExtension(*ext);
        this->extensionList.push_back(extTmp);
    }
    return *this;
}

SignInfo::~SignInfo()
{
    for (Extension* ext : extensionList) {
        if (ext) {
            delete ext;
            ext = nullptr;
        }
    }
}

int32_t SignInfo::GetSize()
{
    int blockSize = SignInfo::SIGN_INFO_SIZE_WITHOUT_SIGNATURE + signature.size() + zeroPadding.size();
    for (Extension* ext : extensionList) {
        blockSize += ext->GetSize();
    }
    return blockSize;
}

void SignInfo::AddExtension(MerkleTreeExtension* extension)
{
    extensionOffset = GetSize();
    extensionList.push_back(extension);
    extensionNum = extensionList.size();
}

Extension* SignInfo::GetExtensionByType(int32_t type)
{
    for (Extension* ext : extensionList) {
        if (ext->IsType(type)) {
            return ext;
        }
    }
    return nullptr;
}

int32_t SignInfo::GetExtensionNum()
{
    return extensionNum;
}

std::vector<int8_t> SignInfo::GetSignature()
{
    return signature;
}

int64_t SignInfo::GetDataSize()
{
    return dataSize;
}

void SignInfo::ToByteArray(std::vector<int8_t> &ret)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(GetSize()));
    std::vector<int8_t> empt(GetSize());
    bf->PutData(empt.data(), empt.size());
    bf->Clear();
    bf->PutInt32(saltSize);
    bf->PutInt32(sigSize);
    bf->PutInt32(flags);
    bf->PutInt64(dataSize);
    bf->PutData(salt.data(), salt.size());
    bf->PutInt32(extensionNum);
    bf->PutInt32(extensionOffset);
    bf->PutData(signature.data(), signature.size());
    bf->PutData(zeroPadding.data(), zeroPadding.size());
    // put extension
    for (Extension* ext : extensionList) {
        std::vector<int8_t> ret;
        ext->ToByteArray(ret);
        bf->PutData(ret.data(), ret.size());
    }
    bf->Flip();
    ret = std::vector<int8_t>(bf->GetBufferPtr(), bf->GetBufferPtr() + bf.get()->GetCapacity());
    return;
}

std::vector<MerkleTreeExtension*> SignInfo::ParseMerkleTreeExtension(ByteBuffer* bf, int32_t inExtensionNum)
{
    std::vector<MerkleTreeExtension*> inExtensionList;
    if (inExtensionNum == 1) {
        // parse merkle tree extension
        int32_t extensionType = 0;
        bf->GetInt32(extensionType);
        if (extensionType != MerkleTreeExtension::MERKLE_TREE_INLINED) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                                "The extension type of SignInfo is incorrect.");
            return inExtensionList;
        }
        int32_t extensionSize = 0;
        bf->GetInt32(extensionSize);
        if (extensionSize != MerkleTreeExtension::MERKLE_TREE_EXTENSION_DATA_SIZE) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR,
                                "The extension size of SignInfo is incorrect.");
            return inExtensionList;
        }
        std::vector<int8_t> merkleTreeExtension(MerkleTreeExtension::MERKLE_TREE_EXTENSION_DATA_SIZE, 0);
        bf->GetByte(merkleTreeExtension.data(), merkleTreeExtension.size());
        MerkleTreeExtension* pMerkleTreeExtension = MerkleTreeExtension::FromByteArray(merkleTreeExtension);
        if (pMerkleTreeExtension) {
            inExtensionList.push_back(pMerkleTreeExtension);
        }
    }
    return inExtensionList;
}

SignInfo SignInfo::FromByteArray(std::vector<int8_t> bytes)
{
    std::unique_ptr<ByteBuffer> bf = std::make_unique<ByteBuffer>(ByteBuffer(bytes.size()));
    bf->PutData(bytes.data(), bytes.size());
    bf->Flip();
    int32_t inSaltSize = 0;
    bool flag = bf->GetInt32(inSaltSize);
    int32_t inSigSize = 0;
    bool ret = bf->GetInt32(inSigSize);
    if (!flag || !ret || inSaltSize < 0 || inSigSize < 0) {
        SIGNATURE_TOOLS_LOGE("Invalid saltSize or sigSize of SignInfo, saltSize: %d, sigSize: %d",
            inSaltSize, inSigSize);
        return SignInfo();
    }
    int32_t inFlags = 0;
    flag = bf->GetInt32(inFlags);
    if (!flag || (inFlags != 0 && inFlags != SignInfo::FLAG_MERKLE_TREE_INCLUDED)) {
        SIGNATURE_TOOLS_LOGE("Invalid flags of SignInfo: %d", inFlags);
        return SignInfo();
    }
    int64_t inDataSize = 0;
    flag = bf->GetInt64(inDataSize);
    if (!flag || (inDataSize < 0)) {
        SIGNATURE_TOOLS_LOGE("Invalid dataSize of SignInfo");
        return SignInfo();
    }
    std::vector<int8_t> inSalt(SignInfo::SALT_BUFFER_LENGTH, 0);
    bf->GetByte(inSalt.data(), SignInfo::SALT_BUFFER_LENGTH);
    int32_t inExtensionNum = 0;
    flag = bf->GetInt32(inExtensionNum);
    if (!flag || inExtensionNum < 0 || inExtensionNum > SignInfo::MAX_EXTENSION_NUM) {
        SIGNATURE_TOOLS_LOGE("Invalid extensionNum of SignInfo: %d", inExtensionNum);
        return SignInfo();
    }
    int32_t inExtensionOffset = 0;
    flag = bf->GetInt32(inExtensionOffset);
    if (!flag || inExtensionOffset < 0 || inExtensionOffset % SignInfo::SIGNATURE_ALIGNMENT != 0) {
        SIGNATURE_TOOLS_LOGE("Invalid extensionOffset of SignInfo: %d", inExtensionOffset);
        return SignInfo();
    }
    std::vector<int8_t> inSignature(inSigSize, 0);
    bf->GetByte(inSignature.data(), inSigSize);
    std::vector<int8_t> inZeroPadding((SignInfo::SIGNATURE_ALIGNMENT - (inSigSize % SignInfo::SIGNATURE_ALIGNMENT))
                                      % SignInfo::SIGNATURE_ALIGNMENT, 0);
    bf->GetByte(inZeroPadding.data(), inZeroPadding.size());
    std::vector<MerkleTreeExtension*> inExtensionList = ParseMerkleTreeExtension(bf.get(), inExtensionNum);
    return SignInfo(inSaltSize, inSigSize, inFlags, inDataSize, inSalt, inExtensionNum, inExtensionOffset,
                    inSignature, inZeroPadding, inExtensionList);
}

}
}