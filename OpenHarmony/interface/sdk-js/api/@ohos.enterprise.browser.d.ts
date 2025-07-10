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
 * @kit MDMKit
 */

import type { AsyncCallback } from './@ohos.base';
import type Want from './@ohos.app.ability.Want';

/**
 * This module provides the capability to manage the browser policies of the enterprise devices.
 *
 * @namespace browser
 * @syscap SystemCapability.Customization.EnterpriseDeviceManager
 * @since 10
 */
declare namespace browser {
  /**
   * Allow the administrator to set the browser policies.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_BROWSER_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - id of the bundle that need to set policies. It cannot be empty.
   * @param { string } policies - browser policies that need to set. It must be a correct JSON character string that
   *                              can be converted into browser policies.
   * @param { AsyncCallback<void> } callback - the callback of setPolicies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function setPolicies(admin: Want, appId: string, policies: string, callback: AsyncCallback<void>): void;

  /**
   * Allow the administrator to set the browser policies.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_BROWSER_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - id of the bundle that need to set policies. It cannot be empty.
   * @param { string } policies - browser policies that need to set. It must be a correct JSON character string that
   *                              can be converted into browser policies.
   * @returns { Promise<void> } the promise returned by the setPolicies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function setPolicies(admin: Want, appId: string, policies: string): Promise<void>;

  /**
   * Allow the administrator to get the browser policies.
   * This function can be called by a super administrator.
   *
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - id of the bundle that need to set policies. It cannot be empty.
   * @param { AsyncCallback<string> } callback - the callback carries the browser policies returned by the getPolicies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function getPolicies(admin: Want, appId: string, callback: AsyncCallback<string>): void;

  /**
   * Allow the administrator to get the browser policies.
   * This function can be called by a super administrator.
   *
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - id of the bundle that need to set policies. It cannot be empty.
   * @returns { Promise<string> } the promise carries the browser policies returned by the getPolicies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function getPolicies(admin: Want, appId: string): Promise<string>;

  /**
   * Sets the browser policy.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_BROWSER_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - appId indicates the id of the bundle that need to set policy. It cannot be empty.
   * @param { string } policyName - policyName indicates the browser policy name that need to set.
   * @param { string } policyValue - policyValue indicates the browser policy value that need to set. It must be a
   *                                 correct JSON character string that can be converted into browser policies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function setPolicySync(admin: Want, appId: string, policyName: string, policyValue: string): void;

  /**
   * Gets the browser policies.
   * This function can be called by a super administrator.
   *
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } appId - id of the bundle that need to get policies. It cannot be empty.
   * @returns { string } the browser policies returned by the getPolicies.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
    function getPoliciesSync(admin: Want, appId: string): string;

    /**
     * Sets the managed browser policy.
     *
     * @permission ohos.permission.ENTERPRISE_SET_BROWSER_POLICY
     * @param { Want } admin - admin indicates the enterprise admin extension ability information.
     *                         The admin must have the corresponding permission.
     * @param { string } bundleName - bundleName indicates the bundle name of the browser.
     * @param { string } policyName - policyName indicates the browser policy name that need to set.
     * @param { string } policyValue - policyValue indicates the browser policy value that need to set. It must be a
     *                                  correct JSON character string that can be converted into browser policies.
     * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
     * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     *                                 2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @stagemodelonly
     * @since 15
     */
    function setManagedBrowserPolicy(admin: Want, bundleName: string, policyName: string, policyValue: string): void;

    /**
     * Gets the managed browser policy.
     *
     * @param { Want } admin - admin indicates the enterprise admin extension ability information.
     *                         The admin must have the corresponding permission.
     * @param { string } bundleName - bundleName indicates the bundle name of the browser.
     * @returns { ArrayBuffer } the browser policy returned by the getManagedBrowserPolicy.
     * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     *                                 2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @stagemodelonly
     * @since 15
     */
    function getManagedBrowserPolicy(admin: Want, bundleName: string): ArrayBuffer;

    /**
     * Gets the version of managed browser policy.
     *
     * @returns { string } return the version of managed browser policy.
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @stagemodelonly
     * @since 15
     */
    function getSelfManagedBrowserPolicyVersion(): string;

    /**
     * Gets the managed browser policy.
     *
     * @returns { ArrayBuffer } return the managed browser policy.
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @stagemodelonly
     * @since 15
     */
    function getSelfManagedBrowserPolicy(): ArrayBuffer;
}

export default browser;
