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
 * @kit AbilityKit
 */

import ApplicationContext from './application/ApplicationContext';
import Context from './application/Context';

/**
 * This module provides application basic functions.
 *
 * @namespace application
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 12
 */
declare namespace application {
  /**
   * Create a module context
   *
   * @param { Context } context - Indicates current context.
   * @param { string } moduleName - Indicates the module name.
   * @returns { Promise<Context> } Returns the application context.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified. 2.Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 12
   */
  export function createModuleContext(context: Context, moduleName: string): Promise<Context>;

  /**
   * Create a module context
   * 
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Context } context - Indicates current context.
   * @param { string } bundleName - Indicates the bundle name.
   * @param { string } moduleName - Indicates the module name.
   * @returns { Promise<Context> } Returns the application context.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified. 2.Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export function createModuleContext(context: Context, bundleName: string, moduleName: string): Promise<Context>;

  /**
   * Create a module context of plugin.
   * 
   * @param { Context } context - Indicates current context.
   * @param { string } pluginBundleName - Indicates the bundle name of plugin.
   * @param { string } pluginModuleName - Indicates the module name of plugin.
   * @returns { Promise<Context> } Returns the module context of plugin.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @since 19
   */
  export function createPluginModuleContext(context: Context, pluginBundleName: string, pluginModuleName: string): Promise<Context>;

  /**
   * Create a bundle context
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Context } context - Indicates current context.
   * @param { string } bundleName - Indicates the bundle name.
   * @returns { Promise<Context> } Returns the application context.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1.Mandatory parameters are left unspecified. 2.Incorrect parameter types.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export function createBundleContext(context: Context, bundleName: string): Promise<Context>;

  /**
   * Get application context.
   *
   * @returns { ApplicationContext } Returns the application context.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 14
   */
  export function getApplicationContext(): ApplicationContext;
}

export default application;