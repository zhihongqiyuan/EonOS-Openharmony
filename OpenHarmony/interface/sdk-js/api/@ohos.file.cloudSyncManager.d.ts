/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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
 * Provides the capabilities to manage the state and data of cloud file synchronization.
 *
 * @namespace cloudSyncManager
 * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
 * @since 10
 */
declare namespace cloudSyncManager {
  /**
   * Modify switch state of the application's cloud synchronization capability.
   *
   * @param { string } accountId - Current account id
   * @param { string } bundleName - Name of bundle whose switchStatus is changing
   * @param { boolean } status - New switch status
   * @returns { Promise<void> } - Return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean): Promise<void>;

  /**
   * Modify switch state of the application's cloud synchronization capability.
   *
   * @param { string } accountId - Current account id
   * @param { string } bundleName - Name of bundle whose switchStatus is changing
   * @param { boolean } status - New switch status
   * @param { AsyncCallback<void> } [callback] - Callback function
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function changeAppCloudSwitch(
    accountId: string,
    bundleName: string,
    status: boolean,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Notify the change of data in cloud.
   *
   * @param { string } accountId - Current account id
   * @param { string } bundleName - Name of bundle whose switchStatus is changing
   * @returns { Promise<void> } Return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function notifyDataChange(accountId: string, bundleName: string): Promise<void>;

  /**
   * Notify the change of data in cloud.
   *
   * @param { string } accountId - Current account id
   * @param { string } bundleName - Name of bundle whose switchStatus is changing
   * @param { AsyncCallback<void> } [callback] - Callback function
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function notifyDataChange(accountId: string, bundleName: string, callback: AsyncCallback<void>): void;

  /**
   * Enable the cloud file synchronization function.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id.
   * @param { Record<string, boolean> } switches - Indicates switches information of all applications.
   * @returns { Promise<void> } Return Promise
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function enableCloud(accountId: string, switches: Record<string, boolean>): Promise<void>;

  /**
   * Enable the cloud file synchronization function.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id
   * @param { Record<string, boolean> } switches - Indicates switches information of all applications.
   * @param { AsyncCallback<void> } [callback] - Callback function
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function enableCloud(accountId: string, switches: Record<string, boolean>, callback: AsyncCallback<void>): void;

  /**
   * Disable the cloud file synchronization function.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id.
   * @returns { Promise<void> } Return Promise.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function disableCloud(accountId: string): Promise<void>;

  /**
   * Disable the cloud file synchronization function.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id.
   * @param { AsyncCallback<void> } callback - Callback function
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function disableCloud(accountId: string, callback: AsyncCallback<void>): void;

  /**
   * Describes the clear action type.
   *
   * @enum { number }
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  enum Action {
    /**
     * Indicates clearing cloud identification only, retaining local cached data.
     *
     * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
     * @systemapi
     * @since 10
     */
    RETAIN_DATA,

    /**
     * Indicates clearing all cloud-related file data, which synchronized with the cloud.
     *
     * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
     * @systemapi
     * @since 10
     */
    CLEAR_DATA
  }

  /**
   * Clean up cloud-related file data based on specific action.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id.
   * @param { Record<string, Action> } appActions - Indicates information about cloud file need to clear in which way.
   * @returns { Promise<void> } Return Promise.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function clean(accountId: string, appActions: Record<string, Action>): Promise<void>;

  /**
   * Clean up cloud-related file data based on specific action.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { string } accountId - Current account id.
   * @param { Record<string, Action> } appActions - Indicates information about cloud file need to clear in which way.
   * @param { AsyncCallback<void> } callback - Callback function
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 10
   */
  function clean(accountId: string, appActions: Record<string, Action>, callback: AsyncCallback<void>): void;

  /**
   * Notify the change of data in cloud.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { number } userId - The Id of the user whose cloud data changed
   * @param { ExtraData } extraData - The change info from push notification
   * @returns { Promise<void> } Return Promise
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 11
   */
  function notifyDataChange(userId: number, extraData: ExtraData): Promise<void>;

  /**
   * Notify the change of data in cloud.
   *
   * @permission ohos.permission.CLOUDFILE_SYNC_MANAGER
   * @param { number } userId - The Id of the user whose cloud data changed
   * @param { ExtraData } extraData - The change info from push notification
   * @param { AsyncCallback<void> } callback - Callback function
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 13600001 - IPC error.
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 11
   */
  function notifyDataChange(userId: number, extraData: ExtraData, callback: AsyncCallback<void>): void;

  /**
   * The change info from push notification.
   *
   * @interface ExtraData
   * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
   * @systemapi
   * @since 11
   */
  interface ExtraData {
    /**
     * The eventId of the push info.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
     * @systemapi
     * @since 11
     */
    eventId: string;
    /**
     * The change info.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.DistributedFileService.CloudSyncManager
     * @systemapi
     * @since 11
     */
    extraData: string;
  }
}

export default cloudSyncManager;
