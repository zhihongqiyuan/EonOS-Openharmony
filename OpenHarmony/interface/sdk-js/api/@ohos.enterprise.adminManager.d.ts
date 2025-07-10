/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
import common from '@ohos.app.ability.common';

/**
 * This module provides the capability to manage the administrator of the enterprise devices.
 *
 * @namespace adminManager
 * @syscap SystemCapability.Customization.EnterpriseDeviceManager
 * @since 9
 */
declare namespace adminManager {
  /**
   * Provides the enterprise information.
   *
   * @typedef EnterpriseInfo
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @since 9
   */
  export interface EnterpriseInfo {
    /**
     * The name of enterprise.
     *
     * @type { string }
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 9
     */
    name: string;

    /**
     * The description of enterprise.
     *
     * @type { string }
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 9
     */
    description: string;
  }

  /**
   * Enum for type of administrator.
   *
   * @enum { number }
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @since 15
   */
  export enum AdminType {
    /**
     * The value of normal administrator.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 9
     */
    ADMIN_TYPE_NORMAL = 0x00,

    /**
     * The value of super administrator.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 9
     */
    ADMIN_TYPE_SUPER = 0x01,

    /**
     * The value of administrator used in BYOD device.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 15
     */
    ADMIN_TYPE_BYOD = 0x02
  }

  /**
   * Enum for managed event
   *
   * @enum { number }
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @since 12
   */
  export enum ManagedEvent {
    /**
     * The event of bundle added.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 12
     */
    MANAGED_EVENT_BUNDLE_ADDED = 0,

    /**
     * The event of bundle removed.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 12
     */
    MANAGED_EVENT_BUNDLE_REMOVED = 1,

    /**
     * The event of app start.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 12
     */
    MANAGED_EVENT_APP_START = 2,

    /**
     * The event of app stop.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 12
     */
    MANAGED_EVENT_APP_STOP = 3,

    /**
     * The event of system update.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 12
     */
    MANAGED_EVENT_SYSTEM_UPDATE = 4,

    /**
     * Event indicating that a system account is added.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 18
     */
    MANAGED_EVENT_ACCOUNT_ADDED = 5,

    /**
     * Event indicating that a system account is switched.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 18
     */
    MANAGED_EVENT_ACCOUNT_SWITCHED = 6,

    /**
     * Event indicating that a system account is removed.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @since 18
     */
    MANAGED_EVENT_ACCOUNT_REMOVED = 7
  }

  /**
   * Enum for admin running mode.
   *
   * @enum { number }
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @since 19
   */
  export enum RunningMode {
    /**
     * The default mode.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 19
     */
    DEFAULT = 0,

    /**
     * Multi-user running mode.
     *
     * @syscap SystemCapability.Customization.EnterpriseDeviceManager
     * @systemapi
     * @since 19
     */
    MULTI_USER = 1,
  }

  /**
   * Enables the given ability as a administrator of the device.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { EnterpriseInfo } enterpriseInfo - enterpriseInfo indicates the enterprise information of the calling application.
   * @param { AdminType } type - type indicates the type of administrator to set.
   * @param { AsyncCallback<void> } callback - the callback of enableAdmin.
   * @throws { BusinessError } 9200003 - The administrator ability component is invalid.
   * @throws { BusinessError } 9200004 - Failed to activate the administrator application of the device.
   * @throws { BusinessError } 9200007 - The system ability works abnormally.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function enableAdmin(admin: Want, enterpriseInfo: EnterpriseInfo, type: AdminType, callback: AsyncCallback<void>): void;

  /**
   * Enables the given ability as a administrator of the device.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { EnterpriseInfo } enterpriseInfo - enterpriseInfo indicates the enterprise information of the calling application.
   * @param { AdminType } type - type indicates the type of administrator to set.
   * @param { number } userId - userId indicates the user ID.
   * @param { AsyncCallback<void> } callback - the callback of enableAdmin.
   * @throws { BusinessError } 9200003 - The administrator ability component is invalid.
   * @throws { BusinessError } 9200004 - Failed to activate the administrator application of the device.
   * @throws { BusinessError } 9200007 - The system ability works abnormally.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function enableAdmin(admin: Want, enterpriseInfo: EnterpriseInfo, type: AdminType, userId: number, callback: AsyncCallback<void>): void;

  /**
   * Enables the given ability as a administrator of the device.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { EnterpriseInfo } enterpriseInfo - enterpriseInfo indicates the enterprise information of the calling application.
   * @param { AdminType } type - type indicates the type of administrator to set.
   * @param { number } [userId] - userId indicates the user ID or do not pass user ID.
   * @returns { Promise<void> } the promise returned by the enableAdmin.
   * @throws { BusinessError } 9200003 - The administrator ability component is invalid.
   * @throws { BusinessError } 9200004 - Failed to activate the administrator application of the device.
   * @throws { BusinessError } 9200007 - The system ability works abnormally.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function enableAdmin(admin: Want, enterpriseInfo: EnterpriseInfo, type: AdminType, userId?: number): Promise<void>;

  /**
   * Disables a current normal administrator ability.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { AsyncCallback<void> } callback - the callback of disableAdmin.
   * @throws { BusinessError } 9200005 - Failed to deactivate the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function disableAdmin(admin: Want, callback: AsyncCallback<void>): void;

  /**
   * Disables a current normal administrator ability.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { number } userId - userId indicates the user ID.
   * @param { AsyncCallback<void> } callback - the callback of disableAdmin.
   * @throws { BusinessError } 9200005 - Failed to deactivate the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function disableAdmin(admin: Want, userId: number, callback: AsyncCallback<void>): void;

  /**
   * Disables a current administrator ability.
   * Only apps with the ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN permission or the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the enterprise admin extension ability information.
   *                         The admin must have the corresponding permission.
   * @param { number } [userId] - userId indicates the user ID or do not pass user ID.
   * @returns { Promise<void> } the promise returned by the disableAdmin.
   * @throws { BusinessError } 9200005 - Failed to deactivate the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function disableAdmin(admin: Want, userId?: number): Promise<void>;

  /**
   * Disables a current super administrator ability.
   * Only the administrator app or apps with the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { String } bundleName - bundleName indicates the administrator bundle information.
   * @param { AsyncCallback<void> } callback - the callback of disableSuperAdmin.
   * @throws { BusinessError } 9200005 - Failed to deactivate the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function disableSuperAdmin(bundleName: String, callback: AsyncCallback<void>): void;

  /**
   * Disables a current super administrator ability.
   * Only the administrator app or apps with the shell uid can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { String } bundleName - bundleName indicates the administrator bundle information.
   * @returns { Promise<void> } the promise returned by the disableSuperAdmin.
   * @throws { BusinessError } 9200005 - Failed to deactivate the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function disableSuperAdmin(bundleName: String): Promise<void>;

  /**
   * Get whether the ability is enabled as device administrator.
   *
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { AsyncCallback<boolean> } callback - callback contained true if the administrator is enabled.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function isAdminEnabled(admin: Want, callback: AsyncCallback<boolean>): void;

  /**
   * Get whether the ability is enabled as device administrator.
   *
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { number } userId - userId indicates the user ID.
   * @param { AsyncCallback<boolean> } callback - callback contained true if the administrator is enabled.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function isAdminEnabled(admin: Want, userId: number, callback: AsyncCallback<boolean>): void;

  /**
   * Get whether the ability is enabled as device administrator.
   *
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { number } [userId] - userId indicates the user ID or do not pass user ID.
   * @returns { Promise<boolean> } promise contained true if the administrator is enabled.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function isAdminEnabled(admin: Want, userId?: number): Promise<boolean>;

  /**
   * Get information of the administrator's enterprise.
   *
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { AsyncCallback<EnterpriseInfo> } callback - callback contained the enterprise info of administrator.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function getEnterpriseInfo(admin: Want, callback: AsyncCallback<EnterpriseInfo>): void;

  /**
   * Get information of the administrator's enterprise.
   *
   * @param { Want } admin - admin indicates the administrator ability information.
   * @returns { Promise<EnterpriseInfo> } promise contained the enterprise info of administrator.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function getEnterpriseInfo(admin: Want): Promise<EnterpriseInfo>;

  /**
   * Set the information of the administrator's enterprise.
   * Only the administrator app can call this method.
   *
   * @permission ohos.permission.SET_ENTERPRISE_INFO
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { EnterpriseInfo } enterpriseInfo - enterpriseInfo indicates the enterprise information of the calling application.
   * @param { AsyncCallback<void> } callback - the callback of setEnterpriseInfo.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function setEnterpriseInfo(admin: Want, enterpriseInfo: EnterpriseInfo, callback: AsyncCallback<void>): void;

  /**
   * Set the information of the administrator's enterprise.
   * Only the administrator app can call this method.
   *
   * @permission ohos.permission.SET_ENTERPRISE_INFO
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { EnterpriseInfo } enterpriseInfo - enterpriseInfo indicates the enterprise information of the calling application.
   * @returns { Promise<void> } the promise returned by the setEnterpriseInfo.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function setEnterpriseInfo(admin: Want, enterpriseInfo: EnterpriseInfo): Promise<void>;

  /**
   * Set the running mode for admin.
   * Only the administrator app can call this method.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { RuningMode } mode - indicates the running mode for admin. 
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 19
   */
  function setAdminRunningMode(admin: Want, mode: RunningMode): void;

  /**
   * Get whether the ability is enabled as super device administrator.
   *
   * @param { String } bundleName - bundleName indicates the administrator bundle information.
   * @param { AsyncCallback<boolean> } callback - callback contained true if the administrator is super administrator.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function isSuperAdmin(bundleName: String, callback: AsyncCallback<boolean>): void;

  /**
   * Get whether the ability is enabled as super device administrator.
   *
   * @param { String } bundleName - bundleName indicates the administrator bundle information.
   * @returns { Promise<boolean> } promise contained true if the administrator is super administrator.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function isSuperAdmin(bundleName: String): Promise<boolean>;

  /**
   * Subscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @param { AsyncCallback<void> } callback - the callback of subscribeManagedEvent.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function subscribeManagedEvent(admin: Want, managedEvents: Array<ManagedEvent>, callback: AsyncCallback<void>): void;

  /**
   * Subscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @returns { Promise<void> } the promise returned by the subscribeManagedEvent.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function subscribeManagedEvent(admin: Want, managedEvents: Array<ManagedEvent>): Promise<void>;

  /**
   * Unsubscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @param { AsyncCallback<void> } callback - the callback of unsubscribeManagedEvent.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function unsubscribeManagedEvent(admin: Want, managedEvents: Array<ManagedEvent>, callback: AsyncCallback<void>): void;

  /**
   * Unsubscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @returns { Promise<void> } the promise returned by the unsubscribeManagedEvent.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 9
   */
  function unsubscribeManagedEvent(admin: Want, managedEvents: Array<ManagedEvent>): Promise<void>;

  /**
   * Administrator authorize permissions to other applications.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { string } bundleName - bundleName indicates the administrator bundle information.
   * @param { AsyncCallback<void> } callback - the callback of authorizeAdmin.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 9200009 - Failed to grant the permission to the application.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 10
   */
  function authorizeAdmin(admin: Want, bundleName: string, callback: AsyncCallback<void>): void;

  /**
   * Administrator authorize permissions to other applications.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { string } bundleName - bundleName indicates the administrator bundle information.
   * @returns { Promise<void> } the promise returned by the authorizeAdmin.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 9200009 - Failed to grant the permission to the application.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 10
   */
  function authorizeAdmin(admin: Want, bundleName: string): Promise<void>;

  /**
   * Get the super administrator of device.
   *
   * @returns { Promise<Want> } promise contained the want indicates the super administrator of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  function getSuperAdmin(): Promise<Want>;

  /**
   * Subscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function subscribeManagedEventSync(admin: Want, managedEvents: Array<ManagedEvent>): void;

  /**
   * Unsubscribes the managed event of admin.
   *
   * @permission ohos.permission.ENTERPRISE_SUBSCRIBE_MANAGED_EVENT
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { Array<ManagedEvent> } managedEvents - managedEvents indicates the managed events to subscribe.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200008 - The specified system event is invalid.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 12
   */
  function unsubscribeManagedEventSync(admin: Want, managedEvents: Array<ManagedEvent>): void;

  /**
   * Administrator delegates access to policies to another application.
   *
   * @permission ohos.permission.ENTERPRISE_MANAGE_DELEGATED_POLICY
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { string } bundleName - bundleName indicates the bundle name of the delegated application.
   * @param { Array<string> } policies - policies indicates the policies accessible to the delegated application.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 9200009 - Failed to grant the permission to the application.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 14
   */
  function setDelegatedPolicies(admin: Want, bundleName: string, policies: Array<string>): void;

  /**
   * Administrator gets the list of delegation policies for the application.
   *
   * @permission ohos.permission.ENTERPRISE_MANAGE_DELEGATED_POLICY
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { string } bundleName - bundleName indicates the bundle name of the delegated application.
   * @returns { Array<string> } the policies accessible to the delegated application.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 14
   */
  function getDelegatedPolicies(admin: Want, bundleName: string): Array<string>;

  /**
   * Administrator gets the delegated applications which access to the policy.
   *
   * @permission ohos.permission.ENTERPRISE_MANAGE_DELEGATED_POLICY
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { string } policy - policy indicates the policy that delegated to other applications.
   * @returns { Array<string> } the bundle names of the delegated application that access to the policy.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200002 - The administrator application does not have permission to manage the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 14
   */
  function getDelegatedBundleNames(admin: Want, policy: string): Array<string>;

  /**
   * Starts an ability of admin provision application.
   *
   * @permission ohos.permission.START_PROVISIONING_MESSAGE
   * @param { Want } admin - admin indicates the administrator ability information.
   * @param { AdminType } type - type indicates the type of administrator to set.
   * @param { common.Context } context - context indicates the context of application.
   * @param { Record<string, string> } parameters - the parameters indicates the custom parameters of start an administrator provision.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @stagemodelonly
   * @since 15
   */
  function startAdminProvision(admin: Want, type: AdminType, context: common.Context, parameters: Record<string, string>): void;

  /**
   * Gets administrators of device.
   *
   * @returns { Promise<Array<Want>> } returns the want list indicates the administrators of the device.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 15
   */
  function getAdmins(): Promise<Array<Want>>;

  /**
   * replace old admin with new admin.
   *
   * @permission ohos.permission.MANAGE_ENTERPRISE_DEVICE_ADMIN
   * @param { Want } oldAdmin - oldAdmin indicates the old administrator ability information.
   * @param { Want } newAdmin - newAdmin indicates the new administrator ability information.
   * @param { boolean } isKeepPolicy - true indicates whether keep admin policy.
   * @throws { BusinessError } 9200001 - The application is not an administrator application of the device.
   * @throws { BusinessError } 9200003 - The administrator ability component is invalid.
   * @throws { BusinessError } 9200011 - Failed to replace the administrator application of the device.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Customization.EnterpriseDeviceManager
   * @systemapi
   * @stagemodelonly
   * @since 18
   */
  function replaceSuperAdmin(oldAdmin: Want, newAdmin: Want, isKeepPolicy: boolean): void;
}

export default adminManager;
