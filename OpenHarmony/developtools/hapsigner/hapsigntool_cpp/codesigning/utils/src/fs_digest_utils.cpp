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
#include "fs_digest_utils.h"
#include "securec.h"

void DigestUtils::AddData(const std::string &data)
{
    AddData(data.data(), (int)data.size());
}

void DigestUtils::AddData(const char* data, int length)
{
    int ret = EVP_DigestUpdate(m_ctx, data, length);
    if (ret < 1) {
        printf("Update DigestFunc failed!\n");
    }
}

std::string DigestUtils::Result(DigestUtils::Type type)
{
    unsigned int len = 0;

    const std::map<HashType, int> hashLength = {
        {HASH_SHA256, SHA256_DIGEST_LENGTH},
        {HASH_SHA384, SHA384_DIGEST_LENGTH},
    };

    unsigned char* md = reinterpret_cast<unsigned char*>(new char[hashLength.at(m_type)]);
    int ret = EVP_DigestFinal_ex(m_ctx, md, &len);
    if (ret < 1) {
        printf("Failed to Calculate Hash Relsult\n");
    }
    int temporaryVariableFirst = 2;
    if (type == Type::HEX) {
        int temporaryVariableSecond = 3;
        char* res = new char[len * temporaryVariableFirst + 1];
        for (unsigned int i = 0; i < len; i++) {
            snprintf_s(&res[i * temporaryVariableFirst], temporaryVariableSecond,
                       temporaryVariableFirst, "%02x", md[i]);
        }
        std::string st{res, len * temporaryVariableFirst};
        delete[]md;
        delete[]res;
        return st;
    }
    std::string st{reinterpret_cast<char*>(md), len};
    delete[]md;
    return st;
}

DigestUtils::DigestUtils(HashType type)
{
    m_type = type;
    m_ctx = EVP_MD_CTX_new();

    const std::map<HashType, hashFunc> hashMethods = {
        {HASH_SHA256, EVP_sha256},
        {HASH_SHA384, EVP_sha384}
    };

    int ret = EVP_DigestInit_ex(m_ctx, hashMethods.at(type)(), nullptr);
    if (ret < 1) {
        printf("Init DigestFunc failed!\n");
    }
}

DigestUtils::~DigestUtils()
{
    if (m_ctx != nullptr) {
        EVP_MD_CTX_free(m_ctx);
    }
}