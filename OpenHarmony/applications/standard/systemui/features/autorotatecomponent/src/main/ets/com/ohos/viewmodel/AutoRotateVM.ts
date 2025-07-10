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

import Log from '../../../../../../../../common/src/main/ets/default/Log';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import AutoRotateService from '../model/AutoRotateService';
import { AutoRotateData } from '../common/Constants';

const TAG = 'AutoRotateVM';

export class AutoRotateVM {
  mAutoRotateData: AutoRotateData= {
    ...new AutoRotateData()
  };

  constructor() {
    Log.showDebug(TAG, 'constructor');
    this.mAutoRotateData = AppStorage.SetAndLink(TAG + '_AutoRotateData', this.mAutoRotateData).get();
    AutoRotateService.registerListener(this);
    AutoRotateService.startService();
  }

  getAutoRotateData(): AutoRotateData {
    Log.showDebug(TAG, 'getAutoRotateData');
    return this.mAutoRotateData;
  }

  updateAutoRotateSwitchStatus(status: boolean): void{
    Log.showInfo(TAG, `updateAutoRotateSwitchStatus, status: ${status}`);
    this.mAutoRotateData.switchStatus = status;
  }

  processAutoRotateSwitchStatusChange(status: boolean): void{
    Log.showDebug(TAG, `processAutoRotateSwitchStatusChange, status: ${status}`);
    AutoRotateService.changeSwitch(status).then(() => {
    }).catch((err) => {
    });
  }
}

let sAutoRotateVM = createOrGet(AutoRotateVM, TAG);

export default sAutoRotateVM;