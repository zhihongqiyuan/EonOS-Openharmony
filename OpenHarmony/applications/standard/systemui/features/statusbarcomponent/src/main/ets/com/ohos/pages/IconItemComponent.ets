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

import Log from '../../../../../../../../common/src/main/ets/default/Log'
import EventManager, { unsubscribe } from '../../../../../../../../common/src/main/ets/default/event/EventManager'
import CheckEmptyUtils from '../../../../../../../../common/src/main/ets/default/CheckEmptyUtils'
import { WINDOW_SHOW_HIDE_EVENT, ArgsInfo } from '../../../../../../../../common/src/main/ets/default/WindowManager'
import { PluginType,itemData } from "../../../../../../../../common/src/main/ets/plugindatasource/common/Constants"
import ViewModel from '../viewmodel/StatusBarVM'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { IconItemComponentStyle } from '../common/StyleConfiguration'
import MetaIconItemComponent from './MetaIconItemComponent'
import PluginIconItemComponent from './PluginIconItemComponent'
import { Event } from '@ohos/common'

const TAG = 'IconItemComponent'

interface itemStatusData{
  selected: boolean
}

@Component
export default struct IconItemComponent {
  @Prop keyId: string
  @State mItemData: itemData = new itemData()
  @State mItemStatus: itemStatusData = {
    selected: false
  }
  mClearCallback: unsubscribe | undefined
  @State style: IconItemComponentStyle = StyleConfiguration.getIconItemComponentStyle()
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()
  @State mHasMarginLR: boolean = false

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId} `);
    this.linkItemData()
    if (this.mItemData?.relationWindowId) {
      this.mClearCallback = EventManager.subscribe(
        WINDOW_SHOW_HIDE_EVENT,
        (args: ArgsInfo) => this.onWindowChange(args));
    }
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
    this.mClearCallback && this.mClearCallback()
    this.mClearCallback = undefined
  }

  linkItemData() {
    this.mItemData = AppStorage.Get('StatusBar_' + this.keyId) as itemData
    Log.showInfo(TAG, `linkItemData, mItemData: ${this.keyId} ${this.mItemData.label} ${
    this.mItemData
    .iconUrl}`)

    const statusBarStatusKey = 'StatusBar_Status_' + this.keyId
    AppStorage.SetOrCreate(statusBarStatusKey, { selected: false })
    this.mItemStatus = AppStorage.Get(statusBarStatusKey) as itemStatusData
  }

  build() {
    Row() {
      if (this.mHasMarginLR) {
        Row().width(this.style.marginLeft).height('100%')
      }
      Row() {
        if (this.mItemData.pluginType == PluginType.META) {
          MetaIconItemComponent({
            keyId: this.keyId
          })
        } else if (this.mItemData.pluginType == PluginType.DATA_ABILITY) {
          // TODO:
        } else if (this.mItemData.pluginType == PluginType.PLUGIN_COMPONENT) {
          PluginIconItemComponent({
            keyId: this.keyId
          })
        }
      }
      .height(this.style.stackHeight)
      .borderRadius(this.style.stackBorderRadius)
      .backgroundColor(this.mItemStatus.selected ? this.style.stackBgColorSelected : this.style.stackBgColorUnSelected)
      .onClick(this.onIconItemClick.bind(this))
      .gesture(LongPressGesture({ repeat: false }).onAction(this.onIconItemLongPressGesture.bind(this)))
      .onTouch(this.onIconItemTouch.bind(this))
      .onAreaChange((e, e2) => {
        this.mHasMarginLR = e2.width > 0
      })

      if (this.mHasMarginLR) {
        Row().width(this.style.marginRight).height('100%')
      }
    }.height('100%')
  }

  onWindowChange(args: ArgsInfo) {
    if (this.mItemData.canSelect && args?.windowName == this.mItemData.relationWindowId) {
      this.mItemStatus.selected = args?.isShow as boolean
    }
  }

  onIconItemClick(event: ClickEvent) {
    Log.showInfo(TAG, `onIconItemClick`);
    if (!this.mItemStatus.selected) {
      this.execClickAction()
    } else {
      this.execSelectedClickAction()
    }
  }

  execClickAction() {
    Log.showInfo(TAG, `execClickAction`);
    if (this.mItemData?.actionData?.clickAction) {
      EventManager.publish(this.mItemData.actionData.clickAction)
    }
  }

  execSelectedClickAction() {
    Log.showInfo(TAG, `execSelectedClickAction ${this.mItemData?.actionData?.selectedClickAction}`);
    if (this.mItemData?.actionData?.selectedClickAction) {
      EventManager.publish(this.mItemData.actionData.selectedClickAction)
    }
  }

  onIconItemLongPressGesture(event: GestureEvent) {
    Log.showInfo(TAG, `onIconItemLongPressGesture, event: ${JSON.stringify(event)}`);
    let longClickEvent = this.mItemData.actionData?.longClickAction as Event;
    longClickEvent && EventManager.publish(longClickEvent);
  }

  onIconItemTouch(event: TouchEvent) {
    Log.showInfo(TAG, `onIconItemTouch, event: ${JSON.stringify(event)}`);
    if (this.mItemData?.actionData?.clickAction || this.mItemData?.actionData?.longClickAction) {
      event.stopPropagation()
    }
  }
}