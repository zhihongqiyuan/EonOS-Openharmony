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

import CapsuleModel from '../model/CapsuleModel';
import Log from '../../../../../../../common/src/main/ets/default/Log';
import AbilityManager from '../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import ResourceUtil from '../../../../../../../common/src/main/ets/default/ResourceUtil';
import Trace from '../../../../../../../common/src/main/ets/default/Trace'

let sCapsuleViewModel;

const SEC_VAL = 1000;
const MIN_VAL = 60 * SEC_VAL;
const TAG = "CapsuleViewModel";

function fixInteger(num) {
  return (num >= 10) ? String(num) : '0' + num;
}

function parseTime(num) {
  return fixInteger(Math.floor(num / MIN_VAL)) + ":" + fixInteger(Math.floor((num % MIN_VAL) / SEC_VAL));
}

export const VIEW_MODEL_ID = "CapsuleViewModelVm";

export enum CallState {
  CALL_ACTIVE, //通话中*
  CALL_HOLDING, //通话保持
  CALL_DIALING, //拨号开始
  CALL_ALERTING, //正在呼出
  CALL_INCOMING, //来电*
  CALL_WAITING, //第三方来电*
  CALL_DISCONNECTED, //挂断完成 *
  CALL_DISCONNECTING, //正在挂断
  CALL_IDLE //空闲
}

export default class CapsuleViewModel {
  mText: string = "00:00";
  mStartTime: number = 0;
  mCallState: CallState = CallState.CALL_DISCONNECTED;
  mIsBackground: boolean = false;
  mWantBundleName: string = "";
  mWantAbilityName: string = "";
  mTimeMeter: any;
  mCallback: any;

  async initViewModel() {
    this.mCallback = {
      "onStateChange": this.onStateChange.bind(this)
    };
    CapsuleModel.registerCallback(this.mCallback);
  }

  async onStateChange(data) {
    Log.showInfo(TAG, `onStateChange, data: ${JSON.stringify(data)}`);
    this.mIsBackground = data.isBackground;
    this.mWantBundleName = data.wantBundleName;
    this.mWantAbilityName = data.wantAbilityName;
    await ResourceUtil.initResourceManager(AbilityManager.ABILITY_NAME_STATUS_BAR);
    if (data.callState == CallState.CALL_INCOMING || data.callState == CallState.CALL_WAITING) {
      this.mStartTime = 0;
      this.mText = await ResourceUtil.getString($r("app.string.incoming_call"));
    } else if (data.callState == CallState.CALL_ACTIVE) {
      clearTimeout(this.mTimeMeter);
      if (this.mCallState != CallState.CALL_ACTIVE) {
        this.mStartTime = new Date().valueOf();
      };
      this.mCallState = data.callState;

      let startTime;
      let commonTimeDiff = new Date().valueOf() - data.startTime;
      let localTimeDiff = new Date().valueOf() - this.mStartTime;
      if (commonTimeDiff < 0) {
        if (localTimeDiff < 0) {
          startTime = new Date().valueOf();
        } else {
          startTime = this.mStartTime;
        };
      } else {
        startTime = data.startTime;
      };

      this.startUpdateTime(startTime);
    } else if (data.callState == CallState.CALL_DISCONNECTED){
      clearTimeout(this.mTimeMeter);
      this.mStartTime = 0;
      this.mIsBackground = false;
      this.mText = "";
      Log.showInfo(TAG, `cannot show`);
    } else {
      this.mStartTime = 0;
      this.mText = await ResourceUtil.getString($r("app.string.communicate_by_phone"));
    };
    this.mCallState = data.callState;
  }

  startUpdateTime(startTime) {
    if (!this.mIsBackground || this.mCallState != CallState.CALL_ACTIVE) {
      return;
    }
    let val = new Date().valueOf() - startTime.valueOf();
    if (val < 0) {
      val = 0;
    }
    this.mText = parseTime(val);
    this.mTimeMeter = setTimeout(() => {
      this.startUpdateTime(startTime);
    }, 1000 - val % 1000);
  }

  onClickEvent() {
    Trace.start(Trace.CORE_METHOD_CLICK_CAPSULE);
    if (this.mIsBackground) {
      this.mIsBackground = false;
    }
    Log.showDebug(TAG, `onClickEvent `);
    Log.showDebug(TAG, `startAbility`);
    AbilityManager.startAbility(AbilityManager.getContext(AbilityManager.ABILITY_NAME_STATUS_BAR), {
      bundleName: this.mWantBundleName,
      abilityName: this.mWantAbilityName
    }, () => {
      Trace.end(Trace.CORE_METHOD_CLICK_CAPSULE);
    });
  }

  static getInstance() {
    if (sCapsuleViewModel == null) {
      sCapsuleViewModel = new CapsuleViewModel();
      AppStorage.SetAndLink(VIEW_MODEL_ID, sCapsuleViewModel);
    };
    return sCapsuleViewModel;
  }
}