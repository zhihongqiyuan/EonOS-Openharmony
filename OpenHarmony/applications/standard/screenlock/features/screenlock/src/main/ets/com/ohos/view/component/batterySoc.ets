/**
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

const TAG = 'ScreenLock-BatterySoc'

@Component
export default struct BatterySoc {
  @StorageLink('batterySoc') @Watch('onBatterySocChanged') batterySoc: number = 100
  @StorageLink('batteryCharging') @Watch('onBatterySocChanged') batteryCharging: boolean = false
  @State batterySocText: Resource | null = null

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`)
    this.onBatterySocChanged('');
  }

  build() {
    Text(this.batterySocText)
      .fontSize($r('app.float.batterysoc_fontsize'))
      .fontColor($r('app.color.batterysoc_text_color'))
  }

  onBatterySocChanged(propName: string): void {
    Log.showInfo(TAG, `onBatterySocChanged batterySoc:${this.batterySoc}  batteryCharging:${this.batteryCharging}` )
    if (this.batteryCharging) {
      if (this.batterySoc < 100) {
        this.batterySocText = $r('app.string.charging', this.batterySoc + '%');
      } else {
        this.batterySocText = $r('app.string.charge_full')
      }
    } else {
      this.batterySocText = $r('app.string.battery_soc', this.batterySoc + '%');
    }
  }
}