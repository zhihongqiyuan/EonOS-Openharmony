/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import { ValueType } from './@ohos.data.ValuesBucket';

/**
 * This module provides data share services based on the ExtensionAbility.
 *
 * @namespace dataSharePredicates
 * @syscap SystemCapability.DistributedDataManager.DataShare.Core
 * @StageModelOnly
 * @since 10
 */
/**
 * This module provides data share services based on the ExtensionAbility.
 *
 * @namespace dataSharePredicates
 * @syscap SystemCapability.DistributedDataManager.DataShare.Core
 * @StageModelOnly
 * @crossplatform
 * @since 12
 */
/**
 * This module provides data share services based on the ExtensionAbility.
 *
 * @namespace dataSharePredicates
 * @syscap SystemCapability.DistributedDataManager.DataShare.Core
 * @StageModelOnly
 * @crossplatform
 * @atomicservice
 * @since 20
 */
declare namespace dataSharePredicates {
  /**
   * Provides a filter object to query data in a database by using DataShare APIs.
   * <strong>This type is not multi-thread safe. If a DataSharePredicates instance is operated by multiple threads
   * at the same time in an application, use a lock for it.</strong>
   *
   * @syscap SystemCapability.DistributedDataManager.DataShare.Core
   * @StageModelOnly
   * @since 10
   */
  /**
   * Provides a filter object to query data in a database by using DataShare APIs.
   * <strong>This type is not multi-thread safe. If a DataSharePredicates instance is operated by multiple threads
   * at the same time in an application, use a lock for it.</strong>
   *
   * @syscap SystemCapability.DistributedDataManager.DataShare.Core
   * @StageModelOnly
   * @crossplatform
   * @since 12
   */
  /**
   * Manages relational database configurations.
   *
   * @syscap SystemCapability.DistributedDataManager.DataShare.Core
   * @StageModelOnly
   * @crossplatform
   * @atomicservice
   * @since 20
   */
  class DataSharePredicates {
    /**
     * Configure the DataSharePredicates to match the field whose data type is ValueType and value is equal
     * to a specified value.
     * This method is similar to = of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { ValueType } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Configure the DataSharePredicates to match the field whose data type is ValueType and value is equal
     * to a specified value.
     * This method is similar to = of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { ValueType } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Configure the DataSharePredicates to match the field whose data type is ValueType and value is equal
     * to a specified value.
     * This method is similar to = of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { ValueType } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    equalTo(field: string, value: ValueType): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the field whose data type is ValueType and value is unequal to
     * a specified value.
     * This method is similar to != of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { ValueType } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    notEqualTo(field: string, value: ValueType): DataSharePredicates;

    /**
     * Adds a left parenthesis to the DataSharePredicates.
     * This method is similar to ( of the SQL statement and needs to be used together with endWrap().
     * Currently only used for RDB.
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the left parenthesis.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    beginWrap(): DataSharePredicates;

    /**
     * Adds a right parenthesis to the DataSharePredicates.
     * This method is similar to ) of the SQL statement and needs to be used together
     * with beginWrap().
     * Currently only used for RDB.
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the right parenthesis.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    endWrap(): DataSharePredicates;

    /**
     * Adds an or condition to the DataSharePredicates.
     * This method is similar to or of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the or condition.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    or(): DataSharePredicates;

    /**
     * Adds an and condition to the DataSharePredicates.
     * This method is similar to and of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the and condition.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Adds an and condition to the DataSharePredicates.
     * This method is similar to and of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the and condition.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Adds an and condition to the DataSharePredicates.
     * This method is similar to and of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the and condition.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    and(): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the field whose data type is string and value
     * contains a specified value.
     * This method is similar to contains of the SQL statement.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    contains(field: string, value: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the field whose data type is string and value starts
     * with a specified string.
     * This method is similar to value% of the SQL statement.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    beginsWith(field: string, value: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the field whose data type is string and value
     * ends with a specified string.
     * This method is similar to %value of the SQL statement.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with the DataSharePredicates.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    endsWith(field: string, value: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the fields whose value is null.
     * This method is similar to is null of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    isNull(field: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the specified fields whose value is not null.
     * This method is similar to is not null of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    isNotNull(field: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the fields whose data type is string and value is
     * similar to a specified string.
     * This method is similar to like of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with the DataSharePredicates. The percent sign (%) in the value
     * is a wildcard (like * in a regular expression).
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    like(field: string, value: string): DataSharePredicates;

    /**
     * Configure the DataSharePredicates to match the fields whose data type is string and value is
     * not similar to a specified string.
     * This method is similar to unlike of the SQL statement.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with the DataSharePredicates. The percent sign (%) in the value
     * is a wildcard (like * in a regular expression).
     * @returns { DataSharePredicates } Returns the DataSharePredicates that match the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    unlike(field: string, value: string): DataSharePredicates;

    /**
     * Configure DataSharePredicates to match the specified field whose data type is string and the value contains
     * a wildcard.
     * Different from like, the input parameters of this method are case-sensitive.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { string } value - Indicates the value to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns the SQL statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    glob(field: string, value: string): DataSharePredicates;

    /**
     * Restricts the value of the field to the range between low value and high value.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name.
     * @param { ValueType } low - Indicates the minimum value.
     * @param { ValueType } high - Indicates the maximum value.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    between(field: string, low: ValueType, high: ValueType): DataSharePredicates;

    /**
     * Configure DataSharePredicates to match the specified field whose data type is int and value is
     * out of a given range.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { ValueType } low - Indicates the minimum value to match with DataSharePredicates.
     * @param { ValueType } high - Indicates the maximum value to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    notBetween(field: string, low: ValueType, high: ValueType): DataSharePredicates;

    /**
     * Restricts the value of the field to be greater than the specified value.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name.
     * @param { ValueType } value - Indicates the String field.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    greaterThan(field: string, value: ValueType): DataSharePredicates;

    /**
     * Restricts the value of the field to be smaller than the specified value.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name.
     * @param { ValueType } value - Indicates the String field.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    lessThan(field: string, value: ValueType): DataSharePredicates;

    /**
     * Restricts the value of the field to be greater than or equal to the specified value.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name.
     * @param { ValueType } value - Indicates the String field.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    greaterThanOrEqualTo(field: string, value: ValueType): DataSharePredicates;

    /**
     * Restricts the value of the field to be smaller than or equal to the specified value.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name.
     * @param { ValueType } value - Indicates the String field.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    lessThanOrEqualTo(field: string, value: ValueType): DataSharePredicates;

    /**
     * Restricts the ascending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Restricts the ascending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Restricts the ascending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    orderByAsc(field: string): DataSharePredicates;

    /**
     * Restricts the descending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Restricts the descending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Restricts the descending order of the return list. When there are several orders,
     * the one close to the head has the highest priority.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name for sorting the return list.
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    orderByDesc(field: string): DataSharePredicates;

    /**
     * Restricts each row of the query result to be unique.
     * Currently only used for RDB.
     *
     * @returns { DataSharePredicates } Returns the SQL query statement with the specified DataSharePredicates.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    distinct(): DataSharePredicates;

    /**
     * Construct a query object to specify the number of results and the starting position.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { number } total - Represents the specified number of results.
     * @param { number } offset - Indicates the starting position.
     * @returns { DataSharePredicates } Returns the query object.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Construct a query object to specify the number of results and the starting position.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { number } total - Represents the specified number of results.
     * @param { number } offset - Indicates the starting position.
     * @returns { DataSharePredicates } Returns the query object.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Construct a query object to specify the number of results and the starting position.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { number } total - Represents the specified number of results.
     * @param { number } offset - Indicates the starting position.
     * @returns { DataSharePredicates } Returns the query object.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    limit(total: number, offset: number): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} to group query results by specified columns.
     * Currently only used for RDB.
     *
     * @param { Array<string> } fields - Indicates the specified columns by which query results are grouped.
     * @returns { DataSharePredicates } Returns the DataSharePredicates with the specified columns by which query results are grouped.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    groupBy(fields: Array<string>): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} to specify the index column.
     * Before using this method, you need to create an index column.
     * Currently only used for RDB.
     *
     * @param { string } field - Indicates the name of the index column.
     * @returns { DataSharePredicates } Returns DataSharePredicates with the specified index column.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    indexedBy(field: string): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} to match the specified field whose data type is ValueType array and values
     * are within a given range.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { Array<ValueType> } value - Indicates the values to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns DataSharePredicates that matches the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @since 10
     */
    /**
     * Configure {@code DataSharePredicates} to match the specified field whose data type is ValueType array and values
     * are within a given range.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { Array<ValueType> } value - Indicates the values to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns DataSharePredicates that matches the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @since 12
     */
    /**
     * Configure {@code DataSharePredicates} to match the specified field whose data type is ValueType array and values
     * are within a given range.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { Array<ValueType> } value - Indicates the values to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns DataSharePredicates that matches the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @StageModelOnly
     * @crossplatform
     * @atomicservice
     * @since 20
     */
    in(field: string, value: Array<ValueType>): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} to match the specified field whose data type is String array and values
     * are out of a given range.
     * Currently only used for RDB and KVDB(schema).
     *
     * @param { string } field - Indicates the column name in the database table.
     * @param { Array<ValueType> } value - Indicates the values to match with DataSharePredicates.
     * @returns { DataSharePredicates } Returns DataSharePredicates that matches the specified field.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    notIn(field: string, value: Array<ValueType>): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} Creates a query condition using the specified key prefix.
     * Currently only used for KVDB.
     *
     * @param { string } prefix - Represents the specified key prefix.
     * @returns { DataSharePredicates } Returns the query object.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    prefixKey(prefix: string): DataSharePredicates;

    /**
     * Configure {@code DataSharePredicates} to match the specified value whose key is within a given range.
     * Currently only used for KVDB.
     *
     * @param { Array<string> } keys - Represents the key names.
     * @returns { DataSharePredicates } Returns the query object.
     * @syscap SystemCapability.DistributedDataManager.DataShare.Core
     * @systemapi
     * @StageModelOnly
     * @since 9
     */
    inKeys(keys: Array<string>): DataSharePredicates;
  }
}

export default dataSharePredicates;
