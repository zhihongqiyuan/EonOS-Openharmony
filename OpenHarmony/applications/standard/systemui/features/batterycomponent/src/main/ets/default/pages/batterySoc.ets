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

import Log from '../../../../../../../common/src/main/ets/default/Log';
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../common/src/main/ets/default/StyleConfiguration';
import { TintContentInfo } from '../../../../../../../common/src/main/ets/default/TintStateManager';
import ViewModel from '../viewmodel/BatteryVM';

const TAG = 'BatteryComponent-batterySoc';

@Component
export default struct BatterySoc {
  @StorageLink('batterySoc') batterySoc: number = 100;
  @State mTintContentInfo: TintContentInfo = ViewModel.getTintContentInfo();
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle();

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear Start');
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Text(this.batterySoc + '%')
      .fontColor(this.mTintContentInfo.contentColor)
      .fontSize(this.styleCommon.statusBarFontSize)
  }
}
