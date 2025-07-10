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

/**
 * @file
 * @kit BasicServicesKit
 */

import { AsyncCallback } from './@ohos.base';
import { Callback } from './@ohos.base';

/**
 * systemScreenLock
 *
 * @namespace screenLock
 * @syscap SystemCapability.MiscServices.ScreenLock
 * @since 7
 */
declare namespace screenLock {
  /**
   * Checks whether the screen is currently locked.
   *
   * @param { AsyncCallback<boolean> } callback - the callback of isScreenLocked.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function isScreenLocked(callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether the screen is currently locked.
   *
   * @returns { Promise<boolean> } the promise returned by the function.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function isScreenLocked(): Promise<boolean>;

  /**
   * Checks whether the screen is currently locked.
   *
   * @returns { boolean } returns true if the screen is currently locked, returns false otherwise.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isLocked(): boolean;

  /**
   * Checks whether the screen lock of the current device is secure.
   *
   * @param { AsyncCallback<boolean> } callback - the callback of isSecureMode.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function isSecureMode(callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether the screen lock of the current device is secure.
   *
   * @returns { Promise<boolean> } the promise returned by the function.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function isSecureMode(): Promise<boolean>;

  /**
   * Unlock the screen.
   *
   * @param { AsyncCallback<void> } callback - the callback of unlockScreen.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function unlockScreen(callback: AsyncCallback<void>): void;

  /**
   * Unlock the screen.
   *
   * @returns { Promise<void> } the promise returned by the function.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @since 7
   * @deprecated since 9
   */
  function unlockScreen(): Promise<void>;

  /**
   * Unlock the screen.
   *
   * @param { AsyncCallback<boolean> } callback - the callback of unlock.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  /**
   * Unlock the screen.
   *
   * @param { AsyncCallback<boolean> } callback - the callback of unlock.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @throws { BusinessError } 13200003 - Invalid use.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function unlock(callback: AsyncCallback<boolean>): void;

  /**
   * Unlock the screen.
   *
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  /**
   * Unlock the screen.
   *
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @throws { BusinessError } 13200003 - Invalid use.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function unlock(): Promise<boolean>;

  /**
   * Lock the screen.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @param { AsyncCallback<boolean> } callback - the callback of lock.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function lock(callback: AsyncCallback<boolean>): void;

  /**
   * Lock the screen.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function lock(): Promise<boolean>;

  /**
   * Indicates the system event type related to the screenlock management service.
   *
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  /**
   * Indicates the system event type related to the screenlock management service. Adding strongAuthChanged and screenLockDisabledChanged.
   *
   * @typedef {('beginWakeUp' | 'endWakeUp' | 'beginScreenOn' | 'endScreenOn' | 'beginScreenOff' | 'endScreenOff' | 'unlockScreen'
   * | 'lockScreen' | 'beginExitAnimation' | 'beginSleep' | 'endSleep' | 'changeUser' | 'screenlockEnabled' | 'serviceRestart'
   * | 'strongAuthChanged' | 'screenLockDisabledChanged')}
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  type EventType =
    'beginWakeUp'
    | 'endWakeUp'
    | 'beginScreenOn'
    | 'endScreenOn'
    | 'beginScreenOff'
    | 'endScreenOff'
    | 'unlockScreen'
    | 'lockScreen'
    | 'beginExitAnimation'
    | 'beginSleep'
    | 'endSleep'
    | 'changeUser'
    | 'screenlockEnabled'
    | 'serviceRestart'
    | 'strongAuthChanged'
    | 'screenLockDisabledChanged';

  /**
   * Indicates the strong authentication reason flags used to request.
   *
   * @enum { number }
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  enum StrongAuthReasonFlags {
    /**
     * Indicates that there are no strong authentication reason flags.
     *
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    NONE = 0x00000000,

    /**
     * Indicates the strong authentication reason requested after boot.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    AFTER_BOOT = 0x00000001,

    /**
     * Indicates the strong authentication reason requested after timeout.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    AFTER_TIMEOUT = 0x00000002,
 
    /**
     * Indicates the strong authentication reason requested by active request.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    ACTIVE_REQUEST = 0x00000004,

    /**
     * Indicates the strong authentication reason requested by policy restriction.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    POLICY_RESTRICTION = 0x00000008
  }

  /**
   * Indicates the screen lock authentication state.
   *
   * @enum { number }
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  enum AuthState {
    /**
     * Indicates the screen lock is not authenticated.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    NOT_AUTHED = 0,

    /**
     * Indicates the screen lock is pre authenticated by credential.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PRE_AUTHED_BY_CREDENTIAL = 1,

    /**
     * Indicates the screen lock is pre authenticated by fingerprint.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PRE_AUTHED_BY_FINGERPRINT = 2,

    /**
     * Indicates the screen lock is pre authenticated by face.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PRE_AUTHED_BY_FACE = 3,

    /**
     * Indicates the screen lock is authenticated by credential.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    AUTHED_BY_CREDENTIAL = 4,

    /**
     * Indicates the screen lock is authenticated by fingerprint.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    AUTHED_BY_FINGERPRINT = 5,

    /**
     * Indicates the screen lock is authenticated by face.
     * 
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    AUTHED_BY_FACE = 6,
  }

  /**
   * Indicates the system event type and parameter related to the screenlock management service.
   *
   * @typedef SystemEvent
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface SystemEvent {
    /**
     * Indicates the system event type related to the screenlock management service.
     *
     * @type { EventType }
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    eventType: EventType;
    /**
     * Identifies the customized extended parameter of an event.
     *
     * @type { string }
     * @syscap SystemCapability.MiscServices.ScreenLock
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    params: string;
  }

  /**
   * Register system event related to screen lock service.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @param { Callback<SystemEvent> } callback - the callback of onSystemEvent.
   * @returns { boolean } returns true if register system event is success, returns false otherwise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function onSystemEvent(callback: Callback<SystemEvent>): boolean;

  /**
   * The screen lock app sends the event to the screen lock service.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @param { String } event - event type.
   * @param { number } parameter - operation result of the event.
   * @param { AsyncCallback<boolean> } callback - the callback of sendScreenLockEvent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function sendScreenLockEvent(event: String, parameter: number, callback: AsyncCallback<boolean>): void;

  /**
   * The screen lock app sends the event to the screen lock service.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @param { String } event - event type.
   * @param { number } parameter - operation result of the event.
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function sendScreenLockEvent(event: String, parameter: number): Promise<boolean>;

  /**
   * Request strong authentication for os account local userId.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK
   * @param { StrongAuthReasonFlags } reasonFlag - The strong authentication reason flag.
   * @param { number } userId - Os account local userId.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function requestStrongAuth(reasonFlag: StrongAuthReasonFlags, userId: number): Promise<void>;

  /**
   * Obtain strong authentication reason flags for os account local userId.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK
   * @param { number } userId - Os account local userId.
   * @returns { number } the strong authentication reason flags.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function getStrongAuth(userId: number): number;

  /**
   * Disable screen lock showing for os account local userId. This only becomes effective when there is no password.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK
   * @param { boolean } disable - disable or enable screen lock showing.
   * @param { number } userId - Os account local userId.
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function setScreenLockDisabled(disable: boolean, userId: number): Promise<boolean>;

  /**
   * Check whether screen lock is disabled for os account local userId.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK
   * @param { number } userId - Os account local userId.
   * @returns { boolean } whether screen lock is disabled.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function isScreenLockDisabled(userId: number): boolean;

  /**
   * Set the screen lock authentication state for os account local userId.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_INNER
   * @param { AuthState } state - the screen lock authentication state.
   * @param { number } userId - Os account local userId.
   * @param { Uint8Array } authToken - the authentication token for this state
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function setScreenLockAuthState(state: AuthState, userId: number, authToken: Uint8Array): Promise<boolean>;

  /**
   * Obtain the screen lock authentication state for os account local userId.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK
   * @param { number } userId - Os account local userId.
   * @returns { AuthState } the screen lock authentication state.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 201 - permission denied.
   * @throws { BusinessError } 202 - permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - the screenlock management service is abnormal.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function getScreenLockAuthState(userId: number): AuthState;

  /**
   * Check whether the device is currently locked and the screenlock requires an identity to authenticate and unlock.
   *
   * @param { number } userId - Os account local userId.
   * @returns { boolean } Whether the device is currently locked.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 13200002 - The screenlock management service is abnormal.
   * @throws { BusinessError } 13200004 - The userId is not same as the caller, and is not allowed for the caller.
   * @syscap SystemCapability.MiscServices.ScreenLock
   * @systemapi Hide this for inner system use.
   * @since 15
   */
  function isDeviceLocked(userId: number): boolean;
}

export default screenLock;