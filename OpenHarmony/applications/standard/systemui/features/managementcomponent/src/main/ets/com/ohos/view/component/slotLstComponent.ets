/**
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
import ConfigData from '../../common/constants';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import SlotItemComponent from './slotItemComponent';
import Notification from '@ohos.notification';
import Router from '@system.router'
import { BundleItemData } from '@ohos/common';
import NotificationManager from '@ohos.notificationManager';

const TAG = 'ManagementComponent-SlotLstComponent';

@Component
export default struct SlotLstComponent {
  @Prop appInfo: BundleItemData;
  @Link slotLst: NotificationManager.NotificationSlot[];

  build() {
    Flex({ justifyContent: FlexAlign.SpaceBetween }) {
      Column() {
        Row() {
          Text($r('app.string.slotType'))
            .fontColor($r('sys.color.ohos_id_color_text_secondary'))
            .fontSize($r('sys.float.ohos_id_text_size_body2'))
            .fontWeight(FontWeight.Medium)
            .width(ConfigData.WH_100_100)
            .maxLines(ConfigData.MAX_LINES_1)
            .textOverflow({ overflow: TextOverflow.Ellipsis })
            .padding({ left: $r('sys.float.ohos_id_max_padding_start'), right: $r('sys.float.ohos_id_max_padding_end')})
        }.height($r('app.float.notice_title_height'))
        .margin({ top: $r('app.float.page_notice_part_margin_t')})
        .align(Alignment.Start);
        Row() {
          List() {
            ForEach(this.slotLst, (item: NotificationManager.NotificationSlot) => {
              ListItem() {
                SlotItemComponent({ slotType: item.type })
              }
              .onClick(() => {
                Log.showInfo(TAG, `onClick`);
                Router.push({ uri: this.appInfo.slotSettingUrl, params: {appInfo:this.appInfo, slotInfo:item} });
              })
            });
          }.divider({
            strokeWidth: 1,
            color: $r('app.color.divider_color'),
            startMargin: $r('app.float.divider_margin_l'),
            endMargin: $r('app.float.divider_margin_r')
          }).width(ConfigData.WH_100_100)
          .visibility(Visibility.Visible)
          .zIndex(0)
          .border({ width: $r('app.float.border_width'), color: Color.White,
            radius: $r('app.float.border_radius') })
          .backgroundColor(Color.White)
          .flexShrink(1)
        }
        .padding({ left: $r('sys.float.ohos_id_default_padding_start'), right: $r('sys.float.ohos_id_default_padding_end')})
        .margin({ top: $r('app.float.page_notice_title_margin_t')})
        .align(Alignment.Start);
      }
    }
    .width(ConfigData.WH_100_100);
  }

  aboutToAppear(): void{
    Log.showInfo(TAG, `aboutToAppear Notification.getSlotsByBundle  bundle:` + 'bundle:' + this.appInfo.appBundleName + 'uid' + this.appInfo.appUid);
  }

  onBackPress() {
    Log.showDebug(TAG, `onBackPress`);
    Router.back();
  }
}