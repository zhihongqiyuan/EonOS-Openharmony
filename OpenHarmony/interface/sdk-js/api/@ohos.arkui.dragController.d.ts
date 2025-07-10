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



import type { AsyncCallback, BusinessError, Callback } from './@ohos.base';
import type unifiedDataChannel from './@ohos.data.unifiedDataChannel';

/**
 * This module allows developers to trigger a drag event.
 * @namespace dragController
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 10
 */
/**
 * This module allows developers to trigger a drag event.
 * @namespace dragController
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 12
 */
/**
 * This module allows developers to trigger a drag event.
 * @namespace dragController
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare namespace dragController {
  /**
   * Defines the Drag Status.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Defines the Drag Status.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Defines the Drag Status.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  const enum DragStatus {
    /**
     * Drag has started.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Drag has started.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Drag has started.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    STARTED = 0,
    /**
     * Drag has ended.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Drag has ended.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Drag has ended.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    ENDED = 1,
  }

  /**
   * Drag and drop information
   *
   * @interface DragAndDropInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Drag and drop information
   *
   * @interface DragAndDropInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Drag and drop information
   *
   * @interface DragAndDropInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface DragAndDropInfo {
    /**
     * The drag status.
     * @type { DragStatus }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * The drag status.
     * @type { DragStatus }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * The drag status.
     * @type { DragStatus }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    status: DragStatus;
    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    event: DragEvent;
    /**
     * Additional information about the drag info.
     * @type { ?string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Additional information about the drag info.
     * @type { ?string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Additional information about the drag info.
     * @type { ?string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    extraParams?: string;
  }

  /**
   * One drag action object for drag process
   *
   * @interface DragAction
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * One drag action object for drag process
   *
   * @interface DragAction
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * One drag action object for drag process
   *
   * @interface DragAction
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface DragAction {
    /**
     * trigger drag action
     *
     * @returns { Promise<void> } A Promise can indicate the start result.
     * @throws { BusinessError } 100001 - Internal handling failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * trigger drag action
     *
     * @returns { Promise<void> } A Promise can indicate the start result.
     * @throws { BusinessError } 100001 - Internal handling failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * trigger drag action
     *
     * @returns { Promise<void> } A Promise can indicate the start result.
     * @throws { BusinessError } 100001 - Internal handling failed.
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    startDrag(): Promise<void>;
    /**
     * Registers a callback for listening on drag status changes.
     * This callback is triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Registers a callback for listening on drag status changes.
     * This callback is triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Registers a callback for listening on drag status changes.
     * This callback is triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    on(type: 'statusChange', callback: Callback<DragAndDropInfo>): void;

    /**
     * Deregisters a callback for listening on drag status changes.
     * This callback is not triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Deregisters a callback for listening on drag status changes.
     * This callback is not triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Deregisters a callback for listening on drag status changes.
     * This callback is not triggered when the drag status change.
     *
     * @param { 'statusChange' } type for status changing
     * @param { Callback<DragAndDropInfo> } callback with drag event and status information
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    off(type: 'statusChange', callback?: Callback<DragAndDropInfo>): void;
  }

  /**
   * DragInfo object description
   * 
   * @interface DragInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * DragInfo object description
   * 
   * @interface DragInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * DragInfo object description
   * 
   * @interface DragInfo
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface DragInfo {
    /**
     * A unique identifier to identify which touch point.
     * @type { number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 10
     */
    /**
     * A unique identifier to identify which touch point.
     * @type { number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * A unique identifier to identify which touch point.
     * @type { number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    pointerId: number;

    /**
    * Drag data.
    * @type { ?unifiedDataChannel.UnifiedData }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @since 10
    */
    /**
    * Drag data.
    * @type { ?unifiedDataChannel.UnifiedData }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @atomicservice
    * @since 12
    */
   /**
    * Drag data.
    * @type { ?unifiedDataChannel.UnifiedData }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @crossplatform
    * @atomicservice
    * @since 18
    */
    data?: unifiedDataChannel.UnifiedData;

    /**
    * Additional information about the drag info.
    * @type { ?string }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @since 10
    */
    /**
    * Additional information about the drag info.
    * @type { ?string }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @atomicservice
    * @since 12
    */
   /**
    * Additional information about the drag info.
    * @type { ?string }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @crossplatform
    * @atomicservice
    * @since 18
    */
    extraParams?: string;

    /**
     * Touch point coordinates.
     * @type { ?TouchPoint }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Touch point coordinates.
     * @type { ?TouchPoint }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Touch point coordinates.
     * @type { ?TouchPoint }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    touchPoint?: TouchPoint;

    /**
     * Drag preview options.
     * @type { ?DragPreviewOptions }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Drag preview options.
     * @type { ?DragPreviewOptions }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Drag preview options.
     * @type { ?DragPreviewOptions }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    previewOptions?: DragPreviewOptions;
  }

  /**
   * Defines the animation options for drag preview.
   * 
   * @interface AnimationOptions
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Defines the animation options for drag preview.
   * 
   * @interface AnimationOptions
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Defines the animation options for drag preview.
   * 
   * @interface AnimationOptions
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface AnimationOptions {
    /**
     * Animation duration, in ms.
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * Animation duration, in ms.
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Animation duration, in ms.
     * @type { ?number }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    duration?: number;
    /**
    * Animation curve.
    * @type { ?(Curve | ICurve) }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @since 11
    */
   /**
    * Animation curve.
    * @type { ?(Curve | ICurve) }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @atomicservice
    * @since 12
    */
   /**
    * Animation curve.
    * @type { ?(Curve | ICurve) }
    * @syscap SystemCapability.ArkUI.ArkUI.Full
    * @crossplatform
    * @atomicservice
    * @since 18
    */
    curve?: Curve | ICurve;
  }

  /**
   * Provides the functions of setting color or updating animation.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Provides the functions of setting color or updating animation.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Provides the functions of setting color or updating animation.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  export class DragPreview {
    /**
     * change foreground color of preview
     * @param { ResourceColor } color - color value
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * change foreground color of preview
     * @param { ResourceColor } color - color value
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * change foreground color of preview
     * @param { ResourceColor } color - color value
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    setForegroundColor(color: ResourceColor): void;
    /**
     * update preview style with animation
     * @param { AnimationOptions } options - animation options
     * @param { function } handler - change style functions
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 11
     */
    /**
     * update preview style with animation
     * @param { AnimationOptions } options - animation options
     * @param { function } handler - change style functions
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * update preview style with animation
     * @param { AnimationOptions } options - animation options
     * @param { function } handler - change style functions
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    animate(options: AnimationOptions, handler: () =>void): void;
  }

  /**
   * Define the drag event paramters
   * 
   * @interface DragEventParam
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Define the drag event paramters
   * 
   * @interface DragEventParam
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface DragEventParam {

    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 10
     */
    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * The information containing the drag event.
     * @type { DragEvent }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    event: DragEvent;

    /**
     * Additional information about the drag info.
     * @type { string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @since 10
     */
    /**
     * Additional information about the drag info.
     * @type { string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 12
     */
    /**
     * Additional information about the drag info.
     * @type { string }
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    extraParams: string;
  }

  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @param { AsyncCallback<{ event: DragEvent, extraParams: string }> } callback - Callback that contains the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @param { AsyncCallback<DragEventParam> } callback - Callback that contains the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @param { AsyncCallback<DragEventParam> } callback - Callback that contains the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   * @deprecated since 18
   * @useinstead ohos.arkui.UIContext.DragController#executeDrag
   */
  function executeDrag(custom: CustomBuilder | DragItemInfo, dragInfo: DragInfo,
    callback: AsyncCallback<DragEventParam>): void;

  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { Promise<{ event: DragEvent, extraParams: string }> } A Promise with the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { Promise<DragEventParam> } A Promise with the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Execute a drag event.
   * @param { CustomBuilder | DragItemInfo } custom - Object used for prompts displayed when the object is dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { Promise<DragEventParam> } A Promise with the drag event information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   * @deprecated since 18
   * @useinstead ohos.arkui.UIContext.DragController#executeDrag
   */
  function executeDrag(custom: CustomBuilder | DragItemInfo, dragInfo: DragInfo): Promise<DragEventParam>;

  /**
   * Create one drag action object, which can be used for starting drag later or monitoring
   * the drag status after drag started.
   * @param { Array<CustomBuilder | DragItemInfo> } customArray - Objects used for prompts
   * displayed when the objects are dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { DragAction } one drag action object
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Create one drag action object, which can be used for starting drag later or monitoring
   * the drag status after drag started.
   * @param { Array<CustomBuilder | DragItemInfo> } customArray - Objects used for prompts
   * displayed when the objects are dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { DragAction } one drag action object
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Create one drag action object, which can be used for starting drag later or monitoring
   * the drag status after drag started.
   * @param { Array<CustomBuilder | DragItemInfo> } customArray - Objects used for prompts
   * displayed when the objects are dragged.
   * @param { DragInfo } dragInfo - Information about the drag event.
   * @returns { DragAction } one drag action object
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   * <br> 1. Mandatory parameters are left unspecified.
   * <br> 2. Incorrect parameters types.
   * <br> 3. Parameter verification failed.
   * @throws { BusinessError } 100001 - Internal handling failed.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   * @deprecated since 18
   * @useinstead ohos.arkui.UIContext.DragController#createDragAction
   */
  function createDragAction(customArray: Array<CustomBuilder | DragItemInfo>, dragInfo: DragInfo): DragAction;

  /**
   * Get drag preview object.
   * @returns { DragPreview } An drag preview object.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 11
   */
  /**
   * Get drag preview object.
   * @returns { DragPreview } An drag preview object.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Get drag preview object.
   * @returns { DragPreview } An drag preview object.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   * @deprecated since 18
   * @useinstead ohos.arkui.UIContext.DragController#getDragPreview
   */
  function getDragPreview(): DragPreview;

  /**
   * Define the status for the application to notify the framework whether to execute drag.
   *
   * @enum { number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  const enum DragStartRequestStatus {

    /**
     * Notify the framework that the application is not yet ready and needs to temporarily block
     * the start of drag, only effective in onDragStart calls.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    WAITING = 0,

    /**
     * Notify the framework that the drag can continue to be started, but only during the start
     * of drag, and will not take effect when the drag is started.
     *
     * @syscap SystemCapability.ArkUI.ArkUI.Full
     * @atomicservice
     * @since 18
     */
    READY = 1,
  }
}

export default dragController;
