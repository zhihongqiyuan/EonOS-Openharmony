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
import getSingleInstance, { getAudioManager
} from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import { AudioVolumeType, VolumeInfo } from '../common/Constants';

const TAG = 'VolumePanelModel';

export interface VolumeListener {
  updateVolumeInfo(status: VolumeInfo): void;
}

export interface VolumeCallQueueItemData {
  interfaceName: InterfaceName,
  params: boolean | number[],

  callbackFunction(err: BusinessError, data?: boolean | number): void;
}

enum InterfaceName {
  isMute,
  mute,
  getVolume,
  setVolume
}

export class VolumePanelService {
  mIsStart = false;
  mListeners = new Set<VolumeListener>();
  mAudioManager = getAudioManager();
  mInterfaceCallQueue: VolumeCallQueueItemData[] = [] ;

  constructor() {
    Log.showInfo(TAG, 'constructor');
  }

  startService(): void {
    if (this.mIsStart) {
      return;
    }
    this.mIsStart = true;
    Log.showInfo(TAG, 'startService');
    this.mAudioManager.on('volumeChange', this.onVolumeChange.bind(this));
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'stopService');
    this.mIsStart = false;
  }

  registerListener(listener: VolumeListener): void {
    let res = this.mListeners.add(listener);
    Log.showInfo(TAG, `registser VolumeListener ${res}`);
  }

  unregisterListener(listener: VolumeListener): void {
    let res = this.mListeners.delete(listener);
    Log.showInfo(TAG, `unregistser VolumeListener ${res}`);
  }

  async onVolumeChange(data): Promise<void> {
    Log.showInfo(TAG, 'onVolumeChange');
    let volumeType = this.formatAudioVolumeTypeFromInterface(data.volumeType);
    this.isMute(volumeType, (volumeType2: AudioVolumeType, data2: boolean) => {
      Log.showInfo(TAG, `onVolumeChange->isMute, volumeType2: ${volumeType2} data2: ${String(data2)}`);
      let volumeInfo: VolumeInfo = {
        volumeType: volumeType,
        volume: data.volume,
        isMute: data2,
        updateUi: data.updateUi
      };
      this.mListeners.forEach(listener => listener.updateVolumeInfo(volumeInfo));
    });
  }

  isMute(volumeType: AudioVolumeType, callback: (volumeType: AudioVolumeType, data: boolean) => void): void {
    Log.showInfo(TAG, `isMute, volumeType: ${volumeType}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.addInterfaceCallQueue({
      interfaceName: InterfaceName.isMute,
      params: [interfaceVolumeType],
      callbackFunction: (err, data: boolean) => {
        Log.showInfo(TAG, `isMute, err: ${err} data: ${JSON.stringify(data)}`);
        if (err) {
          return;
        }
        callback(volumeType, data);
      }
    });
  }

  getVolumeInfo(volumeType: AudioVolumeType, callback?: (volumeInfo: VolumeInfo) => void): void {
    Log.showInfo(TAG, `getVolumeInfo, volumeType: ${volumeType}`);
    this.getVolume(volumeType, (volumeType: AudioVolumeType, data: number) => {
      Log.showInfo(TAG, `getVolumeInfo->getVolume`);
      this.isMute(volumeType, (volumeType2: AudioVolumeType, data2: boolean) => {
        Log.showInfo(TAG, `getVolumeInfo->isMute`);
        let volumeInfo: VolumeInfo = {
          volumeType: volumeType,
          volume: data,
          isMute: data2,
          updateUi: false
        };
        this.mListeners.forEach(listener => listener.updateVolumeInfo(volumeInfo));
        if (callback) {
          callback(volumeInfo);
        }
      });
    });
  }

  isActive(volumeType: AudioVolumeType, callback: (volumeType: AudioVolumeType, data: boolean) => void): void {
    Log.showDebug(TAG, `isActive, volumeType: ${volumeType}`);

    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.mAudioManager.isActive(interfaceVolumeType, (err: BusinessError, data: boolean) => {
      Log.showDebug(TAG, `isActive, err: ${err} data: ${JSON.stringify(data)}`);
      if (err) {
        return;
      }
      callback(volumeType, data);
    });
  }

  getVolume(volumeType: AudioVolumeType, callback: (volumeType: AudioVolumeType, data: number) => void): void {
    Log.showDebug(TAG, `getVolume, volumeType: ${volumeType}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.addInterfaceCallQueue({
      interfaceName: InterfaceName.getVolume,
      params: [interfaceVolumeType],
      callbackFunction: (err: BusinessError, data: number) => {
        Log.showDebug(TAG, `getVolume, err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
        if (err) {
          return;
        }
        callback(volumeType, data);
      }
    });
  }

  getMaxVolume(volumeType: AudioVolumeType, callback: (volumeType: AudioVolumeType, data: number) => void): void {
    Log.showInfo(TAG, `getMaxVolume, volumeType: ${volumeType}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.mAudioManager.getMaxVolume(interfaceVolumeType, (err: BusinessError, data: number) => {
      Log.showInfo(TAG, `getMaxVolume, err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
      if (err) {
        return;
      }
      callback(volumeType, data);
    });
  }

  getMinVolume(volumeType: AudioVolumeType, callback: (volumeType: AudioVolumeType, data: number) => void): void {
    Log.showInfo(TAG, `getMinVolume, volumeType: ${volumeType}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.mAudioManager.getMinVolume(interfaceVolumeType, (err: BusinessError, data: number) => {
      Log.showInfo(TAG, `getMinVolume, err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
      if (err) {
        return;
      }
      callback(volumeType, data);
    });
  }

  setVolumeAndMute(volumeType: AudioVolumeType, volume: number, mute: boolean, callback?: () => void): void {
    Log.showInfo(TAG, `setVolumeAndMute, volumeType: ${volumeType} volume: ${volume} mute: ${mute}`);
    if (volume !== undefined && mute !== undefined) {
      this.setVolume(volumeType, volume, () => {
        Log.showInfo(TAG, `setVolume`);
        this.setMute(volumeType, mute, callback);
      });
    } else if (volume !== undefined) {
      this.setVolume(volumeType, volume);
    } else if (mute !== undefined) {
      this.setMute(volumeType, mute, callback);
    }
  }

  setVolume(volumeType: AudioVolumeType, volume: number, callback?: () => void): void {
    Log.showDebug(TAG, `setVolume, volumeType: ${volumeType} volume: ${volume}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.addInterfaceCallQueue({
      interfaceName: InterfaceName.setVolume,
      params: [interfaceVolumeType, volume],
      callbackFunction: (err: BusinessError) => {
        Log.showDebug(TAG, `setVolume, callback err: ${err} `);
        if (err) {
          return;
        }
        if (callback) {
          callback();
        }
      }
    });
  }

  setMute(volumeType: AudioVolumeType, mute: boolean, callback?: () => void): void {
    Log.showDebug(TAG, `setMute, volumeType: ${volumeType} mute: ${mute}`);
    let interfaceVolumeType = this.formatAudioVolumeTypeToInterface(volumeType);
    this.addInterfaceCallQueue({
      interfaceName: InterfaceName.mute,
      params: [interfaceVolumeType, mute],
      callbackFunction: (err: BusinessError) => {
        Log.showDebug(TAG, `setMute, err: ${JSON.stringify(err)}`);
        if (err) {
          return;
        }
        this.getVolumeInfo(volumeType, (volumeInfo: VolumeInfo) => {
          Log.showDebug(TAG, `setMute, volumeInfo: ${JSON.stringify(volumeInfo)}`);
          if (callback) {
            callback();
          }
        });
      }
    });
  }

  formatAudioVolumeTypeFromInterface(audioVolumeType): AudioVolumeType {
    let formatValue: AudioVolumeType;
    if (audioVolumeType == audio.AudioVolumeType.VOICE_CALL) {
      formatValue = AudioVolumeType.VOICE_CALL;
    } else if (audioVolumeType == audio.AudioVolumeType.RINGTONE) {
      formatValue = AudioVolumeType.RINGTONE;
    } else if (audioVolumeType == audio.AudioVolumeType.MEDIA) {
      formatValue = AudioVolumeType.MEDIA;
    } else if (audioVolumeType == audio.AudioVolumeType.VOICE_ASSISTANT) {
      formatValue = AudioVolumeType.VOICE_ASSISTANT;
    }
    return formatValue;
  }

  formatAudioVolumeTypeToInterface(audioVolumeType: AudioVolumeType) {
    let formatValue;
    if (audioVolumeType == AudioVolumeType.VOICE_CALL) {
      formatValue = audio.AudioVolumeType.VOICE_CALL;
    } else if (audioVolumeType == AudioVolumeType.RINGTONE) {
      formatValue = audio.AudioVolumeType.RINGTONE;
    } else if (audioVolumeType == AudioVolumeType.MEDIA) {
      formatValue = audio.AudioVolumeType.MEDIA;
    } else if (audioVolumeType == AudioVolumeType.VOICE_ASSISTANT) {
      formatValue = audio.AudioVolumeType.VOICE_ASSISTANT;
    }
    return formatValue;
  }

  addInterfaceCallQueue(data: VolumeCallQueueItemData): void {
    Log.showDebug(TAG, `addInterfaceCallQueue, interfaceName: ${data.interfaceName} params: ${JSON.stringify(data.params)}`);
    this.mInterfaceCallQueue.push(data);
    if (this.mInterfaceCallQueue.length == 1) {
      this.execInterfaceCallQueueFirst();
    }
  }

  execInterfaceCallQueueFirst(): void {
    Log.showInfo(TAG, `execInterfaceCallQueueFirst, ${this.mInterfaceCallQueue.length}`);
    let queueData = this.mInterfaceCallQueue[0];
    if (queueData.interfaceName == InterfaceName.isMute) {
      this.mAudioManager.isMute(queueData.params[0], (err: BusinessError, data: boolean) => {
        Log.showInfo(TAG, 'execInterfaceCallQueueFirst, isMute callback');
        this.execInterfaceCallQueueNext().then(() => {
        }).catch((e) => {
        });
        queueData.callbackFunction(err, data);
      });
    } else if (queueData.interfaceName == InterfaceName.mute) {
      this.mAudioManager.mute(queueData.params[0], queueData.params[1], (err: BusinessError) => {
        Log.showInfo(TAG, 'execInterfaceCallQueueFirst, mute callback');
        this.execInterfaceCallQueueNext().then(() => {
        }).catch((e) => {
        });
        queueData.callbackFunction(err);
      });
    } else if (queueData.interfaceName == InterfaceName.getVolume) {
      this.mAudioManager.getVolume(queueData.params[0], (err: BusinessError, data: number) => {
        Log.showInfo(TAG, 'execInterfaceCallQueueFirst, getVolume callback');
        this.execInterfaceCallQueueNext().then(() => {
        }).catch((e) => {
        });
        queueData.callbackFunction(err, data);
      });
    } else if (queueData.interfaceName == InterfaceName.setVolume) {
      this.mAudioManager.setVolume(queueData.params[0], queueData.params[1], (err: BusinessError) => {
        Log.showInfo(TAG, 'execInterfaceCallQueueFirst, setVolume callback');
        this.execInterfaceCallQueueNext().then(() => {
        }).catch((e) => {
        });
        queueData.callbackFunction(err);
      });
    }
  }

  async execInterfaceCallQueueNext(): Promise<void> {
    Log.showInfo(TAG, `execInterfaceCallQueueNext, ${this.mInterfaceCallQueue.length}`);
    this.mInterfaceCallQueue.splice(0, 1);
    if (this.mInterfaceCallQueue.length > 0) {
      this.execInterfaceCallQueueFirst();
    }
  }
}

let volumePanelService = getSingleInstance(VolumePanelService, TAG);

export default volumePanelService;