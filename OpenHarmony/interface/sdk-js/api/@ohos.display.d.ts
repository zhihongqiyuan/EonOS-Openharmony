/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
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

import type { AsyncCallback, Callback } from './@ohos.base';
import type colorSpaceManager from './@ohos.graphics.colorSpaceManager';
import type hdrCapability from './@ohos.graphics.hdrCapability';

/**
 * Interface of display manager.
 *
 * @namespace display
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @since 7
 */
/**
 * Interface of display manager.
 *
 * @namespace display
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @crossplatform
 * @since 10
 */
/**
 * Interface of display manager.
 *
 * @namespace display
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace display {
  /**
   * Obtain the default display.
   *
   * @param { AsyncCallback<Display> } callback the result of display
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.display#getDefaultDisplaySync
   */
  function getDefaultDisplay(callback: AsyncCallback<Display>): void;

  /**
   * Obtain the default display.
   *
   * @returns { Promise<Display> } the result of display
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.display#getDefaultDisplaySync
   */
  function getDefaultDisplay(): Promise<Display>;

  /**
   * Obtain the default display.
   *
   * @returns { Display } the result of display
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Obtain the default display.
   *
   * @returns { Display } the result of display
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @since 10
   */
  /**
   * Obtain the default display.
   *
   * @returns { Display } the result of display
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function getDefaultDisplaySync(): Display;

  /**
   * Obtain the primary display. For devices other than 2in1 devices, the Display object obtained is the built-in screen. 
   * For 2in1 devices with an external screen, the Display object obtained is the primary screen. For 2in1 devices without an external screen, the Display object obtained is the built-in screen.
   *
   * @returns { Display } the result of primary display
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  function getPrimaryDisplaySync(): Display;

  /**
   * Obtain the target display.
   *
   * @param { number } displayId Display id to query. This parameter should be greater than or equal to 0.
   * @returns { Display } the result of display
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *                                                                   2. Incorrect parameter types.
   *                                                                   3. Parameter verification failed.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function getDisplayByIdSync(displayId: number): Display;

  /**
   * Obtain all displays.
   *
   * @param { AsyncCallback<Array<Display>> } callback the result of all displays
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.display#getAllDisplays
   */
  function getAllDisplay(callback: AsyncCallback<Array<Display>>): void;

  /**
   * Obtain all displays.
   *
   * @returns { Promise<Array<Display>> } the result of all displays
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.display#getAllDisplays
   */
  function getAllDisplay(): Promise<Array<Display>>;

  /**
   * Obtain all displays.
   *
   * @param { AsyncCallback<Array<Display>> } callback the result of all displays
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Obtain all displays.
   *
   * @param { AsyncCallback<Array<Display>> } callback the result of all displays
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function getAllDisplays(callback: AsyncCallback<Array<Display>>): void;

  /**
   * Obtain all displays.
   *
   * @returns { Promise<Array<Display>> } the result of all displays
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Obtain all displays.
   *
   * @returns { Promise<Array<Display>> } the result of all displays
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function getAllDisplays(): Promise<Array<Display>>;

  /**
   * Obtain all display physical resolution
   *
   * @returns { Promise<Array<DisplayPhysicalResolution>> } the result of all display physical resolution
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function getAllDisplayPhysicalResolution(): Promise<Array<DisplayPhysicalResolution>>;

  /**
   * Check whether there is a privacy window on the current display.
   *
   * @param { number } displayId Display id to query. This parameter should be greater than or equal to 0.
   * @returns { boolean } true means there is a privacy window on the current display
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function hasPrivateWindow(displayId: number): boolean;

  /**
   * Register the callback for display changes.
   *
   * @param { 'add' | 'remove' | 'change' } type the event of display change
   * @param { Callback<number> } callback the display id of changed
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   */
  /**
   * Register the callback for display changes.
   *
   * @param { 'add' | 'remove' | 'change' } type the event of display change
   * @param { Callback<number> } callback the display id of changed
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function on(type: 'add' | 'remove' | 'change', callback: Callback<number>): void;

  /**
   * Unregister the callback for display changes.
   *
   * @param { 'add' | 'remove' | 'change' } type the event of display change event
   * @param { Callback<number> } callback the display id of changed
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   */
  /**
   * Unregister the callback for display changes.
   *
   * @param { 'add' | 'remove' | 'change' } type the event of display change event
   * @param { Callback<number> } callback the display id of changed
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  function off(type: 'add' | 'remove' | 'change', callback?: Callback<number>): void;

  /**
   * Register the callback for private mode changes.
   *
   * @param { 'privateModeChange' } type the event of private mode changes
   * @param { Callback<boolean> } callback Callback used to return the result whether display is on private mode or not
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function on(type: 'privateModeChange', callback: Callback<boolean>): void;

  /**
   * Unregister the callback for private mode changes.
   *
   * @param { 'privateModeChange' } type the event of private mode changes
   * @param { Callback<boolean> } callback Callback used to return the result whether display is on private mode or not
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function off(type: 'privateModeChange', callback?: Callback<boolean>): void;

  /**
   * Check whether the device is foldable.
   *
   * @returns { boolean } true means the device is foldable.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Check whether the device is foldable.
   *
   * @returns { boolean } true means the device is foldable.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function isFoldable(): boolean;

  /**
   * Get the current fold status of the foldable device.
   *
   * @returns { FoldStatus } fold status of device.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Get the current fold status of the foldable device.
   *
   * @returns { FoldStatus } fold status of device.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function getFoldStatus(): FoldStatus;

  /**
   * Register the callback for fold status changes.
   *
   * @param { 'foldStatusChange' } type the event of fold status changes
   * @param { Callback<FoldStatus> } callback Callback used to return the current fold status of device
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Register the callback for fold status changes.
   *
   * @param { 'foldStatusChange' } type the event of fold status changes
   * @param { Callback<FoldStatus> } callback Callback used to return the current fold status of device
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function on(type: 'foldStatusChange', callback: Callback<FoldStatus>): void;

  /**
   * Unregister the callback for fold status changes.
   *
   * @param { 'foldStatusChange' } type the event of fold status changes
   * @param { Callback<FoldStatus> } callback Callback used to return the current fold status of device
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Unregister the callback for fold status changes.
   *
   * @param { 'foldStatusChange' } type the event of fold status changes
   * @param { Callback<FoldStatus> } callback Callback used to return the current fold status of device
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function off(type: 'foldStatusChange', callback?: Callback<FoldStatus>): void;

  /**
   * Register the callback for fold angle changes.
   *
   * @param { 'foldAngleChange' } type the event of fold angle changes.
   * @param { Callback<Array<number>> } callback Callback used to return the current fold angle of device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function on(type: 'foldAngleChange', callback: Callback<Array<number>>): void;

  /**
   * Unregister the callback for fold angle changes.
   *
   * @param { 'foldAngleChange' } type the event of fold angle changes.
   * @param { Callback<Array<number>> } callback Callback used to return the current fold angle of device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function off(type: 'foldAngleChange', callback?: Callback<Array<number>>): void;

  /**
   * Register the callback for device capture, casting, or recording status changes.
   *
   * @param { 'captureStatusChange' } type the event of capture, casting, or recording status changes.
   * @param { Callback<boolean> } callback Callback used to return the device capture, casting, or recording status.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function on(type: 'captureStatusChange', callback: Callback<boolean>): void;

  /**
   * Unregister the callback for device capture, casting, or recording status changes.
   *
   * @param { 'captureStatusChange' } type the event of capture, casting, or recording status changes.
   * @param { Callback<boolean> } callback Callback used to return the device capture, casting, or recording status .
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function off(type: 'captureStatusChange', callback?: Callback<boolean>): void;


  /**
   * Check whether the device is captured, projected, or recorded.
   *
   * @returns { boolean } true means the device is captured, projected, or recorded.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function isCaptured(): boolean;

  /**
   * Get the display mode of the foldable device.
   *
   * @returns { FoldDisplayMode } display mode of the foldable device.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Get the display mode of the foldable device.
   *
   * @returns { FoldDisplayMode } display mode of the foldable device.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function getFoldDisplayMode(): FoldDisplayMode;

  /**
   * Change the display mode of the foldable device.
   *
   * @param { FoldDisplayMode } mode target display mode to change.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function setFoldDisplayMode(mode: FoldDisplayMode): void;

  /**
   * Change the display mode of the foldable device.
   *
   * @param { FoldDisplayMode } mode target display mode to change.
   * @param { string } reason set display mode reason.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 19
   */
  function setFoldDisplayMode(mode: FoldDisplayMode, reason: string): void;

  /**
   * Register the callback for fold display mode changes.
   *
   * @param { 'foldDisplayModeChange' } type the event of fold display mode changes
   * @param { Callback<FoldDisplayMode> } callback Callback used to return the current fold display mode
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Register the callback for fold display mode changes.
   *
   * @param { 'foldDisplayModeChange' } type the event of fold display mode changes
   * @param { Callback<FoldDisplayMode> } callback Callback used to return the current fold display mode
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function on(type: 'foldDisplayModeChange', callback: Callback<FoldDisplayMode>): void;

  /**
   * Unregister the callback for fold display mode changes.
   *
   * @param { 'foldDisplayModeChange' } type the event of fold display mode changes
   * @param { Callback<FoldDisplayMode> } callback Callback used to return the current fold display mode
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Unregister the callback for fold display mode changes.
   *
   * @param { 'foldDisplayModeChange' } type the event of fold display mode changes
   * @param { Callback<FoldDisplayMode> } callback Callback used to return the current fold display mode
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function off(type: 'foldDisplayModeChange', callback?: Callback<FoldDisplayMode>): void;

  /**
   * Get the fold crease region in the current display mode.
   *
   * @returns { FoldCreaseRegion } fold crease region in the current display mode.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Get the fold crease region in the current display mode.
   *
   * @returns { FoldCreaseRegion } fold crease region in the current display mode.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  function getCurrentFoldCreaseRegion(): FoldCreaseRegion;

  /**
   * set fold status locked or not.
   *
   * @param { boolean } locked - fold status is locked or not.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function setFoldStatusLocked(locked: boolean): void;

    /**
   * Create virtual screen.
   *
   * @permission ohos.permission.ACCESS_VIRTUAL_SCREEN
   * @param { VirtualScreenConfig } config Indicates the options of the virtual screen.
   * @returns { Promise<number> } Promise used to return the created virtual screen id
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.function createVirtualScreen can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap  SystemCapability.Window.SessionManager
   * @since 16
   */
  function createVirtualScreen(config: VirtualScreenConfig): Promise<number>;

  /**
   * Destroy virtual screen.
   *
   * @permission ohos.permission.ACCESS_VIRTUAL_SCREEN
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.function destroyVirtualScreen can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap  SystemCapability.Window.SessionManager
   * @since 16
   */
  function destroyVirtualScreen(screenId: number): Promise<void>;
  
  /**
   * Set surface for the virtual screen.
   *
   * @permission ohos.permission.ACCESS_VIRTUAL_SCREEN
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @param { string } surfaceId Indicates the surface id.
   * @returns { Promise<void> } Promise that returns no value
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.function setVirtualScreenSurface can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap  SystemCapability.Window.SessionManager
   * @since 16
   */
  function setVirtualScreenSurface(screenId: number, surfaceId: string): Promise<void>;

  /**
   * Make screen as unique-screen
   *
   * @permission ohos.permission.ACCESS_VIRTUAL_SCREEN
   * @param { number } screenId Indicates the unique screen id. It's type should be int.
   * @returns { Promise<void> } Promise that returns no value
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.function makeUnique can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @since 16
   */
  function makeUnique(screenId: number): Promise<void>;

  /**
   * Add the list of window ids to the block list which won't display on the virtual screen
   *
   * @param { Array<number> } windowIds - The list of window ids that do not want to display on the virtual screen
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.Function addVirtualScreenBlocklist can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function addVirtualScreenBlocklist(windowIds: Array<number>): Promise<void>;

  /**
   * Remove the list of window ids from the block list which won't display on the virtual screen
   *
   * @param { Array<number> } windowIds - The list of window ids that want to display on the virtual screen
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.Function removeVirtualScreenBlocklist can not work correctly due to limited device capabilities.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function removeVirtualScreenBlocklist(windowIds: Array<number>): Promise<void>;

  /**
   * The parameter for creating virtual screen.
   *
   * @interface VirtualScreenConfig
   * @syscap SystemCapability.Window.SessionManager
   * @since 16
   */
  interface VirtualScreenConfig {
    /**
     * Indicates the name of the virtual screen.
     *
     * @type { string }
     * @syscap  SystemCapability.Window.SessionManager
     * @since 16
     */
    name: string;

    /**
     * Indicates the width of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.Window.SessionManager
     * @since 16
     */
    width: number;

    /**
     * Indicates the height of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.Window.SessionManager
     * @since 16
     */
    height: number;

    /**
     * Indicates the density of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.Window.SessionManager
     * @since 16
     */
    density: number;

    /**
     * Indicates the surface id of the virtual screen.
     *
     * @type { string }
     * @syscap SystemCapability.Window.SessionManager
     * @since 16
     */
    surfaceId: string;
  }

  /**
   * Enumerates the fold status. For dual-fold axis devices, when oriented with the charging port at the bottom, the hinges are identified from right to left as the first and second fold axes, respectively.
   *
   * @enum { number }
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Enumerates the fold status.For dual-fold axis devices, when oriented with the charging port at the bottom, the hinges are identified from right to left as the first and second fold axes, respectively.
   *
   * @enum { number }
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  enum FoldStatus {
    /**
     * Fold Status Unknown.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Fold Status Unknown.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_STATUS_UNKNOWN = 0,
    /**
     * Fold Status Expanded. For dual-fold axis devices, the first fold axis is fully open, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Fold Status Expanded. For dual-fold axis devices, the first fold axis is fully open, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_STATUS_EXPANDED,
    /**
     * Fold Status Folded. For dual-fold axis devices, the first fold axis is folded, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Fold Status Folded. For dual-fold axis devices, the first fold axis is folded, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_STATUS_FOLDED,
    /**
     * Fold Status Half Folded, somehow between fully open and completely closed. For dual-fold axis devices, the first fold axis is half-folded, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Fold Status Half Folded, somehow between fully open and completely closed. For dual-fold axis devices, the first fold axis is half-folded, and the second fold axis is folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_STATUS_HALF_FOLDED,
    /**
     * Fold Status Expanded With Second Expanded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_EXPANDED_WITH_SECOND_EXPANDED = 11,
    /**
     * Fold Status Expanded With Second Half Folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_EXPANDED_WITH_SECOND_HALF_FOLDED = 21,
    /**
     * Fold Status Folded With Second Expanded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_FOLDED_WITH_SECOND_EXPANDED = 12,
    /**
     * Fold Status Folded With Second Half Folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_FOLDED_WITH_SECOND_HALF_FOLDED = 22,
    /**
     * Fold Status Half Folded With Second Expanded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_HALF_FOLDED_WITH_SECOND_EXPANDED = 13,
    /**
     * Fold Status Half Folded With Second Half Folded.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 15
     */
    FOLD_STATUS_HALF_FOLDED_WITH_SECOND_HALF_FOLDED = 23
  }

  /**
   * Enumerates the fold display mode.
   *
   * @enum { number }
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Enumerates the fold display mode.
   *
   * @enum { number }
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  enum FoldDisplayMode {
    /**
     * Unknown Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Unknown Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_DISPLAY_MODE_UNKNOWN = 0,
    /**
     * Full Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Full Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_DISPLAY_MODE_FULL,
    /**
     * Main Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Main Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_DISPLAY_MODE_MAIN,
    /**
     * Sub Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Sub Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_DISPLAY_MODE_SUB,
    /**
     * Coordination Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Coordination Display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    FOLD_DISPLAY_MODE_COORDINATION
  }

  /**
   * Enumerates the display states.
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   */
  /**
   * Enumerates the display states.
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  enum DisplayState {
    /**
     * Unknown.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Unknown.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_UNKNOWN = 0,
    /**
     * Screen off.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Screen off.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_OFF,
    /**
     * Screen on.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Screen on.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_ON,
    /**
     * Doze, but it will update for some important system messages.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Doze, but it will update for some important system messages.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_DOZE,
    /**
     * Doze and not update.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Doze and not update.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_DOZE_SUSPEND,
    /**
     * VR node.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * VR node.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_VR,
    /**
     * Screen on and not update.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Screen on and not update.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    STATE_ON_SUSPEND
  }

  /**
   * Enumerates the display orientation.
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the display orientation.
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  enum Orientation {
    /**
     * Indicate that the display content is in portrait mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Indicate that the display content is in portrait mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    PORTRAIT = 0,

    /**
     * Indicate that the display content is in landscape mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Indicate that the display content is in landscape mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    LANDSCAPE = 1,

    /**
     * Indicate that the display content is in the opposite direction of the portrait mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Indicate that the display content is in the opposite direction of the portrait mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    PORTRAIT_INVERTED = 2,

    /**
     * Indicate that the display content is in the opposite direction of the landscape mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Indicate that the display content is in the opposite direction of the landscape mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    LANDSCAPE_INVERTED = 3
  }

    /**
     * Indicate the source mode of the display
     *
     * @enum { number }
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
  enum DisplaySourceMode {
    /**
     * Indicate that the display is not in use.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    NONE = 0,
 
    /**
     * Indicate that the display is the main display.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    MAIN = 1,

    /**
     * Indicate that the display is in mirror mode.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    MIRROR = 2,

    /**
     * Indicate that the display is in extend mode.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    EXTEND = 3,

    /**
     * Indicate that the display stands alone.
     *
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    ALONE = 4
  }

  /**
   * Enumerates the screen shape.
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @since 18
   */
  enum ScreenShape {
    /**
     *The screen shape is rectangle.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 18
     */
    RECTANGLE = 0,
    /**
     *The screen shape is round.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 18
     */
    ROUND = 1
  }

  /**
   * Fold Crease Region
   *
   * @interface FoldCreaseRegion
   * @syscap SystemCapability.Window.SessionManager
   * @since 10
   */
  /**
   * Fold Crease Region
   *
   * @interface FoldCreaseRegion
   * @syscap SystemCapability.Window.SessionManager
   * @atomicservice
   * @since 12
   */
  interface FoldCreaseRegion {
    /**
     * The display ID is used to identify the screen where the crease is located.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * The display ID is used to identify the screen where the crease is located.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    readonly displayId: number;

    /**
     * Crease Region.
     *
     * @type { Array<Rect> }
     * @readonly
     * @syscap SystemCapability.Window.SessionManager
     * @since 10
     */
    /**
     * Crease Region.
     *
     * @type { Array<Rect> }
     * @readonly
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    readonly creaseRects: Array<Rect>;
  }

  /**
   * Rectangle
   *
   * @interface Rect
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Rectangle
   *
   * @interface Rect
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  interface Rect {
    /**
     * The X-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * The X-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    left: number;

    /**
     * The Y-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * The Y-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    top: number;

    /**
     * Width of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Width of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    width: number;

    /**
     * Height of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Height of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    height: number;
  }

  /**
   * Curved area rects of the waterfall display.
   *
   * @interface WaterfallDisplayAreaRects
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Curved area rects of the waterfall display.
   *
   * @interface WaterfallDisplayAreaRects
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  interface WaterfallDisplayAreaRects {
    /**
     * Indicates the size of left side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Indicates the size of left side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly left: Rect;

    /**
     * Indicates the size of right side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Indicates the size of right side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly right: Rect;

    /**
     * Indicates the size of top side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Indicates the size of top side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly top: Rect;

    /**
     * Indicates the size of bottom side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Indicates the size of bottom side curved area of the waterfall screen.
     *
     * @type { Rect }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly bottom: Rect;
  }

  /**
   * Cutout information of the display.
   *
   * @interface CutoutInfo
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 9
   */
  /**
   * Cutout information of the display.
   *
   * @interface CutoutInfo
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @atomicservice
   * @since 12
   */
  interface CutoutInfo {
    /**
     * Bounding rectangles of the cutout areas of the display.
     *
     * @type { Array<Rect> }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Bounding rectangles of the cutout areas of the display.
     *
     * @type { Array<Rect> }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly boundingRects: Array<Rect>;

    /**
     * Rectangles of curved parts on each side of a waterfall display.
     *
     * @type { WaterfallDisplayAreaRects }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Rectangles of curved parts on each side of a waterfall display.
     *
     * @type { WaterfallDisplayAreaRects }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    readonly waterfallDisplayAreaRects: WaterfallDisplayAreaRects;
  }

  /**
   * Define display physical resolution.
   *
   * @interface DisplayPhysicalResolution
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  interface DisplayPhysicalResolution {
    /**
     * fold display mode.
     *
     * @type { FoldDisplayMode }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    foldDisplayMode: FoldDisplayMode;

    /**
     * Display physical width, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    physicalWidth: number;

    /**
     * Display physical height, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    physicalHeight: number;
  }

  /**
   * Define properties of the display. They cannot be updated automatically.
   *
   * @interface Display
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @since 7
   */
  /**
   * Define properties of the display. They cannot be updated automatically.
   *
   * @interface Display
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @since 10
   */
  /**
   * Define properties of the display. They cannot be updated automatically.
   *
   * @interface Display
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface Display {
    /**
     * Display ID.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display ID.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Display ID.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    id: number;

    /**
     * Display name.
     *
     * @type { string }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display name.
     *
     * @type { string }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    name: string;

    /**
     * Whether the display is alive.
     *
     * @type { boolean }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Whether the display is alive.
     *
     * @type { boolean }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    alive: boolean;

    /**
     * The state of display.
     *
     * @type { DisplayState }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * The state of display.
     *
     * @type { DisplayState }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    state: DisplayState;

    /**
     * Refresh rate, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Refresh rate, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    refreshRate: number;

    /**
     * Rotation degrees of the display.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * An enumeration value for rotation degrees of the display.
     * The value 0 indicates that the screen of the display rotates clockwise by 0°.
     * The value 1 indicates that the screen of the display rotates clockwise by 90°.
     * The value 2 indicates that the screen of the display rotates clockwise by 180°.
     * The value 3 indicates that the screen of the display rotates clockwise by 270°.
     * 
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 11
     */
    rotation: number;

    /**
     * Display width, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display width, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Display width, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    width: number;

    /**
     * Display height, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display height, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Display height, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    height: number;

    /**
     * Display available width, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    availableWidth: number;

    /**
     * Display available height, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    availableHeight: number;

    /**
     * Display resolution, that is, the number of pixels per inch.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display resolution, that is, the number of pixels per inch.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 11
     */
    /**
     * Display resolution, that is, the number of pixels per inch.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    densityDPI: number;

    /**
     * Display orientation.
     *
     * @type { Orientation }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 10
     */
    /**
     * Display orientation.
     *
     * @type { Orientation }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    orientation: Orientation;

    /**
     * Display density, in pixels. which is the scaling coefficient between physical pixels and logical pixels. The value for a low-resolution display is 1.0.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Display density, in pixels. which is the scaling coefficient between physical pixels and logical pixels. The value for a low-resolution display is 1.0.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    densityPixels: number;

    /**
     * Text scale density of the display.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * Text scale density of the display.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @since 11
     */
    /**
     * Text scale density of the display.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    scaledDensity: number;

    /**
     * DPI on the x-axis.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * DPI on the x-axis.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    xDPI: number;

    /**
     * DPI on the y-axis.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 7
     */
    /**
     * DPI on the y-axis.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    yDPI: number;

    /**
     * The shape of screen
     *
     * @type { ?ScreenShape }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 18
     */
    screenShape?: ScreenShape;

    /**
     * All supported color spaces.
     *
     * @type { Array<colorSpaceManager.ColorSpace> }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 11
     */
    /**
     * All supported color spaces.
     *
     * @type { Array<colorSpaceManager.ColorSpace> }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    colorSpaces: Array<colorSpaceManager.ColorSpace>;

    /**
     * All supported HDR formats.
     *
     * @type { Array<hdrCapability.HDRFormat> }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 11
     */
    /**
     * All supported HDR formats.
     *
     * @type { Array<hdrCapability.HDRFormat> }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    hdrFormats: Array<hdrCapability.HDRFormat>;

    /**
     * The source mode of display
     *
     * @type { ?DisplaySourceMode }
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    sourceMode?: DisplaySourceMode;
 
    /**
     * Coordinates of the top-left corner of the display relative to the main display.
     *
     * @type { ?number }
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    x?: number;
  
    /**
     * Coordinates of the top-left corner of the display relative to the main display.
     *
     * @type { ?number }
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 19
     */
    y?: number;

    /**
     * Obtain the cutout info of the display.
     *
     * @param { AsyncCallback<CutoutInfo> } callback
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Obtain the cutout info of the display.
     *
     * @param { AsyncCallback<CutoutInfo> } callback
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    getCutoutInfo(callback: AsyncCallback<CutoutInfo>): void;

    /**
     * Obtain the cutout info of the display.
     *
     * @returns { Promise<CutoutInfo> }
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @since 9
     */
    /**
     * Obtain the cutout info of the display.
     *
     * @returns { Promise<CutoutInfo> }
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @atomicservice
     * @since 12
     */
    getCutoutInfo(): Promise<CutoutInfo>;

    /**
     * Check if current display has immersive window.
     *
     * @param { AsyncCallback<boolean> } callback
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.Window.SessionManager
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    hasImmersiveWindow(callback: AsyncCallback<boolean>): void;

    /**
     * Check if current display has immersive window.
     *
     * @returns { Promise<boolean> }
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.Window.SessionManager
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    hasImmersiveWindow(): Promise<boolean>;

    /**
     * Obtain the available area of the display.
     *
     * @returns { Promise<Rect> }
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @throws { BusinessError } 1400001 - Invalid display or screen.
     * @syscap SystemCapability.Window.SessionManager
     * @atomicservice
     * @since 12
     */
    getAvailableArea(): Promise<Rect>;

    /**
    * Register the callback for available area changes.
    *
    * @param { 'availableAreaChange' } type - the event of available area changes
    * @param { Callback<Rect> } callback - Callback used to return the available area
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
    * <br>2. Incorrect parameter types.
    * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
    * @throws { BusinessError } 1400003 - This display manager service works abnormally.
    * @syscap SystemCapability.Window.SessionManager
    * @atomicservice
    * @since 12
    */
    on(type: 'availableAreaChange', callback: Callback<Rect>): void;

    /**
    * Unregister the callback for available area changes.
    *
    * @param { 'availableAreaChange' } type - the event of available area changes
    * @param { Callback<Rect> } [callback] - Callback used to return the available area
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
    * <br>2. Incorrect parameter types.
    * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
    * @throws { BusinessError } 1400003 - This display manager service works abnormally.
    * @syscap SystemCapability.Window.SessionManager
    * @atomicservice
    * @since 12
    */
    off(type: 'availableAreaChange', callback?: Callback<Rect>): void;

    /**
    * Get current display capability, including foldstatus, displaymode, rotation, and orientation information.
    *
    * @returns { string } Indicates the current foldstatus, displaymode, rotation, and orientation information.
    * @throws { BusinessError } 801 - Capability not supported.Function getDisplayCapability can not work correctly due to limited device capabilities.
    * @throws { BusinessError } 1400001 - Invalid display or screen.
    * @throws { BusinessError } 1400003 - This display manager service works abnormally.
    * @syscap SystemCapability.Window.SessionManager
    * @atomicservice
    * @since 18
    * @test
    */
    getDisplayCapability(): string;
  }
}

export default display;
