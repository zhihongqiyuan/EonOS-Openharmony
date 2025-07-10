/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
 * @kit DistributedServiceKit
 */

import { AsyncCallback } from './@ohos.base';
import { Callback } from './@ohos.base';

/**
 * Implements screen hopping operation management.
 *
 * @namespace cooperate
 * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
 * @systemapi Hide this for inner system use.
 * @since 10
 */
declare namespace cooperate {
  /**
   * Enumerates screen hopping message notifications.
   *
   * @enum { number }
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#CooperateState
   */
  enum CooperateMsg {
    /**
     * Preparing for screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_PREPARE
     */
    COOPERATE_PREPARE = 0,

    /**
     * Canceling the preparation for screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_UNPREPARE
     */
    COOPERATE_UNPREPARE = 1,

    /**
     * Starting screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_ACTIVATE
     */
    COOPERATE_ACTIVATE = 2,

    /**
     * Success in starting screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_ACTIVATE_SUCCESS
     */
    COOPERATE_ACTIVATE_SUCCESS = 3,

    /**
     * Failure to start screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_ACTIVATE_FAILURE
     */
    COOPERATE_ACTIVATE_FAIL = 4,

    /**
     * Success in stopping screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_DEACTIVATE_SUCCESS
     */
    COOPERATE_DEACTIVATE_SUCCESS = 5,

    /**
     * Failure to stop screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_DEACTIVATE_FAILURE
     */
    COOPERATE_DEACTIVATE_FAIL = 6,

    /**
     * Inter-device session disconnected.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 10
     * @deprecated since 11
     * @useinstead cooperate.CooperateState#COOPERATE_SESSION_DISCONNECTED
     */
    COOPERATE_SESSION_DISCONNECTED = 7,
  }

  /**
   * Enumerates the screen hopping states.
   *
   * @enum { number }
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  enum CooperateState {
    /**
     * Preparing for screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_PREPARE = 0,

    /**
     * Canceling the preparation for screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_UNPREPARE = 1,

    /**
     * Starting screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_ACTIVATE = 2,

    /**
     * Success in starting screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_ACTIVATE_SUCCESS = 3,

    /**
     * Failure to start screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_ACTIVATE_FAILURE = 4,

    /**
     * Success in stopping screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_DEACTIVATE_SUCCESS = 5,

    /**
     * Failure to stop screen hopping.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_DEACTIVATE_FAILURE = 6,

    /**
     * Inter-device session disconnected.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    COOPERATE_SESSION_DISCONNECTED = 7,
  }

  /**
   * Defines a screen hopping message.
   *
   * @interface CooperateMessage
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  interface CooperateMessage {
    /**
     * Descriptor of the target device for screen hopping.
     *
     * @type { string }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    networkId: string;

    /**
     * Screen hopping state.
     *
     * @type { CooperateState }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    state: CooperateState;
  }

  /**
   *  MouseLocation
   *
   * @typedef MouseLocation
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  interface MouseLocation {
    /**
     * The mouse pointer is located at the X coordinate on the screen.
     *
     * @type { number }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    displayX: number;

    /**
     * The mouse pointer is located at the Y coordinate on the screen.
     *
     * @type { number }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    displayY: number;

    /**
     * Screen width.
     *
     * @type { number }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    displayWidth: number;

    /**
     * Screen height.
     *
     * @type { number }
     * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    displayHeight: number;
  }

  /**
   * Prepares for screen hopping.
   *
   * @param { AsyncCallback<void> } callback Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#prepareCooperate
   */
  function prepare(callback: AsyncCallback<void>): void;

  /**
   * Prepares for screen hopping.
   *
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error.Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#prepareCooperate
   */
  function prepare(): Promise<void>;

  /**
   * Prepares for screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { AsyncCallback<void> } callback - Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function prepareCooperate(callback: AsyncCallback<void>): void;

  /**
   * Prepares for screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function prepareCooperate(): Promise<void>;

  /**
   * Cancels the preparation for screen hopping.
   *
   * @param { AsyncCallback<void> } callback Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#unprepareCooperate
   */
  function unprepare(callback: AsyncCallback<void>): void;

  /**
   * Cancels the preparation for screen hopping.
   *
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#unprepareCooperate
   */
  function unprepare(): Promise<void>;

  /**
   * Cancels the preparation for screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { AsyncCallback<void> } callback - Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function unprepareCooperate(callback: AsyncCallback<void>): void;

  /**
   * Cancels the preparation for screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function unprepareCooperate(): Promise<void>;

  /**
   * Starts screen hopping.
   *
   * @param { string } targetNetworkId Descriptor of the target device for screen hopping.
   * @param { number } inputDeviceId Identifier of the input device for screen hopping.
   * @param { AsyncCallback<void> } callback Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @throws {BusinessError} 20900001 - Service exception. Possible causes: 1. A system error, such as null pointer,
   * <br>container-related exception, or IPC exception. 2. N-API invocation exception or invalid N-API status.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#activateCooperate
   */
  function activate(targetNetworkId: string, inputDeviceId: number, callback: AsyncCallback<void>): void;

  /**
   * Starts screen hopping.
   *
   * @param { string } targetNetworkId Descriptor of the target device for screen hopping.
   * @param { number }inputDeviceId Identifier of the input device for screen hopping.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @throws {BusinessError} 20900001 - Service exception. Possible causes: 1. A system error, such as null pointer,
   * <br>container-related exception, or IPC exception. 2. N-API invocation exception or invalid N-API status.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#activateCooperate
   */
  function activate(targetNetworkId: string, inputDeviceId: number): Promise<void>;

  /**
   * Starts screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { string } targetNetworkId - Descriptor of the target device for screen hopping.
   * @param { number } inputDeviceId - Identifier of the input device for screen hopping.
   * @param { AsyncCallback<void> } callback - Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @throws {BusinessError} 20900001 - Service exception. Possible causes: 1. A system error, such as null pointer,
   * <br>container-related exception, or IPC exception. 2. N-API invocation exception or invalid N-API status.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function activateCooperate(targetNetworkId: string, inputDeviceId: number, callback: AsyncCallback<void>): void;

  /**
   * Starts screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { string } targetNetworkId - Descriptor of the target device for screen hopping.
   * @param { number }inputDeviceId - Identifier of the input device for screen hopping.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @throws {BusinessError} 20900001 - Service exception. Possible causes: 1. A system error, such as null pointer,
   * <br>container-related exception, or IPC exception. 2. N-API invocation exception or invalid N-API status.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function activateCooperate(targetNetworkId: string, inputDeviceId: number): Promise<void>;

  /**
   * Stops screen hopping.
   *
   * @param { boolean } isUnchained Whether the cross-device link is unchained.
   * @param { AsyncCallback<void> } callback Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#deactivateCooperate
   */
  function deactivate(isUnchained: boolean, callback: AsyncCallback<void>): void;

  /**
   * Stops screen hopping.
   *
   * @param { boolean } isUnchained Whether the cross-device link is unchained.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#deactivateCooperate
   */
  function deactivate(isUnchained: boolean): Promise<void>;

  /**
   * Stops screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { boolean } isUnchained - Whether the cross-device link is unchained.
   * @param { AsyncCallback<void> } callback - Asynchronous callback used to return the operation result.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function deactivateCooperate(isUnchained: boolean, callback: AsyncCallback<void>): void;

  /**
   * Stops screen hopping.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { boolean } isUnchained - Whether the cross-device link is unchained.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function deactivateCooperate(isUnchained: boolean): Promise<void>;

  /**
   * Obtains the screen hopping status.
   *
   * @param { string } networkId Descriptor of the target device for screen hopping.
   * @param { AsyncCallback<boolean> } callback Asynchronous callback used to return the screen hopping status.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#getCooperateSwitchState
   */
  function getCrossingSwitchState(networkId: string, callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the status of the screen hopping switch.
   *
   * @param { string } networkId Descriptor of the target device for screen hopping.
   * @returns { Promise<boolean> } Returns {@code true} the screen hopping status;
   *                               returns {@code false} otherwise.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#getCooperateSwitchState
   */
  function getCrossingSwitchState(networkId: string): Promise<boolean>;


  /**
   * Obtains the screen hopping status.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { string } networkId - Descriptor of the target device for screen hopping.
   * @param { AsyncCallback<boolean> } callback - Asynchronous callback used to return the screen hopping status.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function getCooperateSwitchState(networkId: string, callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the status of the screen hopping switch.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { string } networkId - Descriptor of the target device for screen hopping.
   * @returns { Promise<boolean> } Returns {@code true} the screen hopping status;
   *                               returns {@code false} otherwise.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function getCooperateSwitchState(networkId: string): Promise<boolean>;

  /**
   * Enables listening for screen hopping status change events.
   *
   * @param { 'cooperate' } type Change type.
   * @param { Callback<{ networkId: string, msg: CooperateMsg }> } callback Asynchronous callback used to
   * <br>return the screen hopping status change event.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#on
   */
  function on(type: 'cooperate', callback: Callback<{ networkId: string, msg: CooperateMsg }>): void;

  /**
   * Disables listening for screen hopping status change events.
   *
   * @param { 'cooperate' } type Change type.
   * @param { Callback<void> } callback Callback for which listening
   * <br>is disabled. If this parameter is not specified, listening will be disabled for all registered callbacks.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 10
   * @deprecated since 11
   * @useinstead cooperate#on
   */
  function off(type: 'cooperate', callback?: Callback<void>): void;

  /**
   * Enables listening for screen hopping status change events.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { 'cooperateMessage' } type - Change type.
   * @param { Callback<CooperateMessage> } callback - Asynchronous callback used to
   * <br>return the screen hopping status change event.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function on(type: 'cooperateMessage', callback: Callback<CooperateMessage>): void;

  /**
   * Disables listening for screen hopping status change events.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { 'cooperateMessage' } type - Change type.
   * @param { Callback<CooperateMessage> } [callback] - Callback for which listening
   * <br>is disabled. If this parameter is not specified, listening will be disabled for all registered callbacks.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes:1.Incorrect parameter types.2.Parameter 
   * <br>verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function off(type: 'cooperateMessage', callback?: Callback<CooperateMessage>): void;

  /**
   * Enables listening for mouse pointer position information on the specified device.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { 'cooperateMouse' } type - Event type, which is **cooperateMouse**.
   * @param { string } networkId - Specified device.
   * @param { Callback<MouseLocation> } callback - Callback for receiving reported events.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function on(type: 'cooperateMouse', networkId: string, callback: Callback<MouseLocation>): void;

 /**
   * Disables listening for mouse pointer position information on the specified device.
   *
   * @permission ohos.permission.COOPERATE_MANAGER
   * @param { 'cooperateMouse' } type - Event type, which is **cooperateMouse**.
   * @param { string } networkId - Specified device.
   * @param { Callback<MouseLocation> } [callback] - Callback for receiving reported events.
   * <br>If no callback is specified, listening will be disabled for all **cooperateMouse** events of the device specified by **networkId**.
   * @throws {BusinessError} 201 - Permission denied.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Cooperate
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function off(type: 'cooperateMouse', networkId: string, callback?: Callback<MouseLocation>): void;

}

export default cooperate;
