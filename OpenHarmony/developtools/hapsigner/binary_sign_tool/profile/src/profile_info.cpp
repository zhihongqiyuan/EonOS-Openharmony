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
#include "profile_info.h"
#include "securec.h"
#include "signature_tools_log.h"

namespace OHOS {
namespace SignatureTools {

ProfileInfo::ProfileInfo()
{
    profileBlock = nullptr;
}
ProfileInfo::~ProfileInfo()
{
    profileBlock.reset(nullptr);
}
ProfileInfo::ProfileInfo(const ProfileInfo& profileInfo)
{
    *this = profileInfo;
}
ProfileInfo& ProfileInfo::operator=(const ProfileInfo& profileInfo)
{
    if (this == &profileInfo) {
        return *this;
    }
    this->versionCode = profileInfo.versionCode;
    this->versionName = profileInfo.versionName;
    this->uuid = profileInfo.uuid;
    this->type = profileInfo.type;
    this->distributionType = profileInfo.distributionType;
    this->bundleInfo = profileInfo.bundleInfo;
    this->acls = profileInfo.acls;
    this->permissions = profileInfo.permissions;
    this->debugInfo = profileInfo.debugInfo;
    this->issuer = profileInfo.issuer;
    this->appId = profileInfo.appId;
    this->fingerprint = profileInfo.fingerprint;
    this->appPrivilegeCapabilities = profileInfo.appPrivilegeCapabilities;
    this->validity = profileInfo.validity;
    this->metadatas = profileInfo.metadatas;
    this->profileBlockLength = profileInfo.profileBlockLength;
    (this->profileBlock).reset(nullptr);
    if (profileInfo.profileBlockLength != 0 && profileInfo.profileBlock != nullptr) {
        this->profileBlock = std::make_unique<unsigned char[]>(profileInfo.profileBlockLength);
        unsigned char* profileBlockData = (this->profileBlock).get();
        unsigned char* originalProfile = profileInfo.profileBlock.get();
        if (profileBlockData == nullptr) {
            return *this;
        }
        std::copy(originalProfile, originalProfile + profileInfo.profileBlockLength, profileBlockData);
    }
    this->appServiceCapabilities = profileInfo.appServiceCapabilities;
    this->organization = profileInfo.organization;
    return *this;
}
} // namespace SignatureTools
} // namespace OHOS