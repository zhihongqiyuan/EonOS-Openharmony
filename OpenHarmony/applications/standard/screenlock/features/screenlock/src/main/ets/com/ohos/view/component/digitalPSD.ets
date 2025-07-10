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

import {Log} from '@ohos/common'
import Constants from '../../common/constants'
import NumkeyBoard from './numkeyBoard'
import ViewModel from '../../vm/digitalPSDViewModel'

const TAG = 'ScreenLock-DigitalPSD'

@Component
export default struct DigitalPSD {
  @State mViewModel: ViewModel = new ViewModel()
  @StorageLink('passwdMaskArr') passwdMaskArr: Resource[] = this.mViewModel.passwdMaskArr
  @StorageLink('screenlockdirection') screenlockdirection: number = 1

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`)
    this.mViewModel.checkFreezingTime(() => {
      Log.showInfo(TAG, `onClick callback is called`)
    });
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`)
    this.mViewModel.clearPassword()
  }

  onPageShow() {
    Log.showInfo(TAG, `onPageShow`)
  }

  onPageHide() {
    Log.showInfo(TAG, `onPageHide`)
  }

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.End }) {
      Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.End }) {
        Text(this.mViewModel.prompt)
          .fontSize($r('app.float.digitalpsd_prompt_fontsize'))
          .fontWeight(FontWeight.Medium)
          .fontColor(Color.White)
          .margin({ bottom: $r('app.float.digitalpsd_prompt_margin_bottom') })
          .textAlign(TextAlign.Center)
          .textOverflow({ overflow: TextOverflow.None })
          .width($r('app.float.digitalpsd_prompt_width'))
          .height($r('app.float.digitalpsd_prompt_height'))

        Grid() {
          ForEach(this.passwdMaskArr, (item: PixelMap | ResourceStr | DrawableDescriptor) => {
            GridItem() {
              Image(item)
                .width(Constants.DIGITALPSD_IC_DIAMETER)
                .height(Constants.DIGITALPSD_IC_DIAMETER)
            }
          })
        }
        .columnsTemplate('1fr 1fr 1fr 1fr 1fr 1fr')
        .width($r('app.float.digitalpsd_passwdmask_width'))
        .height($r('app.float.digitalpsd_passwdmask_height'))
        .margin(0)
      }
      .margin({
        bottom: $r('app.float.digitalpsd_passwdmask_margin_bottom')
      })
      .width(Constants.FULL_CONTAINER_WIDTH)
      .height($r('app.float.digitalpsd_input_area_height'))
      .flexShrink(1)

      Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.End }) {
        NumkeyBoard({
          viewModel: this.mViewModel,
          onKeypress: (index, callback) => this.mViewModel.onKeyPress(index, callback)
        })
      }
      .width(Constants.FULL_CONTAINER_WIDTH)
      .height($r('app.float.digitalpsd_key_area_height'))
      .margin({ bottom: this.screenlockdirection ==  1 ? $r('app.float.digitalpsd_key_area_margin_bottom') : $r('app.float.digitalpsd_key_area_margin_bottom_portrait') })
      .flexShrink(0)
    }
    .width(Constants.FULL_CONTAINER_WIDTH)
  }
}