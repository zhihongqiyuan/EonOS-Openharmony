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
import {AutoRotateData} from '../common/Constants';
import ViewModel from '../viewmodel/AutoRotateVM';
import SimpleToggleBase from '../../../../../../../../common/src/main/ets/template/SimpleToggleBase';

const TAG = 'autorotate-ControlCenterSimpleToggleAutoRotateComponent';

@Component
export default struct ControlCenterSimpleToggleAutoRotateComponent {
  private keyId: string = '';
  private mEditMode: boolean = false;
  private mDragMode: boolean = false;
  @State mIcon: Resource = $r("app.media.ic_controlcenter_auto_rotate_off");
  @State mLabel: Resource = $r("app.string.control_center_complex_toggle_auto_rotate_title");
  @State mAutoRotateSwitch : boolean = false
  @State @Watch('onAutoRotateDataChange') mAutoRotateData: AutoRotateData = ViewModel.getAutoRotateData();

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    this.onAutoRotateDataChange('mAutoRotateData');
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear')
  }

  onAutoRotateDataChange(propName: string): void {
    Log.showDebug(TAG, `onAutoRotateDataChange, propName: ${propName}`);
    this.mIcon = this.mAutoRotateData.switchStatus ? $r("app.media.ic_controlcenter_auto_rotate_on") : $r("app.media.ic_controlcenter_auto_rotate_off");
    this.mAutoRotateSwitch = this.mAutoRotateData.switchStatus
  }

  build() {
    SimpleToggleBase({
      mToggleId: this.keyId,
      mIcon: $mIcon,
      mChangeSwitch:$mAutoRotateSwitch,
      mLabel: $mLabel,
      mEditMode: this.mEditMode,
      mDragMode: this.mDragMode,
      mClickEvent: (): void => this.mClickEvent()
    })
  }

  mClickEvent() {
    Log.showDebug(TAG, `mClickEvent, status: ${this.mAutoRotateData.switchStatus}`);
    let newStatus = !this.mAutoRotateData.switchStatus;
    ViewModel.processAutoRotateSwitchStatusChange(newStatus);
  }
}