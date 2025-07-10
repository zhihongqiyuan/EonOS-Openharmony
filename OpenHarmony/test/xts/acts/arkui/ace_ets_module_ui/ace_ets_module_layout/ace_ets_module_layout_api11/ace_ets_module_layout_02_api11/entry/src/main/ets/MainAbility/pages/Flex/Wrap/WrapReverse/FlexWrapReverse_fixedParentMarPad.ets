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
struct FlexWrapReverse_fixedParentMarPad {
  @State testPadding: number = 0;
  @State testMargin: number = 0;
  messageManager: MessageManager<number> = new MessageManager();
  private content: string = "FlexWrapReverse_fixedParentMarPad Page";

  onPageShow() {
    console.info('FlexWrapReverse_fixedParentMarPad page show called');
    let value: Value<number> = {
      name: 'messageManager',
      message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'padding') {
        this.testPadding = message.value;
      }
      if (message.name == 'margin') {
        this.testMargin = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  onBuildDone() {
    console.info('FlexWrapReverse_fixedParentMarPad page build done called');
  }

  build() {
    Column() {
      Text('Wrap:WrapReverseTest16').fontSize(9).fontColor(0xCCCCCC).width('90%')
      Flex({ direction: FlexDirection.Row, wrap: FlexWrap.WrapReverse }) {
        Text('1')
          .width(150)
          .height(50)
          .backgroundColor(0xF5DEB3)
          .padding(this.testPadding)
          .margin(this.testMargin)
          .key('WrapReverseTest46')
        Text('2').width(150).height(100).backgroundColor(0xD2B48C).key('WrapReverseTest47')
        Text('3').width(150).height(150).backgroundColor(0xffa500).key('WrapReverseTest48')
      }
      .key('FlexWrapReverseTest16')
      .height(300)
      .width(500)
      .padding(50)
      .margin(20)
      .backgroundColor(0xAFEEEE)
    }.width('100%')
  }
}