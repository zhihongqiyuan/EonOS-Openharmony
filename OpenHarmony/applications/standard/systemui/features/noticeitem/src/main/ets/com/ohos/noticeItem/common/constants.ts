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
import notificationManager from '@ohos.notificationManager';
import image from '@ohos.multimedia.image';
import { WantAgent } from '@ohos.wantAgent';

export default class Constants {
  static readonly NOTIFICATION_TYPE_BASIC = '0';
  static readonly NOTIFICATION_TYPE_LONG = '1';
  static readonly NOTIFICATION_TYPE_PICTURE = '2';
  static readonly NOTIFICATION_TYPE_MULTILINE = '4';
  static readonly NOTIFICATION_TYPE_SOCIAL = '3';
  static readonly NOTIFICATION_TYPE_MEDIA = '5';
  static readonly NOTIFICATION_TYPE_NONE = 0;
  static readonly NOTIFICATION_TYPE_OPEN = 1;
  static readonly NOTIFICATION_TYPE_CLOSE = 2;
  static readonly TEXT_MAX_LENGTH = 21;
  static readonly TEXT_EXPANDED_MAX_LENGTH = 13;
  static readonly TOUCH_TYPE_DOWN = 0;
  static readonly TOUCH_TYPE_UP = 1;
  static readonly TOUCH_TYPE_MOVE = 2;
  static readonly HIDDEN_TRANSLATE_X = 150;
  static readonly DISPLAY_TRANSLATE_X = -150;
  static readonly REMOVE_TRANSLATE_X = 150;
  static readonly DEFAULT_MAX_LINES= 1;
  static readonly SINGLE_LINE= 1;
  static readonly EXPENDED_MAX_LINES= 8;
  static readonly CONTENT_LINE_HEIGHT = 20;
  static readonly SETTING_CONT_HEIGHT = '40vp';
  static readonly SETTING_DIALOG_WITH = '330vp';
  static readonly SETTING_CONTENT_WITH = '100%';
  static readonly SETTING_DIALOG_HEIGHT = '192vp';
  static readonly SETTING_DIALOG_WIDTH = '362vp';
  static readonly CONFIRM_DIALOG_HEIGHT = '146vp';
  static readonly CONFIRM_DIALOG_WITH = '280';
  static readonly CONFIRM_BUTTON_WITH = '140';
  static readonly QUICKLY_SETTING_H = 83;
  static readonly ERROR_CALLBACK = 0;
  static readonly SUCCESS_CALLBACK = 1;
  static readonly KEY_INPUT = 'inputAction';
  static readonly FULL_CONTAINER_WIDTH = '100%';
}

export class NotificationLayout {
  static readonly ACTION_HEIGHT = 48;
  static readonly ACTION_MARGIN = 12;
  static readonly INPUT_IMAGE_SIZE = 24;
  static readonly INPUT_TEXT_HEIGHT = 40;
  static readonly INPUT_LAYOUT_HEIGHT = 82;
  static readonly NOTIFICATION_PIC_SIZE = 40;
  static readonly ICON_SIZE = 24;
  static readonly TEXT_LINE_HEIGHT = 20;
  static readonly BUTTON_SIZE = 40;
  static readonly ICON_MARGIN = 12;
  static readonly SCROLL_THRESHOLD = 10;
  static readonly TITLE_IMAGE_SIZE = 16;
  static readonly TITLE_HEIGHT = 40;
  static readonly ITEM_MARGIN = 12;
  static readonly EXPAND_PADDING_BOTTOM = 20;
}

export class InputActionButtonData {
  isSending = false;
  content = '';
}

interface RuleData {
  isAllowBanner?: boolean;
  isAllowSound?: boolean;
  isAllowVibrationValues?: boolean;
  isAllowStatusBarShow?: boolean;
  isAllowNotificationListShow?: boolean;
}

export interface NotificationItemData {
  id: string;
  hashcode: string;
  userId: number;
  uid: number;
  contentType: string;
  timestamp: number;
  time: string;
  appName: string;
  want: WantAgent;
  actionButtons: notificationManager.ButtonOptions[];
  bundleName: string;
  smallIcon?: image.PixelMap | string;
  largeIcon?: image.PixelMap | string;
  picture?: image.PixelMap;
  title?: string;
  text?: string;
  additionalText?: string;
  briefText?: string;
  expandedTitle?: string;
  longText?: string;
  lines?: any[];
  longTitle?: string;
  slotLevel?: any;
  source?: number;
  versionName?: string;
  sound?: string;
  vibrationValues?: Array<number>;
  notificationFlags?: any;
  ruleData?: RuleData;
  template?: any;
  isOngoing?: boolean;
  isUnremovable?: boolean;
  isRemoveAllowed?: boolean;
  distributedOption?: any;
  deviceId?: string;
  groupName?: string;
  tapDismissed?: boolean;
}

export interface NotificationServiceListener {
  onNotificationConsume?: (data: NotificationItemData) => void;
  onNotificationLoad?: (data: NotificationItemData) => void;
  onNotificationCancel?: (data: NotificationItemData) => void;
}

export enum ScreenLockStatus {
  Locking = 1,
  Unlock = 2
}
