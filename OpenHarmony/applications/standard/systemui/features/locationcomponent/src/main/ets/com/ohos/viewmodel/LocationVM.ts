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
import SwitchUserManager from '../../../../../../../../common/src/main/ets/default/SwitchUserManager';
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants';
import { TintContentInfo, getOrCreateTintContentInfo
} from '../../../../../../../../common/src/main/ets/default/TintStateManager';
import createOrGet from '../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import { LocationData } from '../common/Constants';
import LocationService from '../model/LocationService';

const TAG = 'LocationVM';

export class LocationVM {
  mIsStart = false;
  mLocationData: LocationData = {
    ...new LocationData()
  };
  mLocationStatus:SubscribedAbstractProperty<boolean>;
  mTintContentInfo: TintContentInfo = getOrCreateTintContentInfo(FASlotName.LOCATION);

  constructor() {
    Log.showDebug(TAG, 'constructor');
  }

  initViewModel(): void {
    if (this.mIsStart) {
      return;
    };
    Log.showInfo(TAG, 'initViewModel ');
    this.mIsStart = true;
    SwitchUserManager.getInstance().registerListener(this);
    LocationService.registerListener(this);
    LocationService.startService();
  }

  userChange():void{
    LocationService.getServiceState()
  }

  updateServiceState(state: boolean): void {
    Log.showInfo(TAG, `updateServiceState, state: ${state} `);
    AppStorage.SetOrCreate(TAG + '_LocationData', state)
  }

  enableLocation(): void {
    Log.showInfo(TAG, 'enableLocation');
    LocationService.enableLocation();
  }

  disableLocation(): void {
    Log.showInfo(TAG, 'disableLocation');
    LocationService.disableLocation();
  }

  getTintContentInfo(): TintContentInfo {
    return this.mTintContentInfo;
  }
}

let sLocationVM = createOrGet(LocationVM, TAG);

export default sLocationVM;