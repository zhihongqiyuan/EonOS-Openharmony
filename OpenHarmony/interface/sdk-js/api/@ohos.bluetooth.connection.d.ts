/*
 * Copyright (C) 2023-2024 Huawei Device Co., Ltd.
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
 * @file
 * @kit ConnectivityKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type constant from './@ohos.bluetooth.constant';

/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace connection
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @since 10
 */
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace connection
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @atomicservice
 * @since 12
 */
/**
 * Provides methods to operate or manage Bluetooth.
 *
 * @namespace connection
 * @syscap SystemCapability.Communication.Bluetooth.Core
 * @crossplatform
 * @atomicservice
 * @since 13
 */
declare namespace connection {
  /**
   * Indicate the profile connection state.
   *
   * @typedef { constant.ProfileConnectionState } ProfileConnectionState
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Indicate the profile connection state.
   *
   * @typedef { constant.ProfileConnectionState } ProfileConnectionState
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  type ProfileConnectionState = constant.ProfileConnectionState;

  /**
   * Indicate the profile id.
   *
   * @typedef { constant.ProfileId } ProfileId
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Indicate the profile id.
   *
   * @typedef { constant.ProfileId } ProfileId
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  type ProfileId = constant.ProfileId;

  /**
   * Indicate the profile uuid.
   *
   * @typedef { constant.ProfileUuids } ProfileUuids
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  /**
   * Indicate the profile uuid.
   *
   * @typedef { constant.ProfileUuids } ProfileUuids
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  /**
   * Indicate the profile uuid.
   *
   * @typedef { constant.ProfileUuids } ProfileUuids
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  type ProfileUuids = constant.ProfileUuids;

  /**
   * Indicate the major class of a bluetooth device.
   *
   * @typedef { constant.MajorClass } MajorClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Indicate the major class of a bluetooth device.
   *
   * @typedef { constant.MajorClass } MajorClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  type MajorClass = constant.MajorClass;

  /**
   * Indicate the major minor class of a bluetooth device.
   *
   * @typedef { constant.MajorMinorClass } MajorMinorClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Indicate the major minor class of a bluetooth device.
   *
   * @typedef { constant.MajorMinorClass } MajorMinorClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  type MajorMinorClass = constant.MajorMinorClass;

  /**
   * Get the profile connection state of the current device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { ProfileId } [profileId] - Indicate the profile id. This is an optional parameter.
   *                                  With profileId, returns the current connection state of this profile, {@link ProfileConnectionState}.
   *                                  Without profileId, if any profile is connected, {@link ProfileConnectionState#STATE_CONNECTED} is returned.
   *                                  Otherwise, {@link ProfileConnectionState#STATE_DISCONNECTED} is returned.
   * @returns { ProfileConnectionState } Returns the connection state.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900004 - Profile not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Get the profile connection state of the current device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { ProfileId } [profileId] - Indicate the profile id. This is an optional parameter.
   *                                  With profileId, returns the current connection state of this profile, {@link ProfileConnectionState}.
   *                                  Without profileId, if any profile is connected, {@link ProfileConnectionState#STATE_CONNECTED} is returned.
   *                                  Otherwise, {@link ProfileConnectionState#STATE_DISCONNECTED} is returned.
   * @returns { ProfileConnectionState } Returns the connection state.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900004 - Profile not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function getProfileConnectionState(profileId?: ProfileId): ProfileConnectionState;

  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback of pairDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback of pairDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback of pairDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function pairDevice(deviceId: string, callback: AsyncCallback<void>): void;

  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Starts pairing with a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function pairDevice(deviceId: string): Promise<void>;

  /**
   * Starts pairing with a credible remote Bluetooth device with transport.
   * This interface does not trigger a dialog box and does not require user authorization.
   * Only specific system application can use this function.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { BluetoothTransport } transport - the transport of the remote device to pair.
   * @param { AsyncCallback<void> } callback - the callback of pairCredibleDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function pairCredibleDevice(deviceId: string, transport: BluetoothTransport, callback: AsyncCallback<void>): void;

  /**
   * Starts pairing with a credible remote Bluetooth device with transport.
   * This interface does not trigger a dialog box and does not require user authorization.
   * Only specific system application can use this function.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { BluetoothTransport } transport - the transport of the remote device to pair.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function pairCredibleDevice(deviceId: string, transport: BluetoothTransport): Promise<void>;

  /**
   * Remove a paired remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback of cancelPairedDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function cancelPairedDevice(deviceId: string, callback: AsyncCallback<void>): void;

  /**
   * Remove a paired remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function cancelPairedDevice(deviceId: string): Promise<void>;

  /**
   * Remove a pairing remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback of cancelPairingDevice.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function cancelPairingDevice(deviceId: string, callback: AsyncCallback<void>): void;

  /**
   * Remove a pairing remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function cancelPairingDevice(deviceId: string): Promise<void>;

  /**
   * Obtains the name of a peer Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { string } Returns the device name in character string format.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Obtains the name of a peer Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { string } Returns the device name in character string format.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Obtains the name of a peer Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { string } Returns the device name in character string format.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function getRemoteDeviceName(deviceId: string): string;

  /**
   * Obtains the name or alias of the Bluetooth peer device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { boolean } [alias] - Indicates whether to obtain the device alias. If the parameter is not provided, the device alias is obtained by default.
   * @returns { string } Returns the device name in character string format.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Failed to obtain the name or alias of the peer Bluetooth device.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 16
   */
  function getRemoteDeviceName(deviceId: string, alias?: boolean): string;

  /**
   * Obtains the class of a peer Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { DeviceClass } The class of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Obtains the class of a peer Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { DeviceClass } The class of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  /**
   * Obtains the class of a peer Bluetooth device.
   *
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { DeviceClass } The class of the remote device.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 18
   */
  function getRemoteDeviceClass(deviceId: string): DeviceClass;

  /**
   * Obtains the Bluetooth local name of a device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { string } Returns the name the device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Obtains the Bluetooth local name of a device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { string } Returns the name the device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function getLocalName(): string;

  /**
   * Obtains the list of Bluetooth devices that have been paired with the current device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { Array<string> } Returns a list of paired Bluetooth devices's address.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Obtains the list of Bluetooth devices that have been paired with the current device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { Array<string> } Returns a list of paired Bluetooth devices's address.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Obtains the list of Bluetooth devices that have been paired with the current device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { Array<string> } Returns a list of paired Bluetooth devices's address.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function getPairedDevices(): Array<string>;

  /**
   * Obtains the pair state of a specified device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { BondState } Returns the pair state.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 11
   */
  /**
   * Obtains the pair state of a specified device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { BondState } Returns the pair state.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Obtains the pair state of a specified device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { BondState } Returns the pair state.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function getPairState(deviceId: string): BondState;

  /**
   * Sets the confirmation of pairing with a certain device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { boolean } accept - Indicates whether to accept the pairing request, {@code true} indicates accept or {@code false} otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function setDevicePairingConfirmation(deviceId: string, accept: boolean): void;

  /**
   * Set the pin during pairing when the pin type is PIN_TYPE_ENTER_PIN_CODE.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { string } code - The pin code entered by the user.
   * @param { AsyncCallback<void> } callback - the callback of setDevicePinCode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function setDevicePinCode(deviceId: string, code: string, callback: AsyncCallback<void>): void;

  /**
   * Set the pin during pairing when the pin type is PIN_TYPE_ENTER_PIN_CODE.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { string } code - The pin code entered by the user.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function setDevicePinCode(deviceId: string, code: string): Promise<void>;

  /**
   * Sets the Bluetooth friendly name of a device. It is used only by system applications for security.
   * If a non-system application invokes the interface, exception 801 is thrown.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } name - Indicates a valid Bluetooth name.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   * @deprecated since 12
   */
  function setLocalName(name: string): void;

  /**
   * Sets the Bluetooth scan mode for a device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { ScanMode } mode - Indicates the Bluetooth scan mode to set.
   * @param { number } duration - Indicates the duration in seconds, in which the host is discoverable.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function setBluetoothScanMode(mode: ScanMode, duration: number): void;

  /**
   * Obtains the Bluetooth scanning mode of a device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { ScanMode } Returns the Bluetooth scanning mode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Obtains the Bluetooth scanning mode of a device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { ScanMode } Returns the Bluetooth scanning mode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function getBluetoothScanMode(): ScanMode;

  /**
   * Starts scanning Bluetooth devices.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Starts scanning Bluetooth devices.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Starts scanning Bluetooth devices.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function startBluetoothDiscovery(): void;

  /**
   * Stops Bluetooth device scanning.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Stops Bluetooth device scanning.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Stops Bluetooth device scanning.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function stopBluetoothDiscovery(): void;

  /**
   * Check if bluetooth is discovering.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { boolean } Returns {@code true} if the local device is discovering; returns {@code false} otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 11
   */
  /**
   * Check if bluetooth is discovering.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { boolean } Returns {@code true} if the local device is discovering; returns {@code false} otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function isBluetoothDiscovering(): boolean;

  /**
   * Obtains the profile UUIDs supported by the local device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { AsyncCallback<Array<ProfileUuids>> } callback - the callback of getLocalProfileUuids.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function getLocalProfileUuids(callback: AsyncCallback<Array<ProfileUuids>>): void;

  /**
   * Obtains the profile UUIDs supported by the local device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @returns { Promise<Array<ProfileUuids>> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  function getLocalProfileUuids(): Promise<Array<ProfileUuids>>;

  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<Array<ProfileUuids>> } callback - the callback of getRemoteProfileUuids.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<Array<ProfileUuids>> } callback - the callback of getRemoteProfileUuids.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<Array<ProfileUuids>> } callback - the callback of getRemoteProfileUuids.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function getRemoteProfileUuids(deviceId: string, callback: AsyncCallback<Array<ProfileUuids>>): void;

  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<Array<ProfileUuids>> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<Array<ProfileUuids>> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  /**
   * Obtains the profile UUIDs supported by the remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<Array<ProfileUuids>> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function getRemoteProfileUuids(deviceId: string): Promise<Array<ProfileUuids>>;

  /**
   * Connects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 11
   */
  /**
   * Connects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 16
   */
  function connectAllowedProfiles(deviceId: string, callback: AsyncCallback<void>): void;

  /**
   * Connects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 11
   */
  /**
   * Connects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 16
   */
  function connectAllowedProfiles(deviceId: string): Promise<void>;

  /**
   * Disconnects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { AsyncCallback<void> } callback - the callback result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 11
   */
  function disconnectAllowedProfiles(deviceId: string, callback: AsyncCallback<void>): void;

  /**
   * Get remote device battery information.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<BatteryInfo> } Returns battery info.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  function getRemoteDeviceBatteryInfo(deviceId: string): Promise<BatteryInfo>;

  /**
   * Obtains the product ID of a remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { string } Returns the remote device's product ID.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 11
   */
  /**
   * Obtains the product ID of a remote device.
   *
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { string } Returns the remote device's product ID.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 16
   */
  function getRemoteProductId(deviceId: string): string;

  /**
   * Disconnects all allowed bluetooth profiles between the local and remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 11
   */
  function disconnectAllowedProfiles(deviceId: string): Promise<void>;

  /**
   * Modify remote device name.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { string } name - New device name. Max length is 64 bytes.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
    function setRemoteDeviceName(deviceId: string, name: string): Promise<void>;

  /**
   * Set remote device custom type.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @param { DeviceType } type - Indicates device type.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  function setRemoteDeviceType(deviceId: string, type: DeviceType): Promise<void>;

  /**
   * Get remote device custom type.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<DeviceType> } Returns the device type.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  /**
   * Get remote device custom type.
   *
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<DeviceType> } Returns the device type.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 18
   */
  function getRemoteDeviceType(deviceId: string): Promise<DeviceType>;

  /**
   * Controls the actions of Bluetooth peripherals.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { ControlDeviceActionParams } controlDeviceActionParams - Indicates the action for Bluetooth peripherals.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  function controlDeviceAction(controlDeviceActionParams: ControlDeviceActionParams): Promise<void>;

  /**
   * Get latest connection time of device.
   *
   * @param { string } deviceId - Indicates device ID. For example, "11:22:33:AA:BB:FF".
   * @returns { Promise<number> } Returns latest connection time.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 15
   */
  function getLastConnectionTime(deviceId: string): Promise<number>;

  /**
   * update cloud devices.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.MANAGE_BLUETOOTH
   * @param { TrustedPairedDevices } trustedPairedDevices - Indicates the cloud devices.
   * @returns { Promise<void> } Returns the promise object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications are not allowed to use system APIs.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900001 - Service stopped.
   * @throws { BusinessError } 2900003 - Bluetooth disabled.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  function updateCloudBluetoothDevice(trustedPairedDevices: TrustedPairedDevices): Promise<void>;


  /**
   * Subscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Subscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Subscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function on(type: 'bluetoothDeviceFind', callback: Callback<Array<string>>): void;

  /**
   * Unsubscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Unsubscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Unsubscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bluetoothDeviceFind' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<string>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  function off(type: 'bluetoothDeviceFind', callback?: Callback<Array<string>>): void;

  /**
   * Subscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.GET_BLUETOOTH_PEERS_MAC
   * @param { 'discoveryResult' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<DiscoveryResult>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  /**
   * Subscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'discoveryResult' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<DiscoveryResult>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 18
   */
  function on(type: 'discoveryResult', callback: Callback<Array<DiscoveryResult>>): void;

  /**
   * Unsubscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH and ohos.permission.GET_BLUETOOTH_PEERS_MAC
   * @param { 'discoveryResult' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<DiscoveryResult>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  /**
   * Unsubscribe the event reported when a remote Bluetooth device is discovered.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'discoveryResult' } type - Type of the discovering event to listen for.
   * @param { Callback<Array<DiscoveryResult>> } callback - Callback used to listen for the discovering event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 18
   */
  function off(type: 'discoveryResult', callback?: Callback<Array<DiscoveryResult>>): void;

  /**
   * Subscribe the event reported when a remote Bluetooth device is bonded.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bondStateChange' } type - Type of the bond state event to listen for.
   * @param { Callback<BondStateParam> } callback - Callback used to listen for the bond state event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Subscribe the event reported when a remote Bluetooth device is bonded.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bondStateChange' } type - Type of the bond state event to listen for.
   * @param { Callback<BondStateParam> } callback - Callback used to listen for the bond state event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function on(type: 'bondStateChange', callback: Callback<BondStateParam>): void;

  /**
   * Unsubscribe the event reported when a remote Bluetooth device is bonded.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bondStateChange' } type - Type of the bond state event to listen for.
   * @param { Callback<BondStateParam> } callback - Callback used to listen for the bond state event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Unsubscribe the event reported when a remote Bluetooth device is bonded.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'bondStateChange' } type - Type of the bond state event to listen for.
   * @param { Callback<BondStateParam> } callback - Callback used to listen for the bond state event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  function off(type: 'bondStateChange', callback?: Callback<BondStateParam>): void;

  /**
   * Subscribe the event of a pairing request from a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'pinRequired' } type - Type of the pairing request event to listen for.
   * @param { Callback<PinRequiredParam> } callback - Callback used to listen for the pairing request event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function on(type: 'pinRequired', callback: Callback<PinRequiredParam>): void;

  /**
   * Unsubscribe the event of a pairing request from a remote Bluetooth device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'pinRequired' } type - Type of the pairing request event to listen for.
   * @param { Callback<PinRequiredParam> } callback - Callback used to listen for the pairing request event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  function off(type: 'pinRequired', callback?: Callback<PinRequiredParam>): void;

  /**
   * Subscribe the event of battery state changed from a remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'batteryChange' } type - Type of the battery event to listen for.
   * @param { Callback<BatteryInfo> } callback - Callback used to listen.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  function on(type: 'batteryChange', callback: Callback<BatteryInfo>): void;

  /**
   * Unsubscribe the event of battery state changed from a remote device.
   *
   * @permission ohos.permission.ACCESS_BLUETOOTH
   * @param { 'batteryChange' } type - Type of the battery event to listen for.
   * @param { Callback<BatteryInfo> } callback - Callback used to listen.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 2900099 - Operation failed.
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  function off(type: 'batteryChange', callback?: Callback<BatteryInfo>): void;

  /**
   * Describes the class of a bluetooth device.
   *
   * @typedef BondStateParam
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Describes the class of a bluetooth device.
   *
   * @typedef BondStateParam
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  interface BondStateParam {
    /**
     * Address of a Bluetooth device.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Address of a Bluetooth device.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    deviceId: string;
    /**
     * Profile connection state of the device.
     *
     * @type { BondState }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Profile connection state of the device.
     *
     * @type { BondState }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    state: BondState;
    /**
     * Cause of unbond.
     *
     * @type { UnbondCause }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    /**
     * Cause of unbond.
     *
     * @type { UnbondCause }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    cause: UnbondCause;
  }

  /**
   * Describes the bond key param.
   *
   * @typedef PinRequiredParam
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  interface PinRequiredParam {
    /**
     * ID of the device to pair.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    deviceId: string;
    /**
     * Key for the device pairing.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    pinCode: string;
    /**
     * Indicates the pairing type to a peer device.
     *
     * @type { PinType }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    pinType: PinType;
  }

  /**
   * Describes the class of a bluetooth device.
   *
   * @typedef DeviceClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * Describes the class of a bluetooth device.
   *
   * @typedef DeviceClass
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  interface DeviceClass {
    /**
     * Major classes of Bluetooth devices.
     *
     * @type { MajorClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Major classes of Bluetooth devices.
     *
     * @type { MajorClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    majorClass: MajorClass;
    /**
     * Major and minor classes of Bluetooth devices.
     *
     * @type { MajorMinorClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Major and minor classes of Bluetooth devices.
     *
     * @type { MajorMinorClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    majorMinorClass: MajorMinorClass;
    /**
     * Class of the device.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Class of the device.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    classOfDevice: number;
  }

  /**
   * Enum for the transport of a remote device
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  enum BluetoothTransport {
    /**
     * The value of bluetooth transport BR/EDR.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    TRANSPORT_BR_EDR = 0,
    /**
     * The value of bluetooth transport LE.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    TRANSPORT_LE = 1
  }

  /**
   * The enum of BR scan mode.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * The enum of BR scan mode.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  enum ScanMode {
    /**
     * Indicates the scan mode is none
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicates the scan mode is none
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    SCAN_MODE_NONE = 0,
    /**
     * Indicates the scan mode is connectable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicates the scan mode is connectable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    SCAN_MODE_CONNECTABLE = 1,
    /**
     * Indicates the scan mode is general discoverable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    SCAN_MODE_GENERAL_DISCOVERABLE = 2,
    /**
     * Indicates the scan mode is limited discoverable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    SCAN_MODE_LIMITED_DISCOVERABLE = 3,
    /**
     * Indicates the scan mode is connectable and general discoverable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicates the scan mode is connectable and general discoverable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    SCAN_MODE_CONNECTABLE_GENERAL_DISCOVERABLE = 4,
    /**
     * Indicates the scan mode is connectable and limited discoverable
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    SCAN_MODE_CONNECTABLE_LIMITED_DISCOVERABLE = 5
  }

  /**
   * The enum of bond state.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 10
   */
  /**
   * The enum of bond state.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @atomicservice
   * @since 12
   */
  /**
   * The enum of bond state.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @atomicservice
   * @since 13
   */
  enum BondState {
    /**
     * Indicate the bond state is invalid
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicate the bond state is invalid
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Indicate the bond state is invalid
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    BOND_STATE_INVALID = 0,
    /**
     * Indicate the bond state is bonding
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicate the bond state is bonding
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Indicate the bond state is bonding
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    BOND_STATE_BONDING = 1,
    /**
     * Indicate the bond state is bonded
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 10
     */
    /**
     * Indicate the bond state is bonded
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Indicate the bond state is bonded
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @atomicservice
     * @since 13
     */
    BOND_STATE_BONDED = 2
  }

  /**
   * Enum for the type of pairing to a remote device
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 10
   */
  enum PinType {
    /**
     * The user needs to enter the pin code displayed on the peer device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_ENTER_PIN_CODE = 0,
    /**
     * The user needs to enter the passkey displayed on the peer device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_ENTER_PASSKEY = 1,
    /**
     * The user needs to confirm the passkey displayed on the local device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_CONFIRM_PASSKEY = 2,
    /**
     * The user needs to accept or deny the pairing request.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_NO_PASSKEY_CONSENT = 3,
    /**
     * The user needs to enter the passkey displayed on the local device on the peer device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_NOTIFY_PASSKEY = 4,
    /**
     * The user needs to enter the pin code displayed on the peer device, used for bluetooth 2.0.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_DISPLAY_PIN_CODE = 5,
    /**
     * The user needs to accept or deny the OOB pairing request.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_OOB_CONSENT = 6,
    /**
     * The user needs to enter the 16-digit pin code displayed on the peer device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 10
     */
    PIN_TYPE_PIN_16_DIGITS = 7
  }

  /**
   * Describes the contents of the discovery results
   *
   * @typedef DiscoveryResult
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  /**
   * Describes the contents of the discovery results
   *
   * @typedef DiscoveryResult
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 18
   */
  interface DiscoveryResult {
    /**
     * Identify of the discovery device
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    /**
     * Identify of the discovery device
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 18
     */
    deviceId: string;
    /**
     * RSSI of the remote device
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    /**
     * RSSI of the remote device
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 18
     */
    rssi: number;
    /**
     * The local name of the device
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    /**
     * The local name of the device
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 18
     */
    deviceName: string;
    /**
     * The class of the device
     *
     * @type { DeviceClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    /**
     * The class of the device
     *
     * @type { DeviceClass }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 18
     */
    deviceClass: DeviceClass;
  }

  /**
   * Describes the contents of the battery information.
   *
   * @typedef BatteryInfo
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  interface BatteryInfo {
    /**
     * Identify of the discovery device.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    deviceId: string;
    /**
     * Electricity value of the general device. {@code -1} means no power information.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    batteryLevel: number;
    /**
     * Electricity value of the left ear. {@code -1} means no power information.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    leftEarBatteryLevel: number;
    /**
     * The charge state of the left ear.
     *
     * @type { DeviceChargeState }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    leftEarChargeState: DeviceChargeState;
    /**
     * Electricity value of the right ear. {@code -1} means no power information.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    rightEarBatteryLevel: number;
    /**
     * The charge state of the right ear.
     *
     * @type { DeviceChargeState }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    rightEarChargeState: DeviceChargeState;
    /**
     * Electricity value of the box. {@code -1} means no power information.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    boxBatteryLevel: number;
    /**
     * The charge state of the box.
     *
     * @type { DeviceChargeState }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    boxChargeState: DeviceChargeState;
  }

  /**
   * Enum for the charge state.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  enum DeviceChargeState {
    /**
     * Not support super charge, and not charged.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    DEVICE_NORMAL_CHARGE_NOT_CHARGED = 0,
    /**
     * Not support super charge, and in charging.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    DEVICE_NORMAL_CHARGE_IN_CHARGING = 1,
    /**
     * Support super charge, and not charged.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    DEVICE_SUPER_CHARGE_NOT_CHARGED = 2,
    /**
     * Support super charge, and in charging.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    DEVICE_SUPER_CHARGE_IN_CHARGING = 3
  }

  /**
   * Enum for the custom type of remote device.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 12
   */
  enum DeviceType {
    /**
     * Default type, the type is consistent with COD.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_DEFAULT = 0,
    /**
     * Car bluetooth.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_CAR = 1,
    /**
     * Headset bluetooth.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_HEADSET = 2,
    /**
     * Hearing Aid.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_HEARING = 3,
    /**
     * Glasses device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_GLASSES = 4,
    /**
     * Watch device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_WATCH = 5,
    /**
     * Speaker device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_SPEAKER = 6,
    /**
     * Others bluetooth.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 12
     */
    DEVICE_TYPE_OTHERS = 7
  }

  /**
   * Enum for cause of unbond.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @since 12
   */
  /**
   * Enum for cause of unbond.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @crossplatform
   * @since 13
   */
  enum UnbondCause {
    /**
     * User proactively removed device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    /**
     * User proactively removed device.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @crossplatform
     * @since 13
     */
    USER_REMOVED = 0,
    /**
     * Remote device shut down.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    REMOTE_DEVICE_DOWN = 1,
    /**
     * Wrong PIN code.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    AUTH_FAILURE = 2,
    /**
     * Remote device rejected.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    AUTH_REJECTED = 3,
    /**
     * Internal error.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @since 12
     */
    INTERNAL_ERROR = 4
  }
  /**
   * Describes information about controlling the Bluetooth peripheral.
   *
   * @typedef ControlDeviceActionParams
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  interface ControlDeviceActionParams {
    /**
     * Indicates the address of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    deviceId: string;
    /**
     * Indicates the control type.
     *
     * @type { ControlType }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    type: ControlType;
    /**
     * Indicates the control value.
     *
     * @type { ControlTypeValue }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    typeValue: ControlTypeValue;
    /**
     * Indicates the control object.
     *
     * @type { ControlObject }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    controlObject: ControlObject;
  }

  /**
   * Describes the control type.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  enum ControlType {
    /**
     * Indicates the control command of play.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    PLAY = 0,
    /**
     * Indicates the control command of vibration.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    VIBRATE = 1,
    /**
     * Indicates the control command of flash.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    FLASH = 2,
    /**
     * Indicates the control command of lock.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    LOCK = 3,
    /**
     * Indicates the control command of erase.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    ERASE = 4,
  }

  /**
   * Describes the control type value.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  enum ControlTypeValue {
    /**
     * Indicates the action of disable.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    DISABLE = 0,
    /**
     * Indicates the action of enable.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    ENABLE = 1,
    /**
     * Indicates the action of query.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    QUERY = 2,
  }

  /**
   * Describes the control object.
   *
   * @enum { number }
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  enum ControlObject {
    /**
     * Control object of left ear.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    LEFT_EAR = 0,
    /**
     * Control object of right ear.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    RIGHT_EAR = 1,
    /**
     * Control object of left and right ear.
     *
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    LEFT_RIGHT_EAR = 2,
  }

  /**
   * Describes the cloud pair device.
   *
   * @typedef TrustedPairedDevices
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  interface TrustedPairedDevices {
    /**
     * The list of cloud pair devices.
     *
     * @type { Array<TrustedPairedDevice> }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    trustedPairedDevices: Array<TrustedPairedDevice>;
  }

  /**
   * Describes device of cloud pair.
   *
   * @typedef TrustedPairedDevice
   * @syscap SystemCapability.Communication.Bluetooth.Core
   * @systemapi
   * @since 15
   */
  interface TrustedPairedDevice {
    /**
     * Indicates the device identify.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    sn: string;
    /**
     * Indicates the device type of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    deviceType: string;
    /**
     * Indicates the modelId of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    modelId: string;
    /**
     * Indicates the manufactory of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    manufactory: string;
    /**
     * Indicates the productId of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    productId: string;
    /**
     * Indicates the HiLink version of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    hiLinkVersion: string;
    /**
     * Indicates the macAddress of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    macAddress: string;
    /**
     * Indicates the service type of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    serviceType: string;
    /**
     * Indicates the service id of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    serviceId: string;
    /**
     * The local name of the device
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    deviceName: string;
    /**
     * Indicates the uuid of the peripheral.
     *
     * @type { string }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    uuids: string;
    /**
     * Indicates the bluetoothClass of the peripheral.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    bluetoothClass: number;
    /**
     * Indicates the token of the peripheral.
     *
     * @type { ArrayBuffer }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    token: ArrayBuffer;
    /**
     * Indicates the deviceNameTime of the peripheral.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    deviceNameTime: number;
    /**
     * Indicates the securityAdvInfo of the peripheral.
     *
     * @type { ArrayBuffer }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    secureAdvertisingInfo: ArrayBuffer;
    /**
     * Indicates the pairState of the peripheral.
     *
     * @type { number }
     * @syscap SystemCapability.Communication.Bluetooth.Core
     * @systemapi
     * @since 15
     */
    pairState: number;
  }
}
export default connection;