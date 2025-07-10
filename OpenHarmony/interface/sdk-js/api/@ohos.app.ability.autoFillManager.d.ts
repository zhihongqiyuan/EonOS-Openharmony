/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import type { UIContext } from './@ohos.arkui.UIContext';
import type * as _ViewData from './application/ViewData';
import type * as _PageNodeInfo from './application/PageNodeInfo';
import { AutoFillType } from './application/AutoFillType';
import type * as _AutoFillRequest from './application/AutoFillRequest';
import type * as _CustomData from './application/CustomData';
import type * as _AutoFillRect from './application/AutoFillRect';
import type * as _AutoFillPopupConfig from './application/AutoFillPopupConfig';
import { PopupPlacement } from './application/AutoFillPopupConfig';

/**
 * This module provides the function of auto fill manager.
 *
 * @namespace autoFillManager
 * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
 * @stagemodelonly
 * @since 11
 */
/**
 * This module provides the function of auto fill manager.
 *
 * @namespace autoFillManager
 * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
 * @stagemodelonly
 * @atomicservice
 * @since 12
 */
declare namespace autoFillManager {
  /**
   * Auto save callback.
   *
   * @interface AutoSaveCallback
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @stagemodelonly
   * @since 11
   */
  /**
   * Auto save callback.
   *
   * @interface AutoSaveCallback
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @stagemodelonly
   * @atomicservice
   * @since 12
   */
  export interface AutoSaveCallback {
    /**
     * Called when auto save request is successfully handled.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @stagemodelonly
     * @since 11
     */
    /**
     * Called when auto save request is successfully handled.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @stagemodelonly
     * @atomicservice
     * @since 12
     */
    onSuccess(): void;

    /**
     * Called when auto save request is failed to be handled.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @stagemodelonly
     * @since 11
     */
    /**
     * Called when auto save request is failed to be handled.
     *
     * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
     * @stagemodelonly
     * @atomicservice
     * @since 12
     */
    onFailure(): void;
  }

  /**
   * Trigger an auto save request.
   *
   * @param { UIContext } context - Indicates the ui context where the save operation will be performed.
   * @param { AutoSaveCallback } [callback] - Indicates the callback that used to receive the result.
   * @throws { BusinessError } 401 - The parameter check failed. Possible causes: 1. Get instance id failed;
   * <br>2. Parse instance id failed; 3. The second parameter is not of type callback.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @stagemodelonly
   * @since 11
   */
  /**
   * Trigger an auto save request.
   *
   * @param { UIContext } context - Indicates the ui context where the save operation will be performed.
   * @param { AutoSaveCallback } [callback] - Indicates the callback that used to receive the result.
   * @throws { BusinessError } 401 - The parameter check failed. Possible causes: 1. Get instance id failed;
   * <br>2. Parse instance id failed; 3. The second parameter is not of type callback.
   * @throws { BusinessError } 16000050 - Internal error.
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @stagemodelonly
   * @atomicservice
   * @since 12
   */
  export function requestAutoSave(context: UIContext, callback?: AutoSaveCallback): void;

  /**
   * The interface of view data.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of view data.
   *
   * @typedef { _ViewData.default } ViewData
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type ViewData = _ViewData.default;

  /**
   * The interface of page node info.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of page node info.
   *
   * @typedef { _PageNodeInfo.default } PageNodeInfo
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type PageNodeInfo = _PageNodeInfo.default;

  /**
   * The enum of auto fill type.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The enum of auto fill type.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export { AutoFillType };

  /**
   * The interface of fill request.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of fill request.
   *
   * @typedef { _AutoFillRequest.FillRequest } FillRequest
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type FillRequest = _AutoFillRequest.FillRequest;

  /**
   * The interface of save request.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of save request.
   *
   * @typedef { _AutoFillRequest.SaveRequest } SaveRequest
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type SaveRequest = _AutoFillRequest.SaveRequest;

  /**
   * The interface of update request.
   *
   * @typedef { _AutoFillRequest.UpdateRequest } UpdateRequest
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type UpdateRequest = _AutoFillRequest.UpdateRequest;

  /**
   * The interface of fill response.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of fill response.
   *
   * @typedef { _AutoFillRequest.FillResponse } FillResponse
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type FillResponse = _AutoFillRequest.FillResponse;

  /**
   * The interface of fill request callback.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of fill request callback.
   *
   * @typedef { _AutoFillRequest.FillRequestCallback } FillRequestCallback
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type FillRequestCallback = _AutoFillRequest.FillRequestCallback;

  /**
   * The interface of save request callback.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 11
   */
  /**
   * The interface of save request callback.
   *
   * @typedef { _AutoFillRequest.SaveRequestCallback } SaveRequestCallback
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type SaveRequestCallback = _AutoFillRequest.SaveRequestCallback;

  /**
   * The custom data.
   *
   * @typedef { _CustomData.default } CustomData
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 13
   */
  export type CustomData = _CustomData.default;

  /**
   * Auto fill rectangle.
   *
   * @typedef { _AutoFillRect.default } AutoFillRect
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type AutoFillRect = _AutoFillRect.default;

  /**
   * The auto fill popup config.
   *
   * @typedef { _AutoFillPopupConfig.default } AutoFillPopupConfig
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type AutoFillPopupConfig = _AutoFillPopupConfig.default;

  /**
   * The popup size of auto fill popup config.
   *
   * @typedef { _AutoFillPopupConfig.PopupSize } PopupSize
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export type PopupSize = _AutoFillPopupConfig.PopupSize;

  /**
   * The popup placement of auto fill popup config.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.AbilityCore
   * @systemapi
   * @stagemodelonly
   * @since 12
   */
  export { PopupPlacement };
}

export default autoFillManager;
