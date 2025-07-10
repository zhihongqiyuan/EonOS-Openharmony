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

#include "sign_block_data.h"

namespace OHOS {
namespace SignatureTools {

SignBlockData::SignBlockData(const std::vector<int8_t>& signData, const char type)
{
    m_signData = std::move(signData);
    m_type = type;
    m_len = signData.size();
    m_isByte = true;
}

SignBlockData::SignBlockData(const std::string& signFile, const char type)
{
    m_signFile = std::move(signFile);
    m_type = type;
    m_len = FileUtils::GetFileLen(signFile);
    m_isByte = false;
}

char SignBlockData::GetType()
{
    return m_type;
}

std::vector<int8_t>& SignBlockData::GetBlockHead()
{
    return m_blockHead;
}

void SignBlockData::SetBlockHead(const std::vector<int8_t>& blockHead)
{
    m_blockHead = std::move(blockHead);
}

std::vector<int8_t>& SignBlockData::GetSignData()
{
    return m_signData;
}

std::string SignBlockData::GetSignFile()
{
    return m_signFile;
}

long SignBlockData::GetLen()
{
    return m_len;
}

bool SignBlockData::GetByte()
{
    return m_isByte;
}

} // namespace SignatureTools
} // namespace OHOS