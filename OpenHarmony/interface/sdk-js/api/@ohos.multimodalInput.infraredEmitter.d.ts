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
 * @kit InputKit
 */

/**
 * The infraredEmitter module generates IR signals of the specified frequency and size, and queries the frequency range supported by the device.
 *
 * @namespace infraredEmitter
 * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
 * @since 12
 */
declare namespace infraredEmitter {
  /**
   * Defines the frequency range of IR signals.
   *
   * @interface InfraredFrequency
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @systemapi hide for inner use
   * @since 12
   */
  /**
   * Defines the frequency range of IR signals.
   *
   * @interface InfraredFrequency
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @since 15
   */
  interface InfraredFrequency {
    /**
     * Maximum frequency, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
     * @systemapi hide for inner use
     * @since 12
     */
    /**
     * Maximum frequency, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
     * @since 15
     */
    max: number;

    /**
     * Minimum frequency, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
     * @systemapi hide for inner use
     * @since 12
     */
    /**
     * Minimum frequency, in Hz.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
     * @since 15
     */
    min: number;
  }

  /**
   * Generates IR signals at the specified frequency and level.
   *
   * @permission ohos.permission.MANAGE_INPUT_INFRARED_EMITTER
   * @param { number} infraredFrequency - IR infrared frequency, in Hz.
   * @param { Array<number>} pattern - IR level signal, in μs. The value must be an even number within the value range of [0,1024]. 
   * For example, in the IR level signal array [100,200,300,400], 100 μs is a high-level signal, 
   * 200 μs is a low-level signal, 300 μs is a high-level signal, and 400 μs is a low-level signal.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @systemapi hide for inner use
   * @since 12
   */
  /**
   * Generates IR signals at the specified frequency and level.
   *
   * @permission ohos.permission.MANAGE_INPUT_INFRARED_EMITTER
   * @param { number} infraredFrequency - IR infrared frequency, in Hz.
   * @param { Array<number>} pattern - IR level signal, in μs. The value must be an even number within the value range of [0,1024]. 
   * For example, in the IR level signal array [100,200,300,400], 100 μs is a high-level signal, 
   * 200 μs is a low-level signal, 300 μs is a high-level signal, and 400 μs is a low-level signal.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @since 15
   */
  function transmitInfrared(infraredFrequency: number, pattern: Array<number>): void;

  /**
   * Queries the frequency range of IR signals supported by the mobile phone.
   *
   * @permission ohos.permission.MANAGE_INPUT_INFRARED_EMITTER
   * @returns { Array<InfraredFrequency> } Frequency range, including multiple groups of maximum and minimum frequencies.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @systemapi hide for inner use
   * @since 12
   */
  /**
   * Queries the frequency range of IR signals supported by the mobile phone.
   *
   * @permission ohos.permission.MANAGE_INPUT_INFRARED_EMITTER
   * @returns { Array<InfraredFrequency> } Frequency range, including multiple groups of maximum and minimum frequencies.
   * @throws { BusinessError } 201 - Permission denied.
   * @syscap SystemCapability.MultimodalInput.Input.InfraredEmitter
   * @since 15
   */
  function getInfraredFrequencies(): Array<InfraredFrequency>;
}

export default infraredEmitter;
