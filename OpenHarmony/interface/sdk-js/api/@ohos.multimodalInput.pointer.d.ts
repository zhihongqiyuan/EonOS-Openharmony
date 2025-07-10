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
 * @kit InputKit
 */

import type { AsyncCallback } from './@ohos.base';
import type image from './@ohos.multimedia.image';

/**
 * The pointer module provides APIs related to pointer attribute management, such as querying and setting pointer attributes.
 *
 * @namespace pointer
 * @syscap SystemCapability.MultimodalInput.Input.Pointer
 * @since 9
 */
/**
 * The pointer module provides APIs related to pointer attribute management, such as querying and setting pointer attributes.
 *
 * @namespace pointer
 * @syscap SystemCapability.MultimodalInput.Input.Pointer
 * @atomicservice
 * @since 12
 */
declare namespace pointer {
  /**
   * Enumerates mouse pointer styles.
   *
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  /**
   * Enumerates mouse pointer styles.
   *
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @atomicservice
   * @since 12
   */
  enum PointerStyle {
    /**
     * Default
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    DEFAULT,

    /**
     * East arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    EAST,

    /**
     * West arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    WEST,

    /**
     * South arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    SOUTH,

    /**
     * North arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH,

    /**
     * East-west arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    WEST_EAST,

    /**
     * North-south arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH_SOUTH,

    /**
     * North-east arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH_EAST,

    /**
     * North-west arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH_WEST,

    /**
     * South-east arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    SOUTH_EAST,

    /**
     * South-west arrow
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    SOUTH_WEST,

    /**
     * Northeast and southwest adjustment
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH_EAST_SOUTH_WEST,

    /**
     * Northwest and southeast adjustment
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    NORTH_WEST_SOUTH_EAST,

    /**
     * Cross (accurate selection)
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    CROSS,

    /**
     * Copy
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    CURSOR_COPY,

    /**
     * Forbid
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    CURSOR_FORBID,

    /**
     * Sucker
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    COLOR_SUCKER,

    /**
     * Grabbing hand
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    HAND_GRABBING,

    /**
     * Opening hand
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    HAND_OPEN,

    /**
     * Hand-shaped pointer
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    HAND_POINTING,

    /**
     * Help
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    HELP,

    /**
     * Move
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MOVE,

    /**
     * Left and right resizing
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    RESIZE_LEFT_RIGHT,

    /**
     * Up and down resizing
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    RESIZE_UP_DOWN,

    /**
     * Screenshot crosshair
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    SCREENSHOT_CHOOSE,

    /**
     * Screenshot
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    SCREENSHOT_CURSOR,

    /**
     * Text selection
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    TEXT_CURSOR,

    /**
     * Zoom in
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    ZOOM_IN,

    /**
     * Zoom out
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    ZOOM_OUT,

    /**
     * Scrolling east
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_EAST,

    /**
     * Scrolling west
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_WEST,

    /**
     * Scrolling south
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_SOUTH,

    /**
     * Scrolling north
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_NORTH,

    /**
     * Scrolling north and south
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_NORTH_SOUTH,

    /**
     * Scrolling northeast
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_NORTH_EAST,

    /**
     * Scrolling northwest
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_NORTH_WEST,

    /**
     * Scrolling southeast
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_SOUTH_EAST,

    /**
     * Scrolling southwest
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_SOUTH_WEST,

    /**
     * Moving as a cone in four directions
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 9
     */
    MIDDLE_BTN_NORTH_SOUTH_WEST_EAST,

    /**
     * Horizontal text selection
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    HORIZONTAL_TEXT_CURSOR,

    /**
     * Precise selection
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    CURSOR_CROSS,

    /**
     * Cursor with circle style
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    CURSOR_CIRCLE,

    /**
     * Loading state with dynamic cursor
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    /**
     * Loading state with dynamic cursor
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @atomicservice
     * @since 12
     */
    LOADING,

    /**
     * Running state with dynamic cursor
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    /**
     * Running state with dynamic cursor
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @atomicservice
     * @since 12
     */
    RUNNING,

    /**
     * Scrolling east and west
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 18
     */
    MIDDLE_BTN_EAST_WEST
  }

  /**
   * Mouse button.
   *
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  enum PrimaryButton {
    /**
     * Left mouse button
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    LEFT = 0,
    /**
     * Right mouse button
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    RIGHT = 1
  }

  /**
   * Enumerates shortcut menu triggering modes.
   *
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  enum RightClickType {
    /**
     * Tapping the right-button area of the touchpad.
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    TOUCHPAD_RIGHT_BUTTON = 1,
    /**
     * Tapping the left-button area of the touchpad.
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    TOUCHPAD_LEFT_BUTTON = 2,
    /**
     * Tapping or pressing the touchpad with two fingers.
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 10
     */
    TOUCHPAD_TWO_FINGER_TAP = 3,
    /**
     * Touchpad two fingers tap or right button
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 20
     */
    TOUCHPAD_TWO_FINGER_TAP_OR_RIGHT_BUTTON = 4,

      /**
     * Touchpad two fingers tap or left button
     *
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 20
     */
    TOUCHPAD_TWO_FINGER_TAP_OR_LEFT_BUTTON = 5,
  }

  /**
   * Pixel map resource.
   *
   * @interface CustomCursor
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 15
   */
  interface CustomCursor {
    /**
     * Defines a custom cursor. The minimum size is subject to the minimum limit of the image. The maximum size is 256 x 256 px.
     * 
     * @type { image.PixelMap }
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 15
     */
    pixelMap: image.PixelMap;
    /**
     * Horizontal coordinate of the cursor focus. The coordinates are restricted by the size of the custom cursor. 
     * The minimum value is 0, and the maximum value is the maximum width of the image. The default value is 0 if the parameter is left empty.
     * 
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 15
     */
    focusX?: number;
    /**
     * Vertical coordinate of the cursor focus. The coordinates are restricted by the size of the custom cursor. 
     * The minimum value is 0, and the maximum value is the maximum height of the image. The default value is 0 if the parameter is left empty.
     * 
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 15
     */
    focusY?: number;
  }
  
  /**
   * Defines the custom cursor configuration.
   *
   * @interface CursorConfig
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 15
   */
  interface CursorConfig {
    /**
     * Whether to adjust the cursor size based on system settings. 
     * The value true means to adjust the cursor size based on system settings, and the value false means to use the custom cursor size. 
     * The adjustment range is [size of the cursor image, 256 x 256].
     * 
     * @type { boolean }
     * @syscap SystemCapability.MultimodalInput.Input.Pointer
     * @since 15
     */
    followSystem : boolean;
  }

  /**
   * Sets the moving speed of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { number } speed - Moving speed of the mouse pointer. The value ranges from 1 to 20. The default value is 10.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Sets the moving speed of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { number } speed - Moving speed of the mouse pointer. The value ranges from 1 to 20. The default value is 10.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 12
   */
  function setPointerSpeed(speed: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the moving speed of the mouse pointer. This API uses a promise to return the result.
   *
   * @param { number } speed - Moving speed of the mouse pointer. The value ranges from 1 to 20. The default value is 10.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Sets the moving speed of the mouse pointer. This API uses a promise to return the result.
   *
   * @param { number } speed - Moving speed of the mouse pointer. The value ranges from 1 to 20. The default value is 10.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 12
   */
  function setPointerSpeed(speed: number): Promise<void>;

  /**
   * Sets the moving speed of the mouse pointer. This API returns the result synchronously.
   *
   * @param { number } speed - Moving speed of the mouse pointer. The value ranges from 1 to 20. The default value is 10.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 10
   */
  function setPointerSpeedSync(speed: number): void;

  /**
   * Obtains the moving speed of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Obtains the moving speed of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 12
   */
  function getPointerSpeed(callback: AsyncCallback<number>): void;

  /**
   * Obtains the moving speed of the mouse pointer. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Obtains the moving speed of the mouse pointer. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 12
   */
  function getPointerSpeed(): Promise<number>;

  /**
   * Obtains the moving speed of the mouse pointer. This API returns the result synchronously.
   * 
   * @returns { number } Returns the pointer speed through sync mode.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use
   * @since 10
   */
  function getPointerSpeedSync(): number;

  /**
   * Sets the mouse pointer style. This API uses an asynchronous callback to return the result.
   *
   * @param { number } windowId - Window ID.
   * @param { PointerStyle } pointerStyle - Pointer style.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function setPointerStyle(windowId: number, pointerStyle: PointerStyle, callback: AsyncCallback<void>): void;

  /**
   * Sets the mouse pointer style. This API uses a promise to return the result.
   *
   * @param { number } windowId - Window ID.
   * @param { PointerStyle } pointerStyle - Pointer style.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function setPointerStyle(windowId: number, pointerStyle: PointerStyle): Promise<void>;

  /**
   * Sets the mouse pointer style. This API returns the result synchronously.
   *
   * @param { number } windowId - Window ID.
   * @param { PointerStyle } pointerStyle - Pointer style.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  function setPointerStyleSync(windowId: number, pointerStyle: PointerStyle): void;

  /**
   * Obtains the mouse pointer style. This API uses an asynchronous callback to return the result.
   *
   * @param { number } windowId - Window ID.
   * @param { AsyncCallback<PointerStyle> } callback - Callback used to return the mouse pointer style.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function getPointerStyle(windowId: number, callback: AsyncCallback<PointerStyle>): void;

  /**
   * Obtains the mouse pointer style. This API uses a promise to return the result.
   *
   * @param { number } windowId - Window ID.
   * @returns { Promise<PointerStyle> } Promise used to return the mouse pointer style.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function getPointerStyle(windowId: number): Promise<PointerStyle>;

  /**
   * Obtains the mouse pointer style, such as the east arrow, west arrow, south arrow, and north arrow. This API returns the result synchronously.
   *
   * @param { number } windowId - Window ID. The default value is -1, indicating the global mouse pointer style.
   * @returns { PointerStyle } Returns the pointerStyle.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  function getPointerStyleSync(windowId: number): PointerStyle;

  /**
   * Sets the visible status of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } visible Whether the mouse pointer is visible. The value true indicates that the pointer
   * icon is visible, and the value false indicates the opposite.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  /**
   * Sets the visible status of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } visible Whether the mouse pointer is visible. The value true indicates that the pointer
   * icon is visible, and the value false indicates the opposite.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported;
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 18
   */
  function setPointerVisible(visible: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the visible status of the mouse pointer. This API uses a promise to return the result.
   *
   * @param { boolean } visible Whether the mouse pointer is visible. The value true indicates that the pointer
   * icon is visible, and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  /**
   * Sets the visible status of the mouse pointer. This API uses a promise to return the result.
   *
   * @param { boolean } visible Whether the mouse pointer is visible. The value true indicates that the pointer
   * icon is visible, and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported;
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 18
   */
  function setPointerVisible(visible: boolean): Promise<void>;

  /**
   * Sets the visible status of the mouse pointer. This API returns the result synchronously.
   *
   * @param { boolean } visible Whether the mouse pointer is visible. The value true indicates that the pointer
   * icon is visible, and the value false indicates the opposite.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  function setPointerVisibleSync(visible: boolean): void;

  /**
   * Checks the visible status of the mouse pointer. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the mouse pointer is visible, and the value false indicates the opposite.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function isPointerVisible(callback: AsyncCallback<boolean>): void;

  /**
   * Checks the visible status of the mouse pointer. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the visible status of the mouse pointer. 
   * The value true indicates that the mouse pointer is visible, and the value false indicates the opposite.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 9
   */
  function isPointerVisible(): Promise<boolean>;

  /**
   * Obtains the visible status of the mouse pointer. This API returns the result synchronously.
   *
   * @returns { boolean } Visible status of the mouse pointer. 
   * The value true indicates that the mouse pointer is visible, and the value false indicates the opposite.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 10
   */
  function isPointerVisibleSync(): boolean;

  /**
   * Sets the pointer color. This API uses an asynchronous callback to return the result.
   *
   * @param { number } color - Pointer color. The default value is black (0x000000).
   * @param { AsyncCallback<void> } callback - Callback used to return the result. 
   * If the operation is successful, err is undefined. Otherwise, err is an error object.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerColor(color: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the pointer color. This API uses a promise to return the result.
   *
   * @param { number } color - Pointer color. The default value is black (0x000000).
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerColor(color: number): Promise<void>;

  /**
   * Sets the pointer color. This API returns the result synchronously.
   *
   * @param { number } color - Pointer color. The default value is black (0x000000).
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerColorSync(color: number): void;

  /**
   * Obtains the pointer color. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerColor(callback: AsyncCallback<number>): void;

  /**
   * Obtains the pointer color. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerColor(): Promise<number>;

  /**
   * Obtains the pointer color. This API returns the result synchronously.
   *
   * @returns { number } Pointer color.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerColorSync(): number;

  /**
   * Sets the pointer size. This API uses an asynchronous callback to return the result.
   *
   * @param { number } size - Pointer size. The value ranges from 1 to 7. The default value is 1.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   *  If the operation is successful, err is undefined. Otherwise, err is an error object.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerSize(size: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the pointer size. This API uses a promise to return the result.
   *
   * @param { number } size - Pointer size. The value ranges from 1 to 7. The default value is 1.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerSize(size: number): Promise<void>;

  /**
   * Sets the pointer size. This API returns the result synchronously.
   *
   * @param { number } size - Pointer size. The value ranges from 1 to 7. The default value is 1.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setPointerSizeSync(size: number): void;

  /**
   * Obtains the pointer size. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerSize(callback: AsyncCallback<number>): void;

  /**
   * Obtains the pointer size. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerSize(): Promise<number>;

  /**
   * Obtains the pointer size. This API returns the result synchronously.
   *
   * @returns { number } Pointer size.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getPointerSizeSync(): number;

  /**
   * Sets the primary button of the mouse. This API uses an asynchronous callback to return the result.
   *
   * @param { PrimaryButton } primary - ID of the primary mouse button.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setMousePrimaryButton(primary: PrimaryButton, callback: AsyncCallback<void>): void;

  /**
   * Sets the primary button of the mouse. This API uses a promise to return the result.
   *
   * @param { PrimaryButton } primary - ID of the primary mouse button.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setMousePrimaryButton(primary: PrimaryButton): Promise<void>;

  /**
   * Obtains the primary button of the mouse. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<PrimaryButton> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getMousePrimaryButton(callback: AsyncCallback<PrimaryButton>): void;

  /**
   * Obtains the primary button of the mouse. This API uses a promise to return the result.
   *
   * @returns { Promise<PrimaryButton> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getMousePrimaryButton(): Promise<PrimaryButton>;

  /**
   * Sets the status of the mouse hover scroll switch. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Status of the mouse hover scroll switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setHoverScrollState(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the status of the mouse hover scroll switch. This API uses a promise to return the result.
   *
   * @param { boolean } state - Status of the mouse hover scroll switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setHoverScrollState(state: boolean): Promise<void>;

  /**
   * Obtains the status of the mouse hover scroll switch. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Obtains the status of the mouse hover scroll switch. 
   * This API uses an asynchronous callback to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getHoverScrollState(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the status of the mouse hover scroll switch. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getHoverScrollState(): Promise<boolean>;

  /**
   * Sets the number of mouse scroll rows. This API uses an asynchronous callback to return the result.
   *
   * @param { number } rows - Number of mouse scroll rows. The value ranges from 1 to 100. The default value is 3.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setMouseScrollRows(rows: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the number of mouse scroll rows. This API uses a promise to return the result.
   *
   * @param { number } rows - Number of mouse scroll rows. The value ranges from 1 to 100. The default value is 3.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setMouseScrollRows(rows: number): Promise<void>;

  /**
   * Obtains the number of mouse scroll rows. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getMouseScrollRows(callback: AsyncCallback<number>): void;

  /**
   * Obtains the moving speed of the mouse pointer. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getMouseScrollRows(): Promise<number>;

  /**
   * Sets the scroll switch of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Scroll switch status. The value true indicates that the switch is enabled, 
   * and the value false indicates the opposite. The default value is true.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadScrollSwitch(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the scroll switch of the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } state - Scroll switch status. The value true indicates that the switch is enabled, 
   * and the value false indicates the opposite. The default value is true.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadScrollSwitch(state: boolean): Promise<void>;

  /**
   * Obtains the scroll switch status of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadScrollSwitch(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the scroll switch status of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadScrollSwitch(): Promise<boolean>;

  /**
   * Sets the scroll direction of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Scroll direction of the touchpad. 
   * The value true indicates that the scroll direction is the same as the finger moving direction, and the value false indicates the opposite.
   * The default value is true.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadScrollDirection(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the scroll direction of the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } state - Scroll direction of the touchpad. 
   * The value true indicates that the scroll direction is the same as the finger moving direction, and the value false indicates the opposite.
   * The default value is true.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadScrollDirection(state: boolean): Promise<void>;

  /**
   * Obtains the scroll direction of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the scroll direction is the same as the finger moving direction, and the value false indicates the opposite. 
   * The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadScrollDirection(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the scroll direction of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the scroll direction is the same as the finger moving direction, and the value false indicates the opposite. 
   * The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadScrollDirection(): Promise<boolean>;

  /**
   * Sets the tap switch of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Tap switch status of the touchpad The value true indicates that the switch is enabled, 
   * and the value false indicates the opposite. The default value is true.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadTapSwitch(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the tap switch of the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } state - Tap switch status of the touchpad. The value true indicates that the switch is enabled, 
   * and the value false indicates the opposite. The default value is true.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadTapSwitch(state: boolean): Promise<void>;

  /**
   * Obtains the tap switch status of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadTapSwitch(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the tap switch status of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadTapSwitch(): Promise<boolean>;

  /**
   * SSets the mouse pointer moving speed of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { number } speed - Mouse pointer moving speed of the touchpad. The value range is [1,11]. The default value is 6.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadPointerSpeed(speed: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the mouse pointer moving speed of the touchpad. This API uses a promise to return the result.
   *
   * @param { number } speed - Mouse pointer moving speed of the touchpad. The value range is [1,11]. The default value is 6.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadPointerSpeed(speed: number): Promise<void>;

  /**
   * Obtains the mouse pointer moving speed of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadPointerSpeed(callback: AsyncCallback<number>): void;

  /**
   * Obtains the mouse pointer moving speed of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<number> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadPointerSpeed(): Promise<number>;

  /**
   * Sets the pinch switch of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Pinch switch status of the touchpad. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadPinchSwitch(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the pinch switch of the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } state - Pinch switch status of the touchpad. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadPinchSwitch(state: boolean): Promise<void>;

  /**
   * Obtains the pinch switch status of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
  */
  function getTouchpadPinchSwitch(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the pinch switch status of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadPinchSwitch(): Promise<boolean>;

  /**
   * Sets the multi-finger swipe switch of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } state - Swipe switch status of the touchpad. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadSwipeSwitch(state: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the swipe switch of the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } state - Swipe switch status of the touchpad. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadSwipeSwitch(state: boolean): Promise<void>;
 
  /**
   * Obtains the multi-finger swipe switch status of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadSwipeSwitch(callback: AsyncCallback<boolean>): void;
 
  /**
   * Obtains the multi-finger swipe switch status of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite. The default value is true.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadSwipeSwitch(): Promise<boolean>;

  /**
   * Sets the shortcut menu type of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { RightClickType } type - Shortcut menu type of the touchpad.
    - TOUCHPAD_RIGHT_BUTTON: Tapping the right-button area of the touchpad.
    - TOUCHPAD_LEFT_BUTTON: Tapping the left-button area of the touchpad.
    - TOUCHPAD_TWO_FINGER_TAP: Tapping or pressing the touchpad with two fingers.
    - TOUCHPAD_TWO_FINGER_TAP_OR_RIGHT_BUTTON: Tapping or pressing the touchpad with two fingers, or tapping the right-button area of the touchpad.
    - TOUCHPAD_TWO_FINGER_TAP_OR_LEFT_BUTTON: Tapping or pressing the touchpad with two fingers, or tapping the left-button area of the touchpad.
   * The default value is TOUCHPAD_TWO_FINGER_TAP_OR_RIGHT_BUTTON.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadRightClickType(type: RightClickType, callback: AsyncCallback<void>): void;

  /**
   * Sets the shortcut menu type of the touchpad. This API uses a promise to return the result.
   *
   * @param { RightClickType } type - Shortcut menu type of the touchpad.
    - TOUCHPAD_RIGHT_BUTTON: Tapping the right-button area of the touchpad.
    - TOUCHPAD_LEFT_BUTTON: Tapping the left-button area of the touchpad.
    - TOUCHPAD_TWO_FINGER_TAP: Tapping or pressing the touchpad with two fingers.
    - TOUCHPAD_TWO_FINGER_TAP_OR_RIGHT_BUTTON: Tapping or pressing the touchpad with two fingers, or tapping the right-button area of the touchpad.
    - TOUCHPAD_TWO_FINGER_TAP_OR_LEFT_BUTTON: Tapping or pressing the touchpad with two fingers, or tapping the left-button area of the touchpad.
   * The default value is TOUCHPAD_TWO_FINGER_TAP_OR_RIGHT_BUTTON.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function setTouchpadRightClickType(type: RightClickType): Promise<void>;

  /**
   * Obtains the shortcut menu type of the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<RightClickType> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadRightClickType(callback: AsyncCallback<RightClickType>): void;

  /**
   * Obtains the shortcut menu type of the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<RightClickType> } Promise used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 10
   */
  function getTouchpadRightClickType(): Promise<RightClickType>;

  /**
   * Sets the custom cursor style. This API uses a promise to return the result.
   *
   * @param { number } windowId - Window ID.
   * @param { image.PixelMap } pixelMap - Pixel map resource.
   * @param { number } focusX - Focus x of the custom cursor. The value is greater than or equal to 0. The default value is 0.
   * @param { number } focusY - Focus y of the custom cursor. The value is greater than or equal to 0. The default value is 0.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 11
   */
  function setCustomCursor(windowId: number, pixelMap: image.PixelMap, focusX?: number, focusY?: number): Promise<void>;

  /**
   * Sets a custom cursor. This API returns the result synchronously.
   *
   * @param { number } windowId - Window ID. The value must be an integer greater than 0.
   * @param { image.PixelMap } pixelMap - Pixel map resource.
   * @param { number } focusX - Focus x of the custom cursor. The value is greater than or equal to 0. The default value is 0.
   * @param { number } focusY - Focus y of the custom cursor. The value is greater than or equal to 0. The default value is 0.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 11
   */
  function setCustomCursorSync(windowId: number, pixelMap: image.PixelMap, focusX?: number, focusY?: number): void;

  /**
   * Sets the custom cursor style. This API uses a promise to return the result. 
   * The cursor may be switched back to the system style in the following cases: application window layout change, hot zone switching, 
   * page redirection, moving of the cursor out of the window and then back to the window, or moving of the cursor in different areas of the window. 
   * In this case, you need to reset the cursor style.
   *
   * @param { number } windowId - Window ID.
   * @param { CustomCursor } cursor - Pixel map resource.
   * @param { CursorConfig } config - Custom cursor configuration, which specifies whether to adjust the cursor size based on system settings. 
   * If followSystem in CursorConfig is set to true, the supported adjustment range is [size of the cursor image, 256 x 256].
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Abnormal windowId parameter passed in;
   * <br>2. Abnormal pixelMap parameter passed in; 3. Abnormal focusX parameter passed in;
   * <br>4. Abnormal focusY parameter passed in.
   * @throws { BusinessError } 26500001 - Invalid windowId. Possible causes: The window id does not belong to the current process.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @since 15
   */
  function setCustomCursor(windowId: number, cursor: CustomCursor, config: CursorConfig): Promise<void>;

  /**
   * Sets the double-tap and drag switch for the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } isOpen - Status of the double-tap and drag switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 14
   */
  function setTouchpadDoubleTapAndDragState(isOpen: boolean, callback: AsyncCallback<void>): void;

  /**
   * Sets the double-tap and drag switch for the touchpad. This API uses a promise to return the result.
   *
   * @param { boolean } isOpen - Status of the double-tap and drag switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 14
   */
  function setTouchpadDoubleTapAndDragState(isOpen: boolean): Promise<void>;

  /**
   * Obtains the status of the double-tap and drag switch for the touchpad. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Callback used to return the status of the double-tap drag switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 14
   */
  function getTouchpadDoubleTapAndDragState(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the status of the double-tap and drag switch for the touchpad. This API uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the status of the touchpad double-tap drag switch. 
   * The value true indicates that the switch is enabled, and the value false indicates the opposite.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @syscap SystemCapability.MultimodalInput.Input.Pointer
   * @systemapi hide for inner use.
   * @since 14
   */
  function getTouchpadDoubleTapAndDragState(): Promise<boolean>;
}
export default pointer;