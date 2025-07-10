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

const TAG = 'statusBar-StyleConfiguration';

export class VerticalStatusBarItemLoadComponentStyle {
  statusBarVerticalComponentHeight: number = vp2px(Constants.VERTICAL_COMPONENT_HEIGHT_VP);
}

export class StatusBarNotificationIconStyle {
  iconWidth: Length = $r('app.float.status_bar_notification_icon_width');
  iconHeight: Length = $r('app.float.status_bar_notification_icon_height');
  iconSpace: Length = $r('app.float.status_bar_notification_icon_space');
}

export class IconItemComponentStyle {
  stackHeight: Length = $r('app.float.status_bar_icon_item_stack_height');
  stackPadding: Length = $r('app.float.status_bar_icon_item_stack_padding');
  stackBorderRadius: Length = $r('app.float.status_bar_icon_item_stack_border_radius');
  stackBgColorSelected: ResourceColor = $r('app.color.status_bar_icon_item_stack_bg_color_selected');
  stackBgColorUnSelected: ResourceColor = $r('app.color.status_bar_icon_item_stack_bg_color_un_selected');
  componentSpace: Length = $r('app.float.status_bar_icon_item_component_space');
  iconWidth: Length = $r('app.float.status_bar_icon_item_icon_width');
  iconHeight: Length = $r('app.float.status_bar_icon_item_icon_height');
  marginLeft: Length = $r('app.float.status_bar_icon_item_margin_left');
  marginRight: Length = $r('app.float.status_bar_icon_item_margin_right');
}

export class PluginIconItemComponentStyle {
  iconWidth: number = vp2px(Constants.PLUGIN_COMPONENT_ICON_WIDTH);
}

export default class StyleConfiguration {
  static getVerticalStatusBarItemLoadComponentStyle(): VerticalStatusBarItemLoadComponentStyle {
    const key: string = TAG + '-VerticalStatusBarItemLoadComponent';
    return CommonStyleManager.getStyle(key, VerticalStatusBarItemLoadComponentStyle);
  }

  static getStatusBarNotificationIconStyle(): StatusBarNotificationIconStyle {
    const key: string = TAG + '-StatusBarNotificationIcon';
    return CommonStyleManager.getStyle(key, StatusBarNotificationIconStyle);
  }

  static getIconItemComponentStyle(): IconItemComponentStyle {
    const key: string = TAG + '-IconItemComponent';
    return CommonStyleManager.getStyle(key, IconItemComponentStyle);
  }

  static getPluginIconItemComponentStyle(): PluginIconItemComponentStyle {
    const key: string = TAG + '-PluginIconItemComponent';
    return CommonStyleManager.getStyle(key, PluginIconItemComponentStyle);
  }
}