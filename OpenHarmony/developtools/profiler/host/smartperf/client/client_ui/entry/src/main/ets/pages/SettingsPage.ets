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
import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';

/**
 * 设置页面
 */
@Entry
@Component
struct SettingsPage {
  build() {
    Column() {
      //开始测试title
      StartTestTitleComponent({ title: '设置' })

      Row({ space: '15vp' }) {
        Image($r('app.media.icon_language')).width('25vp').height('25vp').margin({ left: '2%' })

        Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
          Text('语言切换').fontSize('15fp').fontColor($r('app.color.color_333'))

          Image($r('app.media.icon_enter')).width('15vp').height('15vp').margin({ left: '15vp' })
        }.height('47vp').width('82%')
      }.height('47vp').width('95%').backgroundColor($r('app.color.color_fff')).margin({ top: '10vp' })

      Divider().layoutWeight(1).visibility(Visibility.Hidden)

      Text('退出登录')
        .fontSize('15fp')
        .fontColor($r('app.color.color_fff'))
        .margin({ bottom: '45vp' })
        .border({ radius: '20vp' })
        .backgroundColor($r('app.color.colorPrimary'))
        .width('80%')
        .height('45vp')
        .textAlign(TextAlign.Center)
        .onClick(res => {
          router.replace({ uri: 'pages/LoginPage' })
          router.clear()
        })
    }
  }
}