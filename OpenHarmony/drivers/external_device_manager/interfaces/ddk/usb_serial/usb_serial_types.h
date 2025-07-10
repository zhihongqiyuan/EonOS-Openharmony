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
 * @file usb_serial_types.h
 *
 * @brief Provides the enumerated variables, structures, and macros used in USB SERIAL DDK APIs.
 *
 * @kit DriverDevelopmentKit
 * @library libusb_serial.z.so
 * @syscap SystemCapability.Driver.UsbSerial.Extension
 * @since 16
 */

#ifndef DDK_USB_SERIAL_TYPES_H
#define DDK_USB_SERIAL_TYPES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Opaque usb serial device structure.
 *
 * @since 16
 */
typedef struct UsbSerial_Device UsbSerial_Device;

/**
 * @brief Defines Return codes for USB SERIAL DDK.
 *
 * @since 16
 */
typedef enum {
    /** @error Permission denied */
    USB_SERIAL_DDK_NO_PERM = 201,
    /** @error Invalid parameter */
    USB_SERIAL_DDK_INVALID_PARAMETER = 401,
    /** @error Operation successful */
    USB_SERIAL_DDK_SUCCESS = 31600000,
    /** @error Invalid operation */
    USB_SERIAL_DDK_INVALID_OPERATION = 31600001,
    /** @error Init operation */
    USB_SERIAL_DDK_INIT_ERROR = 31600002,
    /** @error Service Error operation */
    USB_SERIAL_DDK_SERVICE_ERROR = 31600003,
    /** @error Memory-related error, for example, insufficient memory, memory data copy failure,\n
     * or memory application failure.
     */
    USB_SERIAL_DDK_MEMORY_ERROR = 31600004,
    /** @error I/O Error */
    USB_SERIAL_DDK_IO_ERROR = 31600005,
    /** @error Device not found */
    USB_SERIAL_DDK_DEVICE_NOT_FOUND = 31600006,
} UsbSerial_DdkRetCode;

/**
 * @brief Defines USB Serial Port Params for USB SERIAL DDK.
 *
 * @since 16
 */
typedef struct UsbSerial_Params {
    /** The baud rate requested by the system */
    uint32_t baudRate;
    /** The number of data bits to transmit */
    uint8_t nDataBits;
    /** The number of half stop bits. */
    uint8_t nStopBits;
    /** The parity setting to use during communication */
    uint8_t parity;
} __attribute__((aligned(8))) UsbSerial_Params;

/**
 * @brief Defines flow control for USB SERIAL DDK.
 *
 * @since 16
 */
typedef enum {
    /** No flow control */
    USB_SERIAL_NO_FLOW_CONTROL = 0,
    /** Software flow control */
    USB_SERIAL_SOFTWARE_FLOW_CONTROL = 1,
    /** Hardware flow control */
    USB_SERIAL_HARDWARE_FLOW_CONTROL = 2,
} UsbSerial_FlowControl;

/**
 * @brief Defines parity for USB SERIAL DDK.
 *
 * @since 16
 */
typedef enum {
    /** No parity */
    USB_SERIAL_PARITY_NONE = 0,
    /** Odd parity */
    USB_SERIAL_PARITY_ODD = 1,
    /** Even parity */
    USB_SERIAL_PARITY_EVEN = 2,
} UsbSerial_Parity;

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // DDK_USB_SERIAL_TYPES_H
/** @} */
