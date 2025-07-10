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

import {isNfcAvailable} from '../../../../../../../../common/src/main/ets/default/Constants';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import {FASlotName} from '../../../../../../../../common/src/main/ets/default/Constants';
import LocationComponent from '../../../../../../../locationcomponent/src/main/ets/com/ohos/pages/ControlCenterSimpleToggleLocationComponent';
import RingModeComponent from '../../../../../../../ringmodecomponent/src/main/ets/com/ohos/pages/ControlCenterSimpleToggleRingModeComponent';
import AutoRotateComponent from '../../../../../../../autorotatecomponent/src/main/ets/com/ohos/pages/ControlCenterSimpleToggleAutoRotateComponent';
import NFCComponent from '../../../../../../../nfccomponent/src/main/ets/com/ohos/pages/ControlCenterSimpleToggleNFComponent';
import AirplaneComponent from '../../../../../../../airplanecomponent/src/main/ets/default/pages/ControlCenterSimpleToggleAirplaneComponent';
import SimpleToggleComponent from './SimpleToggleComponent';

const TAG = 'SimpleToggleLoadComponent';

@Component
export default struct SimpleToggleLoadComponent {
  private keyId: string = ''
  private mEditMode: boolean = false
  private mDragMode: boolean = false

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}, mEditMode: ${this.mEditMode} mDragMode: ${this.mDragMode}`);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  build() {
    Column() {
      if (this.keyId == FASlotName.LOCATION) {
        LocationComponent({
          keyId: this.keyId,
          mEditMode: this.mEditMode,
          mDragMode: this.mDragMode,
        })
      } else if (this.keyId == FASlotName.RING_MODE) {
        RingModeComponent({
          keyId: this.keyId,
          mEditMode: this.mEditMode,
          mDragMode: this.mDragMode,
        })
      } else if (this.keyId == FASlotName.AUTO_ROTATE) {
        AutoRotateComponent({
          keyId: this.keyId,
          mEditMode: this.mEditMode,
          mDragMode: this.mDragMode,
        })
      } else if (this.keyId == FASlotName.AIR_PLANE) {
        AirplaneComponent({
          keyId: this.keyId,
          mEditMode: this.mEditMode,
          mDragMode: this.mDragMode,
        })
      } else if (this.keyId == FASlotName.NFC) {
        if (isNfcAvailable()) {
          NFCComponent({
            keyId: this.keyId,
            mEditMode: this.mEditMode,
            mDragMode: this.mDragMode,
          })
        }
      } else {
        SimpleToggleComponent({
          keyId: this.keyId,
          mEditMode: this.mEditMode,
          mDragMode: this.mDragMode,
        })
      }
    }.width('100%')
    .height('100%')
  }
}