/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
 */

import { AsyncCallback } from './@ohos.base';
import { Configuration } from './@ohos.application.Configuration';
import { AbilityRunningInfo } from './application/AbilityRunningInfo';
import { ExtensionRunningInfo } from './application/ExtensionRunningInfo';
import { ElementName } from './bundle/elementName';

/**
 * The class of an ability manager.
 *
 * @namespace abilityManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @systemapi
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.app.ability.abilityManager/abilityManager
 */
declare namespace abilityManager {
  /**
   * Status information for the Ability.
   *
   * @enum { number }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.abilityManager/abilityManager#AbilityState
   */
  export enum AbilityState {
    /**
     * Indicates that the ability is initial.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.app.ability.abilityManager/abilityManager.AbilityState#INITIAL
     */
    INITIAL = 0,

    /**
     * Indicates that the ability is foreground.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.app.ability.abilityManager/abilityManager.AbilityState#FOREGROUND
     */
    FOREGROUND = 9,

    /**
     * Indicates that the ability is background.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.app.ability.abilityManager/abilityManager.AbilityState#BACKGROUND
     */
    BACKGROUND = 10,

    /**
     * Indicates that the ability is foregrounding.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.app.ability.abilityManager/abilityManager.AbilityState#FOREGROUNDING
     */
    FOREGROUNDING = 11,

    /**
     * Indicates that the ability is backgrounding.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.Core
     * @systemapi
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.app.ability.abilityManager/abilityManager.AbilityState#BACKGROUNDING
     */
    BACKGROUNDING = 12
  }

  /**
   * Updates the configuration by modifying the configuration.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { Configuration } config - Indicates the new configuration.
   * @param { AsyncCallback<void> } callback - The specified callback method.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.abilityManager/abilityManager#updateConfiguration
   */
  function updateConfiguration(config: Configuration, callback: AsyncCallback<void>): void;

  /**
   * Updates the configuration by modifying the configuration.
   *
   * @permission ohos.permission.UPDATE_CONFIGURATION
   * @param { Configuration } config - Indicates the new configuration.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.abilityManager/abilityManager#updateConfiguration
   */
  function updateConfiguration(config: Configuration): Promise<void>;

  /**
   * Get information about running abilities
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @returns { Promise<Array<AbilityRunningInfo>> } Returns the array of {@link AbilityRunningInfo}.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.abilityManager/abilityManager#getAbilityRunningInfos
   */
  function getAbilityRunningInfos(): Promise<Array<AbilityRunningInfo>>;

  /**
   * Get information about running abilities
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { AsyncCallback<Array<AbilityRunningInfo>> } callback - The specified callback method.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.abilityManager/abilityManager#getAbilityRunningInfos
   */
  function getAbilityRunningInfos(callback: AsyncCallback<Array<AbilityRunningInfo>>): void;
}

export default abilityManager;
