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
import Log from '../../../../../../../../common/src/main/ets/default/Log';
import Notification from '@ohos.notification';
import NotificationManager from '@ohos.notificationManager';
import NotificationSubscribe from '@ohos.notificationSubscribe';

const TAG = 'NotificationManagenment-NotificationListener';

export interface IOnEnableChanged {
    (value: boolean): void;
}

interface EnableListener {
    bundle: string;
    onEnableChanged: IOnEnableChanged;
}

export interface BundleOption {
    bundle: string;
    uid?: number;
}

export class NotificationListener {
    private readonly listeners = new Map<string, Set<IOnEnableChanged>>();
    private subscriber: NotificationSubscribe.NotificationSubscriber = {
        onEnabledNotificationChanged: this.handleEnabledNotificationChanged.bind(this)
    };

    subscribeEnableChanged(): void {
        Log.showDebug(TAG, `subscribeEnableChanged ${this.subscriber}`);

        NotificationSubscribe.subscribe(this.subscriber, err => {
            if (err) {
                Log.showInfo(TAG, `subscribeEnableChanged error ${JSON.stringify(err)}`);
            } else {
                Log.showInfo(TAG, 'subscribeEnableChanged finished');
            }
        });
    }

    unsubscribeEnableChanged(): void {
        Log.showDebug(TAG, `unsubscribeEnableChanged start  ${this.subscriber}`);
        this.unRegisterAll();

        NotificationSubscribe.unsubscribe(this.subscriber, err => {
            if (err) {
                Log.showInfo(TAG, `unsubscribeEnableChanged error ${JSON.stringify(err)}`);
            } else {
                Log.showInfo(TAG, 'unsubscribeEnableChanged finished');
            }
        });
    }

    handleEnabledNotificationChanged(data: NotificationSubscribe.EnabledNotificationCallbackData): void {
        Log.showDebug(TAG, `handleEnabledNotificationChanged data:${JSON.stringify(data)} `);

        const callbacks = this.listeners.get(data.bundle);
        if (callbacks && callbacks.size) {
            callbacks.forEach(cb => cb(data.enable));
        }
    }

    register(listener: EnableListener): void {
        let callbacks: Set<IOnEnableChanged> = this.listeners.get(listener.bundle);
        if (!callbacks) {
            callbacks = new Set();
            this.listeners.set(listener.bundle, callbacks);
        }

        callbacks.add(listener.onEnableChanged);
        Log.showDebug(TAG, 'register finished');
    }

    unRegister(listener: EnableListener): void {
        const callbacks = this.listeners.get(listener.bundle);
        if (!callbacks) {
            Log.showDebug(TAG, 'unRegister finished by empty');
            return;
        }

        callbacks.delete(listener.onEnableChanged);
        Log.showDebug(TAG, 'unRegister finished');
    }

    unRegisterAll(): void {
        this.listeners.clear();
        Log.showDebug(TAG, 'unRegisterAll finished');
    }

    async isNotificationEnabled(bundleOption: BundleOption, callback?: (data) => void): Promise<boolean> {
        Log.showDebug(TAG, `isNotificationEnabled bundleOption:${JSON.stringify(bundleOption)} `);
        return new Promise((resolve, reject) => {
          NotificationManager.isNotificationEnabled(bundleOption, (err, data) => {
                Log.showInfo(TAG, `isNotificationEnabled callback data:${JSON.stringify(data)} err:${JSON.stringify(err)}`);
                if (!!data) {
                    if (callback) {
                        callback(data);
                    }
                    resolve(data);
                } else {
                    reject(err);
                }
            });
        });
    }

    enableNotification(bundleOption: BundleOption, data: boolean): void {
        Log.showDebug(TAG, `enableNotification bundleOption:${JSON.stringify(bundleOption)} data:${JSON.stringify(data)}`);
        let enableValue: boolean = data ? true : false;
        NotificationManager.setNotificationEnable(bundleOption, enableValue, (err, result) => {
            Log.showInfo(TAG, `enableNotification err:${JSON.stringify(err)} result:${JSON.stringify(result)}`);
        });
    }

    async isNotificationSlotEnabled(bundleOption: BundleOption, slotType: Notification.SlotType, callback?: (data: boolean) => void): Promise<boolean> {
        Log.showDebug(TAG, `isNotificationSlotEnabled bundleOption:${JSON.stringify(bundleOption)} `);
        return new Promise((resolve, reject) => {
            NotificationManager.isNotificationSlotEnabled(bundleOption, slotType, (err, data) => {
                Log.showInfo(TAG, `isNotificationSlotEnabled callback data:${JSON.stringify(data)} err:${JSON.stringify(err)}`);
                if (!!data) {
                    if (callback) {
                        callback(data);
                    }
                    resolve(data);
                } else {
                    reject(err);
                }
            });
        });
    }

    enableNotificationSlot(bundleOption: BundleOption, slotType: Notification.SlotType, data: boolean): void {
        Log.showDebug(TAG, `enableNotificationSlot bundleOption:${JSON.stringify(bundleOption)} data:${JSON.stringify(data)}`);
        let enableValue: boolean = data ? true : false;
        NotificationManager.setNotificationEnableSlot(bundleOption, slotType, enableValue, (err, result) => {
            Log.showInfo(TAG, `enableNotificationSlot err:${JSON.stringify(err)} result:${JSON.stringify(result)}`);
        });
    }

    notificationSlotSet(bundleOption: BundleOption, data: NotificationManager.NotificationSlot): void {
        Log.showDebug(TAG, `notificationSlotSet bundleOption:${JSON.stringify(bundleOption)} data:${JSON.stringify(data)}`);
        try {
            NotificationManager.setSlotByBundle(bundleOption, data, (err, result) => {
                Log.showInfo(TAG, `notificationSlotSet err:${JSON.stringify(err)} result:${JSON.stringify(result)}`);
            });
        } catch (err) {
            Log.showError(TAG, `notificationSlotSet error => ${JSON.stringify(err)}`);
        }
    }

    async isDistributedEnabled(): Promise<boolean> {
        Log.showInfo(TAG, 'isDistributedEnabled');
        return new Promise((resolve, reject) => {
            NotificationManager.isDistributedEnabled().then((data) => {
                Log.showInfo(TAG, `isDistributedEnabled data:${data ? 'true' : 'false'}`);
                resolve(data);
            }).catch((err) => {
                Log.showError(TAG, `isDistributedEnabled err:${JSON.stringify(err)}`);
                reject(err);
            });
        });
    }

    enableDistributed(data?: boolean): void {
        Log.showDebug(TAG, `enableDistributed data:${JSON.stringify(data)}`);
        let enableValue: boolean = data ? true : false;
        NotificationManager.setDistributedEnable(enableValue, (err, result) => {
            Log.showInfo(TAG, `enableDistributed err:${JSON.stringify(err)} result:${JSON.stringify(result)}`);
        });
    }
}

let notificationListener = new NotificationListener();

export default notificationListener;