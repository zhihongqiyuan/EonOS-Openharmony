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

#ifndef IUSB_EXTENSION_H
#define IUSB_EXTENSION_H
#include <vector>
#include <map>
#include "ext_object.h"
#include "idev_change_callback.h"
#include "idriver_change_callback.h"
namespace OHOS {
namespace ExternalDeviceManager {
using namespace std;

class IBusExtension {
public:
    virtual ~IBusExtension() = default;
    virtual shared_ptr<DriverInfoExt> ParseDriverInfo(const map<string, string> &metadata) = 0;
    virtual shared_ptr<DriverInfoExt> GetNewDriverInfoExtObject() = 0;
    virtual bool MatchDriver(const DriverInfo &driver, const DeviceInfo &device, const std::string &type = "") = 0;
    virtual int32_t SetDevChangeCallback(shared_ptr<IDevChangeCallback> callback) = 0;
    virtual BusType GetBusType() = 0;
    virtual shared_ptr<IDriverChangeCallback> AcquireDriverChangeCallback() = 0;
};
}
}

#endif