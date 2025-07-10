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

import {Log, sWindowManager, WindowType} from '@ohos/common'
import {StatusBarGroupComponentData} from '../common/constants';

const TAG = 'StatusBarVM';

export class StatusBarVM {
  mConfig: any;
  mIsStart: boolean = false;
  mStatusBarLayoutGroupTemplate: any[] = [];
  mStatusBarLayout: any;
  mStatusBarEmptyWidth: any;
  mUseCount = 0;
  mStatusBarEnable: boolean = true;
  mDefaultStatusBarGroupComponentData: StatusBarGroupComponentData= {
    ...new StatusBarGroupComponentData()
  };
  mStatusBarGroupComponentDataMap = {};
  mWindowWidth = AppStorage.SetAndLink("maxWidth", 0);


  setStatusBarEnable(isEnable: boolean) {
    Log.showInfo(TAG, `setStatusBarEnable, isEnable ${isEnable}`);
    if (this.mStatusBarEnable == isEnable) {
      return;
    }
    this.mStatusBarEnable = isEnable;
    this.mStatusBarEnable ? sWindowManager.showWindow(WindowType.STATUS_BAR) : sWindowManager.hideWindow(WindowType.STATUS_BAR);
  }

  getStatusBarGroupComponentData(groupId: string): StatusBarGroupComponentData{
    Log.showInfo(TAG, `getStatusBarGroupComponentData, groupId: ${groupId}`);
    return this.mStatusBarGroupComponentDataMap[groupId] ?? this.mDefaultStatusBarGroupComponentData;
  }
}

let statusBarVM = new StatusBarVM();

export default statusBarVM as StatusBarVM;