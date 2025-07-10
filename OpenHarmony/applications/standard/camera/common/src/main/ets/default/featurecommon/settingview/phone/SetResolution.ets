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

import { Action } from '../../../redux/actions/Action';
import Timer from '../../../setting/settingitem/Timer';
import { CustomDialogDetails, CustomDialogView } from '../../customdialog/dialogComponent/CustomDialogView';
import { Log } from '../../../utils/Log';
import { Dispatch, getStore, OhCombinedState } from '../../../redux/store';
import { SettingManager } from '../../../setting/SettingManager';
import { SettingData, SettingGroupItem } from '../model/SettingData';

class StateStruct {
}

class SetResolutionDispatcher {
  public setDispatch(dispatch: Dispatch) {
    this.mDispatch = dispatch;
  }

  public closeDialog(isCloseFlag: boolean): void {
    this.mDispatch(Action.closeDialog(isCloseFlag));
  }

  private mDispatch: Dispatch = (data) => data;
}


@Component
export struct SetResolution {
  @Link settingsList: SettingGroupItem[];
  @Link @Watch('onCloseDialog') closeFlag: boolean;
  @StorageLink('settingDialogFlag') settingDialogFlag: boolean = true;
  @State curCheckName: Resource = $r('app.string.default_value');
  @State state: StateStruct = new StateStruct();
  CustomDialogView: CustomDialogController = new CustomDialogController({
    builder: CustomDialogView({ cancel: () => this.existView() }),
    autoCancel: true,
    alignment: DialogAlignment.Bottom,
    cancel: this.existView,
    customStyle: true
  });
  private TAG: string = '[SetResolution]:';
  private itemValue: SettingData = {};
  private WH_100_100: string = "100%";
  private settingManager = SettingManager.getInstance();
  private setAlias: string = '';
  @Provide customDialogDetails: CustomDialogDetails = {
    confirmCallback: Function,
    confirmItem: true,
    height: 256,
    width: 256,
    setAlias: this.setAlias,
    childrenList: [],
    settingTitle: ''
  };
  private mAction: SetResolutionDispatcher = new SetResolutionDispatcher();

  existView(): void {
    Log.info(`${this.TAG} existView E`)
    if (this.settingManager.getSettingValue(this.itemValue.settingAlias) !== undefined) {
      this.curCheckName = JSON.parse(JSON.stringify(this.settingManager.getSettingValue(this.itemValue.settingAlias)))
      Log.info(`${this.TAG} existView curCheckName: ${this.curCheckName} X`)
    }
    this.mAction.closeDialog(false)
  }

  aboutToAppear(): void {
    Log.info(`${this.TAG} aboutToAppear settingsList: ${JSON.stringify(this.settingsList)} E`);
    getStore().subscribe((state: OhCombinedState) => {
      this.state = {};
    }, (dispatch: Dispatch) => {
      this.mAction.setDispatch(dispatch);
    });

    if (this.settingManager.getSettingValue(this.itemValue.settingAlias) !== undefined) {
      this.curCheckName = JSON.parse(JSON.stringify(this.settingManager.getSettingValue(this.itemValue.settingAlias)))
      Log.info(`${this.TAG} aboutToAppear curCheckName: ${this.curCheckName} X`)
    }
  }

  public onCloseDialog(): void {
    Log.info(`${this.TAG} onCloseDialog E`)
    if (this.customDialogDetails.setAlias !== undefined) {
      this.mAction.closeDialog(false)
      this.CustomDialogView.close()
    }
    Log.info(`${this.TAG} onCloseDialog X`)
  }

  public formatCurCheckName(name: Resource): Resource {
    if (name.id === Timer.DEFAULT_VALUE.id) {
      name = Timer.RESOURCE_OFF_ALREADY
    }
    return name
  }

  build() {
    Row() {
      Flex({
        direction: FlexDirection.Row,
        alignItems: ItemAlign.Center,
        justifyContent: FlexAlign.SpaceBetween
      }) {
        Row() {
          Image(this.itemValue.imagePath)
            .width(24)
            .height(24)
            .fillColor('#FFFFFF')
          Text(this.itemValue.settingName)
            .margin({ left: $r('sys.float.ohos_id_elements_margin_horizontal_l') })
            .fontColor('#FFFFFF')
            .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
            .fontWeight(FontWeight.Regular)
        }

        Row() {
          Text(this.formatCurCheckName(this.curCheckName))
            .fontColor($r('app.color.font_color_FFFFFF'))
            .fontSize($r('sys.float.ohos_id_text_size_body2'))
            .opacity(0.6)
            .fontWeight(FontWeight.Regular)
          Image($r('app.media.ic_public_arrow_right'))
            .width(12)
            .height(24)
            .fillColor('#33FFFFFF')
            .opacity(0.4)
            .opacity($r('app.float.opacity_4'))
            .margin({ left: 4 })
        }
      }
      .height('100%')
    }
    .padding({ left: 12, right: 12 })
    .width(this.WH_100_100)
    .height(56)
    .onClick(() => {
      if (this.settingDialogFlag) {
        this.settingDialogFlag = false
        setTimeout(() => {
          this.settingDialogFlag = true
        }, 200)
        this.openDetailsDialog(this.itemValue)
      }
    })
  }

  async openDetailsDialog(itemValue: SettingData): Promise<void> {
    Log.info(`${this.TAG} openDetailsDialog start dataInfo ${JSON.stringify(itemValue)}`)
    // 需要根据相机能力更新这个childrenList，目前是写死状态
    // this.customDialogDetails.childrenList = itemValue.radio
    // this.customDialogDetails.settingTitle = itemValue.settingName
    // this.customDialogDetails.setAlias = itemValue.settingAlias
    Log.info(`${this.TAG} childrenList: ${JSON.stringify(itemValue.radio)}`)
    this.mAction.closeDialog(true)
    this.CustomDialogView.open()
    Log.info(`${this.TAG} openDetailsDialog end`)
  }
}