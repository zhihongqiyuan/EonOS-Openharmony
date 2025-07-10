/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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

/**
 * Provides interfaces to monitor a scene for performance measurement.
 *
 * <p>These interfaces are used to monitor the begin, end, and value changes of finger processes that last for at least 3 ms.
 *
 * <p>Example:
 * import "@ohos.arkui.performanceMonitor.d.ts"
 * To start scene monitoring that is expected to complete within 5 ms:
 * <pre>{@code
 * performanceMonitor.begin(string, ActionType, string);
 * //scene finished
 * performanceMonitor.end(string);
 * }</pre>
 *
 * <p>Each {@code begin} matches one {@code end}, and they must have the same scene id.
 *
 * @namespace performanceMonitor
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @systemapi
 * @since 10
 */
declare namespace performanceMonitor {
  /**
   * Enumerates the input event type.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @systemapi
   * @since 10
   */
  export enum ActionType {
    /**
     * The user presses the finger on the screen.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 10
     */
    LAST_DOWN = 0,

    /**
     * The user lifts up the finger from the screen.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 10
     */
    LAST_UP = 1,

    /**
     * The user first moves the finger after pressing down the screen.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 10
     */
    FIRST_MOVE = 2
  }
  
  /**
   * Enumerates the input source type.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @systemapi
   * @since 12
   */
  export enum SourceType {
    /**
     * The user touches the screen to trigger the scene.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    PERF_TOUCH_EVENT = 0,

    /**
     * TThe user uses the mouse to trigger the scene.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    PERF_MOUSE_EVENT = 1,

    /**
     * The user uses the touchpad to trigger the scene.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    PERF_TOUCHPAD_EVENT = 2,
    
    /**
     * The user uses the joystick to trigger the scene.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    PERF_JOYSTICK_EVENT = 3,

    /**
     * The user uses the keyboard to trigger the scene.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    PERF_KEY_EVENT = 4
  }

  /**
   * Begin monitoring an application scene.
   *
   * @param { string } scene Indicates the scene name.
   * @param { ActionType } startInputType Indicates the scene input event type.
   * @param { string } note Indicates the app expected info delivered.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @systemapi
   * @since 10
   */
  function begin(scene: string, startInputType: ActionType, note?: string): void;   

  /**
   * End monitoring an application scene.
   *
   * @param { string } scene Indicates the scene name. It must be the same with the {@code scene} of start.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @systemapi
   * @since 10
   */
  function end(scene: string): void;
  
  /**
   * recordInputEventTime monitoring an application scene.
   *
   * @param { ActionType } type - Indicates the scene input event type.
   * @param { SourceType } sourceType - Indicates the scene input source type.
   * @param { number } time - Indicates the scene input time.
   * @throws { BusinessError } 202 - not system application. 
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @systemapi
   * @since 12
   */
  function recordInputEventTime(type: ActionType, sourceType: SourceType, time: number): void;  
}
export default performanceMonitor;
