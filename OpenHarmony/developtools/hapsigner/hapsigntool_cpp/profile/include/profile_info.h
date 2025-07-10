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
#ifndef SIGNATRUETOOLS_PROFILE_INFO_H
#define SIGNATRUETOOLS_PROFILE_INFO_H

#include <string>
#include <vector>
#include <memory>

#include "export_define.h"

namespace OHOS {
namespace SignatureTools {

enum ProvisionType {
    NONE_PROVISION_TYPE,
    RELEASE,
    DEBUG
};
enum AppDistType {
    NONE_TYPE,
    APP_GALLERY,
    ENTERPRISE,
    OS_INTEGRATION,
    CROWDTESTING,
    ENTERPRISE_NORMAL,
    ENTERPRISE_MDM
};
typedef struct BundleInfoSt {
    std::string developerId;
    std::string apl;
    std::string distributionCertificate;
    std::string developmentCertificate;
    std::string bundleName;
    std::string appIdentifier;
    std::vector<std::string> dataGroupIds;
    std::string appFeature;
}BundleInfo;
typedef struct AclsSt {
    std::vector<std::string> allowedAcls;
}Acls;
typedef struct PermissionsSt {
    std::vector<std::string> restrictedCapabilities;
    std::vector<std::string> restrictedPermissions;
}Permissions;
typedef struct DebugInfoSt {
    std::vector<std::string> deviceIds;
    std::string deviceIdType;
}DebugInfo;
typedef struct ValiditySt {
    int64_t notAfter = 0;
    int64_t notBefore = 0;
}Validity;
typedef struct MetadataSt {
    std::string value;
    std::string resource;
    std::string name;
}Metadata;
struct ProfileInfo {
    DLL_EXPORT ProfileInfo();
    DLL_EXPORT ~ProfileInfo();
    DLL_EXPORT ProfileInfo(const ProfileInfo& info);
    DLL_EXPORT ProfileInfo& operator=(const ProfileInfo& info);
    int32_t versionCode = 0;
    std::string uuid;
    std::string versionName;
    AppDistType distributionType = NONE_TYPE;
    ProvisionType type = NONE_PROVISION_TYPE;
    BundleInfoSt bundleInfo;
    PermissionsSt permissions;
    AclsSt acls;
    std::string issuer;
    DebugInfoSt debugInfo;
    std::string fingerprint;
    std::string appId;
    std::vector<std::string> appPrivilegeCapabilities;
    ValiditySt validity;
    int32_t profileBlockLength = 0;
    std::vector<MetadataSt> metadatas;
    std::string organization;
    std::string appServiceCapabilities;
    std::unique_ptr<unsigned char[]> profileBlock;
};
} // namespace SignatureTools
} // namespace OHOS

#endif // SIGNATRUETOOLS_PROFILE_INFO_H
