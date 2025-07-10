/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import { TextModifier } from './@ohos.arkui.modifier';


/**
 * Control style of operation element
 * @enum { OperationStyle }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 10
 */
/**
 * Control style of operation element
 * @enum { OperationStyle }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 11
 */
/**
 * Control style of operation element
 * @enum { OperationStyle }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare enum OperationType {
  /**
   * The TextArrow style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The TextArrow style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The TextArrow style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  TEXT_ARROW = 0,

  /**
   * The Button style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The Button style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The Button style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  BUTTON = 1,

  /**
   * The IconGroup style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The IconGroup style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The IconGroup style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  ICON_GROUP = 2,

  /**
   * The LoadingProgress style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The LoadingProgress style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The LoadingProgress style.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  LOADING = 3
}

/**
 * Declare type OperationOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 10
 */
/**
 * Declare type OperationOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 11
 */
/**
 * Declare type OperationOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare class OperationOption {
  /**
   * The content of text or the address of icon.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The content of text or the address of icon.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The content of text or the address of icon.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  value: ResourceStr;

  /**
   * callback function when operate the text or icon.
   * @type { () => void }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * callback function when operate the text or icon.
   * @type { () => void }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * callback function when operate the text or icon.
   * @type { () => void }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  action?: () => void;

  /**
   * The accessibilityText of this text or icon.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  accessibilityText?: ResourceStr;

  /**
   * The accessibilityDescription of this text or icon.
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  accessibilityDescription?: ResourceStr;

  /**
   * The accessibilityLevel of this text or icon.
   * @type { ?string }
   * @default "auto"
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  accessibilityLevel?: string;

  /**
   * Sets the default focus state of the text or icon.
   * @type { ?boolean }.
   * @default { false }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  defaultFocus?: boolean;
}

/**
 * Declare type SelectOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 10
 */
/**
 * Declare type SelectOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 11
 */
/**
 * Declare type SelectOption
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare class SelectOptions {
  /**
   * SubOption array of the select.
   * @type { Array<SelectSubOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * SubOption array of the select.
   * @type { Array<SelectSubOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * SubOption array of the select.
   * @type { Array<SelectSubOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  options: Array<SelectOption>;

  /**
   * The default selected index.
   * @type { number }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The default selected index.
   * @type { number }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The default selected index.
   * @type { number }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selected?: number;

  /**
   * The default text value.
   * @type { string }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The default text value.
   * @type { string }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The default text value.
   * @type { string }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  /**
   * The default text value.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 20
   */
  value?: ResourceStr;

 /**
  * Callback when the select is selected.
  * @type { (index: number, value?: string) => void }.
  * @syscap SystemCapability.ArkUI.ArkUI.Full
  * @since 10
  */
 /**
  * Callback when the select is selected.
  * @type { (index: number, value?: string) => void }.
  * @syscap SystemCapability.ArkUI.ArkUI.Full
  * @atomicservice
  * @since 11
  */
 /**
  * Callback when the select is selected.
  * @type { (index: number, value?: string) => void }.
  * @syscap SystemCapability.ArkUI.ArkUI.Full
  * @crossplatform
  * @atomicservice
  * @since 18
  */
  onSelect?: (index: number, value?: string) => void;

  /**
   * Sets the default focus state of select.
   * @type { ?boolean }.
   * @default { false }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  defaultFocus?: boolean;
}

/**
 * Declare type SymbolOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 12 
 */
 /**
 * Declare type SymbolOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18 
 */
export declare class SymbolOptions {
  /**
   * The size of symbol icon.
   * @type { ?(number | string | Resource) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * The size of symbol icon.
   * @type { ?(number | string | Resource) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  fontSize?: number | string | Resource;

  /**
   * The color of symbol icon.
   * @type { ?(Array<ResourceColor>) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * The color of symbol icon.
   * @type { ?(Array<ResourceColor>) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  fontColor?: Array<ResourceColor>;

  /**
   * The fontWeight of symbol icon.
   * @type { ?(number | FontWeight | string) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * The fontWeight of symbol icon.
   * @type { ?(number | FontWeight | string) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  fontWeight?: number | FontWeight | string;

  /**
   * The effect strategy of symbol icon.
   * @type { ?(SymbolEffectStrategy) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * The effect strategy of symbol icon.
   * @type { ?(SymbolEffectStrategy) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  effectStrategy?: SymbolEffectStrategy;

  /**
   * The rendering strategy of symbol icon.
   * @type { ?(SymbolRenderingStrategy) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * The rendering strategy of symbol icon.
   * @type { ?(SymbolRenderingStrategy) }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  renderingStrategy?: SymbolRenderingStrategy;
}

/**
 * Declare struct SubHeader
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @since 10
 */
/**
 * Declare struct SubHeader
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @atomicservice
 * @since 11
 */
/**
 * Declare struct SubHeader
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@Component
export declare struct SubHeader {

  /**
   * Icon resource of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * Icon resource of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * Icon resource of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop icon?: ResourceStr;

  /**
   * Attributes of Symbol icon.
   * @type { SymbolOptions}.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Attributes of Symbol icon.
   * @type { SymbolOptions}.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  iconSymbolOptions?: SymbolOptions;

  /**
   * The first line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The first line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The first line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop primaryTitle?: ResourceStr;

  /**
   * The secondary line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * The secondary line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * The secondary line text of content area.
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop secondaryTitle?: ResourceStr;

  /**
   * Select option of content area.
   * @type { SelectOptions }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * Select option of content area.
   * @type { SelectOptions }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * Select option of content area.
   * @type { SelectOptions }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  select?: SelectOptions;

  /**
   * Operation style of SubHeader.
   * @type { OperationStyle }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * Operation style of SubHeader.
   * @type { OperationStyle }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * Operation style of SubHeader.
   * @type { OperationStyle }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop operationType?: OperationType;

  /**
   * operation item.
   * @type { Array<OperationOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @since 10
   */
  /**
   * operation item.
   * @type { Array<OperationOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 11
   */
  /**
   * operation item.
   * @type { Array<OperationOption> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  operationItem?: Array<OperationOption>;

  /**
   * Attributes of Symbol icons in operation area.
   * @type { Array<SymbolOptions> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Attributes of Symbol icons in operation area.
   * @type { Array<SymbolOptions> }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 18
   */
  operationSymbolOptions?: Array<SymbolOptions>;

  /**
   * Text modifier for primary title.
   * @type { TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Text modifier for primary title.
   * @type { TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  primaryTitleModifier?: TextModifier;

  /**
   * Text modifier for secondary title.
   * @type { TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Text modifier for secondary title.
   * @type { TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  secondaryTitleModifier?: TextModifier;

  /**
   * Set the title content.
   * @type { () => void }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Set the title content.
   * @type { () => void }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @BuilderParam titleBuilder?: () => void;

  /**
   * Set the content margin.
   * @type { ?LocalizedMargin }
   * @default {start: LengthMetrics.resource($r('sys.float.margin_left')),
   * <br> end: LengthMetrics.resource($r('sys.float.margin_right'))}
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Set the content margin.
   * @type { ?LocalizedMargin }
   * @default {start: LengthMetrics.resource($r('sys.float.margin_left')),
   * <br> end: LengthMetrics.resource($r('sys.float.margin_right'))}
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop contentMargin?: LocalizedMargin;

  /**
   * Set the content padding.
   * @type { ?LocalizedPadding }
   * @default set different default values according to the width of the subHeader:
   * <br> When the left area is secondaryTitle or the group of secondaryTitle and icon,
   * <br> the default value is {start: LengthMetrics.vp(12), end: LengthMetrics.vp(12)};
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @atomicservice
   * @since 12
   */
  /**
   * Set the content padding.
   * @type { ?LocalizedPadding }
   * @default set different default values according to the width of the subHeader:
   * <br> When the left area is secondaryTitle or the group of secondaryTitle and icon,
   * <br> the default value is {start: LengthMetrics.vp(12), end: LengthMetrics.vp(12)};
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Prop contentPadding?: LocalizedPadding;
}