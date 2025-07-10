/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
 * @kit ArkTS
 */

/**
 * The url module provides utilities for URL resolution and parsing.
 *
 * @namespace url
 * @syscap SystemCapability.Utils.Lang
 * @since 7
 */
/**
 * The url module provides utilities for URL resolution and parsing.
 *
 * @namespace url
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @since 10
 */
/**
 * The url module provides APIs for parsing URL strings and constructing URL instances to process URL strings.
 *
 * @namespace url
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace url {
  /**
   * The URLSearchParams interface defines some practical methods to process URL query strings.
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.url.URLParams
   * @name URLSearchParams
   */
  class URLSearchParams {
    /**
     * A parameterized constructor used to create an URLSearchParams instance.
     * As the input parameter of the constructor function, init supports four types.
     * The input parameter is a character string two-dimensional array.
     * The input parameter is the object list.
     * The input parameter is a character string.
     * The input parameter is the URLSearchParams object.
     *
     * @param { string[][] | Record<string, string> | string | URLSearchParams } init - init init
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.constructor
     */
    constructor(init?: string[][] | Record<string, string> | string | URLSearchParams);

    /**
     * Appends a specified key/value pair as a new search parameter.
     *
     * @param { string } name - name name Key name of the search parameter to be inserted.
     * @param { string } value - value value Values of search parameters to be inserted.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.append
     */
    append(name: string, value: string): void;

    /**
     * Deletes the given search parameter and its associated value,from the list of all search parameters.
     *
     * @param { string } name - name name Name of the key-value pair to be deleted.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.delete
     */
    delete(name: string): void;

    /**
     * Returns all key-value pairs associated with a given search parameter as an array.
     *
     * @param { string } name - name name Specifies the name of a key value.
     * @returns { string[] } string[] Returns all key-value pairs with the specified name.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.getAll
     */
    getAll(name: string): string[];

    /**
     * Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     *
     * @returns { IterableIterator<[string, string]> } Returns an iterator for ES6.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.entries
     */
    entries(): IterableIterator<[string, string]>;

    /**
     * Callback functions are used to traverse key-value pairs on the URLSearchParams instance object.
     *
     * @param { function } callbackFn - callbackFn callbackFn Current traversal key value.
     * @param { Object } thisArg - thisArg thisArg thisArg to be used as this value for when callbackFn is called
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.forEach
     */
    forEach(callbackFn: (value: string, key: string, searchParams: URLSearchParams) => void, thisArg?: Object): void;

    /**
     * Returns the first value associated to the given search parameter.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { string | null } Returns the first value found by name. If no value is found, null is returned.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.get
     */
    get(name: string): string | null;

    /**
     * Returns a Boolean that indicates whether a parameter with the specified name exists.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { boolean } Returns a Boolean value that indicates whether a found
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.has
     */
    has(name: string): boolean;

    /**
     * Sets the value associated with a given search parameter to the
     * given value. If there were several matching values, this method
     * deletes the others. If the search parameter doesn't exist, this
     * method creates it.
     *
     * @param { string } name - name name Key name of the parameter to be set.
     * @param { string } value - value value Indicates the parameter value to be set.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.set
     */
    set(name: string, value: string): void;

    /**
     * Sort all key/value pairs contained in this object in place and return undefined.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.sort
     */
    sort(): void;

    /**
     * Returns an iterator allowing to go through all keys contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the names of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.keys
     */
    keys(): IterableIterator<string>;

    /**
     * Returns an iterator allowing to go through all values contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the values of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.values
     */
    values(): IterableIterator<string>;

    /**
     * Returns an iterator allowing to go through all key/value
     * pairs contained in this object.
     * @returns { IterableIterator<[string, string]> } Returns an ES6 iterator. Each item of the iterator is a JavaScript Array. 
     * The first item of Array is name, and the second item of Array is value.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.[Symbol.iterator]
     */
    [Symbol.iterator](): IterableIterator<[string, string]>;

    /**
     * Returns a query string suitable for use in a URL.
     *
     * @returns { string } Returns a search parameter serialized as a string, percent-encoded if necessary.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URLParams.toString
     */
    toString(): string;
  }

  /**
   * The URLParams interface defines some practical methods to process URL query strings.
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   * @name URLParams
   */
  /**
   * The URLParams interface defines some practical methods to process URL query strings.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   * @name URLParams
   */
  /**
   * The URLParams interface defines some practical methods to process URL query strings.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   * @name URLParams
   */
  class URLParams {
    /**
     * A parameterized constructor used to create an URLParams instance.
     * As the input parameter of the constructor function, init supports four types.
     * The input parameter is a character string two-dimensional array.
     * The input parameter is the object list.
     * The input parameter is a character string.
     * The input parameter is the URLParams object.
     *
     * @param { string[][] | Record<string, string> | string | URLParams } [init] - init init
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * A parameterized constructor used to create an URLParams instance.
     * As the input parameter of the constructor function, init supports four types.
     * The input parameter is a character string two-dimensional array.
     * The input parameter is the object list.
     * The input parameter is a character string.
     * The input parameter is the URLParams object.
     *
     * @param { string[][] | Record<string, string> | string | URLParams } [init] - init init
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * A constructor used to create a URLParams instance.
     *
     * @param { string[][] | Record<string, string> | string | URLParams } [init] - Input parameter objects, which include the following:
     * - string[][]: two-dimensional string array.
     * - Record<string, string>: list of objects.
     * - string: string.
     * - URLParams: object.
     * The default value is null.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types; 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(init?: string[][] | Record<string, string> | string | URLParams);

    /**
     * Appends a specified key/value pair as a new search parameter.
     *
     * @param { string } name - name name Key name of the search parameter to be inserted.
     * @param { string } value - value value Values of search parameters to be inserted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Appends a specified key/value pair as a new search parameter.
     *
     * @param { string } name - name name Key name of the search parameter to be inserted.
     * @param { string } value - value value Values of search parameters to be inserted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Appends a key-value pair into the query string.
     *
     * @param { string } name - Key of the key-value pair to append.
     * @param { string } value - Value of the key-value pair to append.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    append(name: string, value: string): void;

    /**
     * Deletes the given search parameter and its associated value,from the list of all search parameters.
     *
     * @param { string } name - name name Name of the key-value pair to be deleted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Deletes the given search parameter and its associated value,from the list of all search parameters.
     *
     * @param { string } name - name name Name of the key-value pair to be deleted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Deletes key-value pairs of the specified key.
     *
     * @param { string } name - Key of the key-value pairs to delete.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    delete(name: string): void;

    /**
     * Returns all key-value pairs associated with a given search parameter as an array.
     *
     * @param { string } name - name name Specifies the name of a key value.
     * @returns { string[] } string[] Returns all key-value pairs with the specified name.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns all key-value pairs associated with a given search parameter as an array.
     *
     * @param { string } name - name name Specifies the name of a key value.
     * @returns { string[] } string[] Returns all key-value pairs with the specified name.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains all the values based on the specified key.
     *
     * @param { string } name - Target key.
     * @returns { string[] } string[] Returns all key-value pairs with the specified name.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    getAll(name: string): string[];

    /**
     * Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     *
     * @returns { IterableIterator<[string, string]> } Returns an iterator for ES6.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     *
     * @returns { IterableIterator<[string, string]> } Returns an iterator for ES6.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains an ES6 iterator. Each item of the iterator is a JavaScript array, and the first and second fields of
     * each array are the key and value respectively.
     *
     * @returns { IterableIterator<[string, string]> } Returns an iterator for ES6.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    entries(): IterableIterator<[string, string]>;

    /**
     * Callback functions are used to traverse key-value pairs on the URLParams instance object.
     *
     * @param { function } callbackFn - callbackFn value Current traversal key value,
	 * key Indicates the name of the key that is traversed.
     * @param { Object } [thisArg] - thisArg thisArg to be used as this value for when callbackFn is called
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Callback functions are used to traverse key-value pairs on the URLParams instance object.
     *
     * @param { function } callbackFn - callbackFn value Current traversal key value,
	 * key Indicates the name of the key that is traversed.
     * @param { Object } [thisArg] - thisArg thisArg to be used as this value for when callbackFn is called
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Traverses the key-value pairs in the URLSearchParams instance by using a callback.
     *
     * @param { function } callbackFn - Callback invoked to traverse the key-value pairs in the URLSearchParams instance.
     * @param { Object } [thisArg] - Value of this to use when callbackFn is invoked. The default value is this object.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    forEach(callbackFn: (value: string, key: string, searchParams: URLParams) => void, thisArg?: Object): void;

    /**
     * Returns the first value associated to the given search parameter.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { string | null } Returns the first value found by name. If no value is found, null is returned.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns the first value associated to the given search parameter.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { string | null } Returns the first value found by name. If no value is found, null is returned.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains the value of the first key-value pair based on the specified key.
     *
     * @param { string } name - Key specified to obtain the value.
     * @returns { string | null } Returns the first value found by name. If no value is found, null is returned.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    get(name: string): string | null;

    /**
     * Returns a Boolean that indicates whether a parameter with the specified name exists.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { boolean } Returns a Boolean value that indicates whether a found
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a Boolean that indicates whether a parameter with the specified name exists.
     *
     * @param { string } name - name name Specifies the name of a key-value pair.
     * @returns { boolean } Returns a Boolean value that indicates whether a found
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Checks whether a key has a value.
     *
     * @param { string } name - Key specified to search for its value.
     * @returns { boolean } Returns a Boolean value that indicates whether a found
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    has(name: string): boolean;

    /**
     * Sets the value associated with a given search parameter to the
     * given value. If there were several matching values, this method
     * deletes the others. If the search parameter doesn't exist, this
     * method creates it.
     *
     * @param { string } name - name name Key name of the parameter to be set.
     * @param { string } value - value value Indicates the parameter value to be set.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Sets the value associated with a given search parameter to the
     * given value. If there were several matching values, this method
     * deletes the others. If the search parameter doesn't exist, this
     * method creates it.
     *
     * @param { string } name - name name Key name of the parameter to be set.
     * @param { string } value - value value Indicates the parameter value to be set.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Sets the value for a key. If key-value pairs matching the specified key exist, the value of the first key-value
     * pair will be set to the specified value and other key-value pairs will be deleted. Otherwise, the key-value pair
     * will be appended to the query string.
     *
     * @param { string } name - Key of the value to set.
     * @param { string } value - Value to set.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    set(name: string, value: string): void;

    /**
     * Sort all key/value pairs contained in this object in place and return undefined.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Sort all key/value pairs contained in this object in place and return undefined.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Sorts all key-value pairs contained in this object based on the Unicode code points of the keys and returns
     * undefined. This method uses a stable sorting algorithm, that is, the relative order between key-value pairs
     * with equal keys is retained.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    sort(): void;

    /**
     * Returns an iterator allowing to go through all keys contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the names of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns an iterator allowing to go through all keys contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the names of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains an ES6 iterator that contains the keys of all the key-value pairs.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the names of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    keys(): IterableIterator<string>;

    /**
     * Returns an iterator allowing to go through all values contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the values of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns an iterator allowing to go through all values contained in this object.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the values of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains an ES6 iterator that contains the values of all the key-value pairs.
     *
     * @returns { IterableIterator<string> } Returns an ES6 Iterator over the values of each name-value pair.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    values(): IterableIterator<string>;

    /**
     * Returns an iterator allowing to go through all key/value
     * pairs contained in this object.
     *
     * @returns { IterableIterator<[string, string]> } Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns an iterator allowing to go through all key/value
     * pairs contained in this object.
     *
     * @returns { IterableIterator<[string, string]> } Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains an ES6 iterator. Each item of the iterator is a JavaScript array, and the first and second fields ofeach array are
     * the key and value respectively.
     *
     * @returns { IterableIterator<[string, string]> } Returns an ES6 iterator. Each item of the iterator is a JavaScript Array.
     * The first item of Array is name, and the second item of Array is value.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    [Symbol.iterator](): IterableIterator<[string, string]>;

    /**
     * Returns a query string suitable for use in a URL.
     *
     * @returns { string } Returns a search parameter serialized as a string, percent-encoded if necessary.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a query string suitable for use in a URL.
     *
     * @returns { string } Returns a search parameter serialized as a string, percent-encoded if necessary.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains search parameters that are serialized as a string and, if necessary, percent-encodes the characters in the string.
     *
     * @returns { string } Returns a search parameter serialized as a string, percent-encoded if necessary.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    toString(): string;
  }

  /**
   * The interface of URL is used to parse, construct, normalize, and encode URLs.
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 7
   * @name URL
   */
  /**
   * The interface of URL is used to parse, construct, normalize, and encode URLs.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   * @name URL
   */
  /**
   * The interface of URL is used to parse, construct, normalize, and encode URLs.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   * @name URL
   */
  class URL {
    /**
     * URL constructor, which is used to instantiate a URL object.
     * url: Absolute or relative input URL to resolve. Base is required if input is relative.
     * If input is an absolute value, base ignores the value.
     * base: Base URL to parse if input is not absolute.
     *
     * @param { string } url - url url
     * @param { string | URL } base - base base
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URL.parseURL
     */
    constructor(url: string, base?: string | URL);

    /**
     * URL constructor, which is used to instantiate a URL object.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * URL constructor, which is used to instantiate a URL object.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * A no-argument constructor used to create a URL. It returns a URL object after parseURL is called.
     * It is not used independently.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor();

    /**
     * Replaces the original constructor to process arguments and return a url object.
     *
     * @param { string } url - url url Absolute or relative input URL to resolve. Base is required if input is relative.
     * If input is an absolute value, base ignores the value.
     * @param { string | URL } [base] - base base Base URL to parse if input is not absolute.
     * @returns { URL }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200002 - Invalid url string.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Replaces the original constructor to process arguments and return a url object.
     *
     * @param { string } url - url url Absolute or relative input URL to resolve. Base is required if input is relative.
     * If input is an absolute value, base ignores the value.
     * @param { string | URL } [base] - base base Base URL to parse if input is not absolute.
     * @returns { URL }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200002 - Invalid url string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Parses a URL.
     *
     * @param { string } url - A string representing an absolute or a relative URL.
     * In the case of a relative URL, you must specify base to parse the final URL.
     * In the case of an absolute URL, the passed base will be ignored.
     * If input is an absolute value, base ignores the value.
     * @param { string | URL } [base] - Either a string or an object. The default value is undefined.
     * - string: string.
     * - URL: URL object.
     * This parameter is used when url is a relative URL.
     * @returns { URL }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200002 - Invalid url string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    static parseURL(url: string, base?: string | URL): URL;

    /**
     * Returns the serialized URL as a string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Returns the serialized URL as a string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Converts the parsed URL into a string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    toString(): string;

    /**
     * Returns the serialized URL as a string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Returns the serialized URL as a string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Converts the parsed URL into a JSON string.
     *
     * @returns { string } Returns the serialized URL as a string.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    toJSON(): string;

    /**
     * Gets and sets the fragment portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the fragment portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the fragment portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    hash: string;

    /**
     * Gets and sets the host portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the host portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the host portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    host: string;

    /**
     * Gets and sets the host name portion of the URL，not include the port.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the host name portion of the URL，not include the port.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the host name portion of the URL，not include the port.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    hostname: string;

    /**
     * Gets and sets the serialized URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the serialized URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the serialized URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    href: string;

    /**
     * Gets the read-only serialization of the URL's origin.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets the read-only serialization of the URL's origin.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets the read-only serialization of the URL's origin.
     *
     * @type { string }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readonly origin: string;

    /**
     * Gets and sets the password portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the password portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the password portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    password: string;

    /**
     * Gets and sets the path portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the path portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the path portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    pathname: string;

    /**
     * Gets and sets the port portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the port portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the port portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    port: string;

    /**
     * Gets and sets the protocol portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the protocol portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the protocol portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    protocol: string;

    /**
     * Gets and sets the serialized query portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the serialized query portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the serialized query portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    search: string;

    /**
     * Gets the URLSearchParams object that represents the URL query parameter.
     * This property is read-only, but URLSearchParams provides an object that can be used to change
     * the URL instance. To replace the entire query parameter for a URL, use url.searchsetter.
     *
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     * @deprecated since 9
     * @useinstead ohos.url.URL.params
     */
    readonly searchParams: URLSearchParams;

    /**
     * Gets the URLParams object that represents the URL query parameter.
     * This property is read-only, but URLParams provides an object that can be used to change
     * the URL instance. To replace the entire query parameter for a URL, use url.searchsetter.
     *
     * @type { URLParams }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Gets the URLParams object that represents the URL query parameter.
     * This property is read-only, but URLParams provides an object that can be used to change
     * the URL instance. To replace the entire query parameter for a URL, use url.searchsetter.
     *
     * @type { URLParams }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets the URLParams object that represents the URL query parameter.
     * This property is read-only, but URLParams provides an object that can be used to change
     * the URL instance. To replace the entire query parameter for a URL, use url.searchsetter.
     *
     * @type { URLParams }
     * @readonly
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readonly params: URLParams;

    /**
     * Gets and sets the username portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 7
     */
    /**
     * Gets and sets the username portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Gets and sets the username portion of the URL.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    username: string;
  }
}
export default url;
