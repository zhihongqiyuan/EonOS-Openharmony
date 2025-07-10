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
#ifndef SIGNATURETOOLS_FSVERITY_GENERATOR_H
#define SIGNATURETOOLS_FSVERITY_GENERATOR_H

#include <vector>
#include <istream>
#include <memory>

#include "merkle_tree.h"
#include "merkle_tree_builder.h"
#include "fs_verity_descriptor.h"
#include "fs_verity_digest.h"

namespace OHOS {
namespace SignatureTools {
class FsVerityGenerator {
public:
    MerkleTree* GenerateMerkleTree(std::istream& inputStream, long size,
                                   const FsVerityHashAlgorithm& fsVerityHashAlgorithm);
    bool GenerateFsVerityDigest(std::istream& inputStream, long size, long fsvTreeOffset);
    std::vector<int8_t> GetFsVerityDigest()
    {
        return fsVerityDigest;
    }

    std::vector<int8_t> Getsalt()
    {
        return salt;
    }
            
    std::vector<int8_t> GetTreeBytes()
    {
        return treeBytes;
    }

    std::vector<int8_t> GetRootHash()
    {
        return rootHash;
    }

    std::vector<int8_t> GetSalt()
    {
        return salt;
    }
            
    int GetSaltSize()
    {
        return salt.empty() ? 0 : salt.size();
    }
            
    static uint8_t GetFsVerityHashAlgorithm()
    {
        return FS_VERITY_HASH_ALGORITHM.GetId();
    }
            
    static uint8_t GetLog2BlockSize()
    {
        return LOG_2_OF_FSVERITY_HASH_PAGE_SIZE;
    }

protected:
    std::vector<int8_t> salt;

private:
    static const FsVerityHashAlgorithm FS_VERITY_HASH_ALGORITHM;
    static const int8_t LOG_2_OF_FSVERITY_HASH_PAGE_SIZE;
    std::vector<int8_t> fsVerityDigest;
    std::vector<int8_t> treeBytes;
    std::vector<int8_t> rootHash;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_FSVERITY_GENERATOR_H