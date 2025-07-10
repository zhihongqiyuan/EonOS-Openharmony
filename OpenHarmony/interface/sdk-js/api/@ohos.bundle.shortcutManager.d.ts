/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import { ShortcutInfo as _ShortcutInfo, ShortcutWant as _ShortcutWant, ParameterItem as _ParameterItem } from './bundleManager/ShortcutInfo';

/**
 * Desktop shortcut bundle manager.
 *
 * @namespace shortcutManager
 * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
 * @systemapi
 * @since 12
 */
declare namespace shortcutManager {
  /**
   * Add desktop shortcut info.
   *
   * @permission ohos.permission.MANAGE_SHORTCUTS
   * @param { ShortcutInfo } shortcutInfo - Indicates the shortcut info which contains shortcut want.
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found. 
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @throws { BusinessError } 17700026 - The specified bundle is disabled.
   * @throws { BusinessError } 17700061 - The specified app index is invalid.
   * @throws { BusinessError } 17700070 - The specified shortcut id is illegal.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  function addDesktopShortcutInfo(shortcutInfo: ShortcutInfo, userId: number): Promise<void>;
  
  /**
   * Delete desktop shortcut info.
   *
   * @permission ohos.permission.MANAGE_SHORTCUTS
   * @param { ShortcutInfo } shortcutInfo - Indicates the shortcut info which contains shortcut want.
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  function deleteDesktopShortcutInfo(shortcutInfo: ShortcutInfo, userId: number): Promise<void>;
  
  /**
   * Get all desktop shortcut info.
   *
   * @permission ohos.permission.MANAGE_SHORTCUTS
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<Array<ShortcutInfo>> } the LauncherShortcutInfo object.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 17700004 - The specified user ID is not found.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  function getAllDesktopShortcutInfo(userId: number): Promise<Array<ShortcutInfo>>;

  /**
   * Set a shortcut of current application is visible or invisible.
   *
   * @param { string } id - Indicates id of shortcut to set.
   * @param { boolean } visible - The value true means to set the shortcut visible, otherwise set the shortcut invisible.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 17700070 - The specified shortcut id is illegal.
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @since 20
   */
  function setShortcutVisibleForSelf(id: string, visible: boolean): Promise<void>;

  /**
   * Provides information about a shortcut, including the shortcut ID and label.
   *
   * @typedef { _ShortcutInfo }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
   */
  export type ShortcutInfo = _ShortcutInfo;
  /**
   * Obtains information about the ability that a shortcut will start.
   *
   * @typedef { _ShortcutWant }
   * @syscap SystemCapability.BundleManager.BundleFramework.Launcher
   * @systemapi
   * @since 12
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

  export default shortcutManager;
