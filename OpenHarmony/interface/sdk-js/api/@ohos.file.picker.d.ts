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

import { AsyncCallback, Callback } from './@ohos.base';
import Context from './application/Context';
import window  from './@ohos.window';
import type { CustomColors } from './@ohos.arkui.theme';
/**
 * The Picker module encapsulates APIs of DocumentViewPicker, AudioViewPicker, and PhotoViewPicker
 * to provide capabilities of selecting and saving files of different types.
 * An application can select the API as required. The APIs of this module must be called in UIAbility.
 * Otherwise, the FilePicker, AudioPicker, or PhotoPicker cannot be started.
 * Chinese characters and non-digit characters in the path are compiled into the corresponding ASCII code
 * and concatenated to the URI returned by calling these APIs.
 *
 * @namespace picker
 * @syscap SystemCapability.FileManagement.UserFileService
 * @since 9
 */
/**
 * The Picker module encapsulates APIs of DocumentViewPicker, AudioViewPicker, and PhotoViewPicker
 * to provide capabilities of selecting and saving files of different types.
 * An application can select the API as required. The APIs of this module must be called in UIAbility.
 * Otherwise, the FilePicker, AudioPicker, or PhotoPicker cannot be started.
 * Chinese characters and non-digit characters in the path are compiled into the corresponding ASCII code
 * and concatenated to the URI returned by calling these APIs.
 *
 * @namespace picker
 * @syscap SystemCapability.FileManagement.UserFileService
 * @atomicservice
 * @since 11
 */
/**
 * The Picker module encapsulates APIs of DocumentViewPicker, AudioViewPicker, and PhotoViewPicker
 * to provide capabilities of selecting and saving files of different types.
 * An application can select the API as required. The APIs of this module must be called in UIAbility.
 * Otherwise, the FilePicker, AudioPicker, or PhotoPicker cannot be started.
 * Chinese characters and non-digit characters in the path are compiled into the corresponding ASCII code
 * and concatenated to the URI returned by calling these APIs.
 *
 * @namespace picker
 * @syscap SystemCapability.FileManagement.UserFileService
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare namespace picker {
  /**
   * Enumerates the media file types that can be selected.
   *
   * @enum { string } PhotoViewMIMETypes
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Enumerates the media file types that can be selected.
   *
   * @enum { string } PhotoViewMIMETypes
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 11
   * @deprecated since 12
   * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoViewMIMETypes
   */
  export enum PhotoViewMIMETypes {
    /**
     * Image type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Image type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoViewMIMETypes.IMAGE_TYPE
     */
    IMAGE_TYPE = 'image/*',

    /**
     * Video type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Video type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoViewMIMETypes.VIDEO_TYPE
     */
    VIDEO_TYPE = 'video/*',

    /**
     * Image and video type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Image and video type.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoViewMIMETypes.IMAGE_VIDEO_TYPE
     */
    IMAGE_VIDEO_TYPE = '*/*'
  }

  /**
   * Defines the options for selecting images/videos.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Defines the options for selecting images/videos.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 11
   * @deprecated since 12
   * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectOptions
   */
  class PhotoSelectOptions {
    /**
     * Media file types to select. IMAGE_VIDEO_TYPE is used by default.
     *
     * @type { ?PhotoViewMIMETypes }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Media file types to select. IMAGE_VIDEO_TYPE is used by default.
     *
     * @type { ?PhotoViewMIMETypes }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectOptions.MIMEType
     */
    MIMEType?: PhotoViewMIMETypes;

    /**
     * Maximum number of media files that can be selected. 
     * The default value is 50, and the maximum value is 500.
     *
     * @type { ?number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Maximum number of media files that can be selected. 
     * The default value is 50, and the maximum value is 500.
     *
     * @type { ?number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectOptions.maxSelectNumber
     */
    maxSelectNumber?: number;
  }

  /**
   * Defines information about the images or videos selected.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Defines information about the images or videos selected.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 11
   * @deprecated since 12
   * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectResult
   */
  class PhotoSelectResult {
    /**
     * URIs of the media files selected. 
     * This URI array can be used only by photoAccessHelper.getAssets. For details, see Using a Media File URI.
     *
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * URIs of the media files selected. 
     * This URI array can be used only by photoAccessHelper.getAssets. For details, see Using a Media File URI.
     *
     * @type { Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectResult.photoUris
     */
    photoUris: Array<string>;

    /**
     * Whether the selected image is the original one. 
     * The value true means the selected image is the original one; the value false means the opposite.
     *
     * @type { boolean }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Whether the selected image is the original one. 
     * The value true means the selected image is the original one; the value false means the opposite.
     *
     * @type { boolean }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 18
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoSelectResult.isOriginalPhoto
     */
    isOriginalPhoto: boolean;
  }

  /**
   * Defines the options for saving images or videos.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   * @deprecated since 12
   */
  class PhotoSaveOptions {
    /**
     * File names of the images or videos to save. If this parameter is not specified, the user needs to enter the file names.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     * @deprecated since 18
     */
    newFileNames?: Array<string>;
  }

  /**
   * Provides APIs for selecting and saving images or videos.
   * You are advised to use PhotoViewPicker of PhotoAccessHelper to select files.
   * Before using the APIs of PhotoViewPicker, you need to create a PhotoViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Provides APIs for selecting and saving images or videos.
   * You are advised to use PhotoViewPicker of PhotoAccessHelper to select files.
   * Before using the APIs of PhotoViewPicker, you need to create a PhotoViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 11
   * @deprecated since 12
   * @useinstead ohos.file.photoAccessHelper/photoAccessHelper#PhotoViewPicker
   */
  class PhotoViewPicker {
    /**
     * A constructor used to create a PhotoViewPicker instance.
     * This constructor is not recommended due to the potential risk of operation failure.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     * @deprecated since 18
     */
    constructor();

    /**
     * A constructor used to create a PhotoViewPicker instance. This constructor is recommended.
     * For details about how to obtain the context, see getHostContext.
     *
     * @param { Context } context - represents the context.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 12
     * @deprecated since 18
     */
    constructor(context: Context);

    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses a promise to return the result.
     * You can pass in PhotoSelectOptions to specify the type and maximum number of the files to select.
     *
     * @param { PhotoSelectOptions } option - Options for selecting images or videos.
     * If this parameter is not specified, images and videos are selected by default.
     * A maximum of 50 files can be selected.
     * @returns { Promise<PhotoSelectResult> } Promise used to return the URIs of the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses a promise to return the result.
     * You can pass in PhotoSelectOptions to specify the type and maximum number of the files to select.
     *
     * @param { PhotoSelectOptions } option - Options for selecting images or videos.
     * If this parameter is not specified, images and videos are selected by default.
     * A maximum of 50 files can be selected.
     * @returns { Promise<PhotoSelectResult> } Promise used to return the URIs of the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 12
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper.PhotoViewPicker#select
     */
    select(option?: PhotoSelectOptions): Promise<PhotoSelectResult>;

    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses an asynchronous callback to return the result.
     * You can pass in PhotoSelectOptions to specify the type and maximum number of the files to select.
     *
     * @param { PhotoSelectOptions } option - Options for selecting images or videos.
     * @param { AsyncCallback<PhotoSelectResult> } callback - Callback used to return the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses an asynchronous callback to return the result.
     * You can pass in PhotoSelectOptions to specify the type and maximum number of the files to select.
     *
     * @param { PhotoSelectOptions } option - Options for selecting images or videos.
     * @param { AsyncCallback<PhotoSelectResult> } callback - Callback used to return the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 12
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper.PhotoViewPicker#select
     */
    select(option: PhotoSelectOptions, callback: AsyncCallback<PhotoSelectResult>): void;

    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses an asynchronous callback to return the result.
     *
     * @param { AsyncCallback<PhotoSelectResult> } callback - Callback used to return the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a photoPicker page for the user to select one or more images or videos.
     * This API uses an asynchronous callback to return the result.
     *
     * @param { AsyncCallback<PhotoSelectResult> } callback - Callback used to return the images or videos selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 11
     * @deprecated since 12
     * @useinstead ohos.file.photoAccessHelper/photoAccessHelper.PhotoViewPicker#select
     */
    select(callback: AsyncCallback<PhotoSelectResult>): void;

    /**
     * Starts a photoPicker page for the user to save one or more images or videos.
     * This API uses a promise to return the result.
     * You can pass in PhotoSaveOptions to specify the URIs of the images or videos to save.
     *
     * @param { PhotoSaveOptions } option - Options for saving images or videos. If this parameter is not specified,
     * a photoPicker page will be displayed for the user to enter the names of the files to save.
     * @returns { Promise<Array<string>> } Returns the uris for the saved files.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     * @deprecated since 12
     * @useinstead save_button/SaveButton
     */
    save(option?: PhotoSaveOptions): Promise<Array<string>>;

    /**
     * Starts a photoPicker page for the user to save one or more images or videos.
     * This API uses an asynchronous callback to return the result.
     * You can pass in PhotoSaveOptions to specify the URIs of the images or videos to save.
     *
     * @param { PhotoSaveOptions } option - Options for saving images or videos.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the images or videos saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     * @deprecated since 12
     * @useinstead save_button/SaveButton
     */
    save(option: PhotoSaveOptions, callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts a photoPicker page for the user to save one or more images or videos.
     * This API uses an asynchronous callback to return the URIs of the images or videos to save.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the images or videos saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     * @deprecated since 12
     * @useinstead save_button/SaveButton
     */
    save(callback: AsyncCallback<Array<string>>): void;
  }

  /**
   * Enumerates the types of documents selected.
   * Only 2-in-1 devices are supported.
   *
   * @enum { number } DocumentSelectMode
   * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
   * @since 11
   */
  /**
   * Enumerates the types of documents selected.
   * Only 2-in-1 devices are supported.
   *
   * @enum { number } DocumentSelectMode
   * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  export enum DocumentSelectMode {
    /**
     * File (default).
     * Indicates that only files are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @since 11
     */
    /**
     * File (default).
     * Indicates that only files are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    FILE = 0,

    /**
     * Folder.
     * Indicates that only folders are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @since 11
     */
    /**
     * Folder.
     * Indicates that only folders are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    FOLDER = 1,

    /**
     * Mixed type of files and folders.
     * Indicates that files and folders are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @since 11
     */
    /**
     * Mixed type of files and folders.
     * Indicates that files and folders are allowed to be selected.
     *
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @atomicservice
     * @since 12
     */
    MIXED = 2,
  }

  /**
   * Enumerates the modes for saving documents.
   *
   * @enum { number } DocumentPickerMode
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 12
   */
    export enum DocumentPickerMode {
      /**
       * Standard mode.
       *
       * @syscap SystemCapability.FileManagement.UserFileService
       * @atomicservice
       * @since 12
       */
      DEFAULT = 0,
  
      /**
       * Download mode.
       *
       * @syscap SystemCapability.FileManagement.UserFileService
       * @atomicservice
       * @since 12
       */
      DOWNLOAD = 1,
    }

  /**
   * Enumerates file aggregation types. Only mobile phones are supported.
   *
   * @enum { number } DocumentPickerMode
   * @syscap SystemCapability.FileManagement.UserFileService
   * @atomicservice
   * @since 15
   */
  export enum MergeTypeMode {
    /**
     * Default mode, indicating that this parameter does not take effect.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    DEFAULT = 0,

    /**
     * Audio mode.
     * Indicates that audios are allowed to be selected
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    AUDIO = 1,

    /**
     * Video mode.
     * Indicates that videos are allowed to be selected
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    VIDEO = 2,

    /**
     * Document mode.
     * Indicates that documents are allowed to be selected
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    DOCUMENT = 3,

    /**
     * Image mode.
     * Indicates that pictures are allowed to be selected
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    PICTURE = 4,
  }

  /**
   * Defines the options for selecting documents.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Defines the options for selecting documents.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class DocumentSelectOptions {
    /**
     * Path of the document or directory to select.
     * It is empty by default (the recently opened page is displayed).
     *
     * @type { ?string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 10
     */
    /**
     * Path of the document or directory to select.
     * It is empty by default (the recently opened page is displayed).
     *
     * @type { ?string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    defaultFilePathUri?: string;

    /**
     * Document suffix of the document to select. The value is a string array. 
     * Each element specifies an option, which includes at most two parts with a vertical bar (|) in between.
     * The first part is the description, and the second part is the document suffix.
     * If there is no "|", the option does not have the description.
     * Multiple document suffixes separated by a comma (,) are allowed in an option.
     * The number of elements in a string array cannot exceed 100.
     * This parameter is available only to the devices that have the required system capability.
     * By default, no filtering is performed, that is, all documents are selected.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 10
     */
    /**
     * Document suffix of the document to select. The value is a string array. 
     * Each element specifies an option, which includes at most two parts with a vertical bar (|) in between.
     * The first part is the description, and the second part is the document suffix.
     * If there is no "|", the option does not have the description.
     * Multiple document suffixes separated by a comma (,) are allowed in an option.
     * The number of elements in a string array cannot exceed 100.
     * This parameter is available only to the devices that have the required system capability.
     * By default, no filtering is performed, that is, all documents are selected.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    fileSuffixFilters?: Array<string>;

    /**
     * Maximum number of documents that can be selected.
     * Value range: 1 to 500.
     * Only the devices with the required system capability can select directories,
     * and only one directory can be selected at a time.
     * Default value: 1.
     *
     * @type { ?number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 10
     */
    /**
     * Maximum number of documents that can be selected.
     * Value range: 1 to 500.
     * Only the devices with the required system capability can select directories,
     * and only one directory can be selected at a time.
     * Default value: 1.
     *
     * @type { ?number }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    maxSelectNumber?: number;

    /**
     * Selection mode. Only 2-in-1 devices are supported. The default value is FILE.
     *
     * @type { ?DocumentSelectMode }
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @since 11
     */
    /**
     * Selection mode. Only 2-in-1 devices are supported. The default value is FILE.
     *
     * @type { ?DocumentSelectMode }
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    selectMode?: DocumentSelectMode;

    /**
     * Whether to start Picker.
     * Default value: false.
     * If authMode is true, defaultFilePathUri is mandatory, which specifies the URI of the file allowed to access.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @atomicservice
     * @since 12
     */
    authMode?: boolean;

    /**
     * Whether to enable the batch authorization mode.
     * The value false (default) means to disable the batch authorization mode;
     * the value true means to enable the batch authorization mode.
     * The multiUriArray parameter only takes effect when multAuthMode is set to true.
     * Only mobile phones are supported.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    multiAuthMode?: boolean;

    /**
     * 	Whether to pass the URIs for batch authorization (only files are supported).
     *  This parameter is used with multAuthMode, and does not take effect when multAuthMode is set to false.
     *  By default, this parameter is left empty.(The files displayed on the batch authorization page are empty.)
     *  Only mobile phones are supported.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    multiUriArray?: Array<string>;  

    /**
     * Whether to enable the aggregation view mode for a file management application.
     * The default value is DEFAULT, indicating that this parameter does not take effect and the aggregation view is disabled.
     * If this parameter is set to a value other than DEFAULT, other parameters do not take effect.
     * Only mobile phones are supported.
     *
     * @type { ?MergeTypeMode }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 15
     */
    mergeMode?: MergeTypeMode;

    /**
     * Whether to support encryption (only files are supported). 
     * The default value is false.
     * If this parameter is set to true, the picker will display a button that allows the user, 
     * files can be encrypted on the Picker page.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 19
     */
    isEncryptionSupported?: boolean;
    
    /**
     * Theme color
     *
     * @type { ?CustomColors }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 18
     */
    themeColor?: CustomColors;
  }

  /**
   * Defines the options for saving documents.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Defines the options for saving documents.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class DocumentSaveOptions {
    /**
     * Name of the document to save.
     * If this parameter is not specified, the user needs to enter the the document name.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Name of the document to save.
     * If this parameter is not specified, the user needs to enter the the document name.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    newFileNames?: Array<string>;

    /**
     * Path of the document or directory to save.
     *
     * @type { ?string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 10
     */
    /**
     * Path of the document or directory to save.
     *
     * @type { ?string }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    defaultFilePathUri?: string;

    /**
     * Document suffix of the document to save. The value is a string array.
     * Each element specifies an option, which includes at most two parts with a vertical bar (|) in between.
     * The first part is the description, and the second part is the document suffix.
     * If there is no "|", the option does not have the description. By default, all documents are saved.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 10
     */
    /**
     * Document suffix of the document to save. The value is a string array.
     * Each element specifies an option, which includes at most two parts with a vertical bar (|) in between.
     * The first part is the description, and the second part is the document suffix.
     * If there is no "|", the option does not have the description. By default, all documents are saved.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    fileSuffixChoices?: Array<string>;
    /**
     * Mode for starting Picker. Default value: DEFAULT.
     * If pickerMode is DOWNLOAD,
     * the settings of newFileNames, defaultFilePathUri, and fileSuffixChoices do not take effect.
     *
     * @type { ?DocumentPickerMode }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    pickerMode?: DocumentPickerMode;

    /**
     * Theme color
     *
     * @type { ?CustomColors }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @systemapi
     * @since 18
     */
    themeColor?: CustomColors;
  }

  /**
   * Provides APIs for selecting and saving documents in different formats.
   * Before using the APIs of DocumentViewPicker, you need to create a DocumentViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Provides APIs for selecting and saving documents in different formats.
   * Before using the APIs of DocumentViewPicker, you need to create a DocumentViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class DocumentViewPicker {
    /**
     * A constructor used to create a DocumentViewPicker instance.
     * This constructor is not recommended due to the potential risk of operation failure.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    constructor();

    /**
     * A constructor used to create a DocumentViewPicker instance.
     * This constructor is recommended. For details about how to obtain the context, see getHostContext.
     *
     * @param { Context } context - Application context (only UIAbilityContext is supported).
     * For details about the application context of the stage model, see Context.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    constructor(context: Context);

    /**
     * A constructor used to create a DocumentViewPicker object in a window created by an application.
     * In other scenarios, you are advised to use constructor(context: Context) to create a DocumentViewPicker object.
     *
     * @param { Context } context - Application context (only UIAbilityContext is supported).
     * For details about the application context of the stage model, see Context.
     * @param { window.Window } window - Window instance created by the application.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 13
     */
    constructor(context: Context, window: window.Window);

    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses a promise to return the result.
     * You can pass in DocumentSelectOptions to specify the URIs of the documents to select.
     *
     * @param { DocumentSelectOptions } option - Options for selecting documents.
     * If this parameter is not specified, the documentPicker page is displayed by default.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses a promise to return the result.
     * You can pass in DocumentSelectOptions to specify the URIs of the documents to select.
     *
     * @param { DocumentSelectOptions } option - Options for selecting documents.
     * If this parameter is not specified, the documentPicker page is displayed by default.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    select(option?: DocumentSelectOptions): Promise<Array<string>>;

    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses an asynchronous callback to return the result.
     * You can pass in DocumentSelectOptions to specify the URIs of the documents to select.
     *
     * @param { DocumentSelectOptions } option - Options for selecting documents.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses an asynchronous callback to return the result.
     * You can pass in DocumentSelectOptions to specify the URIs of the documents to select.
     *
     * @param { DocumentSelectOptions } option - Options for selecting documents.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    select(option: DocumentSelectOptions, callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses an asynchronous callback to return the URIs of the documents selected.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to select one or more documents.
     * This API uses an asynchronous callback to return the URIs of the documents selected.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    select(callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses a promise to return the result.
     * You can pass in DocumentSaveOptions to specify the URIs of the documents to save.
     *
     * @param { DocumentSaveOptions } option - Options for saving the documents. If this parameter is not specified,
     * a documentPicker page will be displayed for the user to enter the names of the documents to save.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses a promise to return the result.
     * You can pass in DocumentSaveOptions to specify the URIs of the documents to save.
     *
     * @param { DocumentSaveOptions } option - Options for saving the documents. If this parameter is not specified,
     * a documentPicker page will be displayed for the user to enter the names of the documents to save.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    save(option?: DocumentSaveOptions): Promise<Array<string>>;

    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses an asynchronous callback to return the result.
     * You can pass in DocumentSaveOptions to specify the URIs of the documents to save.
     *
     * @param { DocumentSaveOptions } option - Options for saving the documents.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses an asynchronous callback to return the result.
     * You can pass in DocumentSaveOptions to specify the URIs of the documents to save.
     *
     * @param { DocumentSaveOptions } option - Options for saving the documents.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    save(option: DocumentSaveOptions, callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses an asynchronous callback to return the URIs of the documents to save.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts a documentPicker page for the user to save one or more documents.
     * This API uses an asynchronous callback to return the URIs of the documents to save.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the documents saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    save(callback: AsyncCallback<Array<string>>): void;

    /**
     * Obtains the index of the file suffix type of the file saved.
     * Only 2-in-1 devices are supported.
     * This method takes effect only when used with save().
     * This method can be used only after DocumentSaveOptions.
     * fileSuffixChoices is configured.
     * The index (number) returned by this method indicates the location of
     * the file suffix specified in DocumentSaveOptions.fileSuffixChoices.
     * If no file suffix is specified, getSelectedIndex() returns -1.
     *
     * @returns { number } Returns the index of the suffix array fileSuffixChoices configured by the user in DocumentSaveOptions.
     * @syscap SystemCapability.FileManagement.UserFileService.FolderSelection
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    getSelectedIndex(): number;
  }

  /**
   * Defines the options for selecting audio clips.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
    /**
   * Defines the options for selecting audio clips.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
    class AudioSelectOptions {
      /**
       * Maximum number of audio clips that can be selected.
       * Default value: 1. Value range: 1 to 500
       *
       * @type { ?number }
       * @syscap SystemCapability.FileManagement.UserFileService
       * @crossplatform
       * @atomicservice
       * @since 12
       */
      maxSelectNumber?: number;
    }

  /**
   * Defines the options for saving audio clips.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Defines the options for saving audio clips.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class AudioSaveOptions {
    /**
     * File names of the audio clips to save.
     * If this parameter is not specified, the user needs to enter the file names.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * File names of the audio clips to save.
     * If this parameter is not specified, the user needs to enter the file names.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    newFileNames?: Array<string>;
  }

  /**
   * Provides APIs for selecting and saving audio clips.
   * Before using the APIs of AudioViewPicker, you need to create an AudioViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @since 9
   */
  /**
   * Provides APIs for selecting and saving audio clips.
   * Before using the APIs of AudioViewPicker, you need to create an AudioViewPicker instance.
   *
   * @syscap SystemCapability.FileManagement.UserFileService
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  class AudioViewPicker {
    /**
     * A constructor used to create an AudioViewPicker instance.
     * This constructor is not recommended due to the potential risk of operation failure.
     *
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    constructor();

    /**
     * A constructor used to create an AudioViewPicker instance.
     * This constructor is recommended. For details about how to obtain the context, see getHostContext.
     *
     * @param { Context } context - Application context (only UIAbilityContext is supported).
     * For details about the application context of the stage model, see Context.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @atomicservice
     * @since 12
     */
    constructor(context: Context);

    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses a promise to return the result.
     * You can pass in AudioSelectOptions to specify the URIs of the audio clips to select.
     *
     * @param { AudioSelectOptions } option - Options for selecting audio clips.
     * If this parameter is not specified, the audioPicker page is displayed by default.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses a promise to return the result.
     * You can pass in AudioSelectOptions to specify the URIs of the audio clips to select.
     *
     * @param { AudioSelectOptions } option - Options for selecting audio clips.
     * If this parameter is not specified, the audioPicker page is displayed by default.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    select(option?: AudioSelectOptions): Promise<Array<string>>;

    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses an asynchronous callback to return the result.
     * You can pass in AudioSelectOptions to specify the URIs of the audio clips to select.
     *
     * @param { AudioSelectOptions } option - Options for selecting audio clips.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses an asynchronous callback to return the result.
     * You can pass in AudioSelectOptions to specify the URIs of the audio clips to select.
     *
     * @param { AudioSelectOptions } option - Options for selecting audio clips.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @since 12
     */
    select(option: AudioSelectOptions, callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses an asynchronous callback to return the URIs of the audio clips selected.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page for the user to select one or more audio clips.
     * This API uses an asynchronous callback to return the URIs of the audio clips selected.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips selected.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @since 12
     */
    select(callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses a promise to return the result.
     * You can pass in AudioSaveOptions to specify the URIs of the audio clips to save.
     *
     * @param { AudioSaveOptions } option - Options for saving audio clips.If this parameter is not specified,
     * an audioPicker page will be displayed for the user to enter the names of the files to save.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses a promise to return the result.
     * You can pass in AudioSaveOptions to specify the URIs of the audio clips to save.
     *
     * @param { AudioSaveOptions } option - Options for saving audio clips.If this parameter is not specified,
     * an audioPicker page will be displayed for the user to enter the names of the files to save.
     * @returns { Promise<Array<string>> } Promise used to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    save(option?: AudioSaveOptions): Promise<Array<string>>;

    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses an asynchronous callback to return the result.
     * You can pass in AudioSaveOptions to specify the URIs of the audio clips to save.
     *
     * @param { AudioSaveOptions } option - Options for saving audio clips.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses an asynchronous callback to return the result.
     * You can pass in AudioSaveOptions to specify the URIs of the audio clips to save.
     *
     * @param { AudioSaveOptions } option -Options for saving audio clips.
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @since 12
     */
    save(option: AudioSaveOptions, callback: AsyncCallback<Array<string>>): void;

    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses an asynchronous callback to return the URIs of the audio clips saved.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @since 9
     */
    /**
     * Starts an audioPicker page (currently, a documentPicker page is displayed)
     * for the user to save one or more audio clips.
     * This API uses an asynchronous callback to return the URIs of the audio clips saved.
     *
     * @param { AsyncCallback<Array<string>> } callback - Callback invoked to return the URIs of the audio clips saved.
     * @syscap SystemCapability.FileManagement.UserFileService
     * @crossplatform
     * @since 12
     */
    save(callback: AsyncCallback<Array<string>>): void;
  }
}

export default picker;
