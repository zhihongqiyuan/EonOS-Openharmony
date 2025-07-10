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

import Log from '../../../../../../../../common/src/main/ets/default/Log';
import EventManager from '../../../../../../../../common/src/main/ets/default/event/EventManager';
import SimpleToggleBase from '../../../../../../../../common/src/main/ets/template/SimpleToggleBase';
import PluginIconItemComponent from './PluginIconItemComponent';
import {PluginType} from "../../../../../../../../common/src/main/ets/plugindatasource/common/Constants"
import { ControlComponentData } from '../common/Constants';

const TAG = 'SimpleToggleComponent';

@Component
export default struct SimpleToggleComponent {
  @Prop keyId: string;
  @State mItemData: ControlComponentData = {};
  private mEditMode: boolean = false;
  private mDragMode: boolean = false;
  @State mDefaultIcon: Resource = $r("app.media.icon");
  @State mDefaultChangeSwitch: boolean  = false;
  @State mDefaultLabel: string = '';

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}, mEditMode: ${this.mEditMode} mDragMode: ${this.mDragMode}`);
    this.linkItemData();
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  linkItemData() {
    Log.showDebug(TAG, `linkItemData, keyId: ${this.keyId}`);
    this.mItemData = AppStorage.Get('ControlCenter_' + this.keyId) as ControlComponentData;
    Log.showDebug(TAG, `linkItemData, mItemData: ${this.keyId} ${this.mItemData.label} ${this.mItemData.iconUrl}`);
  }

  getLabel() {
    switch (this.keyId) {
      case 'screenshot':
        return $r('app.string.control_center_simple_toggle_screenshot_mode_title')
    }
    return this.mItemData.label;
  }

  build() {
    Column() {
      if (this.keyId != ""){
        if (this.mItemData.pluginType == PluginType.META) {
          SimpleToggleBase({
            mToggleId: this.keyId,
            mIcon: $mDefaultIcon,
            mIconStr: this.mItemData.iconUrl,
            mUseIconStr: true,
            mChangeSwitch: $mDefaultChangeSwitch,
            mLabel: $mDefaultLabel,
            mLabelStr: this.getLabel(),
            mUseLabelStr: true,
            mEditMode: this.mEditMode,
            mDragMode: this.mDragMode,
            mClickEvent: (): void => this.onIconItemClick(),
            mLongClickEvent: (): void => this.onIconItemLongPressGesture()
          })
        } else if (this.mItemData.pluginType == PluginType.DATA_ABILITY) {
          // TODO:
        } else if (this.mItemData.pluginType == PluginType.PLUGIN_COMPONENT) {
          PluginIconItemComponent({
            keyId: this.keyId,
            mEditMode: this.mEditMode,
            mDragMode: this.mDragMode
          })
        }
      }
    }
    .width('100%')
    .height('100%')
  }

  onIconItemClick() {
    Log.showDebug(TAG, `onIconItemClick`);
    if (this.mItemData?.actionData?.clickAction) {
      if (this.mItemData.pluginType == PluginType.META) {
        EventManager.publish(this.mItemData.actionData.clickAction, this.mItemData.pluginType);
      } else {
        EventManager.publish(this.mItemData.actionData.clickAction);
      }
    };
  }

  onIconItemLongPressGesture() {
    Log.showDebug(TAG, `onIconItemLongPressGesture`);
    if (this.mItemData?.actionData?.longClickAction) {
      EventManager.publish(this.mItemData.actionData.longClickAction);
    };
  }
}