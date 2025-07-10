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

#ifndef SIGNATRUETOOLS_SIGN_HEAD_H
#define SIGNATRUETOOLS_SIGN_HEAD_H

#include <string>
#include <vector>

namespace OHOS {
namespace SignatureTools {
class SignHead {
public:
    SignHead();

    std::vector<int8_t> GetSignHead(const int subBlockSize);
    static const int SIGN_HEAD_LEN;
    static const std::string MAGIC;
    static const std::string ELF_MAGIC;
    static const std::string VERSION;
    static const int32_t ELF_BLOCK_LEN;
    static const int32_t BIN_BLOCK_LEN;
    static std::vector<int8_t> GetSignHeadLittleEndian(const int subBlockSize, const int subBlockNum);

private:
    static const int NUM_OF_BLOCK;
    static const int RESERVE_LENGTH;
    static std::vector<int8_t> m_reserve;
};
} // namespace SignatureTools
} // namespace OHOS
#endif