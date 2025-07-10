/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 * Defines the arc scroll bar options.
 *
 * @interface ArcScrollBarOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare interface ArcScrollBarOptions {
  /**
   * Describes the controller for scrollable container.
   *
   * @type { ?Scroller }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  scroller: Scroller;

  /**
   * Sets the state of scroll bar.
   *
   * @type { ?BarState }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  state?: BarState;
}

/**
 * Defines the arc scroll bar component.
 *
 * @interface ArcScrollBarInterface
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export interface ArcScrollBarInterface {
  /**
   * Creates the arc scroll bar component.
   *
   * @param { ArcScrollBarOptions } options
   * @returns { ArcScrollBarAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  (options: ArcScrollBarOptions): ArcScrollBarAttribute;
}

/**
 * Defines the arc scroll bar attribute functions.
 * 
 * @extends CommonMethod<ArcScrollBarAttribute>
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare class ArcScrollBarAttribute extends CommonMethod<ArcScrollBarAttribute> {
}

/**
 * Defines ArcScrollBar Component instance.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare const ArcScrollBarInstance: ArcScrollBarAttribute;

/**
 * Defines ArcScrollBar Component.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @uicomponent
 * @since 18
 */
export declare const ArcScrollBar: ArcScrollBarInterface;
