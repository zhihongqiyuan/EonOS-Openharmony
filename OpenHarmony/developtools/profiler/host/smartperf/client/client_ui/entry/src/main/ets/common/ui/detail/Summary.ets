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
import { TIndexInfo } from '../../entity/DatabaseEntity';
import { SummaryItem } from '../../entity/LocalConfigEntity';
import { getCpuCoreInfo, } from '../../utils/SystemUtils';
import CalculationUtils from '../../utils/CalculationUtils';
import SPLogger from '../../utils/SPLogger'

const TAG = 'SummaryTAG'
/*
 * 报告详情概览页
 */
@Component
@Preview
export struct Summary {
  @State private gpData: TIndexInfo[] = []
  @State private summaryItems: SummaryItem[] = []

  aboutToAppear() {

    var cpuCoreArr = getCpuCoreInfo().map(Number).sort()
    console.error(TAG + 'cpuCoreArr:' + JSON.stringify(cpuCoreArr))
    console.error(TAG + 'TIndexInfo:' + new TIndexInfo)
    console.error(TAG + 'JSON TIndexInfo:' + JSON.stringify(new TIndexInfo))
    var cpu0FreqSum: number = 0
    var cpu1FreqSum: number = 0
    var cpu2FreqSum: number = 0
    var cpu3FreqSum: number = 0
    var cpu4FreqSum: number = 0
    var cpu5FreqSum: number = 0
    var cpu6FreqSum: number = 0
    var cpu7FreqSum: number = 0
    var cpuFreqMap = new Map


    var cpuA: number = 0
    var cpuB: number = 0
    var cpuC: number = 0

    var cpuMin: number = 0
    var cpuMid: number = 0
    var cpuMax: number = 0

    var normalCurrentNow: number = 0

    var socThermalTemp: number = 0
    var gpuLoadSum: number = 0
    var gpuFreqSum: number = 0
    var ddrFreqSum: number = 0
    var shellFrameTempSum: number = 0

    // fps和ram 为空时 过滤掉脏数据 0和空
    var fpsNullSum = 0
    var ramNullSum = 0

    var pssSum: number = 0
    var fpsSum: number = 0
    let fpsMax: number = 0
    let fpsList = []
    let fpsJitters = []

    for (var index = 0; index < this.gpData.length; index++) {
      const gpDataCur = this.gpData[index];
      let currentNow = Number(gpDataCur.currentNow).valueOf()
      let curVoltage = Number(gpDataCur.voltageNow).valueOf() / 1e6
      normalCurrentNow += Math.abs(currentNow) * Math.abs(curVoltage) / 3.8
      socThermalTemp += Number(gpDataCur.socThermalTemp).valueOf()
      shellFrameTempSum += Number(gpDataCur.shellFrameTemp).valueOf()
      gpuLoadSum += Number(gpDataCur.gpuLoad).valueOf()
      gpuFreqSum += Number(gpDataCur.gpuFrequency).valueOf()
      ddrFreqSum += Number(gpDataCur.ddrFrequency).valueOf()
      cpu0FreqSum += Number(gpDataCur.cpu0Frequency).valueOf()
      cpu1FreqSum += Number(gpDataCur.cpu1Frequency).valueOf()
      cpu2FreqSum += Number(gpDataCur.cpu2Frequency).valueOf()
      cpu3FreqSum += Number(gpDataCur.cpu3Frequency).valueOf()
      cpu4FreqSum += Number(gpDataCur.cpu4Frequency).valueOf()
      cpu5FreqSum += Number(gpDataCur.cpu5Frequency).valueOf()
      cpu6FreqSum += Number(gpDataCur.cpu6Frequency).valueOf()
      cpu7FreqSum += Number(gpDataCur.cpu7Frequency).valueOf()

      if (gpDataCur.pss == '') {
        ramNullSum++
      }
      if (gpDataCur.fps == '' || gpDataCur.fps == '0') {
        fpsNullSum++
      }

      pssSum += Number(gpDataCur.pss).valueOf()
      let fpxCur = Number(gpDataCur.fps).valueOf()
      fpsSum += fpxCur
      if (fpsMax < fpxCur) {
        fpsMax = fpxCur
      }
      fpsList.push(Number(gpDataCur.fps).valueOf())
      fpsJitters.push(gpDataCur.fpsJitters.toString().replace('\'', ''))
    }

    cpuFreqMap.set('cpu0FreqSum', cpu0FreqSum)
    cpuFreqMap.set('cpu1FreqSum', cpu1FreqSum)
    cpuFreqMap.set('cpu2FreqSum', cpu2FreqSum)
    cpuFreqMap.set('cpu3FreqSum', cpu3FreqSum)
    cpuFreqMap.set('cpu4FreqSum', cpu4FreqSum)
    cpuFreqMap.set('cpu5FreqSum', cpu5FreqSum)
    cpuFreqMap.set('cpu6FreqSum', cpu6FreqSum)
    cpuFreqMap.set('cpu7FreqSum', cpu7FreqSum)

    cpuA = cpuFreqMap.get('cpu' + 0 + 'FreqSum') / 1e3
    cpuB = cpuFreqMap.get('cpu' + 1 + 'FreqSum') / 1e3
    cpuC = cpuFreqMap.get('cpu' + 2 + 'FreqSum') / 1e3

    if (cpuCoreArr.length > 2) {
      cpuA = cpuFreqMap.get('cpu' + cpuCoreArr[0] + 'FreqSum') / 1e3
      cpuB = cpuFreqMap.get('cpu' + cpuCoreArr[1] + 'FreqSum') / 1e3
      cpuC = cpuFreqMap.get('cpu' + cpuCoreArr[2] + 'FreqSum') / 1e3
    }
    let cpuList = [cpuA, cpuB, cpuC].sort()

    cpuMin = cpuList[0]
    cpuMid = cpuList[1]
    cpuMax = cpuList[2]

    let calculationTest = new CalculationUtils(fpsList, CalculationUtils.calculateFPSNew(fpsList))

    if (normalCurrentNow > 0) {
      this.summaryItems.push(
        new SummaryItem($r('app.media.icon_normalized_current'), '归一化电流', (normalCurrentNow / this.gpData.length / 1.1125).toFixed(0) + 'mA', ''),
      )
    }

    if (socThermalTemp > 0) {
      this.summaryItems.push(
        new SummaryItem($r('app.media.icon_max_temperature'), 'soc温度', (socThermalTemp / this.gpData.length / 1000 ).toFixed(0) + '℃', '#fff8f8'),
      )
    }

    if (shellFrameTempSum > 0) {
      this.summaryItems.push(
        new SummaryItem($r('app.media.icon_max_temperature'), '壳温', (shellFrameTempSum / this.gpData.length / 1000 ).toFixed(0) + '℃', '#fff8f8'),
      )
    }

    this.summaryItems.push(
      new SummaryItem($r('app.media.icon_jank_score'), '平均帧率', (fpsSum / (this.gpData.length - fpsNullSum)).toFixed(0) + 'Hz', '#fcf4ee'),
      new SummaryItem($r('app.media.icon_jank_score'), '最高帧率', (fpsMax).toFixed(0) + 'HZ', '#fcf4ee'),
      new SummaryItem($r('app.media.icon_jank_score'), '低帧率', (calculationTest.Low_Frame_Rate()).toFixed(2) + '%', '#fcf4ee'),
      new SummaryItem($r('app.media.icon_jank_score'), '抖动率', (calculationTest.Jitter_rate()).toFixed(2) + '%', '#fcf4ee'),
      new SummaryItem($r('app.media.icon_jank_score'), '卡顿次数', (calculationTest.calculateCaton(fpsJitters)).toFixed(0) + '次', '#fcf4ee'),
      new SummaryItem(
      $r('app.media.icon_frame_score'), 'GPU负载',
        (gpuFreqSum / this.gpData.length / 1e6).toFixed(0) + 'MHZ' + '  ' +
        (gpuLoadSum / this.gpData.length).toFixed(0) + '%', '#fcf9f2'),
      new SummaryItem($r('app.media.icon_frame_score'), 'DDR频率', (ddrFreqSum / this.gpData.length / 1e6).toFixed(0) + 'MHZ', '#fcf9f2'),
      new SummaryItem($r('app.media.icon_average_frame_b'), 'CPU MIN', (cpuMin / this.gpData.length).toFixed(0) + 'MHZ', '#fcf9f2'),
      new SummaryItem($r('app.media.icon_average_frame_b'), 'CPU MID', (cpuMid / this.gpData.length).toFixed(0) + 'MHZ', '#fcf9f2'),
      new SummaryItem($r('app.media.icon_average_frame_b'), 'CPU MAX', (cpuMax / this.gpData.length).toFixed(0) + 'MHZ', '#fcf9f2'),
      new SummaryItem($r('app.media.icon_jank_each_hour'), 'RAM', (pssSum / (this.gpData.length - ramNullSum)).toFixed(0) + 'KB', '#f0faff')
    )
  }

  build() {
    Column() {
      Grid() {
        ForEach(this.summaryItems, item => {
          GridItem() {
            Row({ space: '3vp' }) {
              Image(item.icon).width('25vp').height('25vp')
              Text(item.content).fontSize('12fp').textAlign(TextAlign.Start)
              Text(item.value).fontSize('10fp').textAlign(TextAlign.Start)
            }.alignItems(VerticalAlign.Center).width('100%')
          }
          .width('90%')
          .align(Alignment.Center)
          .backgroundColor(item.backColor)
          .border({ radius: '5vp', color: '#ffffff' }).shadow({radius : 5})
          .margin({top: '20vp'})
          .padding('5vp')
        }, item => item.content)
      }.margin({ bottom: '30%', left: '15%', right: '15%' }).width('90%')
      .columnsTemplate('1fr 1fr')
    }
  }
}