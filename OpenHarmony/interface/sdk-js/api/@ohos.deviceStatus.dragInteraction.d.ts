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
 * @kit ArkUI
 */

import { Callback } from './@ohos.base';

/**
 * Declares the APIs for dragging management.
 *
 * @namespace dragInteraction
 * @syscap SystemCapability.Msdp.DeviceStatus.Drag
 * @systemapi Hide this for inner system use.
 * @since 10
 */
declare namespace dragInteraction {
  /**
   * Enumerates the dragging states.
   *
   * @enum { number }
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  enum DragState {
    /**
     * Dragging starts.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Drag
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    MSG_DRAG_STATE_START = 1,

    /**
     * Dragging ends.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Drag
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    MSG_DRAG_STATE_STOP = 2,

    /**
     * Dragging is canceled.
     *
     * @syscap SystemCapability.Msdp.DeviceStatus.Drag
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    MSG_DRAG_STATE_CANCEL = 3,
  }

  /**
   * Abstract of the dragged data.
   *
   * @interface Summary
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  interface Summary {
    /**
     * Type of the dragged object.
     *
     * @type { string }
     * @syscap SystemCapability.Msdp.DeviceStatus.Drag
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    dataType: string;

    /**
     * Data length of the dragged object.
     *
     * @type { number }
     * @syscap SystemCapability.Msdp.DeviceStatus.Drag
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    dataSize: number;
  }

  /**
   * Listens for dragging state change events.
   *
   * @param { 'drag' } type Indicates the event type.
   * @param { Callback<DragState> } callback Indicates the callback to receive the changed dragging state.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Listens for dragging state change events.
   *
   * @param { 'drag' } type Indicates the event type.
   * @param { Callback<DragState> } callback Indicates the callback to receive the changed dragging state.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function on(type: 'drag', callback: Callback<DragState>): void;

  /**
   * Disables listening for dragging state change events.
   *
   * @param { 'drag' } type Indicates the event type.
   * @param { Callback<DragState> }callback Indicates the callback for which listening is disabled. If this parameter
   * is not specified, listening will be disabled for all registered callbacks.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Disables listening for dragging state change events.
   *
   * @param { 'drag' } type Indicates the event type.
   * @param { Callback<DragState> }callback Indicates the callback for which listening is disabled. If this parameter
   * is not specified, listening will be disabled for all registered callbacks.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function off(type: 'drag', callback?: Callback<DragState>): void;

  /**
   * Obtains the abstract of a dragged object.
   *
   * @returns { Array<Summary> } Data abstract of the dragged object.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function getDataSummary(): Array<Summary>;

  /**
   * Sets the master switch for enhancing the drag capability.
   *
   * @param { boolean } enabled Switch state.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function setDragSwitchState(enabled: boolean): void;

  /**
   * Sets the app switch for enhancing the drag capability.
   *
   * @param { boolean } enabled Switch state.
   * @param { string } bundleName App bundle name.
   * @throws {BusinessError} 202 - Permission verification failed. A non-system application calls a system API.
   * @throws {BusinessError} 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
   * <br>2.Incorrect parameter types.3.Parameter verification failed.
   * @syscap SystemCapability.Msdp.DeviceStatus.Drag
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function setAppDragSwitchState(enabled: boolean, bundleName: string): void;
}

export default dragInteraction;