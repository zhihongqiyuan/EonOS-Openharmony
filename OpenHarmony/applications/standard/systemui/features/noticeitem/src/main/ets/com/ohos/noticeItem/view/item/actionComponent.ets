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

import Log from '../../../../../../../../../../common/src/main/ets/default/Log';
import CheckEmptyUtils from '../../../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';
import NotificationViewModel from '../../viewmodel/NotificationViewModel';
import Constants, {InputActionButtonData, NotificationLayout as Layout, NotificationItemData} from '../../common/constants';
import notificationManager from '@ohos.notificationManager';
import type { WantAgent } from '@kit.AbilityKit';

const TAG = 'NoticeItem-ActionComponent';

class actionInputData {
  inputKey?: string;
  wantAgent?: WantAgent
}

@Component
export default struct ActionComponent {
  @State showInputText: boolean = false;
  @State placeholder: string = '';
  @State inputData: actionInputData | undefined = undefined;
  @State inputActionData: InputActionButtonData = new InputActionButtonData();
  @Prop itemData: NotificationItemData;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start`);
    let actionData = AppStorage.Get(Constants.KEY_INPUT + this.itemData.id) as  actionInputData;
    this.inputActionData = (actionData instanceof InputActionButtonData) ? actionData : this.inputActionData;
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisAppear`);
  }

  build() {
    Flex({ direction: FlexDirection.Row, justifyContent: FlexAlign.Start, alignItems: ItemAlign.Center }) {
      if (!this.inputData) {
        ForEach(this.itemData.actionButtons, (item: notificationManager.NotificationActionButton) => {
          ActionButton({
            content: item.title,
            clickEvent: () => {
              if (CheckEmptyUtils.isEmpty(item.userInput)) {
                NotificationViewModel.clickItem(this.itemData, item.wantAgent);
              } else {
                this.inputData = { inputKey: item.userInput?.inputKey, wantAgent: item.wantAgent };
              }
            }
          })
        })
      } else {
        TextInput({ placeholder: this.placeholder, text: this.inputActionData.content })
          .type(InputType.Normal)
          .caretColor(Color.Green)
          .enterKeyType(EnterKeyType.Send)
          .height(Layout.INPUT_TEXT_HEIGHT)
          .enabled(this.inputActionData.isSending ? false : true)
          .flexGrow(1)
          .margin({ right: Layout.ACTION_MARGIN })
        Image($r("app.media.send"))
          .objectFit(ImageFit.Contain)
          .width(Layout.INPUT_IMAGE_SIZE)
          .height(Layout.INPUT_IMAGE_SIZE)
          .onClick(() => {
            NotificationViewModel.clickReply(this.inputData?.inputKey, this.placeholder, this.inputData?.wantAgent);
            this.inputActionData = {
              isSending: true,
              content: this.placeholder
            };
            AppStorage.SetOrCreate(Constants.KEY_INPUT + this.itemData.id, this.inputActionData);
          })
      }
    }.height(!this.inputData ? Layout.ACTION_HEIGHT : Layout.INPUT_LAYOUT_HEIGHT)
  }
}

@Component
struct ActionButton {
  @State isTouched: boolean = false;
  @Prop content: string;
  clickable: boolean = true;
  clickEvent: () => void = () => {
  };

  build() {
    Text(this.content)
      .fontSize($r("sys.float.ohos_id_text_size_button2"))
      .fontColor($r("sys.color.ohos_id_color_text_primary_activated"))
      .borderRadius($r("sys.float.ohos_fa_corner_radius_default_s"))
      .height(Layout.ACTION_HEIGHT)
      .padding({ left: Layout.ACTION_MARGIN, right: Layout.ACTION_MARGIN })
      .backgroundColor(this.isTouched ? $r("sys.color.ohos_fa_click_effect") : $r("app.color.transparent"))
      .onTouch((event: TouchEvent) => {
        if (!this.clickable) {
          return;
        }
        if (event.type == TouchType.Down) {
          this.isTouched = true;
        } else if (event.type == TouchType.Up) {
          this.isTouched = false;
        }
      })
      .onClick(() => {
        if (!this.clickable) {
          return;
        }
        this.clickEvent();
      })
  }
}