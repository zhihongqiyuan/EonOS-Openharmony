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
import router from '@system.router';

@Entry
@Component
struct Login {
  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
      Column() {
        Image($r('app.media.logo')).width('100vp').height('100vp').margin({ top: '180vp' })
        Blank()
        Text($r('app.string.login'))
          .fontSize('22fp')
          .fontColor($r('app.color.color_fff'))
          .fontWeight(FontWeight.Bold)
          .margin({ bottom: '150vp' })
          .border({ width: '1vp', color: $r('app.color.color_fff'), radius: '20vp' })
          .width('300vp')
          .height('45vp')
          .textAlign(TextAlign.Center)
          .onClick(() => {
            router.push({ uri: 'pages/MainPage' })
          })
      }
      .width('100%')
      .height('100%')
    }
    .width('100%')
    .height('100%')
    .linearGradient({
      angle: 135,
      direction: GradientDirection.Left,
      colors: [[$r('app.color.colorPrimary'), 0.0], [$r('app.color.colorPrimary'), 1.0]]
    })
  }
}

