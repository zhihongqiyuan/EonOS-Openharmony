/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import account_osAccount from '@ohos.account.osAccount';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';
import common from '@ohos.app.ability.common';
import PassWord from '../../common/components/PassWord';
import SixPassword from '../../common/components/SixPassword';
import AuthUtils from '../../common/utils/AuthUtils';
import FuncUtils from '../../common/utils/FuncUtils';
import LogUtils from '../../common/utils/LogUtils';
import TimeUtils from '../../common/utils/TimeUtils';
import Constants, { CmdData, CmdType, WantParams } from '../../common/vm/Constants';

const TAG = 'PasswordAuth';
const INTERVAL = 1000;
const BUTTON_OPACITY = 1;
const TEXT_LENGTH = 4;
const MAX_FAIL_TIMES = 5;
const HINT_TIMES_BY_FAIL_LESS = 3;

let pinAuthManager: account_osAccount.PINAuth;
let pinData = '';

@Component
export default struct PasswordAuth {
  @Link pinSubType: string;
  @State @Watch('onTextValueChange') textValue: string = '';
  @Link @Watch('onCmdDataChange') cmdData: Array<CmdType>;
  @State isEdit: boolean = true;
  // tip
  @State inputValue: string = ' ';
  @StorageLink('SYSTEM_NAVIGATION_BAR_HEIGHT') SYSTEM_NAVIGATION_BAR_HEIGHT: number = 0;
  @StorageLink('IS_LANDSCAPE') IS_LANDSCAPE: boolean = false;

  onTextValueChange(): void {
    pinData = this.textValue;
  }

  onCmdDataChange(num?: string): void {
    this.cmdData.length > 0 && this.cmdData.map((item) => {
      const payload: CmdData = item.payload;
      if (payload.type === Constants.noticeTypePin) {
        if (payload.result && payload.result === Constants.authResultPinExpired) {
          this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_pwd_error').id);
          this.textValue = '';
          return;
        }
        if (payload.remainAttempts && payload.remainAttempts < MAX_FAIL_TIMES) {
          this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_pwd_error').id);
          this.textValue = '';

          // pin auth Remaining number
          if (payload.remainAttempts < HINT_TIMES_BY_FAIL_LESS) {
            this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_pwd_error_can_try').id) +
            payload.remainAttempts + (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_frequency').id);
          }
          if (num === 'first') {
            this.inputValue = ' ';
          }
          return;
        }
        // pin auth Remaining number
        if (payload.remainAttempts === 0 && payload.lockoutDuration) {
          this.countTime(payload.lockoutDuration);
          this.isEdit = false;
          this.textValue = '';
          return;
        }
        if (payload.result === 0) {
          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
        }
        this.inputValue = ' ';
        this.textValue = '';
      }
    })
  }

  aboutToAppear(): void {
    try {
      if (this.cmdData && this.cmdData.length > 0) {
        this.onCmdDataChange('first');
      }
      LogUtils.info(TAG, 'aboutToAppear PINAuth start');
      pinAuthManager = new account_osAccount.PINAuth();
      // register input
      pinAuthManager.registerInputer({
        onGetData: (authSubType, callback) => {
          LogUtils.info(TAG, 'aboutToAppear registerInputer onGetData');
          const uint8PW = FuncUtils.getUint8PW(pinData);
          callback.onSetData(authSubType, uint8PW);
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'aboutToAppear PINAuth catch error: ' + error?.code);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }

  aboutToDisappear(): void {
    LogUtils.info(TAG, 'aboutToAppear pinAuthManager unregisterInputer');
    pinAuthManager?.unregisterInputer?.();
  }

  countTime(freezingTime: number): void {
    const TRY_AGAIN = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager.getStringSync($r('app.string.unified_authwidget_postretry').id);
    let promptText: string = '';
    let freezingMillisecond = freezingTime;
    if (freezingMillisecond > 0) {
      promptText = TimeUtils.getFreezingTimeNm(freezingMillisecond, (AppStorage.get('context') as common.ExtensionContext));
      promptText = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
        .getStringSync($r('app.string.unified_authwidget_many_failures').id) + promptText + TRY_AGAIN;
      setTimeout((t: number):void => this.countTime(t), INTERVAL, freezingTime - INTERVAL);
    } else {
      promptText = ' ';
      this.isEdit = true;
    }
    this.inputValue = promptText;
  }

  handleCancel(): void {
    AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
      (AppStorage.get('wantParams') as WantParams)?.type as string[]);
    (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
  }

  build() {
    Column() {
      GridRow({
        columns: { xs: 4, sm: 4, md: 8, lg: 12 },
        gutter: { x: 24, y: 24 },
        breakpoints: { value: Constants.deviceDpi,
          reference: BreakpointsReference.WindowSize },
        direction: GridRowDirection.Row
      }) {
        GridCol({
          span: { xs: 4, sm: 4, md: 4, lg: 6 },
          offset: { md: 2, lg: 3 },
        }) {
          Column() {
            if (this.pinSubType !== Constants.pinSix) {
              // arbitrary input
              Column() {
                PassWord({
                  pinSubType: $pinSubType,
                  textValue: $textValue,
                  inputValue: $inputValue,
                  isEdit: $isEdit,
                })
                Row() {
                  Column() {
                    Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                      .margin({ left: $r('app.float.content_padding') })
                      .width('90%')
                      .height($r('app.float.btn_height'))
                      .fontSize($r('sys.float.ohos_id_text_size_button1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                      .fontWeight(FontWeight.Medium)
                      .backgroundColor(Color.Transparent)
                      .onClick(() => {
                        this.handleCancel();
                        this.textValue = ' ';
                        this.inputValue = ' ';
                      })
                      .id('cancelBtnDialogPwd')
                  }.width('50%')

                  Divider()
                    .vertical(true)
                    .height($r('app.float.btn_height'))
                    .color($r('sys.color.ohos_id_color_list_separator'))
                    .width($r('app.float.divider_width'))
                  Column() {
                    Button($r('app.string.unified_authwidget_confirm'))
                      .onClick(async () => {
                        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                      })
                      .enabled(this.textValue.length >= TEXT_LENGTH)
                      .margin({ right: $r('app.float.content_padding') })
                      .width('90%')
                      .height($r('app.float.btn_height'))
                      .fontSize($r('sys.float.ohos_id_text_size_button1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                      .opacity(this.textValue.length >= TEXT_LENGTH
                        ? BUTTON_OPACITY
                        : $r('sys.float.ohos_fa_alpha_disabled'))
                      .fontWeight(FontWeight.Medium)
                      .backgroundColor(Color.Transparent)
                      .id('okBtn')
                  }.width('50%')
                }
                .height($r('app.float.btn_height'))
                .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
              }
            } else {
              // six number
              Column() {
                SixPassword({
                  textValue: $textValue,
                  inputValue: $inputValue,
                  isEdit: $isEdit
                })
                Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                  .onClick(() => {
                    this.handleCancel();
                    this.textValue = '';
                    this.inputValue = ' ';
                  })
                  .backgroundColor(Color.Transparent)
                  .height($r('app.float.btn_height'))
                  .width('50%')
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                  .fontSize($r('sys.float.ohos_id_text_size_button1'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                  .fontWeight(FontWeight.Medium)
                  .id('cancelBtnDialogSixPwd')
              }
            }
          }
          .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
          .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
          .margin({
            left: ($r('sys.float.ohos_id_dialog_margin_start')),
            right: ($r('sys.float.ohos_id_dialog_margin_end')),
            bottom: this.IS_LANDSCAPE ? '0' : ($r('sys.float.ohos_id_dialog_margin_bottom'))
          })
        }
      }
    }
    .margin(this.IS_LANDSCAPE ? '0' : { bottom: this.SYSTEM_NAVIGATION_BAR_HEIGHT })
    .height('100%')
    .justifyContent(this.IS_LANDSCAPE ? FlexAlign.Center : FlexAlign.End)
    .backgroundColor(Color.Transparent)
  }
}
