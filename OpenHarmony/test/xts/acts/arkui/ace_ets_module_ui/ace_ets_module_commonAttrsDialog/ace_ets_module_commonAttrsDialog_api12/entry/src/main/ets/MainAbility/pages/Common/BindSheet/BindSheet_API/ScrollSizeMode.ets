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
@Entry
@Component
struct ScrollSizeModeExample {
  @State isShow: boolean = false;
  @State isShow2: boolean = false;
  @State sheetHeight: number = 300;
  @State scrollSizeModeStatus: ScrollSizeMode = ScrollSizeMode.FOLLOW_DETENT;
  @State mScrollSizeMode1: string = '';
  @State mScrollSizeMode2: string = '';

  @Builder
  myBuilder() {
    Column() {
      Button("change")
        .margin(10)
        .fontSize(20)
        .onClick(() => {
          this.scrollSizeModeStatus = ScrollSizeMode.CONTINUOUS;
        }).id('change')

      Button("FOLLOW_DETENT")
        .margin(10)
        .fontSize(20)
        .onClick(() => {
          this.mScrollSizeMode1 = this.scrollSizeModeStatus + ''
        }).id('change1')

      Button("CONTINUOUS")
        .margin(10)
        .fontSize(20)
        .onClick(() => {
          this.mScrollSizeMode2 = this.scrollSizeModeStatus + ''
        }).id('change2')

      Button('close modal')
        .margin(10)
        .fontSize(20)
        .onClick(() => {
          this.isShow = false;
        }).id('close')
    }
    .width('100%')
    .height('100%')
  }

  build() {
    Column() {
      Text(this.mScrollSizeMode1)
        .id('ScrollSizeMode_1')
      Text(this.mScrollSizeMode2)
        .id('ScrollSizeMode_2')
      Button('transition modal 1')
        .onClick(() => {
          this.isShow = true
        })
        .id('btn')
        .fontSize(20)
        .margin(10)
        .bindSheet($$this.isShow, this.myBuilder(), {
          height: this.sheetHeight,
          backgroundColor: Color.Green,
          scrollSizeMode: this.scrollSizeModeStatus
        })
    }
    .justifyContent(FlexAlign.Center)
    .width('100%')
    .height('100%')
  }
}