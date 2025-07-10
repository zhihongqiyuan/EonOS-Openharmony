/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
#include "content_digest_algorithm.h"
namespace OHOS {
namespace SignatureTools {
const ContentDigestAlgorithm ContentDigestAlgorithm::SHA256("SHA-256", 256 / 8);
const ContentDigestAlgorithm ContentDigestAlgorithm::SHA384("SHA-384", 384 / 8);
const ContentDigestAlgorithm ContentDigestAlgorithm::SHA512("SHA-512", 512 / 8);

ContentDigestAlgorithm::ContentDigestAlgorithm()
    : m_digestAlgorithm(""),
    m_digestOutputByteSize(0)
{
}

ContentDigestAlgorithm::ContentDigestAlgorithm(const std::string& digestAlgorithm,
                                               const int digestOutputByteSize)
    : m_digestAlgorithm(digestAlgorithm), m_digestOutputByteSize(digestOutputByteSize)
{
}

ContentDigestAlgorithm::ContentDigestAlgorithm(const ContentDigestAlgorithm& other)
    : m_digestAlgorithm(other.m_digestAlgorithm),
    m_digestOutputByteSize(other.m_digestOutputByteSize)
{
}

ContentDigestAlgorithm& ContentDigestAlgorithm::operator=(const ContentDigestAlgorithm& other)
{
    if (this != &other) {
        m_digestAlgorithm = other.m_digestAlgorithm;
        m_digestOutputByteSize = other.m_digestOutputByteSize;
    }
    return *this;
}

std::string ContentDigestAlgorithm::GetDigestAlgorithm()
{
    return m_digestAlgorithm;
}

int ContentDigestAlgorithm::GetDigestOutputByteSize()
{
    return m_digestOutputByteSize;
}
} // namespace SignatureTools
} // namespace OHOS