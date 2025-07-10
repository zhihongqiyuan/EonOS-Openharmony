/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
 * @kit ArkData
 */

import { AsyncCallback } from './@ohos.base';
import type relationalStore from './@ohos.data.relationalStore';
import commonType from './@ohos.data.commonType';

/**
 * Provides methods for cloud capabilities.
 *
 * @namespace cloudData
 * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
 * @since 10
 */
declare namespace cloudData {
  /**
   * Describes the clear action type.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 10
   */
  enum ClearAction {
    /**
     * Indicates clearing cloud-related data only, which includes cloud meta data and cloud-related local data.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    CLEAR_CLOUD_INFO,

    /**
     * Indicates clearing all cloud-related file data,which synchronized with the cloud.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    CLEAR_CLOUD_DATA_AND_INFO
  }

  /**
   * ID of the event, which indicates the change of the data in the cloud.
   *
   * @constant
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 11
   */
  const DATA_CHANGE_EVENT_ID = 'cloud_data_change';

  /**
   * Extra data for data change notification.
   *
   * @interface ExtraData
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 11
   */
  interface ExtraData {
    /**
     * Event ID.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    eventId: string;

    /**
     * Extra data, which contains the following fields.
     * '{
     *   "data": "{
     *     "accountId": "aaa",
     *     "bundleName": "com.bbb.xxx",
     *     "containerName": "alias",
     *     "databaseScopes": ["private", "shared"],
     *     "recordTypes": ["xxx", "yyy", "zzz"]
     *   }"
     * }'
     * All fields are mandatory.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    extraData: string;
  }

  /**
   * Additional data for querying data statistics information.
   *
   * @interface StatisticInfo
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 12
   */
  interface StatisticInfo {
    /**
     * Cloud table name.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    table: string;

    /**
     * Number of records to be inserted to the cloud.
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    inserted: number;

    /**
     * Number of inconsistent records between the local device and the cloud.
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    updated: number;

    /**
     * Number of consistent records between the local device and the cloud.
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    normal: number;
  }

  /**
   * Describes sync status.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 18
   */
  enum SyncStatus {
    /**
     * Indicates cloud sync status is running.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 18
     */
    RUNNING = 0,

    /**
     * Indicates cloud sync status was finished.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 18
     */
    FINISHED = 1
  }

  /**
   * Sync information.
   *
   * @interface SyncInfo
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 12
   */
  interface SyncInfo {
    /**
     * Sync start time.
     *
     * @type { Date }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    startTime: Date;

    /**
     * Sync finish time.
     *
     * @type { Date }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    finishTime: Date;

    /**
     * Sync progress.
     *
     * @type { relationalStore.ProgressCode }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    code: relationalStore.ProgressCode;

    /**
     * Sync status.
     *
     * @type { ?SyncStatus }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 18
     */
    syncStatus?: SyncStatus;
  }

  /**
   * Provides methods to set CloudSync config.
   *
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
   * @systemapi
   * @since 10
   */
  class Config {
    /**
     * Enables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { object } switches - Indicates switches information of all applications.
     * switches will overwrite the saved application switch information.If the specific application switch changes,
     * the {@link changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean)} method will notify the data manager service.
     * @param { AsyncCallback<void> } callback - the callback of enableCloud.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    /**
     * Enables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { Record<string, boolean> } switches - Indicates switches information of all applications.
     * switches will overwrite the saved application switch information.If the specific application switch changes,
     * the {@link changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean)} method will notify the data manager service.
     * @param { AsyncCallback<void> } callback - the callback of enableCloud.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static enableCloud(
      accountId: string,
      switches: Record<string, boolean>,
      callback: AsyncCallback<void>
    ): void;

    /**
     * Enables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { object } switches - Indicates switches information of all applications.
     * switches will overwrite the saved application switch information.If the specific application switch changes,
     * the {@link changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean)} method will notify the data manager service.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    /**
     * Enables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { Record<string, boolean> } switches - Indicates switches information of all applications.
     * switches will overwrite the saved application switch information.If the specific application switch changes,
     * the {@link changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean)} method will notify the data manager service.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static enableCloud(accountId: string, switches: Record<string, boolean>): Promise<void>;

    /**
     * Disables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { AsyncCallback<void> } callback - the callback of disableCloud.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    static disableCloud(accountId: string, callback: AsyncCallback<void>): void;

    /**
     * Disables the cloud function.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    static disableCloud(accountId: string): Promise<void>;

    /**
     * Changes the cloud switch of a single application.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName -  Indicates the name of application.
     * @param { boolean } status - Indicates the condition of cloud sync switch.true means the switch is on,false means switch is off.
     * @param { AsyncCallback<void> } callback - the callback of changeAppCloudSwitch.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    static changeAppCloudSwitch(
      accountId: string,
      bundleName: string,
      status: boolean,
      callback: AsyncCallback<void>
    ): void;

    /**
     * Changes the cloud switch of a single application.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName -  Indicates the name of application.
     * @param { boolean } status - Indicates the condition of cloud sync switch.true means the switch is on,false means switch is off.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    static changeAppCloudSwitch(accountId: string, bundleName: string, status: boolean): Promise<void>;

    /**
     * Notifies changes of the cloud records.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { ExtraData } extInfo - Indicates the extra data for
     * notification {@link ExtraData}.
     * @param { number } [userId] - Indicates the user ID.
     * @returns { Promise<void> } Promise used to return the result.
     * @throws { BusinessError } 201 - Permission verification failed, which
     * is usually returned by <b>VerifyAccessToken</b>.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static notifyDataChange(extInfo: ExtraData, userId?: number): Promise<void>;

    /**
     * Notifies changes of the cloud records.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { ExtraData } extInfo - Indicates the extra data for
     * notification {@link ExtraData}.
     * @param { AsyncCallback<void> } callback - Indicates the callback invoked
     * to return the data changes.
     * @throws { BusinessError } 201 - Permission verification failed, which
     * is usually returned by <b>VerifyAccessToken</b>.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static notifyDataChange(extInfo: ExtraData, callback: AsyncCallback<void>): void;

    /**
     * Notifies changes of the cloud records.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { ExtraData } extInfo - Indicates the extra data for
     * notification {@link ExtraData}.
     * @param { number } userId - Indicates the user ID.
     * @param { AsyncCallback<void> } callback - Indicates the callback invoked
     * to return the data changes.
     * @throws { BusinessError } 201 - Permission verification failed, which
     * is usually returned by <b>VerifyAccessToken</b>.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static notifyDataChange(extInfo: ExtraData, userId: number, callback: AsyncCallback<void>): void;

    /**
     * notifies changes of the cloud records
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName - Indicates the name of application.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Server
     * @systemapi
     * @since 10
     */
    static notifyDataChange(accountId: string, bundleName: string): Promise<void>;

    /**
     * notifies changes of the cloud records
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName - Indicates the name of application.
     * @param { AsyncCallback<void> } callback - the callback of notifyDataChange.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Server
     * @systemapi
     * @since 10
     */
    static notifyDataChange(accountId: string, bundleName: string, callback: AsyncCallback<void>): void;

    /**
     * Queries statistics of the cloud records.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName - Indicates the name of application.
     * @param { string } [storeId] - Indicates the store ID.
     * @returns { Promise<Record<string, Array<StatisticInfo>>> } Promise used to return the result.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    static queryStatistics(
        accountId: string,
        bundleName: string,
        storeId?: string
    ): Promise<Record<string, Array<StatisticInfo>>>;

    /**
     * Queries last sync information.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { string } bundleName - Indicates the name of application.
     * @param { string } [storeId] - Indicates the store ID.
     * @returns { Promise<Record<string, SyncInfo>> } Promise used to return the result.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    static queryLastSyncInfo(
        accountId: string,
        bundleName: string,
        storeId?: string
    ): Promise<Record<string, SyncInfo>>;

    /**
     * deletes cloud information from local data.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { object } appActions - Indicates the way in which the application data is to be cleared.
     * @param { AsyncCallback<void> } callback - the callback of clear.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    /**
     * deletes cloud information from local data.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing cloud account.
     * @param { Record<string, ClearAction> } appActions - Indicates the way in which the application data is to be cleared.
     * @param { AsyncCallback<void> } callback - the callback of clear.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static clear(
      accountId: string,
      appActions: Record<string, ClearAction>,
      callback: AsyncCallback<void>
    ): void;

    /**
     * deletes cloud information from local data.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing the information of specific opened cloud.
     * @param { object } appActions - Indicates the way in which the application data is to be cleared.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 10
     */
    /**
     * deletes cloud information from local data.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { string } accountId - Indicates the account ID. The account ID is required by hashing the information of specific opened cloud.
     * @param { Record<string, ClearAction> } appActions - Indicates the way in which the application data is to be cleared.
     * @returns { Promise<void> } the promise returned by the function.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 11
     */
    static clear(accountId: string, appActions: Record<string, ClearAction>): Promise<void>;

    /**
     * Sets global cloud strategy.
     *
     * @permission ohos.permission.CLOUDDATA_CONFIG
     * @param { StrategyType } strategy - Indicates the strategy type of the cloud sync.
     * @param { Array<commonType.ValueType> } param - Indicates specific strategy of the cloud sync.
     * @returns { Promise<void> } Promise used to return the result.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Config
     * @systemapi
     * @since 12
     */
    static setGlobalCloudStrategy(strategy: StrategyType, param?: Array<commonType.ValueType>): Promise<void>;
  }

  /**
   * Enumerates the strategy types of cloud sync.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
   * @since 12
   */
  enum StrategyType {

    /**
     * Sync via the network.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @since 12
     */
    NETWORK
  }

  /**
   * Enumerates the types of cloud sync via the network.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
   * @since 12
   */
  enum NetWorkStrategy {

    /**
     * Sync using WiFi.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @since 12
     */
    WIFI = 1,

    /**
     * Sync using the cellular network.
     *
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @since 12
     */
    CELLULAR = 2,
  }

  /**
   * Sets cloud strategy.
   *
   * @param { StrategyType } strategy - Indicates the strategy type of the cloud sync.
   * @param { Array<commonType.ValueType> } param - Indicates specific strategy of the cloud sync.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
   * @since 12
   */
  function setCloudStrategy(strategy: StrategyType, param?: Array<commonType.ValueType>): Promise<void>;

  /**
   * Provides methods to implement cloud sharing.
   *
   * @namespace sharing
   * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
   * @systemapi
   * @since 11
   */
  export namespace sharing {
    /**
     * Enumerates the roles.
     *
     * @enum { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    enum Role {
      /**
       * ROLE_INVITER: means inviter of cloud sharing.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      ROLE_INVITER = 0,

      /**
       * ROLE_INVITEE: means invitee of cloud sharing.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      ROLE_INVITEE = 1,
    }

    /**
     * Enumerates the states of sharing invitation.
     *
     * @enum { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    enum State {
      /**
       * STATE_UNKNOWN: Unknown state.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      STATE_UNKNOWN = 0,

      /**
       * STATE_ACCEPTED: Accept the sharing invitation.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      STATE_ACCEPTED = 1,

      /**
       * STATE_REJECTED: Reject the sharing invitation.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      STATE_REJECTED = 2,

      /**
       * STATE_SUSPENDED: Suspend the sharing process.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      STATE_SUSPENDED = 3,

      /**
       * STATE_UNAVAILABLE: The sharing process unavailable.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 12
       */
      STATE_UNAVAILABLE = 4,
    }

    /**
     * Enumerates the error code of sharing invitation.
     *
     * @enum { number }
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    enum SharingCode {
      /**
       * SUCCESS: means sharing success.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      SUCCESS = 0,

      /**
       * REPEATED_REQUEST: means the user has been invited.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      REPEATED_REQUEST = 1,

      /**
       * NOT_INVITER: means the participant is not inviter.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      NOT_INVITER = 2,

      /**
       * NOT_INVITER_OR_INVITEE: means the participant is not inviter or invitee.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      NOT_INVITER_OR_INVITEE = 3,

      /**
       * OVER_QUOTA: means the number of sharing times today of current user has reached maximum.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      OVER_QUOTA = 4,

      /**
       * TOO_MANY_PARTICIPANTS: means the number of participants reaches the maximum.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      TOO_MANY_PARTICIPANTS = 5,

      /**
       * INVALID_ARGS: means invalid arguments.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      INVALID_ARGS = 6,

      /**
       * NETWORK_ERROR: means the network is unavailable.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      NETWORK_ERROR = 7,

      /**
       * CLOUD_DISABLED: means cloud is disabled.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      CLOUD_DISABLED = 8,

      /**
       * SERVER_ERROR: means invoke cloud space failed.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      SERVER_ERROR = 9,

      /**
       * INNER_ERROR: means an unknown error has occurred.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      INNER_ERROR = 10,

      /**
       * INVALID_INVITATION: means the invitation has expired or does not exist.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      INVALID_INVITATION = 11,

      /**
       * RATE_LIMIT: means the data transfer is rate-limited.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      RATE_LIMIT = 12,

      /**
       * CUSTOM_ERROR: means error codes that exceed this enumerated value are custom error codes.
       *
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      CUSTOM_ERROR = 1000,
    }

    /**
     * Result interface.
     *
     * @interface Result
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    interface Result<T> {
      /**
       * Error code.
       *
       * @type { number }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      code: number;

      /**
       * Error code description.
       *
       * @type { ?string }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      description?: string;

      /**
       * The result value.
       *
       * @type { ?T }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      value?: T;
    }

    /**
     * Privilege for the shared data.
     *
     * @interface Privilege
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    interface Privilege {
      /**
       * Whether the participants can write the shared data. The value <b>true</b>
       * means the participants can write the shared data; the value <b>false</b>
       * means the opposite.
       *
       * @type { ?boolean }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      writable?: boolean;

      /**
       * Whether the participants can read the shared data. The value <b>true</b>
       * means the participants can read the shared data; the value <b>false</b>
       * means the opposite.
       *
       * @type { ?boolean }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      readable?: boolean;

      /**
       * Whether the participants can create data. The value <b>true</b>
       * means the participants can create data; the value <b>false</b>
       * means the opposite.
       *
       * @type { ?boolean }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      creatable?: boolean;

      /**
       * Whether the participants can delete the shared data. The value <b>true</b>
       * means the participants can delete the shared data; the value <b>false</b>
       * means the opposite.
       *
       * @type { ?boolean }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      deletable?: boolean;

      /**
       * Whether the participants can share the data. The value <b>true</b>
       * means the participants can share the data; the value <b>false</b>
       * means the opposite.
       *
       * @type { ?boolean }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      shareable?: boolean;
    }

    /**
     * Participants in cloud sharing.
     *
     * @interface Participant
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    interface Participant {
      /**
       * Identity of participant.
       *
       * @type { string }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      identity: string;

      /**
       * Role of the participant, which can be inviter or invitee.
       *
       * @type { ?Role }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      role?: Role;

      /**
       * State of the sharing invitation.
       *
       * @type { ?State }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      state?: State;

      /**
       * Permissions for the shared data.
       *
       * @type { ?Privilege }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      privilege?: Privilege;

      /**
       * Attach information.
       *
       * @type { ?string }
       * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
       * @systemapi
       * @since 11
       */
      attachInfo?: string;
    }

    /**
     * Allocates shared resources based on conditions,
     * and shares data with the specified privilege to participants.
     *
     * @param { string } storeId - Indicates relational store name.
     * @param { relationalStore.RdbPredicates } predicates - See {@link relationalStore.RdbPredicates}.
     * @param { Array<Participant> } participants - Participants to share.
     * @param { Array<string> } [columns] - Columns to be shared.
     * @returns { Promise<relationalStore.ResultSet> } - Promise used to return {@link relationalStore.ResultSet}.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function allocResourceAndShare(
      storeId: string,
      predicates: relationalStore.RdbPredicates,
      participants: Array<Participant>,
      columns?: Array<string>
    ): Promise<relationalStore.ResultSet>;

    /**
     * Allocates shared resources based on conditions,
     * and shares data with the specified privilege to participants.
     *
     * @param { string } storeId - Indicates relational store name.
     * @param { relationalStore.RdbPredicates } predicates - See {@link relationalStore.RdbPredicates}.
     * @param { Array<Participant> } participants - Participants to share.
     * @param { AsyncCallback<relationalStore.ResultSet> } callback - Indicates the
     * callback invoked to return the {@link relationalStore.ResultSet}.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function allocResourceAndShare(
      storeId: string,
      predicates: relationalStore.RdbPredicates,
      participants: Array<Participant>,
      callback: AsyncCallback<relationalStore.ResultSet>
    ): void;

    /**
     * Allocates shared resources based on conditions,
     * and shares data with the specified privilege to participants.
     *
     * @param { string } storeId - Indicates relational store name.
     * @param { relationalStore.RdbPredicates } predicates - See {@link relationalStore.RdbPredicates}.
     * @param { Array<Participant> } participants - Participants to share.
     * @param { Array<string> } columns - Columns to be shared.
     * @param { AsyncCallback<relationalStore.ResultSet> } callback - Indicates the
     * callback invoked to return the {@link relationalStore.ResultSet}.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function allocResourceAndShare(
      storeId: string,
      predicates: relationalStore.RdbPredicates,
      participants: Array<Participant>,
      columns: Array<string>,
      callback: AsyncCallback<relationalStore.ResultSet>
    ): void;

    /**
     * Shares data with the specified privilege to participants.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * involved in the data sharing.
     * @param { AsyncCallback<Result<Array<Result<Participant>>>> } callback - Indicates the
     * callback invoked to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function share(
      sharingResource: string,
      participants: Array<Participant>,
      callback: AsyncCallback<Result<Array<Result<Participant>>>>
    ): void;

    /**
     * Shares data with the specified privilege to participants.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * involved in the data sharing.
     * @returns { Promise<Result<Array<Result<Participant>>>> } - Promise used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function share(
      sharingResource: string,
      participants: Array<Participant>
    ): Promise<Result<Array<Result<Participant>>>>;

    /**
     * UnShares data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * involved.
     * @param { AsyncCallback<Result<Array<Result<Participant>>>> } callback - Indicates the callback invoked
     * to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function unshare(
      sharingResource: string,
      participants: Array<Participant>,
      callback: AsyncCallback<Result<Array<Result<Participant>>>>
    ): void;

    /**
     * UnShares data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * involved.
     * @returns { Promise<Result<Array<Result<Participant>>>> } - Promise used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function unshare(
      sharingResource: string,
      participants: Array<Participant>
    ): Promise<Result<Array<Result<Participant>>>>;

    /**
     * Exit sharing.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { AsyncCallback<Result<void>> } callback - The callback of exit.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function exit(sharingResource: string, callback: AsyncCallback<Result<void>>): void;

    /**
     * Exit sharing.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @returns { Promise<Result<void>> } - The promise returned by the function.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function exit(sharingResource: string): Promise<Result<void>>;

    /**
     * Changes the permissions for the shared data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * whose permissions are to be changed.
     * @param { AsyncCallback<Result<Array<Result<Participant>>>> } callback - Indicates the
     * callback invoked to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function changePrivilege(
      sharingResource: string,
      participants: Array<Participant>,
      callback: AsyncCallback<Result<Array<Result<Participant>>>>
    ): void;

    /**
     * Changes the permissions for the shared data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { Array<Participant> } participants - Indicates the participants
     * whose permissions are to be changed.
     * @returns { Promise<Result<Array<Result<Participant>>>> } - Promise used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function changePrivilege(
      sharingResource: string,
      participants: Array<Participant>
    ): Promise<Result<Array<Result<Participant>>>>;

    /**
     * Queries the participants based on the specified shared data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { AsyncCallback<Result<Array<Participant>>> } callback - Indicates the
     * callback invoked to return the participants obtained.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function queryParticipants(sharingResource: string, callback: AsyncCallback<Result<Array<Participant>>>): void;

    /**
     * Queries the participants based on the specified shared data.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @returns { Promise<Result<Array<Participant>>> } - Promise used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function queryParticipants(sharingResource: string): Promise<Result<Array<Participant>>>;

    /**
     * Queries the participants based on the specified invitation code.
     *
     * @param { string } invitationCode - Indicates the invitation code.
     * @param { AsyncCallback<Result<Array<Participant>>> } callback - Indicates the
     * callback invoked to return the participants obtained.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function queryParticipantsByInvitation(
      invitationCode: string,
      callback: AsyncCallback<Result<Array<Participant>>>
    ): void;

    /**
     * Queries the participants based on the specified invitation code.
     *
     * @param { string } invitationCode - Indicates the invitation code.
     * @returns { Promise<Result<Array<Participant>>> } - Promise used to return the result.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function queryParticipantsByInvitation(invitationCode: string): Promise<Result<Array<Participant>>>;

    /**
     * Confirms the invitation of cloud sharing.
     *
     * @param { string } invitationCode - Indicates the invitation code.
     * @param { State } state - Indicates the state of invitation.
     * @param { AsyncCallback<Result<string>> } callback - Indicates the callback
     * invoked to return the sharing resource.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function confirmInvitation(invitationCode: string, state: State, callback: AsyncCallback<Result<string>>): void;

    /**
     * Confirms the invitation of cloud sharing.
     *
     * @param { string } invitationCode - Indicates the invitation code.
     * @param { State } state - Indicates the state of invitation.
     * @returns { Promise<Result<string>> } - Promise used to return the sharing resource.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function confirmInvitation(invitationCode: string, state: State): Promise<Result<string>>;

    /**
     * Changes confirmation of shared record.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { State } state - Indicates the state of invitation.
     * @param { AsyncCallback<Result<void>> } callback - Indicates the callback.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function changeConfirmation(sharingResource: string, state: State, callback: AsyncCallback<Result<void>>): void;

    /**
     * Changes confirmation of shared record.
     *
     * @param { string } sharingResource - Indicates the sharing resource.
     * @param { State } state - Indicates the state of invitation.
     * @returns { Promise<Result<void>> } - The promise returned by the function.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types;
     * 3. Parameter verification failed.
     * @throws { BusinessError } 801 - Capability not supported.
     * @syscap SystemCapability.DistributedDataManager.CloudSync.Client
     * @systemapi
     * @since 11
     */
    function changeConfirmation(sharingResource: string, state: State): Promise<Result<void>>;
  }
}

export default cloudData;
