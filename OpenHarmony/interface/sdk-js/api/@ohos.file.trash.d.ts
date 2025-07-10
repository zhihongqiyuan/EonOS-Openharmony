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
 * @kit CoreFileKit
 */

/**
 * Provides the recycling ability to the file manager.
 *
 * @namespace trash
 * @syscap SystemCapability.FileManagement.UserFileService
 * @systemapi
 * @StageModelOnly
 * @since 10
 */
declare namespace trash {
  /**
   * FileInfo Object
   *
   * @interface FileInfo
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  interface FileInfo {
    /**
     * Indicates the uri of the file.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly uri: string;

    /**
     * Indicates the source path of the file.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly srcPath: string;

    /**
     * Indicates the name of the file.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly fileName: string;

    /**
     * Indicates the mode of the file.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly mode: number;

    /**
     * Indicates the size of the file.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly size: number;
    
    /**
     * Indicates the mtime of the file.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly mtime: number;

    /**
     * Indicates the ctime of the file.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
    readonly ctime: number;
  }

  /**
     * List files in the trash.
     *
     * @permission ohos.permission.FILE_ACCESS_MANAGER
     * @returns { Array<FileInfo> } Returns the next level FileInfo Object.
     * @throws { BusinessError } 13900002 - No such file or directory
     * @throws { BusinessError } 13900020 - Invalid argument
     * @throws { BusinessError } 13900042 - Unknown error
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @StageModelOnly
     * @since 10
     */
  function listFile(): Array<FileInfo>;

  /**
   * Recover a file from the trash.
   * 
   * @permission ohos.permission.FILE_ACCESS_MANAGER
   * @param { string } uri - The identity of a file.
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function recover(uri: string): void;

  /**
   * Delete a file completely from th trash.
   * 
   * @permission ohos.permission.FILE_ACCESS_MANAGER
   * @param { string } uri - The identity of a file.
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.UserFileService
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  function completelyDelete(uri: string): void;
}

export default trash;
