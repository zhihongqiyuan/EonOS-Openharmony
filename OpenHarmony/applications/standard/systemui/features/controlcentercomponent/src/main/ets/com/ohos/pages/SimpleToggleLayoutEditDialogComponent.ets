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
import StyleConfiguration, { ControlEditDialogStyle } from '../common/StyleConfiguration';

const TAG = 'Control-SimpleToggleLayoutEditDialogComponent';

@CustomDialog
export default struct SimpleToggleLayoutEditDialogComponent {
  private title: Resource | undefined = undefined;
  private leftButton: Resource| undefined = undefined;
  private rightButton: Resource| undefined = undefined;
  private controller: CustomDialogController;
  private leftAction: () => void = () => {};
  private rightAction: () => void = () =>{};
  @State style: ControlEditDialogStyle = StyleConfiguration.getControlEditDialogStyle();

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Column() {
      Row().height(this.style.editDialogFontMarginTop).layoutWeight(0).width(1)
      Row() {
        Column() {
          Text(this.title)
            .fontSize(this.style.editDialogFontSize)
        }.layoutWeight(1)
      }.width('100%')
      .height(this.style.editDialogFontHeight)

      Row().width('100%').height(this.style.editDialogBtnMarginTop)

      Row() {
        Column().width(this.style.editDialogBtnMarginLF).height(1).layoutWeight(0)
        Row() {
          Column() {
            Button({ type: ButtonType.Capsule, stateEffect: true }) {
              Text(this.leftButton)
                .fontColor(this.style.editDialogBtnFontColor)
                .fontSize(this.style.editDialogButtonSize)
            }
            .backgroundColor(this.style.editDialogBtnBgColor)
            .width('100%')
            .height('100%')
            .onClick(() => {
              Log.showInfo(TAG, `left button on click`)
              if (this.leftAction) {
                this.leftAction()
              }
              this.controller.close();
            })
          }.width(this.style.editDialogBtnWidth)

          Divider()
            .vertical(true)
            .backgroundColor(this.style.editDialogDividerColor)
            .strokeWidth(this.style.editDialogDividerWidth)
            .height(this.style.editDialogDividerHeight)

          Column() {
            Button({ type: ButtonType.Capsule, stateEffect: true }) {
              Text(this.rightButton)
                .fontColor(this.style.editDialogBtnFontColor)
                .fontSize(this.style.editDialogButtonSize)
            }
            .backgroundColor(this.style.editDialogBtnBgColor)
            .width('100%')
            .height('100%')
            .onClick(() => {
              Log.showInfo(TAG, `right button on click`)
              if (this.rightAction) {
                this.rightAction()
              }
              this.controller.close();
            })
          }.width(this.style.editDialogBtnWidth)
        }
        .height(this.style.editDialogButtonHeight)

        Column().width(this.style.editDialogBtnMarginLF).height(1).layoutWeight(0)
      }
      .width('100%')

      Row().width('100%').height(this.style.editDialogBtnMarginTop)
    }.height(this.style.editDialogHeight)
    .width(this.style.editDialogWidth)
    .backgroundColor(this.style.editDialogColor)
    .border({ width: 1, color: this.style.editDialogColor,
      radius: this.style.editDialogRadius })
  }
}
