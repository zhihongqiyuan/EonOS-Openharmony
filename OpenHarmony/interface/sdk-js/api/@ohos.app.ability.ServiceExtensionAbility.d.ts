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

import rpc from './@ohos.rpc';
import ServiceExtensionContext from './application/ServiceExtensionContext';
import Want from './@ohos.app.ability.Want';
import { Configuration } from './@ohos.app.ability.Configuration';

/**
 * class of service extension ability.
 *
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @systemapi
 * @StageModelOnly
 * @since 9
 */
export default class ServiceExtensionAbility {
  /**
   * Indicates service extension ability context.
   *
   * @type { ServiceExtensionContext }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  context: ServiceExtensionContext;

  /**
   * Called back when a service extension is started for initialization.
   *
   * @param { Want } want - Indicates the want of created service extension.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onCreate(want: Want): void;

  /**
   * Called back before a service extension is destroyed.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onDestroy(): void;

  /**
   * Called back when a service extension is started.
   *
   * @param { Want } want - Indicates the want of service extension to start.
   * @param { number } startId - Indicates the number of times the service extension has been started.
   *                             The {@code startId} is incremented by 1 every time the service extension is started.
   *                             For example, if the service extension has been started for six times.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onRequest(want: Want, startId: number): void;

  /**
   * Called back when a service extension is first connected to an ability.
   *
   * @param { Want } want - Indicates connection information about the Service ability.
   * @returns { rpc.RemoteObject | Promise<rpc.RemoteObject> } A RemoteObject for communication between the client
   *                                                           and server.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onConnect(want: Want): rpc.RemoteObject | Promise<rpc.RemoteObject>;

  /**
   * Called back when all abilities connected to a service extension are disconnected.
   *
   * @param { Want } want - Indicates disconnection information about the service extension.
   * @returns { void | Promise<void> } the promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onDisconnect(want: Want): void | Promise<void>;

  /**
   * Called when a new client attempts to connect to a service extension after all previous client connections to it
   * are disconnected.
   *
   * @param { Want } want - Indicates the want of the service extension being connected.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onReconnect(want: Want): void;

  /**
   * Called when the system configuration is updated.
   *
   * @param { Configuration } newConfig - Indicates the updated configuration.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onConfigurationUpdate(newConfig: Configuration): void;

  /**
   * Called when dump client information is required.
   * It is recommended that developers don't DUMP sensitive information.
   *
   * @param { Array<string> } params - Indicates the params from command.
   * @returns { Array<string> } The dump info array.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @StageModelOnly
   * @since 9
   */
  onDump(params: Array<string>): Array<string>;
}
