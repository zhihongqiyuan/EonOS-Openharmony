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

import { Callback } from './@ohos.base';

/**
 * Bundle monitor，monitor application installation, updates, and uninstallation events.
 *
 * @namespace bundleMonitor
 * @syscap SystemCapability.BundleManager.BundleFramework.Core
 * @systemapi
 * @since 9
 */
declare namespace bundleMonitor {
  /**
   * This module defines the result information of monitoring install, update and uninstall.
   *
   * @typedef BundleChangedInfo
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  interface BundleChangedInfo {
    /**
     * The bundle name
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    readonly bundleName: string;
    /**
     * The user id
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    readonly userId: number;
    /**
     * The app index of clone app
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    readonly appIndex: number;
  }

  /**
   * Indicates the event type of bundle change
   *
   * @typedef { 'add' | 'update' | 'remove' }
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  type BundleChangedEvent = 'add' | 'update' | 'remove';

  /**
   * Register to monitor the installation status
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { BundleChangedEvent } type - Indicates the command should be implement.
   * @param { Callback<BundleChangedInfo> } callback - Indicates the callback to be register.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  function on(type: BundleChangedEvent, callback: Callback<BundleChangedInfo>): void;

  /**
   * Unregister to monitor the installation status
   *
   * @permission ohos.permission.LISTEN_BUNDLE_CHANGE
   * @param { BundleChangedEvent } type -type Indicates the command should be implement.
   * @param { Callback<BundleChangedInfo> } callback - Indicates the callback to be unregister.
   * @throws { BusinessError } 201 - Verify permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  function off(type: BundleChangedEvent, callback?: Callback<BundleChangedInfo>): void;
}

export default bundleMonitor;
