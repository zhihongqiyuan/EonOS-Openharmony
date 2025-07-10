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

import commonEvent from '@ohos.commonEvent';
import Log from '../../../../../../../common/src/main/ets/default/Log';

const TAG = 'CapsuleModel';

let commonEventSubscribeInfo = {
  events: ['CAPSULE_EVENT_CALL_UI'],
  publisherPermission: "ohos.permission.GET_TELEPHONY_STATE"
};
let commonEventSubscriber = null;

export class CapsuleModel {
  mCallback: any;

  registerCallback(callback) {
    Log.showInfo(TAG, 'registerCallback');
    this.mCallback = callback;
    if (commonEventSubscriber == null) {
      commonEvent.createSubscriber(
        commonEventSubscribeInfo,
      this.createCapsuleSubscriberCallBack.bind(this)
      );
    }
  }

  createCapsuleSubscriberCallBack(err, data) {
    commonEventSubscriber = data;
    commonEvent.subscribe(commonEventSubscriber, (err, data) => {
      Log.showInfo(TAG, `createCapsuleSubscriberCallBack err: ${err.code}`);
      if (err.code == 0) {
        const processingData = JSON.parse(data.data);
        if (processingData) {
          Log.showInfo(TAG, `createCapsuleSubscriberCallBack processingData: ${JSON.stringify(processingData)} `);
          this.mCallback.onStateChange(processingData);
        }
      } else {
        Log.showError(TAG, 'date is err');
      }
    });
  }

  unregisterCallback() {
    if (commonEventSubscriber != null) {
      commonEvent.unsubscribe(commonEventSubscriber, () => {
        Log.showInfo(TAG, 'Subscriberregister unregister Capsule Status Listener ===============');
      });
    }
  }
}

let mCapsuleModel = new CapsuleModel();

export default mCapsuleModel as CapsuleModel;