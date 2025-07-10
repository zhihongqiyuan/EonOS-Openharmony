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
import { TPowerSensorInfo, TPowerAppInfo } from '../../../common/entity/DatabaseEntity';
import database from '../../../common/database/DatabaseUtils';
import SPLogger from '../../utils/SPLogger'
@Component
@Preview
export struct PowerDubai {
  @State tPowerSensorList: TPowerSensorInfo[] = []
  @State tPowerAppInfoList: TPowerAppInfo[] = []
  aboutToAppear() {
    SPLogger.ERROR('TAG', 'resultSet query_applications_display-----display11111--' + JSON.stringify(this.tPowerSensorList))
    SPLogger.ERROR('TAG', 'resultSet query_applications_display-----display22222--' + JSON.stringify(this.tPowerAppInfoList[0]))
    let sumPower:number = 0
    let sumCurrent:number = 0

    for (var i = 0; i < this.tPowerSensorList.length; i++) {

      if (this.tPowerSensorList[i].power == '' || this.tPowerSensorList[i].power == null) {
        this.tPowerSensorList[i].setPower('0')
        this.tPowerSensorList[i].setCurrent('0')
        this.tPowerSensorList[i].setPerenct('0.00%')
      } else {
        sumPower += Number(this.tPowerSensorList[i].power.toString())
        sumCurrent += Number(this.tPowerSensorList[i].current.toString())
      }
    }
    for (var i = 0; i < this.tPowerSensorList.length; i++) {
      this.tPowerSensorList[i].setPerenct((Number(this.tPowerSensorList[i].power) * 100 / sumPower).toFixed(2) + '%')
    }
    this.tPowerSensorList.sort((a, b) => parseInt(a.current) - parseInt(b.current));
    this.tPowerSensorList.push(new TPowerSensorInfo('', 'Toal', sumPower.toFixed(5) + '', sumCurrent.toFixed(5) + '', '100%', $r('app.color.color_fff')))
    this.tPowerSensorList.unshift(new TPowerSensorInfo('', 'Sensor', 'Power(mAh)', 'Current(mA)', 'Percent(%)', $r('app.color.colorPrimary')))

    this.tPowerAppInfoList.forEach(it=>{
      it.setColor($r('app.color.color_fff'))
    })
    this.tPowerAppInfoList.unshift(new TPowerAppInfo('', '', 'ProcessName', 'Power(mAh)', 'Current(mA)', 'Percent', $r('app.color.colorPrimary')))

  }

  build() {
    Scroll(){
      Column(){
        Column() {

          Text('功耗拆解知识参考项，测试应用功耗请使用归一化电流/整机电流（报告概览页）') {
          }.fontWeight(FontWeight.Bold).fontColor($r('app.color.color_333')).fontSize('12fp').textAlign(TextAlign.Center)

          List() {
            ForEach(this.tPowerSensorList, (powerSensor) => {
              ListItem() {
                Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
                  Text(powerSensor.sensor)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor( powerSensor.color)
                    .textAlign(TextAlign.Center)
                  Text(powerSensor.current)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor(powerSensor.color)
                    .textAlign(TextAlign.Center)
                  Text(powerSensor.percent)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor(powerSensor.color)
                    .textAlign(TextAlign.Center)
                }.width('100%')
              }
            }, powerSensor => powerSensor.sensor)
          }.width('100%')
        }.alignItems(HorizontalAlign.Center)

        Column() {
          Text('Top20进程功耗') {
          }.fontWeight(FontWeight.Bold).fontColor($r('app.color.color_333')).fontSize('12fp').textAlign(TextAlign.Center)


          List() {
            ForEach(this.tPowerAppInfoList, (powerApp) => {
              ListItem() {
                Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
                  Text(powerApp.application)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor(powerApp.color)
                    .textAlign(TextAlign.Center)
                  Text(powerApp.current)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor(powerApp.color)
                    .textAlign(TextAlign.Center)

                  Text(powerApp.percent)
                    .fontSize('12fp')
                    .fontColor($r('app.color.color_333'))
                    .height('30vp')
                    .width('20%')
                    .border({ width: '1vp', color: $r('app.color.color_999') })
                    .backgroundColor(powerApp.color)
                    .textAlign(TextAlign.Center)

                }.width('100%')
              }
            }, powerApp => powerApp.application)
          }.width('100%')

        }.alignItems(HorizontalAlign.Center)
      }
    }.scrollable(ScrollDirection.Vertical).scrollBar(BarState.Auto).height('60%')


  }

}