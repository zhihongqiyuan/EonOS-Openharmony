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

//import { NotificationSubscriber } from './notification/notificationSubscriber';
import Notification from '@ohos.notification';
import PluginComponentManager from '@ohos.pluginComponent';
import Systemparameter from '@ohos.systemparameter';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';
import {AsyncCallback} from '@ohos.base';

const TAG = 'NotificationManager';
// Temporary path
const EXTERNAL_JSON_PATH = '/system/etc/notification_template/external.json';
const DEBUG_SETTING_KEY = 'debug.systemui.notificationtemplate';
const DEBUG_BUNDLE_NAME = 'com.ohos.example.notificationtemplate';
const DEBUG_ABILITY_NAME = 'com.ohos.example.notificationtemplate.MainAbility';
const DEBUG_TEMPLATE_NAME = 'NotificationTemplate';

export default class NotificationManager {
  static readonly TYPE_BASIC: number = Notification.ContentType.NOTIFICATION_CONTENT_BASIC_TEXT;
  static readonly TYPE_LONG: number = Notification.ContentType.NOTIFICATION_CONTENT_LONG_TEXT;
  static readonly TYPE_MULTI: number = Notification.ContentType.NOTIFICATION_CONTENT_MULTILINE;
  static readonly TYPE_PICTURE: number = Notification.ContentType.NOTIFICATION_CONTENT_PICTURE;
  static notificationTemplateMap = new Map();

  static subscribeNotification(tag: string, subscriber, asyncCallback): void {
    Log.showInfo(TAG, `subscribeNotification from: ${tag}`);
    Notification.subscribe(subscriber, asyncCallback);
  }

  static unsubscribeNotification(tag: string, subscriber): void {
    Log.showInfo(TAG, `subscribeNotification from: ${tag}`);
    Notification.unsubscribe(subscriber);
  }

  static removeAll(tag: string, callback): void {
    Log.showInfo(TAG, `removeAll from: ${tag}`);
    Notification.removeAll(callback);
  }

  static remove(tag: string, hashCode,callback, isClickItem?: boolean): void {
    Log.showInfo(TAG, `remove from: ${tag}`);
    var clickValue = isClickItem == true ? Notification.RemoveReason.CLICK_REASON_REMOVE : Notification.RemoveReason.CANCEL_REASON_REMOVE;
    Notification.remove(hashCode, clickValue, callback);
  }

  static getAllActiveNotifications(tag: string, callback): void {
    Log.showInfo(TAG, `getAllActiveNotifications from: ${tag}`);
    Notification.getAllActiveNotifications(callback);
  }

  static enableNotification(tag: string, bundleOption, data, callback): void {
    Log.showInfo(TAG, `enableNotification from: ${tag}`);
    Notification.enableNotification(bundleOption, data, callback);
  }

  static initNotificationTemplateMap(tag: string, ownerWant): void {
    Log.showInfo(TAG, `initNotificationTemplateMap from: ${tag}`);
    if (NotificationManager.isDebugMode(tag)) {
      NotificationManager.requestDebugTemplate(tag, ownerWant);
    } else {
      NotificationManager.requestTemplate(tag, ownerWant, '', EXTERNAL_JSON_PATH);
    }
  }

  static request(tag: string, param, asyncCallback): void {
    Log.showInfo(TAG, `request from: ${tag}`);
    PluginComponentManager.request(param, (err, data) => {
      asyncCallback(err, data);
    });
  }

  static push(tag: string, param, asyncCallback): void {
    Log.showInfo(TAG, `push from: ${tag}`);
    PluginComponentManager.push(param, () => {
      asyncCallback();
    });
  }

  static requestListener(tag: string, ownerWant, asyncCallback): void {
    Log.showInfo(TAG, `requestListener from: ${tag}`);
    PluginComponentManager.on('request', (source, name, data) => {
      asyncCallback(source, name, data);
    });
  }

  static pushListener(tag: string, ownerWant, asyncCallback): void {
    Log.showInfo(TAG, `pushListener from: ${tag}`);
    PluginComponentManager.on('push', (source, template, data, extraData) => {
      asyncCallback(source, template, data, extraData);
    });
  }

  static requestTemplate(tag: string, ownerWant, templateName, templatePath): void {
    Log.showInfo(TAG, `requestTemplate from: ${tag} ownerWant:${JSON.stringify(ownerWant)}`);

    let reqWant = {
      bundleName: '',
      abilityName: ''
    };
    let reqData = {};

    let requestParam = {
      owner: ownerWant,
      want: reqWant,
      name: templateName,
      data: reqData,
      jsonPath: templatePath
    };

    Log.showInfo(TAG, `requestTemplate requestParam: ${JSON.stringify(requestParam)}`);
    NotificationManager.request(tag, requestParam, (err, data) => {
      Log.showInfo(TAG, `request finished err: ${JSON.stringify(err)} data: ${JSON.stringify(data)} templateData: ${templateName}`);
      if (data?.componentTemplate?.source) {
        Log.showInfo(TAG, `request finished data.componentTemplate.source:${JSON.stringify(data.componentTemplate.source)}`);
        let templates = JSON.parse(data.componentTemplate.source);
        Log.showInfo(TAG, `request templates: ${JSON.stringify(templates)}`);
        for (let key in templates) {
          NotificationManager.notificationTemplateMap.set(key, {
            'source': templates[key], 'ability': ''
          });
        }
      }
    });
  }

  static requestDebugTemplate(tag: string, ownerWant): void {
    Log.showInfo(TAG, `requestDebugTemplate from: ${tag} ownerWant:${JSON.stringify(ownerWant)}`);
    let reqWant = {
      bundleName: DEBUG_BUNDLE_NAME,
      abilityName: DEBUG_ABILITY_NAME
    };
    let reqData = {};
    let requestParam = {
      owner: ownerWant,
      want: reqWant,
      name: DEBUG_TEMPLATE_NAME,
      data: reqData
    };
    NotificationManager.request(tag, requestParam, (err, data) => {
      Log.showInfo(TAG, `requestDebugTemplate finished err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
      if (!!data) {
        Log.showInfo(TAG, `requestDebugTemplate finished data.componentTemplate.source:${JSON.stringify(data.componentTemplate.source)}`);
        NotificationManager.notificationTemplateMap.set(DEBUG_TEMPLATE_NAME, data.componentTemplate);
      }
    });
  }

  static isDebugMode(tag: string): boolean{
    let debug = Systemparameter.getSync(DEBUG_SETTING_KEY, '');
    Log.showInfo(TAG, `Systemparameter DEBUG_SETTING: ${debug}`);
    return!!debug;
  }

}