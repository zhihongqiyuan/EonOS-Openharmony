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
 * @kit CoreFileKit
 */
 
/**
 * Provides filesystem KeyManager APIs.
 *
 * @namespace keyManager
 * @syscap SystemCapability.FileManagement.StorageService.Encryption
 * @since 15
 */
declare namespace keyManager {
   /**
    * Initiate the deactivation of user key for the specified user when user screen is locked.
    *
    * @permission ohos.permission.STORAGE_MANAGER_CRYPT
    * @param { number } userId
    * @throws { BusinessError } 201 - Permission verification failed.
    * @throws { BusinessError } 202 - The caller is not a system application.
    * @throws { BusinessError } 401 - The input parameter is invalid. Possible causes: Mandatory
 parameters are left unspecified; Or input parameter has type different from the type the interface requires.
    * @throws { BusinessError } 13600001 - IPC error.
    * @throws { BusinessError } 13600008 - No such object. Possible causes: Cannot find userkey for the specified user.
    * @throws { BusinessError } 13600009 - User ID out of range. Possible causes: input parameter userId < 100 or userId > 10736.
    * @syscap SystemCapability.FileManagement.StorageService.Encryption
    * @systemapi
    * @since 15
    */
   function deactivateUserKey(userId: number):void;
 }
 
 export default keyManager;