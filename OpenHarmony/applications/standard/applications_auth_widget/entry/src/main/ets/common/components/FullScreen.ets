/*
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

import display from '@ohos.display';
import userAuth from '@ohos.userIAM.userAuth';
import Constants, { CmdData, CmdType, FingerPosition, NumKeyBoardItem, WantParams } from '../../common/vm/Constants';
import AuthUtils from '../utils/AuthUtils';
import FuncUtils from '../utils/FuncUtils';
import LogUtils from '../utils/LogUtils';
import TimeUtils from '../utils/TimeUtils';
import NumKeyBoard from './NumkeyBoard';
import common from '@ohos.app.ability.common';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';

const TAG = 'FullScreen';
const INTERVAL = 1000;
const THOUSANDTH = 1000;
const DEL_PWD = -2;
const CALL_PHONE = -1;
const GO_BACK = -3;
const FINGER_TEXT_POSITION = 120;
const FINGER_HIGH = 80;
const FINGER_Y_POSITION = 60;
const MAX_LENGTH = 32;
const SIX_MAX_LENGTH = 6;
const AUTH_TYPE_FACE = 2;
const AUTH_TYPE_FINGER = 4;
const PASSWORD_LENGTH = 4;
const PASSWORD_ARRAY_LENGTH = 6;
const NOTICE_DELAY = 50;
const FINGER_SENSOR_POSITION_LINE = 0.75;
const RADIUS = 2;
const PADDING_8 = 8;
const CANCEL_HIGH = 40;
const BOTTOM = 72;
const MARGIN = 4;
const NUM_KEY_BOTTOM = 0.63;
const NUM_KEY_POSITION = 0.54;
const TIPS_POSITION = 0.245;
const LOCK_POSITION = 0.2675;

interface ControlType {
  isSixPassword?: boolean,
  isLandscape: boolean,
  jumpFinger: boolean,
  isShowFace: boolean,
  isShowFinger: boolean,
}

@Component
export default struct FullScreen {
  @Link pinSubType: string;
  @Link textValue: string;
  @Link authType: Array<userAuth.UserAuthType>;
  @Link @Watch('onCmdDataChange') cmdData: Array<CmdType>;
  @Link cancelImage: boolean;
  @State pinLock: number = 0;
  @State fingerPositionY: number = 0;
  @State fingerTextPositionY: number = 0;
  @State fingerTipsPositionY: number = 0;
  @State fingerButtonPositionY: number = 0;
  @State fingerPositionLine: number = 0;
  @StorageLink('passwordArray') passwordArray: string[] = [];
  @StorageLink('passwordArrayNumber') passwordArrayNumber: string[] = [];
  @State passwordObj: string = '';
  numKeyboard: NumKeyBoardItem[] = Constants.numKeyBoard;
  @State prompt: string = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
    .getStringSync($r('app.string.unified_authwidget_use_pwd').id);
  @State fingerText: string = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
    .getStringSync($r('app.string.unified_authwidget_hint_inscreen_fp').id);
  @StorageLink('screenLockDirection') screenLockDirection: number = 1;
  @StorageLink('SYSTEM_STATUS_BAR_HEIGHT') SYSTEM_STATUS_BAR_HEIGHT: number = 0;
  @StorageLink('SYSTEM_NAVIGATION_BAR_HEIGHT') SYSTEM_NAVIGATION_BAR_HEIGHT: number = 0;
  @State controlType: ControlType = {
    jumpFinger: false,
    isLandscape: false,
    isSixPassword: true,
    isShowFace: this.authType.includes(AUTH_TYPE_FACE),
    isShowFinger: this.authType.includes(AUTH_TYPE_FINGER)
  }
  @State fingerPosition: FingerPosition = {
    sensorType: ''
  }
  @State screen: number[] = [];
  @State faceFingerLockArr: boolean[] = [false, false];
  @StorageLink('IS_LANDSCAPE') IS_LANDSCAPE: boolean = false;
  @Consume underFingerPrint: boolean;

  onCmdDataChange(num?: string): void {
    this.cmdData.length > 0 && this.cmdData.map((item) => {
      const payload: CmdData = item.payload;
      if (payload.type === Constants.noticeTypePin) {
        this.clearPassword();
        if (payload.remainAttempts) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_pwd_error').id);
          if (num === 'first' && this.prompt !== (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_recognition').id)) {
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_use_pwd').id);
          }
          // 3: pin Residual number
          if (payload.remainAttempts < 3 && num !== 'first') {
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_pwd_error_can_try').id) +
              payload.remainAttempts + (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_frequency').id);
          }
        }
        if (payload.remainAttempts === 0 && payload.lockoutDuration) {
          // 1: pin lock
          this.pinLock = 1;
          this.countdown(payload.lockoutDuration);
        }

        if (!payload.remainAttempts && !payload.lockoutDuration) {
          // 1: pin lock
          this.pinLock = 1;
        }
        if (payload.result === 0) {
          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
        }
      } else if (payload.type === Constants.noticeTypeFace) {
        if (payload.remainAttempts < 5 && payload.remainAttempts > 0) {
          if (this.pinLock !== 1) {
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id);
            this.faceFingerLockArr[0] = false;
          }
        }
        if (num === 'first') {
          if (payload.remainAttempts === 0) {
            this.controlType.isShowFace = false;
          } else {
            if (this.pinLock !== 1) {
              this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
            }
          }
        }
        if (payload.remainAttempts === 0) {
          if (num !== 'first') {
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_title_number_failed_face_forbidden').id);
          }
          this.faceFingerLockArr[0] = true;
        }
        if (payload.result === 0) {
          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
        }
      } else if (payload.type === Constants.noticeTypeFinger) {
        let sensor: FingerPosition = { sensorType: '' };
        if (payload.sensorInfo && JSON.stringify(payload.sensorInfo) !== '{}') {
          sensor = JSON.parse(payload.sensorInfo);
          this.fingerPosition = sensor || { sensorType: '' };
        }
        if (payload.remainAttempts && payload.result !== 0) {
          if (this.controlType.jumpFinger) {
            this.fingerText = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_hint_fp_retry_s2').id);
          }
          if (sensor && this.fingerPosition.udSensorCenterYInThousandth !== undefined &&
            this.fingerPosition.udSensorRadiusInPx !== undefined) {
            if (sensor.sensorType === 'UNDER_SCREEN_SENSOR' ||
              sensor.sensorType === 'BOTH_SENSOR' ||
              sensor.sensorType === 'SensorType1') {
              if (num !== 'first' && payload.remainAttempts > 0 && payload.result != 0) {
                setTimeout(() => {
                  AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
                }, NOTICE_DELAY);
                this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                  .getStringSync($r('app.string.unified_authwidget_hint_fp_retry_s2').id);
              }

              this.fingerButtonPositionY =
                px2vp(this.fingerPosition.udSensorCenterYInThousandth / THOUSANDTH * this.screen[1]);
              FuncUtils.judgmentOverflow(this.fingerButtonPositionY);
              this.fingerPositionY = this.fingerButtonPositionY - this.SYSTEM_STATUS_BAR_HEIGHT -
                this.SYSTEM_STATUS_BAR_HEIGHT - px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) - PADDING_8;
              this.fingerPositionLine = this.fingerPositionY / px2vp(this.screen[1]);
              FuncUtils.judgmentOverflow(this.fingerPositionLine);
              if (num === 'first' && this.fingerPositionLine > FINGER_SENSOR_POSITION_LINE && payload.result != 0) {
                AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
              }
            }
          }
        }
        if (payload.remainAttempts === 0) {
          if (num === 'first') {
            this.controlType.isShowFinger = false;
          } else {
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id);
            this.fingerText = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id);
          }
          this.faceFingerLockArr[1] = true;
          this.controlType.jumpFinger = false;
          this.cancelImage = false;
        }
        if (payload.result === 0) {
          (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
        }
      } else {
        LogUtils.error(TAG, 'type: ' + payload.type);
        (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
      }
    })
  }

  aboutToAppear(): void {
    LogUtils.debug(TAG, 'aboutToAppear');
    try {
      const displayClass = display.getDefaultDisplaySync();
      this.screen = [displayClass.width, displayClass.height];
      AppStorage.SetOrCreate('passwordArray', [] as string[]);
      if (this.cmdData && this.cmdData.length > 0) {
        this.onCmdDataChange('first');
      }

      if (this.controlType.isShowFinger && this.fingerPosition.udSensorCenterYInThousandth !== undefined &&
        this.fingerPosition.udSensorRadiusInPx !== undefined) {
        let tempPosition = px2vp(this.fingerPosition.udSensorCenterYInThousandth * this.screen[1]);
        FuncUtils.judgmentOverflow(tempPosition);
        this.fingerButtonPositionY = tempPosition / THOUSANDTH;
        FuncUtils.judgmentOverflow(this.fingerButtonPositionY);
        this.fingerPositionY = this.fingerButtonPositionY - this.SYSTEM_STATUS_BAR_HEIGHT -
          this.SYSTEM_STATUS_BAR_HEIGHT - px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) - PADDING_8;
        FuncUtils.judgmentOverflow(this.fingerPositionY);

        this.fingerTipsPositionY = this.fingerButtonPositionY - px2vp(FINGER_Y_POSITION) - FINGER_HIGH -
          this.SYSTEM_STATUS_BAR_HEIGHT - px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) - PADDING_8;
        FuncUtils.judgmentOverflow(this.fingerTipsPositionY);

        this.fingerTextPositionY = this.fingerButtonPositionY - px2vp(FINGER_Y_POSITION) - FINGER_TEXT_POSITION -
          this.SYSTEM_STATUS_BAR_HEIGHT - px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) - PADDING_8;
        FuncUtils.judgmentOverflow(this.fingerTextPositionY);
      }

      if (this.controlType.isShowFace && this.pinLock !== 1) {
        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFace]);
        return;
      }
    } catch (error) {
      LogUtils.error(TAG, 'aboutToAppear catch error: ' + error?.code);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }

  clearPassword(): void {
    this.passwordArray = [];
    this.passwordArrayNumber = [];
    this.passwordArray = ['', '', '', '', '', ''];
    this.passwordObj = '';
    this.numKeyboard[11].value = GO_BACK;
    this.numKeyboard[11].row1 = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
      .getStringSync($r('app.string.unified_authwidget_back').id);
    this.updateStorage(() => {
    })
  }

  aboutToDisappear(): void {
    this.clearPassword();
  }

  countdown(freezingTime: number): void {
    const TRY_AGAIN = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager.getStringSync($r('app.string.unified_authwidget_postretry').id);
    const PLEASE_TRY = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager.getStringSync($r('app.string.unified_authwidget_pleaseretry').id);
    let promptText: string = '';
    let freezingMillisecond = freezingTime;
    if (freezingMillisecond > 0) {
      promptText = TimeUtils.getFreezingTimeNm(freezingMillisecond, (AppStorage.get('context') as common.ExtensionContext));
      promptText = PLEASE_TRY + promptText + TRY_AGAIN;
      setTimeout((t: number):void => this.countdown(t), INTERVAL, freezingTime - INTERVAL);
    } else {
      promptText = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager.getStringSync($r('app.string.unified_authwidget_use_pwd').id);
      this.clearPassword();
      // 0: pin unlock
      this.pinLock = 0;
    }
    this.prompt = promptText;
  }

  updateStorage(callback: Function): void {
    AppStorage.SetOrCreate('passwordArray', this.passwordArray);
    AppStorage.SetOrCreate('numKeyboard', this.numKeyboard);
    callback();
  }

  build() {
    if (!this.controlType.jumpFinger) {
      Column() {

        // 1: pin lock
        if (this.pinLock === 1) {
          Column() {
            Text($r('app.string.unified_authwidget_locked'))
              .draggable(false)
              .margin({ bottom: $r('app.float.content_padding') })
              .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
              .fontSize($r('sys.float.ohos_id_text_size_headline6'))
              .fontWeight(FontWeight.Medium)
            Text(this.prompt)
              .draggable(false)
              .id('cancelIconCustomPassword')
              .fontSize($r('sys.float.ohos_id_text_size_body2'))
              .fontWeight(FontWeight.Medium)
              .fontColor($r('sys.color.ohos_id_color_text_secondary_contrary'))
              .margin({ bottom: $r('app.float.margin_16') })
              .textAlign(TextAlign.Center)
              .textOverflow({ overflow: TextOverflow.None })
              .height($r('app.float.size_24'))
            Button(($r('app.string.unified_authwidget_forgotpwd')), {
              stateEffect: false
            })
              .id('forgotBtnCustomPwd')
              .height($r('app.float.text_high'))
              .backgroundColor(Color.Transparent)
              .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
              .fontSize($r('sys.float.ohos_id_text_size_button1'))
              .fontWeight(FontWeight.Medium)
              .onClick(() => {
                let str = this.passwordArrayNumber.toString()
                this.textValue = str.replace(new RegExp(',', 'g'), '');
              })
              .visibility(Visibility.Hidden)
          }
          .position({ y: px2vp(this.screen[1] * LOCK_POSITION) - CANCEL_HIGH - this.SYSTEM_STATUS_BAR_HEIGHT})
          .width(Constants.fullContainerWidth)
        } else {
          Column() {
            if (this.controlType.isShowFace) {
              Image($r('app.media.white_faceID'))
                .draggable(false)
                .id('faceImgCustomPwd')
                .width($r('app.float.image_small'))
                .height($r('app.float.image_small'))
                .margin({ bottom: $r('app.float.content_padding') })
                .onClick(() => {
                  if (!this.faceFingerLockArr[0]) {
                    this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                      .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
                    AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFace]);
                  } else {
                    this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                      .getStringSync($r('app.string.unified_authwidget_title_face_forbidden').id);
                  }
                })
            }
            if (!this.controlType.isShowFace) {
              Text()
                .draggable(false)
                .width($r('app.float.image_small'))
                .height($r('app.float.image_small'))
                .margin({ bottom: $r('app.float.content_padding') })
            }
            Text(this.prompt)
              .draggable(false)
              .id('cancelIconCustomPassword')
              .fontSize($r('sys.float.ohos_id_text_size_body1'))
              .fontWeight(FontWeight.Medium)
              .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
              .margin({ bottom: $r('app.float.margin_16') })
              .textAlign(TextAlign.Center)
              .textOverflow({ overflow: TextOverflow.None })
              .height($r('app.float.size_24'))
              .onClick(() => {
                if (this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                  .getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id)) {
                  this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
                  AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFace]);
                }
              })

            Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
              // 0: pin unlock
              if (this.pinLock === 0) {
                if (this.pinSubType === Constants.pinSix) {
                  Stack() {
                    List() {
                      ForEach(this.passwordArray, (item: string) => {
                        if ((!item || item === '') && item !== '0') {
                          ListItem() {
                            Button()
                              .border({
                                color: Color.White,
                                style: BorderStyle.Solid,
                                width: 1
                              })
                              .borderRadius($r('app.float.input_btn_size'))
                              .width($r('app.float.input_btn_size'))
                              .height($r('app.float.input_btn_size'))
                              .type(ButtonType.Circle)
                              .backgroundColor(Color.Transparent)
                          }
                          .margin({
                            left: $r('app.float.margin_12'),
                            right: $r('app.float.margin_12'),
                            top: $r('app.float.margin_14')
                          })
                        } else {
                          ListItem() {
                            Button()
                              .borderRadius($r('app.float.input_btn_size'))
                              .width($r('app.float.input_btn_size'))
                              .height($r('app.float.input_btn_size'))
                              .type(ButtonType.Circle)
                              .backgroundColor($r('sys.color.ohos_id_color_text_primary_contrary'))
                          }
                          .margin({
                            left: $r('app.float.margin_12'),
                            right: $r('app.float.margin_12'),
                            top: $r('app.float.margin_14')
                          })
                        }
                      })
                    }
                    .listDirection(Axis.Horizontal)
                    .height($r('app.float.input_height'))

                    if (this.IS_LANDSCAPE) {
                      TextInput({ placeholder: '', text: this.passwordArray?.join('') })
                        .draggable(false)
                        .onChange(async (value: string) => {
                          const arr = value.replace(new RegExp('[^\\d]', 'g'), '').split('');
                          arr?.map((item, index) => {
                            this.passwordArray[index] = item;
                          });
                          if (arr?.length === SIX_MAX_LENGTH) {
                            this.textValue = arr?.join('');
                            AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                          }
                        })
                        .maxLength(SIX_MAX_LENGTH)
                        .visibility(Visibility.Visible)
                        .opacity(0)
                        .caretColor('transparent')
                    }
                  }

                } else if (this.pinSubType === Constants.pinNumber) {
                  Stack() {
                    TextInput({ placeholder: '', text: this.passwordArrayNumber.join('') })
                      .draggable(false)
                      .onChange(async (value: string) => {
                        // Removes non-numeric characters from a string
                        this.passwordArrayNumber = value.replace(new RegExp('[^\\d]', 'g'), '').split('');
                      })
                      .id('pinInputNumber')
                      .onSubmit(async (enterKey: EnterKeyType) => {
                        let str = this.passwordArrayNumber.toString();
                        this.textValue = str.replace(new RegExp(',', 'g'), '');
                        if (this.passwordArrayNumber.length < PASSWORD_LENGTH) {
                          return;
                        }
                        const strData = this.passwordArrayNumber.toString();
                        this.textValue = strData.replace(new RegExp(',', 'g'), '');
                        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                      })
                      .fontColor(Color.White)
                      .height($r('app.float.btn_height'))
                      .width($r('app.float.fullScreen_input_width'))
                      .type(InputType.Password)
                      .copyOption(CopyOptions.None)
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontWeight(FontWeight.Regular)
                      .maxLength(MAX_LENGTH)
                      .backgroundColor('rgba(255,255,255,0.2)')
                      .textAlign(TextAlign.Start)
                      .margin({
                        left: $r('app.float.custom_password_input_margin'),
                        right: $r('app.float.custom_password_input_margin')
                      })
                  }
                } else if (this.pinSubType === Constants.pinMixed) {
                  TextInput({ text: this.passwordObj })
                    .draggable(false)
                    .fontColor(Color.White)
                    .height($r('app.float.btn_height'))
                    .width($r('app.float.fullScreen_input_width'))
                    .type(InputType.Password)
                    .copyOption(CopyOptions.None)
                    .fontSize($r('sys.float.ohos_id_text_size_body1'))
                    .fontWeight(FontWeight.Regular)
                    .maxLength(MAX_LENGTH)
                    .backgroundColor('rgba(255,255,255,0.2)')
                    .textAlign(TextAlign.Start)
                    .margin({
                      left: $r('app.float.custom_password_input_margin'),
                      right: $r('app.float.custom_password_input_margin')
                    })
                    .onSubmit(async (enterKey: EnterKeyType) => {
                      // check callback
                      if (this.passwordObj.length < PASSWORD_LENGTH) {
                        return;
                      }
                      this.textValue = this.passwordObj;
                      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                    })
                    .onChange((value: string) => {
                      this.passwordObj = value;
                    })
                    .id('pinInput')
                }
              }
            }
            .height($r('app.float.text_high'))
            Button(($r('app.string.unified_authwidget_forgotpwd')), {
              stateEffect: false
            })
              .id('forgotBtnCustomPwd')
              .height($r('app.float.text_high'))
              .backgroundColor(Color.Transparent)
              .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
              .fontSize($r('sys.float.ohos_id_text_size_button1'))
              .fontWeight(FontWeight.Medium)
              .onClick(() => {
                let str = this.passwordArrayNumber.toString()
                this.textValue = str.replace(new RegExp(',', 'g'), '');
              })
              .visibility(Visibility.Hidden)
          }
          .width(Constants.fullContainerWidth)
          .position({ y: px2vp(this.screen[1] * TIPS_POSITION) - CANCEL_HIGH - this.SYSTEM_STATUS_BAR_HEIGHT })
        }

        // 0: pin unlock
        if (!this.IS_LANDSCAPE && this.pinLock === 0 && this.pinSubType === Constants.pinSix) {
          if (this.controlType.isShowFinger && this.fingerPosition.sensorType !== 'OUT_OF_SCREEN_SENSOR' &&
            this.fingerPosition.sensorType !== 'NON_SENSOR') {
            if (this.fingerPositionLine > FINGER_SENSOR_POSITION_LINE &&
              this.fingerPosition.udSensorRadiusInPx !== undefined &&
              this.fingerPosition.udSensorCenterYInThousandth !== undefined) {
              Column() {
                NumKeyBoard({
                  onKeyPress: (index, callback) => {
                    let keyValue = this.numKeyboard[index].value;
                    if (keyValue != undefined && keyValue >= 0) {
                      const index = this.passwordArray.map(item => item).indexOf('')
                      if (index > -1) {
                        this.passwordArray[index] = keyValue + '';
                        this.numKeyboard[11].row1 = $r('app.string.unified_authwidget_delete');
                        this.numKeyboard[11].value = DEL_PWD;
                        if (index === 5) {
                          this.passwordArray[index] = keyValue + '';
                          if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                            return;
                          }
                          let str = this.passwordArray.toString();
                          this.textValue = str.replace(new RegExp(',', 'g'), '');
                          AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                        }
                        this.updateStorage(callback);
                      }
                    } else if (keyValue === DEL_PWD) {
                      const index = this.passwordArray.map(item => item).indexOf('');
                      if (index === -1) {
                        this.passwordArray[5] = '';
                      } else if (index === 1) {
                        this.passwordArray[index - 1] = '';
                        this.numKeyboard[11].value = GO_BACK;
                        this.numKeyboard[11].row1 = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                          .getStringSync($r('app.string.unified_authwidget_back').id);
                      } else {
                        this.passwordArray[index - 1] = '';
                      }
                      this.updateStorage(callback);
                    } else if (keyValue === GO_BACK) {
                      // 0: pin unlock
                      this.pinLock = 0;
                      this.clearPassword();
                      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
                      (AppStorage.get('wantParams') as WantParams)?.type as string[]);
                    } else if (keyValue === CALL_PHONE) {
                      if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                        return;
                      }

                      let str = this.passwordArray.toString();
                      this.textValue = str.replace(new RegExp(',', 'g'), '');
                      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                    }
                  }
                })
              }
              .width(Constants.fullContainerWidth)
              .height('37%')
              .position({ y: px2vp(this.screen[1] * NUM_KEY_BOTTOM) -
                CANCEL_HIGH - MARGIN - px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) -
                px2vp((THOUSANDTH - this.fingerPosition.udSensorCenterYInThousandth) / THOUSANDTH * this.screen[1]) -
                this.SYSTEM_STATUS_BAR_HEIGHT })
            } else {
              Column() {
                NumKeyBoard({
                  onKeyPress: (index, callback) => {
                    let keyValue = this.numKeyboard[index].value;
                    if (keyValue !== undefined && keyValue >= 0) {
                      const index = this.passwordArray.map(item => item).indexOf('')
                      if (index > -1) {
                        this.passwordArray[index] = keyValue + '';
                        this.numKeyboard[11].row1 = $r('app.string.unified_authwidget_delete');
                        this.numKeyboard[11].value = DEL_PWD;
                        if (index === 5) {
                          this.passwordArray[index] = keyValue + '';
                          if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                            return;
                          }
                          let str = this.passwordArray.toString();
                          this.textValue = str.replace(new RegExp(',', 'g'), '');
                          AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                        }
                        this.updateStorage(callback);
                      }
                    } else if (keyValue === DEL_PWD) {
                      const index = this.passwordArray.map(item => item).indexOf('');
                      if (index === -1) {
                        this.passwordArray[5] = '';
                      } else if (index === 1) {
                        this.passwordArray[index - 1] = '';
                        this.numKeyboard[11].value = GO_BACK;
                        this.numKeyboard[11].row1 = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                          .getStringSync($r('app.string.unified_authwidget_back').id);
                      } else {
                        this.passwordArray[index - 1] = '';
                      }
                      this.updateStorage(callback);
                    } else if (keyValue === GO_BACK) {
                      // 0: pin unlock
                      this.pinLock = 0;
                      this.clearPassword();
                      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
                        (AppStorage.get('wantParams') as WantParams)?.type as string[]);
                    } else if (keyValue === CALL_PHONE) {
                      if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                        return;
                      }
                      let str = this.passwordArray.toString();
                      this.textValue = str.replace(new RegExp(',', 'g'), '');
                      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                    }
                  }
                })
              }
              .width(Constants.fullContainerWidth)
              .height('37%')
              .position({ y: px2vp(this.screen[1] * NUM_KEY_BOTTOM) - CANCEL_HIGH - BOTTOM -
                this.SYSTEM_STATUS_BAR_HEIGHT - this.SYSTEM_NAVIGATION_BAR_HEIGHT })
            }
          } else {
            Column() {
              NumKeyBoard({
                onKeyPress: (index, callback) => {
                  let keyValue = this.numKeyboard[index].value;
                  if (keyValue !== undefined && keyValue >= 0) {
                    const index = this.passwordArray.map(item => item).indexOf('')
                    if (index > -1) {
                      this.passwordArray[index] = keyValue + '';
                      this.numKeyboard[11].row1 = $r('app.string.unified_authwidget_delete');
                      this.numKeyboard[11].value = DEL_PWD;
                      if (index === 5) {
                        this.passwordArray[index] = keyValue + '';
                        if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                          return;
                        }
                        let str = this.passwordArray.toString();
                        this.textValue = str.replace(new RegExp(',', 'g'), '');
                        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                      }
                      this.updateStorage(callback);
                    }
                  } else if (keyValue === DEL_PWD) {
                    const index = this.passwordArray.map(item => item).indexOf('');
                    if (index === -1) {
                      this.passwordArray[5] = '';
                    } else if (index === 1) {
                      this.passwordArray[index - 1] = '';
                      this.numKeyboard[11].value = GO_BACK;
                      this.numKeyboard[11].row1 = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                        .getStringSync($r('app.string.unified_authwidget_back').id);
                    } else {
                      this.passwordArray[index - 1] = '';
                    }
                    this.updateStorage(callback);
                  } else if (keyValue === GO_BACK) {
                    // 0: pin unlock
                    this.pinLock = 0;
                    this.clearPassword();
                    (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                    AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
                      (AppStorage.get('wantParams') as WantParams)?.type as string[]);
                  } else if (keyValue === CALL_PHONE) {
                    if (this.passwordArray.join('').length < PASSWORD_ARRAY_LENGTH) {
                      return;
                    }

                    let str = this.passwordArray.toString();
                    this.textValue = str.replace(new RegExp(',', 'g'), '');
                    AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypePin]);
                  }
                }
              })
            }
            .width(Constants.fullContainerWidth)
            .height('37%')
            .position({ y: px2vp(this.screen[1] * NUM_KEY_POSITION) - CANCEL_HIGH -
              this.SYSTEM_STATUS_BAR_HEIGHT })
          }
        }

        // 1: pin lock
        if (this.controlType.isShowFinger && this.pinLock !== 1 && !this.faceFingerLockArr[1]) {
          if (this.fingerPosition.sensorType !== 'OUT_OF_SCREEN_SENSOR' &&
          this.fingerPosition.sensorType !== 'NON_SENSOR') {
            if (this.fingerPosition.sensorType === 'UNDER_SCREEN_SENSOR' ||
            this.fingerPosition.sensorType === 'BOTH_SENSOR' ||
            this.fingerPosition.sensorType === 'SensorType1') {
              if (this.fingerPositionLine > FINGER_SENSOR_POSITION_LINE && !this.IS_LANDSCAPE &&
                this.fingerPosition.udSensorRadiusInPx !== undefined &&
                this.fingerPosition.udSensorCenterYInThousandth !== undefined) {
                Column() {
                  Image($r('app.media.ic_unlock_fingerprint'))
                    .draggable(false)
                    .id('fingerprintImgCustomPassword')
                    .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
                    .onClick(() => {
                      if (!this.faceFingerLockArr[1]) {
                        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
                      }
                    })
                }
                .position({ y: px2vp(this.fingerPosition.udSensorCenterYInThousandth / THOUSANDTH * this.screen[1]) -
                  px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS) -
                  CANCEL_HIGH - this.SYSTEM_STATUS_BAR_HEIGHT })
                .width(Constants.fullContainerWidth)
                .height(this.controlType.isShowFinger ? $r('app.float.finger_high') : '9%')
                .justifyContent(FlexAlign.Start)
                .alignItems(HorizontalAlign.Center)
              } else {
                Column() {
                  Image($r('app.media.finger_white'))
                    .draggable(false)
                    .id('fingerWhiteImgCustomPassword')
                    .width($r('app.float.image_back_size'))
                    .height($r('app.float.image_back_size'))
                    .margin({ top: $r('app.float.input_btn_size') })
                    .onClick(() => {
                      if (this.IS_LANDSCAPE) {
                        this.underFingerPrint = true;
                      } else {
                        this.controlType.jumpFinger = true;
                        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
                        this.cancelImage = true;
                      }
                    })
                }
                .width(Constants.fullContainerWidth)
                .height(this.controlType.isShowFinger ? $r('app.float.finger_high') : '9%')
                .justifyContent(FlexAlign.Start)
                .alignItems(HorizontalAlign.Center)
                .position({ y: px2vp(this.screen[1]) - CANCEL_HIGH - BOTTOM -
                  this.SYSTEM_STATUS_BAR_HEIGHT - this.SYSTEM_NAVIGATION_BAR_HEIGHT })
              }
            }
          }
        }
      }
      .alignItems(HorizontalAlign.Center)
      .height(Constants.fullContainerHeight)
      .width(Constants.fullContainerWidth)
    } else if (!this.controlType.isLandscape && this.fingerPosition.udSensorRadiusInPx !== undefined) {
      Column() {
        if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
          Scroll() {
            Column() {
              Text((AppStorage.get('wantParams') as WantParams)?.title)
              .draggable(false)
              .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
              .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
              .fontWeight(FontWeight.Medium)
              Text(this.fingerText)
              .draggable(false)
              .height($r('app.float.text_high'))
              .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
              .fontSize($r('sys.float.ohos_id_text_size_body2'))
              .fontWeight(FontWeight.Regular)
              .textAlign(TextAlign.Center)
              .margin({ top: $r('app.float.padding_8') })
            }.margin({left: $r('app.float.margin_36'), right: $r('app.float.size_24')})
          }
          .width('96%')
          .id('titleFullScreen')
          .position({
            y: this.fingerTextPositionY,
          })
          .height($r('app.float.scroll_height_72'))
          .scrollable(ScrollDirection.Vertical)
          .scrollBarColor('sys.color.ohos_id_color_foreground')
        } else {
          Text((AppStorage.get('wantParams') as WantParams)?.title)
            .draggable(false)
            .height($r('app.float.text_high'))
            .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
            .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
            .fontWeight(FontWeight.Medium)
            .id('titleFullScreen')
            .position({
              y: this.fingerTextPositionY
            })
            .width('100%')
            .textAlign(TextAlign.Center)
          Text(this.fingerText)
            .draggable(false)
            .height($r('app.float.text_high'))
            .fontColor($r('sys.color.ohos_id_color_text_secondary_contrary'))
            .fontSize($r('sys.float.ohos_id_text_size_body2'))
            .fontWeight(FontWeight.Regular)
            .position({
              y: this.fingerTipsPositionY
            })
            .width('100%')
            .textAlign(TextAlign.Center)
            .margin({ top: $r('app.float.padding_8') })
        }

        Column() {
          Image($r('app.media.ic_unlock_fingerprint'))
            .draggable(false)
            .id('unlockFingerprintImgCustomPwd')
              // radius
            .width(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
            .height(px2vp(this.fingerPosition.udSensorRadiusInPx * RADIUS))
        }.width('100%')
        .position({ y: this.fingerPositionY })
        .justifyContent(FlexAlign.Center)
        .alignItems(HorizontalAlign.Center)

        Button($r('app.string.unified_authwidget_cancel'), {
          type: ButtonType.Normal,
          stateEffect: true
        })
          .width('100%')
          .position({
            y: this.fingerButtonPositionY
          })
          .backgroundColor(Color.Transparent)
          .fontColor($r('sys.color.ohos_id_color_text_primary_contrary'))
          .onClick(() => {
            this.controlType.jumpFinger = false;
            this.cancelImage = false;
            AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFinger]);
            this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
              .getStringSync($r('app.string.unified_authwidget_use_pwd').id);
          })
      }
      .justifyContent(FlexAlign.End)
      .alignItems(HorizontalAlign.Center)
      .height('100%')
      .width('100%')
    }
  }
}