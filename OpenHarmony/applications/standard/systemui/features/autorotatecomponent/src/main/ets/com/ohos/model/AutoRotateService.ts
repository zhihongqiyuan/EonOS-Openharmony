//@ts-nocheck
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

import screen from '@ohos.screen';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';

const TAG = 'AutoRotateModel';

export interface AutoRotateSwitchStatusListener {
  updateAutoRotateSwitchStatus(status: boolean): void;
}

export class AutoRotateService {
  mIsStart = false;
  mListener: AutoRotateSwitchStatusListener;

  constructor() {
  }

  registerListener(listener: AutoRotateSwitchStatusListener): void {
    Log.showInfo(TAG, `registerListener, listener: ${listener}`);
    this.mListener = listener;
  }

  startService(): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'startService');
    this.mIsStart = true;
    this.asyncStartService().then(() => {
    }).catch((err) => {
    });
  }

  async asyncStartService(): Promise<void> {
    Log.showInfo(TAG, 'asyncStartService');
    this.updateAutoRotateSwitchStatus();
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'stopService');
    this.mIsStart = false;
    this.asyncStopService().then(() => {
    }).catch((err) => {
    });
  }

  async asyncStopService(): Promise<void> {
    Log.showInfo(TAG, 'asyncStopService');
    screen.off('change', (value: number) => {
      Log.showInfo(TAG, `asyncStopService, off change value: ${value}`);
    });
  }

  updateAutoRotateSwitchStatus() {
    screen.isScreenRotationLocked().then((isLocked) => {
      this.mListener?.updateAutoRotateSwitchStatus(!isLocked);
      Log.showDebug(TAG, `getScreenLockStatus, isLocked: ${JSON.stringify(isLocked)}`);
    });
  }

  async changeSwitch(status: boolean): Promise<void> {
    screen.setScreenRotationLocked(!status).then((err, data) => {
      this.updateAutoRotateSwitchStatus();
      if (err.code) {
        Log.showDebug(TAG, `changeSwitch, error: ${JSON.stringify(err)}`);
      } else {
        Log.showDebug(TAG, `changeSwitch success, isAutoRotate: ${JSON.stringify(status)}`);
      }
    });
    this.updateAutoRotateSwitchStatus();
  }
}


let sAutoRotateService = createOrGet(AutoRotateService, TAG);

export default sAutoRotateService;
