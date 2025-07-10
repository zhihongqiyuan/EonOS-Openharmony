/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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

import { AsyncCallback } from './@ohos.base';
import stream from './@ohos.util.stream';

export default fileIo;

/**
 * FileIO
 *
 * @namespace fileIo
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * FileIO
 *
 * @namespace fileIo
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * FileIO
 *
 * @namespace fileIo
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * FileIO
 *
 * @namespace fileIo
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare namespace fileIo {

  export { access };

  export { accessSync };

  export { close };

  export { closeSync };

  export { copy };

  export { copyDir };

  export { copyDirSync };

  export { copyFile };

  export { copyFileSync };

  export { createRandomAccessFile };

  export { createRandomAccessFileSync };

  export { createStream };

  export { createStreamSync };

  export { createReadStream };

  export { createWriteStream };

  export { createWatcher };

  export { dup };

  export { fdatasync };

  export { fdatasyncSync };

  export { fdopenStream };

  export { fdopenStreamSync };

  export { fsync };

  export { fsyncSync };

  export { getxattr };

  export { getxattrSync };

  export { listFile };

  export { listFileSync };

  export { lseek };

  export { lstat };

  export { lstatSync };

  export { mkdir };

  export { mkdirSync };

  export { mkdtemp };

  export { mkdtempSync };

  export { moveDir };

  export { moveDirSync };

  export { moveFile };

  export { moveFileSync };

  export { open };

  export { openSync };

  export { read };

  export { readSync };

  export { readLines };

  export { readLinesSync };

  export { readText };

  export { readTextSync };

  export { rename };

  export { renameSync };

  export { rmdir };

  export { rmdirSync };

  export { setxattr };

  export { setxattrSync };

  export { stat };

  export { statSync };

  export { symlink };

  export { symlinkSync };

  export { truncate };

  export { truncateSync };

  export { unlink };

  export { unlinkSync };

  export { utimes };

  export { write };

  export { writeSync };

  export { AccessModeType };

  export { AccessFlagType };

  export { File };

  export { OpenMode };

  export { RandomAccessFile };

  export { ReaderIterator };

  export { Stat };

  export { Stream };

  export { ReadStream };

  export { WriteStream };

  export { AtomicFile };

  export { Watcher };

  export { WhenceType };

  export { TaskSignal };

  export { connectDfs };

  export { disconnectDfs };

  export type { Progress };

  export type { CopyOptions };

  export type { ProgressListener };

  export type { DfsListeners };

  /**
   * Mode Indicates the open flags.
   *
   * @namespace OpenMode
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Mode Indicates the open flags.
   *
   * @namespace OpenMode
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * Mode Indicates the open flags.
   *
   * @namespace OpenMode
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  namespace OpenMode {
    /**
     * Read only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * Read only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * Read only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const READ_ONLY = 0o0;
    /**
     * Write only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * Write only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * Write only Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const WRITE_ONLY = 0o1;
    /**
     * Write and Read Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * Write and Read Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * Write and Read Permission.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const READ_WRITE = 0o2;
    /**
     * If not exist, create file.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * If not exist, create file.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * If not exist, create file.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const CREATE = 0o100;
    /**
     * File truncate len 0.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * File truncate len 0.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * File truncate len 0.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const TRUNC = 0o1000;
    /**
     * File append write.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * File append write.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    /**
     * File append write.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    const APPEND = 0o2000;
    /**
     * File open in nonblocking mode.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * File open in nonblocking mode.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    const NONBLOCK = 0o4000;
    /**
     * File is Dir.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * File is Dir.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    const DIR = 0o200000;
    /**
     * File is not symbolic link.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * File is not symbolic link.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    const NOFOLLOW = 0o400000;
    /**
     * SYNC IO.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @since 9
     */
    /**
     * SYNC IO.
     *
     * @constant
     * @syscap SystemCapability.FileManagement.File.FileIO
     * @crossplatform
     * @since 10
     */
    const SYNC = 0o4010000;
  }
}

/**
 * Access file.
 *
 * @param { string } path - path.
 * @returns { Promise<boolean> } return Promise
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Access file.
 *
 * @param { string } path - path.
 * @returns { Promise<boolean> } Returns the file is accessible or not in promise mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Access file.
 *
 * @param { string } path - path.
 * @returns { Promise<boolean> } Returns the file is accessible or not in promise mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Checks whether the file or directory exists or has the operation permission.
 * This API uses a promise to return the result.
 * If the read, write, or read and write permission verification fails,
 * the error code 13900012 (Permission denied) will be thrown.
 *
 * @param { string } path - Application sandbox path of the file to check.
 * @param { AccessModeType } [mode = fs.AccessModeType.EXIST] - Permission on the file to verify.
 * <br>If this parameter is left blank, the system checks whether the file exists.
 * @returns { Promise<boolean> } Promise used to return a Boolean value.
 * <br>Returns true if the file exists; returns false otherwise.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function access(path: string, mode?: AccessModeType): Promise<boolean>;

/**
 * Access file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<boolean> } callback - The callback is used to return the file is accessible or not.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Access file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<boolean> } callback - The callback is used to return the file is accessible or not.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Checks whether a file or directory exists. This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the file to check.
 * @param { AsyncCallback<boolean> } callback - Callback invoked to return the result.
 * <br>The value true means the file exists; the value false means the opposite.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function access(path: string, callback: AsyncCallback<boolean>): void;

/**
 * Checks whether the file or directory is stored locally or has the operation permission.
 * This API uses a promise to return the result.If the read, write, or read and write permission verification fails,
 * the error code 13900012 (Permission denied) will be thrown.
 *
 * @param { string } path - Application sandbox path of the file to check.
 * @param { AccessModeType } mode - Permission on the file to verify.
 * @param { AccessFlagType } flag - Location of the file to verify.
 * @returns { Promise<boolean> } Promise used to return a Boolean value.
 * <br>The value true means the file is a local file and has the related permission.
 * <br>The value false means the file does not exist or is on the cloud or a distributed device.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function access(path: string, mode: AccessModeType, flag: AccessFlagType): Promise<boolean>;

/**
 * Access file with sync interface.
 *
 * @param { string } path - path.
 * @returns { boolean } Returns the file is accessible or not.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Access file with sync interface.
 *
 * @param { string } path - path.
 * @returns { boolean } Returns the file is accessible or not.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Access file with sync interface.
 *
 * @param { string } path - path.
 * @returns { boolean } Returns the file is accessible or not.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 *
 * Checks whether a file or directory exists or has the operation permission.
 * This API returns the result synchronously.If the read, write, or read and write permission verification fails,
 * the error code 13900012 (Permission denied) will be thrown.
 *
 * @param { string } path - Application sandbox path of the file to check.
 * @param { AccessModeType } [mode = fs.AccessModeType.EXIST] - Permission on the file to verify.
 * <br>If this parameter is left blank, the system checks whether the file exists.
 * @returns { boolean } Returns true if the file exists; returns false otherwise.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function accessSync(path: string, mode?: AccessModeType): boolean;

/**
 * Checks whether a file or directory is stored locally or has the operation permission. This API returns the result synchronously.
 * If the read, write, or read and write permission verification fails,
 * the error code 13900012 (Permission denied) will be thrown.
 *
 * @param { string } path - Application sandbox path of the file to check.
 * @param { AccessModeType } mode - Permission on the file to verify.
 * @param { AccessFlagType } flag - Location of the file to verify.
 * @returns { boolean } Returns true if the file is a local file and has the related permission;
 * <br>returns false if the file does not exist or is on the cloud or a distributed device.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function accessSync(path: string, mode: AccessModeType, flag: AccessFlagType): boolean;

/**
 * Close file or fd.
 *
 * @param { number | File } file - file object or fd.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Close file or fd.
 *
 * @param { number | File } file - file object or fd.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Closes a file or directory. This API uses a promise to return the result.
 *
 * @param { number | File } file - File object or FD of the file to close.
 * <br>Once closed, the File object or FD cannot be used for read or write operations.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function close(file: number | File): Promise<void>;

/**
 * Close file or fd.
 *
 * @param { number | File } file - file object or fd.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Close file or fd.
 *
 * @param { number | File } file - file object or fd.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Closes a file or directory. This API uses an asynchronous callback to return the result.
 *
 * @param { number | File } file - File object or FD of the file to close.
 * <br>Once closed, the File object or FD cannot be used for read or write operations.
 * @param { AsyncCallback<void> } callback - Callback invoked immediately after the file is closed.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function close(file: number | File, callback: AsyncCallback<void>): void;

/**
 * Close file or fd with sync interface.
 *
 * @param { number | File } file - file object or fd.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Close file or fd with sync interface.
 *
 * @param { number | File } file - file object or fd.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Closes a file or directory. This API returns the result synchronously.
 *
 * @param { number | File } file - File object or FD of the file to close.
 * <br>Once closed, the File object or FD cannot be used for read or write operations.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function closeSync(file: number | File): void;

/**
 * Copy file or directory.
 *
 * @param { string } srcUri - src uri.
 * @param { string } destUri - dest uri.
 * @param { CopyOptions } [options] - options.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied by the file system
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900021 - File table overflow
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
/**
 * Copies a file or directory. This API uses a promise to return the result.
 * File copy across devices is supported. This API forcibly overwrites the file or directory.
 * The input parameter can be the URI of the file or directory. A maximum of 10 cross-device copy tasks are allowed at the same time,
 * and the number of files to be copied at a time cannot exceed 500.
 *
 * @param { string } srcUri - URI of the file or directory to copy.
 * @param { string } destUri - URI of the destination file or directory.
 * @param { CopyOptions } [options] - Callback invoked to provide the copy progress.
 * <br>If this parameter is not set, the callback will not be invoked.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied by the file system
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900021 - File table overflow
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function copy(srcUri: string, destUri: string, options?: CopyOptions): Promise<void>;

/**
 * Copies a file or directory. This API uses an asynchronous callback to return the result.
 * File copy across devices is supported. This API forcibly overwrites the file or directory. The file or directory URI is supported.
 * A maximum of 10 cross-device copy tasks are allowed at the same time,
 * and the number of files to be copied at a time cannot exceed 500.
 *
 * @param { string } srcUri - URI of the file or directory to copy.
 * @param { string } destUri - URI of the destination file or directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied by the file system
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900021 - File table overflow
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare function copy(srcUri: string, destUri: string, callback: AsyncCallback<void>): void;

/**
 * Copies a file or directory. This API uses an asynchronous callback to return the result.
 * File copy across devices is supported. This API forcibly overwrites the file or directory. The file or directory URI is supported.
 * A maximum of 10 cross-device copy tasks are allowed at the same time,
 * and the number of files to be copied at a time cannot exceed 500.
 *
 * @param { string } srcUri - URI of the file or directory to copy.
 * @param { string } destUri - URI of the destination file or directory.
 * @param { CopyOptions } options - Callback used to return the copy progress.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied by the file system
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900021 - File table overflow
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare function copy(srcUri: string, destUri: string, options: CopyOptions, callback: AsyncCallback<void>): void;

/**
 * Copy directory.
 *
 * @param { string } src - source path.
 * @param { string } dest - destination path.
 * @param { number } [mode = 0] - mode.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
/**
 * Copies the source directory to the destination directory. This API uses a promise to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination folder.
 * @param { number } [mode = 0] - Copy mode. The default value is 0.
 * <br>0: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory will be moved
 * <br>to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>1: Forcibly overwrite the files with the same name in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function copyDir(src: string, dest: string, mode?: number): Promise<void>;

/**
 * Copies the source directory to the destination directory.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function copyDir(src: string, dest: string, callback: AsyncCallback<void>): void;

/**
 * Copies the source directory to the destination directory.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { AsyncCallback<void, Array<ConflictFiles>> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900015 - File exists
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function copyDir(src: string, dest: string, callback: AsyncCallback<void, Array<ConflictFiles>>): void;


/**
 * Copies the source directory to the destination directory. You can set the copy mode.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } mode - Copy mode. The default value is 0.
 * <br>0: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory will be moved
 * <br>to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>1: Forcibly overwrite the files with the same name in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function copyDir(src: string, dest: string, mode: number, callback: AsyncCallback<void>): void;

/**
 * Copies the source directory to the destination directory. You can set the copy mode.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } mode - Copy mode. The default value is 0.
 * <br>0: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory will be moved
 * <br>to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>1: Forcibly overwrite the files with the same name in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * @param { AsyncCallback<void, Array<ConflictFiles>> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900015 - File exists
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function copyDir(src: string, dest: string, mode: number, callback: AsyncCallback<void, Array<ConflictFiles>>): void;

/**
 * Copy directory with sync interface.
 *
 * @param { string } src - source path.
 * @param { string } dest - destination path.
 * @param { number } [mode = 0] - mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
/**
 * Copies the source directory to the destination directory. This API returns the result synchronously.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the source directory.
 * @param { number } [mode = 0] - Copy mode. The default value is 0.
 * <br>0: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory will be moved
 * <br>to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>1: Forcibly overwrite the files with the same name in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function copyDirSync(src: string, dest: string, mode?: number): void;

/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Copies a file. This API uses a promise to return the result.
 *
 * @param { string | number } src - Path or FD of the file to copy.
 * @param { string | number } dest - Destination path of the file or FD of the file created.
 * @param { number } [mode = 0] - Whether to overwrite the file with the same name in the destination directory.
 * <br>The default value is 0, which is the only value supported.
 * <br>0: overwrite the file with the same name and truncate the part that is not overwritten.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function copyFile(src: string | number, dest: string | number, mode?: number): Promise<void>;

/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Copies a file. This API overwrites the file with the same name in the destination directory and truncates the part that is not overwritten.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string | number } src - Path or FD of the file to copy.
 * @param { string | number } dest - Destination path of the file or FD of the file created.
 * @param { AsyncCallback<void> } callback - Callback invoked immediately after the file is copied.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function copyFile(src: string | number, dest: string | number, callback: AsyncCallback<void>): void;

/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Copy file.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Copies a file. This API overwrites the file with the same name in the destination directory
 * and truncates the part that is not overwritten. This API uses an asynchronous callback to return the result.
 *
 * @param { string | number } src - Path or FD of the file to copy.
 * @param { string | number } dest - Destination path of the file or FD of the file created.
 * @param { number } [mode = 0] - Whether to overwrite the file with the same name in the destination directory.
 * <br>The default value is 0, which is the only value supported.
 * <br>0: overwrite the file with the same name and truncate the part that is not overwritten.
 * @param { AsyncCallback<void> } callback - Callback invoked immediately after the file is copied.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function copyFile(
  src: string | number,
  dest: string | number,
  mode: number,
  callback: AsyncCallback<void>
): void;

/**
 * Copy file with sync interface.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Copy file with sync interface.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Copy file with sync interface.
 *
 * @param { string | number } src - src.
 * @param { string | number } dest - dest.
 * @param { number } [mode = 0] - mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Copies a file. This API returns the result synchronously.
 *
 * @param { string | number } src - Path or FD of the file to copy.
 * @param { string | number } dest - Destination path of the file or FD of the file created.
 * @param { number } [mode = 0] - Whether to overwrite the file with the same name in the destination directory.
 * <br>The default value is 0, which is the only value supported.
 * <br>0: overwrite the file with the same name and truncate the part that is not overwritten.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function copyFileSync(src: string | number, dest: string | number, mode?: number): void;

/**
 * Create class Stream.
 *
 * @param { string } path - path.
 * @param { string } mode - mode.
 * @returns { Promise<Stream> } return Promise
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a stream based on a file path. This API uses a promise to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Promise<Stream> } Promise used to return the stream opened.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
/**
 * Creates a stream based on a file path. This API uses a promise to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Promise<Stream> } Promise used to return the stream opened.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function createStream(path: string, mode: string): Promise<Stream>;

/**
 * Creates a stream based on a file path. This API uses an asynchronous callback to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @param { AsyncCallback<Stream> } callback - Callback used to return the result.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a stream based on a file path. This API uses an asynchronous callback to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @param { AsyncCallback<Stream> } callback - Callback used to return the result.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function createStream(path: string, mode: string, callback: AsyncCallback<Stream>): void;

/**
 * Create class Stream with sync interface.
 *
 * @param { string } path - path.
 * @param { string } mode - mode.
 * @returns { Stream } createStream
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a stream based on a file path. This API returns the result synchronously.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Stream } Stream opened.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
/**
 * Creates a stream based on a file path. This API returns the result synchronously.
 * To close the stream, use close() of Stream.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file.
 * <br>If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Stream } Stream opened.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function createStreamSync(path: string, mode: string): Stream;


/**
 * Create class RandomAccessFile.
 *
 * @param { string | File } file - file path, object.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { Promise<RandomAccessFile> } Returns the RandomAccessFile object which has been created in promise mode.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
/**
 * Creates a RandomAccessFile instance based on a file path or file object.
 * This API uses a promise to return the result.
 *
 * @param { string | File } file - Application sandbox path of the file or an opened file object.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for creating the RandomAccessFile instance.
 * <br>This parameter is valid only when the application sandbox path of the file is passed in.
 * <br>One of the following options must be specified:
 * <br>OpenMode.READ_ONLY(0o0): Create the file in read-only mode. This is the default value.
 * <br>OpenMode.WRITE_ONLY(0o1): Create the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Create the file in read/write mode.
 * <br>You can also specify the following options, separated by a bitwise OR operator (|). By default,
 * <br>no additional options are given.
 * <br>OpenMode.CREATE(0o100): If the file does not exist, create it.
 * <br>OpenMode.TRUNC(0o1000): If the RandomAccessFile object already exists and is created in write mode,
 * <br>truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Create the file in append mode. New data will be added to the end
 * <br>of the RandomAccessFile object.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the created file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Create a RandomAccessFile instance in synchronous I/O mode.
 * @param { RandomAccessFileOptions } [options] - Defines the options used in createRandomAccessFile().
 * @returns { Promise<RandomAccessFile> } Promise used to return the RandomAccessFile instance created.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function createRandomAccessFile(file: string | File, mode?: number,
  options?: RandomAccessFileOptions): Promise<RandomAccessFile>;

/**
 * Creates a RandomAccessFile object in read-only mode based on a file path or file object.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string | File } file - Application sandbox path of the file or an opened file object.
 * @param { AsyncCallback<RandomAccessFile> } callback - Callback used to return the RandomAccessFile instance created.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function createRandomAccessFile(file: string | File, callback: AsyncCallback<RandomAccessFile>): void;

/**
 * Creates a RandomAccessFile instance based on a file path or file object.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string | File } file - Application sandbox path of the file or an opened file object.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for creating the RandomAccessFile instance.
 * <br>This parameter is valid only when the application sandbox path of the file is passed in.
 * <br>One of the following options must be specified:
 * <br>OpenMode.READ_ONLY(0o0): Create the file in read-only mode. This is the default value.
 * <br>OpenMode.WRITE_ONLY(0o1): Create the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Create the file in read/write mode.
 * <br>You can also specify the following options, separated by a bitwise OR operator (|). By default,
 * <br>no additional options are given.
 * <br>OpenMode.CREATE(0o100): If the file does not exist, create it.
 * <br>OpenMode.TRUNC(0o1000): If the RandomAccessFile object already exists and is created in write mode,
 * <br>truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Create the file in append mode. New data will be added to the end
 * <br>of the RandomAccessFile object.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the created file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Create a RandomAccessFile instance in synchronous I/O mode.
 * @param { AsyncCallback<RandomAccessFile> } callback - Callback used to return the RandomAccessFile instance created.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function createRandomAccessFile(file: string | File, mode: number, callback: AsyncCallback<RandomAccessFile>): void;

/**
 * Create class RandomAccessFile with sync interface.
 *
 * @param { string | File } file - file path, object.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { RandomAccessFile } Returns the RandomAccessFile object which has been created.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
/**
 * Creates a RandomAccessFile instance based on a file path or file object.
 *
 * @param { string | File } file - Application sandbox path of the file or an opened file object.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for creating the RandomAccessFile instance.
 * <br>This parameter is valid only when the application sandbox path of the file is passed in.
 * <br>One of the following options must be specified:
 * <br>OpenMode.READ_ONLY(0o0): Create the file in read-only mode. This is the default value.
 * <br>OpenMode.WRITE_ONLY(0o1): Create the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Create the file in read/write mode.
 * <br>You can also specify the following options, separated by a bitwise OR operator (|). By default,
 * <br>no additional options are given.
 * <br>OpenMode.CREATE(0o100): If the file does not exist, create it.
 * <br>OpenMode.TRUNC(0o1000): If the RandomAccessFile object already exists and is created in write mode,
 * <br>truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Create the file in append mode. New data will be added to the end
 * <br>of the RandomAccessFile object.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the created file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Create a RandomAccessFile instance in synchronous I/O mode.
 * @param { RandomAccessFileOptions } [options] - Defines the options used in createRandomAccessFile().
 * @returns { RandomAccessFile } RandomAccessFile instance created.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function createRandomAccessFileSync(file: string | File, mode?: number,
  options?: RandomAccessFileOptions): RandomAccessFile;

/**
 * Creates a readable stream. This API returns the result synchronously.
 *
 * @param { string } path - Path of the file.
 * @param { ReadStreamOptions } [options] - Defines the options used in createReadStream().
 * @returns { ReadStream } ReadStream instance obtained.
 * @throws { BusinessError } 401 - Parameter error
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900017 - No such device
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
/**
 * Creates a readable stream. This API returns the result synchronously.
 *
 * @param { string } path - Path of the file.
 * @param { ReadStreamOptions } [options] - The options are as follows:
 * <br>start (number): start position of the data to read in the file. This parameter is optional.
 * <br>By default, data is read from the current position.
 * <br>end (number): end position of the data to read in the file. This parameter is optional.
 * <br>The default value is the end of the file.
 * @returns { ReadStream } ReadStream instance obtained.
 * @throws { BusinessError } 401 - Parameter error
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900017 - No such device
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 20
 */
declare function createReadStream(path: string, options?: ReadStreamOptions): ReadStream;

/**
 * Creates a writeable stream. This API returns the result synchronously.
 *
 * @param { string } path - Path of the file.
 * @param { WriteStreamOptions } [options] - Defines the options used in createWriteStream().
 * @returns { WriteStream } ReadStream instance obtained.
 * @throws { BusinessError } 401 - Parameter error
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900017 - No such device
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
/**
 * Creates a writeable stream. This API returns the result synchronously.
 *
 * @param { string } path - Path of the file.
 * @param { WriteStreamOptions } [options] - The options are as follows:
 * <br>start (number): start position to write the data in the file. This parameter is optional.
 * <br>By default, data is written from the current position.
 * <br>mode (number): mode for creating the writeable stream. This parameter is optional.
 * <br>The default value is the write-only mode.
 * @returns { WriteStream } ReadStream instance obtained.
 * @throws { BusinessError } 401 - Parameter error
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900017 - No such device
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 20
 */
declare function createWriteStream(path: string, options?: WriteStreamOptions): WriteStream;

/**
 * Creates a Watcher object to listen for file or directory changes.
 *
 * @param { string } path - Application sandbox path of the file or directory to observe.
 * @param { number } events - Events to observe. Multiple events can be separated by a bitwise OR operator (|).
 * @param { WatchEventListener } listener - Callback invoked when an observed event occurs.
 * <br>The callback will be invoked each time an observed event occurs.
 * @returns { Watcher } Watcher object created.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900021 - File table overflow
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function createWatcher(path: string, events: number, listener: WatchEventListener): Watcher;

/**
 * Opens a File object based on an FD.
 *
 * @param { number } fd - FD of the file.
 * @returns { File } File object opened.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function dup(fd: number): File;

/**
 * Synchronize file metadata.
 *
 * @param { number } fd - fd.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the data of a file. This API uses a promise to return the result.
 *
 * @param { number } fd - FD of the file.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fdatasync(fd: number): Promise<void>;

/**
 * Synchronize file metadata.
 *
 * @param { number } fd - fd.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the data (excluding the metadata) of a file.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fdatasync(fd: number, callback: AsyncCallback<void>): void;

/**
 * Synchronize file metadata with sync interface.
 *
 * @param { number } fd - fd.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the data of a file. This API returns the result synchronously.
 *
 * @param { number } fd - FD of the file.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fdatasyncSync(fd: number): void;

/**
 * Opens a stream based on an FD. This API uses a promise to return the result. To close the stream,
 * use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Promise<Stream> } Returns the Stream object in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Opens a stream based on an FD. This API uses a promise to return the result. To close the stream,
 * use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Promise<Stream> } Returns the Stream object in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function fdopenStream(fd: number, mode: string): Promise<Stream>;

/**
 * Opens a stream based on an FD. This API uses an asynchronous callback to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @param { AsyncCallback<Stream> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Opens a stream based on an FD. This API uses an asynchronous callback to return the result.
 * To close the stream, use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @param { AsyncCallback<Stream> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function fdopenStream(fd: number, mode: string, callback: AsyncCallback<Stream>): void;

/**
 * Opens a stream based on an FD. This API returns the result synchronously. To close the stream, use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Stream } Returns the Stream object.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Opens a stream based on an FD. This API returns the result synchronously. To close the stream, use close() of Stream.
 *
 * @param { number } fd - FD of the file.
 * @param { string } mode - r: Open a file for reading. The file must exist.
 * <br>r+: Open a file for both reading and writing. The file must exist.
 * <br>w: Open a file for writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>w+: Open a file for both reading and writing. If the file exists, clear its content. If the file does not exist, create a file.
 * <br>a: Open a file in append mode for writing at the end of the file. If the file does not exist, create a file.
 * <br>If the file exists, write data to the end of the file (the original content of the file is reserved).
 * <br>a+: Open a file in append mode for reading or updating at the end of the file. If the file does not exist,
 * <br>create a file. If the file exists, write data to the end of the file (the original content of the file is reserved).
 * @returns { Stream } Returns the Stream object.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900006 - No such device or address
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare function fdopenStreamSync(fd: number, mode: string): Stream;

/**
 * Synchronize file.
 *
 * @param { number } fd - fd.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the cached data of a file to storage. This API uses a promise to return the result.
 *
 * @param { number } fd - FD of the file.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fsync(fd: number): Promise<void>;

/**
 * Synchronize file.
 *
 * @param { number } fd - fd.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the cached data of a file to storage. This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fsync(fd: number, callback: AsyncCallback<void>): void;

/**
 * Synchronize file with sync interface.
 *
 * @param { number } fd - fd.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Synchronizes the cached data of a file to storage. This API returns the result synchronously.
 *
 * @param { number } fd - FD of the file.
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function fsyncSync(fd: number): void;

/**
 * List file.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @returns { Promise<string[]> } Returns an Array containing the name of files or directories that meet the filter criteria in promise mode.
 *      If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * List file.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @returns { Promise<string[]> } Returns an Array containing the name of files or directories that meet the filter criteria.
 *      If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Lists all file names in a directory. This API supports recursive listing of all file names and file filtering.
 * The returned result starts with a slash (/) and contains the subdirectory. This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { ListFileOptions } [options] - Options for filtering files. The files are not filtered by default.
 * @returns { Promise<string[]> } Promise used to return the file names listed.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function listFile(
  path: string,
  options?: ListFileOptions
): Promise<string[]>;

/**
 * List file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<string[]> } callback - The callback is used to return an Array containing the name of files or directories
 *      that meet the filter criteria in promise mode. If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * List file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<string[]> } callback - The callback is used to return an Array containing the name of files or directories
 *      that meet the filter criteria in promise mode. If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Lists all file names in a directory. This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { AsyncCallback<string[]> } callback - Options for filtering files. The files are not filtered by default
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function listFile(path: string, callback: AsyncCallback<string[]>): void;

/**
 * List file.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @param { AsyncCallback<string[]> } callback - The callback is used to return an Array containing the name of files or directories
 *      that meet the filter criteria in promise mode. If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * List file.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @param { AsyncCallback<string[]> } callback - The callback is used to return an Array containing the name of files or directories
 *      that meet the filter criteria in promise mode. If present, Include the subdirectory structure.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Lists all file names in a directory. This API supports recursive listing of all file names and file filtering.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { ListFileOptions } [options] - Options for filtering files. The files are not filtered by default.
 * @param { AsyncCallback<string[]> } callback - Callback used to return the file names listed.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function listFile(
  path: string,
  options: ListFileOptions,
  callback: AsyncCallback<string[]>
): void;

/**
 * List file with sync interface.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @returns { string[] } Returns an Array containing the name of files or directories that meet the filter criteria.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * List file with sync interface.
 *
 * @param { string } path - path.
 * @param { object } [options] - options.
 * @returns { string[] } Returns an Array containing the name of files or directories that meet the filter criteria.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Lists all file names in a directory. This API returns the result synchronously.
 * This API supports recursive listing of all file names and file filtering.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { ListFileOptions } [options] - Options for filtering files. The files are not filtered by default.
 * @returns { string[] } List of the file names obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function listFileSync(
  path: string,
  options?: ListFileOptions
): string[];

/**
 *  Reposition file offset.
 *
 * @param { number } fd - file descriptor.
 * @param { number } offset - file offset.
 * @param { WhenceType } [whence = WhenceType.SEEK_SET] - directive whence.
 * @returns { number } Returns the file offset relative to starting position of file.
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900026 - Illegal seek
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
/**
 * Adjusts the position of the file offset pointer.
 *
 * @param { number } fd - FD of the file.
 * @param { number } offset - Number of bytes to move the offset.
 * @param { WhenceType } [whence = WhenceType.SEEK_SET] - Where to start the offset.
 * <br>If this parameter is not specified, the file start position is used by default.
 * @returns { number } Returns the file offset relative to starting position of file.
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900026 - Illegal seek
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 12
 */
declare function lseek(fd: number, offset: number, whence?: WhenceType): number;

/**
 * Obtains information about a symbolic link that is used to refer to a file or directory.
 * This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the file.
 * @returns { Promise<Stat> } Promise used to return the symbolic link information obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function lstat(path: string): Promise<Stat>;

/**
 * Obtains information about a symbolic link that is used to refer to a file or directory.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { AsyncCallback<Stat> } callback - Callback used to return the symbolic link information obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function lstat(path: string, callback: AsyncCallback<Stat>): void;

/**
 * Obtains information about a symbolic link that is used to refer to a file or directory.
 * This API returns the result synchronously.
 *
 * @param { string } path - Application sandbox path of the file.
 * @returns { Stat } File information obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function lstatSync(path: string): Stat;

/**
 * Make dir.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Make dir.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Creates a directory. This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function mkdir(path: string): Promise<void>;

/**
 * Creates a directory. This API uses a promise to return the result. The value true means to create a directory recursively.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { boolean } recursion - Whether to create a directory recursively.
 * <br>The value true means to create a directory recursively. The value false means to create a single-level directory.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
declare function mkdir(path: string, recursion: boolean): Promise<void>;

/**
 * Make dir.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Make dir.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Creates a directory. This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function mkdir(path: string, callback: AsyncCallback<void>): void;

/**
 * Creates a directory. This API uses an asynchronous callback to return the result.
 * The value true means to create a directory recursively.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { boolean } recursion - Whether to create a directory recursively.
 * <br>The value true means to create a directory recursively. The value false means to create a single-level directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
declare function mkdir(path: string, recursion: boolean, callback: AsyncCallback<void>): void;

/**
 * Make dir with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Make dir with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Creates a directory. This API returns the result synchronously.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function mkdirSync(path: string): void;

/**
 * Creates a directory. This API returns the result synchronously. The value true means to create a directory recursively.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { boolean } recursion - Whether to create a directory recursively.
 * <br>The value true means to create a directory recursively. The value false means to create a single-level directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
declare function mkdirSync(path: string, recursion: boolean): void;

/**
 * Make temp dir.
 *
 * @param { string } prefix - dir prefix.
 * @returns { Promise<string> } Returns the path to the new directory in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a temporary directory. This API uses a promise to return the result.
 *
 * @param { string } prefix - String to be replaced with six randomly generated characters to create a unique temporary directory.
 * @returns { Promise<string> } Promise used to return the directory created.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function mkdtemp(prefix: string): Promise<string>;

/**
 * Make temp dir.
 *
 * @param { string } prefix - dir prefix.
 * @param { AsyncCallback<string> } callback - The callback is used to return the path to the new directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a temporary directory. This API uses an asynchronous callback to return the result.
 * The directory name is created by replacing a string (specified by prefix) with six randomly generated characters.
 *
 * @param { string } prefix - String to be replaced with six randomly generated characters to create a unique temporary directory.
 * @param { AsyncCallback<string> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function mkdtemp(prefix: string, callback: AsyncCallback<string>): void;

/**
 * Make temp dir with sync interface.
 *
 * @param { string } prefix - dir prefix.
 * @returns { string } Returns the path to the new directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Creates a temporary directory. This API returns the result synchronously.
 * The directory name is created by replacing a string (specified by prefix) with six randomly generated characters.
 *
 * @param { string } prefix - String to be replaced with six randomly generated characters to create a unique temporary directory.
 * @returns { string } Unique path generated.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function mkdtempSync(prefix: string): string;

/**
 * Moves the source directory to the destination directory. This API uses a promise to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } [mode = 0] - Move mode. The default value is 0.
 * <br>0: Throw an exception if a directory conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a non-empty directory with the same name as the source directory.
 * <br>1: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory
 * <br>will be moved to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>2: Forcibly overwrite the conflicting files in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * <br>3: Forcibly overwrite the conflicting directory.
 * <br>The source directory is moved to the destination directory, and the content of the moved directory is the
 * <br>same as that of the source directory. If the destination directory contains a directory with the same name
 * <br>as the source directory, all original files in the directory will be deleted.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDir(src: string, dest: string, mode?: number): Promise<void>;

/**
 * Moves the source directory to the destination directory. This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDir(src: string, dest: string, callback: AsyncCallback<void>): void;

/**
 * Moves the source directory to the destination directory. This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { AsyncCallback<void, Array<ConflictFiles>> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900015 - File exists
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDir(src: string, dest: string, callback: AsyncCallback<void, Array<ConflictFiles>>): void;

/**
 * Moves the source directory to the destination directory. You can set the move mode.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } mode - Move mode. The default value is 0.
 * <br>0: Throw an exception if a directory conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a non-empty directory with the same name as the source directory.
 * <br>1: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory
 * <br>will be moved to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>2: Forcibly overwrite the conflicting files in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * <br>3: Forcibly overwrite the conflicting directory.
 * <br>The source directory is moved to the destination directory, and the content of the moved directory is the
 * <br>same as that of the source directory. If the destination directory contains a directory with the same name
 * <br>as the source directory, all original files in the directory will be deleted.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDir(src: string, dest: string, mode: number, callback: AsyncCallback<void>): void;

/**
 * Moves the source directory to the destination directory. You can set the move mode.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } mode - Move mode. The default value is 0.
 * <br>0: Throw an exception if a directory conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a non-empty directory with the same name as the source directory.
 * <br>1: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory
 * <br>will be moved to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>2: Forcibly overwrite the conflicting files in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * <br>3: Forcibly overwrite the conflicting directory.
 * <br>The source directory is moved to the destination directory, and the content of the moved directory is the
 * <br>same as that of the source directory. If the destination directory contains a directory with the same name
 * <br>as the source directory, all original files in the directory will be deleted.
 * @param { AsyncCallback<void, Array<ConflictFiles>> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900015 - File exists
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDir(src: string, dest: string, mode: number, callback: AsyncCallback<void, Array<ConflictFiles>>): void;

/**
 * Moves the source directory to the destination directory. This API returns the result synchronously.
 *
 * @param { string } src - Application sandbox path of the source directory.
 * @param { string } dest - Application sandbox path of the destination directory.
 * @param { number } [mode = 0] - Move mode. The default value is 0.
 * <br>0: Throw an exception if a directory conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a non-empty directory with the same name as the source directory.
 * <br>1: Throw an exception if a file conflict occurs.
 * <br>An exception will be thrown if the destination directory contains a directory with the same name as the source directory,
 * <br>and a file with the same name exists in the conflict directory. All the non-conflicting files in the source directory
 * <br>will be moved to the destination directory, and the non-conflicting files in the destination directory will be retained.
 * <br>The data attribute in the error returned provides information about the conflicting files in the Array<ConflictFiles> format.
 * <br>2: Forcibly overwrite the conflicting files in the destination directory.
 * <br>When the destination directory contains a directory with the same name as the source directory,
 * <br>the files with the same names in the destination directory are overwritten forcibly;
 * <br>the files without conflicts in the destination directory are retained.
 * <br>3: Forcibly overwrite the conflicting directory.
 * <br>The source directory is moved to the destination directory, and the content of the moved directory is the
 * <br>same as that of the source directory. If the destination directory contains a directory with the same name
 * <br>as the source directory, all original files in the directory will be deleted.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare function moveDirSync(src: string, dest: string, mode?: number): void;

/**
 * Move file.
 *
 * @param { string } src - source file path.
 * @param { string } dest - destination file path.
 * @param { number } [mode = 0] - move mode when duplicate file name exists.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Moves a file. This API uses a promise to return the result.
 *
 * @param { string } src - Application sandbox path of the file to move.
 * @param { string } dest - Application sandbox path of the destination file.
 * @param { number } [mode = 0] - Move mode.
 * <br>The value 0 means to overwrite the file with the same name in the destination directory;
 * <br>the value 1 means to throw an exception. The default value is 0.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function moveFile(src: string, dest: string, mode?: number): Promise<void>;

/**
 * Move file.
 *
 * @param { string } src - source file path.
 * @param { string } dest - destination file path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Moves a file and forcibly overwrites the file with the same name in the destination directory.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the file to move.
 * @param { string } dest - Application sandbox path of the destination file.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function moveFile(src: string, dest: string, callback: AsyncCallback<void>): void;

/**
 * Move file.
 *
 * @param { string } src - source file path.
 * @param { string } dest - destination file path.
 * @param { number } [mode = 0] - move mode when duplicate file name exists.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Moves a file with the specified mode. This API uses an asynchronous callback to return the result.
 *
 * @param { string } src - Application sandbox path of the file to move.
 * @param { string } dest - Application sandbox path of the destination to move.
 * @param { number } [mode = 0] - Move mode.
 * <br>The value 0 means to overwrite the file with the same name in the destination directory;
 * <br>the value 1 means to throw an exception. The default value is 0.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function moveFile(src: string, dest: string, mode: number, callback: AsyncCallback<void>): void;

/**
 * Move file with sync interface.
 *
 * @param { string } src - source file path.
 * @param { string } dest - destination file path.
 * @param { number } [mode = 0] - move mode when duplicate file name exists.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Moves a file. This API returns the result synchronously.
 *
 * @param { string } src - Application sandbox path of the file to move.
 * @param { string } dest - Application sandbox path of the destination to move.
 * @param { number } [mode = 0] - Move mode.
 * <br>The value 0 means to overwrite the file with the same name in the destination directory;
 * <br>the value 1 means to throw an exception. The default value is 0.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
declare function moveFileSync(src: string, dest: string, mode?: number): void;

/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { Promise<File> } Returns the File object in Promise mode to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { Promise<File> } Returns the File object in Promise mode to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { Promise<File> } Returns the File object in Promise mode to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Opens a file or directory. This API uses a promise to return the result. This API supports the use of a URI.
 *
 * @param { string } path - Application sandbox path or URI of the file.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for opening the file.
 * <br>You must specify one of the following options. By default, the file is opened in read-only mode.
 * <br>OpenMode.READ_ONLY(0o0): Open the file in read-only mode.
 * <br>OpenMode.WRITE_ONLY(0o1): Open the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Open the file in read/write mode.
 * <br>You can add the following function options in bitwise OR mode. By default, no additional option is added.
 * <br>OpenMode.CREATE(0o100): Create a file if the file does not exist.
 * <br>OpenMode.TRUNC(0o1000): If the file exists and is opened in write mode, truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Open the file in append mode. New data will be added to the end of the file.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the opened file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Open the file in synchronous I/O mode.
 * @returns { Promise<File> } Promise used to return the File object.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function open(path: string, mode?: number): Promise<File>;

/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<File> } callback - The callback is used to return the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<File> } callback - The callback is used to return the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Opens a file or directory. This API uses an asynchronous callback to return the result. This API supports the use of a URI.
 *
 * @param { string } path - Application sandbox path or URI of the file.
 * @param { AsyncCallback<File> } callback - Callback used to return the result.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function open(path: string, callback: AsyncCallback<File>): void;

/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @param { AsyncCallback<File> } callback - The callback is used to return the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Open file.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @param { AsyncCallback<File> } callback - The callback is used to return the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Opens a file or directory with the specified mode. This API uses an asynchronous callback to return the result.
 * This API supports the use of a URI.
 *
 * @param { string } path - Application sandbox path or URI of the file.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for opening the file.
 * <br>You must specify one of the following options. By default, the file is opened in read-only mode.
 * <br>OpenMode.READ_ONLY(0o0): Open the file in read-only mode.
 * <br>OpenMode.WRITE_ONLY(0o1): Open the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Open the file in read/write mode.
 * <br>You can add the following function options in bitwise OR mode. By default, no additional option is added.
 * <br>OpenMode.CREATE(0o100): Create a file if the file does not exist.
 * <br>OpenMode.TRUNC(0o1000): If the file exists and is opened in write mode, truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Open the file in append mode. New data will be added to the end of the file.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the opened file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Open the file in synchronous I/O mode.
 * @param { AsyncCallback<File> } callback - The callback is used to return the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function open(path: string, mode: number, callback: AsyncCallback<File>): void;

/**
 * Open file with sync interface.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { File } Returns the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Open file with sync interface.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { File } Returns the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Open file with sync interface.
 *
 * @param { string } path - path.
 * @param { number } [mode = OpenMode.READ_ONLY] - mode.
 * @returns { File } Returns the File object to record the file descriptor.
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
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Opens a file or directory. This API returns the result synchronously. This API supports the use of a URI.
 *
 * @param { string } path - Application sandbox path or file URI of the file to open.
 * @param { number } [mode = OpenMode.READ_ONLY] - Mode for opening the file.
 * <br>You must specify one of the following options. By default, the file is opened in read-only mode.
 * <br>OpenMode.READ_ONLY(0o0): Open the file in read-only mode.
 * <br>OpenMode.WRITE_ONLY(0o1): Open the file in write-only mode.
 * <br>OpenMode.READ_WRITE(0o2): Open the file in read/write mode.
 * <br>You can add the following function options in bitwise OR mode. By default, no additional option is added.
 * <br>OpenMode.CREATE(0o100): Create a file if the file does not exist.
 * <br>OpenMode.TRUNC(0o1000): If the file exists and is opened in write mode, truncate the file length to 0.
 * <br>OpenMode.APPEND(0o2000): Open the file in append mode. New data will be added to the end of the file.
 * <br>OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
 * <br>or character special file, perform non-blocking operations on the opened file and in subsequent I/Os.
 * <br>OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
 * <br>OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
 * <br>OpenMode.SYNC(0o4010000): Open the file in synchronous I/O mode.
 * @returns { File } Returns the File object to record the file descriptor.
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
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function openSync(path: string, mode?: number): File;

/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { ReadOptions } [options] - options.
 * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Reads file data. This API uses a promise to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer } buffer - Buffer used to store the file data read.
 * @param { ReadOptions } [options] - The options are as follows:
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
 * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
 * <br>This parameter is optional. By default, data is read from the filePointer.
 * @returns { Promise<number> } Promise used to return the length of the data read, in bytes.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function read(
  fd: number,
  buffer: ArrayBuffer,
  options?: ReadOptions
): Promise<number>;

/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Reads data from a file. This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer } buffer - Buffer used to store the file data read.
 * @param { AsyncCallback<number> } callback - Callback used to return the length of the data read, in bytes.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function read(fd: number, buffer: ArrayBuffer, callback: AsyncCallback<number>): void;

/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Reads data from a file. This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer } buffer - Buffer used to store the file data read.
 * @param { ReadOptions } [options] - The options are as follows:
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
 * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
 * <br>This parameter is optional. By default, data is read from the filePointer.
 * @param { AsyncCallback<number> } callback - Callback used to return the length of the data read, in bytes.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function read(
  fd: number,
  buffer: ArrayBuffer,
  options: ReadOptions,
  callback: AsyncCallback<number>
): void;

/**
 * Read file with sync interface.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { number } Returns the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read file with sync interface.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { number } Returns the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Read file with sync interface.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer } buffer - buffer.
 * @param { ReadOptions } [options] - options.
 * @returns { number } Returns the number of file bytes read to buffer.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Reads data from a file. This API returns the result synchronously.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer } buffer - Buffer used to store the file data read.
 * @param { ReadOptions } [options] - The options are as follows:
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
 * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
 * <br>This parameter is optional. By default, data is read from the filePointer.
 * @returns { number } Length of the data read, in bytes.
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function readSync(
  fd: number,
  buffer: ArrayBuffer,
  options?: ReadOptions
): number;

/**
 * Read content line by line.
 *
 * @param { string } filePath - file path.
 * @param { Options } [options] - optional parameters
 * @returns { Promise<ReaderIterator> } Returns the iterator object in promise mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
/**
 * Reads the text content of a file line by line. This API uses a promise to return the result.
 * Only the files in UTF-8 format are supported.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { Options } [options] - Options for reading the text. The options are as follows:
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @returns { Promise<ReaderIterator> } Promise used to return a ReaderIterator object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function readLines(filePath: string, options?: Options): Promise<ReaderIterator>;

/**
 * Reads a file text line by line. This API uses an asynchronous callback to return the result.
 * Only the files in UTF-8 format are supported.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { AsyncCallback<ReaderIterator> } callback - Callback used to return a ReaderIterator object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare function readLines(filePath: string, callback: AsyncCallback<ReaderIterator>): void;

/**
 * Reads a file text line by line. This API uses an asynchronous callback to return the result.
 * Only the files in UTF-8 format are supported.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { Options } options - Options for reading the text. The options are as follows:
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @param { AsyncCallback<ReaderIterator> } callback - Callback used to return a ReaderIterator object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare function readLines(filePath: string, options: Options, callback: AsyncCallback<ReaderIterator>): void;

/**
 * Read content line by line with sync interface.
 *
 * @param { string } filePath - file path.
 * @param { Options } [options] - optional parameters
 * @returns { ReaderIterator } Returns the iterator object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
/**
 * Reads the text content of a file line by line. This API returns the result synchronously.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { Options } [options] - Options for reading the text. The options are as follows:
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @returns { ReaderIterator } Returns the iterator object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900022 - Too many open files
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function readLinesSync(filePath: string, options?: Options): ReaderIterator;

/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @returns { Promise<string> } Returns the contents of the read file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @returns { Promise<string> } Returns the contents of the read file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { ReadTextOptions } [options] - options.
 * @returns { Promise<string> } Returns the contents of the read file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Reads the text content of a file. This API uses a promise to return the result.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { ReadTextOptions } [options] - The options are as follows:
 * <br>offset (number): start position to read the data. This parameter is optional. By default, data is read from the current position.
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the file length.
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @returns { Promise<string> } Promise used to return the file content read.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function readText(
  filePath: string,
  options?: ReadTextOptions
): Promise<string>;

/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { AsyncCallback<string> } callback - The callback is used to return the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { AsyncCallback<string> } callback - The callback is used to return the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Reads the text content of a file. This API uses an asynchronous callback to return the result.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { AsyncCallback<string> } callback - Callback used to return the content read.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function readText(filePath: string, callback: AsyncCallback<string>): void;

/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @param { AsyncCallback<string> } callback - The callback is used to return the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read text.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @param { AsyncCallback<string> } callback - The callback is used to return the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Reads the text content of a file. This API uses an asynchronous callback to return the result.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { ReadTextOptions } [options] - The options are as follows:
 * <br>offset (number): start position to read the data. This parameter is optional. By default, data is read from the current position.
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the file length.
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @param { AsyncCallback<string> } callback - Callback used to return the content read.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function readText(
  filePath: string,
  options: ReadTextOptions,
  callback: AsyncCallback<string>
): void;

/**
 * Read text with sync interface.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @returns { string } Returns the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Read text with sync interface.
 *
 * @param { string } filePath - file path.
 * @param { object } [options] - options.
 * @returns { string } Returns the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Read text with sync interface.
 *
 * @param { string } filePath - file path.
 * @param { ReadTextOptions } [options] - options.
 * @returns { string } Returns the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
/**
 * Reads the text of a file. This API returns the result synchronously.
 *
 * @param { string } filePath - Application sandbox path of the file.
 * @param { ReadTextOptions } [options] - The options are as follows:
 * <br>offset (number): start position to read the data. This parameter is optional. By default, data is read from the current position.
 * <br>length (number): length of the data to read. This parameter is optional. The default value is the file length.
 * <br>encoding (string): format of the data to be encoded. It is valid only when the data is of the string type.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @returns { string } Returns the contents of the read file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @throws { BusinessError } 13900044 - Network is unreachable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare function readTextSync(
  filePath: string,
  options?: ReadTextOptions
): string;

/**
 * Rename file.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Rename file.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Renames a file or directory. This API uses a promise to return the result.
 *
 * @param { string } oldPath - Application sandbox path of the file or directory to rename.
 * @param { string } newPath - Application sandbox path of the renamed file or directory.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function rename(oldPath: string, newPath: string): Promise<void>;

/**
 * Rename file.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @param { AsyncCallback<void> } callback - Returns the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Rename file.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @param { AsyncCallback<void> } callback - Returns the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Renames a file or directory. This API uses an asynchronous callback to return the result.
 *
 * @param { string } oldPath - Application sandbox path of the file or directory to rename.
 * @param { string } newPath - Application sandbox path of the renamed file or directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function rename(oldPath: string, newPath: string, callback: AsyncCallback<void>): void;

/**
 * Rename file with sync interface.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Rename file with sync interface.
 *
 * @param { string } oldPath - oldPath.
 * @param { string } newPath - newPath.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Renames a file or directory. This API returns the result synchronously.
 *
 * @param { string } oldPath - Application sandbox path of the file or directory to rename.
 * @param { string } newPath - Application sandbox path of the renamed file or directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900016 - Cross-device link
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900028 - Too many links
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function renameSync(oldPath: string, newPath: string): void;

/**
 * Delete dir.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete dir.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Removes a directory. This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function rmdir(path: string): Promise<void>;

/**
 * Delete dir.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete dir.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Removes a directory. This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function rmdir(path: string, callback: AsyncCallback<void>): void;

/**
 * Delete dir with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete dir with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Removes a directory. This API returns the result synchronously.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system1
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900032 - Directory not empty
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function rmdirSync(path: string): void;

/**
 * Get file information.
 *
 * @param { string | number } file - path or file descriptor.
 * @returns { Promise<Stat> } Returns the Stat object in promise mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Get file information.
 *
 * @param { string | number } file - path or file descriptor.
 * @returns { Promise<Stat> } Returns the Stat object in promise mode.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Obtains detailed attribute information of a file or directory. This API uses a promise to return the result.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @returns { Promise<Stat> } Promise used to return detailed file information.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function stat(file: string | number): Promise<Stat>;

/**
 * Get file information.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { AsyncCallback<Stat> } callback - The callback is used to return the Stat object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Get file information.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { AsyncCallback<Stat> } callback - The callback is used to return the Stat object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Obtains detailed attribute information of a file or directory.
 * This API uses an asynchronous callback to return the result.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @param { AsyncCallback<Stat> } callback - Callback used to return the file information obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function stat(file: string | number, callback: AsyncCallback<Stat>): void;

/**
 * Get file information with sync interface.
 *
 * @param { string | number } file - path or file descriptor.
 * @returns { Stat } Returns the Stat object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Get file information with sync interface.
 *
 * @param { string | number } file - path or file descriptor.
 * @returns { Stat } Returns the Stat object.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Obtains detailed attribute information of a file or directory. This API returns the result synchronously.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @returns { Stat } File information obtained.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function statSync(file: string | number): Stat;

/**
 * Creates a symbolic link based on a file path. This API uses a promise to return the result.
 *
 * @param { string } target - Application sandbox path of the target file.
 * @param { string } srcPath - Application sandbox path of the symbolic link.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function symlink(target: string, srcPath: string): Promise<void>;

/**
 * Creates a symbolic link based on a file path. This API uses an asynchronous callback to return the result.
 *
 * @param { string } target - Application sandbox path of the target file.
 * @param { string } srcPath - Application sandbox path of the symbolic link.
 * @param { AsyncCallback<void> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function symlink(target: string, srcPath: string, callback: AsyncCallback<void>): void;

/**
 * Creates a symbolic link based on a file path. This API returns the result synchronously.
 *
 * @param { string } target - Application sandbox path of the target file.
 * @param { string } srcPath - Application sandbox path of the symbolic link.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900015 - File exists
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
declare function symlinkSync(target: string, srcPath: string): void;

/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Truncates a file. This API uses a promise to return the result.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @param { number } [len = 0] - File length, in bytes, after truncation. The default value is 0.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function truncate(file: string | number, len?: number): Promise<void>;

/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Truncates a file. This API uses an asynchronous callback to return the result.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @param { AsyncCallback<void> } callback - Callback that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function truncate(file: string | number, callback: AsyncCallback<void>): void;

/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Truncate file.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Truncates a file. This API uses an asynchronous callback to return the result.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @param { number } len - File length, in bytes, after truncation.
 * @param { AsyncCallback<void> } callback - Callback that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function truncate(file: string | number, len: number, callback: AsyncCallback<void>): void;

/**
 * Truncate file with sync interface.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Truncate file with sync interface.
 *
 * @param { string | number } file - path or file descriptor.
 * @param { number } [len = 0] - len.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Truncates the file content. This API returns the result synchronously.
 *
 * @param { string | number } file - Application sandbox path or FD of the file.
 * @param { number } [len = 0] - File length, in bytes, after truncation. The default value is 0.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900023 - Text file busy
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function truncateSync(file: string | number, len?: number): void;

/**
 * Delete file.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete file.
 *
 * @param { string } path - path.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Deletes a file. This API uses a promise to return the result.
 *
 * @param { string } path - Application sandbox path of the file.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function unlink(path: string): Promise<void>;

/**
 * Delete file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete file.
 *
 * @param { string } path - path.
 * @param { AsyncCallback<void> } callback - Return the callback function.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Deletes a file. This API uses an asynchronous callback to return the result.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { AsyncCallback<void> } callback - Callback invoked immediately after the file is deleted.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function unlink(path: string, callback: AsyncCallback<void>): void;

/**
 * Delete file with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Delete file with sync interface.
 *
 * @param { string } path - path.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Deletes a file. This API returns the result synchronously.
 *
 * @param { string } path - Application sandbox path of the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900014 - Device or resource busy
 * @throws { BusinessError } 13900018 - Not a directory
 * @throws { BusinessError } 13900019 - Is a directory
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900030 - File name too long
 * @throws { BusinessError } 13900033 - Too many symbolic links encountered
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function unlinkSync(path: string): void;

/**
 * Change file mtime.
 *
 * @param { string } path - path.
 * @param { number } mtime - last modification time
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
/**
 * Updates the latest access timestamp of a file.
 *
 * @param { string } path - Application sandbox path of the file.
 * @param { number } mtime - New timestamp. The value is the number of milliseconds elapsed since the Epoch time (00:00:00 UTC on January 1, 1970). 
 * <br>Only the last access time of a file can be modified.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900027 - Read-only file system
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 12
 */
declare function utimes(path: string, mtime: number): void;

/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { Promise<number> } Returns the number of bytes written to the file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { Promise<number> } Returns the number of bytes written to the file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Writes data into a file. This API uses a promise to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
 * @param { WriteOptions } [options] - Defines the options use din write(). It inherits from Options.
 * @returns { Promise<number> } Returns the number of bytes written to the file in promise mode.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function write(
  fd: number,
  buffer: ArrayBuffer | string,
  options?: WriteOptions
): Promise<number>;

/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Writes data to a file. This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
 * @param { AsyncCallback<number> } callback - Callback used to return the result.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function write(fd: number, buffer: ArrayBuffer | string, callback: AsyncCallback<number>): void;

/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Write file.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Writes data to a file. This API uses an asynchronous callback to return the result.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
 * @param { WriteOptions } [options] - The options are as follows:
 * <br>length (number): length of the data to write. This parameter is optional. The default value is the buffer length.
 * <br>offset (number): start position to write the data in the file. This parameter is optional.
 * <br>By default, data is written from the current position.
 * <br>encoding (string): format of the data to be encoded when the data is a string.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function write(
  fd: number,
  buffer: ArrayBuffer | string,
  options: WriteOptions,
  callback: AsyncCallback<number>
): void;

/**
 * Write file with sync interface.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { number } Returns the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Write file with sync interface.
 *
 * @param { number } fd - file descriptor.
 * @param { ArrayBuffer | string } buffer - buffer.
 * @param { object } [options] - options.
 * @returns { number } Returns the number of bytes written to the file.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Writes data to a file. This API returns the result synchronously.
 *
 * @param { number } fd - FD of the file.
 * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
 * @param { WriteOptions } [options] - The options are as follows:
 * <br>length (number): length of the data to write. This parameter is optional. The default value is the buffer length.
 * <br>offset (number): start position to write the data in the file. This parameter is optional.
 * <br>By default, data is written from the current position.
 * <br>encoding (string): format of the data to be encoded when the data is a string.
 * <br>The default value is 'utf-8', which is the only value supported.
 * @returns { number } Length of the data written, in bytes.
 * @throws { BusinessError } 13900001 - Operation not permitted
 * @throws { BusinessError } 13900004 - Interrupted system call
 * @throws { BusinessError } 13900005 - I/O error
 * @throws { BusinessError } 13900008 - Bad file descriptor
 * @throws { BusinessError } 13900010 - Try again
 * @throws { BusinessError } 13900013 - Bad address
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900024 - File too large
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900034 - Operation would block
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare function writeSync(
  fd: number,
  buffer: ArrayBuffer | string,
  options?: WriteOptions
): number;

/**
 * Connect Distributed File System.
 *
 * @permission ohos.permission.DISTRIBUTED_DATASYNC
 * @param { string } networkId - The networkId of device.
 * @param { DfsListeners } listeners - The listeners of Distributed File System.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 201 - Permission denied.
 * @throws { BusinessError } 401 - The parameter check failed.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900045 - Connection failed.
 * @throws { BusinessError } 13900046 - Software caused connection abort.
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function connectDfs(networkId: string, listeners: DfsListeners): Promise<void>;

/**
 * Disconnect Distributed File System.
 *
 * @permission ohos.permission.DISTRIBUTED_DATASYNC
 * @param { string } networkId - The networkId of device.
 * @returns { Promise<void> } The promise returned by the function.
 * @throws { BusinessError } 201 - Permission denied.
 * @throws { BusinessError } 401 - The parameter check failed.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13600004 - Unmount failed.
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function disconnectDfs(networkId: string): Promise<void>;

/**
 * Sets an extended attribute of a file or directory.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { string } key - Key of the extended attribute to obtain.
 * <br>The value is a string of less than 256 bytes and can contain only the user. prefix.
 * @param { string } value -Value of the extended attribute to set.
 * @returns { Promise<void> } Promise that returns no value.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function setxattr(path: string, key: string, value: string): Promise<void>;

/**
 * Sets an extended attribute of a file or directory.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { string } key - Key of the extended attribute to obtain.
 * <br>The value is a string of less than 256 bytes and can contain only the user. prefix.
 * @param { string } value - Value of the extended attribute to set.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900011 - Out of memory
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900020 - Invalid argument
 * @throws { BusinessError } 13900025 - No space left on device
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900041 - Quota exceeded
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function setxattrSync(path: string, key: string, value: string): void;

/**
 * Obtains an extended attribute of a file or directory.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { string } key - Key of the extended attribute to obtain.
 * @returns { Promise<string> } Promise used to return the value of the extended attribute obtained.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900007 - Arg list too long
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900037 - No data available
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function getxattr(path: string, key: string): Promise<string>;

/**
 * Obtains an extended attribute of a file. This API returns the result synchronously.
 *
 * @param { string } path - Application sandbox path of the directory.
 * @param { string } key - Key of the extended attribute to obtain.
 * @returns { string } Value of the extended attribute obtained.
 * @throws { BusinessError } 401 - Parameter error.Possible causes:1.Mandatory parameters are left unspecified;
 * <br>2.Incorrect parameter types.
 * @throws { BusinessError } 13900002 - No such file or directory
 * @throws { BusinessError } 13900007 - Arg list too long
 * @throws { BusinessError } 13900012 - Permission denied
 * @throws { BusinessError } 13900031 - Function not implemented
 * @throws { BusinessError } 13900037 - No data available
 * @throws { BusinessError } 13900038 - Value too large for defined data type
 * @throws { BusinessError } 13900042 - Unknown error
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare function getxattrSync(path: string, key: string): string;

/**
 * Defines the copy progress information.
 *
 * @typedef Progress
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
interface Progress {
  /**
   * Size of the copied data.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  readonly processedSize: number;

  /**
   * Total size of the data to be copied.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  readonly totalSize: number;
}

/**
 * Provides APIs for interrupting a copy task.
 *
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
export class TaskSignal {
  /**
   * Cancels a copy task.
   *
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900012 - Permission denied by the file system
   * @throws { BusinessError } 13900043 - No task can be canceled.
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  cancel(): void;

  /**
   * Subscribes to the event reported when a copy task is canceled.
   *
   * @returns { Promise<string> } Return the result of the cancel event.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  onCancel(): Promise<string>;
}

/**
 * Defines the callback for listening for the copy progress.
 *
 * @typedef CopyOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
interface CopyOptions {
  /**
   * Listener used to observe the copy progress.
   *
   * @type { ?ProgressListener }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  progressListener?: ProgressListener;
  /**
   * Signal used to cancel a copy task.
   *
   * @type { ?TaskSignal }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  copySignal?: TaskSignal;
}

/**
 * Signal used to cancel a copy task.
 *
 * @typedef { function } ProgressListener
 * @param { Progress } progress - indicates the progress data of copyFile
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
type ProgressListener = (progress: Progress) => void;

/**
 * File object.
 *
 * @interface File
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * File object.
 *
 * @interface File
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Represents a File object opened by open().
 *
 * @interface File
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare interface File {
  /**
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * FD of the file.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  readonly fd: number;

  /**
   * Path of the file.
   *
   * @type { string }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300002 - Invalid URI
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly path: string;

  /**
   * Name of the file.
   *
   * @type { string }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly name: string;

  /**
   * Obtains the parent directory of this file object.
   *
   * @returns { string } Parent directory obtained.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 14300002 - Invalid URI
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  getParent(): string;

  /**
   * Applies an exclusive lock or a shared lock on this file in blocking mode.
   * This API uses a promise to return the result.
   *
   * @param { boolean } exclusive - Lock to apply. The value true means an exclusive lock,
   * <br>and the value false (default) means a shared lock.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900043 - No record locks available
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  lock(exclusive?: boolean): Promise<void>;

  /**
   * Applies an exclusive lock or a shared lock on this file in blocking mode.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900043 - No record locks available
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  lock(callback: AsyncCallback<void>): void;

  /**
   * Applies an exclusive lock or a shared lock on this file in blocking mode.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { boolean } exclusive - Lock to apply. The value true means an exclusive lock,
   * <br>and the value false (default) means a shared lock.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900043 - No record locks available
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  lock(exclusive: boolean, callback: AsyncCallback<void>): void;

  /**
   * Applies an exclusive lock or a shared lock on this file in non-blocking mode.
   *
   * @param { boolean } exclusive - Lock to apply. The value true means an exclusive lock,
   * <br>and the value false (default) means a shared lock.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900043 - No record locks available
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  tryLock(exclusive?: boolean): void;

  /**
   * Unlocks a file.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900043 - No record locks available
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  unlock(): void;
}

/**
 * Provides APIs for randomly reading and writing a stream. Before invoking any API of RandomAccessFile,
 * you need to use createRandomAccessFile() to create a RandomAccessFile instance synchronously or asynchronously
 *
 * @interface RandomAccessFile
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
declare interface RandomAccessFile {

  /**
   * FD of the file.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly fd: number;

  /**
   * Offset pointer to the RandomAccessFile instance.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly filePointer: number;

  /**
   * Sets the file offset pointer.
   *
   * @param { number } filePointer - Offset pointer to the RandomAccessFile instance.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  setFilePointer(filePointer: number): void;

  /**
   * Closes this RandomAccessFile instance. This API returns the result synchronously.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  close(): void;

  /**
   * Write randomAccessFile.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { Promise<number> } Returns the number of bytes written to the file in promise mode.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Writes data into a file. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is written from the filePointer.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { Promise<number> } Returns the number of bytes written to the file in promise mode.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  write(
    buffer: ArrayBuffer | string,
    options?: WriteOptions
  ): Promise<number>;

  /**
   * Writes data to a file. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  write(buffer: ArrayBuffer | string, callback: AsyncCallback<number>): void;

  /**
   * Write randomAccessFile.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of bytes written to the file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Writes data to a file. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is written from the filePointer.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  write(
    buffer: ArrayBuffer | string,
    options: WriteOptions,
    callback: AsyncCallback<number>
  ): void;

  /**
   * Write randomAccessFile with sync interface.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { number } Returns the number of bytes written to the file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Writes data to a file. This API returns the result synchronously.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is written from the filePointer.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { number } Length of the data written in the file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  writeSync(
    buffer: ArrayBuffer | string,
    options?: WriteOptions
  ): number;

  /**
   * Read randomAccessFile.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Read randomAccessFile.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { ReadOptions } [options] - options.
   * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Reads data from a file. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is read from the filePointer.
   * @returns { Promise<number> } Promise used to return the data read.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  read(
    buffer: ArrayBuffer,
    options?: ReadOptions
  ): Promise<number>;

  /**
   * Reads data from a file. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  read(buffer: ArrayBuffer, callback: AsyncCallback<number>): void;

  /**
   * Read randomAccessFile.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Reads data from a file. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is read from the filePointer.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  read(
    buffer: ArrayBuffer,
    options: ReadOptions,
    callback: AsyncCallback<number>
  ): void;

  /**
   * Read randomAccessFile with sync interface.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { number } Returns the number of file bytes read to buffer.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Read randomAccessFile with sync interface.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { ReadOptions } [options] - options.
   * @returns { number } Returns the number of file bytes read to buffer.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Reads data from a file. This API returns the result synchronously.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data (it is determined by filePointer plus offset).
   * <br>This parameter is optional. By default, data is read from the filePointer.
   * @returns { number } Returns the number of file bytes read to buffer.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  readSync(
    buffer: ArrayBuffer,
    options?: ReadOptions
  ): number;

  /**
   * Obtains a ReadStream instance of this RandomAccessFile.
   *
   * @returns { ReadStream } ReadStream instance obtained.
   * @throws { BusinessError } 401 - Parameter error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  getReadStream(): ReadStream;

  /**
   * Obtains a WriteStream instance of this RandomAccessFile.
   *
   * @returns { WriteStream } WriteStream instance obtained.
   * @throws { BusinessError } 401 - Parameter error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  getWriteStream(): WriteStream;
}

/**
 * Defines a readable stream. You need to use fs.createReadStream to create a ReadStream instance,
 * which is inherited from the stream base class.
 * The data obtained by ReadStream is a decoded string. Currently, only the UTF-8 format is supported.
 *
 * @extends stream.Readable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare class ReadStream extends stream.Readable {
  /**
   * The ReadStream constructor.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  constructor();

  /**
   * Number of bytes read by the readable stream.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  readonly bytesRead: number;

  /**
   * Path of the file corresponding to the readable stream.
   *
   * @type { string }
   * @readonly
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  readonly path: string;

  /**
   * Adjusts the position of the readable stream offset pointer.
   *
   * @param { number } offset - Number of bytes to move the offset.
   * @param { WhenceType } [whence = WhenceType.SEEK_SET] - Where to start the offset. The default value is SEEK_SET,
   * <br>which indicates the beginning of the file.
   * @returns { number } Returns the offset relative to starting position of stream.
   * @throws { BusinessError } 401 - Parameter error
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900026 - Illegal seek
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  seek(offset: number, whence?: WhenceType): number;

  /**
   * Closes this readable stream.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  close(): void;
}

/**
 * Defines a writeable stream. You need to use fs.createWriteStream to create a WriteStream instance,
 * which is inherited from the stream base class.
 *
 * @extends stream.Writable
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare class WriteStream extends stream.Writable {
  /**
   * The WriteStream constructor.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  constructor();

  /**
   * Number of bytes written to the writable stream.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  readonly bytesWritten: number;

  /**
   * Path of the file corresponding to the writeable stream.
   *
   * @type { string }
   * @readonly
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  readonly path: string;

  /**
   * Adjusts the position of the writeable stream offset pointer.
   *
   * @param { number } offset - Number of bytes to move the offset.
   * @param { WhenceType } [whence = WhenceType.SEEK_SET] - Where to start the offset. The default value is SEEK_SET, which indicates the beginning of the file.
   * @returns { number } Position of the current offset pointer (offset relative to the file header, in bytes).
   * @throws { BusinessError } 401 - Parameter error
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900026 - Illegal seek
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  seek(offset: number, whence?: WhenceType): number;

  /**
   * Closes this writeable stream.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  close(): void;
}

/**
 * AtomicFile is a class used to perform atomic read and write operations on files.
 * A temporary file is written and renamed to the original file location, which ensures file integrity.
 * If the write operation fails, the temporary file is deleted without modifying the original file content.
 * You can call finishWrite() or failWrite() to write or roll back file content.
 *
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 15
 */
export class AtomicFile {
  /**
   * The AtomicFile constructor.
   *
   * @param { string } path - Application sandbox path of the file.
   * @throws { BusinessError } 401 Parameter error.Possible causes:1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  constructor(path: string);

  /**
   * Obtains the file object through the AtomicFile object. The FD needs to be closed by calling close().
   *
   * @returns { File } Returns the file object.
   * @throws { BusinessError } 13900002 No such file or directory
   * @throws { BusinessError } 13900005 IO error
   * @throws { BusinessError } 13900012 Permission denied
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  getBaseFile(): File;

  /**
   * Creates a ReadStream instance.
   *
   * @returns { ReadStream } ReadStream instance obtained.
   * @throws { BusinessError } 13900001 Operation not permitted
   * @throws { BusinessError } 13900002 No such file or directory
   * @throws { BusinessError } 13900012 Permission denied
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  openRead(): ReadStream;

  /**
   * Reads all content of a file.
   *
   * @returns { ArrayBuffer } Full content of a file.
   * @throws { BusinessError } 13900005 I/O error
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  readFully(): ArrayBuffer;

  /**
   * Starts to write new file data in the WriteStream object returned. If the file does not exist, create a file.
   * Call finishWrite() if the write operation is successful; call failWrite() if the write operation fails.
   *
   * @returns { WriteStream } Returns the file write stream.
   * @throws { BusinessError } 13900001 Operation not permitted
   * @throws { BusinessError } 13900002 No such file or directory
   * @throws { BusinessError } 13900012 Permission denied
   * @throws { BusinessError } 13900027 Read-only file system
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  startWrite(): WriteStream;

  /**
   * Finishes writing file data when the write operation is complete.
   *
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  finishWrite(): void;

  /**
   * Rolls back the file after the file fails to be written.
   *
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  failWrite(): void;

  /**
   * Deletes the AtomicFile class, including the original files and temporary files.
   *
   * @throws { BusinessError } 13900001 Operation not permitted
   * @throws { BusinessError } 13900002 No such file or directory
   * @throws { BusinessError } 13900012 Permission denied
   * @throws { BusinessError } 13900027 Read-only file system
   * @throws { BusinessError } 13900042 Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  delete(): void;
}

/**
 * Stat object.
 *
 * @interface Stat
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Stat object.
 *
 * @interface Stat
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Represents detailed file information. Before calling any API of the Stat() class,
 * use stat() to create a Stat instance.
 *
 * @interface Stat
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare interface Stat {
  /**
   * @type { bigint }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * File identifier, which varies with files on the same device.
   *
   * @type { bigint }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  readonly ino: bigint;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * File permissions. The meaning of each bit is as follows:
   * The following values are in octal format. The return values are in decimal format. You need to convert the values.
   * 0o400: The user has the read permission on a regular file or a directory entry.
   * 0o200: The user has the permission to write a regular file or create and delete a directory entry.
   * 0o100: The user has the permission to execute a regular file or search for the specified path in a directory.
   * 0o040: The user group has the read permission on a regular file or a directory entry.
   * 0o020: The user group has the permission to write a regular file or create and delete a directory entry.
   * 0o010: The user group has the permission to execute a regular file or search for the specified path in a directory.
   * 0o004: Other users have the permission to read a regular file or read a directory entry.
   * 0o002: Other users have the permission to write a regular file or create and delete a directory entry.
   * 0o001: Other users have the permission to execute a regular file or search for the specified path in a directory.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  readonly mode: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * ID of the file owner.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  readonly uid: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * ID of the user group of the file.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  readonly gid: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * File size, in bytes. This parameter is valid only for regular files.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  readonly size: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * Time when the file was last accessed. The value is the number of seconds elapsed since 00:00:00 on January 1, 1970.
   * NOTE: Currently, user data partitions are mounted in noatime mode by default, and atime update is disabled.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  readonly atime: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * Time when the file content was last modified. The value is the number of seconds elapsed
   * since 00:00:00 on January 1, 1970.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  readonly mtime: number;
  /**
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Time when the file metadata was last modified. The value is the number of seconds elapsed
   * since 00:00:00 on January 1, 1970.
   *
   * @type { number }
   * @readonly
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  readonly ctime: number;

  /**
   * Time of the last access to the file. The value is the number of nanoseconds elapsed
   * since 00:00:00 on January 1, 1970.
   * NOTE: Currently, user data partitions are mounted in noatime mode by default, and atime update is disabled.
   *
   * @type { bigint }
   * @readonly
   * @throws { BusinessError } 13900042 - Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  readonly atimeNs?:bigint;

  /**
   * Time of the last modification to the file. The value is the number of
   * nanoseconds elapsed since 00:00:00 on January 1, 1970.
   *
   * @type { bigint }
   * @readonly
   * @throws { BusinessError } 13900042 - Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  readonly mtimeNs?:bigint;

  /**
   * Time of the last status change of the file. The value is the number of nanoseconds elapsed
   * since 00:00:00 on January 1, 1970.
   *
   * @type { bigint }
   * @readonly
   * @throws { BusinessError } 13900042 - Internal error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 15
   */
  readonly ctimeNs?:bigint;

  /**
   * File location, which indicates whether the file is stored in a local device or in the cloud.
   *
   * @type { LocationType }
   * @readonly
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  readonly location: LocationType;

  /**
   * Whether path/fd is block device.
   *
   * @returns { boolean } Returns whether the path/fd point to a block device or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Checks whether this file is a block special file. A block special file supports access by block only, and it is cached when accessed.
   *
   * @returns { boolean } Returns whether the path/fd point to a block device or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  isBlockDevice(): boolean;
  /**
   * Whether path/fd is character device.
   *
   * @returns { boolean } Returns whether the path/fd point to a character device or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Checks whether this file is a character special file. A character special device supports random access, and it is not cached when accessed.
   *
   * @returns { boolean } Returns whether the path/fd point to a character device or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  isCharacterDevice(): boolean;
  /**
   * Whether path/fd is directory.
   *
   * @returns { boolean } Returns whether the path/fd point to a directory or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Whether path/fd is directory.
   *
   * @returns { boolean } Returns whether the path/fd point to a directory or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * Checks whether this file is a directory.
   *
   * @returns { boolean } Returns whether the path/fd point to a directory or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  isDirectory(): boolean;
  /**
   * Whether path/fd is fifo.
   *
   * @returns { boolean } Returns whether the path/fd point to a fifo file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Checks whether this file is a named pipe (or FIFO). Named pipes are used for inter-process communication.
   *
   * @returns { boolean } Returns whether the path/fd point to a fifo file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  isFIFO(): boolean;
  /**
   * Whether path/fd is file.
   *
   * @returns { boolean } Returns whether the path/fd point to a normal file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Whether path/fd is file.
   *
   * @returns { boolean } Returns whether the path/fd point to a normal file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  /**
   * Checks whether this file is a regular file.
   *
   * @returns { boolean } Returns whether the path/fd point to a normal file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  isFile(): boolean;
  /**
   * Whether path/fd is socket.
   *
   * @returns { boolean } Returns whether the path/fd point to a socket file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Checks whether this file is a socket.
   *
   * @returns { boolean } Returns whether the path/fd point to a socket file or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  isSocket(): boolean;
  /**
   * Whether path/fd is symbolic link.
   *
   * @returns { boolean } Returns whether the path/fd point to a symbolic link or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Checks whether this file is a symbolic link.
   *
   * @returns { boolean } Returns whether the path/fd point to a symbolic link or not.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @since 10
   */
  isSymbolicLink(): boolean;
}

/**
 * Provides API for stream operations. Before calling any API of Stream,
 * you need to create a Stream instance by using fs.createStream or fs.fdopenStream.
 *
 * @interface Stream
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 9
 */
/**
 * Provides API for stream operations. Before calling any API of Stream,
 * you need to create a Stream instance by using fs.createStream or fs.fdopenStream.
 *
 * @interface Stream
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 20
 */
declare interface Stream {
  /**
   * Close stream.
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Close stream.
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  close(): Promise<void>;

  /**
   * Close stream.
   *
   * @param { AsyncCallback<void> } callback - Return the callback function.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Close stream.
   *
   * @param { AsyncCallback<void> } callback - Return the callback function.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  close(callback: AsyncCallback<void>): void;

  /**
   * Close stream with sync interface.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Close stream with sync interface.
   *
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  closeSync(): void;

  /**
   * Flushes this stream. This API uses a promise to return the result.
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Flushes this stream. This API uses a promise to return the result.
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  flush(): Promise<void>;

  /**
   * Flush stream.
   *
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Flushes this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  flush(callback: AsyncCallback<void>): void;

  /**
   * Flushes this stream. This API returns the result synchronously.
   *
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Flushes this stream. This API returns the result synchronously.
   *
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  flushSync(): void;

  /**
   * Write stream.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { Promise<number> } Returns the number of file bytes written to file in promise mode.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Writes data to this stream. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { Promise<number> } Promise used to return the length of the data written.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Writes data to this stream. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { Promise<number> } Promise used to return the length of the data written.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  write(
      buffer: ArrayBuffer | string,
      options?: WriteOptions
  ): Promise<number>;

  /**
   * Writes data to this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes written to file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
  /**
   * Writes data to this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes written to file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  write(buffer: ArrayBuffer | string, callback: AsyncCallback<number>): void;

  /**
   * Write stream.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes written to file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Writes data to this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Writes data to this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  write(
      buffer: ArrayBuffer | string,
      options: WriteOptions,
      callback: AsyncCallback<number>
  ): void;

  /**
   * Write stream with sync interface.
   *
   * @param { ArrayBuffer | string } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { number } Returns the number of file bytes written to file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Writes data to this stream. This API returns the result synchronously.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { number } Length of the data written in the file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Writes data to this stream. This API returns the result synchronously.
   *
   * @param { ArrayBuffer | string } buffer - Data to write. It can be a string or data from a buffer.
   * @param { WriteOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to write. The default value is the buffer length.
   * <br>offset (number): start position to write the data in the file. This parameter is optional.
   * <br>By default, data is written from the current position.
   * <br>encoding (string): format of the data to be encoded when the data is a string.
   * <br>The default value is 'utf-8', which is the only value supported.
   * @returns { number } Length of the data written in the file.
   * @throws { BusinessError } 13900001 - Operation not permitted
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900024 - File too large
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900041 - Quota exceeded
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  writeSync(
      buffer: ArrayBuffer | string,
      options?: WriteOptions
  ): number;

  /**
   * Read stream.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Read stream.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { ReadOptions } [options] - options.
   * @returns { Promise<number> } Returns the number of file bytes read to buffer in promise mode.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Reads data from the stream. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @returns { Promise<number> } Promise used to return the data read.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  /**
   * Reads data from the stream. This API uses a promise to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @returns { Promise<number> } Promise used to return the data read.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  read(
      buffer: ArrayBuffer,
      options?: ReadOptions
  ): Promise<number>;

  /**
   * Reads data from this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Reads data from this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  read(buffer: ArrayBuffer, callback: AsyncCallback<number>): void;

  /**
   * Read stream.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @param { AsyncCallback<number> } callback - The callback is used to return the number of file bytes read to buffer.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Reads data from this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Reads data from this stream. This API uses an asynchronous callback to return the result.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @param { AsyncCallback<number> } callback - Callback used to return the result.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  read(
      buffer: ArrayBuffer,
      options: ReadOptions,
      callback: AsyncCallback<number>
  ): void;

  /**
   * Read stream with sync interface.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { object } [options] - options.
   * @returns { number } Returns the number of file bytes read to file.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 9
   */
  /**
   * Read stream with sync interface.
   *
   * @param { ArrayBuffer } buffer - buffer.
   * @param { ReadOptions } [options] - options.
   * @returns { number } Returns the number of file bytes read to file.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  /**
   * Reads data from this stream. This API returns the result synchronously.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @returns { number } Length of the data read.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  /**
   * Reads data from this stream. This API returns the result synchronously.
   *
   * @param { ArrayBuffer } buffer - Buffer used to store the file read.
   * @param { ReadOptions } [options] - The options are as follows:
   * <br>length (number): length of the data to read. This parameter is optional. The default value is the buffer length.
   * <br>offset (number): start position to read the data. This parameter is optional. By default,
   * <br>data is read from the current position.
   * @returns { number } Length of the data read.
   * @throws { BusinessError } 13900004 - Interrupted system call
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900010 - Try again
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900019 - Is a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900034 - Operation would block
   * @throws { BusinessError } 13900042 - Unknown error
   * @throws { BusinessError } 13900044 - Network is unreachable
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 20
   */
  readSync(
      buffer: ArrayBuffer,
      options?: ReadOptions
  ): number;
}

/**
 * Provides APIs for observing events.
 *
 * @interface WatchEventListener
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
export interface WatchEventListener {
  /**
   * Specifies the callback function to be invoked.
   *
   * @param { WatchEvent } event - Event for the callback to invoke.
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  (event: WatchEvent): void;
}

/**
 * Defines the event to observe.
 *
 * @interface WatchEvent
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
export interface WatchEvent {
  /**
   * Sandbox path of the file to observe. The sandbox path contains the file name.
   *
   * @type { string }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly fileName: string;

  /**
   * Events to observe. Multiple events can be separated by a bitwise OR operator (|).
   * 0x1: IN_ACCESS: A file is accessed.
   * 0x2: IN_MODIFY: The file content is modified.
   * 0x4: IN_ATTRIB: The file metadata is modified.
   * 0x8: IN_CLOSE_WRITE: A file is opened, written with data, and then closed.
   * 0x10: IN_CLOSE_NOWRITE: A file or directory is opened and then closed without data written.
   * 0x20: IN_OPEN: A file or directory is opened.
   * 0x40: IN_MOVED_FROM: A file in the observed directory is moved.
   * 0x80: IN_MOVED_TO: A file is moved to the observed directory.
   * 0x100: IN_CREATE: A file or directory is created in the observed directory.
   * 0x200: IN_DELETE: A file or directory is deleted from the observed directory.
   * 0x400: IN_DELETE_SELF: The observed directory is deleted. After the directory is deleted, the listening stops.
   * 0x800: IN_MOVE_SELF: The observed file or directory is moved. After the file or directory is moved, the listening continues.
   * 0xfff: IN_ALL_EVENTS: All events.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly event: number;

  /**
   * Cookie bound with the event. Currently, only the IN_MOVED_FROM and IN_MOVED_TO events are supported.
   * The IN_MOVED_FROM and IN_MOVED_TO events of the same file have the same cookie value.
   *
   * @type { number }
   * @readonly
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  readonly cookie: number;
}

/**
 * Provides APIs for observing the changes of files or directories.
 * Before using the APIs of Watcher, call createWatcher() to create a Watcher object.
 *
 * @interface Watcher
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
export interface Watcher {
  /**
   * Starts listening.
   *
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900021 - File table overflow
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  start(): void;

  /**
   * Stops listening and removes the Watcher object.
   *
   * @throws { BusinessError } 13900002 - No such file or directory
   * @throws { BusinessError } 13900008 - Bad file descriptor
   * @throws { BusinessError } 13900011 - Out of memory
   * @throws { BusinessError } 13900012 - Permission denied
   * @throws { BusinessError } 13900013 - Bad address
   * @throws { BusinessError } 13900015 - File exists
   * @throws { BusinessError } 13900018 - Not a directory
   * @throws { BusinessError } 13900020 - Invalid argument
   * @throws { BusinessError } 13900021 - File table overflow
   * @throws { BusinessError } 13900022 - Too many open files
   * @throws { BusinessError } 13900025 - No space left on device
   * @throws { BusinessError } 13900030 - File name too long
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  stop(): void;
}

/**
 * Represents the information obtained by the ReaderIterator object.
 *
 * @interface ReaderIteratorResult
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
export interface ReaderIteratorResult {
  /**
   * Whether the iteration is complete.
   * The value true means the iteration is complete; the value false means the iteration is not complete.
   *
   * @type { boolean }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  done: boolean;

  /**
   * File text content read line by line.
   *
   * @type { string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  value: string;
}

/**
 * Provides a ReaderIterator object. Before calling APIs of ReaderIterator,
 * you need to use readLines() to create a ReaderIterator instance.
 *
 * @interface ReaderIterator
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare interface ReaderIterator {
  /**
   * Obtains the ReaderIterator result.
   *
   * @returns { ReaderIteratorResult } ReaderIteratorResult object obtained.
   * @throws { BusinessError } 13900005 - I/O error
   * @throws { BusinessError } 13900037 - No data available
   * @throws { BusinessError } 13900042 - Unknown error
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  next(): ReaderIteratorResult;
}

/**
 * File filter type
 *
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @since 10
 */
/**
 * Defines the file filtering configuration used by listFile().
 *
 * @interface Filter
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @crossplatform
 * @atomicservice
 * @since 11
 */
export interface Filter {
  /**
   * The suffix of the file.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Locate files that fully match the specified file name extensions, which are of the OR relationship.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  suffix?: Array<string>;
  /**
   * The display name of the file.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Locate files that fuzzy match the specified file names, which are of the OR relationship.
   * Currently, only the wildcard * is supported.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  displayName?: Array<string>;
  /**
   * The mimetype of the file.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Locate files that fully match the specified MIME types, which are of the OR relationship.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  mimeType?: Array<string>;
  /**
   * The exceeding size of the file.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Locate files that are greater than or equal to the specified size.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  fileSizeOver?: number;
  /**
   * The last modification time of the file.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Locate files whose last modification time is the same or later than the specified time.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  lastModifiedAfter?: number;
  /**
   * Whether to exclude media files.
   *
   * @type { ?boolean }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * Whether to exclude the files already in Media. The value true means to exclude the files already in Media;
   * the value false means not to exclude the files already in Media.
   *
   * @type { ?boolean }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  excludeMedia?: boolean;
}

/**
 * Conflict Files type
 *
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 10
 */
/**
 * Defines conflicting file information used in copyDir() or moveDir().
 *
 * @interface ConflictFiles
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
export interface ConflictFiles {
  /**
   * The path of the source file.
   *
   * @type { string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * The path of the source file.
   *
   * @type { string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  srcFile: string;

  /**
   * The path of the source file.
   *
   * @type { string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 10
   */
  /**
   * The path of the destination file.
   *
   * @type { string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  destFile: string;
}

/**
 * Defines the options used in readLines().
 *
 * @interface Options
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
export interface Options {
  /**
   * File encoding format. It is optional.
   *
   * @type { ?string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  encoding?: string;
}

/**
 * Defines the options used in read().
 *
 * @interface ReadOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
export interface ReadOptions {
  /**
   * Length of the data to read, in bytes. This parameter is optional. The default value is the buffer length.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  offset?: number;
  /**
   * Start position of the file to read (current filePointer plus offset), in bytes. This parameter is optional.
   * By default, data is read from the filePointer.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  length?: number;
}

/**
 * Defines the options used in readText(). It inherits from ReadOptions.
 *
 * @extends ReadOptions
 * @interface ReadTextOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
export interface ReadTextOptions extends ReadOptions {
  /**
   * Format of the data to be encoded. This parameter is valid only when the data type is string.
   * The default value is 'utf-8', which is the only value supported.
   *
   * @type { ?string }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  encoding?: string;
}

/**
 * Defines the options use din write(). It inherits from Options.
 *
 * @extends Options
 * @interface WriteOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
export interface WriteOptions extends Options {
  /**
   * Option for creating the writeable stream. You must specify one of the following options.
   * OpenMode.READ_ONLY(0o0): read-only, which is the default value.
   * OpenMode.WRITE_ONLY(0o1): write-only.
   * OpenMode.READ_WRITE(0o2): read/write.
   * You can also specify the following options, separated by a bitwise OR operator (|).
   * By default, no additional options are given.
   * OpenMode.CREATE(0o100): If the file does not exist, create it.
   * OpenMode.TRUNC(0o1000): If the file exists and is opened in write mode, truncate the file length to 0.
   * OpenMode.APPEND(0o2000): Open the file in append mode. New data will be added to the end of the file.
   * OpenMode.NONBLOCK(0o4000): If path points to a named pipe (also known as a FIFO), block special file,
   * or character special file, perform non-blocking operations on the opened file and in subsequent I/Os.
   * OpenMode.DIR(0o200000): If path does not point to a directory, throw an exception. The write permission is not allowed.
   * OpenMode.NOFOLLOW(0o400000): If path points to a symbolic link, throw an exception.
   * OpenMode.SYNC(0o4010000): Open the file in synchronous I/O mode.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  offset?: number;
  /**
   * Length of the data to write, in bytes. This parameter is optional. The default value is the buffer length.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  length?: number;
}

/**
 * ListFileOptions type
 *
 * @interface ListFileOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 11
 */
export interface ListFileOptions {
  /**
   * Whether to list all files in the subdirectories recursively. This parameter is optional.
   * The default value is false. If recursion is false, the names of files and directories that meet the filtering
   * requirements in the current directory are returned. If recursion is true, relative paths (starting with /)
   * of all files that meet the specified conditions in the current directory are returned.
   *
   * @type { ?boolean }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  recursion?: boolean;

  /**
   * Number of file names to list. This parameter is optional. The default value is 0, which means to list all files.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  listNum?: number;

  /**
   * File filtering configuration. This parameter is optional. It specifies the file filtering conditions.
   *
   * @type { ?Filter }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 11
   */
  filter?: Filter;
}

/**
 * Defines the options used in createRandomAccessFile().
 *
 * @interface RandomAccessFileOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
export interface RandomAccessFileOptions {
  /**
   * Start position to read the data, in bytes. This parameter is optional. By default, data is read from the current position.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  start?: number;

  /**
   * End position to read the data, in bytes. This parameter is optional. The default value is the end of the file.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  end?: number;
}

/**
 * Defines the options used in createReadStream().
 *
 * @interface ReadStreamOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
export interface ReadStreamOptions {
  /**
   * Start position to read the data, in bytes. This parameter is optional. By default,
   * data is read from the current position.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  start?: number;

  /**
   * End position to read the data, in bytes. This parameter is optional.
   * The default value is the end of the file.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  end?: number;
}

/**
 * Defines the options used in createWriteStream().
 *
 * @interface WriteStreamOptions
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
export interface WriteStreamOptions {
  /**
   * Option for creating the writeable stream.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  mode?: number;
  /**
   * Start position to write the data, in bytes. This parameter is optional.
   * By default, data is written from the beginning of the file.
   *
   * @type { ?number }
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  start?: number;
}

/**
 * The listeners of Distributed File System.
 *
 * @typedef DfsListeners
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
export interface DfsListeners {
  /**
   * The Listener of Distributed File System status
   *
   * @param { string } networkId - The networkId of device.
   * @param { number } status - The status code of Distributed File System.
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  onStatus(networkId: string, status: number): void;
}

/**
 * Enumerates the types of the relative offset position used in lseek().
 *
 * @enum { number } whence type
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare enum WhenceType {
  /**
   * Starting position of the file offset.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  SEEK_SET = 0,

  /**
   * Current position of the file offset.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  SEEK_CUR = 1,

  /**
   * Ending position of the file offset.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  SEEK_END = 2
}

/**
 * Enumeration of different types of file location.
 *
 * @enum { number } location type
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 11
 */
declare enum LocationType {
  /**
   * The file is stored in a local device.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  LOCAL = 1 << 0,

  /**
   * The file is stored in the cloud.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 11
   */
  CLOUD = 1 << 1
}

/**
 * Enumerates the access modes to verify. If this parameter is left blank, the system checks whether the file exists.
 *
 * @enum { number } access mode type
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @atomicservice
 * @since 12
 */
declare enum AccessModeType {
  /**
   * Whether the file exists.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 12
   */
  EXIST = 0,

  /**
   * Verify the write permission on the file.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 12
   */
  WRITE = 2,

  /**
   * Verify the read permission on the file.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 12
   */
  READ = 4,

  /**
   * Verify the read/write permission on the file.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @atomicservice
   * @since 12
   */
  READ_WRITE = 6
}

/**
 * Enumerates the locations of the file to verify.
 *
 * @enum { number } access flag type
 * @syscap SystemCapability.FileManagement.File.FileIO
 * @since 12
 */
declare enum AccessFlagType {
  /**
   * The file is stored locally.
   *
   * @syscap SystemCapability.FileManagement.File.FileIO
   * @since 12
   */
  LOCAL = 0
}
