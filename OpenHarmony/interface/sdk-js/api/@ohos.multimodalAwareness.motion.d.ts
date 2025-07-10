/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 * @kit MultimodalAwarenessKit
 */

import type { Callback } from "./@ohos.base";

/**
 * This module provides the capability to subscribe to report the action or motion.
 *
 * @namespace motion
 * @syscap SystemCapability.MultimodalAwareness.Motion
 * @since 15
 */

declare namespace motion {
  /**
   * Enum for operating hand status.
   *
   * @enum { number } OperatingHandStatus
   * @syscap SystemCapability.MultimodalAwareness.Motion
   * @since 15
   */
  export enum OperatingHandStatus {
    /**
     * indicates nothing has been detected.
     *
     * @syscap SystemCapability.MultimodalAwareness.Motion
     * @since 15
     */
    UNKNOWN_STATUS = 0,
    /**
     * indicates the operating hand is left hand.
     *
     * @syscap SystemCapability.MultimodalAwareness.Motion
     * @since 15
     */
    LEFT_HAND_OPERATED = 1,
    /**
     * indicates the operating hand is right hand.
     *
     * @syscap SystemCapability.MultimodalAwareness.Motion
     * @since 15
     */
    RIGHT_HAND_OPERATED = 2
  }

  /**
   * Subscribe to detect the operating hand changed event.
   * @permission ohos.permission.ACTIVITY_MOTION
   * @param { 'operatingHandChanged' } type - Indicates the event type.
   * @param { Callback<OperatingHandStatus> } callback - Indicates the callback for getting the event data.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to subscribe operatingHandChanged
   * <br> event forbidden by permission: ohos.permission.ACTIVITY_MOTION.
   * @throws { BusinessError } 401 - Parameter error. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported. Function can not work correctly due to limited
   * <br> device capabilities.
   * @throws { BusinessError } 31500001 - Service exception. Possible causes: 1. A system error, such as null pointer, container-related exception;
   * <br>2. N-API invocation exception, invalid N-API status.
   * @throws { BusinessError } 31500002 - Subscription failed. Possible causes: 1. Callback registration failure;
   * <br>2. Failed to bind native object to js wrapper; 3. N-API invocation exception, invalid N-API status; 4. IPC request exception.
   * @syscap SystemCapability.MultimodalAwareness.Motion
   * @since 15
   */
  function on(type: 'operatingHandChanged', callback: Callback<OperatingHandStatus>): void;

  /**
   * Unsubscribe to detect the operating hand changed event.
   * @permission ohos.permission.ACTIVITY_MOTION
   * @param { 'operatingHandChanged' } type - Indicates the event type.
   * @param { Callback<OperatingHandStatus> } callback - Indicates the callback for getting the event data.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to unsubscribe operatingHandChanged
   * <br> event forbidden by permission: ohos.permission.ACTIVITY_MOTION.
   * @throws { BusinessError } 401 - Parameter error. Parameter verification failed.
   * @throws { BusinessError } 801 - Capability not supported. Function can not work correctly due to limited
   * <br> device capabilities.
   * @throws { BusinessError } 31500001 - Service exception. Possible causes: 1. A system error, such as null pointer, container-related exception;
   * <br>2. N-API invocation exception, invalid N-API status.
   * @throws { BusinessError } 31500003 - Unsubscription failed. Possible causes: 1. Callback failure;
   * <br>2. N-API invocation exception, invalid N-API status; 3. IPC request exception.
   * @syscap SystemCapability.MultimodalAwareness.Motion
   * @since 15
   */
  function off(type: 'operatingHandChanged', callback?: Callback<OperatingHandStatus>): void;

  /**
   * Get the recent operating hand status.
   * @permission ohos.permission.ACTIVITY_MOTION
   * @returns { OperatingHandStatus } The result of operating hand status.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to get the recent operating hand
   * <br> status forbidden by permission: ohos.permission.ACTIVITY_MOTION.
   * @throws { BusinessError } 801 - Capability not supported. Function can not work correctly due to limited
   * <br> device capabilities.
   * @throws { BusinessError } 31500001 - Service exception. Possible causes: 1. A system error, such as null pointer, container-related exception;
   * <br>2. N-API invocation exception, invalid N-API status.
   * @syscap SystemCapability.MultimodalAwareness.Motion
   * @since 15
   */
  function getRecentOperatingHandStatus(): OperatingHandStatus;
}
export default motion;
