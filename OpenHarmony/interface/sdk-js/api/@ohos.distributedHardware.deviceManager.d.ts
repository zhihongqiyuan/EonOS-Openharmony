/*
 * Copyright (c) 2020-2024 Huawei Device Co., Ltd.
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
 * @kit DistributedServiceKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';

/**
 * Providers interfaces to create a {@link deviceManager} instances.
 *
 * @namespace deviceManager
 * @syscap SystemCapability.DistributedHardware.DeviceManager
 * @since 7
 * @deprecated since 11
 * @useinstead ohos.distributedDeviceManager/distributedDeviceManager
 */
declare namespace deviceManager {
  /**
   * DeviceInfo
   *
   * @interface DeviceInfo
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceBasicInfo
   */
  interface DeviceInfo {
    /**
     * DeviceId ID.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceBasicInfo#deviceId
     */
    deviceId: string;

    /**
     * Device name of the device.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceBasicInfo#deviceName
     */
    deviceName: string;

    /**
     * Device type of the device.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceBasicInfo#deviceType
     */
    deviceType: DeviceType;

    /**
     * NetworkId of the device.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceBasicInfo#networkId
     */
    networkId: string;

    /**
     * The distance of discovered device, in centimeter(cm).
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    range: number;

    /**
     * Indicates the device's trusted type
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    authForm: AuthForm;
  }

  /**
   * Device trusted type definitions
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 10
   * @deprecated since 11
   */
  enum AuthForm {
    /**
     * Indicates invalid trusted device type.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    INVALID_TYPE = -1,

    /**
     * Indicates peer to peer trusted device type without account.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    PEER_TO_PEER = 0,

    /**
     * Indicates identical account trusted device type.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    IDENTICAL_ACCOUNT = 1,

    /**
     * Indicates across account trusted device type.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    ACROSS_ACCOUNT = 2
  }

  /**
   * Device Type definitions
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  enum DeviceType {
    /**
     * Indicates an unknown device type.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    UNKNOWN_TYPE = 0,

    /**
     * Indicates a speaker.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    SPEAKER = 0x0A,

    /**
     * Indicates a smartphone.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    PHONE = 0x0E,

    /**
     * Indicates a tablet.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    TABLET = 0x11,

    /**
     * Indicates a smart watch.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    WEARABLE = 0x6D,

    /**
     * Indicates a car.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    CAR = 0x83,

    /**
     * Indicates a smart TV.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    TV = 0x9C
  }

  /**
   * Device state change event definition
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceStateChange
   */
  enum DeviceStateChangeAction {
    /**
     * Device online action, which indicates the device is physically online
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceStateChange#UNKNOWN
     */
    ONLINE = 0,

    /**
     * Device ready action, which indicates the information between devices has been synchronized in the Distributed Data Service (DDS) module,
     * and the device is ready for running distributed services
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceStateChange#AVAILABLE
     */
    READY = 1,

    /**
     * Device offline action, which Indicates the device is physically offline
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceStateChange#UNAVAILABLE
     */
    OFFLINE = 2,

    /**
     * Device change action
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    CHANGE = 3
  }

  /**
   * Service subscribe info for device discover
   *
   * @interface SubscribeInfo
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  interface SubscribeInfo {
    /**
     * Service subscribe ID, the value is in scope [0, 65535], should be unique for each discover process
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    subscribeId: number;

    /**
     * Discovery mode for service subscription.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    mode: DiscoverMode;

    /**
     * Service subscription medium.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    medium: ExchangeMedium;

    /**
     * Service subscription frequency.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    freq: ExchangeFreq;

    /**
     * only find the device with the same account.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    isSameAccount: boolean;

    /**
     * find the sleeping devices.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    isWakeRemote: boolean;

    /**
     * Subscribe capability.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    capability: SubscribeCap;
  }

  /**
   * Service publish info for device discover
   *
   * @interface PublishInfo
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 9
   * @deprecated since 11
   */
  interface PublishInfo {
    /**
     * Service publish ID, the value is in scope [0, 65535], should be unique for each publish process
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    publishId: number;

    /**
     * Discovery mode for service subscription.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    mode: DiscoverMode;

    /**
     * Service subscription frequency.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    freq: ExchangeFreq;

    /**
     *  Whether the device should be ranged by discoverer.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    ranging: boolean;
  }

  /**
   * device discover mode
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  enum DiscoverMode {
    /**
     * when using this key at client side, it means discovering for available nearby devices by
     * calling @startDeviceDiscovery function, while using this key at server side indicating that
     * a device publication or advertisement by calling @publishDeviceDiscovery.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    DISCOVER_MODE_PASSIVE = 0x55,

    /**
     * when using this key at server side, it means discovering for available nearby devices by
     * calling @startDeviceDiscovery function, while using this key at client side indicating that
     * a device publication or advertisement by calling @publishDeviceDiscovery.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    DISCOVER_MODE_ACTIVE = 0xAA
  }

  /**
   * device discover medium
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  enum ExchangeMedium {
    /**
     * Automatic medium selection
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    AUTO = 0,

    /**
     * Bluetooth
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    BLE = 1,

    /**
     * Wi-Fi
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    COAP = 2,

    /**
     * USB
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    USB = 3
  }

  /**
   * device discover freq
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  enum ExchangeFreq {
    /**
     * Low
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    LOW = 0,

    /**
     * Medium
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    MID = 1,

    /**
     * High
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    HIGH = 2,

    /**
     * Super-high
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    SUPER_HIGH = 3
  }

  /**
   * device discover capability
   *
   * @enum { number }
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   */
  enum SubscribeCap {
    /**
     * ddmpCapability, will be discarded later. Currently, it will be converted to OSD capability inner.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    SUBSCRIBE_CAPABILITY_DDMP = 0,

    /**
     * One Super Device Capability
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    SUBSCRIBE_CAPABILITY_OSD = 1
  }

  /**
   * Device Authentication param
   *
   * @interface AuthParam
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications
   * @since 7
   * @deprecated since 11
   */
  interface AuthParam {
    /**
     * Authentication type, 1 for pin code.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications
     * @since 7
     * @deprecated since 11
     */
    authType: number;

    /**
     * Authentication extra infos.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications
     * @since 7
     * @deprecated since 11
     */
    extraInfo: { [key: string]: any };
  }

  /**
   * Device auth info.
   *
   * @interface AuthInfo
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications
   * @since 7
   * @deprecated since 11
   */
  interface AuthInfo {
    /**
     * Authentication type, 1 for pin code.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications
     * @since 7
     * @deprecated since 11
     */
    authType: number;

    /**
     * the token used for this authentication.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications
     * @since 7
     * @deprecated since 11
     */
    token: number;

    /**
     * Authentication extra infos.
     *
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications
     * @since 7
     * @deprecated since 11
     */
    extraInfo: { [key: string]: any };
  }

  /**
   * Creates a {@code DeviceManager} instance.
   * <p>To manage devices, you must first call this method to obtain a {@code DeviceManager} instance and then
   * use this instance to call other device management methods.
   *
   * @param { string } bundleName Indicates the bundle name of the application.
   * @param { AsyncCallback<DeviceManager> } callback Indicates the callback to be invoked upon {@code DeviceManager} instance creation.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *                                                  1. Mandatory parameters are left unspecified;
   *                                                  2. Incorrect parameter types;
   *                                                  3. Parameter verification failed.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @systemapi this method can be used only by system applications.
   * @since 7
   * @deprecated since 11
   * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.createDeviceManager
   */
  function createDeviceManager(bundleName: string, callback: AsyncCallback<DeviceManager>): void;

  /**
   * Provides methods for managing devices.
   *
   * @interface DeviceManager
   * @syscap SystemCapability.DistributedHardware.DeviceManager
   * @since 7
   * @deprecated since 11
   * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager
   */
  interface DeviceManager {
    /**
     * Releases the {@code DeviceManager} instance after the methods for device management are no longer used.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.releaseDeviceManager
     */
    release(): void;

    /**
     * Obtains a list of trusted devices.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @returns { Array<DeviceInfo> } returns a list of trusted devices.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getAvailableDeviceListSync
     */
    getTrustedDeviceListSync(): Array<DeviceInfo>;

    /**
     * Obtains a list of trusted devices.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { boolean } isRefresh Refresh the online device list to quickly bring nearby trusted devices online.
     * @returns { Array<DeviceInfo> } Returns a list of trusted devices.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    getTrustedDeviceListSync(isRefresh: boolean): Array<DeviceInfo>;

    /**
     * Obtains a list of trusted devices.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { AsyncCallback<Array<DeviceInfo>> } callback Indicates the callback to be invoked upon getTrustedDeviceList
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getAvailableDeviceList
     */
    getTrustedDeviceList(callback: AsyncCallback<Array<DeviceInfo>>): void;

    /**
     * Obtains a list of trusted devices.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @returns { Promise<Array<DeviceInfo>> } Returns a list of trusted devices.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getAvailableDeviceList
     */
    getTrustedDeviceList(): Promise<Array<DeviceInfo>>;

    /**
     * Obtains local device info
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @returns { DeviceInfo } Returns local device info.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getLocalDeviceNetworkId
     */
    getLocalDeviceInfoSync(): DeviceInfo;

    /**
     * Obtains local device info
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { AsyncCallback<DeviceInfo> } callback Indicates the callback to be invoked upon getLocalDeviceInfo
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getLocalDeviceNetworkId
     */
    getLocalDeviceInfo(callback: AsyncCallback<DeviceInfo>): void;

    /**
     * Obtains local device info
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @returns { Promise<DeviceInfo> } Returns local device info.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getLocalDeviceNetworkId
     */
    getLocalDeviceInfo(): Promise<DeviceInfo>;

    /**
     * Obtains device info
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { string } networkId - device network id.
     * @param { AsyncCallback<DeviceInfo> } callback - Indicates the callback to be invoked upon getDeviceInfo.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified networkId is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getDeviceName
     */
    getDeviceInfo(networkId: string, callback: AsyncCallback<DeviceInfo>): void;

    /**
     * Obtains device info
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { string } networkId - device network id.
     * @returns { Promise<DeviceInfo> } Returns device info.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified networkId is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#getDeviceName
     */
    getDeviceInfo(networkId: string): Promise<DeviceInfo>;

    /**
     * Start to discover device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { SubscribeInfo } subscribeInfo subscribe info to discovery device
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified param is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600104 - Discovery unavailable.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#startDiscovering
     */
    startDeviceDiscovery(subscribeInfo: SubscribeInfo): void;

    /**
     * Start to discover device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { SubscribeInfo } subscribeInfo subscribe info to discovery device
     * @param { string } filterOptions filterOptions to filter discovery device
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified param is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600104 - Discovery unavailable.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#startDiscovering
     */
    startDeviceDiscovery(subscribeInfo: SubscribeInfo, filterOptions?: string): void;

    /**
     * Stop to discover device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { number } subscribeId Service subscribe ID
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified param is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#stopDiscovering
     */
    stopDeviceDiscovery(subscribeId: number): void;

    /**
     * Publish discover device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { PublishInfo } publishInfo publish info to Publish discovery device
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600105 - Publish unavailable.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    publishDeviceDiscovery(publishInfo: PublishInfo): void;

    /**
     * UnPublish discover device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { number } publishId Service publish ID, identify a publish operation, should be a unique id in package range
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    unPublishDeviceDiscovery(publishId: number): void;

    /**
     * Authenticate the specified device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { DeviceInfo } deviceInfo deviceInfo of device to authenticate
     * @param { AuthParam } authParam authParam of device to authenticate
     * @param { AsyncCallback<{ deviceId: string, pinToken?: number }> } callback Indicates the callback to be invoked upon authenticateDevice
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#bindTarget
     */
    authenticateDevice(
      deviceInfo: DeviceInfo,
      authParam: AuthParam,
      callback: AsyncCallback<{ deviceId: string, pinToken?: number }>
    ): void;

    /**
     * unAuthenticate the specified device.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { DeviceInfo } deviceInfo deviceInfo of device to unAuthenticate
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 11600101 - Failed to execute the function.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 8
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#unbindTarget
     */
    unAuthenticateDevice(deviceInfo: DeviceInfo): void;

    /**
     * verify auth info, such as pin code.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { AuthInfo } authInfo device auth info o verify
     * @param { AsyncCallback<{ deviceId: string, level: number }> } callback Indicates the callback to be invoked upon verifyAuthInfo
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     */
    verifyAuthInfo(authInfo: AuthInfo, callback: AsyncCallback<{ deviceId: string, level: number }>): void;

    /**
     * Set user Operation from devicemanager Sea, this interface can only be used by devicemanager Sea.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { number } operateAction User Operation Actions.
     * @param { string } params Indicates the input param of the user.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified params is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#replyUiAction
     */
    setUserOperation(operateAction: number, params: string): void;

    /**
     * Request credential information.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { string } requestInfo - Request credential params, the params is json string, it includes version and userId.
     * @param { AsyncCallback<{ registerInfo: string }> } callback Indicates the callback to be invoked upon requestCredential
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified requestInfo is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    requestCredentialRegisterInfo(requestInfo: string, callback: AsyncCallback<{ registerInfo: string }>): void;

    /**
     * Import credential information.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { string } credentialInfo - Import credential params, the params is json string, it includes processType, authType,
     * userId, deviceId, version, devicePk and credentialData, the credentialData is array, each array element
     * include credentialType, credentialId, serverPk, pkInfoSignature, pkInfo, authCode, peerDeviceId.
     * @param { AsyncCallback<{ resultInfo: string }> } callback Indicates the callback to be invoked upon importCredential.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified credentialInfo is greater than 5999.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    importCredential(credentialInfo: string, callback: AsyncCallback<{ resultInfo: string }>): void;

    /**
     * delete credential information.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { string } queryInfo - delete credential params. the params is json string, it includes processType, authType, userId.
     * @param { AsyncCallback<{ resultInfo: string }> } callback Indicates the callback to be invoked upon deleteCredential
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified queryInfo is greater than 5999.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 10
     * @deprecated since 11
     */
    deleteCredential(queryInfo: string, callback: AsyncCallback<{ resultInfo: string }>): void;

    /**
     * Register a callback from deviceManager service so that the devicemanager ui can be notified when ui statue
     * changes.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'uiStateChange' } type Ui state to unregister.
     * @param { Callback<{ param: string }> } callback Indicates the devicemanager ui state to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#on
     */
    on(type: 'uiStateChange', callback: Callback<{ param: string }>): void;

    /**
     * Unregister uiStatueChange, this interface can only be used by devicemanager ui.
     *ui state change
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'uiStateChange' } type Ui state to unregister.
     * @param { Callback<{ param: string }> } callback Indicates the devicemanager ui state to unregister.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#off
     */
    off(type: 'uiStateChange', callback?: Callback<{ param: string }>): void;

    /**
     * Register a device state callback so that the application can be notified upon device state changes based on
     * the application bundle name.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'deviceStateChange' } type Device status change.
     * @param { Callback<{ action: DeviceStateChangeAction, device: DeviceInfo }> } callback
     *          Indicates the device state callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#on
     */
    on(type: 'deviceStateChange', callback: Callback<{ action: DeviceStateChangeAction, device: DeviceInfo }>): void;

    /**
     * UnRegister device state callback based on the application bundle name.
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'deviceStateChange' } type Device status change.
     * @param { Callback<{ action: DeviceStateChangeAction, device: DeviceInfo }> } callback Indicates the device state callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#off
     */
    off(type: 'deviceStateChange', callback?: Callback<{ action: DeviceStateChangeAction, device: DeviceInfo }>): void;

    /**
     * Register a device found callback so that the application can be notified when the device was found
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'deviceFound' } type Successfully discovered device.
     * @param { Callback<{ subscribeId: number, device: DeviceInfo }> } callback Indicates the device found callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#on
     */
    on(type: 'deviceFound', callback: Callback<{ subscribeId: number, device: DeviceInfo }>): void;

    /**
     * UnRegister a device found callback so that the application can be notified when the device was found
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'deviceFound' } type Successfully discovered device.
     * @param { Callback<{ subscribeId: number, device: DeviceInfo }> } callback Indicates the device found callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#off
     */
    off(type: 'deviceFound', callback?: Callback<{ subscribeId: number, device: DeviceInfo }>): void;

    /**
     * Register a device found result callback so that the application can be notified when the device discover was failed
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'discoverFail' } type Discovery Device Failure.
     * @param { Callback<{ subscribeId: number, reason: number }> } callback Indicates the device found result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#on
     */
    on(type: 'discoverFail', callback: Callback<{ subscribeId: number, reason: number }>): void;

    /**
     * UnRegister a device found result callback so that the application can be notified when the device discover was failed
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'discoverFail' } type Discovery Device Failure.
     * @param { Callback<{ subscribeId: number, reason: number }> } callback Indicates the device found result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#off
     */
    off(type: 'discoverFail', callback?: Callback<{ subscribeId: number, reason: number }>): void;

    /**
     * Register a device publish result callback so that the application can be notified when the device publish success
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'publishSuccess' } type Successfully published device.
     * @param { Callback<{ publishId: number }> } callback Indicates the device publish result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    on(type: 'publishSuccess', callback: Callback<{ publishId: number }>): void;

    /**
     * UnRegister a device publish result callback so that the application can be notified when the device publish was failed
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'publishSuccess' } type Successfully published device.
     * @param { Callback<{ publishId: number }> } callback Indicates the device publish result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    off(type: 'publishSuccess', callback?: Callback<{ publishId: number }>): void;

    /**
     * Register a device publish result callback so that the application can be notified when the device publish was failed
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'publishFail' } type Failed to publish device.
     * @param { Callback<{ publishId: number, reason: number }> } callback Indicates the device publish result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    on(type: 'publishFail', callback: Callback<{ publishId: number, reason: number }>): void;

    /**
     * UnRegister a device publish result callback so that the application can be notified when the device publish was failed
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'publishFail' } type Failed to publish device.
     * @param { Callback<{ publishId: number, reason: number }> } callback Indicates the device publish result callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 9
     * @deprecated since 11
     */
    off(type: 'publishFail', callback?: Callback<{ publishId: number, reason: number }>): void;

    /**
     * Register a serviceError callback so that the application can be notified when devicemanager service died
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'serviceDie' } type Service death.
     * @param { function } callback Indicates the service error callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#on
     */
    on(type: 'serviceDie', callback: () => void): void;

    /**
     * UnRegister a serviceError callback so that the application can be notified when devicemanager service died
     *
     * @permission ohos.permission.ACCESS_SERVICE_DM
     * @param { 'serviceDie' } type Service death.
     * @param { function } callback Indicates the service error callback to register.
     * @throws { BusinessError } 401 - Parameter error. Possible causes:
     *                                                  1. Mandatory parameters are left unspecified;
     *                                                  2. Incorrect parameter type;
     *                                                  3. Parameter verification failed;
     *                                                  4. The size of specified eventType is greater than 255.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @syscap SystemCapability.DistributedHardware.DeviceManager
     * @systemapi this method can be used only by system applications.
     * @since 7
     * @deprecated since 11
     * @useinstead ohos.distributedDeviceManager/distributedDeviceManager.DeviceManager#off
     */
    off(type: 'serviceDie', callback?: () => void): void;
  }
}

export default deviceManager;
