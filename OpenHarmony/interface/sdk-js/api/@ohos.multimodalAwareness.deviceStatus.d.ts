/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
 * @kit MultimodalAwarenessKit
 */

import type { Callback } from "./@ohos.base";

/**
 * This module provides the capability to subscribe to report the device status.
 *
 * @namespace deviceStatus
 * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
 * @since 18
 */

declare namespace deviceStatus {
  /**
   * Enum for steady standing status.
   *
   * @enum { number } SteadyStandingStatus
   * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
   * @since 18
   */
  export enum SteadyStandingStatus {
    /**
     * indicates exit status
     *
     * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
     * @since 18
     */
    STATUS_EXIT = 0,
    /**
     * indicates enter status
     *
     * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
     * @since 18
     */
    STATUS_ENTER = 1
  }

  /**
   * Subscribe to detect the steady standing status
   * @param { 'steadyStandingDetect' } type - Indicates the event type.
   * @param { Callback<SteadyStandingStatus> } callback - Indicates the callback for getting the event data.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 
   * <br> 2. Incorrect parameter types. 3.Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported. Function can not work correctly due to limited
   * <br> device capabilities.
   * @throws { BusinessError } 32500001 - Service exception.
   * @throws { BusinessError } 32500002 - Subscription failed.
   * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
   * @since 18
   */
  function on(type: 'steadyStandingDetect', callback: Callback<SteadyStandingStatus>): void;

  /**
   * Unsubscribe to detect the steady standing status
   * @param { 'steadyStandingDetect' } type - Indicates the event type.
   * @param { Callback<SteadyStandingStatus> } callback - Indicates the callback for getting the event data.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 
   * <br> 2. Incorrect parameter types. 3.Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported. Function can not work correctly due to limited
   * <br> device capabilities.
   * @throws { BusinessError } 32500001 - Service exception.
   * @throws { BusinessError } 32500003 - Unsubscription failed.
   * @syscap SystemCapability.MultimodalAwareness.DeviceStatus
   * @since 18
   */
  function off(type: 'steadyStandingDetect', callback?: Callback<SteadyStandingStatus>): void;
}
export default deviceStatus;
