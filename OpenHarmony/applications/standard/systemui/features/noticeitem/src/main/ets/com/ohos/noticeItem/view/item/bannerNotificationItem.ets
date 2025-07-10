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
import AbilityManager from '../../../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import WindowManager, { WindowType } from '../../../../../../../../../../common/src/main/ets/default/WindowManager';
// import {notificationData} from '../../../../../../../../../../common/src/main/ets/plugindatasource/common/Constants'
import { NotificationItemData } from '@ohos/common'
import Constants, {NotificationLayout as Layout} from '../../common/constants';
import NotificationViewModel from '../../viewmodel/NotificationViewModel';
import CustomItem from './customItem';
import titleItem from './titleItem';
import basicItem from './basicItem';
import longItem from './longItem';
import multiItem from './multiItem';
import pictureItem from './pictureItem';
import ActionComponent from './actionComponent';
import DevicesDialog from './devicesDialog';
import WantAgent from '@ohos.wantAgent';
import { Rect } from '@ohos/common';

const TAG = 'NoticeItem-BannerNotificationItem';
const INTERVAL = 5 * 1000;

class EmptyObject {}

interface TriggerInfo {
  code: number;
  want: Want | undefined;
  permission: string;
  extraInfo: EmptyObject;
}


@Component
export default struct BannerNotificationItem {
  @State want : NotificationItemData | undefined = undefined;
  @Link distributedDeviceName: string;
  @Link mBannerNoticeCnt: number;
  @Link mLastActionTime: number;
  @Link needExpand: boolean;
  @Link isExpand: boolean;
  @State mCloseEnableFlg: boolean = false;
  private nowWant: Want | undefined = undefined;
  private onBannerNoticeHide: () => void = () => {};
  private devicesDialogController: CustomDialogController | null = new CustomDialogController({
    builder: DevicesDialog({
      action: (deviceID): void => this.selectedDevice(deviceID)
    }),
    autoCancel: false,
    offset: { dx: 0, dy: 200 }
  });

  mInterval: number = INTERVAL;
  mDefaultBannerRect: Rect = {
    left: 0,
    top: 0,
    width: 0,
    height: 0
  };
  expandPaddingBottom: number = Layout.EXPAND_PADDING_BOTTOM;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear Start`);
    WindowManager.setWindowBgColor(WindowType.BANNER_NOTICE,'#00000000')
    this.mCloseEnableFlg = false;
    this.mDefaultBannerRect = AbilityManager.getAbilityData(AbilityManager.ABILITY_NAME_BANNER_NOTICE, 'bannerRect');
    setTimeout(() => {
      this.hideWindowForTimeout();
    }, this.mInterval);
    Log.showInfo(TAG, `aboutToAppear End`);
  }

  aboutToDisappear() {
    this.devicesDialogController = null;
    Log.showInfo(TAG, `aboutToDisAppear`);
  }

  build() {
    Column() {
      if (this.mBannerNoticeCnt > 0) {
        if (this.want?.template) {
          CustomItem({ customItemData: this.want, isSubItem: false })
        } else {
          Row() {
            Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
              Row() {
                titleItem({
                  notificationSmallIcon: this.want?.smallIcon as PixelMap,
                  notificationName: this.want?.appName,
                  notificationTime: this.want?.time,
                  isExpand: $isExpand,
                  needExpand: this.needExpand,
                  distributedDeviceName: this.distributedDeviceName,
                  isSubItem: false,
                  clickTitleAction: (): void => this.refreshLastActionTime()
                })
              }

              if (this.mCloseEnableFlg) {
                Row() {
                  Image($r('app.media.ic_close'))
                    .objectFit(ImageFit.Contain)
                    .width(Layout.TITLE_IMAGE_SIZE)
                    .height(Layout.TITLE_IMAGE_SIZE)
                    .margin({ left: Layout.ICON_MARGIN })
                    .onClick(this.clickCloseIcon.bind(this))
                }
              }
            }
            .margin({ left: 12, right: 12 })
            .height(Layout.TITLE_HEIGHT)
          }
          Row() {
            ContentComponent({
              isExpand: this.isExpand,
              itemData: this.want
            });
            if (this.isExpand && this.want?.actionButtons.length) {
              ActionComponent({ itemData: this.want })
            } else {
              Blank()
                .height(Layout.ITEM_MARGIN)
            }
          }
          .padding({
            bottom: $r('app.float.item_paddingbottom')
          })
          .onClick(this.showDevicesDialog.bind(this))
          .onAreaChange((e, e2) => {
            Log.showInfo(TAG, `onAreaChange, e: ${JSON.stringify(e)} e2: ${JSON.stringify(e2)}`);
            let heightEx = parseInt(String(e['height']));
            let heightCur = parseInt(String(e2['height']));
            let heightWin = this.mDefaultBannerRect?.height;
            if (this.isExpand) {
              heightWin = heightWin as number + (heightCur - heightEx) + this.expandPaddingBottom;
            }
            let bannerRect: Rect = {
              top: this.mDefaultBannerRect.top,
              left: this.mDefaultBannerRect.left,
              width: this.mDefaultBannerRect.width,
              height: heightWin as number
            };
            WindowManager.resetSizeWindow(WindowType.BANNER_NOTICE, bannerRect);
            WindowManager.showWindow(WindowType.BANNER_NOTICE);
          })
        }
      } else {
        Row() {
          Blank()
        }
      }
    }
    .width('100%')
    .height('100%')
    .borderRadius($r("sys.float.ohos_id_corner_radius_default_l"))
    .backgroundColor($r('app.color.notificationitem_background'))
    .clip(true)
    .onHover((isHover) => {
      this.mCloseEnableFlg = isHover;
    })
  }

  isCreateNewNotification(notificationCount:number):boolean{
    Log.showInfo(TAG, `isCreateNewNotification, notificationCount: ${notificationCount}`);
    return true;
  }

  hideWindowForTimeout() {
    Log.showInfo(TAG, `check need hide window or not.`);
    if ((new Date()).getTime() - this.mLastActionTime >= this.mInterval) {
      if (this.mCloseEnableFlg) {
        this.mLastActionTime = (new Date()).getTime();
        setTimeout(() => {
          this.hideWindowForTimeout();
        }, this.mInterval);
        return;
      }
      this.onBannerNoticeHide();
    } else {
      setTimeout(() => {
        this.hideWindowForTimeout();
      }, this.mInterval);
    }
  }

  clickCloseIcon() {
    Log.showInfo(TAG, 'clickCloseIcon');
    this.onBannerNoticeHide();
  }

  showDevicesDialog() {
    Log.showInfo(TAG, `showDevicesDialog`);
    if (!this.want?.distributedOption?.isDistributed) {
      NotificationViewModel.clickItem(this.want);
      this.onBannerNoticeHide();
      return;
    }
    let wantAgent = this.want?.want;
    if (!!wantAgent) {
      WantAgent.getWant(wantAgent).then((wantData) => {
        this.nowWant = wantData
        if (!wantData?.deviceId) {
          this.devicesDialogController?.open();
        } else{
          NotificationViewModel.clickItem(this.want);
        }
        this.onBannerNoticeHide();
      })
    }
  }

  selectedDevice(deviceID: string) {
    Log.showInfo(TAG, `selectedDevice deviceID:${deviceID}`);
    if (this.nowWant) {
      this.nowWant.deviceId = deviceID;
    }
    let triggerInfo: TriggerInfo = {
      code: 0,
      want: this.nowWant,
      permission: '',
      extraInfo: {}
    };
    NotificationViewModel.clickDistributionItem(this.want, triggerInfo)
  }

  refreshLastActionTime() {
    this.mLastActionTime = (new Date()).getTime();
    setTimeout(() => {
      this.hideWindowForTimeout();
    }, this.mInterval);
  }

}

@Component
struct ContentComponent {
  @Prop isExpand: boolean;
  @Prop itemData: NotificationItemData;

  aboutToAppear() {
    Log.showInfo(TAG, `aboutToAppear NotificationItemData: ${JSON.stringify(this.itemData)}`);
  }

  build() {
    Flex({ alignItems: ItemAlign.Center, justifyContent: FlexAlign.Start }) {
      Row() {
        if (this.itemData.contentType === Constants.NOTIFICATION_TYPE_BASIC) {
          basicItem({
            itemData: this.itemData
          });
        }
        if (this.itemData.contentType === Constants.NOTIFICATION_TYPE_LONG) {
          longItem({
            itemData: this.itemData,
            isExpand: this.isExpand
          });
        }
        if (this.itemData.contentType === Constants.NOTIFICATION_TYPE_MULTILINE) {
          multiItem({
            itemData: this.itemData,
            isExpand: this.isExpand
          });
        }
        if (this.itemData.contentType === Constants.NOTIFICATION_TYPE_PICTURE) {
          pictureItem({
            itemData: this.itemData,
            isExpand: this.isExpand
          });
        }
      }.flexGrow(1)

      if (this.itemData.picture) {
        Image(this.itemData.picture)
          .objectFit(ImageFit.Contain)
          .width(Layout.NOTIFICATION_PIC_SIZE)
          .height(Layout.NOTIFICATION_PIC_SIZE)
      }
    }.margin({ left: Layout.ITEM_MARGIN, right: Layout.ITEM_MARGIN })
  }
}