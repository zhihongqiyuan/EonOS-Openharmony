/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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
 * @kit CoreFileKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import Want from './@ohos.app.ability.Want';
import Context from './application/Context';
import { Filter } from './@ohos.file.fs';

/**
 * This module provides the capability to access user public files.
 *
 * @namespace fileAccess
 * @syscap SystemCapability.FileManagement.UserFileService
 * @since 9
 */
declare namespace fileAccess {
  /**
   * Query the want information of HAP configured with fileaccess.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { AsyncCallback<Array<Want>> } callback - The callback is used to return a Array<Want> object.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900006 - No such device or address
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900014 - Device or resource busy
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900017 - No such device
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900023 - Text file busy
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900027 - Read-only file system
   * @throws { BusinessError } 13900029 - Resource deadlock would occur
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900033 - Too many symbolic links encountered
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900038 - Value too large for defined data type
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300001 - IPC error
   * @throws { BusinessError } 14300002 - Invalid uri
   * @throws { BusinessError } 14300003 - Fail to get fileextension info
   * @throws { BusinessError } 14300004 - Get wrong result
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  function getFileAccessAbilityInfo(callback: AsyncCallback<Array<Want>>): void;

  /**
   * Query the want information of HAP configured with fileaccess.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @returns { Promise<Array<Want>> } Returns the wants.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900006 - No such device or address
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900014 - Device or resource busy
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900017 - No such device
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900023 - Text file busy
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900027 - Read-only file system
   * @throws { BusinessError } 13900029 - Resource deadlock would occur
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900033 - Too many symbolic links encountered
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900038 - Value too large for defined data type
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300001 - IPC error
   * @throws { BusinessError } 14300002 - Invalid uri
   * @throws { BusinessError } 14300003 - Fail to get fileextension info
   * @throws { BusinessError } 14300004 - Get wrong result
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  function getFileAccessAbilityInfo(): Promise<Array<Want>>;

  /**
   * Obtains the fileAccessHelper that connects all fileaccess servers in the system.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Context } context Indicates the application context.
   * @returns { FileAccessHelper } Returns the fileAccessHelper.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900006 - No such device or address
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900014 - Device or resource busy
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900017 - No such device
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900023 - Text file busy
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900027 - Read-only file system
   * @throws { BusinessError } 13900029 - Resource deadlock would occur
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900033 - Too many symbolic links encountered
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900038 - Value too large for defined data type
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300001 - IPC error
   * @throws { BusinessError } 14300002 - Invalid uri
   * @throws { BusinessError } 14300003 - Fail to get fileextension info
   * @throws { BusinessError } 14300004 - Get wrong result
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  function createFileAccessHelper(context: Context): FileAccessHelper;

  /**
   * Obtains the fileAccessHelper that connects some specified fileaccess servers in the system.
   *
   * @permission ohos.permission.FILE_ACCESS_MANAGER and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Context } context Indicates the application context.
   * @param { Array<Want> } wants Represents the connected data provider.
   * @returns { FileAccessHelper } Returns the fileAccessHelper.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900006 - No such device or address
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900014 - Device or resource busy
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900017 - No such device
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900023 - Text file busy
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900027 - Read-only file system
   * @throws { BusinessError } 13900029 - Resource deadlock would occur
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900033 - Too many symbolic links encountered
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900038 - Value too large for defined data type
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300001 - IPC error
   * @throws { BusinessError } 14300002 - Invalid uri
   * @throws { BusinessError } 14300003 - Fail to get fileextension info
   * @throws { BusinessError } 14300004 - Get wrong result
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  function createFileAccessHelper(context: Context, wants: Array<Want>): FileAccessHelper;

  /**
   * File Object
   *
   * @interface FileInfo
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  interface FileInfo {
    /**
     * Indicates the path of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    uri: string;
    /**
     * Indicates the relativePath of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    relativePath: string;
    /**
     * Indicates the name of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    fileName: string;
    /**
     * Indicates the mode of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    mode: number;
    /**
     * Indicates the size of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    size: number;
    /**
     * Indicates the mtime of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    mtime: number;
    /**
     * Indicates the mimeType of the file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    mimeType: string;

    /**
     * List files in the current directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { Filter } filter Indicates the filter of file.
     * @returns { FileIterator } Returns the FileIterator Object.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    listFile(filter?: Filter): FileIterator;

    /**
     * Recursively list all files in the current directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { Filter } filter Indicates the filter of file.
     * @returns { FileIterator } Returns the FileIterator Object.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    scanFile(filter?: Filter): FileIterator;
  }

  /**
   * FileIterator Object
   *
   * @interface FileIterator
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  interface FileIterator {
    /**
     * Get the next fileInfo.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @returns { object } Returns FileInfo Object and boolean flag.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    next(): { value: FileInfo, done: boolean };
  }

  /**
   * Root Object
   *
   * @interface RootInfo
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  interface RootInfo {
    /**
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    deviceType: number;
    /**
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    uri: string;
    /**
     * File or directory relative path
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    relativePath: string;
    /**
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    displayName: string;
    /**
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    deviceFlags: number;

    /**
     * List files in the current directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { Filter } filter Indicates the filter of file.
     * @returns { FileIterator } Returns the FileIterator Object.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    listFile(filter?: Filter): FileIterator;

    /**
     * Recursively list all files in the current directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { Filter } filter Indicates the filter of file.
     * @returns { FileIterator } Returns the RootIterator Object.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    scanFile(filter?: Filter): FileIterator;
  }

  /**
   * RootIterator Object
   *
   * @interface RootIterator
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  interface RootIterator {
    /**
     * Get a next RootInfo.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @returns { object } Returns RootInfo Object and boolean flag.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    next(): { value: RootInfo, done: boolean };
  }

  /**
   * Describes the return information of the copy operation.
   *
   * @interface CopyResult
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @stagemodelonly
   * @since 10
   */
  interface CopyResult {
    /**
     * Represents the source file or directory uri to be copied
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 10
     */
    sourceUri: string;
    /**
     * Represents a file or directory uri in the target folder
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 10
     */
    destUri: string;
    /**
     * Error code for failure to copy a file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 10
     */
    errCode: number;
    /**
     * Error message for failure to copy a file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @stagemodelonly
     * @since 10
     */
    errMsg: string;
  }

  /**
   * OPENFLAGS represents the way to open the file.
   *
   * @enum { number } OPENFLAGS
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  enum OPENFLAGS {
    /**
     * Open the file by Read-only mode.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    READ = 0o0,

    /**
     * Open the file by Write-only mode.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    WRITE = 0o1,

    /**
     * Open the file by Write-Read mode.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    WRITE_READ = 0o2
  }

  /**
   * Property elements that support the file queries.
   *
   * @enum { string } FileKey
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  enum FileKey {
    /**
     * The key represents the file name.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    DISPLAY_NAME = 'display_name',

    /**
     * The key represents the date of the file creation.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    DATE_ADDED = 'date_added',

    /**
     * The key represents the modify date of the file.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    DATE_MODIFIED = 'date_modified',

    /**
     * The key represents the relative path.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    RELATIVE_PATH = 'relative_path',

    /**
     * The key represents the file size.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    FILE_SIZE = 'size'
  }

  /**
   * Describes the notifyType.
   *
   * @enum { number } NotifyType
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  enum NotifyType {
    /**
     * Type for add notification
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    NOTIFY_ADD,

    /**
     * Type for delete notification
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    NOTIFY_DELETE,

    /**
     * The file was moved into the monitored directory.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    NOTIFY_MOVED_TO,

    /**
     * The file in the monitored directory was moved.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    NOTIFY_MOVED_FROM,

    /**
     * The monitored file or directory was moved.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    NOTIFY_MOVE_SELF,

    /**
     * Event type of device online.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    NOTIFY_DEVICE_ONLINE,

    /**
     * Event type of device offline.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    NOTIFY_DEVICE_OFFLINE
  }

  /**
   * The value of the notify callback function
   *
   * @interface NotifyMessage
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  interface NotifyMessage {
    /**
     * Indicates the change type.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { NotifyType }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    type: NotifyType;

    /**
     * Indicates the uris generated the change event.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    uris: Array<string>;
  }

  /**
   * Describes the return information of the move operation.
   *
   * @interface MoveResult
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  interface MoveResult {
    /**
     * Indicates the source file or directory uri to be moved.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    sourceUri: string;

    /**
     * Indicates the file or directory uri in the destination folder.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    destUri: string;

    /**
     * Error code for failure to move a file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    errCode: number;

    /**
     * Error message for failure to move a file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    errMsg: string;
  }

  /**
   * Indicates the root uri of the device
   *
   * @constant
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  const DEVICES_URI = 'file://docs';

  /**
   * FileAccessHelper Object
   *
   * @interface FileAccessHelper
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @since 9
   */
  interface FileAccessHelper {
    /**
     * Open a file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the path of the file to open.
     * @param { OPENFLAGS } flags Indicate options of opening a file. The default value is read-only.
     * @returns { Promise<number> } Returns the file descriptor.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    openFile(uri: string, flags: OPENFLAGS): Promise<number>;

    /**
     * Open a file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the path of the file to open.
     * @param { OPENFLAGS } flags Indicate options of opening a file. The default value is read-only.
     * @param { AsyncCallback<number> } callback - The callback is used to return the file descriptor.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    openFile(uri: string, flags: OPENFLAGS, callback: AsyncCallback<number>): void;

    /**
     * Create a file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Represents a specific parent directory.
     * @param { string } displayName Indicates the new file name, and supports with suffix.
     * @returns { Promise<string> } Returns the new file's URI.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    createFile(uri: string, displayName: string): Promise<string>;

    /**
     * Create a file.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Represents a specific parent directory.
     * @param { string } displayName Indicates the new file name, and supports with suffix.
     * @param { AsyncCallback<string> } callback - The callback is used to return the new file's URI.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    createFile(uri: string, displayName: string, callback: AsyncCallback<string>): void;

    /**
     * Create a Directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } parentUri Represents a specific parent directory.
     * @param { string } displayName Indicates the new directory name.
     * @returns { Promise<string> } Returns the new directory's URI.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    mkDir(parentUri: string, displayName: string): Promise<string>;

    /**
     * Create a Directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } parentUri Represents a specific parent directory.
     * @param { string } displayName Indicates the new directory name.
     * @param { AsyncCallback<string> } callback - The callback is used to return the new directory's URI.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    mkDir(parentUri: string, displayName: string, callback: AsyncCallback<string>): void;

    /**
     * Delete a file or delete a directory recursively.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the file or directory to be deleted.
     * @returns { Promise<number> }
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    delete(uri: string): Promise<number>;

    /**
     * Delete a file or delete a directory recursively.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the file or directory to be deleted.
     * @param { AsyncCallback<number> } callback
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    delete(uri: string, callback: AsyncCallback<number>): void;

    /**
     * Move a file or move a directory recursively.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceFile Indicates the file or directory to be moved.
     * @param { string } destFile Represents the destination folder.
     * @returns { Promise<string> } Returns the generated new file or directory.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    move(sourceFile: string, destFile: string): Promise<string>;

    /**
     * Move a file or move a directory recursively.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceFile Indicates the file or directory to be moved.
     * @param { string } destFile Represents the destination folder.
     * @param { AsyncCallback<string> } callback - The callback is used to return the generated new file or directory.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    move(sourceFile: string, destFile: string, callback: AsyncCallback<string>): void;

    /**
     * Copy file or directory in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be copied.
     * @param { string } destUri - Represents the destination directory.
     * @param { boolean } force - Optional parameter that determines whether to forcibly copy files.
     * @returns { Promise<Array<CopyResult>> } Returns the file information where the error occurred.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    copy(sourceUri: string, destUri: string, force?: boolean): Promise<Array<CopyResult>>;

    /**
     * Copy file or directory in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be copied.
     * @param { string } destUri - Represents the destination directory.
     * @param { AsyncCallback<Array<CopyResult>> } callback - The callback is used to return the file information where the error occurred.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    copy(sourceUri: string, destUri: string, callback: AsyncCallback<Array<CopyResult>>): void;

    /**
     * Copy file or directory in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be copied.
     * @param { string } destUri - Represents the destination directory.
     * @param { boolean } force - Determines whether to forcibly copy files.
     * @param { AsyncCallback<Array<CopyResult>> } callback - The callback is used to return the file information where the error occurred.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    copy(sourceUri: string, destUri: string, force: boolean, callback: AsyncCallback<Array<CopyResult>>): void;

    /**
     * copy the file with the specified file name in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination folder.
     * @param { string } fileName - Represents the name of the specified file.
     * @returns { Promise<string> } Returns the generated new file uri.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    copyFile(sourceUri: string, destUri: string, fileName: string): Promise<string>;

    /**
     * copy the file with the specified file name in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination folder.
     * @param { string } fileName - Represents the name of the specified file.
     * @param { AsyncCallback<string> } callback - Returns the generated new file uri.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    copyFile(sourceUri: string, destUri: string, fileName: string, callback: AsyncCallback<string>): void;

    /**
     * Rename the selected file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the selected file or directory.
     * @param { string } displayName Indicates the new directory or file name.
     * @returns { Promise<string> } Returns a URI representing the new file or directory.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    rename(uri: string, displayName: string): Promise<string>;

    /**
     * Rename the selected file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri Indicates the selected file or directory.
     * @param { string } displayName Indicates the new directory or file name.
     * @param { AsyncCallback<string> } callback - The callback is used to return a URI representing the new file or directory.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    rename(uri: string, displayName: string, callback: AsyncCallback<string>): void;

    /**
     * Obtain the status of a file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceFileUri Indicates the selected file or directory.
     * @returns { Promise<boolean> } Returns whether it exists.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    access(sourceFileUri: string): Promise<boolean>;

    /**
     * Obtain the status of a file or directory.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceFileUri Indicates the selected file or directory.
     * @param { AsyncCallback<boolean> } callback - The callback is used to return whether it exists.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    access(sourceFileUri: string, callback: AsyncCallback<boolean>): void;

    /**
     * Query file related information by the uri in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the selected file or directory.
     * @param { string } metaJson The json string includes query property.
     * @returns { Promise<string> } Returns the json string, includes query property and value.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    query(uri: string, metaJson: string): Promise<string>;

    /**
     * Query file related information by the uri in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the selected file or directory.
     * @param { string } metaJson The json string includes query property.
     * @param { AsyncCallback<string> } callback - Returns the json string, includes query property and value.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    query(uri: string, metaJson: string, callback: AsyncCallback<string>): void;

    /**
     * Get a FileInfo by the uri in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the selected file or directory.
     * @returns { Promise<FileInfo> } Returns a FileInfo.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    getFileInfoFromUri(uri: string): Promise<FileInfo>;

    /**
     * Get a FileInfo by the uri in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the selected file or directory.
     * @param { AsyncCallback<FileInfo> } callback - The callback is used to return a fileinfo object.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    getFileInfoFromUri(uri: string, callback: AsyncCallback<FileInfo>): void;

    /**
     * Get a FileInfo by the relative path in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } relativePath - Indicates the selected file or directory.
     * @returns { Promise<FileInfo> } Returns a FileInfo.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    getFileInfoFromRelativePath(relativePath: string): Promise<FileInfo>;

    /**
     * Get a FileInfo by the relative path in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } relativePath - Indicates the selected file or directory.
     * @param { AsyncCallback<FileInfo> } callback - The callback is used to return a fileinfo object.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    getFileInfoFromRelativePath(relativePath: string, callback: AsyncCallback<FileInfo>): void;

    /**
     * Get a RootIterator.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @returns { Promise<RootIterator> } Returns a RootIterator.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    getRoots(): Promise<RootIterator>;

    /**
     * Get a RootIterator.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { AsyncCallback<RootIterator> } callback - The callback is used to return a RootIterator.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900006 - No such device or address
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900013 - Bad address
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900017 - No such device
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900029 - Resource deadlock would occur
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900033 - Too many symbolic links encountered
     * @throws { BusinessError } 13900034 - Operation would block
     * @throws { BusinessError } 13900038 - Value too large for defined data type
     * @throws { BusinessError } 13900041 - Quota exceeded
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14000001 - Invalid display name
     * @throws { BusinessError } 14000002 - Invalid uri
     * @throws { BusinessError } 14000003 - Invalid file extension
     * @throws { BusinessError } 14000004 - File has been put into trash bin
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 9
     */
    getRoots(callback: AsyncCallback<RootIterator>): void;

    /**
     * Turn on monitor the specified uri.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the file or directory to be monitored.
     * @param { boolean } notifyForDescendants - Indicates Whether to monitor changes in descendants.
     * @param { Callback<NotifyMessage> } callback - The callback is used to return the notify message.
     * @throws { BusinessError } 14300002 - Invalid uri
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    registerObserver(uri: string, notifyForDescendants: boolean, callback: Callback<NotifyMessage>): void;

    /**
     * Turn off monitor the specified uri.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } uri - Indicates the file or directory the file that will cancel monitored.
     * @param { Callback<NotifyMessage> } callback - The object bound to the uri.
     * @throws { BusinessError } 14300002 - Invalid uri
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 10
     */
    unregisterObserver(uri: string, callback?: Callback<NotifyMessage>): void;

    /**
     * Move file or directory in the promise way, And return the MoveResult.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination directory.
     * @param { boolean } [force] - Optional parameter that determines whether to forcibly move files.
     * @returns { Promise<Array<MoveResult>> } Returns the file information where the error occurred.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    moveItem(sourceUri: string, destUri: string, force?: boolean): Promise<Array<MoveResult>>;

    /**
     * Move file or directory in the asyncCallback way, and return the MoveResult.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination directory.
     * @param { AsyncCallback<Array<MoveResult>> } callback - The callback is used to return the file information where the error occurred.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    moveItem(sourceUri: string, destUri: string, callback: AsyncCallback<Array<MoveResult>>): void;

    /**
     * Move file or directory in the asyncCallback way, and return the MoveResult.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination directory.
     * @param { boolean } force - Determines whether to forcibly move files.
     * @param { AsyncCallback<Array<MoveResult>> } callback - The callback is used to return the file information where the error occurred.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    moveItem(sourceUri: string, destUri: string, force: boolean, callback: AsyncCallback<Array<MoveResult>>): void;

    /**
     * Move the file with the specified file name in the promise way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination folder.
     * @param { string } fileName - Represents the name of the specified file.
     * @returns { Promise<string> } Returns the generated new file uri.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    moveFile(sourceUri: string, destUri: string, fileName: string): Promise<string>;

    /**
     * Move the file with the specified file name in the asyncCallback way.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @param { string } sourceUri - Indicates the file or directory to be moved.
     * @param { string } destUri - Represents the destination folder.
     * @param { string } fileName - Represents the name of the specified file.
     * @param { AsyncCallback<string> } callback - The callback is used to return the generated new file uri.
     * @throws { BusinessError } 201 - Permission verification failed, usually the result returned by VerifyAccessToken.
     * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses system API.
     * @throws { BusinessError } 13900001 - Operation not permitted
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900004 - Interrupted system call
     * @throws { BusinessError } 13900008 - Bad file descriptor
     * @throws { BusinessError } 13900011 - Out of memory
     * @throws { BusinessError } 13900012 - Permission denied
     * @throws { BusinessError } 13900014 - Device or resource busy
     * @throws { BusinessError } 13900015 - File exists
     * @throws { BusinessError } 13900018 - Not a directory
     * @throws { BusinessError } 13900019 - Is a directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900022 - Too many open files
     * @throws { BusinessError } 13900023 - Text file busy
     * @throws { BusinessError } 13900024 - File too large
     * @throws { BusinessError } 13900025 - No space left on device
     * @throws { BusinessError } 13900027 - Read-only file system
     * @throws { BusinessError } 13900030 - File name too long
     * @throws { BusinessError } 13900042 - Unknown error
     * @throws { BusinessError } 14300001 - IPC error
     * @throws { BusinessError } 14300002 - Invalid uri
     * @throws { BusinessError } 14300003 - Fail to get fileextension info
     * @throws { BusinessError } 14300004 - Get wrong result
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 11
     */
    moveFile(sourceUri: string, destUri: string, fileName: string, callback: AsyncCallback<string>): void;
  }
}

export default fileAccess;
