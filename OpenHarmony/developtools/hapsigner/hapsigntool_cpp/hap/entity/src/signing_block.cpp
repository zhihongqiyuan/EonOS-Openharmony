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

#include "signing_block.h"

namespace OHOS {
namespace SignatureTools {

SigningBlock::SigningBlock(const int32_t type, const std::vector<int8_t> &value)
{
    m_type = type;
    m_length = value.size();
    m_value = std::move(value);
}

SigningBlock::SigningBlock(const int32_t type, const std::vector<int8_t> &value, const int64_t offset)
{
    m_type = type;
    m_length = value.size();
    m_value = std::move(value);
    m_offset = offset;
}

int32_t SigningBlock::GetLength()
{
    return m_length;
}

const std::vector<int8_t>& SigningBlock::GetValue()
{
    return m_value;
}

int64_t SigningBlock::GetOffset()
{
    return m_offset;
}

} // namespace SignatureTools
} // namespace OHOS