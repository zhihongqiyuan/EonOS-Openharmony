/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
 * @kit BasicServicesKit
 */

/**
 * Provides APIs for managing USB-to-virtual serial ports and implementing USB-to-virtual serial port communication.
 * @namespace serialManager
 * @syscap SystemCapability.USB.USBManager.Serial
 * @since 19
 */
declare namespace serialManager {

  /**
   * Obtains the serial port device list.
   * @returns { Readonly<SerialPort>[]} Returns the list of serial port devices obtained.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function getPortList(): Readonly<SerialPort>[];

  /**
   * Checks whether a serial port device has the access right. The system applications have the access right by default.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @returns {boolean} Returns <b class="+ topic/ph hi-d/b " id="b48481049115520">true</b> if the device has the right; returns <b class="+ topic/ph hi-d/b " id="b192241323195610">false</b> otherwise.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 14400005 Database operation exception.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function hasSerialRight(portId: number): boolean;

  /**
   * Requests the right for accessing a serial port device from the user. The system applications do not need to call this API.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @returns { Promise<boolean>} Promise used to return the result. 
   * The value <b class="+ topic/ph hi-d/b " id="b529434412284">true</b> means the user allows the access;
   * the value <b class="+ topic/ph hi-d/b " id="b89755211297">false</b> means the opposite.
   * @throws { BusinessError } 401 Parameter error. Possible causes: 
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 14400005 Database operation exception.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function requestSerialRight(portId: number): Promise<boolean>;

  /**
   * Adds the access right for a serial port device.
   * @permission ohos.permission.MANAGE_USB_CONFIG
   * @param { number } tokenId Token ID of the target application.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @throws { BusinessError } 201 Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 202 Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 14400005 Database operation exception.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @systemapi
   * @since 19
   */
  function addSerialRight(tokenId: number, portId: number): void;

  /**
   * Cancels the access right for a serial port device. This API will not cancel the default access right of system applications.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 14400005 Database operation exception.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400002 Access denied. Call requestSerialRight to request user authorization first.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function cancelSerialRight(portId: number): void;

  /**
   * Opens a serial port device.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400002 Access denied. Call requestSerialRight to request user authorization first.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400004 The serial port device is occupied.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function open(portId: number): void;

  /**
   * Closes a serial port device.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function close(portId: number): void;

  /**
   * Obtains the communication parameters of a serial port device.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @returns { Readonly<SerialAttribute>} Communication parameters obtained, which are read-only.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function getAttribute(portId: number): Readonly<SerialAttribute>;

  /**
   * Sets the communication parameters for a serial port device.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @param { SerialAttribute} attribute Communication parameters to set.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function setAttribute(portId: number, attribute: SerialAttribute): void;

  /**
   * Reads data from a serial port device. This API uses a promise to return the result.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @param { Uint8Array } buffer Buffer for storing the data read, with a maximum length of 8192 bytes.
   * @param { number } timeout Timeout duration for reading data. The value is a non-negative number.
   * The default value <b class="+ topic/ph hi-d/b " id="b98612056312">0</b> indicates that there is no time limit for data reading.
   * @returns { Promise<number> } Promise used to return the length of the data read.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @throws { BusinessError } 31400006 Data transfer timed out.
   * @throws { BusinessError } 31400007 I/O exception. Possible causes:
   * <br>1. The transfer was canceled.
   * <br>2. The device offered more data than allowed.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function read(portId: number, buffer: Uint8Array, timeout?: number): Promise<number>;

  /**
   * Reads data from a serial port device. This API returns the result synchronously.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @param { Uint8Array } buffer Buffer for storing the data read, with a maximum length of 8192 bytes.
   * @param { number } timeout Timeout duration for reading data. The value is a non-negative number.
   * The default value <b class="+ topic/ph hi-d/b " id="b25229382302">0</b> indicates that there is no time limit for data reading.
   * @returns {number} Length of the data read.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @throws { BusinessError } 31400006 Data transfer timed out.
   * @throws { BusinessError } 31400007 I/O exception. Possible causes:
   * <br>1. The transfer was canceled.
   * <br>2. The device offered more data than allowed.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function readSync(portId: number, buffer: Uint8Array, timeout?: number): number;

  /**
   * Writes data to a serial port device. This API uses a promise to return the result.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @param { Uint8Array } buffer Buffer for storing the data written, with a maximum length of 128 KB.
   * @param { number } timeout Timeout duration for writing data. The value is a non-negative number.
   * The default value <b class="+ topic/ph hi-d/b " id="b2661577365">0</b> indicates that there is no time limit for data writing.
   * @returns { Promise<number> } Promise used to return the length of the data written.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @throws { BusinessError } 31400006 Data transfer timed out.
   * @throws { BusinessError } 31400007 I/O exception. Possible causes:
   * <br>1. The transfer was canceled.
   * <br>2. The device offered more data than allowed.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function write(portId: number, buffer: Uint8Array, timeout?: number): Promise<number>;

  /**
   * Writes data to a serial port device. This API returns the result synchronously.
   * @param { number} portId ID of the target device. For details, see <b>SerialPort.portId</b>.
   * @param { Uint8Array } buffer Buffer for storing the data written, with a maximum length of 128 KB.
   * @param { number } timeout Timeout duration for writing data. The value is a non-negative number.
   * The default value <b class="+ topic/ph hi-d/b " id="b45639213380">0</b> indicates that there is no time limit for data writing.
   * @returns { number } Length of the data written.
   * @throws { BusinessError } 401 Parameter error. Possible causes:
   * 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 31400001 Serial port management exception.
   * @throws { BusinessError } 31400003 PortId does not exist.
   * @throws { BusinessError } 31400005 The serial port device is not opened. Call the open API first.
   * @throws { BusinessError } 31400006 Data transfer timed out.
   * @throws { BusinessError } 31400007 I/O exception. Possible causes:
   * <br>1. The transfer was canceled.
   * <br>2. The device offered more data than allowed.
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  function writeSync(portId: number, buffer: Uint8Array, timeout?: number): number;

  /**
   * Represents a serial port device.
   * @typedef SerialPort
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  interface SerialPort {

    /**
     * Serial port device ID.
     * @type { number }
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    portId: number;

    /**
     * Serial port device name.
     * @type { string }
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    deviceName: string;
 }
  /**
   * Represents the communication parameters of a serial port device.
   * @typedef SerialAttribute
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  interface SerialAttribute {

    /**
     * Baud rate.
     * @type { BaudRates }
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    baudRate: BaudRates;

    /**
     * Data bits.
     * @type { DataBits }
     * @default DATABIT_8
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    dataBits?: DataBits;

    /**
     * Parity bit.
     * @type { Parity }
     * @default NONE
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    parity?: Parity;

    /**
     * Stop bits.
     * @type { StopBits }
     * @default STOPBIT_1
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    stopBits?: StopBits;
 }
  /**
   * Enumerates the baud rates of a serial port device, in bit/s.
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  enum BaudRates {

    /**
     * The baud rate is 50 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_50 = 50,

    /**
     * The baud rate is 75 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_75 = 75,

    /**
     * The baud rate is 110 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_110 = 110,

    /**
     * The baud rate is 134 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_134 = 134,

    /**
     * The baud rate is 150 bit/s.
     *
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_150 = 150,

    /**
     * The baud rate is 200 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_200 = 200,

    /**
     * The baud rate is 300 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_300 = 300,

    /**
     * The baud rate is 600 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_600 = 600,

    /**
     * The baud rate is 1200 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_1200 = 1200,

    /**
     * The baud rate is 1800 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_1800 = 1800,

    /**
     * The baud rate is 2400 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_2400 = 2400,

    /**
     * The baud rate is 4800 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_4800 = 4800,

    /**
     * The baud rate is 9600 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_9600 = 9600,

    /**
     * The baud rate is 19200 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_19200 = 19200,

    /**
     * The baud rate is 38400 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_38400 = 38400,

    /**
     * The baud rate is 57600 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_57600 = 57600,

    /**
     * The baud rate is 115200 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_115200 = 115200,

    /**
     * The baud rate is 230400 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_230400 = 230400,

    /**
     * The baud rate is 460800 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_460800 = 460800,

    /**
     * The baud rate is 500000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_500000 = 500000,

    /**
     * The baud rate is 576000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_576000 = 576000,

    /**
     * The baud rate is 921600 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_921600 = 921600,

    /**
     * The baud rate is 1000000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_1000000 = 1000000,

    /**
     * The baud rate is 1152000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_1152000 = 1152000,

    /**
     * The baud rate is 1500000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_1500000 = 1500000,

    /**
     * The baud rate is 2000000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_2000000 = 2000000,

    /**
     * The baud rate is 2500000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_2500000 = 2500000,

    /**
     * The baud rate is 3000000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_3000000 = 3000000,

    /**
     * The baud rate is 3500000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_3500000 = 3500000,

    /**
     * The baud rate is 4000000 bit/s.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    BAUDRATE_4000000 = 4000000
 }
  /**
   * Enumerates the data bits of a serial port device.
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  enum DataBits {

    /**
     * The number of data bits is 8.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    DATABIT_8 = 8,

    /**
     * The number of data bits is 7.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    DATABIT_7 = 7,

    /**
     * The number of data bits is 6.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    DATABIT_6 = 6,

    /**
     * The number of data bits is 5.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    DATABIT_5 = 5
 }
  /**
   * Enumerates the parity bits of a serial port device.
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  enum Parity {

    /**
     * No parity.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    PARITY_NONE = 0,

    /**
     * Odd parity.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    PARITY_ODD = 1,

    /**
     * Even parity.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    PARITY_EVEN = 2,

    /**
     * Mark parity, whose parity bit is always <b class="+ topic/ph hi-d/b " id="b1627884485115">1</b>.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    PARITY_MARK = 3,

    /**
     * Space parity, whose parity bit is always <b class="+ topic/ph hi-d/b " id="b1551818484291">0</b>.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    PARITY_SPACE = 4
 }
  /**
   * Enumerates the stop bits for serial port communication.
   * @enum { number }
   * @syscap SystemCapability.USB.USBManager.Serial
   * @since 19
   */
  enum StopBits {

    /**
     * The number of stop bits is 1.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    STOPBIT_1 = 0,

    /**
     * The number of stop bits is 2.
     * @syscap SystemCapability.USB.USBManager.Serial
     * @since 19
     */
    STOPBIT_2 = 1
 }
}

export default serialManager;