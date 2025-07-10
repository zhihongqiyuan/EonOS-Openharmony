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

#ifndef HDF_EXT_DEVMGR_INTERFACE_CODE_H
#define HDF_EXT_DEVMGR_INTERFACE_CODE_H

/* SAID: 5110 */
namespace OHOS {
namespace ExternalDeviceManager {
enum class DriverExtMgrInterfaceCode : uint32_t {
    QUERY_DEVICE = 1,
    BIND_DEVICE,
    UNBIND_DEVICE,
    BIND_DRIVER_WITH_DEVICE_ID,
    UNBIND_DRIVER_WITH_DEVICE_ID,
    QUERY_DEVICE_INFO,
    QUERY_DRIVER_INFO,
    INVALID_CODE
};
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // HDF_EXT_DEVMGR_INTERFACE_CODE_H