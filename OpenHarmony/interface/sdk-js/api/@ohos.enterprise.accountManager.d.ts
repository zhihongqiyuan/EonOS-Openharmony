/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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
import type osAccount from './@ohos.account.osAccount';

/**
 * This module provides the capability to manage the accounts of the enterprise devices.
 *
 * @namespace accountManager
 * @syscap SystemCapability.Customization.EnterpriseDeviceManager
 * @since 10
 */
declare namespace accountManager {
  /**
   * The policy of domain account
   * 
   * @interface DomainAccountPolicy
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 19
   */
  interface DomainAccountPolicy {
    /**
     * The validity period of authentication.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
	 * @stagemodelonly
     * @since 19
     */
    authenticationValidityPeriod?: number;

    /**
     * The validity period of password.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
	 * @stagemodelonly
     * @since 19
     */
    passwordValidityPeriod?: number;

    /**
     * The password expiration notification.
     * 
     * @type { ?number }
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
	 * @stagemodelonly
     * @since 19
     */
    passwordExpirationNotification?: number;
  }

  /**
   * Disallow the user of device add local account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { boolean } disallow - true if the user is not allowed to add local account.
   * @param { AsyncCallback<void> } callback - the callback of disallowAddLocalAccount.
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
  function disallowAddLocalAccount(admin: Want, disallow: boolean, callback: AsyncCallback<void>): void;

  /**
   * Disallow the user of device add local account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { boolean } disallow - true if the user is not allowed to add local account.
   * @returns { Promise<void> } the promise returned by the disallowAddLocalAccount.
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
  function disallowAddLocalAccount(admin: Want, disallow: boolean): Promise<void>;

  /**
   * Disallows the user to add an OS account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { number } userId - indicates the user ID. It cannot be the ID of a user that does not exist.
   * @param { boolean } disallow - true if the specific user is not allowed to add an OS account.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function disallowAddOsAccountByUser(admin: Want, userId: number, disallow: boolean): void;

  /**
   * Queries whether the user is disallowed to add an OS account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         If the admin is not empty, it must have the corresponding permission.
   * @param { number } userId - indicates the user ID. It cannot be the ID of a user that does not exist.
   * @returns { boolean } true if the specific user is not allowed to add an OS account.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function isAddOsAccountByUserDisallowed(admin: Want, userId: number): boolean;

  /**
   * Adds an OS account using the name and account type.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } name - the OS account name. It cannot be empty.
   * @param { osAccount.OsAccountType } type - the OS account type. It can only be one of specified types.
   * @returns { osAccount.OsAccountInfo } information about the OS account added.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 9201003 - Failed to add an OS account.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  function addOsAccount(admin: Want, name: string, type: osAccount.OsAccountType): osAccount.OsAccountInfo;

  /**
   * Disallows the account or all accounts to add an OS account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { boolean } disallow - true if the specific account or all accounts are not allowed to add an OS account.
   * @param { number } [accountId] - indicates the account ID. It cannot be the ID of an account that does not exist.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function disallowOsAccountAddition(admin: Want, disallow: boolean, accountId?: number): void;

  /**
   * Queries whether the account or all accounts is disallowed to add an OS account.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         If the admin is not empty, it must have the corresponding permission.
   * @param { number } [accountId] - indicates the account ID. It cannot be the ID of an account that does not exist.
   * @returns { boolean } true if the specific account or all accounts are not allowed to add an OS account.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function isOsAccountAdditionDisallowed(admin: Want, accountId?: number): boolean;

  /**
   * Adds an OS account using the name and account type.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { string } name - the OS account name. It cannot be empty.
   * @param { osAccount.OsAccountType } type - the OS account type. It can only be one of correct types.
   * @returns { Promise<osAccount.OsAccountInfo> } information about the OS account added.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 9201003 - Failed to add an OS account.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function addOsAccountAsync(admin: Want, name: string, type: osAccount.OsAccountType): Promise<osAccount.OsAccountInfo>;

  /**
   * Sets domain account policy.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { osAccount.DomainAccountInfo } domainAccountInfo - the infomation of domain account.
   * @param { DomainAccountPolicy } policy - policy indicates the domain account policy.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 19
   */
  function setDomainAccountPolicy(admin: Want, domainAccountInfo: osAccount.DomainAccountInfo, policy: DomainAccountPolicy): void;

  /**
   * Gets domain account policy.
   * This function can be called by a super administrator.
   *
   * @permission ohos.permission.ENTERPRISE_SET_ACCOUNT_POLICY
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { osAccount.DomainAccountInfo } domainAccountInfo - the infomation of domain account.
   * @returns { DomainAccountPolicy } the domain account policy.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 19
   */
  function getDomainAccountPolicy(admin: Want, domainAccountInfo: osAccount.DomainAccountInfo): DomainAccountPolicy;
}

export default accountManager;
