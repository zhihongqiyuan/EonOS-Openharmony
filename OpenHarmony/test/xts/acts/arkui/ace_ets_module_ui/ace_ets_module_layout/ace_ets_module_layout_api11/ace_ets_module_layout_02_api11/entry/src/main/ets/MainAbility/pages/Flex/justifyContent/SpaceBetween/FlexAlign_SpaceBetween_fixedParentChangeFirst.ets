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
struct FlexAlign_SpaceBetween_fixedParentChangeFirst {
  @State testFirstlayoutWeight: number = 0;
  @State testSecondlayoutWeight: number = 0;
  @State testThirdlayoutWeight: number = 0;
  messageManager: MessageManager<number> = new MessageManager();
  private content: string = "FlexAlign_SpaceBetween_fixedParentChangeFirst Page";

  onPageShow() {
    console.info('FlexAlign_SpaceBetween_fixedParentChangeFirst page show called');
    let value: Value<number> = {
      name: 'messageManager',
      message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'testFirstlayoutWeight') {
        this.testFirstlayoutWeight = message.value;
      }
      if (message.name == 'testSecondlayoutWeight') {
        this.testSecondlayoutWeight = message.value;
      }
      if (message.name == 'testThirdlayoutWeight') {
        this.testThirdlayoutWeight = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  onBuildDone() {
    console.info('FlexAlign_SpaceBetween_fixedParentChangeFirst page build done called');
  }

  build() {
    Column() {
      Text('justifyContent:Align_SpaceBetween12').fontSize(9).fontColor(0xCCCCCC).width('90%')
      Flex({ justifyContent: FlexAlign.SpaceBetween }) {
        Text('1')
          .width(150)
          .height(50)
          .backgroundColor(0xF5DEB3)
          .key('AlignSpaceBetween34')
          .layoutWeight(this.testFirstlayoutWeight)
        Text('2')
          .width(150)
          .height(100)
          .backgroundColor(0xD2B48C)
          .key('AlignSpaceBetween35')
          .layoutWeight(this.testSecondlayoutWeight)
        Text('3')
          .width(150)
          .height(150)
          .backgroundColor(0xffa500)
          .key('AlignSpaceBetween36')
          .layoutWeight(this.testThirdlayoutWeight)
      }
      .key('FlexAlignSpaceBetween12')
      .height(200)
      .width(500)
      .backgroundColor(0xAFEEEE)
    }.width('100%')
  }
}