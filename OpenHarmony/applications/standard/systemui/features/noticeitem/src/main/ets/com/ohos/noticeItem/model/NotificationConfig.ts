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

import ReadConfigUtil from '../../../../../../../../../common/src/main/ets/default/ReadConfigUtil';
import Log from '../../../../../../../../../common/src/main/ets/default/Log';

const TAG = 'NotificationConfig';
/**
 * Notification Flow Config：
 * limitTotal:Total number of notifications that can be displayed.Beyond the range will not be displayed.
 * app:The application configuration that app needs to limit is in this array.
 *     See the following for specific configuration attributes:
 *   --bundleName:Application bundleName.
 *   --canShow:Specifies whether application notifications can be displayed.
 *   --limit:Specifies the total number of notifications that the application can display.
 *     Beyond the range will not be displayed.
 */
const conFig = {
  'limitTotal': 10000,
  'app':
    [
      { 'bundleName': 'com.ohos.publishnotifications', 'canShow': true, 'limit': 100 },
      { 'bundleName': 'com.ohos.app2', 'canShow': true, 'limit': 100 }
    ]
};

export class NotificationConfig {
  static readonly USE_NOTIFICATION_ICON = true;

  readNotificationConfig(deviceType?: string): { limitTotal: number; app: { bundleName: string; canShow: boolean; limit: number; }[]; } {
    return conFig;
  }
}

let notificationConfig = new NotificationConfig();

export default notificationConfig ;