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

import geolocation from '@ohos.geoLocationManager';
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';

const TAG = 'LocationModel';

export interface LocationStatrListener {
  updateServiceState(status: boolean): void;
}

export class LocationService {
  mIsStart = false;
  mListener: LocationStatrListener;

  startService(): void {
    if (this.mIsStart) {
      return;
    }
    Log.showInfo(TAG, 'startService');
    this.mIsStart = true;
    this.getServiceState();
    try {
      geolocation.on('locationEnabledChange', (state: boolean) => {
        Log.showInfo(TAG, `startService locationChange, state: ${JSON.stringify(state)}`);
        this.getServiceState();
      });
    } catch {
      Log.showError(TAG, 'geolocation.on try-catch error');
    }
  }

  stopService(): void {
    if (!this.mIsStart) {
      return;
    };
    Log.showInfo(TAG, 'stopService');
    this.mIsStart = false;
    try {
      geolocation.off('locationEnabledChange', (state: boolean) => {
        Log.showInfo(TAG, `stopService locationChange, state: ${JSON.stringify(state)}`)
      });
    } catch {
      Log.showError(TAG, 'geolocation.off try-catch error');
    }
  }

  registerListener(listener: LocationStatrListener): void {
    Log.showInfo(TAG, `registerListener, listener: ${listener}`);
    this.mListener = listener;
  }

  getServiceState(): void {
    Log.showDebug(TAG, 'getServiceState');
    try {
      let data = geolocation.isLocationEnabled()
      Log.showInfo(TAG, `getServiceState isLocationEnabled, data: ${JSON.stringify(data)}`);
      this.mListener?.updateServiceState(data);
    } catch(err) {
      Log.showDebug(TAG, `getServiceState Error：${JSON.stringify(err || {})}`)
    }
  }

  enableLocation(): void {
    Log.showInfo(TAG, 'enableLocation');
    try {
      geolocation.enableLocation()
        .then((res) => Log.showInfo(TAG, `enableLocation, result: ${JSON.stringify(res)}`))
        .then(() => {
        }).catch((err) => {
      });
    } catch {
      Log.showError(TAG, 'geolocation.enableLocation try-catch error');
    }
  }

  disableLocation(): void {
    Log.showInfo(TAG, 'disableLocation');
    try {
      geolocation.disableLocation();
    } catch {
      Log.showError(TAG, 'geolocation.disableLocation try-catch error');
    }
  }
}

let sLocationService = createOrGet(LocationService, TAG);

export default sLocationService;