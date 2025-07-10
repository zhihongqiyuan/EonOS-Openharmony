/*
 * Copyright (c) 2023  Huawei Device Co., Ltd.
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
 * @kit BackgroundTasksKit
 */

import { AsyncCallback, Callback } from './@ohos.base';

/**
 * Provides methods for managing bundle usage statistics,
 * including the methods for querying bundle usage information and state data.
 *
 * <p>You can use the methods defined in this class to query
 * the usage history and states of bundles in a specified period.
 * The system stores the query result in a {@link BundleStatsInfo} instance and
 * then returns it to you.
 *
 * @namespace usageStatistics
 * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
 * @since 9
 */
declare namespace usageStatistics {
  /**
   * @interface BundleStatsInfo
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface BundleStatsInfo {
    /**
     * The identifier of BundleStatsInfo.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    id: number;

    /**
     * The total duration, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityInFgTotalTime?: number;

    /**
     * The last time when the application was accessed, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityPrevAccessTime?: number;

    /**
     * The last time when the application was visible in the foreground, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityPrevSeenTime?: number;

    /**
     * The total duration, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilitySeenTotalTime?: number;

    /**
     * The bundle name of the application.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    bundleName?: string;

    /**
     * The total duration, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    fgAbilityAccessTotalTime?: number;

    /**
     * The last time when the foreground application was accessed, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    fgAbilityPrevAccessTime?: number;

    /**
     * The time of the first bundle usage record in this {@code BundleActiveInfo} object,
     * in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    infosBeginTime?: number;

    /**
     * The time of the last bundle usage record in this {@code BundleActiveInfo} object,
     * in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    infosEndTime?: number;

    /**
     * The app index of the application.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 15
     */
    appIndex?: number;
  }

  /**
   * @interface HapFormInfo
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface HapFormInfo {
    /**
     * The form name.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    formName: string;

    /**
     * The form dimension.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    formDimension: number;

    /**
     * The form id.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    formId: number;

    /**
     * The last time when the form was accessed, in milliseconds..
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    formLastUsedTime: number;

    /**
     * The click count of module.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    count: number;
  }

  /**
   * @interface HapModuleInfo
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface HapModuleInfo {
    /**
     * The device id of module.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    deviceId?: string;

    /**
     * The bundle name.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    bundleName: string;

    /**
     * The module name.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    moduleName: string;

    /**
     * The main ability name of module.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityName?: string;

    /**
     * The label id of application.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    appLabelId?: number;

    /**
     * The label id of module.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    labelId?: number;

    /**
     * The description id of application.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    descriptionId?: number;

    /**
     * The ability id of main ability.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityLableId?: number;

    /**
     * The description id of main ability.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityDescriptionId?: number;

    /**
     * The icon id of main ability.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    abilityIconId?: number;

    /**
     * The launch count of module.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    launchedCount: number;

    /**
     * The last time when the module was accessed, in milliseconds.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    lastModuleUsedTime: number;

    /**
     * The form usage record list of current module.
     * @type { Array<HapFormInfo> }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    formRecords: Array<HapFormInfo>;
  }

  /**
   * @interface DeviceEventStats
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface DeviceEventStats {
    /**
     * The bundle name or system event name.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    name: string;

    /**
     * The event id.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    eventId: number;

    /**
     * The the event occurrence number.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    count: number;
  }

  /**
   * @interface BundleEvents
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface BundleEvents {
    /**
     * The usage group of the application.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    appGroup?: number;

    /**
     * The bundle name.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    bundleName?: string;

    /**
     * The shortcut ID.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    indexOfLink?: string;

    /**
     * The class name.
     * @type { ?string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    nameOfClass?: string;

    /**
     * The time when this state occurred, in milliseconds.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    eventOccurredTime?: number;

    /**
     * The event id.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    eventId?: number;
  }

  /**
   * @interface AppGroupCallbackInfo
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface AppGroupCallbackInfo {
    /**
     * The usage old group of the application
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    appOldGroup: number;

    /**
     * The usage new group of the application
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    appNewGroup: number;

    /**
     * The use id
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    userId: number;

    /**
     * The change reason
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    changeReason: number;

    /**
     * The bundle name
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    bundleName: string;
  }

  /**
   * Checks whether the application with a specified bundle name is in the idle state.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - Indicates the bundle name of the application to query.
   * @param { AsyncCallback<boolean> } callback - the callback of isIdleState.
   * <p> boolean value is true mean the application is idle in a particular period; false mean otherwise.
   * The time range of the particular period is defined by the system, which may be hours or days.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isIdleState(bundleName: string, callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether the application with a specified bundle name is in the idle state.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - Indicates the bundle name of the application to query.
   * @returns { Promise<boolean> } the promise returned by isIdleState.
   * <p> boolean value is true mean the application is idle in a particular period; false mean otherwise.
   * The time range of the particular period is defined by the system, which may be hours or days.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isIdleState(bundleName: string): Promise<boolean>;

  /**
   * Checks whether the application with a specified bundle name is in the idle state.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - Indicates the bundle name of the application to query.
   * @returns { boolean }
   * <p> boolean value is true mean the application is idle in a particular period; false mean otherwise.
   * The time range of the particular period is defined by the system, which may be hours or days.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function isIdleStateSync(bundleName: string): boolean;

  /**
   * Queries the app group of the calling application.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { AsyncCallback<number> } callback - the callback of queryAppGroup.
   * <p> Returns the app group of the calling application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryAppGroup(callback: AsyncCallback<number>): void;

  /**
   * Queries the app group of the calling application.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @returns { Promise<number> } the promise returned by queryAppGroup.
   * <p> Returns the app group of the calling application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryAppGroup(): Promise<number>;

  /**
   * Queries the app group of the calling application.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @returns { number } Returns the app group of the calling application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryAppGroupSync(): number;

  /**
   * Queries the usage priority group by bundleName.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @param { AsyncCallback<number> } callback - the callback of queryAppGroup.
   * <p> the usage priority group of the calling application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryAppGroup(bundleName: string, callback: AsyncCallback<number>): void;

  /**
   * Queries the usage priority group by bundleName.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @returns { Promise<number> } the promise returned by queryAppGroup.
   * <p> the usage priority group of the calling application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryAppGroup(bundleName: string): Promise<number>;

  /**
   * Queries the usage priority group by bundleName.
   * <p>The priority defined in a priority group restricts the resource usage of an application,
   * for example, restricting the running of background tasks. </p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @returns { number } the usage priority group of the calling application.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000005 - Application is not installed.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100002 - Failed to get the application group information.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function queryAppGroupSync(bundleName: string): number;

  /**
   * @typedef { Record<string, BundleStatsInfo> }
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  type BundleStatsMap = Record<string, BundleStatsInfo>;

  /**
   * Queries usage information about each bundle within a specified period.
   * <p>This method queries usage information at the {@link #BY_OPTIMIZED} interval by default.</p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<BundleStatsMap> } callback - the callback of queryBundleStatsInfos,
   * <p> the {@link BundleStatsMap} objects containing the usage information about each bundle.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleStatsInfos(begin: number, end: number, callback: AsyncCallback<BundleStatsMap>): void;

  /**
   * Queries usage information about each bundle within a specified period.
   * <p>This method queries usage information at the {@link #BY_OPTIMIZED} interval by default.</p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<BundleStatsMap> } the promise returned by queryBundleStatsInfos.
   * <p> the {@link BundleStatsMap} objects containing the usage information about each bundle.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleStatsInfos(begin: number, end: number): Promise<BundleStatsMap>;

  /**
   * @typedef { Record<string, Array<BundleStatsInfo>> }
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 15
   */
  type AppStatsMap = Record<string, Array<BundleStatsInfo>>;

  /**
   * Queries usage information about each application within a specified period.
   * <p>This method queries usage information at the {@link #BY_OPTIMIZED} interval by default.</p>
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<AppStatsMap> } the promise returned by queryAppStatsInfos.
   * <p> the {@link AppStatsMap} objects containing the usage information about each application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 15
   */
  function queryAppStatsInfos(begin: number, end: number): Promise<AppStatsMap>;

  /**
   * Queries the last usage timestamp by bundleName and app index.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @param { appInfo } bundle name and app index info for each application.
   * @returns { Promise<AppStatsMap> } the promise returned by queryLastUseTime.
   * <p> the {@link AppStatsMap} objects containing the usage information about each application.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 15
   */
  function queryLastUseTime(appInfo: Record<string, Array<number>>): Promise<AppStatsMap>;

  /**
   * Declares interval type.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export enum IntervalType {
    /**
     * Indicates the interval type that will determine the optimal interval based on the start and end time.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BY_OPTIMIZED = 0,

    /**
     * Indicates the daily interval.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BY_DAILY = 1,

    /**
     * Indicates the weekly interval.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BY_WEEKLY = 2,

    /**
     * Indicates the monthly interval.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BY_MONTHLY = 3,

    /**
     * Indicates the annually interval.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BY_ANNUALLY = 4
  }

  /**
   * Queries usage information about each bundle within a specified period at a specified interval.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { IntervalType } byInterval - Indicates the interval at which the usage statistics are queried.
   * <p> The value can be {@link #BY_OPTIMIZED}, {@link #BY_DAILY},
   * {@link #BY_WEEKLY}, {@link #BY_MONTHLY}, or {@link #BY_ANNUALLY}.</p>
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<Array<BundleStatsInfo>> } callback - the callback of queryBundleStatsInfoByInterval.
   * <p> the list of {@link BundleStatsInfo} objects containing the usage information about each bundle.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleStatsInfoByInterval(
    byInterval: IntervalType,
    begin: number,
    end: number,
    callback: AsyncCallback<Array<BundleStatsInfo>>
  ): void;

  /**
   * Queries usage information about each bundle within a specified period at a specified interval.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { IntervalType } byInterval - Indicates the interval at which the usage statistics are queried.
   * <p> The value can be {@link #BY_OPTIMIZED}, {@link #BY_DAILY},
   * {@link #BY_WEEKLY}, {@link #BY_MONTHLY}, or {@link #BY_ANNUALLY}.</p>
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<Array<BundleStatsInfo>> } the promise returned by queryBundleStatsInfoByInterval.
   * <p> the list of {@link BundleStatsInfo} objects containing the usage information about each bundle.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleStatsInfoByInterval(
    byInterval: IntervalType,
    begin: number,
    end: number
  ): Promise<Array<BundleStatsInfo>>;

  /**
   * Queries state data of all bundles within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<Array<BundleEvents>> } callback - the promise returned by queryBundleEvents.
   * <p> the list of {@link BundleEvents} objects containing the state data of all bundles.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleEvents(begin: number, end: number, callback: AsyncCallback<Array<BundleEvents>>): void;

  /**
   * Queries state data of all bundles within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<Array<BundleEvents>> } the promise returned by queryBundleEvents.
   * <p> the list of {@link BundleEvents} objects containing the state data of all bundles.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryBundleEvents(begin: number, end: number): Promise<Array<BundleEvents>>;

  /**
   * Queries state data of the current bundle within a specified period.
   *
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<Array<BundleEvents>> } callback - the callback of queryCurrentBundleEvents.
   * <p> the {@link BundleEvents} object Array containing the state data of the current bundle.</p>
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryCurrentBundleEvents(begin: number, end: number, callback: AsyncCallback<Array<BundleEvents>>): void;

  /**
   * Queries state data of the current bundle within a specified period.
   *
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<Array<BundleEvents>> } the promise returned by queryCurrentBundleEvents.
   * <p> the {@link BundleEvents} object Array containing the state data of the current bundle.</p>
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryCurrentBundleEvents(begin: number, end: number): Promise<Array<BundleEvents>>;

  /**
   * Queries recently module usage records with maxNum.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } maxNum - Indicates max record number in result, max value is 1000, default value is 1000.
   * @param { AsyncCallback<Array<HapModuleInfo>> } callback - the callback of queryModuleUsageRecords.
   * <p> the {@link HapModuleInfo} object Array containing the usage data of the modules.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryModuleUsageRecords(maxNum: number, callback: AsyncCallback<Array<HapModuleInfo>>): void;

  /**
   * Queries recently module usage records with maxNum.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } maxNum - Indicates max record number in result, max value is 1000, default value is 1000.
   * @returns { Promise<Array<HapModuleInfo>> } the promise returned by queryModuleUsageRecords.
   * <p> the {@link HapModuleInfo} object Array containing the usage data of the modules.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryModuleUsageRecords(maxNum: number): Promise<Array<HapModuleInfo>>;

  /**
   * Queries recently module usage records.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { AsyncCallback<Array<HapModuleInfo>> } callback - the callback of queryModuleUsageRecords.
   * <p> the {@link HapModuleInfo} object Array containing the usage data of the modules.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryModuleUsageRecords(callback: AsyncCallback<Array<HapModuleInfo>>): void;

  /**
   * Queries recently module usage records.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @returns { Promise<Array<HapModuleInfo>> } the promise returned by queryModuleUsageRecords.
   * <p> the {@link HapModuleInfo} object Array containing the usage data of the modules.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryModuleUsageRecords(): Promise<Array<HapModuleInfo>>;

  /**
   * Declares group type.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export enum GroupType {
    /**
     * Indicates the alive group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    ALIVE_GROUP = 10,

    /**
     * Indicates the daily group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    DAILY_GROUP = 20,

    /**
     * Indicates the fixed group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    FIXED_GROUP = 30,

    /**
     * Indicates the rare group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    RARE_GROUP = 40,

    /**
     * Indicates the limit group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    LIMITED_GROUP = 50,

    /**
     * Indicates the never group.
     *
     * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    NEVER_GROUP = 60
  }

  /**
   * Set app group by bundleName.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @param { GroupType } newGroup - the group of the application whose name is bundleName.
   * @param { AsyncCallback<void> } callback - the callback of setAppGroup.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setAppGroup(bundleName: string, newGroup: GroupType, callback: AsyncCallback<void>): void;

  /**
   * Set app group by bundleName.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { string } bundleName - name of the application.
   * @param { GroupType } newGroup - the group of the application whose name is bundleName.
   * @returns { Promise<void> } the promise returned by setAppGroup.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setAppGroup(bundleName: string, newGroup: GroupType): Promise<void>;

  /**
   * Register appGroup change callback to service.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { Callback<AppGroupCallbackInfo> } groupCallback -
   * <p> callback of AppGroupCallbackInfo when the group of app changed.</p>
   * @param { AsyncCallback<void> } callback - the callback of registerAppGroupCallBack.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function registerAppGroupCallBack(groupCallback: Callback<AppGroupCallbackInfo>, callback: AsyncCallback<void>): void;

  /**
   * Register appGroup change callback to service.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { Callback<AppGroupCallbackInfo> } groupCallback -
   * <p> callback of AppGroupCallbackInfo when the group of app changed.</p>
   * @returns { Promise<void> } the promise returned by registerAppGroupCallBack.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function registerAppGroupCallBack(groupCallback: Callback<AppGroupCallbackInfo>): Promise<void>;

  /**
   * Unregister appGroup change callback from service.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { AsyncCallback<void> } callback - the callback of unregisterAppGroupCallBack.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function unregisterAppGroupCallBack(callback: AsyncCallback<void>): void;

  /**
   * Unregister appGroup change callback from service.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @returns { Promise<void> } the promise returned by unregisterAppGroupCallBack.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10100001 - Repeated operation on the application group.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.AppGroup
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function unregisterAppGroupCallBack(): Promise<void>;

  /**
   * Queries device event states data within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<Array<DeviceEventStats>> } callback - the callback of queryDeviceEventStats.
   * <p> the {@link DeviceEventStats} object Array containing the event states data.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryDeviceEventStats(begin: number, end: number, callback: AsyncCallback<Array<DeviceEventStats>>): void;

  /**
   * Queries device event states data within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<Array<DeviceEventStats>> } the promise returned by queryDeviceEventStats.
   * <p> the {@link DeviceEventStats} object Array containing the event states data.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryDeviceEventStats(begin: number, end: number): Promise<Array<DeviceEventStats>>;

  /**
   * Queries app notification number within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @param { AsyncCallback<Array<DeviceEventStats>> } callback - the callback of queryNotificationEventStats.
   * <p> the {@link DeviceEventStats} object Array containing the event states data.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryNotificationEventStats(
    begin: number,
    end: number,
    callback: AsyncCallback<Array<DeviceEventStats>>
  ): void;

  /**
   * Queries app notification number within a specified period identified by the start and end time.
   *
   * @permission ohos.permission.BUNDLE_ACTIVE_INFO
   * @param { number } begin - Indicates the start time of the query period, in milliseconds.
   * @param { number } end - Indicates the end time of the query period, in milliseconds.
   * @returns { Promise<Array<DeviceEventStats>> } the promise returned by queryNotificationEventStats.
   * <p> the {@link DeviceEventStats} object Array containing the event states data.</p>
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 10000001 - Memory operation failed.
   * @throws { BusinessError } 10000002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 10000003 - Failed to get system ability manager.
   * @throws { BusinessError } 10000004 - Failed to access the device usage service.
   * @throws { BusinessError } 10000006 - Failed to get the application information.
   * @throws { BusinessError } 10000007 - Failed to get the system time.
   * @syscap SystemCapability.ResourceSchedule.UsageStatistics.App
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function queryNotificationEventStats(begin: number, end: number): Promise<Array<DeviceEventStats>>;
}

export default usageStatistics;
