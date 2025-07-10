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

import { AsyncCallback } from './@ohos.base';
import type { ElementName } from './bundleManager/ElementName';
import Want from './@ohos.app.ability.Want';

/**
 * Used for application interception control
 *
 * @namespace appControl
 * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
 * @systemapi
 * @since 9
 */
declare namespace appControl {
  /**
   * Indicates the ability component type.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  export enum ComponentType {
    /**
     * Indicates the UI ability type.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    UI_ABILITY = 1,

    /**
     * Indicates the extension ability type.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    UI_EXTENSION = 2
  }

  /**
   * Indicates the ability component type when uninstalled.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 15
   */
  export enum UninstallComponentType {
    /**
     * Indicates the service extension ability type.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 15
     */
    EXTENSION = 1,
  }

  /**
   * Indicates when to intercept the specified application.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  export enum DisposedType {
    /**
     * All abilities will be blocked.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    BLOCK_APPLICATION = 1,
    /**
     * Only the specified abilities will be blocked.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    BLOCK_ABILITY = 2,
    /**
     * All abilities will not be blocked.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    NON_BLOCK = 3
  }

  /**
   * Indicates the strategy to intercept the specified application.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  export enum ControlType {
    /**
     * Only the specified abilities are allowed to run.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    ALLOWED_LIST = 1,
    /**
     * The specified abilities are not allowed to run.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    DISALLOWED_LIST = 2
  }

  /**
   * Indicates the rule for interception.
   *
   * @typedef DisposedRule
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  export interface DisposedRule {
    /**
     * Indicates the ability that will be pulled up when interception occurs.
     *
     * @type { Want }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    want: Want;

    /**
     * Indicates the type of the ability that will be pulled up when interception occurs.
     *
     * @type { ComponentType }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    componentType: ComponentType;

    /**
     * Indicates when to intercept the specified application.
     *
     * @type { DisposedType }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    disposedType: DisposedType;

    /**
     * Indicates the interception policy.
     *
     * @type { ControlType }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    controlType: ControlType;

    /**
     * Indicates the abilities to be disposed of the specified application.
     *
     * @type { Array<ElementName> }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    elementList: Array<ElementName> ;

    /**
     * Indicates priority of the rule.
     *
     * @type { number }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 11
     */
    priority: number;
  }

  /**
   * Indicates the uninstall rule for interception.
   *
   * @typedef UninstallDisposedRule
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 15
   */
  export interface UninstallDisposedRule {
    /**
     * Indicates the ability that will be pulled up when interception occurs.
     *
     * @type { Want }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 15
     */
    want: Want;

    /**
     * Indicates the type of the ability that will be pulled up when interception occurs.
     *
     * @type { UninstallComponentType }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 15
     */
    uninstallComponentType: UninstallComponentType;

    /**
     * Indicates priority of the rule.
     *
     * @type { number }
     * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
     * @systemapi
     * @since 15
     */
    priority: number;
  }

  /**
   * Set the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { Want } disposedWant - Indicates the disposed want.
   * @param { AsyncCallback<void> } callback - The callback of setting the disposed status result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function setDisposedStatus(appId: string, disposedWant: Want, callback: AsyncCallback<void>): void;

  /**
   * Set the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { Want } disposedWant - Indicates the disposed want.
   * @returns { Promise<void> } The result of setting the disposed status of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function setDisposedStatus(appId: string, disposedWant: Want): Promise<void>;

  /**
   * Set the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { Want } disposedWant - Indicates the disposed want.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 10
   */
  function setDisposedStatusSync(appId: string, disposedWant: Want): void;

  /**
   * Obtains the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { AsyncCallback<Want> } callback - The callback of getting the disposed status of a specified bundle result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function getDisposedStatus(appId: string, callback: AsyncCallback<Want>): void;

  /**
   * Obtains the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @returns { Promise<Want> } Returns the disposed status of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function getDisposedStatus(appId: string): Promise<Want>;

  /**
   * Obtains the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @returns { Want } Returns the disposed status of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 10
   */
  function getDisposedStatusSync(appId: string): Want;

  /**
   * Delete the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { AsyncCallback<void> } callback - the callback of deleting the disposed status of a specified bundle result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function deleteDisposedStatus(appId: string, callback: AsyncCallback<void>): void;

  /**
   * Delete the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @returns { Promise<void> } Returns the result of deleting the disposed status of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 9
   */
  function deleteDisposedStatus(appId: string): Promise<void>;

  /**
   * Delete the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 10
   */
  /**
   * Delete the disposed status of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is invalid.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 12
   */
  function deleteDisposedStatusSync(appId: string, appIndex?: number): void;

  /**
   * Obtains the disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @returns { DisposedRule } Returns the disposed rule of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is an empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  /**
   * Obtains the disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @returns { DisposedRule } Returns the disposed rule of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is invalid.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 12
   */
  function getDisposedRule(appId: string, appIndex?: number): DisposedRule;

  /**
   * Sets the disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { DisposedRule } rule - Indicates the disposed rule of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is an empty string.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 11
   */
  /**
   * Sets the disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appId - Indicates the app ID of the application.
   * @param { DisposedRule } rule - Indicates the disposed rule of a specified bundle.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700005 - The specified app ID is invalid.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 12
   */
    function setDisposedRule(appId: string, rule: DisposedRule, appIndex?: number): void;

  /**
   * Sets the uninstall disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appIdentifier - Indicates the appIdentifier of the application.
   * @param { UninstallDisposedRule } rule - Indicates the uninstall disposed rule of a specified bundle.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @throws { BusinessError } 17700074 - The specified appIdentifier is invalid.
   * @throws { BusinessError } 17700075 - The specified bundleName of want is not the same with caller.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 15
   */
  function setUninstallDisposedRule(appIdentifier: string, rule: UninstallDisposedRule, appIndex?: number): void;
  
  /**
   * Obtains the uninstall disposed rule of a specified bundle.
   *
   * @permission ohos.permission.GET_DISPOSED_APP_STATUS or ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appIdentifier - Indicates the appIdentifier of the application.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @returns { UninstallDisposedRule } Returns the uninstall disposed rule of a specified bundle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @throws { BusinessError } 17700074 - The specified appIdentifier is invalid.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 15
   */
  function getUninstallDisposedRule(appIdentifier: string, appIndex?: number): UninstallDisposedRule;

  /**
   * Delete the uninstall disposed rule of a specified bundle.
   *
   * @permission ohos.permission.MANAGE_DISPOSED_APP_STATUS
   * @param { string } appIdentifier - Indicates the appIdentifier of the application.
   * @param { number } [appIndex] Indicates the index of clone app.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied. A non-system application is not allowed to call a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700061 - AppIndex is not in the valid range.
   * @throws { BusinessError } 17700074 - The specified appIdentifier is invalid.
   * @syscap SystemCapability.BundleManager.BundleFramework.AppControl
   * @systemapi
   * @since 15
   */
  function deleteUninstallDisposedRule(appIdentifier: string, appIndex?: number): void;
}

export default appControl;
