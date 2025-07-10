/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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

import type { AsyncCallback } from './@ohos.base';
import type wantConstant from './@ohos.app.ability.wantConstant';

/**
 * This module provides the capability to authorize URI.
 *
 * @namespace uriPermissionManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 10
 */
declare namespace uriPermissionManager {
  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { AsyncCallback<number> } callback - the callback of grantUriPermission.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 10
   */
  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { AsyncCallback<number> } callback - the callback of grantUriPermission.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 19
   */
  function grantUriPermission(
    uri: string,
    flag: wantConstant.Flags,
    targetBundleName: string,
    callback: AsyncCallback<number>
  ): void;

  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @returns { Promise<number> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 10
   */
  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @returns { Promise<number> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 19
   */
  function grantUriPermission(uri: string, flag: wantConstant.Flags, targetBundleName: string): Promise<number>;

  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { number } appCloneIndex - Indicates the clone index of target application.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @throws { BusinessError } 16000081 - Failed to obtain the target application information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 14
   */
  /**
   * Grant URI to another application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { wantConstant.Flags } flag - wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION,
   *                                      wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION or
   *                                      wantConstant.Flags.FLAG_AUTH_PERSISTABLE_URI_PERMISSION.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { number } appCloneIndex - Indicates the clone index of target application.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000058 - Invalid URI flag.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000060 - A sandbox application cannot grant URI permission.
   * @throws { BusinessError } 16000081 - Failed to obtain the target application information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use.
   * @since 19
   */
  function grantUriPermission(uri: string, flag: wantConstant.Flags, targetBundleName: string, appCloneIndex: number): Promise<void>;

  /**
   * Revoke URI from one application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { AsyncCallback<number> } callback - the callback of revokeUriPermission.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 10
   */
  /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { AsyncCallback<number> } callback - the callback of revokeUriPermission.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 12
   */
  /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { AsyncCallback<number> } callback - the callback of revokeUriPermission.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 19
   */
  function revokeUriPermission(uri: string, targetBundleName: string, callback: AsyncCallback<number>): void;

  /**
   * Revoke URI from one application
   *
   * @permission ohos.permission.PROXY_AUTHORIZATION_URI
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @returns { Promise<number> } - the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 10
   */
    /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @returns { Promise<number> } - the promise returned by the function.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 12
   */
  /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @returns { Promise<number> } - the promise returned by the function.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 19
   */
  function revokeUriPermission(uri: string, targetBundleName: string): Promise<number>;

  /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { number } appCloneIndex - Indicates the clone index of target application.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000081 - Failed to obtain the target application information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 14
   */
  /**
   * Revoke URI from one application
   *
   * @param { string } uri - File URI.
   * @param { string } targetBundleName - Indicates the bundle name of authorization target.
   * @param { number } appCloneIndex - Indicates the clone index of target application.
   * @returns { Promise<void> } - the promise returned by the function.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 
   *                                 1. Mandatory parameters are left unspecified;
   *                                 2. Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 16000050 - Internal error.
   * @throws { BusinessError } 16000059 - Invalid URI type.
   * @throws { BusinessError } 16000081 - Failed to obtain the target application information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi hide this for inner system use
   * @since 19
   */
    function revokeUriPermission(uri: string, targetBundleName: string, appCloneIndex: number): Promise<void>;
}

export default uriPermissionManager;