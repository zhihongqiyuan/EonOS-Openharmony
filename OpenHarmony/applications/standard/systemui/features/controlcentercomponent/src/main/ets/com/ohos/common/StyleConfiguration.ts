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

import CommonStyleManager from '../../../../../../../../common/src/main/ets/default/CommonStyleManager';
import Constants from './Constants';

const TAG = 'ControlCenter-StyleConfiguration';

export class ControlCenterComponentStyle {
  marginLeft: Length = $r('app.float.control_center_margin_left');
  marginRight: Length = $r('app.float.control_center_margin_right');
  marginTop: Length = $r('app.float.control_center_margin_top');
  titleMarginBottom: Length = $r('app.float.control_center_title_margin_bottom');
  upTitleHeight: Length = $r('app.float.control_center_title_height');
  toggleAreaGap: Length = $r('app.float.control_center_toggle_area_gap');
  simpleToggleLayoutMarginTop: Length = $r('app.float.control_center_simple_toggle_layout_margin_top');
  simpleToggleLayoutMarginBottom: Length = $r('app.float.control_center_simple_toggle_layout_margin_bottom');
  brightnessMarginBottom: Length = $r('app.float.control_center_brightness_margin_bottom');
  componentBorderRadius: Length = $r('app.float.control_center_component_border_radius');
  componentBackgroundColor: ResourceColor = $r('app.color.control_center_component_background');
}

export class ControlCenterUpTitleStyle {
  marginLeft: Length = $r('app.float.control_center_title_margin_left');
  marginRight: Length = $r('app.float.control_center_title_margin_right');
  fontSize: Length = $r('app.float.control_center_title_font_size');
  fontColor: ResourceColor = $r('app.color.control_center_title_font_color');
  titleImageColor: ResourceColor = $r('app.color.control_center_title_image_color');
  imageWidth: Length = $r('app.float.control_center_title_icon_width');
  imageHeight: Length = $r('app.float.control_center_title_icon_height');
  imageHoverWidth: number | string = Constants.CONTROL_CENTER_TITLE_ICON_HOVER_WIDTH;
  imageHoverHeight: number | string = Constants.CONTROL_CENTER_TITLE_ICON_HOVER_HEIGHT;
  imageHoverRadius: number | string = Constants.CONTROL_CENTER_TITLE_ICON_HOVER_RADIUS;
  imageHoverColor: ResourceColor = $r('app.color.control_center_title_icon_hover_color');
  imageTransparentColor: ResourceColor = $r('app.color.control_center_title_icon_transparent_color');
  upTitleSpace: Length = $r('app.float.control_center_title_space');
}

export class ControlCenterComplexToggleLayoutStyle {
  columnGap: Length = $r('app.float.control_center_complex_toggle_column_gap');
  rowGap: Length = Constants.COMPLEX_TOGGLE_GRID_ROW_GAP;
  rowHeight: Length = Constants.COMPLEX_TOGGLE_GRID_ROW_HEIGHT;
}

export class ControlCenterSimpleToggleLayoutStyle {
  marginLeft: Length = $r('app.float.control_center_simple_toggle_layout_margin_left');
  marginRight: Length = $r('app.float.control_center_simple_toggle_layout_margin_right');
  columnGap: Length = $r('app.float.control_center_simple_toggle_column_gap');
  rowGap: Length = Constants.SIMPLE_TOGGLE_GRID_ROW_GAP;
  rowHeight: Length = Constants.SIMPLE_TOGGLE_GRID_ROW_HEIGHT;
}

export class SimpleToggleLayoutEditComponentStyle {
  marginLeft: Length = $r('app.float.simple_toggle_edit_margin_left');
  marginRight: Length = $r('app.float.simple_toggle_edit_margin_right');
  marginTop: Length = $r('app.float.simple_toggle_edit_margin_top');
  titleHeight: Length = $r('app.float.simple_toggle_edit_title_height');
  titleMarginBottom: Length = $r('app.float.simple_toggle_edit_title_margin_bottom');
  upGridMarginTop: Length = $r('app.float.simple_toggle_edit_up_grid_margin_top');
  upGridMarginBottom: Length = $r('app.float.simple_toggle_edit_up_grid_margin_bottom');
  gridMarginLeft: Length = $r('app.float.simple_toggle_edit_grid_margin_left');
  gridMarginRight: Length = $r('app.float.simple_toggle_edit_grid_margin_right');
  msgMarginTop: Length = $r('app.float.simple_toggle_edit_msg_margin_top');
  msgMarginBottom: Length = $r('app.float.simple_toggle_edit_msg_margin_bottom');
  btnMarginTop: Length = $r('app.float.simple_toggle_edit_btn_margin_top');
  btnMarginBottom: Length = $r('app.float.simple_toggle_edit_btn_margin_bottom');
  borderRadius: Length = $r('app.float.simple_toggle_edit_border_radius');
  upAreaBgColor: ResourceColor = $r('app.color.center_edit_up_area_bg_color');
  downAreaBgColor: ResourceColor = $r('app.color.center_edit_down_area_bg_color');
  editBtnFontColor: ResourceColor = $r('app.color.simple_toggle_edit_btn_font_color');
  editBtnFontSize: Length = $r('sys.float.ohos_id_text_size_button1');
  editBtnBgColor: ResourceColor = $r('app.color.simple_toggle_edit_btn_bg_color');
  editBtnMarginLeft: Length = $r('app.float.simple_toggle_edit_btn_margin_left');
  editBtnMarginRight: Length = $r('app.float.simple_toggle_edit_btn_margin_right');
  editBtnHeight: Length = $r('app.float.simple_toggle_edit_btn_height');
  editBtnSpace: Length = $r('app.float.simple_toggle_edit_btn_space');
}

export class SimpleToggleLayoutEditUpTitleStyle {
  marginLeft: Length = $r('app.float.simple_toggle_edit_title_margin_left');
  imageWidth: Length = $r('app.float.simple_toggle_edit_title_icon_width');
  imageHeight: Length = $r('app.float.simple_toggle_edit_title_icon_height');
  fontColor: ResourceColor = $r('app.color.simple_toggle_edit_title_font_color');
  editTitleSpace: Length = $r('app.float.simple_toggle_edit_icon_width');
  titleFontSize: Length = $r('app.float.simple_toggle_edit_title_font_size');
}

export class SimpleToggleLayoutEditOptMsgStyle {
  fontSize: Length = $r('app.float.simple_toggle_edit_opt_msg_font_size');
  fontColor: ResourceColor = $r('sys.color.ohos_id_color_text_secondary');
  height: Length = $r('app.float.simple_toggle_edit_opt_msg_height');
  marginLeftRight: Length = $r('app.float.simple_toggle_edit_opt_msg_margin_left_right');
}

export class SimpleToggleLayoutEditGridStyle {
  columnGap: Length = $r('app.float.simple_toggle_edit_grid_column_gap');
  rowGap: number = Constants.SIMPLE_TOGGLE_GRID_ROW_GAP;
  rowHeight: number = Constants.SIMPLE_TOGGLE_GRID_ROW_HEIGHT;
  dragBgSize: Length = $r('app.float.simple_toggle_edit_grid_drag_bg_size');
}

export class ControlEditDialogStyle {
  editDialogHeight: Length = $r('app.float.simple_toggle_edit_dialog_height');
  editDialogWidth: Length = $r('app.float.simple_toggle_edit_dialog_width');
  editDialogFontSize: Length = $r('app.float.simple_toggle_edit_dialog_font_size');
  editDialogBtnMarginLF: Length = $r('app.float.simple_toggle_edit_dialog_btn_margin_lf');
  editDialogFontHeight: Length = $r('app.float.simple_toggle_edit_dialog_font_height');
  editDialogBtnMarginTop: Length = $r('app.float.simple_toggle_edit_dialog_btn_margin_top');
  editDialogButtonSize: Length = $r('app.float.simple_toggle_edit_dialog_btn_font_size');
  editDialogDividerHeight: Length = $r('app.float.simple_toggle_edit_dialog_divider_height');
  editDialogButtonHeight: Length = $r('app.float.simple_toggle_edit_dialog_btn_height');
  editDialogRadius: Length = $r('app.float.simple_toggle_edit_dialog_radius');
  editDialogBtnWidth: Length = $r('app.float.simple_toggle_edit_dialog_btn_width');
  editDialogColor: ResourceColor = $r('app.color.center_edit_dialog_color');
  editDialogBtnFontColor: ResourceStr = $r('app.color.center_edit_dialog_btn_font_color');
  editDialogBtnBgColor: ResourceColor = $r('app.color.center_edit_dialog_btn_bg_color');
  editDialogDividerColor: ResourceColor = $r('sys.color.ohos_id_color_secondary');
  editDialogFontMarginTop: Length = $r('app.float.simple_toggle_edit_dialog_font_margin_top');
  editDialogDividerWidth: number | string = Constants.SIMPLE_TOGGLE_EDIT_DIALOG_DIVIDER_WIDTH;
}

export default class StyleConfiguration {
  static getControlCenterComponentStyle(): ControlCenterComponentStyle {
    const key: string = TAG + '-ControlCenterComponent';
    return CommonStyleManager.getStyle(key, ControlCenterComponentStyle);
  }

  static getControlCenterUpTitleStyle(): ControlCenterUpTitleStyle {
    const key: string = TAG + '-ControlCenterUpTitle';
    return CommonStyleManager.getStyle(key, ControlCenterUpTitleStyle);
  }

  static getControlCenterComplexToggleLayoutStyle(): ControlCenterComplexToggleLayoutStyle {
    const key: string = TAG + '-ControlCenterComplexToggleLayout';
    return CommonStyleManager.getStyle(key, ControlCenterComplexToggleLayoutStyle);
  }

  static getControlCenterSimpleToggleLayoutStyle(): ControlCenterSimpleToggleLayoutStyle {
    const key: string = TAG + '-ControlCenterSimpleToggleLayout';
    return CommonStyleManager.getStyle(key, ControlCenterSimpleToggleLayoutStyle);
  }

  static getSimpleToggleLayoutEditComponentStyle(): SimpleToggleLayoutEditComponentStyle {
    const key: string = TAG + '-SimpleToggleLayoutEditComponent';
    return CommonStyleManager.getStyle(key, SimpleToggleLayoutEditComponentStyle);
  }

  static getSimpleToggleLayoutEditUpTitleStyle(): SimpleToggleLayoutEditUpTitleStyle {
    const key: string = TAG + '-SimpleToggleLayoutEditUpTitle';
    return CommonStyleManager.getStyle(key, SimpleToggleLayoutEditUpTitleStyle);
  }

  static getSimpleToggleLayoutEditOptMsgStyle(): SimpleToggleLayoutEditOptMsgStyle {
    const key: string = TAG + '-SimpleToggleLayoutEditOptMsg';
    return CommonStyleManager.getStyle(key, SimpleToggleLayoutEditOptMsgStyle);
  }

  static getSimpleToggleLayoutEditGridStyle(): SimpleToggleLayoutEditGridStyle {
    const key: string = TAG + '-SimpleToggleLayoutEditGrid';
    return CommonStyleManager.getStyle(key, SimpleToggleLayoutEditGridStyle);
  }

  static getControlEditDialogStyle(): ControlEditDialogStyle {
    const key: string = TAG + '-ControlEditDialog';
    return CommonStyleManager.getStyle(key, ControlEditDialogStyle);
  }
}