// @ts-nocheck
/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import osAccount from '@ohos.account.osAccount'
import commonEvent from '@ohos.commonEvent';
import util from '@ohos.util';
import {Callback} from '@ohos.base';
import {Log, Trace, sEventManager, CommonEventManager, getCommonEventManager, SysFaultLogger, FaultID, obtainLocalEvent} from '@ohos/common'
import {UserData} from '../data/userData';

const TAG = "ScreenLock-AccountsModel"
const TYPE_ADMIN = 0;
const TYPE_NORMAL = 1;
const TYPE_GUEST = 2;

export const ACCOUNTS_REFRESH_EVENT = "Accounts_Refresh_Event";

export enum AuthType {
    //Authentication type pin.
    PIN = 1,
    //Authentication type face.
    FACE = 2
}

export enum AuthSubType {
    //Authentication sub type six number pin.
    PIN_SIX = 10000,
    //Authentication sub type self defined number pin.
    PIN_NUMBER = 10001,
    //Authentication sub type mixed number pin.
    PIN_MIXED = 10002,
    //Authentication sub type 2D face.
    FACE_2D = 20000,
    //Authentication sub type 3D face.
    FACE_3D = 20001
}

export enum AuthTurstLevel {
    //Authentication result trusted level 1.
    ATL1 = 10000,
    //Authentication result trusted level 2.
    ATL2 = 20000,
    //Authentication result trusted level 3.
    ATL3 = 30000,
    //Authentication result trusted level 4.
    ATL4 = 40000
}

export enum GetPropertyType {
    //Authentication remain times.
    AUTH_SUB_TYPE = 1,
    //Authentication remain times.
    REMAIN_TIMES = 2,
    //Authentication remain times.
    FREEZING_TIME = 3
}

export enum ResultCode {
    //success
    SUCCESS = 0,
    //fails
    FAIL = 1,
}
const USER_SUBSCRIBE_INFO = {
    events: [
        commonEvent.Support.COMMON_EVENT_USER_REMOVED,
    ],
};

export default class AccountsModel {
    userAuthManager = new osAccount.UserAuth();
    pinAuthManager = new osAccount.PINAuth();
    mCurrentUserId: number = 100
    private mManager?: CommonEventManager;
    modelInit() {
        Log.showDebug(TAG, "start ModelInit")
    }

    @SysFaultLogger({FAULT_ID: FaultID.ACCOUNT_SYSTEM, MSG: "call func on failed"})
    eventListener(typeName: "activate" | "activating", name: string, callback: Callback<void>) {
        Log.showInfo(TAG, `eventListener:typeName ${typeName}`);
        osAccount.getAccountManager().on(typeName, name, (userId: number) => {
            Log.showInfo(TAG, `on ${typeName} callback userId = ${userId}`)
            if (typeName == "activate"){
                this.mCurrentUserId = userId
            }
            callback()
        })
    }

    commonEventListener(callback: Callback<void>) {
        this.mManager = getCommonEventManager(
            TAG,
            USER_SUBSCRIBE_INFO,
            (data) => {
                Log.showInfo(TAG, `USER_REMOVED. ${JSON.stringify(data)}`);
                callback();
            },
        );
        this.mManager.subscriberCommonEvent();

        Log.showInfo(TAG, `commonEventListener eventListener`);
        let subscribeInfo = {events: ['usual.event.USER_REMOVED']};
        commonEvent.createSubscriber(subscribeInfo, (error, subscriber) => {
            Log.showInfo(TAG, `createSubscriber success. ${JSON.stringify(subscriber)}`);
            commonEvent.subscribe(subscriber, (err, commonEventData) => {
                Log.showInfo(TAG, `USER_REMOVED subscribe. ${JSON.stringify(commonEventData)}`);
                callback();
            });
        });
    }

    commonEventCancelListener() {
        Log.showInfo(TAG, "cancel commonEvent");
        this.mManager?.release();
        this.mManager = undefined;
    }

    @SysFaultLogger({FAULT_ID: FaultID.ACCOUNT_SYSTEM, MSG: "call func off failed"})
    eventCancelListener(typeName: "activate" | "activating", name: string) {
        Log.showInfo(TAG, `eventCancleListener:typeName ${typeName}`);
        osAccount.getAccountManager().off(typeName, name);
    }

    updateAllUsers() {
        this.addAllUsers();
    }

    @SysFaultLogger({FAULT_ID: FaultID.ACCOUNT_SYSTEM, MSG: "call func queryAllCreatedOsAccounts failed"})
    private addAllUsers() {
        Log.showDebug(TAG, "start getAllUsers");
        osAccount.getAccountManager().queryAllCreatedOsAccounts().then((list) => {
            Log.showDebug(TAG, "start sort");
            let accountList = [];
            let accountMap = new Map();
            list.sort(this.sortAccount.bind(this));
            for (const user of list) {
                Log.showDebug(TAG, `start get user, localId=${user.localId}, localName=${user.localName}`);
                if (user.isActived) {
                    this.mCurrentUserId = user.localId
                }
                let userData: UserData = {
                    userId: user.localId,
                    userName: user.localName,
                    userIconPath: ""
                }
                accountList.push(userData)
                accountMap.set(user.localId, userData)
                osAccount.getAccountManager().getOsAccountProfilePhoto(user.localId).then((path) => {
                    Log.showDebug(TAG, "start get photo:" + path);
                    accountMap.get(user.localId).userIconPath = path;
                })
            }
            sEventManager.publish(obtainLocalEvent(ACCOUNTS_REFRESH_EVENT, accountList));
        })
    }

    private sortAccount(info1, info2): number {
        if (info1.isActived || info2.isActived) {
            return info1.isActived ? -1 : 1;
        } else if (info1.type.ADMIN == TYPE_ADMIN || info2.type.ADMIN == TYPE_ADMIN) {
            return info1.type.ADMIN == TYPE_ADMIN ? -1 : 1;
        } else if (info1.type.GUEST == TYPE_GUEST || info2.type.GUEST == TYPE_GUEST) {
            return info1.type.GUEST == TYPE_GUEST ? 1 : -1;
        } else {
            return info2.localId - info1.localId;
        }
    }

    @SysFaultLogger({FAULT_ID: FaultID.ACCOUNT_SYSTEM, MSG: "call func activateOsAccount failed"})
    onUserSwitch(userId: number) {
        Log.showDebug(TAG, "onUserSwitch:" + userId)
        osAccount.getAccountManager().activateOsAccount(userId).then(() => {
            Log.showInfo(TAG, "activateOsAccount : " + userId);
        })
    }

    authUser(challenge, authType: AuthType, authLevel: number, callback) {
        Log.showDebug(TAG, `authUser param: userId ${this.mCurrentUserId} challenge ${challenge}`);
        Trace.end(Trace.CORE_METHOD_CALL_ACCOUNT_SYSTEM);
        let challengeArray = new Uint8Array(challenge);
        try {
            this.userAuthManager.authUser(this.mCurrentUserId, challengeArray, authType, authLevel, {
                onResult: (result, extraInfo) => {
                    Log.showInfo(TAG, `authUser UserAuthManager.authUser onResult`);
                    Trace.start(Trace.CORE_METHOD_PASS_ACCOUNT_SYSTEM_RESULT);
                    callback(result, extraInfo);
                },
                onAcquireInfo: (moduleId, acquire, extraInfo) => {
                    Log.showInfo(TAG, `authUser UserAuthManager.authUser onAcquireInfo`);
                }
            }
            )
        } catch(error) {
            console.error(`authUser failed, code is ${error.code}, message is ${error.message}`);
        }
    }

    getAuthProperty(authType, callback) {
        Log.showDebug(TAG, `getAuthProperty param: authType ${authType}`);
        let keyArray = [GetPropertyType.AUTH_SUB_TYPE, GetPropertyType.REMAIN_TIMES, GetPropertyType.FREEZING_TIME]
        let request = {
            'authType': authType,
            'keys': keyArray
        }
        try {
            this.userAuthManager.getProperty(request).then((properties) => {
                Log.showInfo(TAG, `getAuthProperty properties ${JSON.stringify(properties)}`);
                callback(properties)
            })
        } catch (error) {
            console.error(`getProperty failed, code is ${error.code}, message is ${error.message}`);
        };
    }

    registerPWDInputer(password: string): Promise<void> {
        Log.showDebug(TAG, `registerPWDInputer`);
        let result = this.registerInputer(password);
        if (result) {
            return Promise.resolve();
        } else {
            return Promise.reject();
        }
    }

    private registerInputer(password: string): boolean {
        Log.showDebug(TAG, `registerInputer`);
        let result = null
        try {
            result = this.pinAuthManager.registerInputer({
                onGetData: (passType, inputData) => {
                    Log.showDebug(TAG, `registerInputer onSetData passType:${passType}`);
                    let textEncoder = new util.TextEncoder();
                    let uint8PW = textEncoder.encode(password);
                    Log.showDebug(TAG, `registerInputer onSetData call`);
                    inputData.onSetData(passType, uint8PW);
                }
            })
        } catch(e) {
            console.error(`registerInputer failed, code is ${e.code}, message is ${e.message}`);
        }

        Log.showInfo(TAG, `registerInputer result:${result}`);
        return result;
    }

    unregisterInputer() {
        Log.showDebug(TAG, `unregisterInputer`);
        try {
            this.pinAuthManager.unregisterInputer();
        } catch {
            LogUtil.debug(`${this.TAG}unregisterInputer failed`);
        }
    }

    modelFinish() {
        Log.showDebug(TAG, "start modelFinish")
    }

    @SysFaultLogger({FAULT_ID: FaultID.ACCOUNT_SYSTEM, MSG: "call func isOsAccountActived failed"})
    isActivateAccount(callback: Callback<boolean>) {
        Log.showDebug(TAG, `isActivateAccount userId:${this.mCurrentUserId}`)
        osAccount.getAccountManager().isOsAccountActived(this.mCurrentUserId).then((isActivate) => {
            Log.showInfo(TAG, `isActivateAccount userId:${this.mCurrentUserId} result: ${isActivate}`)
            callback(isActivate)
        })
    }
    getCurrentUserId() {
        return this.mCurrentUserId;
    }
}