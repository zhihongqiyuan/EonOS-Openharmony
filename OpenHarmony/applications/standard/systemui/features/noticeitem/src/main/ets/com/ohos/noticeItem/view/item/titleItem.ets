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

import Constants, { NotificationLayout as Layout } from '../../common/constants';
import Log from '../../../../../../../../../../common/src/main/ets/default/Log';
import CheckEmptyUtils from '../../../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';
const TAG = 'NoticeItem-TitleItem';

@Component
export default struct TitleItem {
  private notificationSmallIcon?: PixelMap
  @State notificationName: string = ''
  @State notificationTime: string = ''
  @Link isExpand: boolean
  @Prop needExpand: boolean
  @State rowSpace: Resource = $r('app.float.titleitem_row_space')
  @Prop distributedDeviceName: string
  private isSubItem: boolean= false;
  private clickTitleAction?: () => void

  aboutToAppear() {
    Log.showDebug(TAG, `aboutToAppear Start`);
  }

  aboutToDisappear() {
    Log.showDebug(TAG, `aboutToDisAppear`);
  }

  build() {
    Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
      if (!this.isSubItem) {
        if (this.notificationSmallIcon) {
          Image(this.notificationSmallIcon)
            .objectFit(ImageFit.Contain)
            .width(Layout.TITLE_IMAGE_SIZE)
            .height(Layout.TITLE_IMAGE_SIZE)
        }
        Row() {
          Text(this.notificationName)
            .fontColor($r("sys.color.ohos_fa_text_primary"))
            .fontSize($r("sys.float.ohos_id_text_size_body3"))
            .fontWeight(FontWeight.Regular)
            .textOverflow({ overflow: TextOverflow.Ellipsis })
            .maxLines(Constants.SINGLE_LINE)
        }.margin({ left: 8 })
      }
      Text(this.notificationTime)
        .fontColor($r("sys.color.ohos_fa_text_primary"))
        .fontSize($r("sys.float.ohos_id_text_size_body3"))
        .fontWeight(FontWeight.Regular)
        .maxLines(Constants.SINGLE_LINE)
        .flexGrow(0)
        .margin({ left: !this.isSubItem ? 8 : 0, right: 8 })

      Text($r('app.string.distributed_devicename', this.distributedDeviceName))
        .fontColor($r("sys.color.ohos_fa_text_primary"))
        .fontSize($r("sys.float.ohos_id_text_size_body3"))
        .fontWeight(FontWeight.Regular)
        .maxLines(Constants.SINGLE_LINE)
        .visibility((this.distributedDeviceName ? true : false) ? Visibility.Visible : Visibility.None)

      Blank()
      if (this.needExpand) {
        Image(this.isExpand ? $r('app.media.ic_notification_up') : $r('app.media.ic_notification_down'))
          .objectFit(ImageFit.Contain)
          .width(Layout.TITLE_IMAGE_SIZE)
          .height(Layout.TITLE_IMAGE_SIZE)
      }
    }
    .margin({ left: 12, right: 12 })
    .height(Layout.TITLE_HEIGHT)
    .onClick(this.clickTitleItem.bind(this))
  }

  clickTitleItem() {
    Log.showInfo(TAG, 'clickTitleItem');
    if (this.needExpand) {
      if (this.isExpand) {
        this.isExpand = false;
      } else {
        this.isExpand = true;
      }
      if (this.clickTitleAction) {
        this.clickTitleAction();
      }
    }
  }
}
