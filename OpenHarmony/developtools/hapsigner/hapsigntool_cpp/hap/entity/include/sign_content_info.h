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

#ifndef SIGNATRUETOOLS_SIGN_CONTENT_INFO_H
#define SIGNATRUETOOLS_SIGN_CONTENT_INFO_H

#include <vector>
#include <cstdint>
#include <string>
#include <list>

namespace OHOS {
namespace SignatureTools {
class SignContentHash {
public:
    SignContentHash(const char type, const char tag, const short algId,
                    const int length, const std::vector<int8_t>& hash);

public:
    static const int CONTENT_HEAD_SIZE = 8;
    char m_type;
    char m_tag;
    short m_algId;
    int m_length;
    std::vector<int8_t> m_hash;
    int m_contentHashLen;
};

class SignContentInfo {
public:
    SignContentInfo();

public:
    void AddContentHashData(const char type, const char tag, const short algId,
                            const int length, const std::vector<int8_t> &hash);
    void AddHashData(const SignContentHash &signInfo);
    std::vector<int8_t> GetByteContent();

private:
    std::string m_version;
    short m_size;
    short m_numOfBlocks;
    std::list<SignContentHash> m_hashData;
};
} // namespace SignatureTools
} // namespace OHOS
#endif