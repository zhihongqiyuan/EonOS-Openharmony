/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import type { AsyncCallback } from './@ohos.base';
import type Want from './@ohos.app.ability.Want';
import type { MultiAppMode } from './bundleManager/ApplicationInfo';

/**
 * This module provides the capability to manage dialog session.
 *
 * @namespace dialogSession
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @systemapi
 * @stagemodelonly
 * @since 11
 */
declare namespace dialogSession {

  /**
   * Indicates the ability information displayed in the picker dialog, including bundleName, moduleName, and abilityName.
   *
   * @typedef DialogAbilityInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  export interface DialogAbilityInfo {

    /**
     * Bundle name
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    bundleName: string;

    /**
     * Module name
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    moduleName: string;

    /**
     * Ability name
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    abilityName: string;

    /**
     * The icon id of ability
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    abilityIconId: number;

    /**
     * The label id of ability
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    abilityLabelId: number;

    /**
     * The icon id of bundle
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    bundleIconId: number;

    /**
     * The label id of bundle
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    bundleLabelId: number;

    /**
     * The ability is visible
     *
     * @type { boolean }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    visible: boolean;

    /**
     * Index of an application clone. It takes effect only for cloned applications.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    appIndex: number;

    /**
      * The mode of the multi-application.
      *
      * @type { MultiAppMode }
      * @syscap SystemCapability.Ability.AbilityRuntime.Core
      * @systemapi
      * @stagemodelonly
      * @since 12
      */
    multiAppMode: MultiAppMode;
  }

  /**
   * Dialog session info
   *
   * @typedef DialogSessionInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  export interface DialogSessionInfo {

    /**
     * The dialog info of caller ability
     *
     * @type { DialogAbilityInfo }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    callerAbilityInfo: DialogAbilityInfo;

    /**
     * The dialog infos of target ability to select
     *
     * @type { Array<DialogAbilityInfo> }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    targetAbilityInfos: Array<DialogAbilityInfo>;

    /**
     * The description of the params object in dialog session info
     *
     * @type { ?Record<string, Object> }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 11
     */
    parameters?: Record<string, Object>;
  }

  /**
   * Query the session info of dialog.
   *
   * @param { string } dialogSessionId - Query information by dialog session id.
   * @returns { DialogSessionInfo } Returns the session info.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000005 - The specified process does not have the permission.
   * @throws { BusinessError } 16000006 - Cross-user operations are not allowed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function getDialogSessionInfo(dialogSessionId: string): DialogSessionInfo;

  /**
   * Send the selection result of dialog.
   *
   * @param { string } dialogSessionId - Send Result by dialog session id.
   * @param { Want } targetWant - The selection target ability to start.
   * @param { boolean } isAllowed - allowed or disallowed to start target ability.
   * @returns { Promise<void> } The promise returned by the sendDialogResult.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000005 - The specified process does not have the permission.
   * @throws { BusinessError } 16000006 - Cross-user operations are not allowed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function sendDialogResult(dialogSessionId: string, targetWant: Want, isAllowed: boolean): Promise<void>;

  /**
   * Send the selection result of dialog.
   *
   * @param { string } dialogSessionId - Send Result by dialog session id.
   * @param { Want } targetWant - The selection target ability to start.
   * @param { boolean } isAllowed - allowed or disallowed to start target ability.
   * @param { AsyncCallback<void> } callback - The callback of sendDialogResult.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000005 - The specified process does not have the permission.
   * @throws { BusinessError } 16000006 - Cross-user operations are not allowed.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function sendDialogResult(dialogSessionId: string, targetWant: Want, isAllowed: boolean, callback: AsyncCallback<void>): void;
}

export default dialogSession;