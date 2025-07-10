/*
* Copyright (C) 2024 Huawei Device Co., Ltd.
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

/**
 * Declare the action when the button of dialog is clicked.
 *
 * @typedef { function } AdvancedDialogV2ButtonAction
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare type AdvancedDialogV2ButtonAction = () => void;

/**
 * Declare AdvancedDialogV2Button.
 *
 * @class AdvancedDialogV2Button
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ObservedV2
export declare class AdvancedDialogV2Button {
  /**
   * Sets the Display Content of a Button.
   *
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  content: ResourceStr;
  /**
   * Sets the Button Callback.
   *
   * @type { ?AdvancedDialogV2ButtonAction }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  action?: AdvancedDialogV2ButtonAction;
  /**
   * Sets the background color of a button.
   *
   * @type { ?ColorMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  background?: ColorMetrics;
  /**
   * Sets the Button Text Color.
   *
   * @type { ?ColorMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  fontColor?: ColorMetrics;
  /**
   * Describes the Button style.
   *
   * @type { ?ButtonStyleMode }
   * @default ButtonStyleMode.TEXTUAL
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  buttonStyle?: ButtonStyleMode;
  /**
   * Describes the Button role.
   *
   * @type { ?ButtonRole }
   * @default ButtonRole.NORMAL
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  role?: ButtonRole;
  /**
   * Set the default focus of a button.
   *
   * @type { ?boolean }
   * @default { false }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  defaultFocus?: boolean;
  /**
   * Set the availability of the button.
   *
   * @type { ?boolean }
   * @default { true }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Trace
  enabled?: boolean;
  /**
   * The constructor used to create a AdvancedDialogV2Button object.
   *
   * @param { AdvancedDialogV2ButtonOptions } options - button info.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  constructor(options: AdvancedDialogV2ButtonOptions);
}

/**
 * Declare the options of AdvancedDialogV2Button
 *
 * @interface AdvancedDialogV2ButtonOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare interface AdvancedDialogV2ButtonOptions {
  /**
   * Sets the Display Content of a Button.
   *
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  content: ResourceStr;
  /**
   * Sets the Button Callback.
   *
   * @type { ?AdvancedDialogV2ButtonAction }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  action?: AdvancedDialogV2ButtonAction;
  /**
   * Sets the background color of a button.
   *
   * @type { ?ColorMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  background?: ColorMetrics;
  /**
   * Sets the Button Text Color.
   *
   * @type { ?ColorMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  fontColor?: ColorMetrics;
  /**
   * Describes the Button style.
   *
   * @type { ?ButtonStyleMode }
   * @default ButtonStyleMode.TEXTUAL
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  buttonStyle?: ButtonStyleMode;
  /**
   * Describes the Button role.
   *
   * @type { ?ButtonRole }
   * @default ButtonRole.NORMAL
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  role?: ButtonRole;
  /**
   * Set the default focus of a button.
   *
   * @type { ?boolean }
   * @default { false }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  defaultFocus?: boolean;
  /**
   * Set the availability of the button.
   *
   * @type { ?boolean }
   * @default { true }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  enabled?: boolean;
}

/**
 * Declare the callback when the checkbox of dialog is changed.
 *
 * @typedef { function } AdvancedDialogV2OnCheckedChange
 * @param { boolean } checked - Checkbox status.
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare type AdvancedDialogV2OnCheckedChange = (checked: boolean) => void;

/**
 * Declare CustomDialog TipsDialogV2
 *
 * @struct { TipsDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct TipsDialogV2 {
  /**
   * Sets the TipsDialogV2 imageRes.
   *
   * @type { ResourceStr | PixelMap }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  imageRes: ResourceStr | PixelMap;
  /**
   * Sets the TipsDialogV2 image size.
   *
   * @type { ?SizeOptions }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  imageSize?: SizeOptions;
  /**
   * Sets the borderColor of TipsDialogV2 image.
   *
   * @type { ?ColorMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  imageBorderColor?: ColorMetrics;
  /**
   * Sets the borderWidth of TipsDialogV2 image.
   *
   * @type { ?LengthMetrics }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  imageBorderWidth?: LengthMetrics;
  /**
   * Sets the TipsDialogV2 title.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  title?: ResourceStr;
  /**
   * Sets the TipsDialogV2 content.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  content?: ResourceStr;
  /**
   * Sets the TipsDialogV2 checkbox tips.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  checkTips?: ResourceStr;
  /**
   * Sets the TipsDialogV2 checkbox check state.
   *
   * @type { ?boolean }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  checked?: boolean;
  /**
   * Sets the TipsDialogV2 primary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  primaryButton?: AdvancedDialogV2Button;
  /**
   * Sets the TipsDialogV2 secondary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  secondaryButton?: AdvancedDialogV2Button;
  /**
   * Sets the TipsDialogV2 CheckBox Callback.
   * @type { ?AdvancedDialogV2OnCheckedChange }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  onCheckedChange?: AdvancedDialogV2OnCheckedChange;
}

/**
 * Declare CustomDialog SelectDialogV2
 *
 * @struct { SelectDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct SelectDialogV2 {
  /**
   * Sets the SelectDialogV2 title.
   *
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  title: ResourceStr;
  /**
   * Sets the SelectDialogV2 content.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  content?: ResourceStr;
  /**
   * Sets the SelectDialogV2 selected index.
   *
   * @type { ?number }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  selectedIndex?: number;
  /**
   * Sets the SelectDialogV2 confirm button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  confirm?: AdvancedDialogV2Button;
  /**
   * Sets the SelectDialog sheets.
   * @type { SheetInfo[] }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  radioContent: SheetInfo[];
}

/**
 * Declare CustomDialog ConfirmDialogV2
 *
 * @struct { ConfirmDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct ConfirmDialogV2 {
  /**
   * Sets the ConfirmDialogV2 title.
   *
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  title: ResourceStr;
  /**
   * Sets the ConfirmDialogV2 content.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  content?: ResourceStr;
  /**
   * Sets the ConfirmDialogV2 checkbox tips.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  checkTips?: ResourceStr;
  /**
   * Sets the ConfirmDialogV2 checkbox state.
   *
   * @type { ?boolean }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  checked?: boolean;
  /**
   * Sets the ConfirmDialogV2 primary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  primaryButton?: AdvancedDialogV2Button;
  /**
   * Sets the ConfirmDialogV2 secondary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  secondaryButton?: AdvancedDialogV2Button;
  /**
   * Sets the ConfirmDialogV2 CheckBox Callback.
   *
   * @type { ?AdvancedDialogV2OnCheckedChange }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  onCheckedChange?: AdvancedDialogV2OnCheckedChange;
}

/**
 * Declare CustomDialog AlertDialogV2.
 *
 * @struct { AlertDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct AlertDialogV2 {
  /**
   * Sets the AlertDialogV2 title.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  primaryTitle?: ResourceStr;
  /**
   * Sets the AlertDialogV2 secondary title.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  secondaryTitle?: ResourceStr;
  /**
   * Sets the AlertDialogV2 content.
   *
   * @type { ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  content: ResourceStr;
  /**
   * Sets the AlertDialogV2 primary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  primaryButton?: AdvancedDialogV2Button;
  /**
   * Sets the AlertDialogV2 secondary button.
   *
   * @type { ?AdvancedDialogV2Button }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  secondaryButton?: AdvancedDialogV2Button;
}

/**
 * Declare CustomDialog LoadingDialogV2
 *
 * @struct { LoadingDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct LoadingDialogV2 {
  /**
   * Sets the LoadingDialogV2 content.
   *
   * @type { ?ResourceStr }.
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  content?: ResourceStr;
}

/**
 * Declare custom content dialog
 *
 * @struct { CustomContentDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct CustomContentDialogV2 {
  /**
   * Sets the CustomContentDialogV2 title.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  primaryTitle?: ResourceStr;
  /**
   * Sets the CustomContentDialogV2 secondary title.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  secondaryTitle?: ResourceStr;
  /**
   * Sets the CustomContentDialogV2 content.
   *
   * @type { CustomBuilder }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @BuilderParam
  contentBuilder: CustomBuilder;
  /**
   * Sets the CustomContentDialogV2 content area padding.
   *
   * @type { ?LocalizedPadding }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  contentAreaPadding?: LocalizedPadding;
  /**
   * Sets the CustomContentDialogV2 buttons.
   * @type { ?AdvancedDialogV2Button[] }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Param
  buttons?: AdvancedDialogV2Button[];
}

/**
 * Declare struct PopoverDialogV2
 *
 * @struct { PopoverDialogV2 }
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
@ComponentV2
export declare struct PopoverDialogV2 {
  /**
   * Sets the PopoverDialogV2 Visible Status.
   * 
   * @type { boolean }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  visible: boolean;
  /**
   * Sets the callback when visibility changed.
   *
   * @type { ?PopoverDialogV2OnVisibleChange }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Event
  $visible?: PopoverDialogV2OnVisibleChange;
  /**
   * Sets the PopoverDialogV2 options.
   * 
   * @type { PopoverDialogV2Options }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Require
  @Param
  popover: PopoverDialogV2Options;
  /**
   * Sets the targetBuilder content.
   * 
   * @type { CustomBuilder }
   * @syscap SystemCapability.ArkUI.ArkUI.Full
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @BuilderParam
  targetBuilder: CustomBuilder;
}

/**
 * Declare the callback when the visibility of PopoverDialogV2 is changed.
 *
 * @typedef { function } PopoverDialogV2OnVisibleChange
 * @param { boolean } visible - The visibility of PopoverDialogV2.
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare type PopoverDialogV2OnVisibleChange = (visible: boolean) => void;

/**
 * Defines PopoverDialogV2 Options
 * 
 * @interface PopoverDialogV2Options
 * @extends CustomPopupOptions
 * @syscap SystemCapability.ArkUI.ArkUI.Full
 * @crossplatform
 * @atomicservice
 * @since 18
 */
export declare interface PopoverDialogV2Options extends CustomPopupOptions {
}
