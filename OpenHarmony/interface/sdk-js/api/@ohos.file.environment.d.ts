/*
 * Copyright (C) 2021-2023 Huawei Device Co., Ltd.
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
 * @kit CoreFileKit
 */

import type { AsyncCallback } from './@ohos.base';

/**
 * Provides Environment APIs.
 *
 * @namespace Environment
 * @syscap SystemCapability.FileManagement.File.Environment
 * @since 11
 */
declare namespace Environment {
  /**
   * Get the user data path.
   *
   * @returns { Promise<string> } return Promise
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.Environment
   * @systemapi
   * @since 8
   */
  function getStorageDataDir(): Promise<string>;

  /**
   * Get the user data path.
   *
   * @param { AsyncCallback<string> } [callback] - callback
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.Environment
   * @systemapi
   * @since 8
   */
  function getStorageDataDir(callback: AsyncCallback<string>): void;

  /**
   * Get the User storage path.
   *
   * @returns { Promise<string> } return Promise
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.Environment
   * @systemapi
   * @since 8
   */
  function getUserDataDir(): Promise<string>;

  /**
   * Get the User storage path.
   *
   * @param { AsyncCallback<string> } [callback] - callback
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.Environment
   * @systemapi
   * @since 8
   */
  function getUserDataDir(callback: AsyncCallback<string>): void;

  /**
   * Get the public download directory.
   *
   * @permission ohos.permission.READ_WRITE_DOWNLOAD_DIRECTORY
   * @returns { string } Return the public download directory.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 11
   */
  /**
   * Get the public download directory.
   *
   * @returns { string } Return the public download directory.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 12
   */
  function getUserDownloadDir(): string;

  /**
   * Get the public desktop directory.
   *
   * @permission ohos.permission.READ_WRITE_DESKTOP_DIRECTORY
   * @returns { string } Return the public desktop directory.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 11
   */
  /**
   * Get the public desktop directory.
   *
   * @returns { string } Return the public desktop directory.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 12
   */
  function getUserDesktopDir(): string;

  /**
   * Get the public document directory.
   *
   * @permission ohos.permission.READ_WRITE_DOCUMENTS_DIRECTORY
   * @returns { string } Return the public document directory.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 11
   */
  /**
   * Get the public document directory.
   *
   * @returns { string } Return the public document directory.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @since 12
   */
  function getUserDocumentDir(): string;

  /**
   * Get external storage directory.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER
   * @returns { string } Return external storage directory.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function getExternalStorageDir(): string;

  /**
   * Get home directory of the application sandbox path for the current user.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER
   * @returns { string } Return home directory of the application sandbox path for the current user.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.File.Environment.FolderObtain
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function getUserHomeDir(): string;
}

export default Environment;
