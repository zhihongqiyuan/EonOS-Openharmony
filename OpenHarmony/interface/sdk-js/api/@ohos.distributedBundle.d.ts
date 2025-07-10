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
import { ElementName } from './bundle/elementName';
import { RemoteAbilityInfo } from './bundle/remoteAbilityInfo';

/**
 * distributedBundle.
 *
 * @namespace distributedBundle
 * @syscap SystemCapability.BundleManager.DistributedBundleFramework
 * @systemapi Hide this for inner system use
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.bundle.distributedBundleManager
 */
declare namespace distributedBundle {
  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName Indicates the elementName.
   * @param { AsyncCallback<RemoteAbilityInfo> } callback
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.distributedBundleManager#getRemoteAbilityInfo
   */
  function getRemoteAbilityInfo(elementName: ElementName, callback: AsyncCallback<RemoteAbilityInfo>): void;

  /**
   * Obtains information about the ability info of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { ElementName } elementName Indicates the elementName.
   * @returns { Promise<RemoteAbilityInfo> } Returns the ability info of the remote device.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.distributedBundleManager#getRemoteAbilityInfo
   */
  function getRemoteAbilityInfo(elementName: ElementName): Promise<RemoteAbilityInfo>;

  /**
   * Obtains information about the ability infos of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames Indicates the elementNames, Maximum array length ten.
   * @param { AsyncCallback<Array<RemoteAbilityInfo>> } callback
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.distributedBundleManager#getRemoteAbilityInfo
   */
  function getRemoteAbilityInfos(elementNames: Array<ElementName>,
    callback: AsyncCallback<Array<RemoteAbilityInfo>>): void;

  /**
   * Obtains information about the ability infos of the remote device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Array<ElementName> } elementNames Indicates the elementNames, Maximum array length ten.
   * @returns { Promise<Array<RemoteAbilityInfo>> } Returns the ability infos of the remote device.
   * @syscap SystemCapability.BundleManager.DistributedBundleFramework
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.bundle.distributedBundleManager#getRemoteAbilityInfo
   */
  function getRemoteAbilityInfos(elementNames: Array<ElementName>): Promise<Array<RemoteAbilityInfo>>;
}
export default distributedBundle;
