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

import deviceInfo from '@ohos.deviceInfo';

import { Action } from '../../../redux/actions/Action';
import { Log } from '../../../utils/Log';
import { Dispatch, getStore, OhCombinedState } from '../../../redux/store';
import EntryComponent from './EntryComponent';

@Observed
export class CustomDialogDetails {
  confirmItem: boolean = false;
  height: number = 0;
  width: number = 0;
  setAlias: string = '';
  childrenList: ChildrenItemStruct[] = [];
  settingTitle: string = '';

  confirmCallback: Function = () => {
  };
}

class StateStruct {
}

class CustomDialogViewDispatcher {
  public setDispatch(dispatch: Dispatch) {
    this.mDispatch = dispatch;
  }

  public closeDialog(isCloseFlag: boolean): void {
    this.mDispatch(Action.closeDialog(isCloseFlag));
  }

  private mDispatch: Dispatch = (data) => data;
}

class ChildrenItemStruct {
  itemValue: Resource = $r('app.string.photo_ratio_4_3');
}

class StyleStruct {
  columns: number = 0;
  offset: number = 0;
}

@CustomDialog
export struct CustomDialogView {
  localStyle: StyleStruct = { columns: 4, offset: 0 };
  controller?: CustomDialogController;
  @Consume customDialogDetails: CustomDialogDetails;
  @State settingAlias: string = "";
  @State getValue: string = "";
  @State childrenList: Array<ChildrenItemStruct> = [];
  @State state: StateStruct = new StateStruct();
  private TAG: string = '[CustomDialogView]:'
  private mAction: CustomDialogViewDispatcher = new CustomDialogViewDispatcher();

  cancel: () => void = () => {
  };

  confirm: () => void = () => {
  };

  async aboutToAppear() {
    Log.info(`${this.TAG} aboutToAppear start`)
    this.childrenList = this.customDialogDetails.childrenList
    Log.info(`${this.TAG} childrenList ${JSON.stringify(this.childrenList)}`)
    getStore().subscribe((state: OhCombinedState) => {
      this.state = {};
    }, (dispatch: Dispatch) => {
      this.mAction.setDispatch(dispatch);
    });

    this.settingAlias = this.customDialogDetails.setAlias
    Log.info(`${this.TAG} aboutToAppear end`)
  }

  public onChange(): void {
    Log.info(`${this.TAG} onChange start`)
    this.mAction.closeDialog(false)
    if (this.controller) {
      this.controller.close()
    }
    this.cancel()
    Log.info(`${this.TAG} onChange end`)
  }

  build() {
    if (deviceInfo.deviceType == 'PAD' || deviceInfo.deviceType == 'tablet') {
      Flex({ direction: FlexDirection.Column, alignItems: ItemAlign.Center, justifyContent: FlexAlign.Center }) {
        Column() {
          Row() {
            Text(this.customDialogDetails.settingTitle)
              .fontSize($r('sys.float.ohos_id_text_size_dialog_tittle'))
              .fontColor('#E6000000')
              .opacity(0.9)
              .fontWeight(FontWeight.Medium)
              .opacity($r('sys.float.ohos_id_alpha_content_primary'))
          }
          .width('100%')
          .height(56)

          List() {
            ForEach(this.customDialogDetails.childrenList, (item: ChildrenItemStruct) => {
              ListItem() {
                EntryComponent({
                  itemValue: item.itemValue,
                  checkedValue: this.getValue,
                  settingAlias: this.settingAlias,
                  onChange: () => this.onChange()
                })
              }
              .height(48)
              .width('100%')
            })
          }
          .listDirection(Axis.Vertical)
          .divider({
            strokeWidth: 0.5,
            color: '#33000000',
            startMargin: 0,
            endMargin: 12
          }) // 每行之间的分界线
          Column() {
            Button({ type: ButtonType.Capsule, stateEffect: true }) {
              Text($r('app.string.cancel'))
                .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
                .fontColor('#1095E8')
                .fontWeight(FontWeight.Medium)
                .height('100%')
                .height('100%')
            }
            .width('100%')
            .height('100%')
            .backgroundColor('#00ffffff')
            .onClick(() => {
              this.mAction.closeDialog(false)
              if (this.controller) {
                this.controller?.close()
              }
            })
          }
          .height(56)
          .width('100%')
          .margin({ top: 8 }).padding({ bottom: '16vp' })
        }
        .width(394)
        .margin({ left: '12vp', right: '12vp', bottom: '16vp' })
        .backgroundColor(Color.White)
        .padding({ left: '24vp', right: '24vp' })
        .borderRadius($r('sys.float.ohos_id_corner_radius_default_xl'))
      }
    } else {
      Flex({ justifyContent: FlexAlign.Center }) {
        Column() {
          Row() {
            Text(this.customDialogDetails.settingTitle)
              .fontSize($r('sys.float.ohos_id_text_size_dialog_tittle'))
              .fontColor('#E6000000')
              .opacity(0.9)
              .fontWeight(FontWeight.Medium)
              .opacity($r('sys.float.ohos_id_alpha_content_primary'))
          }
          .width('100%')
          .height(56)

          GridContainer({ columns: this.localStyle.columns, gutter: 12, margin: 12 }) {

            List() {
              ForEach(this.customDialogDetails.childrenList, (item: ChildrenItemStruct) => {
                ListItem() {
                  EntryComponent({
                    itemValue: item.itemValue,
                    checkedValue: this.getValue,
                    settingAlias: this.settingAlias,
                    onChange: () => this.onChange()
                  })
                }
                .height(48)
                .width('100%')
              })
            }
            .listDirection(Axis.Vertical)
            .divider({
              strokeWidth: 0.5,
              color: '#33000000',
              startMargin: 0,
              endMargin: 12
            }) // 每行之间的分界线
            Column() {
              Button({ type: ButtonType.Capsule, stateEffect: true }) {
                Text($r('app.string.cancel'))
                  .fontSize($r('sys.float.ohos_id_text_size_sub_title2'))
                  .fontColor('#1095E8')
                  .fontWeight(FontWeight.Medium)
                  .height('100%')
                  .height('100%')
              }
              .width('100%')
              .height('100%')
              .backgroundColor('#00ffffff')
              .onClick(() => {
                this.mAction.closeDialog(false)
                if (this.controller) {
                  this.controller?.close()
                }
              })
            }
            .height(56)
            .width('100%')
            .margin({ top: 8 }).padding({ bottom: '16vp' })
          }
          .width('96%')
        }
        .margin({ left: '12vp', right: '12vp', bottom: '16vp' })
        .backgroundColor(Color.White)
        .padding({ left: '24vp', right: '24vp' })
        .borderRadius($r('sys.float.ohos_id_corner_radius_default_xl'))
        .useSizeType({
          xs: { span: 4, offset: this.localStyle.offset },
          sm: { span: 4, offset: this.localStyle.offset },
          md: { span: 4, offset: this.localStyle.offset },
          lg: { span: 4, offset: this.localStyle.offset }
        })
      }
    }
  }
}