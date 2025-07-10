/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import AbilityManager from '../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import CommonTemplateStyleConfiguration from '../../../../../../../../common/src/main/ets/template/common/StyleConfiguration';
import ControlCenterStyleConfiguration from '../../../../../../../../features/controlcentercomponent/src/main/ets/com/ohos/common/StyleConfiguration';
import BrightnessStyleConfiguration from '../../../../../../../../features/brightnesscomponent/src/main/ets/default/common/StyleConfiguration';
import RingModeStyleConfiguration from '../../../../../../../../features/ringmodecomponent/src/main/ets/com/ohos/common/StyleConfiguration';

const TAG = 'DropdownPanel-StyleManager';

export default class StyleManager {
  static readonly STANDARD_DISPLAY_WIDTH: number = 720;
  static readonly STANDARD_DISPLAY_HEIGHT: number = 1280;
  static maxWidth: number = StyleManager.STANDARD_DISPLAY_WIDTH;

  static setStyle(): void {
    Log.showDebug(TAG, 'setStyle');

    let rect = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_DROPDOWN_PANEL, 'rect');
    StyleManager.maxWidth = rect.width;

    // ControlCenter
    StyleManager.setControlCenterComponentStyle();
    // ControlCenter-Title
    StyleManager.setControlCenterUpTitleStyle();
    // ControlCenter-ComplexToggleLayout
    StyleManager.setControlCenterComplexToggleLayoutStyle();
    // ControlCenter-SimpleToggleLayout
    StyleManager.setControlCenterSimpleToggleLayoutStyle();
    // CommonTemplate-iconTitleBase
    StyleManager.setIconTitleBaseStyle();
    // CommonTemplate-iconComponent
    StyleManager.setIconComponentStyle();
    // CommonTemplate-simpleToggleBase
    StyleManager.setSimpleToggleBaseStyle();
    // Brightness
    StyleManager.setBrightnessComponentStyle();
    // RingMode
    StyleManager.setControlCenterRingModeComponentStyle();
    // SimpleToggleLayoutEdit
    StyleManager.setSimpleToggleLayoutEditComponentStyle();
    // SimpleToggleLayoutEdit-title
    StyleManager.setSimpleToggleLayoutEditUpTitleStyle();
    // SimpleToggleLayoutEdit-msg
    StyleManager.setSimpleToggleLayoutEditOptMsgStyle();
    // SimpleToggleLayoutEdit-grid
    StyleManager.setSimpleToggleLayoutEditGridStyle();
    // EditDialog
    StyleManager.setControlEditDialogStyle();
  }
  // ControlCenter
  private static setControlCenterComponentStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterComponentStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(48);
    style.marginRight = StyleManager.calcScaleSizePx(48);
    style.marginTop = StyleManager.calcScaleSizePx(36);
    style.upTitleHeight = StyleManager.calcScaleSizePx(127);
    style.titleMarginBottom = StyleManager.calcScaleSizePx(0);
    style.toggleAreaGap = StyleManager.calcScaleSizePx(24);
    style.simpleToggleLayoutMarginTop = StyleManager.calcScaleSizePx(48);
    style.simpleToggleLayoutMarginBottom = StyleManager.calcScaleSizePx(20);
    style.brightnessMarginBottom = StyleManager.calcScaleSizePx(44);
    style.componentBorderRadius = StyleManager.calcScaleSizePx(48);
    style.componentBackgroundColor = '#99FFFFFF';
  }
  // ControlCenter-Title
  private static setControlCenterUpTitleStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterUpTitleStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(72);
    style.marginRight = StyleManager.calcScaleSizePx(72);
    style.fontSize = StyleManager.calcScaleSizePx(40);
    style.fontColor = '#FFFFFFFF';
    style.imageWidth = StyleManager.calcScaleSizePx(48);
    style.imageHeight = StyleManager.calcScaleSizePx(48);
    style.imageHoverWidth = StyleManager.calcScaleSizePx(64);
    style.imageHoverHeight = StyleManager.calcScaleSizePx(64);
    style.imageHoverRadius = StyleManager.calcScaleSizePx(16);
    style.imageHoverColor = 'rgba(0, 0, 0, 0.05)';
    style.imageTransparentColor = 'rgba(255, 255, 255, 0)';
    style.upTitleSpace = StyleManager.calcScaleSizePx(36);
    style.titleImageColor = '#FFFFFFFF';
  }
  // ControlCenter-ComplexToggleLayout
  private static setControlCenterComplexToggleLayoutStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterComplexToggleLayoutStyle();
    style.columnGap = StyleManager.calcScaleSizePx(24);
    style.rowGap = StyleManager.calcScaleSize(24);
    style.rowHeight = StyleManager.calcScaleSize(132);
  }
  // ControlCenter-SimpleToggleLayout
  private static setControlCenterSimpleToggleLayoutStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterSimpleToggleLayoutStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(22);
    style.marginRight = StyleManager.calcScaleSizePx(22);
    style.columnGap = StyleManager.calcScaleSizePx(0);
    style.rowGap = StyleManager.calcScaleSize(0);
    style.rowHeight = StyleManager.calcScaleSize(162);
  }
  // CommonTemplate-iconTitleBase
  private static setIconTitleBaseStyle(): void {
    let style = CommonTemplateStyleConfiguration.getIconTitleBaseStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(24);
    style.marginRight = StyleManager.calcScaleSizePx(16);
    style.componentGap = StyleManager.calcScaleSizePx(8);
    style.titleSize = StyleManager.calcScaleSizePx(24);
    style.titleColor = $r('sys.color.ohos_id_color_text_primary');
    style.borderRadius = StyleManager.calcScaleSizePx(48);
    style.backgroundColor = '#99FFFFFF';
    style.textMargin = StyleManager.calcScaleSizePx(8);
    style.textHoverHeight = StyleManager.calcScaleSizePx(52);
    style.textHoverWidth = StyleManager.calcScaleSizePx(136);
    style.textHoverRadius = StyleManager.calcScaleSizePx(8);
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // CommonTemplate-iconComponent
  private static setIconComponentStyle(): void {
    let style = CommonTemplateStyleConfiguration.getIconComponentStyle();
    style.circleWidth = StyleManager.calcScaleSizePx(96);
    style.circleHeight = StyleManager.calcScaleSizePx(96);
    style.iconWidth = StyleManager.calcScaleSizePx(48);
    style.iconHeight = StyleManager.calcScaleSizePx(48);
    style.iconOffBG = '#1A182431';
    style.iconOnBG = '#FF007DFF';
    style.iconOnColor = '#FFFFFFFF';
    style.iconOffColor = $r('sys.color.ohos_id_color_secondary');
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // CommonTemplate-simpleToggleBase
  private static setSimpleToggleBaseStyle(): void{
    let style = CommonTemplateStyleConfiguration.getSimpleToggleBaseStyle();
    style.circleWidth = StyleManager.calcScaleSizePx(96);
    style.circleHeight = StyleManager.calcScaleSizePx(96);
    style.iconWidth = StyleManager.calcScaleSizePx(48);
    style.iconHeight = StyleManager.calcScaleSizePx(48);
    style.dragCircleWidth = StyleManager.calcScaleSizePx(120);
    style.dragCircleHeight = StyleManager.calcScaleSizePx(120);
    style.dragIconWidth = StyleManager.calcScaleSizePx(72);
    style.dragIconHeight = StyleManager.calcScaleSizePx(72);
    style.iconOffBG = '#1A182431';
    style.iconOnBG = '#FF007DFF';
    style.iconOnColor = '#FFFFFFFF';
    style.iconOffColor = $r('sys.color.ohos_id_color_secondary');
    style.componentGap = StyleManager.calcScaleSizePx(10);
    style.titleSize = StyleManager.calcScaleSizePx(24);
    style.titleColor = $r('sys.color.ohos_id_color_text_primary');
    style.textHoverWidth = StyleManager.calcScaleSizePx(136);
    style.textHoverHeight = StyleManager.calcScaleSizePx(36);
    style.textHoverRadius = StyleManager.calcScaleSizePx(8);
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // Brightness
  private static setBrightnessComponentStyle(): void{
    let style = BrightnessStyleConfiguration.getBrightnessComponentStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(26);
    style.marginRight = StyleManager.calcScaleSizePx(26);
    style.componentGap = StyleManager.calcScaleSizePx(26);
    style.brightnessIconColor = $r('sys.color.ohos_id_color_secondary');
    style.brightnessReduceWidth = StyleManager.calcScaleSizePx(44);
    style.brightnessReduceHeight = StyleManager.calcScaleSizePx(44);
    style.brightnessPlusWidth = StyleManager.calcScaleSizePx(44);
    style.brightnessPlusHeight = StyleManager.calcScaleSizePx(44);
    style.brightnessHeight = StyleManager.calcScaleSizePx(44);
    style.sliderHeight = StyleManager.calcScaleSizePx(40);
    style.sliderBlockColor = '#FFFFFFFF';
    style.sliderTrackColor = '#1A182431';
    style.sliderSelectedColor = '#FF007DFF';
  }
  // RingMode
  private static setControlCenterRingModeComponentStyle(): void{
    let style = RingModeStyleConfiguration.getControlCenterRingModeComponentStyle();
    style.onBgColor = '#FFFF9800';
  }
  // SimpleToggleLayoutEdit
  private static setSimpleToggleLayoutEditComponentStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditComponentStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(48);
    style.marginRight = StyleManager.calcScaleSizePx(48);
    style.marginTop = StyleManager.calcScaleSizePx(33);
    style.titleHeight = StyleManager.calcScaleSizePx(127);
    style.titleMarginBottom = StyleManager.calcScaleSizePx(0);
    style.upGridMarginTop = StyleManager.calcScaleSizePx(48);
    style.upGridMarginBottom = StyleManager.calcScaleSizePx(24);
    style.gridMarginLeft = StyleManager.calcScaleSizePx(18.24);
    style.gridMarginRight = StyleManager.calcScaleSizePx(18.24);
    style.msgMarginTop = StyleManager.calcScaleSizePx(0);
    style.msgMarginBottom = StyleManager.calcScaleSizePx(0);
    style.btnMarginTop = StyleManager.calcScaleSizePx(24);
    style.btnMarginBottom = StyleManager.calcScaleSizePx(32);
    style.borderRadius = StyleManager.calcScaleSizePx(48);
    style.upAreaBgColor = '#99FFFFFF';
    style.downAreaBgColor = '#4DFFFFFF';
    style.editBtnFontColor = '#FF007DFF';
    style.editBtnFontSize = StyleManager.calcScaleSizePx(32);
    style.editBtnBgColor = '#0D000000';
    style.editBtnMarginLeft = StyleManager.calcScaleSizePx(32);
    style.editBtnMarginRight = StyleManager.calcScaleSizePx(32);
    style.editBtnHeight = StyleManager.calcScaleSizePx(80);
    style.editBtnSpace = StyleManager.calcScaleSizePx(32);
  }
  // SimpleToggleLayoutEdit-title
  private static setSimpleToggleLayoutEditUpTitleStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditUpTitleStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(72);
    style.imageWidth = StyleManager.calcScaleSizePx(40);
    style.imageHeight = StyleManager.calcScaleSizePx(40);
    style.fontColor = '#FFFFFFFF';
    style.editTitleSpace = StyleManager.calcScaleSizePx(36);
    style.titleFontSize = StyleManager.calcScaleSizePx(40);
  }
  // SimpleToggleLayoutEdit-msg
  private static setSimpleToggleLayoutEditOptMsgStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditOptMsgStyle();
    style.fontSize = StyleManager.calcScaleSizePx(24);
    style.fontColor = $r('sys.color.ohos_id_color_text_secondary');
    style.height = StyleManager.calcScaleSizePx(98);
    style.marginLeftRight = StyleManager.calcScaleSizePx(44);
  }
  // SimpleToggleLayoutEdit-grid
  private static setSimpleToggleLayoutEditGridStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditGridStyle();
    style.columnGap = StyleManager.calcScaleSizePx(0);
    style.rowGap = StyleManager.calcScaleSize(0);
    style.rowHeight = StyleManager.calcScaleSize(162);
    style.dragBgSize = StyleManager.calcScaleSizePx(120);
  }
  // EditDialog
  private static setControlEditDialogStyle(): void {
    let style = ControlCenterStyleConfiguration.getControlEditDialogStyle();
    style.editDialogHeight = StyleManager.calcScaleSizePx(217);
    style.editDialogWidth = StyleManager.calcScaleSizePx(670);
    style.editDialogFontSize = StyleManager.calcScaleSizePx(30);
    style.editDialogBtnMarginLF = StyleManager.calcScaleSizePx(24);
    style.editDialogFontHeight = StyleManager.calcScaleSizePx(36);
    style.editDialogBtnMarginTop = StyleManager.calcScaleSizePx(36);
    style.editDialogButtonSize = StyleManager.calcScaleSizePx(30);
    style.editDialogDividerHeight = StyleManager.calcScaleSizePx(36);
    style.editDialogButtonHeight = StyleManager.calcScaleSizePx(54);
    style.editDialogRadius = StyleManager.calcScaleSizePx(36);
    style.editDialogBtnWidth = StyleManager.calcScaleSizePx(311);
    style.editDialogFontMarginTop = StyleManager.calcScaleSizePx(52);
    style.editDialogColor = '#FFFFFFFF';
    style.editDialogBtnFontColor = '#FF0000FF';
    style.editDialogBtnBgColor = '#00FFFFFF';
    style.editDialogDividerColor = $r('sys.color.ohos_id_color_secondary');
    style.editDialogDividerWidth = StyleManager.calcScaleSizePx(1);
    Log.showDebug(TAG, 'getControlEditDialogStyle');
  }

  static number2px(n: number): string {
    return n.toString() + 'px';
  }

  static calcScaleSize(n: number): number {
    return n * StyleManager.maxWidth / StyleManager.STANDARD_DISPLAY_WIDTH;
  }

  static calcScaleSizePx(n: number): string {
    return StyleManager.number2px(StyleManager.calcScaleSize(n));
  }
}