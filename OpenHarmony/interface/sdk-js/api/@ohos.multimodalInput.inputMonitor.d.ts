/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
 * @kit InputKit
 */

import { Callback } from './@ohos.base';
import { MouseEvent } from './@ohos.multimodalInput.mouseEvent';
import type display from './@ohos.display';
import type { TouchEvent } from './@ohos.multimodalInput.touchEvent';
import type { Rotate, Pinch, ThreeFingersSwipe, FourFingersSwipe, SwipeInward } from './@ohos.multimodalInput.gestureEvent';
import type { ThreeFingersTap, TouchGestureEvent } from './@ohos.multimodalInput.gestureEvent';
import type { FingerprintEvent } from './@ohos.multimodalInput.shortKey';
import type { KeyEvent } from './@ohos.multimodalInput.keyEvent';
import type { KeyCode } from './@ohos.multimodalInput.keyCode';

/**
 * The inputMonitor module implements listening for events of input devices, including the touchscreen, mouse, touchpad, etc.
 * System API, available only to system processes
 *
 * @namespace inputMonitor
 * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
 * @systemapi hide for inner use
 * @since 7
 */
declare namespace inputMonitor {
  /**
   * Defines the callback for touch (touchscreen) events.
   *
   * @interface TouchEventReceiver
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 7
   */
  interface TouchEventReceiver {
    /**
     * Callback used to receive touch input events.
     *
     * @param { TouchEvent } touchEvent - the reported touch event.
     * @returns { Boolean } Result indicating whether the touch event will be dispatched to the window. 
     * The value true indicates that the touch event will be dispatched to the window, and the value false indicates the opposite.
     * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
     * @systemapi hide for inner use
     * @since 7
     */
    (touchEvent: TouchEvent): Boolean;
  }

  /**
   * Enables listening for global touch (touchscreen) events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touch' } type - Event type. This field has a fixed value of touch.
   * @param { TouchEventReceiver } receiver - Callback used to return touch events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 7
   */
  /**
   * Enables listening for global touch (touchscreen) events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touch' } type - Event type. This field has a fixed value of touch.
   * @param { TouchEventReceiver } receiver - Callback used to return touch events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function on(type: 'touch', receiver: TouchEventReceiver): void;

  /**
   * Enables listening for global mouse events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { Callback<MouseEvent> } receiver - Callback used to return mouse events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Enables listening for global mouse events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { Callback<MouseEvent> } receiver - Callback used to return mouse events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function on(type: 'mouse', receiver: Callback<MouseEvent>): void;

  /**
   * Enables listening for mouse events. When the mouse pointer moves to the specified rectangular area, a callback is triggered.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { display.Rect[] } rect - Rectangular area where a callback is triggered. One or two rectangular areas can be specified.
   * @param { Callback<MouseEvent> } receiver - Callback used to return mouse events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  /**
   * Enables listening for mouse events. When the mouse pointer moves to the specified rectangular area, a callback is triggered.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { display.Rect[] } rect - Rectangular area where a callback is triggered. One or two rectangular areas can be specified.
   * @param { Callback<MouseEvent> } receiver - Callback used to return mouse events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function on(type: 'mouse', rect: display.Rect[], receiver: Callback<MouseEvent>): void;

  /**
   * Disables listening for global touch (touchscreen) events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touch' } type - Event type. This field has a fixed value of touch.
   * @param { TouchEventReceiver } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 7
   */
  /**
   * Disables listening for global touch (touchscreen) events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touch' } type - Event type. This field has a fixed value of touch.
   * @param { TouchEventReceiver } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function off(type: 'touch', receiver?: TouchEventReceiver): void;

  /**
   * Disables listening for global mouse events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { Callback<MouseEvent> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 9
   */
  /**
   * Disables listening for global mouse events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'mouse' } type - Event type. This field has a fixed value of mouse.
   * @param { Callback<MouseEvent> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function off(type: 'mouse', receiver?: Callback<MouseEvent>): void;

  /**
   * Enables listening for global touchpad pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'pinch' } type - Event type. This field has a fixed value of pinch.
   * @param { Callback<Pinch> } receiver - Callback used to return pinch events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function on(type: 'pinch', receiver: Callback<Pinch>): void;

  /**
   * Disables listening for global touchpad pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'pinch' } type - Event type. This field has a fixed value of pinch.
   * @param { Callback<Pinch> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function off(type: 'pinch', receiver?: Callback<Pinch>): void;

  /**
   * Enables listening for global touchpad pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'pinch' } type - Event type, which is **pinch**.
   * @param { number } fingers - Number of fingers that trigger the pinch. The value must be greater than or equal to 2.
   * @param { Callback<Pinch> } receiver - Callback used to return pinch events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function on(type: 'pinch', fingers: number, receiver: Callback<Pinch>): void;

  /**
   * Disables listening for global touchpad pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'pinch' } type - Event type. This field has a fixed value of pinch.
   * @param { number } fingers - Number of fingers that trigger the pinch. The value must be greater than or equal to 2.
   * @param { Callback<Pinch> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function off(type: 'pinch', fingers: number, receiver?: Callback<Pinch>): void;

  /**
   * Enables listening for rotation events of the touchpad.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'rotate' } type - Event type. This field has a fixed value of rotate.
   * @param { number } fingers - Number of fingers that trigger a rotation. The value must not be greater than 2.
   * @param { Callback<Rotate> } receiver - Callback used to return rotation events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function on(type: 'rotate', fingers: number, receiver: Callback<Rotate>): void;

  /**
   * Disables listening for rotation events of the touchpad.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'rotate' } type - Event type. This field has a fixed value of rotate.
   * @param { number } fingers - Number of fingers that trigger a rotation. The value must not be greater than 2.
   * @param { Callback<Rotate> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function off(type: 'rotate', fingers: number, receiver?: Callback<Rotate>): void;

  /**
   * Enables listening for three-finger swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'threeFingersSwipe' } type - Event type. This field has a fixed value of threeFingersSwipe.
   * @param { Callback<ThreeFingersSwipe> } receiver - Callback used to return three-finger swipe events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function on(type: 'threeFingersSwipe', receiver: Callback<ThreeFingersSwipe>): void;

  /**
   * Disables listening for three-finger swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'threeFingersSwipe' } type - Event type. This field has a fixed value of threeFingersSwipe.
   * @param { Callback<ThreeFingersSwipe> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function off(type: 'threeFingersSwipe', receiver?: Callback<ThreeFingersSwipe>): void;

  /**
   * Enables listening for four-finger swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'fourFingersSwipe' } type - Event type. This field has a fixed value of fourFingersSwipe.
   * @param { Callback<FourFingersSwipe> } receiver - Callback used to return four-finger swipe events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function on(type: 'fourFingersSwipe', receiver: Callback<FourFingersSwipe>): void;

  /**
   * Disables listening for four-finger swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'fourFingersSwipe' } type - Event type. This field has a fixed value of fourFingersSwipe.
   * @param { Callback<FourFingersSwipe> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 10
   */
  function off(type: 'fourFingersSwipe', receiver?: Callback<FourFingersSwipe>): void;

  /**
   * Enables listening for three-finger tap events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'threeFingersTap' } type - Event type. This field has a fixed value of threeFingersTap.
   * @param { Callback<ThreeFingersTap> } receiver - Callback used to return three-finger tap events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function on(type: 'threeFingersTap', receiver: Callback<ThreeFingersTap>): void;

  /**
   * Disables listening for three-finger tap events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'threeFingersTap' } type - Event type. This field has a fixed value of threeFingersTap.
   * @param { Callback<ThreeFingersTap> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 11
   */
  function off(type: 'threeFingersTap', receiver?: Callback<ThreeFingersTap>): void;

  /**
   * Enables listening for fingerprint key events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'fingerprint' } type - Event type, which is **fingerprint**.
   * @param { Callback<FingerprintEvent> } receiver - Callback used to receive the reported data.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function on(type: 'fingerprint', receiver: Callback<FingerprintEvent>): void;

  /**
   * Cancels listening for fingerprint key events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'fingerprint' } type - Event type, which is **fingerprint**.
   * @param { Callback<FingerprintEvent> } receiver - Callback used to receive the reported data.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function off(type: 'fingerprint', receiver?: Callback<FingerprintEvent>): void;

  /**
   * Enables listening touchPad swipe inward events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'swipeInward' } type - Event type, which is **swipeInward**.
   * @param { Callback<SwipeInward> } receiver - Callback used to receive the reported data.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function on(type: 'swipeInward', receiver: Callback<SwipeInward>): void;

  /**
   * Cancel listening touchPad swipe inward events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'swipeInward' } type - Event type, which is **swipeInward**.
   * @param { Callback<SwipeInward> } receiver - Callback used to receive the reported data.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permit error.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 12
   */
  function off(type: 'swipeInward', receiver?: Callback<SwipeInward>): void;

  /**
   * Enables listening for touchscreen swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touchscreenSwipe' } type - Event type. This field has a fixed value of touchscreenSwipe.
   * @param { number } fingers - Number of fingers that trigger the swipe. The value range is [3, 5].
   * @param { Callback<TouchGestureEvent> } receiver - Callback used to return touchscreen swipe events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Caller is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1.Mandatory parameters are left unspecified;
   *                                 2.Incorrect parameter types.
   *                                 3.Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 18
   */
  function on(type: 'touchscreenSwipe', fingers: number, receiver: Callback<TouchGestureEvent>): void;

  /**
   * Disables listening for touchscreen swipe events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touchscreenSwipe' } type - Event type. This field has a fixed value of touchscreenSwipe.
   * @param { number } fingers - Number of fingers that trigger the swipe. The value range is [3, 5].
   * @param { Callback<TouchGestureEvent> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Caller is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1.Mandatory parameters are left unspecified;
   *                                 2.Incorrect parameter types.
   *                                 3.Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 18
   */
  function off(type: 'touchscreenSwipe', fingers: number, receiver?: Callback<TouchGestureEvent>): void;

  /**
   * Enables listening for touchscreen pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touchscreenPinch' } type - Event type. This field has a fixed value of touchscreenPinch.
   * @param { number } fingers - Number of fingers that trigger the pinch. The value range is [4, 5].
   * @param { Callback<TouchGestureEvent> } receiver - Callback used to return touchscreen pinch events asynchronously.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Caller is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1.Mandatory parameters are left unspecified;
   *                                 2.Incorrect parameter types.
   *                                 3.Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 18
   */
  function on(type: 'touchscreenPinch', fingers: number, receiver: Callback<TouchGestureEvent>): void;

  /**
   * Disables listening for touchscreen pinch events.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'touchscreenPinch' } type - Event type. This field has a fixed value of touchscreenPinch.
   * @param { number } fingers - Number of fingers that trigger the pinch. The value range is [4, 5].
   * @param { Callback<TouchGestureEvent> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Caller is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1.Mandatory parameters are left unspecified;
   *                                 2.Incorrect parameter types.
   *                                 3.Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 18
   */
  function off(type: 'touchscreenPinch', fingers: number, receiver?: Callback<TouchGestureEvent>): void;

/**
   * Listens for the press and release events of the specified key, which can be the META_LEFT, META_RIGHT, power, or volume key.
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'keyPressed' } type - Event type. This parameter has a fixed value of keyPressed.
   * @param { Array<KeyCode> }  keys - Key code list. 
   * The options are KEYCODE_META_LEFT, KEYCODE_META_RIGHT, KEYCODE_POWER, KEYCODE_VOLUME_DOWN, and KEYCODE_VOLUME_UP.
   * @param { Callback<KeyEvent> } receiver - Callback used to receive reported data.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 4100001 - Event listening not supported for the key.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 15
   */
  function on(type: 'keyPressed', keys: Array<KeyCode>, receiver: Callback<KeyEvent>): void;

/**
   * Cancels listening for the press and release events of the specified key, which can be the META_LEFT, META_RIGHT, power, or volume key. 
   * This API must be used together with inputMonitor.on ('keyPressed').
   *
   * @permission ohos.permission.INPUT_MONITORING
   * @param { 'keyPressed' } type - Event type. This parameter has a fixed value of keyPressed.
   * @param { Callback<KeyEvent> } receiver - Callback for which listening is disabled. 
   * If this parameter is not specified, listening will be disabled for all callbacks registered by the current application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputMonitor
   * @systemapi hide for inner use
   * @since 15
   */
  function off(type: 'keyPressed', receiver?: Callback<KeyEvent>): void;
}
export default inputMonitor;
