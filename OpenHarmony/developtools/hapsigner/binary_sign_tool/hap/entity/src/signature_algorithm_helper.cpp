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
#include "signature_algorithm_helper.h"

namespace OHOS {
namespace SignatureTools {
const SignatureAlgorithmHelper SignatureAlgorithmHelper::ECDSA_WITH_SHA256_INSTANCE{
    SignatureAlgorithmId::ECDSA_WITH_SHA256, "EC", ContentDigestAlgorithm::SHA256,
    {"SHA256withECDSA", nullptr} };

const SignatureAlgorithmHelper SignatureAlgorithmHelper::ECDSA_WITH_SHA384_INSTANCE{
    SignatureAlgorithmId::ECDSA_WITH_SHA384, "EC", ContentDigestAlgorithm::SHA384,
    {"SHA384withECDSA", nullptr} };

SignatureAlgorithmHelper::SignatureAlgorithmHelper() : m_id(SignatureAlgorithmId::ECDSA_WITH_SHA256),
    m_keyAlgorithm(""),
    m_contentDigestAlgorithm(ContentDigestAlgorithm::SHA256),
    m_signatureAlgAndParams("", nullptr)
{
}

SignatureAlgorithmHelper::SignatureAlgorithmHelper(const SignatureAlgorithmHelper& other) : m_id(other.m_id),
    m_keyAlgorithm(other.m_keyAlgorithm),
    m_contentDigestAlgorithm(other.m_contentDigestAlgorithm),
    m_signatureAlgAndParams(other.m_signatureAlgAndParams.first, nullptr)
{
}

SignatureAlgorithmHelper::SignatureAlgorithmHelper(const SignatureAlgorithmId& id_, const std::string& keyAlg_,
                                                   const ContentDigestAlgorithm& digestAlg_,
                                                   const std::pair<std::string, void*>& sigParams_)
    : m_id(id_), m_keyAlgorithm(keyAlg_), m_contentDigestAlgorithm(digestAlg_), m_signatureAlgAndParams(sigParams_)
{
}

SignatureAlgorithmHelper& SignatureAlgorithmHelper::operator=(const SignatureAlgorithmHelper& other)
{
    if (this != &other) {
        m_id = other.m_id;
        m_keyAlgorithm = other.m_keyAlgorithm;
        m_contentDigestAlgorithm = other.m_contentDigestAlgorithm;
        m_signatureAlgAndParams.first = other.m_signatureAlgAndParams.first;
        m_signatureAlgAndParams.second = other.m_signatureAlgAndParams.second;
    }
    return *this;
}

SignatureAlgorithmHelper::~SignatureAlgorithmHelper()
{
}

const SignatureAlgorithmHelper* SignatureAlgorithmHelper::FindById(const SignatureAlgorithmId id)
{
    if (id == SignatureAlgorithmId::ECDSA_WITH_SHA256) return &ECDSA_WITH_SHA256_INSTANCE;
    if (id == SignatureAlgorithmId::ECDSA_WITH_SHA384) return &ECDSA_WITH_SHA384_INSTANCE;
    return nullptr;
}
} // namespace SignatureTools
} // namespace OHOS