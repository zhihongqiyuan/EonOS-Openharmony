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

import userAuth from '@ohos.userIAM.userAuth';
import window from '@ohos.window';
import { DialogType } from '../../../../main/ets/common/module/DialogType';
import FuncUtils from '../../../../main/ets/common/utils/FuncUtils';
import LogUtils from '../../../../main/ets/common/utils/LogUtils';
import Constants, { CmdType, WantParams, WidgetCommand } from '../../../../main/ets/common/vm/Constants';
import CustomPassword from '../../../../main/ets/pages/components/CustomPassword';
import FaceAuth from '../../../../main/ets/pages/components/FaceAuth';
import FingerprintAuth from '../../../../main/ets/pages/components/FingerprintAuth';
import PasswordAuth from '../../../../main/ets/pages/components/PasswordAuth';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';
import { DEFAULT } from '@ohos/hypium';

const TAG = 'Index';
const defaultSize = 72;
let userAuthWidgetMgr: userAuth.UserAuthWidgetMgr;

@Entry
@Component
struct Index {
  @State authType: Array<userAuth.UserAuthType> = [userAuth.UserAuthType.PIN];
  @State type: string[] = [];
  @State topHeight: number = defaultSize;
  @State bottomHeight: number = defaultSize;
  @State pinSubType: string = Constants.pinNumber;
  @State dialogType: DialogType = DialogType.ALL;
  @State windowModeType: string = 'DIALOG_BOX';
  @State cmdData: Array<CmdType> = [];
  @Provide isLandscape: boolean = false;
  @Provide underFingerPrint: boolean = false;

  onCancel(): void {
    LogUtils.debug(TAG, 'Callback when the first button is clicked');
    (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
  }

  onAccept(): void {
    LogUtils.debug(TAG, 'Callback when the second button is clicked');
  }

  existApp(): void {
    LogUtils.debug(TAG, 'Click the callback in the blank area');
    (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
  }

  handleAuthStart(): void {
    try {
      userAuthWidgetMgr = userAuth.getUserAuthWidgetMgr(Constants.userAuthWidgetMgrVersion);
      LogUtils.info(TAG, 'getUserAuthWidgetMgr success');
      let that = this;
      userAuthWidgetMgr.on('command', {
        sendCommand(result) {
          LogUtils.info(TAG, 'sendCommand result: ' + result);
          const cmdDataObj: WidgetCommand = JSON.parse(result || '{}');
          if (cmdDataObj?.cmd?.[0]?.payload?.result === Constants.userAuthWidgetMgrSuccess) {
            that.onCancel();
          } else {
            that.cmdData = cmdDataObj?.cmd || [];
            that.pinSubType = cmdDataObj?.pinSubType;
          }
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'getUserAuthWidgetMgr catch error: ' + error?.code);
    }
  }

  aboutToAppear(): void {
    LogUtils.debug(TAG, 'aboutToAppear test');
    if (AppStorage.get('wantParams') as WantParams) {
      LogUtils.debug(TAG, 'wantParams： ' + JSON.stringify(AppStorage.get('wantParams') as WantParams));
      this.getParams(AppStorage.get('wantParams') as WantParams);
    } else {
      LogUtils.error(TAG, 'aboutToAppear wantParams null');
    }
    this.getWindowHeight();
  }

  aboutToDisappear(): void {
    LogUtils.info(TAG, 'aboutToDisappear');
    if (userAuthWidgetMgr) {
      userAuthWidgetMgr.off('command', {
        sendCommand(result) {
          LogUtils.info(TAG, 'aboutToDisappear userAuthWidgetMgr offCommand result: ' + JSON.stringify(result));
        }
      });
    }
  }

  onPageShow(): void {
    LogUtils.info(TAG, 'onPageShow');
  }

  onPageHide(): void {
    LogUtils.info(TAG, 'onPageHide');
  }

  onBackPress(): void {
    LogUtils.info(TAG, 'onBackPress');
  }

  getParams(result: WantParams): void {
    LogUtils.info(TAG, 'getParams');
    const resultInfo: WantParams = result;
    this.pinSubType = resultInfo?.pinSubType;
    const newType = resultInfo?.type && resultInfo?.type.map(item => {
      if (item === Constants.noticeTypePin) {
        return userAuth.UserAuthType.PIN;
      } else if (item === Constants.noticeTypeFinger) {
        return userAuth.UserAuthType.FINGERPRINT;
      } else if (item === Constants.noticeTypeFace) {
        return userAuth.UserAuthType.FACE;
      } else {
        return userAuth.UserAuthType.PIN;
      }
    })
    AppStorage.setOrCreate('widgetContextId', resultInfo.widgetContextId as number);
    this.authType = newType;
    this.type = resultInfo.type;
    this.windowModeType = resultInfo.windowModeType;
    this.dialogType = FuncUtils.getDialogType(newType);
    this.cmdData = resultInfo.cmd || [];
  }

  getWindowHeight(): void {
    LogUtils.info(TAG, 'getWindowHeight');
    try {
      window.on('systemBarTintChange', (data) => {
        LogUtils.debug(TAG, 'Succeeded in enabling the listener for window stage event changes. Data: ' +
        JSON.stringify(data));
        for (let i = 0; i < data.regionTint.length; i++) {
          let regionData = data.regionTint[i];
          if (regionData.region === undefined) {
            continue;
          } else if (regionData.type === window.WindowType.TYPE_STATUS_BAR) {
            this.topHeight = px2vp(regionData.region.height);
            continue;
          } else if (regionData.type === window.WindowType.TYPE_NAVIGATION_BAR) {
            this.bottomHeight = px2vp(regionData.region.top);
            continue;
          }
        }
      });
    } catch (error) {
      LogUtils.error(TAG, 'Failed to enable the listener for window stage event changes. error: ' + error?.code);
    }
  }

  build() {
    Flex({ direction: FlexDirection.Column, justifyContent: FlexAlign.SpaceAround, alignItems: ItemAlign.Center }) {
      if ('DIALOG_BOX' === this.windowModeType) {
        if (this.authType.includes(userAuth.UserAuthType.FACE)) {
          FaceAuth({
            type: $windowModeType,
            pinSubType: $pinSubType,
            dialogType: $dialogType,
            cmdData: $cmdData,
          })
        } else if (this.authType.includes(userAuth.UserAuthType.FINGERPRINT)) {
          FingerprintAuth({
            type: $windowModeType,
            pinSubType: $pinSubType,
            dialogType: $dialogType,
            cmdData: $cmdData,
          })
        } else {
          PasswordAuth({
            pinSubType: $pinSubType,
            cmdData: $cmdData
          })
        }

      } else {
        // full screen PIN
        CustomPassword({
          authType: $authType,
          pinSubType: $pinSubType,
          cmdData: $cmdData
        })
      }
    }
    .backgroundColor(Color.Transparent)
    .position({ x: 0, y: this.topHeight })
    .height((this.bottomHeight - this.topHeight))
  }
}