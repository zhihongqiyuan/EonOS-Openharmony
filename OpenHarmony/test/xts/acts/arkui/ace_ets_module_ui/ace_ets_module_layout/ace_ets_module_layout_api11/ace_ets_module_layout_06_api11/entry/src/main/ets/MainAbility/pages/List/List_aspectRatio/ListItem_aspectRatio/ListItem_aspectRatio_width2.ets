/**
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import { MessageManager, Callback, Message } from '../../../../common/MessageManager';

@Entry
@Component
struct ListItem_aspectRatio_width2 {
  @State aspectRatio1: number = 1;
  messageManager: MessageManager<number> = new MessageManager();

  onPageShow() {
    console.info('Grid_RowCol_ItemContentSize onPageShow');
    globalThis.value = {
      name: 'messageManager', message: this.messageManager
    };
    let callback: Callback<number> = (message: Message<number>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'aspectRatio1') {
        this.aspectRatio1 = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  build() {
    Column() {
      List({ space: 2 }) {
        ListItem() {
          Text('1')
            .width('100%')
            .height('20%')
            .fontSize(16)
            .borderRadius(10)
            .textAlign(TextAlign.Center)
            .backgroundColor(0xAFEEEE)
        }
        .width('60%')
        .aspectRatio(this.aspectRatio1)
        .key('ListItem_aspectRatio_width2_01')

        ListItem() {
          Text('2')
            .width('80%')
            .height(50)
            .fontSize(16)
            .borderRadius(10)
            .textAlign(TextAlign.Center)
            .backgroundColor(0xAFEEEE)
        }

        ListItem() {
          Text('3')
            .width('80%')
            .height(50)
            .fontSize(16)
            .borderRadius(10)
            .textAlign(TextAlign.Center)
            .backgroundColor(0xAFEEEE)
        }

        ListItem() {
          Text('4')
            .width('80%')
            .height(50)
            .fontSize(16)
            .borderRadius(10)
            .textAlign(TextAlign.Center)
            .backgroundColor(0xAFEEEE)
        }
      }
      .height('90%')
      .aspectRatio(0.5)
      .listDirection(Axis.Vertical)
      .scrollBar(BarState.On)
      .alignListItem(ListItemAlign.Center)
      .key('ListItem_aspectRatio_width2_list')
    }
    .width('100%')
    .height('100%')
  }
}