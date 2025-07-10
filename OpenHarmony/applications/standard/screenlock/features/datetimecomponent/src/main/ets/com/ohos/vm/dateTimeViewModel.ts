/**
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
import {Log, dateTimeCommon, sTimeManager, TimeEventArgs, TIME_CHANGE_EVENT, sEventManager, unsubscribe} from '@ohos/common'

const TAG = 'ScreenLock-DateTimeViewModel'

let mCommonEventSubscribeInfo = {
    events: [
        commonEvent.Support.COMMON_EVENT_TIME_CHANGED,
        commonEvent.Support.COMMON_EVENT_TIMEZONE_CHANGED,
        commonEvent.Support.COMMON_EVENT_TIME_TICK
    ]
};

let mEventSubscriber

/**
 * DateTimeViewModel class
 */
export default class DateTimeViewModel {
    timeVal: string = ''
    dateVal: any = {}
    weekVal: any = {}
    calendarVal: any = {}
    unSubscriber?: unsubscribe;

    ViewModelInit(): void{
        Log.showDebug(TAG, 'ViewModelInit');

        this.getAndSetDateTime.bind(this)()
        commonEvent.createSubscriber(mCommonEventSubscribeInfo, this.createSubscriberCallBack.bind(this));
        this.unSubscriber = sEventManager.subscribe(TIME_CHANGE_EVENT, (args: TimeEventArgs) => {
            this.setDateTime(args.date)
        });
        Log.showDebug(TAG, 'ViewModelInit end');
    }

    private getAndSetDateTime() {
        Log.showDebug(TAG, `getAndSetDateTime`)
        this.setDateTime(new Date())
    }

    private setDateTime(date: Date) {
        Log.showDebug(TAG, `setDateTime`)
        this.timeVal = sTimeManager.formatTime(date)
        this.dateVal = dateTimeCommon.getSystemDate()
        this.weekVal = dateTimeCommon.getSystemWeek()
        this.calendarVal = dateTimeCommon.getCalendarDate()
    }

    private createSubscriberCallBack(err, data) {
        Log.showDebug(TAG, "start createSubscriberCallBack " + JSON.stringify(data))
        mEventSubscriber = data
        commonEvent.subscribe(data, this.getAndSetDateTime.bind(this));
        Log.showDebug(TAG, "start createSubscriberCallBack finish")
    }

    stopPolling() {
        Log.showDebug(TAG, `stopPolling start`)
        commonEvent.unsubscribe(mEventSubscriber);
        this.unSubscriber && this.unSubscriber();
        this.unSubscriber = undefined;
        Log.showDebug(TAG, `stopPolling end`)
    }
}
