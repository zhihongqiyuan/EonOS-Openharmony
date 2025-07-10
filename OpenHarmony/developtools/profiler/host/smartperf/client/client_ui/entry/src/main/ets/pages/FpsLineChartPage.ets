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
import { hideFloatWindow } from '../common/ui/floatwindow/utils/FloatWindowUtils'
import { FloatWindowComponent } from '../common/FloatWindowComponent'

@Entry
@Component
struct FpsLineChartPage {
  data: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] //数据集合
  @State random: number = 0 //用于刷新的随机数
  @State lineFps: number = 0 //数值

  offsetX: number = -1 //悬浮框移动触摸点 X
  offsetY: number = -1 //悬浮框移动触摸点 X
  private floatName: string = 'FPS'
  taskState = 1

  aboutToAppear() {
    globalThis.LineChartCollect = setInterval(() => {
      if (this.data.length >= 22) {
        console.log('GestureEvent--------------shift:' + this.data);
        this.data.shift() //移除第一个元素
      }
      if (globalThis.timerFps == undefined) {
        this.lineFps = 0
        this.data.push(0) //在末尾填充一个元素
      } else {
        this.lineFps = globalThis.timerFps
        if (this.lineFps == 0) {
          this.data.push(0)
        } else {
          let lineCount: number = this.lineFps / 1.5
          this.data.push(lineCount)//在末尾填充一个元素
        }
      }

    }, 1000)
  }

  build() {
    Stack({ alignContent: Alignment.Top }) {
      FloatWindowComponent({ title: `sp_FPS`, data: this.data })
      Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
        Text(this.floatName + ':' )
          .fontSize('10fp')
          .fontColor($r('app.color.color_fff'))
          .margin({ left: 5, top: 1 }) //文本显示
        Text( this.lineFps + 'fps')
          .fontSize('20fp')
          .fontColor('#FF0000')
          .fontWeight(5)
          .margin({ left: 1, top: 1 }) //文本显示
        Text(this.random + '')
          .fontSize('1fp')
          .fontColor($r('app.color.color_fff')).visibility(Visibility.None)
        Image($r('app.media.icon_close_small')).width('15vp').height('15vp').onClick(() => {
          hideFloatWindow('sp_FPS')
          console.log('hideFloatWindow---------------------' + this.floatName)
        }).align(Alignment.TopEnd)
      }.height('25vp').width('100%')

    }

  }
}