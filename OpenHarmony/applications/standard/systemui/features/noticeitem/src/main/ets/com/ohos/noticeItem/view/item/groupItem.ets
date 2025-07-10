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
import Constants, { NotificationItemData, NotificationLayout as Layout } from '../../common/constants';
import NotificationItem from './notificationItem'
import NotificationViewModel from '../../viewmodel/NotificationViewModel';
import SwipeLayout from './SwipeLayout';
import deviceInfo from '@ohos.deviceInfo';
import ExpandInfoManager from '../../model/ExpandInfoManager';
import {getId} from '../../model/SwipeLayoutUtils';
import {BottomLeftItem} from './iconListComponent';
import FocusCallBack from '../../model/SwipeLayoutUtils';

const TAG = 'NoticeItem-GroupItem';
const deviceTypeInfo = deviceInfo.deviceType;

@Component
export default struct GroupItem {
  @Consume('notificationUpdate') updateFlag: number;
  @State distributedDeviceName: string = '';
  @State @Watch('expandChange') toExpand: boolean = false;
  private groupData: NotificationItemData[] = [];

  registerGroupItemEventCapture(id: string){
    if(FocusCallBack.getLastFocusId() == getId(this.groupData[0], true)){
      //如果上次滑动的是该group, 它自身允许滑动， 但它的子控件不应该滑动
      if(id == getId(this.groupData[0], true)){
        return false;
      }
      return true;
    } else {
      return false;
    }
  }

  expandChange() {
    this.updateFlag++;
  }

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear`);
    this.toExpand = ExpandInfoManager.hasExpandInfo(this.groupData[0]);
  }

  aboutToDisappear() {
    Log.showInfo(TAG, `aboutToDisAppear`);
  }

  deleteButtonCallback(): void {
    ExpandInfoManager.deleteExpandInfo(this.groupData[0]);
    NotificationViewModel.removeGroupNotification(this.groupData[0], true);
  }

  @Builder SurfaceComponent() {
    Column() {
      Row() {
        Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
          Image(this.groupData[0].smallIcon)
            .objectFit(ImageFit.Contain)
            .width(Layout.TITLE_IMAGE_SIZE)
            .height(Layout.TITLE_IMAGE_SIZE)
          Row() {
            Text(this.groupData[0].appName)
              .fontColor($r("sys.color.ohos_fa_text_primary"))
              .fontSize($r("sys.float.ohos_id_text_size_body3"))
              .fontWeight(FontWeight.Regular)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .maxLines(Constants.SINGLE_LINE)
          }.margin({ left: 8 })
          Text(this.groupData[0].time)
            .fontColor($r("sys.color.ohos_fa_text_primary"))
            .fontSize($r("sys.float.ohos_id_text_size_body3"))
            .fontWeight(FontWeight.Regular)
            .maxLines(Constants.SINGLE_LINE)
            .flexGrow(0)
            .margin({ left: 8, right: 8 })

          Text($r('app.string.distributed_devicename', this.distributedDeviceName))
            .fontColor($r("sys.color.ohos_fa_text_primary"))
            .fontSize($r("sys.float.ohos_id_text_size_body3"))
            .fontWeight(FontWeight.Regular)
            .maxLines(Constants.SINGLE_LINE)
            .visibility((this.distributedDeviceName ? true : false) ? Visibility.Visible : Visibility.None)

          Blank()
          Image(this.toExpand ? $r('app.media.ic_notification_up') : $r('app.media.ic_notification_down'))
            .objectFit(ImageFit.Contain)
            .width(Layout.TITLE_IMAGE_SIZE)
            .height(Layout.TITLE_IMAGE_SIZE)
        }
        .margin({ left: 12, right: 12 })
        .height(Layout.TITLE_HEIGHT)
        .onClick(() => {
          if (this.toExpand) {
            this.toExpand = false;
            ExpandInfoManager.deleteExpandInfo(this.groupData[0])
          } else {
            this.toExpand = true;
            ExpandInfoManager.addExpandInfo(this.groupData[0])
          }
        })
      }
      .flexShrink(0)
      .backgroundColor($r('app.color.notificationitem_background'))
      .width(Constants.FULL_CONTAINER_WIDTH)

      if (this.toExpand) {
        Row().height($r("app.float.notice_divider")).width(Constants.FULL_CONTAINER_WIDTH)
      }
      Column() {
        ContentList({ groupData: this.groupData, toExpand: this.toExpand, registerEventCapture: this.registerGroupItemEventCapture.bind(this)  })
      }
      .width(Constants.FULL_CONTAINER_WIDTH)
    }
    .borderRadius($r("sys.float.ohos_id_corner_radius_default_l"))
    .clip(true)
  }

  @Builder BottomLeftComponent() {
    BottomLeftItem({itemData: this.groupData[0], bottomLeftItemHeight: 92});
  }

  build() {
    Column(){
      SwipeLayout({
        swipeLayoutId: getId(this.groupData[0], true),
        bottomLeftWidth: 80,
        bottomRightWidth: 60,
        leftThreshold: 100,
        bottomHeight: 92,
        deleteButtonCallback: this.deleteButtonCallback.bind(this),
        SurfaceComponent: this.SurfaceComponent.bind(this),
        BottomLeftComponent: this.BottomLeftComponent.bind(this),
        registerEventCapture: this.registerGroupItemEventCapture.bind(this)
      })
    }
    .width(Constants.FULL_CONTAINER_WIDTH)
    .borderRadius($r("sys.float.ohos_id_corner_radius_default_l"))
    .clip(true)
  }
}

@Component
struct ContentList {
  @State @Watch('remainderChange') groupData: NotificationItemData[] = [];
  @Prop toExpand: boolean;
  @State remainderNum: number= 0;
  registerEventCapture: (id: string) => boolean = (id) => true;

  registerContentListEventCapture(id: string){
    if(this.registerEventCapture != null && this.registerEventCapture(id)){
      return true;
    } else {
      return false;
    }
  }

  aboutToAppear() {
    this.remainderNum = this.groupData.length - 2;
    Log.showInfo(TAG, `aboutToAppear remainderNum:${this.remainderNum}`);
  }

  build() {
    Column() {
      if (!this.toExpand) {
        Column() {
          Row({ space: 5 }) {
            Text(this.groupData[0].title)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontColor($r("sys.color.ohos_fa_text_primary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .fontWeight(FontWeight.Medium)
              .lineHeight(Layout.TEXT_LINE_HEIGHT)
            Text(this.groupData[0].text)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontColor($r("sys.color.ohos_fa_text_secondary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .fontWeight(FontWeight.Regular)
              .lineHeight(Layout.TEXT_LINE_HEIGHT)
            Blank()
              .margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })
          }.width(Constants.FULL_CONTAINER_WIDTH)
          .margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })

          Row({ space: 5 }) {
            Text(this.groupData[1].title)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontColor($r("sys.color.ohos_fa_text_primary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .fontWeight(FontWeight.Medium)
              .lineHeight(Layout.TEXT_LINE_HEIGHT)
            Text(this.groupData[1].text)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.Ellipsis })
              .fontColor($r("sys.color.ohos_fa_text_secondary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .fontWeight(FontWeight.Regular)
              .lineHeight(Layout.TEXT_LINE_HEIGHT)
            Blank()
            Text('+' + this.remainderNum)
              .maxLines(Constants.SINGLE_LINE)
              .textOverflow({ overflow: TextOverflow.None })
              .fontColor($r("sys.color.ohos_fa_text_secondary"))
              .fontSize($r("sys.float.ohos_id_text_size_body2"))
              .visibility((this.remainderNum > 0) ? Visibility.Visible : Visibility.Hidden)
              .margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })
          }
          .width(Constants.FULL_CONTAINER_WIDTH)
          .margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })

          Blank()
            .height(Layout.ITEM_MARGIN)
        }.width(Constants.FULL_CONTAINER_WIDTH)
        .backgroundColor($r('app.color.notificationitem_background'))
        .alignItems(HorizontalAlign.Start)

      } else {
        List() {
          ForEach(this.groupData, (item: NotificationItemData) => {
            ListItem() {
              NotificationItem({ itemData: item, isSubItem: true, registerEventCapture: this.registerContentListEventCapture.bind(this) })
            }
          }, (item: NotificationItemData) => `${item.hashcode}_${item.timestamp}_subitem`)
        }
        .divider({
          strokeWidth: 1,
          color: $r('app.color.device_divider_color')
        })
        .align(Alignment.Top)
      }
    }
    .alignItems(HorizontalAlign.Start)
    .width(Constants.FULL_CONTAINER_WIDTH)
  }

  remainderChange() {
    this.remainderNum = this.groupData.length - 2;
    Log.showInfo(TAG, `remainderChange remainderNum:${this.remainderNum}`);
  }
}
