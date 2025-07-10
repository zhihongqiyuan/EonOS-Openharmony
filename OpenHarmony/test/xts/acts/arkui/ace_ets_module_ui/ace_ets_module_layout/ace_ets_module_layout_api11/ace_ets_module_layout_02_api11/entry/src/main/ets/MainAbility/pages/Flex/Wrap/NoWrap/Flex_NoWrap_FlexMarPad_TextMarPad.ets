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

import GlobalContext from '../../../../common/GlobalContext';
import { Value } from '../../../../common/Value';
import { MessageManager, Callback, Message } from '../../../../common/MessageManager';

@Entry
@Component
struct Flex_NoWrap_FlexMarPad_TextMarPad {
  @State firstTextPadding: number = 0;
  @State secondTextPadding: number = 0;
  @State thirdTextPadding: number = 0;
  @State firstTextMargin: number = 0;
  @State secondTextMargin: number = 0;
  @State thirdTextMargin: number = 0;
  @State flexMargin: number = 0;
  @State flexPadding: number = 0;
  messageManager: MessageManager<number> = new MessageManager();

  onPageShow() {
    console.info('Flex_NoWrap_FlexMarPad_TextMarPad onPageShow');
    let value: Value<number> = {
      name: 'messageManager',
      message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'firstTextPadding') {
        this.firstTextPadding = message.value;
      }
      if (message.name == 'secondTextPadding') {
        this.secondTextPadding = message.value;
      }
      if (message.name == 'thirdTextPadding') {
        this.thirdTextPadding = message.value;
      }
      if (message.name == 'firstTextMargin') {
        this.firstTextMargin = message.value;
      }
      if (message.name == 'secondTextMargin') {
        this.secondTextMargin = message.value;
      }
      if (message.name == 'thirdTextMargin') {
        this.thirdTextMargin = message.value;
      }
      if (message.name == 'flexMargin') {
        this.flexMargin = message.value;
      }
      if (message.name == 'flexPadding') {
        this.flexPadding = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  build() {
    Column({ space: 30 }) {
      Flex({ wrap: FlexWrap.NoWrap }) {
        Text('1')
          .width(150)
          .height(100)
          .backgroundColor(0xF5DEB3)
          .margin(this.firstTextMargin)
          .padding(this.firstTextPadding)
          .key('NoWrap_FlexMarPad_TextPad1');
        Text('2')
          .width(150)
          .height(150)
          .backgroundColor(0xD2B48C)
          .margin(this.secondTextMargin)
          .padding(this.secondTextPadding)
          .key('NoWrap_FlexMarPad_TextPad2');
        Text('3')
          .width(150)
          .height(200)
          .backgroundColor(0xF5DEB3)
          .margin(this.thirdTextMargin)
          .padding(this.thirdTextPadding)
          .key('NoWrap_FlexMarPad_TextPad3');
      }
      .width(500)
      .height(200)
      .margin(this.flexMargin)
      .padding(this.flexPadding)
      .backgroundColor(0xAFEEEE)
      .key('FlexNoWrap_FlexMarPad_TextMarPad_Container01')
    }
    .alignItems(HorizontalAlign.Start)
    .width('100%')
    .height('100%')
    .key('Column_Container01')
  }
}