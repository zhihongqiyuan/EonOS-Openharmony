/**
 * Copyright (c) 2023 iSoftStone Information Technology (Group) Co.,Ltd.
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
struct ListItem_InSertChange {
  @State testWidth: string = '100%';
  @State testHeight: string = '100%';
  private arr: number[] = [0, 1, 2, 3, 4];
  messageManager: MessageManager<string> = new MessageManager();
  private content: string = "ListItem_InSertChange Page";

  onPageShow() {
    console.info('ListItem_InSertChange page show called');
    globalThis.value = {
      name: 'messageManager', message: this.messageManager
    };
    let callback: Callback<string> = (message: Message<string>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'width') {
        this.testWidth = message.value;
      }
      if (message.name == 'height') {
        this.testHeight = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  onBuildDone() {
    console.info('ListItem_InSertChange page build done called');
  }

  build() {
    Column() {
      List({ space: 20, initialIndex: 0 }) {
        ListItem() {
          Text('1')
            .width(this.testWidth)
            .height(this.testHeight)
            .fontSize(16)
            .key('InSertChangeText1')
        }
        .width(300)
        .height(100)
        .key('ListItemTest71')
        .editable(true)
        .backgroundColor(0xF5DEB3)

        ListItem() {
          Text('2')
            .width('100%')
            .height('100%')
            .fontSize(16)
            .key('InSertChangeText2')
        }
        .width(300)
        .height(100)
        .key('ListItemTest72')
        .editable(true)
        .backgroundColor(0xD2B48C)

        ListItem() {
          Text('3')
            .width('100%')
            .height('100%')
            .fontSize(16)
            .key('InSertChangeText3')
        }
        .width(300)
        .height(100)
        .key('ListItemTest73')
        .editable(true)
        .backgroundColor(0xffa500)

        ListItem() {
          Text('4')
            .width('100%')
            .height('100%')
            .fontSize(16)
            .key('InSertChangeText4')
        }
        .width(300)
        .height(100)
        .key('ListItemTest74')
        .editable(true)
        .backgroundColor(0xFFFFFF)
      }
      .height(500)
      .width(350)
      .key('ListItemInSertChange1')
      .backgroundColor(0xAFEEEE)
      .editMode(true)
      .scrollBar(BarState.On)
      .onScrollIndex((firstIndex: number, lastIndex: number) => {
        console.info('first' + firstIndex)
        console.info('last' + lastIndex)
      })
    }.width('100%').height('100%').backgroundColor(0xDCDCDC)
  }
}