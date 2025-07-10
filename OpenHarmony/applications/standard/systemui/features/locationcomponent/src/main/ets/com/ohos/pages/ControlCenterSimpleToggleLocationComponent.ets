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
import {LocationData} from '../common/Constants';
import ViewModel from '../viewmodel/LocationVM';
import SimpleToggleBase from '../../../../../../../../common/src/main/ets/template/SimpleToggleBase';

const TAG = 'location-ControlCenterSimpleToggleLocationComponent';

@Component
export default struct ControlCenterSimpleToggleLocationComponent {
  private keyId: string = '';
  private mEditMode: boolean = false;
  private mDragMode: boolean = false;
  @State mIcon: Resource = $r("app.media.ic_controlcenter_gps");
  @State mLabel: Resource = $r("app.string.control_center_complex_toggle_location_title");
  @StorageLink('LocationVM_LocationData') LocationStatus: boolean = false

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    ViewModel.initViewModel();
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    SimpleToggleBase({
      mToggleId: this.keyId,
      mIcon: $mIcon,
      mChangeSwitch: $LocationStatus,
      mLabel: $mLabel,
      mEditMode: this.mEditMode,
      mDragMode: this.mDragMode,
      mClickEvent: (): void => this.mClickEvent(),
      mLongClickEvent: (): void => this.mLongClickEvent()
    })
  }

  mClickEvent() {
    Log.showDebug(TAG, `mClickEvent, isEnabled: ${this.LocationStatus}`);
    if (this.LocationStatus) {
      ViewModel.disableLocation();
    } else {
      ViewModel.enableLocation();
    };
  }

  mLongClickEvent() {
    Log.showDebug(TAG, `mLongClickEvent`);
    EventManager.publish(obtainStartAbility('com.ohos.settings', 'com.ohos.settings.MainAbility'));
  }
}