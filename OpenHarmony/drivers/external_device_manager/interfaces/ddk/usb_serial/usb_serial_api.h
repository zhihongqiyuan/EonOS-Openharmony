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

/**
 * @addtogroup USBSerialDDK
 * @{
 *
 * @brief Provides USB SERIAL DDK types and declares the macros, enumerated variables, and\n
 * data structures required by the USB SERIAL DDK APIs.
 *
 * @since 16
 */

/**
 * @file usb_serial_api.h
 *
 * @brief Declares the USB SERIAL DDK interfaces for the usb host to access an usb serial device.
 *
 * @kit DriverDevelopmentKit
 * @library libusb_serial.z.so
 * @syscap SystemCapability.Driver.UsbSerial.Extension
 * @since 16
 */

#ifndef DDK_USB_SERIAL_API_H
#define DDK_USB_SERIAL_API_H

#include <stdint.h>
#include "usb_serial_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes the USB serial DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk init error.
 * @since 16
 */
int32_t OH_UsbSerial_Init(void);

/**
 * @brief Releases the USB serial DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 * @since 16
 */
int32_t OH_UsbSerial_Release(void);

/**
 * @brief Open USB serial device by deviceId.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param deviceId ID of the device to be operated.
 * @param interfaceIndex Interface index, which corresponds to interface which supports USB Protocol ACM.
 * @param dev Device handle.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_MEMORY_ERROR} insufficient memory.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_DEVICE_NOT_FOUND} device or interface not found.
 * @since 16
 */
int32_t OH_UsbSerial_Open(uint64_t deviceId, uint8_t interfaceIndex, UsbSerial_Device **dev);

/**
 * @brief Close USB serial device.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1. dev is null.\n
 *         2. *dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_Close(UsbSerial_Device **dev);

/**
 * @brief Read bytesRead into buff from UsbSerial device.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param buff Received data from a serial device.
 * @param bufferSize The buffer size.
 * @param bytesRead Actual bytes read.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *         2.buff is null; 3.bufferSize is zero; 4.bytesRead is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_MEMORY_ERROR} the buff is outside accessible address space error.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_Read(UsbSerial_Device *dev, uint8_t *buff, uint32_t bufferSize, uint32_t *bytesRead);

/**
 * @brief Write bytesWritten from buff to UsbSerial device.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param buff Serial information write to device.
 * @param bufferSize The buffer size.
 * @param bytesWritten Actual bytes written.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *         2.buff is null; 3.bufferSize is zero; 4. bytesWritten is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_MEMORY_ERROR} the buff is outside accessible address space error.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_Write(UsbSerial_Device *dev, uint8_t *buff, uint32_t bufferSize, uint32_t *bytesWritten);

/**
 * @brief Set the serial port baud rate.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param baudRate Serial port baud rate set to connect device.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_SetBaudRate(UsbSerial_Device *dev, uint32_t baudRate);

/**
 * @brief Set the serial port parameters.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param params Serial port params set to connect device.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *         2.params is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_SetParams(UsbSerial_Device *dev, UsbSerial_Params *params);

/**
 * @brief Set the timeout in milliseconds.
 * The timeout value defaults to 0 without calling this function.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param timeout Set to -1 to infinite timeout, 0 to return immediately with any data,
 * or > 0 to wait for data for a specified number of milliseconds.
 * Timeout will be rounded to the nearest 100ms. Maximum value limited to 25500ms.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: 1.dev is null;\n
 *         2. timeout < -1 or timeout > 25500.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_SetTimeout(UsbSerial_Device *dev, int timeout);

/**
 * @brief Set the flow control.
 * It defaults to no flow control without calling this function.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @param flowControl {@link UsbSerial_FlowControl} flow control mode.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_SetFlowControl(UsbSerial_Device *dev, UsbSerial_FlowControl flowControl);

/**
 * @brief Flush the input and output buffers after finish writting.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_Flush(UsbSerial_Device *dev);

/**
 * @brief Flush the input buffer, and the data in the buffer will be cleared directly.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_FlushInput(UsbSerial_Device *dev);

/**
 * @brief Flush the output buffer, and the data in the buffer will be cleared directly.
 *
 * @permission ohos.permission.ACCESS_DDK_USB_SERIAL
 * @param dev Device handle.
 * @return {@link USB_SERIAL_DDK_SUCCESS} the operation is successful.
 *         {@link USB_SERIAL_DDK_NO_PERM} permission check failed.
 *         {@link USB_SERIAL_DDK_INVALID_PARAMETER} parameter check failed. Possible causes: dev is null.
 *         {@link USB_SERIAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link USB_SERIAL_DDK_SERVICE_ERROR} communication with the ddk service failed.
 *         {@link USB_SERIAL_DDK_IO_ERROR} the ddk I/O error.
 *         {@link USB_SERIAL_DDK_INVALID_OPERATION} invalid operation.
 * @since 16
 */
int32_t OH_UsbSerial_FlushOutput(UsbSerial_Device *dev);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // DDK_USB_SERIAL_API_H
/** @} */
