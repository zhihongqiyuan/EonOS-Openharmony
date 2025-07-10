/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#ifndef DRIVER_REPORT_SYS_EVENT_H
#define DRIVER_REPORT_SYS_EVENT_H

#include <string>
#include <vector>
#include <mutex>
#include <memory>
#include <map>

namespace OHOS {
namespace ExternalDeviceManager {
class DeviceInfo;
class DriverInfo;
struct PkgInfoTable;

enum EXTDEV_EXP_EVENT {  // 操作类型枚举
    DRIVER_BIND = 1,             // 绑定设备驱动
    DRIVER_UNBIND,               // 解绑设备驱动
    DRIVER_PACKAGE_DATA_REFRESH, // 驱动包数据刷新
    DRIVER_PACKAGE_CYCLE_MANAGE, // 驱动包管理生命周期
    GET_DEVICE_INFO,             // 设备信息获取
    CHANGE_FUNC,                 // 切换过程中生命周期
    DRIVER_DEVICE_MATCH,         // 设备与驱动匹配
};

typedef struct ExtDevEvent {
    int32_t deviceClass;       // 设备类型
    int32_t deviceSubClass;    // 设备子类型
    int32_t deviceProtocol;    // 设备协议
    std::string snNum;         // 设备SN号
    int32_t vendorId;          // 厂商Id
    int32_t productId;         // 产品Id
    uint64_t deviceId;          // 设备Id
    std::string driverUid;     // 驱动Uid
    std::string driverName;    // 驱动名称
    std::string versionCode;   // 驱动版本
    std::string vids;          // 驱动配置的vid
    std::string pids;          // 驱动配置的pid
    int32_t userId;            // 用户Id
    std::string bundleName;    // 驱动包名
    int32_t operatType;        // 操作类型
    std::string interfaceName; // 接口名称
    std::string message;       // 信息
    int32_t errCode;           // 故障码

    ExtDevEvent(std::string interfaceName = "", const int32_t operatType = 0, const uint64_t deviceId = 0)
        : deviceClass(0), deviceSubClass(0), deviceProtocol(0), vendorId(0), productId(0), deviceId(deviceId),
          userId(0), operatType(operatType), interfaceName(std::move(interfaceName)), errCode(0) {}
} ExtDevEvent;

class ExtDevReportSysEvent {
public:
    enum class EventErrCode {
        SUCCESS = 0,
        BIND_JS_CALLBACK_FAILED = 10001,
        CONNECT_DRIVER_EXTENSION_FAILED,
        BIND_ACCESS_NOT_ALLOWED,
        UNBIND_DRIVER_EMPTY = 20001,
        UNBIND_RELATION_NOT_FOUND,
        DISCONNECT_DRIVER_EXTENSION_FAILED,
        QUERY_DRIVER_EXTENSION_FAILED = 30001,
        UPDATE_DATABASE_FAILED,
        LIFECYCLE_FUNCTION_FAILED = 40001,
        OPEN_DEVICE_FAILED = 50001,
        GET_DEVICE_DESCRIPTOR_FAILED,
        DEVICE_DESCRIPTOR_LENGTH_INVALID,
        GET_INTERFACE_DESCRIPTOR_FAILED,
        STOP_DRIVER_EXTENSION_FAILED = 60001,
        QUERY_DRIVER_INFO_FAILED = 70001,
        NO_MATCHING_DRIVER_FOUND
    };

    static const std::map<EventErrCode, std::string> ErrMsgs;

    static void ReportExternalDeviceEvent(const std::shared_ptr<ExtDevEvent> &extDevEvent);

    static void ReportExternalDeviceEvent(const std::shared_ptr<ExtDevEvent> &extDevEvent, const EventErrCode errCode);

    static void ParseToExtDevEvent(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<ExtDevEvent> &eventObj);

    static void ParseToExtDevEvent(const std::shared_ptr<DriverInfo> &driverInfo,
        const std::shared_ptr<ExtDevEvent> &eventObj);

    static void ParseToExtDevEvent(const std::shared_ptr<DeviceInfo> &deviceInfo,
        const std::shared_ptr<DriverInfo> &driverInfo, const std::shared_ptr<ExtDevEvent> &eventObj);

    static std::string ParseIdVector(std::vector<uint16_t> ids);
};

} // namespace ExternalDeviceManager
} // namespace OHOS
#endif // DRIVER_REPORT_SYS_EVENT_H