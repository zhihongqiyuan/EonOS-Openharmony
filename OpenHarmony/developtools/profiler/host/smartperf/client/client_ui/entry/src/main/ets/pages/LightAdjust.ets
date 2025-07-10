/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

import brightness from '@ohos.brightness';
import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';
import SPLogger from '../common/utils/SPLogger'
/**
 * 亮度调整
 */
@Entry
@Component
struct LightAdjust {
  @State outSetValue: number = 40

  build() {
    Column() {

      StartTestTitleComponent({ title: '亮度调整' })

      Row() {
        Slider({
          value: this.outSetValue,
          min: 0,
          max: 255,
          step: 1,
          style: SliderStyle.OutSet
        })
          .blockColor(Color.Blue)
          .trackColor(Color.Gray)
          .selectedColor(Color.Blue)
          .showSteps(true)
          .onChange((value: number, mode: SliderChangeMode) => {
            this.outSetValue = value
            brightness.setValue(value)
            console.info('value:' + value + 'mode:' + mode.toString())
          })
      }.padding({ top: 50 })
      .width('80%')

      Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {

        Text('+').fontSize('25fp').fontWeight(FontWeight.Bold).onClick(() => {
          if (this.outSetValue == 255) {
            return
          }
          ++this.outSetValue
          brightness.setValue(this.outSetValue)
        })

        Text(this.outSetValue.toFixed(0)).fontWeight(FontWeight.Bold).fontSize('25fp')

        Text('-').fontSize('25fp').fontWeight(FontWeight.Bold).onClick(() => {
          if (this.outSetValue == 0) {
            return
          }
          --this.outSetValue
          brightness.setValue(this.outSetValue)
        })

      }.width('50%').padding({ top: 30 })

      Row() {
      }.backgroundColor($r('app.color.color_fff')).width('50%').height('50%')
    }.width('100%').height('100%')
  }
}