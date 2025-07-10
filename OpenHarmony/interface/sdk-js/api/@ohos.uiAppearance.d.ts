/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
 * @kit ArkUI
 */

import type { AsyncCallback } from './@ohos.base';

/**
 * Provide APIs to set system uiAppearance.
 *
 * @namespace uiAppearance
 * @syscap SystemCapability.ArkUI.UiAppearance
 * @systemapi hide this for inner system use
 * @since 10
 */
declare namespace uiAppearance {
  /**
   * Enumerates dark-mode.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 10
   */
  enum DarkMode {
    /**
     * Always display with dark mode.
     *
     * @syscap SystemCapability.ArkUI.UiAppearance
     * @systemapi hide this for inner system use
     * @since 10
     */
    ALWAYS_DARK = 0,

    /**
     * Always display with light mode.
     *
     * @syscap SystemCapability.ArkUI.UiAppearance
     * @systemapi hide this for inner system use
     * @since 10
     */
    ALWAYS_LIGHT = 1
  }

  /**
   * Set the system dark-mode.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { DarkMode } mode - indicates the dark-mode to set
   * @param { AsyncCallback<void> } callback - the callback of setDarkMode
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 10
   */
  function setDarkMode(mode: DarkMode, callback: AsyncCallback<void>): void;

  /**
   * Set the system dark-mode.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { DarkMode } mode - indicates the dark-mode to set
   * @returns { Promise<void> } the promise returned by the function
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 10
   */
  function setDarkMode(mode: DarkMode): Promise<void>;

  /**
   * Acquire the current dark-mode.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @returns { DarkMode } current dark-mode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 10
   */
  function getDarkMode(): DarkMode;

  /**
   * Set the system font-scale.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { number } fontScale - indicates the font-scale to set
   * @returns { Promise<void> } the promise returned by the function
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 12
   */
  function setFontScale(fontScale: number): Promise<void>;

  /**
   * Acquire the current font-scale.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @returns { number } current font-scale.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 12
   */
  function getFontScale(): number;

  /**
   * Set the system font-weight-scale.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { number } fontWeightScale - indicates the font-weight-scale to set
   * @returns { Promise<void> } the promise returned by the function
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 12
   */
  function setFontWeightScale(fontWeightScale: number): Promise<void>;

  /**
   * Acquire the current font-weight-scale.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @returns { number } current font-weight-scale.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 500001 - Internal error.
   * @syscap SystemCapability.ArkUI.UiAppearance
   * @systemapi hide this for inner system use
   * @since 12
   */
  function getFontWeightScale(): number;
}
export default uiAppearance;
