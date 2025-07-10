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
import Constants from './constants';
import deviceInfo from '@ohos.deviceInfo';

const TAG = 'DropdownPanel-StyleConfiguration';

export class ControlStyle {
  statusBarHeight: Length = deviceInfo.deviceType === 'phone' ? $r("app.float.phone_status_bar_height") : $r('app.float.status_bar_height');
}

export class NotificationStyle {
  statusBarHeight: Length = deviceInfo.deviceType === 'phone' ? $r("app.float.phone_status_bar_height") : $r('app.float.status_bar_height');
  deleteAllImageWidth: Length = $r('app.float.delete_all_image_width');
  deleteAllImageHeight: Length = $r('app.float.delete_all_image_height');
  deleteAllImageBorderRadius: Length = $r('app.float.delete_all_image_border_radius');
  deleteAllImageOpacity: Length = $r('app.float.delete_all_image_opacity');
}

export class QuicklySettingStyle {
  quicklySetTimeFontSize: Length = $r('app.float.quickly_setting_time_font_size');
  quicklySetTimeFontColor: ResourceColor = $r('app.color.quickly_setting_time_font_color');
  quicklyDateFontColor: ResourceColor = $r('app.color.quickly_setting_date_font_color');
  quicklyDateFontSize: Length = $r('app.float.quickly_setting_date_font_size');
  quicklySetTextFlexMarginLeft: Length = $r('app.float.quickly_setting_text_flex_margin_left');
  quicklySetImageWidth: Length = $r('app.float.quickly_setting_image_width');
  quicklySttImageHeight: Length = $r('app.float.quickly_setting_image_height');
  quicklySetImageMarginRight: Length = $r('app.float.quickly_setting_image_margin_right');
  quickLySettingBorderRadius: Length = $r('app.float.quickly_setting_border_radius');
  quickLySettingMarginLeft: Length = $r('app.float.quickly_setting_margin_left');
  quickLySettingMarginRight: Length = $r('app.float.quickly_setting_margin_right');
  quickSettingH: Length = Constants.QUICKLY_SETTING_H;
}

export class DateItemStyle {
  quicklySetDateItemFontSize: Length = $r('app.float.quickly_setting_date_font_size');
  quicklySetDateItemFontColor: ResourceColor = $r('app.color.quickly_setting_date_font_color');
}

export default class StyleConfiguration {
  static getControlStyle(): ControlStyle {
    const key: string = TAG + '-Control';
    return CommonStyleManager.getStyle(key, ControlStyle);
  }

  static getNotificationStyle(): NotificationStyle {
    const key: string = TAG + '-Notification';
    return CommonStyleManager.getStyle(key, NotificationStyle);
  }

  static getQuicklySettingStyle(): QuicklySettingStyle {
    const key: string = TAG + '-QuicklySetting';
    return CommonStyleManager.getStyle(key, QuicklySettingStyle);
  }

  static getDateItemStyle(): DateItemStyle {
    const key: string = TAG + '-DateItem';
    return CommonStyleManager.getStyle(key, DateItemStyle);
  }
}