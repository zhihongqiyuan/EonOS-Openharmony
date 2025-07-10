/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
 * JSON is a syntax for serializing objects, arrays, numbers, strings, booleans, and null.
 *
 * @namespace json
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @atomicservice
 * @since 12
 */
declare namespace json {
  /**
   * The type of conversion result function.
   *
   * @typedef { function } Transformer
   * @param { Object } this - The object to which the parsed key value pair belongs.
   * @param { string } key - Attribute name.
   * @param { Object } value - The value of the parsed key value pair.
   * @syscap SystemCapability.Utils.Lang
   * @atomicservice
   * @since 12
   */
  type Transformer = (this: Object, key: string, value: Object) => Object | undefined | null;

  /**
   * Parses a JSON string into an ArkTS object or null.
   *
   * @param { string } text - Valid JSON string.
   * @param { Transformer } [reviver] - Conversion function. This parameter can be used to modify the value generated
   * after parsing. The default value is undefined.
   * @param {ParseOptions} options - Parsing options. This parameter is used to control the type of the parsing result.
   * The default value is undefined.
   * @returns { Object | null } Return an Object, array, string, number, boolean, or null value corresponding to JSON text.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function parse(text: string, reviver?: Transformer, options?: ParseOptions): Object | null;

  /**
   * Converts an ArkTS object or array into a JSON string. In the case of a container, linear containers are supported,
   * but non-linear containers are not.
   *
   * @param { Object } value - ArkTS object or array. In the case of a container, linear containers are supported, but
   * non-linear containers are not.
   * @param { (number | string)[] | null } [replacer] - If an array is passed in, only the keys in the array are
   * serialized to the final JSON string. If null is passed in, all keys of the object are serialized. The default
   * value is undefined.
   * @param { string | number } [space] - Indentation, white space, or line break characters inserted into the output
   * JSON string for readability purposes. If a number is passed in, it indicates the number of space characters to be
   * used as indentation. If a string is passed in, the string is inserted before the output JSON string. If null is
   * passed in, no white space is used. The default value is an empty string.
   * @returns { string } Return a JSON text.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function stringify(value: Object, replacer?: (number | string)[] | null, space?: string | number): string;

  /**
   * Converts an ArkTS object or array into a JSON string. In the case of a container, linear containers are supported,
   * but non-linear containers are not.
   *
   * @param { Object } value - ArkTS object or array. In the case of a container, linear containers are supported, but
   * non-linear containers are not.
   * @param { Transformer } [replacer] - During serialization, each key of the serialized value is converted and
   * processed by this function. The default value is undefined.
   * @param { string | number } [space] - Indentation, white space, or line break characters inserted into the output
   * JSON string for readability purposes. If a number is passed in, it indicates the number of space characters to be
   * used as indentation. If a string is passed in, the string is inserted before the output JSON string. If null is
   * passed in, no white space is used. The default value is an empty string.
   * @returns { string } Return a JSON text.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function stringify(value: Object, replacer?: Transformer, space?: string | number): string;

  /**
   * Checks whether an ArkTS object contains a key.
   * 
   * @param { object } obj - The object parsed from a JSON string.
   * @param { string } property - Determine whether the object contains the property.
   * @returns { boolean } Return true if the key is in the object, otherwise return false.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function has(obj: object, property: string): boolean;

  /**
   * Removes a key from an ArkTS object.
   *
   * @param { object } obj - The object parsed from a JSON string.
   * @param { string } property - The property to be removed.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  function remove(obj: object, property: string): void;

  /**
   * Enum defining modes for handling bigint.
   *
   * @enum { number } BigIntMode
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  const enum BigIntMode {
    /**
     * BigInt is not supported.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    DEFAULT = 0,
    /**
     * Parse as BigInt when number less than -(2^53 – 1) or greater than (2^53 – 1).
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    PARSE_AS_BIGINT = 1,
    /**
     * All numbers parse as BigInt.
     *
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 12
     */
    ALWAYS_PARSE_AS_BIGINT = 2,
  }

  /**
   * Parse's options
   *
   * @typedef ParseOptions
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 12
   */
  interface ParseOptions {
    /**
    * Enum defining modes for handling bigint.
    * @type { BigIntMode }
    * @syscap SystemCapability.Utils.Lang
    * @crossplatform
    * @atomicservice
    * @since 12
    */
    bigIntMode: BigIntMode;
  }
}
export default json;