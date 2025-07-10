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

import Constants from '../../common/constants';
import Log from '../../../../../../../../../../common/src/main/ets/default/Log';
import deviceInfo from '@ohos.deviceInfo';

const deviceTypeInfo = deviceInfo.deviceType;

const TAG = 'NoticeItem-Confirm';

/**
 * confirm dialog
 */
@CustomDialog
export default struct ConfirmDialog {
  private title: string | Resource = ''
  private bundleName: string = ''
  public controller: CustomDialogController
  public action: () => void = () => {}

  build() {
    Column() {
      Column() {
        Row() {
          Text(this.title)
            .fontSize($r('app.float.confirm_title_fontsize'))
            .fontWeight(FontWeight.Bold)
        }.width($r('app.float.confirm_dialog_row_width'))
        .margin({ top: $r('app.float.title_font_margin_t') })

        Row() {
          Text($r('app.string.confirm_message', this.bundleName))
            .fontSize($r('app.float.confirm_cont_fontsize'))
        }.width($r('app.float.confirm_dialog_row_width'))
        .margin({ top: $r('app.float.message_font_margin_t') })

        Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
          Column() {
            Text($r('app.string.cancel'))
              .fontSize($r('app.float.confirm_cont_fontsize'))
              .fontColor($r("app.color.button_text_color"))
          }.onClick(() => {
            this.controller.close();
          })
          .alignItems(HorizontalAlign.Center)
          .width($r('app.float.confirm_dialog_button_width'))

          Divider()
            .vertical(true)
            .color($r('app.color.confirm_divider_color'))
            .strokeWidth(1)
            .height($r('app.float.confirm_divider_height'))
          Column() {
            Text($r('app.string.close'))
              .fontSize($r('app.float.confirm_cont_fontsize'))
              .fontColor($r("app.color.button_text_color"))
          }.onClick(() => {
            Log.showInfo(TAG, `confirm button of TimeDialog on click`)
            this.controller.close();
            this.action();
          })
          .alignItems(HorizontalAlign.Center)
          .width($r('app.float.confirm_dialog_button_width'))
        }
        .width($r('app.float.confirm_dialog_width'))
        .margin({ top: $r('app.float.confirm_button_margin_t') })
      }
      .height($r('app.float.confirm_dialog_height'))
      .width(deviceTypeInfo === 'phone' ? '100%' : $r('app.float.confirm_dialog_width'))
      .backgroundColor('#FFFFFF')
      .border({ width: $r('app.float.setting_border_width'), color: Color.White,
        radius: $r('app.float.notification_border_radius') })
    }
    .margin({
      left: $r("app.float.confirm_dialog_margin"),
      right: $r("app.float.confirm_dialog_margin")
    })

  }
}
