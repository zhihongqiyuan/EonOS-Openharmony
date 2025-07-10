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
import { TintContentInfo } from '../../../../../../../../common/src/main/ets/default/TintStateManager'
import CheckEmptyUtils from '../../../../../../../../common/src/main/ets/default/CheckEmptyUtils'
import ViewModel from '../viewmodel/StatusBarVM'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { IconItemComponentStyle } from '../common/StyleConfiguration'
import { itemData } from "../../../../../../../../common/src/main/ets/plugindatasource/common/Constants"

const TAG = 'MetaIconItemComponent'

@Component
export default struct MetaIconItemComponent {
  @Prop keyId: string
  @State mItemData: itemData = new itemData()
  @State mTintContentInfo: TintContentInfo = new TintContentInfo()
  @State style: IconItemComponentStyle = StyleConfiguration.getIconItemComponentStyle()
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}`);
    this.linkItemData()
    this.mTintContentInfo = ViewModel.getPluginTintContentInfo(this.keyId)
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  linkItemData() {
    this.mItemData = AppStorage.Get('StatusBar_' + this.keyId) as itemData;
    Log.showInfo(TAG, `linkItemData, mItemData: ${this.keyId} ${this.mItemData.label} ${
    this.mItemData
    .iconUrl}`)
  }

  build() {
    Row() {
      if ((this.mItemData.isShowLabel && !CheckEmptyUtils.isEmpty(this.mItemData.label))
      || (this.mItemData.isShowIcon && !CheckEmptyUtils.isEmpty(this.mItemData.iconUrl))) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
      if (this.mItemData.isShowLabel && !CheckEmptyUtils.isEmpty(this.mItemData.label)) {
        Text(this.mItemData
        .label)
          .fontColor(this.mTintContentInfo.contentColor)
          .fontSize(this.styleCommon.statusBarFontSize)
          .textAlign(TextAlign.End)
      }
      if (this.mItemData.isShowLabel && !CheckEmptyUtils.isEmpty(this.mItemData.label)) {
        if (this.mItemData.isShowIcon && !CheckEmptyUtils.isEmpty(this.mItemData.iconUrl)) {
          Row() {
          }.height(1).width(this.style.componentSpace)
        }
      }
      if (this.mItemData.isShowIcon && !CheckEmptyUtils.isEmpty(this.mItemData.iconUrl)) {
        Image(this.mItemData
        .iconUrl)
          .width(this.style.iconWidth)
          .height(this.style.iconHeight)
          .fillColor(this.mTintContentInfo.contentColor)
          .objectFit(ImageFit.Contain)
      }
      if ((this.mItemData.isShowLabel && !CheckEmptyUtils.isEmpty(this.mItemData.label))
      || (this.mItemData.isShowIcon && !CheckEmptyUtils.isEmpty(this.mItemData.iconUrl))) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
    }.height('100%')
  }
}