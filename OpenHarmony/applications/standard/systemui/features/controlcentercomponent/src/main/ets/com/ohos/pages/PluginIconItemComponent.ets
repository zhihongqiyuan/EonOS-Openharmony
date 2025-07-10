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
import { TintContentInfo } from '../../../../../../../../common/src/main/ets/default/TintStateManager';
import { PluginComponentData } from "../../../../../../../../common/src/main/ets/plugindatasource/common/Constants";
import ViewModel from '../viewmodel/ControlCenterVM';
import StyleConfiguration from '../../../../../../../../common/src/main/ets/template/common/StyleConfiguration';

const TAG = 'PluginIconItemComponent';

@Component
export default struct PluginIconItemComponent {
  private keyId: string = '';
  private mEditMode: boolean = false;
  private mDragMode: boolean = false;
  @State @Watch('onPluginDataChange') mPluginData: PluginComponentData = new PluginComponentData();

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start, keyId: ${this.keyId}`);
    this.linkItemData();

  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisappear`);
  }

  linkItemData() {
    Log.showInfo(TAG, `linkItemData, keyId: ${this.keyId}`);
    this.mPluginData = ViewModel.getPluginData(this.keyId);
  }

  onPluginDataChange(propName: string): void {
    Log.showInfo(TAG, `onPluginDataChange, propName: ${propName} template: ${JSON.stringify(this.mPluginData.template)} data: ${JSON.stringify(this.mPluginData.data)}`);
  }

  build() {
    Row() {
      if (this.mPluginData.template && this.mPluginData.data && Object.keys(this.mPluginData.data).length > 0) {
        PluginComponent({
          template: this.mPluginData.template,
          data: this.mPluginData.data
        }).onComplete(() => {
          Log.showInfo(TAG, `PluginComponent.Complete`)
        }).onError((error) => {
          Log.showInfo(TAG, `PluginComponent.Error code:${error.errcode} message:${error.msg}`)
        })
          .size({ width: '100%', height: '100%' })
      }
    }.height('100%')
  }
}