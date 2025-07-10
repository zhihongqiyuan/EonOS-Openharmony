/*
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

import GlobalContext from '../../../../common/GlobalContext';
import { Value } from '../../../../common/Value';
import { MessageManager, Callback, Message } from '../../../../common/MessageManager';

@Entry
@Component
struct AlignContent_FlexAlign_Start_Margin_padding {
  @State flexPadding: number = 10;
  messageManager: MessageManager<number> = new MessageManager();

  onPageShow() {
    console.info('AlignContentFlex onPageShow');
    let value: Value<number> = {
      name: 'messageManager', message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.error('message = ' + message.name + "--" + message.value);
      if (message.name == 'padding') {
        this.flexPadding = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  build() {
    Column() {
      Column() {
        Flex({
          direction: FlexDirection.Row,
          wrap: FlexWrap.Wrap,
          alignContent: FlexAlign.Start,
          alignItems: ItemAlign.Start
        }) {
          Text('1').width(150).height(50).backgroundColor(0xF5DEB3)
            .key('AlignContentFlexStartMargin&padding_flex001_1')
          Text('2').width(150).height(100).backgroundColor(0xD2B48C)
            .key('AlignContentFlexStartMargin&padding_flex001_2')
          Text('3').width(150).height(150).backgroundColor(0xD2B48C)
            .key('AlignContentFlexStartMargin&padding_flex001_3')
          Text('4').width(150).height(200).backgroundColor(0xF5DEB3)
            .key('AlignContentFlexStartMargin&padding_flex001_4')
        }
        .key('AlignContentFlexStartMargin&padding_flex001')
        .width(400)
        .height(360)
        .backgroundColor(0xAFEEEE)
        .margin(10)
        .padding(this.flexPadding)

      }.key('AlignContentFlexStartMargin&padding_Column_1')

    }
    .width('100%')
    .key('AlignContentFlexStartMargin&padding_Column')
  }
}
