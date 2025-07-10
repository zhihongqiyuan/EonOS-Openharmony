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
import { secToTime } from '../common/utils/TimeUtils';
import { TaskStatus } from '../common/profiler/base/ProfilerConstant';
import { ProfilerTask } from '../common/profiler/ProfilerTask';
import { destoryAllFloatWindow } from '../common/ui/floatwindow/utils/FloatWindowUtils';
import WorkerHandler from '../common/profiler/WorkerHandler';
import worker from '@ohos.worker';
let mainWorker = globalThis.MainWorker
import { CollectorType } from '../common/profiler/base/ProfilerConstant'

mainWorker.onmessage = function (result) {
  WorkerHandler.socketHandler(result)
}
@Entry
@Component
struct FloatBall {
  @State playerState: number = TaskStatus.task_init
  @State timerNum: number = 0
  timerId: number = -1
  @State windShowState: boolean = false
  offsetX: number = -1
  offsetY: number = -1
  //解決手势失效的问题
  COUNTS = 2 // 点击次数
  DURATION: number = 300 // 规定有效时间
  mHits = Array(this.COUNTS) // 数组
  isDoubleFlag = false // 是否是双击
  singleClickId = 0 // 单击事件ID

  aboutToAppear() {
    ProfilerTask.getInstance().initModule()
    ProfilerTask.getInstance().taskInit()
    console.log('cm-floatBall-CreateTitleWindow1')
    globalThis.CreateTitleWindow()
    console.log('cm-floatBall-CreateTitleWindow2')
    globalThis.task_status = TaskStatus.task_init
    console.log('cm-floatBall-CreateTitleWindow3')
  }

  initAllCollect() {
    console.log('collectIntervalCollect initAllCollect....');
    if (globalThis.collectConfigs != -1 && globalThis.collectPkg != -1) {
      if (globalThis.collectConfigs.screenCapture) {
        mainWorker.postMessage({'screenCapture':true})
      }
      if (globalThis.collectConfigs.trace) {
        mainWorker.postMessage({'catchTraceStart':true})
      }
      globalThis.collectIntervalCollect = setInterval(() => {
        if (this.playerState == TaskStatus.task_running) {
          ProfilerTask.getInstance().taskStart()
          this.timerNum++
        }
      }, 1000)

      globalThis.collectPowerCollect = setInterval(() => {
        if (this.playerState == TaskStatus.task_running) {
          ProfilerTask.getInstance().taskSingleItemStart(CollectorType.TYPE_POWER)
        }
      }, 250)
    }
    globalThis.task_status = TaskStatus.task_running
    this.playerState = TaskStatus.task_running
    console.log('collectIntervalCollect initAllCollect finished....');
  }

  singleEvent() {
    console.log('cm-floatBall-singleEvent')
    if (this.playerState == TaskStatus.task_running) {
      globalThis.task_status = TaskStatus.task_pause
      this.playerState = TaskStatus.task_pause
    } else if (this.playerState == TaskStatus.task_pause) {
      globalThis.task_status = TaskStatus.task_running
      this.playerState = TaskStatus.task_running
    }
  }

  doubleEvent() {
    console.log('cm-floatBall-doubleEvent' + this.windShowState)
    // 双击启动悬浮TITLE
    if (this.windShowState) {
      globalThis.HideTitleWindow()
      this.windShowState = false
    } else {
      globalThis.ShowTitleWindow()
      this.windShowState = true
    }
  }

  longEvent() {
    console.log('cm-floatBall-longEvent')
    this.playerState = TaskStatus.task_stop
    ProfilerTask.getInstance().taskStop()
    setTimeout(() => {
      this.destroyAllWindow()
      this.clearAllInterVal()
      ProfilerTask.getInstance().taskGetDubai()
    }, 5000)
  }
  async dubai_data_to_disk() {
    mainWorker.postMessage({'setDuBaiDb': true})
  }
  destroyAllWindow() {
    console.log('cm-floatBall-destroyAllWindow')
    globalThis.DestroyFloatingWindow()
    globalThis.DestroyTitleWindow()
    destoryAllFloatWindow()
  }

  clearAllInterVal() {
    console.log('cm-floatBall-clearAllInterVal')
    if (globalThis.collectConfigs.trace) {
      mainWorker.postMessage({'catchTraceEnd':true})
    }
    clearInterval(globalThis.collectIntervalCollect)
    clearInterval(globalThis.collectPowerCollect)
  }

  MoveWindow(offsetX: number, offsetY: number) {
    globalThis.MoveFloatingWindow(offsetX, offsetY)
  }

  SetWindowPosition(offsetX: number, offsetY: number) {
    globalThis.SetFloatingWindowPosition(offsetX, offsetY)
  }

  build() {
    Stack({ alignContent: Alignment.Center }) {
      if (this.playerState == TaskStatus.task_init) {
        Circle()
          .width('90vp')
          .height('90vp')
          .fill(Color.White)
          .fillOpacity(0)
          .opacity(0.8)
          .border({ radius: '90vp', width: '0.5vp', color: $r('app.color.colorPrimary') })
          .linearGradient({
            angle: 135,
            direction: GradientDirection.Left,
            colors: [[$r('app.color.colorPrimary'), 1.0], [$r('app.color.colorPrimary'), 1.0]]
          })
        Text('start')
          .fontSize(18)
          .textAlign(TextAlign.Center)
          .fontColor($r('app.color.color_fff'))
          .width('100%')
          .height('100%')
          .onClick(() => {
            console.log('collectIntervalCollect  single click ....');
            this.dubai_data_to_disk()
            this.initAllCollect()

            console.log('collectIntervalCollect  single click finished....');
          })
          .gesture(
          GestureGroup(GestureMode.Exclusive,
          TapGesture({ count: 2 })
            .onAction(() => {
              this.doubleEvent()

            }),
          PanGesture({})
            .onActionStart(() => {
            })
            .onActionUpdate((event: GestureEvent) => {
              this.offsetX = event.offsetX
              this.offsetY = event.offsetY
            })
            .onActionEnd(() => {
              this.MoveWindow(this.offsetX, this.offsetY)
              this.SetWindowPosition(this.offsetX, this.offsetY)
            })
          ))
      }

      if (this.playerState == TaskStatus.task_running || this.playerState == TaskStatus.task_pause) {
        if (this.playerState == TaskStatus.task_pause) {
          Circle()
            .width('90vp')
            .height('90vp')
            .fill(Color.White)
            .fillOpacity(0)
            .opacity(0.8)
            .border({ radius: '90vp', width: '0.5vp', color: $r('app.color.color_666') })
            .linearGradient({
              angle: 135,
              direction: GradientDirection.Left,
              colors: [[$r('app.color.color_666'), 0.7], [$r('app.color.color_666'), 0.7]]
            })
        } else {
          Circle()
            .width('90vp')
            .height('90vp')
            .fill(Color.White)
            .fillOpacity(0)
            .opacity(0.5)
            .border({ radius: '90vp', width: '0.5vp', color: $r('app.color.colorPrimary') })
            .linearGradient({
              angle: 135,
              direction: GradientDirection.Left,
              colors: [[$r('app.color.colorPrimary'), 0.7], [$r('app.color.colorPrimary'), 0.7]]
            })
        }
        Text(secToTime(this.timerNum).toString())
          .fontSize('16fp')
          .textAlign(TextAlign.Center)
          .fontColor($r('app.color.color_fff'))
          .width('100%')
          .height('100%')
          .onClick(res => {
            this.isDoubleFlag = false
            for (let i = 0; i < this.mHits.length - 1; i++) {
              this.mHits[i] = this.mHits[i + 1]
            }
            this.mHits[this.mHits.length - 1] = new Date().getTime()
            if (this.mHits[0] >= new Date().getTime() - this.DURATION) {
              this.doubleEvent()
              this.isDoubleFlag = true
              this.mHits = Array(this.COUNTS)
            } else {
              this.singleClickId = setTimeout(()=>{
                if (!this.isDoubleFlag) {
                  this.singleEvent()
                }
              }, 300)
            }
          })
          .gesture(
          GestureGroup(GestureMode.Exclusive,
          LongPressGesture({ fingers: 1, repeat: false, duration: 1000 })
            .onAction(() => {
              this.dubai_data_to_disk()
              this.longEvent()

            }),
          PanGesture({})
            .onActionStart(() => {
            })
            .onActionUpdate((event: GestureEvent) => {
              this.offsetX = event.offsetX
              this.offsetY = event.offsetY
            })
            .onActionEnd(() => {
              this.MoveWindow(this.offsetX, this.offsetY)
              this.SetWindowPosition(this.offsetX, this.offsetY)
            })
          ))
      }
      if (this.playerState == TaskStatus.task_stop) {
        Circle()
          .width('90vp')
          .height('90vp')
          .fill(Color.White)
          .fillOpacity(0)
          .opacity(0.8)
          .border({ radius: '90vp', width: '0.5vp', color: $r('app.color.colorPrimary') })
          .linearGradient({
            angle: 135,
            direction: GradientDirection.Left,
            colors: [[$r('app.color.colorPrimary'), 1.0], [$r('app.color.colorPrimary'), 1.0]]
          })
        Text('saving..')
          .fontSize(12)
          .textAlign(TextAlign.Center)
          .fontColor($r('app.color.color_fff'))
          .width('100%')
          .height('100%')
      }
    }.width('100%').height('100%')

  }
}