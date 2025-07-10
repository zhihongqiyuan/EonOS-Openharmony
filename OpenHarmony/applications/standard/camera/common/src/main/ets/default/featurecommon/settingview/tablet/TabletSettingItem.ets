/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import { SettingManager } from '../../../setting/SettingManager'
import { Log } from '../../../utils/Log'
import { TabletSetResolution } from './TabletSetResolution'
import { TabletSetToggle } from './TabletSetToggle'
import { SettingData, SettingGroupItem } from '../model/SettingData'

@Component
export struct TabletSettingItem {
  @Link settingsList: SettingGroupItem[]
  @Link closeFlag: Boolean
  private TAG: string = '[TabletSettingItem]:'
  private item: SettingGroupItem = new SettingGroupItem();
  private index: number = 0
  private settingsListIndex: number = 0
  private getPhotoValue: Promise<string> = new Promise(() => {
  });
  private getVideoValue: Promise<string> = new Promise(() => {
  });
  private WH_100_100: string = "100%"
  private settingManager = SettingManager.getInstance()

  aboutToAppear() {
    Log.info(`${this.TAG} aboutToAppear start`)
    Log.info(`${this.TAG} aboutToAppear ${JSON.stringify(this.item.settingChildren)}`)
    Log.info(`${this.TAG} aboutToAppear end`)
  }

  build() {
    Flex({
      direction: FlexDirection.Column,
      alignItems: ItemAlign.Center,
      justifyContent: FlexAlign.SpaceBetween
    }) {
      Column() {
        Row() {
          Text(this.item.settingTitle)
            .margin({
              top: $r('app.float.margin_value_20'),
              left: $r('sys.float.ohos_id_card_margin_start'),
              bottom: $r('app.float.margin_value_8')
            })
            .fontColor($r('app.color.font_color_FFFFFF'))
            .opacity($r('app.float.opacity_6'))
            .fontSize($r('app.float.font_14'))
            .fontWeight(FontWeight.Medium)
        }
        .width(this.WH_100_100)
        .height(this.WH_100_100)
      }
      .width(this.WH_100_100)
      .height(48)

      Column() {
        List() {
          ForEach(this.item.settingChildren, (itemValue: SettingData) => {
            ListItem() {
              Column() {
                if (itemValue.selectType === "radio") {
                  TabletSetResolution({
                    closeFlag: $closeFlag,
                    settingsList: $settingsList,
                    itemValue: itemValue
                  })
                }
                if (itemValue.selectType === "toggle") {
                  TabletSetToggle({
                    settingsList: $settingsList,
                    itemValue: itemValue
                  })
                }
              }
            }
          })
        }
        .listDirection(Axis.Vertical)
        .divider({
          strokeWidth: 0.5,
          color: '#33FFFFFF',
          startMargin: 56,
          endMargin: 12
        })
        .borderRadius($r('sys.float.ohos_id_corner_radius_card'))
        .backgroundColor('#202224')
        .padding({ top: 4, bottom: 4 })
      }.width(this.WH_100_100)
    }
  }
}