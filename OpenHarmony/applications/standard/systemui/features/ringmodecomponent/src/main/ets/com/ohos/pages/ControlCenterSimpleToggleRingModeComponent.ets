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

import Log from '../../../../../../../../common/src/main/ets/default/Log'
import EventManager from "../../../../../../../../common/src/main/ets/default/event/EventManager"
import { obtainStartAbility } from "../../../../../../../../common/src/main/ets/default/event/EventUtil"
import { AudioRingMode } from '../common/Constants'
import ViewModel from '../viewmodel/RingModeVM'
import SimpleToggleBase from '../../../../../../../../common/src/main/ets/template/SimpleToggleBase'
import StyleConfiguration, { ControlCenterRingModeComponentStyle } from '../common/StyleConfiguration'

const TAG = 'ringmode-ControlCenterSimpleToggleRingModeComponent'

@Component
export default struct ControlCenterSimpleToggleRingModeComponent {
  private keyId: string = ''
  private mEditMode: boolean = false
  private mDragMode: boolean = false
  @State mIcon: Resource = $r("app.media.ic_controlcenter_ring_on_filled")
  @State mLabel: Resource = $r("app.string.control_center_complex_toggle_ring_mode_title_on")
  @State mDefaultChangeSwitch: boolean  = true
  @StorageLink('RingModeComponentMode') @Watch('onRingModeUpdated') RingModeComponentMode: AudioRingMode = AudioRingMode.RINGER_MODE_NORMAL
  @State style: ControlCenterRingModeComponentStyle = StyleConfiguration.getControlCenterRingModeComponentStyle()

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    ViewModel.initViewModel()
    this.onRingModeUpdated('RingModeComponentMode')
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  onRingModeUpdated(propName: string): void {
    Log.showInfo(TAG, `onRingModeUpdated, propName: ${propName} RingModeComponentMode: ${JSON.stringify(this.RingModeComponentMode)}`);
    if (this.RingModeComponentMode == AudioRingMode.RINGER_MODE_SILENT) {
      this.mIcon = $r("app.media.ic_controlcenter_ring_off_filled")
      this.mLabel = $r("app.string.control_center_complex_toggle_ring_mode_title_off")
    } else if (this.RingModeComponentMode == AudioRingMode.RINGER_MODE_VIBRATE) {
      this.mIcon = $r("app.media.ic_controlcenter_vibration_filled")
      this.mLabel = $r("app.string.control_center_complex_toggle_ring_mode_title_vibration")
    } else if (this.RingModeComponentMode == AudioRingMode.RINGER_MODE_NORMAL) {
      this.mIcon = $r("app.media.ic_controlcenter_ring_on_filled")
      this.mLabel = $r("app.string.control_center_complex_toggle_ring_mode_title_on")
    }
  }

  build() {
    SimpleToggleBase({
      mToggleId: this.keyId,
      mIcon: $mIcon,
      mChangeSwitch: $mDefaultChangeSwitch,
      mLabel: $mLabel,
      mIconOnBG: this.style.onBgColor,
      mEditMode: this.mEditMode,
      mDragMode: this.mDragMode,
      mClickEvent: (): void => this.mClickEvent(),
      mLongClickEvent: (): void => this.mLongClickEvent()
    })
  }

  mClickEvent() {
    Log.showInfo(TAG, `mClickEvent`);
    if (this.RingModeComponentMode == AudioRingMode.RINGER_MODE_SILENT) {
      ViewModel.setRingerMode(AudioRingMode.RINGER_MODE_NORMAL)
    } else if (this.RingModeComponentMode == AudioRingMode.RINGER_MODE_NORMAL) {
      ViewModel.setRingerMode(AudioRingMode.RINGER_MODE_SILENT)
    }
  }

  mLongClickEvent() {
    Log.showInfo(TAG, `mLongClickEvent`);
    EventManager.publish(obtainStartAbility('com.ohos.settings', 'com.ohos.settings.MainAbility'))
  }
}