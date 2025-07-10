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

import CommonStyleManager from '../../../../../../../common/src/main/ets/default/CommonStyleManager';

const TAG = 'battery-StyleConfiguration';

export class BatteryComponentStyle {
  componentGap: Length = $r('app.float.battery_component_gap');
}

export class BatteryPicStyle {
  picGap: Length = $r('app.float.battery_component_pic_gap');
  picBodyWidth: Length = $r('app.float.battery_component_pic_body_width');
  picBodyHeight: Length = $r('app.float.battery_component_pic_body_height');
  picBodyPadding: Length = $r('app.float.battery_component_pic_body_padding');
  picBodyBorderWidth: Length = $r('app.float.battery_component_pic_body_border_width');
  picBorderRadius: Length = $r('app.float.battery_component_pic_border_radius');
  picHeadBorderRadius: Length = $r('app.float.battery_component_pic_head_radius');
  picChargingColor: ResourceColor = $r('app.color.battery_component_pic_charging_color');
  picLevelLowColor: ResourceColor = $r('app.color.battery_component_pic_level_low_color');
  picHeadWidth: Length = $r('app.float.battery_component_pic_head_width');
  picHeadHeight: Length = $r('app.float.battery_component_pic_head_height');
}

export default class StyleConfiguration {
  static getBatteryComponentStyle(): BatteryComponentStyle {
    const key: string = TAG + '-BatteryComponent';
    return CommonStyleManager.getStyle(key, BatteryComponentStyle);
  }

  static getBatteryPicStyle(): BatteryPicStyle {
    const key: string = TAG + '-BatteryPicComponent';
    return CommonStyleManager.getStyle(key, BatteryPicStyle);
  }
}