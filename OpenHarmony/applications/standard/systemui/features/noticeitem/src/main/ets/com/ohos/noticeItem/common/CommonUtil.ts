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
import Trace from '../../../../../../../../../common/src/main/ets/default/Trace'
import WantAgent from '@ohos.wantAgent';

const TAG = 'CommonUtil';
const DEFAULT_INFO = {
  code: 0,
  want: {},
  permission: '',
  extraInfo: {}
};

export default class CommonUtil {
  static startWant(want, triggerInfo?: any): void {
    let info = (triggerInfo) ? triggerInfo : DEFAULT_INFO;
    Log.showDebug(TAG, `startWant ${JSON.stringify(want)}, info ${JSON.stringify(info)}`);
    if (!want) {
      return;
    }
    WantAgent.trigger(want, info, ((data) => {
      Trace.end(Trace.CORE_METHOD_CLICK_NOTIFICATION);
      Log.showInfo(TAG, `wantAgent trigger data ${JSON.stringify(data)}`);
    }));
  }

  static checkVisibilityByUser(notificationUser: number, currentUser: number): boolean{
    return (0 <= notificationUser && notificationUser < 100) || notificationUser == currentUser;
  }
}
