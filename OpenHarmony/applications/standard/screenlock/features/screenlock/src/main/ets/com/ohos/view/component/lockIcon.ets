/**
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

import {Log, ScreenLockStatus} from '@ohos/common'
import ViewModel from '../../vm/lockIconViewModel'
import Constants from '../../common/constants'

const TAG = 'ScreenLock-LockIcon'

@Component
export default struct LockIcon {
  @StorageLink('lockStatus') @Watch("onStatusChange") lockStatus: ScreenLockStatus = ScreenLockStatus.Locking
  @State mViewModel: ViewModel = new ViewModel()

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`)
    this.mViewModel.ViewModelInit()
  }

  build() {
    Flex({ direction: FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
      Image(this.mViewModel.iconPath)
        .margin({ top: $r('app.float.lockicon_top_margin'), bottom: $r('app.float.lockicon_bottom_margin') })
        .objectFit(ImageFit.Contain)
        .width($r('app.float.lockicon_width'))
        .height($r('app.float.lockicon_height'))
        .fillColor($r('app.color.lock_ic_color'))
      Text(this.mViewModel.cutMessage).fontSize($r('app.float.lock_prompt_fontsize'))
        .fontColor($r('app.color.lock_prompt_color'))
        .margin({ top: $r("app.float.lock_Text_top_margin") })
    }
    .width(Constants.FULL_CONTAINER_WIDTH)
    .gesture(GestureGroup(GestureMode.Exclusive, TapGesture({ count: 2 })
      .onAction((event: GestureEvent) => {
        Log.showInfo(TAG, 'double click');
        this.mViewModel.onRecognizeFace(this.lockStatus);
      })
    ))
  }

  onStatusChange(propName: string): void {
    Log.showInfo(TAG, `lockStatus:${this.lockStatus}`);
    this.mViewModel.onStatusChange(this.lockStatus)
  }
}