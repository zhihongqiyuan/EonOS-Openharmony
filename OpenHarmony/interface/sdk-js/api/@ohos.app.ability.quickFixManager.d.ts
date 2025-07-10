/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

import { AsyncCallback } from './@ohos.base';

/**
 * Interface of quickFixManager.
 *
 * @namespace quickFixManager
 * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
 * @systemapi
 * @since 9
 */
declare namespace quickFixManager {
  /**
   * Quick fix info of hap module.
   *
   * @typedef HapModuleQuickFixInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  export interface HapModuleQuickFixInfo {
    /**
     * Indicates hap module name.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly moduleName: string;

    /**
     * Indicates hash value of a hap.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly originHapHash: string;

    /**
     * Indicates installed path of quick fix file.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly quickFixFilePath: string;
  }

  /**
   * Quick fix info of application.
   *
   * @typedef ApplicationQuickFixInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  export interface ApplicationQuickFixInfo {
    /**
     * Bundle name.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly bundleName: string;

    /**
     * The version number of the bundle.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly bundleVersionCode: number;

    /**
     * The version name of the bundle.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly bundleVersionName: string;

    /**
     * The version number of the quick fix.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly quickFixVersionCode: number;

    /**
     * The version name of the quick fix.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly quickFixVersionName: string;

    /**
     * Hap module quick fix info.
     *
     * @type { Array<HapModuleQuickFixInfo> }
     * @readonly
     * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
     * @systemapi
     * @since 9
     */
    readonly hapModuleQuickFixInfo: Array<HapModuleQuickFixInfo>;
  }

  /**
   * Apply quick fix files.
   *
   * @permission ohos.permission.INSTALL_BUNDLE
   * @param { Array<string> } hapModuleQuickFixFiles - Quick fix files need to apply, this value should include file
   *                                                   path and file name.
   * @param { AsyncCallback<void> } callback - The callback of applyQuickFix.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500002 - Invalid patch package.
   * @throws { BusinessError } 18500008 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  function applyQuickFix(hapModuleQuickFixFiles: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Apply quick fix files.
   *
   * @permission ohos.permission.INSTALL_BUNDLE
   * @param { Array<string> } hapModuleQuickFixFiles - Quick fix files need to apply, this value should include file
   *                                                   path and file name.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500002 - Invalid patch package.
   * @throws { BusinessError } 18500008 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  function applyQuickFix(hapModuleQuickFixFiles: Array<string>): Promise<void>;

  /**
   * Revoke quick fix by bundle name.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED and ohos.permission.INSTALL_BUNDLE
   * @param { string } bundleName - Bundle name wish to revoke quick fix.
   * @param { AsyncCallback<void> } callback - The callback of revokeQuickFix.
   * @throws { BusinessError } 201 - The application does not have permission to call the interface.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500001 - The bundle does not exist or no patch has been applied.
   * @throws { BusinessError } 18500009 - The application has an ongoing quick fix task.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 10
   */
  function revokeQuickFix(bundleName: string, callback: AsyncCallback<void>): void;

  /**
   * Revoke quick fix by bundle name.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED and ohos.permission.INSTALL_BUNDLE
   * @param { string } bundleName - Bundle name wish to revoke quick fix.
   * @returns { Promise<void> }
   * @throws { BusinessError } 201 - The application does not have permission to call the interface.
   * @throws { BusinessError } 202 - The application is not system-app, can not use system-api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500001 - The bundle does not exist or no patch has been applied.
   * @throws { BusinessError } 18500009 - The application has an ongoing quick fix task.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 10
   */
  function revokeQuickFix(bundleName: string): Promise<void>;

  /**
   * Get application quick fix info by bundle name.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Bundle name wish to query.
   * @param { AsyncCallback<ApplicationQuickFixInfo> } callback - The callback is used to return the ApplicationQuickFixInfo.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500001 - The bundle does not exist or no patch has been applied.
   * @throws { BusinessError } 18500008 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  function getApplicationQuickFixInfo(bundleName: string, callback: AsyncCallback<ApplicationQuickFixInfo>): void;

  /**
   * Get application quick fix info by bundle name.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Bundle name wish to query.
   * @returns { Promise<ApplicationQuickFixInfo> } Returns the ApplicationQuickFixInfo.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 18500001 - The bundle does not exist or no patch has been applied.
   * @throws { BusinessError } 18500008 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.QuickFix
   * @systemapi
   * @since 9
   */
  function getApplicationQuickFixInfo(bundleName: string): Promise<ApplicationQuickFixInfo>;
}

export default quickFixManager;
