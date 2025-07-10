/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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
 * @kit UniversalKeystoreKit
 */

import type { AsyncCallback } from './@ohos.base';

/**
 * OpenHarmony Universal KeyStore
 *
 * @namespace huks
 * @syscap SystemCapability.Security.Huks.Core
 * @since 8
 */
/**
 * OpenHarmony Universal KeyStore
 *
 * @namespace huks
 * @syscap SystemCapability.Security.Huks.Core
 * @atomicservice
 * @since 11
 */
declare namespace huks {
  /**
   * Generate Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksResult> } callback - the callback of generateKey.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.generateKeyItem
   */
  function generateKey(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Generate Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.generateKeyItem
   */
  function generateKey(keyAlias: string, options: HuksOptions): Promise<HuksResult>;

  /**
   * Generates a key. This API uses an asynchronous callback to return the result.
   * 
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for generating the key. The algorithm, key purpose,
   * and key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * this API does not return the key content because the key is always protected in a TEE. If an exception occurs in
   * the generation process, an error is captured.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Generates a key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for generating the key. The algorithm, key purpose,
   * and key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * this API does not return the key content because the key is always protected in a TEE. If an exception occurs in
   * the generation process, an error is captured.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function generateKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<void>): void;

  /**
   * Generates a key. This API uses a promise to return the result. Because the key is always
   * protected in a trusted environment (such as a TEE), the promise does not return the key content.
   * It returns only the information indicating whether the API is successfully called.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for generating the key. The algorithm, key purpose,
   * and key length are mandatory.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Generates a key. This API uses a promise to return the result. Because the key is always
   * protected in a trusted environment (such as a TEE), the promise does not return the key content.
   * It returns only the information indicating whether the API is successfully called.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for generating the key. The algorithm, key purpose,
   * and key length are mandatory.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function generateKeyItem(keyAlias: string, options: HuksOptions): Promise<void>;

  /**
   * Generate Key As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function generateKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<void>;

  /**
   * Delete Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksResult> } callback - the callback of deleteKey.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.deleteKeyItem
   */
  function deleteKey(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Delete Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.deleteKeyItem
   */
  function deleteKey(keyAlias: string, options: HuksOptions): Promise<HuksResult>;

  /**
   * Deletes a key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the key to delete. It must be the key alias passed in when the key
   * was generated.
   * @param { HuksOptions } options - Properties of the key to delete. For example, you can pass in HuksAuthStorageLevel
   * to specify the security level of the key to delete. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Deletes a key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the key to delete. It must be the key alias passed in when the key
   * was generated.
   * @param { HuksOptions } options - Properties of the key to delete. For example, you can pass in HuksAuthStorageLevel
   * to specify the security level of the key to delete. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function deleteKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<void>): void;

  /**
   * Deletes a key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the key to delete. It must be the key alias passed in when the key
   * was generated.
   * @param { HuksOptions } options - Options for deleting the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to delete. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Deletes a key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the key to delete. It must be the key alias passed in when the key
   * was generated.
   * @param { HuksOptions } options - Options for deleting the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to delete. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function deleteKeyItem(keyAlias: string, options: HuksOptions): Promise<void>;

  /**
   * Delete Key As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function deleteKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<void>;

  /**
   * Import Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksResult> } callback - the callback of importKey.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.importKeyItem
   */
  function importKey(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Import Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.importKeyItem
   */
  function importKey(keyAlias: string, options: HuksOptions): Promise<HuksResult>;

  /**
   * Imports a key in plaintext. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for the import and key to import. The algorithm, key purpose, and
   * key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful, no
   * error value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Imports a key in plaintext. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for the import and key to import. The algorithm, key purpose, and
   * key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful, no
   * error value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  /**
   * Imports a key in plaintext. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for the import and key to import. The algorithm, key purpose, and
   * key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful, no
   * error value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  function importKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<void>): void;

  /**
   * Imports a key in plaintext. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for the import and key to import. The algorithm, key purpose, and
   * key length are mandatory.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Imports a key in plaintext. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - Tags required for the import and key to import. The algorithm, key purpose, and
   * key length are mandatory.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function importKeyItem(keyAlias: string, options: HuksOptions): Promise<void>;

  /**
   * Import Key As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function importKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<void>;

  /**
   * Imports a wrapped key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the wrapped key to import.
   * @param { string } wrappingKeyAlias - Alias of the data used to unwrap the key imported.
   * @param { HuksOptions } options - Tags required for the import and the wrapped key to import.
   * The algorithm, key purpose, and key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Imports a wrapped key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the wrapped key to import.
   * @param { string } wrappingKeyAlias - Alias of the data used to unwrap the key imported.
   * @param { HuksOptions } options - Tags required for the import and the wrapped key to import.
   * The algorithm, key purpose, and key length are mandatory.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. If the operation is successful,
   * no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  function importWrappedKeyItem(
    keyAlias: string,
    wrappingKeyAlias: string,
    options: HuksOptions,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Import Wrapped Key As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the name of key to be imported.
   * @param { string } wrappingKeyAlias - wrappingKeyAlias indicates the name of key for wrapping the key to be imported.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<void> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function importWrappedKeyItemAsUser(userId: number, keyAlias: string, wrappingKeyAlias: string, huksOptions: HuksOptions): Promise<void>;

  /**
   * Imports a wrapped key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the wrapped key to import.
   * @param { string } wrappingKeyAlias - Alias of the data used to unwrap the key imported.
   * @param { HuksOptions } options - Tags required for the import and the wrapped key to import. The algorithm, key
   * purpose, and key length are mandatory.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Imports a wrapped key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the wrapped key to import.
   * @param { string } wrappingKeyAlias - Alias of the data used to unwrap the key imported.
   * @param { HuksOptions } options - Tags required for the import and the wrapped key to import. The algorithm, key
   * purpose, and key length are mandatory.
   * @returns { Promise<void> } Promise that returns no value.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000013 - queried credential does not exist
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function importWrappedKeyItem(keyAlias: string, wrappingKeyAlias: string, options: HuksOptions): Promise<void>;

  /**
   * Export Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksResult> } callback - the callback of exportKey.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.exportKeyItem
   */
  function exportKey(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Export Key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.exportKeyItem
   */
  function exportKey(keyAlias: string, options: HuksOptions): Promise<HuksResult>;

  /**
   * Exports a key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned and outData contains the public key exported. Otherwise,
   * an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Exports a key. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned and outData contains the public key exported. Otherwise,
   * an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  function exportKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksReturnResult>): void;

  /**
   * Export Key As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function exportKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Exports a key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful, outData
   * in HuksReturnResult is the public key exported.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Exports a key. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful, outData
   * in HuksReturnResult is the public key exported.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function exportKeyItem(keyAlias: string, options: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Get properties of the key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksResult> } callback - the callback of getKeyProperties.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.getKeyItemProperties
   */
  function getKeyProperties(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Get properties of the key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.getKeyItemProperties
   */
  function getKeyProperties(keyAlias: string, options: HuksOptions): Promise<HuksResult>;

  /**
   * Obtains key properties. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned and properties contains the parameters required for generating the key. If the
   * operation fails, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Obtains key properties. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned and properties contains the parameters required for generating the key. If the
   * operation fails, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  function getKeyItemProperties(
    keyAlias: string,
    options: HuksOptions,
    callback: AsyncCallback<HuksReturnResult>
  ): void;

  /**
   * Get properties of the key as user.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function getKeyItemPropertiesAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Obtains key properties. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful,
   * properties in HuksReturnResult holds the parameters required for generating the key.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Obtains key properties. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Key alias, which must be the same as the alias used when the key was generated.
   * @param { HuksOptions } options - Empty object (leave this parameter empty).
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful,
   * properties in HuksReturnResult holds the parameters required for generating the key.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function getKeyItemProperties(keyAlias: string, options: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Check whether the key exists.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<boolean> } callback - the callback of isKeyExist.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.isKeyItemExist
   */
  function isKeyExist(keyAlias: string, options: HuksOptions, callback: AsyncCallback<boolean>): void;

  /**
   * Check whether the key exists.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<boolean> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.isKeyItemExist
   */
  function isKeyExist(keyAlias: string, options: HuksOptions): Promise<boolean>;

  /**
   * Checks whether a key exists. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the key to check.
   * @param { HuksOptions } options - Options for checking the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to check. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. If the key exists, data is true.
   * If the key does not exist, error is the error code.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  function isKeyItemExist(keyAlias: string, options: HuksOptions, callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether a key exists. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the key to check.
   * @param { HuksOptions } options - Options for checking the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to check. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @returns { Promise<boolean> } Promise used to return the result. If the key exists, then() performs subsequent
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  function isKeyItemExist(keyAlias: string, options: HuksOptions): Promise<boolean>;

  /**
   * Checks whether a key exists. This API uses an asynchronous callback to return the result.
   *
   * @param { string } keyAlias - Alias of the key to check.
   * @param { HuksOptions } options - Options for checking the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to check. HuksAuthStorageLevel can be left empty, which means the default
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. If the key exists, data is true.
   * Otherwise, data is false.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function hasKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<boolean>): void;

  /**
   * Check whether the key exists as user.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<boolean> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function hasKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<boolean>;

  /**
   * Checks whether a key exists. This API uses a promise to return the result.
   *
   * @param { string } keyAlias - Alias of the key to check.
   * @param { HuksOptions } options - Options for checking the key. For example, you can pass in HuksAuthStorageLevel to
   * specify the security level of the key to check. HuksAuthStorageLevel can be left empty, which means the default 
   * value HUKS_AUTH_STORAGE_LEVEL_DE is used.
   * @returns { Promise<boolean> } Promise used to return the result. If the key exists, true is returned. If the key
   * does not exist, false is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function hasKeyItem(keyAlias: string, options: HuksOptions): Promise<boolean>;

  /**
   * Init Operation.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @param { AsyncCallback<HuksHandle> } callback - the callback of init, include the handle.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.initSession
   */
  function init(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksHandle>): void;

  /**
   * Init Operation.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } options - options indicates the properties of the key.
   * @returns { Promise<HuksHandle> } the promise returned by the function, include the handle.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.initSession
   */
  function init(keyAlias: string, options: HuksOptions): Promise<HuksHandle>;

  /**
   * Initializes a session for a key operation. This API uses an asynchronous callback to return the
   * result. huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { string } keyAlias - Alias of the key involved in the initSession operation.
   * @param { HuksOptions } options - Parameter set used for the initSession operation.
   * @param { AsyncCallback<HuksSessionHandle> } callback - Callback used to return a session handle for subsequent
   * operations.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000010 - the number of sessions has reached limit
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Initializes a session for a key operation. This API uses an asynchronous callback to return the
   * result. huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { string } keyAlias - Alias of the key involved in the initSession operation.
   * @param { HuksOptions } options - Parameter set used for the initSession operation.
   * @param { AsyncCallback<HuksSessionHandle> } callback - Callback used to return a session handle for subsequent
   * operations.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000010 - the number of sessions has reached limit
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function initSession(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksSessionHandle>): void;

  /**
   * Initializes a session for a key operation. This API uses a promise to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { string } keyAlias - Alias of the key involved in the initSession operation.
   * @param { HuksOptions } options - Parameter set used for the initSession operation.
   * @returns { Promise<HuksSessionHandle> } Promise used to return a session handle for subsequent operations.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000010 - the number of sessions has reached limit
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Initializes a session for a key operation. This API uses a promise to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { string } keyAlias - Alias of the key involved in the initSession operation.
   * @param { HuksOptions } options - Parameter set used for the initSession operation.
   * @returns { Promise<HuksSessionHandle> } Promise used to return a session handle for subsequent operations.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000010 - the number of sessions has reached limit
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function initSession(keyAlias: string, options: HuksOptions): Promise<HuksSessionHandle>;

  /**
   * Init Operation As User.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key.
   * @returns { Promise<HuksSessionHandle> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000010 - the number of sessions has reached limit
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function initSessionAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<HuksSessionHandle>;

  /**
   * Update Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { Uint8Array } token - token indicates the value of token.
   * @param { HuksOptions } options - options indicates the properties of the update operation.
   * @param { AsyncCallback<HuksResult> } callback - the callback of update.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.updateSession
   */
  function update(handle: number, token?: Uint8Array, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Update Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { Uint8Array } token - indicates the value of token.
   * @param { HuksOptions } options - options indicates the properties of the update operation.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.updateSession
   */
  function update(handle: number, token?: Uint8Array, options: HuksOptions): Promise<HuksResult>;

  /**
   * Updates the key operation by segment. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Updates the key operation by segment. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function updateSession(handle: number, options: HuksOptions, callback: AsyncCallback<HuksReturnResult>): void;

  /**
   * Updates the key operation by segment. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Updates the key operation by segment. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function updateSession(
    handle: number,
    options: HuksOptions,
    token: Uint8Array,
    callback: AsyncCallback<HuksReturnResult>
  ): void;

  /**
   * Updates the key operation by segment. This API uses a promise to return the result. huks.initSession,
   * huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control. If this parameter is left blank,
   * refined key access control is not performed.
   * @returns { Promise<HuksReturnResult> } Promise used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Updates the key operation by segment. This API uses a promise to return the result. huks.initSession,
   * huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the updateSession operation.
   * @param { HuksOptions } options - Parameter set used for the updateSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control. If this parameter is left blank,
   * refined key access control is not performed.
   * @returns { Promise<HuksReturnResult> } Promise used to return the updateSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function updateSession(handle: number, options: HuksOptions, token?: Uint8Array): Promise<HuksReturnResult>;

  /**
   * Finish Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { HuksOptions } options - options indicates the properties of the finish operation.
   * @param { AsyncCallback<HuksResult> } callback - the callback of finish.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.finishSession
   */
  function finish(handle: number, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Finish Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { HuksOptions } options - options indicates the properties of the finish operation.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.finishSession
   */
  function finish(handle: number, options: HuksOptions): Promise<HuksResult>;

  /**
   * Finishes the key operation. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the finishSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Finishes the key operation. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the finishSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function finishSession(handle: number, options: HuksOptions, callback: AsyncCallback<HuksReturnResult>): void;

  /**
   * Finishes the key operation. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the finishSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Finishes the key operation. This API uses an asynchronous callback to return the result.
   * huks.initSession, huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the finishSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function finishSession(
    handle: number,
    options: HuksOptions,
    token: Uint8Array,
    callback: AsyncCallback<HuksReturnResult>
  ): void;

  /**
   * Finishes the key operation. This API uses a promise to return the result. huks.initSession,
   * huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control. If this parameter is left blank,
   * refined key access control is not performed.
   * @returns { Promise<HuksReturnResult> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Finishes the key operation. This API uses a promise to return the result. huks.initSession,
   * huks.updateSession, and huks.finishSession must be used together.
   *
   * @param { number } handle - Handle for the finishSession operation.
   * @param { HuksOptions } options - Parameter set used for the finishSession operation.
   * @param { Uint8Array } token - Authentication token for refined key access control. If this parameter is left blank,
   * refined key access control is not performed.
   * @returns { Promise<HuksReturnResult> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function finishSession(handle: number, options: HuksOptions, token?: Uint8Array): Promise<HuksReturnResult>;

  /**
   * Abort Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { HuksOptions } options - options indicates the properties of the abort operation.
   * @param { AsyncCallback<HuksResult> } callback - the callback of finishSession.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.abortSession
   */
  function abort(handle: number, options: HuksOptions, callback: AsyncCallback<HuksResult>): void;

  /**
   * Abort Operation.
   *
   * @param { number } handle - indicates the handle of the init operation.
   * @param { HuksOptions } options - options indicates the properties of the abort operation.
   * @returns { Promise<HuksResult> } the promise returned by the function.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.abortSession
   */
  function abort(handle: number, options: HuksOptions): Promise<HuksResult>;

  /**
   * Aborts a key operation. This API uses an asynchronous callback to return the result.
   *
   * @param { number } handle - Handle for the abortSession operation.
   * @param { HuksOptions } options - Parameter set used for the abortSession operation.
   * @param { AsyncCallback<void> } callback - Callback used to return the abortSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Aborts a key operation. This API uses an asynchronous callback to return the result.
   *
   * @param { number } handle - Handle for the abortSession operation.
   * @param { HuksOptions } options - Parameter set used for the abortSession operation.
   * @param { AsyncCallback<void> } callback - Callback used to return the abortSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  function abortSession(handle: number, options: HuksOptions, callback: AsyncCallback<void>): void;

  /**
   * Aborts a key operation. This API uses a promise to return the result.
   *
   * @param { number } handle - Handle for the abortSession operation.
   * @param { HuksOptions } options - Parameter set used for the abortSession operation.
   * @returns { Promise<void> } Promise used to return the abortSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Aborts a key operation. This API uses a promise to return the result.
   *
   * @param { number } handle - Handle for the abortSession operation.
   * @param { HuksOptions } options - Parameter set used for the abortSession operation.
   * @returns { Promise<void> } Promise used to return the abortSession operation result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 11
   */
  function abortSession(handle: number, options: HuksOptions): Promise<void>;

  /**
   * Obtains the certificate used to attest a key. This API uses an asynchronous callback to return the result.
   *
   * @permission ohos.permission.ATTEST_KEY
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 201 - check permission failed
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  function attestKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksReturnResult>): void;

  /**
   * Key Attestation As User.
   *
   * @permission ohos.permission.ATTEST_KEY and ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key attestation operation.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.ATTEST_KEY or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function attestKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Obtains the certificate used to attest a key. This API uses a promise to return the result.
   *
   * @permission ohos.permission.ATTEST_KEY
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful,
   * certChains in HuksReturnResult is the certificate chain obtained.
   * @throws { BusinessError } 201 - check permission failed
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  function attestKeyItem(keyAlias: string, options: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Obtains the certificate for anonymous attestation. This API uses an asynchronous callback to return
   * the result. This operation requires Internet access and takes time. If error code 12000012 is returned, the network
   * is abnormal. If the device is not connected to the network, display a message, indicating that the network is not
   * connected. If the network is connected, the failure may be caused by network jitter. Try again later.
   *
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 11
   */
  /**
   * Obtains the certificate for anonymous attestation. This API uses an asynchronous callback to return
   * the result. This operation requires Internet access and takes time. If error code 12000012 is returned, the network
   * is abnormal. If the device is not connected to the network, display a message, indicating that the network is not
   * connected. If the network is connected, the failure may be caused by network jitter. Try again later.
   *
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @param { AsyncCallback<HuksReturnResult> } callback - Callback used to return the result. If the operation is
   * successful, no err value is returned; otherwise, an error code is returned.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function anonAttestKeyItem(keyAlias: string, options: HuksOptions, callback: AsyncCallback<HuksReturnResult>): void;

  /**
   * Key Attestation with anonymous certificate as user.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
   * @param { number } userId - userId indicates the userId of the owner of the key.
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } huksOptions - huksOptions indicates the properties of the key attestation operation.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied. An attempt was made to use key as user forbidden by permission:
   * <br>ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS.
   * @throws { BusinessError } 202 - not system app
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @systemapi this method can be used only by system applications.
   * @since 12
   */
  function anonAttestKeyItemAsUser(userId: number, keyAlias: string, huksOptions: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Obtains the certificate for anonymous attestation. This API uses a promise to return the result. This
   * operation requires Internet access and takes time. If error code 12000012 is returned, the network is abnormal. If
   * the device is not connected to the network, display a message, indicating that the network is not connected. If the
   * network is connected, the failure may be caused by network jitter. Try again later.
   *
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful,
   * certChains in HuksReturnResult is the certificate chain obtained.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 11
   */
  /**
   * Obtains the certificate for anonymous attestation. This API uses a promise to return the result. This
   * operation requires Internet access and takes time. If error code 12000012 is returned, the network is abnormal. If
   * the device is not connected to the network, display a message, indicating that the network is not connected. If the
   * network is connected, the failure may be caused by network jitter. Try again later.
   *
   * @param { string } keyAlias - Alias of the key. The certificate to be obtained stores the key.
   * @param { HuksOptions } options - Parameters and data required for obtaining the certificate.
   * @returns { Promise<HuksReturnResult> } Promise used to return the result. If the operation is successful,
   * certChains in HuksReturnResult is the certificate chain obtained.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000001 - algorithm mode is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000011 - queried entity does not exist
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function anonAttestKeyItem(keyAlias: string, options: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Obtains the SDK version of the current system.
   *
   * @param { HuksOptions } options - Empty object, which is used to hold the SDK version.
   * @returns { string } SDK version obtained.
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 11
   */
  function getSdkVersion(options: HuksOptions): string;

  /**
   * Lists key aliases. This API uses a promise to return the result.
   *
   * @param { HuksOptions } options - Parameters for listing key aliases.
   * @returns { Promise<HuksListAliasesReturnResult> } Promise used to return the key aliases obtained.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                 1. Mandatory parameters are left unspecified.
   *                                 2. Incorrect parameter types.
   *                                 3. Parameter verification failed.
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  function listAliases(options: HuksOptions): Promise<HuksListAliasesReturnResult>;

  /**
   * Export the wrapped key protected by a specific key.
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } params - params indicates the export properties.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000018 - the input parameter is invalid
   * @syscap SystemCapability.Security.Huks.Core
   * @since 20
   */
  function wrapKeyItem(keyAlias: string, params: HuksOptions): Promise<HuksReturnResult>;

  /**
   * Import the wrapped key protected by a specific key
   *
   * @param { string } keyAlias - keyAlias indicates the key's name.
   * @param { HuksOptions } params - params indicates the import properties.
   * @param { Uint8Array } wrappedKey -indicates the wrapped key.
   * @returns { Promise<HuksReturnResult> } the promise returned by the function.
   * @throws { BusinessError } 801 - api is not supported
   * @throws { BusinessError } 12000002 - algorithm param is missing
   * @throws { BusinessError } 12000003 - algorithm param is invalid
   * @throws { BusinessError } 12000004 - operating file failed
   * @throws { BusinessError } 12000005 - IPC communication failed
   * @throws { BusinessError } 12000006 - error occurred in crypto engine
   * @throws { BusinessError } 12000007 - this credential is already invalidated permanently
   * @throws { BusinessError } 12000008 - verify auth token failed
   * @throws { BusinessError } 12000009 - auth token is already timeout
   * @throws { BusinessError } 12000012 - Device environment or input parameter abnormal
   * @throws { BusinessError } 12000014 - memory is insufficient
   * @throws { BusinessError } 12000015 - Failed to obtain the security information via UserIAM
   * @throws { BusinessError } 12000018 - the input parameter is invalid
   * @syscap SystemCapability.Security.Huks.Core
   * @since 20
   */
  function unwrapKeyItem(keyAlias: string, params: HuksOptions, wrappedKey: Uint8Array): Promise<HuksReturnResult>;

  /**
   * Defines the param field in the properties array of options used in the APIs.
   *
   * @typedef HuksParam
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Defines the param field in the properties array of options used in the APIs.
   *
   * @typedef HuksParam
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export interface HuksParam {
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * @type { HuksTag }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    tag: HuksTag;
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * @type { boolean | number | bigint | Uint8Array }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    value: boolean | number | bigint | Uint8Array;
  }

  /**
   * Interface of huks handle.
   *
   * @typedef HuksHandle
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.HuksSessionHandle
   */
  export interface HuksHandle {
    /**
     * @type { number }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    errorCode: number;
    /**
     * @type { number }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    handle: number;
    /**
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    token?: Uint8Array;
  }

  /**
   * Defines the struct for a HUKS handle.
   *
   * @typedef HuksSessionHandle
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Defines the struct for a HUKS handle.
   *
   * @typedef HuksSessionHandle
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export interface HuksSessionHandle {
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * @type { number }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    handle: number;
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    challenge?: Uint8Array;
  }

  /**
   * Defines options used in the APIs.
   *
   * @typedef HuksOptions
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Defines options used in the APIs.
   *
   * @typedef HuksOptions
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export interface HuksOptions {
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * @type { ?Array<HuksParam> }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    properties?: Array<HuksParam>;
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    inData?: Uint8Array;
  }

  /**
   * Interface of huks result.
   *
   * @typedef HuksResult
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.HuksReturnResult
   */
  export interface HuksResult {
    /**
     * @type { number }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    errorCode: number;
    /**
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    outData?: Uint8Array;
    /**
     * @type { ?Array<HuksParam> }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    properties?: Array<HuksParam>;
    /**
     * @type { ?Array<string> }
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    certChains?: Array<string>;
  }

  /**
   * Represents the result returned.
   *
   * @typedef HuksReturnResult
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Represents the result returned.
   *
   * @typedef HuksReturnResult
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export interface HuksReturnResult {
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    outData?: Uint8Array;
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * @type { ?Array<HuksParam> }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    properties?: Array<HuksParam>;
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * @type { ?Array<string> }
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    certChains?: Array<string>;
  }

    /**
   * Represents an array of key aliases.
   *
   * @typedef HuksListAliasesReturnResult
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export interface HuksListAliasesReturnResult {

    /**
     * the returned list of key aliases
     *
     * @type { Array<string> }
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    keyAliases: Array<string>;
  }

  /**
   * Enum for huks error code.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.security.huks.HuksExceptionErrCode
   */
  export enum HuksErrorCode {
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_SUCCESS = 0,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_FAILURE = -1,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_BAD_STATE = -2,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_ARGUMENT = -3,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_NOT_SUPPORTED = -4,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_NO_PERMISSION = -5,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INSUFFICIENT_DATA = -6,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_BUFFER_TOO_SMALL = -7,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INSUFFICIENT_MEMORY = -8,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_COMMUNICATION_FAILURE = -9,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_STORAGE_FAILURE = -10,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_HARDWARE_FAILURE = -11,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_ALREADY_EXISTS = -12,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_NOT_EXIST = -13,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_NULL_POINTER = -14,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_FILE_SIZE_FAIL = -15,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_READ_FILE_FAIL = -16,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_PUBLIC_KEY = -17,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_PRIVATE_KEY = -18,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_KEY_INFO = -19,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_HASH_NOT_EQUAL = -20,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_MALLOC_FAIL = -21,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_WRITE_FILE_FAIL = -22,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_REMOVE_FILE_FAIL = -23,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_OPEN_FILE_FAIL = -24,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CLOSE_FILE_FAIL = -25,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_MAKE_DIR_FAIL = -26,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_KEY_FILE = -27,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_IPC_MSG_FAIL = -28,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_REQUEST_OVERFLOWS = -29,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_PARAM_NOT_EXIST = -30,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CRYPTO_ENGINE_ERROR = -31,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_COMMUNICATION_TIMEOUT = -32,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_IPC_INIT_FAIL = -33,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_IPC_DLOPEN_FAIL = -34,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_EFUSE_READ_FAIL = -35,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_NEW_ROOT_KEY_MATERIAL_EXIST = -36,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_UPDATE_ROOT_KEY_MATERIAL_FAIL = -37,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_VERIFICATION_FAILED = -38,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_ALG_FAIL = -100,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_KEY_SIZE_FAIL = -101,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_PADDING_FAIL = -102,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_PURPOSE_FAIL = -103,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_DIGEST_FAIL = -104,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_MODE_FAIL = -105,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_NONCE_FAIL = -106,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_AAD_FAIL = -107,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_IV_FAIL = -108,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_AE_TAG_FAIL = -109,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_SALT_FAIL = -110,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_CHECK_GET_ITERATION_FAIL = -111,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_ALGORITHM = -112,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_KEY_SIZE = -113,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_PADDING = -114,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_PURPOSE = -115,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_MODE = -116,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_DIGEST = -117,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_SIGNATURE_SIZE = -118,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_IV = -119,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_AAD = -120,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_NONCE = -121,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_AE_TAG = -122,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_SALT = -123,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_ITERATION = -124,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INVALID_OPERATION = -125,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_INTERNAL_ERROR = -999,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_ERROR_UNKNOWN_ERROR = -1000
  }

  /**
   * Enumerates the error codes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 9
   */
  /**
   * Enumerates the error codes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksExceptionErrCode {
    /**
     * Permission verification failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Permission verification failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_PERMISSION_FAIL = 201,
    /**
     * The caller is not a system application and cannot call the system API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 12
     */
    HUKS_ERR_CODE_NOT_SYSTEM_APP = 202,
    /**
     * Invalid parameters are detected. Possible causes:
     * 1. Mandatory parameters are left unspecified.
     * 2. Incorrect parameter types.
     * 3. Parameter verification failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Invalid parameters are detected. Possible causes:
     * 1. Mandatory parameters are left unspecified.
     * 2. Incorrect parameter types.
     * 3. Parameter verification failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_ILLEGAL_ARGUMENT = 401,
    /**
     * The API is not supported.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The API is not supported.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_NOT_SUPPORTED_API = 801,
    /**
     * The feature is not supported.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The feature is not supported.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_FEATURE_NOT_SUPPORTED = 12000001,
    /**
     * Key algorithm parameters are missing.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Key algorithm parameters are missing.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_MISSING_CRYPTO_ALG_ARGUMENT = 12000002,
    /**
     * Invalid key algorithm parameters are detected.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Invalid key algorithm parameters are detected.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_INVALID_CRYPTO_ALG_ARGUMENT = 12000003,
    /**
     * The file operation failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The file operation failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_FILE_OPERATION_FAIL = 12000004,
    /**
     * The communication failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The communication failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_COMMUNICATION_FAIL = 12000005,
    /**
     * Failed to operate the algorithm library.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Failed to operate the algorithm library.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_CRYPTO_FAIL = 12000006,
    /**
     * Failed to access the key because the key has expired.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Failed to access the key because the key has expired.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_KEY_AUTH_PERMANENTLY_INVALIDATED = 12000007,
    /**
     * Failed to access the key because the authentication has failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Failed to access the key because the authentication has failed.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_KEY_AUTH_VERIFY_FAILED = 12000008,
    /**
     * Key access timed out.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Key access timed out.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_KEY_AUTH_TIME_OUT = 12000009,
    /**
     * The number of key operation sessions has reached the limit.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The number of key operation sessions has reached the limit.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_SESSION_LIMIT = 12000010,
    /**
     * The target object does not exist.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The target object does not exist.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_ITEM_NOT_EXIST = 12000011,
    /**
     * An external error occurs.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * An external error occurs.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_EXTERNAL_ERROR = 12000012,
    /**
     * The credential does not exist.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The credential does not exist.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_CREDENTIAL_NOT_EXIST = 12000013,
    /**
     * The memory is insufficient.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * The memory is insufficient.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_INSUFFICIENT_MEMORY = 12000014,
    /**
     * Failed to call other system services.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 9
     */
    /**
     * Failed to call other system services.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ERR_CODE_CALL_SERVICE_FAILED = 12000015,
    /**
     * The required lock screen password is not set.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * The required lock screen password is not set.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_ERR_CODE_DEVICE_PASSWORD_UNSET = 12000016,
    /**
     * The input parameter is invalid.
     * 
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 20
     */
    HUKS_ERR_CODE_INVALID_ARGUMENT = 12000018,
  }

  /**
   * Enumerates the key purposes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the key purposes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksKeyPurpose {
    /**
     * Used to encrypt the plaintext.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Used to encrypt the plaintext.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_KEY_PURPOSE_ENCRYPT = 1,
    /**
     * Used to decrypt the cipher text.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Used to decrypt the cipher text.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_KEY_PURPOSE_DECRYPT = 2,
    /**
     * Used for signing.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used for signing.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_SIGN = 4,
    /**
     * Used to verify the signature.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used to verify the signature.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_VERIFY = 8,
    /**
     * Used to derive a key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used to derive a key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_DERIVE = 16,
    /**
     * Used for an encrypted export.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used for an encrypted export.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_WRAP = 32,
    /**
     * Used for an encrypted import.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used for an encrypted import.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_UNWRAP = 64,
    /**
     * Used to generate a message authentication code (MAC).
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used to generate a message authentication code (MAC).
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_MAC = 128,
    /**
     * Used for key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Used for key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_PURPOSE_AGREE = 256
  }

  /**
   * Enumerates the digest algorithms.
   *
   * @enum { number }
   *
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   */
  /**
   * Enumerates the digest algorithms.
   *
   * @enum { number }
   *
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksKeyDigest {
    /**
     * No digest algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * No digest algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_NONE = 0,
    /**
     * MD5.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * MD5.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_MD5 = 1,
    /**
     * SM3.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * SM3.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SM3 = 2,
    /**
     * SHA-1.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * SHA-1.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SHA1 = 10,
    /**
     * SHA-224.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * SHA-224.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SHA224 = 11,
    /**
     * SHA-256.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * SHA-256.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SHA256 = 12,
    /**
     * SHA-384.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * SHA-384.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SHA384 = 13,
    /**
     * SHA-512.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * SHA-512.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DIGEST_SHA512 = 14
  }

  /**
   * Enumerates the padding algorithms.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the padding algorithms.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksKeyPadding {
    /**
     * No padding algorithm is used.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * No padding algorithm is used.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_PADDING_NONE = 0,
    /**
     * Optimal Asymmetric Encryption Padding (OAEP).
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Optimal Asymmetric Encryption Padding (OAEP).
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_OAEP = 1,
    /**
     * Probabilistic Signature Scheme (PSS).
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Probabilistic Signature Scheme (PSS).
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_PSS = 2,
    /**
     * Public Key Cryptography Standards (PKCS) #1 v1.5.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Public Key Cryptography Standards (PKCS) #1 v1.5.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_PKCS1_V1_5 = 3,
    /**
     * PKCS #5.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * PKCS #5.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_PKCS5 = 4,
    /**
     * PKCS #7.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * PKCS #7.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_PKCS7 = 5,
    /**
     * ISO_IEC_9796_2.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_ISO_IEC_9796_2 = 6,
    /**
     * ISO_IEC_9797_1.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_PADDING_ISO_IEC_9797_1 = 7,
  }

  /**
   * Enumerates the cipher modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the cipher modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksCipherMode {
    /**
     * Electronic Code Block (ECB) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Electronic Code Block (ECB) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_ECB = 1,
    /**
     * Cipher Block Chaining (CBC) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Cipher Block Chaining (CBC) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_CBC = 2,
    /**
     * Counter (CTR) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Counter (CTR) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_CTR = 3,
    /**
     * Output Feedback (OFB) mode.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Output Feedback (OFB) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_OFB = 4,
    /**
     * Ciphertext Feedback (CFB) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_CFB = 5,
    /**
     * Counter with CBC-MAC (CCM) mode.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Counter with CBC-MAC (CCM) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_MODE_CCM = 31,

    /**
     * Galois/Counter (GCM) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Galois/Counter (GCM) mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_MODE_GCM = 32
  }

  /**
   * Enumerates the key sizes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the key sizes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksKeySize {
    /**
     * Rivest-Shamir-Adleman (RSA) key of 512 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Rivest-Shamir-Adleman (RSA) key of 512 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_512 = 512,
    /**
     * RSA key of 768 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA key of 768 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_768 = 768,
    /**
     * RSA key of 1024 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA key of 1024 bits.
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_1024 = 1024,
    /**
     * RSA key of 2048 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA key of 2048 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_2048 = 2048,
    /**
     * RSA key of 3072 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA key of 3072 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_3072 = 3072,
    /**
     * RSA key of 4096 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA key of 4096 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_KEY_SIZE_4096 = 4096,

    /**
     * Elliptic Curve Cryptography (ECC) key of 224 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Elliptic Curve Cryptography (ECC) key of 224 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ECC_KEY_SIZE_224 = 224,
    /**
     * ECC key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ECC key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ECC_KEY_SIZE_256 = 256,
    /**
     * ECC key of 384 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ECC key of 384 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ECC_KEY_SIZE_384 = 384,
    /**
     * ECC key of 521 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ECC key of 521 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ECC_KEY_SIZE_521 = 521,

    /**
     * Advanced Encryption Standard (AES) key of 128 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Advanced Encryption Standard (AES) key of 128 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_AES_KEY_SIZE_128 = 128,
    /**
     * AES key of 192 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * AES key of 192 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_AES_KEY_SIZE_192 = 192,
    /**
     * AES key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * AES key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_AES_KEY_SIZE_256 = 256,

    /**
     * AES key of 512 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 11
     */
    HUKS_AES_KEY_SIZE_512 = 512,

    /**
     * Curve25519 key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Curve25519 key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_CURVE25519_KEY_SIZE_256 = 256,

    /**
     * Diffie-Hellman (DH) key of 2048 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Diffie-Hellman (DH) key of 2048 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DH_KEY_SIZE_2048 = 2048,
    /**
     * DH key of 3072 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * DH key of 3072 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DH_KEY_SIZE_3072 = 3072,
    /**
     * DH key of 4096 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * DH key of 4096 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DH_KEY_SIZE_4096 = 4096,

    /**
     * ShangMi2 (SM2) key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * ShangMi2 (SM2) key of 256 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_SM2_KEY_SIZE_256 = 256,
    /**
     * ShangMi4 (SM4) key of 128 bits.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * ShangMi4 (SM4) key of 128 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_SM4_KEY_SIZE_128 = 128,
    /**
     * DES key of 64 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_DES_KEY_SIZE_64 = 64,
    /**
     * 3DES key of 128 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_3DES_KEY_SIZE_128 = 128,
    /**
     * 3DES key of 192 bits.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_3DES_KEY_SIZE_192 = 192,
  }

  /**
   * Enumerates the key algorithms.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
    /**
   * Enumerates the key algorithms.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksKeyAlg {
    /**
     * RSA.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * RSA.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_RSA = 1,
    /**
     * ECC.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ECC.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_ECC = 2,
    /**
     * DSA.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * DSA.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_DSA = 3,

    /**
     * AES.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * AES.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_ALG_AES = 20,
    /**
     * HMAC.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * HMAC.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_HMAC = 50,
    /**
     * HKDF.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * HKDF.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_HKDF = 51,
    /**
     * PBKDF2.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * PBKDF2.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_PBKDF2 = 52,

    /**
     * ECDH.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ECDH.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_ECDH = 100,
    /**
     * X25519.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * X25519.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_X25519 = 101,
    /**
     * Ed25519.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Ed25519.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_ED25519 = 102,
    /**
     * DH.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * DH.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_DH = 103,

    /**
     * SM2.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * SM2.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_SM2 = 150,
    /**
     * SM3.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * SM3.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_SM3 = 151,
    /**
     * SM4.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * SM4.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_SM4 = 152,
    /**
     * DES.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_DES = 160,
    /**
     * 3DES.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_3DES = 161,
    /**
     * CMAC.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_ALG_CMAC = 162
  }

  /**
   * Enumerates the algorithm suites that can be used for importing a key in ciphertext.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the algorithm suites that can be used for importing a key in ciphertext.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksUnwrapSuite {
    /**
     * Use X25519 for key agreement and then use AES-256 GCM to encrypt the key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Use X25519 for key agreement and then use AES-256 GCM to encrypt the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_UNWRAP_SUITE_X25519_AES_256_GCM_NOPADDING = 1,
    /**
     * Use ECDH for key agreement and then use AES-256 GCM to encrypt the key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Use ECDH for key agreement and then use AES-256 GCM to encrypt the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_UNWRAP_SUITE_ECDH_AES_256_GCM_NOPADDING = 2
  }

  /**
   * Enumerates the key generation types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   */
  /**
   * Enumerates the key generation types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksKeyGenerateType {
    /**
     * Key generated by default.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Key generated by default.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_GENERATE_TYPE_DEFAULT = 0,
    /**
     * Derived key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Derived key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_GENERATE_TYPE_DERIVE = 1,
    /**
     * Key generated by agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Key generated by agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_GENERATE_TYPE_AGREE = 2
  }

  /**
   * Enumerates the key generation modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the key generation modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksKeyFlag {
    /**
     * Import a key using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Import a key using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_FLAG_IMPORT_KEY = 1,
    /**
     * Generate a key by using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Generate a key by using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_FLAG_GENERATE_KEY = 2,
    /**
     * Generate a key by using a key agreement API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Generate a key by using a key agreement API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_FLAG_AGREE_KEY = 3,
    /**
     * Derive a key by using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Derive a key by using an API.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_FLAG_DERIVE_KEY = 4
  }

  /**
   * Enumerates the key storage modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the key storage modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksKeyStorageType {
    /**
     * The key is managed locally. NOTE: This tag is deprecated since API version 10. No substitute is
     * provided because this tag is not used in key management. In key derivation scenarios, use
     * HUKS_STORAGE_ONLY_USED_IN_HUKS or HUKS_STORAGE_KEY_EXPORT_ALLOWED.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 10
     */
    HUKS_STORAGE_TEMP = 0,
    /**
     * The key is managed by the HUKS service. NOTE: This tag is deprecated since API version 10. No
     * substitute is provided because this tag is not used in key management. In key derivation scenarios, use
     * HUKS_STORAGE_ONLY_USED_IN_HUKS or HUKS_STORAGE_KEY_EXPORT_ALLOWED.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 10
     */
    HUKS_STORAGE_PERSISTENT = 1,
    /**
     * The key derived from the master key is stored in the HUKS and managed by the HUKS.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * The key derived from the master key is stored in the HUKS and managed by the HUKS.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_STORAGE_ONLY_USED_IN_HUKS = 2,
    /**
     * The key derived from the master key is exported to the service, and not managed by the HUKS.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * The key derived from the master key is exported to the service, and not managed by the HUKS.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_STORAGE_KEY_EXPORT_ALLOWED = 3
  }

  /**
   * Enumerates the types of keys to import. By default, a public key is imported. This field is not
   * required when a symmetric key is imported.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the types of keys to import. By default, a public key is imported. This field is not
   * required when a symmetric key is imported.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksImportKeyType {
    /**
     * Public key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Public key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_TYPE_PUBLIC_KEY = 0,
    /**
     * Private key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Private key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_TYPE_PRIVATE_KEY = 1,
    /**
     * Public and private key pair.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Public and private key pair.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_KEY_TYPE_KEY_PAIR = 2
  }

  /**
   * Enumerates the salt_len types to set when PSS padding is used in RSA signing or signature
   * verification.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 10
   */
  /**
   * Enumerates the salt_len types to set when PSS padding is used in RSA signing or signature
   * verification.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksRsaPssSaltLenType {
    /**
     * salt_len is set to the digest length.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * salt_len is set to the digest length.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_PSS_SALT_LEN_DIGEST = 0,

    /**
     * salt_len is set to the maximum length.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * salt_len is set to the maximum length.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_RSA_PSS_SALT_LEN_MAX = 1
  }

  /**
   * Enumerates the user authentication types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the user authentication types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksUserAuthType {
    /**
     * Fingerprint authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Fingerprint authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_USER_AUTH_TYPE_FINGERPRINT = 1 << 0,
    /**
     * Facial authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Facial authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_USER_AUTH_TYPE_FACE = 1 << 1,
    /**
     * PIN authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * PIN authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_USER_AUTH_TYPE_PIN = 1 << 2,
    /**
     * Tui pin auth type.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 20
     */
    HUKS_USER_AUTH_TYPE_TUI_PIN = 1 << 5
  }

  /**
   * Enumerates the access control types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the access control types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksAuthAccessType {
    /**
     * The key becomes invalid after the password is cleared.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * The key becomes invalid after the password is cleared.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_ACCESS_INVALID_CLEAR_PASSWORD = 1 << 0,
    /**
     * The key becomes invalid after a new biometric feature is added.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * The key becomes invalid after a new biometric feature is added.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_ACCESS_INVALID_NEW_BIO_ENROLL = 1 << 1,
    /**
     * The key is always valid.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * The key is always valid.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_ACCESS_ALWAYS_VALID = 1 << 2
  }

  /**
   * Enumerates the user authentication modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksUserAuthMode {
    /**
     * Local authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_USER_AUTH_MODE_LOCAL = 0,
    /**
     * Cross-device collaborative authentication.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_USER_AUTH_MODE_COAUTH = 1,
  }
  /**
   * Enumerates the storage security levels of a key.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 11
   */
  /**
   * Enumerates the storage security levels of a key.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksAuthStorageLevel {
    /**
     * The key can be accessed only after the device is started.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * The key can be accessed only after the device is started.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_STORAGE_LEVEL_DE = 0,
    /**
     * The key can be accessed only after the first unlock of the device.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * The key can be accessed only after the first unlock of the device.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_STORAGE_LEVEL_CE = 1,
    /**
     * The key can be accessed only when the device is unlocked.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * The key can be accessed only when the device is unlocked.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_AUTH_STORAGE_LEVEL_ECE = 2,
  }

  /**
   * Enumerates the types of the challenges generated when a key is used.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the types of the challenges generated when a key is used.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksChallengeType {
    /**
     * Normal challenge, which is of 32 bytes by default.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Normal challenge, which is of 32 bytes by default.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_TYPE_NORMAL = 0,
    /**
     * Custom challenge, which supports only one authentication for multiple keys.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Custom challenge, which supports only one authentication for multiple keys.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_TYPE_CUSTOM = 1,
    /**
     * Challenge is not required.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Challenge is not required.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_TYPE_NONE = 2
  }

  /**
   * Enumerates the positions of the 8-byte valid value in a custom challenge generated.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the positions of the 8-byte valid value in a custom challenge generated.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksChallengePosition {
    /**
     * Bytes 0 to 7.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Bytes 0 to 7.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_POS_0 = 0,
    /**
     * Bytes 8 to 15.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Bytes 8 to 15.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_POS_1,
    /**
     * Bytes 16 to 23.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Bytes 16 to 23.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_POS_2,
    /**
     * Bytes 24 to 31.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Bytes 24 to 31.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_CHALLENGE_POS_3
  }

  /**
   * Enumerates the signature types of the key generated or imported.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 9
   */
  /**
   * Enumerates the signature types of the key generated or imported.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @atomicservice
   * @since 12
   */
  export enum HuksSecureSignType {
    /**
     * The signature carries authentication information. This field is specified when a key is generated or
     * imported. When the key is used for signing, the data will be added with the authentication information and then
     * be signed.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * The signature carries authentication information. This field is specified when a key is generated or
     * imported. When the key is used for signing, the data will be added with the authentication information and then
     * be signed.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_SECURE_SIGN_WITH_AUTHINFO = 1
  }

  /**
   * Enumerates the tag transfer modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Extension
   * @since 8
   */
  /**
   * Enumerates the tag transfer modes.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 12
   */
  export enum HuksSendType {
    /**
     * The tag is sent asynchronously.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * The tag is sent asynchronously.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_SEND_TYPE_ASYNC = 0,
    /**
     * The tag is sent synchronously.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * The tag is sent synchronously.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_SEND_TYPE_SYNC = 1
  }

  /**
   * Enum for key wrap type
   * 
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 20
   */
  export enum HuksKeyWrapType {
    /**
     * The hardware unique key wrap type
     * 
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 20
     */
    HUKS_KEY_WRAP_TYPE_HUK_BASED = 2,
  }

  /**
   * Enumerates the tag data types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the tag data types.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksTagType {
    /**
     * Invalid tag type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Invalid tag type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_INVALID = 0 << 28,
    /**
     * Number of the int type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Number of the int type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_INT = 1 << 28,
    /**
     * Number of the uint type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Number of the uint type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_UINT = 2 << 28,
    /**
     * BigInt.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * BigInt.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_ULONG = 3 << 28,
    /**
     * Boolean.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Boolean.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_BOOL = 4 << 28,
    /**
     * Uint8Array.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Uint8Array.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_TYPE_BYTES = 5 << 28
  }

  /**
   * Enumerates the tags used to invoke parameters.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @since 8
   */
  /**
   * Enumerates the tags used to invoke parameters.
   *
   * @enum { number }
   * @syscap SystemCapability.Security.Huks.Core
   * @atomicservice
   * @since 11
   */
  export enum HuksTag {
    /**
     * Invalid TAG
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_INVALID = HuksTagType.HUKS_TAG_TYPE_INVALID | 0,

    /* Base algorithm TAG: 1 - 200 */
    /**
     * Algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_ALGORITHM = HuksTagType.HUKS_TAG_TYPE_UINT | 1,
    /**
     * Purpose of the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Purpose of the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_PURPOSE = HuksTagType.HUKS_TAG_TYPE_UINT | 2,
    /**
     * Key size.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Key size.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_KEY_SIZE = HuksTagType.HUKS_TAG_TYPE_UINT | 3,

    /**
     * Digest algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Digest algorithm.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_DIGEST = HuksTagType.HUKS_TAG_TYPE_UINT | 4,
    /**
     * Padding mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Padding mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_PADDING = HuksTagType.HUKS_TAG_TYPE_UINT | 5,
    /**
     * Cipher mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Cipher mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_BLOCK_MODE = HuksTagType.HUKS_TAG_TYPE_UINT | 6,
    /**
     * Key type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Key type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 7,
    /**
     * Associated authentication data.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Associated authentication data.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_ASSOCIATED_DATA = HuksTagType.HUKS_TAG_TYPE_BYTES | 8,

    /**
     * Nonce for key encryption and decryption.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Nonce for key encryption and decryption.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_NONCE = HuksTagType.HUKS_TAG_TYPE_BYTES | 9,
    /**
     * IV.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * IV.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IV = HuksTagType.HUKS_TAG_TYPE_BYTES | 10,

    /**
     * Information generated during key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Information generated during key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_INFO = HuksTagType.HUKS_TAG_TYPE_BYTES | 11,
    /**
     * Salt value used for key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Salt value used for key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_SALT = HuksTagType.HUKS_TAG_TYPE_BYTES | 12,
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_PWD = HuksTagType.HUKS_TAG_TYPE_BYTES | 13,
    /**
     * Number of iterations for key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Number of iterations for key derivation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ITERATION = HuksTagType.HUKS_TAG_TYPE_UINT | 14,

    /**
     * Key generation type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Key generation type.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_GENERATE_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 15,

    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_DERIVE_MAIN_KEY = HuksTagType.HUKS_TAG_TYPE_BYTES | 16,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_DERIVE_FACTOR = HuksTagType.HUKS_TAG_TYPE_BYTES | 17,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_DERIVE_ALG = HuksTagType.HUKS_TAG_TYPE_UINT | 18,
    /**
     * Type of the algorithm used for key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Type of the algorithm used for key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AGREE_ALG = HuksTagType.HUKS_TAG_TYPE_UINT | 19,
    /**
     * Public key alias used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Public key alias used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AGREE_PUBLIC_KEY_IS_KEY_ALIAS = HuksTagType.HUKS_TAG_TYPE_BOOL | 20,
    /**
     * Private key alias used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Private key alias used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AGREE_PRIVATE_KEY_ALIAS = HuksTagType.HUKS_TAG_TYPE_BYTES | 21,
    /**
     * Public key used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Public key used in key agreement.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AGREE_PUBLIC_KEY = HuksTagType.HUKS_TAG_TYPE_BYTES | 22,
    /**
     * Key alias.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Key alias.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_KEY_ALIAS = HuksTagType.HUKS_TAG_TYPE_BYTES | 23,
    /**
     * Size of the derived key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Size of the derived key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_DERIVE_KEY_SIZE = HuksTagType.HUKS_TAG_TYPE_UINT | 24,

    /**
     * Type of the imported key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Type of the imported key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IMPORT_KEY_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 25,

    /**
     * Algorithm suite required for encrypted imports.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Algorithm suite required for encrypted imports.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_UNWRAP_ALGORITHM_SUITE = HuksTagType.HUKS_TAG_TYPE_UINT | 26,

    /**
     * Storage type of the derived key or agreed key.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * Storage type of the derived key or agreed key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_DERIVED_AGREED_KEY_STORAGE_FLAG = HuksTagType.HUKS_TAG_TYPE_UINT | 29,

    /**
     * Type of the rsa_pss_salt_length.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * Type of the rsa_pss_salt_length.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_RSA_PSS_SALT_LEN_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 30,

    /*
     * Key authentication related TAG: 201 - 300
     *
     * Start of validity
     */
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ACTIVE_DATETIME = HuksTagType.HUKS_TAG_TYPE_ULONG | 201,
    /**
     * Date when new "messages" should not be created.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ORIGINATION_EXPIRE_DATETIME = HuksTagType.HUKS_TAG_TYPE_ULONG | 202,
    /**
     * Date when existing "messages" should not be used.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_USAGE_EXPIRE_DATETIME = HuksTagType.HUKS_TAG_TYPE_ULONG | 203,
    /**
     * Key creation time.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_CREATION_DATETIME = HuksTagType.HUKS_TAG_TYPE_ULONG | 204,

    /* Other authentication related TAG: 301 - 500 */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ALL_USERS = HuksTagType.HUKS_TAG_TYPE_BOOL | 301,
    /**
     * ID of the user to which the key belongs.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * ID of the user to which the key belongs.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_USER_ID = HuksTagType.HUKS_TAG_TYPE_UINT | 302,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_NO_AUTH_REQUIRED = HuksTagType.HUKS_TAG_TYPE_BOOL | 303,
    /**
     * User authentication type. For details, see HuksUserAuthType. This parameter must be set together with
     * HuksAuthAccessType. You can set a maximum of two user authentication types at a time. For example, if
     * HuksAuthAccessType is HUKS_SECURE_ACCESS_INVALID_NEW_BIO_ENROLL, you can set the user authentication type to
     * HUKS_USER_AUTH_TYPE_FACE, HUKS_USER_AUTH_TYPE_FINGERPRINT or
     * HUKS_USER_AUTH_TYPE_FACE | HUKS_USER_AUTH_TYPE_FINGERPRINT.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * User authentication type. For details, see HuksUserAuthType. This parameter must be set together with
     * HuksAuthAccessType. You can set a maximum of two user authentication types at a time. For example, if
     * HuksAuthAccessType is HUKS_SECURE_ACCESS_INVALID_NEW_BIO_ENROLL, you can set the user authentication type to
     * HUKS_USER_AUTH_TYPE_FACE, HUKS_USER_AUTH_TYPE_FINGERPRINT or
     * HUKS_USER_AUTH_TYPE_FACE | HUKS_USER_AUTH_TYPE_FINGERPRINT.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_USER_AUTH_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 304,
    /**
     * One-time validity period of the authentication token.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * One-time validity period of the authentication token.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AUTH_TIMEOUT = HuksTagType.HUKS_TAG_TYPE_UINT | 305,
    /**
     * Authentication token.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Authentication token.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AUTH_TOKEN = HuksTagType.HUKS_TAG_TYPE_BYTES | 306,

    /* Key secure access control and user auth TAG */
    /**
     * Access control type. For details, see HuksAuthAccessType. This parameter must be set together with
     * HuksUserAuthType.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Access control type. For details, see HuksAuthAccessType. This parameter must be set together with
     * HuksUserAuthType.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_AUTH_ACCESS_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 307,

    /**
     * Signature type of the key generated or imported.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Signature type of the key generated or imported.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_SECURE_SIGN_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 308,

    /**
     * Type of the challenge generated for a key. For details, see HuksChallengeType.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Type of the challenge generated for a key. For details, see HuksChallengeType.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_CHALLENGE_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 309,

    /**
     * Position of the 8-byte valid value in a custom challenge. For details, see HuksChallengePosition.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 9
     */
    /**
     * Position of the 8-byte valid value in a custom challenge. For details, see HuksChallengePosition.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_CHALLENGE_POS = HuksTagType.HUKS_TAG_TYPE_UINT | 310,

    /**
     * Key authentication purpose.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 10
     */
    /**
     * Key authentication purpose.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_AUTH_PURPOSE = HuksTagType.HUKS_TAG_TYPE_UINT | 311,

    /**
     * Key storage security level, which is a value of HuksAuthStorageLevel.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * Key storage security level, which is a value of HuksAuthStorageLevel.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_AUTH_STORAGE_LEVEL = HuksTagType.HUKS_TAG_TYPE_UINT | 316,

    /**
     * User authentication mode, which is a value of HuksUserAuthMode.
     * 
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_USER_AUTH_MODE = HuksTagType.HUKS_TAG_TYPE_UINT | 319,

    /* Attestation related TAG: 501 - 600 */
    /**
     * Challenge value used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Challenge value used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ATTESTATION_CHALLENGE = HuksTagType.HUKS_TAG_TYPE_BYTES | 501,
    /**
     * Application ID used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Application ID used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ATTESTATION_APPLICATION_ID = HuksTagType.HUKS_TAG_TYPE_BYTES | 502,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_BRAND = HuksTagType.HUKS_TAG_TYPE_BYTES | 503,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_DEVICE = HuksTagType.HUKS_TAG_TYPE_BYTES | 504,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_PRODUCT = HuksTagType.HUKS_TAG_TYPE_BYTES | 505,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_SERIAL = HuksTagType.HUKS_TAG_TYPE_BYTES | 506,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_IMEI = HuksTagType.HUKS_TAG_TYPE_BYTES | 507,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_MEID = HuksTagType.HUKS_TAG_TYPE_BYTES | 508,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_MANUFACTURER = HuksTagType.HUKS_TAG_TYPE_BYTES | 509,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_MODEL = HuksTagType.HUKS_TAG_TYPE_BYTES | 510,
    /**
     * Key alias used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Key alias used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ATTESTATION_ID_ALIAS = HuksTagType.HUKS_TAG_TYPE_BYTES | 511,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_SOCID = HuksTagType.HUKS_TAG_TYPE_BYTES | 512,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ATTESTATION_ID_UDID = HuksTagType.HUKS_TAG_TYPE_BYTES | 513,
    /**
     * Security level used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Security level used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ATTESTATION_ID_SEC_LEVEL_INFO = HuksTagType.HUKS_TAG_TYPE_BYTES | 514,
    /**
     * Version information used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Version information used in the attestation.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ATTESTATION_ID_VERSION_INFO = HuksTagType.HUKS_TAG_TYPE_BYTES | 515,

    /*
     * Other reserved TAG: 601 - 1000
     *
     * Extension TAG: 1001 - 9999
     */

    /**
     * Whether to use the alias passed in during key generation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Whether to use the alias passed in during key generation.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IS_KEY_ALIAS = HuksTagType.HUKS_TAG_TYPE_BOOL | 1001,
    /**
     * Key storage mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Key storage mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_STORAGE_FLAG = HuksTagType.HUKS_TAG_TYPE_UINT | 1002,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IS_ALLOWED_WRAP = HuksTagType.HUKS_TAG_TYPE_BOOL | 1003,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_WRAP_TYPE = HuksTagType.HUKS_TAG_TYPE_UINT | 1004,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_AUTH_ID = HuksTagType.HUKS_TAG_TYPE_BYTES | 1005,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_ROLE = HuksTagType.HUKS_TAG_TYPE_UINT | 1006,
    /**
     * Flag of the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Flag of the key.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_FLAG = HuksTagType.HUKS_TAG_TYPE_UINT | 1007,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IS_ASYNCHRONIZED = HuksTagType.HUKS_TAG_TYPE_UINT | 1008,
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_SECURE_KEY_ALIAS = HuksTagType.HUKS_TAG_TYPE_BOOL | 1009,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_SECURE_KEY_UUID = HuksTagType.HUKS_TAG_TYPE_BYTES | 1010,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY_DOMAIN = HuksTagType.HUKS_TAG_TYPE_UINT | 1011,

    /**
     * Whether the key is accessible only when the user sets a lock screen password.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 11
     */
    /**
     * Whether the key is accessible only when the user sets a lock screen password.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_IS_DEVICE_PASSWORD_SET = HuksTagType.HUKS_TAG_TYPE_BOOL | 1012,

    /* Inner-use TAG: 10001 - 10999 */

    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_PROCESS_NAME = HuksTagType.HUKS_TAG_TYPE_BYTES | 10001,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_PACKAGE_NAME = HuksTagType.HUKS_TAG_TYPE_BYTES | 10002,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_ACCESS_TIME = HuksTagType.HUKS_TAG_TYPE_UINT | 10003,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_USES_TIME = HuksTagType.HUKS_TAG_TYPE_UINT | 10004,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_CRYPTO_CTX = HuksTagType.HUKS_TAG_TYPE_ULONG | 10005,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_KEY = HuksTagType.HUKS_TAG_TYPE_BYTES | 10006,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_KEY_VERSION = HuksTagType.HUKS_TAG_TYPE_UINT | 10007,
    /**
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_PAYLOAD_LEN = HuksTagType.HUKS_TAG_TYPE_UINT | 10008,

    /**
     * Used to pass in the AEAD in GCM mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Used to pass in the AEAD in GCM mode.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 11
     */
    HUKS_TAG_AE_TAG = HuksTagType.HUKS_TAG_TYPE_BYTES | 10009,

    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_IS_KEY_HANDLE = HuksTagType.HUKS_TAG_TYPE_ULONG | 10010,

    /**
     * Os version related TAG.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_OS_VERSION = HuksTagType.HUKS_TAG_TYPE_UINT | 10101,
    /**
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     * @deprecated since 9
     */
    HUKS_TAG_OS_PATCHLEVEL = HuksTagType.HUKS_TAG_TYPE_UINT | 10102,

    /*
     * Reserved TAGs: 11000 - 12000
     *
     * Other TAGs: 20001 - N
     * TAGs used for paramSetOut
     */

    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_SYMMETRIC_KEY_DATA = HuksTagType.HUKS_TAG_TYPE_BYTES | 20001,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ASYMMETRIC_PUBLIC_KEY_DATA = HuksTagType.HUKS_TAG_TYPE_BYTES | 20002,
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Extension
     * @since 8
     */
    /**
     * Reserved.
     *
     * @syscap SystemCapability.Security.Huks.Core
     * @atomicservice
     * @since 12
     */
    HUKS_TAG_ASYMMETRIC_PRIVATE_KEY_DATA = HuksTagType.HUKS_TAG_TYPE_BYTES | 20003
  }
}

export default huks;
