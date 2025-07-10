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

#include "block_data.h"

namespace OHOS {
namespace SignatureTools {

BlockData::BlockData(const int32_t blockNum, const int64_t blockStart)
{
    m_blockNum = blockNum;
    m_blockStart = blockStart;
}

int32_t BlockData::GetBlockNum()
{
    return m_blockNum;
}

void BlockData::SetBlockNum(const int32_t blockNum)
{
    m_blockNum = blockNum;
}

int64_t BlockData::GetBlockStart()
{
    return m_blockStart;
}

void BlockData::SetBlockStart(const int64_t blockStart)
{
    m_blockStart = blockStart;
}

} // namespace SignatureTools
} // namespace OHOS
