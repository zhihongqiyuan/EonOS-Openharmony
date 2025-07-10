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
#ifndef SIGNATURETOOLS_CONTENT_DIGEST_ALGORITHM_H
#define SIGNATURETOOLS_CONTENT_DIGEST_ALGORITHM_H
#include <string>

namespace OHOS {
namespace SignatureTools {
// content digest algorithm
class ContentDigestAlgorithm {
public:
    static const ContentDigestAlgorithm SHA256;
    static const ContentDigestAlgorithm SHA384;
    static const ContentDigestAlgorithm SHA512;

    ContentDigestAlgorithm();
    ContentDigestAlgorithm(const ContentDigestAlgorithm& other);
    ContentDigestAlgorithm(const std::string& digestAlgorithm, const int digestOutputByteSize);
    ContentDigestAlgorithm& operator=(const ContentDigestAlgorithm& other);

    std::string GetDigestAlgorithm();
    int GetDigestOutputByteSize();

private:
    std::string m_digestAlgorithm;
    int m_digestOutputByteSize;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATURETOOLS_CONTENT_DIGEST_ALGORITHM_H