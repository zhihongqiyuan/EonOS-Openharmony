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
import EventManager from '../../../../../../../../../common/src/main/ets/default/event/EventManager';
import {obtainLocalEvent} from '../../../../../../../../../common/src/main/ets/default/event/EventUtil';

const TAG = 'NotificationWindowManager';

class NotificationWindowManager {

  showNotificationWindow(): void {
    // todo show Notification window
  }

  hideNotificationWindow(): void {
    Log.showInfo(TAG, 'hide Notification window');
    EventManager.publish(obtainLocalEvent('hideNotificationWindowEvent', ''));
  }
}

let sNotificationWindowManager = new NotificationWindowManager();

export default sNotificationWindowManager ;