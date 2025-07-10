/*
 * Copyright (c) 2022-2025 Huawei Device Co., Ltd.
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
 * @kit AbilityKit
 */

import { AsyncCallback } from './@ohos.base';
import * as _ErrorObserver from './application/ErrorObserver';
import { LoopObserver as _LoopObserver } from './application/LoopObserver';

/**
 * This module provides the function of error manager.
 *
 * @namespace errorManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 9
 */
/**
 * This module provides the function of error manager.
 *
 * @namespace errorManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @atomicservice
 * @since 11
 */
/**
 * This module provides the function of error manager.
 *
 * @namespace errorManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @crossplatform
 * @atomicservice
 * @since 19
 */
declare namespace errorManager {
  /**
   * Register error observer.
   *
   * @param { 'error' } type - error.
   * @param { ErrorObserver } observer - The error observer.
   * @returns { number } Returns the number code of the observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Register error observer.
   *
   * @param { 'error' } type - error.
   * @param { ErrorObserver } observer - The error observer.
   * @returns { number } Returns the number code of the observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Register error observer.
   *
   * @param { 'error' } type - error.
   * @param { ErrorObserver } observer - The error observer.
   * @returns { number } Returns the number code of the observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function on(type: 'error', observer: ErrorObserver): number;

  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @param { AsyncCallback<void> } callback - The callback of off.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @param { AsyncCallback<void> } callback - The callback of off.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @param { AsyncCallback<void> } callback - The callback of off.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function off(type: 'error', observerId: number, callback: AsyncCallback<void>): void;

  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 11
   */
  /**
   * Unregister error observer.
   *
   * @param { 'error' } type - error.
   * @param { number } observerId - Indicates the number code of the observer.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16000003 - The specified ID does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function off(type: 'error', observerId: number): Promise<void>;

  /**
   * Register an error observer for all VM instances include worker and taskpool.
   * @param { 'globalErrorOccurred'} type - globalErrorOccurred
   * @param { GlobalObserver } observer - the global error observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  function on(type: 'globalErrorOccurred', observer: GlobalObserver): void;

  /**
   * Register a rejection observer for all VM instances include worker and taskpool.
   * @param { 'globalUnhandledRejectionDetected'} type - globalUnhandledRejectionDetected.
   * @param { GlobalObserver } observer - the global error observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  function on(type: 'globalUnhandledRejectionDetected', observer: GlobalObserver): void;

  /**
   * Unregister the error observer for all VM instance include worker and taskpool.
   * @param { 'globalErrorOccurred'} type - globalErrorOccurred.
   * @param { GlobalObserver } observer - the global error observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @throws { BusinessError } 16300004 - The observer does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  function off(type: 'globalErrorOccurred', observer?: GlobalObserver): void;

  /**
   * Unregister the rejection observer for all VM instance include worker and taskpool.
   * @param { 'globalUnhandledRejectionDetected'} type - globalUnhandledRejectionDetected.
   * @param { GlobalObserver } observer - the global error observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @throws { BusinessError } 16300004 - The observer does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  function off(type: 'globalUnhandledRejectionDetected', observer?: GlobalObserver): void;

  /**
   * The observer will be called by system when an error or unhandled rejection occurs
   * from all VM instances include worker and taskpool.
   * @typedef { function }
   * { GlobalError } reason - the reason of the error or rejection.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  export type GlobalObserver = (reason: GlobalError) => void;

  /**
   * Defines GlobalError.
   * 
   * @extends Error
   * @typedef GlobalError
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  export interface GlobalError extends Error {
    /**
     * Define the instance name of VM.
     * @type { string } instanceName
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 18
     */
    instanceName: string;

    /**
     * Define the instance type of VM.
     * @type { InstanceType } instanceType
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 18
     */
    instanceType: InstanceType;
  }

  /**
   * Define the instance type of VM.
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @atomicservice
   * @since 18
   */
  export enum InstanceType {
    /**
     * Indicates it is the main VM instance.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @atomicservice
     * @since 18
     */
    MAIN = 0,

    /**
     * Indicates it is the worker VM instance.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @atomicservice
     * @since 18
     */
     WORKER = 1,

    /**
     * Indicates it is the taskpool VM instance.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @atomicservice
     * @since 18
     */
    TASKPOOL = 2,

    /**
     * Indicates it is a VM instance created by napi_create_ark_runtime from native code by user.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @atomicservice
     * @since 18
     */
    CUSTOM = 3,
  }

  /**
   * Register loop observer. This function can only by called from main thread,
   * and if call this function multiple times, the last
   * modification will overwrite the previous one.
   * 
   * @param { 'loopObserver' } type - loopObserver.
   * @param { number } timeout - Indicates timeout(ms) value of loop observer.
   * @param { LoopObserver } observer - The loop observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Register loop observer. This function can only by called from main thread,
   * and if call this function multiple times, the last
   * modification will overwrite the previous one.
   * 
   * @param { 'loopObserver' } type - loopObserver.
   * @param { number } timeout - Indicates timeout(ms) value of loop observer.
   * @param { LoopObserver } observer - The loop observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function on(type: 'loopObserver', timeout: number, observer: LoopObserver): void;

  /**
   * Unregister loop observer. This function can only by called from main thread.
   *
   * @param { 'loopObserver' } type - loopObserver.
   * @param { LoopObserver } observer - The loop observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Unregister loop observer. This function can only by called from main thread.
   *
   * @param { 'loopObserver' } type - loopObserver.
   * @param { LoopObserver } observer - The loop observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function off(type: 'loopObserver', observer?: LoopObserver): void;

  /**
   * Register unhandled rejection observer.
   *
   * @param { 'unhandledRejection' } type - 'unhandledRejection'.
   * @param { UnhandledRejectionObserver } observer - The unhandled rejection observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Register unhandled rejection observer.
   *
   * @param { 'unhandledRejection' } type - 'unhandledRejection'.
   * @param { UnhandledRejectionObserver } observer - The unhandled rejection observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function on(type: 'unhandledRejection', observer: UnhandledRejectionObserver): void;

  /**
   * Unregister unhandled rejection observer.
   *
   * @param { 'unhandledRejection' } type - error.
   * @param { UnhandledRejectionObserver } [observer]  - the registered observer
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @throws { BusinessError } 16300004 - The observer does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * Unregister unhandled rejection observer.
   *
   * @param { 'unhandledRejection' } type - error.
   * @param { UnhandledRejectionObserver } [observer]  - the registered observer
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16200001 - The caller has been released.
   * @throws { BusinessError } 16300004 - The observer does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  function off(type: 'unhandledRejection', observer?: UnhandledRejectionObserver): void;

  /**
   * Register an observer for freeze event.
   * This function can only be called from main thread.
   * Please note that each process only supports registering one observer.
   * If you register multiple times, the later one will overwrite the previous one.
   *
   * @param { 'freeze' } type - 'freeze'.
   * @param { FreezeObserver } observer - The freeze event observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
    function on(type: 'freeze', observer: FreezeObserver): void;

  /**
   * Unregister the observer for freeze event.
   * This function can only be called from main thread.
   *
   * @param { 'freeze' } type - 'freeze'.
   * @param { FreezeObserver } observer - The freeze event observer.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 16300004 - The observer does not exist.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  function off(type: 'freeze', observer?: FreezeObserver): void;

  /**
   * The observer will be called by system when an error occurs.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * The observer will be called by system when an error occurs.
   *
   * @typedef { _ErrorObserver.default }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 11
   */
  /**
   * The observer will be called by system when an error occurs.
   *
   * @typedef { _ErrorObserver.default }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  export type ErrorObserver = _ErrorObserver.default;
  /**
   * The observer will be called when application main thread execute timeout.
   *
   * @typedef { _LoopObserver }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * The observer will be called when application main thread execute timeout.
   *
   * @typedef { _LoopObserver }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  export type LoopObserver = _LoopObserver;
  /**
   * The observer will be called by system when an unhandled rejection occurs.
   *
   * @typedef { function }
   * { Error | any } reason - the reason of the rejection, typically of Error type
   * { Promise<any> } promise - the promise that is rejected
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  /**
   * The observer will be called by system when an unhandled rejection occurs.
   *
   * @typedef { function }
   * { Error | any } reason - the reason of the rejection, typically of Error type
   * { Promise<any> } promise - the promise that is rejected
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @crossplatform
   * @atomicservice
   * @since 19
   */
  export type UnhandledRejectionObserver = (reason: Error | any, promise: Promise<any>) => void;
  /**
   * The observer will be called by system when freeze happens.
   *
   * @typedef { function }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 18
   */
  export type FreezeObserver = () => void;
}

export default errorManager;
