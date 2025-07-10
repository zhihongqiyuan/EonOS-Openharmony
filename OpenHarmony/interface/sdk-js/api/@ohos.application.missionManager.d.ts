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
import { MissionInfo } from './application/MissionInfo';
import { MissionListener } from './application/MissionListener';
import { MissionSnapshot } from './application/MissionSnapshot';
import StartOptions from './@ohos.app.ability.StartOptions';

/**
 * This module provides the capability to manage abilities and obtaining system task information.
 *
 * @namespace missionManager
 * @permission ohos.permission.MANAGE_MISSIONS
 * @syscap SystemCapability.Ability.AbilityRuntime.Mission
 * @systemapi
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.app.ability.missionManager/missionManager
 */
declare namespace missionManager {
  /**
   * Register the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { MissionListener } listener - Indicates the MissionListener to be registered.
   * @returns { number } The index number of the MissionListener.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#on
   */
  function registerMissionListener(listener: MissionListener): number;

  /**
   * Unregister the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } listenerId - Indicates the listener id to be unregistered.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#off
   */
  function unregisterMissionListener(listenerId: number, callback: AsyncCallback<void>): void;

  /**
   * Unregister the missionListener to ams.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } listenerId - Indicates the listener id to be unregistered.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#off
   */
  function unregisterMissionListener(listenerId: number): Promise<void>;

  /**
   * Get the missionInfo with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } missionId - Indicates mission id to be queried.
   * @param { AsyncCallback<MissionInfo> } callback - Execute the result callback function to return task information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionInfo
   */
  function getMissionInfo(deviceId: string, missionId: number, callback: AsyncCallback<MissionInfo>): void;

  /**
   * Get the missionInfo with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } missionId - Indicates mission id to be queried.
   * @returns { Promise<MissionInfo> } the {@link MissionInfo} of the given id.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionInfo
   */
  function getMissionInfo(deviceId: string, missionId: number): Promise<MissionInfo>;

  /**
   * Get the missionInfo with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } numMax - Indicates the maximum number of returned missions.
   * @param { AsyncCallback<Array<MissionInfo>> } callback - Execute the result callback function to return an array of
   *                                                         task information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionInfos
   */
  function getMissionInfos(deviceId: string, numMax: number, callback: AsyncCallback<Array<MissionInfo>>): void;

  /**
   * Get the missionInfo with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } numMax - Indicates the maximum number of returned missions.
   * @returns { Promise<Array<MissionInfo>> } The array of the {@link MissionInfo}.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionInfos
   */
  function getMissionInfos(deviceId: string, numMax: number): Promise<Array<MissionInfo>>;

  /**
   * Get the mission snapshot with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } missionId - Indicates mission id to be queried.
   * @param { AsyncCallback<MissionSnapshot> } callback - Execute the result callback function to return task snapshot
   *                                                      information.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionSnapShot
   */
  function getMissionSnapShot(deviceId: string, missionId: number, callback: AsyncCallback<MissionSnapshot>): void;

  /**
   * Get the mission snapshot with the given missionId.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { string } deviceId - Indicates the device to be queried.
   * @param { number } missionId - Indicates mission id to be queried.
   * @returns { Promise<MissionSnapshot> } The {@link MissionSnapshot} of the given id.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#getMissionSnapShot
   */
  function getMissionSnapShot(deviceId: string, missionId: number): Promise<MissionSnapshot>;

  /**
   * Lock the mission.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be locked.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#lockMission
   */
  function lockMission(missionId: number, callback: AsyncCallback<void>): void;

  /**
   * Lock the mission.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be locked.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#lockMission
   */
  function lockMission(missionId: number): Promise<void>;

  /**
   * Unlock the mission.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be unlocked.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#unlockMission
   */
  function unlockMission(missionId: number, callback: AsyncCallback<void>): void;

  /**
   * Unlock the mission.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be unlocked.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#unlockMission
   */
  function unlockMission(missionId: number): Promise<void>;

  /**
   * Clear the given mission in the ability manager service.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be cleared.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#clearMission
   */
  function clearMission(missionId: number, callback: AsyncCallback<void>): void;

  /**
   * Clear the given mission in the ability manager service.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be cleared.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#clearMission
   */
  function clearMission(missionId: number): Promise<void>;

  /**
   * Clear all missions in the ability manager service.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { AsyncCallback<void> } callback - Returns the callback result in the form of a callback.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#clearAllMissions
   */
  function clearAllMissions(callback: AsyncCallback<void>): void;

  /**
   * Clear all missions in the ability manager service.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#clearAllMissions
   */
  function clearAllMissions(): Promise<void>;

  /**
   * Schedule the given mission to foreground.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be moved to foreground.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#moveMissionToFront
   */
  function moveMissionToFront(missionId: number, callback: AsyncCallback<void>): void;

  /**
   * Schedule the given mission to foreground.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be moved to foreground.
   * @param { StartOptions } options - Indicates the start options.
   * @param { AsyncCallback<void> } callback - Execution result callback function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#moveMissionToFront
   */
  function moveMissionToFront(missionId: number, options: StartOptions, callback: AsyncCallback<void>): void;

  /**
   * Schedule the given mission to foreground.
   *
   * @permission ohos.permission.MANAGE_MISSIONS
   * @param { number } missionId - Indicates mission id to be moved to foreground.
   * @param { StartOptions } [options] - Indicates the start options.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Mission
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.missionManager/missionManager#moveMissionToFront
   */
  function moveMissionToFront(missionId: number, options?: StartOptions): Promise<void>;
}

export default missionManager;
