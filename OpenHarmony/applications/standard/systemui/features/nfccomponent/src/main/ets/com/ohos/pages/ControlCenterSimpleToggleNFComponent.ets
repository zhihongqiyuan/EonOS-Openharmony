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
import { obtainStartAbility } from "../../../../../../../../common/src/main/ets/default/event/EventUtil";
import { nfcController } from '@kit.ConnectivityKit';
import ViewModel from '../viewmodel/NFCModeVM';
import SimpleToggleBase from '../../../../../../../../common/src/main/ets/template/SimpleToggleBase';

const TAG = 'nfcmode-ControlCenterSimpleToggleNFComponent';

@Component
export default struct ControlCenterSimpleToggleNFComponent {
  private keyId: string = '';
  private mEditMode: boolean = false;
  private mDragMode: boolean = false;
  @State mIcon: Resource = $r("app.media.ic_notification_nfc_filled");
  @State mLabel: Resource = $r("app.string.control_center_complex_toggle_nfc_mode_title");
  @State mDefaultChangeSwitch: boolean  = false;
  @StorageLink('NFCModeComponentMode') @Watch('onNFCModeUpdated') NFCModeComponentMode: nfcController.NfcState = nfcController.NfcState.STATE_OFF;
  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    ViewModel.initViewModel();
    this.onNFCModeUpdated('NFCModeComponentMode');
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  onNFCModeUpdated(propName: string): void {
    Log.showInfo(TAG, `onNFCModeUpdated, propName: ${propName} NFCModeComponentMode: ${JSON.stringify(this.NFCModeComponentMode)}`);
    if (this.NFCModeComponentMode == nfcController.NfcState.STATE_OFF) {
      this.mDefaultChangeSwitch = false;
    } else if (this.NFCModeComponentMode == nfcController.NfcState.STATE_ON) {
      this.mDefaultChangeSwitch = true;
    }
  }

  build() {
    SimpleToggleBase({
      mToggleId: this.keyId,
      mIcon: $mIcon,
      mChangeSwitch: $mDefaultChangeSwitch,
      mLabel: $mLabel,
      mEditMode: this.mEditMode,
      mDragMode: this.mDragMode,
      mClickEvent: (): void => this.mClickEvent()
    })
  }

  mClickEvent() {
    Log.showInfo(TAG, `mClickEvent---${this.NFCModeComponentMode}`);
    if (this.NFCModeComponentMode == nfcController.NfcState.STATE_OFF) {
      ViewModel.setNFCMode(nfcController.NfcState.STATE_OFF)
    } else if (this.NFCModeComponentMode == nfcController.NfcState.STATE_ON) {
      ViewModel.setNFCMode(nfcController.NfcState.STATE_ON)
    }
  }
}