/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

import { AsyncCallback, Callback } from './@ohos.base';
import image from './@ohos.multimedia.image';

/**
 * Interface of screen manager
 *
 * @namespace screen
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @systemapi Hide this for inner system use.
 * @since 9
 */
declare namespace screen {
  /**
   * Get all screen instances, then can get or set detail information.
   *
   * @param { AsyncCallback<Array<Screen>> } callback the callback of all screens info
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getAllScreens(callback: AsyncCallback<Array<Screen>>): void;

  /**
   * Get all screen instances, then can get or set detail information.
   *
   * @returns { Promise<Array<Screen>> } the result of all screens info
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getAllScreens(): Promise<Array<Screen>>;

  /**
   * Register the callback for screen changes.
   *
   * @param { 'connect' | 'disconnect' | 'change' } eventType the event of screen changes. This parameter is of string
   * type and cannot be empty.
   * @param { Callback<number> } callback Callback used to return the screen ID. This parameter is callable.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function on(eventType: 'connect' | 'disconnect' | 'change', callback: Callback<number>): void;

  /**
   * Unregister the callback for screen changes.
   *
   * @param { 'connect' | 'disconnect' | 'change' } eventType the event of screen changes. This parameter is of string
   * type and cannot be empty.
   * @param { Callback<number> } callback Callback used to return the screen ID. If this parameter is specified, it must
   * be a callback.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function off(eventType: 'connect' | 'disconnect' | 'change', callback?: Callback<number>): void;

  /**
   * Make screens as expand-screen
   *
   * @param { Array<ExpandOption> } options Parameters for expanding the screen. The options must be valid, and make
   * sure it's type of Array<ExpandOption>.
   * @param { AsyncCallback<number> } callback callback used to return the group ID of the expanded screens. It must
   * be a callback.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function makeExpand(options: Array<ExpandOption>, callback: AsyncCallback<number>): void;

  /**
   * Make screens as expand-screen
   *
   * @param { Array<ExpandOption> } options Parameters for expanding the screen. The options must be valid, and make
   * sure it's type of Array<ExpandOption>.
   * @returns { Promise<number> } used to return the group ID of the expanded screens
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function makeExpand(options: Array<ExpandOption>): Promise<number>;

  /**
   * Stop expand screens
   *
   * @param { Array<number> } expandScreen IDs of expand screens to stop. The size of the expandScreen Array should not
   * exceed 1000.
   * @param { AsyncCallback<void> } callback used to return the result
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopExpand(expandScreen: Array<number>, callback: AsyncCallback<void>): void;

  /**
   * Stop expand screens
   *
   * @param { Array<number> } expandScreen IDs of expand screens to stop. The size of the expandScreen Array should not
   * exceed 1000.
   * @returns { Promise<void> } promise used to return the result
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopExpand(expandScreen: Array<number>): Promise<void>;

  /**
   * Make screens as mirror-screen
   *
   * @param { number } mainScreen ID of the primary screen. It's type should be int.
   * @param { Array<number> } mirrorScreen IDs of secondary screens
   * @param { AsyncCallback<number> } callback Callback used to return the group ID of the secondary screens
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function makeMirror(mainScreen: number, mirrorScreen: Array<number>, callback: AsyncCallback<number>): void;

  /**
   * Make screens as mirror-screen
   *
   * @param { number } mainScreen ID of the primary screen. It's type should be int.
   * @param { Array<number> } mirrorScreen IDs of secondary screens
   * @returns { Promise<number> } Promise used to return the group ID of the secondary screens
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function makeMirror(mainScreen: number, mirrorScreen: Array<number>): Promise<number>;

  /**
   * Make screens as mirror-screen
   *
   * @param { number } mainScreen ID of the primary screen. It's type should be int.
   * @param { Array<number> } mirrorScreen IDs of secondary screens
   * @param { Rect } mainScreenRegion mirror screen region
   * @returns { Promise<number> } Promise used to return the group ID of the secondary screens
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 19
   */
  function makeMirrorWithRegion(mainScreen: number, mirrorScreen: Array<number>, mainScreenRegion: Rect): Promise<number>;

  /**
   * Stop mirror screens
   *
   * @param { Array<number> } mirrorScreen IDs of mirror screens to stop. The size of the mirrorScreen Array should not
   * exceed 1000.
   * @param { AsyncCallback<void> } callback used to return the result
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopMirror(mirrorScreen: Array<number>, callback: AsyncCallback<void>): void;

  /**
   * Stop mirror screens
   *
   * @param { Array<number> } mirrorScreen IDs of mirror screens to stop. The size of the mirrorScreen Array should not
   * exceed 1000.
   * @returns { Promise<void> } promise used to return the result
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function stopMirror(mirrorScreen: Array<number>): Promise<void>;
  
  /**
   * Make screens as unique-screen
   *
   * @param { Array<number> } uniqueScreen IDs of the unique screens. It's type should be int.
   * @returns { Promise<Array<number>> } Promise used to return the display IDs of unique screens.
   * @throws { BusinessError } 202 - Permission verification failed, non-system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function makeUnique(uniqueScreen: Array<number>): Promise<Array<number>>;

  /**
   * Create virtual screen. if surfaceId is valid, this permission is necessary.
   *
   * @permission ohos.permission.CAPTURE_SCREEN
   * @param { VirtualScreenOption } options Indicates the options of the virtual screen.
   * @param { AsyncCallback<Screen> } callback Callback used to return the created virtual screen
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function createVirtualScreen(options: VirtualScreenOption, callback: AsyncCallback<Screen>): void;

  /**
   * Create virtual screen. if surfaceId is valid, this permission is necessary.
   *
   * @permission ohos.permission.CAPTURE_SCREEN
   * @param { VirtualScreenOption } options Indicates the options of the virtual screen.
   * @returns { Promise<Screen> } Promise used to return the created virtual screen
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function createVirtualScreen(options: VirtualScreenOption): Promise<Screen>;

  /**
   * Destroy virtual screen.
   *
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @param { AsyncCallback<void> } callback Callback used to return the result.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400002 - Unauthorized operation.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function destroyVirtualScreen(screenId: number, callback: AsyncCallback<void>): void;

  /**
   * Destroy virtual screen.
   *
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400002 - Unauthorized operation.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function destroyVirtualScreen(screenId: number): Promise<void>;

  /**
   * Set surface for the virtual screen.
   *
   * @permission ohos.permission.CAPTURE_SCREEN
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @param { string } surfaceId Indicates the surface id.
   * @param { AsyncCallback<void> } callback Callback used to return the result
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setVirtualScreenSurface(screenId: number, surfaceId: string, callback: AsyncCallback<void>): void;

  /**
   * Set surface for the virtual screen.
   *
   * @permission ohos.permission.CAPTURE_SCREEN
   * @param { number } screenId Indicates the screen id of the virtual screen.
   * @param { string } surfaceId Indicates the surface id.
   * @returns { Promise<void> } Promise that returns no value
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setVirtualScreenSurface(screenId: number, surfaceId: string): Promise<void>;

  /**
   * Set privacy mask image for the screen.
   *
   * @param { number } screenId Indicates the screen id of the screen.
   * @param { image.PixelMap } image Indicates the privacy mask image. This parameter is optional. If not provided,
   * the mask image will be cleared;
   * @returns { Promise<void> } Promise that returns no value
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
   * @throws { BusinessError } 1400001 - Invalid display or screen.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.Window.SessionManager
   * @systemapi Hide this for inner system use.
   * @since 19
   */
  function setScreenPrivacyMaskImage(screenId: number, image?: image.PixelMap): Promise<void>;

  /**
   * Get screen rotation lock status.
   *
   * @param { AsyncCallback<boolean> } callback If true, auto rotate is locked. If false, auto rotate is unlocked
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isScreenRotationLocked(callback: AsyncCallback<boolean>): void;

  /**
   * Get screen rotation lock status.
   *
   * @returns { Promise<boolean> } If true, auto rotate is locked. If false, auto rotate is unlocked
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isScreenRotationLocked(): Promise<boolean>;

  /**
   * Set screen rotation lock status.
   *
   * @param { boolean } isLocked Indicates whether the screen rotation switch is locked.
   * @param { AsyncCallback<void> } callback Callback used to return the result.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setScreenRotationLocked(isLocked: boolean, callback: AsyncCallback<void>): void;

  /**
   * Set screen rotation lock status.
   *
   * @param { boolean } isLocked Indicates whether the screen rotation switch is locked.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setScreenRotationLocked(isLocked: boolean): Promise<void>;

  /**
   * Set multi screen mode(mirror/extend).
   *
   * @param { number } primaryScreenId - primary screen id.
   * @param { number } secondaryScreenId - secondary screen id.
   * @param { MultiScreenMode } secondaryScreenMode - secondary screen mode.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 202 - Permission verification failed, non-system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *                                                                   2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 13
   */
  function setMultiScreenMode(primaryScreenId: number, secondaryScreenId: number,
    secondaryScreenMode: MultiScreenMode): Promise<void>;

    /**
   * Set multi screen relative position.
   *
   * @param { MultiScreenPositionOptions } mainScreenOptions - main screen position.
   * @param { MultiScreenPositionOptions } secondaryScreenOptions - secondary screen position.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 202 - Permission verification failed, non-system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *                                                                   2. Incorrect parameter types.
   * @throws { BusinessError } 1400003 - This display manager service works abnormally.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 13
   */
  function setMultiScreenRelativePosition(mainScreenOptions: MultiScreenPositionOptions,
    secondaryScreenOptions: MultiScreenPositionOptions): Promise<void>;

  /**
   * Indicate the screen mode
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 13
   */
  enum MultiScreenMode {

    /**
     * Indicate that the screen is in mirror mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 13
     */
    SCREEN_MIRROR = 0,

    /**
     * Indicate that the screen is in extend mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 13
     */
    SCREEN_EXTEND = 1
  }

  /**
   * The parameter of making extend screen
   *
   * @interface MultiScreenPositionOptions
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 13
   */
  interface MultiScreenPositionOptions {
    /**
     * Screen id
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 13
     */
    id: number;

    /**
     * The start coordinate X of the screen origin
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 13
     */
    startX: number;

    /**
     * The start coordinate Y of the screen origin
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 13
     */
    startY: number;
  }

  /**
   * The parameter of making expand screen
   *
   * @interface ExpandOption
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface ExpandOption {
    /**
     * Screen id
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    screenId: number;

    /**
     * The start coordinate X of the screen origin
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    startX: number;

    /**
     * The start coordinate Y of the screen origin
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    startY: number;
  }

  /**
   * The parameter for creating virtual screen.
   *
   * @interface VirtualScreenOption
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface VirtualScreenOption {
    /**
     * Indicates the name of the virtual screen.
     *
     * @type { string }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    name: string;

    /**
     * Indicates the width of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    width: number;

    /**
     * Indicates the height of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    height: number;

    /**
     * Indicates the density of the virtual screen.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    density: number;

    /**
     * Indicates the surface id of the virtual screen.
     *
     * @type { string }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    surfaceId: string;
  }

  /**
   * Indicate the source mode of the screen
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  enum ScreenSourceMode {
    /**
     * Indicate that the screen is the default screen.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SCREEN_MAIN = 0,

    /**
     * Indicate that the screen is in mirror mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SCREEN_MIRROR = 1,

    /**
     * Indicate that the screen is in extend mode.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SCREEN_EXTEND = 2,

    /**
     * Indicate that the screen stands alone.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SCREEN_ALONE = 3
  }

  /**
   * Interface for screen
   *
   * @interface Screen
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface Screen {
    /**
     * Screen id
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly id: number;

    /**
     * Group id
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly parent: number;

    /**
     * Mode supported by the screen
     *
     * @type { Array<ScreenModeInfo> }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly supportedModeInfo: Array<ScreenModeInfo>;

    /**
     * Currently active mode
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly activeModeIndex: number;

    /**
     * Orientation of the screen
     *
     * @type { Orientation }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    readonly orientation: Orientation;

    /**
     * Source mode of the screen
     *
     * @type { ScreenSourceMode }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    readonly sourceMode: ScreenSourceMode;

    /**
     * Screen Serial Number
     *
     * @type { ?string }
     * @readonly
     * @syscap SystemCapability.WindowManager.WindowManager
     * @systemapi Hide this for inner system use.
     * @since 15
     */
    readonly serialNumber?: string;
    
    /**
     * Set the orientation of the screen
     *
     * @param { Orientation } orientation Screen orientation. orientation value must from enum Orientation.
     * @param { AsyncCallback<void> } callback Callback used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setOrientation(orientation: Orientation, callback: AsyncCallback<void>): void;

    /**
     * Set the orientation of the screen
     *
     * @param { Orientation } orientation Screen orientation. orientation value must from enum Orientation.
     * @returns { Promise<void> } Promise that returns no value.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types. 3. Parameter verification failed.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setOrientation(orientation: Orientation): Promise<void>;

    /**
     * Set the active mode of the screen.
     *
     * @param { number } modeIndex Index of the mode to set.
     * @param { AsyncCallback<void> } callback Callback used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setScreenActiveMode(modeIndex: number, callback: AsyncCallback<void>): void;

    /**
     * Sets the active mode of the screen.
     *
     * @param { number } modeIndex Index of the mode to set.
     * @returns { Promise<void> } Promise that returns no value.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setScreenActiveMode(modeIndex: number): Promise<void>;

    /**
     * Set display density of the screen
     *
     * @param { number } densityDpi Pixel density. The value ranges from 80 to 640.
     * @param { AsyncCallback<void> } callback Callback used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setDensityDpi(densityDpi: number, callback: AsyncCallback<void>): void;

    /**
     * Set display density of the screen
     *
     * @param { number } densityDpi Pixel density. The value ranges from 80 to 640.
     * @returns { Promise<void> } Promise that returns no value.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * @throws { BusinessError } 1400003 - This display manager service works abnormally.
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    setDensityDpi(densityDpi: number): Promise<void>;
  }

  /**
   * Screen orientation
   *
   * @enum { number }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  enum Orientation {
    /**
     * Indicates that the orientation of the screen is unspecified.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    UNSPECIFIED = 0,

    /**
     * Indicates that the orientation of the screen is vertical.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    VERTICAL = 1,

    /**
     * Indicates that the orientation of the screen is horizontal.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    HORIZONTAL = 2,

    /**
     * Indicates that the orientation of the screen is reverse_vertical.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    REVERSE_VERTICAL = 3,

    /**
     * Indicates that the orientation of the screen is reverse_horizontal.
     *
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    REVERSE_HORIZONTAL = 4
  }

  /**
   * The information of the screen mode
   *
   * @interface ScreenModeInfo
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface ScreenModeInfo {
    /**
     * Mode id
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    id: number;

    /**
     * Indicates the width of the screen
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    width: number;

   /**
     * Indicates the height of the screen
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    height: number;

    /**
     * Indicates the refreshRate of the screen
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    refreshRate: number;
  }

  /**
   * Rectangle
   *
   * @interface Rect
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi Hide this for inner system use.
   * @since 19
   */
  interface Rect {
    /**
     * The X-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    left: number;

    /**
     * The Y-axis coordinate of the upper left vertex of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    top: number;

    /**
     * Width of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    width: number;

    /**
     * Height of the rectangle, in pixels.
     *
     * @type { number }
     * @syscap SystemCapability.WindowManager.WindowManager.Core
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    height: number;
  }
}

export default screen;
