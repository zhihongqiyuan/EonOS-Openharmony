/*
 * Copyright (C) 2022-2024 Huawei Device Co., Ltd.
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
 * @kit CoreFileKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type wantConstant from './@ohos.ability.wantConstant';

/**
 * Provides fileshare APIS
 *
 * @namespace fileShare
 * @syscap SystemCapability.FileManagement.AppFileService
 * @since 9
 */
declare namespace fileShare {
  /**
   * Enumerates the uri operate mode types.
   *
   * @enum { number } OperationMode
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  export enum OperationMode {
    /**
     * Indicates read permissions.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    READ_MODE = 0b1,

    /**
     * Indicates write permissions.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    WRITE_MODE = 0b10,

    /**
     * Indicates creating permissions.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 20
     */
    CREATE_MODE = 0b100,

    /**
     * Indicates deleting permissions.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 20
     */
    DELETE_MODE = 0b1000,

    /**
     * Indicates renaming permissions.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 20
     */
    RENAME_MODE = 0b10000,
  }

  /**
   * Enumerates the error code of the permission policy for the URI operation.
   *
   * @enum { number } PolicyErrorCode
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  export enum PolicyErrorCode {
    /**
     * Indicates that the policy is not allowed to be persisted.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    PERSISTENCE_FORBIDDEN = 1,

    /**
     * Indicates that the mode of this policy is invalid.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    INVALID_MODE = 2,

    /**
     * Indicates that the path of this policy is invalid.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    INVALID_PATH = 3,

    /**
     * Indicates that the permission is not persistent.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 12
     */
    PERMISSION_NOT_PERSISTED = 4,
  }

  /**
   * Failed policy result on URI.
   *
   * @typedef { object }
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  export type PolicyErrorResult = {
    /**
     * Indicates the failed uri of the policy information.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    uri: string;

    /**
     * Indicates the error code of the failure in the policy information.
     *
     * @type { PolicyErrorCode }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    code: PolicyErrorCode;

    /**
     * Indicates the reason of the failure in the policy information.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    message: string;
  };

  /**
   * Policy information to manager permissions on a URI.
   *
   * @interface PolicyInfo
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  export interface PolicyInfo {
    /**
     * Indicates the uri of the policy information.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    uri: string;

    /**
     * Indicates the mode of operation for the URI, example { OperationMode.READ_MODE } or { OperationMode.READ_MODE | OperationMode.WRITE_MODE }
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 11
     */
    operationMode: number;
  }

  /**
   * Policy information to manager permissions on a path.
   *
   * @interface PathPolicyInfo
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 15
   */
  export interface PathPolicyInfo {
    /**
     * Indicates the path of the policy information.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 15
     */
    path: string;
 
    /**
     * Indicates the mode of operation for the path.
     *
     * @type { OperationMode }
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 15
     */
    operationMode: OperationMode;
  }

  /**
   * Indicates the policy type of the path.
   *
   * @enum { number } policyType
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 15
   */
  export enum PolicyType {
    /**
     * Indicates that the policy is temporary.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 15
     */
    TEMPORARY_TYPE = 0,
 
    /**
     * Indicates that the policy is persistent.
     *
     * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
     * @since 15
     */
    PERSISTENT_TYPE = 1,
  }

  /**
   * Provides grant uri permission for app
   *
   * @permission ohos.permission.WRITE_MEDIA
   * @param { string } uri uri
   * @param { string } bundleName bundleName
   * @param { wantConstant.Flags } flag wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION or wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION
   * @param { AsyncCallback<void> } callback
   * @throws { BusinessError } 201 - Permission verification failed
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 14300001 - IPC error
   * @syscap SystemCapability.FileManagement.AppFileService
   * @systemapi
   * @since 9
   */
  function grantUriPermission(
    uri: string,
    bundleName: string,
    flag: wantConstant.Flags,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Provides grant uri permission for app
   *
   * @permission ohos.permission.WRITE_MEDIA
   * @param { string } uri uri
   * @param { string } bundleName bundleName
   * @param { wantConstant.Flags } flag wantConstant.Flags.FLAG_AUTH_READ_URI_PERMISSION or wantConstant.Flags.FLAG_AUTH_WRITE_URI_PERMISSION
   * @returns { Promise<void> } no callback return Promise otherwise return void
   * @throws { BusinessError } 201 - Permission verification failed
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 401 - The input parameter is invalid.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 14300001 - IPC error
   * @syscap SystemCapability.FileManagement.AppFileService
   * @systemapi
   * @since 9
   */
  function grantUriPermission(uri: string, bundleName: string, flag: wantConstant.Flags): Promise<void>;

  /**
   * Grant URI permissions for an application.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER
   * @param { Array<PolicyInfo> } policies - Policy information for the user to grant permissions on URIs.
   * @param { string } targetBundleName - Name of the target bundle to authorize.
   * @param { number } appCloneIndex - Clone index of the target application.
   * @returns { Promise<void> } Returns void.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900001 - Operation not permitted.
   * @throws { BusinessError } 13900011 - Out of memory.
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @systemapi
   * @since 20
   */
  function grantUriPermission(policies: Array<PolicyInfo>, targetBundleName: string, appCloneIndex: number): Promise<void>;

  /**
   * Set persistence permissions for the URI
   *
   * @permission ohos.permission.FILE_ACCESS_PERSIST
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900001 - Operation not permitted.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  function persistPermission(policies: Array<PolicyInfo>): Promise<void>;

  /**
   * Revoke persistence permissions for the URI
   *
   * @permission ohos.permission.FILE_ACCESS_PERSIST
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900001 - Operation not permitted.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  function revokePermission(policies: Array<PolicyInfo>): Promise<void>;

  /**
   * Enable the URI that have been permanently authorized
   *
   * @permission ohos.permission.FILE_ACCESS_PERSIST
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900001 - Operation not permitted.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  function activatePermission(policies: Array<PolicyInfo>): Promise<void>;

  /**
   * Stop the authorized URI that has been enabled
   *
   * @permission ohos.permission.FILE_ACCESS_PERSIST
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900001 - Operation not permitted.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 11
   */
  function deactivatePermission(policies: Array<PolicyInfo>): Promise<void>;

  /**
   * Check persistent permissions for the URI.
   * 
   * @permission ohos.permission.FILE_ACCESS_PERSIST
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<Array<boolean>> } Returns the persistent state of uri permissions.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 12
   */
  /**
   * Check persistent permissions for the URI.
   * 
   * @param { Array<PolicyInfo> } policies - Policy information to grant permission on URIs.
   * @returns { Promise<Array<boolean>> } Returns the persistent state of uri permissions.
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @throws { BusinessError } 13900042 - Out of memory
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @since 17
   */
  function checkPersistentPermission(policies: Array<PolicyInfo>): Promise<Array<boolean>>;

  /**
   * Check permissions for the path.
   * 
   * @permission ohos.permission.CHECK_SANDBOX_POLICY
   * @param { number } tokenID - Token ID of the application.
   * @param { Array<PathPolicyInfo> } policies - Policy information to check on paths.
   * @param { PolicyType } policyType - Persistent or temporary type.
   * @returns { Promise<Array<boolean>> } Returns the permission state of paths.
   * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
   * @throws { BusinessError } 202 - The caller is not a system application
   * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameter types.
   * @throws { BusinessError } 801 - Capability not supported.
   * @syscap SystemCapability.FileManagement.AppFileService.FolderAuthorization
   * @systemapi
   * @since 15
   */
  function checkPathPermission(tokenID: number, policies: Array<PathPolicyInfo>, policyType: PolicyType): Promise<Array<boolean>>;
}

export default fileShare;
