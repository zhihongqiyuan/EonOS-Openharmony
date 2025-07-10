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
import { StartTestTitleComponent } from '../common/ui/StartTestTitleComponent';
import { AppInfoItem } from '../common/entity/LocalConfigEntity';
import router from '@system.router';
import SPLogger from '../common/utils/SPLogger'


const TAG = 'AppSelectPage'
/**
 * app应用选择页
 */
@Entry
@Component
struct AppSelectPage {
  build() {
    Column() {
      StartTestTitleComponent({ title: '选择应用' })
      appInfoComponent()
    }.width('100%').height('100%')
  }
}

@Component
struct appInfoComponent {
  @State appInfoList: Array<AppInfoItem> = globalThis.appList
  aboutToAppear() {

  }

  build() {
    List() {
      ForEach(this.appInfoList, (appInfoItem) => {

        ListItem() {
          Row() {
            Image(appInfoItem.appIcon).width('40vp').height('40vp').margin({ left: '2%' })
            Flex({
              justifyContent: FlexAlign.SpaceBetween,
              alignItems: ItemAlign.Start,
              direction: FlexDirection.Column
            }) {
              Text(appInfoItem.appName)
                .fontSize('15fp')
                .fontColor($r('app.color.color_333'))
                .fontWeight(FontWeight.Bold)
              Text(appInfoItem.appVersion).fontSize('12fp').fontColor($r('app.color.color_333'))
              Divider().vertical(false).margin({ top: '5vp' }).height('1vp')
            }.margin({ left: '4%' }).height('100%').padding({ top: '20vp' })
          }.alignSelf(ItemAlign.Start).width('100%').height('80vp').onClick(() => {
            if (router.getParams()['startPage'] == 'startTest') {
              router.back({ uri: 'pages/StartTestPage', params: {
                selectPackageName: appInfoItem.packageName,
                selectAbilityName: appInfoItem.abilityName,
                appName: appInfoItem.appName,
                appVersion: appInfoItem.appVersion,
                appIconId: appInfoItem.appIcon
              } })
            }
          })
        }
      }, appInfoItem => appInfoItem.packageName)
    }.edgeEffect(EdgeEffect.None) // 滑动到边缘无效果
    .chainAnimation(false) // 联动特效关闭
  }
}