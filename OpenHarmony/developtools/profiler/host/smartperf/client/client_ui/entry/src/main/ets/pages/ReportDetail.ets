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
import { TIndexInfo } from '../common/entity/DatabaseEntity';
import router from '@system.router';
import { Summary } from '../common/ui/detail/Summary';
import { PowerDubai } from '../common/ui/detail/PowerDubai';
import { Performance } from '../common/ui/detail/Performance';
import { Load } from '../common/ui/detail/Load';
import { Temperature } from '../common/ui/detail/Temperature';
import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';
import { ReportItem } from '../common/entity/LocalConfigEntity';
import CheckEmptyUtils from '../common/utils/CheckEmptyUtils';

import { TPowerSensorInfo, TPowerAppInfo } from '../common/entity/DatabaseEntity';
import database from '../common/database/DatabaseUtils';
import SPLogger from '../common/utils/SPLogger'

@Entry
@Component
struct ReportDetail {
  private controller: TabsController = new TabsController()
  private gpData: TIndexInfo[] = []
  private reportItem: ReportItem = null
  private tPowerSensorList: TPowerSensorInfo[] = []
  private tPowerAppList: TPowerAppInfo[] = []
  aboutToAppear() {

    let data:any = router.getParams()['gpData']
    let report:any = router.getParams()['reportItem']
    let timeStamp:any = router.getParams()['timeStamp']
    if (data != null) {
      this.gpData = data
    }
    if (report != null) {
      this.reportItem = report
    }
    globalThis.testDuration = this.reportItem.testDuration

    database.query_powersensor_info(timeStamp).then(data => {
      data.forEach(t=>{
        this.tPowerSensorList.push(t)
      })
    })
    //归一化电流
    database.queryData(timeStamp + '.db').then(data => {
      let normalCurrentNow = data.reduce((pre, cur)=> {
        return pre + Number(cur.currentNow).valueOf()
      }, 0)
      return Math.abs(normalCurrentNow / data.length / 1.1125)
    }).then((normalCurrentNow)=>{
      database.query_powerapp_info(timeStamp).then(data => {
        data.forEach(t=>{
          let current = (Number(t.percent)) * normalCurrentNow / 100
          t.setCurrent(current.toFixed(5))
          t.setPercent(Number(t.percent).toFixed(2) + '%')
          this.tPowerAppList.push(t)
        })
      })
    })

  }

  build() {

    Column() {
      StartTestTitleComponent({ title: '报告详情' })

      Row() {
        Flex({ justifyContent: FlexAlign.SpaceBetween }) {
          Column() {
            Image(globalThis.iconMap.get(this.reportItem.packageName))
              .width('60vp')
              .height('60vp')
              .margin({ top: '10vp', left: '20vp' })
          }.margin({ left: '4%' })


          Column() {
            Text(`SP工具`).fontSize('15fp').margin({ top: '30vp' })
            Text(`应用版本：v1.0.2`).fontSize('15fp').margin({ top: '10vp' })
          }.margin({ right: '4%' })
        }
      }

      Row() {
        Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Start, justifyContent: FlexAlign.Start }) {
          Text(`开始时间:${this.reportItem.startTime}`)
            .fontSize('13fp')
            .fontColor($r('app.color.color_666'))
            .margin({ top: '5vp' })
          Text(`测试时长:${secToTime(Number(this.reportItem.testDuration))}`)
            .fontSize('13fp')
            .fontColor($r('app.color.color_666'))
            .margin({ top: '5vp' })
          Text(`文件路径:${this.reportItem.dbPath}/t_index_info.csv`)
            .fontSize('13fp')
            .fontColor($r('app.color.color_666'))
            .margin({ top: '5vp' })
        }.width('100%').margin({ left: '10vp' }).height('95')
      }.margin({ left: '4%' })

      Row() {
        Flex() {
          Tabs({ barPosition: BarPosition.Start, index: 0, controller: this.controller }) {
            TabContent() {
              Summary({ gpData: this.gpData })
            }.tabBar('概览')

            TabContent() {
              Column() {
                Performance({ gpData: this.gpData })
              }.width('100%').height('100%')
            }.tabBar('性能')

            TabContent() {
              Column() {
                Load({ gpData: this.gpData })
              }.width('100%').height('100%')
            }.tabBar('负载')

            if (!CheckEmptyUtils.checkStrIsEmpty(this.gpData[0].currentNow)) {
              TabContent() {
                Column() {
                  PowerDubai({tPowerSensorList:this.tPowerSensorList, tPowerAppInfoList:this.tPowerAppList})
                }.width('100%').height('100%')
              }.tabBar('功耗')
            }

            TabContent() {
              Column() {
                Temperature({ gpData: this.gpData })
              }.width('100%').height('100%')
            }.tabBar('热')
          }.backgroundColor('#f5f5f5')
          .barWidth(360)
          .scrollable(true)
          .barHeight(60)
          .width('100%')
          .height('100%')
        }
      }
    }
  }
}