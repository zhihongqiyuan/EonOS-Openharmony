/*
  * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import { AsyncCallback, Callback } from './@ohos.base';

/**
* Provides interfaces to get system capability.
* @namespace systemCapability
* @syscap SystemCapability.Developtools.Syscap
* @systemapi
* @since 9
*/
declare namespace systemCapability {
  /**
   * Get System Capability.
   * @param { AsyncCallback<string> }callback indicates the system capability string result.
   * @syscap SystemCapability.Developtools.Syscap
   * @systemapi
   * @since 9
   */
  function querySystemCapabilities(callback: AsyncCallback<string>): void;

  /**
   * Get System Capability.
   * @returns { Promise<string> } system capability string.
   * @syscap SystemCapability.Developtools.Syscap
   * @systemapi
   * @since 9
   */
  function querySystemCapabilities(): Promise<string>;
}

export default systemCapability;

