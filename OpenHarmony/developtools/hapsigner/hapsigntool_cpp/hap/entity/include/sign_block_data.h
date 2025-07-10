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

#ifndef SIGNATRUETOOLS_SIGN_BLOCK_DATA_H
#define SIGNATRUETOOLS_SIGN_BLOCK_DATA_H

#include <string>
#include <vector>

#include "file_utils.h"

namespace OHOS {
namespace SignatureTools {
class SignBlockData {
public:
    SignBlockData(const std::vector<int8_t>& signData, const char type);
    SignBlockData(const std::string &signFile, const char type);

    char GetType();
    std::vector<int8_t>& GetBlockHead();
    void SetBlockHead(const std::vector<int8_t>& blockHead);
    std::vector<int8_t>& GetSignData();
    std::string GetSignFile();
    long GetLen();
    bool GetByte();

private:
    char m_type;
    long m_len;
    bool m_isByte;
    std::vector<int8_t> m_blockHead;
    std::vector<int8_t> m_signData;
    std::string m_signFile;
};
} // namespace SignatureTools
} // namespace OHOS
#endif
