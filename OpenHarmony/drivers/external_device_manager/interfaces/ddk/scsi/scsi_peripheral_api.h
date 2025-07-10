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
#ifndef SCSI_PERIPHERAL_API_H
#define SCSI_PERIPHERAL_API_H

/**
 * @addtogroup ScsiPeripheralDDK
 * @{
 *
 * @brief Provide ScsiPeripheral DDK interface, including initializing DDK, releasing DDK, opening devices,\n
 * reading and writing devices, etc.
 *
 * @syscap SystemCapability.Driver.SCSI.Extension
 * @since 16
 */

/**
 * @file scsi_peripheral_api.h
 *
 * @brief Declares the ScsiPeripheral DDK APIs.
 *
 * @since 16
 */

#include <stdint.h>

#include "scsi_peripheral_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Initializes the ScsiPeripheral DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk init error.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Init(void);

/**
 * @brief Releases the ScsiPeripheral DDK.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Release(void);

/**
 * @brief Open SCSI device by deviceId.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param deviceId ID of the device to be operated.
 * @param interfaceIndex Interface index, which corresponds to interface which supports USB Protocol UAS.
 * @param dev Device handle.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_DEVICE_NOT_FOUND} device not found by deviceId.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Open(uint64_t deviceId, uint8_t interfaceIndex, ScsiPeripheral_Device **dev);

/**
 * @brief Close SCSI device.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Close(ScsiPeripheral_Device **dev);

/**
 * @brief Check if the logical unit is ready.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request Test unit ready request information.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_TestUnitReady(ScsiPeripheral_Device *dev, ScsiPeripheral_TestUnitReadyRequest *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Get the information regarding the logical unit and SCSI target device.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request Inquiry request information.
 * @param inquiryInfo The data of inquiry command.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or inquiryInfo is null or\n
 *             inquiryInfo->data is null or response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Inquiry(ScsiPeripheral_Device *dev, ScsiPeripheral_InquiryRequest *request,
    ScsiPeripheral_InquiryInfo *inquiryInfo, ScsiPeripheral_Response *response);

/**
 * @brief Get the device capacity.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request ReadCapacity request information.
 * @param capacityInfo The data of read capacity command.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or capacityInfo is null or\n
 *             response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_ReadCapacity10(ScsiPeripheral_Device *dev, ScsiPeripheral_ReadCapacityRequest *request,
    ScsiPeripheral_CapacityInfo *capacityInfo, ScsiPeripheral_Response *response);

/**
 * @brief Get the sense data.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request RequestSense request information.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_RequestSense(ScsiPeripheral_Device *dev, ScsiPeripheral_RequestSenseRequest *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Read from the specified logical block(s).
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request The request parameters.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or request->data is null or\n
 *             response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Read10(ScsiPeripheral_Device *dev, ScsiPeripheral_IORequest *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Write data to the specified logical block(s).
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request The request parameters.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or request->data is null or\n
 *             response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Write10(ScsiPeripheral_Device *dev, ScsiPeripheral_IORequest *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Verify the specified logical block(s) on the medium.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request Verify request information.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_Verify10(ScsiPeripheral_Device *dev, ScsiPeripheral_VerifyRequest *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Send SCSI command that specified by CDB.
 *
 * @permission ohos.permission.ACCESS_DDK_SCSI_PERIPHERAL
 * @param dev Device handle.
 * @param request The request parameters.
 * @param response The response parameters.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_NO_PERM} permission check failed.
 *         {@link SCSIPERIPHERAL_DDK_INIT_ERROR} the ddk not init.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or request is null or request->data is null or\n
 *             request->cdbLength is 0 or response is null.
 *         {@link SCSIPERIPHERAL_DDK_SERVICE_ERROR} communication with ddk service failed.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 *         {@link SCSIPERIPHERAL_DDK_IO_ERROR} i/o operation error.
 *         {@link SCSIPERIPHERAL_DDK_TIMEOUT} transmission timeout.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_OPERATION} this operation is not supported.
 * @since 16
 */
int32_t OH_ScsiPeripheral_SendRequestByCdb(ScsiPeripheral_Device *dev, ScsiPeripheral_Request *request,
    ScsiPeripheral_Response *response);

/**
 * @brief Creates a buffer. To avoid resource leakage, destroy a buffer by calling\n
 * <b>OH_ScsiPeripheral_DestroyDeviceMemMap</b> after use.
 *
 * @param dev Device handle.
 * @param size Buffer size.
 * @param devMmap Data memory map, through which the created buffer is returned to the caller.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} dev is null or devMmap is null.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 * @since 16
 */
int32_t OH_ScsiPeripheral_CreateDeviceMemMap(ScsiPeripheral_Device *dev, size_t size,
    ScsiPeripheral_DeviceMemMap **devMmap);

/**
 * @brief Destroys a buffer. To avoid resource leakage, destroy a buffer in time after use.
 *
 * @param devMmap Device memory map created by calling <b>OH_ScsiPeripheral_CreateDeviceMemMap</b>.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} devMmap is null.
 *         {@link SCSIPERIPHERAL_DDK_MEMORY_ERROR} memory data operation failed.
 * @since 16
 */
int32_t OH_ScsiPeripheral_DestroyDeviceMemMap(ScsiPeripheral_DeviceMemMap *devMmap);

/**
 * @brief Parse the basic sense data of Information、Command-specific information、Sense key specific.
 *
 * @param senseData Sense data.
 * @param senseDataLen The length of sense data.
 * @param senseInfo Basic sense data.
 * @return {@link SCSIPERIPHERAL_DDK_SUCCESS} the operation is successful.
 *         {@link SCSIPERIPHERAL_DDK_INVALID_PARAMETER} senseData is null or senseInfo is null or\n
 *             senseData format is not Descriptor/Fixed format or\n
 *             senseDataLen is smaller than SCSIPERIPHERAL_MIN_DESCRIPTOR_FORMAT_SENSE or\n
 *             senseDataLen is smaller than SCSIPERIPHERAL_MIN_FIXED_FORMAT_SENSE.
 * @since 16
 */
int32_t OH_ScsiPeripheral_ParseBasicSenseInfo(uint8_t *senseData, uint8_t senseDataLen,
    ScsiPeripheral_BasicSenseInfo *senseInfo);

/** @} */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // SCSI_PERIPHERAL_API_H