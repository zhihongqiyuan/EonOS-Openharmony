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
import { TIndexInfo } from '../common/entity/DatabaseEntity';
import { initFloatWindow, showFloatWindow, hideFloatWindow } from '../common/ui/floatwindow/utils/FloatWindowUtils';
import { getCpuCoreInfo} from '../common/utils/SystemUtils';
import FloatWindowConstant from '../common/ui/floatwindow/FloatWindowConstant';
import CommonEvent from '@ohos.commonEvent';


@Component
export struct ItemContent {
  private icon
  private tittle: string
  @State value: string = '-1'
  private onClickCallBack: () => void

  build() {
    Row() {
      Image(this.icon).width(16).height(16).margin({ right: '2%' })
      Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
        Text(this.tittle).fontSize(10).fontColor(Color.White)
        Text(this.value).fontSize(10).fontColor(Color.White)
      }.width('88%').height(20)
    }
    .height(22)
    .width('88%')
    .onClick(() => {
      this.onClickCallBack()
    })
  }
}

@Entry
@Component
struct TitleWindowPage {
  private data: boolean[] = [false, false, false, false, false, false, false, false]
  @State tIndexInfo: TIndexInfo = new TIndexInfo()
  offsetX: number = -1
  offsetY: number = -1
  cpuCoreArr: Array<Number>
  @State isInitFloatWindow: boolean = false
  aboutToAppear() {

    this.cpuCoreArr = getCpuCoreInfo().map(Number).sort()

    let that = this
    var subscriber
    //订阅者信息
    var subscribeInfo = {
      events: ['event']
    };
    //订阅公共事件回调
    function subscribeCallBack(err, data) {
      if (data.data == '') {
      } else {
        console.error('subscriberCurData:' + data.data);
        that.tIndexInfo = JSON.parse(data.data)
        globalThis.cpu0Frequency = that.tIndexInfo.cpu0Frequency
        globalThis.cpu1Frequency = that.tIndexInfo.cpu1Frequency
        globalThis.cpu2Frequency = that.tIndexInfo.cpu2Frequency

        if (that.tIndexInfo.cpu4Frequency != undefined && that.tIndexInfo.cpu7Frequency != undefined) {
          globalThis.cpu1Frequency = that.tIndexInfo.cpu4Frequency
          globalThis.cpu2Frequency = that.tIndexInfo.cpu7Frequency
          that.tIndexInfo.cpu1Frequency = that.tIndexInfo.cpu4Frequency
          that.tIndexInfo.cpu2Frequency = that.tIndexInfo.cpu7Frequency
          that.tIndexInfo.cpu1Load = that.tIndexInfo.cpu1Load
          that.tIndexInfo.cpu2Load = that.tIndexInfo.cpu2Load
        }

        globalThis.currentNow = that.tIndexInfo.currentNow
        globalThis.ddrFrequency = that.tIndexInfo.ddrFrequency
        globalThis.lineFps = that.tIndexInfo.fps
        globalThis.gpuFrequency = that.tIndexInfo.gpuFrequency
        globalThis.pss = that.tIndexInfo.pss
        globalThis.shellBackTemp = that.tIndexInfo.shellFrameTemp

      }
    }
    //创建订阅者回调
    function createSubscriberCallBack(err, data) {
      subscriber = data;
      //订阅公共事件
      CommonEvent.subscribe(subscriber, subscribeCallBack);
    }
    //创建订阅者
    CommonEvent.createSubscriber(subscribeInfo, createSubscriberCallBack);
  }

  MoveWindow(offsetX: number, offsetY: number) {
    globalThis.MoveTitleWindow(offsetX, offsetY)
  }

  SetWindowPosition(offsetX: number, offsetY: number) {
    globalThis.SetTitleWindowPosition(offsetX, offsetY)
  }

  floatWindowEvent(floatName: string, flag: number) {
    if (!this.isInitFloatWindow) {
      initFloatWindow()
      this.isInitFloatWindow = true
    }
    if (this.data[flag]) {
      hideFloatWindow(floatName)
      this.data[flag] = false
    } else {
      showFloatWindow(floatName)
      this.data[flag] = true
    }
  }

  build() {
    Stack({ alignContent: Alignment.Center }) {
      Rect({ width: '100%', height: '100%' }).radius(20).opacity(0.4)
      Column({ space: 2 }) {

        Row() {
          Image($r('app.media.logo')).width(10).height(10).margin({ left: '2%' })
          Text('SmartPerf')
            .fontSize(12)
            .fontColor($r('app.color.color_fff')).margin({ left: '2%' })
          Image($r('app.media.icon_close_small')).height(15).width(15).margin({ left: '45%' }).onClick(() => {
            //关闭实时悬浮框
            globalThis.HideTitleWindow()
          })
        }.height(20)
        .width('90%')


        if (this.tIndexInfo.fps != undefined) {
          ItemContent({
            icon: $r('app.media.icon_average_frame_b'),
            value: (this.tIndexInfo.fps.toString()) + 'FPS',
            tittle: '帧率',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_FPS', FloatWindowConstant.FPS)
            }
          })
        }

        if (this.tIndexInfo.currentNow != undefined) {
          ItemContent({
            icon: $r('app.media.icon_normalized_current'),
            value:  (0 - Number(this.tIndexInfo.currentNow)).toString() + 'mA',
            tittle: '电流',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_currentNow', FloatWindowConstant.CURRENT_NOW)
            }
          })
        }
        if (this.tIndexInfo.ddrFrequency != undefined) {
          ItemContent({
            icon: $r('app.media.icon_counter'),
            value: (parseInt(this.tIndexInfo.ddrFrequency.toString()) / 1e6).toString() + 'MHz',
            tittle: 'DDR频率',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_ddrFrequency', FloatWindowConstant.DDR_FREQUENCY)
            }
          })
        }

        if (this.tIndexInfo['cpu' + this.cpuCoreArr[0] + 'Frequency'] != undefined) {
          ItemContent({
            icon: $r('app.media.icon_counter'),
            value: (parseInt(this.tIndexInfo['cpu' + this.cpuCoreArr[0] + 'Frequency'].toString()) / 1e3).toString() + 'MHz' + this.tIndexInfo.cpu0Load + '%',
            tittle: 'CPU-A频率',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_cpu0Frequency', FloatWindowConstant.CPU0_FREQUENCY)
            }
          })
        }

        if (this.tIndexInfo['cpu' + this.cpuCoreArr[1] + 'Frequency'] != undefined) {
          ItemContent({
            icon: $r('app.media.icon_counter'),
            value: (parseInt(this.tIndexInfo['cpu' + this.cpuCoreArr[1] + 'Frequency'].toString()) / 1e3).toString() + 'MHz' + this.tIndexInfo.cpu1Load + '%',
            tittle: 'CPU-B频率',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_cpu1Frequency', FloatWindowConstant.CPU1_FREQUENCY)
            }
          })
        }
        if (this.tIndexInfo['cpu' + this.cpuCoreArr[2] + 'Frequency'] != undefined) {

          ItemContent({
            icon: $r('app.media.icon_counter'),
            value: (parseInt(this.tIndexInfo['cpu' + this.cpuCoreArr[2] + 'Frequency'].toString()) / 1e3).toString() + 'MHz' + this.tIndexInfo.cpu2Load + '%',
            tittle: 'CPU-C频率',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_cpu2Frequency', FloatWindowConstant.CPU2_FREQUENCY)
            }
          })
        }

        if (this.tIndexInfo.gpuFrequency != undefined) {
          ItemContent({
            icon: $r('app.media.icon_frame_score'),
            value: (parseInt(this.tIndexInfo.gpuFrequency.toString()) / 1e6).toString() + 'MHz' + this.tIndexInfo.gpuLoad + '%',
            tittle: 'GPU频点',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_gpuFrequency', FloatWindowConstant.GPU_FREQUENCY)
            }
          })
        }
        if (this.tIndexInfo.pss != undefined) {
          ItemContent({
            icon: $r('app.media.icon_jank_each_hour'),
            value: this.tIndexInfo.pss + 'KB',
            tittle: 'RAM',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_RAM', FloatWindowConstant.RAM)
            }
          })
        }
        if (this.tIndexInfo.socThermalTemp != undefined) {
          ItemContent({
            icon: $r('app.media.icon_max_temperature'),
            value: (parseInt(this.tIndexInfo.socThermalTemp.toString()) / 1e3).toString() + '℃',
            tittle: 'SOC温度',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_shellBackTemp', FloatWindowConstant.SHELL_BACK_TEMP)
            }
          })
        }

        if (this.tIndexInfo.shellFrameTemp != undefined) {
          ItemContent({
            icon: $r('app.media.icon_max_temperature'),
            value: (parseInt(this.tIndexInfo.shellFrameTemp.toString()) / 1e3).toString() + '℃',
            tittle: '壳温',
            onClickCallBack: () => {
              this.floatWindowEvent('sp_shellBackTemp', FloatWindowConstant.SHELL_BACK_TEMP)
            }
          })
        }

      }.width('100%')
      .gesture(
      GestureGroup(GestureMode.Exclusive,
      PanGesture({})
        .onActionStart((event: GestureEvent) => {
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
  }
}