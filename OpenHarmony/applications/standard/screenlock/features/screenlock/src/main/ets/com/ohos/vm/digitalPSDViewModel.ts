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

import {Log, Trace} from '@ohos/common'
import Constants from '../common/constants'
import BaseViewModel, {service, AuthSubType} from './baseViewModel'
import {Callback} from '@ohos.base';
import { NumKeyBoardValue } from '../../../../../../../noticeitem/src/main/ets/com/ohos/noticeItem/common/constants'

const TAG = 'ScreenLock-DigitalPSDViewModel'

const PW_LEN = 6

export default class DigitalPSDViewModel extends BaseViewModel {
    password: number[]= []
    passwdMaskArr: Resource[];
    numKeyboard: NumKeyBoardValue[] = Constants.NUMKEY_BOARD;

    constructor() {
        super();
    }

    ViewModelInit(): void{
        Log.showDebug(TAG, 'ViewModelInit');
        this.passwdMaskArr = new Array(PW_LEN).fill($r('app.media.ic_hollow_dot'));
        super.ViewModelInit();
    }

    onKeyPress(index: number, callback: Callback<void>): void {
        Log.showInfo(TAG, `onKeyPress start param: ${index}`)
        let keyValue = this.numKeyboard[index].value;
        if (keyValue >= 0 && !this.inhibitInput) {
            if (this.password.length < PW_LEN) {
                this.password.push(keyValue);
                this.passwdMaskArr[this.password.length-1] = $r('app.media.ic_solid_dot');
                this.numKeyboard[11].row1 = $r('app.string.delete');
                this.numKeyboard[11].value = Constants.DEL_PWD;
                this.updateStorage(callback);
            }
            if (this.password.length >= PW_LEN) {
                Trace.start(Trace.CORE_METHOD_UNLOCK_SCREEN);
                Trace.start(Trace.CORE_METHOD_CALL_ACCOUNT_SYSTEM);
                service.authUser(AuthSubType.PIN_SIX, this.password, (result, extraInfo) => {
                    if (result == 0) {
                        //unlock the screen
                        service.unlocking();
                        service.goBack();
                    } else {
                        //Clear the entered password
                        super.changePrompt(extraInfo.remainTimes, extraInfo.freezingTime, callback)
                    }
                })
                this.clearPassword()
            }
        } else if (keyValue == Constants.DEL_PWD) {
            this.passwdMaskArr[this.password.length-1] = $r('app.media.ic_hollow_dot');
            this.password.pop()
            if (this.password.length == 0) {
                this.numKeyboard[11].row1 = $r('app.string.back');
                this.numKeyboard[11].value = Constants.GO_BACK;
            }
            this.updateStorage(callback);
        } else if (keyValue == Constants.GO_BACK) {
            AppStorage.SetOrCreate('slidestatus', false);
            service.goBack();
        } else if (keyValue == Constants.CALL_PHONE) {
        }
    }

    clearPassword() {
        Log.showInfo(TAG, `clearPassword`)
        this.password = [];
        this.passwdMaskArr = new Array(PW_LEN).fill($r('app.media.ic_hollow_dot'));
        AppStorage.SetOrCreate('passwdMaskArr', this.passwdMaskArr);
        this.numKeyboard[11].row1 = $r('app.string.back');
        this.numKeyboard[11].value = Constants.GO_BACK;
        this.updateStorage(() => {
        })
    }

    updateStorage(callback) {
        Log.showInfo(TAG, `updateStorage child`)
        //refresh  the page
        AppStorage.SetOrCreate('passwdMaskArr', this.passwdMaskArr);
        AppStorage.SetOrCreate('numKeyboard', this.numKeyboard);
        super.updateStorage(callback)
    }
}
