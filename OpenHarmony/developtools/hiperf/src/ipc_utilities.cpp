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

#include "ipc_utilities.h"

#include <atomic>

#include "debug_logger.h"
#include "hiperf_hilog.h"
#include "utilities.h"
#if defined(is_ohos) && is_ohos && defined(BUNDLE_FRAMEWORK_ENABLE)
#include "application_info.h"
#include "bundle_mgr_proxy.h"
#endif
#if defined(is_ohos) && is_ohos
#include "iservice_registry.h"
#include "system_ability_definition.h"
#endif

namespace OHOS::Developtools::HiPerf {

static std::atomic<bool> g_haveIpc = false;

bool IsDebugableApp(const std::string& bundleName)
{
#if defined(is_ohos) && is_ohos && defined(BUNDLE_FRAMEWORK_ENABLE)
    g_haveIpc.store(true);
    std::string err = "";
    do {
        if (bundleName.empty()) {
            err = "bundleName is empty!";
            break;
        }

        sptr<ISystemAbilityManager> sam = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
        if (sam == nullptr) {
            err = "GetSystemAbilityManager failed!";
            break;
        }

        sptr<IRemoteObject> remoteObject = sam->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
        if (remoteObject == nullptr) {
            err = "Get BundleMgr SA failed!";
            break;
        }

        sptr<AppExecFwk::BundleMgrProxy> proxy = iface_cast<AppExecFwk::BundleMgrProxy>(remoteObject);
        if (proxy == nullptr) {
            err = "iface_cast failed!";
            break;
        }

        AppExecFwk::ApplicationInfo appInfo;
        bool ret = proxy->GetApplicationInfo(bundleName, AppExecFwk::GET_APPLICATION_INFO_WITH_DISABLE,
                                             AppExecFwk::Constants::ANY_USERID, appInfo);
        if (!ret) {
            err = "GetApplicationInfo failed!";
            break;
        }

        if (appInfo.appProvisionType != AppExecFwk::Constants::APP_PROVISION_TYPE_DEBUG) {
            err = "appProvisionType is " + appInfo.appProvisionType;
            break;
        }
        HIPERF_HILOGI(MODULE_DEFAULT, "bundleName is %{public}s,appProvisionType: %{public}s",
                      bundleName.c_str(), appInfo.appProvisionType.c_str());
        return true;
    } while (0);

    HIPERF_HILOGE(MODULE_DEFAULT, "IsDebugableApp error, bundleName: [%{public}s] err: [%{public}s]",
                  bundleName.c_str(), err.c_str());
    return false;
#else
    return false;
#endif
}

bool IsApplicationEncryped(const int pid)
{
#if defined(is_ohos) && is_ohos && defined(BUNDLE_FRAMEWORK_ENABLE)
    g_haveIpc.store(true);
    CHECK_TRUE(pid <= 0, true, LOG_TYPE_PRINTF, "Invalid -p value '%d', the pid should be larger than 0\n", pid);
    std::string bundleName = GetProcessName(pid);
    CHECK_TRUE(bundleName.empty(), true, 1, "bundleName is empty,pid is %d", pid);
    auto pos = bundleName.find(":");
    if (pos != std::string::npos) {
        bundleName = bundleName.substr(0, pos);
    }
    sptr<ISystemAbilityManager> sam = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    CHECK_TRUE(sam == nullptr, true, LOG_TYPE_PRINTF, "GetSystemAbilityManager failed!\n");
    sptr<IRemoteObject> remoteObject = sam->GetSystemAbility(BUNDLE_MGR_SERVICE_SYS_ABILITY_ID);
    CHECK_TRUE(remoteObject == nullptr, true, LOG_TYPE_PRINTF, "Get BundleMgr SA failed!\n");
    sptr<AppExecFwk::BundleMgrProxy> proxy = iface_cast<AppExecFwk::BundleMgrProxy>(remoteObject);
    CHECK_TRUE(proxy == nullptr, true, LOG_TYPE_PRINTF, "iface_cast failed!\n");

    AppExecFwk::ApplicationInfo appInfo;
    bool ret = proxy->GetApplicationInfo(bundleName, AppExecFwk::ApplicationFlag::GET_BASIC_APPLICATION_INFO,
                                         AppExecFwk::Constants::ANY_USERID, appInfo);
    CHECK_TRUE(!ret, true, 1, "%s:%s GetApplicationInfo failed!", __func__, bundleName.c_str());
    bool isEncrypted = (appInfo.applicationReservedFlag &
                        static_cast<uint32_t>(AppExecFwk::ApplicationReservedFlag::ENCRYPTED_APPLICATION)) != 0;
    HLOGD("check application encryped.%d : %s, pid:%d", isEncrypted, bundleName.c_str(), pid);
    return isEncrypted;
#else
    return false;
#endif
}

void CheckIpcBeforeFork()
{
    if (g_haveIpc.load()) {
        HIPERF_HILOGW(MODULE_DEFAULT, "fork after ipc!");
    }
}

} // namespace OHOS::Developtools::HiPerf
