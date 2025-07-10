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

#ifndef SCSI_PERIPHERAL_TYPES_H
#define SCSI_PERIPHERAL_TYPES_H
/**
 * @addtogroup ScsiPeripheralDDK
 * @{
 *
 * @brief Provides ScsiPeripheral DDK types and declares macros, enumerations, and\n
 * data structures used by the ScsiPeripheral DDK.
 *
 * @syscap SystemCapability.Driver.SCSI.Extension
 * @since 16
 */

/**
 * @file scsi_peripheral_types.h
 *
 * @brief Provides the enums, structs, and macros used in SCSI Peripheral DDK APIs.
 *
 * @since 16
 */

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief The min length of descriptor format sense data: 8.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_MIN_DESCRIPTOR_FORMAT_SENSE 8

/**
 * @brief The min length of fixed format sense data: 18.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_MIN_FIXED_FORMAT_SENSE 18

/**
 * @brief Defines error codes for SCSI DDK.
 *
 * @since 16
 */
typedef enum {
    /** @error Permission denied. */
    SCSIPERIPHERAL_DDK_NO_PERM = 201,
    /** @error Invalid parameter. */
    SCSIPERIPHERAL_DDK_INVALID_PARAMETER = 401,
    /** @error The operation is successful. */
    SCSIPERIPHERAL_DDK_SUCCESS = 31700000,
    /** @error Memory-related error, for example, insufficient memory, memory data copy failure,\n
     * memory data init failure, or memory map failure.
     */
    SCSIPERIPHERAL_DDK_MEMORY_ERROR = 31700001,
    /** @error Invalid operation. */
    SCSIPERIPHERAL_DDK_INVALID_OPERATION = 31700002,
    /** @error Device I/O operation failed. */
    SCSIPERIPHERAL_DDK_IO_ERROR = 31700003,
    /** @error Transmission timeout. */
    SCSIPERIPHERAL_DDK_TIMEOUT = 31700004,
    /** @error The ddk init error or the ddk not init. */
    SCSIPERIPHERAL_DDK_INIT_ERROR = 31700005,
    /** @error Communication with the SCSI ddk service failed. */
    SCSIPERIPHERAL_DDK_SERVICE_ERROR = 31700006,
    /** @error Device not found. */
    SCSIPERIPHERAL_DDK_DEVICE_NOT_FOUND = 31700007,
} ScsiPeripheral_DdkErrCode;

/**
 * @brief Defines SCSI status for response.
 *
 * @since 16
 */
typedef enum {
    /** Good condition */
    SCSIPERIPHERAL_STATUS_GOOD = 0x00,
    /** Check condition needed */
    SCSIPERIPHERAL_STATUS_CHECK_CONDITION_NEEDED = 0x02,
    /** Condition met */
    SCSIPERIPHERAL_STATUS_CONDITION_MET = 0x04,
    /** Busy */
    SCSIPERIPHERAL_STATUS_BUSY = 0x08,
    /** Reservation conflict */
    SCSIPERIPHERAL_STATUS_RESERVATION_CONFLICT = 0x18,
    /** Task set full */
    SCSIPERIPHERAL_STATUS_TASK_SET_FULL = 0x28,
    /** ACA active */
    SCSIPERIPHERAL_STATUS_ACA_ACTIVE = 0x30,
    /** Task aborted */
    SCSIPERIPHERAL_STATUS_TASK_ABORTED = 0x40,
} ScsiPeripheral_Status;

/**
 * @brief Opaque SCSI device structure.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_Device ScsiPeripheral_Device;

/**
 * @brief Device memory map created by calling <b>OH_ScsiPeripheral_CreateDeviceMemMap</b>.\n
 * A buffer using the device memory map can provide better performance.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_DeviceMemMap {
    /** Buffer address. */
    uint8_t * const address;
    /** Buffer size. */
    const size_t size;
    /** Offset of the used buffer. The default value is 0, indicating that there is no offset\n
     * and the buffer starts from the specified address.
     */
    uint32_t offset;
    /** Length of the used buffer. By default, the value is equal to the size, indicating that\n
     * the entire buffer is used.
     */
    uint32_t bufferLength;
    /** Length of the transferred data. */
    uint32_t transferredLength;
} ScsiPeripheral_DeviceMemMap;

/**
 * @brief Request parameters for read/write.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_IORequest {
    /** Starting with the logical block. */
    uint32_t lbAddress;
    /** Number of contiguous logical blocks that shall be read. */
    uint16_t transferLength;
    /** Control byte. */
    uint8_t control;
    /** Byte 1 of the CDB. */
    uint8_t byte1;
    /** Byte 6 of the CDB. */
    uint8_t byte6;
    /** Buffer of data transfer. */
    ScsiPeripheral_DeviceMemMap *data;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_IORequest;

/**
 * @brief The max length of command descriptor block: 16.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_MAX_CMD_DESC_BLOCK_LEN 16

/**
 * @brief Request parameters.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_Request {
    /** Command descriptor block. */
    uint8_t commandDescriptorBlock[SCSIPERIPHERAL_MAX_CMD_DESC_BLOCK_LEN];
    /** The length of command descriptor block. */
    uint8_t cdbLength;
    /** Data transfer direction. */
    int8_t dataTransferDirection;
    /** Buffer of data transfer. */
    ScsiPeripheral_DeviceMemMap *data;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_Request;

/**
 * @brief The max length of sense data: 252.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_MAX_SENSE_DATA_LEN 252

/**
 * @brief Response parameters.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_Response {
    /** Sense data. */
    uint8_t senseData[SCSIPERIPHERAL_MAX_SENSE_DATA_LEN];
    /** The status at completion of the call, such as good, busy, or timeout. */
    ScsiPeripheral_Status status;
    /** Shifted, masked scsi status. */
    uint8_t maskedStatus;
    /** Messaging level data (optional). */
    uint8_t msgStatus;
    /** Byte count actually written to sbp. */
    uint8_t sbLenWr;
    /** Errors from host adapter. */
    uint16_t hostStatus;
    /** Errors from software driver. */
    uint16_t driverStatus;
    /** Dxfer_len - actual_transferred. */
    int32_t resId;
    /** Time taken by cmd (unit: millisec). */
    uint32_t duration;
} ScsiPeripheral_Response;

/**
 * @brief SCSI test unit ready request.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_TestUnitReadyRequest {
    /** Control byte. */
    uint8_t control;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_TestUnitReadyRequest;

/**
 * @brief SCSI inquiry request.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_InquiryRequest {
    /** Page code. */
    uint8_t pageCode;
    /** Allocation length. */
    uint16_t allocationLength;
    /** Control byte. */
    uint8_t control;
    /** Byte 1 of the CDB. */
    uint8_t byte1;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_InquiryRequest;

/**
 * @brief The length of vendor identification: 8.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_VENDOR_ID_LEN 8

/**
 * @brief The length of product identification: 16.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_PRODUCT_ID_LEN 16

/**
 * @brief The length of product revision: 4.
 *
 * @since 16
 */
#define SCSIPERIPHERAL_PRODUCT_REV_LEN 4

/**
 * @brief SCSI inquiry data.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_InquiryInfo {
    /** Peripheral device type. */
    uint8_t deviceType;
    /** Vendor identification. */
    char idVendor[SCSIPERIPHERAL_VENDOR_ID_LEN + 1];
    /** Product identification. */
    char idProduct[SCSIPERIPHERAL_PRODUCT_ID_LEN + 1];
    /** Product revision. */
    char revProduct[SCSIPERIPHERAL_PRODUCT_REV_LEN + 1];
    /** All inquiry data. */
    ScsiPeripheral_DeviceMemMap *data;
} ScsiPeripheral_InquiryInfo;

/**
 * @brief SCSI read capacity request.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_ReadCapacityRequest {
    /** Logical block address. */
    uint32_t lbAddress;
    /** Control byte. */
    uint8_t control;
    /** Byte 8 of the CDB. */
    uint8_t byte8;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_ReadCapacityRequest;

/**
 * @brief SCSI read capacity data.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_CapacityInfo {
    /** Returned logical block address. */
    uint32_t lbAddress;
    /** Logical block length in bytes. */
    uint32_t lbLength;
} ScsiPeripheral_CapacityInfo;

/**
 * @brief SCSI request sense request.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_RequestSenseRequest {
    /** Allocation length. */
    uint8_t allocationLength;
    /** Control byte. */
    uint8_t control;
    /** Byte 1 of the CDB. */
    uint8_t byte1;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_RequestSenseRequest;

/**
 * @brief Basic sense data of Information、Command-specific information、Sense key specific.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_BasicSenseInfo {
    /** Response code. */
    uint8_t responseCode;
    /** Information valid bit. */
    bool valid;
    /** Information sense data descriptor. */
    uint64_t information;
    /** Command-specific information sense data descriptor. */
    uint64_t commandSpecific;
    /** Sense key specific valid bit. */
    bool sksv;
    /** Sense key specific sense data descriptor. */
    uint32_t senseKeySpecific;
} ScsiPeripheral_BasicSenseInfo;

/**
 * @brief SCSI verify request.
 *
 * @since 16
 */
typedef struct ScsiPeripheral_VerifyRequest {
    /** Starting with the logical block. */
    uint32_t lbAddress;
    /** Number of contiguous logical blocks that shall be verify. */
    uint16_t verificationLength;
    /** Control byte. */
    uint8_t control;
    /** Byte 1 of the CDB. */
    uint8_t byte1;
    /** Byte 6 of the CDB. */
    uint8_t byte6;
    /** Timeout(unit: millisec). */
    uint32_t timeout;
} ScsiPeripheral_VerifyRequest;
#ifdef __cplusplus
}
/** @} */
#endif /* __cplusplus */
#endif // SCSI_PERIPHERAL_TYPES_H