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
#ifndef SIGNATRUETOOLS_PKCS7_CONTEXT_H
#define SIGNATRUETOOLS_PKCS7_CONTEXT_H

#include <vector>
#include <string>
#include "openssl/pkcs7.h"
#include "openssl/x509.h"
#include "byte_buffer.h"
#include "matching_result.h"

namespace OHOS {
namespace SignatureTools {

using CertChain = std::vector<X509*>;
using Pkcs7CertChains = std::vector<CertChain>;

struct Pkcs7Context {
    bool needWriteCrl;
    int32_t digestAlgorithm = 0;
    MatchingResult matchResult;
    std::string certIssuer;
    PKCS7* p7;
    Pkcs7CertChains certChain;
    ByteBuffer content;
    Pkcs7Context()
        : needWriteCrl(false), digestAlgorithm(0), matchResult(), certIssuer(),
        p7(nullptr), certChain(), content()
    {
    }
    ~Pkcs7Context()
    {
        if (p7 != nullptr) {
            PKCS7_free(p7);
            p7 = nullptr;
        }
        for (auto certChain : certChain) {
            for (auto cert : certChain) {
                X509_free(cert);
            }
        }
        certChain.clear();
    }
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_PKCS7_CONTEXT_H
