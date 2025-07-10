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
#ifndef SIGNATURETOOLS_FSVERITY_DIGEST_H
#define SIGNATURETOOLS_FSVERITY_DIGEST_H

#include <string>
#include <vector>
#include <memory>

#include "byte_buffer.h"

namespace OHOS {
namespace SignatureTools {
class FsVerityDigest {
public:
    static void GetFsVerityDigest(int8_t algoID, std::vector<int8_t>& digest, std::vector<int8_t>& ret);

private:
    static const std::string FSVERITY_DIGEST_MAGIC;
    static const int DIGEST_HEADER_SIZE;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_FSVERITY_DIGEST_H