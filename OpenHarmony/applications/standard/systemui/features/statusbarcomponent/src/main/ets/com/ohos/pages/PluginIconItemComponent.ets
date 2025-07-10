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
import { PluginComponentData } from "../../../../../../../../common/src/main/ets/plugindatasource/common/Constants"
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { PluginIconItemComponentStyle } from '../common/StyleConfiguration'
import ViewModel from '../viewmodel/StatusBarVM'

const TAG = 'PluginIconItemComponent'

@Component
export default struct PluginIconItemComponent {
  @Prop keyId: string
  @State @Watch('onPluginDataChange') mPluginData: PluginComponentData = new PluginComponentData()
  @State @Watch('onTintContentChange') mTintContentInfo: TintContentInfo = new TintContentInfo()
  @State style: PluginIconItemComponentStyle = StyleConfiguration.getPluginIconItemComponentStyle()
  @State @Watch('onCommonStyleChange') styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle()

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}`)
    this.linkItemData()
    this.mTintContentInfo = ViewModel.getPluginTintContentInfo(this.keyId)
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`)
  }

  linkItemData() {
    Log.showInfo(TAG, `linkItemData, keyId: ${this.keyId}`)
    this.mPluginData = ViewModel.getPluginData(this.keyId)
  }

  onPluginDataChange(propName: string): void {
    Log.showInfo(TAG, `onPluginDataChange, propName: ${propName} template: ${JSON.stringify(this.mPluginData.template)} data: ${JSON.stringify(this.mPluginData.data)}`)
  }

  onTintContentChange(propName: string): void {
    Log.showInfo(TAG, `onTintContentChange, propName: ${propName} value: ${this.mTintContentInfo.contentColor}`)
    this.setStyle()
  }

  onCommonStyleChange(propName: string): void {
    Log.showInfo(TAG, `onCommonStyleChange, propName: ${propName} value: ${this.styleCommon.statusBarFontSize}`)
    this.setStyle()
  }

  setStyle(): void{
    Log.showInfo(TAG, `setStyle`)
    if (this.mPluginData.data && Object.keys(this.mPluginData.data).length > 0) {
      this.mPluginData.data['fontSize'] = this.styleCommon.statusBarFontSize
      this.mPluginData.data['fontColor'] = this.mTintContentInfo.contentColor
      Log.showInfo(TAG, `setStyle, data: ${JSON.stringify(this.mPluginData.data)}`)
    }
  }

  build() {
    Row() {
      if (this.mPluginData.template && this.mPluginData.data && Object.keys(this.mPluginData.data).length > 0) {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
        PluginComponent({
          template: this.mPluginData.template,
          data: this.mPluginData.data
        }).onComplete(() => {
          Log.showInfo(TAG, `PluginComponent.Complete`)
        }).onError((error) => {
          Log.showInfo(TAG, `PluginComponent.Error code:${error.errcode} message:${error.msg}`)
        })
          .size({ width: px2vp(this.style.iconWidth), height: '100%' })
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }
    }.height('100%')
  }
}