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
 * Define the initialization parameters of the arc alphabet index bar
 *
 * @typedef ArcAlphabetIndexerInitInfo
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare interface ArcAlphabetIndexerInitInfo  {
  /**
   * Array of alphabetic indexed strings, cannot be set to empty.
   *
   * @type { string[] }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  arrayValue: string[];
  /**
   * The index value of the initial selected item.
   *
   * @type { number } - If it is out of the index range, the default value is 0
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selected: number;
}

/**
  * Callback when index bar is selected.
  *
  * @typedef { function } OnSelectCallback
  * @param { number } index - the selected index
  * @syscap SystemCapability.ArkUI.ArkUI.Circle
  * @crossplatform
  * @atomicservice
  * @since 18
  */
declare type OnSelectCallback =  (index: number) => void;

/**
 * Arc Alphabet index bar.
 *
 * @interface ArcAlphabetIndexerInterface
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export interface ArcAlphabetIndexerInterface {
  /**
   * Create ArcAlphabetIndexer component
   *
   * ArrayValue: Alphabetical index string array.
   * selected: ID of the selected item.
   *
   * @param { ArcAlphabetIndexerInitInfo } info
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  (info: ArcAlphabetIndexerInitInfo): ArcAlphabetIndexerAttribute;
}

/**
 * Defines the arc alphabet index bar attribute functions.
 *
 * @extends CommonMethod<ArcAlphabetIndexerAttribute>
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare class ArcAlphabetIndexerAttribute extends CommonMethod<ArcAlphabetIndexerAttribute> {
  /**
   * Definitions text color.
   *
   * @param { Optional<ColorMetrics> } color
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  color(color: Optional<ColorMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Selected text color.
   *
   * @param { Optional<ColorMetrics> } color
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedColor(color: Optional<ColorMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Font color of the pop-up prompt text.
   *
   * @param { Optional<ColorMetrics> } color
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  popupColor(color: Optional<ColorMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Select the text background color.
   *
   * @param { Optional<ColorMetrics> } color
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedBackgroundColor(color: Optional<ColorMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Background color of the pop-up window.
   *
   * @param { Optional<ColorMetrics> } color
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  popupBackground(color: Optional<ColorMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Whether to use pop-up index hints.
   *
   * @param { Optional<boolean> } enabled
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  usePopup(enabled: Optional<boolean>): ArcAlphabetIndexerAttribute;

  /**
   * Selected text style.
   *
   * @param { Optional<Font> } font
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedFont(font: Optional<Font>): ArcAlphabetIndexerAttribute;

  /**
   * Popup text style.
   *
   * @param { Optional<Font> } font
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  popupFont(font: Optional<Font>): ArcAlphabetIndexerAttribute;

  /**
   * Definitions fonts.
   *
   * @param { Optional<Font> } font
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  font(font: Optional<Font>): ArcAlphabetIndexerAttribute;

  /**
   * Size of the letter area on the letter index bar. The letter area is a circle. Set the diameter of the circle.
   *
   * @param { Optional<LengthMetrics> } size
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  itemSize(size: Optional<LengthMetrics>): ArcAlphabetIndexerAttribute;

  /**
   * Sets the selected index.
   *
   * @param { Optional<number> } index
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selected(index: Optional<number>): ArcAlphabetIndexerAttribute;

  /**
   * Automatically collapses the characters when the indexer bar not enough to display all characters.
   *
   * @param { Optional<boolean> } enable - A boolean value determines whether auto collapses is enabled for indexer bar.
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  autoCollapse(enable: Optional<boolean>): ArcAlphabetIndexerAttribute;

  /**
   * Index bar selection callback.
   *
   * @param { Optional<OnSelectCallback> } handler
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onSelect(handler: Optional<OnSelectCallback>): ArcAlphabetIndexerAttribute;

  /**
   * Set the background blurStyle of the pop-up window.
   *
   * @param { Optional<BlurStyle> } style
   * @returns { ArcAlphabetIndexerAttribute }
   * @syscap SystemCapability.ArkUI.ArkUI.Circle
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  popupBackgroundBlurStyle(style: Optional<BlurStyle>): ArcAlphabetIndexerAttribute;
}

/**
 * Defines ArcAlphabetIndexer Component.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare const ArcAlphabetIndexer: ArcAlphabetIndexerInterface;

/**
 * Defines ArcAlphabetIndexer Component instance.
 *
 * @syscap SystemCapability.ArkUI.ArkUI.Circle
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare const ArcAlphabetIndexerInstance: ArcAlphabetIndexerAttribute;