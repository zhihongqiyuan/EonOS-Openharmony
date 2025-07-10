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

#ifndef SIGNATRUETOOLS_SIGNING_BLOCK_H
#define SIGNATRUETOOLS_SIGNING_BLOCK_H

#include <vector>
#include <string>

#include "file_utils.h"

namespace OHOS {
namespace SignatureTools {
class SigningBlock {
public:
    SigningBlock(const int32_t type, const std::vector<int8_t> &value);
    SigningBlock(const int32_t type, const std::vector<int8_t> &value, const int64_t offset);

    int32_t GetLength();
    const std::vector<int8_t>& GetValue();
    int64_t GetOffset();

private:
    int32_t m_type = 0;
    int32_t m_length = 0;
    std::vector<int8_t> m_value;
    int64_t m_offset = 0;
};
} // namespace SignatureTools
} // namespace OHOS
#endif
