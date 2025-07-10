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

import { commonStartAbility } from '../common/utils/AbilityUtils';
import router from '@system.router';
import { SwitchItem, CollectItem } from '../common/entity/LocalConfigEntity';
import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';
import { ProfilerTask } from '../common/profiler/ProfilerTask';
import { CollectorType } from '../common/profiler/base/ProfilerConstant';
import promptAction from '@ohos.promptAction';


const TAG = 'StartTestPage'
/*
 * 测试配置页
 */
@Entry
@Component
struct StartTestPage {
  @State selectApp: string = '请选择一个应用'
  @State selectAppIcon: string = ''
  @State private collectConfigs: CollectItem[] = []
  @State private switchList: SwitchItem[] = [
    new SwitchItem('trace', '是否抓取trace', $r('app.media.icon_average_frame_b'), false, true),
    new SwitchItem('screenCapture', '是否开启截图', $r('app.media.icon_screencap'), false, true)
  ]
  @State private testName: string = '' // 测试名称

  dialogController: CustomDialogController = new CustomDialogController({
    builder: CustomDialogCollect({ cancel: () => {
    }, confirm: () => {
    }, collectConfigs: $collectConfigs }),
    cancel: () => {
    },
    autoCancel: true
  })
  textController: CustomDialogController = new CustomDialogController({
    builder: TextInputDialog({ cancel: () => {
    }, confirm: () => {
    }, testName: $testName }),
    cancel: () => {
    },
    autoCancel: true
  })

  aboutToAppear() {
    let supportMap = ProfilerTask
     .getInstance()
     .getSupports([
        CollectorType.TYPE_CPU.toString(),
        CollectorType.TYPE_GPU.toString(),
        CollectorType.TYPE_DDR.toString(),
        CollectorType.TYPE_FPS.toString(),
        CollectorType.TYPE_POWER.toString(),
        CollectorType.TYPE_TEMPERATURE.toString(),
        CollectorType.TYPE_RAM.toString()
     ])

    var iterator = supportMap.keys()
    for (var i = 0; i < supportMap.size; i++) {
      let key = iterator.next().value
      let val = supportMap.get(key).valueOf()
      this.collectConfigs.push(
        new CollectItem(key, val, val)
      )
    }

  }

  build() {

    Column() {
      //开始测试title
      StartTestTitleComponent({ title: '开始测试' })

      Scroll() {
        Column() {
          //请选择一个应用
          Row({ space: '15vp' }) {
            if (this.selectAppIcon == '') {
              Image($r('app.media.logo')).width('70vp').height('70vp').margin({ left: '2%' })
            } else {
              Image(this.selectAppIcon).width('70vp').height('70vp').margin({ left: '2%' })
            }
            Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
              Text(this.selectApp).fontSize('15fp').fontColor($r('app.color.color_333'))

              Image($r('app.media.icon_enter')).width('15vp').height('15vp').margin({ right: '35vp' })
            }.height('70vp').width('85%')
          }
          .height('100vp')
          .width('95%')
          .borderRadius('5vp')
          .backgroundColor($r('app.color.color_fff'))
          .margin({ top: '20vp' })
          .onClick(() => {
            router.push({ uri: 'pages/AppSelectPage', params: {
              startPage: 'startTest'
            } })
          })

          //测试指标
          Row({ space: '15vp' }) {
            Image($r('app.media.icon_test_index')).width('25vp').height('25vp').margin({ left: '2%' })

            Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
              Text('测试指标').fontSize('15fp').fontColor($r('app.color.color_333'))

              Image($r('app.media.icon_enter')).width('15vp').height('15vp').margin({ right: '15vp' })
            }.height('60vp').width('90%').onClick(() => {
              console.log('TestIndicators---------onClick')
              if (this.selectApp == '请选择一个应用' || this.selectApp == 'SmartPerf') {
                console.log('TestIndicators---------please choose app')
                try {
                  promptAction.showToast({ message: 'please choose app!', duration: 1000 })
                } catch (error) {
                  console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
                }
                return
              }
              console.log('TestIndicators---------dialogController.open()')
              this.dialogController.open()
              console.log('TestIndicators---------dialogController.open  End()')
            })
          }
          .height('60vp')
          .width('95%')
          .borderRadius('5vp')
          .backgroundColor($r('app.color.color_fff'))
          .margin({ top: '10vp' })

          //测试名称
          Row({ space: '15vp' }) {
            Image($r('app.media.icon_test_name')).width('25vp').height('25vp').margin({ left: '2%' })

            Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
              Text('测试名称').fontSize('15fp').fontColor($r('app.color.color_333'))

              Row() {
                Text(this.testName).fontSize('15fp').fontColor($r('app.color.color_333'))

                Image($r('app.media.icon_enter')).width('15vp').height('15vp').margin({ right: '15vp' })
              }
            }.height('60vp').width('90%').onClick(() => {
              this.textController.open()
            })

          }.height('60vp').width('95%').borderRadius('5vp').backgroundColor($r('app.color.color_fff'))

          SwitchComponent({ switchList: $switchList })
          Blank()
          Button('开始测试')
            .fontSize('15fp')
            .fontColor($r('app.color.color_fff'))
            .border({ radius: '20vp' })
            .width('80%')
            .height('60vp')
            .backgroundColor($r('app.color.colorPrimary'))
            .onClick(() => {

              let taskConfig = this.resolveTaskConfig()
              console.log('console.log:' + JSON.stringify(taskConfig));

              if (this.selectApp == '请选择一个应用' || this.selectApp == 'SmartPerf') {
                try {
                  promptAction.showToast({ message: 'please choose app!', duration: 1000 })
                } catch (error) {
                  console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
                }
                return
              }
			if (router != null) {
				if (router.getParams() != null) {
					if (router.getParams()['selectPackageName'] != null) {
					  //启动app
            commonStartAbility(`${router.getParams()['selectPackageName']}`.toString(), `${router.getParams()['selectAbilityName']}`.toString())
					  //启动悬浮窗
					  globalThis.CreateFloatingWindow()
					  router.back({ uri: 'pages/MainPage' })
					 }
			  }
			}
            })

          Divider().height('15%').width('80%').visibility(Visibility.Hidden)
        }.height('100%')
      }.width('100%').scrollable(ScrollDirection.Vertical).scrollBar(BarState.Auto)

    }.height('100%').width('100%').backgroundColor('#EEEEEE')
  }

  onPageShow() {
    let routerParams = router.getParams()
    let appName
    let appVersion
    let selectPackageName
    let appIconId
    if (routerParams != undefined && routerParams != null) {
      appName = routerParams['appName']
      appVersion = routerParams['appVersion']
      selectPackageName = routerParams['selectPackageName']
      appIconId = routerParams['appIconId']
    }
    if (appName == null && appName == undefined) {
      this.selectApp = '请选择一个应用'
      this.testName = ''
      this.selectAppIcon = ''
    } else {
      this.selectApp = `${appName}`.toString()
      globalThis.appName = `${appName}`.toString()
      globalThis.appVersion = `${appVersion}`.toString()
      globalThis.packageName = `${selectPackageName}`.toString()
      this.selectAppIcon = `${appIconId}`.toString()
      let date = new Date()
      let M = (date.getMonth() + 1 < 10 ? '0' + (date.getMonth() + 1) : date.getMonth() + 1);
      let D = date.getDate() + '-';
      let h
      if (date.getHours() < 10) {
        h = '0' + date.getHours();
      } else {
        h = date.getHours();
      }
      let m = date.getMinutes();
      globalThis.testTaskName = '游戏测试' + M + D + h + m
      this.testName = '游戏测试' + M + D + h + m
    }
  }

  resolveTaskConfig() {
    let collects = this.collectConfigs
    let collectStr = ''
    for (var i = 0; i < collects.length; i++) {
      const collect = collects[i];
      if (i != collects.length - 1) {
        collectStr += collect.name + '::' + collect.isSelect + ','
      } else {
        collectStr += collect.name + '::' + collect.isSelect
      }
    }

    let switchs = this.switchList
    let switchStr = ''
    for (var j = 0; j < switchs.length; j++) {
      const st = switchs[j];
      if (j != switchs.length - 1) {
        switchStr += st.id + '::' + st.isOpen + ','
      } else {
        switchStr += st.id + '::' + st.isOpen
      }
    }

    let taskConfig = {
      'selectAppName': globalThis.packageName,
      'allConfigs': collectStr + ',' + switchStr
    }

    let configItems: { [key: string]: boolean } = {}
    let allConfigsArr: string[] = []
    let curSelectPkg = ''
    if (taskConfig !== undefined) {
      allConfigsArr = taskConfig.allConfigs.split(',')
      curSelectPkg = taskConfig.selectAppName
    }
    for (var index = 0; index < allConfigsArr.length; index++) {
      const config = allConfigsArr[index];
      let params = config.split('::')
      if (params[1] == 'true' || params[1] == '1') {
        configItems[params[0]] = true
      } else if (params[1] == 'false' || params[1] == '0') {
        configItems[params[0]] = false
      } else {
        configItems[params[0]] = false
      }
    }
    globalThis.collectConfigs = configItems
    globalThis.collectPkg = curSelectPkg

    return taskConfig
  }

  search(id: string, myArray: Array<SwitchItem>): SwitchItem {
    for (var i = 0; i < myArray.length; i++) {
      if (myArray[i].id === id) {
        return myArray[i];
      }
    }
  }
}


@Component
struct SwitchComponent {
  @Link private switchList:Array<SwitchItem>

  build() {
    Column() {
      List() {
        ForEach(this.switchList, (switchItem) => {
          ListItem() {
            Row({ space: '15vp' }) {
              Image(switchItem.switchSrc).width('25vp').height('25vp').margin({ left: '2%' })

              Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
                Text(switchItem.switchName).fontSize('15fp').fontColor($r('app.color.color_333'))

                Toggle({ type: ToggleType.Switch, isOn: switchItem.isOpen })
                  .width('60vp')
                  .height('25vp')
                  .enabled(switchItem.enable)
                  .onChange((isOn) => {
                    console.log('isOn' + isOn)
                    switchItem.isOpen = isOn
                  })
                  .margin({ right: '10vp' })
              }.height('60vp').width('90%')
            }
            .height('60vp')
            .width('100%')
            .borderRadius('5vp')
            .backgroundColor($r('app.color.color_fff'))
            .margin({ top: '10vp' })
          }
        }, switchItem => switchItem.switchName)
      }
    }.width('95%')
  }
}

@CustomDialog
struct CustomDialogCollect {
  @Link private collectConfigs: Array<CollectItem>
  controller: CustomDialogController
  cancel: () => void
  confirm: () => void

  build() {
    Column() {
      List() {
        ForEach(this.collectConfigs, (item) => {
          ListItem() {
            if (item.isSupport) {
              Row({ space: '15vp' }) {
                Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
                  Text(item.name).fontSize('18fp').fontColor($r('app.color.color_333')).margin({ left: 20 })
                  Toggle({ type: ToggleType.Switch, isOn: item.isSupport })
                    .width('60vp')
                    .height('25vp')
                    .enabled(true)
                    .onChange((isOn) => {
                      item.isSelect = isOn
                    })
                    .margin({ right: '5vp' })
                }.height('60vp').width('90%')
              }
              .height('60vp')
              .width('100%')
              .borderRadius('5vp')
              .backgroundColor($r('app.color.color_fff'))
              .margin({ top: '10vp' })
            } else {
              Row({ space: '15vp' }) {
                Flex({ justifyContent: FlexAlign.SpaceBetween, alignItems: ItemAlign.Center }) {
                  Text(item.name).fontSize('18fp').fontColor($r('app.color.color_333')).margin({ left: 20 })
                  Toggle({ type: ToggleType.Switch, isOn: item.isSupport })
                    .width('60vp')
                    .height('25vp')
                    .enabled(false)
                    .margin({ right: '5vp' })
                }.height('60vp').width('90%')
              }
              .onClick(() => {
                try {
                  promptAction.showToast({ message: 'not support!', duration: 1000 })
                } catch (error) {
                  console.error(`showToast args error code is ${error.code}, message is ${error.message}`);
                }
              })
              .height('60vp')
              .width('100%')
              .borderRadius('5vp')
              .backgroundColor($r('app.color.color_eee'))
              .margin({ top: '10vp' })
            }
          }
        }, item => item.name)
      }

      Flex({ justifyContent: FlexAlign.SpaceAround }) {
        Button('cancel')
          .onClick(() => {
            this.controller.close()
            this.cancel()
          }).backgroundColor(0xffffff).fontColor(Color.Black)
        Button('confirm')
          .onClick(() => {
            this.controller.close()
            this.confirm()
          }).backgroundColor(0xffffff).fontColor(Color.Red)
      }.margin({ bottom: 10 })
    }
  }
}

@CustomDialog
export struct TextInputDialog {
  @Link private testName: String
  controller: CustomDialogController
  cancel: () => void
  confirm: () => void

  aboutToAppear() {
    console.log('TextInputDialog called')
  }

  build() {
    Column() {
      TextArea({ placeholder: '请输入测试名称', text: this.testName.toString() })
        .placeholderFont({ size: 15 })
        .fontSize('15fp')
        .textAlign(TextAlign.Center)
        .fontSize(30)
        .onChange((value: string) => {
          this.testName = value
        })
        .padding(20)
      Flex({ justifyContent: FlexAlign.SpaceAround }) {
        Button('cancel')
          .onClick(() => {
            this.controller.close()
            this.cancel()
          }).backgroundColor(0xffffff).fontColor(Color.Black)
        Button('confirm')
          .onClick(() => {
            this.controller.close()
            this.confirm()
          }).backgroundColor(0xffffff).fontColor(Color.Red)
      }.margin({ bottom: 10 })
    }
  }
}