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
#ifndef SIGNATRUETOOLS_DIGESTCOMMON_H
#define SIGNATRUETOOLS_DIGESTCOMMON_H
#include <string>
#include <vector>

#include "byte_buffer.h"
#include "openssl/evp.h"
#include "openssl/ossl_typ.h"
#include "digest_parameter.h"
#include "pkcs7_context.h"
#include "signature_info.h"
#include "export_define.h"
#include "openssl/pkcs7.h"
#include "openssl/safestack.h"

namespace OHOS {
namespace SignatureTools {

enum SignatureAlgorithm {
    ALGORITHM_SHA256_WITH_ECDSA = 0x00000201,
    ALGORITHM_SHA384_WITH_ECDSA,
    ALGORITHM_SHA512_WITH_ECDSA,
    ALGORITHM_SHA256_WITH_DSA = 0x00000301,
    ALGORITHM_SHA384_WITH_DSA,
    ALGORITHM_SHA512_WITH_DSA,
};

class DigestCommon {
public:
    DigestCommon() = delete;

    static int32_t GetDigest(const ByteBuffer& chunk, const std::vector<OptionalBlock>& optionalBlocks,
                             const DigestParameter& digestParameter, unsigned char(&out)[EVP_MAX_MD_SIZE]);
    static bool DigestInit(const DigestParameter& digestParameter);
    static bool DigestUpdate(const DigestParameter& digestParameter,
                             const unsigned char content[], int32_t len);
    static int32_t GetDigest(const DigestParameter& digestParameter, unsigned char(&out)[EVP_MAX_MD_SIZE]);
    static int32_t GetDigestAlgorithmOutputSizeBytes(int32_t nId);
    DLL_EXPORT static int32_t GetDigestAlgorithmId(int32_t signAlgorithm);
    static std::string GetDigestAlgorithmString(int32_t signAlgorithm);
    static void GetOpensslErrorMessage();

private:
    static bool CheckDigestParameter(const DigestParameter& digestParameter);

    static const int32_t OPENSSL_ERR_MESSAGE_MAX_LEN;
};
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_DIGESTCOMMON_H
