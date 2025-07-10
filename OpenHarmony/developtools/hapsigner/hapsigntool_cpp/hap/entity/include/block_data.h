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

#ifndef SIGNATRUETOOLS_BLOCK_DATA_H
#define SIGNATRUETOOLS_BLOCK_DATA_H

#include <string>

namespace OHOS {
namespace SignatureTools {
class BlockData {
public:
    BlockData(const int32_t blockNum, const int64_t blockStart);

    int32_t GetBlockNum();
    void SetBlockNum(const int32_t blockNum);
    int64_t GetBlockStart();
    void SetBlockStart(const int64_t blockStart);

private:
    int32_t m_blockNum;
    int64_t m_blockStart;
};
} // namespace SignatureTools
} // namespace OHOS
#endif