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
#include "digest_parameter.h"
#include "openssl/evp.h"

namespace OHOS {
namespace SignatureTools {
DigestParameter::DigestParameter() : digestOutputSizeBytes(0), md(nullptr), ctxPtr(nullptr)
{
}

DigestParameter::~DigestParameter()
{
    if (ctxPtr != nullptr) {
        EVP_MD_CTX_destroy(ctxPtr);
        ctxPtr = nullptr;
    }
    /* md points to the OpenSSL global static struct constant, no need to free. */
    md = nullptr;
}

DigestParameter::DigestParameter(const DigestParameter& other)
{
    digestOutputSizeBytes = other.digestOutputSizeBytes;
    md = other.md;
    ctxPtr = EVP_MD_CTX_create();
    EVP_MD_CTX_copy(ctxPtr, other.ctxPtr);
}

DigestParameter& DigestParameter::operator = (const DigestParameter& other)
{
    if (ctxPtr != nullptr) {
        EVP_MD_CTX_destroy(ctxPtr);
        ctxPtr = nullptr;
    }
    digestOutputSizeBytes = other.digestOutputSizeBytes;
    md = other.md;
    ctxPtr = EVP_MD_CTX_create();
    EVP_MD_CTX_copy(ctxPtr, other.ctxPtr);
    return *this;
}
}
}