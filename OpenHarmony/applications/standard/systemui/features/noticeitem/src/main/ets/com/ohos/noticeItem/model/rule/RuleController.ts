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

import {NotificationItemData} from '../../common/constants';
import Log from '../../../../../../../../../../common/src/main/ets/default/Log';
import Notification from '@ohos.notification';
import CheckEmptyUtils from '../../../../../../../../../../common/src/main/ets/default/CheckEmptyUtils';

const TAG = 'NotificationRuleController';

export class RuleController {
  /**
     * Check if this notification is allow show or not
     *
     * @param {notificationItemData} Data of the notification
     * @param {callback} Data of the type to show the notification
     */
  getNotificationData(notificationItemData: NotificationItemData, callback): void {
    this.isAllowSendNotification(notificationItemData, (isSuccess) => {
      if (!isSuccess) {
        Log.showWarn(TAG, 'user is not allow this to send notification');
        callback(undefined);
        return;
      }
      this.getNotificationDataByApp(notificationItemData, (originalData) => {
        this.updateNotificationDataBySense(originalData, (finalData) => {
          callback(finalData);
        });
      });
    });
  }

  /**
     * Check if user allow the app send notification or not
     *
     * @param {notificationItemData} The origin notification data
     * @param {callback} The user allow the app send notification or not
     */
  isAllowSendNotification(notificationItemData, callback): void {
    Notification.isNotificationEnabled({ bundle: notificationItemData.bundleName, uid: notificationItemData.uid })
      .then((flag) => {
        Log.showInfo(TAG, `Notification.isNotificationEnabled: ${flag}`);
        callback(flag);
      });
  }

  /**
   * Check whether sound or vibration is allowed
   *
   * @param {notificationItemData} The origin notification data
   * @param {callback} whether sound or vibration is allowed
   */

  SoundOrVibrate(notificationItemData, callback): void {
    Log.showInfo(TAG, 'SoundOrVibrate start');
    let sound = false;
    let vibrationValues = false;
    if (!CheckEmptyUtils.checkStrIsEmpty(notificationItemData.sound)) {
      sound = true;
      Log.showInfo(TAG, `notificationItemData.sound is allowed = ${sound}`);
    }
    if (!CheckEmptyUtils.isEmptyArr(notificationItemData.vibrationValues)) {
      vibrationValues = true;
      Log.showInfo(TAG, `notificationItemData.vibrationValues is allowed = ${vibrationValues}`);
    }
    callback(sound, vibrationValues);
  }


  /**
     * Get notification data of the notification
     *
     * @param {notificationItemData} The origin notification data
     * @param {callback} The type to show notification
     */
  getNotificationDataByApp(notificationItemData, callback): void {
    let mNotificationItemData : NotificationItemData = notificationItemData;
    mNotificationItemData.ruleData = {
      isAllowBanner: false,
      isAllowSound: false,
      isAllowVibrationValues: false,
      isAllowStatusBarShow : false,
      isAllowNotificationListShow : false
    };
    
    Log.showInfo(TAG, `notificationItemData.slotLevel = ${notificationItemData.slotLevel}`);
    if (notificationItemData.slotLevel === Notification.SlotLevel.LEVEL_HIGH) {
      mNotificationItemData.ruleData.isAllowBanner = true;
      this.SoundOrVibrate(notificationItemData, (sound, vibrationValues) => {
        if(mNotificationItemData.ruleData){
          mNotificationItemData.ruleData.isAllowSound = sound;
          mNotificationItemData.ruleData.isAllowVibrationValues = vibrationValues;
        }
      });
      mNotificationItemData.ruleData.isAllowStatusBarShow = true;
      mNotificationItemData.ruleData.isAllowNotificationListShow = true;

    } else if (notificationItemData.slotLevel === Notification.SlotLevel.LEVEL_DEFAULT) {
      this.SoundOrVibrate(notificationItemData, (sound, vibrationValues) => {
        if(mNotificationItemData.ruleData){
          mNotificationItemData.ruleData.isAllowSound = sound;
          mNotificationItemData.ruleData.isAllowVibrationValues = vibrationValues;
        }
      });
      mNotificationItemData.ruleData.isAllowStatusBarShow = true;
      mNotificationItemData.ruleData.isAllowNotificationListShow = true;
    } else if (notificationItemData.slotLevel === Notification.SlotLevel.LEVEL_LOW ||
      notificationItemData.slotLevel === Notification.SlotLevel.LEVEL_MIN) {
      mNotificationItemData.ruleData.isAllowNotificationListShow = true;
    } else {
      mNotificationItemData.ruleData.isAllowNotificationListShow = false;
    }
    Log.showInfo(TAG, `mNotificationItemData.ruleData = ${JSON.stringify(mNotificationItemData.ruleData)}`);
    callback(mNotificationItemData);
  }



  /**
   * Check the sense of the phone to update the show type
   *
   * @param {notificationItemData} The origin notification data
   * @param {callback} The final notification data
   */
  updateNotificationDataBySense(notificationItemData, callback): void {
    let mNotificationItemData = notificationItemData;
    // TODO Scenario Management
    callback(mNotificationItemData);
  }

}

let ruleController = new RuleController();

export default ruleController ;