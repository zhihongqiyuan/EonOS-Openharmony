/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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

import type appManager from './@ohos.app.ability.appManager';

/**
 * The definition of AbilityConstant.
 *
 * @namespace AbilityConstant
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @since 9
 */
/**
 * The definition of AbilityConstant.
 *
 * @namespace AbilityConstant
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @crossplatform
 * @since 10
 */
/**
 * The <code>AbilityConstant</code> module defines the UIAbility-related enums, including the initial launch reasons,
 * reasons for the last exit, ability continuation results, and window modes.
 *
 * @namespace AbilityConstant
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @crossplatform
 * @atomicservice
 * @since 11
 */
declare namespace AbilityConstant {
  /**
   * Interface of launch param.
   *
   * @typedef LaunchParam
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Interface of launch param.
   *
   * @typedef LaunchParam
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Defines the parameters for starting an ability.
   * The parameter values are automatically passed in by the system when the ability is started. You do not need to
   * change the values.
   *
   * @typedef LaunchParam
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  export interface LaunchParam {
    /**
     * Indicates launch reason.
     *
     * @type { LaunchReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Indicates launch reason.
     *
     * @type { LaunchReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @since 10
     */
    /**
     * Ability launch reason, which is an enumerated type.
     *
     * @type { LaunchReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    launchReason: LaunchReason;

    /**
     * Detailed message that describes the ability launch reason.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    launchReasonMessage?: string;

    /**
     * Indicates last exit reason.
     *
     * @type { LastExitReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Indicates last exit reason.
     *
     * @type { LastExitReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @since 10
     */
    /**
     * Reason for the last exit, which is an enumerated type.
     *
     * @type { LastExitReason }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    lastExitReason: LastExitReason;

    /**
     * Reason for the last exit.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 12
     */
    lastExitMessage: string;

    /**
     * Detailed information about the last exit.
     *
     * @type { LastExitDetailInfo }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    lastExitDetailInfo?: LastExitDetailInfo;
  }

  /**
   * Describes the detailed information about the last exit.
   *
   * @typedef LastExitDetailInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 18
   */
  export interface LastExitDetailInfo {
    /**
     * ID of the process where the ability is running when it exits last time.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    pid: number;

    /**
     * Name of the process.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    processName: string;

    /**
     * UID of the application.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    uid: number;

    /**
     * Specific reason for the last exit of the ability.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    exitSubReason: number;

    /**
     * Reason why the process was killed.
     *
     * @type { string }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    exitMsg: string;

    /**
     * RSS value of the process.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    rss: number;

    /**
     * PSS value of the process.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    pss: number;

    /**
     * Exact time when the ability last exits.
     *
     * @type { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    timestamp: number;

    /**
     * The process state when the process exits.
     *
     * @type { ?appManager.ProcessState }
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 20
     */
    processState?: appManager.ProcessState;
  }

  /**
   * Type of launch reason.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Type of launch reason.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the initial ability launch reasons.
   * You can use it together with the value of <code>launchParam.launchReason</code> in
   * <code>onCreate(want, launchParam)</code> of the UIAbility to complete different operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  export enum LaunchReason {
    /**
     * Unknown reason.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @since 10
     */
    /**
     * Unknown reason.
     * 
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    UNKNOWN = 0,

    /**
     * Start ability through the startAbility interface.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability is started by calling <code>startAbility</code>.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    START_ABILITY = 1,

    /**
     * Start ability through the startAbilityByCall interface.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability is started by calling <code>startAbilityByCall</code>.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CALL = 2,

    /**
     * Start ability through cross-end device migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability is started by means of cross-device migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CONTINUATION = 3,

    /**
     * After the application is restored, the ability is automatically restored and started when the application fails.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability is automatically started when the application is restored from a fault.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    APP_RECOVERY = 4,

    /**
     * Start ability through the acquireShareData interface.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability is started by means of atomic service sharing.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    SHARE = 5,

    /**
     * The ability is automatically started upon system boot.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 11
     */
    AUTO_STARTUP = 8,

    /**
     * The ability is started by the InsightIntent framework.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    INSIGHT_INTENT = 9,

    /**
     * The ability is started in advance during cross-device migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 12
     */
    PREPARE_CONTINUATION = 10,
  }

  /**
   * Type of last exit reason.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Type of last exit reason.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Enumerates the reasons for the last exit.
   * You can use it together with the value of <code>launchParam.lastExitReason</code> in
   * <code>onCreate(want, launchParam)</code> of the UIAbility to complete different operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  export enum LastExitReason {
    /**
     * Exit reason : Unknown. The reason for the last exit of the target application is not recorded in the application
     *               framework.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Exit reason : Unknown. The reason for the last exit of the target application is not recorded in the application
     *               framework.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @since 10
     */
    /**
     * Unknown reason.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @crossplatform
     * @atomicservice
     * @since 11
     */
    UNKNOWN = 0,

    /**
     * The ability does not respond.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     * @deprecated since 10
     * @useinstead AbilityConstant.LastExitReason#APP_FREEZE
     */
    ABILITY_NOT_RESPONDING = 1,

    /**
     * Exit reason : normally. App exit due to user active close.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability exits normally because the user closes the application.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    NORMAL = 2,

    /**
     * Exit reason : cpp crash. The app exit due to native exception signal.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits due to abnormal signals on the local host.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CPP_CRASH = 3,

    /**
     * Exit reason : js error. App exit due to js error.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits due to a JS_ERROR fault triggered when an application has a JS syntax error that is not
     * captured by developers.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    JS_ERROR = 4,

    /**
     * Exit reason : app freeze. App exit due to appFreeze error.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits because watchdog detects that the application is frozen.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    APP_FREEZE = 5,

    /**
     * Exit reason : performance control. App exit due to system performance issues, such as device low memory.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits due to system performance problems, for example, insufficient device memory.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    PERFORMANCE_CONTROL = 6,

    /**
     * Exit reason : resource control. App exit due to resource usage violation, such as exceed cpu/io/memory usage.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits due to improper use of system resources. The specific error cause can be obtained through
     * LaunchParam.lastExitMessage. The possible causes are as follows:
     * - CPU Highload: The CPU load is high.
     * - CPU_EXT Highload: A fast CPU load detection is carried out.
     * - IO Manage Control: An I/O management and control operation is carried out.
     * - App Memory Deterioration: The application memory usage exceeds the threshold.
     * - Temperature Control: The temperature is too high or too low.
     * - Memory Pressure: The system is low on memory, triggering ability exiting in ascending order of priority.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    RESOURCE_CONTROL = 7,

    /**
     * Exit reason : upgrade. App exit due to upgrade.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * The ability exits due to an update.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    UPGRADE = 8,

    /**
     * The ability exits because of an action in the multitasking center, for example, when users swipe up or hit the
     * one-click clean button in the multitasking view.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    USER_REQUEST = 9,

    /**
     * The ability exits because it receives a kill signal from the system.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 18
     */
    SIGNAL = 10
  }

  /**
   * Type of onContinue result.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Enumerates the ability continuation results.
   * You can use it together with <code>onContinue(wantParam)</code> of the UIAbility to complete different
   * operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  export enum OnContinueResult {
    /**
     * Agree to the result of Ability migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability continuation is accepted.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    AGREE = 0,

    /**
     * Reject to the result of Ability migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The ability continuation is rejected.
     * If the application is abnormal in onContinue, which results in abnormal display during data restoration, this
     * error code is returned.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    REJECT = 1,

    /**
     * Mismatch to the result of Ability migration.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * The version does not match.
     * The application on the initiator can obtain the version of the target application from onContinue. If the
     * ability continuation cannot be performed due to version mismatch, this error code is returned.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    MISMATCH = 2
  }

  /**
   * Type of memory level.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Enumerates the memory levels. You can use it in <code>onMemoryLevel(level)</code> of the UIAbility to complete
   * different operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  export enum MemoryLevel {
    /**
     * Memory footprint is moderate.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Moderate memory usage.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    MEMORY_LEVEL_MODERATE = 0,

    /**
     * Low memory footprint.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Low memory usage.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    MEMORY_LEVEL_LOW = 1,

    /**
     * High memory footprint.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * High memory usage.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    MEMORY_LEVEL_CRITICAL = 2
  }

  /**
   * Enumerates the window mode when the ability is started.
   * It can be used together with <code>startAbility</code> to specify the window mode for starting the ability.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 12
   */
  export enum WindowMode {
    /**
     * Undefined window mode.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    WINDOW_MODE_UNDEFINED = 0,

    /**
     * Full screen mode. It takes effect only on 2-in-1 devices and tablets.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 12
     */
    WINDOW_MODE_FULLSCREEN = 1,

    /**
     * Primary screen (left screen in the case of horizontal orientation) in split-screen mode. It is valid only in
     * intra-app redirection scenarios.
     * It takes effect only on foldable devices and tablets.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 12
     */
    WINDOW_MODE_SPLIT_PRIMARY = 100,

    /**
     * Secondary screen (right screen in the case of horizontal orientation) in split-screen mode. It is valid only in
     * intra-app redirection scenarios.
     * It takes effect only on foldable devices and tablets.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 12
     */
    WINDOW_MODE_SPLIT_SECONDARY = 101,

    /**
     * The ability is displayed in a floating window.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @stagemodelonly
     * @since 12
     */
    WINDOW_MODE_FLOATING = 102
  }

  /**
   * Type of onSave result.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Enumerates the result types for the operation of saving application data. You can use it in
   * <code>onSaveState(reason, wantParam)</code> of the UIAbility to complete different operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  export enum OnSaveResult {
    /**
     * Always agree to save the state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Always agreed to save the status.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    ALL_AGREE = 0,

    /**
     * Refuse to migrate the saved state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Rejected to save the status in continuation.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CONTINUATION_REJECT = 1,

    /**
     * Migration mismatch.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Continuation mismatch.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CONTINUATION_MISMATCH = 2,

    /**
     * Agree to restore the saved state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Agreed to restore the saved status.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    RECOVERY_AGREE = 3,

    /**
     * Refuse to restore the saved state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Rejected to restore the saved status.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    RECOVERY_REJECT = 4,

    /**
     * Always refuses to save the state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Always rejected to save the status.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    ALL_REJECT
  }

  /**
   * Type of save state.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Enumerates the scenarios for saving application data. You can use it in
   * <code>onSaveState(reason, wantParam)</code> of the UIAbility to complete different operations.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  export enum StateType {
    /**
     * Migrate and save the state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Saving the status in continuation.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    CONTINUATION = 0,

    /**
     * App recovery to restore the saved state.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 9
     */
    /**
     * Saving the status in application recovery.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    APP_RECOVERY = 1
  }

  /**
   * Continue state.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 10
   */
  /**
   * Enumerates the mission continuation states of the application. It is used in the
   * <code>setMissionContinueState</code> API of UIAbilityContext.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  export enum ContinueState {
    /**
     * Mission continuable active.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * Mission continuation is activated for the current application.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    ACTIVE = 0,

    /**
     * Mission continuable inactive.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 10
     */
    /**
     * Mission continuation is not activated for the current application.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 11
     */
    INACTIVE = 1
  }

  /**
   * Enumerates the actions triggered when an application is closed by the user. It must be used together with
   * <code>onPrepareTermination</code> or <code>onPrepareTerminationAsync</code> of AbilityStage.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 15
   */
  export enum PrepareTermination {
    /**
     * Executes the termination action immediately. This is the default behavior.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 15
     */
    TERMINATE_IMMEDIATELY = 0,

    /**
     * Cancels the termination action.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @atomicservice
     * @since 15
     */
    CANCEL = 1
  }

  /**
   * Enumerates the collaboration request results.
   * This enum is used in multi-device collaboration scenarios to specify whether the target application accepts the
   * collaboration request from the caller application.
   * It is used in <code>onCollaborate(wantParam)</code> of the UIAbility.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 18
   */
  export enum CollaborateResult {
    /**
     * 	Accepts the collaboration request.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 18
     */
    ACCEPT = 0,

    /**
     * Rejects the collaboration request.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @stagemodelonly
     * @since 18
     */
    REJECT = 1,
  }
}

export default AbilityConstant;
