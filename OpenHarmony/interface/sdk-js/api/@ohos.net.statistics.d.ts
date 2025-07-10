/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
 * @kit NetworkKit
 */

import type { AsyncCallback, Callback } from './@ohos.base';
import type connection from './@ohos.net.connection';

/**
 * Obtains traffic statistics.
 * @namespace statistics
 * @syscap SystemCapability.Communication.NetManager.Core
 * @since 10
 */
/**
 * Obtains traffic statistics.
 * @namespace statistics
 * @syscap SystemCapability.Communication.NetManager.Core
 * @atomicservice
 * @since 15
 */
declare namespace statistics {
  /**
   * @typedef {connection.NetBearType}
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 12
   */
  type NetBearType = connection.NetBearType;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through a specified NIC.
   * @param { string } nic - Network interface card.
   * @param { AsyncCallback<number> } callback - Returns the data traffic received through the specified NIC.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getIfaceRxBytes(nic: string, callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through a specified NIC.
   * @param { string } nic - Network interface card.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getIfaceRxBytes(nic: string): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through a specified NIC.
   * @param { string } nic - Network interface card.
   * @param { AsyncCallback<number> } callback - Returns the data traffic sent through the specified NIC.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getIfaceTxBytes(nic: string, callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through a specified NIC.
   * @param { string } nic - Network interface card.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getIfaceTxBytes(nic: string): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through the cellular network.
   * @param { AsyncCallback<number> } callback - Returns the data traffic received through the cellular network.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getCellularRxBytes(callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through the cellular network.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getCellularRxBytes(): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through the cellular network.
   * @param { AsyncCallback<number> } callback - Returns the data traffic sent through the cellular network.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getCellularTxBytes(callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through the cellular network.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @throws { BusinessError } 2103012 - Failed to obtain the NIC name.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getCellularTxBytes(): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through all NICs.
   * @param { AsyncCallback<number> } callback - Returns the data traffic received through all NICs.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through all NICs.
   * @param { AsyncCallback<number> } callback - Returns the data traffic received through all NICs.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @atomicservice
   * @since 15
   */
  function getAllRxBytes(callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through all NICs.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through all NICs.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @atomicservice
   * @since 15
   */
  function getAllRxBytes(): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through all NICs.
   * @param { AsyncCallback<number> } callback - Returns the data traffic sent through all NICs.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through all NICs.
   * @param { AsyncCallback<number> } callback - Returns the data traffic sent through all NICs.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @atomicservice
   * @since 15
   */
  function getAllTxBytes(callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through all NICs.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through all NICs.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @atomicservice
   * @since 15
   */
  function getAllTxBytes(): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received by a specified application.
   * @param { number } uid - Indicates the process ID of the application.
   * @param { AsyncCallback<number> } callback - Returns the data traffic received by the specified application.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getUidRxBytes(uid: number, callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received by a specified application.
   * @param { number } uid - Indicates the process ID of the application.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getUidRxBytes(uid: number): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent by a specified application.
   * @param { number } uid - Indicates the process ID of the application.
   * @param { AsyncCallback<number> } callback - Returns the data traffic sent by the specified application.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getUidTxBytes(uid: number, callback: AsyncCallback<number>): void;
  
  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent by a specified application.
   * @param { number } uid - Indicates the process ID of the application.
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103005 - Failed to read the system map.
   * @throws { BusinessError } 2103011 - Failed to create a system map.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 10
   */
  function getUidTxBytes(uid: number): Promise<number>;

  /**
   * Register notifications of network traffic updates.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { 'netStatsChange' } type - Indicates Event name.
   * @param { Callback<{ iface: string, uid?: number }> } callback - The callback of on.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Register notifications of network traffic updates.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { 'netStatsChange' } type - Indicates Event name.
   * @param { Callback<NetStatsChangeInfo> } callback - The callback of on.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function on(type: 'netStatsChange', callback: Callback<NetStatsChangeInfo>): void;

  /**
   * Unregister notifications of network traffic updates.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { 'netStatsChange' } type - Indicates Event name.
   * @param { Callback<{ iface: string, uid?: number }> } callback - The callback of off.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  /**
   * Unregister notifications of network traffic updates.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { 'netStatsChange' } type - Indicates Event name.
   * @param { Callback<NetStatsChangeInfo> } callback - The callback of off.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function off(type: 'netStatsChange', callback?: Callback<NetStatsChangeInfo>): void;

  /**
   * Get the traffic usage details of the network interface in the specified time period.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { IfaceInfo } ifaceInfo - Detailed query content. See {@link IfaceInfo}.
   * @param { AsyncCallback<NetStatsInfo> } callback - Returns the {@link NetStatsInfo} object;
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getTrafficStatsByIface(ifaceInfo: IfaceInfo, callback: AsyncCallback<NetStatsInfo>): void;
  
  /**
   * Get the traffic usage details of the network interface in the specified time period.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { IfaceInfo } ifaceInfo - Detailed query content. See {@link IfaceInfo}.
   * @returns { Promise<NetStatsInfo> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getTrafficStatsByIface(ifaceInfo: IfaceInfo): Promise<NetStatsInfo>;

  /**
   * Get the traffic usage details of the specified time period of the application.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { UidInfo } uidInfo - Detailed query content. See {@link UidInfo}.
   * @param { AsyncCallback<NetStatsInfo> } callback - Returns the {@link NetStatsInfo} object;
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getTrafficStatsByUid(uidInfo: UidInfo, callback: AsyncCallback<NetStatsInfo>): void;
  
  /**
   * Get the traffic usage details of the specified time period of the application.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { UidInfo } uidInfo - Detailed query content. See {@link UidInfo}.
   * @returns { Promise<NetStatsInfo> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getTrafficStatsByUid(uidInfo: UidInfo): Promise<NetStatsInfo>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through a specified sockfd.
   * @param { number } sockfd - Indicates the file descriptor of the given socket.
   * @param { AsyncCallback<number> } callback - Returns the data traffic bytes received by the specified sockfd.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 11
   */
  function getSockfdRxBytes(sockfd: number, callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) received through a specified sockfd.
   * @param { number } sockfd - Indicates the file descriptor of the given socket.
   * @returns { Promise<number> } Returns the data traffic bytes received by the specified sockfd.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 11
   */
  function getSockfdRxBytes(sockfd: number): Promise<number>;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through a specified sockfd.
   * @param { number } sockfd - Indicates the file descriptor of the given socket.
   * @param { AsyncCallback<number> } callback - Returns the data traffic bytes sent by the specified sockfd.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 11
   */
  function getSockfdTxBytes(sockfd: number, callback: AsyncCallback<number>): void;

  /**
   * Queries the data traffic (including all TCP and UDP data packets) sent through a specified sockfd.
   * @param { number } sockfd - Indicates the file descriptor of the given socket.
   * @returns { Promise<number> } Returns the data traffic bytes sent by the specified sockfd.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 11
   */
  function getSockfdTxBytes(sockfd: number): Promise<number>;

  /**
   * Parameters for obtaining detailed information on network interface traffic usage.
   * @interface IfaceInfo
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface IfaceInfo {
    /**
     * Network interface for querying traffic.
     * @type {string}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    iface: string;

    /**
     * Start time for querying traffic.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    startTime: number;

    /**
     * End time for querying traffic.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    endTime: number;
  }

  /**
   * Parameters for obtaining detailed information on application traffic usage.
   * @interface UidInfo
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface UidInfo {
    /**
     * See {@link IfaceInfo}
     * @type {IfaceInfo}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ifaceInfo: IfaceInfo;

    /**
     * Uid of app for querying traffic.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    uid: number;
  }

  /**
   * Detailed information of statistics.
   * @interface NetStatsInfo
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  export interface NetStatsInfo {
    /**
     * Bytes of received.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    rxBytes: number;

    /**
     * Bytes of send.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    txBytes: number;

    /**
     * Packets of received.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    rxPackets: number;

    /**
     * Packets of send.
     * @type {number}
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    txPackets: number;
  }

  /**
   * Used to monitor and manage the status and usage of network interfaces.
   * @interface NetStatsChangeInfo
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  export interface NetStatsChangeInfo {
    /**
     * Network interface for querying traffic.
     * @type { string }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    iface: string;
    /**
     * Network interface for querying traffic.
     * @type { ?number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    uid?: number;
  }

  /**
   * An {@link NetStatsInfo} array with start time and end time.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  export type NetStatsInfoSequence = {
    /**
     * Start time for querying traffic.
     * @type { number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    startTime: number;
    /**
     * End time for querying traffic.
     * @type { number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    endTime: number;
    /**
     * Detailed information of statistics.
     * @type { NetStatsInfo }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    info: NetStatsInfo;
  }[];

  /**
   * {@link NetStatsInfo} for every UID. Key is UID.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  export type UidNetStatsInfo = {
    [uid: number]: NetStatsInfo;
  };

  /**
   * Parameters for obtaining detailed information on specified network traffic usage.
   * @interface NetworkInfo
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  export interface NetworkInfo {
    /**
     * Network type for querying traffic.
     * @type { NetBearType }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    type: NetBearType;
    /**
     * Start time for querying traffic.
     * @type { number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    startTime: number;
    /**
     * End time for querying traffic.
     * @type { number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    endTime: number;
    /**
     * SIM card id for querying traffic.
     * @type { ?number }
     * @syscap SystemCapability.Communication.NetManager.Core
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    simId?: number;
  }

  /**
   * Get the traffic usage details of the specified network of all applications in the specified time period.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { NetworkInfo } networkInfo - Information about the network to be queried.
   * @returns { Promise<UidNetStatsInfo> } The statistics of the sim card.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function getTrafficStatsByNetwork(networkInfo: NetworkInfo): Promise<UidNetStatsInfo>;
  /**
   * Get the traffic usage sequence of the specified network of the application in the specified time period.
   * @permission ohos.permission.GET_NETWORK_STATS
   * @param { number } uid - UID with this parameter, get stats info of this UID.
   * @param { NetworkInfo } networkInfo - Information about the network to be queried.
   * @returns { Promise<NetStatsInfoSequence> } The statistics history of the sim card.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2100001 - Invalid parameter value.
   * @throws { BusinessError } 2100002 - Failed to connect to the service.
   * @throws { BusinessError } 2100003 - System internal error.
   * @throws { BusinessError } 2103017 - Failed to read the database.
   * @syscap SystemCapability.Communication.NetManager.Core
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  function getTrafficStatsByUidNetwork(uid: number, networkInfo: NetworkInfo): Promise<NetStatsInfoSequence>;
}

export default statistics;