/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
 * @kit AbilityKit
 */

import { AbilityResult } from './ability/abilityResult';
import { AsyncCallback } from './@ohos.base';
import { Configuration } from './@ohos.app.ability.Configuration';
import Context from './application/Context';
import { AbilityRunningInfo as _AbilityRunningInfo } from './application/AbilityRunningInfo';
import { ExtensionRunningInfo as _ExtensionRunningInfo } from './application/ExtensionRunningInfo';
import { ElementName } from './bundleManager/ElementName';
import * as _AbilityForegroundStateObserver from './application/AbilityForegroundStateObserver';
import * as _AbilityStateData from './application/AbilityStateData';

/**
 * The class of an ability manager.
 *
 * @namespace abilityManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 9
 */
declare namespace abilityManager {
  /**
   * Enum for the ability state.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 14
   */
  export enum AbilityState {
    /**
     * Ability is initialized.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    INITIAL = 0,

    /**
     * Ability is in the state of getting focus.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    FOCUS = 2,

    /**
     * Ability is in the foreground state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    FOREGROUND = 9,

    /**
     * Ability is in the background state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    BACKGROUND = 10,

    /**
     * Ability is in the process of scheduling at the foreground.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    FOREGROUNDING = 11,

    /**
     * Ability is in the process of scheduling in the background.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 14
     */
    BACKGROUNDING = 12
  }

  /**
   * Enum for the user status.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export enum UserStatus {
    /**
     * Indicates the status of the operation that the user clicks to terminate.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    ASSERT_TERMINATE = 0,

    /**
     * Indicates the status of the operation that the user clicks to continue.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    ASSERT_CONTINUE = 1,

    /**
     * Indicates the status of the operation that the user clicks to retry.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    ASSERT_RETRY = 2
  }

  /**
   * The class of an embedded atomic service open rule.
   *
   * @typedef AtomicServiceStartupRule
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 18
   */
  export interface AtomicServiceStartupRule {
    /**
     * the flag indicated whether openning atomic service is allowed.
     *
     * @type { boolean }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 18
     */
    isOpenAllowed: boolean;

    /**
     * the flag indicated whether embedded atomic service is allowed.
     *
     * @type { boolean }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 18
     */
    isEmbeddedAllowed: boolean;
  }

  /**
   * Register Ability foreground or background state observer.
   *
   * @permission ohos.permission.RUNNING_STATE_OBSERVER
   * @param { 'abilityForegroundState' } type - ability foreground or background state.
   * @param { AbilityForegroundStateObserver } observer - The ability foreground state observer.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function on(type: 'abilityForegroundState', observer: AbilityForegroundStateObserver): void;

  /**
   * Unregister Ability foreground or background state observer.
   *
   * @permission ohos.permission.RUNNING_STATE_OBSERVER
   * @param { 'abilityForegroundState' } type - ability foreground or background state.
   * @param { AbilityForegroundStateObserver } [observer] - The ability foreground state observer.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function off(type: 'abilityForegroundState', observer?: AbilityForegroundStateObserver): void;

  /**
   * Updates the configuration by modifying the configuration.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { Configuration } config - Indicates the new configuration.
   * @param { AsyncCallback<void> } callback - The callback of updateConfiguration.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function updateConfiguration(config: Configuration, callback: AsyncCallback<void>): void;

  /**
   * Updates the configuration by modifying the configuration.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { Configuration } config - Indicates the new configuration.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function updateConfiguration(config: Configuration): Promise<void>;

  /**
   * If you apply for permission, you can obtain information about all abilities. If you do not apply, you can only
   * obtain information about the current ability.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @returns { Promise<Array<AbilityRunningInfo>> } Returns the array of AbilityRunningInfo.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 14
   */
  function getAbilityRunningInfos(): Promise<Array<AbilityRunningInfo>>;

  /**
   * If you apply for permission, you can obtain information about all abilities. If you do not apply, you can only
   * obtain information about the current ability.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { AsyncCallback<Array<AbilityRunningInfo>> } callback - The callback is used to return the array of
   *                                                                AbilityRunningInfo.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getAbilityRunningInfos(callback: AsyncCallback<Array<AbilityRunningInfo>>): void;

  /**
   * If you apply for permission, you can obtain information about all extensions. If you do not apply, you can only
   * obtain information about the current extension.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { number } upperLimit - Get the maximum limit of the number of messages.
   * @returns { Promise<Array<ExtensionRunningInfo>> } Returns the array of ExtensionRunningInfo.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getExtensionRunningInfos(upperLimit: number): Promise<Array<ExtensionRunningInfo>>;

  /**
   * If you apply for permission, you can obtain information about all extensions. If you do not apply, you can only
   * obtain information about the current extension.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { number } upperLimit - Get the maximum limit of the number of messages.
   * @param { AsyncCallback<Array<ExtensionRunningInfo>> } callback - The callback is used to return the array of
   *                                                                  ExtensionRunningInfo.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getExtensionRunningInfos(upperLimit: number, callback: AsyncCallback<Array<ExtensionRunningInfo>>): void;

  /**
   * Get the top ability information of the display.
   *
   * @returns { Promise<ElementName> } Returns the elementName info of the top ability.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getTopAbility(): Promise<ElementName>;

  /**
   * Get the top ability information of the display.
   *
   * @param { AsyncCallback<ElementName> } callback - The callback is used to return elementName info of top ability.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getTopAbility(callback: AsyncCallback<ElementName>): void;

  /**
   * Acquire the shared data from target ability.
   *
   * @param { number } missionId - The missionId of target ability.
   * @param { AsyncCallback<{ [key: string]: Object }> } callback - The callback is used to return the params of sharing
   *                                                 data and result code.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 10
   */
  /**
   * Acquire the shared data from target ability.
   *
   * @param { number } missionId - The missionId of target ability.
   * @param { AsyncCallback<Record<string, Object>> } callback - The callback is used to return the params of sharing
   *                                                             data and result code.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function acquireShareData(missionId: number, callback: AsyncCallback<Record<string, Object>>): void;

  /**
   * Acquire the shared data from target ability.
   *
   * @param { number } missionId - The missionId of target ability.
   * @returns { Promise<{ [key: string]: Object }> } The promise returned by the function.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 10
   */
  /**
   * Acquire the shared data from target ability.
   *
   * @param { number } missionId - The missionId of target ability.
   * @returns { Promise<Record<string, Object>> } The promise returned by the function.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function acquireShareData(missionId: number): Promise<Record<string, Object>>;

  /**
   * Notify the result of save as to target ability.
   * @param { AbilityResult } parameter - Indicates the result to return.
   * @param { number } requestCode - Request code defined by the user.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function notifySaveAsResult(parameter: AbilityResult, requestCode: number, callback: AsyncCallback<void>): void;

  /**
   * Notify the result of save as to target ability.
   * @param { AbilityResult } parameter - Indicates the result to return.
   * @param { number } requestCode - Request code defined by the user.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function notifySaveAsResult(parameter: AbilityResult, requestCode: number): Promise<void>;

  /**
   * Get the foreground ui abilities.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { AsyncCallback<Array<AbilityStateData>> } callback - The callback is used to return the list of AbilityStateDatas.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function getForegroundUIAbilities(callback: AsyncCallback<Array<AbilityStateData>>): void;

  /**
   * Get the foreground ui abilities.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @returns { Promise<Array<AbilityStateData>> } Returns the list of AbilityStateDatas.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  function getForegroundUIAbilities(): Promise<Array<AbilityStateData>>;

  /**
   * Querying whether to allow embedded startup of atomic service.
   *
   * @param { Context } context - The context that initiates the query request.
   * @param { string } appId - Globally unique identifier of an application, which is allocated by the cloud.
   * @returns { Promise<boolean> } Returns the result in the form of callback.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 12
   */
  function isEmbeddedOpenAllowed(context: Context, appId: string): Promise<boolean>;

  /**
   * Notifies the application of the assertion debugging result.
   *
   * @permission ohos.permission.NOTIFY_DEBUG_ASSERT_RESULT
   * @param { string } sessionId - Indicates the request ID of AssertFault.
   * @param { UserStatus } status - Operation status of the user.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - The application does not have permission to call the interface.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  function notifyDebugAssertResult(sessionId: string, status: UserStatus): Promise<void>;

  /**
   * Set the enable status for starting and stopping resident processes.
   * The caller application can only set the resident status of the configured process.
   *
   * @param { string } bundleName - The bundle name of the resident process.
   * @param { boolean } enable - Set resident process enable status.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 202 - Not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1.Non empty package name needs to be provided, 2.The second parameter needs to provide a Boolean type setting value.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16200006 - The caller application can only set the resident status of the configured process.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 12
   */
  function setResidentProcessEnabled(bundleName: string, enable: boolean): Promise<void>;

  /**
   * Query the rule to open embedded atomic service.
   *
   * @param { Context } context - The context that initiates the query request.
   * @param { string } appId - Globally unique identifier of an application, which is allocated by the cloud.
   * @returns { Promise<AtomicServiceStartupRule> } Returns the result in the form of callback.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 18
   */
  function queryAtomicServiceStartupRule(context: Context, appId: string): Promise<AtomicServiceStartupRule>;

  /**
   * The class of an ability running information.
   *
   * @typedef { _AbilityRunningInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 14
   */
  export type AbilityRunningInfo = _AbilityRunningInfo;

  /**
   * The ability state data.
   *
   * @typedef { _AbilityStateData.default }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 14
   */
  export type AbilityStateData = _AbilityStateData.default;

  /**
   * The class of an extension running information.
   *
   * @typedef { _ExtensionRunningInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  export type ExtensionRunningInfo = _ExtensionRunningInfo;

  /**
   * The ability foreground state observer.
   *
   * @typedef { _AbilityForegroundStateObserver.default }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 11
   */
  export type AbilityForegroundStateObserver = _AbilityForegroundStateObserver.default;
}

export default abilityManager;
