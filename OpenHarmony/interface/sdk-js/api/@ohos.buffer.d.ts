/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (The type of "License");
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
 * The Buffer class is a global type for dealing with binary data directly. It can be constructed in a variety of ways.
 *
 * @namespace buffer
 * @syscap SystemCapability.Utils.Lang
 * @since 9
 */
/**
 * The Buffer class is a global type for dealing with binary data directly. It can be constructed in a variety of ways.
 *
 * @namespace buffer
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @since 10
 */
/**
 * The Buffer class is a global type for dealing with binary data directly. It can be constructed in a variety of ways.
 *
 * @namespace buffer
 * @syscap SystemCapability.Utils.Lang
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace buffer {
  /**
   * This parameter specifies the type of a common encoding format.
   *
   * @typedef { 'ascii' | 'utf8' | 'utf-8' | 'utf16le' | 'ucs2' | 'ucs-2' | 'base64' | 'base64url' | 'latin1' | 'binary' | 'hex' }
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * This parameter specifies the type of a common encoding format.
   *
   * @typedef { 'ascii' | 'utf8' | 'utf-8' | 'utf16le' | 'ucs2' | 'ucs-2' | 'base64' | 'base64url' | 'latin1' | 'binary' | 'hex' }
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the supported encoding formats.
   *
   * @typedef { 'ascii' | 'utf8' | 'utf-8' | 'utf16le' | 'ucs2' | 'ucs-2' | 'base64' | 'base64url' | 'latin1' | 'binary' | 'hex' }
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  type BufferEncoding =
    | 'ascii'
    | 'utf8'
    | 'utf-8'
    | 'utf16le'
    | 'ucs2'
    | 'ucs-2'
    | 'base64'
    | 'base64url'
    | 'latin1'
    | 'binary'
    | 'hex';
  /**
   * TypedArray inherits the features and methods of Int8Array
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * TypedArray inherits the features and methods of Int8Array
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * TypedArray inherits the features and methods of Int8Array
   * 
   * @extends Int8Array
   * @typedef TypedArray
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  interface TypedArray extends Int8Array {}
  /**
   * Allocates a new Buffer for a fixed size bytes. If fill is undefined, the Buffer will be zero-filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @param { string | Buffer | number } [fill] - fill [fill=0] A value to pre-fill the new Buffer with
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8']  If `fill` is a string, this is its encoding
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Allocates a new Buffer for a fixed size bytes. If fill is undefined, the Buffer will be zero-filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @param { string | Buffer | number } [fill] - fill [fill=0] A value to pre-fill the new Buffer with
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8']  If `fill` is a string, this is its encoding
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates and initializes a Buffer instance of the specified length.
   *
   * @param { number } size - Size of the Buffer instance to create, in bytes.
   * @param { string | Buffer | number } [fill] - Value to be filled in the buffer. The default value is 0.
   * @param { BufferEncoding } [encoding] - Encoding format (valid only when fill is a string). The default value is 'utf8'.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function alloc(size: number, fill?: string | Buffer | number, encoding?: BufferEncoding): Buffer;

  /**
   * Allocates a new Buffer for a fixed size bytes. The Buffer will not be initially filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Allocates a new Buffer for a fixed size bytes. The Buffer will not be initially filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance of the specified size from the buffer pool, without initializing it.
   * You need to use fill() to initialize the Buffer instance created.
   *
   * @param { number } size - Size of the Buffer instance to create, in bytes.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function allocUninitializedFromPool(size: number): Buffer;

  /**
   * Allocates a new un-pooled Buffer for a fixed size bytes. The Buffer will not be initially filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Allocates a new un-pooled Buffer for a fixed size bytes. The Buffer will not be initially filled.
   *
   * @param { number } size - size size The desired length of the new Buffer
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance of the specified size, without initializing it. 
   *
   * @param { number } size - Size of the Buffer instance to create, in bytes.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function allocUninitialized(size: number): Buffer;

  /**
   * Returns the byte length of a string when encoded using `encoding`.
   * This is not the same as [`String.prototype.length`], which does not account
   * for the encoding that is used to convert the string into bytes.
   *
   * @param { string | Buffer | TypedArray | DataView | ArrayBuffer | SharedArrayBuffer } string - string string A value to calculate the length of
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If `string` is a string, this is its encoding
   * @returns { number } The number of bytes contained within `string`
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Returns the byte length of a string when encoded using `encoding`.
   * This is not the same as [`String.prototype.length`], which does not account
   * for the encoding that is used to convert the string into bytes.
   *
   * @param { string | Buffer | TypedArray | DataView | ArrayBuffer | SharedArrayBuffer } string - string string A value to calculate the length of
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If `string` is a string, this is its encoding
   * @returns { number } The number of bytes contained within `string`
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Obtains the number of bytes of a string based on the encoding format.
   *
   * @param { string | Buffer | TypedArray | DataView | ArrayBuffer | SharedArrayBuffer } string - Target string.
   * @param { BufferEncoding } [encoding] - Encoding format of the string. The default value is 'utf8'.
   * @returns { number } The number of bytes contained within `string`
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function byteLength(
    string: string | Buffer | TypedArray | DataView | ArrayBuffer | SharedArrayBuffer,
    encoding?: BufferEncoding
  ): number;

  /**
   * Returns a new `Buffer` which is the result of concatenating all the `Buffer`instances in the `list` together.
   *
   * @param { Buffer[] | Uint8Array[] } list - list list List of `Buffer` or Uint8Array instances to concatenate
   * @param { number } [totalLength] - totalLength totalLength Total length of the `Buffer` instances in `list` when concatenated
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200001 - The value of "length" is out of range. It must be >= 0 and <= uint32 max. Received value is: [length]
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Returns a new `Buffer` which is the result of concatenating all the `Buffer`instances in the `list` together.
   *
   * @param { Buffer[] | Uint8Array[] } list - list list List of `Buffer` or Uint8Array instances to concatenate
   * @param { number } [totalLength] - totalLength totalLength Total length of the `Buffer` instances in `list` when concatenated
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200001 - The value of "length" is out of range. It must be >= 0 and <= uint32 max. Received value is: [length]
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Concatenates an array of Buffer instances of the specified length into a new instance.
   *
   * @param { Buffer[] | Uint8Array[] } list - Array of instances to concatenate.
   * @param { number } [totalLength] - Total length of bytes to be copied. The default value is 0.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types;
   * 3.Parameter verification failed.
   * @throws { BusinessError } 10200001 - The value of "length" is out of range. It must be >= 0 and <= uint32 max. Received value is: [length]
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function concat(list: Buffer[] | Uint8Array[], totalLength?: number): Buffer;

  /**
   * Allocates a new Buffer using an array of bytes in the range 0 – 255. Array entries outside that range will be truncated to fit into it.
   *
   * @param { number[] } array - array array an array of bytes in the range 0 – 255
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Allocates a new Buffer using an array of bytes in the range 0 – 255. Array entries outside that range will be truncated to fit into it.
   *
   * @param { number[] } array - array array an array of bytes in the range 0 – 255
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance with the specified array.
   *
   * @param { number[] } array - Array to create a Buffer instance.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function from(array: number[]): Buffer;

  /**
   * This creates a view of the ArrayBuffer without copying the underlying memory.
   *
   * @param { ArrayBuffer | SharedArrayBuffer } arrayBuffer - arrayBuffer arrayBuffer An ArrayBuffer, 
   * SharedArrayBuffer, for example the .buffer property of a TypedArray.
   * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Index of first byte to expose
   * @param { number } [length] - length [length = arrayBuffer.byteLength - byteOffset] Number of bytes to expose
   * @returns { Buffer } Return a view of the ArrayBuffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @throws { BusinessError } 10200001 - The value of "[byteOffset/length]" is out of range.
   * It must be >= [left range] and <= [right range]. Received value is: [byteOffset/length]
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * This creates a view of the ArrayBuffer without copying the underlying memory.
   *
   * @param { ArrayBuffer | SharedArrayBuffer } arrayBuffer - arrayBuffer arrayBuffer An ArrayBuffer, 
   * SharedArrayBuffer, for example the .buffer property of a TypedArray.
   * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Index of first byte to expose
   * @param { number } [length] - length [length = arrayBuffer.byteLength - byteOffset] Number of bytes to expose
   * @returns { Buffer } Return a view of the ArrayBuffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @throws { BusinessError } 10200001 - The value of "[byteOffset/length]" is out of range.
   * It must be >= [left range] and <= [right range]. Received value is: [byteOffset/length]
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance of the specified length that shares memory with arrayBuffer.
   *
   * @param { ArrayBuffer | SharedArrayBuffer } arrayBuffer - ArrayBuffer or SharedArrayBuffer instance whose memory is to be shared.
   * @param { number } [byteOffset] - Byte offset. The default value is 0.
   * @param { number } [length] - Length of the Buffer instance to create, in bytes. The default value is arrayBuffer.byteLength minus byteOffset.
   * @returns { Buffer } Return a view of the ArrayBuffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @throws { BusinessError } 10200001 - The value of "[byteOffset/length]" is out of range.
   * It must be >= [left range] and <= [right range]. Received value is: [byteOffset/length]
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function from(arrayBuffer: ArrayBuffer | SharedArrayBuffer, byteOffset?: number, length?: number): Buffer;

  /**
   * Copies the passed buffer data onto a new Buffer instance.
   *
   * @param { Buffer | Uint8Array } buffer - buffer buffer An existing Buffer or Uint8Array from which to copy data
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Copies the passed buffer data onto a new Buffer instance.
   *
   * @param { Buffer | Uint8Array } buffer - buffer buffer An existing Buffer or Uint8Array from which to copy data
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Copies the data of a passed Buffer instance to create a new Buffer instance and returns the new one.
   *
   * @param { Buffer | Uint8Array } buffer - Buffer or Uint8Array instance.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function from(buffer: Buffer | Uint8Array): Buffer;

  /**
   * For the object whose value returned by valueof() function is strictly equal to object
   * or supports symbol To primitive object, a new buffer instance is created.
   *
   * @param { Object } object - object object An object supporting Symbol.toPrimitive or valueOf()
   * @param { number | string } offsetOrEncoding - offsetOrEncoding offsetOrEncoding A byte-offset or encoding
   * @param { number } length - length length A length
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * For the object whose value returned by valueof() function is strictly equal to object
   * or supports symbol To primitive object, a new buffer instance is created.
   *
   * @param { Object } object - object object An object supporting Symbol.toPrimitive or valueOf()
   * @param { number | string } offsetOrEncoding - offsetOrEncoding offsetOrEncoding A byte-offset or encoding
   * @param { number } length - length length A length
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance based on the specified object.
   *
   * @param { Object } object - Object that supports Symbol.toPrimitive or valueOf().
   * @param { number | string } offsetOrEncoding - Byte offset or encoding format.
   * @param { number } length - Length of the Buffer instance to create, in bytes.
   * @returns { Buffer } Return a new allocated Buffer
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function from(object: Object, offsetOrEncoding: number | string, length: number): Buffer;

  /**
   * Creates a new Buffer containing string. The encoding parameter identifies the character encoding
   * to be used when converting string into bytes.
   *
   * @param { String } string - string string  A string to encode
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] The encoding of string
   * @returns { Buffer } Return a new Buffer containing string
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Creates a new Buffer containing string. The encoding parameter identifies the character encoding
   * to be used when converting string into bytes.
   *
   * @param { String } string - string string  A string to encode
   * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] The encoding of string
   * @returns { Buffer } Return a new Buffer containing string
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Creates a Buffer instance based on a string in the given encoding format.
   *
   * @param { String } string - String.
   * @param { BufferEncoding } [encoding] - Encoding format of the string. The default value is 'utf8'.
   * @returns { Buffer } Return a new Buffer containing string
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function from(string: String, encoding?: BufferEncoding): Buffer;

  /**
   * Returns true if obj is a Buffer, false otherwise
   *
   * @param { Object } obj - obj obj Objects to be judged
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Returns true if obj is a Buffer, false otherwise
   *
   * @param { Object } obj - obj obj Objects to be judged
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Checks whether the specified object is a Buffer instance.
   *
   * @param { Object } obj - Object to check.
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function isBuffer(obj: Object): boolean;

  /**
   * Returns true if encoding is the name of a supported character encoding, or false otherwise.
   *
   * @param { string } encoding - encoding encoding A character encoding name to check
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Returns true if encoding is the name of a supported character encoding, or false otherwise.
   *
   * @param { string } encoding - encoding encoding A character encoding name to check
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Checks whether the encoding format is supported.
   *
   * @param { string } encoding - Encoding format.
   * @returns { boolean } true or false
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function isEncoding(encoding: string): boolean;

  /**
   * Compares buf1 to buf2
   *
   * @param { Buffer | Uint8Array } buf1 - buf1 buf1 A Buffer or Uint8Array instance.
   * @param { Buffer | Uint8Array } buf2 - buf2 buf2 A Buffer or Uint8Array instance.
   * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
   *         1 is returned if target should come before buf when sorted.
   *        -1 is returned if target should come after buf when sorted.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Compares buf1 to buf2
   *
   * @param { Buffer | Uint8Array } buf1 - buf1 buf1 A Buffer or Uint8Array instance.
   * @param { Buffer | Uint8Array } buf2 - buf2 buf2 A Buffer or Uint8Array instance.
   * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
   *         1 is returned if target should come before buf when sorted.
   *        -1 is returned if target should come after buf when sorted.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Compares two Buffer instances. This API is used for sorting Buffer instances.
   *
   * @param { Buffer | Uint8Array } buf1 - Buffer instance to compare.
   * @param { Buffer | Uint8Array } buf2 - Buffer instance to compare.
   * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
   *         1 is returned if target should come before buf when sorted.
   *        -1 is returned if target should come after buf when sorted.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function compare(buf1: Buffer | Uint8Array, buf2: Buffer | Uint8Array): -1 | 0 | 1;

  /**
   * Re-encodes the given Buffer or Uint8Array instance from one character encoding to another.
   *
   * @param { Buffer | Uint8Array } source - source source A Buffer or Uint8Array instance.
   * @param { string } fromEnc - fromEnc fromEnc The current encoding
   * @param { string } toEnc - toEnc toEnc To target encoding
   * @returns { Buffer } Returns a new Buffer instance
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Re-encodes the given Buffer or Uint8Array instance from one character encoding to another.
   *
   * @param { Buffer | Uint8Array } source - source source A Buffer or Uint8Array instance.
   * @param { string } fromEnc - fromEnc fromEnc The current encoding
   * @param { string } toEnc - toEnc toEnc To target encoding
   * @returns { Buffer } Returns a new Buffer instance
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Transcodes the given Buffer or Uint8Array object from one encoding format to another.
   *
   * @param { Buffer | Uint8Array } source - Instance to encode.
   * @param { string } fromEnc - Current encoding format
   * @param { string } toEnc - Target encoding format.
   * @returns { Buffer } Returns a new Buffer instance
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   * 1.Mandatory parameters are left unspecified;
   * 2.Incorrect parameter types.
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  function transcode(source: Buffer | Uint8Array, fromEnc: string, toEnc: string): Buffer;


  /**
   * The Buffer object is a method of handling buffers dedicated to binary data.
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * The Buffer object is a method of handling buffers dedicated to binary data.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * The Buffer object is a method of handling buffers dedicated to binary data.
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class Buffer {
    /**
     * Returns the number of bytes in buf
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - Length  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns the number of bytes in buf
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - Length  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Returns the number of bytes in buf
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - Length  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    length: number;

    /**
     * The underlying ArrayBuffer object based on which this Buffer object is created.
     *
     * @type { ArrayBuffer }
     * @throws { BusinessError } 10200013 - Buffer cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The underlying ArrayBuffer object based on which this Buffer object is created.
     *
     * @type { ArrayBuffer }
     * @throws { BusinessError } 10200013 - Buffer cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The underlying ArrayBuffer object based on which this Buffer object is created.
     *
     * @type { ArrayBuffer }
     * @throws { BusinessError } 10200013 - Buffer cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    buffer: ArrayBuffer;

    /**
     * The byteOffset of the Buffers underlying ArrayBuffer object
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - ByteOffset  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The byteOffset of the Buffers underlying ArrayBuffer object
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - ByteOffset  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * The byteOffset of the Buffers underlying ArrayBuffer object
     *
     * @type { number }
     * @throws { BusinessError } 10200013 - ByteOffset  cannot be set for the buffer that has only a getter.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    byteOffset: number;

    /**
     * Fills buf with the specified value. If the offset and end are not given, the entire buf will be filled.
     *
     * @param { string | Buffer | Uint8Array | number } value - value value The value with which to fill buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to fill buf
     * @param { number } [end] - end [end = buf.length] Where to stop filling buf (not inclusive)
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] The encoding for value if value is a string
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200001 - The value of "[offset/end]" is out of range. It must be >= 0 and <= [right range]. Received value is: [offset/end]
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Incorrect parameter types;
     * 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Fills buf with the specified value. If the offset and end are not given, the entire buf will be filled.
     *
     * @param { string | Buffer | Uint8Array | number } value - value value The value with which to fill buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to fill buf
     * @param { number } [end] - end [end = buf.length] Where to stop filling buf (not inclusive)
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] The encoding for value if value is a string
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200001 - The value of "[offset/end]" is out of range. It must be >= 0 and <= [right range]. Received value is: [offset/end]
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Incorrect parameter types;
     * 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Fills this Buffer instance at the specified position. By default, data is filled cyclically.
     *
     * @param { string | Buffer | Uint8Array | number } value - Value to fill.
     * @param { number } [offset] - Offset to the start position in this Buffer instance where data is filled. The default value is 0.
     * @param { number } [end] - Offset to the end position in this Buffer instance (not inclusive). The default value is the length of this Buffer instance.
     * @param { BufferEncoding } [encoding] - Encoding format (valid only when value is a string). The default value is 'utf8'.
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200001 - The value of "[offset/end]" is out of range. It must be >= 0 and <= [right range]. Received value is: [offset/end]
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Incorrect parameter types;
     * 2.Parameter verification failed.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    fill(
      value: string | Buffer | Uint8Array | number,
      offset?: number,
      end?: number,
      encoding?: BufferEncoding
    ): Buffer;

    /**
     * Compares buf with target and returns a number indicating whether buf comes before, after,
     * or is the same as target in sort order. Comparison is based on the actual sequence of bytes in each Buffer.
     *
     * @param { Buffer | Uint8Array } target - target target A Buffer or Uint8Array with which to compare buf
     * @param { number } [targetStart] - targetStart [targetStart = 0] The offset within target at which to begin comparison
     * @param { number } [targetEnd] - targetEnd [targetEnd = target.length] The offset within target at which to end comparison (not inclusive)
     * @param { number } [sourceStart] - sourceStart [sourceStart = 0] The offset within buf at which to begin comparison
     * @param { number } [sourceEnd] - sourceEnd [sourceEnd = buf.length] The offset within buf at which to end comparison (not inclusive)
     * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
     *         1 is returned if target should come before buf when sorted.
     *        -1 is returned if target should come after buf when sorted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/targetEnd/sourceStart/sourceEnd]" is out of range.
     *         It must be >= 0 and <= [right range]. Received value is: [targetStart/targetEnd/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Compares buf with target and returns a number indicating whether buf comes before, after,
     * or is the same as target in sort order. Comparison is based on the actual sequence of bytes in each Buffer.
     *
     * @param { Buffer | Uint8Array } target - target target A Buffer or Uint8Array with which to compare buf
     * @param { number } [targetStart] - targetStart [targetStart = 0] The offset within target at which to begin comparison
     * @param { number } [targetEnd] - targetEnd [targetEnd = target.length] The offset within target at which to end comparison (not inclusive)
     * @param { number } [sourceStart] - sourceStart [sourceStart = 0] The offset within buf at which to begin comparison
     * @param { number } [sourceEnd] - sourceEnd [sourceEnd = buf.length] The offset within buf at which to end comparison (not inclusive)
     * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
     *         1 is returned if target should come before buf when sorted.
     *        -1 is returned if target should come after buf when sorted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/targetEnd/sourceStart/sourceEnd]" is out of range.
     *         It must be >= 0 and <= [right range]. Received value is: [targetStart/targetEnd/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Compares this Buffer instance with another instance.
     *
     * @param { Buffer | Uint8Array } target - Target Buffer instance to compare.
     * @param { number } [targetStart] - Offset to the start of the data to compare in the target Buffer instance. The default value is 0.
     * @param { number } [targetEnd] - Offset to the end of the data to compare in the target Buffer instance (not inclusive).
     * The default value is the length of the target Buffer instance.
     * @param { number } [sourceStart] - Offset to the start of the data to compare in this Buffer instance. The default value is 0.
     * @param { number } [sourceEnd] - Offset to the end of the data to compare in this Buffer instance (not inclusive).
     * The default value is the length of this Buffer instance.
     * @returns { -1 | 0 | 1 } 0 is returned if target is the same as buf
     *         1 is returned if target should come before buf when sorted.
     *        -1 is returned if target should come after buf when sorted.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/targetEnd/sourceStart/sourceEnd]" is out of range.
     *         It must be >= 0 and <= [right range]. Received value is: [targetStart/targetEnd/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    compare(
      target: Buffer | Uint8Array,
      targetStart?: number,
      targetEnd?: number,
      sourceStart?: number,
      sourceEnd?: number
    ): -1 | 0 | 1;

    /**
     * Copies data from a region of buf to a region in target, even if the target memory region overlaps with buf.
     * If sourceEnd is greater than the length of the target, the length of the target shall prevail, and the extra part will not be overwritten.
     *
     * @param { Buffer | Uint8Array } target - target target A Buffer or Uint8Array to copy into
     * @param { number } [targetStart] - targetStart [targetStart = 0] The offset within target at which to begin writing
     * @param { number } [sourceStart] - sourceStart [sourceStart = 0] The offset within buf from which to begin copying
     * @param { number } [sourceEnd] - sourceEnd [sourceEnd = buf.length] The offset within buf at which to stop copying (not inclusive)
     * @returns { number } The number of bytes copied
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/sourceStart/sourceEnd]" is out of range. It must be >= 0.
     *                                    Received value is: [targetStart/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Copies data from a region of buf to a region in target, even if the target memory region overlaps with buf.
     * If sourceEnd is greater than the length of the target, the length of the target shall prevail, and the extra part will not be overwritten.
     *
     * @param { Buffer | Uint8Array } target - target target A Buffer or Uint8Array to copy into
     * @param { number } [targetStart] - targetStart [targetStart = 0] The offset within target at which to begin writing
     * @param { number } [sourceStart] - sourceStart [sourceStart = 0] The offset within buf from which to begin copying
     * @param { number } [sourceEnd] - sourceEnd [sourceEnd = buf.length] The offset within buf at which to stop copying (not inclusive)
     * @returns { number } The number of bytes copied
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/sourceStart/sourceEnd]" is out of range. It must be >= 0.
     *                                    Received value is: [targetStart/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Copies data at the specified position in this Buffer instance to the specified position in another Buffer instance.
     *
     * @param { Buffer | Uint8Array } target - Instance to which data is copied.
     * @param { number } [targetStart] - Offset to the start position in the target instance where data is copied. The default value is 0.
     * @param { number } [sourceStart] - Offset to the start position in this Buffer instance where data is copied. The default value is 0.
     * @param { number } [sourceEnd] - Offset to the end position in this Buffer instance (not inclusive).
     * The default value is the length of this Buffer instance.
     * @returns { number } The number of bytes copied
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[targetStart/sourceStart/sourceEnd]" is out of range. It must be >= 0.
     *                                    Received value is: [targetStart/sourceStart/sourceEnd]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    copy(target: Buffer | Uint8Array, targetStart?: number, sourceStart?: number, sourceEnd?: number): number;

    /**
     * Returns true if both buf and otherBuffer have exactly the same bytes, false otherwise
     *
     * @param { Uint8Array | Buffer } otherBuffer - otherBuffer otherBuffer A Buffer or Uint8Array with which to compare buf
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns true if both buf and otherBuffer have exactly the same bytes, false otherwise
     *
     * @param { Uint8Array | Buffer } otherBuffer - otherBuffer otherBuffer A Buffer or Uint8Array with which to compare buf
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Checks whether this Buffer instance is the same as another Buffer instance.
     *
     * @param { Uint8Array | Buffer } otherBuffer - Buffer instance to compare.
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified; 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    equals(otherBuffer: Uint8Array | Buffer): boolean;

    /**
     * Returns true if value was found in buf, false otherwise
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf. If negative, then offset is calculated from the end of buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string, this is its encoding
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns true if value was found in buf, false otherwise
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf. If negative, then offset is calculated from the end of buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string, this is its encoding
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Checks whether this Buffer instance contains the specified value.
     *
     * @param { string | number | Buffer | Uint8Array } value - Value to match.
     * @param { number } [byteOffset] - Number of bytes to skip before starting to check data.
     * Number of bytes to skip before starting to check data. If the offset is a negative number,
     * data is checked from the end of the Buffer instance. The default value is 0.
     * @param { BufferEncoding } [encoding] - Encoding format (valid only when value is a string). The default value is 'utf8'.
     * @returns { boolean } true or false
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    includes(value: string | number | Buffer | Uint8Array, byteOffset?: number, encoding?: BufferEncoding): boolean;

    /**
     * The index of the first occurrence of value in buf
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string,
     * this is the encoding used to determine the binary representation of the string that will be searched for in buf
     * @returns { number } The index of the first occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The index of the first occurrence of value in buf
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string,
     * this is the encoding used to determine the binary representation of the string that will be searched for in buf
     * @returns { number } The index of the first occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains the index of the first occurrence of the specified value in this Buffer instance.
     *
     * @param { string | number | Buffer | Uint8Array } value - Value to match.
     * @param { number } [byteOffset] - Number of bytes to skip before starting to check data.
     * If the offset is a negative number, data is checked from the end of the Buffer instance. The default value is 0.
     * @param { BufferEncoding } [encoding] - Encoding format (valid only when value is a string). The default value is 'utf8'.
     * @returns { number } The index of the first occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    indexOf(value: string | number | Buffer | Uint8Array, byteOffset?: number, encoding?: BufferEncoding): number;

    /**
     * Creates and returns an iterator of buf keys (indices).
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Creates and returns an iterator of buf keys (indices).
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Creates and returns an iterator that contains the keys of this Buffer instance.
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    keys(): IterableIterator<number>;

    /**
     * Creates and returns an iterator for buf values (bytes).
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Creates and returns an iterator for buf values (bytes).
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Creates and returns an iterator that contains the values of this Buffer instance.
     *
     * @returns { IterableIterator<number> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    values(): IterableIterator<number>;

    /**
     * Creates and returns an iterator of [index, byte] pairs from the contents of buf.
     *
     * @returns { IterableIterator<[number, number]> }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Creates and returns an iterator of [index, byte] pairs from the contents of buf.
     *
     * @returns { IterableIterator<[number, number]> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Creates and returns an iterator that contains key-value pairs of this Buffer instance.
     *
     * @returns { IterableIterator<[number, number]> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    entries(): IterableIterator<[number, number]>;

    /**
     * The index of the last occurrence of value in buf
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string,
     * this is the encoding used to determine the binary representation of the string that will be searched for in buf
     * @returns { number } The index of the last occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The index of the last occurrence of value in buf
     *
     * @param { string | number | Buffer | Uint8Array } value - value value What to search for
     * @param { number } [byteOffset] - byteOffset [byteOffset = 0] Where to begin searching in buf
     * @param { BufferEncoding } [encoding] - encoding [encoding='utf8'] If value is a string,
     * this is the encoding used to determine the binary representation of the string that will be searched for in buf
     * @returns { number } The index of the last occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Obtains the index of the last occurrence of the specified value in this Buffer instance.
     *
     * @param { string | number | Buffer | Uint8Array } value - Value to match.
     * @param { number } [byteOffset] - Number of bytes to skip before starting to check data.
     * If the offset is a negative number, data is checked from the end of the Buffer instance.
     * The default value is the length of this Buffer instance.
     * @param { BufferEncoding } [encoding] - Encoding format (valid only when value is a string). The default value is 'utf8'.
     * @returns { number } The index of the last occurrence of value in buf, or -1 if buf does not contain value
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    lastIndexOf(value: string | number | Buffer | Uint8Array, byteOffset?: number, encoding?: BufferEncoding): number;

    /**
     * Reads a signed, big-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a signed, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, big-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a signed, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, big-endian, signed big integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { bigint } Return a signed, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readBigInt64BE(offset?: number): bigint;

    /**
     * Reads a signed, little-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a signed, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, little-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a signed, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, little-endian, signed big integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { bigint } Return a signed, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readBigInt64LE(offset?: number): bigint;

    /**
     * Reads a unsigned, big-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a unsigned, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a unsigned, big-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a unsigned, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, big-endian, unsigned big integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { bigint } Return a unsigned, big-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readBigUInt64BE(offset?: number): bigint;

    /**
     * Reads a unsigned, little-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a unsigned, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a unsigned, little-endian 64-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { bigint } Return a unsigned, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, little-endian, unsigned big integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { bigint } Return a unsigned, little-endian 64-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readBigUInt64LE(offset?: number): bigint;

    /**
     * Reads a 64-bit, big-endian double from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } Return a 64-bit, big-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a 64-bit, big-endian double from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } Return a 64-bit, big-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, big-endian, double-precision floating-point number from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } Return a 64-bit, big-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readDoubleBE(offset?: number): number;

    /**
     * Reads a 64-bit, little-endian double from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } Return a 64-bit, little-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a 64-bit, little-endian double from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } Return a 64-bit, little-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 64-bit, little-endian, double-precision floating-point number from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } Return a 64-bit, little-endian double
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readDoubleLE(offset?: number): number;

    /**
     * Reads a 32-bit, big-endian float from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a 32-bit, big-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a 32-bit, big-endian float from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a 32-bit, big-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, big-endian, single-precision floating-point number from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Return a 32-bit, big-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readFloatBE(offset?: number): number;

    /**
     * Reads a 32-bit, little-endian float from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a 32-bit, little-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a 32-bit, little-endian float from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a 32-bit, little-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, little-endian, single-precision floating-point number from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Return a 32-bit, little-endian float
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readFloatLE(offset?: number): number;

    /**
     * Reads a signed 8-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 1
     * @returns { number } Return a signed 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed 8-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 1
     * @returns { number } Return a signed 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads an 8-bit signed integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 1].
     * @returns { number } Return a signed 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readInt8(offset?: number): number;

    /**
     * Reads a signed, big-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } Return a signed, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, big-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } Return a signed, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 16-bit, big-endian, signed integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } Return a signed, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readInt16BE(offset?: number): number;

    /**
     * Reads a signed, little-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } Return a signed, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, little-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } Return a signed, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 16-bit, little-endian, signed integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } Return a signed, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readInt16LE(offset?: number): number;

    /**
     * Reads a signed, big-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a signed, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, big-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a signed, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, big-endian, signed integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Return a signed, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readInt32BE(offset?: number): number;

    /**
     * Reads a signed, little-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a signed, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads a signed, little-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } Return a signed, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, little-endian, signed integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Return a signed, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readInt32LE(offset?: number): number;

    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as a big-endian,
     * two's complement signed value supporting up to 48 bits of accuracy
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as a big-endian,
     * two's complement signed value supporting up to 48 bits of accuracy
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads the specified number of bytes from this Buffer instance at the specified offset, and interprets the result as a big-endian,
     * two's complement signed value that supports up to 48 bits of precision.
     *
     * @param { number } offset - Number of bytes to skip before starting to read data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to read. The value range is [1, 6].
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readIntBE(offset: number, byteLength: number): number;

    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as a little-endian,
     * two's complement signed value supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as a little-endian,
     * two's complement signed value supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads the specified number of bytes from this Buffer instance at the specified offset and interprets the result as a little-endian,
     * two's complement signed value that supports up to 48 bits of precision.
     *
     * @param { number } offset - Number of bytes to skip before starting to read data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to read. The value range is [1, 6].
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readIntLE(offset: number, byteLength: number): number;

    /**
     * Reads an unsigned 8-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 1
     * @returns { number } Reads an unsigned 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads an unsigned 8-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 1
     * @returns { number } Reads an unsigned 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads an 8-bit unsigned integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 1].
     * @returns { number } Reads an unsigned 8-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 1. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUInt8(offset?: number): number;

    /**
     * Reads an unsigned, big-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } Reads an unsigned, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads an unsigned, big-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } Reads an unsigned, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 16-bit, big-endian, unsigned integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } Reads an unsigned, big-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUInt16BE(offset?: number): number;

    /**
     * Reads an unsigned, little-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } Reads an unsigned, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads an unsigned, little-endian 16-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } Reads an unsigned, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 16-bit, little-endian, unsigned integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } Reads an unsigned, little-endian 16-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 2. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUInt16LE(offset?: number): number;

    /**
     * Reads an unsigned, big-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } Reads an unsigned, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads an unsigned, big-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } Reads an unsigned, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, big-endian, unsigned integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Reads an unsigned, big-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUInt32BE(offset?: number): number;

    /**
     * Reads an unsigned, little-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } Reads an unsigned, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads an unsigned, little-endian 32-bit integer from buf at the specified offset
     *
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to read. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } Reads an unsigned, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads a 32-bit, little-endian, unsigned integer from this Buffer instance at the specified offset.
     *
     * @param { number } [offset] - Number of bytes to skip before starting to read data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } Reads an unsigned, little-endian 32-bit integer
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUInt32LE(offset?: number): number;

    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as
     * an unsigned big-endian integer supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as
     * an unsigned big-endian integer supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads the specified number of bytes from this Buffer instance at the specified offset, and interprets the result as an unsigned,
     * big-endian integer that supports up to 48 bits of precision.
     *
     * @param { number } offset - Number of bytes to skip before starting to read data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to read. The value range is [1, 6].
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUIntBE(offset: number, byteLength: number): number;

    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as an unsigned,
     * little-endian integer supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Reads byteLength number of bytes from buf at the specified offset and interprets the result as an unsigned,
     * little-endian integer supporting up to 48 bits of accuracy.
     *
     * @param { number } offset - offset offset Number of bytes to skip before starting to read. Must satisfy: 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to read. Must satisfy 0 < byteLength <= 6
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Reads the specified number of bytes from this Buffer instance at the specified offset, and interprets the result as an unsigned,
     * little-endian integer that supports up to 48 bits of precision.
     *
     * @param { number } offset - Number of bytes to skip before starting to read data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to read. The value range is [1, 6].
     * @returns { number }
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    readUIntLE(offset: number, byteLength: number): number;

    /**
     * Returns a new Buffer that references the same memory as the original, but offset and cropped by the start and end indices.
     *
     * @param { number } [start] - start [start = 0] Where the new Buffer will start
     * @param { number } [end] - end [end = buf.length] Where the new Buffer will end (not inclusive)
     * @returns { Buffer } Returns a new Buffer that references the same memory as the original
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a new Buffer that references the same memory as the original, but offset and cropped by the start and end indices.
     *
     * @param { number } [start] - start [start = 0] Where the new Buffer will start
     * @param { number } [end] - end [end = buf.length] Where the new Buffer will end (not inclusive)
     * @returns { Buffer } Returns a new Buffer that references the same memory as the original
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Truncates this Buffer instance from the specified position to create a new Buffer instance.
     *
     * @param { number } [start] - Offset to the start position in this Buffer instance where data is truncated. The default value is 0.
     * @param { number } [end] - Offset to the end position in this Buffer instance (not inclusive). The default value is the length of this Buffer instance.
     * @returns { Buffer } Returns a new Buffer that references the same memory as the original
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    subarray(start?: number, end?: number): Buffer;

    /**
     * Interprets buf as an array of unsigned 16-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 16-bits
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Interprets buf as an array of unsigned 16-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 16-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Interprets this Buffer instance as an array of unsigned 16-bit integers and swaps the byte order in place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 16-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    swap16(): Buffer;

    /**
     * Interprets buf as an array of unsigned 32-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 32-bits
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Interprets buf as an array of unsigned 32-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 32-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Interprets this Buffer instance as an array of unsigned 32-bit integers and swaps the byte order in place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 32-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    swap32(): Buffer;

    /**
     * Interprets buf as an array of unsigned 64-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 64-bits
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Interprets buf as an array of unsigned 64-bit integers and swaps the byte order in-place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 64-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Interprets this Buffer instance as an array of unsigned 64-bit integers and swaps the byte order in place.
     *
     * @returns { Buffer } A reference to buf
     * @throws { BusinessError } 10200009 - The buffer size must be a multiple of 64-bits
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    swap64(): Buffer;

    /**
     * Returns a JSON representation of buf
     *
     * @returns { Object } Returns a JSON
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a JSON representation of buf
     *
     * @returns { Object } Returns a JSON
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Converts this Buffer instance into a JSON object.
     *
     * @returns { Object } Returns a JSON
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    toJSON(): Object;

    /**
     * Decodes buf to a string according to the specified character encoding in encoding
     *
     * @param { string } [encoding] - encoding [encoding='utf8'] The character encoding to use
     * @param { number } [start] - start [start = 0] The byte offset to start decoding at
     * @param { number } [end] - end [end = buf.length] The byte offset to stop decoding at (not inclusive)
     * @returns { string }
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Decodes buf to a string according to the specified character encoding in encoding
     *
     * @param { string } [encoding] - encoding [encoding='utf8'] The character encoding to use
     * @param { number } [start] - start [start = 0] The byte offset to start decoding at
     * @param { number } [end] - end [end = buf.length] The byte offset to stop decoding at (not inclusive)
     * @returns { string }
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Converts the data at the specified position in this Buffer instance into a string in the specified encoding format.
     *
     * @param { string } [encoding] - Encoding format (valid only when value is a string). The default value is 'utf8'.
     * @param { number } [start] - Offset to the start position of the data to convert. The default value is 0.
     * @param { number } [end] - Offset to the end position of the data to convert. The default value is the length of this Buffer instance.
     * @returns { string }
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    toString(encoding?: string, start?: number, end?: number): string;

    /**
     * Writes string to buf at offset according to the character encoding in encoding
     *
     * @param { string } str - str str Writes string to buf at offset according to the character encoding in encoding
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write string
     * @param { number } [length] - length [length = buf.length - offset] Maximum number of bytes to write (written bytes will not exceed buf.length - offset)
     * @param { string } [encoding] - encoding [encoding='utf8'] The character encoding of string.
     * @returns { number } Number of bytes written.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[offset/length]" is out of range. It must be >= 0 and <= buf.length. Received value is: [offset/length]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes string to buf at offset according to the character encoding in encoding
     *
     * @param { string } str - str str Writes string to buf at offset according to the character encoding in encoding
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write string
     * @param { number } [length] - length [length = buf.length - offset] Maximum number of bytes to write (written bytes will not exceed buf.length - offset)
     * @param { string } [encoding] - encoding [encoding='utf8'] The character encoding of string.
     * @returns { number } Number of bytes written.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[offset/length]" is out of range. It must be >= 0 and <= buf.length. Received value is: [offset/length]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a string of the specified length to this Buffer instance at the specified position in the given encoding format.
     *
     * @param { string } str - String to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0.
     * @param { number } [length] - Maximum number of bytes to write. The default value is Buffer.length minus offset.
     * @param { string } [encoding] - Encoding format of the string. The default value is 'utf8'.
     * @returns { number } Number of bytes written.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[offset/length]" is out of range. It must be >= 0 and <= buf.length. Received value is: [offset/length]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    write(str: string, offset?: number, length?: number, encoding?: string): number;

    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types; 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, big-endian, signed big integer to this Buffer instance at the specified offset.
     *
     * @param { bigint } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeBigInt64BE(value: bigint, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, little-endian, signed big integer to this Buffer instance at the specified offset.
     *
     * @param { bigint } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeBigInt64LE(value: bigint, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, big-endian, unsigned big integer to this Buffer instance at the specified offset.
     *
     * @param { bigint } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeBigUInt64BE(value: bigint, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { bigint } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, little-endian, unsigned big integer to this Buffer instance at the specified offset.
     *
     * @param { bigint } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeBigUInt64LE(value: bigint, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, big-endian, double-precision floating-point number to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeDoubleBE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 8
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 64-bit, little-endian, double-precision floating-point number to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 8].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 8. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeDoubleLE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, big-endian, single-precision floating-point number to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeFloatBE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0]  Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, little-endian, single-precision floating-point number to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "offset" is out of range. It must be >= 0 and <= buf.length - 4. Received value is: [offset]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeFloatLE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset. value must be a valid signed 8-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 1
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset. value must be a valid signed 8-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 1
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes an 8-bit signed integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 1].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeInt8(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid signed 16-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid signed 16-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 16-bit, big-endian, signed integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeInt16BE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid signed 16-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid signed 16-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 16-bit, little-endian, signed integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeInt16LE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid signed 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid signed 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, big-endian, signed integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeInt32BE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid signed 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid signed 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy: 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, little-endian, signed integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeInt32LE(value: number, offset?: number): number;

    /**
     * Writes byteLength bytes of value to buf at the specified offset as big-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes byteLength bytes of value to buf at the specified offset as big-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a big-endian signed value of the specified length to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } offset - Number of bytes to skip before starting to write data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to write.
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeIntBE(value: number, offset: number, byteLength: number): number;

    /**
     * Writes byteLength bytes of value to buf at the specified offset as little-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset  Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes byteLength bytes of value to buf at the specified offset as little-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset  Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a little-endian signed value of the specified length to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } offset - Number of bytes to skip before starting to write data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to write.
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeIntLE(value: number, offset: number, byteLength: number): number;

    /**
     * Writes value to buf at the specified offset. value must be a valid unsigned 8-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 1
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset. value must be a valid unsigned 8-bit integer
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 1
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes an 8-bit unsigned integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 1].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUInt8(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid unsigned 16-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid unsigned 16-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 16-bit, big-endian, unsigned integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - 	Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUInt16BE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid unsigned 16-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid unsigned 16-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 2
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 16-bit, little-endian, unsigned integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 2].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUInt16LE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid unsigned 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as big-endian. The value must be a valid unsigned 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, big-endian, unsigned integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUInt32BE(value: number, offset?: number): number;

    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid unsigned 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes value to buf at the specified offset as little-endian. The value must be a valid unsigned 32-bit integer.
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } [offset] - offset [offset = 0] Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - 4
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes a 32-bit, little-endian, unsigned integer to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } [offset] - Number of bytes to skip before starting to write data. The default value is 0. The value range is [0, Buffer.length - 4].
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types;
     * 3.Parameter verification failed.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUInt32LE(value: number, offset?: number): number;

    /**
     * Writes byteLength bytes of value to buf at the specified offset as big-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes byteLength bytes of value to buf at the specified offset as big-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes an unsigned big-endian value of the specified length to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } offset - Number of bytes to skip before starting to write data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to write.
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUIntBE(value: number, offset: number, byteLength: number): number;

    /**
     * Writes byteLength bytes of value to buf at the specified offset as little-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Writes byteLength bytes of value to buf at the specified offset as little-endian
     *
     * @param { number } value - value value Number to be written to buf
     * @param { number } offset - offset offset Number of bytes to skip before starting to write. Must satisfy 0 <= offset <= buf.length - byteLength
     * @param { number } byteLength - byteLength byteLength Number of bytes to write. Must satisfy 0 < byteLength <= 6
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Writes an unsigned little-endian value of the specified length to this Buffer instance at the specified offset.
     *
     * @param { number } value - Data to write.
     * @param { number } offset - Number of bytes to skip before starting to write data. The default value is 0.
     * The value range is [0, Buffer.length - byteLength].
     * @param { number } byteLength - Number of bytes to write.
     * @returns { number } offset plus the number of bytes written
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @throws { BusinessError } 10200001 - The value of "[param]" is out of range. It must be >= [left range] and <= [right range]. Received value is: [param]
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    writeUIntLE(value: number, offset: number, byteLength: number): number;
  }

  /**
   * Process data as blob type
   *
   * @syscap SystemCapability.Utils.Lang
   * @since 9
   */
  /**
   * Process data as blob type
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @since 10
   */
  /**
   * Process data as blob type
   *
   * @syscap SystemCapability.Utils.Lang
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  class Blob {
    /**
     * Creates a new Blob object containing a concatenation of the given sources.
     *
     * @param { string[] | ArrayBuffer[] | TypedArray[] | DataView[] | Blob[] } sources - sources sources An array of string, <ArrayBuffer>,
     * <TypedArray>, <DataView>, or <Blob> objects, or any mix of such objects, that will be stored within the Blob
     * @param { Object } [options] - options options {endings: string, type: string}
     *                 endings:  One of either 'transparent' or 'native'.
     *                 type: The Blob content-type
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Creates a new Blob object containing a concatenation of the given sources.
     *
     * @param { string[] | ArrayBuffer[] | TypedArray[] | DataView[] | Blob[] } sources - sources sources An array of string, <ArrayBuffer>,
     * <TypedArray>, <DataView>, or <Blob> objects, or any mix of such objects, that will be stored within the Blob
     * @param { Object } [options] - options options {endings: string, type: string}
     *                 endings:  One of either 'transparent' or 'native'.
     *                 type: The Blob content-type
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * A constructor used to create a Blob instance.
     *
     * @param { string[] | ArrayBuffer[] | TypedArray[] | DataView[] | Blob[] } sources - Data sources of the Blob instance.
     * @param { Object } [options] - options {endings: string, type: string}
     * - endings: specifies how the terminator '\n' is output. The value can be 'native' or 'transparent'. 'native'
     * means that the terminator follows the system. 'transparent' means that the terminator stored in the Blob
     * instance remains unchanged. The default value is 'transparent'.
     * - type: type of the data in the Blob instance. This type represents the MIME type of the data. However,
     * it is not used for type format validation. The default value is ''.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     * 1.Mandatory parameters are left unspecified;
     * 2.Incorrect parameter types.
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    constructor(sources: string[] | ArrayBuffer[] | TypedArray[] | DataView[] | Blob[], options?: Object);

    /**
     * The total size of the Blob in bytes
     *
     * @type { number }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The total size of the Blob in bytes
     *
     * @type { number }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Total size of the Blob instance, in bytes.
     *
     * @type { number }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    size: number;

    /**
     * The content-type of the Blob
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * The content-type of the Blob
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Type of the data in the Blob instance.
     *
     * @type { string }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    type: string;

    /**
     * Returns a promise that fulfills with an <ArrayBuffer> containing a copy of the Blob data.
     *
     * @returns { Promise<ArrayBuffer> }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a promise that fulfills with an <ArrayBuffer> containing a copy of the Blob data.
     *
     * @returns { Promise<ArrayBuffer> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Puts the Blob data into an ArrayBuffer instance. This API uses a promise to return the result.
     *
     * @returns { Promise<ArrayBuffer> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    arrayBuffer(): Promise<ArrayBuffer>;

    /**
     * Creates and returns a new Blob containing a subset of this Blob objects data. The original Blob is not altered
     *
     * @param { number } [start] - start start The starting index
     * @param { number } [end] - end end The ending index
     * @param { string } [type] - type type The content-type for the new Blob
     * @returns { Blob }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Creates and returns a new Blob containing a subset of this Blob objects data. The original Blob is not altered
     *
     * @param { number } [start] - start start The starting index
     * @param { number } [end] - end end The ending index
     * @param { string } [type] - type type The content-type for the new Blob
     * @returns { Blob }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Creates a Blob instance by copying specified data from this Blob instance.
     *
     * @param { number } [start] - Offset to the start position of the data to copy. The default value is 0.
     * @param { number } [end] - Offset to the end position of the data to copy. The default value is the data length in the original Blob instance.
     * @param { string } [type] - Type of the data in the new Blob instance. The default value is ''.
     * @returns { Blob }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    slice(start?: number, end?: number, type?: string): Blob;

    /**
     * Returns a promise that fulfills with the contents of the Blob decoded as a UTF-8 string.
     *
     * @returns { Promise<string> }
     * @syscap SystemCapability.Utils.Lang
     * @since 9
     */
    /**
     * Returns a promise that fulfills with the contents of the Blob decoded as a UTF-8 string.
     *
     * @returns { Promise<string> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @since 10
     */
    /**
     * Returns text in UTF-8 format. This API uses a promise to return the result.
     *
     * @returns { Promise<string> }
     * @syscap SystemCapability.Utils.Lang
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    text(): Promise<string>;
  }
}
export default buffer;
