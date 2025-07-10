/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import {Log} from '@ohos/common'

const TAG = 'ScreenLock-BatterySocViewModel'

export default class BatterySocViewModel {
  batterySocStr: any= undefined
  mbatterySocStr= AppStorage.SetAndLink('batterySoc', 0);

  ViewModelInit(): void{
    Log.showDebug(TAG, 'ViewModelInit');
    let batteryValue: number = this.mbatterySocStr.get();
    if (batteryValue < 100) {
      this.batterySocStr = ""
    } else {
      this.batterySocStr =  $r('app.string.charge_full')
    }
  }
}
