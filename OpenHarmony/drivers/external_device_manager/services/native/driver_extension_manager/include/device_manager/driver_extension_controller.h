/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef DRIVER_EXTENSION_CONTROLLER_H
#define DRIVER_EXTENSION_CONTROLLER_H
#include <string>
#include <memory>
#include <map>
#include "single_instance.h"
#include "iremote_object.h"

namespace OHOS {
namespace ExternalDeviceManager {
class IDriverExtensionConnectCallback;
class DriverExtensionController {
    DECLARE_SINGLE_INSTANCE_BASE(DriverExtensionController);
public:
    ~DriverExtensionController() = default;
    int32_t StartDriverExtension(const std::string& bundleName, const std::string& abilityName);
    int32_t StopDriverExtension(const std::string& bundleName, const std::string& abilityName, int32_t userId = -1);
    int32_t ConnectDriverExtension(
        const std::string& bundleName,
        const std::string& abilityName,
        std::shared_ptr<IDriverExtensionConnectCallback> callback,
        uint32_t deviceId = 0
    );
    int32_t DisconnectDriverExtension(
        const std::string& bundleName,
        const std::string& abilityName,
        std::shared_ptr<IDriverExtensionConnectCallback> callback,
        uint32_t deviceId = 0
    );
    class DriverExtensionAbilityConnection;

private:
    DriverExtensionController() = default;
};

struct DrvExtConnectionInfo {
    std::string bundleName_;
    std::string abilityName_;
    uint32_t deviceId_;
    sptr<DriverExtensionController::DriverExtensionAbilityConnection> connectInner_;
};

class IDriverExtensionConnectCallback {
public:
    virtual ~IDriverExtensionConnectCallback() = default;
    virtual int32_t OnConnectDone(const sptr<IRemoteObject> &remote, int resultCode) = 0;
    virtual int32_t OnDisconnectDone(int resultCode) = 0;
    bool IsConnectDone();
    sptr<IRemoteObject> GetRemoteObj();
protected:
    friend class DriverExtensionController;
    std::shared_ptr<DrvExtConnectionInfo> info_ = nullptr;
};
}
}
#endif