/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit BackgroundTasksKit
 */

import { AsyncCallback, Callback } from './@ohos.base';
import { WantAgent } from './@ohos.wantAgent';
import Context from './application/BaseContext';
import type notificationManager from './@ohos.notificationManager';

/**
 * Manages background tasks.
 *
 * @namespace backgroundTaskManager
 * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.Core
 * @since 9
 */
/**
 * Manages background tasks.
 *
 * @namespace backgroundTaskManager
 * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.Core
 * @atomicservice
 * @since 12
 */
declare namespace backgroundTaskManager {
  /**
   * The info of delay suspend.
   *
   * @interface DelaySuspendInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 9
   */
  interface DelaySuspendInfo {
    /**
     * The unique identifier of the delay request.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
     * @since 9
     */
    requestId: number;
    /**
     * The actual delay duration (ms).
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
     * @since 9
     */
    actualDelayTime: number;
  }

  /**
   * The callback info of transient task.
   *
   * @interface TransientTaskInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 20
   */
  interface TransientTaskInfo {
    /**
     * Total remaining quota of an application in one day.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
     * @since 20
     */
    remainingQuota: number;
    /**
     * The info list of delay suspend.
     *
     * @type { DelaySuspendInfo[] }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
     * @since 20
     */
    transientTasks: DelaySuspendInfo[];
  }

  /**
   * The info of continuous task notification.
   *
   * @interface ContinuousTaskNotification
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  interface ContinuousTaskNotification {
    /**
     * The notification slot type.
     *
     * @type { notificationManager.SlotType }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @atomicservice
     * @since 12
     */
    slotType: notificationManager.SlotType;
    /**
     * The notification content type.
     *
     * @type { notificationManager.ContentType }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @atomicservice
     * @since 12
     */
    contentType: notificationManager.ContentType;
    /**
     * The notification id.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @atomicservice
     * @since 12
     */
    notificationId: number;
    /**
     * The continuous task id.
     * @type { ?number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    continuousTaskId?: number;
  }

  /**
   * The continuous task cancel info.
   *
   * @interface ContinuousTaskCancelInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 15
   */
  interface ContinuousTaskCancelInfo {
    /**
     * The cancel reason of continuous task.
     *
     * @type { ContinuousTaskCancelReason }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    reason: ContinuousTaskCancelReason;

    /**
     * The id of cancelled continuous task.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    id: number;
  }

  /**
   * The continuous task active info.
   *
   * @interface ContinuousTaskActiveInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  interface ContinuousTaskActiveInfo {
    /**
     * The id of active continuous task.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    id: number;
  }

  /**
   * The continuous task info.
   *
   * @interface ContinuousTaskInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  interface ContinuousTaskInfo {
    /**
     * The ability name of apply continuous task.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    abilityName: string;
   /**
     * The uid of apply continuous task.
     *
     * @type { number}
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    uid: number;
   /**
     * The pid of apply continuous task.
     *
     * @type { number}
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    pid: number;
    /**
     * Is apply continuous task from webview.
     *
     * @type { boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    isFromWebView: boolean;
    /**
     * Background modes of apply continuous task.
     *
     * @type { string[] }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    backgroundModes: string[];
    /**
     * Background sub modes of apply continuous task.
     *
     * @type { string[] }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    backgroundSubModes: string[];
    /**
     * The notification id of apply continuous task.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    notificationId: number;
    /**
     * The continuous task id of apply continuous task.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    continuousTaskId: number;
   /**
     * The ability id of apply continuous task.
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    abilityId: number;
    /**
     * The wantAgent bundle name of apply continuous task.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    wantAgentBundleName: string;
    /**
     * The wantAgent ability name of apply continuous task.
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    wantAgentAbilityName: string;
  }

  /**
   * The continuous task suspend info.
   *
   * @interface ContinuousTaskSuspendInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  interface ContinuousTaskSuspendInfo {
    /**
     * The id of suspended continuous task.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    continuousTaskId: number;

    /**
     * The suspend state of continuous task.
     *
     * @type { boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    suspendState: boolean;

    /**
     * The suspend reason of continuous task.
     *
     * @type { ContinuousTaskSuspendReason }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    suspendReason: ContinuousTaskSuspendReason;
  }

  /**
   * Efficiency Resources information.
   *
   * @interface EfficiencyResourcesInfo
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi Hide this for inner system use.
   * @since 20
   */
  interface EfficiencyResourcesInfo {
    /**
     * The set of resource types that app wants to apply.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    resourceTypes: number;
    /**
     * The duration that the resource can be used most.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    timeout: number;
    /**
     * True if the apply action is persistent, else false. Default value is false.
     *
     * @type { boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    isPersistent: boolean;
    /**
     * True if apply action is for process, false is for package. Default value is false.
     *
     * @type { boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    isForProcess: boolean;
    /**
     * The apply reason.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    reason: string;
   /**
     * The uid of apply efficiency resources.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    uid: number;
   /**
     * The pid of apply efficiency resources.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 20
     */
    pid: number;
  }

  /**
   * Cancels delayed transition to the suspended state.
   *
   * @param { number } requestId - The identifier of the delay request.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9900001 - Caller information verification failed for a transient task.
   * @throws { BusinessError } 9900002 - Transient task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 9
   */
  function cancelSuspendDelay(requestId: number): void;

  /**
   * Obtains the remaining time before an application enters the suspended state.
   *
   * @param { number } requestId - The identifier of the delay request.
   * @param { AsyncCallback<number> } callback - The callback of the remaining delay time.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9900001 - Caller information verification failed for a transient task.
   * @throws { BusinessError } 9900002 - Transient task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 9
   */
  function getRemainingDelayTime(requestId: number, callback: AsyncCallback<number>): void;

  /**
   * Obtains the remaining time before an application enters the suspended state.
   *
   * @param { number } requestId - The identifier of the delay request.
   * @returns { Promise<number> } The promise returns the remaining delay time.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9900001 - Caller information verification failed for a transient task.
   * @throws { BusinessError } 9900002 - Transient task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 9
   */
  function getRemainingDelayTime(requestId: number): Promise<number>;

  /**
   * Requests delayed transition to the suspended state.
   *
   * @param { string } reason - Indicates the reason for delayed transition to the suspended state.
   * @param { Callback<void> } callback - The callback delay time expired.
   * @returns { DelaySuspendInfo } Info of delay request.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9900001 - Caller information verification failed for a transient task.
   * @throws { BusinessError } 9900002 - Transient task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 9
   */
  function requestSuspendDelay(reason: string, callback: Callback<void>): DelaySuspendInfo;

  /**
   * Obtains transient task info before an application enters the suspended state.
   *
   * @returns { Promise<TransientTaskInfo> } The promise returns the transient task info.
   * @throws { BusinessError } 9900001 - Caller information verification failed for a transient task.
   * @throws { BusinessError } 9900003 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9900004 - System service operation failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.TransientTask
   * @since 20
   */
  function getTransientTaskInfo(): Promise<TransientTaskInfo>;

  /**
   * Service ability uses this method to request start running in background.
   * <p> System will publish a notification related to the this service. </p>
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { BackgroundMode } bgMode - Indicates which background mode to request.
   * @param { WantAgent } wantAgent - Indicates which ability to start when user click the notification bar.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 9
   */
  /**
   * Service ability uses this method to request start running in background.
   * <p> System will publish a notification related to the this service. </p>
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { BackgroundMode } bgMode - Indicates which background mode to request.
   * @param { WantAgent } wantAgent - Indicates which ability to start when user click the notification bar.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  function startBackgroundRunning(context: Context, bgMode: BackgroundMode, wantAgent: WantAgent, callback: AsyncCallback<void>): void;

  /**
   * Service ability uses this method to request start running in background.
   * <p> System will publish a notification related to the this service. </p>
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { BackgroundMode } bgMode - Indicates which background mode to request.
   * @param { WantAgent } wantAgent - Indicates which ability to start when user click the notification bar.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 9
   */
  /**
   * Service ability uses this method to request start running in background.
   * <p> System will publish a notification related to the this service. </p>
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { BackgroundMode } bgMode - Indicates which background mode to request.
   * @param { WantAgent } wantAgent - Indicates which ability to start when user click the notification bar.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  function startBackgroundRunning(context: Context, bgMode: BackgroundMode, wantAgent: WantAgent): Promise<void>;

  /**
   * UIAbility uses this method to request start running in background.
   * <p> System will publish a notification related to the UIAbility. </p>
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { string[] } bgModes - Indicates which background mode to request.
   * @param { WantAgent } wantAgent - Indicates which ability to start when user click the notification bar.
   * @returns { Promise<ContinuousTaskNotification> } The The continuous task notification.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  function startBackgroundRunning(context: Context, bgModes: string[], wantAgent: WantAgent): Promise<ContinuousTaskNotification>;

  /**
   * UIAbility uses this method to update background mode.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @param { string[] } bgModes - Indicates which background mode to request.
   * @returns { Promise<ContinuousTaskNotification> } The continuous task notification.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  function updateBackgroundRunning(context: Context, bgModes: string[]): Promise<ContinuousTaskNotification>;

  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 9
   */
  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @param { AsyncCallback<void> } callback - The callback of the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 19
   */
  function stopBackgroundRunning(context: Context, callback: AsyncCallback<void>): void;

  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 9
   */
  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Parcel operation failed.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  /**
   * Service ability uses this method to request stop running in background.
   *
   * @param { Context } context - App running context.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @throws { BusinessError } 9800006 - Notification verification failed for a continuous task.
   * @throws { BusinessError } 9800007 - Continuous task storage failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 19
   */
  function stopBackgroundRunning(context: Context): Promise<void>;

  /**
   * Obtains all the continuous tasks before an application enters the suspended state.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { Context } context - App running context.
   * @returns { Promise<ContinuousTaskInfo[]> } The promise returns the continuous task info.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  function getAllContinuousTasks(context: Context): Promise<ContinuousTaskInfo[]>;

  /**
   * Apply or unapply efficiency resources.
   *
   * @param { EfficiencyResourcesRequest } request - The request of apply or unapply efficiency resources.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br> 2. Incorrect parameters types; 3. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - Failed to get bgtask manager service, necessary system service is not ready.
   * @throws { BusinessError } 18700001 - Caller information verification failed for an energy resource request.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function applyEfficiencyResources(request: EfficiencyResourcesRequest): void;

  /**
   * Reset all efficiency resources apply.
   *
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Parameter verification failed.
   * @throws { BusinessError } 9800001 - Memory operation failed.
   * @throws { BusinessError } 9800002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 9800003 - Internal transaction failed.
   * @throws { BusinessError } 9800004 - System service operation failed.
   * @throws { BusinessError } 18700001 - Caller information verification failed for an energy resource request.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function resetAllEfficiencyResources(): void;

  /**
   * Obtains all the efficiency resources of current application.
   *
   * @returns { Promise<EfficiencyResourcesInfo[]> } The promise returns the efficiency resources info.
   * @throws { BusinessError } 202 - Not System App.
   * @throws { BusinessError } 18700001 - Caller information verification failed for an energy resource request.
   * @throws { BusinessError } 18700002 - Failed to write data into parcel. Possible reasons: 1. Invalid parameters;
   * <br> 2. Failed to apply for memory.
   * @throws { BusinessError } 18700004 - System service operation failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi
   * @since 20
   */
  function getAllEfficiencyResources(): Promise<EfficiencyResourcesInfo[]>;

  /**
   * Register continuous task cancel callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskCancel' } type - The type of continuous task cancel.
   * @param { Callback<ContinuousTaskCancelInfo> } callback - the callback of continuous task cancel.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Callback parameter error;
   * <br> 2. Register a exist callback type; 3. Parameter verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 15
   */
  function on(type: 'continuousTaskCancel', callback: Callback<ContinuousTaskCancelInfo>): void;

  /**
   * Unregister continuous task cancel callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskCancel' } type - The type of continuous task cancel.
   * @param { Callback<ContinuousTaskCancelInfo> } callback - the callback of continuous task cancel.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible cause: 1. Callback parameter error;
   * <br> 2. Unregister type has not register; 3. Parameter verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 15
   */
  function off(type: 'continuousTaskCancel', callback?: Callback<ContinuousTaskCancelInfo>): void;

  /**
   * Register continuous task suspend callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskSuspend' } type - The type of continuous task suspend.
   * @param { Callback<ContinuousTaskSuspendInfo> } callback - the callback of continuous task suspend.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  function on(type: 'continuousTaskSuspend', callback: Callback<ContinuousTaskSuspendInfo>): void;

  /**
   * Unregister continuous task suspend callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskSuspend' } type - The type of continuous task suspend.
   * @param { Callback<ContinuousTaskSuspendInfo> } callback - the callback of continuous task suspend.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  function off(type: 'continuousTaskSuspend', callback?: Callback<ContinuousTaskSuspendInfo>): void;

  /**
   * Register continuous task active callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskActive' } type - The type of continuous task active.
   * @param { Callback<ContinuousTaskActiveInfo> } callback - the callback of continuous task active.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  function on(type: 'continuousTaskActive', callback: Callback<ContinuousTaskActiveInfo>): void;

  /**
   * Unregister continuous task suspend callback.
   *
   * @permission ohos.permission.KEEP_BACKGROUND_RUNNING
   * @param { 'continuousTaskActive' } type - The type of continuous task active.
   * @param { Callback<ContinuousTaskActiveInfo> } callback - the callback of continuous task active.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 9800005 - Continuous task verification failed.
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  function off(type: 'continuousTaskActive', callback?: Callback<ContinuousTaskActiveInfo>): void;

  /**
   * Supported background mode.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 9
   */
  /**
   * Supported background mode.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @atomicservice
   * @since 12
   */
  export enum BackgroundMode {
    /**
     * data transfer mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    DATA_TRANSFER = 1,

    /**
     * audio playback mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    /**
     * audio playback mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @atomicservice
     * @since 12
     */
    AUDIO_PLAYBACK = 2,

    /**
     * audio recording mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    AUDIO_RECORDING = 3,

    /**
     * location mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    LOCATION = 4,

    /**
     * bluetooth interaction mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    BLUETOOTH_INTERACTION = 5,

    /**
     * multi-device connection mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    /**
     * multi-device connection mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @atomicservice
     * @since 12
     */
    MULTI_DEVICE_CONNECTION = 6,

    /**
     * wifi interaction mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    WIFI_INTERACTION = 7,

    /**
     * Voice over Internet Phone mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 13
     */
    VOIP = 8,

    /**
     * background continuous calculate mode, for example 3D render.
     * only supported in particular device
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 9
     */
    TASK_KEEPING = 9,
  }

  /**
   * The type of resource.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export enum ResourceType {
    /**
     * The cpu resource for not being suspended.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    CPU = 1,

    /**
     * The resource for not being proxyed common_event.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    COMMON_EVENT = 1 << 1,

    /**
     * The resource for not being proxyed timer.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    TIMER = 1 << 2,

    /**
     * The resource for not being proxyed workscheduler.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    WORK_SCHEDULER = 1 << 3,

    /**
     * The resource for not being proxyed bluetooth.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    BLUETOOTH = 1 << 4,

    /**
     * The resource for not being proxyed gps.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    GPS = 1 << 5,

    /**
     * The resource for not being proxyed audio.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    AUDIO = 1 << 6,

    /**
     * The resource for not being proxyed running lock.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    RUNNING_LOCK = 1 << 7,

    /**
     * The resource for not being proxyed sensor.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SENSOR = 1 << 8
  }

  /**
   * The request of efficiency resources.
   *
   * @interface EfficiencyResourcesRequest
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export interface EfficiencyResourcesRequest {
    /**
     * The set of resource types that app wants to apply.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    resourceTypes: number;

    /**
     * True if the app begin to use, else false.
     *
     * @type { boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    isApply: boolean;

    /**
     * The duration that the resource can be used most.
     *
     * @type { number }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    timeOut: number;

    /**
     * True if the apply action is persist, else false. Default value is false.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    isPersist?: boolean;

    /**
     * True if apply action is for process, false is for package. Default value is false.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    isProcess?: boolean;

    /**
     * The apply reason.
     *
     * @type { string }
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.EfficiencyResourcesApply
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    reason: string;
  }

  /**
   * The type of continuous task cancel reason.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 15
   */
  export enum ContinuousTaskCancelReason {
    /**
     * User cancel.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    USER_CANCEL = 1,
    /**
     * System cancel.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL = 2,
    /**
     * User remove notification.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    USER_CANCEL_REMOVE_NOTIFICATION = 3,

    /**
     * Low network speed when request data transfer mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_DATA_TRANSFER_LOW_SPEED = 4,

    /**
     *  Not use avsession when request audio playback mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_AUDIO_PLAYBACK_NOT_USE_AVSESSION = 5,

    /**
     * Audio is not running when request audio playback mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_AUDIO_PLAYBACK_NOT_RUNNING = 6,

    /**
     * Audio is not running when request audio recording mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_AUDIO_RECORDING_NOT_RUNNING = 7,

    /**
     * Not use location when request location mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_NOT_USE_LOCATION = 8,

    /**
     * Not use bluetooth when request bluetooth interaction mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_NOT_USE_BLUETOOTH = 9,

    /**
     * Not use multi device when request multi-device connection mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_NOT_USE_MULTI_DEVICE = 10,

    /**
     * Use some mode illegally.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 15
     */
    SYSTEM_CANCEL_USE_ILLEGALLY = 11,
  }

  /**
   * Supported background submode.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 16
   */
  export enum BackgroundSubMode {
    /**
     * bluetooth car key mode
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 16
     */
    CAR_KEY = 1
  }

  /**
   * Supported background mode type.
   *
   * @enum { string }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 16
   */
  export enum BackgroundModeType {
    /**
     * subMode type
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 16
     */
    SUB_MODE = 'subMode'
  }

  /**
   * Type of continuous task suspend reason.
   *
   * @enum { number }
   * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
   * @since 20
   */
  export enum ContinuousTaskSuspendReason {
    /**
     * Low network speed when request data transfer mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_DATA_TRANSFER_LOW_SPEED = 4,
  
    /**
     *  Not use avsession when request audio playback mode.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_AUDIO_PLAYBACK_NOT_USE_AVSESSION = 5,
  
    /**
     * Audio is not running when request audio playback mode.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_AUDIO_PLAYBACK_NOT_RUNNING = 6,
  
    /**
     * Audio is not running when request audio recording mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_AUDIO_RECORDING_NOT_RUNNING = 7,
  
    /**
     * Not use location when request location mode.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_LOCATION_NOT_USED = 8,
  
    /**
     * Not use bluetooth when request bluetooth interaction mode.
     *
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_BLUETOOTH_NOT_USED = 9,
  
    /**
     * Not use multi device when request multi-device connection mode.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_MULTI_DEVICE_NOT_USED = 10,
  
    /**
     * Use some mode illegally.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_USED_ILLEGALLY = 11,

    /**
     * System load warning.
     * 
     * @syscap SystemCapability.ResourceSchedule.BackgroundTaskManager.ContinuousTask
     * @since 20
     */
    SYSTEM_SUSPEND_SYSTEM_LOAD_WARNING = 12,
  }
}

export default backgroundTaskManager;
