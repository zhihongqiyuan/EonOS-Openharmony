/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import StartupConfig from './@ohos.app.appstartup.StartupConfig';
import Want from './@ohos.app.ability.Want';

/**
 * The configuration entry for running startup tasks.
 *
 * @syscap SystemCapability.Ability.AppStartup
 * @stagemodelonly
 * @since 12
 */
export default class StartupConfigEntry {
  /**
   * Called when startup initialization to configure startup mode.
   *
   * @returns { StartupConfig } The developer returns a startup configuration.
   * @syscap SystemCapability.Ability.AppStartup
   * @stagemodelonly
   * @since 12
   */
  onConfig?(): StartupConfig;

  /**
   * Called when a custom match rule is requested.
   * The custom match rule returned will be matched against the match rules defined in the appStartup profile.
   * If any of the specified tasks meets the custom match rule, that task will be initiated;
   * otherwise, the default automatic startup tasks will be initiated.
   *
   * @param { Want } want Want information of the started ability.
   * @returns { string } Custom match rule
   * @syscap SystemCapability.Ability.AppStartup
   * @stagemodelonly
   * @since 20
   */
  onRequestCustomMatchRule(want: Want): string;
}
