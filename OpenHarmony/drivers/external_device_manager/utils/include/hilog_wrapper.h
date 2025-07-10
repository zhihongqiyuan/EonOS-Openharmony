/*
 * Copyright (c) 2021-2025 Huawei Device Co., Ltd.
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

#ifndef HILOG_WRAPPER_H
#define HILOG_WRAPPER_H

#define CONFIG_HILOG
#ifdef CONFIG_HILOG
#include "hilog/log.h"
namespace OHOS {
namespace ExternalDeviceManager {

#ifdef EDM_LOGF
#undef EDM_LOGF
#endif

#ifdef EDM_LOGE
#undef EDM_LOGE
#endif

#ifdef EDM_LOGW
#undef EDM_LOGW
#endif

#ifdef EDM_LOGI
#undef EDM_LOGI
#endif

#ifdef EDM_LOGD
#undef EDM_LOGD
#endif

// 0xD002550: part:ExternalDeviceManager module:Edm.
constexpr unsigned int BASE_EDM_DOMAIN_ID = 0xD002550;

enum PkgErrCode {
    PKG_OK = 0,
    PKG_FAILURE = -1,
    PKG_RDB_EXECUTE_FAILTURE = -2,
    PKG_RDB_NO_INIT = -3,
    PKG_RDB_EMPTY = -4,
    PKG_PERMISSION_DENIED = -5,
    PKG_NOP = -6,
    PKG_OVERFLOW = -7,
};

// param of log interface, such as EDM_LOGF.
enum UsbMgrSubModule {
    MODULE_FRAMEWORK = 0,
    MODULE_SERVICE,
    MODULE_DEV_MGR,
    MODULE_PKG_MGR,
    MODULE_EA_MGR,
    MODULE_BUS_USB,
    MODULE_COMMON,
    MODULE_USB_DDK,
    EDM_MODULE_TEST,
    MODULE_HID_DDK,
    MODULE_BASE_DDK,
    MODULE_USB_SERIAL_DDK,
    MODULE_SCSIPERIPHERAL_DDK,
    EDM_MODULE_BUTT,
};

enum UsbMgrDomainId {
    EDM_FRAMEWORK_DOMAIN = BASE_EDM_DOMAIN_ID + MODULE_FRAMEWORK,
    EDM_SERVICE_DOMAIN,
    EDM_DEV_MGR_DOMAIN,
    EDM_PKG_MGR_DOMAIN,
    EDM_DDK_DOMAIN,
    EDM_BUTT,
};


static constexpr OHOS::HiviewDFX::HiLogLabel EDM_MGR_LABEL[EDM_MODULE_BUTT] = {
    { LOG_CORE, EDM_FRAMEWORK_DOMAIN, "EdmFwk" },
    { LOG_CORE, EDM_SERVICE_DOMAIN, "EdmService" },
    { LOG_CORE, EDM_DEV_MGR_DOMAIN, "EdmDevMgr" },
    { LOG_CORE, EDM_PKG_MGR_DOMAIN, "EdmPkgMgr" },
    { LOG_CORE, EDM_FRAMEWORK_DOMAIN, "EdmEaMgr" },
    { LOG_CORE, EDM_FRAMEWORK_DOMAIN, "EdmBusUsbMgr" },
    { LOG_CORE, EDM_FRAMEWORK_DOMAIN, "EdmCommon" },
    { LOG_CORE, EDM_DDK_DOMAIN, "EdmUsbDdk" },
    { LOG_CORE, EDM_FRAMEWORK_DOMAIN, "EdmTest" },
    { LOG_CORE, EDM_DDK_DOMAIN, "EdmHidDdk" },
    { LOG_CORE, EDM_DDK_DOMAIN, "EdmBaseDdk" },
    { LOG_CORE, EDM_DDK_DOMAIN, "EdmUsbSerialDdk" },
    { LOG_CORE, EDM_DDK_DOMAIN, "EdmScsiDdk" },
};

#ifndef EDM_FILENAME
#define EDM_FILENAME (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifndef EDM_FUNC_FMT
#define EDM_FUNC_FMT "[%{public}s(%{public}s:%{public}d)]"
#endif

#ifndef EDM_FUNC_INFO
#define EDM_FUNC_INFO EDM_FILENAME, __FUNCTION__, __LINE__
#endif

// In order to improve performance, do not check the module range, module should less than EDM_MODULE_BUTT.
#define EDM_LOGF(module, fmt, ...) \
    (void)HILOG_IMPL(LOG_CORE, LOG_FATAL, EDM_MGR_LABEL[module].domain, EDM_MGR_LABEL[module].tag, \
    EDM_FUNC_FMT fmt, EDM_FUNC_INFO, ##__VA_ARGS__)
#define EDM_LOGE(module, fmt, ...) \
    (void)HILOG_IMPL(LOG_CORE, LOG_ERROR, EDM_MGR_LABEL[module].domain, EDM_MGR_LABEL[module].tag, \
    EDM_FUNC_FMT fmt, EDM_FUNC_INFO, ##__VA_ARGS__)
#define EDM_LOGW(module, fmt, ...) \
    (void)HILOG_IMPL(LOG_CORE, LOG_WARN, EDM_MGR_LABEL[module].domain, EDM_MGR_LABEL[module].tag, \
    EDM_FUNC_FMT fmt, EDM_FUNC_INFO, ##__VA_ARGS__)
#define EDM_LOGI(module, fmt, ...) \
    (void)HILOG_IMPL(LOG_CORE, LOG_INFO, EDM_MGR_LABEL[module].domain, EDM_MGR_LABEL[module].tag, \
    EDM_FUNC_FMT fmt, EDM_FUNC_INFO, ##__VA_ARGS__)
#define EDM_LOGD(module, fmt, ...) \
    (void)HILOG_IMPL(LOG_CORE, LOG_DEBUG, EDM_MGR_LABEL[module].domain, EDM_MGR_LABEL[module].tag, \
    EDM_FUNC_FMT fmt, EDM_FUNC_INFO, ##__VA_ARGS__)
} // namespace ExternalDeviceManager
} // namespace OHOS

#else

#define EDM_LOGF(...)
#define EDM_LOGE(...)
#define EDM_LOGW(...)
#define EDM_LOGI(...)
#define EDM_LOGD(...)

#endif // CONFIG_HILOG

#endif // HILOG_WRAPPER_H
