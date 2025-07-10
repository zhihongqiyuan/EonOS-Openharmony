// @ts-nocheck
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
import {Log, Trace, SysFaultLogger, FaultID} from '@ohos/common'
import ScreenLockMar from '@ohos.screenLock';
import windowManager  from '@ohos.window'
import Constants from '../common/constants'
import {Callback} from '@ohos.base';

const TAG = 'ScreenLock-ScreenLockModel';

export default class ScreenLockModel {
    @SysFaultLogger({FAULT_ID: FaultID.SCREEN_LOCK_MANAGER, MSG: "call func on failed"})
    eventListener(callback: Callback<String>) {
        let isSuccess = null
        try {
            isSuccess = ScreenLockMar.onSystemEvent((event)=>{
                Log.showInfo(TAG, `eventListener:callback:${event.eventType}`)
                callback(event.eventType);
            });
        }  catch (err: any) {
            Log.showError(TAG, `on callback error -> ${JSON.stringify(err)}`);
        }
        if (!isSuccess) {
            callback('serviceRestart');
        }
    }

    eventCancelListener(typeName: string) {
        Log.showDebug(TAG, `eventCancleListener:typeName ${typeName}`);
        // As off has some problem and there is no case to cancel, do nothing
    }

    @SysFaultLogger({FAULT_ID: FaultID.SCREEN_LOCK_MANAGER, MSG: "call func sendScreenLockEvent failed"})
    sendScreenLockEvent(typeName: string, typeNo: number, callback) {
        Log.showInfo(TAG, `sendScreenLockEvent: typeName ${typeName} typeNo  ${typeNo} `);
        ScreenLockMar.sendScreenLockEvent(typeName, typeNo, (err, data) => {
            callback(err, data);
        })
    }

    showScreenLockWindow(callback: Callback<void>) {
        Log.showInfo(TAG, `isWallpaperShow is true: ${AppStorage.Get('isWallpaperShow')}`);
        AppStorage.SetOrCreate('isWallpaperShow', true);
        windowManager.find(Constants.WIN_NAME).then((win) => {
            win.show().then(() => {
                Log.showInfo(TAG, `window show`);
                callback();
            })
        })
    }

    hiddenScreenLockWindow(callback: Callback<void>) {
        Log.showInfo(TAG, `window hide`);
        AppStorage.SetOrCreate('isWallpaperShow', false);
        Trace.end(Trace.CORE_METHOD_PASS_ACCOUNT_SYSTEM_RESULT);
        windowManager.find(Constants.WIN_NAME).then((win) => {
            Trace.start(Trace.CORE_METHOD_HIDE_PSD_PAGE);
            win.hide().then(() => {
                Log.showInfo(TAG, `window hide`);
                callback();
            })
        })
    }
}
