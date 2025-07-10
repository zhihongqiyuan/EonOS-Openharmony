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

#ifndef SIGNATURETOOLS_SIGN_BLOCK_INFO_H
#define SIGNATURETOOLS_SIGN_BLOCK_INFO_H

#include <vector>
#include <unordered_map>

#include "signing_block.h"

namespace OHOS {
namespace SignatureTools {
class SignBlockInfo {
public:
    SignBlockInfo();
    SignBlockInfo(bool needGenerateDigest);

public:
    std::unordered_map<int8_t, SigningBlock>& GetSignBlockMap();
    std::vector<int8_t>& GetFileDigest();
    void SetFileDigest(const std::vector<int8_t>& fileDigest);
    std::vector<int8_t>& GetRawDigest();
    void SetRawDigest(const std::vector<int8_t>& rawDigest);
    bool GetNeedGenerateDigest();

private:
    std::unordered_map<int8_t, SigningBlock> m_signBlockMap;
    std::vector<int8_t> m_fileDigest;
    std::vector<int8_t> m_rawDigest;
    bool m_needGenerateDigest;
};
} // namespace SignatureTools
} // namespace OHOS
#endif