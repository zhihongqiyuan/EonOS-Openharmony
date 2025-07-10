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

/**
 * home页、报告页、我的页面top栏
 */
@Component
export struct TopComponent {
  @State title: string = 'SmartPerf'

  build() {
    //开始测试title
    Text(this.title)
      .width('100%')
      .height('8%')
      .fontSize('20fp')
      .fontWeight(FontWeight.Bold)
      .fontColor($r('app.color.color_fff'))
      .alignSelf(ItemAlign.Start)
      .textAlign(TextAlign.Start)
      .margin({ left: '15vp', top: '30vp' })
  }
}