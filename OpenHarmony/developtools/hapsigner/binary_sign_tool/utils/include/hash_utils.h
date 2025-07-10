/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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

#ifndef SIGNATRUETOOLS_HASH_UTILS_H
#define SIGNATRUETOOLS_HASH_UTILS_H

#include <string>
#include <vector>

namespace OHOS {
namespace SignatureTools {
enum class HashAlgs {
    USE_NONE = 0,
    USE_MD2 = 1,
    USE_MD4 = 2,
    USE_MD5 = 3,
    USE_SHA1 = 4,
    USE_SHA224 = 5,
    USE_SHA256 = 6,
    USE_SHA384 = 7,
    USE_SHA512 = 8,
    USE_RIPEMD160 = 9,
};

class HashUtils {
public:
    HashUtils() = delete;
    static int GetHashAlgsId(const std::string& algMethod);
    static std::vector<int8_t> GetFileDigest(const std::string& inputFile, const std::string& algName);
    static std::string GetHashAlgName(int algId);
    static std::vector<int8_t> GetDigestFromBytes(const std::vector<int8_t>& fileBytes, int64_t length,
                                                        const std::string& algName);

private:
    static std::vector<int8_t> GetByteDigest(const std::string& str, int count,
                                                    const std::string& algMethod);

    static const int HASH_LEN = 4096;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_HASH_UTILS_H