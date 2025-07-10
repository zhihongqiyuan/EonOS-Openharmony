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

import Log from '../../../../../../../../common/src/main/ets/default/Log'
import { TintContentInfo } from '../../../../../../../../common/src/main/ets/default/TintStateManager'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { StatusRingModeComponentStyle } from '../common/StyleConfiguration'
import Constants, { AudioRingMode } from '../common/Constants'
import ViewModel from '../viewmodel/RingModeVM'

const TAG = 'ringmode-StatusBarIconItemRingModeComponent'

@Component
export default struct StatusBarIconItemRingModeComponent {
  @StorageLink('RingModeComponentMode') RingModeComponentMode: AudioRingMode = AudioRingMode.RINGER_MODE_NORMAL
  @State mTintContentInfo: TintContentInfo = ViewModel.getTintContentInfo()
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()
  @State style: StatusRingModeComponentStyle = StyleConfiguration.getStatusRingModeComponentStyle()
  private ringModeIcons: Resource[] = []

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
    this.ringModeIcons = [];
    this.ringModeIcons[AudioRingMode.RINGER_MODE_SILENT] = $r("app.media.ic_statusbar_ring_off_filled");
    this.ringModeIcons[AudioRingMode.RINGER_MODE_VIBRATE] = $r("app.media.ic_statusbar_vibration_on");
    ViewModel.initViewModel();
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Row() {
      if (this.RingModeComponentMode != AudioRingMode.RINGER_MODE_NORMAL) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
        Image(this.ringModeIcons[this.RingModeComponentMode])
          .objectFit(ImageFit.Contain)
          .width(this.style.statusBarRingModeWidth)
          .height(this.style.statusBarRingModeHeight)
          .fillColor(this.mTintContentInfo.contentColor)
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
    }
    .height('100%')
    .opacity($r("app.float.icon_component_opacity"))
  }
}