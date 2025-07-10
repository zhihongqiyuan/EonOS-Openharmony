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
struct Grid_RowCol_ItemPosition {
  @State secondGridItemPosition: Position = { x: 0, y: 0 };
  messageManager: MessageManager<Position> = new MessageManager();

  onPageShow() {
    console.info('Grid_RowCol_ItemPosition onPageShow');
    globalThis.value = {
      name: 'messageManager',
      message: this.messageManager
    };
    let callback: Callback<Position> = (message: Message<Position>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'secondGridItemPosition') {
        this.secondGridItemPosition = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  build() {
    Column({ space: 30 }) {
      Grid() {
        GridItem() {
          Text('1').width('100%').height('100%').backgroundColor(0xF5DEB3);
        }
        .key('Grid_RowCol_ItemPosition01')

        GridItem() {
          Text('2').width('100%').height('100%').backgroundColor(0xD2B48C);
        }
        .position(this.secondGridItemPosition)
        .key('Grid_RowCol_ItemPosition02')

        GridItem() {
          Text('3').width('100%').height('100%').backgroundColor(0xD2B48C);
        }
        .key('Grid_RowCol_ItemPosition03')

        GridItem() {
          Text('4').width('100%').height('100%').backgroundColor(0xF5DEB3);
        }
        .key('Grid_RowCol_ItemPosition04')
      }
      .width(300)
      .height(300)
      .columnsTemplate('1fr 1fr')
      .rowsTemplate('1fr 1fr')
      .backgroundColor(0xAFEEEE)
      .key('Grid_RowCol_ItemPosition_Container01')
    }
    .width('100%')
    .height('100%')
  }
}