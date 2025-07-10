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
import router from '@system.router';
import database from '../../database/DatabaseUtils';
import { secToTime } from '../../utils/TimeUtils';
import { ReportItem } from '../../entity/LocalConfigEntity';
import { TopComponent } from '../main/TopComponent';
import promptAction from '@ohos.promptAction';

@Component
@Preview
export struct Report {
  @State private reportItem: Array<ReportItem> = globalThis.reportList

  build() {
    Column() {
      Row() {
        TopComponent({ title: '报告' })
      }.backgroundColor($r('app.color.colorPrimary'))
      .width('100%')
      .padding({ bottom: '15vp' })

      List({ space: '20vp', initialIndex: 0 }) {
        ForEach(this.reportItem, (item) => {

          ListItem() {

            Flex({
              direction: FlexDirection.Row,
              alignItems: ItemAlign.Start,
              justifyContent: FlexAlign.SpaceBetween
            }) {
              Row() {
                Image(globalThis.iconMap.get(item.packageName))
                  .width('60vp')
                  .height('60vp')
                  .padding('10vp')

                Flex({
                  direction: FlexDirection.Column,
                  alignItems: ItemAlign.Start,
                  justifyContent: FlexAlign.SpaceBetween
                }) {
                  Text(`${item.name}`).fontSize('15fp')

                  Text(`${item.appName}`).fontSize('15fp').backgroundColor(0xFFFFFF)

                  Text(`${item.startTime}`).fontSize('15fp').backgroundColor(0xFFFFFF)

                  Text(secToTime(item.testDuration).toString()).fontSize('15fp').backgroundColor(0xFFFFFF)
                }

                Image($r('app.media.report_upload'))
                  .width('40vp')
                  .height('40vp')
                  .padding('10vp')
                  .margin({ left: 800 })
                  .onClick(function (item) {
                    try {
                      promptAction.showToast({ message: '报告上传功能正在开发中', duration: 1000 })
                    } catch (error) {
                      console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
                    }
                  })

              }.margin({ left: '5%', top: '2%', bottom: '2%', right: '2%' }).onClick(() => {
                let databasePath: string = item.dbPath

                //截取时间戳
                let timeStamp = databasePath.substring(databasePath.lastIndexOf('/') + 1, databasePath.length)

                database.queryData(timeStamp + '.db').then(data => {
                  if (item.testDuration < 5) {
                    try {
                      promptAction.showToast({ message: '测试时长过短', duration: 1000 })
                    } catch (error) {
                      console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
                    }
                  } else {
                    router.push({ uri: 'pages/ReportDetail', params: {
                      gpData: data,
                      reportItem: item,
                      timeStamp:timeStamp
                    } })
                  }

                })
              })
            }
          }
        }, item => item.startTime.toString())
      }
      .edgeEffect(EdgeEffect.None) // 滑动到边缘无效果
      .chainAnimation(false) // 联动特效关闭
      .listDirection(Axis.Vertical) // 排列方向
      .divider({ strokeWidth: '2vp', color: 0xdddddd, startMargin: '10vp', endMargin: '12vp' }) // 每行之间的分界线

    }.height('88%')
  }
}