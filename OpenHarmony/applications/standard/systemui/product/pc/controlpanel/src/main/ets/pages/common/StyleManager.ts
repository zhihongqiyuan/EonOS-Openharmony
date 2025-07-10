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
import IndexStyleConfiguration from './StyleConfiguration';

const TAG = 'ControlPanel-StyleManager';

export default class StyleManager {
  static readonly STANDARD_DISPLAY_WIDTH = 1280;
  static readonly STANDARD_DISPLAY_HEIGHT = 800;
  static maxWidth: number = StyleManager.STANDARD_DISPLAY_WIDTH;

  static setStyle(): void{
    Log.showDebug(TAG, 'setStyle');

    let dis = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_CONTROL_PANEL, 'dis');
    StyleManager.maxWidth = dis.width;

    // Index
    StyleManager.setIndexStyle();
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
  // Index
  private static setIndexStyle(): void {
    let style = IndexStyleConfiguration.getIndexStyle();
    style.borderRadius = StyleManager.calcScaleSizePx(24);
  }
  // ControlCenter
  private static setControlCenterComponentStyle(): void {
    let style = ControlCenterStyleConfiguration.getControlCenterComponentStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(20);
    style.marginRight = StyleManager.calcScaleSizePx(20);
    style.marginTop = StyleManager.calcScaleSizePx(0);
    style.upTitleHeight = StyleManager.calcScaleSizePx(80);
    style.titleMarginBottom = StyleManager.calcScaleSizePx(0);
    style.toggleAreaGap = StyleManager.calcScaleSizePx(12);
    style.simpleToggleLayoutMarginTop = StyleManager.calcScaleSizePx(23);
    style.simpleToggleLayoutMarginBottom = StyleManager.calcScaleSizePx(0);
    style.brightnessMarginBottom = StyleManager.calcScaleSizePx(12);
    style.componentBorderRadius = StyleManager.calcScaleSizePx(24);
    style.componentBackgroundColor = '#99FFFFFF';
  }
  // ControlCenter-Title
  private static setControlCenterUpTitleStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterUpTitleStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(24);
    style.marginRight = StyleManager.calcScaleSizePx(21);
    style.fontSize = StyleManager.calcScaleSizePx(20);
    style.fontColor = '#000000';
    style.imageWidth = StyleManager.calcScaleSizePx(24);
    style.imageHeight = StyleManager.calcScaleSizePx(24);
    style.imageHoverWidth = StyleManager.calcScaleSizePx(32);
    style.imageHoverHeight = StyleManager.calcScaleSizePx(32);
    style.imageHoverRadius = StyleManager.calcScaleSizePx(8);
    style.imageHoverColor = 'rgba(0, 0, 0, 0.05)';
    style.imageTransparentColor = 'rgba(255, 255, 255, 0)';
    style.upTitleSpace = StyleManager.calcScaleSizePx(24);
    style.titleImageColor = '#ff182431';
  }
  // ControlCenter-ComplexToggleLayout
  private static setControlCenterComplexToggleLayoutStyle(): void {
    let style = ControlCenterStyleConfiguration.getControlCenterComplexToggleLayoutStyle();
    style.columnGap = StyleManager.calcScaleSizePx(12);
    style.rowGap = StyleManager.calcScaleSize(12);
    style.rowHeight = StyleManager.calcScaleSize(66);
  }
  // ControlCenter-SimpleToggleLayout
  private static setControlCenterSimpleToggleLayoutStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlCenterSimpleToggleLayoutStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(11);
    style.marginRight = StyleManager.calcScaleSizePx(11);
    style.columnGap = StyleManager.calcScaleSizePx(0);
    style.rowGap = StyleManager.calcScaleSize(0);
    style.rowHeight = StyleManager.calcScaleSize(81);
  }
  // CommonTemplate-iconTitleBase
  private static setIconTitleBaseStyle(): void{
    let style = CommonTemplateStyleConfiguration.getIconTitleBaseStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(8);
    style.marginRight = StyleManager.calcScaleSizePx(4);
    style.componentGap = StyleManager.calcScaleSizePx(8);
    style.titleSize = StyleManager.calcScaleSizePx(12);
    style.titleColor = $r('sys.color.ohos_id_color_text_secondary'),
    style.borderRadius = StyleManager.calcScaleSizePx(24);
    style.backgroundColor = '#99FFFFFF';
    style.textMargin = StyleManager.calcScaleSizePx(8);
    style.textHoverHeight = StyleManager.calcScaleSizePx(26);
    style.textHoverWidth = StyleManager.calcScaleSizePx(68);
    style.textHoverRadius = StyleManager.calcScaleSizePx(4);
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // CommonTemplate-iconComponent
  private static setIconComponentStyle(): void {
    let style = CommonTemplateStyleConfiguration.getIconComponentStyle();
    style.circleWidth = StyleManager.calcScaleSizePx(48);
    style.circleHeight = StyleManager.calcScaleSizePx(48);
    style.iconWidth = StyleManager.calcScaleSizePx(24);
    style.iconHeight = StyleManager.calcScaleSizePx(24);
    style.iconOffBG = '#1A000000';
    style.iconOnBG = '#FF007DFF';
    style.iconOnColor = '#FFFFFFFF';
    style.iconOffColor = $r('sys.color.ohos_id_color_secondary');
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // CommonTemplate-simpleToggleBase
  private static setSimpleToggleBaseStyle(): void {
    let style = CommonTemplateStyleConfiguration.getSimpleToggleBaseStyle();
    style.circleWidth = StyleManager.calcScaleSizePx(48);
    style.circleHeight = StyleManager.calcScaleSizePx(48);
    style.iconWidth = StyleManager.calcScaleSizePx(24);
    style.iconHeight = StyleManager.calcScaleSizePx(24);
    style.dragCircleWidth = StyleManager.calcScaleSizePx(60);
    style.dragCircleHeight = StyleManager.calcScaleSizePx(60);
    style.dragIconWidth = StyleManager.calcScaleSizePx(36);
    style.dragIconHeight = StyleManager.calcScaleSizePx(36);
    style.iconOffBG = '#1A000000';
    style.iconOnBG = '#FF007DFF';
    style.iconOnColor = '#FFFFFFFF';
    style.iconOffColor = $r('sys.color.ohos_id_color_secondary');
    style.componentGap = StyleManager.calcScaleSizePx(4);
    style.titleSize = StyleManager.calcScaleSizePx(12);
    style.titleColor = $r('sys.color.ohos_id_color_text_secondary');
    style.textHoverWidth = StyleManager.calcScaleSizePx(68);
    style.textHoverHeight = StyleManager.calcScaleSizePx(18);
    style.textHoverRadius = StyleManager.calcScaleSizePx(4);
    style.hoverColor = 'rgba(0, 0, 0, 0.05)';
    style.transparentColor = 'rgba(255, 255, 255, 0)';
  }
  // Brightness
  private static setBrightnessComponentStyle(): void{
    let style = BrightnessStyleConfiguration.getBrightnessComponentStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(12.5);
    style.marginRight = StyleManager.calcScaleSizePx(12.5);
    style.componentGap = StyleManager.calcScaleSizePx(8.5);
    style.brightnessIconColor = $r('sys.color.ohos_id_color_secondary');
    style.brightnessReduceWidth = StyleManager.calcScaleSizePx(22);
    style.brightnessReduceHeight = StyleManager.calcScaleSizePx(22);
    style.brightnessPlusWidth = StyleManager.calcScaleSizePx(22);
    style.brightnessPlusHeight = StyleManager.calcScaleSizePx(22);
    style.brightnessHeight = StyleManager.calcScaleSizePx(40);
    style.sliderHeight = px2vp(StyleManager.calcScaleSize(20)).toString() + 'vp';
    style.sliderBlockColor = '#FFFFFFFF';
    style.sliderTrackColor = '#0D000000';
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
    style.marginLeft = StyleManager.calcScaleSizePx(0);
    style.marginRight = StyleManager.calcScaleSizePx(0);
    style.marginTop = StyleManager.calcScaleSizePx(0);
    style.titleHeight = StyleManager.calcScaleSizePx(56);
    style.titleMarginBottom = StyleManager.calcScaleSizePx(0);
    style.upGridMarginTop = StyleManager.calcScaleSizePx(4);
    style.upGridMarginBottom = StyleManager.calcScaleSizePx(20);
    style.gridMarginLeft = StyleManager.calcScaleSizePx(21);
    style.gridMarginRight = StyleManager.calcScaleSizePx(21);
    style.msgMarginTop = StyleManager.calcScaleSizePx(0);
    style.msgMarginBottom = StyleManager.calcScaleSizePx(4);
    style.btnMarginTop = StyleManager.calcScaleSizePx(4);
    style.btnMarginBottom = StyleManager.calcScaleSizePx(16);
    style.borderRadius = StyleManager.calcScaleSizePx(24);
    style.upAreaBgColor = '#00FFFFFF';
    style.downAreaBgColor = '#4DFFFFFF';
    style.editBtnFontColor = '#FF007DFF';
    style.editBtnFontSize = $r('sys.float.ohos_id_text_size_button1');
    style.editBtnBgColor = '#0D000000';
    style.editBtnMarginLeft = StyleManager.calcScaleSizePx(16);
    style.editBtnMarginRight = StyleManager.calcScaleSizePx(16);
    style.editBtnHeight = StyleManager.calcScaleSizePx(40);
    style.editBtnSpace = StyleManager.calcScaleSizePx(16);
  }
  // SimpleToggleLayoutEdit
  private static setSimpleToggleLayoutEditUpTitleStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditUpTitleStyle();
    style.marginLeft = StyleManager.calcScaleSizePx(24);
    style.imageWidth = StyleManager.calcScaleSizePx(20);
    style.imageHeight = StyleManager.calcScaleSizePx(20);
    style.fontColor = '#FF000000';
    style.editTitleSpace = StyleManager.calcScaleSizePx(18);
    style.titleFontSize = StyleManager.calcScaleSizePx(20);
  }
  // SimpleToggleLayoutEdit
  private static setSimpleToggleLayoutEditOptMsgStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditOptMsgStyle();
    style.fontSize = StyleManager.calcScaleSizePx(12);
    style.fontColor = $r('sys.color.ohos_id_color_text_secondary');
    style.height = StyleManager.calcScaleSizePx(41);
    style.marginLeftRight = StyleManager.calcScaleSizePx(30);
  }
  // SimpleToggleLayoutEdit-grid
  private static setSimpleToggleLayoutEditGridStyle(): void{
    let style = ControlCenterStyleConfiguration.getSimpleToggleLayoutEditGridStyle();
    style.columnGap = StyleManager.calcScaleSizePx(0);
    style.rowGap = StyleManager.calcScaleSize(0);
    style.rowHeight = StyleManager.calcScaleSize(80);
    style.dragBgSize = StyleManager.calcScaleSizePx(60);
  }
  // EditDialog
  private static setControlEditDialogStyle(): void{
    let style = ControlCenterStyleConfiguration.getControlEditDialogStyle();
    style.editDialogHeight = StyleManager.calcScaleSizePx(121);
    style.editDialogWidth = StyleManager.calcScaleSizePx(374);
    style.editDialogFontSize = StyleManager.calcScaleSizePx(18);
    style.editDialogBtnMarginLF = StyleManager.calcScaleSizePx(13);
    style.editDialogFontHeight = StyleManager.calcScaleSizePx(20);
    style.editDialogBtnMarginTop = StyleManager.calcScaleSizePx(20);
    style.editDialogButtonSize = StyleManager.calcScaleSizePx(18);
    style.editDialogDividerHeight = StyleManager.calcScaleSizePx(20);
    style.editDialogButtonHeight = StyleManager.calcScaleSizePx(30);
    style.editDialogRadius = StyleManager.calcScaleSizePx(20);
    style.editDialogBtnWidth = StyleManager.calcScaleSizePx(173);
    style.editDialogFontMarginTop = StyleManager.calcScaleSizePx(29);
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