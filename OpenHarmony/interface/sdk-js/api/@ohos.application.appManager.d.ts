/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
 */

import { AsyncCallback } from './@ohos.base';
import ApplicationStateObserver from './application/ApplicationStateObserver';
import AbilityStateData from './application/AbilityStateData';
import AppStateData from './application/AppStateData';
import { ProcessRunningInfo } from './application/ProcessRunningInfo';

/**
 * This module provides the function of app manager service.
 *
 * @namespace appManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.app.ability.appManager/appManager
 */
declare namespace appManager {
  /**
   * Register application state observer.
   *
   * @permission ohos.permission.RUNNING_STATE_OBSERVER
   * @param { ApplicationStateObserver } observer - The application state observer.
   * @returns { number } Returns the number code of the observer.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#on
   */
  function registerApplicationStateObserver(observer: ApplicationStateObserver): number;

  /**
   * Unregister application state observer.
   *
   * @permission ohos.permission.RUNNING_STATE_OBSERVER
   * @param { number } observerId - Indicates the number code of the observer.
   * @param { AsyncCallback<void> } callback - Represents the specified callback method.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#off
   */
  function unregisterApplicationStateObserver(observerId: number, callback: AsyncCallback<void>): void;

  /**
   * Unregister application state observer.
   *
   * @permission ohos.permission.RUNNING_STATE_OBSERVER
   * @param { number } observerId - Indicates the number code of the observer.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#off
   */
  function unregisterApplicationStateObserver(observerId: number): Promise<void>;

  /**
   * getForegroundApplications.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { AsyncCallback<Array<AppStateData>> } callback - Return all application information currently in the
   *                                                          foreground in the form of callback.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getForegroundApplications
   */
  function getForegroundApplications(callback: AsyncCallback<Array<AppStateData>>): void;

  /**
   * getForegroundApplications.
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @returns { Promise<Array<AppStateData>> } Returns the list of AppStateData.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getForegroundApplications
   */
  function getForegroundApplications(): Promise<Array<AppStateData>>;

  /**
   * Kill process with account.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS and ohos.permission.CLEAN_BACKGROUND_PROCESSES
   * @param { string } bundleName - The process bundle name.
   * @param { number } accountId - The account id.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#killProcessWithAccount
   */
  function killProcessWithAccount(bundleName: string, accountId: number): Promise<void>;

  /**
   * Kill process with account.
   *
   * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS and ohos.permission.CLEAN_BACKGROUND_PROCESSES
   * @param { string } bundleName - The process bundle name.
   * @param { number } accountId - The account id.
   * @param { AsyncCallback<void> } callback - Cut off the callback function of the account process.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#killProcessWithAccount
   */
  function killProcessWithAccount(bundleName: string, accountId: number, callback: AsyncCallback<void>): void;

  /**
   * Is user running in stability test.
   *
   * @param { AsyncCallback<boolean> } callback - Returns whether the current stability testing scenario is in progress.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#isRunningInStabilityTest
   */
  function isRunningInStabilityTest(callback: AsyncCallback<boolean>): void;

  /**
   * Is user running in stability test.
   *
   * @returns { Promise<boolean> } Returns true if user is running stability test.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#isRunningInStabilityTest
   */
  function isRunningInStabilityTest(): Promise<boolean>;

  /**
   * Get information about running processes
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @returns { Promise<Array<ProcessRunningInfo>> } Returns the array of {@link ProcessRunningInfo}.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getRunningProcessInformation
   */
  function getProcessRunningInfos(): Promise<Array<ProcessRunningInfo>>;

  /**
   * Get information about running processes
   *
   * @permission ohos.permission.GET_RUNNING_INFO
   * @param { AsyncCallback<Array<ProcessRunningInfo>> } callback - Obtain information about running processes.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getRunningProcessInformation
   */
  function getProcessRunningInfos(callback: AsyncCallback<Array<ProcessRunningInfo>>): void;

  /**
   * Kill processes by bundle name
   *
   * @permission ohos.permission.CLEAN_BACKGROUND_PROCESSES
   * @param { string } bundleName - bundle name.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#killProcessesByBundleName
   */
  function killProcessesByBundleName(bundleName: string): Promise<void>;

  /**
   * Kill processes by bundle name
   *
   * @permission ohos.permission.CLEAN_BACKGROUND_PROCESSES
   * @param { string } bundleName - bundle name.
   * @param { AsyncCallback<void> } callback - cut off the callback function of the account process.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#killProcessesByBundleName
   */
  function killProcessesByBundleName(bundleName: string, callback: AsyncCallback<void>);

  /**
   * Clear up application data by bundle name
   *
   * @permission ohos.permission.CLEAN_APPLICATION_DATA
   * @param { string } bundleName - bundle name.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#clearUpApplicationData
   */
  function clearUpApplicationData(bundleName: string): Promise<void>;

  /**
   * Clear up application data by bundle name
   *
   * @permission ohos.permission.CLEAN_APPLICATION_DATA
   * @param { string } bundleName - bundle name.
   * @param { AsyncCallback<void> } callback - Represents the specified callback method.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#clearUpApplicationData
   */
  function clearUpApplicationData(bundleName: string, callback: AsyncCallback<void>);

  /**
   * Is it a ram-constrained device
   *
   * @returns { Promise<boolean> } whether a ram-constrained device.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#isRamConstrainedDevice
   */
  function isRamConstrainedDevice(): Promise<boolean>;

  /**
   * Is it a ram-constrained device
   *
   * @param { AsyncCallback<boolean> } callback - Returns whether the current device is a RAM restricted device.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#isRamConstrainedDevice
   */
  function isRamConstrainedDevice(callback: AsyncCallback<boolean>): void;

  /**
   * Get the memory size of the application
   *
   * @returns { Promise<number> } application memory size.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getAppMemorySize
   */
  function getAppMemorySize(): Promise<number>;

  /**
   * Get the memory size of the application
   *
   * @param { AsyncCallback<number> } callback - application memory size in M.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @since 7
   * @deprecated since 9
   * @useinstead ohos.app.ability.appManager/appManager#getAppMemorySize
   */
  function getAppMemorySize(callback: AsyncCallback<number>): void;
}

export default appManager;
