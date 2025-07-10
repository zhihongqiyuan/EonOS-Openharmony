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

import {Log, Trace} from '@ohos/common'
import BaseViewModel, {service, AuthSubType} from './baseViewModel'
import {Callback} from '@ohos.base';

const TAG = 'ScreenLock-MixedPSDViewModel'

export default class MixedPSDViewModel extends BaseViewModel {
  password: string= '';

  constructor() {
    super();
  }

  ViewModelInit(): void{
    Log.showDebug(TAG, 'ViewModelInit');
    super.ViewModelInit();
  }

  onInputChange(value: string) {
    Log.showInfo(TAG, `onInputChange`);
    this.password = value;
  }

  onCallPhone() {
    Log.showInfo(TAG, 'onCallPhone');
  }

  onAuthPassword(callback: Callback<void>) {
    Log.showInfo(TAG, `onAuthPassword`);
    if (!this.password || this.inhibitInput) {
      callback();
      return;
    }
    Trace.start(Trace.CORE_METHOD_UNLOCK_SCREEN);
    Trace.start(Trace.CORE_METHOD_CALL_ACCOUNT_SYSTEM);
    service.authUser(AuthSubType.PIN_MIXED, this.password, (result, extraInfo) => {
      this.clearPassword();
      if (result == 0) {
        //unlock the screen
        service.unlocking();
        service.goBack();
      } else {
        //Clear the entered password
        super.changePrompt(extraInfo.remainTimes, extraInfo.freezingTime, callback);
      }
    });
  }

  clearPassword() {
    Log.showInfo(TAG, `clearPassword`)
    this.password = '';
  }
}
