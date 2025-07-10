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
#ifndef SIGNATURETOOLS_SIGNATUR_ALGORITHM_H
#define SIGNATURETOOLS_SIGNATUR_ALGORITHM_H
#include <string>
#include <utility> // for std::pair

#include "content_digest_algorithm.h"

namespace OHOS {
namespace SignatureTools {
enum class SignatureAlgorithmId {
    ECDSA_WITH_SHA256 = 0x201,
    ECDSA_WITH_SHA384 = 0x202,
    ECDSA_WITH_SHA512 = 0x203,
    DSA_WITH_SHA256 = 0x301,
    DSA_WITH_SHA384 = 0x302,
    DSA_WITH_SHA512 = 0x303
};

class SignatureAlgorithmHelper {
public:
    static const SignatureAlgorithmHelper ECDSA_WITH_SHA256_INSTANCE;
    static const SignatureAlgorithmHelper ECDSA_WITH_SHA384_INSTANCE;

    SignatureAlgorithmHelper();
    SignatureAlgorithmHelper(const SignatureAlgorithmHelper& other);
    SignatureAlgorithmHelper(const SignatureAlgorithmId &id_, const std::string &keyAlg_,
                             const ContentDigestAlgorithm& digestAlg_,
                             const std::pair<std::string, void*>& sigParams_);
    SignatureAlgorithmHelper& operator=(const SignatureAlgorithmHelper& other);
    ~SignatureAlgorithmHelper();

    static const SignatureAlgorithmHelper* FindById(const SignatureAlgorithmId id);

    SignatureAlgorithmId m_id;
    std::string m_keyAlgorithm;
    ContentDigestAlgorithm m_contentDigestAlgorithm;
    std::pair<std::string, void*> m_signatureAlgAndParams;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_SIGNER_CONFIG_H