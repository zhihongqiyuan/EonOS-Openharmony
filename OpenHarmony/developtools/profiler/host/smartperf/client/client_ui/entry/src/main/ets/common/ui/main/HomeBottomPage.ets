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
 * 主页底部tab
 */
@Component
export struct HomeBottomPage {
  @Link currentPage: number
  imgW = '32vp'
  imgH = '32vp'

  build() {
    Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {

      Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center, direction: FlexDirection.Column }) {
        if (this.currentPage == 0) {
          Image($r('app.media.icon_home_selected')).width(this.imgW).height(this.imgH)
          Text('首页').fontSize('12vp').fontColor($r('app.color.colorPrimary'))
        } else {
          Image($r('app.media.icon_home_unselected')).width(this.imgW).height(this.imgH)
          Text('首页').fontSize('12vp').fontColor(Color.Black)
        }
      }
      .onClick(() => {
        this.currentPage = 0
      })
      .backgroundColor($r('app.color.color_fff'))
      .width('180px')
      .height('100%')

      Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center, direction: FlexDirection.Column }) {
        if (this.currentPage == 1) {
          Image($r('app.media.icon_report_selected')).width(this.imgW).height(this.imgH)
          Text('报告').fontSize('12vp').fontColor($r('app.color.colorPrimary'))
        } else {
          Image($r('app.media.icon_report_unselected')).width(this.imgW).height(this.imgH)
          Text('报告').fontSize('12vp').fontColor(Color.Black)
        }
      }
      .onClick(() => {
        this.currentPage = 1
      })
      .backgroundColor($r('app.color.color_fff'))
      .width('180px')
      .height('100%')

      Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center, direction: FlexDirection.Column }) {
        if (this.currentPage == 2) {
          Image($r('app.media.icon_mine_selected')).width(this.imgW).height(this.imgH)
          Text('我的').fontSize('12vp').fontColor($r('app.color.colorPrimary'))
        } else {
          Image($r('app.media.icon_mine_unselected')).width(this.imgW).height(this.imgH)
          Text('我的').fontSize('12vp').fontColor(Color.Black)
        }
      }
      .onClick(() => {
        this.currentPage = 2
      })
      .backgroundColor($r('app.color.color_fff'))
      .width('180px')
      .height('100%')
    }
    .backgroundColor($r('app.color.color_fff'))
    .width('100%')
    .height('12%')
  }
}
