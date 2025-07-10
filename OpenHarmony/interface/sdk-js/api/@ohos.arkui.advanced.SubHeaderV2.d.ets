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
  * limitations under the License.@
  */

/**
 * @file api/@ohos.arkui.advanced.SubHeaderV2.d.ts
 * @kit ArkUI
 */
import { TextModifier as TextModifier } from '@ohos.arkui.modifier';

/**
 * SubHeaderV2IconType
 *
 * @typedef { ResourceStr | SymbolGlyphModifier } SubHeaderV2IconType
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type SubHeaderV2IconType = ResourceStr | SymbolGlyphModifier;

/**
 * Defines the SubHeaderV2TitleOptions.
 *
 * @interface SubHeaderV2TitleOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export interface SubHeaderV2TitleOptions {
  /**
   * The first line text of content area.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  primaryTitle?: ResourceStr;
  /**
   * Text modifier for primary title.
   * @type { ?TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  primaryTitleModifier?: TextModifier;
  /**
   * The secondary line text of content area.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  secondaryTitle?: ResourceStr;
  /**
   * Text modifier for secondary title.
   * @type { ?TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  secondaryTitleModifier?: TextModifier;
}

/**
 * Declare the SubHeaderV2Title
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
export declare class SubHeaderV2Title {
  /**
   * The first line text of content area.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace primaryTitle?: ResourceStr;
  /**
   * Text modifier for primary title.
   * @type { ?TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace primaryTitleModifier?: TextModifier;
  /**
   * The secondary line text of content area.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace secondaryTitle?: ResourceStr;
  /**
   * Text modifier for secondary title.
   * @type { ?TextModifier }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace secondaryTitleModifier?: TextModifier;
  /**
   * The constructor of SubHeaderV2Title
   *
   * @param options { SubHeaderV2TitleOptions } the options of the SubHeaderV2Title
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options: SubHeaderV2TitleOptions);
}

/**
 * The callback function that will be invoked when the selectedIndex of the SubHeaderV2Select is selected.
 *
 * @typedef { function } SubHeaderV2SelectOnSelect
 * @param { number } selectedIndex - the index of selected item
 * @param { ?string } selectedContent - the content of selected item
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export type SubHeaderV2SelectOnSelect = (selectedIndex: number, selectedContent?: string) => void;

/**
 * Defines the SubHeaderV2SelectOptions.
 *
 * @interface SubHeaderV2SelectOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export interface SubHeaderV2SelectOptions {
  /**
   * Sets the options of the SubHeaderV2SelectOptions.
   *
   * @type { SelectOption[] }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  options: SelectOption[];
  /**
   * Sets the selected index of the SubHeaderV2SelectOptions.
   *
   * @type { ?number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  selectedIndex?: number;
  /**
   * Sets the selected content of the SubHeaderV2SelectOptions.
   *
   * @type { ?string }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  /**
   * Sets the selected content of the SubHeaderV2SelectOptions.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 20
   */
  selectedContent?: ResourceStr;
  /**
   * Sets the onSelect of the SubHeaderV2SelectOptions.
   *
   * @type { ?SubHeaderV2SelectOnSelect }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  onSelect?: SubHeaderV2SelectOnSelect;
  /**
   * Sets the default focus state of SubHeaderV2SelectOptions.
   * @type { ?boolean }
   * @default false
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  defaultFocus?: boolean;
}

/**
 * Declare the SubHeaderV2Select
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
export declare class SubHeaderV2Select {
  /**
   * Sets the options of the SubHeaderV2SelectOptions.
   *
   * @type { SelectOption[] }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace options: SelectOption[];
  /**
   * Sets the selected index of the SubHeaderV2SelectOptions.
   *
   * @type { ?number }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace selectedIndex?: number;
  /**
   * Sets the selected content of the SubHeaderV2SelectOptions.
   *
   * @type { ?string }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  /**
   * Sets the selected content of the SubHeaderV2SelectOptions.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 20
   */
  @Trace selectedContent?: ResourceStr;
  /**
   * Sets the onSelect of the SubHeaderV2SelectOptions.
   *
   * @type { ?SubHeaderV2SelectOnSelect }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace onSelect?: SubHeaderV2SelectOnSelect;
  /**
   * Sets the default focus state of SubHeaderV2SelectOptions.
   * @type { ?boolean }
   * @default false
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace defaultFocus?: boolean;
  /**
   * The constructor of SubHeaderV2Select
   *
   * @param options { SubHeaderV2SelectOptions } the options of the SubHeaderV2Select
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options: SubHeaderV2SelectOptions);
}

/**
 * Control style of operation element
 * @enum { number }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare enum SubHeaderV2OperationType {
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
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  BUTTON = 1,
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
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  LOADING = 3
}

/**
 * Defines the action callback of the SubHeaderV2OperationItem.
 *
 * @typedef { function } OnClicked
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export type SubHeaderV2OperationItemAction = () => void;

/**
 * SubHeaderV2OperationItemType
 *
 * @typedef { ResourceStr | SymbolGlyphModifier } SubHeaderV2OperationItemType
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare type SubHeaderV2OperationItemType = ResourceStr | SymbolGlyphModifier;

/**
 * Defines the SubHeaderV2OperationItemOptions.
 *
 * @interface SubHeaderV2OperationItemOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export interface SubHeaderV2OperationItemOptions {
  /**
   * The content of text or the address of icon.
   * @type { SubHeaderV2OperationItemType }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  content: SubHeaderV2OperationItemType;
  /**
   * callback function when operate the text or icon.
   * @type { ?SubHeaderV2OperationItemAction }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  action?: SubHeaderV2OperationItemAction;
  /**
   * The accessibilityText of this text or icon.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  accessibilityText?: ResourceStr;
  /**
   * The accessibilityDescription of this text or icon.
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  accessibilityDescription?: ResourceStr;
  /**
   * The accessibilityLevel of this text or icon.
   * @type { ?string }
   * @default "auto".The options are as follows:<br/>
   *     "auto":The value is converted to "yes" or "no" based on the component.
   *     "yes": the current component is selectable for the accessibility service.
   *     "no": The current component is not selectable for the accessibility service.
   *     "no-hide-descendants":The current component and all its child components are not selectable<br/>
   *     for the accessibility service.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  accessibilityLevel?: string;
  /**
   * Sets the default focus state of the text or icon.
   * @type { ?boolean }
   * @default false
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  defaultFocus?: boolean;
}

/**
 * Declare the SubHeaderV2OperationItem
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
export declare class SubHeaderV2OperationItem {
  /**
   * The content of text or the address of icon.
   * @type { SubHeaderV2OperationItemType }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace content: SubHeaderV2OperationItemType;
  /**
   * callback function when operate the text or icon.
   * @type { ?SubHeaderV2OperationItemAction }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace action?: SubHeaderV2OperationItemAction;
  /**
   * The constructor of SubHeaderV2OperationItem
   *
   * @param options { SubHeaderV2OperationItemOptions } the options of the SubHeaderV2OperationItem
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options: SubHeaderV2OperationItemOptions);
  /**
   * The accessibilityText of this text or icon.
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace accessibilityText?: ResourceStr;
  /**
   * The accessibilityDescription of this text or icon.
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace accessibilityDescription?: ResourceStr;
  /**
   * The accessibilityLevel of this text or icon.
   * @type { ?string }
   * @default "auto".The options are as follows:<br/>
   *     "auto":The value is converted to "yes" or "no" based on the component.
   *     "yes": the current component is selectable for the accessibility service.
   *     "no": The current component is not selectable for the accessibility service.
   *     "no-hide-descendants":The current component and all its child components are not selectable<br/>
   *     for the accessibility service.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace accessibilityLevel?: string;
  /**
   * Sets the default focus state of the text or icon.
   * @type { ?boolean }
   * @default false
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace defaultFocus?: boolean;
}

/**
 * The callback function that will be invoked when the title build.
 *
 * @typedef { function } SubHeaderV2TitleBuilder
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export type SubHeaderV2TitleBuilder = () => void;

/**
 * Defines struct SubHeaderV2.
 *
 * @struct SubHeaderV2
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct SubHeaderV2 {
  /**
   * Sets the icon of the SubHeaderV2.
   *
   * @type { ?SubHeaderV2IconType }
   * @readonly
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  readonly icon?: SubHeaderV2IconType;
  /**
   * Sets the title of the SubHeaderV2.
   *
   * @type { ?SubHeaderV2Title }
   * @readonly
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  readonly title?: SubHeaderV2Title;
  /**
   * Sets the select of the SubHeaderV2.
   *
   * @type { ?SubHeaderV2Select }
   * @readonly
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  readonly select?: SubHeaderV2Select;
  /**
   * Sets the operationType of the SubHeaderV2.
   *
   * @type { ?SubHeaderV2OperationType }
   * @readonly
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  readonly operationType?: SubHeaderV2OperationType;
  /**
   * Sets the operationItems of the SubHeaderV2.
   *
   * @type { ?SubHeaderV2OperationItem[] }
   * @readonly
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  readonly operationItems?: SubHeaderV2OperationItem[];
  /**
   * Set the title content.
   * @type { ?SubHeaderV2TitleBuilder }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @BuilderParam
  titleBuilder?: SubHeaderV2TitleBuilder;
}