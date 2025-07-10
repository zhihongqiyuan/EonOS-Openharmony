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
struct AlignContent_End_FlexP_Fixed_Visibility {
  @State textVisibility: number = Visibility.Visible;
  messageManager: MessageManager<number> = new MessageManager();

  onPageShow() {
    console.info('AlignContentFlex onPageShow');
    let value: Value<number> = {
      name: 'messageManager', message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.error('message = ' + message.name + "--" + message.value);
      if (message.name == 'visibility') {
        this.textVisibility = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  build() {
    Column() {
      Text('AlignContent_End_FlexP_Fixed_Visibility').fontSize(9).fontColor(0xCCCCCC).width('90%')
      Flex({
        direction: FlexDirection.Row,
        wrap: FlexWrap.Wrap,
        alignContent: FlexAlign.End,
        alignItems: ItemAlign.Start
      }) {
        Text('1')
          .width(150)
          .height(50)
          .backgroundColor(0xF5DEB3)
          .key('AlignContent_End_FlexP_Fixed_Visibility_1')
          .visibility(this.textVisibility)
        Text('2').width(150).height(100).backgroundColor(0xD2B48C).key('AlignContent_End_FlexP_Fixed_Visibility_2')
        Text('3').width(150).height(150).backgroundColor(0xF5DEB3).key('AlignContent_End_FlexP_Fixed_Visibility_3')
        Text('4').width(150).height(200).backgroundColor(0xD2B48C).key('AlignContent_End_FlexP_Fixed_Visibility_4')
      }
      .key('AlignContent_End_FlexP_Fixed_Visibility_flex001')
      .width(500)
      .height(400)
      .backgroundColor(0xAFEEEE)

    }.width('100%')
  }
}
