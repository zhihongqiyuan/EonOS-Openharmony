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
import ViewModel from '../../vm/mixedPSDViewModel'
import deviceInfo from '@ohos.deviceInfo';

const DEVICETYPE = deviceInfo.deviceType;
const TAG = 'ScreenLock-MixedPSD'

@Component
export default struct MixedPSD {
  @State mViewModel: ViewModel = new ViewModel()

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

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
      if (DEVICETYPE == 'phone') {
        Column() {
          Text(this.mViewModel.prompt)
            .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
            .fontWeight(FontWeight.Medium)
            .fontColor(Color.White)
            .margin({
              bottom: $r('app.float.mixedpsd_prompt_margin_bottom'),
              top: $r("app.float.mixedpsd_prompt_margin_top")
            })
            .textAlign(TextAlign.Center)
            .textOverflow({ overflow: TextOverflow.None })
            .width($r('app.float.mixedpsd_prompt_width'))
            .height($r('app.float.mixedpsd_prompt_height'))

          TextInput({ placeholder: '', text: this.mViewModel.password })
            //            .textAlign(TextAlign.Center)
            //            .showPasswordIcon(false)
            .type(InputType.Password)
            .maxLength(Constants.PASSWORD_MAX_LEN)
            .fontSize($r('app.float.mixedpsd_prompt_text_fontsize'))
            .width($r('app.float.mixedpsd_input_width'))
            .height($r('app.float.mixedpsd_input_height'))
            .backgroundColor("rgba(255,255,255,0.2)")
            .fontColor(Color.White)
            .enterKeyType(EnterKeyType.Done)
            .enabled(!this.mViewModel.inhibitInput)
            .onChange(this.mViewModel.onInputChange.bind(this.mViewModel))
            .onSubmit((entryKey) => {
              this.mViewModel.onAuthPassword(() => {
                Log.showInfo(TAG, `onSubmit callback prompt:${this.mViewModel.prompt} inhibitInput:${this.mViewModel.inhibitInput}`)
              })
            })
            .margin({ bottom: $r('app.float.mixedpsd_input_margin_bottom') })
          Column() {
            Text('确认')
              .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
              .width(Constants.PASSWORD_TEXT_WIDTH)
              .height(Constants.PASSWORD_TEXT_HEIGHT)
              .backgroundColor('#0a59f7')
              .fontWeight(FontWeight.Medium)
              .fontColor(Color.White)
              .textAlign(TextAlign.Center)
              .textOverflow({ overflow: TextOverflow.None })
              .borderRadius(Constants.PASSWORD_TEXT_BORDER)
              .onClick(() => {
                Log.showInfo(TAG, `onClick`)
                this.mViewModel.onAuthPassword(() => {
                  Log.showInfo(TAG, `onClick callback prompt:${this.mViewModel.prompt} inhibitInput:${this.mViewModel.inhibitInput}`)
                });
              })
            Text($r('app.string.emergency_call'))
              .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
              .fontWeight(FontWeight.Medium)
              .fontColor(Color.White)
              .textAlign(TextAlign.Center)
              .textOverflow({ overflow: TextOverflow.None })
              .margin({ top: Constants.ACCOUNT_SPACE })
              .onClick(() => {
                this.mViewModel.onCallPhone.bind(this.mViewModel)
              });
          }
          .width(Constants.HALF_CONTAINER_WIDTH)
        }.width(Constants.FULL_CONTAINER_WIDTH)
        .height(Constants.FULL_CONTAINER_HEIGHT)


      } else {
        Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
          Text(this.mViewModel.prompt)
            .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
            .fontWeight(FontWeight.Bold)
            .fontColor(Color.White)
            .margin({ bottom: $r('app.float.mixedpsd_prompt_margin_bottom') })
            .textAlign(TextAlign.Center)
            .textOverflow({ overflow: TextOverflow.None })
            .width($r('app.float.mixedpsd_prompt_width'))
            .height($r('app.float.mixedpsd_prompt_height'))

          TextInput({ placeholder: '', text: this.mViewModel.password })
            .type(InputType.Password)
            .maxLength(Constants.PASSWORD_MAX_LEN)
            .fontSize($r('app.float.mixedpsd_prompt_text_fontsize'))
            .textAlign(TextAlign.Center)
            .width($r('app.float.mixedpsd_input_width'))
            .height($r('app.float.mixedpsd_input_height'))
            .backgroundColor(Color.Gray)
            .enterKeyType(EnterKeyType.Done)
            .border({ width: 2, color: Color.Gray, radius: 15 })
            .enabled(!this.mViewModel.inhibitInput)
            .onChange(this.mViewModel.onInputChange.bind(this.mViewModel))
            .onSubmit((entryKey) => {
              this.mViewModel.onAuthPassword(() => {
                Log.showInfo(TAG, `onSubmit callback prompt:${this.mViewModel.prompt} inhibitInput:${this.mViewModel.inhibitInput}`)
              })
            })

          Button('确认', {
            type: ButtonType.Capsule,
            stateEffect: true
          })
            .width($r('app.float.mixedpsd_input_width'))
            .height($r('app.float.mixedpsd_input_height'))
            .onClick(() => {
              Log.showInfo(TAG, `onClick`)
              this.mViewModel.onAuthPassword(() => {
                Log.showInfo(TAG, `onClick callback prompt:${this.mViewModel.prompt} inhibitInput:${this.mViewModel.inhibitInput}`)
              });
            })
            .margin({ top:  $r('sys.float.ohos_id_elements_margin_vertical_l'),bottom: $r('app.float.mixedpsd_input_margin_bottom')})

          Text($r('app.string.emergency_call'))
            .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
            .fontWeight(FontWeight.Bold)
            .fontColor(Color.White)
            .textAlign(TextAlign.Center)
            .textOverflow({ overflow: TextOverflow.None })
            .onClick(() => {
              this.mViewModel.onCallPhone.bind(this.mViewModel)
            });

          /*            Text($r('app.string.done'))
                        .fontSize($r('app.float.mixedpsd_prompt_fontsize'))
                        .fontWeight(FontWeight.Bold)
                        .fontColor(Color.White)
                        .textAlign(TextAlign.Center)
                        .textOverflow({ overflow: TextOverflow.None })
                        .onClick(() => {
                          Log.showInfo(TAG, `onClick`)
                          this.mViewModel.onAuthPassword(() => {
                            Log.showInfo(TAG, `onClick callback prompt:${this.mViewModel.prompt} inhibitInput:${this.mViewModel.inhibitInput}`)
                          });
                        })*/

        }
        .width(Constants.FULL_CONTAINER_WIDTH)
        .height(Constants.FULL_CONTAINER_HEIGHT)
      }
    }
    .width(Constants.FULL_CONTAINER_WIDTH)
    .height(Constants.FULL_CONTAINER_HEIGHT)
  }
}