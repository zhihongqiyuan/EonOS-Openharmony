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

#include <algorithm>

#include "nlohmann/json.hpp"
#include "signature_tools_log.h"
#include "signature_tools_errno.h"
#include "profile_verify.h"

using namespace std;
using namespace nlohmann;

namespace {
const string KEY_VERSION_CODE = "version-code";
const string KEY_VERSION_NAME = "version-name";
const string KEY_UUID = "uuid";
const string KEY_TYPE = "type";
const string KEY_APP_DIST_TYPE = "app-distribution-type";
const string KEY_BUNDLE_INFO = "bundle-info";
const string KEY_DEVELOPER_ID = "developer-id";
const string KEY_DEVELOPMENT_CERTIFICATE = "development-certificate";
const string KEY_DISTRIBUTION_CERTIFICATE = "distribution-certificate";
const string KEY_BUNDLE_NAME = "bundle-name";
const string KEY_APL = "apl";
const string KEY_APP_FEATURE = "app-feature";
const string KEY_ACLS = "acls";
const string KEY_ALLOWED_ACLS = "allowed-acls";
const string KEY_PERMISSIONS = "permissions";
const string KEY_DATA_GROUP_IDS = "data-group-ids";
const string KEY_RESTRICTED_PERMISSIONS = "restricted-permissions";
const string KEY_RESTRICTED_CAPABILITIES = "restricted-capabilities";
const string KEY_DEBUG_INFO = "debug-info";
const string KEY_DEVICE_ID_TYPE = "device-id-type";
const string KEY_DEVICE_IDS = "device-ids";
const string KEY_ISSUER = "issuer";
const string KEY_APP_PRIVILEGE_CAPABILITIES = "app-privilege-capabilities";
const string KEY_APP_SERVICE_CAPABILITIES = "app-service-capabilities";
const string VALUE_TYPE_RELEASE = "release";
const string VALUE_TYPE_DEBUG = "debug";
const string VALUE_DIST_TYPE_APP_GALLERY = "app_gallery";
const string VALUE_DIST_TYPE_ENTERPRISE = "enterprise";
const string VALUE_DIST_TYPE_ENTERPRISE_NORMAL = "enterprise_normal";
const string VALUE_DIST_TYPE_ENTERPRISE_MDM = "enterprise_mdm";
const string VALUE_DIST_TYPE_OS_INTEGRATION = "os_integration";
const string VALUE_DIST_TYPE_CROWDTESTING = "crowdtesting";
const string VALUE_DEVICE_ID_TYPE_UDID = "udid";
const string VALUE_VALIDITY = "validity";
const string VALUE_NOT_BEFORE = "not-before";
const string VALUE_NOT_AFTER = "not-after";
// reserved field
const string KEY_BASEAPP_INFO = "baseapp-info";
const string KEY_PACKAGE_NAME = "package-name";
const string KEY_PACKAGE_CERT = "package-cert";
const string KEY_APP_IDENTIFIER = "app-identifier";
const string GENERIC_BUNDLE_NAME = ".*";

inline void GetStringIfExist(const json& obj, const string& key, string& out)
{
    if (obj.find(key.c_str()) != obj.end() && obj[key.c_str()].is_string()) {
        obj[key.c_str()].get_to(out);
    }
}
inline void GetInt32IfExist(const json& obj, const string& key, int32_t& out)
{
    if (obj.find(key.c_str()) != obj.end() && obj[key.c_str()].is_number_integer()) {
        obj[key.c_str()].get_to(out);
    }
}
inline void GetInt64IfExist(const json& obj, const string& key, int64_t& out)
{
    if (obj.find(key.c_str()) != obj.end() && obj[key.c_str()].is_number_integer()) {
        obj[key.c_str()].get_to(out);
    }
}
inline void GetStringArrayIfExist(const json& obj, const string& key, vector<string>& out)
{
    if (obj.find(key.c_str()) != obj.end() && obj[key.c_str()].is_array()) {
        for (auto& item : obj[key.c_str()]) {
            if (item.is_string()) {
                out.push_back(item.get<string>());
            }
        }
    }
}
inline bool IsObjectExist(const json& obj, const string& key)
{
    return obj.find(key.c_str()) != obj.end() && obj[key.c_str()].is_object();
}
} // namespace
namespace OHOS {
namespace SignatureTools {
const std::map<std::string, int32_t> distTypeMap = {
    {VALUE_DIST_TYPE_APP_GALLERY, AppDistType::APP_GALLERY},
    {VALUE_DIST_TYPE_ENTERPRISE, AppDistType::ENTERPRISE},
    {VALUE_DIST_TYPE_ENTERPRISE_NORMAL, AppDistType::ENTERPRISE_NORMAL},
    {VALUE_DIST_TYPE_ENTERPRISE_MDM, AppDistType::ENTERPRISE_MDM},
    {VALUE_DIST_TYPE_OS_INTEGRATION, AppDistType::OS_INTEGRATION},
    {VALUE_DIST_TYPE_CROWDTESTING, AppDistType::CROWDTESTING}
};

void ParseType(const json& obj, ProfileInfo& out)
{
    string type;
    GetStringIfExist(obj, KEY_TYPE, type);
    /* If not release, then it's debug */
    if (type == VALUE_TYPE_RELEASE)
        out.type = RELEASE;
    else if (type == VALUE_TYPE_DEBUG)
        out.type = DEBUG;
    else out.type = NONE_PROVISION_TYPE;
}
void ParseAppDistType(const json& obj, ProfileInfo& out)
{
    string distType;
    GetStringIfExist(obj, KEY_APP_DIST_TYPE, distType);
    auto ite = distTypeMap.find(distType);
    if (ite != distTypeMap.end()) {
        out.distributionType = static_cast<AppDistType>(distTypeMap.at(distType));
        return;
    }
    out.distributionType = AppDistType::NONE_TYPE;
}
void ParseBundleInfo(const json& obj, ProfileInfo& out)
{
    if (IsObjectExist(obj, KEY_BUNDLE_INFO)) {
        const auto& bundleInfo = obj[KEY_BUNDLE_INFO];
        GetStringIfExist(bundleInfo, KEY_DEVELOPER_ID, out.bundleInfo.developerId);
        GetStringIfExist(bundleInfo, KEY_DEVELOPMENT_CERTIFICATE,
                         out.bundleInfo.developmentCertificate);
        GetStringIfExist(bundleInfo, KEY_DISTRIBUTION_CERTIFICATE,
                         out.bundleInfo.distributionCertificate);
        GetStringIfExist(bundleInfo, KEY_BUNDLE_NAME, out.bundleInfo.bundleName);
        GetStringIfExist(bundleInfo, KEY_APL, out.bundleInfo.apl);
        GetStringIfExist(bundleInfo, KEY_APP_FEATURE, out.bundleInfo.appFeature);
        GetStringIfExist(bundleInfo, KEY_APP_IDENTIFIER, out.bundleInfo.appIdentifier);
        GetStringArrayIfExist(bundleInfo, KEY_DATA_GROUP_IDS, out.bundleInfo.dataGroupIds);
    }
}
void ParseAcls(const json& obj, ProfileInfo& out)
{
    if (IsObjectExist(obj, KEY_ACLS)) {
        const auto& acls = obj[KEY_ACLS];
        GetStringArrayIfExist(acls, KEY_ALLOWED_ACLS, out.acls.allowedAcls);
    }
}
void ParsePermissions(const json& obj, ProfileInfo& out)
{
    if (IsObjectExist(obj, KEY_PERMISSIONS)) {
        const auto& permissions = obj[KEY_PERMISSIONS];
        GetStringArrayIfExist(permissions, KEY_RESTRICTED_PERMISSIONS,
                              out.permissions.restrictedPermissions);
        GetStringArrayIfExist(permissions, KEY_RESTRICTED_CAPABILITIES,
                              out.permissions.restrictedCapabilities);
    }
}
void ParseDebugInfo(const json& obj, ProfileInfo& out)
{
    if (IsObjectExist(obj, KEY_DEBUG_INFO)) {
        GetStringIfExist(obj[KEY_DEBUG_INFO], KEY_DEVICE_ID_TYPE, out.debugInfo.deviceIdType);
        GetStringArrayIfExist(obj[KEY_DEBUG_INFO], KEY_DEVICE_IDS, out.debugInfo.deviceIds);
    }
}
void ParseValidity(const json& obj, Validity& out)
{
    if (IsObjectExist(obj, VALUE_VALIDITY)) {
        GetInt64IfExist(obj[VALUE_VALIDITY], VALUE_NOT_BEFORE, out.notBefore);
        GetInt64IfExist(obj[VALUE_VALIDITY], VALUE_NOT_AFTER, out.notAfter);
    }
}
void ParseMetadata(const json& obj, ProfileInfo& out)
{
    if (IsObjectExist(obj, KEY_BASEAPP_INFO)) {
        const auto& baseAppInfo = obj[KEY_BASEAPP_INFO];
        Metadata metadata;
        metadata.name = KEY_PACKAGE_NAME;
        GetStringIfExist(baseAppInfo, KEY_PACKAGE_NAME, metadata.value);
        out.metadatas.emplace_back(metadata);
        metadata.name = KEY_PACKAGE_CERT;
        GetStringIfExist(baseAppInfo, KEY_PACKAGE_CERT, metadata.value);
        out.metadatas.emplace_back(metadata);
    }
}
void from_json(const json& obj, ProfileInfo& out)
{
    if (!obj.is_object()) {
        return;
    }
    GetInt32IfExist(obj, KEY_VERSION_CODE, out.versionCode);
    GetStringIfExist(obj, KEY_VERSION_NAME, out.versionName);
    GetStringIfExist(obj, KEY_UUID, out.uuid);
    ParseType(obj, out);
    ParseAppDistType(obj, out);
    ParseBundleInfo(obj, out);
    ParseAcls(obj, out);
    ParsePermissions(obj, out);
    ParseDebugInfo(obj, out);
    GetStringIfExist(obj, KEY_ISSUER, out.issuer);
    GetStringArrayIfExist(obj, KEY_APP_PRIVILEGE_CAPABILITIES, out.appPrivilegeCapabilities);
    ParseValidity(obj, out.validity);
    ParseMetadata(obj, out);
    GetStringIfExist(obj, KEY_APP_SERVICE_CAPABILITIES, out.appServiceCapabilities);
}

AppProvisionVerifyResult ReturnIfStringIsEmpty(const std::string& str, const std::string& errMsg)
{
    if (str.empty()) {
        PrintErrorNumberMsg("PROVISION_INVALID_ERROR", PROVISION_INVALID_ERROR, errMsg);
        return PROVISION_INVALID;
    }
    return PROVISION_OK;
}

AppProvisionVerifyResult ReturnIfIntIsNonPositive(int num, const std::string& errMsg)
{
    if (num <= 0) {
        PrintErrorNumberMsg("PROVISION_INVALID_ERROR", PROVISION_INVALID_ERROR, errMsg);
        return PROVISION_INVALID;
    }
    return PROVISION_OK;
}

static AppProvisionVerifyResult CheckProfileValidType(const ProfileInfo& info)
{
    if (info.type == ProvisionType::DEBUG) {
        if (ReturnIfStringIsEmpty(info.bundleInfo.developmentCertificate,
                                  "Tag development-certificate is empty.") != PROVISION_OK) {
            return PROVISION_INVALID;
        }
    } else if (info.type == ProvisionType::RELEASE) {
        if (ReturnIfIntIsNonPositive(info.distributionType,
                                     "Tag app-distribution-type is empty.") != PROVISION_OK) {
            return PROVISION_INVALID;
        }
        if (ReturnIfStringIsEmpty(info.bundleInfo.distributionCertificate,
                                  "Tag distribution-certificate is empty.") != PROVISION_OK) {
            return PROVISION_INVALID;
        }
    } else {
        PrintErrorNumberMsg("PROVISION_INVALID_ERROR", PROVISION_INVALID_ERROR,
                            "The type field in the profile file is incorrect");
        return PROVISION_INVALID;
    }
    return PROVISION_OK;
}

AppProvisionVerifyResult ParseProvision(const string& appProvision, ProfileInfo& info)
{
    if (ParseProfile(appProvision, info) != PROVISION_OK) {
        return PROVISION_INVALID;
    }

    if (CheckProfileValidType(info) != PROVISION_OK) {
        return PROVISION_INVALID;
    }
    return PROVISION_OK;
}

AppProvisionVerifyResult ParseAndVerify(const string& appProvision, ProfileInfo& info)
{
    SIGNATURE_TOOLS_LOGD("Enter HarmonyAppProvision Verify");
    AppProvisionVerifyResult ret = ParseProvision(appProvision, info);
    if (ret != PROVISION_OK) {
        return ret;
    }
    SIGNATURE_TOOLS_LOGD("Leave HarmonyAppProvision Verify");
    return PROVISION_OK;
}
AppProvisionVerifyResult ParseProfile(const std::string& appProvision, ProfileInfo& info)
{
    json obj = json::parse(appProvision, nullptr, false);
    if (obj.is_discarded() || (!obj.is_structured())) {
        std::string errStr = "invalid json object, parse provision failed, json: " + appProvision;
        PrintErrorNumberMsg("PROVISION_INVALID_ERROR", PROVISION_INVALID_ERROR, errStr.c_str());
        return PROVISION_INVALID;
    }
    obj.get_to(info);
    return PROVISION_OK;
}
} // namespace SignatureTools
} // namespace OHOS