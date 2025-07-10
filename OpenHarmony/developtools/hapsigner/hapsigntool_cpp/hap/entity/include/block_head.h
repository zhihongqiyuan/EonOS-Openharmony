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

#ifndef SIGNATRUETOOLS_BLOCK_HEAD_H
#define SIGNATRUETOOLS_BLOCK_HEAD_H

#include <vector>
#include <string>

namespace OHOS {
namespace SignatureTools {
class BlockHead {
public:
    static const int BLOCK_LEN = 8;
    static const int ELF_BLOCK_LEN = 12;
    static const int BIT_SIZE = 8;
    static const int DOUBLE_BIT_SIZE = 16;
    static const int TRIPLE_BIT_SIZE = 24;
    static const int32_t SIGN_HEAD_LEN = 32;

public:
    static int GetBlockLen();
    static int GetElfBlockLen();
    static std::string GetBlockHead(const char type, const char tag, const short length, const int offset);
    static std::vector<int8_t> GetBlockHeadLittleEndian(const char type, const char tag,
                                                        const int length, const int offset);
};
} // namespace SignatureTools
} // namespace OHOS
#endif