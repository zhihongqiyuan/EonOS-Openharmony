/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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
 * @kit BasicServicesKit
 */

/**
 * @namespace usb
 * @syscap SystemCapability.USB.USBManager
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.usbManager
 */
declare namespace usb {
  /**
   * Obtains the USB device list.
   *
   * @returns { Array<Readonly<USBDevice>> } USB device list.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function getDevices(): Array<Readonly<USBDevice>>;

  /**
   * Connects to the USB device based on the device information returned by getDevices().
   *
   * @param { USBDevice } device - USB device on the device list returned by getDevices().
   * @returns { Readonly<USBDevicePipe> } object for data transfer.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function connectDevice(device: USBDevice): Readonly<USBDevicePipe>;

  /**
   * Checks whether the application has the permission to access the device.
   *
   * @param { string } deviceName - device name defined by USBDevice.name.
   * @returns { boolean } indicates if the user has the permission to access the device.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function hasRight(deviceName: string): boolean;

  /**
   * Requests the temporary permission for a given application to access the USB device.
   *
   * @param { string } deviceName - device name defined by USBDevice.name.
   * @returns { Promise<boolean> } indicates if the device access permissions are granted.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function requestRight(deviceName: string): Promise<boolean>;

  /**
   * Converts the string descriptor of a given USB function list to a numeric mask combination.
   *
   * @param { string } funcs - descriptor of the supported function list.
   * @returns { number } the numeric mask combination of the function list.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function usbFunctionsFromString(funcs: string): number;

  /**
   * Converts the numeric mask combination of a given USB function list to a string descriptor.
   *
   * @param { FunctionType } funcs - numeric mask combination of the function list.
   * @returns { string } - descriptor of the supported function list.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function usbFunctionsToString(funcs: FunctionType): string;

  /**
   * Sets the current USB function list in Device mode.
   *
   * @param { FunctionType } funcs - numeric mask combination of the supported function list.
   * @returns { Promise<boolean> } returns **true** if the setting is successful; returns **false** otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function setCurrentFunctions(funcs: FunctionType): Promise<boolean>;

  /**
   * Obtains the numeric mask combination for the current USB function list in Device mode.
   *
   * @returns { FunctionType } the numeric mask combination for the current USB function list in FunctionType.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function getCurrentFunctions(): FunctionType;

  /**
   * Obtains the USBPort list.
   *
   * @returns { Array<USBPort> } the USBPort list.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function getPorts(): Array<USBPort>;

  /**
   * Gets the mask combination for the supported mode list of the specified USBPort.
   *
   * @param { number } portId
   * @returns { PortModeType } the mask combination for the supported mode list in PortModeType.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function getSupportedModes(portId: number): PortModeType;

  /**
   * Sets the role types supported by the specified USBPort, which can be powerRole (for charging) and dataRole (for data transfer).
   *
   * @param { number } portId - unique ID of the port.
   * @param { PowerRoleType } powerRole - charging role.
   * @param { DataRoleType } dataRole - data role.
   * @returns { Promise<boolean> } returns **true** if the setting is successful; returns **false** otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  function setPortRoles(portId: number, powerRole: PowerRoleType, dataRole: DataRoleType): Promise<boolean>;

  /* usb pipe functions begin */
  /**
   * Claims a USB interface.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the bus number and device address.
   * @param { USBInterface } iface - USB interface , which is used to determine the interface to claim.
   * @param { boolean } [force] - optional parameter that determines whether to forcibly claim the USB interface.
   * @returns { number } returns **0** if the USB interface is successfully claimed; returns an error code otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function claimInterface(pipe: USBDevicePipe, iface: USBInterface, force?: boolean): number;

  /**
   * Releases a USB interface.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the bus number and device address.
   * @param { USBInterface } iface - USB interface , which is used to determine the interface to release.
   * @returns { number } returns **0** if the USB interface is successfully released; returns an error code otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function releaseInterface(pipe: USBDevicePipe, iface: USBInterface): number;

  /**
   * Sets the device configuration.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the bus number and device address.
   * @param { USBConfig } config - device configuration.
   * @returns { number } returns **0** if the device configuration is successfully set; returns an error code otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function setConfiguration(pipe: USBDevicePipe, config: USBConfig): number;

  /**
   * Sets a USB interface.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the bus number and device address.
   * @param { USBInterface } iface - USB interface , which is used to determine the interface to set.
   * @returns { number } returns **0** if the USB interface is successfully set; return an error code otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function setInterface(pipe: USBDevicePipe, iface: USBInterface): number;

  /**
   * Obtains the raw USB descriptor.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the bus number and device address.
   * @returns { Uint8Array } returns the raw descriptor data.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function getRawDescriptor(pipe: USBDevicePipe): Uint8Array;

  /**
   * Obtains the file descriptor.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the USB device.
   * @returns { number } returns the file descriptor of the USB device.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function getFileDescriptor(pipe: USBDevicePipe): number;

  /**
   * Performs control transfer.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the USB device.
   * @param { USBControlParams } controlparam - control transfer parameters.
   * @param { number } [timeout] - timeout duration. This parameter is optional. The default value is **0**, indicating no timeout.
   * @returns { Promise<number> } returns the size of the transmitted or received data block if the control transfer is successful;
   * return -1 if an exception occurs.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function controlTransfer(pipe: USBDevicePipe, controlparam: USBControlParams, timeout?: number): Promise<number>;

  /**
   * Performs bulk transfer.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the USB device.
   * @param { USBEndpoint } endpoint - USB endpoint, which is used to determine the USB port for data transfer.
   * @param { Uint8Array } buffer - buffer for writing or reading data.
   * @param { number } [timeout] - timeout duration. This parameter is optional. The default value is **0**, indicating no timeout.
   * @returns { Promise<number> } the size of the transmitted or received data block if the control transfer is successful;
   * return -1 if an exception occurs.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function bulkTransfer(
    pipe: USBDevicePipe,
    endpoint: USBEndpoint,
    buffer: Uint8Array,
    timeout?: number
  ): Promise<number>;

  /**
   * Closes a USB device pipe.
   *
   * @param { USBDevicePipe } pipe - device pipe , which is used to determine the USB device.
   * @returns { number } returns **0** if the USB device pipe is closed successfully; return an error code otherwise.
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  function closePipe(pipe: USBDevicePipe): number;

  /**
   * Represents the USB endpoint from which data is sent or received. You can obtain the USB endpoint through USBInterface.
   *
   * @typedef USBEndpoint
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBEndpoint {
    /**
     * Endpoint address
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    address: number;

    /**
     * Endpoint attributes
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    attributes: number;

    /**
     * Endpoint interval
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    interval: number;

    /**
     * Maximum size of data packets on the endpoint
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    maxPacketSize: number;

    /**
     * Endpoint direction
     *
     * @type { USBRequestDirection }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    direction: USBRequestDirection;

    /**
     * Endpoint number
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    number: number;

    /**
     * Endpoint type
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    type: number;

    /**
     * Unique ID defined by {@link USBInterface.id}, which indicates the interface to which the endpoint belongs
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    interfaceId: number;
  }

  /**
   * Represents a USB interface. One config can contain multiple **USBInterface** instances, each providing a specific function.
   *
   * @typedef USBInterface
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBInterface {
    /**
     * Unique ID of the USB interface
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    id: number;

    /**
     * Interface protocol
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    protocol: number;

    /**
     * Device type
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    clazz: number;

    /**
     * Device subclass
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    subClass: number;

    /**
     * Alternation between descriptors of the same USB interface
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    alternateSetting: number;

    /**
     * Interface name
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    name: string;

    /**
     * USBEndpoint that belongs to the USB interface
     *
     * @type { Array<USBEndpoint> }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    endpoints: Array<USBEndpoint>;
  }

  /**
   * USB configuration. One USBDevice can contain multiple USBConfig instances.
   *
   * @typedef USBConfig
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBConfig {
    /**
     * Unique ID of the USB configuration
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    id: number;

    /**
     * Configuration attributes
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    attributes: number;

    /**
     * Maximum power consumption, in mA
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    maxPower: number;

    /**
     * Configuration name, which can be left empty
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    name: string;

    /**
     * Support for remote wakeup
     *
     * @type { boolean }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    isRemoteWakeup: boolean;

    /**
     * Support for independent power supplies
     *
     * @type { boolean }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    isSelfPowered: boolean;

    /**
     * Supported interface
     *
     * @type { Array<USBInterface> }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    interfaces: Array<USBInterface>;
  }

  /**
   * Represents a USB device.
   *
   * @typedef USBDevice
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBDevice {
    /**
     * Bus address
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    busNum: number;

    /**
     * Device address
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    devAddress: number;

    /**
     * Device SN
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    serial: string;

    /**
     * Device name
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    name: string;

    /**
     * Device manufacturer
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    manufacturerName: string;

    /**
     * Product information
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    productName: string;

    /**
     * Product version
     *
     * @type { string }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    version: string;

    /**
     * Vendor ID
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    vendorId: number;

    /**
     * Product ID
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    productId: number;

    /**
     * Device class
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    clazz: number;

    /**
     * Device subclass
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    subClass: number;

    /**
     * Device protocol code
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    protocol: number;

    /**
     * Device configuration descriptor information
     *
     * @type { Array<USBConfig> }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    configs: Array<USBConfig>;
  }

  /**
   * Represents a USB device pipe, which is used to determine the USB device.
   *
   * @typedef USBDevicePipe
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBDevicePipe {
    /**
     * Bus address.
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    busNum: number;

    /**
     * Device address
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    devAddress: number;
  }

  /**
   * Enumerates power role types.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  export enum PowerRoleType {
    /**
     * None
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NONE = 0,

    /**
     * External power supply
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    SOURCE = 1,

    /**
     * Internal power supply
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    SINK = 2
  }

  /**
   * Enumerates data role types.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  export enum DataRoleType {
    /**
     * None
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NONE = 0,

    /**
     * Host mode
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    HOST = 1,

    /**
     * Device mode
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    DEVICE = 2
  }

  /**
   * Enumerates port mode types
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  export enum PortModeType {
    /**
     * None
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NONE = 0,

    /**
     * Upstream facing port, which functions as the sink of power supply
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    UFP = 1,

    /**
     * Downstream facing port, which functions as the source of power supply
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    DFP = 2,

    /**
     * Dynamic reconfiguration port (DRP), which can function as the DFP (host) or UFP (device). It is not supported currently.
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    DRP = 3,

    /**
     * Not supported currently
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NUM_MODES = 4
  }

  /**
   * Enumerates USB device port roles.
   *
   * @typedef USBPortStatus
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  interface USBPortStatus {
    /**
     * USB mode
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    currentMode: number;

    /**
     * Power role
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    currentPowerRole: number;

    /**
     * Data role
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    currentDataRole: number;
  }

  /**
   * Represents a USB device port.
   *
   * @typedef USBPort
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  interface USBPort {
    /**
     * Unique ID of the USB port
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    id: number;

    /**
     * Mask combination for the supported mode list of the USB port
     *
     * @type { PortModeType }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    supportedModes: PortModeType;

    /**
     * USB port role
     *
     * @type { USBPortStatus }
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    status: USBPortStatus;
  }

  /**
   * Represents control transfer parameters.
   *
   * @typedef USBControlParams
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  interface USBControlParams {
    /**
     * Request type
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    request: number;

    /**
     * Request target type
     *
     * @type { USBRequestTargetType }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    target: USBRequestTargetType;

    /**
     * Control request type
     *
     * @type { USBControlRequestType }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    reqType: USBControlRequestType;

    /**
     * Request parameter value
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    value: number;

    /**
     * Index of the parameter value
     *
     * @type { number }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    index: number;

    /**
     * Data written to or read from the buffer
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    data: Uint8Array;
  }

  /**
   * Enumerates USB request target types.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  export enum USBRequestTargetType {
    /**
     * USB device
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TARGET_DEVICE = 0,

    /**
     * USB interface
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TARGET_INTERFACE = 1,

    /**
     * Endpoint
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TARGET_ENDPOINT = 2,

    /**
     * Others
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TARGET_OTHER = 3
  }

  /**
   * Enumerates control request types.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  export enum USBControlRequestType {
    /**
     * Standard
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TYPE_STANDARD = 0,

    /**
     * Class
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TYPE_CLASS = 1,

    /**
     * Vendor
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_TYPE_VENDOR = 2
  }

  /**
   * Enumerates request directions.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @since 8
   * @deprecated since 9
   */
  export enum USBRequestDirection {
    /**
     * Request for writing data from the host to the device
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_DIR_TO_DEVICE = 0,

    /**
     * Request for reading data from the device to the host
     *
     * @syscap SystemCapability.USB.USBManager
     * @since 8
     * @deprecated since 9
     */
    USB_REQUEST_DIR_FROM_DEVICE = 0x80
  }

  /**
   * Enumerates function modes.
   *
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager
   * @systemapi
   * @since 9
   * @deprecated since 9
   */
  export enum FunctionType {
    /**
     * None
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NONE = 0,

    /**
     * Serial port device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    ACM = 1,

    /**
     * Ethernet port device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    ECM = 2,

    /**
     * HDC device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    HDC = 4,

    /**
     * MTP device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    MTP = 8,

    /**
     * PTP device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    PTP = 16,

    /**
     * RNDIS device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    RNDIS = 32,

    /**
     * MIDI device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    MIDI = 64,

    /**
     * Audio source device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    AUDIO_SOURCE = 128,

    /**
     * NCM device
     *
     * @syscap SystemCapability.USB.USBManager
     * @systemapi
     * @since 9
     * @deprecated since 9
     */
    NCM = 256
  }
}

export default usb;
