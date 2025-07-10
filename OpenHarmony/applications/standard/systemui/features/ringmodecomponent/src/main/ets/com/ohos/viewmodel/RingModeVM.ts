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
import { AudioRingMode } from '../common/Constants';
import RingModeService from '../model/RingModeService';

export const RING_MODE_COMPONENT_MODE_KEY = 'RingModeComponentMode';

const TAG = 'RingModeVM';

export class RingModeVM {
  mIsStart = false;
  mRingModeComponentMode: SubscribedAbstractProperty<AudioRingMode>;
  mTintContentInfo: TintContentInfo = getOrCreateTintContentInfo(FASlotName.RING_MODE);

  constructor() {
    Log.showInfo(TAG, 'constructor');
  }

  initViewModel(): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'initViewModel ');
    this.mIsStart = true;

    this.mRingModeComponentMode = AppStorage.SetAndLink(RING_MODE_COMPONENT_MODE_KEY, AudioRingMode.RINGER_MODE_NORMAL);

    RingModeService.registerListener(this);
    RingModeService.startService();
  }

  updateRingerMode(mode: AudioRingMode): void {
    Log.showInfo(TAG, `updateRingerMode, mode: ${JSON.stringify(mode)} `);
    this.mRingModeComponentMode.set(mode);
  }

  setRingerMode(mode: AudioRingMode): void {
    Log.showInfo(TAG, `setRingerMode, mode: ${JSON.stringify(mode)} `);
    RingModeService.setRingerMode(mode);
  }

  getTintContentInfo(): TintContentInfo {
    return this.mTintContentInfo;
  }
}

let sRingModeVM = createOrGet(RingModeVM, TAG);

export default sRingModeVM;