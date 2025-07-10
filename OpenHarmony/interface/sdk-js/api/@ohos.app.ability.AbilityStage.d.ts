/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

import AbilityConstant from './@ohos.app.ability.AbilityConstant';
import AbilityStageContext from './application/AbilityStageContext';
import Want from './@ohos.app.ability.Want';
import { Configuration } from './@ohos.app.ability.Configuration';

/**
 * The class of an ability stage.
 *
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @since 9
 */
/**
 * The class of an ability stage.
 *
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @crossplatform
 * @since 10
 */
/**
 * AbilityStage is a runtime class for HAP files.
 * AbilityStage notifies you of when you can perform HAP initialization such as resource pre-loading and thread
 * creation during the HAP loading.
 *
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @crossplatform
 * @atomicservice
 * @since 11
 */
export default class AbilityStage {
  /**
   * Indicates configuration information about context.
   *
   * @type { AbilityStageContext }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Indicates configuration information about context.
   *
   * @type { AbilityStageContext }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Defines the context of AbilityStage.
   * The context is obtained in the callback invoked when initialization is performed during ability startup.
   *
   * @type { AbilityStageContext }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  context: AbilityStageContext;

  /**
   * Called back when an ability stage is started for initialization.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Called back when an ability stage is started for initialization.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Called when the application is created.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  onCreate(): void;

  /**
   * Called back when start specified ability.
   *
   * @param { Want } want - Indicates the want info of started ability.
   * @returns { string } The user returns an ability string ID. If the ability of this ID has been started before,
   *         do not create a new instance and pull it back to the top of the stack.
   *         Otherwise, create a new instance and start it.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Called when a specified ability is started.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   *
   * @param { Want } want - Want information about the target ability, such as the ability name and bundle name.
   * @returns { string } Ability ID. If the ability with this ID has been started, no new instance is created and the
   * ability is placed at the top of the stack. Otherwise, a new instance is created and started.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  onAcceptWant(want: Want): string;

  /**
  * Called back asynchronously upon starting specified ability.
  *
  * @param { Want } want - Indicates the want info of the started ability.
  * @returns { Promise<string> }  The user returns an ability string ID. If the ability of this ID has been started before,
  *         do not create a new instance and pull it back to the top of the stack.
  *         Otherwise, create a new instance and start it.
  * @syscap SystemCapability.Ability.AbilityRuntime.Core
  * @stagemodelonly
  * @atomicservice
  * @since 20
  */
  onAcceptWantAsync(want: Want): Promise<string>;

  /**
   * Called when the UIAbility is started in the specified process.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   *
   * @param { Want } want - Want information about the target ability, such as the ability name and bundle name.
   * @returns { string } Custom process identifier. If the process with this identifier has been created, the ability
   * runs in the process. Otherwise, a new process is created and the ability runs in it.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 11
   */
  onNewProcessRequest(want: Want): string;

  /**
  * Called back asynchronously upon starting UIAbility in specified process.
  *
  * @param { Want } want - Indicates the want info of the started ability.
  * @returns { Promise<string> }  The user returns an ability string ID. If the ability of this ID has been started before,
  *         let the ability run in this process. Otherwise, create a new process.
  * @syscap SystemCapability.Ability.AbilityRuntime.Core
  * @stagemodelonly
  * @atomicservice
  * @since 20
  */
  onNewProcessRequestAsync(want: Want): Promise<string>;

  /**
   * Called when the system configuration is updated.
   *
   * @param { Configuration } newConfig - Indicates the updated configuration.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Called when the system configuration is updated.
   *
   * @param { Configuration } newConfig - Indicates the updated configuration.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @since 10
   */
  /**
   * Called when the global configuration is updated.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   *
   * @param { Configuration } newConfig - Callback invoked when the global configuration is updated. The global
   * configuration indicates the configuration of the environment where the application is running and includes
   * the language and color mode.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  onConfigurationUpdate(newConfig: Configuration): void;

  /**
   * Called when the system has determined to trim the memory, for example, when the ability is running in the
   * background and there is no enough memory for running as many background processes as possible.
   *
   * @param { AbilityConstant.MemoryLevel } level - Indicates the memory trim level, which shows the current memory usage status.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 9
   */
  /**
   * Listens for changes in the system memory level status.
   * When the system detects low memory resources, it will proactively invoke this callback.
   * You can implement this callback to promptly release non-essential resources (such as cached data or temporary
   * objects) upon receiving a memory shortage event, thereby preventing the application process from being forcibly
   * terminated by the system.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   *
   * @param { AbilityConstant.MemoryLevel } level - Memory level that indicates the memory usage status. When the
   * specified memory level is reached, a callback will be invoked and the system will start adjustment.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 11
   */
  onMemoryLevel(level: AbilityConstant.MemoryLevel): void;

  /**
   * Called when the application is destroyed. This API is called during the normal lifecycle.
   * If the application exits abnormally or is terminated, this API is not called.
   *
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * </p>
   * 
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 12
   */
  onDestroy(): void;

  /**
   * Called when the application is closed by the user, allowing the user to choose between immediate termination or
   * cancellation.
   * 
   * <p>**NOTE**:
   * <br>This API returns the result synchronously and does not support asynchronous callbacks.
   * <br>Currently, this API takes effect only on 2-in-1 devices.
   * <br>This API is called only when the application exits normally. It is not called if the application is forcibly
   * closed.
   * <br>This API is not executed when {@link AbilityStage.onPrepareTerminationAsync} is implemented.
   * </p>
   *
   * @permission ohos.permission.PREPARE_APP_TERMINATE
   * @returns { AbilityConstant.PrepareTermination } The user's choice.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 15
   */
  onPrepareTermination(): AbilityConstant.PrepareTermination;

  /**
   * Called when the application is closed by the user, allowing the user to choose between immediate termination or
   * cancellation.
   * This API uses a promise to return the result.
   * 
   * <p>**NOTE**:
   * <br>Currently, this API takes effect only on 2-in-1 devices.
   * <br>This API is called only when the application exits normally. It is not called if the application is forcibly
   * closed.
   * <br>If an asynchronous callback crashes, it will be handled as a timeout. If the application does not respond
   * within 10 seconds, it will be terminated forcibly.
   * </p>
   *
   * @permission ohos.permission.PREPARE_APP_TERMINATE
   * @returns { Promise<AbilityConstant.PrepareTermination> } Promise used to return the user's choice.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 15
   */
  onPrepareTerminationAsync(): Promise<AbilityConstant.PrepareTermination>;
}
