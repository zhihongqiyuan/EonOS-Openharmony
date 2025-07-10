/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

import type { Callback, AsyncCallback } from './@ohos.base';
import type { KeyCode } from './@ohos.multimodalInput.keyCode';

/**
 * The inputDevice module implements input device management functions such as listening for the connection
 * and disconnection of input devices and querying input device information such as the device name.
 * 
 * @namespace inputDevice
 * @syscap SystemCapability.MultimodalInput.Input.InputDevice
 * @since 8
 */
declare namespace inputDevice {
  /**
   * Enumerates hot swap events.
   * 
   * @typedef { 'add' | 'remove' }
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  type ChangedType = 'add' | 'remove';

  /**
   * Enumerates input source types of the axis. For example, if a mouse reports an x-axis event, the input source of the x-axis is the mouse.
   * 
   * @typedef { 'keyboard' | 'mouse' | 'touchpad' | 'touchscreen' | 'joystick' | 'trackball' }
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  type SourceType = 'keyboard' | 'mouse' | 'touchpad' | 'touchscreen' | 'joystick' | 'trackball';

  /**
   * Defines the axis type of an input device.
   * 
   * @typedef { 'touchmajor'| 'touchminor' | 'orientation' | 'x' | 'y' | 'pressure' | 'toolminor' | 'toolmajor' | 'null' }
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  type AxisType =
    'touchmajor'
    | 'touchminor'
    | 'orientation'
    | 'x'
    | 'y'
    | 'pressure'
    | 'toolminor'
    | 'toolmajor'
    | 'null';

  /**
   * Enumerates keyboard types.
   * 
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  enum KeyboardType {
    /**
     * Keyboard without keys.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    NONE = 0,

    /**
     * Keyboard with unknown keys.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    UNKNOWN = 1,

    /**
     * Full keyboard.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    ALPHABETIC_KEYBOARD = 2,

    /**
     * Keypad.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    DIGITAL_KEYBOARD = 3,

    /**
     * Stylus.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    HANDWRITING_PEN = 4,

    /**
     * Remote control.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    REMOTE_CONTROL = 5
  }

  /**
   * Enumerates function key types.
   * 
   * @enum { number }
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 15
   */
  enum FunctionKey {
    /**
     * CapsLock key. This key can be enabled or disabled only for the input keyboard extension.
     *
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 15
     */
    CAPS_LOCK = 1
  }

  /**
   * Provides hot swap information about an input device.
   *
   * @interface DeviceListener
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  interface DeviceListener {
    /**
     * Device change type, which indicates whether an input device is inserted or removed.
     *
     * @type { ChangedType }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    type: ChangedType;

    /**
     * Unique ID of the input device.
     * If the same physical device is repeatedly inserted and removed, its ID changes.
     * 
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    deviceId: number;
  }

  /**
   * Enables listening for device hot swap events.
   * When performing this operation, you need to connect to external devices such as a mouse, keyboard, and touchscreen.
   * 
   * @param { 'change' } type - Event type. This field has a fixed value of change.
   * @param { Callback<DeviceListener> } listener - Listener for events of the input device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function on(type: 'change', listener: Callback<DeviceListener>): void;

  /**
   * Disables listening for device hot swap events.
   * This API is called before the application exits.
   * 
   * @param { 'change' } type - Event type. This field has a fixed value of change.
   * @param { Callback<DeviceListener> } listener - Listener for events of the input device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function off(type: 'change', listener?: Callback<DeviceListener>): void;

  /**
   * Defines the axis range of an input device.
   *
   * @interface AxisRange
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   */
  interface AxisRange {
    /**
     * Input source type of the axis.
     *
     * @type { SourceType }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    source: SourceType;

    /**
     * Axis type.
     *
     * @type { AxisType }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    axis: AxisType;

    /**
     * Maximum value of the axis.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    max: number;

    /**
     * Minimum value of the axis.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    min: number;

    /**
     * Fuzzy value of the axis.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    fuzz: number;

    /**
     * Benchmark value of the axis.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    flat: number;

    /**
     * Resolution of the axis.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    resolution: number;
  }

  /**
   * Provides information about an input device.
   *
   * @interface InputDeviceData
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   */
  interface InputDeviceData {
    /**
     * Unique ID of the input device.
     * If the same physical device is repeatedly inserted and removed, its ID changes.
     * 
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    id: number;

    /**
     * Name of the input device.
     *
     * @type { string }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    name: string;

    /**
     * Source type supported by the input device. For example, if a keyboard is attached with a touchpad,
     * the device has two input sources: keyboard and touchpad.
     *
     * @type { Array<SourceType> }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    sources: Array<SourceType>;

    /**
     * Axis information of the input device.
     *
     * @type { Array<AxisRange> }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 8
     */
    axisRanges: Array<AxisRange>;

    /**
     * Bus type of the input device. By default, the bus type reported by the input device prevails.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    bus: number;

    /**
     * Product information of the input device.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    product: number;

    /**
     * Vendor information of the input device.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    vendor: number;

    /**
     * Version information of the input device.
     *
     * @type { number }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    version: number;

    /**
     * Physical address of the input device.
     *
     * @type { string }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    phys: string;

    /**
     * Unique ID of the input device.
     *
     * @type { string }
     * @syscap SystemCapability.MultimodalInput.Input.InputDevice
     * @since 9
     */
    uniq: string;
  }

  /**
   * Obtains the IDs of all input devices.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { AsyncCallback<Array<number>> } callback - Callback used to return the IDs of all input devices.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.multimodalInput.inputDevice#getDeviceList
   */
  function getDeviceIds(callback: AsyncCallback<Array<number>>): void;

  /**
   * Obtains the IDs of all input devices.
   * This API uses a promise to return the result.
   * 
   * @returns { Promise<Array<number>> } Promise used to return the IDs of all input devices.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.multimodalInput.inputDevice#getDeviceList
   */
  function getDeviceIds(): Promise<Array<number>>;

  /**
   * Obtains the information about the input device with the specified ID.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } deviceId - ID of the input device.
   * @param { AsyncCallback<InputDeviceData> } callback - Callback used to return the information about the input device.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.multimodalInput.inputDevice#getDeviceInfo
   */
  function getDevice(deviceId: number, callback: AsyncCallback<InputDeviceData>): void;

  /**
   * Obtains the information about the input device with the specified ID.
   * This API uses a promise to return the result.
   * 
   * @param { number } deviceId - ID of the input device.
   * @returns { Promise<InputDeviceData> } Promise used to return the information about the input device.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.multimodalInput.inputDevice#getDeviceInfo
   */
  function getDevice(deviceId: number): Promise<InputDeviceData>;

  /**
   * Obtains the IDs of all input devices.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { AsyncCallback<Array<number>> } callback - Callback used to return the IDs of all input devices. id is the unique ID of an input device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getDeviceList(callback: AsyncCallback<Array<number>>): void;

  /**
   * Obtains the IDs of all input devices.
   * This API uses a promise to return the result.
   * 
   * @returns { Promise<Array<number>> } - Promise used to return the IDs of all input devices.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getDeviceList(): Promise<Array<number>>;

  /**
   * Obtains the information about the input device with the specified ID.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } deviceId - ID of the input device.
   * @param { AsyncCallback<InputDeviceData> } callback - Callback used to return the information about the input device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getDeviceInfo(deviceId: number, callback: AsyncCallback<InputDeviceData>): void;

  /**
   * Obtains the information about the input device with the specified ID.
   * This API uses a promise to return the result.
   * 
   * @param { number } deviceId - ID of the input device.
   * @returns { Promise<InputDeviceData> } Promise used to return the information about the input device.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getDeviceInfo(deviceId: number): Promise<InputDeviceData>;

  /**
   * Obtains information about the specified input device.
   *
   * @param { number } deviceId - ID of the input device.
   * @returns { InputDeviceData } Information about the input device, including device ID, name, supported source,
   * physical address, version information, and product information.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 10
   */
  function getDeviceInfoSync(deviceId: number): InputDeviceData;

  /**
   * Checks whether the input device supports the specified keys.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } deviceId - ID of the input device. The device ID changes if the same physical device is repeatedly removed and inserted.
   * @param { Array<KeyCode> } keys - Keycodes to be queried. A maximum of five keycodes can be specified.
   * @param { AsyncCallback<Array<boolean>> } callback -Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function supportKeys(deviceId: number, keys: Array<KeyCode>, callback: AsyncCallback<Array<boolean>>): void;

  /**
   * Checks whether the input device supports the specified keys. This API uses a promise to return the result.
   *
   * @param { number } deviceId - ID of the input device. The device ID changes if the same physical device is repeatedly removed and inserted.
   * @param { Array<KeyCode> } keys - Keycodes to be queried. A maximum of five keycodes can be specified.
   * @returns { Promise<Array<boolean>> } Returns a result indicating whether the specified key codes are supported.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function supportKeys(deviceId: number, keys: Array<KeyCode>): Promise<Array<boolean>>;

  /**
   * Checks whether the input device supports the specified keys.
   *
   * @param { number } deviceId - ID of the input device. The device ID changes if the same physical device is repeatedly removed and inserted.
   * @param { Array<KeyCode> } keys - Keycodes to be queried. A maximum of five keycodes can be specified.
   * @returns { Array<boolean> } Result indicating whether the input device supports the keycode value. 
   * The value true indicates yes, and the value false indicates no.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 10
   */
  function supportKeysSync(deviceId: number, keys: Array<KeyCode>): Array<boolean>;

  /**
   * Obtains the keyboard type of an input device.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } deviceId - Unique ID of the input device. If the same physical device is repeatedly inserted and removed, its ID changes.
   * @param { AsyncCallback<KeyboardType> } callback - Callback used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getKeyboardType(deviceId: number, callback: AsyncCallback<KeyboardType>): void;

  /**
   * Obtains the keyboard type of an input device.
   * This API uses a promise to return the result.
   * 
   * @param { number } deviceId - Unique ID of the input device. If the same physical device is repeatedly inserted and removed, its ID changes.
   * @returns { Promise<KeyboardType> } Promise used to return the result.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 9
   */
  function getKeyboardType(deviceId: number): Promise<KeyboardType>;

  /**
   * Obtains the keyboard type of the input device.
   *
   * @param { number } deviceId - Unique ID of the input device. If the same physical device is repeatedly inserted and removed, its ID changes.
   * @returns { KeyboardType } Keyboard type.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 10
   */
  function getKeyboardTypeSync(deviceId: number): KeyboardType;

  /**
   * Sets the keyboard repeat delay.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } delay - Keyboard repeat delay, in ms. The value range is [300, 1000] and the default value is 500.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function setKeyboardRepeatDelay(delay: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the keyboard repeat delay.
   * This API uses a promise to return the result.
   * 
   * @param { number } delay - Keyboard repeat delay, in ms. The value range is [300, 1000] and the default value is 500.
   * @returns { Promise<void> } A promise that returns no value.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function setKeyboardRepeatDelay(delay: number): Promise<void>;

  /**
   * Obtains the keyboard repeat delay.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { AsyncCallback<number> } callback - Callback used to return the keyboard repeat delay.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function getKeyboardRepeatDelay(callback: AsyncCallback<number>): void;

  /**
   * Obtains the keyboard repeat delay.
   * This API uses a promise to return the result.
   * 
   * @returns { Promise<number> } Promise used to return the keyboard repeat delay.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function getKeyboardRepeatDelay(): Promise<number>;

  /**
   * Sets the keyboard repeat rate.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { number } rate - Keyboard repeat rate, in ms/time. The value range is [36, 100] and the default value is 50.
   * @param { AsyncCallback<void> } callback - Callback used to return the result.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function setKeyboardRepeatRate(rate: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the keyboard repeat rate.
   * This API uses a promise to return the result.
   * 
   * @param { number } rate - Keyboard repeat rate, in ms/time. The value range is [36, 100] and the default value is 50.
   * @returns { Promise<void> } A promise that returns no value.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function setKeyboardRepeatRate(rate: number): Promise<void>;

  /**
   * Obtains the keyboard repeat rate.
   * This API uses an asynchronous callback to return the result.
   * 
   * @param { AsyncCallback<number> } callback - Callback used to return the keyboard repeat rate.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function getKeyboardRepeatRate(callback: AsyncCallback<number>): void;

  /**
   * Obtains the keyboard repeat rate.
   * This API uses a promise to return the result.
   * 
   * @returns { Promise<number> } Promise used to return the keyboard repeat rate.
   * @throws { BusinessError } 202 - SystemAPI permission error.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi hide for inner use.
   * @since 10
   */
  function getKeyboardRepeatRate(): Promise<number>;

  /**
   * Obtains the interval since the last input.
   *
   * @returns { Promise<number> } Promise used to return the interval since the last input.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 14
   */
  function getIntervalSinceLastInput(): Promise<number>;
  
  /**
   * Sets the input switch status of an input device. Take the touchscreen as an example.
   * If the input switch is off, the touchscreen does not respond when being touched.
   * If the input switch is on, the touchscreen wakes up when being touched.
   * 
   * @permission ohos.permission.INPUT_DEVICE_CONTROLLER
   * @param { number } deviceId - Device id.
   * @param { boolean } enabled - Switch status of the input device. The value true indicates that the input device is enabled,
   * and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 201 - Permission verification failed.
   *     The application does not have the permission required to call the API
   * @throws { BusinessError } 202 - Permission verification failed. 
   *     A non-system application calls a system API.
   * @throws { BusinessError } 401 - Input parameter error. Possible causes:
   *     1. Mandatory parameters are left unspecified;
   *     2. Incorrect parameter types;
   *     3. Parameter verification failed.
   * @throws { BusinessError } 3900001 - The specified device does not exist.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @systemapi
   * @since 18
   */
  function setInputDeviceEnabled(deviceId: number, enabled: boolean): Promise<void>;

  /**
   * Sets the status of the function key.
   * This API uses a promise to return the result.
   * 
   * @permission ohos.permission.INPUT_KEYBOARD_CONTROLLER
   * @param { number } functionKey - Type of the function key.
   * @param { boolean } enabled - Status of the function key. The value true indicates that the function key is enabled, 
   * and the value false indicates the opposite.
   * @returns { Promise<void> } Returns the result through a promise.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 3900002 - There is currently no keyboard device connected.
   * @throws { BusinessError } 3900003 - It is prohibited for non-input applications.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 15
   */
  function setFunctionKeyEnabled(functionKey: FunctionKey, enabled: boolean): Promise<void>;

  /**
   * Checks whether the function key is enabled.
   * This API uses a promise to return the result.
   * 
   * @param { number } functionKey - Type of the function key.
   * @returns { Promise<boolean> } Promise used to return the result. The value true indicates that the function key is enabled, 
   * and the value false indicates the opposite.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 3900002 - There is currently no keyboard device connected.
   * @syscap SystemCapability.MultimodalInput.Input.InputDevice
   * @since 15
   */
  function isFunctionKeyEnabled(functionKey: FunctionKey): Promise<boolean>;
}

export default inputDevice;