/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

/**
 * System time and timezone.
 *
 * @namespace systemDateTime
 * @syscap SystemCapability.MiscServices.Time
 * @since 9
 */
/**
 * System time and timezone.
 *
 * @namespace systemDateTime
 * @syscap SystemCapability.MiscServices.Time
 * @crossplatform
 * @since 18
 */
declare namespace systemDateTime {
  /**
   * Sets the system time.
   *
   * @permission ohos.permission.SET_TIME
   * @param { number } time - Indicates the target timestamp(in milliseconds)
   * @param { AsyncCallback<void> } callback - The callback of setTime
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   */
  function setTime(time: number, callback: AsyncCallback<void>): void;
  /**
   * Sets the system time.
   *
   * @permission ohos.permission.SET_TIME
   * @param { number } time - Indicates the target timestamp(in milliseconds)
   * @returns { Promise<void> } The promise returned by the function
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   */
  function setTime(time: number): Promise<void>;

  /**
   * Obtains the number of milliseconds that have elapsed since the Unix epoch.
   *
   * @param { boolean } isNano - True if the result is in nanoseconds, otherwise in milliseconds
   * @param { AsyncCallback<number> } callback - The callback of getCurrentTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getCurrentTime(isNano: boolean, callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds that have elapsed since the Unix epoch.
   *
   * @param { AsyncCallback<number> } callback - The callback of getCurrentTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getCurrentTime(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds that have elapsed since the Unix epoch.
   *
   * @param { boolean } isNano - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { Promise<number> } The promise returned by the function
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getCurrentTime(isNano?: boolean): Promise<number>;

  /**
   * Obtains the number of timestamp that have elapsed since the Unix epoch.
   *
   * @param { boolean } [ isNanoseconds ] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { number } The timestamp returned of getTime.
   * @syscap SystemCapability.MiscServices.Time
   * @since 10
   */
  /**
   * Obtains the number of timestamp that have elapsed since the Unix epoch.
   *
   * @param { boolean } [ isNanoseconds ] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { number } The timestamp returned of getTime.
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  function getTime(isNanoseconds?: boolean): number;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, not including deep sleep time.
   *
   * @param { boolean } isNano - True if the result is in nanoseconds., otherwise in milliseconds
   * @param { AsyncCallback<number> } callback - The callback of getRealActiveTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealActiveTime(isNano: boolean, callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, not including deep sleep time.
   *
   * @param { AsyncCallback<number> } callback - The callback of getRealActiveTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealActiveTime(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, not including deep sleep time.
   *
   * @param { boolean } [isNano] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { Promise<number> } The promise returned by the function
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealActiveTime(isNano?: boolean): Promise<number>;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, including deep sleep time.
   *
   * @param { boolean } isNano - True if the result is in nanoseconds, otherwise in milliseconds
   * @param { AsyncCallback<number> } callback - The callback of getRealTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealTime(isNano: boolean, callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, including deep sleep time.
   *
   * @param { AsyncCallback<number> } callback - The callback of getRealTime
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealTime(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of milliseconds elapsed since the system was booted, including deep sleep time.
   *
   * @param { boolean } [isNano] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { Promise<number> } The promise returned by the function
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 12
   */
  function getRealTime(isNano?: boolean): Promise<number>;

  /**
   * Indicates time type.
   *
   * @enum { number } TimeType
   * @syscap SystemCapability.MiscServices.Time
   * @since 10
   */
  /**
   * Indicates time type.
   *
   * @enum { number } TimeType
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  enum TimeType {
    /**
     * Indicates the time elapsed since the system was booted, including deep sleep time.
     *
     * @syscap SystemCapability.MiscServices.Time
     * @since 10
     */
    /**
     * Indicates the time elapsed since the system was booted, including deep sleep time.
     *
     * @syscap SystemCapability.MiscServices.Time
     * @crossplatform
     * @since 18
     */
    STARTUP,

    /**
     * Indicates the time elapsed since the system was booted, not including deep sleep time.
     *
     * @syscap SystemCapability.MiscServices.Time
     * @since 10
     */
    /**
     * Indicates the time elapsed since the system was booted, not including deep sleep time.
     *
     * @syscap SystemCapability.MiscServices.Time
     * @crossplatform
     * @since 18
     */
    ACTIVE
  }

  /**
   * Obtains the number of milliseconds since the system has been running.
   *
   * @param { TimeType } timeType - indicates the type of get uptime. It can only be `STARTUP` or `ACTIVE`.
   * @param { boolean } [ isNanoseconds ] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { number } The timestamp returned of getUpTime.
   * @syscap SystemCapability.MiscServices.Time
   * @since 10
   */
  /**
   * Obtains the number of milliseconds since the system has been running.
   *
   * @param { TimeType } timeType - indicates the type of get uptime. It can only be `STARTUP` or `ACTIVE`.
   * @param { boolean } [ isNanoseconds ] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { number } The timestamp returned of getUpTime.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
   *     3. Parameter verification failed. This error code was added due to missing issues.
   * @syscap SystemCapability.MiscServices.Time
   * @since 12
   */
  /**
   * Obtains the number of milliseconds since the system has been running.
   *
   * @param { TimeType } timeType - indicates the type of get uptime. It can only be `STARTUP` or `ACTIVE`.
   * @param { boolean } [ isNanoseconds ] - True if the result is in nanoseconds, otherwise in milliseconds
   * @returns { number } The timestamp returned of getUpTime.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
   *     3. Parameter verification failed. This error code was added due to missing issues.
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  function getUptime(timeType: TimeType, isNanoseconds?: boolean): number;

  /**
   * Sets the system time.
   *
   * @permission ohos.permission.SET_TIME
   * @param { Date } date - The target date, it must > 0
   * @param { AsyncCallback<void> } callback - The callback of setDate
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   *     3.Parameter verification failed;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   * @deprecated since 10
   * @useinstead systemDateTime.setTime
   */
  function setDate(date: Date, callback: AsyncCallback<void>): void;

  /**
   * Sets the system time.
   * 
   * @permission ohos.permission.SET_TIME
   * @param { Date } date - The target date, it must > 0
   * @returns { Promise<void> } The promise returned by the function
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   *     3.Parameter verification failed;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   * @deprecated since 10
   * @useinstead systemDateTime.setTime
   */
  function setDate(date: Date): Promise<void>;

  /**
   * Obtains the system date.
   *
   * @param { AsyncCallback<Date> } callback - The callback of getDate
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.System error;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 10
   * @useinstead new Date()
   */
  function getDate(callback: AsyncCallback<Date>): void;

  /**
   * Obtains the system date.
   *
   * @returns { Promise<Date> } The promise returned by the function
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.System error;
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   * @deprecated since 10
   * @useinstead new Date()
   */
  function getDate(): Promise<Date>;

  /**
   * Sets the system time zone.
   *
   * @permission ohos.permission.SET_TIME_ZONE
   * @param { string } timezone - The system time zone
   * @param { AsyncCallback<void> } callback - The callback of setTimezone
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   */
  function setTimezone(timezone: string, callback: AsyncCallback<void>): void;

  /**
   * Sets the system time zone.
   *
   * @permission ohos.permission.SET_TIME_ZONE
   * @param { string } timezone -  The system time zone
   * @returns { Promise<void> } The promise returned by the function
   * @throws { BusinessError } 201 - Permission denied
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types;
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 9
   */
  function setTimezone(timezone: string): Promise<void>;

  /**
   * Obtains the system time zone.
   *
   * @param { AsyncCallback<string> } callback - The callback of getTimezone
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.System error.
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   */
  /**
   * Obtains the system time zone.
   *
   * @param { AsyncCallback<string> } callback - The callback of getTimezone
   * @syscap SystemCapability.MiscServices.Time
   * @since 12
   */
  /**
   * Obtains the system time zone.
   *
   * @param { AsyncCallback<string> } callback - The callback of getTimezone
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  function getTimezone(callback: AsyncCallback<string>): void;

  /**
   * Obtains the system time zone.
   *
   * @returns { Promise<string> } The promise returned by the function
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.System error.
   * @syscap SystemCapability.MiscServices.Time
   * @since 9
   */
  /**
   * Obtains the system time zone.
   *
   * @returns { Promise<string> } The promise returned by the function
   * @syscap SystemCapability.MiscServices.Time
   * @since 12
   */
  /**
   * Obtains the system time zone.
   *
   * @returns { Promise<string> } The promise returned by the function
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  function getTimezone(): Promise<string>;

  /**
   * Obtains the system time zone.
   *
   * @returns { string } The timezone returned of getTimezoneSync.
   * @syscap SystemCapability.MiscServices.Time
   * @since 10
   */
  /**
   * Obtains the system time zone.
   *
   * @returns { string } The timezone returned of getTimezoneSync.
   * @syscap SystemCapability.MiscServices.Time
   * @crossplatform
   * @since 18
   */
  function getTimezoneSync(): string;

  /**
   * Updates the NTP time from the ntp server. This method will only update NTP time once within an hour.
   *
   * @returns { Promise<void> } The promise returned by the function
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 13000001 - Network connection error or OS error.
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 14
   */
  function updateNtpTime(): Promise<void>;

  /**
   * Obtains the current time of milliseconds since the Unix epoch calculated based on the last updated NTP time.
   *
   * @returns { number } The current NTP time returned of getNtpTime.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 13000002 - updateNtpTime() is not called successfully.
   * @syscap SystemCapability.MiscServices.Time
   * @systemapi Hide this for inner system use
   * @since 14
   */
  function getNtpTime(): number;
}

export default systemDateTime;