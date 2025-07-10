/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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



import { Callback } from './@ohos.base';
import window from './@ohos.window';
/**
 * uiExtension.
 *
 * @namespace uiExtension
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 12
 */
declare namespace uiExtension {
  /**
   * The proxy of the UIExtension window.
   *
   * @interface WindowProxy
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  interface WindowProxy {
    /**
     * Obtains the area where this window cannot be displayed, for example, the system bar area, notch, gesture area, and soft keyboard area.
     *
     * @param { window.AvoidAreaType } type - Type of the avoid area.
     * @returns { window.AvoidArea } Area where the window cannot be displayed.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    getWindowAvoidArea(type: window.AvoidAreaType): window.AvoidArea;

    /**
     * Subscribes to the event indicating changes to the area where the window cannot be displayed.
     *
     * @param { 'avoidAreaChange' } type - Event type. The value is fixed at 'avoidAreaChange',
     *     indicating the event of changes to the area where the window cannot be displayed.
     * @param { Callback<AvoidAreaInfo> } callback - Callback used to return the avoid area information.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    on(type: 'avoidAreaChange', callback: Callback<AvoidAreaInfo>): void;

    /**
     * Unsubscribes from the event indicating changes to the area where the window cannot be displayed.
     *
     * @param { 'avoidAreaChange' } type - Event type. The value is fixed at 'avoidAreaChange',
     *     indicating the event of changes to the area where the window cannot be displayed.
     * @param { Callback<AvoidAreaInfo> } callback - Callback used to return the avoid area information.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    off(type: 'avoidAreaChange', callback?: Callback<AvoidAreaInfo>): void;

    /**
     * Subscribes to the window size change event of the host application.
     *
     * @param { 'windowSizeChange' } type - Event type. The value is fixed at 'windowSizeChange', indicating the window size change event.
     * @param { Callback<window.Size> } callback - Callback used to return the window size.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    on(type: 'windowSizeChange', callback: Callback<window.Size>): void;

    /**
     * Unsubscribes from the window size change event of the host application.
     *
     * @param { 'windowSizeChange' } type - Event type. The value is fixed at 'windowSizeChange', indicating the window size change event.
     * @param { Callback<window.Size> } callback - Callback used to return the window size.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    off(type: 'windowSizeChange', callback?: Callback<window.Size>): void;

    /**
     * Subscribes to changes in the position and size of the component (EmbeddedComponent or UIExtensionComponent). This API can be used only on 2-in-1 devices.
     *
     * @param { 'rectChange' } type - Event type. The value is fixed at 'rectChange',
     *     indicating the rectangle change event for the component (EmbeddedComponent or UIExtensionComponent).
     * @param { 'number' } reasons - The reasons of component rect change.
     * @param { Callback<RectChangeOptions> } callback - Callback used to return the RectChangeOptions.
     * @throws { BusinessError } 401 - Parameter error. Possible cause: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    on(type: 'rectChange', reasons: number, callback: Callback<RectChangeOptions>): void;

    /**
     * Unsubscribes from changes in the position and size of the component (EmbeddedComponent or UIExtensionComponent).
     * This API can be used only on 2-in-1 devices.
     *
     * @param { 'rectChange' } type - Event type. The value is fixed at 'rectChange',
     *     indicating the rectangle change event for the component (EmbeddedComponent or UIExtensionComponent).
     * @param { Callback<RectChangeOptions> } callback - Callback used to return the RectChangeOptions.
     * @throws { BusinessError } 401 - Parameter error. Possible cause: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    off(type: 'rectChange', callback?: Callback<RectChangeOptions>): void;

    /**
     * Sets whether to hide insecure windows.
     * When called by modal UIExtension and shouldHide == false, the "ohos.permission.ALLOW_SHOW_NON_SECURE_WINDOWS" permission is required.
     *
     * @permission ohos.permission.ALLOW_SHOW_NON_SECURE_WINDOWS
     * @param { boolean } shouldHide - Whether to hide insecure windows. The value true means to hide insecure windows, and false means the opposite.
     * @returns { Promise<void> } - The promise returned by the function.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @throws { BusinessError } 1300002 - Abnormal state. Possible causes:
     * <br> 1. Permission denied. Interface caller does not have permission "ohos.permission.ALLOW_SHOW_NON_SECURE_WINDOWS".
     * <br> 2. The UIExtension window proxy is abnormal.
     * @throws { BusinessError } 1300003 - This window manager service works abnormally.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    hideNonSecureWindows(shouldHide: boolean): Promise<void>;

    /**
     * Creates a subwindow for this window proxy. This API uses a promise to return the result.
     *
     * @param { string } name - Name of the subwindow.
     * @param { window.SubWindowOptions } subWindowOptions - Parameters used for creating the subwindow.
     * @returns { Promise<window.Window> } Promise used to return the subwindow.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported. Failed to call the API due to limited device capabilities.
     * @throws { BusinessError } 1300002 - This window state is abnormal.
     * @throws { BusinessError } 1300005 - This window proxy is abnormal.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @StageModelOnly
     * @atomicservice
     * @since 12
     */
    createSubWindowWithOptions(name: string, subWindowOptions: window.SubWindowOptions): Promise<window.Window>;

    /**
     * Adds or deletes the watermark flag for this window. This API uses a promise to return the result.
     *
     * @param { boolean } enable - Whether to add or delete the flag. The value true means to add the watermark flag,
     *     and false means to delete the watermark flag.
     * @returns { Promise<void> } - The promise returned by the function
     * @throws { BusinessError } 1300002 - The UIExtension window proxy is abnormal.
     * @throws { BusinessError } 1300003 - This window manager service works abnormally.
     * @throws { BusinessError } 1300008 - The display device is abnormal.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @systemapi
     * @since 12
     */
    setWaterMarkFlag(enable: boolean): Promise<void>;

    /**
     * Sets the events that the component (EmbeddedComponent or UIExtensionComponent) will occupy,
     * preventing the host from responding to these events within the component's area.
     *
     * @param { EventFlag } eventFlags - Type of events to occupy. For details about the available values, see {@link uiExtension.EventFlag }.
     * @returns { Promise<void> } - The promise returned by the function
     * @throws { BusinessError } 401 - Parameter error. Possible cause: 
     * <br> 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameters types.
     * <br> 3. Parameter verification failed.
     * @throws { BusinessError } 1300002 - This window state is abnormal.
     * @throws { BusinessError } 1300003 - This window manager service works abnormally.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    occupyEvents(eventFlags: number): Promise<void>;

    /**
     * The properties of the UIExtension window
     *
     * @type { WindowProxyProperties } 
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    properties: WindowProxyProperties;
  }

  /**
   * Gesture event type
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  enum EventFlag {
    /**
     * None gesture event
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_NONE = 0x00000000,
    /**
     * Pan gesture left events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_PAN_GESTURE_LEFT = 0x00000001,
    /**
     * Pan gesture right events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_PAN_GESTURE_RIGHT = 0x00000002,
    /**
     * Pan gesture up events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_PAN_GESTURE_UP = 0x00000004,
    /**
     * Pan gesture down events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_PAN_GESTURE_DOWN = 0x00000008,
    /**
     * Click gesture events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_CLICK = 0x00000100,
    /**
     * Long press gesture events
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    EVENT_LONG_PRESS = 0x00000200,
  }

  /**
   * Defines the avoid area information.
   *
   * @interface AvoidAreaInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  interface AvoidAreaInfo {
    /**
     * Type of the area where the window cannot be displayed.
     *
     * @type { window.AvoidAreaType }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    type: window.AvoidAreaType;

    /**
     * Area where the window cannot be displayed.
     *
     * @type { window.AvoidArea }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    area: window.AvoidArea;
  }

  /**
   * Properties of UIExtension window
   *
   * @interface WindowProxyProperties
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 14
   * 
   */
  interface WindowProxyProperties {
    /**
     * Position and size of the component (EmbeddedComponent or UIExtensionComponent).
     *
     * @type { window.Rect } 
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    uiExtensionHostWindowProxyRect: window.Rect;
  }

  /**
   * Defines the reason of component rect change
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 14
   */
  enum RectChangeReason {
    /**
     * The rectangle of the host window containing the component changes.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    HOST_WINDOW_RECT_CHANGE = 0x0001,
  }

  /**
   * Rect change options
   *
   * @interface RectChangeOptions
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 14
   */
  interface RectChangeOptions {
    /**
     * New values of the rectangle of the component after the change.
     *
     * @type { window.Rect }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    rect: window.Rect,

    /**
     * Reason for the rectangle change.
     *
     * @type { RectChangeReason }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 14
     */
    reason: RectChangeReason
  }
}

export default uiExtension;
