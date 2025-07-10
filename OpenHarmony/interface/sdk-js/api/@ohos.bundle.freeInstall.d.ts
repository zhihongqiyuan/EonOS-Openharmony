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
import { DispatchInfo as _DispatchInfo } from './bundleManager/DispatchInfo';
import * as _PackInfo from './bundleManager/BundlePackInfo';

/**
 * Free install bundle manager.
 *
 * @namespace freeInstall
 * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
 * @systemapi
 * @since 9
 */
declare namespace freeInstall {
  /**
   * Used to set the enumeration value of upgrading for free installation.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export enum UpgradeFlag {
    /**
     * Indicates module not need to be upgraded
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    NOT_UPGRADE = 0,
    /**
     * Indicates single module need to be upgraded
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    SINGLE_UPGRADE = 1,
    /**
     * Indicates relation module need to be upgraded
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    RELATION_UPGRADE = 2
  }

  /**
   * Used to query the enumeration value of bundlePackInfo.
   * Multiple value input, such as GET_PACKAGES | GET_BUNDLE_SUMMARY.
   *
   * @enum { number }
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi
   * @since 9
   */
  export enum BundlePackFlag {
    /**
     * Query all package information.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    GET_PACK_INFO_ALL = 0x00000000,
    /**
     * Query package information
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    GET_PACKAGES = 0x00000001,
    /**
     * Query the brief information of the package
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    GET_BUNDLE_SUMMARY = 0x00000002,
    /**
     * Query the brief information of the module.
     *
     * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
     * @systemapi
     * @since 9
     */
    GET_MODULE_SUMMARY = 0x00000004
  }

  /**
   * Sets whether to upgrade the module.
   *
   * @permission ohos.permission.INSTALL_BUNDLE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } moduleName - Indicates the module name of the application.
   * @param { UpgradeFlag } upgradeFlag - Indicates upgradeFlag of the application.
   * @param { AsyncCallback<void> } callback - The callback of setting module upgrade flag result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700002 - The specified module name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function setHapModuleUpgradeFlag(bundleName: string,
    moduleName: string, upgradeFlag: UpgradeFlag, callback: AsyncCallback<void>): void;

  /**
   * Sets whether to upgrade the module.
   *
   * @permission ohos.permission.INSTALL_BUNDLE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } moduleName - Indicates the module name of the application.
   * @param { UpgradeFlag } upgradeFlag - Indicates upgradeFlag of the application.
   * @returns { Promise<void> } - Return the result of setting module upgrade flag.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700002 - The specified module name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function setHapModuleUpgradeFlag(bundleName: string, moduleName: string, upgradeFlag: UpgradeFlag): Promise<void>;

  /**
   * Checks whether a specified module is removable.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } moduleName - Indicates the module name of the application.
   * @param { AsyncCallback<boolean> } callback - The callback of checking module removable result.
   *  The result is true if the module is removable, false otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700002 - The specified module name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function isHapModuleRemovable(bundleName: string, moduleName: string, callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether a specified module is removable.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } moduleName - Indicates the module name of the application.
   * @returns { Promise<boolean> } Returns true if the module is removable; returns false otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700002 - The specified module name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function isHapModuleRemovable(bundleName: string, moduleName: string): Promise<boolean>;

  /**
   * Obtains bundlePackInfo based on bundleName and bundlePackFlags.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { BundlePackFlag } bundlePackFlag - Indicates the application bundle pack flag to be queried.
   * @param { AsyncCallback<BundlePackInfo> } callback - The callback of getting the BundlePackInfo object result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function getBundlePackInfo(bundleName: string,
    bundlePackFlag: BundlePackFlag, callback: AsyncCallback<BundlePackInfo>): void;

  /**
   * Obtains bundlePackInfo based on bundleName and bundlePackFlags.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { BundlePackFlag } bundlePackFlag - Indicates the application bundle pack flag to be queried.
   * @returns { Promise<BundlePackInfo> } Returns the BundlePackInfo object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function getBundlePackInfo(bundleName: string, bundlePackFlag: BundlePackFlag): Promise<BundlePackInfo>;

  /**
   * Obtains information about the dispatcher version.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { AsyncCallback<DispatchInfo> } callback - The callback of getting the dispatchInfo object for the current ability result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 801 - Capability not supported.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function getDispatchInfo(callback: AsyncCallback<DispatchInfo>): void;

  /**
   * Obtains information about the dispatcher version.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @returns { Promise<DispatchInfo> } Returns the DispatchInfo object for the current ability.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 801 - Capability not supported.
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  function getDispatchInfo(): Promise<DispatchInfo>;

  /**
   * The dispatch info class.
   *
   * @typedef { _DispatchInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type DispatchInfo = _DispatchInfo;

  /**
   * The bundle pack info class.
   *
   * @typedef { _PackInfo.BundlePackInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type BundlePackInfo = _PackInfo.BundlePackInfo;

  /**
   * The package info class.
   *
   * @typedef { _PackInfo.PackageConfig }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type PackageConfig = _PackInfo.PackageConfig;

  /**
   * The package summary class.
   *
   * @typedef { _PackInfo.PackageSummary }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type PackageSummary = _PackInfo.PackageSummary;

  /**
   * The bundle summary class.
   *
   * @typedef { _PackInfo.BundleConfigInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type BundleConfigInfo = _PackInfo.BundleConfigInfo;

  /**
   * The extension ability forms class.
   *
   * @typedef { _PackInfo.ExtensionAbility }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type ExtensionAbility = _PackInfo.ExtensionAbility;

  /**
   * The module summary of a bundle.
   *
   * @typedef { _PackInfo.ModuleConfigInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type ModuleConfigInfo = _PackInfo.ModuleConfigInfo;

  /**
   * The bundle info summary class.
   *
   * @typedef { _PackInfo.ModuleDistroInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type ModuleDistroInfo = _PackInfo.ModuleDistroInfo;

  /**
   * The ability info of a module.
   *
   * @typedef { _PackInfo.ModuleAbilityInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type ModuleAbilityInfo = _PackInfo.ModuleAbilityInfo;

  /**
   * The form info of an ability.
   *
   * @typedef { _PackInfo.AbilityFormInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type AbilityFormInfo = _PackInfo.AbilityFormInfo;

  /**
   * The bundle version class.
   *
   * @typedef { _PackInfo.Version }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type Version = _PackInfo.Version;

  /**
   * The bundle Api version class.
   *
   * @typedef { _PackInfo.ApiVersion }
   * @syscap SystemCapability.BundleManager.BundleFramework.FreeInstall
   * @systemapi
   * @since 9
   */
  export type ApiVersion = _PackInfo.ApiVersion;
}

export default freeInstall;
