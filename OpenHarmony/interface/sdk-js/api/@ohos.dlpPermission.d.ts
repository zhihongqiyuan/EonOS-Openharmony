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
 * @kit DataProtectionKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type common from './@ohos.app.ability.common';
import type Want from './@ohos.app.ability.Want';

/**
 * Provides the capability to access the data loss prevention (DLP) files.
 *
 * @namespace dlpPermission
 * @syscap SystemCapability.Security.DataLossPrevention
 * @since 10
 */
declare namespace dlpPermission {
  /**
   * Enumerates the types of actions that can be performed on a DLP file.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  export enum ActionFlagType {
    /**
     * View a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_VIEW = 0x00000001,

    /**
     * Save a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_SAVE = 0x00000002,

    /**
     * Save a DLP file as another file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_SAVE_AS = 0x00000004,

    /**
     * Edit a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_EDIT = 0x00000008,

    /**
     * Take a screenshot of a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_SCREEN_CAPTURE = 0x00000010,

    /**
     * Share the screen, on which a DLP file is opened.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_SCREEN_SHARE = 0x00000020,

    /**
     * Record the screen, on which a DLP file is opened.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_SCREEN_RECORD = 0x00000040,

    /**
     * Copy in the editor, on which a DLP file is opened.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_COPY = 0x00000080,

    /**
     * Print a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_PRINT = 0x00000100,

    /**
     * Export a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_EXPORT = 0x00000200,

    /**
     * Change the permissions for a DLP file.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    ACTION_PERMISSION_CHANGE = 0x00000400
  }

  /**
   * Enumerates the access permissions for a DLP file.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  export enum DLPFileAccess {
    /**
     * No permission.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    NO_PERMISSION = 0,

    /**
     * Read-only.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    READ_ONLY = 1,

    /**
     * Edit.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    CONTENT_EDIT = 2,

    /**
     * Full control.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    FULL_CONTROL = 3
  }

  /**
   * Represents the permission info of a DLP file.
   *
   * @interface DLPPermissionInfo
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  export interface DLPPermissionInfo {
    /**
     * Access permission for the DLP file.
     *
     * @type { DLPFileAccess }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    dlpFileAccess: DLPFileAccess;

    /**
     * Actions allowed for the DLP file. The value is a combination of flags in {@link ActionFlagType}.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    flags: number;
  }

  /**
   * Represents the accessed DLP file info.
   *
   * @interface AccessedDLPFileInfo
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  export interface AccessedDLPFileInfo {
    /**
     * URI of the DLP file.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    uri: string;

    /**
     * Time when the DLP file was last opened.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    lastOpenTime: number;
  }

  /**
   * Represents the retention sandbox info.
   *
   * @interface RetentionSandboxInfo
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  export interface RetentionSandboxInfo {
    /**
     * Application index of the DLP sandbox.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    appIndex: number;

    /**
     * Bundle name of the application.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    bundleName: string;

    /**
     * List of file URIs.
     *
     * @type { Array<string> }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 10
     */
    docUris: Array<string>;
  }

  /**
   * Checks whether a file is a DLP file. This method uses a promise to return the result.
   *
   * @param { number } fd - Indicates the file descriptor of the file to check.
   * @returns { Promise<boolean> } Returns {@code true} if {@link fd} is a DLP file; returns {@code false} otherwise.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function isDLPFile(fd: number): Promise<boolean>;

  /**
   * Checks whether a file is a DLP file. This method uses an asynchronous callback to return the result.
   *
   * @param { number } fd - Indicates the file descriptor of the file to check.
   * @param { AsyncCallback<boolean> } callback - Indicates the callback of isDLPFile.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function isDLPFile(fd: number, callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the permission info of this DLP file. This method uses a promise to return the result.
   *
   * @returns { Promise<DLPPermissionInfo> } Returns the {@link DLPPermissionInfo}.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100006 - No permission to call this API,
   *     which is available only for DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPPermissionInfo(): Promise<DLPPermissionInfo>;

  /**
   * Obtains the permission info of this DLP file. This method uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<DLPPermissionInfo> } callback - Indicates the callback of getDLPPermissionInfo.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100006 - No permission to call this API,
   *     which is available only for DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPPermissionInfo(callback: AsyncCallback<DLPPermissionInfo>): void;

  /**
   * Obtains the original file name from a DLP file name. This method removes the DLP file name extension from the DLP file name.
   *
   * @param { string } fileName - Indicates the DLP file name.
   * @returns { string } Returns the original file name obtained.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getOriginalFileName(fileName: string): string;

  /**
   * Obtains the DLP file name extension.
   *
   * @returns { string } Returns the DLP file name extension obtained.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPSuffix(): string;

  /**
   * Subscribes to the event reported when a DLP file is opened by current application.
   *
   * @param { 'openDLPFile' } type - Indicates the type of the event to subscribe to.
   *     The value of type must be openDLPFile.
   * @param { Callback<AccessedDLPFileInfo> } listener - Indicates the callback invoked when a DLP file is opened by current application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function on(type: 'openDLPFile', listener: Callback<AccessedDLPFileInfo>): void;

  /**
   * Unsubscribes from the event reported when a DLP file is opened by current application.
   *
   * @param { 'openDLPFile' } type - Indicates the type of the event to unsubscribe from.
   *     The value of type must be openDLPFile.
   * @param { Callback<AccessedDLPFileInfo> } listener - Indicates the callback invoked when a DLP file is opened by current application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function off(type: 'openDLPFile', listener?: Callback<AccessedDLPFileInfo>): void;

  /**
   * Checks whether current application is in the DLP sandbox. This method uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Returns {@code true} if current application is in a DLP sandbox; returns {@code false} otherwise.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function isInSandbox(): Promise<boolean>;

  /**
   * Checks whether current application is in the DLP sandbox. This method uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<boolean> } callback - Indicates the callback of isInSandbox.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function isInSandbox(callback: AsyncCallback<boolean>): void;

  /**
   * Obtains the file types supported by DLP. This method uses a promise to return the result.
   *
   * @returns { Promise<Array<string>> } Returns the list of file types supported.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPSupportedFileTypes(): Promise<Array<string>>;

  /**
   * Obtains the file types supported by DLP. This method uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<Array<string>> } callback - Indicates the callback of getDLPSupportedFileTypes.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPSupportedFileTypes(callback: AsyncCallback<Array<string>>): void;

  /**
   * Sets the retention status for the files specified by URI list. This method uses a promise to return the result.
   *
   * @param { Array<string> } docUris - Indicates the URIs of the files, for which the retention status is to set.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100006 - No permission to call this API,
   *     which is available only for DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function setRetentionState(docUris: Array<string>): Promise<void>;

  /**
   * Sets the retention status for the files specified by URI list. This method uses an asynchronous callback to return the result.
   *
   * @param { Array<string> } docUris - Indicates the URIs of the files, for which the retention status is to set.
   * @param { AsyncCallback<void> } callback - Indicates the callback of setRetentionState.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100006 - No permission to call this API,
   *     which is available only for DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function setRetentionState(docUris: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Cancels the retention status for the files specified by URI list. This method uses a promise to return the result.
   *
   * @param { Array<string> } docUris - Indicates the list of the file URIs.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function cancelRetentionState(docUris: Array<string>): Promise<void>;

  /**
   * Cancels the retention status for the files specified by URI list. This method uses an asynchronous callback to return the result.
   *
   * @param { Array<string> } docUris - Indicates the list of the file URIs.
   * @param { AsyncCallback<void> } callback - Indicates the callback of cancelRetentionState.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function cancelRetentionState(docUris: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Obtains information about the retained DLP sandboxes of an application. This method uses a promise to return the result.
   *
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @returns { Promise<Array<RetentionSandboxInfo>> } Returns a list of {@link RetentionSandboxInfo}.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getRetentionSandboxList(bundleName?: string): Promise<Array<RetentionSandboxInfo>>;

  /**
   * Obtains information about the retained DLP sandboxes of an application. This method uses an asynchronous callback to return the result.
   *
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { AsyncCallback<Array<RetentionSandboxInfo>> } callback - Indicates the callback of getRetentionSandboxList.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getRetentionSandboxList(bundleName: string, callback: AsyncCallback<Array<RetentionSandboxInfo>>): void;

  /**
   * Obtains information about the retained DLP sandboxes of an application. This method uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<Array<RetentionSandboxInfo>> } callback - Indicates the callback of getRetentionSandboxList.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getRetentionSandboxList(callback: AsyncCallback<Array<RetentionSandboxInfo>>): void;

  /**
   * Obtains the DLP file access records. This method uses a promise to return the result.
   *
   * @returns { Promise<Array<AccessedDLPFileInfo>> } Returns a list of {@link AccessedDLPFileInfo}.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPFileAccessRecords(): Promise<Array<AccessedDLPFileInfo>>;

  /**
   * Obtains the DLP file access records. This method uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<Array<AccessedDLPFileInfo>> } callback - Indicates the callback of getDLPFileAccessRecords.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 10
   */
  function getDLPFileAccessRecords(callback: AsyncCallback<Array<AccessedDLPFileInfo>>): void;

  /**
   * Represents the return value of the function startDLPManagerForResult.
   *
   * @interface DLPManagerResult
   * @syscap SystemCapability.Security.DataLossPrevention
   * @StageModelOnly
   * @since 11
   */
  export interface DLPManagerResult {
    /**
     * Indicates the result code returned after the DLP manager is destroyed.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @StageModelOnly
     * @since 11
     */
    resultCode: number;

    /**
     * Indicates the data returned after the DLP manager is destroyed.
     *
     * @type { Want }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @StageModelOnly
     * @since 11
     */
    want: Want;
  }

  /**
   * Starts the DLP manager. This method uses a promise to return the result.
   *
   * @param { common.UIAbilityContext } context - Indicates the UIAbility context of the caller.
   * @param { Want } want - Indicates the request to the DLP manager.
   * @returns { Promise<DLPManagerResult> } Returns the {@link DLPManagerResult}.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100016 - The uri field is missing in the want parameter.
   * @throws { BusinessError } 19100017 - The displayName field is missing in the want parameter.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @StageModelOnly
   * @since 11
   */
  function startDLPManagerForResult(context: common.UIAbilityContext, want: Want): Promise<DLPManagerResult>;

  /**
   * Enumerates the gathering policy types for DLP files.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export enum GatheringPolicyType {
    /**
     * Gathering, which allows multiple DLP files to be opened in a sandbox.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    GATHERING = 1,

    /**
     * Non-gathering, which allows only one DLP file to be opened in a sandbox.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    NON_GATHERING = 2
  }

  /**
   * Obtains the DLP sandbox gathering policy. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @returns { Promise<GatheringPolicyType> } Returns the {@link GatheringPolicyType}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getDLPGatheringPolicy(): Promise<GatheringPolicyType>;

  /**
   * Obtains the DLP sandbox gathering policy. This method uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { AsyncCallback<GatheringPolicyType> } callback - Indicates the callback of getDLPGatheringPolicy.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getDLPGatheringPolicy(callback: AsyncCallback<GatheringPolicyType>): void;

  /**
   * Represents the installed DLP sandbox application info.
   *
   * @interface DLPSandboxInfo
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface DLPSandboxInfo {
    /**
     * Index of the installed DLP sandbox application.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    appIndex: number;

    /**
     * Token ID of the installed DLP sandbox application.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    tokenID: number;
  }

  /**
   * Installs a DLP sandbox application. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { DLPFileAccess } access - Indicates the access permission for the DLP file.
   * @param { number } userId - Indicates the user ID.
   * @param { string } uri - Indicates the URI of the file.
   * @returns { Promise<DLPSandboxInfo> } Returns the {@link DLPSandboxInfo}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function installDLPSandbox(
    bundleName: string,
    access: DLPFileAccess,
    userId: number,
    uri: string
  ): Promise<DLPSandboxInfo>;

  /**
   * Installs a DLP sandbox application. This method uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { DLPFileAccess } access - Indicates the access permission for the DLP file.
   * @param { number } userId - Indicates the user ID.
   * @param { string } uri - Indicates the URI of the file.
   * @param { AsyncCallback<DLPSandboxInfo> } callback - Indicates the callback of installDLPSandbox.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function installDLPSandbox(
    bundleName: string,
    access: DLPFileAccess,
    userId: number,
    uri: string,
    callback: AsyncCallback<DLPSandboxInfo>
  ): void;

  /**
   * Uninstalls a DLP sandbox application. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { number } userId - Indicates the user ID.
   * @param { number } appIndex - Indicates the index of DLP sandbox.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function uninstallDLPSandbox(bundleName: string, userId: number, appIndex: number): Promise<void>;

  /**
   * Uninstalls a DLP sandbox application. This method uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { number } userId - Indicates the user ID.
   * @param { number } appIndex - Indicates the index of DLP sandbox.
   * @param { AsyncCallback<void> } callback - Indicates the callback of uninstallDLPSandbox.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function uninstallDLPSandbox(
    bundleName: string,
    userId: number,
    appIndex: number,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Represents the DLP sandbox state.
   *
   * @interface DLPSandboxState
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface DLPSandboxState {
    /**
     * Bundle name of the application.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    bundleName: string;

    /**
     * Application index of the DLP sandbox.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    appIndex: number;
  }

  /**
   * Subscribes to the event reported when a DLP sandbox application is uninstalled.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { 'uninstallDLPSandbox' } type - Indicates the type of event to subscribe to.
   *     The value of type must be uninstallDLPSandbox.
   * @param { Callback<DLPSandboxState> } listener - Indicates the callback for the DLP sandbox application uninstall event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function on(type: 'uninstallDLPSandbox', listener: Callback<DLPSandboxState>): void;

  /**
   * Unsubscribes from the event reported when a DLP sandbox application is uninstalled.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { 'uninstallDLPSandbox' } type - Indicates the type of event to unsubscribe from.
   *     The value of type must be uninstallDLPSandbox.
   * @param { Callback<DLPSandboxState> } listener - Indicates the callback for the DLP sandbox application uninstall event.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function off(type: 'uninstallDLPSandbox', listener?: Callback<DLPSandboxState>): void;

  /**
   * Enumerates the account types for a DLP file.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export enum AccountType {
    /**
     * Cloud account.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    CLOUD_ACCOUNT = 1,

    /**
     * Domain account.
     *
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    DOMAIN_ACCOUNT = 2,
  }

  /**
   * Represents the authorized user information.
   *
   * @interface AuthUser
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface AuthUser {
    /**
     * Authorized account of the DLP file.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    authAccount: string;

    /**
     * Type of the authorized account.
     *
     * @type { AccountType }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    authAccountType: AccountType;

    /**
     * Authorized permission for the DLP file.
     *
     * @type { DLPFileAccess }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    dlpFileAccess: DLPFileAccess;

    /**
     * Authorization expiration time of the DLP file.
     *
     * @type { number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    permExpiryTime: number;
  }

  /**
   * Represents the DLP file property.
   *
   * @interface DLPProperty
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface DLPProperty {
    /**
     * Owner account of the DLP file.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ownerAccount: string;

    /**
     * Owner account ID of the DLP file.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ownerAccountID: string;

    /**
     * Type of the owner account of the DLP file.
     *
     * @type { AccountType }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ownerAccountType: AccountType;

    /**
     * Authorized users of the DLP file.
     *
     * @type { ?Array<AuthUser> }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    authUserList?: Array<AuthUser>;

    /**
     * Contact account of the DLP file.
     *
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    contactAccount: string;

    /**
     * Whether the DLP file can be accessed offline.
     * If the DLP file supports offline access, the credential server needs to be connected to the network only when the DLP file is opened for the first time.
     *
     * @type { boolean }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    offlineAccess: boolean;

    /**
     * Everyone access list for the DLP file.
     *
     * @type { ?Array<DLPFileAccess> }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    everyoneAccessList?: Array<DLPFileAccess>;

    /**
     * Timestamp of the time when the DLP file expires.
     *
     * @type { ?number }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    expireTime?: number;

    /**
     * Action type when the DLP file exceed expire time.
     *
     * @type { ?ActionType }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    actionUponExpiry?: ActionType;
  }

  /**
   * Defines the DLP file object.
   *
   * @interface DLPFile
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface DLPFile {
    /**
     * DLP file property.
     *
     * @type { DLPProperty }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    dlpProperty: DLPProperty;

    /**
     * Adds a link file for the DLP file. This method uses a promise to return the result.
     * The link file is implemented through the Filesystem in Userspace (FUSE).
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file to add.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    addDLPLinkFile(linkFileName: string): Promise<void>;

    /**
     * Adds a link file for the DLP file. This method uses an asynchronous callback to return the result.
     * The link file is implemented through the FUSE.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file to add.
     * @param { AsyncCallback<void> } callback - Indicates the callback of addDLPLinkFile.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    addDLPLinkFile(linkFileName: string, callback: AsyncCallback<void>): void;

    /**
     * Stops the FUSE link between the DLP file and a link file. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    stopFuseLink(): Promise<void>;

    /**
     * Stops the FUSE link between the DLP file and a link file. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { AsyncCallback<void> } callback - Indicates the callback of stopFuseLink.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    stopFuseLink(callback: AsyncCallback<void>): void;

    /**
     * Resumes the FUSE link between the DLP file and a link file. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    resumeFuseLink(): Promise<void>;

    /**
     * Resumes the FUSE link between the DLP file and a link file. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { AsyncCallback<void> } callback - Indicates the callback of resumeFuseLink.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    resumeFuseLink(callback: AsyncCallback<void>): void;

    /**
     * Replaces the link file of the DLP file. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    replaceDLPLinkFile(linkFileName: string): Promise<void>;

    /**
     * Replaces the link file of the DLP file. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file.
     * @param { AsyncCallback<void> } callback - Indicates the callback of replaceDLPLinkFile.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    replaceDLPLinkFile(linkFileName: string, callback: AsyncCallback<void>): void;

    /**
     * Deletes a link file of the DLP file. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file to delete.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    deleteDLPLinkFile(linkFileName: string): Promise<void>;

    /**
     * Deletes a link file of the DLP file. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { string } linkFileName - Indicates the name of link file to delete.
     * @param { AsyncCallback<void> } callback - Indicates the callback of deleteDLPLinkFile.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    deleteDLPLinkFile(linkFileName: string, callback: AsyncCallback<void>): void;

    /**
     * Recovers the file in plaintext from the DLP file. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { number } plaintextFd - Indicates the file descriptor of the file in plaintext.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
     * @throws { BusinessError } 19100003 - Credential task time out.
     * @throws { BusinessError } 19100004 - Credential service error.
     * @throws { BusinessError } 19100005 - Credential authentication server error.
     * @throws { BusinessError } 19100008 - The file is not a DLP file.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100010 - The DLP file is read only.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    recoverDLPFile(plaintextFd: number): Promise<void>;

    /**
     * Recovers the file in plaintext from the DLP file. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { number } plaintextFd - Indicates the file descriptor of the file in plaintext.
     * @param { AsyncCallback<void> } callback - Indicates the callback of recoverDLPFile.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     *     2. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
     * @throws { BusinessError } 19100003 - Credential task time out.
     * @throws { BusinessError } 19100004 - Credential service error.
     * @throws { BusinessError } 19100005 - Credential authentication server error.
     * @throws { BusinessError } 19100008 - The file is not a DLP file.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100010 - The DLP file is read only.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    recoverDLPFile(plaintextFd: number, callback: AsyncCallback<void>): void;

    /**
     * Closes the DLP file when the object is no longer used. This method uses a promise to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    closeDLPFile(): Promise<void>;

    /**
     * Closes the DLP file when the object is no longer used. This method uses an asynchronous callback to return the result.
     *
     * @permission ohos.permission.ACCESS_DLP_FILE
     * @param { AsyncCallback<void> } callback - Indicates the callback of closeDLPFile.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Non-system applications use system APIs.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
     * @throws { BusinessError } 19100001 - Invalid parameter value.
     * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
     * @throws { BusinessError } 19100011 - The system ability works abnormally.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    closeDLPFile(callback: AsyncCallback<void>): void;
  }

  /**
   * Generates a DLP file. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { number } plaintextFd - Indicates the file descriptor of the file in plaintext.
   * @param { number } ciphertextFd - Indicates the file descriptor of the DLP file.
   * @param { DLPProperty } property - Indicates the property of the DLP file.
   * @returns { Promise<DLPFile> } Returns the {@link DLPFile}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function generateDLPFile(plaintextFd: number, ciphertextFd: number, property: DLPProperty): Promise<DLPFile>;

  /**
   * Generates a DLP file. This method uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { number } plaintextFd - Indicates the file descriptor of the file in plaintext.
   * @param { number } ciphertextFd - Indicates the file descriptor of the DLP file.
   * @param { DLPProperty } property - Indicates the property of the DLP file.
   * @param { AsyncCallback<DLPFile> } callback - Indicates the callback of generateDLPFile.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function generateDLPFile(
    plaintextFd: number,
    ciphertextFd: number,
    property: DLPProperty,
    callback: AsyncCallback<DLPFile>
  ): void;

  /**
   * Opens a DLP file. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { number } ciphertextFd - Indicates the file descriptor of the DLP file to open.
   * @param { string } appId - Indicates the app ID of the application which opens the DLP file.
   * @returns { Promise<DLPFile> } Returns the {@link DLPFile}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100008 - The file is not a DLP file.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100018 - The application is not authorized.
   * @throws { BusinessError } 19100019 - The DLP file has expired.
   * @throws { BusinessError } 19100020 - No network connection.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function openDLPFile(ciphertextFd: number, appId: string): Promise<DLPFile>;

  /**
   * Opens a DLP file. This method uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ACCESS_DLP_FILE
   * @param { number } ciphertextFd - Indicates the file descriptor of the DLP file to open.
   * @param { string } appId - Indicates the app ID of the application which opens the DLP file.
   * @param { AsyncCallback<DLPFile> } callback - Indicates the callback of openDLPFile.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100008 - The file is not a DLP file.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100018 - The application is not authorized.
   * @throws { BusinessError } 19100019 - The DLP file has expired.
   * @throws { BusinessError } 19100020 - No network connection.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function openDLPFile(ciphertextFd: number, appId: string, callback: AsyncCallback<DLPFile>): void;

  /**
   * Sets sandbox application configuration. This method uses a promise to return the result.
   *
   * @param { string } configInfo - Configuration of the sandbox application.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   *     2. Incorrect parameter types.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100018 - The application is not authorized.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 11
   */
  function setSandboxAppConfig(configInfo: string): Promise<void>;

  /**
   * Cleans sandbox application configuration. This method uses a promise to return the result.
   *
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100007 - No permission to call this API,
   *     which is available only for non-DLP sandbox applications.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100018 - The application is not authorized.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 11
   */
  function cleanSandboxAppConfig(): Promise<void>;

  /**
   * Obtains sandbox application configuration. This method uses a promise to return the result.
   *
   * @returns { Promise<string> } Promise used to return the result.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100018 - The application is not authorized.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 11
   */
  function getSandboxAppConfig(): Promise<string>;

   /**
   * Checks whether the current system provides the DLP feature. This method uses a promise to return the result.
   *
   * @returns { Promise<boolean> } Promise used to return the result.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 12
   */
   function isDLPFeatureProvided(): Promise<boolean>;

  /**
   * Enumerates the action types when exceed expiry time.
   * @enum { number } Valuable
   * @syscap SystemCapability.Security.DataLossPrevention
   * @since 20
   */
  export enum ActionType {
    /**
     * NOT_OPEN, which not allows DLP files to be opened exceed expire time.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 20
     */
    NOT_OPEN = 0,

    /**
     * OPEN, which allows DLP files to be opened exceed expire time.
     * @syscap SystemCapability.Security.DataLossPrevention
     * @since 20
     */
    OPEN = 1
  }

  /**
   * Represent the DLP file Custom property.
   * 
   * @interface CustomProperty
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 20
   */
  export interface CustomProperty {
    /**
     * User defined information for enterprise space
     * 
     * @type { string }
     * @syscap SystemCapability.Security.DataLossPrevention
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    enterprise: string;
  }

  /**
   * Generates a DLP file. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ENTERPRISE_ACCESS_DLP_FILE
   * @param { number } plaintextFd - FD of the file in plaintext.
   * @param { number } dlpFd - FD of the DLP file to generate.
   * @param { DLPProperty } property - General DLP policy to use.
   * @param { CustomProperty } customProperty - Custom DLP policy to use.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100014 - Account not logged in.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 20
   */
  function generateDlpFileForEnterprise(plaintextFd: number, dlpFd: number, property: DLPProperty, customProperty: CustomProperty): Promise<void>;

  /**
   * Queries the DLP policy. This method uses a promise to return the result.
   * 
   * @permission ohos.permission.ENTERPRISE_ACCESS_DLP_FILE
   * @param { number } dlpFd - Indicates the file descriptor of the DLP file.
   * @returns { Promise<string> }.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100008 - The file is not a DLP file.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100013 - The user does not have the permission.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 20
   */
  function queryDlpPolicy(dlpFd: number): Promise<string>;

  /**
   * Decrypts a DLP file. This method uses a promise to return the result.
   *
   * @permission ohos.permission.ENTERPRISE_ACCESS_DLP_FILE
   * @param { number } dlpFd - Indicates the file descriptor of the DLP file.
   * @param { number } plaintextFd - Indicates the file descriptor of the file in plaintext.
   * @returns { Promise<void> }.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 19100001 - Invalid parameter value.
   * @throws { BusinessError } 19100002 - Credential service busy due to too many tasks or duplicate tasks.
   * @throws { BusinessError } 19100003 - Credential task time out.
   * @throws { BusinessError } 19100004 - Credential service error.
   * @throws { BusinessError } 19100005 - Credential authentication server error.
   * @throws { BusinessError } 19100008 - The file is not a DLP file.
   * @throws { BusinessError } 19100009 - Failed to operate the DLP file.
   * @throws { BusinessError } 19100011 - The system ability works abnormally.
   * @throws { BusinessError } 19100013 - The user does not have the permission.
   * @syscap SystemCapability.Security.DataLossPrevention
   * @systemapi Hide this for inner system use.
   * @since 20
   */
  function decryptDlpFile(dlpFd: number, plaintextFd: number): Promise<void>;

}
export default dlpPermission;