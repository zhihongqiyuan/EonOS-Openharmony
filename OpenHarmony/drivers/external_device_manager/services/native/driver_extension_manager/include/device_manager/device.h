/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

#ifndef OHOS_EXTERNAL_DEVICE_MANAGER_DEVICE_H
#define OHOS_EXTERNAL_DEVICE_MANAGER_DEVICE_H

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <unordered_map>
#include "driver_extension_controller.h"
#include "ext_object.h"
#include "idriver_ext_mgr_callback.h"

namespace OHOS {
namespace ExternalDeviceManager {
struct CallerInfo {
    bool isBound = false;
};

class DrvExtConnNotify;
class Device : public std::enable_shared_from_this<Device> {
public:
    explicit Device(std::shared_ptr<DeviceInfo> info) : info_(info) {}

    int32_t Connect();
    int32_t Connect(const sptr<IDriverExtMgrCallback> &connectCallback, uint32_t callingTokenId);
    int32_t Disconnect(const bool isFromBind);

    bool HasDriver() const
    {
        return !bundleInfo_.empty();
    };

    std::shared_ptr<DriverInfo> GetDriverInfo() const
    {
        return driverInfo_;
    }

    std::shared_ptr<DeviceInfo> GetDeviceInfo() const
    {
        return info_;
    }

    void AddBundleInfo(const std::string &bundleInfo, const std::shared_ptr<DriverInfo> &driverInfo)
    {
        bundleInfo_ = bundleInfo;
        driverInfo_ = driverInfo;
        if (driverInfo != nullptr) {
            driverUid_ = driverInfo->GetDriverUid();
        }
    }

    std::string GetDriverUid()
    {
        return driverUid_;
    }

    void RemoveBundleInfo()
    {
        bundleInfo_.clear();
        driverUid_.clear();
    }

    void RemoveDriverInfo()
    {
        driverInfo_ = nullptr;
    }

    std::string GetBundleInfo() const
    {
        return bundleInfo_;
    }

    static inline std::string GetStiching()
    {
        return stiching_;
    }

    sptr<IRemoteObject> GetDrvExtRemote()
    {
        return drvExtRemote_;
    }

    void UpdateDrvExtRemote(const sptr<IRemoteObject> &remote)
    {
        drvExtRemote_ = remote;
    }

    void ClearDrvExtRemote()
    {
        drvExtRemote_ = nullptr;
    }

    void AddDrvExtConnNotify()
    {
        if (connectNofitier_ == nullptr) {
            connectNofitier_ = std::make_shared<DrvExtConnNotify>(shared_from_this());
        }
    }

    void RemoveDrvExtConnNotify()
    {
        connectNofitier_ = nullptr;
    }

    bool IsUnRegisted()
    {
        return isUnRegisted;
    }

    void UnRegist()
    {
        isUnRegisted = true;
    }

    void RemoveCaller(uint32_t callingTokenId)
    {
        boundCallerInfos_.erase(callingTokenId);
    }

    void ClearBoundCallerInfos()
    {
        boundCallerInfos_.clear();
    }

    bool IsLastCaller(uint32_t caller) const;
    bool IsBindCaller(uint32_t caller) const;

    static std::string GetBundleName(const std::string &bundleInfo);
    static std::string GetAbilityName(const std::string &bundleInfo);

private:
    void OnConnect(const sptr<IRemoteObject> &remote, int resultCode);
    void OnDisconnect(int resultCode);
    void UpdateDrvExtConnNotify();
    int32_t RegisterDrvExtMgrCallback(const sptr<IDriverExtMgrCallback> &callback);
    void UnregisterDrvExtMgrCallback(const sptr<IDriverExtMgrCallback> &callback);
    void UnregisterDrvExtMgrCallback(const wptr<IRemoteObject> &object);
    bool RegisteDeathRecipient(const sptr<IDriverExtMgrCallback> &callback);

    struct DrvExtMgrCallbackCompare {
        bool operator()(const sptr<IDriverExtMgrCallback> &lhs, const sptr<IDriverExtMgrCallback> &rhs) const
        {
            sptr<IRemoteObject> lhsRemote = lhs->AsObject();
            sptr<IRemoteObject> rhsRemote = rhs->AsObject();
            if (lhsRemote != rhsRemote) {
                return false;
            }

            return lhsRemote.GetRefPtr() < rhsRemote.GetRefPtr();
        }
    };

    friend class DriverExtMgrCallbackDeathRecipient;
    friend class DrvExtConnNotify;
    static std::string stiching_;
    std::string bundleInfo_;
    std::string driverUid_;
    std::shared_ptr<DriverInfo> driverInfo_;
    std::shared_ptr<DeviceInfo> info_;

    std::recursive_mutex deviceMutex_;
    sptr<IRemoteObject> drvExtRemote_;
    std::set<sptr<IDriverExtMgrCallback>, DrvExtMgrCallbackCompare> callbacks_;
    std::shared_ptr<DrvExtConnNotify> connectNofitier_;
    bool isUnRegisted = false;
    std::unordered_map<uint32_t, CallerInfo> boundCallerInfos_;
};

class DriverExtMgrCallbackDeathRecipient : public IRemoteObject::DeathRecipient {
public:
    DriverExtMgrCallbackDeathRecipient(const std::weak_ptr<Device> device) : device_(device) {}
    ~DriverExtMgrCallbackDeathRecipient() = default;
    void OnRemoteDied(const wptr<IRemoteObject> &remote);

private:
    DISALLOW_COPY_AND_MOVE(DriverExtMgrCallbackDeathRecipient);
    std::weak_ptr<Device> device_;
};

class DrvExtConnNotify : public IDriverExtensionConnectCallback {
public:
    explicit DrvExtConnNotify(std::weak_ptr<Device> device) : device_(device) {}
    int32_t OnConnectDone(const sptr<IRemoteObject> &remote, int resultCode) override;
    int32_t OnDisconnectDone(int resultCode) override;
    bool IsInvalidDrvExtConnectionInfo()
    {
        return info_ == nullptr;
    }
    void ClearDrvExtConnectionInfo()
    {
        info_ = nullptr;
    }
    int32_t GetCurrentActiveUserId();

private:
    std::weak_ptr<Device> device_;
};
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // OHOS_EXTERNAL_DEVICE_MANAGER_DEVICE_H
