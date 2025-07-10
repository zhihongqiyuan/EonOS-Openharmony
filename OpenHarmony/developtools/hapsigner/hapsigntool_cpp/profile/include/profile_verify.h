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
#ifndef SIGNATRUETOOLS_PROFILE_VERIFY_H
#define SIGNATRUETOOLS_PROFILE_VERIFY_H

#include "export_define.h"
#include "profile_info.h"

namespace OHOS {
namespace SignatureTools {

#define DEV_UUID_LEN 65
enum AppProvisionVerifyResult {
    PROVISION_INVALID, // Invalid json string
    PROVISION_OK, // Passed
    PROVISION_UNSUPPORTED_DEVICE_TYPE, // Failed to get device id
    PROVISION_NUM_DEVICE_EXCEEDED, // No. of device exceeds maximum number
    PROVISION_DEVICE_UNAUTHORIZED, // Device id not included
};
/**
* @brief Parse and verify the app provision
* @param appProvision The app provision json string.
* @param info Out param, the parsed app provision structure.
* @return AppProvisionVerifyResult Verification result.
*/
DLL_EXPORT AppProvisionVerifyResult ParseAndVerify(const std::string& appProvision, ProfileInfo& info);
/**
* @brief Parse app provision
* @param appProvision The app provision json string.
* @param info Out param, the parsed app provision structure.
* @return AppProvisionVerifyResult parse result.
*/
DLL_EXPORT AppProvisionVerifyResult ParseProvision(const std::string& appProvision, ProfileInfo& info);
/**
* @brief Parse app provision profile
* @param appProvision The app provision json string.
* @param info Out param, the parsed app provision structure.
* @return AppProvisionVerifyResult parse result.
*/
DLL_EXPORT AppProvisionVerifyResult ParseProfile(const std::string& appProvision, ProfileInfo& info);
} // namespace SignatureTools
} // namespace OHOS
#endif // SIGNATRUETOOLS_PROFILE_VERIFY_H
