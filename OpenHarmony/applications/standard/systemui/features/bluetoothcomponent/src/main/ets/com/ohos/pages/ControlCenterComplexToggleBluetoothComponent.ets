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
import EventManager from "../../../../../../../../common/src/main/ets/default/event/EventManager";
import {obtainStartAbility} from "../../../../../../../../common/src/main/ets/default/event/EventUtil";
import ViewModel from '../viewmodel/BluetoothVM';
import iconTitleBase from '../../../../../../../../common/src/main/ets/template/iconTitleBase';

const TAG = 'bluetooth-ControlCenterComplexToggleBluetoothComponent';

@Component
export default struct ControlCenterComplexToggleBluetoothComponent {
  private IconInfo: Resource[] = [
    $r("app.media.ic_controlcenter_bt_d"),
    $r("app.media.ic_controlcenter_bt"),
  ];
  @State mTitle: Resource = $r("app.string.control_center_complex_toggle_bluetooth_title");
  @StorageLink('BluetoothOpenStatus') BluetoothOpenStatus: boolean = false;

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    ViewModel.initViewModel();
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Column() {
      iconTitleBase({
        iconOff: this.IconInfo[0],
        iconOn: this.IconInfo[1],
        mTitle: $mTitle,
        changeSwitch: $BluetoothOpenStatus,
        mClickEvent: (): void => this.mClickEvent(),
        mLongClickEvent: (): void => this.mLongClickEvent()
      })
    }.width('100%')
    .height('100%')
  }

  mClickEvent() {
    Log.showDebug(TAG, `mClickEvent, BluetoothOpenStatus: ${this.BluetoothOpenStatus}`);
    if (this.BluetoothOpenStatus) {
      ViewModel.disableBluetooth();
    } else {
      ViewModel.enableBluetooth();
    };
  }

  mLongClickEvent() {
    Log.showDebug(TAG, `mLongClickEvent, BluetoothOpenStatus: ${this.BluetoothOpenStatus}`);
    EventManager.publish(obtainStartAbility('com.ohos.settings', 'com.ohos.settings.BluetoothAbility'));
  }
}