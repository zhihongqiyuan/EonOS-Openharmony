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
import SwitchUserManager from '../../../../../../../../../common/src/main/ets/default/SwitchUserManager';
import media from '@ohos.multimedia.media';
import vibrator from '@ohos.vibrator';
import wantAgent from '@ohos.wantAgent';
import Notification from '@ohos.notification';
import NotificationService from '../model/NotificationService';
import NotificationWindowManager from '../model/NotificationWindowManager';
import NotificationConfig from '../model/NotificationConfig';
import CheckEmptyUtils from '../../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';
import createOrGet from '../../../../../../../../../common/src/main/ets/default/SingleInstanceHelper';
import AbilityManager from '../../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import EventManager from '../../../../../../../../../common/src/main/ets/default/event/EventManager';
import {obtainLocalEvent} from '../../../../../../../../../common/src/main/ets/default/event/EventUtil';
import Trace from '../../../../../../../../../common/src/main/ets/default/Trace'
import CommonUtil from '../common/CommonUtil';
import Constants,{NotificationItemData} from '../common/constants';

const TAG = 'NotificationViewModel';

/**
 * Notification ViewModel class.
 */
export class NotificationViewModel {
  audioPlayer: any;
  mNotificationList: NotificationItemData[];
  mCallback: any;
  mNotificationCtrl: any = {};

  constructor() {
    Log.showInfo(TAG, 'constructor');
    this.mNotificationList = [];
    this.audioPlayer = media.createAudioPlayer();
    //    this.audioPlayer.src = 'file://system/etc/capture.ogg';
    SwitchUserManager.getInstance().registerListener(this);
    this.initFlowControlInfos();
    SwitchUserManager.getInstance()
      .getCurrentUserInfo()
      .then((userInfo) => this.registerCallback(userInfo.userId))
      .catch((err) => Log.showError(TAG, `Can't get current user, err: ${JSON.stringify(err)}`));
  }

  registerCallback(userId): void {
    this.mCallback = {
      userId: userId,
      onNotificationConsume: this.onNotificationConsume.bind(this),
      onNotificationCancel: this.onNotificationCancel.bind(this)
    };
    NotificationService.register(this.mCallback);
  }

  unregisterCallback(): void {
    NotificationService.unRegister(this.mCallback);
  }

  userChange(userInfo): void {
    Log.showInfo(TAG, `UserChange, userId: ${userInfo.userId}`);
    this.unregisterCallback();
    this.mNotificationList.length = 0;
    this.initFlowControlInfos();
    this.registerCallback(userInfo.userId);
    this.updateNotification();
    NotificationService.loadAllNotifications();
  }

  /**
   * notification CancelCallback
   *
   * @param {Object} data - return notificationItemData.
   */
  onNotificationConsume(notificationItemData: NotificationItemData): void {
    if (notificationItemData === undefined) {
      Log.showError(TAG, 'onNotificationConsume notificationItemData is undefined');
      return;
    }
    this.onNotificationCancel(notificationItemData.hashcode);
    Log.showDebug(TAG, `onNotificationConsume  ${JSON.stringify(notificationItemData)}`);
    //Verify the notifications can be displayed
    if (!this.isCanShow(notificationItemData.bundleName)) {
      //can not displayed
      return;
    }

    if (notificationItemData.ruleData&&notificationItemData.ruleData.isAllowStatusBarShow) {
      //TODO statusbar show
    }
    if (notificationItemData.ruleData&&notificationItemData.ruleData.isAllowNotificationListShow) {
      this.mNotificationList.unshift(notificationItemData);
      Log.showInfo(TAG, 'reminder start ');
      this.reminderWay(notificationItemData);
      Log.showInfo(TAG, 'reminder end ');
      this.updateFlowControlInfos(notificationItemData.bundleName, true);
    }
    this.updateNotification();
  }

  /**
   * notification CancelCallback
   */
  onNotificationCancel(hashCode: string): void {
    Log.showInfo(TAG, `onNotificationCancel hashCode: ` + hashCode);

    // Common Notification Deletion Logic Processing
    for (let i = 0, len = this.mNotificationList.length; i < len; i++) {
      if (this.mNotificationList[i].hashcode == hashCode) {
        let removeItemArr = this.mNotificationList.splice(i, 1);
        if (!CheckEmptyUtils.isEmpty(removeItemArr)) {
          this.updateFlowControlInfos(removeItemArr[0].bundleName, false);
        }
        this.updateNotification();
        break;
      }
    }
  }

  updateNotification(): void {
    this.sortNotification();
    let notificationList = this.groupByGroupName();
    Log.showInfo(TAG, `updateNotification length: ${notificationList.length}`);
    AppStorage.SetOrCreate('notificationList', notificationList);
  }

  groupByGroupName(): NotificationItemData[][]{
    if (!this.mNotificationList || this.mNotificationList.length < 1) {
      Log.showWarn(TAG, 'groupByGroupName, list is empty.');
      return [];
    }
    let groupArr: NotificationItemData[][] = [];
    let groups = {};
    this.mNotificationList.forEach((item) => {
      const groupName = `${item.bundleName}_${item.groupName}`;
      Log.showDebug(TAG, `groupByGroupName, groupName:${groupName}`);
      if (!groups[groupName] || groups[groupName].length < 1) {
        groups[groupName] = [];
        groupArr.push(groups[groupName]);
      }
      groups[groupName].push(item);
    });
    return groupArr;
  }


  /**
   * Sort the notifications.
   */
  sortNotification(): void {
    Log.showInfo(TAG, 'sortNotification');
    if (this.mNotificationList == undefined || this.mNotificationList == null || this.mNotificationList.length < 1) {
      return;
    }
    this.mNotificationList.sort((itemA, itemB) => {
      //long term notification come first
      if (itemA.source == Notification.SourceType.TYPE_CONTINUOUS && itemB.source != Notification.SourceType.TYPE_CONTINUOUS) {
        return -1;
      }
      //long term notification come first
      if (itemA.source != Notification.SourceType.TYPE_CONTINUOUS && itemB.source == Notification.SourceType.TYPE_CONTINUOUS) {
        return 1;
      }
      if ((itemA.source == Notification.SourceType.TYPE_CONTINUOUS && itemB.source == Notification.SourceType.TYPE_CONTINUOUS) ||
      (itemA.source != Notification.SourceType.TYPE_CONTINUOUS && itemB.source != Notification.SourceType.TYPE_CONTINUOUS)
      ) {
        return -1 * (itemA.timestamp - itemB.timestamp);
      }
    });
  }

  /**
   * Remove all notifications.
   */
  removeAllNotifications(): void {
    Log.showInfo(TAG, 'removeAllNotifications');
    if (this.mNotificationList.length < 1) {
      this.mNotificationList = [];
    } else {
      let index = this.mNotificationList.length;
      while (index--) {
        Log.showInfo(TAG, `removeAllNotifications isRemoveAllowed: ${index}  ${this.mNotificationList[index].isRemoveAllowed}
        isOngoing: ${this.mNotificationList[index].isOngoing} isUnremovable: ${this.mNotificationList[index].isUnremovable}`);

        //Except the Long term notifications
        if (this.mNotificationList[index].isRemoveAllowed &&
        !this.mNotificationList[index].isOngoing && !this.mNotificationList[index].isUnremovable) {
          let hashCode = this.mNotificationList[index].hashcode;
          this.removeSysNotificationItem(hashCode);
          let removeItemArr = this.mNotificationList.splice(index, 1);
          Log.showInfo(TAG, `removeAllNotifications hashCode = ${hashCode}, removeItemArr = ${JSON.stringify(removeItemArr)}`);
          if (!CheckEmptyUtils.isEmpty(removeItemArr)) {
            this.updateFlowControlInfos(removeItemArr[0].bundleName, false);
          }
        }
      }
    }
    this.updateNotification();
  }

  removeNotificationItem(itemData: NotificationItemData, isDelSysConent: boolean, isClickItem?: boolean): void {
    Log.showInfo(TAG, `removeNotificationItem, hashcode: ${itemData.hashcode}`);
    for (let i = 0, len = this.mNotificationList.length; i < len; i++) {
      if (this.mNotificationList[i].hashcode == itemData.hashcode) {
        let removeItemArr = this.mNotificationList.splice(i, 1);
        if (!CheckEmptyUtils.isEmpty(removeItemArr)) {
          this.updateFlowControlInfos(removeItemArr[0].bundleName, false);
        }
        break;
      }
    }
    this.updateNotification();
    if (isDelSysConent) {
      this.removeSysNotificationItem(itemData.hashcode, isClickItem);
    }
    AppStorage.Delete(Constants.KEY_INPUT + itemData.id);
  }

  removeGroupNotification(itemData: NotificationItemData, isDelSysConent: boolean): void {
    Log.showInfo(TAG, `removeGroupNotification, groupName: ${itemData.groupName??''}`);
    let groupName = itemData.groupName;
    for (let i =  this.mNotificationList.length - 1; i >= 0; i--) {
      if (this.mNotificationList[i].groupName == groupName) {
        let id = this.mNotificationList[i].id;
        let hashcode = this.mNotificationList[i].hashcode;
        let removeItemArr = this.mNotificationList.splice(i, 1);
        Log.showDebug(TAG, `removeGroupNotification i = ${i} removeItemArr= ${JSON.stringify(removeItemArr)}`);
        if (!CheckEmptyUtils.isEmpty(removeItemArr)) {
          this.updateFlowControlInfos(removeItemArr[0].bundleName, false);
        }
        if (isDelSysConent) {
          this.removeSysNotificationItem(hashcode);
        }
        AppStorage.Delete(Constants.KEY_INPUT + id);
      }
    }
    this.updateNotification();
  }

  removeSysNotificationItem(hashcode: string, isClickItem?: boolean): void {
    NotificationService.remove(hashcode, isClickItem);
  }

  clickItem(itemData: NotificationItemData, want?: any): void {
    Log.showInfo(TAG, `clickItem itemId: ${itemData.id}, want: ${JSON.stringify(want)}, tapDismissed: ${itemData.tapDismissed}`);
    NotificationWindowManager.hideNotificationWindow();
    Trace.start(Trace.CORE_METHOD_CLICK_NOTIFICATION);
    CommonUtil.startWant((want) ? want : itemData.want);
    if (itemData.tapDismissed) {
      this.removeNotificationItem(itemData, true, true);
    }
  }

  clickReply(inputKey, content, want): void {
    Log.showInfo(TAG, `clickReply inputKey: ${inputKey}, content: ${content}, want: ${JSON.stringify(want)}`);
    let info = {
      code: 0,
      want: { key: inputKey, data: content },
      permission: '',
      extraInfo: {}
    };
    CommonUtil.startWant(want, info);
  }

  initFlowControlInfos(): void {
    Log.showInfo(TAG, 'initFlowControlInfos enter');
    let notificationConfig = NotificationConfig.readNotificationConfig('statusbar');
    Log.showInfo(TAG, 'NotificationConfig: ' + JSON.stringify(notificationConfig));
    if (CheckEmptyUtils.isEmpty(notificationConfig)) {
      Log.showInfo(TAG, 'NotificationConfig is no definition');
      return;
    }
    this.mNotificationCtrl = {
      currentTotal: 0,
      limitTotal: notificationConfig.limitTotal,
      app: new Map()
    };
    for (let item of notificationConfig.app) {
      let tmp = {
        'canShow': item.canShow,
        'currentNum': 0,
        'limit': item.limit
      };
      this.mNotificationCtrl['app'].set(item.bundleName, tmp);
    }
    Log.showDebug(TAG, 'initFlowControlInfos end, mNotificationCtrl: ' + JSON.stringify(this.mNotificationCtrl));
  }

  isCanShow(bundleName: string): boolean {
    Log.showInfo(TAG, 'isCanShow');
    let result = true;
    if (!CheckEmptyUtils.isEmpty(this.mNotificationCtrl)) {
      let currentTotal: number = this.mNotificationCtrl['currentTotal'];
      let limitTotal = this.mNotificationCtrl['limitTotal'];
      Log.showInfo(TAG, `isCanShow Total: currentTotal=${currentTotal},limitTotal=${limitTotal}`);
      if (currentTotal + 1 > limitTotal) {
        result = false;
      } else if (this.mNotificationCtrl['app'].has(bundleName)) {
        let tmp = this.mNotificationCtrl['app'].get(bundleName);
        Log.showInfo(TAG, `isCanShow appTotal: canShow=${tmp['canShow']},tmp['currentNum']=${tmp['currentNum']}`);
        if (tmp['canShow'] === false || (tmp['currentNum'] + 1 > tmp['limit'])) {
          result = false;
        }
      }
    }
    Log.showInfo(TAG, `isCanShow :${result?'true':'false'}`);
    return result;
  }

  updateFlowControlInfos(bundleName: string, plusOrMinus: boolean): void {
    if (!CheckEmptyUtils.isEmpty(this.mNotificationCtrl)) {
      if (this.mNotificationCtrl['app'].has(bundleName)) {
        let tmp = this.mNotificationCtrl['app'].get(bundleName);
        if (plusOrMinus) {
          tmp['currentNum'] += 1;
        } else if (tmp['currentNum'] > 0) {
          tmp['currentNum'] -= 1;
        }
        this.mNotificationCtrl['app'].set(bundleName, tmp);
      }

      if (plusOrMinus) {
        this.mNotificationCtrl['currentTotal'] += 1;
      } else if (this.mNotificationCtrl['currentTotal'] > 0) {
        this.mNotificationCtrl['currentTotal'] -= 1;
      }
    }
    Log.showInfo(TAG, `updateFlowControlInfos:${JSON.stringify(this.mNotificationCtrl)}`);
  }

  reminderWay(itemData: NotificationItemData): void {
    if (itemData.ruleData&&itemData.ruleData.isAllowBanner) {
      Log.showInfo(TAG, 'banner start ');
      AbilityManager.setAbilityData(AbilityManager.ABILITY_NAME_BANNER_NOTICE, 'itemData', itemData);
      EventManager.publish(obtainLocalEvent('onBannerNoticeShow', { 'itemData': itemData }));
      Log.showInfo(TAG, 'banner end ');
    }
    if (itemData.notificationFlags?.soundEnabled != Constants.NOTIFICATION_TYPE_CLOSE) {
      if (itemData.ruleData&&itemData.ruleData.isAllowSound) {
        try {
          this.audioPlayer.src = itemData.sound;
          Log.showInfo(TAG, 'sound start ');
          this.audioPlayer.play();
          Log.showInfo(TAG, 'sound end ');
        } catch (e) {
          Log.showError(TAG, `sound error: ${JSON.stringify(e)}`);
        }
      }
    }
    if (itemData.notificationFlags?.vibrationEnabled != Constants.NOTIFICATION_TYPE_CLOSE) {
      if (itemData.ruleData&&itemData.ruleData.isAllowVibrationValues) {
        //      Log.showInfo(TAG, `vibrate start`);
        //      for (let i = 0, len = itemData.vibrationValues.length; i < len; i++) {
        //        vibrator.vibrate(itemData.vibrationValues[i], function(error){
        //          Log.showInfo(TAG, `vibrate id：${i}`);
        //          if (error) {
        //            Log.showInfo(TAG, "error.code" + error.code + "error.message" + error.message);
        //          } else {
        //            Log.showInfo(TAG, "Callback returned to indicate a successful vibration.");
        //          }
        //        })
        //      }
      }
    }
  }

  getPluginTempLate(templateName: string): any {
    Log.showInfo(TAG, `getPluginTempLate: ${templateName}`);
    return NotificationService.getPluginTempLate(templateName);
  }

  enableNotification(itemData: NotificationItemData, enable: boolean): void {
    Log.showInfo(TAG, `enableNotification, bundleName: ${itemData.bundleName}  uid: ${itemData.uid}`);
    return NotificationService.enableNotification({ bundle: itemData.bundleName, uid: itemData.uid }, enable);
  }

  clickDistributionItem(itemData: NotificationItemData, triggerInfo): void {
    Log.showInfo(TAG, `clickDistributionItem wantAgen: ${JSON.stringify(itemData.want)}, triggerInfo: ${JSON.stringify(triggerInfo)}`);
    NotificationWindowManager.hideNotificationWindow();
    CommonUtil.startWant(itemData.want, triggerInfo);
    this.removeNotificationItem(itemData, true);
  }

  //get distributed device name
  async getDistributedDeviceName(itemData: NotificationItemData): Promise<string>{
    Log.showInfo(TAG, `getDistributedDeviceName itemData want:${JSON.stringify(itemData.want)} deviceId:${itemData.deviceId}`);

    return new Promise((resolve) => {
      let deviceName = '';
      if (itemData.distributedOption?.isDistributed && !!itemData.deviceId) {
        deviceName = NotificationService.getTrustedDeviceDeviceName(itemData.deviceId);
        resolve(deviceName);
      } else {
        resolve(deviceName);
      }
    });
  }
}

let sNotificationViewModel = createOrGet(NotificationViewModel, TAG);

export default sNotificationViewModel ;