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
import {FASlotName} from '../../../../../../../../common/src/main/ets/default/Constants';
import AirPlaneComponent from "../../../../../../../airplanecomponent/src/main/ets/default/pages/airplaneComponent";
import WifiComponent from '../../../../../../../wificomponent/src/main/ets/default/pages/wifiComponent';
import BluetoothComponent from '../../../../../../../bluetoothcomponent/src/main/ets/com/ohos/pages/ControlCenterComplexToggleBluetoothComponent';
import ComplexToggleComponent from './ComplexToggleComponent';

const TAG = 'ComplexToggleLoadComponent';

@Component
export default struct ComplexToggleLoadComponent {
  @State keyId: string = '';

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}`);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  build() {
    Column() {
      if (this.keyId == FASlotName.WIFI) {
        WifiComponent()
      } else if (this.keyId == FASlotName.BLUETOOTH) {
        BluetoothComponent()
      } else if (this.keyId == FASlotName.AIR_PLANE) {
        AirPlaneComponent()
      } else {
        ComplexToggleComponent({
          keyId: this.keyId
        })
      }
    }.width('100%')
    .height('100%')
  }
}