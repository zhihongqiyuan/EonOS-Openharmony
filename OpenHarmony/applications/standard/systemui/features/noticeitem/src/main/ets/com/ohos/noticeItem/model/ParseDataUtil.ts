//@ts-nocheck
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

import Notification from '@ohos.notification';
import BundleMgr from "@ohos.bundle.bundleManager";
import AccountManager from '@ohos.account.osAccount';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import BundleManager from '../../../../../../../../../common/src/main/ets/default/abilitymanager/bundleManager';
import AbilityManager from '../../../../../../../../../common/src/main/ets/default/abilitymanager/abilityManager';
import DistributionManager from './NotificationDistributionManager';
import NotificationManager from './NotificationManager';
import Constants, {NotificationItemData} from '../common/constants';
import {NotificationConfig} from './NotificationConfig';
import TimeManager from '../../../../../../../../../common/src/main/ets/default/TimeManager'

const TAG = 'Notification_ParseDataUtil';
let mDate = new Date();
let appDataMap = new Map();

type NotificationContent = {
  title: string,
  text: string,
  additionalText: string,
  briefText?: string,
  expandedTitle?: string,
  longText?: string,
  lines?: [],
  picture?: any
}

async function getUserId(uid): Promise<any> {
  let userId = await AccountManager.getAccountManager().getOsAccountLocalIdFromUid(uid);
  Log.showInfo(TAG, `getOsAccountLocalIdFromUid uid = ${uid}, userId = ${userId}`);
  return userId;
}

/**
 * parse data util class.
 */
export default class ParseDataUtil {
  static async parseData(request, sortingMap?): Promise<NotificationItemData> {
    if (!request) {
      return Promise.reject('consumeCallback request is empty');
    }
    Log.showInfo(TAG, `parse data start, want = ${JSON.stringify(request.wantAgent)} actionButtons = ${JSON.stringify(request.actionButtons)}`);
    let userId = await getUserId(request.creatorUid);
    let appMessage = await ParseDataUtil.getAppData(request.creatorBundleName, userId);
    let notificationItem: NotificationItemData = {
      id: request.id,
      userId: userId,
      uid: request.creatorUid,
      hashcode: request.hashCode,
      contentType: `${request?.content?.contentType}`,
      timestamp: request.deliveryTime,
      time: TimeManager.formatTime(new Date(request.deliveryTime)),
      appName: appMessage.appName,
      want: request.wantAgent,
      bundleName: request.creatorBundleName,
      actionButtons: request.actionButtons,
      largeIcon: request.largeIcon,
      slotLevel: sortingMap?.sortings[request?.hashCode]?.slot?.level,
      source: request.source,
      sound: sortingMap?.sortings[request?.hashCode]?.slot?.sound ?? '',
      vibrationValues: sortingMap?.sortings[request?.hashCode]?.slot?.vibrationValues ?? [],
      notificationFlags: request.notificationFlags,
      template: request?.template,
      isOngoing: request.isOngoing,
      isUnremovable: request.isUnremovable,
      isRemoveAllowed: request.isRemoveAllowed,
      distributedOption: request.distributedOption,
      deviceId: request.deviceId,
      groupName: request.groupName??request.hashcode,
      smallIcon: NotificationConfig.USE_NOTIFICATION_ICON ? (request?.smallIcon ?? appMessage.icon) : appMessage.icon,
      tapDismissed: request.tapDismissed
    };
    notificationItem = {
      ...notificationItem, ...ParseDataUtil.getContentByType(request?.content?.contentType, request)
    };
    return notificationItem;
  }

  /**
   * Get app data by bundleName.
   *
   * @param {string} bundleName - BundleName of the target app.
   * @return {object} appData
   */
  static async getAppData(bundleName, userId): Promise<any> {
    Log.showInfo(TAG, `getAppName start by ${bundleName}`);
    if (appDataMap.has(bundleName)) {
      Log.showInfo(TAG, 'getAppData success.');
      return appDataMap.get(bundleName);
    }
    let data = await BundleManager.getBundleInfo(TAG, bundleName, BundleMgr.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION, userId);
    let labelResource = data?.appInfo?.labelResource;
    let iconResource = data?.appInfo?.iconResource;
    let appMessage = {
      appName: await BundleManager.getString(labelResource),
      icon: await BundleManager.getMediaBase64(iconResource)
    };
    if (appMessage.icon != null && appMessage.appName) {
      appDataMap.set(bundleName, appMessage);
      Log.showInfo(TAG, `getAppData success ${appMessage.appName} by ${bundleName}`);
    }
    return appMessage;
  }

  /**
   * Reformat the timestamp to hh:mm.
   *
   * @param {number} timestamp - Target timestamp.
   */
  static getStandardTime(timestamp): string {
    if (timestamp == 0 || timestamp == undefined || timestamp == null) {
      return '';
    }
    mDate.setTime(timestamp);
    let hoursNumber = mDate.getHours();
    let minutesNumber = mDate.getMinutes();
    let hours = hoursNumber < 10 ? `0${hoursNumber}` : hoursNumber;
    let minutes = minutesNumber < 10 ? `0${minutesNumber}` : minutesNumber;
    return `${hours}:${minutes}`;
  }

  static getContentByType(notificationType, request): NotificationContent {
    let content = {
      title: '', text: '', additionalText: ''
    };

    function getContentByKey(keysAndDefault, source): void {
      if (source) {
        keysAndDefault.forEach(([key, defualt]) => {
          content[key] = source[key] ?? defualt;
        });
      }
    }

    switch (notificationType) {
    case NotificationManager.TYPE_BASIC:
      getContentByKey([['title', ''], ['text', ''], ['additionalText', '']], request.content.normal);
      break;
    case NotificationManager.TYPE_LONG:
      getContentByKey([['title', ''], ['text', ''], ['additionalText', []], ['briefText', ''],
        ['expandedTitle', ''], ['longText', '']], request.content.longText);
      break;
    case NotificationManager.TYPE_MULTI:
      getContentByKey([['title', ''], ['text', ''], ['additionalText', []], ['briefText', ''],
        ['longTitle', ''], ['lines', []]], request.content.multiLine);
      break;
    case NotificationManager.TYPE_PICTURE:
      getContentByKey([['title', ''], ['text', ''], ['additionalText', []], ['briefText', ''],
        ['expandedTitle', ''], ['picture', '']], request.content.picture);
      break;
    default:
      Log.showInfo(TAG, 'no match content type');
      break;
    }
    Log.showInfo(TAG, `notificationType = ${notificationType}`);
    return content;
  }
}
