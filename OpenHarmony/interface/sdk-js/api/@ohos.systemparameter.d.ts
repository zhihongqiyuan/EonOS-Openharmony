/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
 * @namespace systemParameter
 * @syscap SystemCapability.Startup.SystemInfo
 * @systemapi Hide this for inner system use.
 * @since 6
 */
declare namespace systemParameter {
  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } def Default value.
   * @returns { string } if the parameter is empty or doesn't exist, empty string will be returned.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function getSync(key: string, def?: string): string;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { AsyncCallback<string> } callback Callback function.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function get(key: string, callback: AsyncCallback<string>): void;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } def Default value.
   * @param { AsyncCallback<string> } callback Callback function.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function get(key: string, def: string, callback: AsyncCallback<string>): void;

  /**
   * Gets the value of the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } def Default value.
   * @returns { Promise<string> } , which is used to obtain the result asynchronously.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function get(key: string, def?: string): Promise<string>;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } value value System attribute value to set.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   */
  function setSync(key: string, value: string): void;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } value System attribute value to set.
   * @param { AsyncCallback<void> } callback Callback function.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function set(key: string, value: string, callback: AsyncCallback<void>): void;

  /**
   * Sets a value for the attribute with the specified key.
   *
   * @param { string } key Key of the system attribute.
   * @param { string } value Default value.
   * @returns { Promise<void> } which is used to obtain the result asynchronously.
   * @syscap SystemCapability.Startup.SystemInfo
   * @systemapi Hide this for inner system use.
   * @since 6
   * @deprecated since 9
   */
  function set(key: string, value: string): Promise<void>;
}

export default systemParameter;
