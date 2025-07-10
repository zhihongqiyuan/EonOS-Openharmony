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

import GlobalContext from '../../../../common/GlobalContext';
import { Value } from '../../../../common/Value';
import { MessageManager, Callback, Message } from '../../../../common/MessageManager';

@Entry
@Component
struct AlignSpaceEvenly_AddMargin {
  @State DadMargin: number = 10;
  messageManager: MessageManager<number> = new MessageManager();

  onPageShow() {
    console.info('AlignSpaceEvenly_AddMargin onPageShow');
    let value: Value<number> = {
      name: 'messageManager', message: this.messageManager
    };
    GlobalContext.getContext().setObject('value', value);
    let callback: Callback<number> = (message: Message<number>) => {
      console.log('message = ' + message.name + "--" + message.value);
      if (message.name == 'DadMargin') {
        this.DadMargin = message.value;
      }
    };
    this.messageManager.registerCallback(callback);
  }

  @State justifyContent: FlexAlign = FlexAlign.SpaceEvenly;

  build() {
    Column() {
      Flex({ justifyContent: this.justifyContent, direction: FlexDirection.Row }) {
        Text('1').width(150).height(50).backgroundColor(0xF5DEB3).key("SpaceEvenly_AddMargin_011");
        Text('2').width(150).height(100).backgroundColor(0xD2B48C).key("SpaceEvenly_AddMargin_012");
        Text('3').width(150).height(150).backgroundColor(0xF5DEB3).key("SpaceEvenly_AddMargin_013");
      }
      .key("SpaceEvenly_AddMargin_01")
      .width(500)
      .height(200)
      .margin(this.DadMargin)
      .backgroundColor(0xAFEEEE)
    }
    .key("SpaceEvenly_AddMargin_01_Box")
    .width("100%")
    .alignItems(HorizontalAlign.Start)
  }
}
