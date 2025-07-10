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

import mWifiManger from '../wifiModel'
import Log from '../../../../../../../common/src/main/ets/default/Log'
import EventManager from "../../../../../../../common/src/main/ets/default/event/EventManager"
import {obtainStartAbility} from "../../../../../../../common/src/main/ets/default/event/EventUtil"
import iconTitleBase from '../../../../../../../common/src/main/ets/template/iconTitleBase'
import Constants from '../common/constants'

const TAG = 'Control-wifiComponent'

@Component
export default struct WifiComponent {
  private IconInfo: Resource[] = [
    $r('app.media.wifi_d'),
    $r('app.media.wifi'),
  ]
  @StorageLink('wifiName') wifiName: string = Constants.DEFAULT_WIFI_NAME
  @StorageLink('wifiOpenStatus') wifiOpenStatus: boolean = Constants.DEFAULT_WIFI_OPEN_STATUS

  mClickEvent() {
    Log.showInfo(TAG, `mClickEvent, wifiOpenStatus: ${this.wifiOpenStatus}`);

    if (this.wifiOpenStatus == false) {
      Log.showInfo(TAG, `WLAN status is closing：${this.wifiOpenStatus} “no”， set WLAN status enableWifi`);
      mWifiManger.enableWifi()
    } else {
      Log.showInfo(TAG, `WLAN status is opening：${this.wifiOpenStatus} “yes”，se WLAN status disableWifi`);
      mWifiManger.disableWifi()
    }
  }

  mLongClickEvent() {
    Log.showInfo(TAG, `mLongClickEvent, wifiOpenStatus: ${this.wifiOpenStatus}`);
    EventManager.publish(obtainStartAbility('com.ohos.settings', 'com.ohos.settings.WifiAbility'))
  }

  aboutToAppear() {
    Log.showDebug(TAG, 'aboutToAppear');
    mWifiManger.initWifiModel();
  }

  aboutToDisappear() {
    Log.showDebug(TAG, 'aboutToDisappear');
  }

  build() {
    Column() {
      iconTitleBase({
        iconOff: this.IconInfo[0],
        iconOn: this.IconInfo[1],
        mTitle: $wifiName,
        changeSwitch: $wifiOpenStatus,
        mClickEvent: (): void => this.mClickEvent(),
        mLongClickEvent: (): void => this.mLongClickEvent()
      })
    }.width('100%')
    .height('100%')
  }
}