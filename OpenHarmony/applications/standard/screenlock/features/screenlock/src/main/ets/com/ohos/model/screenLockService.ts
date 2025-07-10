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
import {Log, Trace, WriteFaultLog, FaultID, createOrGet, ScreenLockStatus} from '@ohos/common'
import ScreenLockModel from './screenLockModel';
import AccountModel, {AuthType, AuthSubType, AuthTurstLevel} from './accountsModel'
import Router from '@system.router';
import commonEvent from '@ohos.commonEvent';
import hiDebug from '@ohos.hidebug';
import systemParameter from '@ohos.systemparameter';
import {CommonEventPublishData} from 'commonEvent/commonEventPublishData';
import {Callback} from '@ohos.base';
import {UIContext} from '@ohos.arkui.UIContext';

const TAG = 'ScreenLock-ScreenLockService';
const URI_DIGITALPASSWORD = 'pages/digitalPassword'
const URI_MIXEDPASSWORD = 'pages/mixedPassword'
const URI_CUSTOMPASSWORD = 'pages/customPassword'

//Event type name
const EVENT_BEGIN_WAKEUP: string = 'beginWakeUp'
const EVENT_END_WAKEUP: string = 'endWakeUp'
const EVENT_BEGIN_SCREENON: string = 'beginScreenOn'
const EVENT_END_SCREEN_ON: string = 'endScreenOn'
const EVENT_BEGIN_SCREENOFF: string = 'beginScreenOff'
const EVENT_END_SCREENOFF: string = 'endScreenOff'
const EVENT_UNLOCK_SCREEN: string = 'unlockScreen'
const EVENT_LOCK_SCREEN: string = 'lockScreen'
const EVENT_BEGIN_EXITANIMATION: string = 'beginExitAnimation'
const EVENT_BEGIN_SLEEP: string = 'beginSleep'
const EVENT_END_SLEEP: string = 'endSleep'
const EVENT_CHANGE_USER: string = 'changeUser'
const EVENT_SCREENLOCK_ENABLE: string = 'screenlockEnabled'
const EVENT_SYSTEM_READY: string = 'systemReady'

const SERVICE_RESTART: string = 'serviceRestart'

const LOCK_SCREEN_RESULT: string = 'lockScreenResult'
const UNLOCK_SCREEN_RESULT: string = 'unlockScreenResult'
const SCREENLOCK_DRAW_DONE: string = 'screenDrawDone'

const ACTIVATING_TYPE = "activating"
const ACTIVATE_TYPE = "activate"
const ACTIVATING_EVENT = "activatingEvent"
const ACTIVATE_EVENT = "activateEvent"

const CHALLENGE_INT = 0

const MAIN_USER = 100

const MEMORY_MONITOR_PERIOD_MS = 600000
const MEMORY_MONITOR_LIMIT_KB = 120 * 1024

export {AuthType, AuthSubType};

export enum UnlockResult {
    Success = 0,
    Fail = 1,
    Cancel = 2
}

export enum LockResult {
    Success = 0,
    Fail = 1,
    Cancel = 2
}

let mRouterPath: string = ""

let mWillRecognizeFace: boolean = false

let mUnLockBeginAnimation: Callback<Callback<void>> = (callback: Callback<void>) => {
    callback()
}

export class ScreenLockService {
    accountModel: AccountModel = new AccountModel()
    screenLockModel: ScreenLockModel = new ScreenLockModel()
    currentLockStatus : ScreenLockStatus;
    memoryMonitor: number = -1;
    init() {
        Log.showDebug(TAG, 'init');
        this.startMonitorMemory();
        this.accountModel.modelInit();
        this.monitorEvents();
        this.accountModel.updateAllUsers()
        this.checkPinAuthProperty(() => {
            Log.showInfo(TAG, `checkPinAuthProperty back`)
            this.authUserByFace()
        })
    }

    monitorEvents() {
        Log.showDebug(TAG, 'registered events start');
        this.screenLockModel.eventListener((typeName: String) => {
            switch (typeName) {
            // System ready on device boot
                case EVENT_SYSTEM_READY:
                    Log.showInfo(TAG, `EVENT_SYSTEM_READY event`);
                    this.lockScreen();
                    break;
            //Bright screen
                case EVENT_END_SCREEN_ON:
                    Log.showInfo(TAG, `EVENT_END_SCREEN_ON event`);
                    this.authUserByFace()
                    AppStorage.SetOrCreate('deviceStatus', EVENT_END_SCREEN_ON);
                    break;
            //The device is going to sleep
                case EVENT_BEGIN_SLEEP:
                    Trace.start(Trace.CORE_METHOD_SLEEP_TO_LOCK_SCREEN);
                    Log.showInfo(TAG, `EVENT_BEGIN_SLEEP event`);
                    this.backPage();
                    this.lockScreen();
                    this.accountModel.updateAllUsers()
                    AppStorage.SetOrCreate('deviceStatus', EVENT_BEGIN_SLEEP);
                    break;
            //unlock request was received
                case EVENT_UNLOCK_SCREEN:
                    Log.showInfo(TAG, `EVENT_UNLOCK_SCREEN event`);
                    this.unlockScreen();
                    break;
            //lock request was received
                case EVENT_LOCK_SCREEN:
                    Log.showInfo(TAG, `EVENT_LOCK_SCREEN event`);
                    this.lockScreen();
                    break;
                case SERVICE_RESTART:
                    setTimeout(() => {
                        this.monitorEvents();
                        this.lockScreen();
                    }, 2000);
                    break;
                default:
                    Log.showError(TAG, `eventListener:typeName ${typeName}`)
            }
        })

        this.accountModel.eventListener(ACTIVATING_TYPE, ACTIVATING_EVENT, () => {
            Log.showInfo(TAG, `ACTIVATING_TYPE event`);
        })

        this.accountModel.eventListener(ACTIVATE_TYPE, ACTIVATE_EVENT, () => {
            Log.showInfo(TAG, `ACTIVATE_TYPE event`);
            this.lockScreen();
            this.accountModel.updateAllUsers()
            this.checkPinAuthProperty(() => {
                Log.showInfo(TAG, `checkPinAuthProperty back`)
                this.authUserByFace()
            })
        })

        this.accountModel.commonEventListener(()=>{
            Log.showInfo(TAG, `commonEventListener event`);
            this.accountModel.updateAllUsers();
        })

        Log.showDebug(TAG, 'registered events end');
    }

    lockScreen() {
        Trace.start(Trace.CORE_METHOD_SHOW_LOCK_SCREEN);
        Log.showDebug(TAG, `lockScreen`);

        //lock the screen
        this.screenLockModel.showScreenLockWindow(() => {
            Log.showInfo(TAG, `showScreenLockWindow finish`);
            this.checkPinAuthProperty(() => {
            });
            Log.showInfo(TAG, `screenlock status:${this.currentLockStatus}, userId : ${this.accountModel.getCurrentUserId()}`);
            if (this.currentLockStatus == ScreenLockStatus.Locking) {
                Log.showInfo(TAG, `had locked, no need to publish lock_screen`);
            } else {
                this.notifyLockScreenResult(LockResult.Success)
                systemParameter.set('bootevent.lockscreen.ready','true')
                this.currentLockStatus = ScreenLockStatus.Locking;
            }
        });
    }

    private checkPinAuthProperty(callback: Callback<void>) {
        Log.showDebug(TAG, "checkPinAuthProperty")
        this.accountModel.getAuthProperty(AuthType.PIN, (properties) => {
            Log.showInfo(TAG, `checkPinAuthProperty: AUTH_SUB_TYPE:${properties.authSubType}`);
            switch (properties.authSubType) {
                case AuthSubType.PIN_SIX:
                    AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Locking);
                    mRouterPath = URI_DIGITALPASSWORD;
                    this.checkFaceAuthProperty(() => {
                        callback()
                    })
                    break;
                case AuthSubType.PIN_MIXED:
                    AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Locking);
                    mRouterPath = URI_MIXEDPASSWORD;
                    this.checkFaceAuthProperty(() => {
                        callback()
                    })
                    break;
                case AuthSubType.PIN_NUMBER:
                    AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Locking);
                    mRouterPath = URI_CUSTOMPASSWORD;
                    this.checkFaceAuthProperty(() => {
                        callback()
                    })
                    break;
                default:
                    AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Unlock);
                    mWillRecognizeFace = false
            }
        })
    }

    private checkFaceAuthProperty(callback: Callback<void>) {
        Log.showDebug(TAG, "checkFaceAuthProperty")
        this.accountModel.getAuthProperty(AuthType.FACE, (properties) => {
            Log.showInfo(TAG, `checkFaceAuthProperty：AUTH_SUB_TYPE:${properties.authSubType}`);
            switch (properties.authSubType) {
                case AuthSubType.FACE_2D:
                case AuthSubType.FACE_3D:
                    mWillRecognizeFace = true
                    callback()
                    break;
                default:
                    mWillRecognizeFace = false
            }
        })
    }

    unlockScreen() {
        Log.showInfo(TAG, `unlockScreen`);
        this.accountModel.isActivateAccount((isActivate: boolean) => {
            if (!isActivate) {
                return
            }
            mUnLockBeginAnimation(() => {
                let status = AppStorage.Link('lockStatus')
                Log.showDebug(TAG, `unlocking lockStatus:${JSON.stringify(status?.get())}`);
                if (status?.get() == ScreenLockStatus.Unlock) {
                    Log.showInfo(TAG, `unlock the screen`);
                    this.unlocking();
                } else {
                    let slidestatus = AppStorage.Get('slidestatus')
                    if(!slidestatus){
                        AppStorage.SetOrCreate('slidestatus', true);
                        const UIContext: UIContext = AppStorage.get('UIContext');
                        Log.showInfo(TAG, `this.UIContext is ${UIContext}`)
                        Log.showInfo(TAG, `unlockScreen Router.push`);
                        UIContext.getRouter().pushUrl({ url: mRouterPath })
                    }
                }
            })
        })
    }

    unlocking() {
        Log.showInfo(TAG, `unlocking`);
        //set the lockStatus to 'Unlock'
        AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Unlock);
        this.currentLockStatus = ScreenLockStatus.Unlock;
        AppStorage.SetOrCreate('slidestatus', false);
        //unlock the screen
        this.screenLockModel.hiddenScreenLockWindow(() => {
            Log.showInfo(TAG, `hiddenScreenLockWindow finish`);
            //notify the base service that the unlock is completed
            this.notifyUnlockScreenResult(UnlockResult.Success);
        });
    }

    notifyUnlockScreenResult(result: UnlockResult) {
        Log.showInfo(TAG, `notifyUnlockScreenResult`);
        this.screenLockModel.sendScreenLockEvent(UNLOCK_SCREEN_RESULT, result, (error, data) => {
            Log.showInfo(TAG, `notifyUnlockScreenResult: error:${JSON.stringify(error)} data:${JSON.stringify(data)}`);
        });
    }

    notifyLockScreenResult(result: LockResult) {
        Log.showInfo(TAG, `notifyLockScreenResult`);
        this.screenLockModel.sendScreenLockEvent(LOCK_SCREEN_RESULT, result, (error, data) => {
            Log.showInfo(TAG, `notifyLockScreenResult: error:${JSON.stringify(error)} data:${JSON.stringify(data)}`);
        });
    }

    notifyDrawDone() {
        Log.showInfo(TAG, `notifyDrawDone`);
        //notify the base service that the screen is loaded
        this.screenLockModel.sendScreenLockEvent(SCREENLOCK_DRAW_DONE, 0, (error, result) => {
            Log.showInfo(TAG, `notifyDrawDone:  error:${JSON.stringify(error)} result:${JSON.stringify(result)}`);
        });
    }

    authUser(authSubType: AuthSubType, passwordData: number[] | string, callback): void {
        Log.showInfo(TAG, `authUser authSubType:${authSubType}`);
        let password: string = '';
        if (typeof passwordData == 'string') {
            password = passwordData;
        } else {
            password = passwordData.join('');
        }
        this.accountModel.registerPWDInputer(password).then(() => {
            Log.showInfo(TAG, `registerPWDInputer success`);
            this.accountModel.authUser(CHALLENGE_INT, AuthType.PIN, AuthTurstLevel.ATL4, (result, extraInfo) => {
                Log.showDebug(TAG, `authUser  callback:${result} extraInfo:${JSON.stringify(extraInfo)}`);
                this.accountModel.unregisterInputer();
                callback(result, extraInfo);
            })
        }).catch(() => {
            Log.showError(TAG, `registerPWDInputer fails`);
        })
    }

    authUserByFace() {
        if (!mWillRecognizeFace) {
            Log.showInfo(TAG, "Recognize face is not support")
            return
        }
        Log.showInfo(TAG, `authUserByFace`);
        this.accountModel.authUser(CHALLENGE_INT, AuthType.FACE, AuthTurstLevel.ATL1, (result, extraInfo) => {
            Log.showDebug(TAG, `authUserByFace callback:${result} extraInfo:${JSON.stringify(extraInfo)}`);
            if (result == 0) {
                AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.Unlock);
                this.unlockScreen()
            } else {
                AppStorage.SetOrCreate('lockStatus', ScreenLockStatus.FaceNotRecognized);
                this.currentLockStatus = ScreenLockStatus.FaceNotRecognized;
            }
        })
    }

    onUserSwitch(userId: number) {
        this.accountModel.onUserSwitch(userId)
    }

    goBack() {
        Log.showInfo(TAG, `screen lock service goBack`);
        Router.back();
        this.notifyUnlockScreenResult(UnlockResult.Cancel)
        this.accountModel.unregisterInputer();
    }

    backPage(): void {
        if (mRouterPath != "" && this.currentLockStatus !== ScreenLockStatus.Unlock) {
            this.goBack();
        }
    }

    destroy() {
        this.screenLockModel.eventCancelListener(EVENT_SYSTEM_READY);
        this.screenLockModel.eventCancelListener(EVENT_END_SCREEN_ON);
        this.screenLockModel.eventCancelListener(EVENT_BEGIN_SLEEP);
        this.screenLockModel.eventCancelListener(EVENT_UNLOCK_SCREEN);
        this.accountModel.eventCancelListener(ACTIVATING_TYPE, ACTIVATING_EVENT);
        this.accountModel.eventCancelListener(ACTIVATE_TYPE, ACTIVATE_EVENT)
        this.accountModel.commonEventCancelListener();
        this.accountModel.modelFinish()
        this.stopMonitorMemory()
    }

    setUnlockAnimation(beginAnimation: Callback<Callback<void>>) {
        mUnLockBeginAnimation = beginAnimation;
    }

    getAuthProperty(authType, callback) {
        Log.showInfo(TAG, `getAuthProperty param: authType ${authType}`);
        this.accountModel.getAuthProperty(authType, (properties) => {
            callback(properties);
        })
    }

    private publishByUser(eventName: string, activeUserId: number) {
        Log.showDebug(TAG, `publishByUser event name: ${eventName}, userId: ${activeUserId}`)
        let publishData : CommonEventPublishData = {
            parameters : {
                userId : activeUserId
            }
        };
        commonEvent.publish(eventName, publishData, (error, value) => {
            if (error.code) {
                Log.showError(TAG, 'Operation failed. Cause: ' + JSON.stringify(error));
            } else {
                Log.showDebug(TAG, 'publish common event success. ' + JSON.stringify(value));
            }
        });
    }

    private startMonitorMemory() {
        this.memoryMonitor = setInterval(() => {
            const pss = hiDebug.getPss();
            Log.showInfo(TAG, `app pss info is: ${pss}`);
            if (pss > MEMORY_MONITOR_LIMIT_KB) {
                WriteFaultLog({FAULT_ID: FaultID.MEMORY, MSG: "pss over limit"})
            }
        }, MEMORY_MONITOR_PERIOD_MS)
    }

    private stopMonitorMemory() {
        if (this.memoryMonitor !== -1) {
            clearInterval(this.memoryMonitor);
            this.memoryMonitor = -1;
        }
    }
}

let screenLockService = createOrGet(ScreenLockService, TAG);

export default screenLockService as ScreenLockService;