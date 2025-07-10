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

const TAG = 'FaceAuth';

let pinAuthManager: account_osAccount.PINAuth;
const INTERVAL = 1000;
const THOUSANDTH = 1000;
const PADDING_24 = 24;
const MARGIN_TOP = 113;
const PIN_FACE = 1;
const PIN_FACE_FINGER = 2;
const FACE_FINGER_NAVIGATION = 3;
const MULTI_PIN = 4;
const SIX_PIN = 5;
const SINGLE_FACE = 6;
const SCREEN_LAND = 1;
const SCREEN_PORTRAIT = 2;
const NOTICE_DELAY = 50;
const FINGER_SENSOR_POSITION_LINE = 0.75;
const HINT_TIMES_BY_FAIL_LESS = 3;
const SECOND = 1000;
let pinData = '';

@Component
export default struct FaceAuth {
  @State prompt: string = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
    .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
  @State @Watch('onTextValueChange') textValue: string = '';
  @Link @Watch('onCmdDataChange') cmdData: Array<CmdType>;
  @StorageLink('IS_LANDSCAPE') IS_LANDSCAPE: boolean = false;
  @State isEdit: boolean = true;
  // 0:six number 1:arbitrary character 2:arbitrary number
  @Link type: string;
  @Link pinSubType: string;
  @Link dialogType: DialogType;
  @State inputValue: string = '';
  @State dialogTypeFlag: number = this.initDialogTypeFlag();
  // 1:Portrait screen 2:Landscape mode
  @State screenType: number = 0;
  @State fingerPositionY: number = 0;
  @State fingerPosition: FingerPosition = {
    sensorType: ''
  }
  @State screen: number[] = [];
  @State faceLock: boolean = false;
  @State fingerLock: boolean = false;
  @State isPinFaceFinger: boolean = false;
  @StorageLink('SYSTEM_NAVIGATION_BAR_HEIGHT') SYSTEM_NAVIGATION_BAR_HEIGHT: number = 0;

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

  initDialogTypeFlag(): number {
    if (this.dialogType === DialogType.PIN_FACE) {
      return PIN_FACE;
    } else if (this.dialogType === DialogType.ALL) {
      return PIN_FACE_FINGER;
    } else if (this.dialogType === DialogType.FACE_FINGER_NAVIGATION) {
      return FACE_FINGER_NAVIGATION;
    } else {
      return SINGLE_FACE;
    }
  }

  aboutToAppear(): void {
    LogUtils.debug(TAG, 'aboutToAppear');
    try {
      if (this.dialogType === DialogType.PIN_FACE || this.dialogType === DialogType.FACE) {
        this.prompt =
          (AppStorage.get('context') as common.ExtensionContext)?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_recognition')
            .id);
        AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFace]);
      } else if (this.dialogType === DialogType.ALL || this.dialogType === DialogType.FACE_FINGER_NAVIGATION) {
        AuthUtils.getInstance()
          .sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger, Constants.noticeTypeFace]);
      } else {
        LogUtils.error(TAG, 'aboutToAppear not support type');
        AuthUtils.getInstance().sendNotice(Constants.noticeEventInvalidParam, [Constants.noticeTypePin]);
        (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
      }

      if (this.cmdData && this.cmdData.length > 0) {
        this.onCmdDataChange('first');
      }

      if ((this.isPinFaceFinger || this.dialogType === DialogType.FACE_FINGER_NAVIGATION) &&
        this.fingerPosition.udSensorCenterYInThousandth !== undefined &&
        this.fingerPosition.udSensorRadiusInPx !== undefined) {
        let tempPosition = this.fingerPosition.udSensorCenterYInThousandth / this.screen[1];
        FuncUtils.judgmentOverflow(tempPosition);
        let tempFingerPositionY = px2vp(this.screen[1]) * this.fingerPosition.udSensorCenterYInThousandth;
        FuncUtils.judgmentOverflow(tempFingerPositionY);
        if (tempPosition < FINGER_SENSOR_POSITION_LINE) {
          this.fingerPositionY = (tempFingerPositionY / THOUSANDTH) - px2vp(this.screen[1]) +
          this.SYSTEM_NAVIGATION_BAR_HEIGHT - MARGIN_TOP - px2vp(this.fingerPosition.udSensorRadiusInPx);
        } else {
          this.fingerPositionY = (tempFingerPositionY / THOUSANDTH) - px2vp(this.screen[1]) +
          this.SYSTEM_NAVIGATION_BAR_HEIGHT - PADDING_24 - px2vp(this.fingerPosition.udSensorRadiusInPx);
        }
        FuncUtils.judgmentOverflow(this.fingerPositionY);
      }
      LogUtils.info(TAG, 'aboutToAppear this.fingerPositionY: ' + this.fingerPositionY);
      pinAuthManager = new account_osAccount.PINAuth();
      pinAuthManager.registerInputer({
        onGetData: (authSubType, callback) => {
          LogUtils.info(TAG, 'aboutToAppear registerInputer onGetData');
          let uint8PW = FuncUtils.getUint8PW(pinData);
          callback.onSetData(authSubType, uint8PW);
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'aboutToAppear catch error: ' + error?.code);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }

  aboutToDisappear(): void {
    pinAuthManager?.unregisterInputer?.();
  }

  toPin(): void {
    if ([DialogType.PIN_FACE, DialogType.ALL].includes(this.dialogType)) {
      LogUtils.debug(TAG, 'toPin this.dialogType: ' + this.dialogType);
      if (this.dialogType === DialogType.ALL) {
        AuthUtils.getInstance()
          .sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFace, Constants.noticeTypeFinger]);
      } else if (this.dialogType === DialogType.PIN_FACE) {
        AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFace]);
      } else {
        AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypeFinger]);
      }
      if (this.pinSubType !== Constants.pinSix) {
        this.dialogTypeFlag = MULTI_PIN;
      } else {
        this.dialogTypeFlag = SIX_PIN;
      }
    }
  }

  sendFaceEvent(): void {
    if (!this.faceLock) {
      this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
        .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFace]);
    }
  }

  sendFingerEvent(): void {
    if (!this.fingerLock) {
      AuthUtils.getInstance().sendNotice('EVENT_AUTH_TYPE_READY', [Constants.noticeTypeFinger]);
    }
  }

  onTextValueChange(): void {
    pinData = this.textValue;
  }

  onFontColor(prompt: string, context: Context): Resource {
    if (prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_fp_retry_s2').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_title_number_failed_face_forbidden').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id)) {
      return $r('sys.color.ohos_id_color_warning');
    } else if (prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_recognition').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id) ||
      prompt === context?.resourceManager?.getStringSync($r('app.string.unified_authwidget_hint_inscreen_fp').id)) {
      return $r('sys.color.ohos_id_color_text_secondary');
    } else {
      return $r('sys.color.ohos_id_color_text_secondary');
    }
  }

  onCmdDataChange(num?: string): void {
    this.cmdData.length > 0 && this.cmdData.map(async (item) => {
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
            if (payload.remainAttempts < HINT_TIMES_BY_FAIL_LESS) {
              this.inputValue = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                .getStringSync($r('app.string.unified_authwidget_pwd_error_can_try').id) +
                payload.remainAttempts + (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                .getStringSync($r('app.string.unified_authwidget_frequency').id);
            }
          }
          if (payload.remainAttempts === 0 && payload.lockoutDuration) {
            this.countTime(payload.lockoutDuration);
            this.textValue = '';
            this.toPin();
            this.isEdit = false;
          }
        }
      } else if (payload.type === Constants.noticeTypeFace) {
        if ([SIX_PIN, MULTI_PIN].includes(this.dialogTypeFlag)) {
          return;
        }
        if (payload.result && payload.result === Constants.authResultPinExpired) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id);
          this.faceLock = false;
          return;
        }
        if (payload.remainAttempts && payload.result !== 0) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id);
          this.faceLock = false;
        }
        if (num === 'first') {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_recognition').id);
        }
        if (payload.remainAttempts === 0) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_title_number_failed_face_forbidden').id);
          this.faceLock = true;
          if (this.dialogType === DialogType.ALL) {
            if (this.fingerLock) {
              this.toPin();
            }
          } else {
            this.toPin();
          }
        }
        if (payload.result === 0) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id)
          setTimeout(() => {
            (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
          }, SECOND);
        }
      } else if (payload.type === Constants.noticeTypeFinger) {
        if ([SIX_PIN, MULTI_PIN].includes(this.dialogTypeFlag)) {
          return;
        }
        if (payload.sensorInfo && JSON.stringify(payload.sensorInfo) !== '{}') {
          this.fingerPosition = JSON.parse(payload.sensorInfo);
          const displayClass = display.getDefaultDisplaySync();
          this.screen = [displayClass.width, displayClass.height];
          switch (JSON.parse(payload.sensorInfo)?.sensorType as string) {
            case 'NON_SENSOR':
            case 'OUT_OF_SCREEN_SENSOR': {
              this.dialogTypeFlag = PIN_FACE;
              break;
            }
            default:
              if (this.dialogType !== DialogType.FACE_FINGER_NAVIGATION) {
                this.isPinFaceFinger = true;
              }
              let tempPositionLine = JSON.parse(payload.sensorInfo).udSensorCenterYInThousandth / displayClass.height;
              FuncUtils.judgmentOverflow(tempPositionLine);
              if (tempPositionLine < FINGER_SENSOR_POSITION_LINE) {
                if (!this.IS_LANDSCAPE && ![SIX_PIN, MULTI_PIN].includes(this.dialogTypeFlag)) {
                  this.screenType = SCREEN_LAND;
                }
              } else if (tempPositionLine > FINGER_SENSOR_POSITION_LINE) {
                if (!this.IS_LANDSCAPE && ![SIX_PIN, MULTI_PIN].includes(this.dialogTypeFlag)) {
                  this.screenType = SCREEN_PORTRAIT;
                }
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
            .getStringSync($r('app.string.unified_authwidget_hint_normal_fp_only').id);
        }
        if (payload.remainAttempts === 0) {
          this.prompt = (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
            .getStringSync($r('app.string.unified_authwidget_title_number_failed_fp_forbidden').id);
          this.fingerLock = true;
          if (this.dialogType === DialogType.ALL) {
            if (this.faceLock) {
              this.toPin();
            }
          } else {
            this.toPin();
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

  handleCancel(): void {
    if (this.dialogTypeFlag === SIX_PIN || this.dialogTypeFlag === MULTI_PIN) {
      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [Constants.noticeTypePin]);
    } else {
      AuthUtils.getInstance().sendNotice(Constants.noticeEventCancel, [] ||
        (AppStorage.get('wantParams') as WantParams)?.type as string[]);
    }
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
          if (this.isPinFaceFinger) {
            if (this.dialogTypeFlag === MULTI_PIN) {
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
                        .id('cancelBtnMixedPinFaceAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.textValue = '';
                          this.handleCancel();
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button($r('app.string.unified_authwidget_confirm'))
                        .id('okBtnFaceAuth')
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
            } else if (this.dialogTypeFlag === SIX_PIN) {
              Column() {
                // Password 6-bit
                Column() {
                  SixPassword({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit
                  })
                  Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                    .id('cancelBtnSixPinFaceAuth')
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
            } else if (this.dialogTypeFlag === PIN_FACE_FINGER && this.screenType === SCREEN_PORTRAIT &&
            this.fingerPosition.udSensorRadiusInPx !== undefined) {
              Column() {
                // The first form of facial fingerprint password
                Flex({ justifyContent: FlexAlign.SpaceBetween }) {
                  Image($r('app.media.ic_public_cancel'))
                    .draggable(false)
                    .id('cancelImgAllFaceAuth')
                    .width($r('app.float.digital_password_mask_height'))
                    .height($r('app.float.digital_password_mask_height'))
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      left: $r('app.float.digital_password_mask_height')
                    })
                    .onClick(() => {
                      this.textValue = '';
                      this.handleCancel();
                    })
                  Button($r('app.string.unified_authwidget_usepwd'))
                    .id('usePwdBtnAllFaceAuth')
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
                      Image($r('app.media.error_faceID'))
                        .draggable(false)
                        .id('imgAllFaceAuth')
                        .width($r('app.float.image_small'))
                        .height($r('app.float.image_small'))
                        .margin({ bottom: $r('app.float.content_padding') })
                        .colorBlend($r('sys.color.ohos_id_color_activated'))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                      Text(this.prompt)
                        .draggable(false)
                        .id('resetPromptTextAllFaceAuth')
                        .margin({ top: $r('app.float.element_margin') })
                        .height($r('app.float.size_24'))
                        .fontSize($r('sys.float.ohos_id_text_size_body2'))
                        .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                  }
                  .width('100%')
                  .height($r('app.float.scroll_height_122'))
                  .margin({top: $r('app.float.content_padding')})
                  .scrollable(ScrollDirection.Vertical)
                  .scrollBarColor(Color.Gray)
                } else {
                  Image($r('app.media.error_faceID'))
                    .draggable(false)
                    .id('imgAllFaceAuth')
                    .width($r('app.float.image_small'))
                    .height($r('app.float.image_small'))
                    .margin({ top: $r('app.float.content_padding'), bottom: $r('app.float.content_padding') })
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                  Text((AppStorage.get('wantParams') as WantParams)?.title)
                    .draggable(false)
                    .fontSize($r('sys.float.ohos_id_text_size_body1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary'))
                    .height($r('app.float.size_24'))
                    .fontWeight(FontWeight.Medium)
                  Text(this.prompt)
                    .draggable(false)
                    .id('resetPromptTextAllFaceAuth')
                    .margin({ top: $r('app.float.element_margin') })
                    .height($r('app.float.size_24'))
                    .fontSize($r('sys.float.ohos_id_text_size_body2'))
                    .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                }
                Image($r('app.media.ic_blue_fingerprint'))
                  .draggable(false)
                  .id('blueFingerprintImgAllFaceAuth')
                    // radius
                  .width(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                  .height(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                  .margin({
                    top: $r('app.float.digital_password_mask_height'),
                    bottom: $r('app.float.digital_password_mask_height')
                  })
                  .colorBlend($r('sys.color.ohos_id_color_activated'))
                  .onClick(() => {
                    this.sendFingerEvent();
                  })
              }
              .position({ y: this.fingerPositionY })
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            } else if (this.dialogTypeFlag === PIN_FACE_FINGER && this.screenType === SCREEN_LAND &&
                this.fingerPosition.udSensorRadiusInPx !== undefined) {
              Column() {
                if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                  Scroll() {
                    Column() {
                      // Facial fingerprint password, second form
                      Image($r('app.media.error_faceID'))
                        .draggable(false)
                        .id('errFaceImgFaceAuth')
                        .width($r('app.float.image_small'))
                        .height($r('app.float.image_small'))
                        .margin({bottom: $r('app.float.content_padding') })
                        .colorBlend($r('sys.color.ohos_id_color_activated'))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                    }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                  }
                  .width('100%')
                  .height($r('app.float.scroll_height_90'))
                  .margin({ top: $r('app.float.content_padding_top')})
                  .scrollable(ScrollDirection.Vertical)
                  .scrollBarColor(Color.Gray)
                } else {
                  // Facial fingerprint password, second form
                  Image($r('app.media.error_faceID'))
                    .draggable(false)
                    .id('errFaceImgFaceAuth')
                    .width($r('app.float.image_small'))
                    .height($r('app.float.image_small'))
                    .margin({ top: $r('app.float.content_padding_top'), bottom: $r('app.float.content_padding') })
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                  Text((AppStorage.get('wantParams') as WantParams)?.title)
                    .draggable(false)
                    .fontSize($r('sys.float.ohos_id_text_size_body1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary'))
                    .height($r('app.float.size_24'))
                    .fontWeight(FontWeight.Medium)
                }
                Image($r('app.media.ic_blue_fingerprint'))
                  .draggable(false)
                  .id('blueFingerprintImgFaceAuth')
                    // radius
                  .width(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                  .height(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                  .margin({ top: $r('app.float.content_padding') })
                  .colorBlend($r('sys.color.ohos_id_color_activated'))
                  .onClick(() => {
                    this.sendFingerEvent();
                  })
                Text(this.prompt)
                  .draggable(false)
                  .margin({ top: $r('app.float.content_padding') })
                  .fontSize($r('sys.float.ohos_id_text_size_body2'))
                  .height($r('app.float.size_24'))
                  .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                  .onClick(() => {
                    if (this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    ?.getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id)) {
                      this.sendFaceEvent();
                    }
                  })
                Row() {
                  Column() {
                    Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                      .id('cancelBtnFaceAuth')
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
                      .id('usePwdBtnFaceAuth')
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
              }
              .position({ y: this.fingerPositionY })
              .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
              .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
              .margin({
                left: ($r('sys.float.ohos_id_dialog_margin_start')),
                right: ($r('sys.float.ohos_id_dialog_margin_end')),
                bottom: ($r('sys.float.ohos_id_dialog_margin_bottom'))
              })
            }
          } else if (this.dialogType === DialogType.FACE_FINGER_NAVIGATION &&
            (AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string) {
            Column() {
              if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                Scroll() {
                  Column() {
                    // Facial fingerprint password, second form
                    Image($r('app.media.error_faceID'))
                      .draggable(false)
                      .id('errFaceImgFaceAuth')
                      .width($r('app.float.image_small'))
                      .height($r('app.float.image_small'))
                      .margin({bottom: $r('app.float.content_padding') })
                      .colorBlend($r('sys.color.ohos_id_color_activated'))
                      .onClick(() => {
                        this.sendFaceEvent();
                      })
                    Text((AppStorage.get('wantParams') as WantParams)?.title)
                      .draggable(false)
                      .fontSize($r('sys.float.ohos_id_text_size_body1'))
                      .fontColor($r('sys.color.ohos_id_color_text_primary'))
                      .fontWeight(FontWeight.Medium)
                  }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                }
                .width('100%')
                .height($r('app.float.scroll_height_90'))
                .margin({ top: $r('app.float.content_padding_top')})
                .scrollable(ScrollDirection.Vertical)
                .scrollBarColor(Color.Gray)
              } else {
                // Facial fingerprint password, second form
                Image($r('app.media.error_faceID'))
                  .draggable(false)
                  .id('errFaceImgFaceAuth')
                  .width($r('app.float.image_small'))
                  .height($r('app.float.image_small'))
                  .margin({ top: $r('app.float.content_padding_top'), bottom: $r('app.float.content_padding') })
                  .colorBlend($r('sys.color.ohos_id_color_activated'))
                  .onClick(() => {
                    this.sendFaceEvent();
                  })
                Text((AppStorage.get('wantParams') as WantParams)?.title)
                  .draggable(false)
                  .fontSize($r('sys.float.ohos_id_text_size_body1'))
                  .fontColor($r('sys.color.ohos_id_color_text_primary'))
                  .height($r('app.float.size_24'))
                  .fontWeight(FontWeight.Medium)
              }
              Image($r('app.media.ic_blue_fingerprint'))
                .draggable(false)
                .id('blueFingerprintImgFaceAuth')
                  // radius
                .width(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                .height(px2vp(this.fingerPosition.udSensorRadiusInPx * 2))
                .margin({ top: $r('app.float.content_padding') })
                .colorBlend($r('sys.color.ohos_id_color_activated'))
                .onClick(() => {
                  this.sendFingerEvent();
                })
              Text(this.prompt)
                .draggable(false)
                .margin({ top: $r('app.float.content_padding') })
                .fontSize($r('sys.float.ohos_id_text_size_body2'))
                .height($r('app.float.size_24'))
                .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                .onClick(() => {
                  if (this.prompt === (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                  ?.getStringSync($r('app.string.unified_authwidget_hint_face_verify_fail_click_retry_s1').id)) {
                    this.sendFaceEvent();
                  }
                })
              Row() {
                Column() {
                  Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                    .id('cancelBtnFaceAuth')
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
                  Button((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)
                    .id('usePwdBtnFaceAuth')
                    .margin({ right: $r('app.float.content_padding') })
                    .width(Constants.ninetyPercentWidth)
                    .height($r('app.float.btn_height'))
                    .fontSize($r('sys.float.ohos_id_text_size_button1'))
                    .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                    .fontWeight(FontWeight.Medium)
                    .backgroundColor(Color.Transparent)
                    .onClick(() => {
                      AuthUtils.getInstance()
                        .sendNotice('EVENT_AUTH_USER_NAVIGATION',
                          [Constants.noticeTypeFace, Constants.noticeTypeFinger]);
                      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
                    })

                }.width(Constants.halfContainerWidth)

              }
              .height($r('app.float.btn_height'))
              .margin({ top: $r('app.float.element_margin'), bottom: $r('app.float.content_padding') })
            }
            .position({ y: this.fingerPositionY })
            .backgroundColor($r('sys.color.ohos_id_color_dialog_bg'))
            .borderRadius($r('sys.float.ohos_id_corner_radius_dialog'))
            .margin({
              left: ($r('sys.float.ohos_id_dialog_margin_start')),
              right: ($r('sys.float.ohos_id_dialog_margin_end')),
              bottom: ($r('sys.float.ohos_id_dialog_margin_bottom'))
            })
          } else {
            Column() {
              if (this.dialogTypeFlag === PIN_FACE) {
                // Pin FACE
                if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                  Scroll() {
                    Column() {
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                      Image(this.prompt !== (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                        .getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id)
                        ? $r('app.media.error_faceID') : $r('app.media.right_faceID'))
                        .draggable(false)
                        .margin({
                          top: $r('app.float.digital_password_mask_height'),
                          bottom: $r('app.float.digital_password_mask_height')
                        })
                        .width($r('app.float.image_size'))
                        .height($r('app.float.image_size'))
                        .colorBlend($r('sys.color.ohos_id_color_activated'))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                      Text(this.prompt).fontSize($r('sys.float.ohos_id_text_size_body2'))
                        .draggable(false)
                        .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                        .height($r('app.float.size_24'))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                    }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                  }
                  .width('100%')
                  .height($r('app.float.scroll_height_182'))
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
                  Image(this.prompt !== (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id)
                    ? $r('app.media.error_faceID') : $r('app.media.right_faceID'))
                    .draggable(false)
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .width($r('app.float.image_size'))
                    .height($r('app.float.image_size'))
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                  Text(this.prompt).fontSize($r('sys.float.ohos_id_text_size_body2'))
                    .draggable(false)
                    .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    .height($r('app.float.size_24'))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                }
                Row() {
                  Column() {
                    Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                      .id('cancelBtnPinFaceAuth')
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
                    Button($r('app.string.unified_authwidget_usepwd'))
                      .id('usePwdPinFaceAuth')
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
              }
              if (this.dialogTypeFlag === MULTI_PIN) {
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
                        .id('cancelBtnMixedPinFaceAuth')
                        .margin({ left: $r('app.float.content_padding') })
                        .width(Constants.ninetyPercentWidth)
                        .height($r('app.float.btn_height'))
                        .fontSize($r('sys.float.ohos_id_text_size_button1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary_activated'))
                        .fontWeight(FontWeight.Medium)
                        .backgroundColor(Color.Transparent)
                        .onClick(() => {
                          this.textValue = '';
                          this.handleCancel();
                        })
                    }.width(Constants.halfContainerWidth)

                    Divider()
                      .vertical(true)
                      .height($r('app.float.digital_password_mask_height'))
                      .color($r('sys.color.ohos_id_color_list_separator'))
                      .width($r('app.float.divider_width'))
                    Column() {
                      Button($r('app.string.unified_authwidget_confirm'))
                        .id('okBtnFaceAuth')
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
              if (this.dialogTypeFlag === SIX_PIN) {
                // Password 6-bit
                Column() {
                  SixPassword({
                    textValue: $textValue,
                    inputValue: $inputValue,
                    isEdit: $isEdit
                  })
                  Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                    .id('cancelBtnSixPinFaceAuth')
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
              if (this.dialogTypeFlag === SINGLE_FACE) {
                // Single FACE
                if (AppStorage.Get('titleLength') as number < (AppStorage.get('wantParams') as WantParams)?.title.length) {
                  Scroll() {
                    Column() {
                      Text((AppStorage.get('wantParams') as WantParams)?.title)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body1'))
                        .fontColor($r('sys.color.ohos_id_color_text_primary'))
                        .fontWeight(FontWeight.Medium)
                        .id('titleFaceAuth')
                      Image(this.prompt !== (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                        .getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id)
                        ? $r('app.media.error_faceID') : $r('app.media.right_faceID'))
                        .draggable(false)
                        .id('imageFaceAuth')
                        .margin({
                          top: $r('app.float.digital_password_mask_height'),
                          bottom: $r('app.float.digital_password_mask_height')
                        })
                        .width($r('app.float.image_size'))
                        .height($r('app.float.image_size'))
                        .colorBlend($r('sys.color.ohos_id_color_activated'))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                      Text(this.prompt)
                        .draggable(false)
                        .fontSize($r('sys.float.ohos_id_text_size_body2'))
                        .id('textFaceAuth')
                        .height($r('app.float.size_24'))
                        .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                        .onClick(() => {
                          this.sendFaceEvent();
                        })
                    }.margin({left: $r('app.float.size_24'), right: $r('app.float.size_24')})
                  }
                  .width('100%')
                  .height($r('app.float.scroll_height_182'))
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
                    .id('titleFaceAuth')
                  Image(this.prompt !== (AppStorage.get('context') as common.ExtensionContext)?.resourceManager
                    .getStringSync($r('app.string.unified_authwidget_hint_recognize_success').id)
                    ? $r('app.media.error_faceID') : $r('app.media.right_faceID'))
                    .draggable(false)
                    .id('imageFaceAuth')
                    .margin({
                      top: $r('app.float.digital_password_mask_height'),
                      bottom: $r('app.float.digital_password_mask_height')
                    })
                    .width($r('app.float.image_size'))
                    .height($r('app.float.image_size'))
                    .colorBlend($r('sys.color.ohos_id_color_activated'))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                  Text(this.prompt)
                    .draggable(false)
                    .fontSize($r('sys.float.ohos_id_text_size_body2'))
                    .id('textFaceAuth')
                    .height($r('app.float.size_24'))
                    .fontColor(this.onFontColor(this.prompt, (AppStorage.get('context') as common.ExtensionContext)))
                    .onClick(() => {
                      this.sendFaceEvent();
                    })
                }
                if (!((AppStorage.get('wantParams') as WantParams)?.navigationButtonText as string)) {
                  Row() {
                    Column() {
                      Button($r('app.string.unified_authwidget_cancel'), { stateEffect: true })
                        .id('cancelBtnAllFace')
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
                          AuthUtils.getInstance().sendNotice('EVENT_AUTH_USER_NAVIGATION', [Constants.noticeTypeFace]);
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
    }
    .margin(this.IS_LANDSCAPE ? '0' : { bottom: this.SYSTEM_NAVIGATION_BAR_HEIGHT })
    .height(Constants.fullContainerHeight)
    .justifyContent(this.IS_LANDSCAPE ? FlexAlign.Center : FlexAlign.End)
    .backgroundColor(Color.Transparent)
    .id('faceAuth')
  }
}
