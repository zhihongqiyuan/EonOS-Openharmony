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

import {Log} from '@ohos/common'
import Constants from '../common/constants'
import service, {UnlockResult, AuthType, AuthSubType} from '../model/screenLockService'
import {Callback} from '@ohos.base';

const TAG = 'ScreenLock-BaseViewModel'
const MINUTE_NM = '分钟'
const SECOND_NM = "秒"
const TRY_AGAIN = "后重试"

export {service, UnlockResult, AuthType, AuthSubType}

export default class BaseViewModel {
    prompt: Resource | string;
    inhibitInput: boolean = false;

    constructor() {
        this.ViewModelInit()
    }

    ViewModelInit(): void{
        Log.showDebug(TAG, 'ViewModelInit');
        this.prompt = $r('app.string.input');
    }

    countdown() {
        Log.showInfo(TAG, `countdown`)
        service.getAuthProperty(AuthType.PIN, (properties) => {
            let promptText: Resource | string = '';
            let freezingMillisecond = properties.freezingTime;
            Log.showInfo(TAG, `countdown freezingMillisecond:${freezingMillisecond}`)
            if (freezingMillisecond > 0) {
                promptText = this.getFreezingTimeNm(freezingMillisecond);
                promptText += TRY_AGAIN;
                setTimeout(this.countdown.bind(this), Constants.INTERVAL)
            } else {
                Log.showDebug(TAG, `countdown clearInterval`)
                this.inhibitInput = false
                promptText = $r('app.string.input');
            }
            Log.showDebug(TAG, `countdown promptText:${promptText}`)
            this.prompt = promptText;
        })
    }

    changePrompt(remainTimes, freezingTime, callback: Callback<void>) {
        Log.showInfo(TAG, `changePrompt remainTimes:${remainTimes} freezingTime:${freezingTime}`)
        let promptText: Resource | string = $r('app.string.incorrect');

        if (0 < remainTimes && remainTimes <= 2) {
            if (freezingTime > 0) {
                let freezingTimeNm = this.getFreezingTimeNm(freezingTime)
                promptText = $r('app.string.incorrect_promp_freezing', remainTimes, freezingTimeNm);
            } else {
                promptText = $r('app.string.incorrect_promp_times', remainTimes);
            }
        } else if (0 == remainTimes) {
            if (freezingTime > 0) {
                this.inhibitInput = true
                promptText = $r('app.string.input_promp', this.getFreezingTimeNm(freezingTime));
                setTimeout(this.countdown.bind(this), Constants.INTERVAL)
            }
        }
        this.prompt = promptText;
        this.updateStorage(callback)
        //notify the base service that the unlock is fail
        service.notifyUnlockScreenResult(UnlockResult.Fail);
    }

    getFreezingTimeNm(freezingMillisecond: number): string {
        let minute = Math.floor(freezingMillisecond / (60 * 1000));
        let second = Math.round((freezingMillisecond % (60 * 1000)) / 1000);
        Log.showInfo(TAG, `getFreezingTimeNm minute:${minute}, second:${second}`)
        let timeName = '';
        if (minute != 0) {
            timeName += minute + MINUTE_NM
        }
        if (second != 0) {
            timeName += second + SECOND_NM
        }
        return timeName;
    }

    updateStorage(callback) {
        Log.showInfo(TAG, `updateStorage`)
        callback()
    }

    checkFreezingTime(callback: Callback<void>) {
        service.getAuthProperty(AuthType.PIN, (properties) => {
            if (properties.freezingTime > 0) {
                this.inhibitInput = true
                //Clear the entered password
                this.changePrompt(properties.remainTimes, properties.freezingTime, callback)
            }
        })
    }
}
