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
#include "fs_verity_hash_algorithm.h"

namespace OHOS {
namespace SignatureTools {
const FsVerityHashAlgorithm FsVerityHashAlgorithm::SHA256((char)1, "SHA-256", 256 / 8);
const FsVerityHashAlgorithm FsVerityHashAlgorithm::SHA512((char)2, "SHA-512", 512 / 8);

char FsVerityHashAlgorithm::GetId() const
{
    return id;
}

const std::string& FsVerityHashAlgorithm::GetHashAlgorithm() const
{
    return hashAlgorithm;
}

int FsVerityHashAlgorithm::GetOutputByteSize() const
{
    return outputByteSize;
}
} // namespace SignatureTools
} // namespace OHOS