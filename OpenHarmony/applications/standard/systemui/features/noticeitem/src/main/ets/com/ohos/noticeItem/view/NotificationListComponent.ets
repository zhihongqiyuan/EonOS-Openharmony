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

import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import Constants, {NotificationLayout as Layout, NotificationItemData} from '../common/constants';
import NotificationItem from './item/notificationItem';
import GroupNotificationItem from './item/groupItem';
import ExpandInfoManager from '../model/ExpandInfoManager';

const TAG = 'NoticeItem-NotificationListComponent';

interface  NotificationLayoutConfig {
  itemMargin: number;
}

@Component
export default struct NotificationListComponent {
  // 当长通知消息展开时，这里用于重新刷新item布局，扩展高度
  @Provide('notificationUpdate') update: number = 0;
  @StorageLink('notificationList') notificationList:  NotificationItemData[][] = [];
  @Link listIsEnd: boolean
  private config: NotificationLayoutConfig = {
    itemMargin: Layout.ITEM_MARGIN
  };

  aboutToAppear() {
    Log.showDebug(TAG, `notificationList, aboutToAppear`);
  }

  aboutToDisappear() {
    Log.showDebug(TAG, `aboutToDisAppear`);
  }

  deleteExpandInfo(itemData: NotificationItemData){
    ExpandInfoManager.deleteExpandInfo(itemData);
    return true;
  }

  build() {
    List({ space: this.config.itemMargin }) {
      if (this.update && false) {
        ListItem() {
        }
      }
      ForEach(this.notificationList, (item: NotificationItemData[]) => {
        ListItem() {
          if (item.length > 1) {
            GroupNotificationItem({ groupData: item })
          } else {
            if(this.deleteExpandInfo(item[0])){
              //更新组展开信息
              NotificationItem({ itemData: item[0] })
            }
          }
        }
      }, (list: NotificationItemData[]) => JSON.stringify(list.map((item) => `${item.hashcode}_${item.timestamp}`)))
    }
    .onReachEnd(() => {
      this.listIsEnd = true;
    })
    .onScroll(() => {
      this.listIsEnd = false;
    })
    .align(Alignment.Top)
    .width('100%')
    .height('100%')
  }
}