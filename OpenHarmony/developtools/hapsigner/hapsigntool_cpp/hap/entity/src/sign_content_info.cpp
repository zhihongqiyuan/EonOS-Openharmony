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

#include "sign_content_info.h"
#include "byte_array_utils.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {
    
SignContentHash::SignContentHash(const char type, const char tag, const short algId,
                                 const int length, const std::vector<int8_t> &hash)
{
    m_type = type;
    m_tag = tag;
    m_algId = algId;
    m_length = length;
    m_hash = hash;
    m_contentHashLen = CONTENT_HEAD_SIZE + hash.size();
}

SignContentInfo::SignContentInfo()
{
    m_version = "1000";
    m_size = SignContentHash::CONTENT_HEAD_SIZE;
    m_numOfBlocks = 0;
}

void SignContentInfo::AddContentHashData(const char type, const char tag, const short algId,
                                         const int length, const std::vector<int8_t> &hash)
{
    SignContentHash signInfo(type, tag, algId, length, hash);
    AddHashData(signInfo);
}

void SignContentInfo::AddHashData(const SignContentHash& signInfo)
{
    m_hashData.push_back(signInfo);
    ++m_numOfBlocks;
    m_size += signInfo.m_contentHashLen;
}

std::vector<int8_t> SignContentInfo::GetByteContent()
{
    std::vector<int8_t> ret(m_size, 0);
    int index = 0;

    index = ByteArrayUtils::InsertCharToByteArray(ret, index, m_version);
    if (index < 0) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "InsertCharToByteArray failed.");
        return std::vector<int8_t>();
    }

    index = ByteArrayUtils::InsertShortToByteArray(ret, ret.size(), index, m_size);
    if (index < 0) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "InsertShortToByteArray failed.");
        return std::vector<int8_t>();
    }

    index = ByteArrayUtils::InsertShortToByteArray(ret, ret.size(), index, m_numOfBlocks);
    if (index < 0) {
        PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "InsertShortToByteArray failed.");
        return std::vector<int8_t>();
    }

    for (const auto& tmp : m_hashData) {
        ret[index] = tmp.m_type;
        index++;
        ret[index] = tmp.m_tag;
        index++;
        index = ByteArrayUtils::InsertShortToByteArray(ret, ret.size(), index, tmp.m_algId);
        index = ByteArrayUtils::InsertIntToByteArray(ret, index, tmp.m_length);
        index = ByteArrayUtils::InsertByteToByteArray(ret, index, tmp.m_hash, tmp.m_hash.size());
        if (index < 0) {
            PrintErrorNumberMsg("SIGN_ERROR", SIGN_ERROR, "InsertShortToByteArray failed.");
            return std::vector<int8_t>();
        }
    }
    return ret;
}

} // namespace SignatureTools
} // namespace OHOS