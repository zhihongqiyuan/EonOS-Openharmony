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
import deviceInfo from '@ohos.deviceInfo';
import router from '@system.router';
import { TopComponent } from '../main/TopComponent';
import { OtherSupport, TestMode } from '../../entity/LocalConfigEntity';
import SPLogger from '../../utils/SPLogger'
import batteryinfo from '@ohos.batteryInfo';
import promptAction from '@ohos.promptAction';

const TAG = 'Home'
/*
  home页
 */

@Component
export struct Home {
  @State private deviceStr: string = ''
  @State private versionStr: string = ''
  @State private circleHeight: string = '180vp'
  @State private circleWidth: string = '180vp'
  @State private circleRadius: string = '90vp'
  @State atOpacity: number = 0.6

  @State state: string = '电池状态'

  aboutToAppear() {

    globalThis.collectIntervalPower = setInterval(() => {
      let nowCurrent = batteryinfo.nowCurrent
      let remainingEnergy = batteryinfo.remainingEnergy
      let voltage = Math.round(batteryinfo.voltage / 1000)
      let temperature = batteryinfo.batteryTemperature

      this.state = '剩余电量：' + remainingEnergy.toString() + 'mAh     瞬时电流：' +
      nowCurrent.toString() + 'mA     \n瞬时电压：' + voltage.toString() + 'mV       电池温度：' + (temperature / 10).toFixed(1).toString() + '℃'
    }, 1000)


  }

  build() {
    Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Start, justifyContent: FlexAlign.Start }) {
      Column() {
        Column() {
          TopComponent({ title: 'SmartPerf ' })

          Text('性能/功耗测试')
            .fontSize('18fp')
            .fontColor($r('app.color.color_fff'))
            .textAlign(TextAlign.Start)
            .margin({ top: '20vp' })

          Stack() {
            Circle()
              .width('180vp')
              .height('180vp')
              .fill(Color.White)
              .fillOpacity(0)
              .border({ radius: '90vp', width: '0.5vp', color: $r('app.color.color_fff') })
              .linearGradient({
                angle: 135,
                direction: GradientDirection.Left,
                colors: [[$r('app.color.color_80B3193F'), 0.0], [$r('app.color.color_80fff'), 1.0]]
              })

            Text('开始测试')
              .fontColor($r('app.color.color_fff'))
              .fontSize('20fp')

            Column() {
            }.width('220vp')
            .height('220vp')

            Flex({ justifyContent: FlexAlign.Center, alignItems: ItemAlign.Center }) {
            }
            .width(this.circleWidth)
            .height(this.circleHeight)
            .border({ width: '1vp', radius: this.circleRadius, color: $r('app.color.color_80fff') })
            .opacity(this.atOpacity)
            .animation({
              duration: 1000,
              iterations: -1,
              curve: Curve.FastOutLinearIn })
            .onAppear(() => {
              this.circleWidth = '220vp'
              this.circleHeight = '220vp'
              this.circleRadius = '100vp'
              this.atOpacity = 0
            })


          }.onClick(() => {
            if (!globalThis.showFloatingWindow) {
              router.push({ uri: 'pages/StartTestPage' })
            } else {
              try {
                promptAction.showToast({ message: '已经有应用正在测试，请关闭测试再进行操作', duration: 1000 })
              } catch (error) {
                console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
              }
            }
          })
          Text(this.state.toString()).fontSize(12).height('5%').fontColor($r('app.color.color_fff'))
          Text(this.deviceStr)
            .fontSize('25fp')
            .fontColor($r('app.color.color_fff'))
            .fontWeight(FontWeight.Bold)
            .textAlign(TextAlign.Start)
            .margin({ top: '20vp', bottom: '40vp' })
        }.backgroundColor($r('app.color.colorPrimary')).width('100%')
        .alignItems(HorizontalAlign.Center)
        .onAppear(() => {
          SPLogger.DEBUG(TAG, 'deviceInfo.brand:->' + deviceInfo.brand + 'deviceInfo.productModel:->' + deviceInfo.productModel )
          this.deviceStr = deviceInfo.brand + ' ' + deviceInfo.productModel
        })

        GridExample()
      }
    }.width('100%').height('100%')
  }
}

@Component
struct GridExample {
  @State support: Array<OtherSupport> = new Array(
    new OtherSupport('亮度调节', '调节屏幕亮度', TestMode.BRIGHTNESS, $r('app.media.icon_brightness_plus'))
  )


  build() {
    Column({ space: 5 }) {
      Grid() {
        ForEach(this.support, (otherSupport) => {
          GridItem() {
            Column() {
              Image(otherSupport.resource).width(60)
                .height(60)
              Text(otherSupport.testName)
                .fontSize('13fp')
                .fontColor($r('app.color.color_333'))
                .height('30%')
                .textAlign(TextAlign.Center)

              Text(otherSupport.testSrc)
                .fontSize('10fp')
                .fontColor($r('app.color.color_999'))
                .height('30%')
                .textAlign(TextAlign.Center)
            }
          }
          .backgroundColor($r('app.color.color_fff'))
          .width('100%')
          .margin({ top: '10vp' })
          .padding({ top: '8vp', right: '20vp', bottom: '8vp', left: '20vp' })
          .onClick(() => {
            if (otherSupport.testMode == TestMode.ONLINE) {
              try {
                promptAction.showToast({ message: '开发中' })
              } catch (error) {
                console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
              }
            } else if (otherSupport.testMode == TestMode.BRIGHTNESS) {
              router.push({ uri: 'pages/LightAdjust' })
            }
          })
        }, otherSupport => otherSupport.testName)
      }
      .columnsTemplate('1fr 1fr 1fr')
      .rowsTemplate('1fr 1fr 1fr')
      .width('100%')
      .height(450)
    }.width('100%').height('100%').backgroundColor('#EEEEEE')
  }
}
