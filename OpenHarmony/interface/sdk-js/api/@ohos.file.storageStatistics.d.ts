/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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

import { AsyncCallback, Callback } from './@ohos.base';

/**
 * Provides filesystem statistics APIs.
 *
 * @namespace storageStatistics
 * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
 * @since 8
 */
declare namespace storageStatistics {
  /**
   * Get the total size of volume.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } volumeUuid - The uuid of the volume
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 8
   */
  function getTotalSizeOfVolume(volumeUuid: string, callback: AsyncCallback<number>): void;

  /**
   * Get the total size of volume.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } volumeUuid - The uuid of the volume
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 8
   */
  function getTotalSizeOfVolume(volumeUuid: string): Promise<number>;
  /**
   * Get the free size of volume.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } volumeUuid - The uuid of the volume
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 8
   */
  function getFreeSizeOfVolume(volumeUuid: string, callback: AsyncCallback<number>): void;

  /**
   * Get the free size of volume.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } volumeUuid - The uuid of the volume
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 8
   */
  function getFreeSizeOfVolume(volumeUuid: string): Promise<number>;

  /**
   * Get the bundle statistics.
   *
   * @interface BundleStats
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 9
   */
  export interface BundleStats {
    /**
     * The size of application installation data.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @since 9
     */
    appSize: number;

    /**
     * The size of application cache data.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @since 9
     */
    cacheSize: number;

    /**
     * The size of application local data, distributed data and database data.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @since 9
     */
    dataSize: number;
  }
  /**
   * Get the bundle statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } packageName - The name of the application
   * @param { AsyncCallback<BundleStats> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */ 
  /**
   * Get the bundle statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } packageName - The name of the application
   * @param { AsyncCallback<BundleStats> } callback - callback
   * @param { number } index - The index number of the clone application, the default value is 0.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 12
   */
  function getBundleStats(packageName: string, callback: AsyncCallback<BundleStats>, index?: number): void;

  /**
   * Get the bundle statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } packageName - The name of the application
   * @returns { Promise<BundleStats> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  /**
   * Get the bundle statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { string } packageName - The name of the application
   * @param { number } index - The index number of the clone application, the default value is 0.
   * @returns { Promise<BundleStats> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600008 - No such object.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 12
   */
  function getBundleStats(packageName: string, index?: number): Promise<BundleStats>;

  /**
   * Get the current bundle statistics.
   *
   * @param { AsyncCallback<BundleStats> } callback - callback
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 9
   */
  function getCurrentBundleStats(callback: AsyncCallback<BundleStats>): void;

  /**
   * Get the current bundle statistics.
   *
   * @returns { Promise<BundleStats> } return Promise
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 9
   */
  function getCurrentBundleStats(): Promise<BundleStats>;

  /**
   * Get the system size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getSystemSize(callback: AsyncCallback<number>): void;

  /**
   * Get the system size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getSystemSize(): Promise<number>;

  /**
   * Get the user storage statistics.
   *
   * @interface StorageStats
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  export interface StorageStats {
    /**
     * The total size of device.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    total: number;

    /**
     * The size of audio file.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    audio: number;

    /**
     * The size of video file.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    video: number;

    /**
     * The size of image file.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    image: number;

    /**
     * The size of other file.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    file: number;

    /**
     * The size of application.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
     * @systemapi
     * @since 9
     */
    app: number;
  }

  /**
   * Get the user storage statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { Promise<StorageStats> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getUserStorageStats(): Promise<StorageStats>;

  /**
   * Get the user storage statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { AsyncCallback<StorageStats> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getUserStorageStats(callback: AsyncCallback<StorageStats>): void;

  /**
   * Get the user storage statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { number } userId - The id of the user
   * @returns { Promise<StorageStats> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600009 - User if out of range.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getUserStorageStats(userId: number): Promise<StorageStats>;

  /**
   * Get the user storage statistics.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { number } userId - The id of the user
   * @param { AsyncCallback<StorageStats> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory
parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13600009 - User if out of range.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  function getUserStorageStats(userId: number, callback: AsyncCallback<StorageStats>): void;

  /**
   * Get the total size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  /**
   * Get the total size.
   *
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getTotalSize(callback: AsyncCallback<number>): void;

  /**
   * Get the total size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  /**
   * Get the total size.
   *
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getTotalSize(): Promise<number>;

  /**
   * Get the total size with sync interface
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { number } return the total size
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 10
   */
  /**
   * Get the total size with sync interface
   *
   * @returns { number } return the total size
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getTotalSizeSync(): number;

  /**
   * Get the free size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  /**
   * Get the free size.
   *
   * @param { AsyncCallback<number> } callback - callback
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getFreeSize(callback: AsyncCallback<number>): void;

  /**
   * Get the free size.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 9
   */
  /**
   * Get the free size.
   *
   * @returns { Promise<number> } return Promise
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getFreeSize(): Promise<number>;

  /**
   * Get the free size with sync interface.
   *
   * @permission ohos.permission.STORAGE_MANAGER
   * @returns { number } return the free size
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:Mandatory
parameters are left unspecified;
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @systemapi
   * @since 10
   */
  /**
   * Get the free size with sync interface.
   *
   * @returns { number } return the free size
   * @throws { BusinessError } 13600001 - IPC error.
   * @throws { BusinessError } 13900042 - Unknown error.
   * @syscap SystemCapability.FileManagement.StorageService.SpatialStatistics
   * @since 15
   */
  function getFreeSizeSync(): number;
}

export default storageStatistics;
