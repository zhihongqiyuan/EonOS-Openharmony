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
 * @kit BackgroundTasksKit
 */

import { AsyncCallback } from './@ohos.base';

/**
 * Provides methods for managing device standby,
 * including the methods for querying standby status and exemption list.
 *
 * @namespace deviceStandby
 * @since 10
 */
declare namespace deviceStandby {

  /**
   * Returns the information about the specified exempted application.
   *
   * @permission ohos.permission.DEVICE_STANDBY_EXEMPTION
   * @param { number } resourceTypes - the combination of {@link ResourceType} values.
   * @param { AsyncCallback<Array<ExemptedAppInfo>> } callback - the callback of getExemptedApps.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters.
   * @throws { BusinessError } 9800003 - Failed to complete inner transaction.
   * @throws { BusinessError } 9800004 - Failed to get device standby service. Possible cause: A necessary system service is not ready.
   * @throws { BusinessError } 18700001 - Caller information verification failed.
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getExemptedApps(resourceTypes: number, callback: AsyncCallback<Array<ExemptedAppInfo>>): void;

  /**
   * Returns the information about the specified exempted application.
   *
   * @permission ohos.permission.DEVICE_STANDBY_EXEMPTION
   * @param { number } resourceTypes - the combination of {@link ResourceType} values.
   * @returns { Promise<Array<ExemptedAppInfo>> } the promise returned by getExemptedApps.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters.
   * @throws { BusinessError } 9800003 - Failed to complete inner transaction.
   * @throws { BusinessError } 9800004 - Failed to get device standby service. Possible cause: A necessary system service is not ready.
   * @throws { BusinessError } 18700001 - Caller information verification failed.
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getExemptedApps(resourceTypes: number): Promise<Array<ExemptedAppInfo>>;

  /**
   * Requests exemption resources.
   *
   * @permission ohos.permission.DEVICE_STANDBY_EXEMPTION
   * @param { ResourceRequest } request - requesting or releasing resources.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters.
   * @throws { BusinessError } 9800003 - Failed to complete inner transaction.
   * @throws { BusinessError } 9800004 - Failed to get device standby service. Possible cause: A necessary system service is not ready.
   * @throws { BusinessError } 18700001 - Caller information verification failed.
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function requestExemptionResource(request: ResourceRequest): void;

  /**
   * Releases exemption resources.
   *
   * @permission ohos.permission.DEVICE_STANDBY_EXEMPTION
   * @param { ResourceRequest } request - requesting or releasing resources.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters.
   * @throws { BusinessError } 9800003 - Failed to complete inner transaction.
   * @throws { BusinessError } 9800004 - Failed to get device standby service. Possible cause: A necessary system service is not ready.
   * @throws { BusinessError } 18700001 - Caller information verification failed.
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function releaseExemptionResource(request: ResourceRequest): void;

  /**
   * The type of exemption resources requested by the application.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export enum ResourceType {
    /**
     * The resource for non-standby network access.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    NETWORK = 1,

    /**
     * The resource for non-standby cpu running-lock.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    RUNNING_LOCK = 1 << 1,

    /**
     * The resource for non-standby timer.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    TIMER = 1 << 2,

    /**
     * The resource for non-standby workscheduler.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    WORK_SCHEDULER = 1 << 3,

    /**
     * The resource for non-standby automatic synchronization.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    AUTO_SYNC = 1 << 4,

    /**
     * The resource for non-standby push-kit.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    PUSH = 1 << 5,

    /**
     * The resource for non-standby freezing application.
     *
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FREEZE = 1 << 6
  }

  /**
   * Information about an exempted application.
   *
   * @interface ExemptedAppInfo
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface ExemptedAppInfo {
    /**
     * The set of resource types that an application requests.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    resourceTypes: number;

    /**
     * The application name.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    name: string;

    /**
     * The exemption duration.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    duration: number;
  }

  /**
   * The request of standby resources.
   *
   * @interface ResourceRequest
   * @syscap SystemCapability.ResourceSchedule.DeviceStandby
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface ResourceRequest {
    /**
     * The set of resource types that an application requests.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    resourceTypes: number;

    /**
     * The application uid.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    uid: number;

    /**
     * The application name.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    name: string;

    /**
     * The exemption duration.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    duration: number;

    /**
     * The reason for the request.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.DeviceStandby
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    reason: string;
  }
}

export default deviceStandby;