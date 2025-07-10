/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import {isNfcAvailable} from '../../../../../../../../common/src/main/ets/default/Constants';
import Log from '../../../../../../../../common/src/main/ets/default/Log'
import { StatusBarData, StatusBarBackgroundData, StatusBarConfig } from '../common/Constants'
import { FASlotName } from '../../../../../../../../common/src/main/ets/default/Constants'
import { TintContentInfo } from '../../../../../../../../common/src/main/ets/default/TintStateManager'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { StatusBarNotificationIconStyle, VerticalStatusBarItemLoadComponentStyle
} from '../common/StyleConfiguration'
import ViewModel from '../viewmodel/StatusBarVM'
import { StringArray } from '../viewmodel/StatusBarVM'
import IconItemComponent from './IconItemComponent'
import BatteryIcon from '../../../../../../../batterycomponent/src/main/ets/default/pages/batteryIcon'
import ClockIcon from '../../../../../../../clockcomponent/src/main/ets/default/pages/clockIcon'
import AirplaneIcon from '../../../../../../../airplanecomponent/src/main/ets/default/pages/StatusBarIconItemAirplaneComponent'
import WifiIcon from '../../../../../../../wificomponent/src/main/ets/default/pages/wifiIcon'
import BluetoothIcon from '../../../../../../../bluetoothcomponent/src/main/ets/com/ohos/pages/StatusBarIconItemBluetoothComponent'
import SignalIcon from '../../../../../../../signalcomponent/src/main/ets/default/pages/signalIcon'
import CapsuleIcon from '../../../../../../../capsulecomponent/src/main/ets/default/pages/CapsuleIcon'
import LocationIcon from '../../../../../../../locationcomponent/src/main/ets/com/ohos/pages/StatusBarIconItemLocationComponent'
import RingModeIcon from '../../../../../../../ringmodecomponent/src/main/ets/com/ohos/pages/StatusBarIconItemRingModeComponent'
import NfcIcon from '../../../../../../../nfccomponent/src/main/ets/com/ohos/pages/StatusBarIconItemNFComponent'
import { NotificationItemData } from '@ohos/common';

const TAG = 'StatusBarComponent'
const TAG_StatusBarGroup = 'StatusBarGroup'
const TAG_VerticalStatusBarItemLoadComponent = 'VerticalStatusBarItemLoadComponent'
const TAG_StatusBarItemLoadComponent = 'StatusBarItemLoadComponent'
const TAG_StatusBarEmptyIcon = 'StatusBarEmptyIcon'
const TAG_StatusBarNotificationIcon = 'StatusBarNotificationIcon'
const TAG_StatusBarBackground = 'StatusBarBackground'

interface Item {
  index: number;
  data: NotificationItemData[];
}

@Component
export default struct StatusBarComponent {
  private mStatusBarComponentConfig: StatusBarConfig | undefined = undefined
  @State mStatusBarData: StatusBarData = ViewModel.getStatusBarData()
  private moduleName: string = ''

  aboutToAppear() {
    AppStorage.SetOrCreate('size', $r("app.float.status_bar_margin_left_right"));
    Log.showInfo(TAG, `aboutToAppear Start, mStatusBarComponentConfig: ${JSON.stringify(this.mStatusBarComponentConfig)}`);
    this.initViewModel(this.moduleName);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  initViewModel(moduleName: string) {
    Log.showInfo(TAG, `initViewModel`);
    ViewModel.initViewModel(this.mStatusBarComponentConfig, moduleName);
  }

  build() {
    Stack() {
      StatusBarBackground()
      if (this.mStatusBarData.showHorizontal) {
        Row() {
          StatusBarGroup({
            index: 0,
            mLayoutWeight: 1,
            mAlignItems: HorizontalAlign.Start
          })
          StatusBarGroup({
            index: 1,
            mLayoutWeight: 0,
            mAlignItems: HorizontalAlign.Center
          })
          StatusBarGroup({
            index: 2,
            mLayoutWeight: 1,
            mAlignItems: HorizontalAlign.End
          })
        }
        .padding({ left: $r('app.float.status_bar_padding_start'), right: $r('app.float.status_bar_padding_end') })
        .width('100%')
        .height('100%')
      } else {
        Column() {
          StatusBarGroup({
            index: 0,
            mLayoutWeight: 1,
            mAlignItems: VerticalAlign.Center
          })
          StatusBarGroup({
            index: 1,
            mLayoutWeight: 0,
            mAlignItems: VerticalAlign.Center
          })
          StatusBarGroup({
            index: 2,
            mLayoutWeight: 1,
            mAlignItems: VerticalAlign.Center
          })
        }
        .width('100%')
        .height('100%')
      }
    }
    .width('100%')
    .height('100%')
  }
}

@Component
struct StatusBarBackground {
  @State mStatusBarData: StatusBarData = ViewModel.getStatusBarData()
  @State mBackgroundDatas: StatusBarBackgroundData[] = ViewModel.getBackgroundDatas()

  aboutToAppear() {
    Log.showInfo(TAG_StatusBarBackground, `aboutToAppear`);
  }

  aboutToDisappear() {
    Log.showInfo(TAG_StatusBarBackground, `aboutToDisAppear`);
  }

  build() {
    if (this.mStatusBarData.showHorizontal) {
      Row() {
        ForEach(this.mBackgroundDatas, (data: StatusBarBackgroundData) => {
          if (data.width > 0) {
            Column() {
            }
            .width(data.width + 'px')
            .height('100%')
            .backgroundColor(data.backgroundColor)
          }
        })
      }
      .width('100%')
      .height('100%')
    } else {
      Column() {
        ForEach(this.mBackgroundDatas, (data: StatusBarBackgroundData) => {
          if (data.width > 0) {
            Column() {
            }
            .width('100%')
            .height(data.width + 'px')
            .backgroundColor(data.backgroundColor)
          }
        })
      }
      .width('100%')
      .height('100%')
    }
  }
}

@Component
struct StatusBarGroup {
  @StorageLink('StatusBarLayout') mStatusBarLayout: string[][] = [[], [], []]
  private mLayoutWeight: number = 1
  private mAlignItems: number = HorizontalAlign.Center;
  private index: number = -1;
  @State mStatusBarData: StatusBarData = ViewModel.getStatusBarData()

  aboutToAppear() {
    Log.showInfo(TAG_StatusBarGroup, `aboutToAppear, mLayoutWeight: ${this.mLayoutWeight} mAlignItems: ${this.mAlignItems} `);
  }

  aboutToDisappear() {
    Log.showInfo(TAG_StatusBarGroup, `aboutToDisAppear`);
  }

  build() {
    if (this.mStatusBarData.showHorizontal) {
      Column() {
        Row() {
          ForEach(this.mStatusBarLayout[this.index], (componentName: string) => {
            StatusBarItemLoadComponent({
              mComponentName: componentName
            })
          }, (componentName: string) => componentName)
        }.height('100%')
      }
      .alignItems(this.mAlignItems)
      .layoutWeight(this.mLayoutWeight)
      .height('100%')
    } else {
      Row() {
        Column() {
          ForEach(this.mStatusBarLayout[this.index], (componentName: string) => {
            VerticalStatusBarItemLoadComponent({
              mComponentName: componentName
            })
          }, (componentName: string) => componentName)
        }.width('100%')
        .alignItems(HorizontalAlign.Start)
      }
      .alignItems(this.mAlignItems)
      .width('100%')
      .layoutWeight(this.mLayoutWeight)
    }
  }
}

@Component
struct VerticalStatusBarItemLoadComponent {
  private mComponentName: string = ''
  @State mComponentHeight: number = 0
  @State style: VerticalStatusBarItemLoadComponentStyle = StyleConfiguration.getVerticalStatusBarItemLoadComponentStyle()

  aboutToAppear() {
    Log.showInfo(TAG_VerticalStatusBarItemLoadComponent, `aboutToAppear, mComponentName: ${this.mComponentName}`);
  }

  aboutToDisappear() {
    Log.showInfo(TAG_VerticalStatusBarItemLoadComponent, `aboutToDisAppear, mComponentName: ${this.mComponentName}`);
  }

  build() {
    Row() {
      StatusBarItemLoadComponent({
        mComponentName: this.mComponentName
      })
    }
    .height(this.mComponentHeight + 'px')
    .onAreaChange((e, e2) => {
      Log.showInfo(TAG_VerticalStatusBarItemLoadComponent, `onAreaChange, mComponentName: ${this.mComponentName} e: ${JSON.stringify(e)} e2: ${JSON.stringify(e2)}`);
      this.mComponentHeight = e2.width > 0 ? this.style.statusBarVerticalComponentHeight : 0
      Log.showInfo(TAG_VerticalStatusBarItemLoadComponent, `onAreaChange, mComponentName: ${this.mComponentName} mComponentHeight: ${this.mComponentHeight}`);
    })
  }
}

@Component
struct StatusBarItemLoadComponent {
  private mComponentName: string = ''
  aboutToAppear() {
    Log.showInfo(TAG_StatusBarItemLoadComponent, `aboutToAppear, mComponentName: ${this.mComponentName} `)
  }

  aboutToDisappear() {
    Log.showInfo(TAG_StatusBarGroup, `aboutToDisAppear, mComponentName: ${this.mComponentName} `)
  }

  build() {
    Row() {
      if (this.mComponentName == FASlotName.EMPTY) {
        StatusBarEmptyIcon()
      } else if (this.mComponentName == FASlotName.WIFI) {
        WifiIcon()
      } else if (this.mComponentName == FASlotName.BLUETOOTH) {
        BluetoothIcon()
      } else if (this.mComponentName == FASlotName.SIGNAL) {
        SignalIcon()
      } else if (this.mComponentName == FASlotName.CLOCK) {
        ClockIcon()
      } else if (this.mComponentName == FASlotName.BATTERY) {
        BatteryIcon()
      } else if (this.mComponentName == FASlotName.AIR_PLANE) {
        AirplaneIcon()
      } else if (this.mComponentName == FASlotName.CAPSULE) {
        CapsuleIcon()
      } else if (this.mComponentName == FASlotName.NOTIFICATION) {
        StatusBarNotificationIcon()
      } else if (this.mComponentName == FASlotName.LOCATION) {
        LocationIcon()
      } else if (this.mComponentName == FASlotName.RING_MODE) {
        RingModeIcon()
      } else if (this.mComponentName == FASlotName.NFC) {
        if (isNfcAvailable()) {
          NfcIcon()
        }
      } else {
        IconItemComponent({
          keyId: this.mComponentName
        })
      }
    }.height('100%')
    .onAreaChange((e: Area, e2: Area) => {
      Log.showInfo(TAG_StatusBarItemLoadComponent, `onAreaChange, componentName: ${this.mComponentName}}, new area: ${JSON.stringify(e2)} `)
      ViewModel.updateComponentArea(this.mComponentName, e2)
    })
  }
}

@Component
struct StatusBarEmptyIcon {
  @StorageLink('StatusBarEmptyWidth') mStatusBarEmptyWidth: number = 0
  @State mTintContentInfo: TintContentInfo = ViewModel.getEmptyTintContentInfo()

  aboutToAppear() {
    Log.showInfo(TAG_StatusBarEmptyIcon, `aboutToAppear, mStatusBarEmptyWidth: ${this.mStatusBarEmptyWidth} `)
  }

  aboutToDisappear() {
    Log.showInfo(TAG_StatusBarEmptyIcon, `aboutToDisAppear`);
  }

  build() {
    Row().width(this.mStatusBarEmptyWidth).height('100%')
  }
}

@Component
struct StatusBarNotificationIcon {
  @StorageLink('notificationList') notificationList: NotificationItemData[][] = []
  @StorageLink('StatusCoefficient') StatusCoefficient: number = 1.0
  @State mTintContentInfo: TintContentInfo = ViewModel.getNotificationTintContentInfo()
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()
  @State style: StatusBarNotificationIconStyle = StyleConfiguration.getStatusBarNotificationIconStyle()

  aboutToAppear() {
    Log.showInfo(TAG_StatusBarNotificationIcon, `aboutToAppear`);
  }

  aboutToDisappear() {
    Log.showInfo(TAG_StatusBarNotificationIcon, `aboutToDisAppear`);
  }

  build() {
    Row() {
      if (this.notificationList.length > 0) {
        Row().height(1).width(this.styleCommon.statusBarMarginLeftRight)
      }
      if (this.notificationList.length > 3) {
        ForEach(this.notificationList.slice(0, 3), (item: NotificationItemData[]) => {
          Image(item[0].smallIcon)
            .objectFit(ImageFit.ScaleDown)
            .height(this.style.iconHeight)
            .width(this.style.iconWidth)
            .margin({ right: this.style.iconSpace })
            .fillColor(this.mTintContentInfo.contentColor)
        })
        Row() {
          Text('...')
            .fontSize(this.styleCommon.statusBarFontSize)
            .fontColor(this.mTintContentInfo.contentColor)
        }
      } else {
        ForEach(this.notificationList.map((item, index1) => {
          let res: Item = { index: index1, data: item }
          return res
        }), (res: Item) => {
          if (res.index > 0) {
            Row().height(1).width(this.style.iconSpace)
          }
          Image(res.data[0].smallIcon)
            .objectFit(ImageFit.ScaleDown)
            .height(this.style.iconHeight)
            .width(this.style.iconWidth)
            .fillColor(this.mTintContentInfo.contentColor)
        })
      }
      if (this.notificationList.length > 0) {
        Row().height(1).width(this.styleCommon.statusBarMarginLeftRight)
      }
    }
    .height('100%')
  }
}
