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

const TAG = 'volumepanel-StyleConfiguration';

export class VolumePanelComponentStyle {
  volumePanelSliderMarginTop: Length = $r('app.float.volume_panel_component_slider_margin_top');
  volumePanSliderWidth: Length = $r('app.float.volume_panel_component_slider_width');
  volumePanSliderHeight: Length = $r('app.float.volume_panel_component_slider_height');
  volumePanelSliderMarginBottom: Length = $r('app.float.volume_panel_component_slider_margin_bottom');
  volumePanelMutBtnIconSize: Length = $r('app.float.volume_panel_component_mute_btn_icon_size');
  volumePanelMutBtnIconMarginBottom: Length = $r('app.float.volume_panel_component_mute_btn_icon_margin_bottom');
  volumePanelMuteBtnHeight: Length = $r('app.float.volume_panel_component_mute_btn_height');
  volumePanelSettingIconSize: Length = $r('app.float.volume_panel_component_setting_icon_size');
  volumePanelBackground: ResourceColor = $r('app.color.panel_background');
  volumePanelSliderBlockColor: ResourceColor = $r('app.color.volume_slider_block_color');
  volumePanelDividerHeight: number | string = Constants.VOLUME_PANEL_DIVIDER_HEIGHT;
  volumePanelBorderRadius: Length = $r('app.float.volume_panel_border_radius');
  volumeDividerWidth: Length = $r('app.float.volume_divider_width');
  volumeSliderTrackColor: ResourceColor = $r('app.color.volume_slider_track_color');
  volumeSelectedColor: ResourceColor = $r('app.color.volume_slider_selected_color');
  volumeButtonBackgroundColor: ResourceColor = $r('app.color.volume_button_background_color');
  volumePanelRingModeColor: ResourceColor = $r('app.color.volume_ring_mode_color');
  volumePanelDividerColor: ResourceColor = $r('app.color.volume_divider_color');
  volumePanelSettingColor: ResourceColor = $r('sys.color.ohos_id_color_secondary');
  volumePanelSettingButtonSize: Length = $r('app.float.volume_panel_component_setting_btn_icon_size');
}

export default class StyleConfiguration {
  static getVolumePanelComponentStyle(): VolumePanelComponentStyle {
    const key: string = TAG + '-VolumePanelComponent';
    return CommonStyleManager.getStyle(key, VolumePanelComponentStyle);
  }
}