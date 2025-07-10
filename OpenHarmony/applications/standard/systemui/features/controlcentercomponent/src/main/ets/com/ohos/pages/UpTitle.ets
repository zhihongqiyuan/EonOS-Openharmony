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
import EventManager from "../../../../../../../../common/src/main/ets/default/event/EventManager";
import { obtainStartAbility } from "../../../../../../../../common/src/main/ets/default/event/EventUtil";
import Constants from '../common/Constants';
import StyleConfiguration, { ControlCenterUpTitleStyle } from '../common/StyleConfiguration';

const TAG = 'Control-UpTitle';

interface IDirection {
  direction: string
}

@Component
export default struct UpTitle {
  private touchMoveCallback: Function = (data: IDirection) => {};
  private simpleToggleLayoutEditStartCallback: Function = () => {};
  startX: number = 0;
  startY: number = 0;
  @State moveX: number = 0;
  @State moveY: number = 0;
  @State mSettingIsHover: boolean = false;
  @State mEditIsHover: boolean = false;
  @State style: ControlCenterUpTitleStyle = StyleConfiguration.getControlCenterUpTitleStyle();

  aboutToAppear() {
    Log.showInfo(TAG, 'aboutToAppear');
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    Row() {
      Text($r('app.string.control_center'))
        .height('100%')
        .margin({left: this.style.marginLeft})
        .fontColor(this.style.fontColor)
        .fontSize(this.style.fontSize)
        .fontWeight(FontWeight.Medium)

      Blank().height('100%')

      Stack() {
        Flex()
          .backgroundColor(this.mSettingIsHover ? this.style.imageHoverColor : this.style.imageTransparentColor)
          .clip(new Rect({
            width: this.style.imageHoverWidth,
            height: this.style.imageHoverHeight
          }).radius(this.style.imageHoverRadius))
          .width(this.style.imageHoverWidth)
          .height(this.style.imageHoverHeight)
        Image($r('app.media.ic_public_settings'))
          .objectFit(ImageFit.Contain)
          .size({ width: this.style.imageWidth,
            height: this.style.imageHeight })
          .fillColor(this.style.titleImageColor)
          .onClick(this.settingClick.bind(this))
          .onHover((isHover) => {
            this.mSettingIsHover = isHover;
          })
      }
      .width(this.style.imageHoverWidth)
      .height(this.style.imageHoverHeight)

      Stack() {
        Flex()
          .backgroundColor(this.mEditIsHover ? this.style.imageHoverColor : this.style.imageTransparentColor)
          .clip(new Rect({
            width: this.style.imageHoverWidth,
            height: this.style.imageHoverHeight
          }).radius(this.style.imageHoverRadius))
          .width(this.style.imageHoverWidth)
          .height(this.style.imageHoverHeight)
        Image($r('app.media.ic_public_edit'))
          .objectFit(ImageFit.Contain)
          .size({ width: this.style.imageWidth,
            height: this.style.imageHeight })
          .fillColor(this.style.titleImageColor)
          .onClick(this.editClick.bind(this))
          .onHover((isHover) => {
            this.mEditIsHover = isHover;
          })
      }
      .width(this.style.imageHoverWidth)
      .height(this.style.imageHoverHeight)
      .margin({left: this.style.upTitleSpace, right: this.style.marginRight})
    }
    .width('100%')
    .height('100%')
    .alignItems(VerticalAlign.Center)
    .onTouch(this.touchEvent.bind(this))
  }

  settingClick() {
    Log.showDebug(TAG, `settingClick`);
    EventManager.publish(obtainStartAbility('com.ohos.settings', 'com.ohos.settings.MainAbility'));
  }

  editClick() {
    Log.showDebug(TAG, `editClick`);
    if (this.simpleToggleLayoutEditStartCallback) {
      this.simpleToggleLayoutEditStartCallback();
    };
  }

  touchEvent(event: TouchEvent) {
    if (event.type == Constants.TOUCHTYPE_DOWN) {
      this.startX = event.touches[0].screenX;
      this.startY = event.touches[0].screenY;
      Log.showDebug(TAG, `quicklySetting touchStart2=======startX: ${this.startX}, startY: ${this.startY}`);
    } else if (event.type == Constants.TOUCHTYPE_MOVE) {
      this.moveX = event.touches[0].screenX - this.startX;
      this.moveY = event.touches[0].screenY - this.startY;
    } else if (event.type == Constants.TOUCHTYPE_UP) {
      Log.showDebug(TAG, `quicklySetting touchEnd2, moveX: ${this.moveX}, moveY: ${this.moveY}`);
      if (this.moveX < -30) {
        if (this.touchMoveCallback) {
          this.touchMoveCallback({ 'direction': 'left' });
        };
      } else if (this.moveX > 30) {
        if (this.touchMoveCallback) {
          this.touchMoveCallback({ 'direction': 'right' });
        };
      };
    };
  }
}