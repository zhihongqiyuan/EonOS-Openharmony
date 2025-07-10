/*
 * Copyright (C) 2021-2023 Huawei Device Co., Ltd.
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
 * @kit TelephonyKit
 */

import type { AsyncCallback } from './@ohos.base';
import type Context from './application/BaseContext';

/**
 * Provides the capabilities and methods for obtaining Short Message Service (SMS) management objects.
 *
 * @namespace sms
 * @syscap SystemCapability.Telephony.SmsMms
 * @since 6
 */
declare namespace sms {
  /**
   * Splits a long SMS message into multiple fragments.
   *
   * <p>If the length of an SMS message exceeds the maximum length allowed (140 bytes),
   * the SMS message is split into multiple segments for processing.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { string } content - Indicates the short message content, which cannot be {@code null}.
   * @param { AsyncCallback<Array<string>> } callback - Indicates the callback for getting a list of split segments,
   * which can be combined into a complete SMS message;
   * Returns an empty string if no permission is granted or the short message content is {@code null}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function splitMessage(content: string, callback: AsyncCallback<Array<string>>): void;

  /**
   * Splits a long SMS message into multiple fragments.
   *
   * <p>If the length of an SMS message exceeds the maximum length allowed (140 bytes),
   * the SMS message is split into multiple segments for processing.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { string } content - Indicates the short message content, which cannot be {@code null}.
   * @returns { Promise<Array<string>> } Returns a list of split segments, which can be combined into a complete SMS
   * message; Returns an empty string if no permission is granted or the short message content is {@code null}.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function splitMessage(content: string): Promise<Array<string>>;

  /**
   * Creates an SMS message instance based on the protocol data unit (PDU) and the specified SMS protocol.
   *
   * <p>After receiving the original PDU data, the system creates an SMS message instance according to the specified
   * SMS protocol.
   *
   * @param { Array<number> } pdu - Indicates the original data, which is obtained from the received SMS.
   * @param { string } specification - Indicates the SMS protocol type. The value {@code 3gpp} indicates GSM/UMTS/LTE
   * SMS, and the value {@code 3gpp2} indicates CDMA/LTE SMS.
   * @param { AsyncCallback<ShortMessage> } callback - Indicates the callback for getting an SMS message instance;
   * returns {@code null} if {@code pdu} is empty or {@code specification} is not supported.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  function createMessage(pdu: Array<number>, specification: string, callback: AsyncCallback<ShortMessage>): void;

  /**
   * Creates an SMS message instance based on the protocol data unit (PDU) and the specified SMS protocol.
   *
   * <p>After receiving the original PDU data, the system creates an SMS message instance according to the specified
   * SMS protocol.
   *
   * @param { Array<number> } pdu - Indicates the original data, which is obtained from the received SMS.
   * @param { string } specification - Indicates the SMS protocol type. The value {@code 3gpp} indicates GSM/UMTS/LTE
   * SMS, and the value {@code 3gpp2} indicates CDMA/LTE SMS.
   * @returns { Promise<ShortMessage> } Returns an SMS message instance;
   * returns {@code null} if {@code pdu} is empty or {@code specification} is not supported.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  function createMessage(pdu: Array<number>, specification: string): Promise<ShortMessage>;

  /**
   * Sends a text or data SMS message.
   *
   * <p>This method checks whether the length of an SMS message exceeds the maximum length. If the
   * maximum length is exceeded, the SMS message is split into multiple parts and sent separately.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { SendMessageOptions } options - Indicates the parameters and callback for sending the SMS message.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   * @deprecated since 10
   * @useinstead telephony.sms#sendShortMessage
   */
  function sendMessage(options: SendMessageOptions): void;

  /**
   * Sends a text or data SMS message.
   *
   * <p>This method checks whether the length of an SMS message exceeds the maximum length. If the
   * maximum length is exceeded, the SMS message is split into multiple parts and sent separately.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { SendMessageOptions } options - Indicates the parameters and callback for sending the SMS message.
   * @param { AsyncCallback<void> } callback - The callback of sendShortMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 10
   */
  function sendShortMessage(options: SendMessageOptions, callback: AsyncCallback<void>): void;

  /**
   * Sends a text or data SMS message.
   *
   * <p>This method checks whether the length of an SMS message exceeds the maximum length. If the
   * maximum length is exceeded, the SMS message is split into multiple parts and sent separately.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { SendMessageOptions } options - Indicates the parameters and callback for sending the SMS message.
   * @returns { Promise<void> } The promise returned by the sendShortMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 10
   */
  function sendShortMessage(options: SendMessageOptions): Promise<void>;

  /**
   * Sets the default SIM card for sending SMS messages. You can obtain the default SIM card by
   * using {@code getDefaultSmsSlotId}.
   *
   * @permission ohos.permission.SET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the default SIM card for sending SMS messages. The value {@code 0} indicates
   * card slot 1, and the value {@code 1} indicates card slot 2.
   * @param { AsyncCallback<void> } callback - The callback of setDefaultSmsSlotId.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300004 - Do not have sim card.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setDefaultSmsSlotId(slotId: number, callback: AsyncCallback<void>): void;

  /**
   * Sets the default SIM card for sending SMS messages. You can obtain the default SIM card by
   * using {@code getDefaultSmsSlotId}.
   *
   * @permission ohos.permission.SET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the default SIM card for sending SMS messages. The value {@code 0} indicates
   * card slot 1, and the value {@code 1} indicates card slot 2.
   * @returns { Promise<void> } The promise returned by the setDefaultSmsSlotId.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300004 - Do not have sim card.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setDefaultSmsSlotId(slotId: number): Promise<void>;

  /**
   * Obtains the default SIM card for sending SMS messages.
   *
   * @param { AsyncCallback<number> } callback - Indicates the callback for getting the default SIM card for sending SMS
   * messages.
   * Returns {@code 0} if the default SIM card for sending SMS messages is in card slot 1;
   * Returns {@code 1} if the default SIM card for sending SMS messages is in card slot 2.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 7
   */
  function getDefaultSmsSlotId(callback: AsyncCallback<number>): void;

  /**
   * Obtains the default SIM card for sending SMS messages.
   *
   * @returns { Promise<number> } Returns {@code 0} if the default SIM card for sending SMS messages is in card slot 1;
   * Returns {@code 1} if the default SIM card for sending SMS messages is in card slot 2.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 7
   */
  function getDefaultSmsSlotId(): Promise<number>;

  /**
   * Sets the address for the Short Message Service Center (SMSC) based on a specified slot ID.
   *
   * @permission ohos.permission.SET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the ID of the slot holding the SIM card for sending SMS messages.
   * @param { string } smscAddr - Indicates the SMSC address.
   * @param { AsyncCallback<void> } callback - The callback of setSmscAddr.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setSmscAddr(slotId: number, smscAddr: string, callback: AsyncCallback<void>): void;

  /**
   * Sets the address for the Short Message Service Center (SMSC) based on a specified slot ID.
   *
   * @permission ohos.permission.SET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the ID of the slot holding the SIM card for sending SMS messages.
   * @param { string } smscAddr - Indicates the SMSC address.
   * @returns { Promise<void> } The promise returned by the setSmscAddr.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setSmscAddr(slotId: number, smscAddr: string): Promise<void>;

  /**
   * Obtains the SMSC address based on a specified slot ID.
   *
   * @permission ohos.permission.GET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the ID of the slot holding the SIM card for sending SMS messages.
   * @param { AsyncCallback<string> } callback - Indicates the callback for getting the SMSC address.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function getSmscAddr(slotId: number, callback: AsyncCallback<string>): void;

  /**
   * Obtains the SMSC address based on a specified slot ID.
   *
   * @permission ohos.permission.GET_TELEPHONY_STATE
   * @param { number } slotId - Indicates the ID of the slot holding the SIM card for sending SMS messages.
   * @returns { Promise<string> } Returns the SMSC address.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function getSmscAddr(slotId: number): Promise<string>;

  /**
   * Returns whether a device is capable of sending and receiving SMS messages.
   *
   * @returns { boolean } Returns {@code true} if the device is capable of sending and receiving SMS messages;
   * Returns {@code false} otherwise.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 7
   */
  function hasSmsCapability(): boolean;

  /**
   * Add an SMS Message to SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { SimMessageOptions } options - Indicates SIM message options.
   * @param { AsyncCallback<void> } callback - The callback of addSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function addSimMessage(options: SimMessageOptions, callback: AsyncCallback<void>): void;

  /**
   * Add an SMS Message to SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { SimMessageOptions } options - Indicates SIM message options.
   * @returns { Promise<void> } The promise returned by the addSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function addSimMessage(options: SimMessageOptions): Promise<void>;

  /**
   * Delete an SMS Message from the SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @param { number } msgIndex - Indicates the message index.
   * @param { AsyncCallback<void> } callback - The callback of delSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function delSimMessage(slotId: number, msgIndex: number, callback: AsyncCallback<void>): void;

  /**
   * Delete an SMS Message from the SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @param { number } msgIndex - Indicates the message index.
   * @returns { Promise<void> } The promise returned by the delSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function delSimMessage(slotId: number, msgIndex: number): Promise<void>;

  /**
   * Update a SIM SMS of SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { UpdateSimMessageOptions } options - Indicates update SIM message options.
   * @param { AsyncCallback<void> } callback - The callback of updateSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function updateSimMessage(options: UpdateSimMessageOptions, callback: AsyncCallback<void>): void;

  /**
   * Update a SIM SMS of SIM card.
   *
   * @permission ohos.permission.RECEIVE_SMS and ohos.permission.SEND_MESSAGES
   * @param { UpdateSimMessageOptions } options - Indicates update SIM message options.
   * @returns { Promise<void> } The promise returned by the updateSimMessage.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function updateSimMessage(options: UpdateSimMessageOptions): Promise<void>;

  /**
   * Get all SMS records in SIM.
   *
   * @permission ohos.permission.RECEIVE_SMS
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @param { AsyncCallback<Array<SimShortMessage>> } callback - Indicates the callback for getting a
   * {@code SimShortMessage} object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function getAllSimMessages(slotId: number, callback: AsyncCallback<Array<SimShortMessage>>): void;

  /**
   * Get all SMS records in SIM.
   *
   * @permission ohos.permission.RECEIVE_SMS
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @returns { Promise<Array<SimShortMessage>> } Returns a {@code SimShortMessage} object.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function getAllSimMessages(slotId: number): Promise<Array<SimShortMessage>>;

  /**
   * Turn on or off Cell BroadCast.
   *
   * @permission ohos.permission.RECEIVE_SMS
   * @param { CBConfigOptions } options - Indicates cell broadcast configuration options.
   * @param { AsyncCallback<void> } callback - The callback of setCBConfig.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setCBConfig(options: CBConfigOptions, callback: AsyncCallback<void>): void;

  /**
   * Turn on or off Cell BroadCast.
   *
   * @permission ohos.permission.RECEIVE_SMS
   * @param { CBConfigOptions } options - Indicates cell broadcast configuration options.
   * @returns { Promise<void> } The promise returned by the setCBConfig.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  function setCBConfig(options: CBConfigOptions): Promise<void>;

  /**
   * Get an SMS segment encode relation information.
   *
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @param { string } message - Indicates short message.
   * @param { boolean } force7bit - Indicates whether to use 7 bit encoding.
   * @param { AsyncCallback<SmsSegmentsInfo> } callback - Indicates the callback for getting a {@code SmsSegmentsInfo}
   * object.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function getSmsSegmentsInfo(slotId: number, message: string, force7bit: boolean, callback: AsyncCallback<SmsSegmentsInfo>): void;

  /**
   * Get an SMS segment encode relation information.
   *
   * @param { number } slotId - Indicates the card slot index number, ranging from 0 to the maximum card slot index
   * number supported by the device.
   * @param { string } message - Indicates short message.
   * @param { boolean } force7bit - Indicates whether to use 7 bit encoding.
   * @returns { Promise<SmsSegmentsInfo> } Returns a {@code SmsSegmentsInfo} object.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function getSmsSegmentsInfo(slotId: number, message: string, force7bit: boolean): Promise<SmsSegmentsInfo>;

  /**
   * SMS over IMS is supported if IMS is registered and SMS is supported on IMS.
   *
   * @param { number } slotId - Indicates the default SIM card for Ims Sms. The value {@code 0} indicates card slot 1,
   * and the value {@code 1} indicates card slot 2.
   * @param { AsyncCallback<boolean> } callback - Indicates the callback of isImsSmsSupported.
   * Returns {@code true} if SMS over IMS is supported, {@code false} otherwise.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function isImsSmsSupported(slotId: number, callback: AsyncCallback<boolean>): void;

  /**
   * SMS over IMS is supported if IMS is registered and SMS is supported on IMS.
   *
   * @param { number } slotId - Indicates the default SIM card for Ims Sms. The value {@code 0} indicates card slot 1,
   * and the value {@code 1} indicates card slot 2.
   * @returns { Promise<boolean> } Returns {@code true} if SMS over IMS is supported, {@code false} otherwise.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function isImsSmsSupported(slotId: number): Promise<boolean>;

  /**
   * Gets SMS format supported on IMS. SMS over IMS format is either 3GPP or 3GPP2.
   *
   * @param { AsyncCallback<string> } callback - Indicates the callback for getting format, 3gpp, 3gpp2 or unknown.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function getImsShortMessageFormat(callback: AsyncCallback<string>): void;

  /**
   * Gets SMS format supported on IMS. SMS over IMS format is either 3GPP or 3GPP2.
   *
   * @returns { Promise<string> } Returns format, 3gpp, 3gpp2 or unknown.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function getImsShortMessageFormat(): Promise<string>;

  /**
   * Decode the message content.
   *
   * @param { string | Array<number> } mmsFilePathName - Indicates the path name of the multimedia message file.
   * @param { AsyncCallback<MmsInformation> } callback - Indicates the callback for getting a {@code MmsInformation}
   * object.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function decodeMms(mmsFilePathName: string | Array<number>, callback: AsyncCallback<MmsInformation>): void;

  /**
   * Decode the message content.
   *
   * @param { string | Array<number> } mmsFilePathName - Indicates the path name of the multimedia message file.
   * @returns { Promise<MmsInformation> } Returns a {@code MmsInformation} object.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function decodeMms(mmsFilePathName: string | Array<number>): Promise<MmsInformation>;

  /**
   * Encode the message content.
   *
   * @param { MmsInformation } mms - Indicates MMS messages.
   * @param { AsyncCallback<Array<number>> } callback - Indicates the callback for getting the result of MMS encoding.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function encodeMms(mms: MmsInformation, callback: AsyncCallback<Array<number>>): void;

  /**
   * Encode the message content.
   *
   * @param { MmsInformation } mms - Indicates MMS messages.
   * @returns { Promise<Array<number>> } Returns the result of MMS encoding.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  function encodeMms(mms: MmsInformation): Promise<Array<number>>;

  /**
   * Obtains the default SIM ID for sending SMS messages.
   *
   * @param { AsyncCallback<number> } callback - Returns the SIM ID of the default sms sim and
   * SIM ID will increase from 1.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300004 - Do not have sim card.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @throws { BusinessError } 8301001 - SIM card is not activated.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 10
   */
  function getDefaultSmsSimId(callback: AsyncCallback<number>): void;

  /**
   * Obtains the default SIM ID for sending SMS messages.
   *
   * @returns { Promise<number> } Returns the SIM ID of the default sms sim and
   * SIM ID will increase from 1.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300004 - Do not have sim card.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @throws { BusinessError } 8301001 - SIM card is not activated.
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 10
   */
  function getDefaultSmsSimId(): Promise<number>;

  /**
   * Defines the MMS message information.
   *
   * @interface MmsInformation
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsInformation {
    /**
     * Indicates the message type for the MMS message.
     *
     * @type { MessageType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageType: MessageType;

    /**
     * Indicates the PDU header type for the MMS message.
     *
     * @type { MmsSendReq | MmsSendConf | MmsNotificationInd | MmsRespInd | MmsRetrieveConf | MmsAcknowledgeInd |
     * MmsDeliveryInd | MmsReadOrigInd | MmsReadRecInd }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    mmsType: MmsSendReq | MmsSendConf | MmsNotificationInd | MmsRespInd | MmsRetrieveConf | MmsAcknowledgeInd | MmsDeliveryInd | MmsReadOrigInd | MmsReadRecInd;

    /**
     * Indicates the attachment for the MMS message.
     *
     * @type { ?Array<MmsAttachment> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    attachment?: Array<MmsAttachment>;
  }

  /**
   * Sends an MMS message.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { Context } context - Indicates the context of application or capability.
   * @param { MmsParams } mmsParams - Indicates the parameters of the MMS message.
   * @param { AsyncCallback<void> } callback - The callback of sendMms. For error code, see MmsFailCode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function sendMms(context: Context, mmsParams: MmsParams, callback: AsyncCallback<void>): void;

  /**
   * Sends an MMS message.
   *
   * @permission ohos.permission.SEND_MESSAGES
   * @param { Context } context - Indicates the context of application or capability.
   * @param { MmsParams } mmsParams - Indicates the parameters of the MMS message.
   * @returns { Promise<void> } The promise returned by the sendMms.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types. 3. Parameter verification failed.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function sendMms(context: Context, mmsParams: MmsParams): Promise<void>;

  /**
   * Downloads an MMS message.
   *
   * @permission ohos.permission.RECEIVE_MMS
   * @param { Context } context - Indicates the context of application or capability.
   * @param { MmsParams }  mmsParams - Indicates the parameters of the MMS message.
   * @param { AsyncCallback<void> } callback - The callback of downloadMms. For error code, see MmsFailCode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function downloadMms(context: Context, mmsParams: MmsParams, callback: AsyncCallback<void>): void;

  /**
   * Downloads an MMS message.
   *
   * @permission ohos.permission.RECEIVE_MMS
   * @param { Context } context - Indicates the context of application or capability.
   * @param { MmsParams }  mmsParams - Indicates the parameters of the MMS message.
   * @returns { Promise<void> } The promise returned by the downloadMms. For error code, see MmsFailCode.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Non-system applications use system APIs.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified. 2.
   * Incorrect parameter types.
   * @throws { BusinessError } 8300001 - Invalid parameter value.
   * @throws { BusinessError } 8300002 - Operation failed. Cannot connect to service.
   * @throws { BusinessError } 8300003 - System internal error.
   * @throws { BusinessError } 8300999 - Unknown error code.
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  function downloadMms(context: Context, mmsParams: MmsParams): Promise<void>;

  /**
   * Defines the MMS message param.
   *
   * @interface MmsParams
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  export interface MmsParams {
    /**
     * Indicates the ID of the SIM card slot used for sending the MMS message.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    slotId: number;

    /**
     * Indicates the MMSC used for sending the MMS message.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    mmsc: string;

    /**
     * Indicates the MMS pdu url used for sending the MMS message.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    data: string;

    /**
     * Indicates the MMS UA and MMS UaProf used for sending the MMS message.
     *
     * @type { ?MmsConfig }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    mmsConfig?: MmsConfig;
  }

  /**
   * Defines the MMS message config.
   *
   * @interface MmsConfig
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 11
   */
  export interface MmsConfig {
    /**
     * Indicates the user agent used for the MMS message.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    userAgent: string;

    /**
     * Indicates the user agent profile for the MMS message.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    userAgentProfile: string;
  }

  /**
   * Defines an MMS message sending request.
   *
   * @interface MmsSendReq
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsSendReq {
    /**
     * Indicates the source address for the MMS message sending request.
     *
     * @type { MmsAddress }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    from: MmsAddress;

    /**
     * Indicates the transaction ID for the MMS message sending request.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the content type for the MMS message sending request.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentType: string;

    /**
     * Indicates the version for the MMS message sending request.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the destination address for the MMS message sending request.
     *
     * @type { ?Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    to?: Array<MmsAddress>;

    /**
     * Indicates the date for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    date?: number;

    /**
     * Indicates the carbon copy address for the MMS message sending request.
     *
     * @type { ?Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    cc?: Array<MmsAddress>;

    /**
     * Indicates the blind carbon copy address for the MMS message sending request.
     *
     * @type { ?Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    bcc?: Array<MmsAddress>;

    /**
     * Indicates the subject for the MMS message sending request.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    subject?: string;

    /**
     * Indicates the message class for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageClass?: number;

    /**
     * Indicates the expiration for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    expiry?: number;

    /**
     * Indicates the priority for the MMS message sending request.
     *
     * @type { ?MmsPriorityType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    priority?: MmsPriorityType;

    /**
     * Indicates the sender visibility for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    senderVisibility?: number;

    /**
     * Indicates the delivery report for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    deliveryReport?: number;

    /**
     * Indicates the read report for the MMS message sending request.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    readReport?: number;
  }

  /**
   * Defines the MMS message sending configuration.
   *
   * @interface MmsSendConf
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsSendConf {
    /**
     * Indicates the response status for the MMS message sending configuration.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    responseState: number;

    /**
     * Indicates the transaction ID for the MMS message sending configuration.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the version for the MMS message sending configuration.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the message ID for the MMS message sending configuration.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageId?: string;
  }

  /**
   * Defines an MMS notification indication.
   *
   * @interface MmsNotificationInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsNotificationInd {
    /**
     * Indicates the transaction ID for the MMS notification indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the message class for the MMS notification indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageClass: number;

    /**
     * Indicates the message size for the MMS notification indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageSize: number;

    /**
     * Indicates the expiration for the MMS notification indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    expiry: number;

    /**
     * Indicates the content location for the MMS notification indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentLocation: string;

    /**
     * Indicates the version for the MMS notification indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the source address for the MMS notification indication.
     *
     * @type { ?MmsAddress }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    from?: MmsAddress;

    /**
     * Indicates the subject for the MMS notification indication.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    subject?: string;

    /**
     * Indicates the status report for the MMS notification indication.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    deliveryReport?: number;

    /**
     * Indicates the content class for the MMS notification indication.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentClass?: number;
  }

  /**
   * Defines an MMS response indication.
   *
   * @interface MmsRespInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsRespInd {
    /**
     * Indicates the event ID for the MMS response indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the status for the MMS response indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    status: number;

    /**
     * Indicates the version for the MMS response indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the report allowed for the MMS response indication.
     *
     * @type { ?ReportType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    reportAllowed?: ReportType;
  }

  /**
   * Defines the MMS message retrieval configuration.
   *
   * @interface MmsRetrieveConf
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsRetrieveConf {
    /**
     * Indicates the transaction ID for the MMS message retrieval configuration.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the message ID for the MMS message retrieval configuration.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageId: string;

    /**
     * Indicates the date for the MMS message retrieval configuration.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    date: number;

    /**
     * Indicates the content type for the MMS message retrieval configuration.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentType: string;

    /**
     * Indicates the destination address for the MMS message retrieval configuration.
     *
     * @type { Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    to: Array<MmsAddress>;

    /**
     * Indicates the version for the MMS message retrieval configuration.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the source address for the MMS message retrieval configuration.
     *
     * @type { ?MmsAddress }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    from?: MmsAddress;

    /**
     * Indicates the carbon copy address for the MMS message retrieval configuration.
     *
     * @type { ?Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    cc?: Array<MmsAddress>;

    /**
     * Indicates the subject for the MMS message retrieval configuration.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    subject?: string;

    /**
     * Indicates the priority for the MMS message retrieval configuration.
     *
     * @type { ?MmsPriorityType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    priority?: MmsPriorityType;

    /**
     * Indicates the status report for the MMS message retrieval configuration.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    deliveryReport?: number;

    /**
     * Indicates the read report for the MMS message retrieval configuration.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    readReport?: number;

    /**
     * Indicates the retrieval status for the MMS message retrieval configuration.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    retrieveStatus?: number;

    /**
     * Indicates the retrieval text for the MMS message retrieval configuration.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    retrieveText?: string;
  }

  /**
   * Defines an MMS confirmation indication.
   *
   * @interface MmsAcknowledgeInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsAcknowledgeInd {
    /**
     * Indicates the transaction ID for the MMS confirmation indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    transactionId: string;

    /**
     * Indicates the version for the MMS confirmation indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the report allowed for the MMS confirmation indication.
     *
     * @type { ?ReportType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    reportAllowed?: ReportType;
  }

  /**
   * Defines an MMS message delivery indication.
   *
   * @interface MmsDeliveryInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsDeliveryInd {
    /**
     * Indicates the message ID for the MMS message delivery indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageId: string;

    /**
     * Indicates the date for the MMS message delivery indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    date: number;

    /**
     * Indicates the destination address for the MMS message delivery indication.
     *
     * @type { Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    to: Array<MmsAddress>;

    /**
     * Indicates the status for the MMS message delivery indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    status: number;

    /**
     * Indicates the version for the MMS message delivery indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;
  }

  /**
   * Defines the original MMS message reading indication.
   *
   * @interface MmsReadOrigInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsReadOrigInd {
    /**
     * Indicates the version for the original MMS message reading indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the message ID for the original MMS message reading indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageId: string;

    /**
     * Indicates the destination address for the original MMS message reading indication.
     *
     * @type { Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    to: Array<MmsAddress>;

    /**
     * Indicates the source address for the original MMS message reading indication.
     *
     * @type { MmsAddress }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    from: MmsAddress;

    /**
     * Indicates the date for the original MMS message reading indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    date: number;

    /**
     * Indicates the read status for the original MMS message reading indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    readStatus: number;
  }

  /**
   * Defines the MMS message reading indication.
   *
   * @interface MmsReadRecInd
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsReadRecInd {
    /**
     * Indicates the version for the MMS message reading indication.
     *
     * @type { MmsVersionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    version: MmsVersionType;

    /**
     * Indicates the message ID for the MMS message reading indication.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    messageId: string;

    /**
     * Indicates the destination address for the MMS message reading indication.
     *
     * @type { Array<MmsAddress> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    to: Array<MmsAddress>;

    /**
     * Indicates the source address for the MMS message reading indication.
     *
     * @type { MmsAddress }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    from: MmsAddress;

    /**
     * Indicates the read status for the MMS message reading indication.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    readStatus: number;

    /**
     * Indicates the date for the MMS message reading indication.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    date?: number;
  }

  /**
   * Defines the attachment of an MMS message.
   *
   * @interface MmsAttachment
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsAttachment {
    /**
     * Indicates the content ID for the attachment.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentId: string;

    /**
     * Indicates the content location.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentLocation: string;

    /**
     * Indicates the content disposition for the attachment.
     *
     * @type { DispositionType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentDisposition: DispositionType;

    /**
     * Indicates the encoding for content transfer.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentTransferEncoding: string;

    /**
     * Indicates the content type for the attachment.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    contentType: string;

    /**
     * Indicates whether the synchronized multimedia integration language is used.
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    isSmil: boolean;

    /**
     * Indicates the path for the attachment.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    path?: string;

    /**
     * Indicates whether the message is in the buffer.
     *
     * @type { ?Array<number> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    inBuff?: Array<number>;

    /**
     * Indicates the file name for the attachment.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    fileName?: string;

    /**
     * Indicates the character set for the attachment.
     *
     * @type { ?MmsCharSets }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    charset?: MmsCharSets;
  }

  /**
   * Defines an MMSC address.
   *
   * @interface MmsAddress
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface MmsAddress {
    /**
     * Indicates the network address for the MMSC address.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    address: string;

    /**
     * Indicates the character set for the MMSC address.
     *
     * @type { MmsCharSets }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    charset: MmsCharSets;
  }

  /**
   * Enumerates message type.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum MessageType {
    /**
     * Indicates an MMS message sending request.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_SEND_REQ = 128,

    /**
     * Indicates an MMS message sending configuration.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_SEND_CONF,

    /**
     * Indicates an MMS notification.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_NOTIFICATION_IND,

    /**
     * Indicates an MMS message response.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_RESP_IND,

    /**
     * Indicates an MMS message retrieval configuration.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_RETRIEVE_CONF,

    /**
     * Indicates the type of multimedia message confirmation index.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_ACKNOWLEDGE_IND,

    /**
     * Indicates an MMS message delivery.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_DELIVERY_IND,

    /**
     * Indicates an MMS message read report on the recipient side.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_READ_REC_IND,

    /**
     * Indicates an original MMS message read report on the originating side.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_MMS_READ_ORIG_IND,
  }

  /**
   * Enumerates MMS message priorities.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum MmsPriorityType {
    /**
     * Indicates low priority.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_LOW = 128,

    /**
     * Indicates normal priority.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_NORMAL,

    /**
     * Indicates high priority.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_HIGH,
  }

  /**
   * Enumerates MMS versions.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum MmsVersionType {
    /**
     * Indicates MMS version 1_0.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_VERSION_1_0 = 0x10,

    /**
     * Indicates MMS version 1_1.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_VERSION_1_1,

    /**
     * Indicates MMS version 1_2.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_VERSION_1_2,

    /**
     * Indicates MMS version 1_3.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_VERSION_1_3,
  }

  /**
   * Enumerates MMS character sets.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum MmsCharSets {
    /**
     * Indicates the BIG5 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    BIG5 = 0X07EA,

    /**
     * Indicates the ISO_10646_UCS_2 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_10646_UCS_2 = 0X03E8,

    /**
     * Indicates the ISO_8859_1 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_1 = 0X04,

    /**
     * Indicates the ISO_8859_2 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_2,

    /**
     * Indicates the ISO_8859_3 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_3,

    /**
     * Indicates the ISO_8859_4 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_4,

    /**
     * Indicates the ISO_8859_5 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_5,

    /**
     * Indicates the ISO_8859_6 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_6,

    /**
     * Indicates the ISO_8859_7 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_7,

    /**
     * Indicates the ISO_8859_8 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_8,

    /**
     * Indicates the ISO_8859_9 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ISO_8859_9,

    /**
     * Indicates the SHIFT_JIS format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SHIFT_JIS = 0X11,

    /**
     * Indicates the US_ASCII format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    US_ASCII = 0X03,

    /**
     * Indicates the UTF_8 format.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    UTF_8 = 0X6A,
  }

  /**
   * Enumerates disposition types.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum DispositionType {
    /**
     * Indicates the data source type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FROM_DATA = 0,

    /**
     * Indicates the attachment type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ATTACHMENT,

    /**
     * Indicates the inlining type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    INLINE,
  }

  /**
   * Enumerates report types.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum ReportType {
    /**
     * Indicates that a report is required.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_YES = 128,

    /**
     * Indicates that a report is not required.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    MMS_NO,
  }

  /**
   * Defines the cell broadcast configuration options.
   *
   * @interface CBConfigOptions
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export interface CBConfigOptions {
    /**
     * Indicates the card slot ID for the cell broadcast configuration options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    slotId: number;

    /**
     * Indicates whether to enable cell broadcast.
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    enable: boolean;

    /**
     * Indicates the start message ID for the cell broadcast configuration options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    startMessageId: number;

    /**
     * Indicates the end message ID for the cell broadcast configuration options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    endMessageId: number;

    /**
     * Indicates the RAN type for the cell broadcast configuration options.
     *
     * @type { RanType }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    ranType: RanType;
  }

  /**
   * Defines the SIM message options.
   *
   * @interface SimMessageOptions
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export interface SimMessageOptions {
    /**
     * Indicates the card slot ID for the SIM message options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    slotId: number;

    /**
     * Indicates the short message service center for the SIM message options.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    smsc: string;

    /**
     * Indicates the protocol data unit for the SIM message options.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    pdu: string;

    /**
     * Indicates the status for the SIM message options.
     *
     * @type { SimMessageStatus }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    status: SimMessageStatus;
  }

  /**
   * Defines the updating SIM message options.
   *
   * @interface UpdateSimMessageOptions
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export interface UpdateSimMessageOptions {
    /**
     * Indicates the card slot ID for the updating SIM message options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    slotId: number;

    /**
     * Indicates the message index for the updating SIM message options.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    msgIndex: number;

    /**
     * Indicates the new status for the updating SIM message options.
     *
     * @type { SimMessageStatus }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    newStatus: SimMessageStatus;

    /**
     * Indicates the protocol data unit for the updating SIM message options.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    pdu: string;

    /**
     * Indicates the short message service center for the updating SIM message options.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    smsc: string;
  }

  /**
   * Defines an SMS message instance.
   *
   * @interface ShortMessage
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export interface ShortMessage {
    /**
     * Indicates the SMS message body.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    visibleMessageBody: string;

    /**
     * Indicates the address of the sender, which is to be displayed on the UI.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    visibleRawAddress: string;

    /**
     * Indicates the SMS type.
     *
     * @type { ShortMessageClass }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    messageClass: ShortMessageClass;

    /**
     * Indicates the protocol identifier.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    protocolId: number;

    /**
     * Indicates the short message service center (SMSC) address.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    scAddress: string;

    /**
     * Indicates the SMSC timestamp.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    scTimestamp: number;

    /**
     * Indicates whether the received SMS is a "replace short message".
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    isReplaceMessage: boolean;

    /**
     * Indicates whether the received SMS contains "TP-Reply-Path".
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    hasReplyPath: boolean;

    /**
     * Indicates Protocol Data Units (PDUs) from an SMS message.
     *
     * @type { Array<number> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    pdu: Array<number>;

    /**
     * Indicates the SMS message status from the SMS-STATUS-REPORT message sent by the
     * Short Message Service Center (SMSC).
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    status: number;

    /**
     * Indicates whether the current message is SMS-STATUS-REPORT.
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    isSmsStatusReportMessage: boolean;
  }

  /**
   * Defines a SIM message.
   *
   * @interface SimShortMessage
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export interface SimShortMessage {
    /**
     * Indicates the SMS message in the SIM.
     *
     * @type { ShortMessage }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    shortMessage: ShortMessage;

    /**
     * Indicates the storage status of SMS messages in the SIM.
     *
     * @type { SimMessageStatus }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    simMessageStatus: SimMessageStatus;

    /**
     * Indicates the index of SMS messages in the SIM.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    indexOnSim: number;
  }

  /**
   * Defines the SIM message status.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export enum SimMessageStatus {
    /**
     * Status free space on SIM.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    SIM_MESSAGE_STATUS_FREE = 0,

    /**
     * Indicates a read message.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    SIM_MESSAGE_STATUS_READ = 1,

    /**
     * Indicates an unread message.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    SIM_MESSAGE_STATUS_UNREAD = 3,

    /**
     * Indicates a sent message (only applicable to SMS).
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    SIM_MESSAGE_STATUS_SENT = 5,

    /**
     * Indicates an unsent message (only applicable to SMS).
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    SIM_MESSAGE_STATUS_UNSENT = 7,
  }

  /**
   * Enumerates SMS message types.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export enum ShortMessageClass {
    /**
     * Indicates an unknown type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    UNKNOWN,

    /**
     * Indicates an instant message, which is displayed immediately after being received.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    INSTANT_MESSAGE,

    /**
     * Indicates an SMS message that can be stored on the device or SIM card based on the storage status.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    OPTIONAL_MESSAGE,

    /**
     * Indicates an SMS message containing SIM card information, which is to be stored in a SIM card.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    SIM_MESSAGE,

    /**
     * Indicates an SMS message to be forwarded to another device.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    FORWARD_MESSAGE
  }

  /**
   * Provides the options (including callbacks) for sending an SMS message.
   *
   * @interface SendMessageOptions
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export interface SendMessageOptions {
    /**
     * Indicates the ID of the SIM card slot used for sending the SMS message.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    slotId: number;

    /**
     * Indicates the address to which the SMS message is sent.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    destinationHost: string;

    /**
     * Indicates the SMSC address. If the value is {@code null}, the default SMSC address of the SIM card.
     *
     * @type { ?string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    serviceCenter?: string;

    /**
     * If the content is a string, this is a short message. If the content is a byte array, this is a data message.
     *
     * @type { string | Array<number> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    content: string | Array<number>;

    /**
     * If send data message, destinationPort is mandatory. Otherwise is optional.
     *
     * @type { ?number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    destinationPort?: number;

    /**
     * Indicates the callback invoked after the SMS message is sent.
     *
     * @type { ?AsyncCallback<ISendShortMessageCallback> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    sendCallback?: AsyncCallback<ISendShortMessageCallback>;

    /**
     * Indicates the callback invoked after the SMS message is delivered.
     *
     * @type { ?AsyncCallback<IDeliveryShortMessageCallback> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    deliveryCallback?: AsyncCallback<IDeliveryShortMessageCallback>;
  }

  /**
   * Provides the callback for the SMS message sending result.
   *
   * @interface ISendShortMessageCallback
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export interface ISendShortMessageCallback {
    /**
     * Indicates the SMS message sending result.
     *
     * @type { SendSmsResult }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    result: SendSmsResult;

    /**
     * Indicates the URI to store the sent SMS message.
     *
     * @type { string }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    url: string;

    /**
     * Specifies whether this is the last part of a multi-part SMS message.
     *
     * @type { boolean }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    isLastPart: boolean;
  }

  /**
   * Provides the callback for the SMS message delivery report.
   *
   * @interface IDeliveryShortMessageCallback
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export interface IDeliveryShortMessageCallback {
    /**
     * Indicates the SMS delivery report.
     *
     * @type { Array<number> }
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    pdu: Array<number>;
  }

  /**
   * Enumerates SMS message sending results.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @since 6
   */
  export enum SendSmsResult {
    /**
     * Indicates that the SMS message is successfully sent.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    SEND_SMS_SUCCESS = 0,

    /**
     * Indicates that sending the SMS message fails due to an unknown reason.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    SEND_SMS_FAILURE_UNKNOWN = 1,

    /**
     * Indicates that sending the SMS fails because the modem is powered off.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    SEND_SMS_FAILURE_RADIO_OFF = 2,

    /**
     * Indicates that sending the SMS message fails because the network is unavailable
     * or does not support sending or reception of SMS messages.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @since 6
     */
    SEND_SMS_FAILURE_SERVICE_UNAVAILABLE = 3
  }

  /**
   * Enumerates RAN type.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 7
   */
  export enum RanType {
    /**
     * Indicates GSM type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    TYPE_GSM = 1,

    /**
     * Indicates CDMA type.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    TYPE_CDMA = 2,
  }

  /**
   * Defines the SMS message segment information.
   *
   * @interface SmsSegmentsInfo
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export interface SmsSegmentsInfo {
    /**
     * Indicates the split count for the SMS message segment information.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    splitCount: number;

    /**
     * Indicates the encoding count for the SMS message segment information.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    encodeCount: number;

    /**
     * Indicates the remaining encoding count for the SMS message segment information.
     *
     * @type { number }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    encodeCountRemaining: number;

    /**
     * Indicates the encoding scheme for the SMS message segment information.
     *
     * @type { SmsEncodingScheme }
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    scheme: SmsEncodingScheme;
  }

  /**
   * Enumerates SMS encoding schemes.
   *
   * @enum { number }
   * @syscap SystemCapability.Telephony.SmsMms
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  export enum SmsEncodingScheme {
    /**
     * Indicates an unknown encoding schemes.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SMS_ENCODING_UNKNOWN = 0,

    /**
     * Indicates that the encoding schemes is 7-digit.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SMS_ENCODING_7BIT,

    /**
     * Indicates that the encoding schemes is 8-digit.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SMS_ENCODING_8BIT,

    /**
     * Indicates that the encoding schemes is 16-digit.
     *
     * @syscap SystemCapability.Telephony.SmsMms
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SMS_ENCODING_16BIT,
  }
}

export default sms;
