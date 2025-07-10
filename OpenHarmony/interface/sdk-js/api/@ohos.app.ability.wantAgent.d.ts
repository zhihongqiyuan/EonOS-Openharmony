/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
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

import { AsyncCallback, Callback } from './@ohos.base';
import Want from './@ohos.app.ability.Want';
import { WantAgentInfo as _WantAgentInfo } from './wantAgent/wantAgentInfo';
import { TriggerInfo as _TriggerInfo } from './wantAgent/triggerInfo';

/**
 * Provide the method obtain trigger, cancel, and compare and to obtain
 * the bundle name, UID of an WantAgent object.
 *
 * @namespace wantAgent
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 9
 */
/**
 * app.ability.WantAgent is a class that encapsulates a {@link Want} object and allows the application to execute the
 * Want at a future time point. The module provides APIs for creating and comparing WantAgent objects, and obtaining
 * the user ID and bundle name of a WantAgent object.
 * 
 * A typical use scenario of WantAgent is notification processing. For example, when a user touches a notification,
 * the {@link trigger} API of WantAgent is triggered and the target application is started. For details, see
 * {@link Notification}. You are advised to use this module, since it will replace the @ohos.wantAgent module
 * in the near future.
 *
 * @namespace wantAgent
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @atomicservice
 * @since 12
 */
declare namespace wantAgent {
  /**
   * Obtains the bundle name of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { AsyncCallback<string> } callback - The callback is used to return the bundle name.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the bundle name of a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { AsyncCallback<string> } callback - Callback used to return the bundle name.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getBundleName(agent: WantAgent, callback: AsyncCallback<string>): void;

  /**
   * Obtains the bundle name of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @returns { Promise<string> } Returns the bundle name.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the bundle name of a WantAgent object.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @returns { Promise<string> } Promise used to return the bundle name.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getBundleName(agent: WantAgent): Promise<string>;

  /**
   * Obtains the UID of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { AsyncCallback<number> } callback - The callback is used to return the UID.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the user ID of a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { AsyncCallback<number> } callback - Callback used to return the user ID.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getUid(agent: WantAgent, callback: AsyncCallback<number>): void;

  /**
   * Obtains the UID of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @returns { Promise<number> } Returns the UID.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the user ID of a WantAgent object.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @returns { Promise<number> } Promise used to return the user ID.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getUid(agent: WantAgent): Promise<number>;

  /**
   * Obtains the Want in a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { AsyncCallback<Want> } callback - Callback used to return the Want.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getWant(agent: WantAgent, callback: AsyncCallback<Want>): void;

  /**
   * Obtains the Want in a WantAgent object.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @returns { Promise<Want> } Promise used to return the Want.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 9
   */
  function getWant(agent: WantAgent): Promise<Want>;

  /**
   * Cancel a WantAgent. Only the application that creates the WantAgent can cancel it.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { AsyncCallback<void> } callback - The callback of cancel.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Cancels a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function cancel(agent: WantAgent, callback: AsyncCallback<void>): void;

  /**
   * Cancel a WantAgent. Only the application that creates the WantAgent can cancel it.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Cancels a WantAgent object.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @returns { Promise<void> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function cancel(agent: WantAgent): Promise<void>;

  /**
   * Triggers a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { TriggerInfo } triggerInfo - Indicates the information required for triggering a WantAgent.
   * @param { AsyncCallback<CompleteData> } [callback] - The callback is used to return the CompleteData.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Proactively triggers a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { TriggerInfo } triggerInfo - {@link TriggerInfo} object.
   * @param { AsyncCallback<CompleteData> } [callback] - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function trigger(agent: WantAgent, triggerInfo: TriggerInfo, callback?: AsyncCallback<CompleteData>): void;

  /**
   * Checks whether two WantAgent objects are equal.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { WantAgent } otherAgent - Indicates the other WantAgent.
   * @param { AsyncCallback<boolean> } callback - Returns true if the two WantAgents are the same.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Checks whether two WantAgent objects are equal, so as to determine whether the same operation is from the
   * same application.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - The first WantAgent object.
   * @param { WantAgent } otherAgent - The second WantAgent object.
   * @param { AsyncCallback<boolean> } callback - Callback used to return the result. The value <code>true</code> means
   * that the two WantAgent objects are equal, and <code>false</code> means the opposite.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function equal(agent: WantAgent, otherAgent: WantAgent, callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether two WantAgent objects are equal.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { WantAgent } otherAgent - Indicates the other WantAgent.
   * @returns { Promise<boolean> } Returns true if the two WantAgents are the same.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Checks whether two WantAgent objects are equal, so as to determine whether the same operation is from the
   * same application.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - The first WantAgent object.
   * @param { WantAgent } otherAgent - The second WantAgent object.
   * @returns { Promise<boolean> } Promise used to return the result. The value <code>true</code> means that the two
   * WantAgent objects are equal, and <code>false</code> means the opposite.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function equal(agent: WantAgent, otherAgent: WantAgent): Promise<boolean>;

  /**
   * Obtains a WantAgent object.
   *
   * @param { WantAgentInfo } info - Information about the WantAgent object to obtain.
   * @param { AsyncCallback<WantAgent> } callback - The callback is used to return the created WantAgent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   * If the creation fails, a null WantAgent object is returned.
   * 
   * <p>**NOTE**:
   * <br>Third-party applications can set only their own abilities.
   * </p>
   *
   * @param { WantAgentInfo } info - Information about the WantAgent object to obtain.
   * @param { AsyncCallback<WantAgent> } callback - Callback used to return the WantAgent object.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getWantAgent(info: WantAgentInfo, callback: AsyncCallback<WantAgent>): void;

  /**
   * Obtains a WantAgent object.
   *
   * @param { WantAgentInfo } info - Information about the WantAgent object to obtain.
   * @returns { Promise<WantAgent> } Returns the created WantAgent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains a WantAgent object.
   * This API uses a promise to return the result.
   * If the creation fails, a null WantAgent object is returned.
   *
   * <p>**NOTE**:
   * <br>Third-party applications can set only their own abilities.
   * </p>
   * 
   * @param { WantAgentInfo } info - Information about the WantAgent object to obtain.
   * @returns { Promise<WantAgent> } Promise used to return the WantAgent object.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getWantAgent(info: WantAgentInfo): Promise<WantAgent>;

  /**
   * Obtains the {@link OperationType} of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @param { AsyncCallback<number> } callback - The callback is used to return the OperationType of the WantAgent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the operation type of a WantAgent object.
   * This API uses an asynchronous callback to return the result.
   *
   * @param { WantAgent } agent - Target WantAgent object.
   * @param { AsyncCallback<number> } callback - Callback used to return the operation type.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getOperationType(agent: WantAgent, callback: AsyncCallback<number>): void;

  /**
   * Obtains the {@link OperationType} of a WantAgent.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @returns { Promise<number> } Returns the OperationType of the WantAgent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Obtains the operation type of a WantAgent object.
   * This API uses a promise to return the result.
   *
   * @param { WantAgent } agent - Indicates the WantAgent.
   * @returns { Promise<number> } Returns the OperationType of the WantAgent.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @throws { BusinessError } 16000007 - Service busy. There are concurrent tasks. Try again later.
   * @throws { BusinessError } 16000015 - Service timeout.
   * @throws { BusinessError } 16000151 - Invalid wantAgent object.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  function getOperationType(agent: WantAgent): Promise<number>;

  /**
   * Enables or disables the WantAgent multithreading feature.
   *
   * @param { boolean } isMultithreadingSupported - Whether to enable the multithreading feature. The value
   * <code>true</code> means to enable multithreading, and <code>false</code> means the opposite.
   * @throws { BusinessError } 202 - Not System App. Interface caller is not a system app.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * 2. Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 18
   */
  function setWantAgentMultithreading(isMultithreadingSupported: boolean) : void;

  /**
   * Enumerates flags for using a WantAgent.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Enumerates the flags used by the WantAgent objects.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  export enum WantAgentFlags {
    /**
     * Indicates that the WantAgent can be used only once.
     * This flag is valid only when OperationType is set to START_ABILITY, START_SERVICE, or SEND_COMMON_EVENT.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The WantAgent object can be used only once.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    ONE_TIME_FLAG = 0,

    /**
     * Indicates that null is returned if the WantAgent does not exist.
     * This flag is valid only when OperationType is set to START_ABILITY, START_SERVICE, or SEND_COMMON_EVENT.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The WantAgent object does not exist and hence it is not created. In this case, <code>null</code> is returned.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    NO_BUILD_FLAG,

    /**
     * Indicates that the existing WantAgent should be canceled before a new object is generated.
     * This flag is valid only when OperationType is set to START_ABILITY, START_SERVICE, or SEND_COMMON_EVENT.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The existing WantAgent object should be canceled before a new object is generated.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    CANCEL_PRESENT_FLAG,

    /**
     * Indicates that the system only replaces the extra data of the existing WantAgent with that of the new object.
     * This flag is valid only when OperationType is set to START_ABILITY, START_SERVICE, or SEND_COMMON_EVENT.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Extra information of the existing WantAgent object is replaced with that of the new object.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    UPDATE_PRESENT_FLAG,

    /**
     * Indicates that the created WantAgent should be immutable.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The WantAgent object is immutable.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    CONSTANT_FLAG,

    /**
     * Indicates that the current value of element can be replaced when the WantAgent is triggered.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The element property in the current Want can be replaced by the element property in the Want passed in
     * WantAgent.trigger().
     * 
     * <p>**NOTE**:
     * <br>This processing is not supported yet.
     * </p>
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    REPLACE_ELEMENT,

    /**
     * Indicates that the current value of action can be replaced when the WantAgent is triggered.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The action property in the current Want can be replaced by the action property in the Want passed in
     * WantAgent.trigger().
     * 
     * <p>**NOTE**:
     * <br>This processing is not supported yet.
     * </p>
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    REPLACE_ACTION,

    /**
     * Indicates that the current value of uri can be replaced when the WantAgent is triggered.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The uri property in the current Want can be replaced by the uri property in the Want passed in
     * WantAgent.trigger().
     * 
     * <p>**NOTE**:
     * <br>This processing is not supported yet.
     * </p>
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    REPLACE_URI,

    /**
     * Indicates that the current value of entities can be replaced when the WantAgent is triggered.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The <code>entities</code> property in the current Want can be replaced by the <code>entities</code> property in
     * the Want passed in WantAgent.trigger().
     * 
     * <p>**NOTE**:
     * <br>This processing is not supported yet.
     * </p>
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    REPLACE_ENTITIES,

    /**
     * Indicates that the current value of packageName can be replaced when the WantAgent is triggered.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * The <code>bundleName</code> property in the current Want can be replaced by the <code>bundleName</code> property
     * in the Want passed in WantAgent.trigger().
     * 
     * <p>**NOTE**:
     * <br>This processing is not supported yet.
     * </p>
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    REPLACE_BUNDLE
  }

  /**
   * Identifies the operation for using a WantAgent, such as starting an ability or sending a common event.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Enumerates the operation types of the WantAgent objects.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  export enum OperationType {
    /**
     * Unknown operation.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Unknown operation type.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    UNKNOWN_TYPE = 0,

    /**
     * Starts an ability with a UI.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Starts an ability with a UI.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    START_ABILITY,

    /**
     * Starts multiple abilities with a UI.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Starts multiple abilities with a UI.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    START_ABILITIES,

    /**
     * Starts an ability without a UI.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Starts an ability without a UI (valid only in the FA model).
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    START_SERVICE,

    /**
     * Sends a common event.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Sends a common event.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    SEND_COMMON_EVENT,

    /**
     * Starts a service extension.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    START_SERVICE_EXTENSION = 6
  }

  /**
   * Describes the data returned by after wantAgent.trigger is called.
   *
   * @typedef CompleteData
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Describes the data returned by the operation of proactive triggering a WantAgent object.
   *
   * @typedef CompleteData
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  export interface CompleteData {
    /**
     * Triggered WantAgent.
     *
     * @type { WantAgent }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * WantAgent object that is triggered.
     *
     * @type { WantAgent }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    info: WantAgent;

    /**
     * Existing Want that is triggered.
     *
     * @type { Want }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Existing Want that is triggered.
     *
     * @type { Want }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    want: Want;

    /**
     * Request code used to trigger the WantAgent.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Request code that triggers the WantAgent object.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    finalCode: number;

    /**
     * Final data collected by the common event.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Final data collected by the common event.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    finalData: string;

    /**
     * Extra data collected by the common event.
     *
     * @type { ?object }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 9
     */
    /**
     * Extra data collected by the common event.
     *
     * @type { ?Record<string, Object> }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @since 11
     */
    /**
     * Extra information.
     *
     * @type { ?Record<string, Object> }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @atomicservice
     * @since 12
     */
    extraInfo?: Record<string, Object>;
  }

  /**
   * Provides the information required for triggering a WantAgent.
   *
   * @typedef { _TriggerInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Defines the TriggerInfo object.
   *
   * @typedef { _TriggerInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  export type TriggerInfo = _TriggerInfo;

  /**
   * Provides the information required for triggering a WantAgent.
   *
   * @typedef { _WantAgentInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 9
   */
  /**
   * Defines the WantAgentInfo object.
   *
   * @typedef { _WantAgentInfo }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @atomicservice
   * @since 12
   */
  export type WantAgentInfo = _WantAgentInfo;
}

/**
 * Target WantAgent object.
 *
 * @typedef { object }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 9
 */
export type WantAgent = object;

export default wantAgent;
