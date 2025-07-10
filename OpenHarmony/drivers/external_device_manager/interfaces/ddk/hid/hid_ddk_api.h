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
#ifndef HID_DDK_API_H
#define HID_DDK_API_H

/**
 * @addtogroup HidDdk
 * @{
 *
 * @brief Provides HID DDK interfaces, including creating a device, sending an event, and destroying a device.
 *
 * @syscap SystemCapability.Driver.HID.Extension
 * @since 11
 * @version 1.0
 */

/**
 * @file hid_ddk_api.h
 *
 * @brief Declares the HID DDK interfaces for the host to access an input device.
 *
 * File to include: <hid/hid_ddk_api.h>
 * @since 11
 * @version 1.0
 */

#include <stdint.h>
#include "hid_ddk_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
  * @brief Creates a device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param hidDevice Pointer to the basic information required for creating a device, including the device name,
 * vendor ID, and product ID.
 * @param hidEventProperties Pointer to the events of the device to be observed, including the event type and
 * properties of the key event, absolute coordinate event, and relative coordinate event.
 * @return Returns the device ID (a non-negative number) if the operation is successful;
 * returns a negative number otherwise.
 * @since 11
 * @version 1.0
 */
int32_t OH_Hid_CreateDevice(Hid_Device *hidDevice, Hid_EventProperties *hidEventProperties);

/**
 * @brief Sends an event list to a device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param deviceId ID of the device, to which the event list is sent.
 * @param items List of events to sent. The event information includes the event type (<b>Hid_EventType</b>),
 * event code (<b>Hid_SynEvent</b> for a synchronization event code, <b>Hid_KeyCode</b> for a key code,
 * <b>Hid_AbsAxes</b> for an absolute coordinate code, <b>Hid_RelAxes</b> for a relative coordinate event,
 * and <b>Hid_MscEvent</b> for other input event code), and value input by the device.
 * @param length Length of the event list (number of events sent at a time).
 * @return Returns <b>0</b> if the operation is successful; returns a negative number otherwise.
 * @since 11
 * @version 1.0
 */
int32_t OH_Hid_EmitEvent(int32_t deviceId, const Hid_EmitItem items[], uint16_t length);

/**
 * @brief Destroys a device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param deviceId ID of the device to destroy.
 * @return Returns <b>0</b> if the operation is successful; returns a negative number otherwise.
 * @since 11
 * @version 1.0
 */
int32_t OH_Hid_DestroyDevice(int32_t deviceId);

/**
 * @brief Initializes the HID DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INIT_ERROR} create DDK instance failed.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 * @since 16
 */
int32_t OH_Hid_Init(void);

/**
 * @brief Releases the HID DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 * @since 16
 */
int32_t OH_Hid_Release(void);

/**
 * @brief Open HID device by deviceId in blocking mode.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param deviceId ID of the device to be operated.
 * @param interfaceIndex Interface index, which corresponds to interface which supports USB protocol HID.
 * @param dev Device operation handle.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } alloc memory of dev failed.
 *         {@link HID_DDK_IO_ERROR} open device failed.
 *         {@link HID_DDK_INVALID_PARAMETER} dev is null.
 *         {@link HID_DDK_DEVICE_NOT_FOUND} device not found by deviceId.
 * @since 16
 */
int32_t OH_Hid_Open(uint64_t deviceId, uint8_t interfaceIndex, Hid_DeviceHandle **dev);

/**
 * @brief Close HID device by dev.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_IO_ERROR} close device failed.
 *         {@link HID_DDK_INVALID_PARAMETER} dev is null.
 * @since 16
 */
int32_t OH_Hid_Close(Hid_DeviceHandle **dev);

/**
 * @brief Write an Output report to a HID device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data The data to be sent.
 * @param length The length in bytes of the data to send.
 * @param bytesWritten The acture bytes of the data be sent.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.length is 0; 4.length is greater than HID_MAX_REPORT_BUFFER_SIZE;\n
 *             5.bytesWritten is null.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_IO_ERROR } send data failed.
 * @since 16
*/
int32_t OH_Hid_Write(Hid_DeviceHandle *dev, uint8_t *data, uint32_t length, uint32_t *bytesWritten);

/**
 * @brief Read an input report from the device with timeout.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @param timeout Timeout in milliseconds or -1 for blocking wait.
 * @param bytesRead The number of bytes to read.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE;\n
 *             5.bytesRead is null.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_TIMEOUT } read timeout.
 * @since 16
*/
int32_t OH_Hid_ReadTimeout(Hid_DeviceHandle *dev, uint8_t *data, uint32_t bufSize, int timeout, uint32_t *bytesRead);

/**
 * @brief Read an input report from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @param bytesRead The number of bytes to read.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE;\n
 *             5.bytesRead is null.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_TIMEOUT } read timeout.
 * @since 16
*/
int32_t OH_Hid_Read(Hid_DeviceHandle *dev, uint8_t *data, uint32_t bufSize, uint32_t *bytesRead);

/**
 * @brief Set the device handle to be non-blocking.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param nonBlock Enable or not the nonblocking reads
 *             - 1 to enable nonblocking
 *             - 0 to disable nonblocking.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes:1.dev is null;\n
 *             2.nonBlock is not 1 or 0.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 * @since 16
*/
int32_t OH_Hid_SetNonBlocking(Hid_DeviceHandle *dev, int nonBlock);

/**
 * @brief Get a raw info from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param rawDevInfo Vendor id, product id and bus type get from the device.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.rawDevInfo is null.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetRawInfo(Hid_DeviceHandle *dev, Hid_RawDevInfo *rawDevInfo);

/**
 * @brief Get a raw name from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetRawName(Hid_DeviceHandle *dev, char *data, uint32_t bufSize);

/**
 * @brief Get a physical address from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetPhysicalAddress(Hid_DeviceHandle *dev, char *data, uint32_t bufSize);

/**
 * @brief Get a raw unique id from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetRawUniqueId(Hid_DeviceHandle *dev, uint8_t *data, uint32_t bufSize);

/**
 * @brief Send a report to the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param reportType Report type will be sent.
 * @param data The data to be sent.
 * @param length The length in bytes of the data to send.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.length is 0; 4.length is greater than HID_MAX_REPORT_BUFFER_SIZE.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_IO_ERROR } send data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_SendReport(Hid_DeviceHandle *dev, Hid_ReportType reportType, const uint8_t *data, uint32_t length);

/**
 * @brief Get a report from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param reportType Report type get from device.
 * @param data A buffer to put the read data into.
 * @param bufSize A buffer size to put the read data into.
 * @return {@link HID_DDK_SUCCESS} the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetReport(Hid_DeviceHandle *dev, Hid_ReportType reportType, uint8_t *data, uint32_t bufSize);

/**
 * @brief Get a report descriptor from the device.
 *
 * @permission ohos.permission.ACCESS_DDK_HID
 * @param dev Device operation handle.
 * @param buf The buffer to copy descriptor into.
 * @param bufSize The size of the buffer in bytes, the recommended value is HID_MAX_REPORT_DESCRIPTOR_SIZE.
 * @param bytesRead The number of bytes to read.
 * @return {@link HID_DDK_SUCCESS} if the operation is successful.
 *         {@link HID_DDK_NO_PERM} permission check failed.
 *         {@link HID_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *             2.data is null; 3.bufSize is 0; 4.bufSize is greater than HID_MAX_REPORT_BUFFER_SIZE;\n
 *             5.bytesRead is null.
 *         {@link HID_DDK_INIT_ERROR} the DDK not init.
 *         {@link HID_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link HID_DDK_MEMORY_ERROR } the memory of data copies failed.
 *         {@link HID_DDK_IO_ERROR } read data failed.
 *         {@link HID_DDK_INVALID_OPERATION } the operation is not supported.
 * @since 16
*/
int32_t OH_Hid_GetReportDescriptor(Hid_DeviceHandle *dev, uint8_t *buf, uint32_t bufSize, uint32_t *bytesRead);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // HID_DDK_API_H
