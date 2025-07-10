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
 * @kit AbilityKit
 */

import { AsyncCallback } from './@ohos.base';
import { ElementName } from './bundleManager/ElementName';
import { RemoteAbilityInfo as _RemoteAbilityInfo } from './bundleManager/RemoteAbilityInfo';

/**
 * DistributedBundle manager.
 *
 * @namespace distributedBundleManager
 * @syscap SystemCapability.BundleManager.DistributedBundleFramework
 * @systemapi
 * @since 9
 */
declare namespace distributedBundleManager {
  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName - Indicates the elementName.
   * @param { AsyncCallback<RemoteAbilityInfo> } callback - The callback of getting the ability info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementName: ElementName, callback: AsyncCallback<RemoteAbilityInfo>): void;

  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName - Indicates the elementName.
   * @returns { Promise<RemoteAbilityInfo> } Returns the ability info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementName: ElementName): Promise<RemoteAbilityInfo>;

  /**
   * Obtains information about the abilities info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames - Indicates the elementNames, Maximum array length ten.
   * @param { AsyncCallback<Array<RemoteAbilityInfo>> } callback - the callback of getting the abilities info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementNames: Array<ElementName>,
    callback: AsyncCallback<Array<RemoteAbilityInfo>>): void;

  /**
   * Obtains information about the abilities info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames - Indicates the elementNames, Maximum array length ten.
   * @returns { Promise<Array<RemoteAbilityInfo>> } The result of getting the abilities info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementNames: Array<ElementName>): Promise<Array<RemoteAbilityInfo>>;

  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName - Indicates the elementName.
   * @param { string } locale - Indicates the locale info
   * @param { AsyncCallback<RemoteAbilityInfo> } callback - The callback of getting the ability info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementName: ElementName,
    locale: string, callback: AsyncCallback<RemoteAbilityInfo>): void;

  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName - Indicates the elementName.
   * @param { string } locale - Indicates the locale info
   * @returns { Promise<RemoteAbilityInfo> } The result of getting the ability info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementName: ElementName, locale: string): Promise<RemoteAbilityInfo>;

  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames - Indicates the elementNames, Maximum array length ten.
   * @param { string } locale - Indicates the locale info
   * @param { AsyncCallback<Array<RemoteAbilityInfo>> } callback - Returns the abilities info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementNames: Array<ElementName>,
    locale: string, callback: AsyncCallback<Array<RemoteAbilityInfo>>): void;

  /**
   * Obtains information about the abilities info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames - Indicates the elementNames, Maximum array length ten.
   * @param { string } locale - Indicates the locale info
   * @returns { Promise<Array<RemoteAbilityInfo>> } Returns the abilities info of the remote device.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 17700001 - The specified bundle name is not found.
   * @throws { BusinessError } 17700003 - The specified ability name is not found.
   * @throws { BusinessError } 17700007 - The specified device ID is not found.
   * @throws { BusinessError } 17700027 - The distributed service is not running.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  function getRemoteAbilityInfo(elementNames: Array<ElementName>, locale: string): Promise<Array<RemoteAbilityInfo>>;

  /**
   * Contains basic remote ability information.
   *
   * @typedef { _RemoteAbilityInfo }
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 9
   */
  export type RemoteAbilityInfo = _RemoteAbilityInfo;
}

export default distributedBundleManager;
