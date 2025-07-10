/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit AbilityKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type { ContinuableInfo as _ContinuableInfo } from './application/ContinuableInfo';
import type { ContinueCallback as _ContinueCallback } from './application/ContinueCallback';
import type { ContinueDeviceInfo as _ContinueDeviceInfo } from './application/ContinueDeviceInfo';
import type { ContinueMissionInfo as _ContinueMissionInfo } from './application/ContinueMissionInfo';
import type { MissionCallback as _MissionCallback } from './application/MissionCallbacks';
import type { MissionDeviceInfo as _MissionDeviceInfo } from './application/MissionDeviceInfo';
import type { MissionParameter as _MissionParameter } from './application/MissionParameter';

/**
 * This module provides the capability to manage abilities and obtain system task information for distributed
 * scenario.
 *
 * @namespace distributedMissionManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Mission
 * @systemapi Hide this for inner system use.
 * @since 9
 */
declare namespace distributedMissionManager {
  /**
   * Continue state change event definition.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  enum ContinueState {
    /**
     * Mission continuable active.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ACTIVE = 0,

    /**
     * Mission continuable inactive.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    INACTIVE = 1
  }

  /**
   * ContinueCallbackInfo.
   *
   * @interface ContinueCallbackInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  interface ContinueCallbackInfo {
    /**
     * Continue state event.
     *
     * @type { ContinueState }
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    state: ContinueState;

    /**
     * Continuable information corresponding to ability.
     *
     * @type { ContinuableInfo }
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    info: ContinuableInfo;
  }

  /**
   * Start sync missions from remote device.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionParameter } parameter - Parameters corresponding to mission.
   * @param { AsyncCallback<void> } callback - the callback of startSyncRemoteMissions.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function startSyncRemoteMissions(parameter: MissionParameter, callback: AsyncCallback<void>): void;

  /**
   * Start sync missions from remote device.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionParameter } parameter - Parameters corresponding to mission.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function startSyncRemoteMissions(parameter: MissionParameter): Promise<void>;

  /**
   * Stop sync missions from remote device.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @param { AsyncCallback<void> } callback - the callback of stopSyncRemoteMissions.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function stopSyncRemoteMissions(parameter: MissionDeviceInfo, callback: AsyncCallback<void>): void;

  /**
   * Stop sync missions from remote device.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function stopSyncRemoteMissions(parameter: MissionDeviceInfo): Promise<void>;

  /**
   * Register the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @param { MissionCallback } options - The callbacks for registering mission.
   * @param { AsyncCallback<void> } callback - the callback of registerMissionListener.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function registerMissionListener(
    parameter: MissionDeviceInfo,
    options: MissionCallback,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Register the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @param { MissionCallback } options - The callbacks for registering mission.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function registerMissionListener(parameter: MissionDeviceInfo, options: MissionCallback): Promise<void>;

  /**
   * Unregister the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @param { AsyncCallback<void> } callback - the callback of registerMissionListener.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function unRegisterMissionListener(parameter: MissionDeviceInfo, callback: AsyncCallback<void>): void;

  /**
   * Unregister the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionDeviceInfo } parameter - Parameters corresponding to mission.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function unRegisterMissionListener(parameter: MissionDeviceInfo): Promise<void>;

  /**
   * Register continuable info listener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { 'continueStateChange' } type - Indicate the event type.
   * @param { Callback<{ state: ContinueState, info: ContinuableInfo }> } callback - The callback of continueStateChange.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Register continuable info listener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { 'continueStateChange' } type - Indicate the event type.
   * @param { Callback<ContinueCallbackInfo> } callback - The callback of continueStateChange.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function on(type: 'continueStateChange', callback: Callback<ContinueCallbackInfo>): void;

  /**
   * Unregister continuable info listener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { 'continueStateChange' } type - Indicate the event type.
   * @param { Callback<{ state: ContinueState, info: ContinuableInfo }> } [callback] - The callback of continueStateChange.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Unregister continuable info listener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { 'continueStateChange' } type - Indicate the event type.
   * @param { Callback<ContinueCallbackInfo> } callback - The callback of continueStateChange.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function off(type: 'continueStateChange', callback?: Callback<ContinueCallbackInfo>): void;

  /**
   * Continue mission
   *
   * @permission ohos.permission.MANAGE_MISSIONS and ohos.permission.DISTRIBUTED_DATASYNC
   * @param { ContinueDeviceInfo } parameter - Parameters corresponding to continue mission.
   * @param { ContinueCallback } options - the migration task completes the callback function.
   * @param { AsyncCallback<void> } callback - the callback of continueMission.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16300501 - The system ability work abnormally.
   * @throws { BusinessError } 16300502 - Failed to get the missionInfo of the specified missionId.
   * @throws { BusinessError } 16300503 - The application is not installed on the remote end and installation-free is
   * not supported.
   * @throws { BusinessError } 16300504 - The application is not installed on the remote end but installation-free is
   * supported, try again with freeInstall flag.
   * @throws { BusinessError } 16300505 - The operation device must be the device where the application to be continued
   * is located or the target device to be continued.
   * @throws { BusinessError } 16300506 - The local continuation task is already in progress.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function continueMission(
    parameter: ContinueDeviceInfo,
    options: ContinueCallback,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Continue mission
   *
   * @permission ohos.permission.MANAGE_MISSIONS and ohos.permission.DISTRIBUTED_DATASYNC
   * @param { ContinueDeviceInfo } parameter - Parameters corresponding to continue mission.
   * @param { ContinueCallback } options - the migration task completes the callback function.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16300501 - The system ability work abnormally.
   * @throws { BusinessError } 16300502 - Failed to get the missionInfo of the specified missionId.
   * @throws { BusinessError } 16300503 - The application is not installed on the remote end and installation-free is
   * not supported.
   * @throws { BusinessError } 16300504 - The application is not installed on the remote end but installation-free is
   * supported, try again with freeInstall flag.
   * @throws { BusinessError } 16300505 - The operation device must be the device where the application to be continued
   * is located or the target device to be continued.
   * @throws { BusinessError } 16300506 - The local continuation task is already in progress.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function continueMission(parameter: ContinueDeviceInfo, options: ContinueCallback): Promise<void>;

  /**
   * Continue mission by ContinueMissionInfo.
   *
   * @permission ohos.permission.MANAGE_MISSIONS and ohos.permission.DISTRIBUTED_DATASYNC
   * @param { ContinueMissionInfo } parameter - Parameters corresponding to continue mission info.
   * @param { AsyncCallback<void> } callback - The callback of continue mission result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16300501 - The system ability work abnormally.
   * @throws { BusinessError } 16300503 - The application is not installed on the remote end and installation-free is
   * not supported.
   * @throws { BusinessError } 16300504 - The application is not installed on the remote end but installation-free is
   * supported, try again with freeInstall flag.
   * @throws { BusinessError } 16300505 - The operation device must be the device where the application to be continued
   * is located or the target device to be continued.
   * @throws { BusinessError } 16300506 - The local continuation task is already in progress.
   * @throws { BusinessError } 16300507 - Failed to get the missionInfo of the specified bundle name.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function continueMission(parameter: ContinueMissionInfo, callback: AsyncCallback<void>): void;

  /**
   * Continue mission by ContinueMissionInfo.
   *
   * @permission ohos.permission.MANAGE_MISSIONS and ohos.permission.DISTRIBUTED_DATASYNC
   * @param { ContinueMissionInfo } parameter - Parameters corresponding to continue mission info.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16300501 - The system ability work abnormally.
   * @throws { BusinessError } 16300503 - The application is not installed on the remote end and installation-free is
   * not supported.
   * @throws { BusinessError } 16300504 - The application is not installed on the remote end but installation-free is
   * supported, try again with freeInstall flag.
   * @throws { BusinessError } 16300505 - The operation device must be the device where the application to be continued
   * is located or the target device to be continued.
   * @throws { BusinessError } 16300506 - The local continuation task is already in progress.
   * @throws { BusinessError } 16300507 - Failed to get the missionInfo of the specified bundle name.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function continueMission(parameter: ContinueMissionInfo): Promise<void>;

  /**
   * Continuable information corresponding to ability.
   * @typedef { _ContinuableInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export type ContinuableInfo = _ContinuableInfo;

  /**
   * Parameters corresponding to continue mission.
   * @typedef { _ContinueMissionInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export type ContinueMissionInfo = _ContinueMissionInfo;

  /**
   * ContinueCallback registered for notify continue result.
   * @typedef { _ContinueCallback }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 10
   */
  export type ContinueCallback = _ContinueCallback;

  /**
   * Parameters corresponding to continue mission.
   * @typedef { _ContinueDeviceInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 10
   */
  export type ContinueDeviceInfo = _ContinueDeviceInfo;

  /**
   * MissionCallback registered by app.
   * @typedef { _MissionCallback }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 10
   */
  export type MissionCallback = _MissionCallback;

  /**
   * Parameters corresponding to mission.
   * @typedef { _MissionDeviceInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 10
   */
  export type MissionDeviceInfo = _MissionDeviceInfo;

  /**
   * Parameters corresponding to mission.
   * @typedef { _MissionParameter }
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 10
   */
  export type MissionParameter = _MissionParameter;
}
export default distributedMissionManager;
