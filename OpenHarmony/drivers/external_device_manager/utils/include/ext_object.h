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
#ifndef EXT_OBJECT_H
#define EXT_OBJECT_H

#include <memory>
#include <string>
#include "edm_errors.h"
namespace OHOS {
namespace ExternalDeviceManager {
enum BusType : uint32_t {
    BUS_TYPE_INVALID = 0,
    BUS_TYPE_USB = 1,
    BUS_TYPE_MAX,
    BUS_TYPE_TEST,
};

class DrvBundleStateCallback;
class DriverInfoExt {
public:
    virtual ~DriverInfoExt() = default;
    virtual int32_t Serialize(std::string &str) = 0;
    virtual int32_t UnSerialize(const std::string &str) = 0;
};

class DriverInfo : public DriverInfoExt {
public:
    DriverInfo() = default;
    DriverInfo(const std::string &bundleName, const std::string &driverName, const std::string &driverUid = "",
        const int32_t userId = -1) : userId_(userId), bundleName_(bundleName), driverName_(driverName)
    {
        if (driverUid.empty()) {
            driverUid_ = bundleName + "-" + driverName;
        } else {
            driverUid_ = driverUid;
        }
    }
    int32_t Serialize(std::string &str) override;
    int32_t UnSerialize(const std::string &str) override;
    std::string GetBusName() const
    {
        return bus_;
    }
    BusType GetBusType() const
    {
        return busType_;
    }
    std::string GetDriverUid() const
    {
        return driverUid_;
    }
    int32_t GetUserId() const
    {
        return userId_;
    }
    std::string GetBundleName() const
    {
        return bundleName_;
    }
    std::string GetDriverName() const
    {
        return driverName_;
    }
    std::string GetVersion() const
    {
        return version_;
    }
    std::string GetDescription() const
    {
        return description_;
    }
    std::string GetDriverSize() const
    {
        return driverSize_;
    }
    bool GetLaunchOnBind() const
    {
        return launchOnBind_;
    }
    std::shared_ptr<DriverInfoExt> GetInfoExt() const
    {
        return driverInfoExt_;
    }
    bool IsAccessAllowed() const
    {
        return accessAllowed_;
    }
private:
    friend class DrvBundleStateCallback;
    std::string bus_;
    BusType busType_{0};
    std::string driverUid_;
    int32_t userId_ = -1;
    std::string bundleName_;
    std::string driverName_;
    std::string vendor_;
    std::string version_;
    std::string description_;
    std::string driverSize_;
    bool launchOnBind_ = false;
    bool accessAllowed_ = false;
    std::shared_ptr<DriverInfoExt> driverInfoExt_;
};

class DeviceInfo {
public:
    DeviceInfo(
        uint32_t busDeviceId,
        BusType busType = BusType::BUS_TYPE_INVALID,
        const std::string &description = "") : description_(description)
    {
        devInfo_.devBusInfo.busType = busType;
        devInfo_.devBusInfo.busDeviceId = busDeviceId;
    }
    virtual ~DeviceInfo() = default;
    BusType GetBusType() const
    {
        return devInfo_.devBusInfo.busType;
    }
    uint64_t GetDeviceId() const
    {
        return devInfo_.deviceId;
    }
    uint32_t GetBusDevId() const
    {
        return devInfo_.devBusInfo.busDeviceId;
    }
    const std::string& GetDeviceDescription() const
    {
        return description_;
    }

private:
    union DevInfo {
        uint64_t deviceId;
        struct {
            BusType busType;
            uint32_t busDeviceId;
        } devBusInfo;
    } devInfo_;
    std::string description_ {""};
};
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // EXT_OBJECT_H