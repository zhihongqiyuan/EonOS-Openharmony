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

import { Log } from '../../../utils/Log';
import { SettingManager } from '../../../setting/SettingManager';
import Timer from '../../../setting/settingitem/Timer';
import AspectRatio from '../../../setting/settingitem/AspectRatio';
import Resolution from '../../../setting/settingitem/Resolution';
import { Dispatch, getStore, OhCombinedState } from '../../../redux/store';

class StateStruct {
  mode: string = '';
}

class EntryComponentDispatcher {
  public setDispatch(dispatch: Dispatch) {
    this.mDispatch = dispatch;
  }

  private mDispatch: Dispatch = (data) => data;
}

@Component
export default struct EntryComponent {
  @State itemValue: Resource = $r('app.string.photo_ratio_4_3');
  @State checkedValue: string = '';
  @State settingAlias: string = '';
  @State state: StateStruct = new StateStruct();
  private TAG: string = '[EntryComponent]:';
  private getValue: Resource = $r('app.string.resolution_1280_720');
  private settingManager = SettingManager.getInstance();
  private mAction: EntryComponentDispatcher = new EntryComponentDispatcher();

  aboutToAppear(): void {
    Log.info(`${this.TAG} aboutToAppear calle1d = ${this.settingAlias}`)
    getStore().subscribe((state: OhCombinedState) => {
      this.state = {
        mode: state.modeReducer.mode
      };
    }, (dispatch: Dispatch) => {
      this.mAction.setDispatch(dispatch);
    });

    try {
      this.getValue = JSON.parse(JSON.stringify(this.settingManager.getSettingValue(this.settingAlias)))
      Log.log(`${this.TAG} EntryComponent.getValue=${this.getValue}`)
    } catch {
      Log.log(`${this.TAG} catch this.settingAlias=${this.settingAlias}`)
      if (this.settingAlias === Resolution.ALIAS) {
        this.getValue = Resolution.DEFAULT_VALUE
      } else if (this.settingAlias === Timer.ALIAS) {
        this.getValue = Timer.DEFAULT_VALUE
      } else {
        this.getValue = AspectRatio.DEFAULT_VALUE
      }
      Log.error(`${this.TAG} catch this.getValue=${this.getValue}`)
    }
  }

  build() {
    Flex({ direction: FlexDirection.Row, alignItems: ItemAlign.Center, justifyContent: FlexAlign.SpaceBetween }) {
      Text(this.itemValue)
        .fontColor('#E6000000')
        .fontSize($r('sys.float.ohos_id_text_size_body1'))
        .fontWeight(FontWeight.Regular)

      Radio({ group: 'settingChildren', value: this.itemValue.toString() })
        .width(24)
        .height(24)
        .margin({ right: 14 })
        .borderColor('#007DFF')
        .checked(JSON.stringify(this.itemValue) === JSON.stringify(this.getValue))
        .enabled(true)
        .onClick(() => {
          Log.info(`${this.TAG} onChange settingAlias:${this.settingAlias}  itemValue:${this.itemValue}`)
          this.settingManager.setSettingValue(this.settingAlias, this.itemValue, this.state.mode)
          this.onChange()
        })
    }
    .width('100%')
  }

  private onChange: Function = () => {
  };
}