/**
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import userAuth from '@ohos.userIAM.userAuth';
import UIExtensionContentSession from '@ohos.app.ability.UIExtensionContentSession';
import Constants from '../vm/Constants';
import LogUtils from './LogUtils';

const TAG = 'AuthUtils';

export default class AuthUtils {
  private static authUtilsInstance: AuthUtils;

  public static getInstance(): AuthUtils {
    if (!AuthUtils.authUtilsInstance) {
      AuthUtils.authUtilsInstance = new AuthUtils();
    }
    return AuthUtils.authUtilsInstance;
  }

  sendNotice(cmd: string, type: Array<string>): void {
    try {
      const eventData = {
        widgetContextId: AppStorage.get('widgetContextId'),
        event: cmd,
        version: Constants.noticeVersion,
        payload: {
          type: type
        }
      };
      const jsonEventData = JSON.stringify(eventData);
      LogUtils.info(TAG, 'sendNotice start eventData: ' + jsonEventData);
      userAuth.sendNotice(userAuth.NoticeType.WIDGET_NOTICE, jsonEventData);
      LogUtils.info(TAG, 'sendNotice success');
    } catch (error) {
      LogUtils.error(TAG, 'sendNotice catch error: ' + error?.code);
      (AppStorage.get('session') as UIExtensionContentSession)?.terminateSelf();
    }
  }
}