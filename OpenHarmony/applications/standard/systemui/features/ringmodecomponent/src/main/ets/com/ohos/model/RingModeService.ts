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

import audio from '@ohos.multimedia.audio';
import { BusinessError } from '@ohos.base';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import { getAudioManager } from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import { AudioRingMode } from '../common/Constants';

const TAG = 'RingModeModel';

export interface RingModeStatusListener {
  updateRingerMode(status: AudioRingMode): void;
}

export class RingModeService {
  mIsStart = false;
  mListeners = new Set<RingModeStatusListener>();
  mAudioManager: any;

  async startService(): Promise<void> {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'startService');
    this.mIsStart = true;

    this.mAudioManager = await getAudioManager().getVolumeManager().getVolumeGroupManager(audio.DEFAULT_VOLUME_GROUP_ID);

    this.getRingerMode();

    this.mAudioManager.on('ringerModeChange', (data: AudioRingMode) => {
      Log.showInfo(TAG, `startService->ringerModeChange, data: ${JSON.stringify(data)}`);
      this.mListeners.forEach(listener => listener.updateRingerMode(data));
    });
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'stopService');
    this.mIsStart = false;

    this.mAudioManager = null;
  }

  registerListener(listener: RingModeStatusListener): void {
    let res = this.mListeners.add(listener);
    Log.showInfo(TAG, `registser ringMode Listener ${res}`);
  }

  unregisterListener(listener: RingModeStatusListener): void {
    let res = this.mListeners.delete(listener);
    Log.showInfo(TAG, `unregistser ringMode Listener ${res}`);
  }

  getRingerMode(): void {
    this.mAudioManager?.getRingerMode((error: BusinessError, action: AudioRingMode) => {
      Log.showInfo(TAG, `getRingerMode, error: ${JSON.stringify(error)} action: ${JSON.stringify(action)}`);
      if (error) {
        return;
      }
      this.mListeners.forEach(listener => listener.updateRingerMode(action));
    });
  }

  setRingerMode(mode: AudioRingMode): void {
    Log.showInfo(TAG, `setRingerMode, mode: ${JSON.stringify(mode)}`);
    this.mAudioManager?.setRingerMode(mode, (err: BusinessError) => {
      Log.showInfo(TAG, 'mAudioManager.setRingerMode');
    });
  }
}

let sRingModeService = createOrGet(RingModeService, TAG);

export default sRingModeService;