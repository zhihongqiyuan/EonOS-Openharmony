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

#ifndef SIGNATRUETOOLS_BYTEARRAYTUTILS_EXCEPTION_H
#define SIGNATRUETOOLS_BYTEARRAYTUTILS_EXCEPTION_H

#include <string>
#include <algorithm>

#include "memory.h"

namespace OHOS {
namespace SignatureTools {
class ByteArrayUtils {
public:
    static int InsertIntToByteArray(std::vector<int8_t>& desByte, int index, int num);
    static int InsertShortToByteArray(std::vector<int8_t>& desByte, size_t desByteLen, int index, short num);
    static int InsertByteToByteArray(std::vector<int8_t>& des, int start, std::vector<int8_t> src, int srcLen);
    static int InsertCharToByteArray(std::vector<int8_t>& des, int start, std::string src);

private:
    static constexpr int BIT_SIZE = 8;
    static constexpr int DOUBLE_BIT_SIZE = 16;
    static constexpr int TRIPLE_BIT_SIZE = 24;
    static constexpr int HALF_INTEGER_BYTES = 2;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_BYTEARRAYTUTILS_EXCEPTION_H