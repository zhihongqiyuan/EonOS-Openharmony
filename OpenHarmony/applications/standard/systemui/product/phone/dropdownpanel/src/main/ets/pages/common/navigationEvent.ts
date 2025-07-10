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
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import { inputConsumer } from '@kit.InputKit';

const TAG = 'navigationEvent';

let commonEventSubscribeInfo = {
  events: ['SET_STATUSBAR_MIN_EVENT']
};

let commonEventSubscriber = null;

export interface NavigationCallBack {
  "onStateChange": (data: string) => void;
  "onNotificationShowOrHide": (data: inputConsumer.KeyOptions) => void;
  "onControlShowOrHide": (data: inputConsumer.KeyOptions) => void;
}

export class navigationEvent {
  mCallback: NavigationCallBack;

  registerCallback(callback): void {
    Log.showDebug(TAG, 'registerCallback');
    this.mCallback = callback;
    if (commonEventSubscriber == null) {
      commonEvent.createSubscriber(
        commonEventSubscribeInfo,
      this.createCapsuleSubscriberCallBack.bind(this)
      );
    }
  }

  createCapsuleSubscriberCallBack(err, data): void {
    Log.showDebug(TAG, `createCapsuleSubscriberCallBack err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
    commonEventSubscriber = data;
    commonEvent.subscribe(data, (err, data) => {
      Log.showInfo(TAG, `subscribe err: ${JSON.stringify(err)} data: ${JSON.stringify(data)}`);
      if (err.code == 0) {
        let processingData;;
        try{
          processingData = JSON.parse(data.data)
        }catch(e){
          Log.showError(TAG, 'date.data is err');
        }
        if (processingData) {
          Log.showInfo(TAG, `subscribe processingData: ${JSON.stringify(processingData)} `);
          this.mCallback.onStateChange(processingData);
        }
      } else {
        Log.showError(TAG, 'date is err');
      }
    });
  }

  unregisterCallback(): void {
    if (commonEventSubscriber != null) {
      commonEvent.unsubscribe(commonEventSubscriber, () => {
        Log.showInfo(TAG, 'Subscriberregister unregister Capsule Status Listener ===============');
      });
    }
  }
}

let mNavigationEvent = new navigationEvent();

export default mNavigationEvent as navigationEvent;