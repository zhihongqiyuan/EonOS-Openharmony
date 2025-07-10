/**
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
import { SettingData, SettingGroupItem } from '../model/SettingData';
import { Dispatch, getStore, OhCombinedState } from '../../../redux/store';

class StateStruct {
  mode: string = '';
}

class SetToggleDispatcher {
  public setDispatch(dispatch: Dispatch) {
    this.mDispatch = dispatch;
  }

  private mDispatch: Dispatch = (data) => data;
}

@Component
export struct SetToggle {
  @Link settingsList: SettingGroupItem[];
  @State isOn: boolean = false;
  @State generalStatusValue: boolean = false;
  @State state: StateStruct = new StateStruct();
  private TAG: string = '[SetToggle]:';
  private itemValue: SettingData = {};
  private WH_100_100: string = "100%";
  private settingManager = SettingManager.getInstance();
  private mAction: SetToggleDispatcher = new SetToggleDispatcher();

  aboutToAppear() {
    Log.info(`${this.TAG} aboutToAppear start`)
    getStore().subscribe((state: OhCombinedState) => {
      this.state = {
        mode: state.modeReducer.mode
      };
    }, (dispatch: Dispatch) => {
      this.mAction.setDispatch(dispatch);
    });

    try {
      if (this.settingManager.getSettingValue(this.itemValue.settingAlias) === "1") {
        this.generalStatusValue = true
      } else {
        this.generalStatusValue = false
      }
    } catch {
      Log.info(`${this.TAG} aboutToAppear settingsList ${JSON.stringify(this.settingsList)}`)
      this.generalStatusValue = false
    }
    Log.info(`${this.TAG} aboutToAppear end`)
  }

  build() {
    Row() {
      Flex({
        direction: FlexDirection.Row,
        alignItems: ItemAlign.Center,
        justifyContent: FlexAlign.SpaceBetween
      }) {
        Column() {
          Row() {
            Image(this.itemValue.imagePath)
              .width(24)
              .height(24)
              .fillColor('#FFFFFF')
            Text(this.itemValue.settingName)
              .fontColor('#FFFFFF')
              .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
              .fontWeight(FontWeight.Regular)
              .margin({ left: $r('sys.float.ohos_id_elements_margin_horizontal_l') })
          }

          if (this.itemValue?.description) {
            Row() {
              Text(this.itemValue.description)
                .fontColor('#FFFFFF')
                .fontSize($r('sys.float.ohos_id_text_size_body2'))
                .fontWeight(FontWeight.Regular)
                .opacity($r('sys.float.ohos_id_alpha_content_secondary'))
            }.margin({
              top: $r('sys.float.ohos_id_text_margin_vertical'),
              left: 40
            })
          }
        }
        .layoutWeight(1)
        .alignItems(HorizontalAlign.Start)

        Row() {
          Toggle({ type: ToggleType.Switch, isOn: this.generalStatusValue })
            .width(36)
            .height(20)
            .selectedColor('#1095E8')
            .onChange((isOn: boolean) => {
              Log.info(this.TAG + ' SettingItemToggle onChange for Wlan Enable:' + isOn)
              this.isOn = new Boolean(isOn).valueOf()
              let setToggle: string = "0"
              if (this.isOn) {
                setToggle = "1"
              } else {
                setToggle = "0"
              }
              this.settingManager.setSettingValue(this.itemValue.settingAlias, setToggle, this.state.mode)
            })
        }.margin({ left: $r('sys.float.ohos_id_card_margin_end') })
      }
      .height('100%')
    }
    .padding({ left: 12, right: 12 })
    .height(this.itemValue?.description ? 72 : 56)
    .width(this.WH_100_100)
  }
}