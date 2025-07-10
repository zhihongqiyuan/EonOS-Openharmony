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
#ifndef SIGNATRUETOOLS_SIGN_INFO_H
#define SIGNATRUETOOLS_SIGN_INFO_H

#include <vector>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <typeinfo>

#include "signature_tools_log.h"
#include "merkle_tree_extension.h"

namespace OHOS {
namespace SignatureTools {

class SignInfo {
public:
    static constexpr int32_t FLAG_MERKLE_TREE_INCLUDED = 0x1;
    static constexpr int32_t MAX_EXTENSION_NUM = 1;
    static constexpr int32_t SIGN_INFO_SIZE_WITHOUT_SIGNATURE = 60;
    static constexpr int32_t SALT_BUFFER_LENGTH = 32;
    static constexpr int32_t SIGNATURE_ALIGNMENT = 4;
    SignInfo();
    SignInfo(int32_t saltSize, int32_t flags, int64_t dataSize,
             const std::vector<int8_t>& salt, const std::vector<int8_t>& sig);
    SignInfo(int32_t saltSize,
             int32_t sigSize,
             int32_t flags,
             int64_t dataSize,
             const std::vector<int8_t>& salt,
             int32_t extensionNum,
             int32_t extensionOffset,
             const std::vector<int8_t>& signature,
             const std::vector<int8_t>& zeroPadding,
             const std::vector<MerkleTreeExtension*>& extensionList);
    SignInfo(const SignInfo& other);
    SignInfo& operator=(const SignInfo& other);
    virtual ~SignInfo();
    static std::vector<MerkleTreeExtension*> ParseMerkleTreeExtension(ByteBuffer* bf, int32_t inExtensionNum);
    static SignInfo FromByteArray(std::vector<int8_t> bytes);
    int32_t GetSize();
    void AddExtension(MerkleTreeExtension* extension);
    Extension* GetExtensionByType(int32_t type);
    int32_t GetExtensionNum();
    std::vector<int8_t> GetSignature();
    int64_t GetDataSize();
    void ToByteArray(std::vector<int8_t> &ret);

private:
    int32_t saltSize;
    int32_t sigSize;
    int32_t flags;
    int64_t dataSize;
    std::vector<int8_t> salt;
    int32_t extensionNum;
    int32_t extensionOffset;
    std::vector<int8_t> signature;
    std::vector<int8_t> zeroPadding;
    std::vector<MerkleTreeExtension*> extensionList;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_ELF_SIGN_BLOCK_H