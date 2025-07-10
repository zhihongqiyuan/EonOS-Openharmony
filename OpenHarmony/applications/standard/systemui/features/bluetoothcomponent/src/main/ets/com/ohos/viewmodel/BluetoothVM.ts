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
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants';
import { TintContentInfo, getOrCreateTintContentInfo
} from '../../../../../../../../common/src/main/ets/default/TintStateManager';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import BluetoothService from '../model/BluetoothService';

export const BLUETOOTH_OPEN_STATUS_KEY = 'BluetoothOpenStatus';

const TAG = 'BluetoothVM';

export class BluetoothVM {
  mIsStart = false;
  mBluetoothOpenStatus: SubscribedAbstractProperty<boolean>;
  mTintContentInfo: TintContentInfo = getOrCreateTintContentInfo(FASlotName.BLUETOOTH);

  constructor() {
    Log.showDebug(TAG, 'constructor');
  }

  initViewModel(): void {
    if (this.mIsStart) {
      return;
    }
    Log.showDebug(TAG, 'initViewModel ');
    this.mIsStart = true;
    this.mBluetoothOpenStatus = AppStorage.SetAndLink(BLUETOOTH_OPEN_STATUS_KEY, false);
    BluetoothService.startService();
    BluetoothService.registerListener(this);
  }

  updateState(state: boolean): void {
    Log.showInfo(TAG, `updateState, state: ${state} this.mBluetoothOpenStatus ${this.mBluetoothOpenStatus}`);
    this.mBluetoothOpenStatus.set(state);
  }

  enableBluetooth(): boolean{
    return BluetoothService.enableBluetooth();
  }

  disableBluetooth(): boolean{
    return BluetoothService.disableBluetooth();
  }

  getTintContentInfo(): TintContentInfo {
    return this.mTintContentInfo;
  }
}

let sBluetoothVM = createOrGet(BluetoothVM, TAG);

export default sBluetoothVM;