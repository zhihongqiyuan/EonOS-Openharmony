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

#ifndef DEVICE_MANAGER_ETX_DEVICE_MGR_H
#define DEVICE_MANAGER_ETX_DEVICE_MGR_H

#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include "device.h"
#include "ext_object.h"
#include "idriver_change_callback.h"
#include "single_instance.h"
#include "timer.h"

namespace OHOS {
namespace ExternalDeviceManager {
using namespace std;
class ExtDeviceManager final {
    DECLARE_SINGLE_INSTANCE_BASE(ExtDeviceManager);

public:
    ~ExtDeviceManager();
    int32_t Init();
    int32_t RegisterDevice(shared_ptr<DeviceInfo> devInfo);
    int32_t UnRegisterDevice(const shared_ptr<DeviceInfo> devInfo);
    vector<shared_ptr<DeviceInfo>> QueryDevice(const BusType busType);
    vector<shared_ptr<Device>> QueryAllDevices();
    vector<shared_ptr<Device>> QueryDevicesById(const uint64_t deviceId);
    int32_t ConnectDevice(uint64_t deviceId, uint32_t callingTokenId,
        const sptr<IDriverExtMgrCallback> &connectCallback);
    int32_t DisConnectDevice(uint64_t deviceId, uint32_t callingTokenId);
    int32_t ConnectDriverWithDeviceId(uint64_t deviceId, uint32_t callingTokenId,
        const unordered_set<std::string> &accessibleBundles, const sptr<IDriverExtMgrCallback> &connectCallback);
    int32_t DisConnectDriverWithDeviceId(uint64_t deviceId, uint32_t callingTokenId);
    void RemoveDeviceOfDeviceMap(shared_ptr<Device> device);
    std::unordered_set<uint64_t> DeleteBundlesOfBundleInfoMap(const std::string &bundleName = "");
    void MatchDriverInfos(std::unordered_set<uint64_t> deviceIds);
    void ClearMatchedDrivers(const int32_t userId);
    void SetDriverChangeCallback(shared_ptr<IDriverChangeCallback> &driverChangeCallback);

private:
    ExtDeviceManager() = default;
    void PrintMatchDriverMap();
    int32_t AddDevIdOfBundleInfoMap(shared_ptr<Device> device, string &bundleInfo);
    int32_t RemoveDevIdOfBundleInfoMap(shared_ptr<Device> device, string &bundleInfo);
    void UpdateDriverInfo(const shared_ptr<Device> &device);
    void RemoveDriverInfo(const shared_ptr<Device> &device);
    std::shared_ptr<Device> QueryDeviceByDeviceID(uint64_t deviceId);
    void UnLoadSelf(void);
    size_t GetTotalDeviceNum(void) const;
    int32_t CheckAccessPermission(const std::shared_ptr<DriverInfo> &driverInfo,
        const unordered_set<std::string> &accessibleBundles) const;
    unordered_map<BusType, unordered_map<uint64_t, shared_ptr<Device>>> deviceMap_;
    unordered_map<string, unordered_set<uint64_t>> bundleMatchMap_; // driver matching table
    mutex deviceMapMutex_;
    mutex bundleMatchMapMutex_;
    Utils::Timer unloadSelftimer_ {"unLoadSelfTimer"};
    uint32_t unloadSelftimerId_ {UINT32_MAX};
    std::shared_ptr<IDriverChangeCallback> driverChangeCallback_ = nullptr;
};
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // DEVICE_MANAGER_ETX_DEVICE_MGR_H