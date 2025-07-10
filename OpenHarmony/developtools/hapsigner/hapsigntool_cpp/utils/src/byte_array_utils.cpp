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

#include "byte_array_utils.h"
#include "signature_tools_errno.h"
#include "securec.h"

namespace OHOS {
namespace SignatureTools {
int ByteArrayUtils::InsertIntToByteArray(std::vector<int8_t>& desByte, int index, int num)
{
    if (index + sizeof(int) > desByte.size()) {
        return RET_FAILED;
    }
    int pos = index;
    desByte[pos] = (num >> TRIPLE_BIT_SIZE) & 0xff;
    pos++;
    desByte[pos] = (num >> DOUBLE_BIT_SIZE) & 0xff;
    pos++;
    desByte[pos] = (num >> BIT_SIZE) & 0xff;
    pos++;
    desByte[pos] = num & 0xff;
    pos++;
    return pos;
}

int ByteArrayUtils::InsertShortToByteArray(std::vector<int8_t>& desByte, size_t desByteLen, int index, short num)
{
    if (index + HALF_INTEGER_BYTES > desByteLen) {
        return RET_FAILED;
    }
    int pos = index;
    desByte[pos] = (num >> BIT_SIZE) & 0xff;
    pos++;
    desByte[pos] = num & 0xff;
    pos++;
    return pos;
}

int ByteArrayUtils::InsertByteToByteArray(std::vector<int8_t>& des, int start, std::vector<int8_t> src, int srcLen)
{
    if (src.size() < srcLen) {
        return -1;
    }
    if (memcpy_s(des.data() + start, srcLen, src.data(), srcLen) != EOK) {
        return -1;
    }
    return start + srcLen;
}

int ByteArrayUtils::InsertCharToByteArray(std::vector<int8_t>& des, int start, std::string src)
{
    if (src.length() + start > des.size()) {
        return RET_FAILED;
    }
    for (int i = 0; i < src.length(); ++i) {
        des[i + start] = src[i];
    }
    return start + src.length();
}

} // namespace SignatureTools
} // namespace OHOS
