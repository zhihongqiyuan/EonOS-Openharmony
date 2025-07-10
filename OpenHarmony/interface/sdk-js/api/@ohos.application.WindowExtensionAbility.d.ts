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
 * @kit ArkUI
 */

import rpc from './@ohos.rpc';
import _WindowExtensionContext from './application/WindowExtensionContext';
import Want from './@ohos.app.ability.Want';
import window from './@ohos.window';

/**
 * class of window extension ability.
 *
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @systemapi hide for inner use.
 * @StageModelOnly
 * @since 9
 */
export default class WindowExtensionAbility {
  /**
   * Indicates window extension ability context.
   *
   * @type { WindowExtensionContext }
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi hide for inner use.
   * @StageModelOnly
   * @since 9
   */
  context: WindowExtensionContext;

  /**
   * Called back when a window extension is first connected to an ability.
   *
   * @param { Want } want Indicates connection information about the Window ability.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi hide for inner use.
   * @StageModelOnly
   * @since 9
   */
  onConnect(want: Want): void;

  /**
   * Called back when all abilities connected to a window extension are disconnected.
   *
   * @param { Want } want Indicates disconnection information about the window extension.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi hide for inner use.
   * @StageModelOnly
   * @since 9
   */
  onDisconnect(want: Want): void;

  /**
   * Called back when window is created.
   *
   * @param { window.Window } window Current Window instance.
   * @syscap SystemCapability.WindowManager.WindowManager.Core
   * @systemapi hide for inner use.
   * @StageModelOnly
   * @since 9
   */
  onWindowReady(window: window.Window): void;
}

/**
 * The context of window extension. It allows access to
 * windowExtension-specific resources.
 *
 * @typedef { _WindowExtensionContext }
 * @syscap SystemCapability.WindowManager.WindowManager.Core
 * @systemapi
 * @stagemodelonly
 * @since 9
 */
export type WindowExtensionContext = _WindowExtensionContext;
