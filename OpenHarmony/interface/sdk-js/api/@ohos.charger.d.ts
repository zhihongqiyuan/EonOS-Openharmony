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
 * @kit BasicServicesKit
 */

/**
 * Provides charger information includes type and protocol.
 *
 * @namespace charger
 * @syscap SystemCapability.PowerManager.BatteryManager.Core
 * @systemapi
 * @since 10
 */
declare namespace charger {
  /**
   * Indicates the type of the charger plugged-in.
   *
   * @enum { number }
   * @syscap SystemCapability.PowerManager.BatteryManager.Core
   * @systemapi
   * @since 10
   */
  export enum ChargeType {
    /**
     * Unknown type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    NONE,
    /**
     * Wired normal type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRED_NORMAL,
    /**
     * Wired quick type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRED_QUICK,
    /**
     * Wired super quick type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRED_SUPER_QUICK,
    /**
     * Wireless normal type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRELESS_NORMAL,
    /**
     * Wireless quick type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRELESS_QUICK,
    /**
     * Wireless super quick type
     *
     * @syscap SystemCapability.PowerManager.BatteryManager.Core
     * @systemapi
     * @since 10
     */
    WIRELESS_SUPER_QUICK
  }
}
export default charger;
