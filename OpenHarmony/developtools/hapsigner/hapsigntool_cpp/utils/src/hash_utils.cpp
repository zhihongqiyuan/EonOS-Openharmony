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

#include <fstream>
#include <map>
#include <cstdio>
#include <cstdlib>

#include "fs_digest_utils.h"
#include "file_utils.h"
#include "hash_utils.h"

namespace OHOS {
namespace SignatureTools {
    
int HashUtils::GetHashAlgsId(const std::string& algMethod)
{
    int result = static_cast<int>(HashAlgs::USE_NONE);
    if (0 == algMethod.compare("SHA-256")) {
        result = static_cast<int>(HashAlgs::USE_SHA256);
    }
    if (0 == algMethod.compare("SHA-384")) {
        result = static_cast<int>(HashAlgs::USE_SHA384);
    }
    if (0 == algMethod.compare("SHA-512")) {
        result = static_cast<int>(HashAlgs::USE_SHA512);
    }
    return result;
}

std::string HashUtils::GetHashAlgName(int algId)
{
    if (static_cast<int>(HashAlgs::USE_SHA256) == algId) {
        return "SHA-256";
    }
    if (static_cast<int>(HashAlgs::USE_SHA384) == algId) {
        return "SHA-384";
    }
    if (static_cast<int>(HashAlgs::USE_SHA512) == algId) {
        return "SHA-512";
    }
    return "";
}

std::vector<int8_t> HashUtils::GetFileDigest(const std::string& inputFile, const std::string& algName)
{
    std::vector<int8_t> result;

    std::ifstream input(inputFile, std::ios::binary);
    if (0 != input.rdstate()) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "failed to get input stream object!");
        return std::vector<int8_t>();
    }

    char buffer[HASH_LEN] = { 0 };
    int num = 0;
    std::map<int, std::vector<int8_t>> hashMap;

    while (!input.eof()) {
        input.read(buffer, HASH_LEN);

        if (input.fail() && !input.eof()) {
            PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "error occurred while reading data.");
            return std::vector<int8_t>();
        }

        std::streamsize readLen = input.gcount();
        std::string str;
        for (int i = 0; i < readLen; ++i) {
            str.push_back(buffer[i]);
        }

        std::vector<int8_t> dig = GetByteDigest(str, readLen, algName);
        hashMap.emplace(num, dig);
        ++num;
    }

    if (hashMap.empty()) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "hashMap is empty.");
        return std::vector<int8_t>();
    }

    DigestUtils fileDigestUtils(HASH_SHA256);
    for (const auto& hashMapItem : hashMap) {
        std::string str(hashMapItem.second.begin(), hashMapItem.second.end());
        fileDigestUtils.AddData(str);
    }
    std::string digest = fileDigestUtils.Result(DigestUtils::Type::BINARY);
    for (std::string::size_type i = 0; i < digest.size(); i++) {
        result.push_back(digest[i]);
    }
    return result;
}

std::vector<int8_t> HashUtils::GetDigestFromBytes(const std::vector<int8_t>& fileBytes, int64_t length,
                                                  const std::string& algName)
{
    if (fileBytes.empty() || length <= 0) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "file bytes is empty.");
        return std::vector<int8_t>();
    }
    std::map<int, std::vector<int8_t>> hashMap;
    int64_t readLength = 0;
    int64_t num = 0;
    while (readLength < length) {
        int64_t blockLength = length - readLength > HASH_LEN ? HASH_LEN : (length - readLength);
        std::string readStr(fileBytes.begin() + readLength, fileBytes.begin() + readLength + blockLength);
        std::vector<int8_t> dig = GetByteDigest(readStr, readStr.size(), algName);
        hashMap.emplace(num, dig);
        ++num;
        readLength += readStr.size();
    }
    if (hashMap.empty()) {
        PrintErrorNumberMsg("VERIFY_ERROR", VERIFY_ERROR, "hashMap is empty.");
        return std::vector<int8_t>();
    }
    DigestUtils digestUtils(HASH_SHA256);
    for (const auto& item : hashMap) {
        std::string str(item.second.begin(), item.second.end());
        digestUtils.AddData(str);
    }
    std::string digest = digestUtils.Result(DigestUtils::Type::BINARY);
    std::vector<int8_t> result;
    for (std::string::size_type i = 0; i < digest.size(); i++) {
        result.push_back(digest[i]);
    }
    return result;
}

std::vector<int8_t> HashUtils::GetByteDigest(const std::string& str, int count, const std::string& algMethod)
{
    std::vector<int8_t> result;
    DigestUtils digestUtils(HASH_SHA256);
    digestUtils.AddData(str);
    std::string digest = digestUtils.Result(DigestUtils::Type::BINARY);
    for (std::string::size_type i = 0; i < digest.size(); i++) {
        result.push_back(digest[i]);
    }
    return result;
}

} // namespace SignatureTools
} // namespace OHOS