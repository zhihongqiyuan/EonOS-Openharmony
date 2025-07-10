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
#include "digest_common.h"
#include "signature_tools_log.h"
#include "openssl/err.h"

namespace OHOS {
namespace SignatureTools {
const int32_t DigestCommon::OPENSSL_ERR_MESSAGE_MAX_LEN = 1024;

int32_t DigestCommon::GetDigestAlgorithmOutputSizeBytes(int32_t nId)
{
    return EVP_MD_size(EVP_get_digestbynid(nId));
}

bool DigestCommon::CheckDigestParameter(const DigestParameter& digestParameter)
{
    if (digestParameter.md == nullptr) {
        SIGNATURE_TOOLS_LOGE("md is nullptr");
        return false;
    }
    if (digestParameter.ctxPtr == nullptr) {
        SIGNATURE_TOOLS_LOGE("ctxPtr is nullptr");
        return false;
    }
    return true;
}

bool DigestCommon::DigestInit(const DigestParameter& digestParameter)
{
    if (!CheckDigestParameter(digestParameter)) {
        return false;
    }
    if (EVP_DigestInit(digestParameter.ctxPtr, digestParameter.md) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestInit failed");
        return false;
    }
    return true;
}

/* the caller must ensure that EVP_DigestInit was called before calling this function */
bool DigestCommon::DigestUpdate(const DigestParameter& digestParameter,
                                const unsigned char content[], int32_t len)
{
    if (content == nullptr) {
        SIGNATURE_TOOLS_LOGE("content is nullptr");
        return false;
    }
    if (!CheckDigestParameter(digestParameter)) {
        return false;
    }
    if (EVP_DigestUpdate(digestParameter.ctxPtr, content, len) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestUpdate failed");
        return false;
    }
    return true;
}

int32_t DigestCommon::GetDigest(const DigestParameter& digestParameter,
                                unsigned char(&out)[EVP_MAX_MD_SIZE])
{
    uint32_t outLen = 0;
    if (!CheckDigestParameter(digestParameter)) {
        return outLen;
    }
    if (EVP_DigestFinal(digestParameter.ctxPtr, out, &outLen) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestFinal failed");
        outLen = 0;
    }
    return outLen;
}

int32_t DigestCommon::GetDigest(const ByteBuffer& chunk,
                                const std::vector<OptionalBlock>& optionalBlocks,
                                const DigestParameter& digestParameter,
                                unsigned char(&out)[EVP_MAX_MD_SIZE])
{
    int32_t chunkLen = chunk.Remaining();
    uint32_t outLen = 0;
    if (digestParameter.md == nullptr) {
        SIGNATURE_TOOLS_LOGE("md is nullprt");
        return outLen;
    }
    if (digestParameter.ctxPtr == nullptr) {
        SIGNATURE_TOOLS_LOGE("ctxPtr is nullprt");
        return outLen;
    }
    if (EVP_DigestInit(digestParameter.ctxPtr, digestParameter.md) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestInit failed");
        return outLen;
    }
    if (EVP_DigestUpdate(digestParameter.ctxPtr, chunk.GetBufferPtr(), chunkLen) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestUpdate chunk failed");
        return outLen;
    }
    for (int32_t i = 0; i < static_cast<int>(optionalBlocks.size()); i++) {
        chunkLen = optionalBlocks[i].optionalBlockValue.GetCapacity();
        if (EVP_DigestUpdate(digestParameter.ctxPtr, optionalBlocks[i].optionalBlockValue.GetBufferPtr(),
            chunkLen) <= 0) {
            GetOpensslErrorMessage();
            SIGNATURE_TOOLS_LOGE("EVP_DigestUpdate %dst optional block failed", i);
            return outLen;
        }
    }
    if (EVP_DigestFinal(digestParameter.ctxPtr, out, &outLen) <= 0) {
        GetOpensslErrorMessage();
        SIGNATURE_TOOLS_LOGE("EVP_DigestFinal failed");
        outLen = 0;
    }
    return outLen;
}

void DigestCommon::GetOpensslErrorMessage()
{
    unsigned long retOpenssl;
    char errOpenssl[OPENSSL_ERR_MESSAGE_MAX_LEN];
    while ((retOpenssl = ERR_get_error()) != 0) {
        ERR_error_string(retOpenssl, errOpenssl);
        SIGNATURE_TOOLS_LOGE("openssl err: %lu, message: %s", retOpenssl, errOpenssl);
    }
}

int32_t DigestCommon::GetDigestAlgorithmId(int32_t signAlgorithm)
{
    switch (signAlgorithm) {
        case ALGORITHM_SHA256_WITH_ECDSA:
        case ALGORITHM_SHA256_WITH_DSA:
            return NID_sha256;
        case ALGORITHM_SHA384_WITH_ECDSA:
        case ALGORITHM_SHA384_WITH_DSA:
            return NID_sha384;
        case ALGORITHM_SHA512_WITH_ECDSA:
        case ALGORITHM_SHA512_WITH_DSA:
            return NID_sha512;
        default:
            SIGNATURE_TOOLS_LOGE("signAlgorithm: %d error", signAlgorithm);
            return NID_undef;
    }
}

std::string DigestCommon::GetDigestAlgorithmString(int32_t signAlgorithm)
{
    switch (signAlgorithm) {
        case ALGORITHM_SHA256_WITH_ECDSA:
            return "SHA-256";
        case ALGORITHM_SHA384_WITH_ECDSA:
            return "SHA-384";
        case ALGORITHM_SHA512_WITH_ECDSA:
            return "SHA-512";
        default:
            SIGNATURE_TOOLS_LOGE("signAlgorithm: %d error", signAlgorithm);
            return "";
    }
}
} // namespace SignatureTools
} // namespace OHOS