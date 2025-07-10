/*
* Copyright (c) 2023 Huawei Device Co., Ltd.
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**
 * @file
 * @kit InputKit
 */

import type { AsyncCallback } from './@ohos.base';
/**
 * The shortKey module provides APIs to set the delay for starting an ability using a shortcut key. 
 * For example, you can set the delay to 3 seconds so that a screenshot is taken when you press and hold the shortcut key for 3 seconds.
 *
 * @namespace shortKey
 * @syscap SystemCapability.MultimodalInput.Input.ShortKey
 * @systemapi hide for inner use
 * @since 10
 */

declare namespace shortKey {
  /**
   * Sets the delay for starting an ability using shortcut keys. 
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { string } businessKey - Unique service ID registered on the multimodal side. 
   * It corresponds to businessId in the ability_launch_config.json file. You need to query this parameter on your own before calling the API.
   * @param { number } delay - Delay for starting an ability using shortcut keys, in milliseconds. This field is invalid only when shortcut keys are used.
   * @param { AsyncCallback<void> } callback - Callback used to return the result. 
   * If the operation is successful, err is undefined. Otherwise, err is an error object.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.ShortKey
   * @systemapi hide for inner use
   * @since 10
   */
  function setKeyDownDuration(businessKey: string, delay: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the delay for starting an ability using shortcut keys. 
   * This API uses a promise to return the result.
   * 
   * @param { string } businessKey - Unique service ID registered on the multimodal side. 
   * It corresponds to businessId in the ability_launch_config.json file. You need to query this parameter on your own before calling the API.
   * @param { number } delay - Delay for starting an ability using shortcut keys, in milliseconds. This field is invalid only when shortcut keys are used.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.ShortKey
   * @systemapi hide for inner use
   * @since 10
   */
  function setKeyDownDuration(businessKey: string, delay: number): Promise<void>;

}
export default shortKey;

/**
 * Enumerates fingerprint key event types.
 *
 * @enum { number }
 * @syscap SystemCapability.MultimodalInput.Input.Core
 * @systemapi Hide this for inner system use.
 * @since 12
 */
export declare enum FingerprintAction {
  /**
   * Pressing down.
   *
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  DOWN = 0,

  /**
   * Lifting up.
   *
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  UP = 1,

  /**
   * Sliding.
   *
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  SLIDE = 2,

  /**
   * Retouching.
   *
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  RETOUCH = 3,

  /**
   * Clicking.
   *
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  CLICK = 4,
}

/**
 * Defines the key event type and the offset position relative to the key.
 *
 * @interface FingerprintEvent
 * @syscap SystemCapability.MultimodalInput.Input.Core
 * @systemapi Hide this for inner system use.
 * @since 12
 */
export declare interface FingerprintEvent {
  /**
   * Fingerprint key event type.
   *
   * @type { FingerprintAction }
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  action: FingerprintAction;

  /**
   * This value indicates the sliding percentage of the fingerprint key on the X axis,
   * that is, the ratio of the relative sliding distance to the device length
   * compared with the previous report of the sliding event.
   * A positive value indicates moving in the positive direction of the X axis,
   * and a negative value indicates the opposite.
   * The vertical upward direction of the device stands for the positive direction of the Y axis,
   * and the horizontal rightward direction stands for the positive direction of the X axis.
   * This way, a rectangular coordinate system is constructed.
   *
   * @type { number }
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  distanceX: number;

  /**
   * This value indicates the sliding percentage of the fingerprint key on the Y axis,
   * that is, the ratio of the relative sliding distance to the component length
   * compared with the previous report of the sliding event.
   * A positive value indicates moving in the positive direction of the Y axis,
   * and a negative value indicates the opposite.
   * The vertical upward direction of the device stands for the positive direction of the Y axis,
   * and the horizontal rightward direction stands for the positive direction of the X axis.
   * This way, a rectangular coordinate system is constructed.
   *
   * @type { number }
   * @syscap SystemCapability.MultimodalInput.Input.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  distanceY: number;
}