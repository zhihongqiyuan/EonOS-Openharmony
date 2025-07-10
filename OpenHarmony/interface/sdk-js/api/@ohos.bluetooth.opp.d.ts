/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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
 * @file
 * @kit ConnectivityKit
 */

import type { Callback } from './@ohos.base';

/**
 * Provides methods to accessing bluetooth OPP(OBEX OBJECT PUSH Profile)-related capabilities.
 *
 * @namespace opp
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @since 16
 */
declare namespace opp {
  /**
   * create the instance of OPP server profile.
   *
   * @returns { OppServerProfile } Returns the instance of opp profile.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 801 - Capability not supported.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  function createOppServerProfile(): OppServerProfile;

  /**
   * Manager OPP server profile.
   *
   * @typedef OppServerProfile
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 16
   */
  interface OppServerProfile {
    /**
     * Subscribe the event reported when the file transfer status changes.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { 'transferStateChange' } type - Type of transport state change event to listen for.
     * @param { Callback<OppTransferInformation> } callback - Callback used to listen for event.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth disabled.
     * @throws { BusinessError } 2900004 - Profile not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    on(type: 'transferStateChange', callback: Callback<OppTransferInformation>): void;

    /**
     * Unsubscribe the event reported when the file transfer status changes.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { 'transferStateChange' } type - Type of transport state change event to listen for.
     * @param { Callback<OppTransferInformation> } callback - Callback used to listen for event.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth disabled.
     * @throws { BusinessError } 2900004 - Profile not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    off(type: 'transferStateChange', callback?: Callback<OppTransferInformation>): void;

    /**
     * Subscribe to the event of receiving a file transfer request.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { 'receiveIncomingFile' } type - Type of the event for receiving a file request to listen for.
     * @param { Callback<OppTransferInformation> } callback - Callback used to listen for event.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth disabled.
     * @throws { BusinessError } 2900004 - Profile not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    on(type: 'receiveIncomingFile', callback: Callback<OppTransferInformation>): void;

    /**
     * Unsubscribe to the event of receiving a file transfer request.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { 'receiveIncomingFile' } type - Type of the event for receiving a file request to listen for.
     * @param { Callback<OppTransferInformation> } callback - Callback used to listen for event.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth disabled.
     * @throws { BusinessError } 2900004 - Profile not supported.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    off(type: 'receiveIncomingFile', callback?: Callback<OppTransferInformation>): void;
  
    /**
     * Send files to the remote device.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
     * @param { Array<FileHolder> } fileHolds - Indicates the information about files to be sent.
     * @returns { Promise<void> } Returns the promise object.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth switch is off.
     * @throws { BusinessError } 2900004 - Profile is not supported.
     * @throws { BusinessError } 2900099 - Failed to send file.
     * @throws { BusinessError } 2903001 - The file type is not supported.
     * @throws { BusinessError } 2903002 - Current Transfer Information is busy.
     * @throws { BusinessError } 2903003 - The file is not accessible.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    sendFile(deviceId: string, fileHolds: Array<FileHolder>): Promise<void>;

    /**
     * Set the user confirmation information for incoming files.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { boolean } accept - Indicates whether to accept the incoming file, {@code true} indicates accept or {@code false} otherwise.
     * @param { number } fileFd : the receive file fd to receive need contains open when accepting.
     * @returns { Promise<void> } Returns the promise object.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth switch is off.
     * @throws { BusinessError } 2900004 - Profile is not supported.
     * @throws { BusinessError } 2900099 - Failed to confirm the received file information.
     * @throws { BusinessError } 2903002 - Current Transfer Information is busy.
     * @throws { BusinessError } 2903003 - The file is not accessible.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    setIncomingFileConfirmation(accept: boolean, fileFd: number): Promise<void>;

    /**
     * cancel the current file transfer action.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @returns { Promise<void> } Returns the promise object.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth switch is off.
     * @throws { BusinessError } 2900004 - Profile is not supported.
     * @throws { BusinessError } 2900099 - Failed to cancel the current transfer.
     * @throws { BusinessError } 2903002 - Current Transfer Information is busy.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    cancelTransfer(): Promise<void>;

    /**
     * Obtains the information about the file that is being transferred.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @returns { Promise<OppTransferInformation> } Returns the promise object.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth switch is off.
     * @throws { BusinessError } 2900004 - Profile is not supported.
     * @throws { BusinessError } 2900099 - Failed to obtain the current transmission information.
     * @throws { BusinessError } 2903004 - Current Transfer Information is empty.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    getCurrentTransferInformation(): Promise<OppTransferInformation>;

    /**
     * Set the URI of the last received file.
     *
     * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
     * @param { string } uri - the uri of last received file.
     * @returns { Promise<void> } Returns the promise object.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
     * @throws { BusinessError } 203 - This function is prohibited by enterprise management policies.
     * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 2900001 - Service stopped.
     * @throws { BusinessError } 2900003 - Bluetooth disabled.
     * @throws { BusinessError } 2900004 - Profile not supported.
     * @throws { BusinessError } 2900099 - Failed to set the URI of the last file.
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    setLastReceivedFileUri(uri: string): Promise<void>;
  }

  /**
   * Enum for file transfer direction.
   * 
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  enum DirectionType {
    /**
     * The file transfer direction to send.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    OUTBOUND = 0,

    /**
     * The file transfer direction to receive.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    INBOUND = 1
  }

  /**
   * Enum for the file transfer status.
   * 
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  enum TransferStatus {
    /**
     * The file is waiting next operate.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    PENDING = 0,

    /**
     * The file is transfering.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    RUNNING = 1,

    /**
     * The file is transfer finished.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    FINISH = 2
  }

  /**
   * Enum for the file transfer result.
   * 
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  enum TransferResult {
    /**
     * Success
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    SUCCESS = 0,

    /**
     * The transfer file type is not supported.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_UNSUPPORTED_TYPE = 1,

    /**
     * The peer device cannot process the request.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_BAD_REQUEST = 2,

    /**
     * The peer device refuses to receive the file.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_NOT_ACCEPTABLE = 3,


    /**
     * The peer device cancels the ongoing file transfer process.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_CANCELED = 4,

    /**
     * Failed to connect to the peer device.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_CONNECTION_FAILED = 5,

    /**
     * Indicates that the file transfer fails.
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_TRANSFER_FAILED = 6,

    /**
     * Unknown error
     * 
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    ERROR_UNKNOWN = 7
  }

  /**
   * Describes the transferred file information.
   *
   * @typedef OppTransferInformation
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  interface OppTransferInformation {
    /**
     * Path of the file to be transferred.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    filePath: string;

    /**
     * Device name of the peer transmission object
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    remoteDeviceName: string;

    /**
     * Device Address of the peer transmission object
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    remoteDeviceId: string;

    /**
     * File Transfer Direction
     *
     * @type { DirectionType }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    direction: DirectionType;

    /**
     * File transfer status
     *
     * @type { TransferStatus }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    status: TransferStatus;

    /**
     * File transfer result
     *
     * @type { TransferResult }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    result: TransferResult;

    /**
     * Number of bytes of the file that have been transferred currently
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    currentBytes: number;

    /**
     * Total number of file bytes to transfer
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    totalBytes: number;

    /**
     * Number of files currently transferred
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    currentCount: number;

    /**
     * Total number of transferred files
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    totalCount: number;
  }

  /**
   * Describes the file info for transfer
   *
   * @typedef FileHolder
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  interface FileHolder {
    /**
     * The file path
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    filePath: string;

    /**
     * The file size
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    fileSize: number;

    /**
     * The file fd
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 16
     */
    fileFd: number;
  }
}
export default opp;
