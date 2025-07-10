/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

import { BusinessError } from './@ohos.base';

/**
 * Provides interfaces to control the power of display.
 *
 * @namespace brightness
 * @syscap SystemCapability.PowerManager.DisplayPowerManager
 * @systemapi
 * @since 7
 */
declare namespace brightness {
  /**
   * Sets the screen brightness.
   *
   * @param { number } value Brightness value, ranging from 0 to 255.
   * value parameter must be of type number.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types;
   * @throws { BusinessError } 4700101 - Failed to connect to the service.
   * @syscap SystemCapability.PowerManager.DisplayPowerManager
   * @systemapi
   * @since 7
   */
  function setValue(value: number): void;

  /**
   * Sets the screen brightness.
   *
   * @param { number } value - Brightness value, ranging from 0 to 255.
   * value parameter must be of type number.
   * @param { boolean } continuous - This parameter is used in the scenario of continuous adjustment to the brightness.
   * You are advised to set this parameter to true during the continuous adjustment and
   * to false at the end of the continuous adjustment for better performance.
   * continuous parameter must be of type boolean.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types;
   * @throws { BusinessError } 4700101 - Failed to connect to the service.
   * @syscap SystemCapability.PowerManager.DisplayPowerManager
   * @systemapi
   * @since 11
   */
  function setValue(value: number, continuous: boolean): void;
}
export default brightness;
