/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

import { MouseEvent } from './@ohos.multimodalInput.mouseEvent';
import { TouchEvent } from './@ohos.multimodalInput.touchEvent';
/**
 * The inputEventClient module implements the input event injection capability.
 *
 * @namespace inputEventClient
 * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
 * @systemapi hide for inner use
 * @since 8
 */
declare namespace inputEventClient {
  /**
   * Defines the key event to inject.
   *
   * @interface KeyEvent
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 8
   */
  interface KeyEvent {
    /**
     * Whether the key is pressed. 
     * The value true indicates that the key is pressed, and the value false indicates the opposite.
     * @type { boolean }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 8
     */
    isPressed: boolean;

    /**
     * Keycode value. Currently, only the KEYCODE_BACK key is supported.
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 8
     */
    keyCode: number;

    /**
     * Duration for pressing a key, in μs.
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 8
     */
    keyDownDuration: number;

    /**
     * Whether the key event can be intercepted. 
     * The value true indicates that the key event can be intercepted, and the value false indicates the opposite.
     * @type { boolean }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 8
     */
    isIntercepted: boolean;
  }

  /**
   * Defines the mouse event data.
   *
   * @interface MouseEventData
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  interface MouseEventData {
    /**
     * Mouse event data.
     *
     * @type { MouseEvent }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 11
     */
    mouseEvent: MouseEvent;
  }

  /**
   * Defines the touchscreen event data.
   *
   * @interface TouchEventData
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  interface TouchEventData {
    /**
     * Touchscreen event data.
     *
     * @type { TouchEvent }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 11
     */
    touchEvent: TouchEvent;
  }

  /**
   * Defines the key event to inject.
   *
   * @interface KeyEventData
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  interface KeyEventData {
    /**
     * Key event to inject.
     *
     * @type { KeyEvent }
     * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
     * @systemapi hide for inner use
     * @since 11
     */
    keyEvent: KeyEvent;
  }

  /**
   * Injects key events (for both single keys and combination keys).
   *
   * @param { KeyEventData } keyEvent - Key event to inject.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  /**
   * Injects key events (for both single keys and combination keys).
   *
   * @permission ohos.permission.INJECT_INPUT_EVENT
   * @param { KeyEventData } keyEvent - Key event to inject.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 12
   */
  function injectKeyEvent(keyEvent: KeyEventData): void;

  /**
   * Injects keys (including single keys and combination keys).
   *
   * @param { { KeyEvent } } KeyEvent - Key event to inject.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 8
   */
  /**
   * Injects keys (including single keys and combination keys).
   *
   * @permission ohos.permission.INJECT_INPUT_EVENT
   * @param { { KeyEvent } } KeyEvent - Key event to inject.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 12
   */
  function injectEvent({ KeyEvent: KeyEvent }): void;

  /**
   * Injects a mouse/touchpad event.
   *
   * @param { MouseEventData } mouseEvent - Mouse/touchpad event to inject.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  /**
   * Injects a mouse/touchpad event.
   *
   * @permission ohos.permission.INJECT_INPUT_EVENT
   * @param { MouseEventData } mouseEvent - Mouse/touchpad event to inject.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 12
   */
  function injectMouseEvent(mouseEvent: MouseEventData): void;

  /**
   * Injects a touchscreen event.
   *
   * @param { TouchEventData } touchEvent - Touchscreen event to inject.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 11
   */
  /**
   * Injects a touchscreen event.
   *
   * @permission ohos.permission.INJECT_INPUT_EVENT
   * @param { TouchEventData } touchEvent - Touchscreen event to inject.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 12
   */
  function injectTouchEvent(touchEvent: TouchEventData): void;

  /**
   * Specifies whether to authorize event injection.
   *
   * @permission ohos.permission.INJECT_INPUT_EVENT
   * @param { boolean } result - Authorization result. The value true indicates that event injection is allowed, and the value false indicates the opposite.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputSimulator
   * @systemapi hide for inner use
   * @since 12
   */
   function permitInjection(result: boolean): void;
}

export default inputEventClient;
