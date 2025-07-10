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

import Constants from '../common/constants'
import CapsuleViewModel, { VIEW_MODEL_ID, CallState } from '../viewmodel/CapsuleViewModel'
import Log from '../../../../../../../common/src/main/ets/default/Log'
import StyleConfigurationCommon, { CommonStyle
} from '../../../../../../../common/src/main/ets/default/StyleConfiguration'
import StyleConfiguration, { CapsuleComponentStyle } from '../common/StyleConfiguration'

const TAG = "CapsuleIcon";

@Component
export default struct CapsuleIcon {
  @StorageLink(VIEW_MODEL_ID) mCapsuleViewModel: CapsuleViewModel = CapsuleViewModel.getInstance();
  startX: number = 0;
  startY: number = 0;
  moveX: number = 0;
  moveY: number = 0;
  @State style: CapsuleComponentStyle = StyleConfiguration.getCapsuleComponentStyle();
  @State styleCommon: CommonStyle = StyleConfigurationCommon.getCommonStyle();

  aboutToAppear() {
    this.mCapsuleViewModel.initViewModel();
    Log.showInfo(TAG, 'aboutToAppear, text: ' + this.mCapsuleViewModel.mText);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, 'aboutToDisappear');
  }

  build() {
    if (this.mCapsuleViewModel.mIsBackground) {
      Row() {
        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
        Row() {
          Row().width(this.style.greenCapsuleTextMarginLeftRight).height('100%')
          Image($r("app.media.ic_statusbar_phone"))
            .width(this.style.greenCapsulePhoneWidth)
            .height(this.style.greenCapsulePhoneHeight)
            .objectFit(ImageFit.Contain)
          Row().width($r("app.float.green_capsule_phone_text_left")).height('100%')
          Text(this.mCapsuleViewModel.mText)
            .fontSize(this.styleCommon.statusBarFontSize)
            .fontWeight(FontWeight.Regular)
            .fontColor(this.style.greenCapsuleTextColor)
            .maxLines(this.style.maxLines)
            .textOverflow({ overflow: TextOverflow.Ellipsis })
          Row().width(this.style.greenCapsuleTextMarginLeftRight).height('100%')
        }
        .alignItems(VerticalAlign.Center)
        .height(this.style.greenCapsuleHeight)
        .backgroundColor(this.style.greenCapsuleBackgroundColor)
        .borderRadius(this.style.greenCapsuleRadius)
        .onTouch(this.touchEvent.bind(this))

        Row().width(this.styleCommon.statusBarMarginLeftRight).height('100%')
      }.margin({
        left: $r("app.float.green_capsule_phone_margin_left"),
        right: $r("app.float.green_capsule_phone_margin_left")
      })
    }
  }

  touchEvent(event: TouchEvent) {
    Log.showDebug(TAG, `touchEventtouchEventtouchEvent`);
    if (event.type == Constants.TOUCH_TYPE_DOWN) {
      this.startX = event.touches[0].screenX;
      this.startY = event.touches[0].screenY;
      this.moveX = 0;
      this.moveY = 0;
      Log.showDebug(TAG, `touchStart2=======startX: ${this.startX}, startY: ${this.startY}`);
    } else if (event.type == Constants.TOUCH_TYPE_MOVE) {
      this.moveX = event.touches[0].screenX - this.startX;
      this.moveY = event.touches[0].screenY - this.startY;
    } else if (event.type == Constants.TOUCH_TYPE_UP) {
      Log.showDebug(TAG, `touchEnd2, moveX: ${this.moveX}, moveY: ${this.moveY}`);
      if (this.moveX < 5 && this.moveX > -5 && this.moveY < 5 && this.moveY > -5) {
        event.stopPropagation();
        this.mCapsuleViewModel.onClickEvent();
        this.mCapsuleViewModel.mIsBackground = false;
      };
    };
  }
}