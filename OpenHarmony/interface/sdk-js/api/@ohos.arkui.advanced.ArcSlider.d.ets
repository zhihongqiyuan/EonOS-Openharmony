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
 * Enum for ArcSliderPosition
 *
 * @enum { string }
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */

export declare enum ArcSliderPosition {

  /**
   * The position is on the left
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  LEFT = 0,

  /**
   * The position is on the right.
   *
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  RIGHT = 1
}

/**
 * Construct parameter types for ArcSliderValueOptions.
 *
 * @typedef ArcSliderValueOptionsConstructorOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface ArcSliderValueOptionsConstructorOptions {
  /**
   * Set current progress value.
   *
   * @type { ?number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  progress?: number;

  /**
   * Set the minimum progress value.
   *
   * @type { ?number }
   * @default 0
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  min?: number;

  /**
   * Set the maximum progress value.
   *
   * @type { ?number }
   * @default 100
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  max?: number;
}

/**
 * The options for ArcSlider progress value.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
declare class ArcSliderValueOptions {
  /**
   * Set current progress value.
   *
   * @type { ?number }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  progress?: number;

  /**
   * Set the minimum progress value.
   *
   * @type { ?number }
   * @default 0
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  min?: number;

  /**
   * Set the maximum progress value.
   *
   * @type { ?number }
   * @default 100
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  max?: number;

  /**
   * The constructor used to create a ArcSliderValueOptions object.
   *
   * @param { ArcSliderValueOptionsConstructorOptions } options
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options?: ArcSliderValueOptionsConstructorOptions);
}

/**
 * Construct parameter types for ArcSliderLayoutOptions.
 *
 * @typedef ArcSliderLayoutOptionsConstructorOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface ArcSliderLayoutOptionsConstructorOptions {
  /**
   * Set whether the component's arc direction is counterclockwise.
   *
   * @type { ?boolean }
   * @default true
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  reverse?: boolean;
  /**
   * Set the component's position on the screen.
   *
   * @type { ?ArcSliderPosition }
   * @default ArcSliderPosition.RIGHT
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  position?: ArcSliderPosition;
}

/**
 * The options for ArcSlider layout.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
declare class ArcSliderLayoutOptions {
  /**
   * Set whether the component's arc direction is counterclockwise.
   *
   * @type { ?boolean }
   * @default true
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  reverse?: boolean;

  /**
   * Set the component's position on the screen.
   *
   * @type { ?ArcSliderPosition }
   * @default ArcSliderPosition.RIGHT
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  position?: ArcSliderPosition;

  /**
   * The constructor used to create a ArcSliderLayoutOptions object.
   *
   * @param { ArcSliderLayoutOptionsConstructorOptions } options
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options?: ArcSliderLayoutOptionsConstructorOptions);
}

/**
 * Construct parameter types for ArcSliderStyleOptions.
 *
 * @typedef ArcSliderStyleOptionsConstructorOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface ArcSliderStyleOptionsConstructorOptions {
  /**
   * Set the track thickness of the component under normal status, unit is vp.
   *
   * @type { ?number }
   * @default 5
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  trackThickness?: number;

  /**
   * Set the track thickness of the component under active status, unit is vp.
   *
   * @type { ?number }
   * @default 24
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  activeTrackThickness?: number;

  /**
   * Set the track color of the component.
   *
   * @type { ?string  }
   * @default #33FFFFFF
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  trackColor?: string;

  /**
   * Set the selected color of the component.
   *
   * @type { ?string }
   * @default #FF5EA1FF
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedColor?: string;

  /**
   * Set the track blur of the component, unit is vp.
   *
   * @type { ?number }
   * @default 20
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  trackBlur?: number;
}

/**
 * The options for ArcSlider style.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
declare class ArcSliderStyleOptions {
  /**
   * Set the track thickness of the component under normal status, unit is vp.
   *
   * @type { ?number }
   * @default 5
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  trackThickness?: number;

  /**
   * Set the track thickness of the component under active status, unit is vp.
   *
   * @type { ?number }
   * @default 24
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  activeTrackThickness?: number;

  /**
   * Set the track color of the component.
   *
   * @type { ?string }
   * @default #33FFFFFF
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  trackColor?: string;

  /**
   * Set the selected color of the component.
   *
   * @type { ?string }
   * @default #FF5EA1FF
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  selectedColor?: string;

  /**
   * Set the track blur of the component, unit is vp.
   *
   * @type { ?number }
   * @default 20
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  trackBlur?: number;

  /**
   * The constructor used to create a ArcSliderStyleOptions object.
   *
   * @param { ArcSliderStyleOptionsConstructorOptions } options
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options?: ArcSliderStyleOptionsConstructorOptions);
}

/**
 * Handler of ArcSlider, used in onTouch.
 *
 * @typedef { function } ArcSliderTouchHandler
 * @param { TouchEvent } event - the TouchEvent when ArcSlider is touched.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type ArcSliderTouchHandler = (event: TouchEvent) => void;

/**
 * Handler of ArcSlider, used in OnChange.
 *
 * @typedef { function } ArcSliderChangeHandler
 * @param { number } progress - the progress value of the ArcSlider.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type ArcSliderChangeHandler = (progress: number) => void;

/**
 * Handler of ArcSlider, used in OnEnlarge.
 *
 * @typedef { function } ArcSliderEnlargeHandler
 * @param { boolean } isEnlarged - the enlarged type of the ArcSlider.
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type ArcSliderEnlargeHandler = (isEnlarged: boolean) => void;

/**
 * Construct parameter types for ArcSliderOptions.
 *
 * @typedef ArcSliderOptionsConstructorOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
interface ArcSliderOptionsConstructorOptions {
  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderValueOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  valueOptions?: ArcSliderValueOptions;

  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderLayoutOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  layoutOptions?: ArcSliderLayoutOptions;

  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderStyleOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  styleOptions?: ArcSliderStyleOptions;

  /**
   * Set the sensitivity of rotating crown.
   *
   * @type { ?CrownSensitivity }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  digitalCrownSensitivity?: CrownSensitivity;

  /**
   * Touching triggers the callback.
   *
   * @type { ?ArcSliderTouchHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onTouch?: ArcSliderTouchHandler;

  /**
   * Value changing triggers the callback.
   *
   * @type { ?ArcSliderChangeHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onChange?: ArcSliderChangeHandler;

  /**
   * Enlarged type changging triggers the callback.
   *
   * @type { ?ArcSliderEnlargeHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onEnlarge?: ArcSliderEnlargeHandler;
}

/**
 * The options for ArcSlider.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
declare class ArcSliderOptions {
  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderValueOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  valueOptions?: ArcSliderValueOptions;

  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderLayoutOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  layoutOptions?: ArcSliderLayoutOptions;

  /**
   * Configure parameters for component.
   *
   * @type { ?ArcSliderStyleOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  styleOptions?: ArcSliderStyleOptions;

  /**
   * Set the sensitivity of rotating crown.
   *
   * @type { ?CrownSensitivity }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  digitalCrownSensitivity?: CrownSensitivity;

  /**
   * Touching triggers the callback.
   *
   * @type { ?ArcSliderTouchHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  onTouch?: ArcSliderTouchHandler;

  /**
   * Value changing triggers the callback.
   *
   * @type { ?ArcSliderChangeHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  onChange?: ArcSliderChangeHandler;

  /**
   * Enlarged type changging triggers the callback.
   *
   * @type { ?ArcSliderEnlargeHandler }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  onEnlarge?: ArcSliderEnlargeHandler;

  /**
   * The constructor used to create a ArcSliderOptions object.
   *
   * @param { ArcSliderOptionsConstructorOptions } options
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options?: ArcSliderOptionsConstructorOptions);
}

/**
 * Defines ArcSlider Component.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@Component
declare struct ArcSlider {
  /**
   * The options of a ArcSlider component.
   *
   * @type { ArcSliderOptions }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  options: ArcSliderOptions;
}

export {
  ArcSlider,
  ArcSliderOptions,
  ArcSliderValueOptions,
  ArcSliderLayoutOptions,
  ArcSliderStyleOptions,
  ArcSliderValueOptionsConstructorOptions,
  ArcSliderLayoutOptionsConstructorOptions,
  ArcSliderStyleOptionsConstructorOptions,
  ArcSliderOptionsConstructorOptions,
  ArcSliderTouchHandler,
  ArcSliderChangeHandler
};