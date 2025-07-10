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
import iconTitleBase from '../../../../../../../../common/src/main/ets/template/iconTitleBase';
import { ControlComponentData } from '../common/Constants';

const TAG = 'ComplexToggleComponent';

@Component
export default struct ComplexToggleComponent {
  @Prop keyId: string;
  @State mItemData: ControlComponentData = { };
  @State mStatus: boolean = false;
  @State mDefaultTitle: string = '';

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}`);
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

  build() {
    Column() {
      iconTitleBase({
        useTitleStr: true,
        mTitle: $mDefaultTitle,
        mTitleStr: this.mItemData
        .label,
        useIconStr: true,
        iconOffStr: this.mItemData
        .iconUrl,
        iconOnStr: this.mItemData
        .iconUrl,
        changeSwitch: $mStatus,
        mClickEvent: (): void => this.onIconItemClick(),
        mLongClickEvent: (): void => this.onIconItemLongPressGesture()
      })
    }.width('100%')
    .height('100%')
  }

  onIconItemClick() {
    Log.showDebug(TAG, `onIconItemClick`);
    if (this.mItemData?.actionData?.clickAction) {
      EventManager.publish(this.mItemData.actionData.clickAction);
    }
  }

  onIconItemLongPressGesture() {
    Log.showDebug(TAG, `onIconItemLongPressGesture`);
    if (this.mItemData?.actionData?.longClickAction) {
      EventManager.publish(this.mItemData.actionData.longClickAction);
    }
  }
}