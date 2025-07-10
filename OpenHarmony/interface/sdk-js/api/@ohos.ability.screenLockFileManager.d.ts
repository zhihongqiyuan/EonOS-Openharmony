/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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
 * @file Provides the capability to access data under lock screen.
 * @kit AbilityKit
 */

/**
 * Provides the capability to access data under lock screen.
 *
 * @namespace screenLockFileManager
 * @syscap SystemCapability.Security.ScreenLockFileManager
 * @since 12
 */
declare namespace screenLockFileManager {
  /**
   * Enumerates the types of data under lock screen.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 12
   */
  export enum DataType {
    /**
     * Media photo and video data.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    MEDIA_DATA = 0x00000001,

    /**
     * All data type.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    ALL_DATA = 0xffffffff
  }

  /**
   * AccessStatus.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 12
   */
  export enum AccessStatus {
    /**
     * Acquire the access of data failed.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    ACCESS_DENIED = -1,

    /**
     * Acquire the access of data success.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    ACCESS_GRANTED = 0
  }

  /**
   * ReleaseStatus.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 12
   */
  export enum ReleaseStatus {
    /**
     * Release the access of data failed.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    RELEASE_DENIED = -1,

    /**
     * Release the access of data success.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 12
     */
    RELEASE_GRANTED = 0
  }

  /**
   * KeyStatus.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 18
   */
  export enum KeyStatus {
    /**
     * The application is not enabled the data protection under lock screen.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 18
     */
    KEY_NOT_EXIST = -2,

    /**
     * AppKey has been released.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 18
     */
    KEY_RELEASED = -1,

    /**
     * AppKey exists.
     *
     * @syscap SystemCapability.Security.ScreenLockFileManager
     * @since 18
     */
    KEY_EXIST = 0
  }

  /**
   * Acquire the access of the caller data under the lock screen.
   *
   * @returns { AccessStatus } Returns acquire the access result.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @throws { BusinessError } 29300003 - The application is not enabled the data protection under lock screen.
   * @throws { BusinessError } 29300004 - File access is denied.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 12
   */
  function acquireAccess(): AccessStatus;

  /**
   * Acquire the access of the specified data under the lock screen.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_MEDIA_DATA or ohos.permission.ACCESS_SCREEN_LOCK_ALL_DATA
   * @param { DataType } dataType - Indicates the specified data to allow access under the lock screen.
   * @returns { AccessStatus } Returns acquire the access result.
   * @throws { BusinessError } 201 - Permission verification failed, usually returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameter is left unspecified. 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300001 - Invalid DataType.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @throws { BusinessError } 29300003 - The application is not enabled the data protection under lock screen.
   * @throws { BusinessError } 29300004 - File access is denied.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @systemapi
   * @since 12
   */
  function acquireAccess(dataType: DataType): AccessStatus;

  /**
   * Release the access of the caller data and do not allow data access under the lock screen.
   *
   * @returns { ReleaseStatus } Returns release the access result.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @throws { BusinessError } 29300003 - The application is not enabled the data protection under lock screen.
   * @throws { BusinessError } 29300005 - File access was not acquired.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 12
   */
  function releaseAccess(): ReleaseStatus;

  /**
   * Release the access of the specified data type and do not allow data access under the lock screen.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_MEDIA_DATA or ohos.permission.ACCESS_SCREEN_LOCK_ALL_DATA
   * @param { DataType } dataType - Indicates the specified data type and do not allow data access under the lock screen.
   * @returns { ReleaseStatus } Returns release the access result.
   * @throws { BusinessError } 201 - Permission verification failed, usually returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameter is left unspecified. 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300001 - Invalid DataType.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @throws { BusinessError } 29300003 - The application is not enabled the data protection under lock screen.
   * @throws { BusinessError } 29300005 - File access was not acquired.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @systemapi
   * @since 12
   */
  function releaseAccess(dataType: DataType): ReleaseStatus;

  /**
   * Query the AppKey's status of the caller.
   *
   * @returns { KeyStatus } Returns the AppKey status.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @since 18
   */
  function queryAppKeyState(): KeyStatus;

  /**
   * Query the specified AppKey's status.
   *
   * @permission ohos.permission.ACCESS_SCREEN_LOCK_MEDIA_DATA or ohos.permission.ACCESS_SCREEN_LOCK_ALL_DATA
   * @param { DataType } dataType - Indicates the specified data to allow access.
   * @returns { KeyStatus } Returns the AppKey status.
   * @throws { BusinessError } 201 - Permission verification failed, usually returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameter is left unspecified. 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - The specified SystemCapability name was not found.
   * @throws { BusinessError } 29300001 - Invalid DataType.
   * @throws { BusinessError } 29300002 - The system ability work abnormally.
   * @syscap SystemCapability.Security.ScreenLockFileManager
   * @systemapi
   * @since 18
   */
  function queryAppKeyState(dataType: DataType): KeyStatus;
}
export default screenLockFileManager;