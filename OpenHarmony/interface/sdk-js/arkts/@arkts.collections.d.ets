/*
 * Copyright (c) 2024-2025 Huawei Device Co., Ltd.
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
 * @file Defines the collections for ArkTS
 * @kit ArkTS
 */

import lang from './@arkts.lang'

/**
 * ArkTS collections.
 *
 * @namespace collections
 * @syscap SystemCapability.Utils.Lang
 * @atomicservice
 * @since 12
 */
/**
 * ArkTS collections.
 *
 * @namespace collections
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @atomicservice
 * @since 18
 */
declare namespace collections {
  /**
   * Callback function used in the typed Array's 'from' function.
   * 
   * @typedef { function } TypedArrayFromMapFn
   * @param { FromElementType } value - The value in the original array.
   * @param { number } index - The index in the original array.
   * @returns { ToElementType } The transformed value.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the mapping function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayFromMapFn
   * @param { FromElementType } value - Element that is currently traversed and used to construct an ArkTS typed array.
   * @param { number } index - Index of the element.
   * @returns { ToElementType } The transformed value.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayFromMapFn<FromElementType, ToElementType> = (value: FromElementType, index: number) => ToElementType;
  /**
   * Callback function used in typed Array functions which needs to determine
   * whether some element satisfies the specified predicate test
   * 
   * @typedef { function } TypedArrayPredicateFn
   * @param { ElementType } value - The value of the element.
   * @param { number } index - The index of the element.
   * @param { ArrayType } array - The array that the element belongs to.
   * @returns { boolean } True if the value meets the predicate, otherwise false.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the assertion function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayPredicateFn
   * @param { ElementType } value - Element that is being traversed in the ArkTS typed array.
   * @param { number } index - Index of the element.
   * @param { ArrayType } array - ArkTS typed array that is being traversed.
   * @returns { boolean } True if the value meets the predicate, otherwise false.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayPredicateFn<ElementType, ArrayType> =
    (value: ElementType, index: number, array: ArrayType) => boolean;
  /**
   * Callback function used in typed Array functions that perform specific action for each element.
   * 
   * @typedef { function } TypedArrayForEachCallback
   * @param { ElementType } value - The value of the element.
   * @param { number } index - The index of the element.
   * @param { ArrayType } array - The array that the element belongs to.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the traversal function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayForEachCallback
   * @param { ElementType } value - Element that is being traversed in the ArkTS typed array.
   * @param { number } index - Index of the element.
   * @param { ArrayType } array - ArkTS typed array that is being traversed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayForEachCallback<ElementType, ArrayType> =
    (value: ElementType, index: number, array: ArrayType) => void;
  /**
   * Callback function used in typed Array functions that perform specific action for each element and
   *    produce corresponding new element.
   * 
   * @typedef { function } TypedArrayMapCallback
   * @param { ElementType } value - The value of the element.
   * @param { number } index - The index of the element.
   * @param { ArrayType } array - The array that the element belongs to.
   * @returns { ElementType } The result of the mapping.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the conversion mapping function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayMapCallback
   * @param { ElementType } value - Element that is being mapped in the ArkTS typed array.
   * @param { number } index - Index of the element.
   * @param { ArrayType } array - ArkTS typed array that is being mapped.
   * @returns { ElementType } The result of the mapping.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayMapCallback<ElementType, ArrayType> =
    (value: ElementType, index: number, array: ArrayType) => ElementType;
  /**
   * Callback function used in typed Array functions that require a reduction.
   * 
   * @typedef { function } TypedArrayReduceCallback
   * @param { AccType } previousValue - The accumulator value.
   * @param { ElementType } currentValue - The current element being processed in the array.
   * @param { number } currentIndex - The index of the current element being processed in the array.
   * @param { ArrayType } array - The array that the element belongs to.
   * @returns { AccType } The result of the reduction.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the reduce function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayReduceCallback
   * @param { AccType } previousValue - Accumulated value of the current traversal.
   * @param { ElementType } currentValue - Element that is being traversed in the ArkTS typed array.
   * @param { number } currentIndex - Index of the element.
   * @param { ArrayType } array - ArkTS typed array that is being traversed.
   * @returns { AccType } The result of the reduction.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayReduceCallback<AccType, ElementType, ArrayType> =
    (previousValue: AccType, currentValue: ElementType, currentIndex: number, array: ArrayType) => AccType;
  /**
   * Callback function used in the typed Array's 'sort' function.
   * 
   * @typedef { function } TypedArrayCompareFn
   * @param { ElementType } first - The first element of the comparison.
   * @param { ElementType } second - The second element of the comparison.
   * @returns { number } The result of the comparison.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Describes the sort function of the ArkTS typed array.
   * 
   * @typedef { function } TypedArrayCompareFn
   * @param { ElementType } first - First element to be compared.
   * @param { ElementType } second - Second element to be compared.
   * @returns { number } The result of the comparison.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type TypedArrayCompareFn<ElementType> = (first: ElementType, second: ElementType) => number;
  /**
   * Defines the ArkTS Array reduction function, which is used by the 'from' API of the Array class.
   *
   * @typedef { function } ArrayFromMapFn
   * @param { FromElementType } value - Element that is being processed.
   * @param { number } index - Index of the element in the ArkTS array.
   * @returns { ToElementType } The transformed value.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 18
   */
  type ArrayFromMapFn<FromElementType, ToElementType> = (value: FromElementType, index: number) => ToElementType;

  /**
   * Defines the ArkTS Array reduction function, which is used by the 'some' and 'every'
   * APIs of the Array class to determine whether array elements meet certain test conditions.
   *
   * @typedef { function } ArrayPredicateFn
   * @param { ElementType } value - Element that is being processed.
   * @param { number } index - Index of the element in the ArkTS array.
   * @param { ArrayType } array - ArkTS array that is being traversed.
   * @returns { boolean } True if the value meets the predicate, otherwise false.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 18
   */
  type ArrayPredicateFn<ElementType, ArrayType> =
    (value: ElementType, index: number, array: ArrayType) => boolean;

  /**
   * Defines the ArkTS Array reduction function, which is used by the 'reduceRight' API of the Array class.
   *
   * @typedef { function } ArrayReduceCallback
   * @param { AccType } previousValue - Accumulated value of the current traversal.
   * @param { ElementType } currentValue - Element that is being traversed in the ArkTS array.
   * @param { number } currentIndex - Index of the element in the ArkTS array.
   * @param { ArrayType } array - ArkTS array that is being traversed.
   * @returns { AccType } The result of the reduction.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 18
   */
  type ArrayReduceCallback<AccType, ElementType, ArrayType> =
    (previousValue: AccType, currentValue: ElementType, currentIndex: number, array: ArrayType) => AccType;
  /**
   * Redefines ISendable for convenience.
   *
   * @typedef { lang.ISendable } ISendable
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * 'ISendable' is the parent type of all sendable types except null and undefined.
   * It does not have any necessary methods or properties.
   *
   * @typedef { lang.ISendable } ISendable
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type ISendable = lang.ISendable;
  /**
   * Represents an array-like object that can be concatenated.
   * 
   * @interface ConcatArray
   * @extends ISendable
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * An array-like object that can be concatenated. This API extends 'ISendable'.
   * 
   * @interface ConcatArray
   * @extends ISendable
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  interface ConcatArray<T> extends ISendable {
    /**
     * Gets the length of the ArkTS ConcatArray. This is a number one higher than the highest index in the array.
     * 
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets the length of the ArkTS ConcatArray. This is a number one higher than the highest index in the array.
     * 
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * Returns the item at that index.
     * 
     * @param { number } index - The zero-based index of the desired code unit.
     *     Throws error if index < 0 or index >= array.length.
     * @returns { T } The element in the ConcatArray matching the given index.
     * @readonly
     * @throws { BusinessError } 401 - Parameter error. Illegal index.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @syscap SystemCapability.Utils.Lang
     * @since 12
     */
    /**
     * Returns the element at a given index in this ConcatArray.
     * 
     * @param { number } index - Index of the element. The index starts from zero.
     *     If the passed-in index is less than 0 or greater than or equal to the value of 'length', an error is thrown.
     * @returns { T } The element in the ConcatArray matching the given index.
     * @readonly
     * @throws { BusinessError } 401 - Parameter error. Illegal index.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 18
     */
    readonly [index: number]: T;
    /**
     * Adds all the elements of an ArkTS ConcatArray into a string, separated by the specified separator string.
     * 
     * @param { string } [separator] - A string used to separate one element of the array from
     *     the next in the resulting string. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all array elements joined.
     *     If ConcatArray.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error. Invalid separator.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Concatenates all elements in this array into a string, with a given separator.
     * 
     * @param { string } [separator] - Separator to be used. If no value is passed in,
     *     a comma (,) is used as the separator.
     * @returns { string } A string with all array elements joined.
     *     If ConcatArray.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error. Invalid separator.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Returns a copy of a section of an ArkTS ConcatArray.
     *
     * @param { number } [start] - The beginning index of the specified portion of the array.
     *     If start is undefined, then the slice begins at index 0.
     * @param { number } [end] - The end index of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     *     If end is undefined, then the slice extends to the end of the array.
     * @returns { ConcatArray<T> } A new ConcatArray containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error. Invalid `start` or `end` parameters.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Selects a range of elements in this array to create an array.
     *
     * @param { number } [start] - Start index of the range. If a negative number is passed in,
     *     it refers to the index of 'start + array.length' The default value is '0'.
     * @param { number } [end] - End index of the range (exclusive). If a negative number is passed in,
     *     it refers to the index of 'end + array.length'. The default value is the length of the ArkTS array.
     * @returns { ConcatArray<T> } A new ConcatArray containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error. Invalid `start` or `end` parameters.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): ConcatArray<T>;
  }
  /**
   * Array is a data structure that stores a collection of elements. 
   * If multiple threads access a Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   * 
   * @implements ConcatArray<T>
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A linear data structure that is implemented on arrays and can be passed between ArkTS concurrent instances.
   * Pass-by-reference is recommended for better transfer performance.
   * This section uses the following to identify the use of generics:
   * T: type, which can be any of the sendable data types.
   * 
   * @implements ConcatArray<T>
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Array<T> implements ConcatArray<T> {
    /**
     * Gets the length of the ArkTS array. This is a number one higher than the highest index in the ArkTS array.
     * 
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets the length of the ArkTS array. This is a number one higher than the highest index in the ArkTS array.
     * 
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * Creates an ArkTS Array with arrayLength elements initialized to initialValue.
     *
     * @param { number } arrayLength - The length of the array.
     * @param { T } initialValue - Element initial value that will be filled into the Array.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The create method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an ArkTS array of a fixed length, with each element set to a given initial value.
     *
     * @param { number } arrayLength - Length of the ArkTS array.
     * @param { T } initialValue - Initial value of each element in the ArkTS array.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The create method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static create<T>(arrayLength: number, initialValue: T): Array<T>;
    /**
     * Creates an ArkTS Array from an array-like object.
     * 
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an ArkTS Array.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an ArkTS array from an array-like object.
     * 
     * @param { ArrayLike<T> } arrayLike - Array-like object.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>): Array<T>;
    /**
     * Creates an ArkTS Array from an iterable object.
     * 
     * @param { Iterable<T> } iterable - An iterable object to convert to an ArkTS Array.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an ArkTS array from an iterable object.
     * 
     * @param { Iterable<T> } iterable - Array-like object.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error: Possible causes: 1. Mandatory parameters are left unspecified;
     *     2. Incorrect parameter types; 3. Parameter verification failed.
     * @throws { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(iterable: Iterable<T>): Array<T>;
    /**
     * Creates an ArkTS array from an array-like object, and uses a custom function to process each array element.
     *
     * @param { ArrayLike<T> | Iterable<T> } arrayLike - Array-like object.
     * @param { ArrayFromMapFn<T, T> } mapFn - Functions used to process the array elements.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error: Possible causes: 1. Mandatory parameters are left unspecified;
     *     2. Incorrect parameter types; 3. Parameter verification failed.
     * @throws { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T> | Iterable<T>, mapFn: ArrayFromMapFn<T, T>): Array<T>;
    /**
     * Creates an ArkTS array from an array-like object, and uses a custom function to process each array element.
     * The type of the elements in the array-like object can be different from that of the array elements.
     *
     * @param  { ArrayLike<U> | Iterable<U> } arrayLike - Array-like object.
     * @param  { ArrayFromMapFn<U, T> } mapFn - Functions used to process the array elements.
     * @returns { Array<T> } A new Array instance
     * @throws { BusinessError } 401 - Parameter error: Possible causes: 1. Mandatory parameters are left unspecified;
     *     2. Incorrect parameter types; 3. Parameter verification failed.
     * @throws  { BusinessError } 10200011 - The from method cannot be bound.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static from<U, T>(arrayLike: ArrayLike<U> | Iterable<U>, mapFn: ArrayFromMapFn<U, T>): Array<T>;
    /**
     * A constructor used to create an ArkTS Array.
     *
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an empty ArkTS array.
     *
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an ArkTS Array.
     *
     * @param { T } first - First element when initializing an ArkTS Array.
     * @param { T[] } left - Left elements when initializing an ArkTS Array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an ArkTS array with the given elements.
     *
     * @param { T } first - First element to be included in the ArkTS array.
     * @param { T[] } left - Remaining elements to be included in the ArkTS array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(first: T, ...left: T[]);
    /**
     * A constructor used to create an ArkTS Array.
     *
     * @param { T[] } items - Elements when initializing an ArkTS Array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an ArkTS array with the given elements.
     *
     * @param { T[] } items - Elements to be included in the ArkTS array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(...items: T[]);
    /**
     * Removes the last element from an ArkTS array and returns it.
     * If the array is empty, undefined is returned and the array is not modified.
     * 
     * @returns { T | undefined } - The removed element from the array; undefined if the array is empty.
     * @throws { BusinessError } 10200011 - The pop method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Removes the last element from this ArkTS array and returns that element.
     * If the array is empty, undefined is returned and the array does not change.
     * 
     * @returns { T | undefined } - The removed element from the array; undefined if the array is empty.
     * @throws { BusinessError } 10200011 - The pop method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    pop(): T | undefined;
    /**
     * Appends new elements to the end of an ArkTS Array, and returns the new length of the array.
     * 
     * @param { T[] } items - New elements to add to the ArkTS array.
     * @returns { number } - The new length property of the object upon which the method was called.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The push method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds one or more elements to the end of this ArkTS array and returns the new length of the array.
     * 
     * @param { T[] } items - Elements to add.
     * @returns { number } - The new length property of the object upon which the method was called.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The push method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    push(...items: T[]): number;
    /**
     * Adds all the elements of an ArkTS Array into a string, separated by the specified separator string.
     * 
     * @param { string } [separator] - A string used to separate one element of the array from
     *     the next in the resulting string. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all array elements joined. If Array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Concatenates all elements in this ArkTS array into a string, with a given separator.
     * 
     * @param { string } [separator] - Separator to be used. If no value is passed in,
     *     a comma (,) is used as the separator.
     * @returns { string } A string with all array elements joined. If Array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Removes the first element from an ArkTS Array and returns it.
     * If the array is empty, undefined is returned and the array is not modified.
     * 
     * @returns { T | undefined } The removed element from the array; undefined if the array is empty
     * @throws { BusinessError } 10200011 - The shift method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Removes the first element from this ArkTS array and returns that element.
     * If the array is empty, undefined is returned and the array does not change.
     * 
     * @returns { T | undefined } The removed element from the array; undefined if the array is empty
     * @throws { BusinessError } 10200011 - The shift method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shift(): T | undefined;
    /**
     * Inserts new elements at the start of an array, and returns the new length of the array.
     *
     * @param { T[] } items - Elements to insert at the start of the array.
     * @returns { number } The new length property of the object upon which the method was called.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The unshift method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds one or more elements to the beginning of this ArkTS array and returns the new length of the array.
     *
     * @param { T[] } items - Elements to add.
     * @returns { number } The new length property of the object upon which the method was called.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The unshift method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    unshift(...items: T[]): number;
    /**
     * Returns a copy of a section of an ArkTS Array.
     * For both start and end, a negative index can be used to indicate an offset from the end of the array.
     * For example, -2 refers to the second to last element of the array.
     *
     * @param { number } [start] - The beginning index of the specified portion of the array.
     *     If start is undefined, then the slice begins at index 0.
     * @param { number } [end] - The end index of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     *     If end is undefined, then the slice extends to the end of the array.
     * @returns { Array<T> } A new array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a copy of a section of an ArkTS Array.
     * For both start and end, a negative index can be used to indicate an offset from the end of the array.
     * For example, -2 refers to the second to last element of the array.
     *
     * @param { number } [start] - Start index of the range. If a negative number is passed in,
     *     it refers to the index of start + array.length. The default value is 0.
     * @param { number } [end] - End index of the range (exclusive). If a negative number is passed in,
     *     it refers to the index of end + array.length. The default value is the length of the ArkTS array.
     * @returns { Array<T> } A new array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Array<T>;
    /**
     * Sorts an array in place. This method mutates the array and returns a reference to the same array.
     *
     * @param { function } [compareFn] - Function used to determine the order of the elements. It is expected to return
     *     a negative value if the first argument is less than the second argument, zero if they're equal,
     *     and a positive value otherwise. If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Array<T> } The reference to the original array, now sorted.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts elements in this ArkTS array and returns a new array.
     *
     * @param { function } [compareFn] - Function used to determine the order of the elements. It is expected to return
     *     a negative value if the first argument is less than the second argument, zero if they're equal,
     *     and a positive value otherwise. If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Array<T> } The reference to the original array, now sorted.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: (a: T, b: T) => number): Array<T>;
    /**
     * Returns the index of the first occurrence of a value in an ArkTS Array, or -1 if it is not present.
     * 
     * @param { T } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search.
     *     If fromIndex is omitted, the search starts at index 0.
     * @returns { number } The first index of searchElement in the array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in this ArkTS Array.
     * If the value is not found, -1 is returned.
     * 
     * @param { T } searchElement - Value to search for.
     * @param { number } [fromIndex] - Index from which the search starts. The default value is 0.
     * @returns { number } The first index of searchElement in the array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: T, fromIndex?: number): number;
    /**
     * Executes a provided function once for each value in the Array object.
     *
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The function to be called for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a callback function for each element in this ArkTS Array.
     *
     * @param { function } callbackFn - Callback function to run for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: (value: T, index: number, array: Array<T>) => void): void;
    /**
     * Calls a defined callback function on each element of an ArkTS Array,
     * and returns an array that contains the results.
     * 
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The map method calls the callbackFn function one time for each element in the array.
     * @returns { Array<U> } A new array with each element being the result of the callback function.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a callback function for each element in this ArkTS Array and returns a new array that 
     * contains the result of the callback function.
     * 
     * @param { function } callbackFn - Callback function to run for each element.
     * @returns { Array<U> } A new array with each element being the result of the callback function.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map<U>(callbackFn: (value: T, index: number, array: Array<T>) => U): Array<U>;
    /**
     * Returns the elements of an ArkTS Array that meet the condition specified in a callback function.
     * 
     * @param { function } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Array<T> } A shallow copy of the given containing just the elements that pass the test.
     *     If no elements pass the test, an empty array is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a new array containing all elements that pass a test provided by a callback function.
     * 
     * @param { function } predicate - Function that takes three arguments. It is used to filter elements.
     *     The value true means that the current element passes the test and should be retained in the new array. 
     *     The value false means that the current element fails the test and should be excluded from the new array.
     * @returns { Array<T> } A shallow copy of the given containing just the elements that pass the test.
     *     If no elements pass the test, an empty array is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: (value: T, index: number, array: Array<T>) => boolean): Array<T>;
    /**
     * Calls the specified callback function for all the elements in an ArkTS Array.
     * The return value of the callback function is the accumulated result,
     * and is provided as an argument in the next call to the callback function.
     * 
     * @param { function } callbackFn - A function that accepts up to four arguments.
     *     The reduce method calls the callbackFn function one time for each element in the array.
     * @returns { T } The value that results from running the "reducer" callback function to
     *     completion over the entire array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an ArkTS Array.
     * The return value of the callback function is the accumulated result,
     * and is provided as an argument in the next call to the callback function.
     * 
     * @param { function } callbackFn - Function that takes four arguments.
     *     It performs an operation on each element and passes the result as an accumulated value to the next element.
     * @returns { T } The value that results from running the "reducer" callback function to
     *     completion over the entire array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: (previousValue: T, currentValue: T, currentIndex: number, array: Array<T>) => T): T;
    /**
     * Calls the specified callback function for all the elements in an array.
     * The return value of the callback function is the accumulated result,
     * and is provided as an argument in the next call to the callback function.
     * 
     * @param { function } callbackFn - A function that accepts up to four arguments.
     *     The reduce method calls the callbackFn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified,
     *     it is used as the initial value to start the accumulation.
     *     The first call to the callbackFn function provides this value as an argument instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
    */
    /**
     * Calls the specified callback function for all the elements in an array.
     * The return value of the callback function is the accumulated result,
     * and is provided as an argument in the next call to the callback function.
     * 
     * @param { function } callbackFn - Function that takes four arguments.
     *     It performs an operation on each element and passes the result as an accumulated value to the next element.
     * @param { U } initialValue - Initial value of the accumulator.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
    */
    reduce<U>(
      callbackFn: (previousValue: U, currentValue: T, currentIndex: number, array: Array<T>) => U,
      initialValue: U
    ): U;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.
     *     A negative index will count back from the last item.
     * @returns { T | undefined } The element in the array matching the given index.
     *     Always returns undefined if index < -array.length or index >= array.length without
     *     attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the element at a given index in this ArkTS array.
     *
     * @param { number } index - Index of the element. The index in an array always starts from 0 and is an integer.
     *     If a negative number is passed in, it refers to the index of index + array.length.
     * @returns { T | undefined } The element in the array matching the given index.
     *     Always returns undefined if index < -array.length or index >= array.length without
     *     attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): T | undefined;
    /**
     * Returns an iterator that can be used to iterate over elements of type T.
     *
     * @returns { IterableIterator<T> } Iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Obtains an iterator, each item of which is a JavaScript object.
     *
     * @returns { IterableIterator<T> } Iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<T>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     * 
     * @returns { IterableIterator<[number, T]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator object that contains the key-value pair of each element in this ArkTS array.
     * 
     * @returns { IterableIterator<[number, T]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, T]>;
    /**
     * Returns an iterable of keys in the array
     * 
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator object that contains the key of each element in this ArkTS array.
     * 
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     * 
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator object that contains the value of each element in this ArkTS array.
     * 
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<T>;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     * 
     * @param { function } predicate - Find calls predicate once for each element of the array, in ascending
     *     order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { T | undefined } The first element in the array that satisfies the provided testing function.
     *     Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element that passes a test provided by a callback function.
     * If none of the elements pass the test, undefined is returned.
     * 
     * @param { function } predicate - Function that takes three arguments. It is used to filter elements.
     *     The value true means that the current element meets the conditions, the traversal stops,
     *     and that element is returned. The value false means that the current element does not meet the condition,
     *     and the traversal continues until the element that meets the condition is found
     *     or the entire array is traversed.
     * @returns { T | undefined } The first element in the array that satisfies the provided testing function.
     *     Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: (value: T, index: number, obj: Array<T>) => boolean): T | undefined;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { T } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found within
     *     the array (or the part of the array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The includes method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Checks whether this ArkTS array contains an element and returns a Boolean value.
     *
     * @param { T } searchElement - Element to search for.
     * @param { number } [fromIndex] - Index from which the search starts. The default value is 0.
     * @returns { boolean } A boolean value which is true if the value searchElement is found within
     *     the array (or the part of the array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The includes method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: T, fromIndex?: number): boolean;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { function } predicate - Find calls predicate once for each element of the array, in ascending
     *     order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the array that passes the test. Otherwise, -1;
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element that passes a test provided by a callback function.
     * If none of the elements pass the test, -1 is returned.
     *
     * @param { function } predicate - Function that takes three arguments. It is used to filter elements.
     *     The value true means that the current element meets the conditions, the traversal stops,
     *     and the index of that element is returned. The value false means that the current element does not
     *     meet the condition, and the traversal continues until the element that meets the condition is found
     *     or the entire array is traversed.
     * @returns { number } The index of the first element in the array that passes the test. Otherwise, -1;
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: (value: T, index: number, obj: Array<T>) => boolean): number;
    /**
     * Returns the this object after filling the section identified by start and end with value
     * 
     * @param { T } value - Value to fill array section with
     * @param { number } [start] - Index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - Index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Array<T> } The modified array, filled with value.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Fills elements in the specified range of this ArkTS array with a given value.
     * 
     * @param { T } value - Value to fill in.
     * @param { number } [start] - Start index of the range. The default value is 0.
     * @param { number } [end] - End index of the range (exclusive). If no value is passed in,
     *     it refers to the last element of the array.
     * @returns { Array<T> } The modified array, filled with value.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: T, start?: number, end?: number): Array<T>;
    /**
     * Shrinks the ArkTS array to the given arrayLength.
     * 
     * @param { number } arrayLength - The new Array length.
     *     Throws error when arrayLength < 0 or arrayLength > 2^32.
     *     If arrayLength > array.length, array remains unchanged.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The shrinkTo method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Shrinks this ArkTS array to a given length.
     * 
     * @param { number } arrayLength - New length of the array.
     *     If a value greater than or equal to the current array length is passed in, the array does not change.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The shrinkTo method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    shrinkTo(arrayLength: number): void;
    /**
     * Extends the ArkTS array to the given arrayLength,
     * and appends new elements with given initialValue up to the arrayLength.
     * 
     * @param { number } arrayLength - The new Array length.
     *     Throws error when arrayLength < 0 or arrayLength > 2^32.
     *     If arrayLength < array.length, array remains unchanged.
     * @param { T } initialValue - Element initial value that will be appended to the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The extendTo method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Extends this array to a given length by adding elements with the specified initial value.
     * 
     * @param { number } arrayLength - New length of the array.
     *     If a value less than or equal to the current array length is passed in, the array does not change.
     * @param { T } initialValue - Initial value of the elements to be added.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The extendTo method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    extendTo(arrayLength: number, initialValue: T): void;
    /**
     * Returns the item at that index.
     * 
     * @param { number } index - The zero-based index of the desired code unit.
     *     Throws error if index < 0 or index >= array.length.
     * @returns { T } The element in the array matching the given index. 
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the element at a given index in this array.
     * 
     * @param { number } index - Index of the element. The index starts from zero.
     *     If the passed-in index is less than 0 or greater than or equal to the value of length, an error is thrown.
     * @returns { T } The element in the array matching the given index. 
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: T;
    /**
     * Concatenates two or more arrays.
     *
     * @param { ConcatArray<T>[] } items - The arrays to concatenate.
     * @returns { Array<T> } A new array containing the elements of the concatenated arrays.
     * @throws { BusinessError } 401 - Parameter error. Not a valid array.
     * @throws { BusinessError } 10200011 - The concat method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Concatenates this ArkTS array with one or more arrays.
     *
     * @param { ConcatArray<T>[] } items - Arrays to concatenate.
     * @returns { Array<T> } A new array containing the elements of the concatenated arrays.
     * @throws { BusinessError } 401 - Parameter error. Not a valid array.
     * @throws { BusinessError } 10200011 - The concat method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    concat(...items: ConcatArray<T>[]): Array<T>;
    /**
     * Removes elements from the array at the specified position.
     *
     * @param { number } start - The zero-based index at which to start changing the contents of the array.
     *    All the elements from start to the end of the array will be deleted.
     * @returns { Array<T> } An array containing the deleted elements.
     * @throws { BusinessError } 401 - Parameter error.Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The splice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Removes elements from a specified position in an array.
     *
     * @param { number } start - Index from which the removal starts. If -array.length =< start < 0,
     *     the removal starts from start + array.length. If start < -array.length, the removal starts from 0.
     * @returns { Array<T> } An array containing the deleted elements.
     * @throws { BusinessError } 401 - Parameter error.Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The splice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    splice(start: number): Array<T>;
    /**
     * Removes elements from the array and, if necessary, inserts new elements at the specified position.
     *
     * @param { number } start - The zero-based index at which to start changing the contents of the array.
     * @param { number } deleteCount - The number of elements to remove from the array,
     *    starting at the index specified by the start parameter.
     * @param { T[] } items - An array of elements to insert into the array,
     *    starting at the index specified by the start parameter.
     * @returns { Array<T> } An array containing the deleted elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The splice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Removes elements from a specified position in an array, and inserts new elements from the same position.
     *
     * @param { number } start - Index from which the removal starts. If -array.length =< start < 0,
     *     the removal starts from start + array.length. If start < -array.length, the removal starts from 0.
     * @param { number } deleteCount - Number of elements to remove.
     * @param { T[] } items - New elements to insert from the start position.
     *     If no value is passed in, only the elements in the array are removed.
     * @returns { Array<T> } An array containing the deleted elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The splice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    splice(start: number, deleteCount: number, ...items: T[]): Array<T>;
    /**
     * Check whether the input parameter is an ArkTS array.
     *
     * @param { Object|undefined|null } value - Value to check.
     * @returns { boolean } Returns true if the value is an array; otherwise, returns false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                     1. Mandatory parameters are left unspecified.
     *                                     2. Incorrect parameter types.
     *                                     3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static isArray(value: Object | undefined | null): boolean;
    /**
     * Creates an ArkTS array with a variable number of parameters.
     *
     * @param { T[] } items - Array of elements used to create the array.
     *     The number of elements can be zero, one, or more.
     * @returns { Array<T> } Returns a new Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                     1. Mandatory parameters are left unspecified.
     *                                     2. Incorrect parameter types.
     *                                     3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of<T>(...items: T[]): Array<T>;
    /**
     * Copies elements within a given range from this ArkTS array to another position in sequence.
     *
     * @param { number } target - Index to copy the elements to.
     * @param { number } start - Start index of the range. If a negative number is passed in,
     *     it refers to the index of start + array.length.
     * @param { number } end - End index of the range. If a negative number is passed in,
     *     it refers to the index of end + array.length. The default value is the length of the ArkTS array.
     * @returns { Array<T> } Returns the modified array after the copy operation.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                     1. Mandatory parameters are left unspecified.
     *                                     2. Incorrect parameter types.
     *                                     3. Parameter verification failed.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Array<T>;
    /**
     * Reverses elements in this ArkTS array and returns a reference to the same array.
     *
     * @returns { Array<T> } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reverse(): Array<T>;
    /**
     * Obtains the index of the last occurrence of the specified value in in this ArkTS array.
     *
     * @param { T } searchElement - Value to search for.
     * @param { number } fromIndex - Index from which the search starts. The default value is 0.
     *     If the index is greater than or equal to the length of the ArkTS array, -1 is returned.
     *     If a negative number is passed in, it refers to the index of fromIndex + array.length.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: T, fromIndex?: number): number;
    /**
     * Checks whether all elements in this ArkTS array meet a given condition.
     *
     * @param { ArrayPredicateFn<T, Array<T>> } predicate - Assertion function used for the test.
     * @returns { boolean } Returns true if the callback function returns a truthy value for every element;
     *	   <br>otherwise, it returns false. If the array is empty, it returns true.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    every(predicate: ArrayPredicateFn<T, Array<T>>): boolean;
    /**
     * Checks whether this ArkTS array contains an element that meets certain conditions.
     *
     * @param { ArrayPredicateFn<T, Array<T> } predicate - Assertion function used for the test.
     * @returns { boolean } Returns true if the callback function returns a truthy value for any element;
     *	   <br>otherwise, it returns false. If the array is empty, it returns false.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    some(predicate: ArrayPredicateFn<T, Array<T>>): boolean;
    /**
     * Converts an ArkTS array into a string.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Generates a string that matches the cultural conversions of the current system locale.
     * Each element converts itself to a string via its toLocaleString API, and these strings are then joined
     * together in sequence with commas (,).
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Reduce elements in an ArkTs Array from right to left.
     *
     * @param { ArrayReduceCallback<U, T, Array<T>> } callbackFn - Function that takes four arguments.
     *     It performs an operation on each element and passes the result as an accumulated value to the next element.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = T>(callbackFn: ArrayReduceCallback<U, T, Array<T>>, initialValue: U): U;
    /**
     * Reduce elements in an ArkTs Array from right to left.
     *
     * @param { ArrayReduceCallback<T, T, Array<T>> } callbackFn - Function that takes four arguments.
     *     It performs an operation on each element and passes the result as an accumulated value to the next element.
     * @returns { T } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: ArrayReduceCallback<T, T, Array<T>>): T;
 }

  /**
   * The Map holds key-value pairs.
   * If multiple threads access a Map instance concurrently, 
   * and at least one of the threads modifies the map structurally,
   * it must be synchronized externally.
   * 
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * The Map holds key-value pairs.
   * If multiple threads access a Map instance concurrently, 
   * and at least one of the threads modifies the map structurally,
   * it must be synchronized externally.
   * 
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Map<K, V> {
    /**
     * Returns the number of elements in the Map.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the number of elements in the Map.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly size: number;
    /**
     * A constructor used to create a Map.
     *
     * @param { readonly (readonly [K, V])[] | null } [entries] - An Array or other iterable object
     *    whose elements are key-value pairs.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Map's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a Map.
     *
     * @param { readonly (readonly [K, V])[] | null } [entries] - An Array or other iterable object
     *    whose elements are key-value pairs.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Map's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(entries?: readonly (readonly [K, V])[] | null)
    /**
     * A constructor used to create a Map.
     *
     * @param { Iterable<readonly [K, V]>} [iterable] - An iterable object to convert to an ArkTS Map.
     *    whose elements are key-value pairs.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Map's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a Map.
     *
     * @param { Iterable<readonly [K, V]>} [iterable] - An iterable object to convert to an ArkTS Map.
     *    whose elements are key-value pairs.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Map's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(iterable: Iterable<readonly [K, V]>);
    /**
     * Returns an iterator that iterates over key-value pairs.
     *
     * @returns { IterableIterator<[K, V]> } Iterator object that yields key-value pairs.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over key-value pairs.
     *
     * @returns { IterableIterator<[K, V]> } Iterator object that yields key-value pairs.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<[K, V]>
    /**
     * Returns an iterable of key, value pairs for every entry in the map.
     *
     * @returns { IterableIterator<[K, V]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the map.
     *
     * @returns { IterableIterator<[K, V]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[K, V]>;
    /**
     * Returns an iterable of keys in the map.
     *
     * @returns { IterableIterator<K> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the map.
     *
     * @returns { IterableIterator<K> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<K>;
    /**
     * Returns an iterable of values in the map.
     *
     * @returns { IterableIterator<V> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the map.
     *
     * @returns { IterableIterator<V> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<V>;
    /**
     * Clears the map.
     *
     * @throws { BusinessError } 10200011 - The clear method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Clears the map.
     *
     * @throws { BusinessError } 10200011 - The clear method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    clear(): void;
    /**
     * Returns true if an element in the Map existed and has been removed, or false if the element does not exist.
     *
     * @param { K } key - The key of the element to remove from the Map object.
     * @returns { boolean } True if an element in the Map Object existed and has been removed,
     *     or false if the element does not exist.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The delete method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns true if an element in the Map existed and has been removed, or false if the element does not exist.
     *
     * @param { K } key - The key of the element to remove from the Map object.
     * @returns { boolean } True if an element in the Map Object existed and has been removed,
     *     or false if the element does not exist.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The delete method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    delete(key: K): boolean;
    /**
     * Executes the provided callback once for each key of the map which actually exist.
     *
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The function to be called for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Executes the provided callback once for each key of the map which actually exist.
     *
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The function to be called for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: (value: V, key: K, map: Map<K, V>) => void): void;
    /**
     * Returns a specified element from the Map object.
     * If the value that is associated to the provided key is an object,
     * then you will get a reference to that object and any change made to that object
     * will effectively modify it inside the Map.
     *
     * @param { K } key - The key of the element to return from the Map object
     * @returns { V | undefined } The element associated with the specified key,
     *     or undefined if the key can''t be found in the Map object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The get method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a specified element from the Map object.
     * If the value that is associated to the provided key is an object,
     * then you will get a reference to that object and any change made to that object
     * will effectively modify it inside the Map.
     *
     * @param { K } key - The key of the element to return from the Map object
     * @returns { V | undefined } The element associated with the specified key,
     *     or undefined if the key can''t be found in the Map object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The get method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    get(key: K): V | undefined;
    /**
     * Returns boolean indicating whether an element with the specified key exists or not.
     *
     * @param { K } key - The key of the element to test for presence in the Map object.
     * @returns { boolean } true if an element with the specified key exists in the Map Object; otherwise false.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns boolean indicating whether an element with the specified key exists or not.
     *
     * @param { K } key - The key of the element to test for presence in the Map object.
     * @returns { boolean } true if an element with the specified key exists in the Map Object; otherwise false.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    has(key: K): boolean;
    /**
     * Adds a new element with a specified key and value to the Map.
     * If an element with the same key already exists, the element will be updated.
     *
     * @param { K } key - The key of the element to add to the Map object.
     * @param { V } value - The value of the element to add to the object.
     * @returns { Map<K, V> } The Object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds a new element with a specified key and value to the Map.
     * If an element with the same key already exists, the element will be updated.
     *
     * @param { K } key - The key of the element to add to the Map object.
     * @param { V } value - The value of the element to add to the object.
     * @returns { Map<K, V> } The Object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(key: K, value: V): Map<K, V>;
  }
  
  /**
   * Set lets you store unique values of any type.
   * If multiple threads access a Set instance concurrently, 
   * and at least one of the threads modifies the set structurally,
   * it must be synchronized externally.
   * 
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Set lets you store unique values of any type.
   * If multiple threads access a Set instance concurrently, 
   * and at least one of the threads modifies the set structurally,
   * it must be synchronized externally.
   * 
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Set<T> {
    /**
     * Returns the number of elements in the Set.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the number of elements in the Set.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly size: number;
    /**
     * A constructor used to create a Set.
     *
     * @param { readonly T[] | null } [values] - If an iterable object is passed,
     *     all of its elements will be added to the new Set.
     *     If you don't specify this parameter, or its value is null, the new Set is empty.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Set's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a Set.
     *
     * @param { readonly T[] | null } [values] - If an iterable object is passed,
     *     all of its elements will be added to the new Set.
     *     If you don't specify this parameter, or its value is null, the new Set is empty.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Set's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(values?: readonly T[] | null);
    /**
     * A constructor used to create a Set.
     *
     * @param { Iterable<T>} [iterable] - If an iterable object is passed,
     *     all of its elements will be added to the new Set.
     *     If you don't specify this parameter, or its value is null, the new Set is empty.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Set's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a Set.
     *
     * @param { Iterable<T>} [iterable] - If an iterable object is passed,
     *     all of its elements will be added to the new Set.
     *     If you don't specify this parameter, or its value is null, the new Set is empty.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200012 - The Set's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(iterable: Iterable<T>);
    /**
     * Returns an iterator that can be used to iterate over elements of type T.
     *
     * @returns { IterableIterator<T> } Iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that can be used to iterate over elements of type T.
     *
     * @returns { IterableIterator<T> } Iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<T>;
    /**
     * Returns an iterable of [value, value] pairs for each element in this set.
     * 
     * @returns { IterableIterator<[T, T]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of [value, value] pairs for each element in this set.
     * 
     * @returns { IterableIterator<[T, T]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The entries method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[T, T]>;
    /**
     * Returns an iterable of the values in the set.
     *
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of the values in the set.
     *
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The keys method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<T>;
    /**
     * Returns an iterable of values in the set.
     *
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the set.
     *
     * @returns { IterableIterator<T> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<T>;
    /**
     * Appends a new element with a specified value to the end of the Set.
     *
     * @param { T } value - The value of the element to add to the Set object.
     * @returns { Set<T> } The Set object with added value.
     * @throws { BusinessError } 10200011 - The add method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Appends a new element with a specified value to the end of the Set.
     *
     * @param { T } value - The value of the element to add to the Set object.
     * @returns { Set<T> } The Set object with added value.
     * @throws { BusinessError } 10200011 - The add method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    add(value: T): Set<T>;
    /**
     * Clears the Set.
     *
     * @throws { BusinessError } 10200011 - The clear method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Clears the Set.
     *
     * @throws { BusinessError } 10200011 - The clear method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    clear(): void;
    /**
     * Returns true if an element in the Set existed and has been removed, or false if the element does not exist.
     *
     * @param { T } value - The value to remove from Set.
     * @returns { boolean } Returns true if value was already in Set; otherwise false.
     * @throws { BusinessError } 10200011 - The delete method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns true if an element in the Set existed and has been removed, or false if the element does not exist.
     *
     * @param { T } value - The value to remove from Set.
     * @returns { boolean } Returns true if value was already in Set; otherwise false.
     * @throws { BusinessError } 10200011 - The delete method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    delete(value: T): boolean;
    /**
     * Executes a provided function once per each value in the Set object, in insertion order.
     *
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The function to be called for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Executes a provided function once per each value in the Set object, in insertion order.
     *
     * @param { function } callbackFn - A function that accepts up to three arguments.
     *     The function to be called for each element.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: (value: T, value2: T, set: Set<T>) => void): void;
    /**
     * A boolean indicating whether an element with the specified value exists in the Set or not.
     *
     * @param { T } value -  The value to test for presence in the Object.
     * @returns { boolean } Returns true if an element with the specified value exists in the Set object;
     *     otherwise false.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A boolean indicating whether an element with the specified value exists in the Set or not.
     *
     * @param { T } value -  The value to test for presence in the Object.
     * @returns { boolean } Returns true if an element with the specified value exists in the Set object;
     *     otherwise false.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    has(value: T): boolean;
  }
  /**
   * Represents a raw buffer of binary data, which is used to store data for the
   * different typed arrays. ArrayBuffers cannot be read from or written to directly,
   * but can be passed to a typed array or DataView Object to interpret the raw
   * buffer as needed.
   * If multiple threads access a ArrayBuffer instance concurrently, 
   * and at least one of the threads modifies the buffer structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * Represents a raw buffer of binary data, which is used to store data for the
   * different typed arrays. ArrayBuffers cannot be read from or written to directly,
   * but can be passed to a typed array or DataView Object to interpret the raw
   * buffer as needed.
   * If multiple threads access a ArrayBuffer instance concurrently, 
   * and at least one of the threads modifies the buffer structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class ArrayBuffer {
    /**
     * Read-only. The length of the ArrayBuffer (in bytes).
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Read-only. The length of the ArrayBuffer (in bytes).
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * A constructor used to create a ArrayBuffer.
     *
     * @param { number } byteLength - The length of the ArkTS array buffer
     * @throws { BusinessError } 10200012 - The ArrayBuffer's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a ArrayBuffer.
     *
     * @param { number } byteLength - The length of the ArkTS array buffer
     * @throws { BusinessError } 10200012 - The ArrayBuffer's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(byteLength: number);
    /**
     * Returns a section of an ArrayBuffer.
     *
     * @param { number } begin - Zero-based index at which to start extraction, converted to an integer.
     * @param { number } [end] - Zero-based index at which to end extraction, converted to an integer.
     *     Default is buffer.length
     * @returns { ArrayBuffer } A new ArrayBuffer containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an ArrayBuffer.
     *
     * @param { number } begin - Zero-based index at which to start extraction, converted to an integer.
     * @param { number } [end] - Zero-based index at which to end extraction, converted to an integer.
     *     Default is buffer.length
     * @returns { ArrayBuffer } A new ArrayBuffer containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(begin: number, end?: number): ArrayBuffer;
  }

  /**
   * A typed array of 8-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int8Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 8-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int8Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Int8Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Int8Array.
     *
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int8Array.
     *
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int8Array.
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int8Array.
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int8Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Int8Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int8Array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int8Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int8Array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Int8Array;
    
    /**
     * Creates an Int8Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int8Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int8Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int8Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Int8Array;
    /**
     * Creates an Int8Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int8Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int8Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int8Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int8Array } A new Int8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Int8Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Int8Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Int8Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Int8Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Int8Array>): Int8Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Int8Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Int8Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int8Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int8Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Int8Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int8Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int8Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Int8Array>): Int8Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int8Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int8Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Int8Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int8Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int8Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Int8Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int8Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int8Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Int8Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int8Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Int8Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int8Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int8Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Int8Array;
    /**
     * Gets a new Int8Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int8Array } A new Int8Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Int8Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int8Array } A new Int8Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Int8Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Int8Array.
     *
     * @param { number } searchElement - Element to search for in the Int8Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Int8Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Int8Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Int8Array> } callbackFn - A function that is called for
     * each element in the Int8Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Int8Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Int8Array>, initialValue: U): U;
    /**
     * Reduce elements in an Int8Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Int8Array> } callbackFn - A function that is called for
     * each element in the Int8Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Int8Array>): number;
    /**
     * Convert an Int8Array to a string.
     *
     * @returns { string } Returns a string representing the specified Int8Array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Int8Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Int8Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Int8Array.
     * @returns { Int8Array } Returns a new Int8Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Int8Array;
 }

  /**
   * The Uint8ClampedArray typed array represents an array of 8-bit unsigned integers clamped to 0–255.
   * The contents are initialized to 0.
   * If multiple threads access a Uint8ClampedArray instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * The Uint8ClampedArray typed array represents an array of 8-bit unsigned integers clamped to 0–255.
   * The contents are initialized to 0.
   * If multiple threads access a Uint8ClampedArray instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Uint8ClampedArray {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint8ClampedArray.
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint8ClampedArray.
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8ClampedArray.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint8ClampedArray's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Uint8ClampedArray from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint8ClampedArray.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8ClampedArray from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint8ClampedArray.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Uint8ClampedArray;
    
    /**
     * Creates an Uint8ClampedArray from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint8ClampedArray.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8ClampedArray from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint8ClampedArray.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Uint8ClampedArray;
    /**
     * Creates an Uint8ClampedArray from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint8ClampedArray.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8ClampedArray from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint8ClampedArray.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Uint8ClampedArray;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Uint8ClampedArray;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Uint8ClampedArray>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Uint8ClampedArray;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Uint8ClampedArray>): Uint8ClampedArray;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Uint8ClampedArray>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, findIndex immediately returns that element index.
     *     Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, findIndex immediately returns that element index.
     *     Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Uint8ClampedArray>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint8ClampedArray> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint8ClampedArray> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Uint8ClampedArray>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint8ClampedArray } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Uint8ClampedArray>): Uint8ClampedArray;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint8ClampedArray>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8ClampedArray> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Uint8ClampedArray>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint8ClampedArray } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint8ClampedArray } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Uint8ClampedArray;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint8ClampedArray } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint8ClampedArray } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Uint8ClampedArray;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint8ClampedArray> } predicate - A function
     *     that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Uint8ClampedArray>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint8ClampedArray } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint8ClampedArray } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Uint8ClampedArray;
    /**
     * Gets a new Uint8ClampedArray view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Uint8ClampedArray view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint8ClampedArray } A new Uint8ClampedArray object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Uint8ClampedArray;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Uint8ClampedArray.
     *
     * @param { number } searchElement - Element to search for in the Uint8ClampedArray..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Uint8ClampedArray.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Uint8ClampedArray from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8ClampedArray> } callbackFn - A function that is called for
     * each element in the Uint8ClampedArray.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Uint8ClampedArray will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Uint8ClampedArray>, initialValue: U): U;
    /**
     * Reduce elements in an Uint8ClampedArray from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8ClampedArray> } callbackFn - A function that is called for
     * each element in the Uint8ClampedArray.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Uint8ClampedArray>): number;
    /**
     * Convert an Uint8ClampedArray to a string.
     *
     * @returns { string } Returns a string representing the specified Uint8ClampedArray and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Uint8ClampedArray to a string, The elements are converted to string using their
     * toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Uint8ClampedArray containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the
     * Uint8ClampedArray.
     * @returns { Uint8ClampedArray } Returns a new Uint8ClampedArray instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Uint8ClampedArray;
 }

  /**
   * A typed array of 8-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint8Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 8-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint8Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Uint8Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Uint8Array.
     *
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8Array.
     *
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint8Array.
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint8Array.
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint8Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint8Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Uint8Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint8Array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint8Array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Uint8Array;
    /**
     * Creates an Uint8Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint8Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint8Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Uint8Array;
    /**
     * Creates an Uint8Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint8Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     * call on every element of the array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint8Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint8Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     * call on every element of the array.
     * @returns { Uint8Array } A new Uint8Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Uint8Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Uint8Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Uint8Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Uint8Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Uint8Array>): Uint8Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *      that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *      that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Uint8Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Uint8Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint8Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint8Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Uint8Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint8Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint8Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint8Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Uint8Array>): Uint8Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint8Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint8Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Uint8Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint8Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint8Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Uint8Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint8Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint8Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Uint8Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint8Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Uint8Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint8Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint8Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Uint8Array;
    /**
     * Gets a new Uint8Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint8Array } A new Uint8Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Uint8Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint8Array } A new Uint8Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Uint8Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Uint8Array.
     *
     * @param { number } searchElement - Element to search for in the Uint8Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Uint8Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Uint8Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint8Array> } callbackFn - A function that is called for
     * each element in the Uint8Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Uint8Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Uint8Array>, initialValue: U): U;
    /**
     * Reduce elements in an Uint8Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint8Array> } callbackFn - A function that is called for
     * each element in the Uint8Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Uint8Array>): number;
    /**
     * Convert an Uint8Array to a string.
     *
     * @returns { string } Returns a string representing the specified Uint8Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Uint8Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Uint8Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Uint8Array.
     * @returns { Uint8Array } Returns a new Uint8Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Uint8Array;
 }

  /**
   * A typed array of 16-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int16Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 16-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int16Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Int16Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Int16Array.
     *
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int16Array.
     *
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int16Array.
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int16Array.
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int16Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Int16Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int16Array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int16Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int16Array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Int16Array;
    /**
     * Creates an Int16Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int16Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int16Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int16Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Int16Array;
    /**
     * Creates an Int16Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int16Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int16Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int16Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int16Array } A new Int16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Int16Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Int16Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Int16Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Int16Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Int16Array>): Int16Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Int16Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Int16Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int16Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int16Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Int16Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int16Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int16Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Int16Array>): Int16Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int16Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int16Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Int16Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int16Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int16Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Int16Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int16Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int16Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Int16Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int16Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Int16Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int16Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int16Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Int16Array;
    /**
     * Gets a new Int16Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int16Array } A new Int16Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Int16Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int16Array } A new Int16Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Int16Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Int16Array.
     *
     * @param { number } searchElement - Element to search for in the Int16Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Int16Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Int16Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Int16Array> } callbackFn - A function that is called for
     * each element in the Int16Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Int16Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Int16Array>, initialValue: U): U;
    /**
     * Reduce elements in an Int16Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Int16Array> } callbackFn - A function that is called for
     * each element in the Int16Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Int16Array>): number;
    /**
     * Convert an Int16Array to a string.
     *
     * @returns { string } Returns a string representing the specified Int16Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Int16Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Int16Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Int16Array.
     * @returns { Int16Array } Returns a new Int16Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Int16Array;
 }

  /**
   * A typed array of 16-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint16Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 16-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint16Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Uint16Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Uint16Array.
     *
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint16Array.
     *
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint16Array.
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint16Array.
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint16Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint16Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Uint16Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint16Array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint16Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint16Array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Uint16Array;
    /**
     * Creates an Uint16Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint16Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint16Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint16Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Uint16Array;
    /**
     * Creates an Uint16Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint16Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint16Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint16Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint16Array } A new Uint16Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Uint16Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Uint16Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Uint16Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Uint16Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Uint16Array>): Uint16Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Uint16Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Uint16Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint16Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint16Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Uint16Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint16Array> } callbackFn - A function that accepts up to
     *     three arguments. The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint16Array> } callbackFn - A function that accepts up to
     *     three arguments. The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint16Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Uint16Array>): Uint16Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint16Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint16Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint16Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Uint16Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint16Array } The reference to the original typed array, now reversed.
     *     <br/>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint16Array } The reference to the original typed array, now reversed.
     *     <br/>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Uint16Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint16Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint16Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Uint16Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint16Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Uint16Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint16Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint16Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Uint16Array;
    /**
     * Gets a new Uint16Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint16Array } A new Uint16Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Uint16Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint16Array } A new Uint16Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Uint16Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Uint16Array.
     *
     * @param { number } searchElement - Element to search for in the Uint16Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Uint16Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Uint16Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint16Array> } callbackFn - A function that is called for
     * each element in the Uint16Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Uint16Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Uint16Array>, initialValue: U): U;
    /**
     * Reduce elements in an Uint16Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint16Array> } callbackFn - A function that is called for
     * each element in the Uint16Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Uint16Array>): number;
    /**
     * Convert an Uint16Array to a string.
     *
     * @returns { string } Returns a string representing the specified Uint16Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Uint16Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Uint16Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Uint16Array.
     * @returns { Uint16Array } Returns a new Uint16Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Uint16Array;
 }

  /**
   * A typed array of 32-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 32-bit integer values. The contents are initialized to 0.
   * If multiple threads access a Int32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Int32Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Int32Array.
     *
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int32Array.
     *
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int32Array.
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Int32Array.
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Int32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Int32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Int32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int32Array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Int32Array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Int32Array;
    /**
     * Creates an Int32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Int32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Int32Array;
    /**
     * Creates an Int32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Int32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Int32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Int32Array } A new Int32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Int32Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Int32Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Int32Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Int32Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Int32Array>): Int32Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Int32Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Int32Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Int32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Int32Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int32Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Int32Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Int32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Int32Array>): Int32Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int32Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Int32Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Int32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Int32Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int32Array } The reference to the original typed array, now reversed.
     *     <br/>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Int32Array } The reference to the original typed array, now reversed.
     *     <br/>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Int32Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Int32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Int32Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Int32Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Int32Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Int32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Int32Array;
    /**
     * Gets a new Int32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int32Array } A new Int32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Int32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Int32Array } A new Int32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Int32Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Int32Array.
     *
     * @param { number } searchElement - Element to search for in the Int32Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Int32Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Int32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Int32Array> } callbackFn - A function that is called for
     * each element in the Int32Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Int32Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Int32Array>, initialValue: U): U;
    /**
     * Reduce elements in an Int32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Int32Array> } callbackFn - A function that is called for
     * each element in the Int32Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Int32Array>): number;
    /**
     * Convert an Int32Array to a string.
     *
     * @returns { string } Returns a string representing the specified Int32Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Int32Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Int32Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Int32Array.
     * @returns { Int32Array } Returns a new Int32Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Int32Array;
 }

  /**
   * A typed array of 32-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * A typed array of 32-bit unsigned integer values. The contents are initialized to 0.
   * If multiple threads access a Uint32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Uint32Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Uint32Array.
     *
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint32Array.
     *
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint32Array.
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Uint32Array.
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Uint32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Uint32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Uint32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint32Array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Uint32Array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Uint32Array;
    /**
     * Creates an Uint32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Uint32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Uint32Array;
    /**
     * Creates an Uint32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Uint32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Uint32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Uint32Array } A new Uint32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Uint32Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Uint32Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Uint32Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Uint32Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Uint32Array>): Uint32Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Uint32Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - find calls predicate once for each element of
     *     the array, in ascending order, until it finds one where predicate returns true. If such an element is found,
     *     findIndex immediately returns that element index. Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Uint32Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Uint32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Uint32Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint32Array> } callbackFn - A function that accepts up to
     *     three arguments. The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Uint32Array> } callbackFn - A function that accepts up to
     *     three arguments. The map method calls the callbackfn function one time for each element in the array.
     * @returns { Uint32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Uint32Array>): Uint32Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint32Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { number } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Uint32Array>, initialValue: number): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U>(callbackFn: TypedArrayReduceCallback<U, number, Uint32Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint32Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Uint32Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Uint32Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Uint32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Uint32Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Uint32Array> } predicate - A function that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Uint32Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Uint32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Uint32Array;
    /**
     * Gets a new Uint32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint32Array } A new Uint32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Uint32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Uint32Array } A new Uint32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Uint32Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Uint32Array.
     *
     * @param { number } searchElement - Element to search for in the Uint32Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Uint32Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Uint32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Uint32Array> } callbackFn - A function that is called for
     * each element in the Uint32Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Uint32Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Uint32Array>, initialValue: U): U;
    /**
     * Reduce elements in an Uint32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Uint32Array> } callbackFn - A function that is called for
     * each element in the Uint32Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Uint32Array>): number;
    /**
     * Convert an Uint32Array to a string.
     *
     * @returns { string } Returns a string representing the specified Uint32Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Uint32Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Uint32Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of the Uint32Array.
     * @returns { Uint32Array } Returns a new Uint32Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Uint32Array;
 }

  /**
   * The Float32Array typed array represents an array of 32-bit float numbers.
   * The contents are initialized to 0.
   * If multiple threads access a Float32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * The Float32Array typed array represents an array of 32-bit float numbers.
   * The contents are initialized to 0.
   * If multiple threads access a Float32Array instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class Float32Array {
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The size in bytes of each element in the array.
     *
     * @type { number }
     * @readonly
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static readonly BYTES_PER_ELEMENT: number;
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The ArrayBuffer instance referenced by the array.
     *
     * @type { ArrayBuffer }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly buffer: ArrayBuffer;
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteLength: number;
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The offset in bytes of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly byteOffset: number;
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * The length of the array.
     *
     * @type { number }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * A constructor used to create an Float32Array.
     *
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Float32Array.
     *
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor();
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { number } length - The length of the array
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Float32Array.
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { Iterable<number> } elements - An iterable object to convert to an Float32Array.
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(elements: Iterable<number>);
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { ArrayLike<number> | ArrayBuffer } array - An array is initialized with the given elements
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(array: ArrayLike<number> | ArrayBuffer);
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create an Float32Array.
     *
     * @param { ArrayBuffer } buffer - An array is initialized with the given elements
     * @param { number } [byteOffset] - The byteOffset (in bytes) parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @param { number } [length] - The length parameter specifies the memory range
     *     that will be exposed by the typed array view.
     * @throws { BusinessError } 10200012 - The Float32Array's constructor cannot be directly invoked.
     * @throws { BusinessError } 401 - Parameter error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(buffer: ArrayBuffer, byteOffset?: number, length?: number);
    /**
     * Creates an Float32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Float32Array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Float32Array from an array-like object.
     *
     * @param { ArrayLike<number> } arrayLike - An array-like object to convert to an Float32Array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: ArrayLike<number>): Float32Array;
    /**
     * Creates an Float32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Float32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Float32Array from an array-like object.
     *
     * @param { ArrayLike<T> } arrayLike - An array-like object to convert to an Float32Array.
     * @param { TypedArrayFromMapFn<T, number> } mapFn - A mapping function to call on every element of the array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from<T>(arrayLike: ArrayLike<T>, mapFn: TypedArrayFromMapFn<T, number>): Float32Array;
    /**
     * Creates an Float32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Float32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Creates an Float32Array from an iterable object.
     *
     * @param { Iterable<number> } arrayLike - An iterable object to convert to an Float32Array.
     * @param { TypedArrayFromMapFn<number, number> } [mapFn] - A mapping function to
     *     call on every element of the array.
     * @returns { Float32Array } A new Float32Array instance
     * @throws { BusinessError } 401 - Parameter error.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    static from(arrayLike: Iterable<number>, mapFn?: TypedArrayFromMapFn<number, number>): Float32Array;
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after copying a section of the array identified by start and end
     * to the same array starting at position target.
     *
     * @param { number } target - If target is negative, it is treated as length+target where length is the
     *     length of the array.
     * @param { number } start - If start is negative, it is treated as length+start. If end is negative, it
     *     is treated as length+end.
     * @param { number } [end] - If not specified, length of the this object is used as its default value.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The copyWithin method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    copyWithin(target: number, start: number, end?: number): Float32Array;
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether all the members of an array satisfy the specified test.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The every method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value false, or until the end of the array.
     * @returns { boolean } true unless predicate returns a false value for a typed array element,
     *     in which case false is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The every method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    every(predicate: TypedArrayPredicateFn<number, Float32Array>): boolean;
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the this object after filling the section identified by start and end with value.
     *
     * @param { number } value - value to fill array section with.
     * @param { number } [start] - index to start filling the array at. If start is negative, it is treated as
     *     length+start where length is the length of the array.
     * @param { number } [end] - index to stop filling the array at. If end is negative, it is treated as
     *     length+end.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The fill method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    fill(value: number, start?: number, end?: number): Float32Array;
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the elements of an array that meet the condition specified in a callback function.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The filter method calls the predicate function one time for each element in the array.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The filter method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    filter(predicate: TypedArrayPredicateFn<number, Float32Array>): Float32Array;
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the value of the first element in the array where predicate is true, and undefined
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, find immediately returns that element value. Otherwise, find returns undefined.
     * @returns { number | undefined } The first element in the typed array
     *     that satisfies the provided testing function. Otherwise, undefined is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The find method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    find(predicate: TypedArrayPredicateFn<number, Float32Array>): number | undefined;
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, findIndex immediately returns that element index.
     *     Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first element in the array where predicate is true, and -1
     * otherwise.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - find calls predicate once for
     *     each element of the array, in ascending order, until it finds one where predicate returns true.
     *     If such an element is found, findIndex immediately returns that element index.
     *     Otherwise, findIndex returns -1.
     * @returns { number } The index of the first element in the typed array that passes the test. Otherwise, -1.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The findIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    findIndex(predicate: TypedArrayPredicateFn<number, Float32Array>): number;
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Float32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Performs the specified action for each element in an array.
     *
     * @param { TypedArrayForEachCallback<number, Float32Array> } callbackFn -  A function that
     *     accepts up to three arguments.
     *     forEach calls the callbackfn function one time for each element in the array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The forEach method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    forEach(callbackFn: TypedArrayForEachCallback<number, Float32Array>): void;
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the index of the first occurrence of a value in an array.
     *
     * @param { number } searchElement - The value to locate in the array.
     * @param { number } [fromIndex] - The array index at which to begin the search. If fromIndex is omitted, the
     *      search starts at index 0.
     * @returns { number } The first index of searchElement in the typed array; -1 if not found.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The indexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    indexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Adds all the elements of an array separated by the specified separator string.
     * @param { string } [separator] - A string used to separate one element of an array from the next in the
     *     resulting String. If omitted, the array elements are separated with a comma.
     * @returns { string } A string with all typed array elements joined.
     *     If array.length is 0, the empty string is returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The join method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    join(separator?: string): string;
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Float32Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls a defined callback function on each element of an array, and returns an array that
     * contains the results.
     *
     * @param { TypedArrayMapCallback<number, Float32Array> } callbackFn - A function that
     *     accepts up to three arguments.
     *     The map method calls the callbackfn function one time for each element in the array.
     * @returns { Float32Array } The array itself.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The map method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    map(callbackFn: TypedArrayMapCallback<number, Float32Array>): Float32Array;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Float32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<number, number, Float32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @returns { number } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce(callbackFn: TypedArrayReduceCallback<number, number, Float32Array>): number;
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Float32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Calls the specified callback function for all the elements in an array. The return value of
     * the callback function is the accumulated result, and is provided as an argument in the next
     * call to the callback function.
     *
     * @param { TypedArrayReduceCallback<U, number, Float32Array> } callbackFn - A function that
     *     accepts up to four arguments.
     *     The reduce method calls the callbackfn function one time for each element in the array.
     * @param { U } initialValue - If initialValue is specified, it is used as the initial value to start
     *     the accumulation. The first call to the callbackfn function provides this value as an argument
     *     instead of an array value.
     * @returns { U } The value that results from running the "reducer" callback function to
     *     completion over the entire typed array.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The reduce method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reduce<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Float32Array>, initialValue: U): U;
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Float32Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Reverses the elements in an Array.
     *
     * @returns { Float32Array } The reference to the original typed array, now reversed.
     *     <br>Note that the typed array is reversed in place, and no copy is made.
     * @throws { BusinessError } 10200011 - The reverse method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    reverse(): Float32Array;
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a value or an array of values.
     *
     * @param { ArrayLike<number> } array - A typed or untyped array of values to set.
     * @param { number } [offset] - The index in the current array at which the values are to be written.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The set method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    set(array: ArrayLike<number>, offset?: number): void;
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Float32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns a section of an array.
     *
     * @param { number } [start] - The beginning of the specified portion of the array.
     * @param { number } [end] - The end of the specified portion of the array.
     *     This is exclusive of the element at the index 'end'.
     * @returns { Float32Array } A new typed array containing the extracted elements.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The slice method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    slice(start?: number, end?: number): Float32Array;
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether the specified callback function returns true for any element of an array.
     *
     * @param { TypedArrayPredicateFn<number, Float32Array> } predicate - A function
     *     that accepts up to three arguments.
     *     The some method calls the predicate function for each element in the array until
     *     the predicate returns a value which is coercible to the Boolean value true, or until the end of the array.
     * @returns { boolean } false unless predicate returns a truthy value for a typed array element,
     *     in which case true is immediately returned.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The some method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    some(predicate: TypedArrayPredicateFn<number, Float32Array>): boolean;
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Float32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sorts an array.
     *
     * @param { TypedArrayCompareFn<number> } [compareFn] - Function used to determine the order of the elements.
     *     It is expected to return a negative value if first argument is less than second argument,
     *     zero if they're equal and a positive value otherwise.
     *     If omitted, the elements are sorted in ascending, ASCII character order.
     * @returns { Float32Array } The reference to the original typed array, now sorted.
     *     Note that the typed array is sorted in place and no copy is made.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The sort method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    sort(compareFn?: TypedArrayCompareFn<number>): Float32Array;
    /**
     * Gets a new Float32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Float32Array } A new Float32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets a new Float32Array view of the ArrayBuffer store for this array, referencing the elements
     * at begin, inclusive, up to end, exclusive.
     *
     * @param { number } [begin] - The index of the beginning of the array.
     * @param { number } [end] - The index of the end of the array.
     * @returns { Float32Array } A new Float32Array object.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The subarray method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    subarray(begin?: number, end?: number): Float32Array;
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item located at the specified index.
     *
     * @param { number } index - The zero-based index of the desired code unit.<br/>
     *     A negative index will count back from the last item.
     * @returns { number | undefined } The element in the array matching the given index.<br/>
     *     Always returns undefined if index < -array.length or
     *     index >= array.length without attempting to access the corresponding property.
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    at(index: number): number | undefined;
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over numbers.
     *
     * @returns { IterableIterator<number> } Iterator object that yields numbers.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of key, value pairs for every entry in the array
     *
     * @returns { IterableIterator<[number, number]> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    entries(): IterableIterator<[number, number]>;
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of keys in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    keys(): IterableIterator<number>;
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the array
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Determines whether an array includes a certain element, returning true or false as appropriate.
     *
     * @param { number } searchElement - The element to search for.
     * @param { number } [fromIndex] - The position in this array at which to begin searching for searchElement.
     * @returns { boolean } A boolean value which is true if the value searchElement is found <br/>
     *     within the typed array (or the part of the typed array indicated by the index fromIndex, if specified).
     * @throws { BusinessError } 401 - Parameter error.
     * @throws { BusinessError } 10200011 - The at method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    includes(searchElement: number, fromIndex?: number): boolean;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
    /**
     * Find the last occurrence of a specified element in an Float32Array.
     *
     * @param { number } searchElement - Element to search for in the Float32Array..
     * @param { number } fromIndex - The index at which to start the search. If provided:
     *	   <br>If this index is negative, it is treated as Float32Array.length + fromIndex.
     * @returns { number } Returns the last index of the specified element if found; otherwise, returns -1.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The lastIndexOf method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    lastIndexOf(searchElement: number, fromIndex?: number): number;
    /**
     * Reduce elements in an Float32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<U, number, Float32Array> } callbackFn - A function that is called
     * for each element in the Float32Array.
     * @param { U } initialValue - A value to use as the first argument to the first call of the callback.
     *     <br>If no initial value is provided, the last element of the Float32Array will be used,
     *     <br>and the callback will start with the second-to-last element.
     * @returns { U } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight<U = number>(callbackFn: TypedArrayReduceCallback<U, number, Float32Array>, initialValue: U): U;
    /**
     * Reduce elements in an Float32Array from right to left.
     *
     * @param { TypedArrayReduceCallback<number, number, Float32Array> } callbackFn - A function that is called
     * for each element in the Float32Array.
     * @returns { number } Returns the single value that results from the reduction.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The reduceRight method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    reduceRight(callbackFn: TypedArrayReduceCallback<number, number, Float32Array>): number;
    /**
     * Convert an Float32Array to a string.
     *
     * @returns { string } Returns a string representing the specified Float32Array and its elements,
     * separated by commas.
     * @throws { BusinessError } 10200011 - The toString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toString(): string;
    /**
     * Convert an Float32Array to a string, The elements are converted to string using their toLocaleString methods.
     *
     * @returns { string } Returns a string representing the specified array and its elements, separated by commas.
     * @throws { BusinessError } 10200011 - The toLocaleString method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    toLocaleString(): string;
    /**
     * Create an Float32Array containing these parameters.
     *
     * @param { number[] } items - A variable number of arguments that will be used as the elements of
     * the Float32Array.
     * @returns { Float32Array } Returns a new Float32Array instance containing the specified elements.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * <br>2. Incorrect parameter types; 3. Parameter verification failed.
     * @static
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 18
     */
    static of(...items: number[]): Float32Array;
 }

  /**
   * An ordered collections of bit values, which are either 0 or 1.
   * If multiple threads access a BitVector instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  /**
   * An ordered collections of bit values, which are either 0 or 1.
   * If multiple threads access a BitVector instance concurrently, 
   * and at least one of the threads modifies the array structurally,
   * it must be synchronized externally. 
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  @Sendable
  class BitVector {
    /**
     * A constructor used to create a BitVector object.
     *
     * @param { number } length - The length of BitVector object.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * A constructor used to create a BitVector object.
     *
     * @param { number } length - The length of BitVector object.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    constructor(length: number);
    /**
     * Gets the element number of the BitVector. This is a number one higher than the highest index in the bit vector.
     * It can be changed by resize().
     *
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Gets the element number of the BitVector. This is a number one higher than the highest index in the bit vector.
     * It can be changed by resize().
     *
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    readonly length: number;
    /**
     * Appends the bit element to the end of this bit vector.
     *
     * @param { number } element - Element to be appended to this bit vector (0 means 0, else means 1).
     * @returns { boolean } The boolean type, returns true if the addition is successful, and returns false if it fails.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The push method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Appends the bit element to the end of this bit vector.
     *
     * @param { number } element - Element to be appended to this bit vector (0 means 0, else means 1).
     * @returns { boolean } The boolean type, returns true if the addition is successful, and returns false if it fails.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The push method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    push(element: number): boolean;
    /**
     * Retrieves and removes the bit element to the end of this bit vector.
     *
     * @returns { number } The boolean type, if the bit push successfully, return true, else return false.
     * @throws { BusinessError } 10200011 - The pop method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Retrieves and removes the bit element to the end of this bit vector.
     *
     * @returns { number } The boolean type, if the bit push successfully, return true, else return false.
     * @throws { BusinessError } 10200011 - The pop method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    pop(): number;
    /**
     * Check if bit vector contains a particular bit element.
     *
     * @param { number } element - Element to be contained (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, containing the value at that index.
     * @returns { boolean } The boolean type, if bit vector contains the specified element, return true,
                            else return false.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Check if bit vector contains a particular bit element.
     *
     * @param { number } element - Element to be contained (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, containing the value at that index.
     * @returns { boolean } The boolean type, if bit vector contains the specified element, return true,
                            else return false.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The has method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    has(element: number, fromIndex: number, toIndex: number): boolean;
    /**
     * Sets a range of bits in a bit vector to a particular element.
     *
     * @param { number } element - Element to be set (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The setBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets a range of bits in a bit vector to a particular element.
     *
     * @param { number } element - Element to be set (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The setBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    setBitsByRange(element: number, fromIndex: number, toIndex: number): void;
    /**
     * Sets all of bits in a bit vector to a particular element.
     *
     * @param { number } element - Element to be set (0 means 0, else means 1).
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The setAllBits method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Sets all of bits in a bit vector to a particular element.
     *
     * @param { number } element - Element to be set (0 means 0, else means 1).
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The setAllBits method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    setAllBits(element: number): void;
    /**
     * Returns the bit values in a range of indices in a bit vector.
     *
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { BitVector } The BitVector type, returns the bit values in a range of indices in a bit vector.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the bit values in a range of indices in a bit vector.
     *
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { BitVector } The BitVector type, returns the bit values in a range of indices in a bit vector.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    getBitsByRange(fromIndex: number, toIndex: number): BitVector;
    /**
     * Resize the bitVector's length.
     *
     * @param { number } size - The new size for bitVector. If count is greater than the current size of bitVector,
     * the additional bit elements are set to 0.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The resize method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Resize the bitVector's length.
     *
     * @param { number } size - The new size for bitVector. If count is greater than the current size of bitVector,
     * the additional bit elements are set to 0.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200011 - The resize method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    resize(size: number): void;
    /**
     * Counts the number of times a certain bit element occurs within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be counted (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the number of times a certain bit element
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getBitCountByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Counts the number of times a certain bit element occurs within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be counted (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the number of times a certain bit element
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getBitCountByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    getBitCountByRange(element: number, fromIndex: number, toIndex: number): number;
    /**
     * Locates the first occurrence of a certain bit value within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be Located (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the first index of specified bit within a range,
     * or -1 if this range of the bitVector does not contain the element.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getIndexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Locates the first occurrence of a certain bit value within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be Located (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the first index of specified bit within a range,
     * or -1 if this range of the bitVector does not contain the element.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getIndexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    getIndexOf(element: number, fromIndex: number, toIndex: number): number;
    /**
     * Locates the last occurrence of a certain bit value within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be Located (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the last index of specified bit within a range,
     * or -1 if this range of the bitVector does not contain the element.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getLastIndexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Locates the last occurrence of a certain bit value within a range of bits in a bit vector.
     *
     * @param { number } element - Element to be Located (0 means 0, else means 1).
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @returns { number } The number type, return the last index of specified bit within a range,
     * or -1 if this range of the bitVector does not contain the element.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The getLastIndexOf method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    getLastIndexOf(element: number, fromIndex: number, toIndex: number): number;
    /**
     * Flips the bit value by index in a bit vector.(Flip 0 to 1, flip 1 to 0)
     *
     * @param { number } index - The index in the bit vector.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @throws { BusinessError } 10200011 - The flipBitByIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Flips the bit value by index in a bit vector.(Flip 0 to 1, flip 1 to 0)
     *
     * @param { number } index - The index in the bit vector.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of index is out of range.
     * @throws { BusinessError } 10200011 - The flipBitByIndex method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    flipBitByIndex(index: number): void;
    /**
     * Flips a range of bit values in a bit vector.(Flip 0 to 1, flip 1 to 0).
     *
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The flipBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Flips a range of bit values in a bit vector.(Flip 0 to 1, flip 1 to 0).
     *
     * @param { number } fromIndex - The starting position of the index, containing the value at that index position.
     * @param { number } toIndex - The end of the index, excluding the value at that index.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                    1.Mandatory parameters are left unspecified.
     *                                    2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of fromIndex or toIndex is out of range.
     * @throws { BusinessError } 10200011 - The flipBitsByRange method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    flipBitsByRange(fromIndex: number, toIndex: number): void;
    /**
     * Returns an iterator that iterates over bit vector.
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterator that iterates over bit vector.
     *
     * @returns { IterableIterator<number> } A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The Symbol.iterator method cannot be bound.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [Symbol.iterator](): IterableIterator<number>;    
    /**
     * Returns an iterable of values in the bit vector
     *
     * @returns { IterableIterator<number> }  A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns an iterable of values in the bit vector
     *
     * @returns { IterableIterator<number> }  A new iterable iterator object.
     * @throws { BusinessError } 10200011 - The values method cannot be bound.
     * @throws { BusinessError } 10200201 - Concurrent modification error.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    values(): IterableIterator<number>;
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @atomicservice
     * @since 12
     */
    /**
     * Returns the item at that index.
     * 
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 18
     */
    [index: number]: number;
  }
}

export default collections;