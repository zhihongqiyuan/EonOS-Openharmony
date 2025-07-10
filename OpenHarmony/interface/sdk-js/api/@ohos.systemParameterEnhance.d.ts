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

import { AsyncCallback, BusinessError } from './@ohos.base';

/**
 * The interface of system parameters class.
 *
 * @namespace systemParameterEnhance
 * @syscap SystemCapability.Startup.SystemInfo
 * @systemapi Hide this for inner system use.
 * @since 9
 */
declare namespace systemParameterEnhance {
  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } def Default value.
   * @returns { string } the value of the parameter.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700101 - System parameter not found.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSync(key: string, def?: string): string;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { AsyncCallback<string> } callback Callback function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700101 - System parameter not found.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function get(key: string, callback: AsyncCallback<string>): void;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } def Default value.
   * @param { AsyncCallback<string> } callback Callback function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700101 - System parameter not found.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function get(key: string, def: string, callback: AsyncCallback<string>): void;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } def Default value.
   * @returns { Promise<string> }, which is used to obtain the result asynchronously.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700101 - System parameter not found.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function get(key: string, def?: string): Promise<string>;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } value System attribute value to set, cannot exceed 96 characters.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700102 - Invalid system parameter value.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function setSync(key: string, value: string): void;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } value System attribute value to set, cannot exceed 96 characters.
   * @param { AsyncCallback<void> } callback Callback function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700102 - Invalid system parameter value.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function set(key: string, value: string, callback: AsyncCallback<void>): void;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute, cannot exceed 128 characters,
   * only allow alphanumeric, plus '.', '-', '@', ':', or '_', don't allow '..'.
   * @param { string } value Default value, cannot exceed 96 characters.
   * @returns { Promise<void> }, which is used to obtain the result asynchronously.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified;
   * 2.incorrect parameter types; 3.parameter verification failed.
   * @throws { BusinessError } 14700102 - Invalid system parameter value.
   * @throws { BusinessError } 14700103 - The operation on the system permission is denied.
   * @throws { BusinessError } 14700104 - System internal error such as out memory or deadlock.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function set(key: string, value: string): Promise<void>;
}

export default systemParameterEnhance;
