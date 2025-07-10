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
#ifndef SIGNATURETOOLS_MERKLE_TREE_H
#define SIGNATURETOOLS_MERKLE_TREE_H

#include <vector>

#include "fs_verity_hash_algorithm.h"

namespace OHOS {
namespace SignatureTools {
class MerkleTree {
public:
    MerkleTree(const std::vector<int8_t>& rootHash, const std::vector<int8_t>& tree,
               const SignatureTools::FsVerityHashAlgorithm& fsVerityHashAlgorithm)
        : rootHash(rootHash),
        tree(tree),
        fsVerityHashAlgorithm(fsVerityHashAlgorithm)
    {
    }
    std::vector<int8_t> rootHash;
    std::vector<int8_t> tree;
    SignatureTools::FsVerityHashAlgorithm fsVerityHashAlgorithm;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_MERKLE_TREE_H