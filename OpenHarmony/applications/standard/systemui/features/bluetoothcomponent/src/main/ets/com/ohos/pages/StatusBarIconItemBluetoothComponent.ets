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
import { TintContentInfo } from '../../../../../../../../common/src/main/ets/default/TintStateManager';
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration';
import StyleConfiguration, { StatusBarBluetoothComponentStyle } from '../common/StyleConfiguration';
import Constants from '../common/Constants';
import ViewModel from '../viewmodel/BluetoothVM';

const TAG = 'bluetooth-StatusBarIconItemBluetoothComponent';

@Component
export default struct StatusBarIconItemBluetoothComponent {
  @StorageLink('BluetoothOpenStatus') BluetoothOpenStatus: boolean = false;
  @State mTintContentInfo: TintContentInfo = ViewModel.getTintContentInfo();
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle();
  @State style: StatusBarBluetoothComponentStyle = StyleConfiguration.getStatusBarBluetoothComponentStyle();

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    ViewModel.initViewModel();
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Row() {
      if (this.BluetoothOpenStatus) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
        Image($r("app.media.ic_status_bt"))
          .objectFit(ImageFit.Contain)
          .width(this.style.staticBarBluetoothWidth)
          .height(this.style.staticBarBluetoothHeight)
          .fillColor(this.mTintContentInfo.contentColor)
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
    }
    .height('100%')
    .opacity($r("app.float.icon_component_opacity"))
  }
}