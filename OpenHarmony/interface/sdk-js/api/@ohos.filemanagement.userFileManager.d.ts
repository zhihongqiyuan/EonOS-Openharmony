/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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
import type Context from './application/Context';
import image from './@ohos.multimedia.image';
import dataSharePredicates from './@ohos.data.dataSharePredicates';

/**
 * @namespace userFileManager
 * @syscap SystemCapability.FileManagement.UserFileManager.Core
 * @systemapi
 * @since 9
 */
declare namespace userFileManager {
  /**
   * Returns an instance of UserFileManager
   *
   * @param { Context } context - Hap context information
   * @returns { UserFileManager } Instance of UserFileManager
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  function getUserFileMgr(context: Context): UserFileManager;

  /**
   * Enumeration types for different kinds of Files
   *
   * @enum { number } FileType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  enum FileType {
    /**
     * Image file type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    IMAGE = 1,
    /**
     * Video file type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    VIDEO,
    /**
     * Audio file type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    AUDIO
  }

  /**
   * Enumeration types for different types of FileAsset
   *
   * @enum { number } PhotoSubType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum PhotoSubType {
    /**
     * Default Photo Type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    DEFAULT,
    /**
     * Screenshot Photo Type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    SCREENSHOT,
    /**
     * Camera Photo Type
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    CAMERA
  }

  /**
   * File position
   *
   * @enum { number } File position, which indicates the file is on local device or cloud
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum PositionType {
    /**
     * File exists only on local device
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    LOCAL = 1,
    /**
     * File exists only on cloud
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    CLOUD,
    /**
     * File exists on both local and cloud
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    BOTH
  }

  /**
   * Indicates the type of file asset member.
   *
   * @typedef { number | string | boolean } MemberType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  type MemberType = number | string | boolean;

  /**
   * Indicates the type of notify event.
   *
   * @typedef { 'deviceChange' | 'albumChange' | 'imageChange' | 'audioChange' | 'videoChange' | 'remoteFileChange' } ChangeEvent
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  type ChangeEvent =
    'deviceChange'
    | 'albumChange'
    | 'imageChange'
    | 'audioChange'
    | 'videoChange'
    | 'remoteFileChange';

  /**
   * Provides methods to encapsulate file attributes.
   *
   * @interface FileAsset
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface FileAsset {
    /**
     * URI of the file.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    readonly uri: string;
    /**
     * File type, for example, IMAGE, VIDEO, AUDIO
     *
     * @type { FileType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    readonly fileType: FileType;
    /**
     * Display name (with a file name extension) of the file.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    displayName: string;
    /**
     * Return the fileAsset member parameter.
     *
     * @param { string } member - The name of the parameter. for example : get(ImageVideoKey.URI)
     * @returns { MemberType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    get(member: string): MemberType;
    /**
     * Set the fileAsset member parameter.
     *
     * @param { string } member - The name of the parameter
     * @param { string } value - The value of the parameter.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     * @example : Set(ImageVideoKey.TITLE, "newTitle"), call commitModify after set value
     */
    set(member: string, value: string): void;
    /**
     * Modify meta data where the file is located.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @param { AsyncCallback<void> } callback - No value will be returned.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    commitModify(callback: AsyncCallback<void>): void;
    /**
     * Modify meta data where the file is located.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @returns { Promise<void> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    commitModify(): Promise<void>;
    /**
     * Open local file.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO or ohos.permission.READ_AUDIO or ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @param { string } mode - Mode for open, for example: rw, r, w.
     * @param { AsyncCallback<number> } callback - Callback return the fd of the file.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    open(mode: string, callback: AsyncCallback<number>): void;
    /**
     * Open local file.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO or ohos.permission.READ_AUDIO or ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @param { string } mode - Mode for open, for example: rw, r, w.
     * @returns { Promise<number> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    open(mode: string): Promise<number>;
    /**
     * Close the file is located.
     *
     * @param { number } fd - Fd of the file which had been opened
     * @param { AsyncCallback<void> } callback - No value will be returned.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    close(fd: number, callback: AsyncCallback<void>): void;
    /**
     * Close the file is located.
     *
     * @param { number } fd - Fd of the file which had been opened
     * @returns { Promise<void> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    close(fd: number): Promise<void>;
    /**
     * Get thumbnail of the file when the file is located.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO or ohos.permission.READ_AUDIO
     * @param { AsyncCallback<image.PixelMap> } callback - Callback used to return the thumbnail's pixelMap.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getThumbnail(callback: AsyncCallback<image.PixelMap>): void;
    /**
     * Get thumbnail of the file when the file is located.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO or ohos.permission.READ_AUDIO
     * @param { image.Size } size - Thumbnail's size
     * @param { AsyncCallback<image.PixelMap> } callback - Callback used to return the thumbnail's pixelMap.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getThumbnail(size: image.Size, callback: AsyncCallback<image.PixelMap>): void;
    /**
     * Get thumbnail of the file when the file is located.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO or ohos.permission.READ_AUDIO
     * @param { image.Size } size - Thumbnail's size
     * @returns { Promise<image.PixelMap> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getThumbnail(size?: image.Size): Promise<image.PixelMap>;
    /**
     * Set favorite for the file when the file is located.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @param { boolean } isFavorite - True is favorite file, false is not favorite file
     * @param { AsyncCallback<void> } callback - Callback used to return, No value is returned.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    favorite(isFavorite: boolean, callback: AsyncCallback<void>): void;
    /**
     * Set favorite for the file when the file is located.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.WRITE_AUDIO
     * @param { boolean } isFavorite - isFavorite True is favorite file, false is not favorite file
     * @returns { Promise<void> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    favorite(isFavorite: boolean): Promise<void>;
    /**
     * Set file hidden state.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { boolean } hiddenState - true: Put the asset into hidden album; false: Recover the asset from hidden album.
     * @param { AsyncCallback<void> } callback - Return void.
     * @throws { BusinessError } 202 - Called by non-system application.
     * @throws { BusinessError } 13900020 - if parameter is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    setHidden(hiddenState: boolean, callback: AsyncCallback<void>): void;
    /**
     * Set file hidden state.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { boolean } hiddenState - true: Put the asset into hidden album; false: Recover the asset from hidden album.
     * @returns { Promise<void> } Returns the promise
     * @throws { BusinessError } 202 - Called by non-system application.
     * @throws { BusinessError } 13900020 - if parameter is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    setHidden(hiddenState: boolean): Promise<void>;
    /**
     * Set user comment info to the asset.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } userComment - user comment info
     * @param { AsyncCallback<void> } callback - Returns void.
     * @throws { BusinessError } 202 - Called by non-system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    setUserComment(userComment: string, callback: AsyncCallback<void>): void;
    /**
     * Set user comment info to the asset.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } userComment - user comment info
     * @returns { Promise<void> } Returns void
     * @throws { BusinessError } 202 - Called by non-system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    setUserComment(userComment: string): Promise<void>;
    /**
     * Get exif info of the asset.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AsyncCallback<string> } callback - Returns exif info into a json string
     * @throws { BusinessError } 202 - Called by non-system application.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getExif(callback: AsyncCallback<string>): void;
    /**
     * Get exif info of the asset.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @returns { Promise<string> } Returns exif info into a json string
     * @throws { BusinessError } 202 - Called by non-system application.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getExif(): Promise<string>;
  }

  /**
   * Describes AUDIO TYPE FetchOptions's predicate
   *
   * @enum { string } AudioKey
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  enum AudioKey {
    /**
     * File uri, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    URI,
    /**
     * File name
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DISPLAY_NAME,
    /**
     * Date of the file creation, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_ADDED,
    /**
     * Modify date of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_MODIFIED,
    /**
     * Title of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    TITLE,
    /**
     * Artist of the audio file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    ARTIST,
    /**
     * Audio album of the audio file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    AUDIOALBUM,
    /**
     * Duration of the audio file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DURATION,
    /**
     * Favorite state of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    FAVORITE
  }

  /**
   * Describes Image, Video TYPE FetchOptions's predicate
   *
   * @enum { string } ImageVideoKey
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  enum ImageVideoKey {
    /**
     * File uri, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    URI,
    /**
     * File type of the Asset, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    FILE_TYPE,
    /**
     * File name
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DISPLAY_NAME,
    /**
     * Date of the file creation, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_ADDED,
    /**
     * Modify date of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_MODIFIED,
    /**
     * Title of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    TITLE,
    /**
     * Duration of the audio and video file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DURATION,
    /**
     * Width of the image file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    WIDTH,
    /**
     * Height of the image file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    HEIGHT,
    /**
     * Date taken of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_TAKEN,
    /**
     * Orientation of the image file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    ORIENTATION,
    /**
     * Favorite state of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    FAVORITE,
    /**
     * File position, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    POSITION,
    /**
     * Trashed date of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    DATE_TRASHED,
    /**
     * Hidden state of the file, read only
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    HIDDEN,
    /**
     * User comment info
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    USER_COMMENT,
    /**
     * Camera shot key
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    CAMERA_SHOT_KEY
  }

  /**
   * Describes Album TYPE predicate
   *
   * @enum { string } AlbumKey
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  enum AlbumKey {
    /**
     * Album uri
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    URI,
    /**
     * File type of the Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    FILE_TYPE,
    /**
     * Album name
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    ALBUM_NAME,
    /**
     * Date of the Album creation
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_ADDED,
    /**
     * Modify date of the Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    DATE_MODIFIED
  }

  /**
   * Fetch parameters
   *
   * @interface FetchOptions
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface FetchOptions {
    /**
     * Indicates the columns to query.
     *
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    fetchColumns: Array<string>;
    /**
     * Predicate to query
     *
     * @type { dataSharePredicates.DataSharePredicates }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    predicates: dataSharePredicates.DataSharePredicates;
  }

  /**
   * Fetch parameters
   *
   * @interface AlbumFetchOptions
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface AlbumFetchOptions {
    /**
     * Predicate to query
     *
     * @type { dataSharePredicates.DataSharePredicates }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    predicates: dataSharePredicates.DataSharePredicates;
  }

  /**
   * Describe additional operations for creating photo
   *
   * @interface PhotoCreateOptions
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  interface PhotoCreateOptions {
    /**
     * SubType of the photo
     *
     * @type { ?PhotoSubType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    subType?: PhotoSubType;
    /**
     * Camera shot key
     *
     * @type { ?string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    cameraShotKey?: string;
  }

  /**
   * Implements file retrieval.
   *
   * @interface FetchResult
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface FetchResult<T> {
    /**
     * Obtains the total number of files in the file retrieval result.
     *
     * @returns { number } Total number of files.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getCount(): number;
    /**
     * Checks whether the result set points to the last row.
     *
     * @returns { boolean } Whether the file is the last one.
     * You need to check whether the file is the last one before calling getNextObject,
     * which returns the next file only when False is returned for this method.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    isAfterLast(): boolean;
    /**
     * Releases the FetchResult instance and invalidates it. Other methods cannot be called.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    close(): void;
    /**
     * Obtains the first FileAsset in the file retrieval result. This method uses a callback to return the file.
     *
     * @param { AsyncCallback<T> } callback - Callback used to return the file in the format of a FileAsset instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getFirstObject(callback: AsyncCallback<T>): void;
    /**
     * Obtains the first T in the file retrieval result. This method uses a promise to return the file.
     *
     * @returns { Promise<T> } A Promise instance used to return the file in the format of a T instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getFirstObject(): Promise<T>;
    /**
     * Obtains the next T in the file retrieval result.
     * This method uses a callback to return the file.
     * Before calling this method, you must use isAfterLast() to check whether the result set points to the last row.
     * This method returns the next file only when False is returned for isAfterLast().
     *
     * @param { AsyncCallback<T> } callback - Callback used to return the file in the format of a T instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getNextObject(callback: AsyncCallback<T>): void;
    /**
     * Obtains the next T in the file retrieval result.
     * This method uses a promise to return the file.
     * Before calling this method, you must use isAfterLast() to check whether the result set points to the last row.
     * This method returns the next file only when False is returned for isAfterLast().
     *
     * @returns { Promise<T> } A Promise instance used to return the file in the format of a T instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getNextObject(): Promise<T>;
    /**
     * Obtains the last T in the file retrieval result. This method uses a callback to return the file.
     *
     * @param { AsyncCallback<T> } callback - Callback used to return the file in the format of a T instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getLastObject(callback: AsyncCallback<T>): void;
    /**
     * Obtains the last T in the file retrieval result. This method uses a promise to return the file.
     *
     * @returns { Promise<T> } A Promise instance used to return the file in the format of a T instance.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getLastObject(): Promise<T>;
    /**
     * Obtains the T with the specified index in the file retrieval result.
     * This method uses a callback to return the file.
     *
     * @param { number } index - Index of the file to obtain.
     * @param { AsyncCallback<T> } callback - Callback used to return the file in the format of a T instance.
     * @throws { BusinessError } 13900020 - if type index is not number
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPositionObject(index: number, callback: AsyncCallback<T>): void;
    /**
     * Obtains the T with the specified index in the file retrieval result.
     * This method uses a promise to return the file.
     *
     * @param { number } index - Index of the file to obtain.
     * @returns { Promise<T> } A Promise instance used to return the file in the format of a T instance.
     * @throws { BusinessError } 13900020 - if type index is not number
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPositionObject(index: number): Promise<T>;
    /**
     * Obtains all T in the file retrieval result.
     * This method uses a callback to return the result. After this method is called,
     *
     * @param { AsyncCallback<Array<T>> } callback - Callback used to return a T array.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getAllObject(callback: AsyncCallback<Array<T>>): void;
    /**
     * Obtains all T in the file retrieval result.
     * This method uses a promise to return the result. that store the selected media resources.
     *
     * @returns { Promise<Array<T>> } A Promise instance used to return a T array.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getAllObject(): Promise<Array<T>>;
  }

  /**
   * Album type.
   *
   * @enum { number } AlbumType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum AlbumType {
    /**
     * Album created by user.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    USER = 0,
    /**
     * Album created by system, which metadata cannot be modified by user.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    SYSTEM = 1024
  }

  /**
   * Album subtype
   *
   * @enum { number } AlbumSubType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum AlbumSubType {
    /**
     * Generic user-created albums.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    USER_GENERIC = 1,
    /**
     * Favorite album, which assets are marked as favorite.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    FAVORITE = 1025,
    /**
     * Video album, which contains all video assets.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    VIDEO,
    /**
     * Hidden album, which assets are marked as hidden.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    HIDDEN,
    /**
     * Trash album, which assets are deleted.
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    TRASH,
    /**
     * Screenshot album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    SCREENSHOT,
    /**
     * Camera album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    CAMERA,
    /**
     * Any album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    ANY = 2147483647
  }

  /**
   * Defines the AbsAlbum.
   *
   * @interface AbsAlbum
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface AbsAlbum {
    /**
     * Album type
     *
     * @type { AlbumType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    readonly albumType: AlbumType;
    /**
     * Album subtype
     *
     * @type { AlbumSubType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    readonly albumSubType: AlbumSubType;
    /**
     * Album name.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    albumName: string;
    /**
     * Album uri.
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    readonly albumUri: string;
    /**
     * Date (timestamp) when the album was last modified.
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    readonly dateModified: number;
    /**
     * File count for the album
     *
     * @type { number }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    readonly count: number;
    /**
     * CoverUri for the album
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    coverUri: string;
    /**
     * Obtains files in an album. This method uses an asynchronous callback to return the files.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { FetchOptions } options - Retrieval options.
     * @param { AsyncCallback<FetchResult<FileAsset>> } callback - Callback used to return the files in the format of a FetchResult instance.
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAssets(options: FetchOptions, callback: AsyncCallback<FetchResult<FileAsset>>): void;
    /**
     * Obtains files in an album. This method uses a promise to return the files.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { FetchOptions } options - Retrieval options.
     * @returns { Promise<FetchResult<FileAsset>> } A Promise instance used to return the files in the format of a FetchResult instance.
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAssets(options: FetchOptions): Promise<FetchResult<FileAsset>>;
  }

  /**
   * Defines the album.
   *
   * @interface Album
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface Album extends AbsAlbum {
    /**
     * Modify the meta data for the album
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { AsyncCallback<void> } callback - No value will be returned.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    commitModify(callback: AsyncCallback<void>): void;
    /**
     * Modify the meta data for the album
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @returns { Promise<void> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    commitModify(): Promise<void>;
    /**
     * Add PhotoAssets to the album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to add
     * @param { AsyncCallback<void> } callback - Returns void
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    addPhotoAssets(assets: Array<FileAsset>, callback: AsyncCallback<void>): void;
    /**
     * Add PhotoAssets to the album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to add
     * @returns { Promise<void> } Returns the promise
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    addPhotoAssets(assets: Array<FileAsset>): Promise<void>;
    /**
     * Remove PhotoAssets from the album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to remove
     * @param { AsyncCallback<void> } callback - Returns void
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    removePhotoAssets(assets: Array<FileAsset>, callback: AsyncCallback<void>): void;
    /**
     * Remove PhotoAssets from the album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to remove
     * @returns { Promise<void> } Returns the promise
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    removePhotoAssets(assets: Array<FileAsset>): Promise<void>;
    /**
     * Recover PhotoAssets from the trash album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to recover
     * @param { AsyncCallback<void> } callback - Returns void
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    recoverPhotoAssets(assets: Array<FileAsset>, callback: AsyncCallback<void>): void;
    /**
     * Recover PhotoAssets from the trash album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to recover
     * @returns { Promise<void> } Returns the promise
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    recoverPhotoAssets(assets: Array<FileAsset>): Promise<void>;
    /**
     * Delete PhotoAssets permanently from the trash album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to delete
     * @param { AsyncCallback<void> } callback - Returns void
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    deletePhotoAssets(assets: Array<FileAsset>, callback: AsyncCallback<void>): void;
    /**
     * Delete PhotoAssets permanently from the trash album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<FileAsset> } assets - Assets to delete
     * @returns { Promise<void> } Returns the promise
     * @throws { BusinessError } 13900020 - if PhotoAssets is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    deletePhotoAssets(assets: Array<FileAsset>): Promise<void>;
  }

  /**
   * Defines the UserFileManager class and provides functions to access the data in user file storage.
   *
   * @interface UserFileManager
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface UserFileManager {
    /**
     * Query photo, video assets
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { FetchOptions } options - retrieval options.
     * @param { AsyncCallback<FetchResult<FileAsset>> } callback - Callback return the FetchResult.
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAssets(options: FetchOptions, callback: AsyncCallback<FetchResult<FileAsset>>): void;
    /**
     * Query photo, video assets
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { FetchOptions } options - Retrieval options.
     * @returns { Promise<FetchResult<FileAsset>> } A promise instance used to return the files in the format of a FetchResult instance
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAssets(options: FetchOptions): Promise<FetchResult<FileAsset>>;
    /**
     * Create Photo Asset
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } displayName - File name
     * @param { string } albumUri - Asset will put into the album.
     * @param { AsyncCallback<FileAsset> } callback - Callback used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName or albumUri is not string
     * @throws { BusinessError } 14000001 - if type displayName invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    createPhotoAsset(displayName: string, albumUri: string, callback: AsyncCallback<FileAsset>): void;
    /**
     * Create Photo Asset
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } displayName - File name
     * @param { AsyncCallback<FileAsset> } callback - Callback used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName is not string
     * @throws { BusinessError } 14000001 - if type displayName invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    createPhotoAsset(displayName: string, callback: AsyncCallback<FileAsset>): void;
    /**
     * Create Photo Asset
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } displayName - File name
     * @param { string } albumUri - Album uri is optional, PhotoAssets will put into the default album without albumUri
     * @returns { Promise<FileAsset> } A Promise instance used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName or albumUri is not string
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    createPhotoAsset(displayName: string, albumUri?: string): Promise<FileAsset>;
    /**
     * Create Photo Asset
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } displayName - File name
     * @param { PhotoCreateOptions } createOption - Create operation
     * @returns { Promise<FileAsset> } A Promise instance used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName is not string
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    createPhotoAsset(displayName: string, createOption: PhotoCreateOptions): Promise<FileAsset>;
    /**
     * Create Photo Asset
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } displayName - File name
     * @param { PhotoCreateOptions } createOption - Photo create operation
     * @param { AsyncCallback<FileAsset> } callback - Callback used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName is not string
     * @throws { BusinessError } 14000001 - if type displayName invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    createPhotoAsset(displayName: string, createOption: PhotoCreateOptions, callback: AsyncCallback<FileAsset>): void;
    /**
     * Create Audio Asset
     *
     * @permission ohos.permission.WRITE_AUDIO
     * @param { string } displayName - File name
     * @param { AsyncCallback<FileAsset> } callback - Callback used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName is not string
     * @throws { BusinessError } 14000001 - if type displayName invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    createAudioAsset(displayName: string, callback: AsyncCallback<FileAsset>): void;
    /**
     * Create Audio Asset
     *
     * @permission ohos.permission.WRITE_AUDIO
     * @param { string } displayName - File name
     * @returns { Promise<FileAsset> } A Promise instance used to return the FileAsset
     * @throws { BusinessError } 13900020 - if type displayName is not string
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    createAudioAsset(displayName: string): Promise<FileAsset>;
    /**
     * Obtains albums based on the retrieval options. This method uses an asynchronous callback to return.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AlbumFetchOptions } options - Retrieval options.
     * @param { AsyncCallback<FetchResult<Album>> } callback - Callback used to return an album array.
     * @throws { BusinessError } 13900020 - if type options is not AlbumFetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAlbums(options: AlbumFetchOptions, callback: AsyncCallback<FetchResult<Album>>): void;
    /**
     * Obtains albums based on the retrieval options. This method uses a promise to return the albums.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AlbumFetchOptions } options - Retrieval options.
     * @returns { Promise<FetchResult<Album>> } A Promise instance used to return an album array.
     * @throws { BusinessError } 13900020 - if type options is not AlbumFetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPhotoAlbums(options: AlbumFetchOptions): Promise<FetchResult<Album>>;
    /**
     * Create a generic user album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } name - Album name to be created.
     * @param { AsyncCallback<Album> } callback - Returns the instance of newly created Album
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    createAlbum(name: string, callback: AsyncCallback<Album>): void;
    /**
     * Create a generic user album.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { string } name - Album name to be created.
     * @returns { Promise<Album> } Returns the instance of newly created Album
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    createAlbum(name: string): Promise<Album>;
    /**
     * Delete generic user-created albums.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<Album> } albums - Specify which album to delete
     * @param { AsyncCallback<void> } callback - Returns void
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    deleteAlbums(albums: Array<Album>, callback: AsyncCallback<void>): void;
    /**
     * Delete generic user-created albums.
     *
     * @permission ohos.permission.WRITE_IMAGEVIDEO
     * @param { Array<Album> } albums - Specify which album to delete
     * @returns { Promise<void> } Returns the promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    deleteAlbums(albums: Array<Album>): Promise<void>;
    /**
     * Obtains albums based on the retrieval options and album types.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AlbumType } type - Album type.
     * @param { AlbumSubType } subType - Album subtype.
     * @param { FetchOptions } options - options to fetch albums
     * @param { AsyncCallback<FetchResult<Album>> } callback - Returns the fetch result of the albums
     * @throws { BusinessError } 13900020 - if type options is not FetchOption
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getAlbums(
      type: AlbumType,
      subType: AlbumSubType,
      options: FetchOptions,
      callback: AsyncCallback<FetchResult<Album>>
    ): void;
    /**
     * Obtains albums based on the album types.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AlbumType } type - Album type.
     * @param { AlbumSubType } subType - Album subtype.
     * @param { AsyncCallback<FetchResult<Album>> } callback - Returns the fetch result of the albums
     * @throws { BusinessError } 13900020 - if type options is not FetchOption
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getAlbums(type: AlbumType, subType: AlbumSubType, callback: AsyncCallback<FetchResult<Album>>): void;
    /**
     * Obtains albums based on the retrieval options and album types.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { AlbumType } type - Album type.
     * @param { AlbumSubType } subType - Album subtype.
     * @param { FetchOptions } [options] -options to fetch albums
     * @returns { Promise<FetchResult<Album>> } - Returns the fetch result of the albums
     * @throws { BusinessError } 13900020 - if type options is not FetchOption
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getAlbums(type: AlbumType, subType: AlbumSubType, options?: FetchOptions): Promise<FetchResult<Album>>;
    /**
     * Obtains system private albums based on the private album type. This method uses an asynchronous callback to return.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { PrivateAlbumType } type - Private album type
     * @param { AsyncCallback<FetchResult<PrivateAlbum>> } callback - Used to return a private album FetchResult.
     * @throws { BusinessError } 13900020 - if type type is not PrivateAlbumType
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPrivateAlbum(type: PrivateAlbumType, callback: AsyncCallback<FetchResult<PrivateAlbum>>): void;
    /**
     * Obtains system private albums based on the private album type. This method uses a promise to return.
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { PrivateAlbumType } type - Private album type
     * @returns { Promise<FetchResult<PrivateAlbum>> } A Promise instance used to return a private album FetchResult.
     * @throws { BusinessError } 13900020 - if type type is not PrivateAlbumType
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getPrivateAlbum(type: PrivateAlbumType): Promise<FetchResult<PrivateAlbum>>;
    /**
     * Query audio assets
     *
     * @permission ohos.permission.READ_AUDIO
     * @param { FetchOptions } options - Retrieval options.
     * @param { AsyncCallback<FetchResult<FileAsset>> } callback - Callback return the FetchResult.
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getAudioAssets(options: FetchOptions, callback: AsyncCallback<FetchResult<FileAsset>>): void;
    /**
     * Query audio assets
     *
     * @permission ohos.permission.READ_AUDIO
     * @param { FetchOptions } options - Retrieval options.
     * @returns { Promise<FetchResult<FileAsset>> } A promise instance used to return the files in the format of a FetchResult instance
     * @throws { BusinessError } 13900020 - if type options is not FetchOptions
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    getAudioAssets(options: FetchOptions): Promise<FetchResult<FileAsset>>;
    /**
     * Delete Asset
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - Uri of FileAsset
     * @param { AsyncCallback<void> } callback - No value returned
     * @throws { BusinessError } 13900020 - if type uri is not string
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    delete(uri: string, callback: AsyncCallback<void>): void;
    /**
     * Delete Asset
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - Uri of FileAsset
     * @returns { Promise<void> } A Promise instance, no value returned
     * @throws { BusinessError } 13900020 - if type uri is not string
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    delete(uri: string): Promise<void>;
    /**
     * Get the index of the asset in the album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { string } photoUri - The photo asset uri.
     * @param { string } albumUri - The album uri.
     * @param { FetchOptions } options - fetch options
     * @param { AsyncCallback<number> } callback - Returns the index of the asset in the album
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getPhotoIndex(photoUri: string, albumUri: string, options: FetchOptions, callback: AsyncCallback<number>): void;
    /**
     * Get the index of the asset in the album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO
     * @param { string } photoUri - The photo asset uri.
     * @param { string } albumUri - The album uri.
     * @param { FetchOptions } options - fetch options
     * @returns { Promise<number> } - Returns the index of the asset in the album
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    getPhotoIndex(photoUri: string, albumUri: string, options: FetchOptions): Promise<number>;
    /**
     * Turn on monitor the data changes
     *
     * @param { ChangeEvent } type - One of 'deviceChange','albumChange','imageChange','audioChange','videoChange','remoteFileChange'
     * @param { Callback<void> } callback - No value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    on(type: ChangeEvent, callback: Callback<void>): void;
    /**
     * Turn off monitor the data changes
     *
     * @param { ChangeEvent } type - One of 'deviceChange','albumChange','imageChange','audioChange','videoChange','remoteFileChange'
     * @param { Callback<void> } callback - No value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    off(type: ChangeEvent, callback?: Callback<void>): void;
    /**
     * Turn on monitor for the specified uri.
     *
     * @param { string } uri - FileAsset's uri, album's uri or DefaultChangeUri
     * @param { boolean } forSubUri - Monitor the sub uri.
     * @param { Callback<ChangeData> } callback - callback function, return the ChangeData to be monitored
     * @throws { BusinessError } 13900020 - if parameter is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    on(uri: string, forSubUri: boolean, callback: Callback<ChangeData>): void;
    /**
     * Turn off monitor for the specified uri.
     *
     * @param { string } uri - FileAsset's uri, Album's uri or DefaultChangeUri value
     * @param { Callback<ChangeData> } [callback] - Remove specified callback from monitoring to a specified uri
     * @throws { BusinessError } 13900020 - if parameter is invalid
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    off(uri: string, callback?: Callback<ChangeData>): void;
    /**
     * Get Active Peer device information
     *
     * @param { AsyncCallback<Array<PeerInfo>> } callback - Callback return the list of the active peer devices' information
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    getActivePeers(callback: AsyncCallback<Array<PeerInfo>>): void;
    /**
     * Get Active Peer device information
     *
     * @returns { Promise<Array<PeerInfo>> } Promise used to return the list of the active peer devices' information
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    getActivePeers(): Promise<Array<PeerInfo>>;
    /**
     * Get all the peer devices' information
     *
     * @param { AsyncCallback<Array<PeerInfo>> } callback - Callback return the list of the all the peer devices' information
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    getAllPeers(callback: AsyncCallback<Array<PeerInfo>>): void;
    /**
     * Get all the peer devices' information
     *
     * @returns { Promise<Array<PeerInfo>> } Promise used to return the list of the all the peer devices' information
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    getAllPeers(): Promise<Array<PeerInfo>>;
    /**
     * Release UserFileManager instance
     *
     * @param { AsyncCallback<void> } callback - No value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    release(callback: AsyncCallback<void>): void;
    /**
     * Release UserFileManager instance
     *
     * @returns { Promise<void> } Return promise
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    release(): Promise<void>;
  }

  /**
   * NotifyType subtype
   *
   * @enum { number } NotifyType subtype
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum NotifyType {
    /**
     * Type for add notification of the FileAsset or Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    NOTIFY_ADD,
    /**
     * Type for update notification of the FileAsset or Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    NOTIFY_UPDATE,
    /**
     * Type for remove notification of the FileAsset or Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    NOTIFY_REMOVE,
    /**
     * Type for notification of the FileAsset added at an Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    NOTIFY_ALBUM_ADD_ASSET,
    /**
     * Type for notification of the FileAsset removed at an Album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    NOTIFY_ALBUM_REMOVE_ASSET
  }

  /**
   * DefaultChangeUri subtype
   *
   * @enum { string } DefaultChangeUri subtype
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  enum DefaultChangeUri {
    /**
     * Uri for default PhotoAsset, use with forDescendant{true}, will recieve all PhotoAsset's change notifications
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    DEFAULT_PHOTO_URI,
    /**
     * Uri for default Album, use with forDescendant{true}, will recieve all Album's change notifications
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    DEFAULT_ALBUM_URI,
    /**
     * Uri for default AudioAsset, use with forDescendant{true}, will recieve all AudioAsset's change notifications
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    DEFAULT_AUDIO_URI
  }

  /**
   * the value of the monitor callback function
   *
   * @interface ChangeData
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 10
   */
  interface ChangeData {
    /**
     * the NotifyType of ChangeData
     * 
     * @type { NotifyType }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    type: NotifyType;
    /**
     * all uris of the same NotifyType, could be FileAssets' or Albums'
     *
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    uris: Array<string>;
    /**
     * change details of the Album's FileAssets when uris is the Album's uri type
     *
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 10
     */
    subUris: Array<string>;
  }

  /**
   * Peer devices' information
   *
   * @interface PeerInfo
   * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
   * @systemapi
   * @since 9
   */
  interface PeerInfo {
    /**
     * Peer device name
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    readonly deviceName: string;
    /**
     * Peer device network id
     *
     * @type { string }
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    readonly networkId: string;
    /**
     * Peer device online status
     *
     * @type { boolean }
     * @syscap SystemCapability.FileManagement.UserFileManager.DistributedCore
     * @systemapi
     * @since 9
     */
    readonly isOnline: boolean;
  }

  /**
   * Private album type
   *
   * @enum { string } PrivateAlbumType
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  enum PrivateAlbumType {
    /**
     * System Private Album: Favorite album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    TYPE_FAVORITE,
    /**
     * System Private Album: Trash album
     *
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    TYPE_TRASH
  }

  /**
   * Defines the private album
   *
   * @interface PrivateAlbum
   * @syscap SystemCapability.FileManagement.UserFileManager.Core
   * @systemapi
   * @since 9
   */
  interface PrivateAlbum extends AbsAlbum {
    /**
     * Delete asset permanently from Trash bin, only support the Trash album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - uri of asset
     * @param { AsyncCallback<void> } callback - No value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    delete(uri: string, callback: AsyncCallback<void>): void;
    /**
     * Delete asset permanently from Trash bin, only support the Trash album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - Uri of asset
     * @returns { Promise<void> } A Promise instance, no value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    delete(uri: string): Promise<void>;
    /**
     * Recover asset from Trash bin, only support the Trash album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - Uri of asset
     * @param { AsyncCallback<void> } callback - No value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    recover(uri: string, callback: AsyncCallback<void>): void;
    /**
     * Recover asset from Trash bin, only support the Trash album
     *
     * @permission ohos.permission.READ_IMAGEVIDEO and ohos.permission.WRITE_IMAGEVIDEO or ohos.permission.READ_AUDIO and ohos.permission.WRITE_AUDIO
     * @param { string } uri - Uri of asset
     * @returns { Promise<void> } A Promise instance, no value returned
     * @syscap SystemCapability.FileManagement.UserFileManager.Core
     * @systemapi
     * @since 9
     */
    recover(uri: string): Promise<void>;
  }
}

export default userFileManager;
