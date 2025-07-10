/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import { LengthMetrics } from '@ohos.arkui.node'
@Entry
@Component
struct LineSpacingExample_2 {
  @State lineSpacingValue: LengthMetrics = LengthMetrics.vp(20)
  @State LengthMetricsValue: string = ''
  build() {
    Row() {
      Column({ space: 12 }) {
        TextArea({ placeholder: 'This is the TextArea with lineSpacing set to 20_vp.' })
          .fontSize(12)
          .lineSpacing(this.lineSpacingValue)
        Text(this.LengthMetricsValue)
          .id('LengthMetricsValue')
        Button('show strategyStatus')
          .id('btn')
          .onClick(() => {
            if (this.lineSpacingValue.value == 20) {
              this.LengthMetricsValue = '20vp'
            } else {
              this.LengthMetricsValue = 'null'
            }
          })
      }.width('100%')
    }.height('100%')
  }
}