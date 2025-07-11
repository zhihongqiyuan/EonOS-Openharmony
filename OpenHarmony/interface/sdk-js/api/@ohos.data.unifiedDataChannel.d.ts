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
 * @kit ArkData
 */

import { AsyncCallback } from './@ohos.base';
import image from "./@ohos.multimedia.image";
import Want from "./@ohos.app.ability.Want";

/**
 * As a part of the Unified Data Management Framework (UDMF), the unifiedDataChannel module provides unified data
 * channels and standard data access interfaces for many-to-many data sharing across applications. It also provides
 * definitions for uniform data types, such as text and image, to streamline data interaction between different
 * applications and minimize the workload of data type adaptation. Although the UDMF does not parse user data, you are
 * advised not to transfer sensitive personal data or privacy data due to low-level security of storage path.
 *
 * @namespace unifiedDataChannel
 * @syscap SystemCapability.DistributedDataManager.UDMF.Core
 * @since 10
 */
/**
 * As a part of the Unified Data Management Framework (UDMF), the unifiedDataChannel module provides unified data
 * channels and standard data access interfaces for many-to-many data sharing across applications. It also provides
 * definitions for uniform data types, such as text and image, to streamline data interaction between different
 * applications and minimize the workload of data type adaptation. Although the UDMF does not parse user data, you are
 * advised not to transfer sensitive personal data or privacy data due to low-level security of storage path.
 *
 * @namespace unifiedDataChannel
 * @syscap SystemCapability.DistributedDataManager.UDMF.Core
 * @atomicservice
 * @since 11
 */
/**
 * As a part of the Unified Data Management Framework (UDMF), the unifiedDataChannel module provides unified data
 * channels and standard data access interfaces for many-to-many data sharing across applications. It also provides
 * definitions for uniform data types, such as text and image, to streamline data interaction between different
 * applications and minimize the workload of data type adaptation. Although the UDMF does not parse user data, you are
 * advised not to transfer sensitive personal data or privacy data due to low-level security of storage path.
 *
 * @namespace unifiedDataChannel
 * @syscap SystemCapability.DistributedDataManager.UDMF.Core
 * @crossplatform
 * @atomicservice
 * @since 14
 */
declare namespace unifiedDataChannel {
  /**
   * Types of scope that UnifiedData can be used.
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 12
   */
  enum ShareOptions {
    /**
     * IN_APP indicates that only use in the same app is allowed.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    IN_APP,
    /**
     * CROSS_APP indicates that use in any app in this device is allowed.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    CROSS_APP
  }

  /**
   * Indicated delay get UnifiedData
   * 
   * @typedef {function} GetDelayData
   * @param { string } type - the type of UnifiedData required.
   * @returns { UnifiedData } Return the UnifiedData required.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 12
   */
  type GetDelayData = (type: string) => UnifiedData;

  /**
   * Indicates type of value.
   * @typedef {number | string | boolean | image.PixelMap | Want | ArrayBuffer | object | null | undefined}
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Indicates type of value.
   * @typedef {number | string | boolean | image.PixelMap | Want | ArrayBuffer | object | null | undefined}
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  type ValueType = number | string | boolean | image.PixelMap | Want | ArrayBuffer | object | null | undefined;

  /**
   * Describe the unified data properties.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 12
   */
  class UnifiedDataProperties {
    /**
     * extra property data. key-value pairs.
     * @type { ?Record<string, object> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    extras?: Record<string, object>;

    /**
     * the user-defined tag of a UnifiedData object.
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    tag?: string;
    /**
     * a timestamp, which indicates when data is written.
     * @type { ?Date }
     * @readonly
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    readonly timestamp?: Date;
    /**
     * Indicates the scope of clipboard data which can be used.
     * If it is not set or is incorrectly set, The default value is CrossDevice.
     * @type { ?ShareOptions }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    shareOptions?: ShareOptions;

    /**
     * Indicated delay get UnifiedData.
     * @type { ?GetDelayData }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    getDelayData?: GetDelayData;
  }

  /**
   * Describe the unified data.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified data.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified data.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class UnifiedData {
    /**
     * Create unified data with a record
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Create unified data with a record
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
     /**
     * Create unified data with a record
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    constructor(record: UnifiedRecord);
    /**
     * Create a empty unified data.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Create a empty unified data.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    constructor();
    /**
     * Add a record into unified data
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Add a record into unified data
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Add a record into unified data
     *
     * @param { UnifiedRecord } record - Record will add into unified data.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    addRecord(record: UnifiedRecord): void;
    /**
     * Get all records of unified data
     *
     * @returns { Array<UnifiedRecord> } Return the records of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Get all records of unified data
     *
     * @returns { Array<UnifiedRecord> } Return the records of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
     /**
     * Get all records of unified data
     *
     * @returns { Array<UnifiedRecord> } Return the records of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    getRecords(): Array<UnifiedRecord>;

    /**
     * Checks whether this UnifiedData object contains the specified data type, including the data types added by using
     * the addEntry function.
     * For file types, if the type set of UnifiedData contains general.jpeg, true is returned when the hasType API is
     * called to check whether the general.image type is included, because the general.jpeg type belongs to the
     * general.image type.
     * @param { string } type - indicates to query data type.
     * @returns { boolean } if having mimeType in UnifiedData returns true, else returns false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Checks whether this UnifiedData object contains the specified data type, including the data types added by using
     * the addEntry function.
     * For file types, if the type set of UnifiedData contains general.jpeg, true is returned when the hasType API is
     * called to check whether the general.image type is included, because the general.jpeg type belongs to the
     * general.image type.
     * @param { string } type - indicates to query data type.
     * @returns { boolean } if having mimeType in UnifiedData returns true, else returns false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    hasType(type: string): boolean;

    /**
     * UTD types of all content in the UnifiedData.
     * @returns { Array<string> } type of array
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * UTD types of all content in the UnifiedData.
     * @returns { Array<string> } type of array
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    getTypes(): Array<string>;

    /**
     * UnifiedData properties.
     * @type { UnifiedDataProperties }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    properties: UnifiedDataProperties;
  }

  /**
   * Summarizes the data information of the unifiedData object, including the data type and size.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Summarizes the data information of the unifiedData object, including the data type and size.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class Summary {
    /**
     * A map for each type and data size, key is data type, value is the corresponding data size
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * A map for each type and data size, key is data type, value is the corresponding data size
     *
     * @type { Record<string, number> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    summary: Record<string, number>;
    /**
     * Total data size of data in Bytes
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Total data size of data in Bytes
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    totalSize: number;
  }

  /**
   * Describe the unified record
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified record
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * An abstract definition of the data content supported by the UDMF. A UnifiedRecord object contains one or more data
   * records, for example, a text record, an image record, or an HTML record. Since API version 15, different styles of
   * the same content can be added to a UnifiedRecord object. Data users can obtain the corresponding styles as
   * required.
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class UnifiedRecord {
    /**
     * Get type of unified record
     *
     * @returns { string } Return the type of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Get type of unified record
     *
     * @returns { string } Return the type of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Get type of unified record
     *
     * @returns { string } Return the type of unified data
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    getType(): string;

    /**
     * Create unified record.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Create unified record.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    constructor();

    /**
     * Create unified record by type and value.
     *
     * @param { string } type - indicates to data type of unified record. It can not be empty. When type of value is
     * object, parameter type must be pixel-map or want UTD type.
     * @param { ValueType } value - indicates to value of unified record.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types;
     * <br>3.Parameter verification failed.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Create unified record by type and value.
     *
     * @param { string } type - indicates to data type of unified record. It can not be empty. When type of value is
     * object, parameter type must be pixel-map or want UTD type.
     * @param { ValueType } value - indicates to value of unified record.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types;
     * <br>3.Parameter verification failed.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    constructor(type: string, value: ValueType);

    /**
     * Get the value of unified record.
     *
     * @returns { ValueType } Return the value of unified record.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Get the value of unified record.
     *
     * @returns { ValueType } Return the value of unified record.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    getValue(): ValueType;

    /**
     * Obtains all the data types in the data record. This API can be called using the UnifiedRecord object to query all
     * data types in the record, including the data types added using the addEntry function.
     *
     * @returns { Array<string> } Return the types of unified record.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    getTypes(): Array<string>;

    /**
     * Adds data of a specified data type and content to the current data record. You can use this API to add different
     * data types and contents to the same data.
     *
     * @param { string } type - type of entry.
     * @param { ValueType } value - value of entry.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    addEntry(type: string, value: ValueType): void;

    /**
     * Get an entry from unified record
     *
     * @param { string } type - type of entry want to get.
     * @returns { ValueType } Return the value corresponding to the type.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
     * <br>2.Incorrect parameters types.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    getEntry(type: string): ValueType;

    /**
     * Get entries from unified record
     *
     * @returns { Record<string, ValueType> } Return all entries in the record.
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    getEntries(): Record<string, ValueType>;
  }

  /**
   * Describe the unified text data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified text data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified text data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Text extends UnifiedRecord {
    /**
     * Indicates the details of unified text
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the details of unified text
     *
     * @type { ?Record<string, string> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the details of unified text
     *
     * @type { ?Record<string, string> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    details?: Record<string, string>;
  }

  /**
   * Describe the unified plain text data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified plain text data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified plain text data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class PlainText extends Text {
    /**
     * Indicates the content of text
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the content of text
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the content of text
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    textContent: string;
    /**
     * Indicates the abstract of text
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the abstract of text
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the abstract of text
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    abstract?: string;
  }

  /**
   * Describe the unified link data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified link data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified link data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Hyperlink extends Text {
    /**
     * Indicates the url of a link
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the url of a link
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the url of a link
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    url: string;
    /**
     * Indicates the description of a link
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the description of a link
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the description of a link
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    description?: string;
  }

  /**
   * Describe the unified html data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified html data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified html data
   *
   * @extends Text
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class HTML extends Text {
    /**
     * Indicates the content of html, with html tags
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the content of html, with html tags
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the content of html, with html tags
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    htmlContent: string;
    /**
     * Indicates the plain content of html
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the plain content of html
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the plain content of html
     *
     * @type { ?string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    plainContent?: string;
  }

  /**
   * Describe the unified file data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified file data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified file data
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class File extends UnifiedRecord {
    /**
     * Indicates the details of unified File
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the details of unified File
     *
     * @type { ?Record<string, string> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * Indicates the details of unified File
     *
     * @type { ?Record<string, string> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    details?: Record<string, string>;
    /**
     * URI of the local file or online file. The local file URI can be obtained using the getUriFromPath function.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * URI of the local file or online file. The local file URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * URI of the local file or online file. The local file URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    uri: string;
  }

  /**
   * Describe the unified image data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified image data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified image data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Image extends File {
    /**
     * URI of the local image or online image. The local image URI can be obtained using the getUriFromPath function.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * URI of the local image or online image. The local image URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * URI of the local image or online image. The local image URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    imageUri: string;
  }

  /**
   * Describe the unified video data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified video data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified video data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Video extends File {
    /**
     * URI of the local video or online video. The local video URI can be obtained using the getUriFromPath function.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * URI of the local video or online video. The local video URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * URI of the local video or online video. The local video URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    videoUri: string;
  }

  /**
   * Describe the unified audio data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified audio data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified audio data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Audio extends File {
    /**
     * URI of the local audio or online audio. The local audio URI can be obtained using the getUriFromPath function.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * URI of the local audio or online audio. The local audio URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * URI of the local audio or online audio. The local audio URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    audioUri: string;
  }

  /**
   * Describe the unified folder data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the unified folder data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Describe the unified folder data
   *
   * @extends File
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @crossplatform
   * @atomicservice
   * @since 14
   */
  class Folder extends File {
    /**
     * URI of the local folder or online folder. The local folder URI can be obtained using the getUriFromPath function.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * URI of the local folder or online folder. The local folder URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    /**
     * URI of the local folder or online folder. The local folder URI can be obtained using the getUriFromPath function.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @crossplatform
     * @atomicservice
     * @since 14
     */
    folderUri: string;
  }

  /**
   * Describe system defined type data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe system defined type data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class SystemDefinedRecord extends UnifiedRecord {
    /**
     * Indicates the details of system defined data
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the details of system defined data
     *
     * @type { ?Record<string, number | string | Uint8Array> }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    details?: Record<string, number | string | Uint8Array>;
  }

  /**
   * Describe system defined form data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe system defined form data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class SystemDefinedForm extends SystemDefinedRecord {
    /**
     * Indicates the id of form
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the id of form
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    formId: number;
    /**
     * Indicates the name of form
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the name of form
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    formName: string;
    /**
     * Indicates the bundle name of form
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the bundle name of form
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    bundleName: string;
    /**
     * Indicates the ability name of form
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the ability name of form
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    abilityName: string;
    /**
     * Indicates the module of form
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the module of form
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    module: string;
  }

  /**
   * Describe system defined app item data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe system defined app item data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class SystemDefinedAppItem extends SystemDefinedRecord {
    /**
     * Indicates the app id
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the app id
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    appId: string;
    /**
     * Indicates the app name
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the app name
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    appName: string;
    /**
     * Indicates the id of app icon
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the id of app icon
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    appIconId: string;
    /**
     * Indicates the id of app label
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the id of app label
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    appLabelId: string;
    /**
     * Indicates the bundle name of app
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the bundle name of app
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    bundleName: string;
    /**
     * Indicates the ability name of app
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the ability name of app
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    abilityName: string;
  }

  /**
   * Describe system defined pixel map data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe system defined pixel map data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends SystemDefinedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class SystemDefinedPixelMap extends SystemDefinedRecord {
    /**
     * Indicates the raw data of pixel map
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the raw data of pixel map
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    rawData: Uint8Array;
  }

  /**
   * Describe application defined data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe application defined data(this kind of data is provided and bound to OpenHarmony,
   * also can be parsed by system provided API)
   *
   * @extends UnifiedRecord
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  class ApplicationDefinedRecord extends UnifiedRecord {
    /**
     * Indicates the type of data, should always be started with 'ApplicationDefined.', will
     * return error otherwise
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the type of data, should always be started with 'ApplicationDefined.', will
     * return error otherwise
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    applicationDefinedType: string;
    /**
     * Indicates the raw data of application defined data
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the raw data of application defined data
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    rawData: Uint8Array;
  }

  /**
   * Describe the sharing channel that UDMF support
   *
   * @enum { string }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the sharing channel that UDMF support
   *
   * @enum { string }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  enum Intention {
    /**
     * Indicates the intention of data hub
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the intention of data hub
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    DATA_HUB = 'DataHub',

    /**
     * Indicates the intention of drag
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @systemapi
     * @StageModelOnly
     * @since 12
     */
    /**
     * Indicates the intention of drag
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @stagemodelonly
     * @since 14
     */
     DRAG = 'Drag',

    /**
     * Indicates a system sharing channel.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @stagemodelonly
     * @since 20
     */
    SYSTEM_SHARE = 'SystemShare',

    /**
     * Indicates a Picker channel.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @stagemodelonly
     * @since 20
     */
    PICKER = 'Picker',

    /**
     * Indicates a menu channel.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @stagemodelonly
     * @since 20
     */
    MENU = 'Menu'
  }

  /**
   * Describe the optional arguments of data operation
   *
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Describe the optional arguments of data operation
   *
   * @typedef { object }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  type Options = {
    /**
     * Indicates the target Intention
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the target Intention
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    intention?: Intention;

    /**
     * Indicates the unique identifier of target UnifiedData
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @since 10
     */
    /**
     * Indicates the unique identifier of target UnifiedData
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 11
     */
    key?: string;
  };

  /**
   * Defines the types of file conflict options when getting data from the UDMF.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  enum FileConflictOptions {
    /**
     * Overwrite when destUri has file with same name.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    OVERWRITE,

    /**
     * Skip when destUri has file with same name.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    SKIP
  }

  /**
   * Defines the types of progress indicator when getting data from the UDMF.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  enum ProgressIndicator {
    /**
     * Getting data without system default progress indicator.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    NONE,

    /**
     * Getting data with system default progress indicator.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    DEFAULT
  }

  /**
   * Defines the listener status of obtaining progress and data.
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  enum ListenerStatus {
    /**
     * Indicates the finished status.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    FINISHED = 0,

    /**
     * Indicates that processing is still in progress.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    PROCESSING,

    /**
     * Indicates that the process has been canceled.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    CANCELED,

    /**
     * Indicates that an internal error has occurred.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    INNER_ERROR = 200,

    /**
     * Indicates that the GetDataParams contains invalid parameters.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    INVALID_PARAMETERS,

    /**
     * Indicates that no data is obtained.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    DATA_NOT_FOUND,

    /**
     * Indicates that an error occurred in the synchronization process.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    SYNC_FAILED,

    /**
     * Indicates that an error occurred during file copying.
     *
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    COPY_FILE_FAILED,
  }

  /**
   * Indicates the progress information.
   *
   * @interface ProgressInfo
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  interface ProgressInfo {
    /**
     * Indicates the UDMF processing progress.
     *
     * @type { number }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    progress: number;

    /**
     * Indicates the status of obtaining progress and data.
     *
     * @type { ListenerStatus }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    status: ListenerStatus;
  }

  /**
   * A listener callback function that fetches progress info and data.
   *
   * @typedef { function } DataProgressListener
   * @param { ProgressInfo } progressInfo - the progress information.
   * @param { UnifiedData | null } data - Data obtained when the progress reaches 100%.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  type DataProgressListener = (progressInfo: ProgressInfo, data: UnifiedData | null) => void;

  /**
   * Represents the get data parameters when getting data from udmf.
   *
   * @interface GetDataParams
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 15
   */
  interface GetDataParams {
    /**
     * Indicates whether to use default system progress indicator.
     *
     * @type { ProgressIndicator }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    progressIndicator: ProgressIndicator;

    /**
     * Indicates progress and data listener when getting unified data.
     *
     * @type { DataProgressListener }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    dataProgressListener: DataProgressListener;

    /**
     * Indicates the dest path uri where copy file will be copied to sandbox of application.
     *
     * @type { string }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    destUri?: string;

    /**
     * Indicates file conflict options when dest path has file with same name.
     *
     * @type { FileConflictOptions }
     * @syscap SystemCapability.DistributedDataManager.UDMF.Core
     * @atomicservice
     * @since 15
     */
    fileConflictOptions?: FileConflictOptions;
  }

  /**
   * Insert data into unified data channel by Intention
   *
   * @param { Options } options - fill the intention field to indicate the target {@link Intention}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to insert into target intention.
   * @param { AsyncCallback<string> } callback - {string}: the unique identifier.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Insert data into unified data channel by Intention
   *
   * @param { Options } options - fill the intention field to indicate the target {@link Intention}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to insert into target intention.
   * @param { AsyncCallback<string> } callback - {string}: the unique identifier.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function insertData(options: Options, data: UnifiedData, callback: AsyncCallback<string>): void;

  /**
   * Insert data into unified data channel by Intention
   *
   * @param { Options } options - fill the intention field to indicate the target {@link Intention}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to insert into target intention.
   * @returns { Promise<string> } {string}: the unique identifier.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Insert data into unified data channel by Intention
   *
   * @param { Options } options - fill the intention field to indicate the target {@link Intention}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to insert into target intention.
   * @returns { Promise<string> } {string}: the unique identifier.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function insertData(options: Options, data: UnifiedData): Promise<string>;

  /**
   * Update data to unified data channel by Unique Identifier
   *
   * @param { Options } options - fill the unique identifier field to indicate the target {@link UnifiedData}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to update the target data.
   * @param { AsyncCallback<void> } callback - the callback of updateData.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Update data to unified data channel by Unique Identifier
   *
   * @param { Options } options - fill the unique identifier field to indicate the target {@link UnifiedData}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to update the target data.
   * @param { AsyncCallback<void> } callback - the callback of updateData.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function updateData(options: Options, data: UnifiedData, callback: AsyncCallback<void>): void;

  /**
   * Update data to unified data channel by Unique Identifier
   *
   * @param { Options } options - fill the unique identifier field to indicate the target {@link UnifiedData}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to update the target data.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Update data to unified data channel by Unique Identifier
   *
   * @param { Options } options - fill the unique identifier field to indicate the target {@link UnifiedData}.
   * @param { UnifiedData } data - {@link UnifiedData} data object to update the target data.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function updateData(options: Options, data: UnifiedData): Promise<void>;

  /**
   * Query data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @param { AsyncCallback<Array<UnifiedData>> } callback - {Array<UnifiedData>}: the target {@link UnifiedData}
   * object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Query data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @param { AsyncCallback<Array<UnifiedData>> } callback - {Array<UnifiedData>}: the target {@link UnifiedData} object
   *  array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function queryData(options: Options, callback: AsyncCallback<Array<UnifiedData>>): void;

  /**
   * Query data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @returns { Promise<Array<UnifiedData>> } {Array<UnifiedData>}: the target {@link UnifiedData} object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Query data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @returns { Promise<Array<UnifiedData>> } {Array<UnifiedData>}: the target {@link UnifiedData} object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function queryData(options: Options): Promise<Array<UnifiedData>>;

  /**
   * Delete data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @param { AsyncCallback<Array<UnifiedData>> } callback - {Array<UnifiedData>}: the deleted {@link UnifiedData}
   * object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Delete data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @param { AsyncCallback<Array<UnifiedData>> } callback - {Array<UnifiedData>}: the deleted {@link UnifiedData}
   * object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function deleteData(options: Options, callback: AsyncCallback<Array<UnifiedData>>): void;

  /**
   * Delete data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @returns { Promise<Array<UnifiedData>> } {Array<UnifiedData>}: the deleted {@link UnifiedData} object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @since 10
   */
  /**
   * Delete data of unified data channel by Intention or Unique Identifier
   *
   * @param { Options } options - fill the intention or unique identifier field to indicate the target {@link Intention}
   * or {@link UnifiedData}.
   * @returns { Promise<Array<UnifiedData>> } {Array<UnifiedData>}: the deleted {@link UnifiedData} object array.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:1.Mandatory parameters are left unspecified;
   * <br>2.Incorrect parameters types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @atomicservice
   * @since 11
   */
  function deleteData(options: Options): Promise<Array<UnifiedData>>;

  /**
   * Sets the ShareOptions for the application data. Currently, only the drag-and-drop data channel is supported.
   *
   * @param { Intention } intention - Describe the sharing channel that UDMF support.
   * @param { ShareOptions } shareOptions - Types of scope that UnifiedData can be used.
   * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application uses
   * system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                                                   2. Incorrect parameter types.
   * @throws { BusinessError } 20400001 - Settings already exist. To reconfigure, remove the existing sharing options.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @systemapi
   * @StageModelOnly
   * @since 12
   */
  /**
   * Sets the ShareOptions for the application data. Currently, only the drag-and-drop data channel is supported.
   *
   * @permission ohos.permission.MANAGE_UDMF_APP_SHARE_OPTION
   * @param { Intention } intention - Describe the sharing channel that UDMF support. Currently only supports DRAG
   * intention.
   * @param { ShareOptions } shareOptions - Types of scope that UnifiedData can be used.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission
   * required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                                                   2. Incorrect parameter types;
   *                                                                   3. Parameter verification failed.
   * @throws { BusinessError } 20400001 - Settings already exist. To reconfigure, remove the existing sharing options.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @stagemodelonly
   * @since 14
   */
   function setAppShareOptions(intention: Intention, shareOptions: ShareOptions): void;

   /**
    * Remove app sharing options.
    *
    * @param { Intention } intention - Describe the sharing channel that UDMF support.
    * @throws { BusinessError } 202 - Permission verification failed, application which is not a system application
    * uses system API.
    * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
    *                                                                   2. Incorrect parameter types.
    * @syscap SystemCapability.DistributedDataManager.UDMF.Core
    * @systemapi
    * @StageModelOnly
    * @since 12
    */
  /**
   * Remove app sharing options.
   *
   * @permission ohos.permission.MANAGE_UDMF_APP_SHARE_OPTION
   * @param { Intention } intention - Describe the sharing channel that UDMF support. Currently only supports DRAG
   * intention.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission
   * required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   *                                                                   2. Incorrect parameter types;
   *                                                                   3. Parameter verification failed.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @stagemodelonly
   * @since 14
   */
   function removeAppShareOptions(intention: Intention): void;

  /**
   * Converts the provided data into a multi-style data structure, which is useful when the original data uses multiple
   * records to represent different styles of the same data. This API is used only when the following rules are met:
   * 1.The number of records in data is greater than
   * 2.The value of unifiedData.properties.tag is records_to_entries_data_format.
   *
   * @param { UnifiedData } data - {@link UnifiedData} Data to convert.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.DistributedDataManager.UDMF.Core
   * @stagemodelonly
   * @atomicservice
   * @since 17
   */
   function convertRecordsToEntries(data: UnifiedData): void;
}

export default unifiedDataChannel;
