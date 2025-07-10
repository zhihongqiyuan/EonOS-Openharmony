/*
 * Copyright (C) 2022-2023 Huawei Device Co., Ltd.
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
 * Provides network sharing related interfaces.
 * @namespace sharing
 * @syscap SystemCapability.Communication.NetManager.NetSharing
 * @since 9
 */
declare namespace sharing {
  /**
   * Get the handle of the data network.
   * @typedef { connection.NetHandle }
   * @syscap SystemCapability.Communication.NetManager.Core
   * @since 9
   */
  type NetHandle = connection.NetHandle;

  /**
   * Checks whether this device allows for network sharing.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { AsyncCallback<boolean> } callback - Returns {@code true} indicating network sharing is supported;
   * returns {@code false} otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
  */
  function isSharingSupported(callback: AsyncCallback<boolean>): void;

  /**
   * Checks whether this device allows for network sharing.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @returns { Promise<boolean> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isSharingSupported(): Promise<boolean>;

  /**
   * Return the global network sharing state.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { AsyncCallback<boolean> } callback - Returns {@code true} indicating network sharing is running;
   * returns {@code false} otherwise.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function isSharing(callback: AsyncCallback<boolean>): void;

  /**
   * Return the global network sharing state.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @returns { Promise<boolean> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use. 
   * @since 9
   */
  function isSharing(): Promise<boolean>;

  /**
   * Start network sharing for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Enumeration of shareable interface types.
   * @param { AsyncCallback<void> } callback - the callback of startSharing.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202004 - Try to share an unavailable iface.
   * @throws { BusinessError } 2202005 - WiFi sharing failed.
   * @throws { BusinessError } 2202006 - Bluetooth sharing failed.
   * @throws { BusinessError } 2202009 - Failed to enable forwarding for network sharing.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function startSharing(type: SharingIfaceType, callback: AsyncCallback<void>): void;

  /**
   * Start network sharing for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Enumeration of shareable interface types.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202004 - Try to share an unavailable iface.
   * @throws { BusinessError } 2202005 - WiFi sharing failed.
   * @throws { BusinessError } 2202006 - Bluetooth sharing failed.
   * @throws { BusinessError } 2202009 - Failed to enable forwarding for network sharing.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function startSharing(type: SharingIfaceType): Promise<void>;

  /**
   * Stop network sharing for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Enumeration of shareable interface types.
   * @param { AsyncCallback<void> } callback - the callback of startSharing.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202004 - Try to share an unavailable iface.
   * @throws { BusinessError } 2202005 - WiFi sharing failed.
   * @throws { BusinessError } 2202006 - Bluetooth sharing failed.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function stopSharing(type: SharingIfaceType, callback: AsyncCallback<void>): void;

  /**
   * Stop network sharing for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Enumeration of shareable interface types.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @throws { BusinessError } 2202004 - Try to share an unavailable iface.
   * @throws { BusinessError } 2202005 - WiFi sharing failed.
   * @throws { BusinessError } 2202006 - Bluetooth sharing failed.
   * @throws { BusinessError } 2202011 - Cannot get network sharing configuration.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function stopSharing(type: SharingIfaceType): Promise<void>;

  /**
   * Obtains the number of downlink data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { AsyncCallback<number> } callback - Returns the number of downlink data bytes of the sharing network interfaces.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsRxBytes(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of downlink data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsRxBytes(): Promise<number>;

  /**
   * Obtains the number of uplink data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { AsyncCallback<number> } callback - Returns the number of uplink data bytes of the sharing network interfaces.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsTxBytes(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of uplink data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsTxBytes(): Promise<number>;

  /**
   * Obtains the number of total data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { AsyncCallback<number> } callback - Returns the number of total data bytes of the sharing network interfaces.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsTotalBytes(callback: AsyncCallback<number>): void;

  /**
   * Obtains the number of total data bytes of the sharing network interfaces.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @returns { Promise<number> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getStatsTotalBytes(): Promise<number>;

  /**
   * Obtains the names of interfaces in each sharing state.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceState } state - Is the network sharing state.
   * @param { AsyncCallback<Array<string>> } callback - Returns an array of interface names that meet this status.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharingIfaces(state: SharingIfaceState, callback: AsyncCallback<Array<string>>): void;

  /**
   * Obtains the names of interfaces in each sharing state.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceState } state - Is the network sharing state.
   * @returns { Promise<Array<string>> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharingIfaces(state: SharingIfaceState): Promise<Array<string>>;

  /**
   * Obtains the network sharing state for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Is the enumeration of shareable interface types.
   * @param { AsyncCallback<SharingIfaceState> } callback - the callback of getSharingState. {@code SharingIfaceState}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharingState(type: SharingIfaceType, callback: AsyncCallback<SharingIfaceState>): void;

  /**
   * Obtains the network sharing state for given type.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Is the enumeration of shareable interface types.
   * @returns { Promise<SharingIfaceState> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharingState(type: SharingIfaceType): Promise<SharingIfaceState>;

  /**
   * Get a list regular expression that defines any interface that can support network sharing.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Is the enumeration of shareable interface types.
   * @param { AsyncCallback<Array<string>> } callback - the callback of getSharableRegexes.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharableRegexes(type: SharingIfaceType, callback: AsyncCallback<Array<string>>): void;

  /**
   * Get a list regular expression that defines any interface that can support network sharing.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { SharingIfaceType } type - Is the enumeration of shareable interface types.
   * @returns { Promise<Array<string>> } The promise returned by the function. 
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 2200001 - Invalid parameter value.
   * @throws { BusinessError } 2200002 - Failed to connect to the service.
   * @throws { BusinessError } 2200003 - System internal error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function getSharableRegexes(type: SharingIfaceType): Promise<Array<string>>;

  /**
   * Register a callback for the global network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'sharingStateChange' } type - Indicates Event name.
   * @param { Callback<boolean> } callback - the callback function that returns the status.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function on(type: 'sharingStateChange', callback: Callback<boolean>): void;

  /**
   * Unregister a callback for the global network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'sharingStateChange' } type Indicates Event name.
   * @param { Callback<boolean> } callback - the callback function that returns the status.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function off(type: 'sharingStateChange', callback?: Callback<boolean>): void;

  /**
   * Register a callback for the interface network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'interfaceSharingStateChange' } type - Indicates Event name.
   * @param { Callback<{ type: SharingIfaceType, iface: string, state: SharingIfaceState }> } callback - the callback function that returns the message.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  /**
   * Register a callback for the interface network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'interfaceSharingStateChange' } type - Indicates Event name.
   * @param { Callback<InterfaceSharingStateInfo> } callback - the callback function that returns the message.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function on(type: 'interfaceSharingStateChange', callback: Callback<InterfaceSharingStateInfo>): void;

  /**
   * Unregister a callback for the interface network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'interfaceSharingStateChange' } type - Indicates Event name.
   * @param { Callback<{ type: SharingIfaceType, iface: string, state: SharingIfaceState }> } callback - the callback function that returns the message.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  /**
   * Unregister a callback for the interface network sharing state change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'interfaceSharingStateChange' } type - Indicates Event name.
   * @param { Callback<InterfaceSharingStateInfo> } callback - the callback function that returns the message.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function off(type: 'interfaceSharingStateChange', callback?: Callback<InterfaceSharingStateInfo>): void;

  /**
   * Register a callback for the sharing upstream network change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'sharingUpstreamChange' } type - Indicates Event name.
   * @param { Callback<NetHandle> } callback - the callback function that returns the network handle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function on(type: 'sharingUpstreamChange', callback: Callback<NetHandle>): void;

  /**
   * Unregister a callback for the sharing upstream network change.
   * @permission ohos.permission.CONNECTIVITY_INTERNAL
   * @param { 'sharingUpstreamChange' } type - Indicates Event name.
   * @param { Callback<NetHandle> } callback - the callback function that returns the network handle.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error.
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  function off(type: 'sharingUpstreamChange', callback?: Callback<NetHandle>): void;

  /**
   * Enumerates the network sharing states of an NIC.
   * @enum {number}
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export enum SharingIfaceState {
    /**
     * Indicates the names of the NICs that are serving as network sharing.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_NIC_SERVING = 1,

    /**
     * Indicates the names of the NICs that can serve as network sharing.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_NIC_CAN_SERVER = 2,

    /**
     * Indicates the names of the NICs that serving error.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_NIC_ERROR = 3
  }

  /**
   * The interface is used to notify listeners of changes in shared interface status.
   * @interface InterfaceSharingStateInfo
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  export interface InterfaceSharingStateInfo {
    /**
     * Enumerates the network sharing types of an NIC.
     * @type { SharingIfaceType }
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    type: SharingIfaceType;
    /**
     * The specified network interface name.
     * @type { string }
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    iface: string;
    /**
     * Network card sharing status.
     * @type { SharingIfaceState }
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    state: SharingIfaceState;
  }

  /**
   * Enumerates the network sharing types of an NIC.
   * @enum {number}
   * @syscap SystemCapability.Communication.NetManager.NetSharing
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  export enum SharingIfaceType {
    /**
     * Network sharing type for Wi-Fi.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_WIFI = 0,

    /**
     * Network sharing type for USB.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_USB = 1,

    /**
     * Network sharing type for BLUETOOTH.
     * @syscap SystemCapability.Communication.NetManager.NetSharing
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    SHARING_BLUETOOTH = 2
  }
}

export default sharing;
