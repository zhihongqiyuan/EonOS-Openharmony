/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import ExtensionAbility from './@ohos.app.ability.ExtensionAbility';
import type UIExtensionContentSession from './@ohos.app.ability.UIExtensionContentSession';
import type AutoFillExtensionContext from './application/AutoFillExtensionContext';
import type { FillRequest, SaveRequest, UpdateRequest, FillRequestCallback, SaveRequestCallback } from './application/AutoFillRequest';

/**
 * The class of auto fill extension ability.
 *
 * @extends ExtensionAbility
 * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
 * @systemapi
 * @StageModelOnly
 * @since 11
 */
export default class AutoFillExtensionAbility extends ExtensionAbility {
  /**
   * Indicates configuration information about an auto fill extension ability context.
   *
   * @type { AutoFillExtensionContext }
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  context: AutoFillExtensionContext;

  /**
   * Called back when an auto fill extension is started for initialization.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onCreate(): void;

  /**
   * Called back when a fill request is coming.
   *
   * @param { UIExtensionContentSession } session - Indicates the session of the UI extension page.
   * @param { FillRequest } request - Indicates the fill request.
   * @param { FillRequestCallback } callback - Indicates the fill request callback.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onFillRequest(session: UIExtensionContentSession, request: FillRequest, callback: FillRequestCallback): void;

  /**
   * Called back when a save request is coming.
   *
   * @param { UIExtensionContentSession } session - Indicates the session of the UI extension page.
   * @param { SaveRequest } request - Indicates the fill request.
   * @param { SaveRequestCallback } callback - Indicates the fill request callback.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onSaveRequest(session: UIExtensionContentSession, request: SaveRequest, callback: SaveRequestCallback): void;

  /**
   * Called back when an update request is coming.
   *
   * @param { UpdateRequest } request - Indicates the update request.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 12
   */
  onUpdateRequest(request: UpdateRequest): void;

  /**
   * Called back when an auto fill extension session is destroyed.
   *
   * @param { UIExtensionContentSession } session - Indicates the session of the UI extension page.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onSessionDestroy(session: UIExtensionContentSession): void;

  /**
   * Called back when the state of an auto fill extension changes to foreground.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onForeground(): void;

  /**
   * Called back when the state of an auto fill extension changes to background.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onBackground(): void;

  /**
   * Called back before an auto fill extension is destroyed.
   *
   * @returns { void | Promise<void> } the promise returned by the function.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @StageModelOnly
   * @since 11
   */
  onDestroy(): void | Promise<void>;
}