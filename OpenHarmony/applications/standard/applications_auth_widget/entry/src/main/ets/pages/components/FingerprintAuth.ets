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
import display from '@ohos.display';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';
import PassWord from '../../common/components/PassWord';
import SixPassword from '../../common/components/SixPassword';
import { DialogType } from '../../common/module/DialogType';
import AuthUtils from '../../common/utils/AuthUtils';
import FuncUtils from '../../common/utils/FuncUtils';
import LogUtils from '../../common/utils/LogUtils';
import TimeUtils from '../../common/utils/TimeUtils';
import Constants, { CmdData, CmdType, FingerPosition, WantParams } from '../../common/vm/Constants';
import common from '@ohos.app.ability.common';

const TAG = 'FingerprintAuth';
const INTERVAL = 1000;
let pinAuthManager: account_osAccount.PINAuth;
let pinData = '';
const THOUSANDTH = 1000;
const BOTTOM_BUTTON = 56;
const BOTTOM_TEXT = 20;
const PADDING_8 = 8;
const PADDING_24 = 24;
const SINGLE_FINGER = 1;
const PIN_FINGER = 2;
const SIX_PIN = 6;
const MULTI_PIN = 5;
const RADIUS = 2;
const PIN_FAIL_TIP = 3;
const AUTH_LOCK = 0;
const NOTICE_DELAY = 50;
const ON_SCREEN = 1;
const UNDER_SCREEN = 2;
const FINGER_SENSOR_POSITION_LINE = 0.75;
const SECOND = 1000;

@Component
export default struct FingerprintAuth {
  @Link type: string;
  @Link pinSubType: string;
  @Link dialogType: DialogType;
  @State prompt: string = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
    .getStringSync($r('app.string.unified_authwidget_hint_inscreen_fp').id);
  @State @Watch('onTextValueChange') textValue: string = '';
  @Link @Watch('onCmdDataChange') cmdData: Array<CmdType>;
  @State isEdit: boolean = true;
  @State inputValue: string = '';
  @State state: number = this.dialogType === DialogType.PIN_FINGER ? PIN_FINGER : SINGLE_FINGER;
  @State fingerPositionY: number = 0;
  @State fingerPosition: FingerPosition = {
    sensorType: '',
    udSensorRadiusInPx: 60
  }
  @State fingerLock: boolean = false;
  @StorageLink('SYSTEM_NAVIGATION_BAR_HEIGHT') SYSTEM_NAVIGATION_BAR_HEIGHT: number = 0;
  @State screen: number[] = [];
  @StorageLink('IS_LANDSCAPE') IS_LANDSCAPE: boolean = false;
  @State isOffFinger: boolean = false;
  @State screenType: number = 0;

  aboutToAppear(): void {
    AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
    try {
      if (this.cmdData && this.cmdData.length > 0) {
        this.onCmdDataChange('first');
      }
      if (this.fingerPosition.udSensorCenterYInThousandth !== undefined && (this.screenType === ON_SCREEN ||
        this.screenType === UNDER_SCREEN)) {
        let tempPositionY = px2vp(this.fingerPosition.udSensorCenterYInThousandth * this.screen[1]);
        FuncUtils.judgmentOverflow(tempPositionY);
        if (this.screenType === ON_SCREEN) {
          this.fingerPositionY = px2vp(this.screen[1]) - tempPositionY / THOUSANDTH + BOTTOM_BUTTON +
          PADDING_8 + BOTTOM_TEXT + PADDING_24;
        } else if (this.screenType === UNDER_SCREEN) {
          this.fingerPositionY = px2vp(this.screen[1]) - tempPositionY / THOUSANDTH + PADDING_24;
        }
        FuncUtils.judgmentOverflow(this.fingerPositionY);
      }
      LogUtils.info(TAG, 'aboutToAppear this.fingerPositionY: ' + this.fingerPositionY);
      pinAuthManager = new account_osAccount.PINAuth();
      pinAuthManager.registerInputer({
        onGetData: (authSubType: account_osAccount.AuthSubType, callback: account_osAccount.IInputData) => {
          LogUtils.info(TAG, 'aboutToAppear registerInputer onGetData');
          let uint8PW = FuncUtils.getUint8PW(pinData);
          callback.onSetData(authSubType, uint8PW);
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'aboutToAppear PINAuth catch error: ' + error?.code);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }

  aboutToDisappear(): void {
    LogUtils.info(TAG, 'PINAuth unregisterInputer');
    pinAuthManager?.unregisterInputer?.();
  }

  onTextValueChange(): void {
    pinData = this.textValue;
  }

  onCmdDataChange(num?: string): void {
    this.cmdData.length > 0 && this.cmdData.map((item) => {
      const payload: CmdData = item.payload;
      if (payload.type === Constants.noticeTypePin) {
        if (payload.result === 0) {
          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
        } else if (payload.result && payload.result === Constants.authResultPinExpired) {
            this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_hint_pwd_error').id);
            this.textValue = '';
            return;
        } else {
          if (payload.remainAttempts) {
            this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_hint_pwd_error').id);
            this.textValue = '';
            if (num === 'first') {
              this.inputValue = '';
            }
            if (payload.remainAttempts < PIN_FAIL_TIP) {
              this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                .getStringSync($r('app.string.unified_authwidget_pwd_error_can_try').id) +
              payload.remainAttempts + (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                .getStringSync($r('app.string.unified_authwidget_frequency').id);
            }
          }
          if (payload.remainAttempts === AUTH_LOCK && payload.lockoutDuration) {
            this.countTime(payload.lockoutDuration);
            this.textValue = '';
            this.toPin();
            this.isEdit = false;
          }
        }
      } else if (payload.type === Constants.noticeTypeFinger) {
        if ([MULTI_PIN, SIX_PIN].includes(this.state)) {
          return;
        }
        const displayClass = display.getDefaultDisplaySync();
        this.screen = [displayClass.width, displayClass.height];
        if (payload.sensorInfo && JSON.stringify(payload.sensorInfo) !== '{}') {
          this.fingerPosition = JSON.parse(payload.sensorInfo);
          switch (this.fingerPosition.sensorType) {
            case 'OUT_OF_SCREEN_SENSOR': {
              this.isOffFinger = true;
              break;
            }
            default:
              let tempPositionLine = JSON.parse(payload.sensorInfo).udSensorCenterYInThousandth / displayClass.height;
              FuncUtils.judgmentOverflow(tempPositionLine);
              if (tempPositionLine < FINGER_SENSOR_POSITION_LINE) {
                this.screenType = ON_SCREEN;
              } else if (tempPositionLine > FINGER_SENSOR_POSITION_LINE) {
                this.screenType = UNDER_SCREEN;
              }
              break;
          }
        }
        if ((payload.remainAttempts && payload.result !== 0) || payload.result === Constants.authResultPinExpired) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_fp_retry_s2').id);
          this.fingerLock = false;
        }
        if (num === 'first') {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync(this.isOffFinger ?
            $r('app.string.unified_authwidget_hint_normal_fp_only').id :
            $r('app.string.unified_authwidget_hint_inscreen_fp').id);
        }
        if (payload.remainAttempts === AUTH_LOCK) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id);
          this.fingerLock = true;
          if (this.dialogType === DialogType.PIN_FINGER) {
            AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFinger]);
            if (this.pinSubType !== Constants.pinSix) {
              this.state = MULTI_PIN;
            } else {
              this.state = SIX_PIN;
            }
          }
        }
        if (payload.result === 0) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id)
          setTimeout(() => {
            (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
          }, SECOND);
        }
      } else {
        LogUtils.error(TAG, 'onCmdDataChange default');
        (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
      }
    })
  }

  sendFingerEvent(): void {
    if (!this.fingerLock) {
      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
    }
  }

  countTime(freezingTime: number): void {
    const TRY_AGAIN = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
      .getStringSync($r('app.string.unified_authwidget_postretry').id);
    let promptText: string = '';
    let freezingMillisecond = freezingTime;
    // O: freezing FINISH
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

  onFingerPrintFontColor(prompt: string, context: Context): Resource {
    if (prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_fp_retry_s2').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id)) {
      return $r('sys.color.ohos_id_color_warning');
    } else if (prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id) ||
    context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_inscreen_fp').id) ||
    context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_normal_fp_only').id)) {
      return $r('sys.color.ohos_id_color_text_secondary');
    } else {
      return $r('sys.color.ohos_id_color_text_secondary');
    }
  }

  getFingerPosition(type: string): Resource {
    switch (type) {
      case 'BACK':
        return $r('app.media.icon_applock_2');
      case 'FRONT':
        return $r('app.media.icon_applock_3');
      case 'SIDE':
        return $r('app.media.icon_applock_4');
      default:
        return $r('app.media.icon_applock_3');
    }
  }

  handleCancel(): void {
    if (this.state === SIX_PIN || this.state === MULTI_PIN) {
      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypePin]);
    } else {
      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
        (AppStorage.get('wantParams') as WantParams)?.type as string[]);
    }
    (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
  }

  toPin(): void {
    LogUtils.debug(TAG, 'toPin this.pinSubType: ' + this.pinSubType);
    AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFinger]);
    if (this.pinSubType !== Constants.pinSix) {
      this.state = MULTI_PIN;
    } else {
      this.state = SIX_PIN;
    }
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
          if (this.isOffFinger) {
            Column() {
              if ([PIN_FINGER, SINGLE_FINGER].includes(this.state) && this.fingerPosition !== undefined) {
                if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                  Scroll() {
                    Column() {
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                      Image(this.getFingerPosition(this.fingerPosition.outOfScreenSensorType as string))
                        .draggable(false)
                        .width($r('app.float.image_big'))
                        .height($r('app.float.image_big'))
                        .margin({ top: $r('app.float.content_padding'), bottom: $r('app.float.content_padding') })
                        .id('outFingerImage')
                      Text(this.prompt)
                        .draggable(false)
                        .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                        .fontSize($r('sys.float.ohos_id_text_size_body2'))
                    }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                  }
                  .width('100%')
                  .height($r('app.float.scroll_height_220'))
                  .margin({ top: $r('app.float.title_padding_top') })
                  .scrollable(ScrollDirection.Vertical)
                  .scrollBarColor(Color.Gray)
                } else {
                  Text((AppStorage.get('wantParams') as WantParams)?.title)
                    .draggable(false)
                    .margin({ top: $r('app.float.title_padding_top') })
                    .fontSize($r('sys.float.ohos_id_text_size_body1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary'))
                    .height($r('app.float.size_24'))
                    .fontWeight(FontWeight.Medium)
                  Image(this.getFingerPosition(this.fingerPosition.outOfScreenSensorType as string))
                    .draggable(false)
                    .width($r('app.float.image_big'))
                    .height($r('app.float.image_big'))
                    .margin({ top: $r('app.float.content_padding'), bottom: $r('app.float.content_padding') })
                    .id('outFingerImage')
                  Text(this.prompt)
                    .draggable(false)
                    .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    .fontSize($r('sys.float.ohos_id_text_size_body2'))
                }
              }

              if (this.state === PIN_FINGER) {
                Row() {
                  Column() {
                    Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                      .id('cancelBthState3FingerprintAuth')
                      .margin({ left: $r('app.float.content_padding') })
                      .width(Constants.ninetyPercentWidth)
                      .height($r('app.float.btn_height'))
                      .fontSize($r('sys.float.ohos_id_text_size_button1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                      .fontWeight(FontWeight.Medium)
                      .backgroundColor(Color.Transparent)
                      .onClick(() => {
                        this.handleCancel();
                        this.textValue = '';
                      })
                  }.width(Constants.halfContainerWidth)

                  Divider()
                    .vertical(true)
                    .height($r('app.float.digital_password_mask_height'))
                    .color($r('sys.color.ohos_id_color_list_separator'))
                    .width($r('app.float.divider_width'))
                  Column() {
                    Button($r('app.string.unified_authwidget_usepwd'))
                      .id('usePwdBtnState3FingerprintAuth')
                      .margin({ right: $r('app.float.content_padding') })
                      .width(Constants.ninetyPercentWidth)
                      .height($r('app.float.btn_height'))
                      .fontSize($r('sys.float.ohos_id_text_size_button1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                      .fontWeight(FontWeight.Medium)
                      .backgroundColor(Color.Transparent)
                      .onClick(() => {
                        this.inputValue = ' ';
                        this.toPin();
                      })
                  }.width(Constants.halfContainerWidth)
                }
                .height($r('app.float.btn_height'))
                .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
              } else if (this.state === SINGLE_FINGER) {
                if (!((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)) {
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelDefBtnState3FingerprintAuth')
                        .onClick(() => {
                          this.handleCancel();
                        })
                        .backgroundColor(Color.Transparent)
                        .height($r('app.float.btn_height'))
                        .width(Constants.halfContainerWidth)
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                    }.width(Constants.fullContainerHeight)
                  }
                  .height($r('app.float.btn_height'))
                  .padding({ left: $r('app.float.content_padding'), right: $r('app.float.content_padding') })
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                } else {
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelBtnState3FingerprintAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.handleCancel();
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)
                        .onClick(() => {
                          AuthUtils.getInstance()
                            .sendNotice('EVENT_AUTH_USER_NAVIGATION', [Constants.noticeTypeFinger]);
                          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                        })
                        .margin({ right: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                    }.width(Constants.halfContainerWidth)
                  }
                  .height($r('app.float.btn_height'))
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                }
              } else if (this.state === MULTI_PIN) {
                // Password 32-bit
                Column() {
                  PassWord({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit,
                    pinSubType: $pinSubType
                  })
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelStateMultiFingerprintAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.handleCancel();
                          this.textValue = '';
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button($r('app.string.unified_authwidget_confirm'))
                        .margin({ right: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(async (e) => {
                          AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                        })
                    }.width(Constants.halfContainerWidth)
                  }
                  .height($r('app.float.btn_height'))
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                }
              } else if (this.state === SIX_PIN) {
                // Password 6-bit
                Column() {
                  SixPassword({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit
                  })
                  Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                    .id('cancelState6FingerprintAuth')
                    .onClick(() => {
                      this.handleCancel();
                      this.textValue = '';
                    })
                    .backgroundColor(Color.Transparent)
                    .height($r('app.float.btn_height'))
                    .width(Constants.halfContainerWidth)
                    .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                    .fontSize($r('sys.float.ohos_id_text_size_button1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                    .fontWeight(FontWeight.Medium)
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
          } else {
            if (this.state === MULTI_PIN) {
              Column() {
                // Password 32-bit
                Column() {
                  PassWord({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit,
                    pinSubType: $pinSubType
                  })
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelBtnState5FingerprintAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.handleCancel();
                          this.textValue = '';
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button($r('app.string.unified_authwidget_confirm'))
                        .id('okBthState5FingerprintAuth')
                        .margin({ right: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(async (e) => {
                          AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                        })
                    }.width(Constants.halfContainerWidth)
                  }
                  .height($r('app.float.btn_height'))
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                }
              }
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: this.IS_LANDSCAPE ? '0' : ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            } else if (this.state === SIX_PIN) {
              Column() {
                // Password 6-bit
                Column() {
                  SixPassword({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit
                  })
                  Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                    .id('sixPassCancel')
                    .onClick(() => {
                      this.handleCancel();
                      this.textValue = '';
                    })
                    .backgroundColor(Color.Transparent)
                    .height($r('app.float.btn_height'))
                    .width(Constants.halfContainerWidth)
                    .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                    .fontSize($r('sys.float.ohos_id_text_size_button1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                    .fontWeight(FontWeight.Medium)
                }
              }
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: this.IS_LANDSCAPE ? '0' : ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            } else if (this.state === PIN_FINGER && this.fingerPosition.udSensorRadiusInPx !== undefined ) {
              Column() {
                if (this.screenType === ON_SCREEN) {
                  if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                    Scroll() {
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                        .margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                    }
                    .width('100%')
                    .height($r('app.float.textArea_height'))
                    .margin({ top: $r('app.float.title_padding_top') })
                    .scrollable(ScrollDirection.Vertical)
                    .scrollBarColor(Color.Gray)
                  } else {
                    Text((AppStorage.get('wantParams') as WantParams)?.title)
                      .draggable(false)
                      .margin({ top: $r('app.float.title_padding_top') })
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary'))
                      .height($r('app.float.size_24'))
                      .fontWeight(FontWeight.Medium)
                  }
                  Image(this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id)
                    ? $r('app.media.right_faceID') : $r('app.media.ic_blue_fingerprint'))
                    .draggable(false)
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .id('blueFingerprintImgState2FingerprintAuth')
                    .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFingerEvent();
                    })
                  Text(this.prompt)
                    .draggable(false)
                    .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelBtnState2FingerprintAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.handleCancel();
                          this.textValue = '';
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button($r('app.string.unified_authwidget_usepwd'))
                        .id('usePwdBtnState2FingerprintAuth')
                        .margin({ right: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.inputValue = ' ';
                          this.toPin();
                        })
                    }.width(Constants.halfContainerWidth)
                  }
                  .height($r('app.float.btn_height'))
                  .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                } else if (this.screenType === UNDER_SCREEN) {
                  Flex({ justifyContent: FlexAlign.SpaceBetween }) {
                    Image($r('app.media.ic_public_cancel'))
                      .draggable(false)
                      .id('cancelBtnState4FingerprintAuth')
                      .width($r('app.float.digital_password_mask_height'))
                      .height($r('app.float.digital_password_mask_height'))
                      .margin({
                        top: $r('app.float.digital_password_mask_height'),
                        left: $r('app.float.digital_password_mask_height')
                      })
                      .onClick(() => {
                        this.handleCancel();
                        this.textValue = '';
                      })
                    Button($r('app.string.unified_authwidget_usepwd'))
                      .backgroundColor(Color.White)
                      .height($r('app.float.digital_password_mask_height'))
                      .padding(0)
                      .margin({
                        top: $r('app.float.digital_password_mask_height'),
                        right: $r('app.float.digital_password_mask_height')
                      })
                      .fontColor($r('sys.color.ohos_id_color_activated'))
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontWeight(FontWeight.Medium)
                      .onClick(() => {
                        this.inputValue = ' ';
                        this.toPin();
                      })
                  }

                  if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                    Scroll() {
                      Column() {
                        Text((AppStorage.get('wantParams') as WantParams)?.title)
                          .draggable(false)
                          .fontSize($r('sys.float.ohos_id_text_size_body1'))
                          .fontColor($r('sys.color.ohos_id_color_text_primary'))
                          .fontWeight(FontWeight.Medium)
                        Text(this.prompt)
                          .draggable(false)
                          .margin({ top: $r('app.float.element_margin') })
                          .height($r('app.float.size_24'))
                          .fontSize($r('sys.float.ohos_id_text_size_body2'))
                          .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                      }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                    }
                    .width('100%')
                    .height($r('app.float.scroll_height_76'))
                    .scrollable(ScrollDirection.Vertical)
                    .scrollBarColor(Color.Gray)
                  } else {
                    Text((AppStorage.get('wantParams') as WantParams)?.title)
                      .draggable(false)
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary'))
                      .height($r('app.float.size_24'))
                      .fontWeight(FontWeight.Medium)
                    Text(this.prompt)
                      .draggable(false)
                      .margin({ top: $r('app.float.element_margin') })
                      .height($r('app.float.size_24'))
                      .fontSize($r('sys.float.ohos_id_text_size_body2'))
                      .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                  }
                  Image(this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id)
                    ? $r('app.media.right_faceID') : $r('app.media.ic_blue_fingerprint'))
                    .draggable(false)
                    .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFingerEvent();
                    })
                }
              }
              .position({ y: -Math.ceil(this.fingerPositionY) })
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            } else if (this.state === SINGLE_FINGER) {
              Column() {
                if (this.screenType === ON_SCREEN && this.fingerPosition.udSensorRadiusInPx !== undefined) {
                  if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                    Scroll() {
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                        .margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                    }
                    .width('100%')
                    .height($r('app.float.textArea_height'))
                    .margin({ top: $r('app.float.title_padding_top') })
                    .scrollable(ScrollDirection.Vertical)
                    .scrollBarColor(Color.Gray)
                  } else {
                    Text((AppStorage.get('wantParams') as WantParams)?.title)
                      .draggable(false)
                      .margin({ top: $r('app.float.title_padding_top') })
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary'))
                      .height($r('app.float.size_24'))
                      .fontWeight(FontWeight.Medium)
                  }
                  Image(this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id)
                    ? $r('app.media.right_faceID') : $r('app.media.ic_blue_fingerprint'))
                    .draggable(false)
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .id('blueFingerprintState1FingerprintAuth')
                    .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFingerEvent();
                    })
                  Text(this.prompt)
                    .draggable(false)
                    .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  if (!((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)) {
                    Row() {
                      Column() {
                        Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                          .id('cancelDefBtnState1FingerprintAuth')
                          .onClick(() => {
                            this.handleCancel();
                          })
                          .backgroundColor(Color.Transparent)
                          .height($r('app.float.btn_height'))
                          .width(Constants.halfContainerWidth)
                          .fontSize($r('sys.float.ohos_id_text_size_button1'))
                          .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                          .fontWeight(FontWeight.Medium)
                      }.width(Constants.fullContainerHeight)
                    }
                    .height($r('app.float.btn_height'))
                    .padding({ left: $r('app.float.content_padding'), right: $r('app.float.content_padding') })
                    .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                  } else {
                    Row() {
                      Column() {
                        Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                          .id('cancelBtnState1FingerprintAuth')
                          .margin({ left: $r('app.float.content_padding') })
                          .width(Constants.ninetyPercentWidth)
                          .height($r('app.float.btn_height'))
                          .fontSize($r('sys.float.ohos_id_text_size_button1'))
                          .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                          .fontWeight(FontWeight.Medium)
                          .backgroundColor(Color.Transparent)
                          .onClick(() => {
                            this.handleCancel();
                          })
                      }.width(Constants.halfContainerWidth)

                      Divider()
                        .vertical(true)
                        .height($r('app.float.digital_password_mask_height'))
                        .color($r('sys.color.ohos_id_color_list_separator'))
                        .width($r('app.float.divider_width'))
                      Column() {
                        Button(((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string))
                          .onClick(() => {
                            AuthUtils.getInstance()
                              .sendNotice('EVENT_AUTH_USER_NAVIGATION', [Constants.noticeTypeFinger]);
                            (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                          })
                          .id('navigationButtonTextFingerprintAuth')
                          .margin({ right: $r('app.float.content_padding') })
                          .width(Constants.ninetyPercentWidth)
                          .height($r('app.float.btn_height'))
                          .fontSize($r('sys.float.ohos_id_text_size_button1'))
                          .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                          .fontWeight(FontWeight.Medium)
                          .backgroundColor(Color.Transparent)
                      }.width(Constants.halfContainerWidth)
                    }
                    .height($r('app.float.btn_height'))
                    .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
                  }
                } else if (this.screenType === UNDER_SCREEN && this.fingerPosition.udSensorRadiusInPx !== undefined) {
                  Flex({ justifyContent: FlexAlign.SpaceBetween }) {
                    Image($r('app.media.ic_public_cancel'))
                      .draggable(false)
                      .id('fingerUnderImage')
                      .width($r('app.float.digital_password_mask_height'))
                      .height($r('app.float.digital_password_mask_height'))
                      .margin({
                        top: $r('app.float.digital_password_mask_height'),
                        left: $r('app.float.digital_password_mask_height')
                      })
                      .onClick(() => {
                        this.handleCancel();
                        this.textValue = '';
                      })
                    if ((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string) {
                      Button((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)
                        .id('fingerUnderButton')
                        .backgroundColor(Color.White)
                        .height($r('app.float.digital_password_mask_height'))
                        .padding(0)
                        .margin({
                          top: $r('app.float.digital_password_mask_height'),
                          right: $r('app.float.digital_password_mask_height')
                        })
                        .fontColor($r('sys.color.ohos_id_color_activated'))
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontWeight(FontWeight.Medium)
                        .onClick(() => {
                          AuthUtils.getInstance()
                            .sendNotice('EVENT_AUTH_USER_NAVIGATION', [Constants.noticeTypeFinger]);
                          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                        })
                    }
                  }

                  if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                    Scroll() {
                      Column() {
                        Text((AppStorage.get('wantParams') as WantParams)?.title)
                          .draggable(false)
                          .fontSize($r('sys.float.ohos_id_text_size_body1'))
                          .fontColor($r('sys.color.ohos_id_color_text_primary'))
                          .fontWeight(FontWeight.Medium)
                        Text(this.prompt)
                          .draggable(false)
                          .margin({ top: $r('app.float.element_margin') })
                          .height($r('app.float.size_24'))
                          .fontSize($r('sys.float.ohos_id_text_size_body2'))
                          .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                      }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                    }
                    .width('100%')
                    .height($r('app.float.scroll_height_76'))
                    .scrollable(ScrollDirection.Vertical)
                    .scrollBarColor(Color.Gray)
                  } else {
                    Text((AppStorage.get('wantParams') as WantParams)?.title)
                      .draggable(false)
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary'))
                      .height($r('app.float.size_24'))
                      .fontWeight(FontWeight.Medium)
                    Text(this.prompt)
                      .draggable(false)
                      .margin({ top: $r('app.float.element_margin') })
                      .height($r('app.float.size_24'))
                      .fontSize($r('sys.float.ohos_id_text_size_body2'))
                      .fontColor(this.onFingerPrintFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                  }
                  Image(this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_fp_verify_success').id)
                    ? $r('app.media.right_faceID') : $r('app.media.ic_blue_fingerprint'))
                    .draggable(false)
                    .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFingerEvent();
                    })
                }
              }
              .position({ y: -Math.ceil(this.fingerPositionY) })
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            }
          }
        }
      }
    }
    .margin(this.IS_LANDSCAPE ? '0' : { bottom: this.SYSTEM_NAVIGATION_BAR_HEIGHT })
    .height(Constants.fullContainerHeight)
    .justifyContent(this.IS_LANDSCAPE ? FlexAlign.Center : FlexAlign.End)
    .backgroundColor(Color.Transparent)
    .id('fingerprintAuth')
  }
}

