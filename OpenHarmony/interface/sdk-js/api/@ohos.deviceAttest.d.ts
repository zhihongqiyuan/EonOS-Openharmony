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

import { AsyncCallback, Callback } from './@ohos.base';

/**
 * xts_device_attest authResult information
 *
 * @namespace deviceAttest
 * @syscap SystemCapability.XTS.DeviceAttest
 * @since 9
 */
declare namespace deviceAttest {
  /**
   * Obtains the AttestResultInfo object.
   *
   * @param { AsyncCallback<AttestResultInfo> } callback Indicates the callback containing the AttestResultInfo object.
   * @throws { BusinessError } 202 - This api is system api, Please use the system application to call this api.
   * @throws { BusinessError } 401 - Input parameters wrong, the number of parameters is incorrect,
                or the type of parameters is incorrect.
   * @throws { BusinessError } 20000001 - System service exception, please try again or reboot your device.
   * @syscap SystemCapability.XTS.DeviceAttest
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getAttestStatus(callback: AsyncCallback<AttestResultInfo>): void;

  /**
   * Obtains the AttestResultInfo object.
   *
   * @returns { Promise<AttestResultInfo> } Returns that the AttestResultInfo object is returned in Promise mode.
   * @throws { BusinessError } 202 - This api is system api, Please use the system application to call this api.
   * @throws { BusinessError } 401 - Input parameters wrong, the number of parameters is incorrect,
                or the type of parameters is incorrect.
   * @throws { BusinessError } 20000001 - System service exception, please try again or reboot your device.
   * @syscap SystemCapability.XTS.DeviceAttest
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getAttestStatus(): Promise<AttestResultInfo>;

  /**
   * Obtains the AttestResultInfo object.
   *
   * @returns { AttestResultInfo } Obtains the AttestResultInfo object synchronously.
   * @throws { BusinessError } 202 - This api is system api, Please use the system application to call this api.
   * @throws { BusinessError } 401 - Input parameters wrong, the number of parameters is incorrect,
                or the type of parameters is incorrect.
   * @throws { BusinessError } 20000001 - System service exception, please try again or reboot your device.
   * @syscap SystemCapability.XTS.DeviceAttest
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getAttestStatusSync(): AttestResultInfo;

  /**
   * Device attest result information.
   *
   * @interface AttestResultInfo
   * @syscap SystemCapability.XTS.DeviceAttest
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface AttestResultInfo {

    /**
     * Result of the device hardware information authentication.
     *
     * @type { number }
     * @syscap SystemCapability.XTS.DeviceAttest
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    authResult: number;

    /**
     * Result of the device software information authentication.
     *
     * @type { number }
     * @syscap SystemCapability.XTS.DeviceAttest
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    softwareResult: number;

    /**
     * Software result detail array that includes versionId, patchLevel,
     * rootHash and a reserved space.
     *
     * @type { Array<number> }
     * @syscap SystemCapability.XTS.DeviceAttest
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    softwareResultDetail: Array<number>;

    /**
     * Credential sent from the cloud.
     *
     * @type { string }
     * @syscap SystemCapability.XTS.DeviceAttest
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    ticket: string;
  }
}

export default deviceAttest;