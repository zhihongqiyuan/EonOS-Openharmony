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

import parameter from '@ohos.systemparameter'
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants';
import { TintContentInfo, getOrCreateTintContentInfo
} from '../../../../../../../../common/src/main/ets/default/TintStateManager';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import NFCModeService from '../model/NFCModeService';
import NfcController  from '@ohos.nfc.controller';

export const NFC_MODE_COMPONENT_MODE_KEY = 'NFCModeComponentMode';

const TAG = 'NFCModeVM';

export class NFCModeVM {
  mIsStart = false;
  mNFCModeComponentMode: SubscribedAbstractProperty<NfcController.NfcState>;
  mTintContentInfo: TintContentInfo = getOrCreateTintContentInfo(FASlotName.NFC);

  constructor() {
    Log.showInfo(TAG, 'constructor');
  }

  initViewModel(): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'initViewModel ');
    this.mIsStart = true;

    this.mNFCModeComponentMode = AppStorage.SetAndLink(NFC_MODE_COMPONENT_MODE_KEY, NfcController.NfcState.STATE_OFF);

    NFCModeService.registerListener(this);
    NFCModeService.startService();
  }

  updateNFCMode(mode: NfcController.NfcState): void {
    Log.showInfo(TAG, `updateNFCMode, mode: ${JSON.stringify(mode)} `);
    this.mNFCModeComponentMode.set(mode);
  }

  setNFCMode(mode: NfcController.NfcState): void {
    Log.showInfo(TAG, `setNFCMode, mode: ${JSON.stringify(mode)} `);
    NFCModeService.setNFCMode(mode);
  }

  getTintContentInfo(): TintContentInfo {
    return this.mTintContentInfo;
  }

  isSupported(): string {
    return parameter.getSync("const.SystemCapability.Communication.NFC.Core", "false")
  }
}

let sNFCModeVM = createOrGet(NFCModeVM, TAG);

export default sNFCModeVM;