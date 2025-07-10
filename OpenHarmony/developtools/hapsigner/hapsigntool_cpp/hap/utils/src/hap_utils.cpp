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
#include <algorithm>
#include <stdexcept>

#include "profile_verify.h"
#include "hap_utils.h"

namespace OHOS {
namespace SignatureTools {
    
const std::vector<int8_t> HapUtils::HAP_SIGNING_BLOCK_MAGIC_V2 =
    std::vector<int8_t>{ 0x48, 0x41, 0x50, 0x20, 0x53, 0x69, 0x67, 0x20, 0x42,
    0x6c, 0x6f, 0x63, 0x6b, 0x20, 0x34, 0x32 };
const std::vector<int8_t> HapUtils::HAP_SIGNING_BLOCK_MAGIC_V3 =
    std::vector<int8_t>{ 0x3c, 0x68, 0x61, 0x70, 0x20, 0x73, 0x69, 0x67, 0x6e,
    0x20, 0x62, 0x6c, 0x6f, 0x63, 0x6b, 0x3e };
const std::string HapUtils::HEX_CHAR_ARRAY = "0123456789ABCDEF";
const std::string HapUtils::HAP_DEBUG_OWNER_ID = "DEBUG_LIB_ID";
std::set<int> HapUtils::HAP_SIGNATURE_OPTIONAL_BLOCK_IDS;

HapUtils::StaticConstructor::StaticConstructor()
{
    HAP_SIGNATURE_OPTIONAL_BLOCK_IDS.insert(HAP_PROOF_OF_ROTATION_BLOCK_ID);
    HAP_SIGNATURE_OPTIONAL_BLOCK_IDS.insert(HAP_PROFILE_BLOCK_ID);
    HAP_SIGNATURE_OPTIONAL_BLOCK_IDS.insert(HAP_PROPERTY_BLOCK_ID);
}

HapUtils::StaticConstructor HapUtils::staticConstructor;

std::string HapUtils::GetAppIdentifier(const std::string& profileContent)
{
    std::pair<std::string, std::string> resultPair = ParseAppIdentifier(profileContent);

    std::string ownerID = resultPair.first;
    std::string profileType = resultPair.second;

    if (profileType == "debug") {
        return HAP_DEBUG_OWNER_ID;
    } else if (profileType == "release") {
        return ownerID;
    } else {
        return "";
    }
}

std::pair<std::string, std::string> HapUtils::ParseAppIdentifier(const std::string& profileContent)
{
    std::string ownerID;
    std::string profileType;

    ProfileInfo provisionInfo;
    ParseProfile(profileContent, provisionInfo);

    if (DEBUG == provisionInfo.type) {
        profileType = "debug";
    } else {
        profileType = "release";
    }

    BundleInfo bundleInfo = provisionInfo.bundleInfo;

    if (!bundleInfo.appIdentifier.empty()) {
        ownerID = bundleInfo.appIdentifier;
    }

    return std::pair(ownerID, profileType);
}

std::vector<int8_t> HapUtils::GetHapSigningBlockMagic(int compatibleVersion)
{
    if (compatibleVersion >= MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3) {
        return HAP_SIGNING_BLOCK_MAGIC_V3;
    }
    return HAP_SIGNING_BLOCK_MAGIC_V2;
}

int HapUtils::GetHapSigningBlockVersion(int compatibleVersion)
{
    if (compatibleVersion >= MIN_COMPATIBLE_VERSION_FOR_SCHEMA_V3) {
        return HAP_SIGN_SCHEME_V3_BLOCK_VERSION;
    }
    return HAP_SIGN_SCHEME_V2_BLOCK_VERSION;
}

bool HapUtils::ReadFileToByteBuffer(const std::string& file, ByteBuffer& buffer)
{
    std::string ret;
    if (FileUtils::ReadFile(file, ret) < 0) {
        PrintErrorNumberMsg("IO_ERROR", IO_ERROR, file + " not exist or can not read!");
        return false;
    }
    buffer.SetCapacity(static_cast<int32_t>(ret.size()));
    buffer.PutData(ret.data(), ret.size());
    return true;
}

} // namespace SignatureTools
} // namespace OHOS