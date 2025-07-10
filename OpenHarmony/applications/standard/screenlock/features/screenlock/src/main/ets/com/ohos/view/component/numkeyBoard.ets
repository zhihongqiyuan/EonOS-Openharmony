/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
import BaseViewModel from '../../vm/baseViewModel'
import deviceInfo from '@ohos.deviceInfo';
import {Callback} from '@ohos.base';
import { NumKeyBoardValue } from '../../../../../../../../noticeitem/src/main/ets/com/ohos/noticeItem/common/constants';

const TAG = 'ScreenLock-NumkeyBoard'

@Component
export default struct NumkeyBoard {
  @Prop viewModel: BaseViewModel;
  private onKeypress: (params: number, callback: Callback<void>) => void = () => {
  };
  @StorageLink('numKeyboard') numKeyboard: NumKeyBoardValue[] = Constants.NUMKEY_BOARD;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`)
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`)
  }

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
      Grid() {
        ForEach(this.numKeyboard, (item: NumKeyBoardValue) => {
          GridItem() {
            Stack({ alignContent: Alignment.Center }) {
              Column({ space: 0 }) {
                Flex({
                  direction: FlexDirection.Column,
                  alignItems: ItemAlign.Center,
                  justifyContent: FlexAlign.Center
                }) {
                  if (item?.value == Constants.CALL_PHONE ||
                  item?.value == Constants.GO_BACK ||
                  item?.value == Constants.DEL_PWD) {
                    Text(item?.row1)
                      .fontSize($r('app.float.digitalpsd_prompt_fontsize'))
                      .fontWeight(FontWeight.Medium)
                      .fontColor(Color.White)
                      .textAlign(TextAlign.Center)
                      .width(Constants.FULL_CONTAINER_WIDTH)
                      .padding(0)
                      .margin(0)
                  } else {
                    Text(item?.row1)
                      .fontSize($r('app.float.digitalpsd_row1_fontsize'))
                      .fontWeight(FontWeight.Medium)
                      .fontColor(Color.White)
                      .textAlign(TextAlign.Center)
                      .width(Constants.FULL_CONTAINER_WIDTH)
                      .padding(0)
                      .margin(0)
                  }
                  if (!!item?.row2) {
                    Text(item?.row2)
                      .fontSize($r('app.float.digitalpsd_row2_fontsize'))
                      .fontWeight(FontWeight.Regular)
                      .fontColor(Color.White)
                      .opacity($r("app.float.numkeyBoard_text_opacity"))
                      .textAlign(TextAlign.Center)
                      .width(Constants.FULL_CONTAINER_WIDTH)
                      .padding(0)
                      .margin(0)
                  }
                }
                .width(Constants.FULL_CONTAINER_WIDTH)
                .height($r('app.float.digitalpsd_row_height'))
              }

              if (deviceInfo.deviceType === 'tablet') {
                if (item?.value == Constants.CALL_PHONE ||
                item?.value == Constants.GO_BACK ||
                item?.value == Constants.DEL_PWD) {
                  Column() {
                    Button({ type: ButtonType.Normal, stateEffect: true })
                      .backgroundColor("#00FFFFFF")
                      .width(Constants.DIGITALPSD_BUTTON_RECT_WH)
                      .height(Constants.DIGITALPSD_BUTTON_RECT_HH)
                  }
                } else {
                  Column() {
                    Button({ type: ButtonType.Circle, stateEffect: !this.viewModel.inhibitInput })
                      .backgroundColor("#00FFFFFF")
                      .width(Constants.DIGITALPSD_BUTTON_DIAMETER)
                      .height(Constants.DIGITALPSD_BUTTON_DIAMETER)
                  }
                }
              }
            }
            .onClick(() => {
              Log.showInfo(TAG, `onClick:${item?.index}`)
              setTimeout(
                this.onKeypress.bind(this)
                , 0, item?.index, () => {
                Log.showInfo(TAG, `onClick callback is called`)
              });
            })
          }
        })
      }
      .columnsTemplate('1fr 1fr 1fr')
      .rowsTemplate('1fr 1fr 1fr 1fr')
      .rowsGap($r("app.float.numkeyBoard_rowsGap"))
      .width(Constants.FULL_CONTAINER_WIDTH)
      .height(Constants.FULL_CONTAINER_HEIGHT)
    }
    .width(Constants.FULL_CONTAINER_WIDTH)
    .height(Constants.FULL_CONTAINER_HEIGHT)
  }
}