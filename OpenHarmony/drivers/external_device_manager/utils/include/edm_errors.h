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
#ifndef EDM_ERRORS_H
#define EDM_ERRORS_H

#include <cstdint>
#include <errors.h>

namespace OHOS {
namespace ExternalDeviceManager {
#define EDM_MODULE_ID 1

constexpr int32_t EDM_ERR_OFFSET = ErrCodeOffset(SUBSYS_DRIVERS, EDM_MODULE_ID);

enum UsbErrCode : int32_t {
    EDM_OK = 0,
    EDM_NOK = EDM_ERR_OFFSET,
    EDM_ERR_GET_SYSTEM_ABILITY_MANAGER_FAILED,
    EDM_ERR_GET_SERVICE_FAILED,
    EDM_ERR_CONNECTION_FAILED,
    EDM_ERR_NOT_SUPPORT,
    EDM_ERR_INVALID_PARAM,
    EDM_ERR_INVALID_OBJECT,
    EDM_EER_MALLOC_FAIL,
    EDM_ERR_TIMEOUT,
    EDM_ERR_DEVICE_BUSY,
    EDM_ERR_IO,
    EDM_ERR_NO_PERM,
    EDM_ERR_OUT_OF_RANGE,
    EDM_ERR_JSON_PARSE_FAIL,
    EDM_ERR_JSON_OBJ_ERR,
    EDM_ERR_USB_ERR,
    EDM_ERR_NOT_SYSTEM_APP,
    EDM_ERR_SERVICE_NOT_ALLOW_ACCESS,
    EDM_ERR_SERVICE_NOT_BOUND,
};
} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // EDM_ERRORS_H