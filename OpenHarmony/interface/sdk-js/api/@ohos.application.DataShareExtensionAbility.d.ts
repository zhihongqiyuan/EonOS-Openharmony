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
 * @kit ArkData
 */

import { AsyncCallback } from './@ohos.base';
import ExtensionContext from './application/ExtensionContext';
import Want from './@ohos.app.ability.Want';
import dataSharePredicates from './@ohos.data.dataSharePredicates';
import { ValuesBucket } from './@ohos.data.ValuesBucket';
import dataShare from './@ohos.data.dataShare';

/**
   * Struct for a batch update operation.
   *
   * @typedef { dataShare.UpdateOperation }
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
type UpdateOperation = dataShare.UpdateOperation;

/**
 * This module provides data sharing and expansion capabilities.
 *
 * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
 * @systemapi
 * @StageModelOnly
 * @since 9
 */
export default class DataShareExtensionAbility {
  /**
   * Indicates datashare extension ability context.
   *
   * @type { ExtensionContext }
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 10
   */
  context: ExtensionContext;

  /**
   * Called back when a datashare extension ability is started for initialization.
   *
   * @param { Want } want - Indicates connection information about the datashare extension ability.
   * @param { AsyncCallback<void> } callback - callback function, no return value.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onCreate?(want: Want, callback: AsyncCallback<void>): void;

  /**
   * Inserts a data record into the database. This method should be implemented by a data share.
   *
   * @param { string } uri - Indicates the position where the data is to insert.
   * @param { ValuesBucket } valueBucket - Indicates the data to insert.
   * @param { AsyncCallback<number> } callback - Returns the index of the newly inserted data record.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  insert?(uri: string, valueBucket: ValuesBucket, callback: AsyncCallback<number>): void;

  /**
   * Updates one or more data records in the database. This method should be implemented by a data share.
   *
   * @param { string } uri - Indicates the database table storing the data to update.
   * @param { dataSharePredicates.DataSharePredicates } predicates - Indicates filter criteria. If this parameter is
   *                                                               null, all data records will be updated by default.
   * @param { ValuesBucket } valueBucket - Indicates the data to update. This parameter can be null.
   * @param { AsyncCallback<number> } callback - Returns the number of data records updated.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  update?(
    uri: string,
    predicates: dataSharePredicates.DataSharePredicates,
    valueBucket: ValuesBucket,
    callback: AsyncCallback<number>
  ): void;

  /**
   * Updates data records in the database.
   *
   * @param { Record<string, Array<UpdateOperation>> } operations - Indicates the data to update.
   * @param { AsyncCallback<Record<string, Array<number>>> } callback - Callback used to return the result.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 12
   */
  batchUpdate?(
    operations: Record<string, Array<UpdateOperation>>,
    callback: AsyncCallback<Record<string, Array<number>>>
  ): void;

  /**
   * Deletes one or more data records. This method should be implemented by a data share.
   *
   * @param { string } uri - Indicates the database table storing the data to delete.
   * @param { dataSharePredicates.DataSharePredicates } predicates - Indicates filter criteria. If this parameter is
   *                                                                 null, all data records will be deleted by default.
   * @param { AsyncCallback<number> } callback - Returns the number of data records deleted.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  delete?(uri: string, predicates: dataSharePredicates.DataSharePredicates, callback: AsyncCallback<number>): void;

  /**
   * Queries one or more data records in the database. This method should be implemented by a data share.
   * Only RDB and distributed KVDB resultsets are supported. The current version does not support custom resultsets.
   *
   * @param { string } uri - Indicates the database table storing the data to query.
   * @param { dataSharePredicates.DataSharePredicates } predicates - Indicates filter criteria. If this parameter is
   *                                                                 null, all data records will be queried by default.
   * @param { Array<string> } columns - Indicates the columns to be queried, in array, for example, {"name","age"}.
   *                                    You should define the processing logic when this parameter is null.
   * @param { AsyncCallback<Object> } callback - Returns the queried data, only support result set of rdb or kvstore.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  query?(
    uri: string,
    predicates: dataSharePredicates.DataSharePredicates,
    columns: Array<string>,
    callback: AsyncCallback<Object>
  ): void;

  /**
   * Inserts multiple data records into the database. This method should be implemented by a data share.
   *
   * @param { string } uri - Indicates the position where the data is to insert.
   * @param { Array<ValuesBucket> } valueBuckets - Indicates the data to insert.
   * @param { AsyncCallback<number> } callback - Returns the number of data records inserted.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  batchInsert?(uri: string, valueBuckets: Array<ValuesBucket>, callback: AsyncCallback<number>): void;

  /**
   * Converts the given {@code uri} that refer to the data share into a normalized URI. A normalized URI can be
   * used across devices, persisted, backed up, and restored. It can refer to the same item in the data share
   * even if the context has changed.
   *
   * @param { string } uri - Indicates the uri to normalize.
   * @param { AsyncCallback<string> } callback - Returns the normalized uri if the data share supports URI normalization;
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  normalizeUri?(uri: string, callback: AsyncCallback<string>): void;

  /**
   * Converts the given normalized {@code uri} generated by {@link #normalizeUri(uri)} into a denormalized one.
   * The default implementation of this method returns the original uri passed to it.
   *
   * @param { string } uri - Indicates the uri to denormalize.
   * @param { AsyncCallback<string> } callback - Returns the denormalized {@code uri} object if the denormalization is
   *                                             successful; returns the original {@code uri} passed to this method if
   *                                             there is nothing to do; returns {@code null} if the data identified by
   *                                             the original {@code uri} cannot be found in the current environment.
   * @syscap SystemCapability.DistributedDataManager.DataShare.Provider
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  denormalizeUri?(uri: string, callback: AsyncCallback<string>): void;
}
