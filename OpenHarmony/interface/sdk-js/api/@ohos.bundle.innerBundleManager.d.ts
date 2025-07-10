/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
import type { BundleStatusCallback as _BundleStatusCallback } from './bundle/bundleStatusCallback';
import { LauncherAbilityInfo } from './bundle/launcherAbilityInfo';
import { ShortcutInfo } from './bundle/shortcutInfo';

/**
 * inner bundle manager.
 *
 * @namespace innerBundleManager
 * @syscap SystemCapability.BundleManager.BundleFramework
 * @systemapi Hide this for inner system use
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.bundle.launcherBundleManager
 */
declare namespace innerBundleManager {
  /**
   * Obtains based on a given bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the id for the user.
   * @param { AsyncCallback<Array<LauncherAbilityInfo>> } callback
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getLauncherAbilityInfo
   */
  function getLauncherAbilityInfos(bundleName: string,
    userId: number, callback: AsyncCallback<Array<LauncherAbilityInfo>>): void;

  /**
   * Obtains based on a given bundleName and userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<Array<LauncherAbilityInfo>> } Returns the LauncherAbilityInfo array.
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getLauncherAbilityInfo
   */
  function getLauncherAbilityInfos(bundleName: string, userId: number): Promise<Array<LauncherAbilityInfo>>;

  /**
   * Register Callback.
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { 'BundleStatusChange' } type - Indicates the command should be implement.
   * @param { BundleStatusCallback } bundleStatusCallback - Indicates the callback to be register.
   * @param { AsyncCallback<string> } callback
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.bundleMonitor#on
   */
  function on(type: 'BundleStatusChange',
    bundleStatusCallback: BundleStatusCallback, callback: AsyncCallback<string>): void;

  /**
   * Register Callback.
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { 'BundleStatusChange' } type - Indicates the command should be implement.
   * @param { BundleStatusCallback } bundleStatusCallback - Indicates the callback to be register.
   * @returns { Promise<string> } - Returns the result of register.
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.bundleMonitor#on
   */
  function on(type: 'BundleStatusChange', bundleStatusCallback: BundleStatusCallback): Promise<string>;

  /**
   * UnRegister Callback.
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { 'BundleStatusChange' } type - Indicates the command should be implement.
   * @param { AsyncCallback<string> } callback
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.bundleMonitor#off
   */
  function off(type: 'BundleStatusChange', callback: AsyncCallback<string>): void;

  /**
   * UnRegister Callback.
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { 'BundleStatusChange' } type - Indicates the command should be implement.
   * @returns { Promise<string> } Returns the result of unregister.
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.bundleMonitor#off
   */
  function off(type: 'BundleStatusChange'): Promise<string>;

  /**
   * Obtains based on a given userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { number } userId - Indicates the id for the user.
   * @param { AsyncCallback<Array<LauncherAbilityInfo>> } callback
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getAllLauncherAbilityInfos
   */
  function getAllLauncherAbilityInfos(userId: number, callback: AsyncCallback<Array<LauncherAbilityInfo>>): void;

  /**
   * Obtains based on a given userId.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { number } userId - Indicates the id for the user.
   * @returns { Promise<Array<LauncherAbilityInfo>> } Returns the LauncherAbilityInfo array.
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getAllLauncherAbilityInfos
   */
  function getAllLauncherAbilityInfos(userId: number): Promise<Array<LauncherAbilityInfo>>;

  /**
   * Obtains based on a given bundleName.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @param { AsyncCallback<Array<ShortcutInfo>> } callback
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getShortcutInfo
   */
  function getShortcutInfos(bundleName: string, callback: AsyncCallback<Array<ShortcutInfo>>): void;

  /**
   * Obtains based on a given bundleName.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the application bundle name to be queried.
   * @returns { Promise<Array<ShortcutInfo>> } Returns the LauncherShortcutInfo array.
   * @syscap SystemCapability.BundleManager.BundleFramework
   * @systemapi Hide this for inner system use
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.launcherBundleManager#getShortcutInfo
   */
  function getShortcutInfos(bundleName: string): Promise<Array<ShortcutInfo>>;
}

/**
 * Contains basic Ability information, which uniquely identifies a launcher StatusCallback.
 *
 * @typedef { _BundleStatusCallback }
 * @syscap SystemCapability.BundleManager.BundleFramework
 * @systemapi Hide this for inner system use
 * @since 8
 * @deprecated since 9
 */
export type BundleStatusCallback = _BundleStatusCallback;

export default innerBundleManager;
