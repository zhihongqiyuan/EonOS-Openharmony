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
import { LauncherAbilityInfo as _LauncherAbilityInfo } from './bundleManager/LauncherAbilityInfo';
import { ShortcutInfo as _ShortcutInfo, ShortcutWant as _ShortcutWant, ParameterItem as _ParameterItem } from './bundleManager/ShortcutInfo';
import { StartOptions } from './@ohos.app.ability.StartOptions';

/**
 * Launcher bundle manager.
 *
 * @namespace launcherBundleManager
 * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
 * @since 18
 */
declare namespace launcherBundleManager {
  /**
   * Obtains launcher abilities info based on a given bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the id for the user.
   * @param { AsyncCallback<Array<LauncherAbilityInfo>> } callback -The callback of the LauncherAbilityInfo object result.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getLauncherAbilityInfo(bundleName: string,
    userId: number, callback: AsyncCallback<Array<LauncherAbilityInfo>>): void;

  /**
   * Obtains launcher abilities info based on a given bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<Array<LauncherAbilityInfo>> } the LauncherAbilityInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getLauncherAbilityInfo(bundleName: string, userId: number): Promise<Array<LauncherAbilityInfo>>;

  /**
   * Obtains launcher abilities info based on a given bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the id for the user.
   * @returns { Array<LauncherAbilityInfo> } the LauncherAbilityInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @since 18
   */
  function getLauncherAbilityInfoSync(bundleName: string, userId: number): Array<LauncherAbilityInfo>;

  /**
   * Obtains launcher abilities info based on a given userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { number } userId - Indicates the id for the user.
   * @param { AsyncCallback<Array<LauncherAbilityInfo>> } callback -The callback of the LauncherAbilityInfo object result.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getAllLauncherAbilityInfo(userId: number, callback: AsyncCallback<Array<LauncherAbilityInfo>>): void;

  /**
   * Obtains launcher abilities info based on a given userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<Array<LauncherAbilityInfo>> } the LauncherAbilityInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getAllLauncherAbilityInfo(userId: number): Promise<Array<LauncherAbilityInfo>>;

  /**
   * Obtains shortcut info based on a given bundleName.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { AsyncCallback<Array<ShortcutInfo>> } callback -The callback of the ShortcutInfo object result.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getShortcutInfo(bundleName: string, callback: AsyncCallback<Array<ShortcutInfo>>): void;

  /**
   * Obtains shortcut info based on a given bundleName.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @returns { Promise<Array<ShortcutInfo>> } the LauncherShortcutInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  function getShortcutInfo(bundleName: string): Promise<Array<ShortcutInfo>>;

  /**
   * Obtains shortcut info based on a given bundleName.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @returns { Array<ShortcutInfo> } the LauncherShortcutInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 10
   */
  function getShortcutInfoSync(bundleName: string): Array<ShortcutInfo>;


  /**
   * Obtains shortcut info based on bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the user ID.
   * @returns { Array<ShortcutInfo> } the LauncherShortcutInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 13
   */
  function getShortcutInfoSync(bundleName: string, userId: number): Array<ShortcutInfo>;

  /**
   * Starts shortcut.
   *
   * @permission ohos.permission.START_SHORTCUT
   * @param { ShortcutInfo } shortcutInfo - Indicates the shortcut info which contains shortcut want.
   * @param { StartOptions } [options] - Indicates the start options.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not support.
   * @throws { BusinessError } 17700065 - The specified shortcut want in shortcut info is not supported to be started.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  function startShortcut(shortcutInfo: ShortcutInfo, options?: StartOptions): Promise<void>;

  /**
   * Contains basic launcher Ability information, which uniquely identifies an LauncherAbilityInfo.
   *
   * @typedef { _LauncherAbilityInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @since 18
   */
  export type LauncherAbilityInfo = _LauncherAbilityInfo;

  /**
   * Provides information about a shortcut, including the shortcut ID and label.
   *
   * @typedef { _ShortcutInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  export type ShortcutInfo = _ShortcutInfo;
  /**
   * Obtains information about the ability that a shortcut will start.
   *
   * @typedef { _ShortcutWant }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 9
   */
  export type ShortcutWant = _ShortcutWant;
  /**
   * Indicates the custom parameters in shortcut want.
   *
   * @typedef { _ParameterItem }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  export type ParameterItem = _ParameterItem;
}

export default launcherBundleManager;
