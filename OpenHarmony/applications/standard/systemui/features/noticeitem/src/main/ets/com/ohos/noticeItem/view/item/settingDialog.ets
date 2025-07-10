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

import Constants, { NotificationItemData } from '../../common/constants';
import Log from '../../../../../../../../../../common/src/main/ets/default/Log';
import CheckEmptyUtils from '../../../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';
import EventManager from "../../../../../../../../../../common/src/main/ets/default/event/EventManager"
import { obtainStartAbility } from "../../../../../../../../../../common/src/main/ets/default/event/EventUtil"
import Notification from '@ohos.notification';
import deviceInfo from '@ohos.deviceInfo';

const deviceTypeInfo = deviceInfo.deviceType;

const TAG = 'NoticeItem-Setting';

/**
 * Setting dialog
 */
@CustomDialog
export default struct SettingDialog {
  @Prop itemData: NotificationItemData
  public controller: CustomDialogController
  public action: () => void = () => {}

  build() {
    Column() {
      Column({ space: 4 }) {
        Row({ space: 4 }) {
          if (!CheckEmptyUtils.isEmpty(this.itemData.smallIcon)) {
            Image(this.itemData.smallIcon)
              .objectFit(ImageFit.Contain)
              .width($r('app.float.title_image_width'))
              .height($r('app.float.title_image_height'))
          }
          Text(this.itemData.appName)
            .fontSize($r('app.float.setting_title_fontsize'))
            .fontWeight(500)
        }.width($r('app.float.setting_dialog_row_width'))
        .margin({ top: $r('app.float.notification_appname_margin_top') })
        .height($r('app.float.title_font_height'))

        Row() {
          Column() {
            Text($r('app.string.closeNotification'))
              .fontSize($r('app.float.setting_cont_fontsize'))
              .fontColor($r('app.color.dialog_font_color'))
              .height($r('app.float.setting_dialog_row_height'))
          }.alignItems(HorizontalAlign.Center)
          .width(Constants.SETTING_CONTENT_WITH)
        }
        .width($r('app.float.setting_dialog_row_width'))
        .alignItems(VerticalAlign.Center)
        .align(Alignment.Center)
        .margin({ top: $r('app.float.close_notification_margin_top') })
        .border({ width: $r('app.float.setting_border_width'), color: Color.White,
          radius: $r('app.float.setting_border_radius') })
        .backgroundColor($r('app.color.dialog_font_back_color'))
        .onClick(this.closeNotification.bind(this))

        Row() {
          Column() {
            Text($r('app.string.moreSettings'))
              .fontSize($r('app.float.setting_cont_fontsize'))
              .fontColor($r('app.color.button_text_color'))
              .height($r('app.float.setting_dialog_row_height'))
          }.alignItems(HorizontalAlign.Center)
          .width(Constants.SETTING_CONTENT_WITH)
          .align(Alignment.Center)
        }
        .width($r('app.float.setting_dialog_row_width'))
        .alignItems(VerticalAlign.Center)
        .border({ width: $r('app.float.setting_border_width'), color: Color.White,
          radius: $r('app.float.setting_border_radius') })
        .backgroundColor($r('app.color.action_button_click_color'))
        .onClick(this.openAbility.bind(this))

        Row() {
          Column() {
            Text($r('app.string.cancel'))
              .maxLines(Constants.DEFAULT_MAX_LINES)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontSize($r('app.float.setting_cont_fontsize'))
              .fontColor($r('app.color.button_text_color'))
          }.alignItems(HorizontalAlign.Center)
          .width(Constants.SETTING_CONTENT_WITH)
        }
        .width($r('app.float.setting_dialog_row_width'))
        .margin({ top: $r('app.float.notification_cancle_margin_top') })
        .alignItems(VerticalAlign.Center)
        .align(Alignment.Center)
        .border({ width: $r('app.float.setting_border_width'), color: Color.White,
          radius: $r('app.float.setting_border_radius') })
        .backgroundColor($r('app.color.dialog_font_color'))
        .onClick(this.closeAbility.bind(this))
      }
      .height($r('app.float.setting_dialog_height'))
      .width(deviceTypeInfo === 'phone' ? '100%' : $r('app.float.setting_dialog_width'))
      .backgroundColor('#FFFFFF')
      .margin({ left: 20, right: 20 })
      .border({ width: $r('app.float.setting_border_width'), color: Color.White,
        radius: $r('app.float.notification_border_radius') })
    }
    .margin({
      left: $r("app.float.confirm_dialog_margin"),
      right: $r("app.float.confirm_dialog_margin")
    })
  }

  closeNotification() {
    this.action()
    this.closeAbility()
  }

  openAbility() {
    Log.showInfo(TAG, ` openAbility:showNotificationManagement`);
    EventManager.publish(obtainStartAbility('com.ohos.systemui', 'com.ohos.systemui.notificationmanagement.MainAbility',
      { 'migrateUri': 'pages/setEnable', 'migrateBundle': this.itemData.bundleName }))
    this.closeAbility()
  }

  closeAbility() {
    Log.showInfo(TAG, `closeAbility`);
    this.controller.close()
  }
}