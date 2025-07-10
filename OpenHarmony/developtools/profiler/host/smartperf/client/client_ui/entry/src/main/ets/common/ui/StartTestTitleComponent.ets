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

@Component
export struct StartTestTitleComponent {
  private title: string = '开始测试'

  build() {
    //开始测试title
    Row({ space: '15vp' }) {
      Image($r('app.media.icon_back'))
        .width('25vp')
        .height('15%')
        .margin({ left: '5%', top: '30vp' })
        .alignSelf(ItemAlign.Center)
        .onClick(() => {
          router.back()
        })

      Text(this.title)
        .fontSize('20fp')
        .fontWeight(FontWeight.Bold)
        .fontColor($r('app.color.color_fff'))
        .margin({ top: '30vp' })
    }.backgroundColor($r('app.color.colorPrimary')).width('100%').height('12%')
  }
}