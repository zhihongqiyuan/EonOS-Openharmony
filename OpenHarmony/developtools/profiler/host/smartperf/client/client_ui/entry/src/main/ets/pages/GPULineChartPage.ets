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
import { hideFloatWindow } from '../common/ui/floatwindow/utils/FloatWindowUtils';
import { FloatWindowComponent } from '../common/FloatWindowComponent';
import { TaskStatus } from '../common/profiler/base/ProfilerConstant';

@Entry
@Component
struct GPULineChartPage {
  data: number[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0] //数据集合
  @State random: number = 0 //用于刷新的随机数
  @State gpuFrequency: number = 0 //数值

  offsetX: number = -1 //悬浮框移动触摸点 X
  offsetY: number = -1 //悬浮框移动触摸点 X
  private floatName: string = 'gpu频点'
  taskState = 1

  aboutToAppear() {
    globalThis.LineChartCollect = setInterval(() => {
      this.taskState = globalThis.task_status
      if (this.taskState == TaskStatus.task_running) {

        if (this.data.length >= 22) {

          this.data.shift() //移除第一个元素
        }
        if (globalThis.gpuFrequency == undefined) {
          this.gpuFrequency = 0
        } else {
          this.gpuFrequency = Number((globalThis.gpuFrequency / 1e6).toFixed(2)).valueOf()

          if (this.gpuFrequency == 0) {
            this.data.push(0)
          } else {
            let lineCount: number = this.gpuFrequency / 10
            this.data.push(Math.abs(lineCount)) //在末尾填充一个元素
          }
          this.random = Math.random()
        }
      } else {
        //暂停
      }
    }, 1000)
  }

  build() {
    Stack({ alignContent: Alignment.Top }) {
      FloatWindowComponent({ title: `sp_gpuFrequency`, data: this.data })
      Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {

        Text(this.floatName + ':' )
          .fontSize('10fp')
          .fontColor($r('app.color.color_fff'))
          .margin({ left: 5, top: 1 }) //文本显示
        Text( this.gpuFrequency + 'MHz')
          .fontSize('20fp')
          .fontColor('#FF0000')
          .fontWeight(5)
          .margin({ left: 1, top: 1 }) //文本显示
        Text(this.random + '')
          .fontSize('1fp')
          .fontColor($r('app.color.color_fff')).visibility(Visibility.None)
        Image($r('app.media.icon_close_small')).width('20vp').height('20vp').onClick(() => {
          hideFloatWindow('sp_gpuFrequency')


        }).align(Alignment.TopEnd)
      }.height('20vp').width('100%')

    }
  }
}