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
import NotificationViewModel from '../../viewmodel/NotificationViewModel';
import NotificationManager from '../../model/NotificationManager';
import {NotificationLayout as Layout} from '../../common/constants';
import { NotificationItemData } from '@ohos/common'
import NtfMgr from '@ohos.notificationManager';

const TAG = 'NoticeItem-CustomItem';

@Component
export default struct CustomItem {
  private customItemData: NotificationItemData | undefined = undefined;
  private template: PluginComponentTemplate | undefined = undefined;
  private templateData: Record<string, Object> = {};
  private isSubItem: boolean= false;
  private isDebugMode: boolean = false;
  private clickAction: () => void = () => {}

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start`);
    this.template = NotificationViewModel.getPluginTempLate(this.customItemData?.template?.name);
    this.templateData = this.customItemData?.template?.data as Record<string, Object>;
    this.isDebugMode = NotificationManager.isDebugMode(TAG);
    Log.showInfo(TAG, `template = ${JSON.stringify(this.template)} templateData = ${JSON.stringify(this.templateData)} isDebugMode = ${this.isDebugMode}`);
  }

  build() {
    Stack({ alignContent: Alignment.TopEnd }) {
      Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
        PluginComponent({
          template: this.template,
          data: this.templateData
        }).onComplete(() => {
          Log.showInfo(TAG, `Complete`);
        }).onError((error) => {
          Log.showInfo(TAG, `Error code:${error.errcode} message:${error.msg}`);
        })
          .size({ width: '100%', height: 110 })
      }
      .margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })
      .onClick(() => {
        this.clickAction();
      })

      if (this.isDebugMode) {
        Column() {
          Text("-- DEBUG --")
            .fontSize('20')
            .fontWeight(FontWeight.Bold)
            .fontColor(0xCCCCCC)
            .opacity(0.5)
        }.margin(10)
      }
    }
    .borderRadius(!this.isSubItem ? $r('app.float.item_borderradius') : 0)
    .clip(true)
  }
}
