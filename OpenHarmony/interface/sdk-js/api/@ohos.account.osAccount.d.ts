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

import type distributedAccount from './@ohos.account.distributedAccount';
import type { AsyncCallback, Callback } from './@ohos.base';

/**
 * This module provides the capability to manage os accounts.
 *
 * @namespace osAccount
 * @syscap SystemCapability.Account.OsAccount
 * @since 7
 */
declare namespace osAccount {
  /**
   * Obtains the AccountManager instance.
   *
   * @returns { AccountManager } Returns the instance of the AccountManager.
   * @syscap SystemCapability.Account.OsAccount
   * @since 7
   */
  function getAccountManager(): AccountManager;

  /**
   * Provides abilities for you to manage and perform operations on your OS accounts.
   *
   * @interface AccountManager
   * @syscap SystemCapability.Account.OsAccount
   * @since 7
   */
  interface AccountManager {
    /**
     * Activates a specified OS account.
     * <p>
     * If multiple OS accounts are available, you can call this method to enable a specific OS account
     * to run in the foreground. Then, the OS account originally running in the foreground will be
     * switched to the background.
     * </p>
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<void> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300009 - Account has been activated.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    /**
     * Activates a specified OS account.
     * <p>
     * If multiple OS accounts are available, you can call this method to enable a specific OS account
     * to run in the foreground. Then, the OS account originally running in the foreground will be
     * switched to the background.
     * </p>
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<void> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300010 - Service busy. Possible causes: The target account is being operated.
     * @throws { BusinessError } 12300016 - The number of logged in accounts reaches the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    activateOsAccount(localId: number, callback: AsyncCallback<void>): void;

    /**
     * Activates a specified OS account.
     * <p>
     * If multiple OS accounts are available, you can call this method to enable a specific OS account
     * to run in the foreground. Then, the OS account originally running in the foreground will be
     * switched to the background.
     * </p>
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300009 - Account has been activated.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    /**
     * Activates a specified OS account.
     * <p>
     * If multiple OS accounts are available, you can call this method to enable a specific OS account
     * to run in the foreground. Then, the OS account originally running in the foreground will be
     * switched to the background.
     * </p>
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300010 - Service busy. Possible causes: The target account is being operated.
     * @throws { BusinessError } 12300016 - The number of logged in accounts reaches the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    activateOsAccount(localId: number): Promise<void>;

    /**
     * Deactivates a specified OS account.
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300010 - Service busy. Possible causes: The target account is being operated.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    deactivateOsAccount(localId: number): Promise<void>;

    /**
     * Checks whether the function of supporting multiple OS accounts is enabled.
     *
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if this function is enabled; returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkMultiOsAccountEnabled
     */
    isMultiOsAccountEnable(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the function of supporting multiple OS accounts is enabled.
     *
     * @returns { Promise<boolean> } Returns {@code true} if this function is enabled; returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkMultiOsAccountEnabled
     */
    isMultiOsAccountEnable(): Promise<boolean>;

    /**
     * Checks whether the function of supporting multiple OS accounts is enabled.
     *
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if this function is enabled; returns {@code false} otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    checkMultiOsAccountEnabled(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the function of supporting multiple OS accounts is enabled.
     *
     * @returns { Promise<boolean> } Returns {@code true} if this function is enabled; returns {@code false} otherwise.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    checkMultiOsAccountEnabled(): Promise<boolean>;

    /**
     * Checks whether an OS account is activated based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the OS account is activated.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountActivated
     */
    isOsAccountActived(localId: number, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether an OS account is activated based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } Returns {@code true} if the OS account is activated; returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountActivated
     */
    isOsAccountActived(localId: number): Promise<boolean>;

    /**
     * Checks whether an OS account is activated based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the OS account is activated.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountActivated(localId: number, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether an OS account is activated based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } - Returns {@code true} if the OS account is activated; returns {@code false} otherwise.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountActivated(localId: number): Promise<boolean>;

    /**
     * Checks whether an OS account is activated based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } - Returns {@code true} if the OS account is activated; returns {@code false} otherwise.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    isOsAccountActivated(localId: number): Promise<boolean>;

    /**
     * Checks whether a constraint has been enabled for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to check. The value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the constraint is enabled for the specified OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountConstraintEnabled
     */
    isOsAccountConstraintEnable(localId: number, constraint: string, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether a constraint has been enabled for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to check. The value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @returns { Promise<boolean> } Returns {@code true} if the constraint has been enabled for the OS account;
     *         returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountConstraintEnabled
     */
    isOsAccountConstraintEnable(localId: number, constraint: string): Promise<boolean>;

    /**
     * Checks whether the given constraint is enabled for the specified OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to check. For example: the value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the constraint is enabled for the specified OS account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or constraint.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountConstraintEnabled(localId: number, constraint: string, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the given constraint is enabled for the specified OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to check. For example: the value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @returns { Promise<boolean> } Returns whether the given constraint is enabled for the specified OS account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or constraint.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountConstraintEnabled(localId: number, constraint: string): Promise<boolean>;

    /**
     * Checks whether the given constraint is enabled for the current OS account.
     *
     * @param { string } constraint - Indicates the constraint to check. For example: the value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @returns { Promise<boolean> } Returns whether the given constraint is enabled for the current OS account.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 11
     */
    isOsAccountConstraintEnabled(constraint: string): Promise<boolean>;

    /**
     * Checks whether the given constraint is enabled for the specified OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to check. For example: the value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @returns { Promise<boolean> } Returns whether the given constraint is enabled for the specified OS account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    isOsAccountConstraintEnabled(localId: number, constraint: string): Promise<boolean>;

    /**
     * Checks whether this OS account is a test OS account.
     *
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if this OS account is a test OS account; returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountTestable
     */
    isTestOsAccount(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether this OS account is a test OS account.
     *
     * @returns { Promise<boolean> } Returns {@code true} if this OS account is a test OS account; returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountTestable
     */
    isTestOsAccount(): Promise<boolean>;

    /**
     * Checks whether current OS account is testable.
     *
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if this account is testable; returns {@code false} otherwise.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    checkOsAccountTestable(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether current OS account is testable.
     *
     * @returns { Promise<boolean> } Returns {@code true} if this account is testable; returns {@code false} otherwise.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    checkOsAccountTestable(): Promise<boolean>;

    /**
     * Checks whether an OS account has been verified based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if the OS account has been verified successfully;
     *          returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountVerified
     */
    isOsAccountVerified(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether an OS account has been verified based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if the OS account has been verified successfully;
     *          returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountVerified
     */
    isOsAccountVerified(localId: number, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether an OS account has been verified based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } Returns {@code true} if the OS account has been verified successfully;
     *          returns {@code false} otherwise.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#checkOsAccountVerified
     */
    isOsAccountVerified(localId?: number): Promise<boolean>;

    /**
     * Checks whether the current OS account is verified.
     *
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the current OS account is verified.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     * @useinstead osAccount.AccountManager#isOsAccountUnlocked
     */
    checkOsAccountVerified(callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the current OS account is verified.
     *
     * @returns { Promise<boolean> } Returns whether the current OS account is verified.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     * @useinstead osAccount.AccountManager#isOsAccountUnlocked
     */
    checkOsAccountVerified(): Promise<boolean>;

    /**
     * Checks whether the specified OS account is verified.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for checking whether the specified OS account is verified.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountVerified(localId: number, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the specified OS account is verified.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } Returns whether the specified OS account is verified.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    checkOsAccountVerified(localId: number): Promise<boolean>;

    /**
     * Checks whether the current OS account is unlocked.
     *
     * @returns { Promise<boolean> } Returns whether the current OS account is unlocked.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 11
     */
    isOsAccountUnlocked(): Promise<boolean>;

    /**
     * Checks whether the specified OS account is unlocked.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<boolean> } Returns whether the specified OS account is unlocked.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    isOsAccountUnlocked(localId: number): Promise<boolean>;

    /**
     * Removes an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<void> } callback - Indicates the callback for removing the specified OS account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300010 - Service busy. Possible causes: The target account is being operated.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    removeOsAccount(localId: number, callback: AsyncCallback<void>): void;

    /**
     * Removes an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @throws { BusinessError } 12300010 - Service busy. Possible causes: The target account is being operated.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    removeOsAccount(localId: number): Promise<void>;

    /**
     * Sets constraints for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { Array<string> } constraints - Indicates the constraints to set for the OS account. The value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @param { boolean } enable - Specifies whether to enable the constraint.
     * @param { AsyncCallback<void> } callback - Indicates the callback for setting the specified OS account constraints.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or constraints.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountConstraints(
      localId: number,
      constraints: Array<string>,
      enable: boolean,
      callback: AsyncCallback<void>
    ): void;

    /**
     * Sets constraints for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { Array<string> } constraints - Indicates the constraints to set for the OS account. The value can be:
     * <br> {@code constraint.wifi.set} - Indicates the constraint on configuring the Wi-Fi access point.
     * <br> {@code constraint.sms.use} - Indicates the constraint on sending and receiving short messages.
     * <br> {@code constraint.calls.outgoing} - Indicates the constraint on making calls.
     * <br> {@code constraint.unknown.sources.install} - Indicates the constraint on installing applications
     * <br> from unknown sources.
     * @param { boolean } enable - Specifies whether to enable the constraint.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or constraints.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountConstraints(localId: number, constraints: Array<string>, enable: boolean): Promise<void>;

    /**
     * Sets the local name for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } localName - Indicates the local name to set for the OS account.
     * @param { AsyncCallback<void> } callback - Indicates the callback for setting the specified OS account name.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or localName.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountName(localId: number, localName: string, callback: AsyncCallback<void>): void;

    /**
     * Sets the local name for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } localName - Indicates the local name to set for the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or localName.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountName(localId: number, localName: string): Promise<void>;

    /**
     * Gets the name of the OS account to which the caller belongs.
     *
     * @returns { Promise<string> } The promise returned by the function.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 12
     */
    getOsAccountName(): Promise<string>;

    /**
     * Obtains the number of all OS accounts created on a device.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<number> } callback - Returns the number of created OS accounts.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountCount
     */
    getCreatedOsAccountsCount(callback: AsyncCallback<number>): void;

    /**
     * Obtains the number of all OS accounts created on a device.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<number> } Returns the number of created OS accounts.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountCount
     */
    getCreatedOsAccountsCount(): Promise<number>;

    /**
     * Obtains the number of all OS accounts created on a device.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<number> } callback - Returns the number of created OS accounts.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * This API can be called only by system applications.
     * @since 9
     */
    getOsAccountCount(callback: AsyncCallback<number>): void;

    /**
     * Obtains the number of all OS accounts created on a device.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<number> } Returns the number of created OS accounts.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * This API can be called only by system applications.
     * @since 9
     */
    getOsAccountCount(): Promise<number>;

    /**
     * Obtains the local ID of an OS account from the current process UID.
     *
     * @param { AsyncCallback<number> } callback - Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalId
     */
    getOsAccountLocalIdFromProcess(callback: AsyncCallback<number>): void;

    /**
     * Obtains the local ID of an OS account from the current process UID.
     *
     * @returns { Promise<number> } Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalId
     */
    getOsAccountLocalIdFromProcess(): Promise<number>;

    /**
     * Gets the local ID of the current OS account.
     *
     * @param { AsyncCallback<number> } callback - Indicates the callback for getting the local ID of the current OS account.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalId(callback: AsyncCallback<number>): void;

    /**
     * Get the local ID of the current OS account.
     *
     * @returns { Promise<number> } Returns the local ID of the current account.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalId(): Promise<number>;

    /**
     * Gets the local ID of an OS account from the process UID
     *
     * @param { number } uid - Indicates the process UID.
     * @param { AsyncCallback<number> } callback - Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForUid
     */
    getOsAccountLocalIdFromUid(uid: number, callback: AsyncCallback<number>): void;

    /**
     * Gets the local ID of an OS account from the process UID
     *
     * @param { number } uid - Indicates the process UID.
     * @returns { Promise<number> } Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForUid
     */
    getOsAccountLocalIdFromUid(uid: number): Promise<number>;

    /**
     * Gets the local ID of the OS account associated with the specified UID.
     *
     * @param { number } uid - Indicates the process UID.
     * @param { AsyncCallback<number> } callback - Indicates the callback for getting the local ID of the OS account associated with the specified UID.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalIdForUid(uid: number, callback: AsyncCallback<number>): void;

    /**
     * Get the local ID of the OS account associated with the specified UID.
     *
     * @param { number } uid - Indicates the process UID.
     * @returns { Promise<number> } - Returns the local ID of the OS account associated with the specified UID.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalIdForUid(uid: number): Promise<number>;

    /**
     * Gets the local ID of the OS account associated with the specified UID synchronously.
     *
     * @param { number } uid - Indicates the process UID.
     * @returns { number } Returns the local ID of the OS account associated with the specified UID.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @since 10
     */
    getOsAccountLocalIdForUidSync(uid: number): number;

    /**
     * Queries the local ID of an OS account which is bound to the specified domain account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @param { AsyncCallback<number> } callback - Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForDomain
     */
    getOsAccountLocalIdFromDomain(domainInfo: DomainAccountInfo, callback: AsyncCallback<number>): void;

    /**
     * Queries the local ID of an OS account which is bound to the specified domain account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @returns { Promise<number> } Returns the local ID of the OS account.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForDomain
     */
    getOsAccountLocalIdFromDomain(domainInfo: DomainAccountInfo): Promise<number>;

    /**
     * Gets the local ID of the OS account associated with the specified domain account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @param { AsyncCallback<number> } callback - Indicates the callback for
     *   getting the local ID of the OS account associated with the specified domain account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid domainInfo.
     * @syscap SystemCapability.Account.OsAccount
     * This API can be called only by system applications.
     * @since 9
     */
    getOsAccountLocalIdForDomain(domainInfo: DomainAccountInfo, callback: AsyncCallback<number>): void;

    /**
     * Gets the local ID of the OS account associated with the specified domain account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @returns { Promise<number> } Returns the local ID of the OS account associated with the specified domain account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid domainInfo.
     * @syscap SystemCapability.Account.OsAccount
     * This API can be called only by system applications.
     * @since 9
     */
    getOsAccountLocalIdForDomain(domainInfo: DomainAccountInfo): Promise<number>;

    /**
     * Queries the maximum number of OS accounts that can be created on a device.
     *
     * @param { AsyncCallback<number> } callback - Returns the maximum number of OS accounts that can be created.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryMaxOsAccountNumber(callback: AsyncCallback<number>): void;

    /**
     * Queries the maximum number of OS accounts that can be created on a device.
     *
     * @returns { Promise<number> } Returns the maximum number of OS accounts that can be created.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryMaxOsAccountNumber(): Promise<number>;

    /**
     * Queries the maximum number of OS accounts that can be logged in.
     *
     * @returns { Promise<number> } Returns the maximum number of OS accounts that can be logged in.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    queryMaxLoggedInOsAccountNumber(): Promise<number>;

    /**
     * Obtains all constraints of an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<Array<string>> } callback - Returns a list of constraints.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountConstraints
     */
    getOsAccountAllConstraints(localId: number, callback: AsyncCallback<Array<string>>): void;

    /**
     * Obtains all constraints of an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<Array<string>> } Returns a list of constraints.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountConstraints
     */
    getOsAccountAllConstraints(localId: number): Promise<Array<string>>;

    /**
     * Obtains all constraints of an account based on its ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<Array<string>> } callback - Returns a list of constraints.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    getOsAccountConstraints(localId: number, callback: AsyncCallback<Array<string>>): void;

    /**
     * Obtains all constraints of an account based on its ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<Array<string>> } Returns a list of constraints.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    getOsAccountConstraints(localId: number): Promise<Array<string>>;

    /**
     * Gets all enabled constraints of the specified OS account by its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<Array<string>> } Returns a list of constraints.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    getEnabledOsAccountConstraints(localId: number): Promise<Array<string>>;

    /**
     * Queries the list of all the OS accounts that have been created in the system.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<Array<OsAccountInfo>> } callback - Returns a list of OS accounts.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryAllCreatedOsAccounts(callback: AsyncCallback<Array<OsAccountInfo>>): void;

    /**
     * Queries the list of all the OS accounts that have been created in the system.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<Array<OsAccountInfo>> } Returns a list of OS accounts.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryAllCreatedOsAccounts(): Promise<Array<OsAccountInfo>>;

    /**
     * Queries the id list of all activated OS accounts.
     *
     * @param { AsyncCallback<Array<number>> } callback - Returns a id list of OS accounts.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getActivatedOsAccountLocalIds
     */
    queryActivatedOsAccountIds(callback: AsyncCallback<Array<number>>): void;

    /**
     * Queries the id list of all activated OS accounts.
     *
     * @returns { Promise<Array<number>> } Returns a id list of OS accounts.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getActivatedOsAccountLocalIds
     */
    queryActivatedOsAccountIds(): Promise<Array<number>>;

    /**
     * Gets the local IDs of all activated OS accounts.
     *
     * @param { AsyncCallback<Array<number>> } callback - Indicates the callback for getting the local IDs of all activated OS accounts.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getActivatedOsAccountLocalIds(callback: AsyncCallback<Array<number>>): void;

    /**
     * Gets the local IDs of all activated OS accounts.
     *
     * @returns { Promise<Array<number>> } Returns all activated accounts.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getActivatedOsAccountLocalIds(): Promise<Array<number>>;

    /**
     * Gets the local ID of the foreground OS account.
     *
     * @returns { Promise<number> } Returns local ID of the foreground OS account.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 15
     */
    getForegroundOsAccountLocalId(): Promise<number>;

    /**
     * Creates an OS account using the local name and account type.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { string } localName - Indicates the local name of the OS account to create.
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localName or type.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    /**
     * Creates an OS account with the specified local name and type.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { string } localName - Indicates the local name of the OS account to create.
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localName or type.
     * @throws { BusinessError } 12300004 - Local name already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    createOsAccount(localName: string, type: OsAccountType, callback: AsyncCallback<OsAccountInfo>): void;

    /**
     * Creates an OS account using the local name and account type.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { string } localName - Indicates the local name of the OS account to create.
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @returns { Promise<OsAccountInfo> } Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localName or type.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    /**
     * Creates an OS account with the specified local name, type and options.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { string } localName - Indicates the local name of the OS account to create.
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { CreateOsAccountOptions } [options] - Indicates the options for creating an OS account.
     * @returns { Promise<OsAccountInfo> } Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localName, type or options.
     * @throws { BusinessError } 12300004 - Local name already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @throws { BusinessError } 12300015 - The short name already exists.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    createOsAccount(localName: string, type: OsAccountType, options?: CreateOsAccountOptions): Promise<OsAccountInfo>;

    /**
     * Creates an OS account using the account type and domain account info.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type or domainInfo.
     * @throws { BusinessError } 12300004 - Account already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Creates an OS account using the account type and domain account info.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type or domainInfo.
     * @throws { BusinessError } 12300004 - Account already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    createOsAccountForDomain(
      type: OsAccountType,
      domainInfo: DomainAccountInfo,
      callback: AsyncCallback<OsAccountInfo>
    ): void;

    /**
     * Creates an OS account using the account type and domain account info.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @returns { Promise<OsAccountInfo> } Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type or domainInfo.
     * @throws { BusinessError } 12300004 - Account already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Creates an OS account using the account type, domain account info and options.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { OsAccountType } type - Indicates the type of the OS account to create.
     *        {@link OsAccountType} specifies the account types available in the system.
     * @param { DomainAccountInfo } domainInfo - Indicates the domain account info.
     * @param { CreateOsAccountForDomainOptions } [options] - Indicates the options to create an OS account for domain.
     * @returns { Promise<OsAccountInfo> } Returns information about the created OS account; returns {@code null} if the creation fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type, domainInfo or options.
     * @throws { BusinessError } 12300004 - Account already exists.
     * @throws { BusinessError } 12300005 - Multi-user not supported.
     * @throws { BusinessError } 12300006 - Unsupported account type.
     * @throws { BusinessError } 12300007 - The number of accounts has reached the upper limit.
     * @throws { BusinessError } 12300015 - The short name already exists.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    createOsAccountForDomain(type: OsAccountType, domainInfo: DomainAccountInfo, options?: CreateOsAccountForDomainOptions): Promise<OsAccountInfo>;

    /**
     * Queries information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the current OS account; returns {@code null} if the query fails.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getCurrentOsAccount
     */
    queryCurrentOsAccount(callback: AsyncCallback<OsAccountInfo>): void;

    /**
     * Queries information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<OsAccountInfo> } Returns information about the current OS account; returns {@code null} if the query fails.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getCurrentOsAccount
     */
    queryCurrentOsAccount(): Promise<OsAccountInfo>;

    /**
     * Gets information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the current OS account; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    /**
     * Gets information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.GET_LOCAL_ACCOUNTS
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns information about the current OS account; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 10
     * @deprecated since 11
     */
    getCurrentOsAccount(callback: AsyncCallback<OsAccountInfo>): void;

    /**
     * Gets information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<OsAccountInfo> } Returns information about the current OS account; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     * @deprecated since 11
     */
    /**
     * Gets information about the current OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.GET_LOCAL_ACCOUNTS
     * @returns { Promise<OsAccountInfo> } Returns information about the current OS account; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 10
     * @deprecated since 11
     */
    getCurrentOsAccount(): Promise<OsAccountInfo>;

    /**
     * Queries the current OS account information.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.GET_LOCAL_ACCOUNTS
     * @returns { Promise<OsAccountInfo> } Returns information about the current OS account
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    queryOsAccount(): Promise<OsAccountInfo>;

    /**
     * Queries OS account information based on the local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<OsAccountInfo> } callback - Returns the OS account information; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryOsAccountById(localId: number, callback: AsyncCallback<OsAccountInfo>): void;

    /**
     * Queries OS account information based on the local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<OsAccountInfo> } Returns the OS account information; returns {@code null} if the query fails.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    queryOsAccountById(localId: number): Promise<OsAccountInfo>;

    /**
     * Gets the domain account information associated with the specified OS account.
     *
     * @permission ohos.permission.GET_DOMAIN_ACCOUNTS and ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the specified OS account.
     * @returns { Promise<DomainAccountInfo> } Returns the domain account information.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - OS account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 15
     */
    getOsAccountDomainInfo(localId: number): Promise<DomainAccountInfo>;

    /**
     * Obtains the type of this OS account from the current process.
     *
     * @param { AsyncCallback<OsAccountType> } callback - Returns the OS account type. The value can be {@link OsAccountType#ADMIN},
     *         {@link OsAccountType#NORMAL}, and {@link OsAccountType#GUEST}.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountType
     */
    getOsAccountTypeFromProcess(callback: AsyncCallback<OsAccountType>): void;

    /**
     * Obtains the type of this OS account from the current process.
     *
     * @returns { Promise<OsAccountType> } Returns the OS account type. The value can be {@link OsAccountType#ADMIN},
     *         {@link OsAccountType#NORMAL}, and {@link OsAccountType#GUEST}.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountType
     */
    getOsAccountTypeFromProcess(): Promise<OsAccountType>;

    /**
     * Obtains the type of this OS account from the current process.
     *
     * @param { AsyncCallback<OsAccountType> } callback - Returns the OS account type. The value can be {@link OsAccountType#ADMIN},
     *         {@link OsAccountType#NORMAL}, and {@link OsAccountType#GUEST}.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountType(callback: AsyncCallback<OsAccountType>): void;

    /**
     * Obtains the type of this OS account from the current process.
     *
     * @returns { Promise<OsAccountType> } Returns the OS account type. The value can be {@link OsAccountType#ADMIN},
     *         {@link OsAccountType#NORMAL}, and {@link OsAccountType#GUEST}.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountType(): Promise<OsAccountType>;

    /**
     * Gets the type of the specified OS account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<OsAccountType> } Returns the OS account type.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    getOsAccountType(localId: number): Promise<OsAccountType>;

    /**
     * Obtains the distributed virtual device ID (DVID).
     * <p>
     * If the same OHOS account has logged in to multiple devices, these devices constitute a super device
     * through the distributed networking. On the connected devices, you can call this method to obtain the DVIDs.
     * The same application running on different devices obtains the same DVID, whereas different applications
     * obtain different DVIDs.
     * <p>
     *
     * @permission ohos.permission.DISTRIBUTED_DATASYNC or ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<string> } callback - Returns the DVID if obtained; returns an empty string if no OHOS account has logged in.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#queryDistributedVirtualDeviceId
     */
    getDistributedVirtualDeviceId(callback: AsyncCallback<string>): void;

    /**
     * Obtains the distributed virtual device ID (DVID).
     * <p>
     * If the same OHOS account has logged in to multiple devices, these devices constitute a super device
     * through the distributed networking. On the connected devices, you can call this method to obtain the DVIDs.
     * The same application running on different devices obtains the same DVID, whereas different applications
     * obtain different DVIDs.
     * <p>
     *
     * @permission ohos.permission.DISTRIBUTED_DATASYNC or ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<string> } Returns the DVID if obtained; returns an empty string if no OHOS account has logged in.
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#queryDistributedVirtualDeviceId
     */
    getDistributedVirtualDeviceId(): Promise<string>;

    /**
     * Queries the distributed virtual device ID (DVID).
     * <p>
     * If the same OHOS account has logged in to multiple devices, these devices constitute a super device
     * through the distributed networking. On the connected devices, you can call this method to obtain the DVIDs.
     * The same application running on different devices obtains the same DVID, whereas different applications
     * obtain different DVIDs.
     * <p>
     *
     * @permission ohos.permission.DISTRIBUTED_DATASYNC or ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<string> } callback - Returns the DVID if obtained; returns an empty string if no OHOS account has logged in.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    queryDistributedVirtualDeviceId(callback: AsyncCallback<string>): void;

    /**
     * Queries the distributed virtual device ID (DVID).
     * <p>
     * If the same OHOS account has logged in to multiple devices, these devices constitute a super device
     * through the distributed networking. On the connected devices, you can call this method to obtain the DVIDs.
     * The same application running on different devices obtains the same DVID, whereas different applications
     * obtain different DVIDs.
     * <p>
     *
     * @permission ohos.permission.DISTRIBUTED_DATASYNC or ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<string> } Returns the DVID if obtained; returns an empty string if no OHOS account has logged in.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    queryDistributedVirtualDeviceId(): Promise<string>;

    /**
     * Obtains the profile photo of an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<string> } callback - Returns the profile photo if obtained;
     *         returns {@code null} if the profile photo fails to be obtained.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    getOsAccountProfilePhoto(localId: number, callback: AsyncCallback<string>): void;

    /**
     * Obtains the profile photo of an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<string> } Returns the profile photo if obtained;
     *         returns {@code null} if the profile photo fails to be obtained.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    getOsAccountProfilePhoto(localId: number): Promise<string>;

    /**
     * Sets the profile photo for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } photo - Indicates the profile photo to set for the OS account.
     * @param { AsyncCallback<void> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or photo.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountProfilePhoto(localId: number, photo: string, callback: AsyncCallback<void>): void;

    /**
     * Sets the profile photo for an OS account based on its local ID.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } photo - Indicates the profile photo to set for the OS account.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId or photo.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted Account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    setOsAccountProfilePhoto(localId: number, photo: string): Promise<void>;

    /**
     * Obtain localId according to serial number
     *
     * @param { number } serialNumber - Indicates serial number.
     * @param { AsyncCallback<number> } callback - Returns localId.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForSerialNumber
     */
    getOsAccountLocalIdBySerialNumber(serialNumber: number, callback: AsyncCallback<number>): void;

    /**
     * Obtain localId according to serial number
     *
     * @param { number } serialNumber - Indicates serial number.
     * @returns { Promise<number> } Returns localId.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getOsAccountLocalIdForSerialNumber
     */
    getOsAccountLocalIdBySerialNumber(serialNumber: number): Promise<number>;

    /**
     * Gets the local ID of the OS account associated with the serial number.
     *
     * @param { number } serialNumber - Indicates serial number.
     * @param { AsyncCallback<number> } callback - Indicates the callback for getting the local ID of the OS account associated with the serial number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid serialNumber.
     * @throws { BusinessError } 12300003 - The account indicated by serialNumber dose not exist.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalIdForSerialNumber(serialNumber: number, callback: AsyncCallback<number>): void;

    /**
     * Gets the local ID of the OS account associated with the serial number.
     *
     * @param { number } serialNumber - Indicates serial number.
     * @returns { Promise<number> } Returns the local ID of the OS account associated with the serial number.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid serialNumber.
     * @throws { BusinessError } 12300003 - The account indicated by serialNumber dose not exist.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getOsAccountLocalIdForSerialNumber(serialNumber: number): Promise<number>;

    /**
     * Obtain serial number according to localId.
     *
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<number> } callback - Returns serial number.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getSerialNumberForOsAccountLocalId
     */
    getSerialNumberByOsAccountLocalId(localId: number, callback: AsyncCallback<number>): void;

    /**
     * Obtain serial number according to localId.
     *
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<number> } Returns serial number.
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 9
     * @useinstead osAccount.AccountManager#getSerialNumberForOsAccountLocalId
     */
    getSerialNumberByOsAccountLocalId(localId: number): Promise<number>;

    /**
     * Gets the serial number for the specified os account local id.
     *
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<number> } callback - Indicates the callback for getting the serial number for the specified os account local id.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getSerialNumberForOsAccountLocalId(localId: number, callback: AsyncCallback<number>): void;

    /**
     * Gets the serial number for the specified os account local id.
     *
     * @param { number } localId - Indicates the local ID of the OS account.
     * @returns { Promise<number> } Returns the serial number according to local ID.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @since 9
     */
    getSerialNumberForOsAccountLocalId(localId: number): Promise<number>;

    /**
     * Subscribes to the change events of accounts.
     * <p>
     * When user change the account, the subscriber will receive a notification
     * about the account change event.
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { 'activate' | 'activating' } type - Event type.
     * @param { string } name - Indicates the name of subscriber.
     * @param { Callback<number> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type or name.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    on(type: 'activate' | 'activating', name: string, callback: Callback<number>): void;

    /**
     * Unsubscribes from account events.
     *
     * @permission ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS_EXTENSION
     * @param { 'activate' | 'activating' } type - Event type.
     * @param { string } name - Indicates the name of subscriber.
     * @param { Callback<number> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type or name.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 7
     */
    off(type: 'activate' | 'activating', name: string, callback?: Callback<number>): void;

    /**
     * Subscribes to the OS account switching event.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { 'switching' } type - Indicates the event type.
     * @param { Callback<OsAccountSwitchEventData> } callback - Indicates the callback for getting the event data.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    on(type: 'switching', callback: Callback<OsAccountSwitchEventData>): void;

    /**
     * Unsubscribes from the OS account switching event.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { 'switching' } type - Indicates the event type.
     * @param { Callback<OsAccountSwitchEventData> } [callback] - Indicates the callback for getting the event data.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    off(type: 'switching', callback?: Callback<OsAccountSwitchEventData>): void;

    /**
     * Subscribes to the OS account switched event.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { 'switched' } type - Indicates the event type.
     * @param { Callback<OsAccountSwitchEventData> } callback - Indicates the callback for getting the event data.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    on(type: 'switched', callback: Callback<OsAccountSwitchEventData>): void;

    /**
     * Unsubscribes from the OS account switched event.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { 'switched' } type - Indicates the event type.
     * @param { Callback<OsAccountSwitchEventData> } [callback] - Indicates the callback for getting the event data.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid type.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    off(type: 'switched', callback?: Callback<OsAccountSwitchEventData>): void;

    /**
     * Gets the bundle ID associated with the specified UID.
     *
     * @param { number } uid - Indicates the target uid.
     * @param { AsyncCallback<number> } callback - Indicates the callback for getting the bundle ID associated with the specified UID.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    getBundleIdForUid(uid: number, callback: AsyncCallback<number>): void;

    /**
     * Gets the bundle ID associated with the specified UID.
     *
     * @param { number } uid - Indicates the target uid.
     * @returns { Promise<number> } Returns the bundle ID associated with the specified UID.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    getBundleIdForUid(uid: number): Promise<number>;

    /**
     * Gets the bundle ID associated with the specified UID synchronously.
     *
     * @param { number } uid - Indicates the target uid.
     * @returns { number } Returns the bundle ID associated with the specified UID.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300002 - Invalid uid.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    getBundleIdForUidSync(uid: number): number;

    /**
     * Check whether current process belongs to the main account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { AsyncCallback<boolean> } callback - Returns {@code true} if current process belongs to the main os account;
     *         returns {@code false} otherwise.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    isMainOsAccount(callback: AsyncCallback<boolean>): void;

    /**
     * Check whether current process belongs to the main account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @returns { Promise<boolean> } Returns {@code true} if current process belongs to the main os account;
     *         returns {@code false} otherwise.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    isMainOsAccount(): Promise<boolean>;

    /**
     * Gets a list of constraint source types for the specified os account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to query the source type.
     * @param { AsyncCallback<Array<ConstraintSourceTypeInfo>> } callback - Indicates the callback for
     *   getting a list of constraint source types for the specified os account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid name or constraint.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    getOsAccountConstraintSourceTypes(
      localId: number,
      constraint: string,
      callback: AsyncCallback<Array<ConstraintSourceTypeInfo>>
    ): void;

    /**
     * Gets a list of constraint source types for the specified os account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { string } constraint - Indicates the constraint to query the source type.
     * @returns { Promise<Array<ConstraintSourceTypeInfo>> } Returns a list of constraint source types for the specified os account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid name or constraint.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    getOsAccountConstraintSourceTypes(localId: number, constraint: string): Promise<Array<ConstraintSourceTypeInfo>>;
  }

  /**
   * Provides information about OS accounts, including the local ID, local name, and type of an OS account.
   *
   * @interface OsAccountInfo
   * @syscap SystemCapability.Account.OsAccount
   * @since 7
   */
  interface OsAccountInfo {
    /**
     * The local ID of an OS account.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    localId: number;

    /**
     * The local name of an OS account.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    localName: string;

    /**
     * The short name of an OS account.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    shortName?: string;

    /**
     * Include: ADMIN, Normal, GUEST.
     *
     * @type { OsAccountType }
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    type: OsAccountType;

    /**
     * Account constraints information.
     *
     * @type { Array<string> }
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    constraints: Array<string>;

    /**
     * The account is verified or not.
     *
     * @type { boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 11
     * @useinstead osAccount.OsAccountInfo#isUnlocked
     */
    isVerified: boolean;

    /**
     * The OS account is unlocked or not.
     *
     * @type { boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @since 11
     */
    isUnlocked: boolean;

    /**
     * OS account photo.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    photo: string;

    /**
     * Os account create time.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    createTime: number;

    /**
     * The last time to log in.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    lastLoginTime: number;

    /**
     * Os account serial number.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    serialNumber: number;

    /**
     * Os account is activated or not.
     *
     * @type { boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     * @deprecated since 11
     * @useinstead osAccount.OsAccountInfo#isActivated
     */
    isActived: boolean;

    /**
     * The OS account is activated or not.
     *
     * @type { boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @since 11
     */
    isActivated: boolean;

    /**
     * Indicates whether the OS account is logged in.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    isLoggedIn?: boolean;

    /**
     * Os account create completed or not.
     *
     * @type { boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    isCreateCompleted: boolean;

    /**
     * Distributed account info.
     *
     * @type { distributedAccount.DistributedInfo }
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    distributedInfo: distributedAccount.DistributedInfo;

    /**
     * Domain account info.
     *
     * @type { DomainAccountInfo }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    domainInfo: DomainAccountInfo;
  }

  /**
   * Defines the switching/switched event data structure for the OS account.
   *
   * @typedef OsAccountSwitchEventData
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  interface OsAccountSwitchEventData {
    /**
     * Indicates which OS account to switch from.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    fromAccountId: number;

    /**
     * Indicates which OS account to switch to.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    toAccountId: number;
  }

  /**
   * Options for creating an OS account.
   *
   * @interface CreateOsAccountOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  interface CreateOsAccountOptions {
    /**
     * Indicates the short name of the OS account.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    shortName: string;

    /**
     * Indicates the bundles are disallowed to be preinstalled on the OS account.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    disallowedPreinstalledBundles?: Array<string>;

    /**
     * Indicates the bundles are allowed to be preinstalled on the OS account.
     *
     * @type { ?Array<string> }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 19
     */
    allowedPreinstalledBundles?: Array<string>;
  }

  /**
   * Options to create an OS account for domain.
   *
   * @extends CreateOsAccountOptions
   * @interface CreateOsAccountForDomainOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  interface CreateOsAccountForDomainOptions extends CreateOsAccountOptions {}

  /**
   * Provides information about domain accounts.
   *
   * @interface DomainAccountInfo
   * @syscap SystemCapability.Account.OsAccount
   * @since 8
   */
  interface DomainAccountInfo {
    /**
     * The domain name
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    domain: string;

    /**
     * The account name in the domain
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 8
     */
    accountName: string;

    /**
     * The account identifier in the domain.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    accountId?: string;

    /**
     * Indicates whether the account is authenticated.
     *
     * @type { ?boolean }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    isAuthenticated?: boolean;

    /**
     * Indicates the server config identifier for the domain to which the account belongs.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    serverConfigId?: string;
  }

  /**
   * Enumerates OS account types.
   *
   * @enum { number } OsAccountType
   * @syscap SystemCapability.Account.OsAccount
   * @since 7
   */
  enum OsAccountType {
    /**
     * Indicates the administrator account, which has the permission to manage other OS accounts.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    ADMIN = 0,

    /**
     * Indicates a normal account, which has access to common functions of OS accounts.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    NORMAL = 1,

    /**
     * Indicates a guest account, which is used to temporarily access the device and may be deleted at any time.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @since 7
     */
    GUEST = 2,

    /**
     * Indicates a private account.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PRIVATE = 1024
  }

  /**
   * Provides the abilities for user authentication.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   * @name UserAuth
   */
  class UserAuth {
    /**
     * Constructor to get the UserAuth class instance.
     *
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    constructor();

    /**
     * Gets version information.
     *
     * @returns { number } Returns the version information.
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    getVersion(): number;

    /**
     * Checks whether the authentication capability is available.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { AuthType } authType - Indicates the credential type for authentication.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @returns { number } Returns a status result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid authType or authTrustLevel.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    getAvailableStatus(authType: AuthType, authTrustLevel: AuthTrustLevel): number;

    /**
     * Gets the property based on the specified request information.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { GetPropertyRequest } request - Indicates the request information, including authentication type, and property type list.
     * @param { AsyncCallback<ExecutorProperty> } callback - Returns an executor property.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Gets the property based on the specified request information.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { GetPropertyRequest } request - Indicates the request information, including authentication type, and property type list.
     * @param { AsyncCallback<ExecutorProperty> } callback - Returns an executor property.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    getProperty(request: GetPropertyRequest, callback: AsyncCallback<ExecutorProperty>): void;

    /**
     * Gets the property based on the specified request information.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { GetPropertyRequest } request - Indicates the request information, including authentication type, and property type list.
     * @returns { Promise<ExecutorProperty> } Returns an executor property.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Gets the property based on the specified request information.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { GetPropertyRequest } request - Indicates the request information, including authentication type, and property type list.
     * @returns { Promise<ExecutorProperty> } Returns an executor property.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    getProperty(request: GetPropertyRequest): Promise<ExecutorProperty>;

    /**
     * Gets the executor property associated with the specified credential.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { Uint8Array } credentialId - Indicates the id for getting the credential information.
     * @param { Array<GetPropertyType> } keys - Indicates the array of property types to get.
     * @returns { Promise<ExecutorProperty> } Returns an executor property.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid keys.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 14
     */
    getPropertyByCredentialId(credentialId: Uint8Array, keys: Array<GetPropertyType>): Promise<ExecutorProperty>;

    /**
     * Sets property that can be used to initialize algorithms.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { SetPropertyRequest } request - Indicates the request information, including authentication type and the key-value to be set.
     * @param { AsyncCallback<void> } callback - Asynchronous callback interface.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    setProperty(request: SetPropertyRequest, callback: AsyncCallback<void>): void;

    /**
     * Sets property that can be used to initialize algorithms.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { SetPropertyRequest } request - Indicates the request information, including authentication type and the key-value to be set.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid request.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    setProperty(request: SetPropertyRequest): Promise<void>;

    /**
     * Prepares remote authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { string } remoteNetworkId - Indicates the remote network identifier.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - System service exception.
     * @throws { BusinessError } 12300002 - Invalid remoteNetworkId.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    prepareRemoteAuth(remoteNetworkId: string): Promise<void>;

    /**
     * Executes authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @param { IUserAuthCallback } callback - Indicates the callback to get result and acquireInfo.
     * @returns { Uint8Array } Returns a context ID for cancellation.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid challenge, authType or authTrustLevel.
     * @throws { BusinessError } 12300101 - The credential is incorrect.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300105 - The trust level is not supported.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Executes authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @param { IUserAuthCallback } callback - Indicates the callback to get result and acquireInfo.
     * @returns { Uint8Array } Returns a context ID for cancellation.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid challenge, authType or authTrustLevel.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - The credential is incorrect.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300105 - The trust level is not supported.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The authentication service does not exist.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300117 - PIN is expired.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    auth(
      challenge: Uint8Array,
      authType: AuthType,
      authTrustLevel: AuthTrustLevel,
      callback: IUserAuthCallback
    ): Uint8Array;

    /**
     * Executes authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @param { AuthOptions } options - Indicates authentication options.
     * @param { IUserAuthCallback } callback - Indicates the callback to get result and acquireInfo.
     * @returns { Uint8Array } Returns a context ID for cancellation.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid challenge, authType, authTrustLevel or options.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - The credential is incorrect.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300105 - The trust level is not supported.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The authentication service does not exist.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300117 - PIN is expired.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    auth(
      challenge: Uint8Array,
      authType: AuthType,
      authTrustLevel: AuthTrustLevel,
      options: AuthOptions,
      callback: IUserAuthCallback
    ): Uint8Array;

    /**
     * Executes user authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { number } userId - Indicates the user identification.
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @param { IUserAuthCallback } callback - Indicates the callback to get result and acquireInfo.
     * @returns { Uint8Array } Returns a context ID for cancellation.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid userId, challenge, authType or authTrustLevel.
     * @throws { BusinessError } 12300101 - The credential is incorrect.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300105 - The trust level is not supported.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Executes user authentication.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { number } userId - Indicates the user identification.
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AuthTrustLevel } authTrustLevel - Indicates the trust level of authentication result.
     * @param { IUserAuthCallback } callback - Indicates the callback to get result and acquireInfo.
     * @returns { Uint8Array } Returns a context ID for cancellation.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid challenge, authType or authTrustLevel.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - The credential is incorrect.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300105 - The trust level is not supported.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The authentication service does not exist.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300117 - PIN is expired.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    authUser(
      userId: number,
      challenge: Uint8Array,
      authType: AuthType,
      authTrustLevel: AuthTrustLevel,
      callback: IUserAuthCallback
    ): Uint8Array;

    /**
     * Cancels authentication with context ID.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { Uint8Array } contextID - Indicates the authentication context ID.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid contextId.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    cancelAuth(contextID: Uint8Array): void;
  }

  /**
   * Provides the abilities for Pin code authentication.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   * @name PINAuth
   */
  class PINAuth {
    /**
     * Constructor to get the PINAuth class instance.
     *
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    constructor();

    /**
     * Register inputer.
     *
     * @permission ohos.permission.ACCESS_PIN_AUTH
     * @param { IInputer } inputer - Indicates the password input box callback
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid inputer.
     * @throws { BusinessError } 12300103 - The credential inputer already exists.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    registerInputer(inputer: IInputer): void;

    /**
     * Unregister inputer.
     *
     * @permission ohos.permission.ACCESS_PIN_AUTH
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    unregisterInputer(): void;
  }

  /**
   * Provides the management of credential inputers.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 9
   * @name InputerManager
   */
  class InputerManager {
    /**
     * Register credential inputer by authentication type.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL or ohos.permission.MANAGE_USER_IDM
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { IInputer } inputer - Indicates the credential input box callback.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid authType or inputer.
     * @throws { BusinessError } 12300103 - The credential inputer already exists.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    static registerInputer(authType: AuthType, inputer: IInputer): void;

    /**
     * Unregister credential inputer by authentication type.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL or ohos.permission.MANAGE_USER_IDM
     * @param { AuthType } authType - Indicates the authentication type.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300002 - Invalid authType.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    static unregisterInputer(authType: AuthType): void;
  }

  /**
   * Provides the AuthStatusInfo type.
   *
   * @interface AuthStatusInfo
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface AuthStatusInfo {
    /**
     * Indicates the remaining times that authentication can be performed.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    remainTimes: number;

    /**
     * Indicates the freezing time before performing the next authentication.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    freezingTime: number;
  }

  /**
   * Provides the GetDomainAccessTokenOptions type.
   *
   * @typedef GetDomainAccessTokenOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface GetDomainAccessTokenOptions {
    /**
     * Indicates the domain account information.
     *
     * @type { DomainAccountInfo }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    domainAccountInfo: DomainAccountInfo;

    /**
     * Indicates the domain account token.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    domainAccountToken: Uint8Array;

    /**
     * Indicates the business parameters.
     *
     * @type { Record<string, Object> }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    businessParams: Record<string, Object>;

    /**
     * Indicates caller UID.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    callerUid: number;
  }

  /**
   * Options for getting domain account information.
   *
   * @typedef GetDomainAccountInfoOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface GetDomainAccountInfoOptions {
    /**
     * Indicates the account name.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    accountName: string;

    /**
     * Indicates the domain to which the account belongs.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    domain?: string;

    /**
     * Indicates the server config identifier for the domain to which the account belongs.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    serverConfigId?: string;
  }

  /**
   * Options for getting domain account information in the domain plugin.
   *
   * @typedef GetDomainAccountInfoPluginOptions
   * @extends GetDomainAccountInfoOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface GetDomainAccountInfoPluginOptions extends GetDomainAccountInfoOptions {
    /**
     * Indicates the caller UID.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    callerUid: number;
  }

  /**
   * Provides the definition of domain plugin.
   *
   * @interface DomainPlugin
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface DomainPlugin {
    /**
     * Authenticates the specified domain account.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information for authentication.
     * @param { Uint8Array } credential - Indicates the credential for authentication.
     * @param { IUserAuthCallback } callback - Indicates the authentication callback.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    auth(domainAccountInfo: DomainAccountInfo, credential: Uint8Array, callback: IUserAuthCallback): void;

    /**
     * Authenticates the specified domain account with a popup.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information for authentication.
     * @param { IUserAuthCallback } callback - Indicates the callback for notifying the authentication result.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    authWithPopup(domainAccountInfo: DomainAccountInfo, callback: IUserAuthCallback): void;

    /**
     * Authenticates the specified domain account with an authorization token.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information for authentication.
     * @param { Uint8Array } token - Indicates the authorization token generated when PIN or biometric authentication is successful.
     * @param { IUserAuthCallback } callback - Indicates the callback for notifying the authentication result.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    authWithToken(domainAccountInfo: DomainAccountInfo, token: Uint8Array, callback: IUserAuthCallback): void;

    /**
     * Gets the domain account information with the specified options.
     *
     * @param { GetDomainAccountInfoPluginOptions } options - Indicates the options for getting domain account information.
     * @param { AsyncCallback<DomainAccountInfo> } callback - Indicates the callback for notifying the domain account information.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    getAccountInfo(options: GetDomainAccountInfoPluginOptions, callback: AsyncCallback<DomainAccountInfo>): void;

    /**
     * Gets the domain authentication property for the specified domain account.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information for authentication.
     * @param { AsyncCallback<AuthStatusInfo> } callback - Indicates the callback for notifying the domain authentication status information.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    getAuthStatusInfo(domainAccountInfo: DomainAccountInfo, callback: AsyncCallback<AuthStatusInfo>): void;

    /**
     * Binds the specified domain account with an OS account.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { number } localId - Indicates the local ID of the OS account.
     * @param { AsyncCallback<void> } callback - Indicates the callback for notifying the binding result.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    bindAccount(domainAccountInfo: DomainAccountInfo, localId: number, callback: AsyncCallback<void>): void;

    /**
     * Unbind the specified domain account.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { AsyncCallback<void> } callback - Indicates the callback for notifying the unbinding result.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    unbindAccount(domainAccountInfo: DomainAccountInfo, callback: AsyncCallback<void>): void;

    /**
     * Checks whether the token of specified domain account is valid.
     *
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { Uint8Array } token - Indicates the account token.
     * @param { AsyncCallback<boolean> } callback - Indicates the callback for notifying the checking result.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    isAccountTokenValid(
      domainAccountInfo: DomainAccountInfo,
      token: Uint8Array,
      callback: AsyncCallback<boolean>
    ): void;

    /**
     * Gets the access token based on the specified options.
     *
     * @param { GetDomainAccessTokenOptions } options - Indicates the options for getting th access token.
     * @param { AsyncCallback<Uint8Array> } callback - Indicates the callback for returning the access token.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    getAccessToken(options: GetDomainAccessTokenOptions, callback: AsyncCallback<Uint8Array>): void;
  }

  /**
   * Provides abilities for the management of domain account.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @since 18
   */
  class DomainAccountManager {
    /**
     * Registers the domain plugin, which provides the capabilities for domain authentication.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainPlugin } plugin - Indicates the domain plugin.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300201 - The domain plugin has been registered.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    /**
     * Registers the domain plugin, which provides the capabilities for domain authentication.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainPlugin } plugin - Indicates the domain plugin.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300201 - The domain plugin has been registered.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    static registerPlugin(plugin: DomainPlugin): void;

    /**
     * Unregisters domain plugin.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    /**
     * Unregisters domain plugin.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 801 - Capability not supported.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    static unregisterPlugin(): void;

    /**
     * Authenticates the specified domain account with a credential.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { Uint8Array } credential - Indicates the credential for authentication.
     * @param { IUserAuthCallback } callback - Indicates the callback for getting the authentication result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid domainAccountInfo or credential.
     * @throws { BusinessError } 12300003 - Domain account does not exist.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - Authentication failed.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The account authentication service does not exist.
     * @throws { BusinessError } 12300114 - The account authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static auth(domainAccountInfo: DomainAccountInfo, credential: Uint8Array, callback: IUserAuthCallback): void;

    /**
     * Authenticates the domain account bound to the current OS account with a popup.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { IUserAuthCallback } callback - Indicates the callback for getting the authentication result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - No domain account is bound.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - Authentication failed.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The account authentication service does not exist.
     * @throws { BusinessError } 12300114 - The account authentication service works abnormally.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    /**
     * Authenticates the domain account bound to the current OS account with a popup.
     *
     * @param { IUserAuthCallback } callback - Indicates the callback for getting the authentication result.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - No domain account is bound.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - Authentication failed.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The account authentication service does not exist.
     * @throws { BusinessError } 12300114 - The account authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    static authWithPopup(callback: IUserAuthCallback): void;

    /**
     * Authenticates the domain account bound to the specified OS account with a popup.
     *
     * @permission ohos.permission.ACCESS_USER_AUTH_INTERNAL
     * @param { number } localId - Indicates the local ID of the specified OS account.
     * @param { IUserAuthCallback } callback - Indicates the callback for getting the authentication result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - No domain account is bound.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - Authentication failed.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The account authentication service does not exist.
     * @throws { BusinessError } 12300114 - The account authentication service works abnormally.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    /**
     * Authenticates the domain account bound to the specified OS account with a popup.
     *
     * @param { number } localId - Indicates the local ID of the specified OS account.
     * @param { IUserAuthCallback } callback - Indicates the callback for getting the authentication result.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid localId.
     * @throws { BusinessError } 12300003 - No domain account is bound.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300101 - Authentication failed.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300110 - The authentication is locked.
     * @throws { BusinessError } 12300111 - The authentication time out.
     * @throws { BusinessError } 12300112 - The authentication service is busy.
     * @throws { BusinessError } 12300113 - The account authentication service does not exist.
     * @throws { BusinessError } 12300114 - The account authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    static authWithPopup(localId: number, callback: IUserAuthCallback): void;

    /**
     * Checks whether the specified domain account exists.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { AsyncCallback<boolean> } callback Indicates - The callback for checking whether the specified domain account exists.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid domainAccountInfo.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - Not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static hasAccount(domainAccountInfo: DomainAccountInfo, callback: AsyncCallback<boolean>): void;

    /**
     * Checks whether the specified domain account exists.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @returns { Promise<boolean> } Returns whether the specified domain account exists.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid domainAccountInfo.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - Not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static hasAccount(domainAccountInfo: DomainAccountInfo): Promise<boolean>;

    /**
     * Updates the token for the specified domain account.
     * <p>Only the registered domain plugin has the permission to call this function.<br/>
     * An empty token indicates the token of the target domain account is invalid.</p>
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { Uint8Array } token - Indicates the domain account token.
     * @param { AsyncCallback<void> } callback - Indicates the result callback.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid token.
     * @throws { BusinessError } 12300003 - Account not found.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static updateAccountToken(
      domainAccountInfo: DomainAccountInfo,
      token: Uint8Array,
      callback: AsyncCallback<void>
    ): void;

    /**
     * Updates the token for the specified domain account.
     * <p>Only the registered domain plugin has the permission to call this function.<br/>
     * An empty token indicates the token of the target domain account is invalid.</p>
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @param { Uint8Array } token - Indicates the domain account token.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid token.
     * @throws { BusinessError } 12300003 - Account not found.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static updateAccountToken(domainAccountInfo: DomainAccountInfo, token: Uint8Array): Promise<void>;

    /**
     * Updates the information of the specified domain account.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.MANAGE_DOMAIN_ACCOUNTS
     * @param { DomainAccountInfo } oldAccountInfo - Indicates the old domain account information.
     * @param { DomainAccountInfo } newAccountInfo - Indicates the new domain account information.
     * @returns { Promise<void> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - The new account info is invalid.
     * @throws { BusinessError } 12300003 - The old account not found.
     * @throws { BusinessError } 12300004 - The new account already exists.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static updateAccountInfo(oldAccountInfo: DomainAccountInfo, newAccountInfo: DomainAccountInfo): Promise<void>;

    /**
     * Gets the specified domain account information.
     *
     * @permission ohos.permission.GET_DOMAIN_ACCOUNTS
     * @param { GetDomainAccountInfoOptions } options - Indicates the options for getting domain account information.
     * @param { AsyncCallback<DomainAccountInfo> } callback - Indicates the result callback.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - Not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static getAccountInfo(options: GetDomainAccountInfoOptions, callback: AsyncCallback<DomainAccountInfo>): void;

    /**
     * Gets the specified domain account information.
     *
     * @permission ohos.permission.GET_DOMAIN_ACCOUNTS
     * @param { GetDomainAccountInfoOptions } options - Indicates the options for getting domain account information.
     * @returns { Promise<DomainAccountInfo> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - Not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    static getAccountInfo(options: GetDomainAccountInfoOptions): Promise<DomainAccountInfo>;

    /**
     * Gets the business access token of the current domain account.
     *
     * @param { Record<string, Object> } businessParams - Indicates the business parameters.
     * @param { AsyncCallback<Uint8Array> } callback - Indicates the result callback.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid business parameters.
     * @throws { BusinessError } 12300003 - Domain account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - The domain account is not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    static getAccessToken(businessParams: Record<string, Object>, callback: AsyncCallback<Uint8Array>): void;

    /**
     * Gets the business access token for the current domain account.
     *
     * @param { Record<string, Object> } businessParams - Indicates the business parameters.
     * @returns { Promise<Uint8Array> } The promise returned by the function.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid business parameters.
     * @throws { BusinessError } 12300003 - Domain account not found.
     * @throws { BusinessError } 12300013 - Network exception.
     * @throws { BusinessError } 12300014 - The domain account is not authenticated.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300114 - The authentication service works abnormally.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 11
     */
    static getAccessToken(businessParams: Record<string, Object>): Promise<Uint8Array>;

    /**
     * Checks whether the authentication of the target domain account is expired.
     *
     * @permission ohos.permission.MANAGE_LOCAL_ACCOUNTS or ohos.permission.INTERACT_ACROSS_LOCAL_ACCOUNTS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @returns { Promise<boolean> } The promise returned by the function.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Domain account not found.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    static isAuthenticationExpired(domainAccountInfo: DomainAccountInfo): Promise<boolean>;
  }

  /**
   * Defines the domain server config.
   *
   * @typedef DomainServerConfig
   * @syscap SystemCapability.Account.OsAccount
   * @since 18
   */
  interface DomainServerConfig {
    /**
     * Indicates the detail config parameters.
     *
     * @type { Record<string, Object> }
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    parameters: Record<string, Object>;

    /**
     * Indicates the config identifier.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    id: string;

    /**
     * Indicates the domain to which the server config belongs.
     *
     * @type { string }
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    domain: string;
  }

  /**
   * Provides abilities for managing domain server config.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @since 18
   */
  class DomainServerConfigManager {
    /**
     * Adds a domain server config.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @param { Record<string, Object> } parameters - Indicates the server config parameters.
     * @returns { Promise<DomainServerConfig> } Returns the added domain server config.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid server config parameters.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @throws { BusinessError } 12300213 - Server config already exists.
     * @throws { BusinessError } 12300215 - The number of server config reaches the upper limit.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static addServerConfig(parameters: Record<string, Object>): Promise<DomainServerConfig>;

    /**
     * Removes a domain server config.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @param { string } configId - Indicates the server config identifier.
     * @returns { Promise<void> } Returns void.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300212 - Server config not found.
     * @throws { BusinessError } 12300214 - Server config has been associated with an account.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static removeServerConfig(configId: string): Promise<void>;

    /**
     * Updates the target server config with the specified parameters.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @param { string } configId - Indicates the server config identifier.
     * @param { Record<string, Object> } parameters - Indicates the server config parameters.
     * @returns { Promise<DomainServerConfig> } Returns the updated domain server config.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid server config parameters.
     * @throws { BusinessError } 12300211 - Server unreachable.
     * @throws { BusinessError } 12300212 - Server config not found.
     * @throws { BusinessError } 12300213 - Server config already exists.
     * @throws { BusinessError } 12300214 - Server config has been associated with an account.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static updateServerConfig(configId: string, parameters: Record<string, Object>): Promise<DomainServerConfig>;

    /**
     * Gets the specified server config by identifier.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @param { string } configId - Indicates the server config identifier.
     * @returns { Promise<DomainServerConfig> } Returns the server config.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300212 - Server config not found.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static getServerConfig(configId: string): Promise<DomainServerConfig>;

    /**
     * Gets all server configs.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @returns { Promise<Array<DomainServerConfig>> } Returns a list of server configs.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static getAllServerConfigs(): Promise<Array<DomainServerConfig>>;

    /**
     * Gets the server config of the specified domain account.
     *
     * @permission ohos.permission.MANAGE_DOMAIN_ACCOUNT_SERVER_CONFIGS
     * @param { DomainAccountInfo } domainAccountInfo - Indicates the domain account information.
     * @returns { Promise<DomainServerConfig> } Returns the domain server config.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 801 - Capability not supported.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Domain account not found.
     * @static
     * @syscap SystemCapability.Account.OsAccount
     * @since 18
     */
    static getAccountServerConfig(domainAccountInfo: DomainAccountInfo): Promise<DomainServerConfig>;
  }

  /**
   * Provides the abilities for managing user identity.
   *
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   * @name UserIdentityManager
   */
  class UserIdentityManager {
    /**
     * Constructor to get the UserIdentityManager class instance.
     *
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    constructor();

    /**
     * Opens session.
     * <p>
     * Start an IDM operation to obtain challenge value.
     * A challenge value of 0 indicates that opensession failed.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { AsyncCallback<Uint8Array> } callback - Returns a challenge value.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    openSession(callback: AsyncCallback<Uint8Array>): void;

    /**
     * Opens session.
     * <p>
     * Start an IDM operation to obtain challenge value.
     * A challenge value of 0 indicates that opensession failed.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @returns { Promise<Uint8Array> } Returns a challenge value.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Opens a session.
     * <p>
     * Start an IDM operation to obtain challenge value.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { number } [accountId] - Indicates the local ID of the OS account.
     * @returns { Promise<Uint8Array> } Returns a challenge value.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    openSession(accountId?: number): Promise<Uint8Array>;

    /**
     * Adds credential.
     * <p>
     * Add user credential information, pass in credential addition method and credential information
     * (credential type, subclass, if adding user's non password credentials, pass in password authentication token),
     * and get the result / acquireInfo callback.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { CredentialInfo } credentialInfo - Indicates the credential information.
     * @param { IIdmCallback } callback - Indicates the callback to get results and acquireInfo.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid credentialInfo, i.e. authType or authSubType.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300115 - The number of credentials reaches the upper limit.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Adds a credential.
     * <p>
     * Add user credential information, pass in credential addition method and credential information
     * (credential type, subclass, if adding user's non password credentials, pass in password authentication token),
     * and get the result / acquireInfo callback.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { CredentialInfo } credentialInfo - Indicates the credential information.
     * @param { IIdmCallback } callback - Indicates the callback to get results and acquireInfo.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid credentialInfo, i.e. authType or authSubType.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300008 - Restricted account.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300115 - The number of credentials reaches the upper limit.
     * @throws { BusinessError } 12300116 - Credential complexity verification failed.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    addCredential(credentialInfo: CredentialInfo, callback: IIdmCallback): void;

    /**
     * Updates credential.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { CredentialInfo } credentialInfo - Indicates the credential information.
     * @param { IIdmCallback } callback - Indicates the callback to get results and acquireInfo.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid credentialInfo, i.e. authType or authSubType.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Updates a credential.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { CredentialInfo } credentialInfo - Indicates the credential information.
     * @param { IIdmCallback } callback - Indicates the callback to get results and acquireInfo.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid credentialInfo, i.e. authType or authSubType.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @throws { BusinessError } 12300109 - The authentication, enrollment, or update operation is canceled.
     * @throws { BusinessError } 12300111 - The operation time out.
     * @throws { BusinessError } 12300116 - Credential complexity verification failed.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    updateCredential(credentialInfo: CredentialInfo, callback: IIdmCallback): void;

    /**
     * Closes session.
     * <p>
     * End an IDM operation.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    /**
     * Closes a session.
     * <p>
     * End an IDM operation.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { number } [accountId] - Indicates the local ID of the OS account.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300008 - Restricted account.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    closeSession(accountId?: number): void;

    /**
     * Cancels entry with a challenge value.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { Uint8Array } challenge - Indicates the challenge value.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid challenge.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    cancel(challenge: Uint8Array): void;

    /**
     * Deletes the user with the authentication token.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { Uint8Array } token - Indicates the authentication token.
     * @param { IIdmCallback } callback - Indicates the callback to get the deletion result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    delUser(token: Uint8Array, callback: IIdmCallback): void;

    /**
     * Deletes the user credential information.
     *
     * @permission ohos.permission.MANAGE_USER_IDM
     * @param { Uint8Array } credentialId - Indicates the credential index.
     * @param { Uint8Array } token - Indicates the authentication token.
     * @param { IIdmCallback } callback - Indicates the callback to get the deletion result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid credentialId.
     * @throws { BusinessError } 12300101 - The token is invalid.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    delCred(credentialId: Uint8Array, token: Uint8Array, callback: IIdmCallback): void;

    /**
     * Gets authentication information.
     *
     * @permission ohos.permission.USE_USER_IDM
     * @param { AsyncCallback<Array<EnrolledCredInfo>> } callback - Indicates the callback to get all registered credential information of
     * the specified type for the current user.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    getAuthInfo(callback: AsyncCallback<Array<EnrolledCredInfo>>): void;

    /**
     * Gets authentication information.
     *
     * @permission ohos.permission.USE_USER_IDM
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { AsyncCallback<Array<EnrolledCredInfo>> } callback - Indicates the callback to get all registered credential information of
     * the specified type for the current user.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid authType.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    getAuthInfo(authType: AuthType, callback: AsyncCallback<Array<EnrolledCredInfo>>): void;

    /**
     * Gets authentication information.
     *
     * @permission ohos.permission.USE_USER_IDM
     * @param { AuthType } authType - Indicates the authentication type.
     * @returns { Promise<Array<EnrolledCredInfo>> } Returns all registered credential information of
     * the specified type for the current user.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid authType.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    getAuthInfo(authType?: AuthType): Promise<Array<EnrolledCredInfo>>;

    /**
     * Gets authentication information.
     *
     * @permission ohos.permission.USE_USER_IDM
     * @param { GetAuthInfoOptions } [options] - Indicates the options for getting the authentication information.
     * @returns { Promise<Array<EnrolledCredInfo>> } Returns all enrolled credential information
     * according to the options.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid options.
     * @throws { BusinessError } 12300003 - Account not found.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    getAuthInfo(options?: GetAuthInfoOptions): Promise<Array<EnrolledCredInfo>>;

    /**
     * Gets the credential enrolled identifier of the specified authentication type.
     *
     * @permission ohos.permission.USE_USER_IDM
     * @param { AuthType } authType - Indicates the authentication type.
     * @param { number } [accountId] - Indicates the OS account identifier.
     * @returns { Promise<Uint8Array> } Returns the enrolled identifier.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300001 - The system service works abnormally.
     * @throws { BusinessError } 12300002 - Invalid authType.
     * @throws { BusinessError } 12300003 - Account not found.
     * @throws { BusinessError } 12300102 - The credential does not exist.
     * @throws { BusinessError } 12300106 - The authentication type is not supported.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    getEnrolledId(authType: AuthType, accountId?: number): Promise<Uint8Array>;
  }

  /**
   * Options for getting authentication information.
   *
   * @typedef GetAuthInfoOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi
   * @since 12
   */
  interface GetAuthInfoOptions {
    /**
     * Indicates the authentication credential type.
     *
     * @type { ?AuthType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    authType?: AuthType;

    /**
     * Indicates the OS account identifier.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    accountId?: number;
  }

  /**
   * Indicates the enumeration of the authentication intent.
   *
   * @enum { number } AuthIntent
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi
   * @since 12
   */
  enum AuthIntent {
    /**
     * Indicates the intent to unlock screen.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    UNLOCK = 1,

    /**
     * Indicates the intent of slient authentication.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 14
     */
    SILENT_AUTH = 2,

    /**
     * Indicates the intent of question authentication.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 14
     */
    QUESTION_AUTH = 3
  }

  /**
   * Options for remote authentication.
   *
   * @interface RemoteAuthOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi
   * @since 12
   */
  interface RemoteAuthOptions {
    /**
     * Indicates the verifier network identifier.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    verifierNetworkId?: string;

    /**
     * Indicates the collector network identifier.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    collectorNetworkId?: string;

    /**
     * Indicates the collector token identifier.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    collectorTokenId?: number;
  }

  /**
   * Options for authentication.
   *
   * @interface AuthOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi
   * @since 12
   */
  interface AuthOptions {
    /**
     * Indicates the local ID of the OS account to be authenticated.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    accountId?: number;

    /**
     * Indicates the authentication intent.
     *
     * @type { ?AuthIntent }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    authIntent?: AuthIntent;

    /**
     * Indicates the remote authentication options.
     *
     * @type { ?RemoteAuthOptions }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi
     * @since 12
     */
    remoteAuthOptions?: RemoteAuthOptions;
  }

  /**
   * Password data callback.
   *
   * @interface IInputData
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface IInputData {
    /**
     * Notifies to set data.
     *
     * @type { function }
     * @throws { BusinessError } 202 - Not system application.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified.
     * <br> 2. Incorrect parameter types.
     * @throws { BusinessError } 12300002 - Invalid pinSubType.
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onSetData: (authSubType: AuthSubType, data: Uint8Array) => void;
  }

  /**
   * Options for getting input data.
   *
   * @interface GetInputDataOptions
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 12
   */
  interface GetInputDataOptions {
    /**
     * Indicates the challenge.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    challenge?: Uint8Array;
  }

  /**
   * Password input box callback.
   *
   * @interface IInputer
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface IInputer {
    /**
     * Notifies to get data.
     *
     * @type { function }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onGetData: (authSubType: AuthSubType, callback: IInputData, options: GetInputDataOptions) => void;
  }

  /**
   * User authentication callback.
   *
   * @interface IUserAuthCallback
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface IUserAuthCallback {
    /**
     * The authentication result code is returned through the callback.
     * If the authentication is passed, the authentication token is returned in extraInfo,
     * If the authentication fails, the remaining authentication times are returned in extraInfo,
     * If the authentication executor is locked, the freezing time is returned in extraInfo.
     *
     * @type { function }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onResult: (result: number, extraInfo: AuthResult) => void;

    /**
     * During an authentication, the TipsCode is returned through the callback.
     *
     * @type { ?function }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onAcquireInfo?: (module: number, acquire: number, extraInfo: Uint8Array) => void;
  }

  /**
   * Identity manager callback.
   *
   * @interface IIdmCallback
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface IIdmCallback {
    /**
     * The authentication result code is returned through the callback.
     *
     * @type { function }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onResult: (result: number, extraInfo: RequestResult) => void;

    /**
     * During an authentication, the TipsCode is returned through the callback.
     *
     * @type { ?function }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    onAcquireInfo?: (module: number, acquire: number, extraInfo: Uint8Array) => void;
  }

  /**
   * Provides the information of the get property request.
   *
   * @interface GetPropertyRequest
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface GetPropertyRequest {
    /**
     * Indicates the authentication credential type.
     *
     * @type { AuthType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    authType: AuthType;

    /**
     * Indicates the array of property types to get.
     *
     * @type { Array<GetPropertyType> }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    keys: Array<GetPropertyType>;

    /**
     * Indicates the OS account identifier.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    accountId?: number;
  }

  /**
   * Provides the information of the set property request.
   *
   * @interface SetPropertyRequest
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface SetPropertyRequest {
    /**
     * Indicates the authentication credential type.
     *
     * @type { AuthType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    authType: AuthType;

    /**
     * Indicates the property type to set.
     *
     * @type { SetPropertyType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    key: SetPropertyType;

    /**
     * Indicates the information to set.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    setInfo: Uint8Array;
  }

  /**
   * Provides the property of executor.
   *
   * @interface ExecutorProperty
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface ExecutorProperty {
    /**
     * Indicates the result.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    result: number;

    /**
     * Indicates the authentication credential subtype.
     *
     * @type { AuthSubType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    authSubType: AuthSubType;

    /**
     * Indicates the remaining times.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    remainTimes?: number;

    /**
     * Indicates the freezing times.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    freezingTime?: number;

    /**
     * Indicates next phase freezing time.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    nextPhaseFreezingTime?: number;

    /**
     * Indicates the enrollment progress.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    enrollmentProgress?: string;

    /**
     * Indicates the sensor information.
     *
     * @type { ?string }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    sensorInfo?: string;
  }

  /**
   * Indicates the information of authentication result.
   *
   * @interface AuthResult
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface AuthResult {
    /**
     * Indicates the authentication token.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    token?: Uint8Array;

    /**
     * Indicates the remaining times.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    remainTimes?: number;

    /**
     * Indicates the freezing times.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    freezingTime?: number;

    /**
     * Indicates next phase freezing time.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    nextPhaseFreezingTime?: number;

    /**
     * Indicates the credential ID.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    credentialId?: Uint8Array;

    /**
     * Indicates the local ID of the authenticated OS account.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    accountId?: number;

    /**
     * Indicates the validity period after which the PIN will expire.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    pinValidityPeriod?: number;
  }

  /**
   * Indicates the information of credential.
   *
   * @interface CredentialInfo
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface CredentialInfo {
    /**
     * Indicates the credential type.
     *
     * @type { AuthType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    credType: AuthType;

    /**
     * Indicates the credential subtype.
     *
     * @type { AuthSubType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    credSubType: AuthSubType;

    /**
     * Indicates the authentication token.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    token: Uint8Array;

    /**
     * Indicates the local ID of the OS account to which the credential belongs.
     *
     * @type { ?number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    accountId?: number;
  }

  /**
   * Indicates the information of request result.
   *
   * @interface RequestResult
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface RequestResult {
    /**
     * Indicates the credential index.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    credentialId?: Uint8Array;
  }

  /**
   * Indicates the information of enrolled credential.
   *
   * @interface EnrolledCredInfo
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  interface EnrolledCredInfo {
    /**
     * Indicates the credential index.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    credentialId: Uint8Array;

    /**
     * Indicates the authentication credential type.
     *
     * @type { AuthType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    authType: AuthType;

    /**
     * Indicates the authentication credential subtype.
     *
     * @type { AuthSubType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    authSubType: AuthSubType;

    /**
     * Indicates the credential template ID.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    templateId: Uint8Array;
  }

  /**
   * Indicates the property type to get.
   *
   * @enum { number } GetPropertyType
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum GetPropertyType {
    /**
     * Indicates the authentication subtype.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    AUTH_SUB_TYPE = 1,

    /**
     * Indicates the remain times.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    REMAIN_TIMES = 2,

    /**
     * Indicates the freezing time.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FREEZING_TIME = 3,

    /**
     * Indicates the enrollment progress.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    ENROLLMENT_PROGRESS = 4,

    /**
     * Indicates the sensor information.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    SENSOR_INFO = 5,

    /**
     * Indicates the next phase freezing time.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    NEXT_PHASE_FREEZING_TIME = 6
  }

  /**
   * Indicates the property type to set.
   *
   * @enum { number } SetPropertyType
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum SetPropertyType {
    /**
     * Indicates the init algorithm.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    INIT_ALGORITHM = 1
  }

  /**
   * Indicates the credential type for authentication.
   *
   * @enum { number } AuthType
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum AuthType {
    /**
     * Indicates the PIN authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    PIN = 1,

    /**
     * Indicates the FACE authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE = 2,

    /**
     * Indicates the FINGERPRINT authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT = 4,

    /**
     * Indicates the RECOVERY_KEY authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    RECOVERY_KEY = 8,

    /**
     * Indicates the PRIVATE_PIN authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 14
     */
    PRIVATE_PIN = 16,

    /**
     * Indicates the DOMAIN authentication type.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    DOMAIN = 1024
  }

  /**
   * Indicates the credential subtype for authentication.
   *
   * @enum { number } AuthSubType
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum AuthSubType {
    /**
     * Indicates the 6-digit credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    PIN_SIX = 10000,

    /**
     * Indicates the self-defined digital credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    PIN_NUMBER = 10001,

    /**
     * Indicates the self-defined mixed credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    PIN_MIXED = 10002,

    /**
     * Indicates the 4-digit credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PIN_FOUR = 10003,

    /**
     * Indicates the pattern credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 12
     */
    PIN_PATTERN = 10004,

    /**
     * Indicates the question credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 14
     */
    PIN_QUESTION = 10005,

    /**
     * Indicates the 2D face credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_2D = 20000,

    /**
     * Indicates the 3D face credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_3D = 20001,

    /**
     * Indicates the capacitive fingerprint credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT_CAPACITIVE = 30000,

    /**
     * Indicates the optical fingerprint credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT_OPTICAL = 30001,

    /**
     * Indicates the ultrasonic fingerprint credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT_ULTRASONIC = 30002,

    /**
     * Indicates the mixed domain credential.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    DOMAIN_MIXED = 10240001
  }

  /**
   * Indicates the trusted level of authentication results.
   *
   * @enum { number } AuthTrustLevel
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum AuthTrustLevel {
    /**
     * Indicates the trusted level 1.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ATL1 = 10000,

    /**
     * Indicates the trusted level 2.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ATL2 = 20000,

    /**
     * Indicates the trusted level 3.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ATL3 = 30000,

    /**
     * Indicates the trusted level 4.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    ATL4 = 40000
  }

  /**
   * Indicates the module of acquired information.
   *
   * @enum { number } Module
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum Module {
    /**
     * Indicates the information acquired from FaceAuth.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH = 1
  }

  /**
   * Indicates the enumeration of authentication result code.
   *
   * @enum { number } ResultCode
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum ResultCode {
    /**
     * Indicates that authentication is success or ability is supported.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    SUCCESS = 0,

    /**
     * Indicates the authenticator fails to identify user.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FAIL = 1,

    /**
     * Indicates other errors.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    GENERAL_ERROR = 2,

    /**
     * Indicates that authentication has been canceled.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    CANCELED = 3,

    /**
     * Indicates that authentication has timed out.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TIMEOUT = 4,

    /**
     * Indicates that this authentication type is not supported.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TYPE_NOT_SUPPORT = 5,

    /**
     * Indicates that the authentication trust level is not supported.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    TRUST_LEVEL_NOT_SUPPORT = 6,

    /**
     * Indicates that the authentication task is busy. Wait for a few seconds and try again.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    BUSY = 7,

    /**
     * Indicates incorrect parameters.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    INVALID_PARAMETERS = 8,

    /**
     * Indicates that the authenticator is locked.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    LOCKED = 9,

    /**
     * Indicates that the user has not enrolled the authenticator.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    NOT_ENROLLED = 10
  }

  /**
   * Indicates the enumeration of prompt codes in the process of face authentication.
   *
   * @enum { number } FaceTipsCode
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum FaceTipsCode {
    /**
     * Indicates that the obtained facial image is too bright due to high illumination.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_BRIGHT = 1,

    /**
     * Indicates that the obtained facial image is too dark due to low illumination.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_DARK = 2,

    /**
     * Indicates that the face is too close to the device.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_CLOSE = 3,

    /**
     * Indicates that the face is too far away from the device.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_FAR = 4,

    /**
     * Indicates that the device is too high, and that only the upper part of the face is captured.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_HIGH = 5,

    /**
     * Indicates that the device is too low, and that only the lower part of the face is captured.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_LOW = 6,

    /**
     * Indicates that the device is deviated to the right, and that only the right part of the face is captured.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_RIGHT = 7,

    /**
     * Indicates that the device is deviated to the left, and that only the left part of the face is captured.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_LEFT = 8,

    /**
     * Indicates that the face moves too fast during facial information collection.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_TOO_MUCH_MOTION = 9,

    /**
     * Indicates that the face is not facing the device.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_POOR_GAZE = 10,

    /**
     * Indicates that no face is detected.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FACE_AUTH_TIP_NOT_DETECTED = 11
  }

  /**
   * Indicates the enumeration of prompt codes in the process of fingerprint authentication.
   *
   * @enum { number } FingerprintTips
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 8
   */
  enum FingerprintTips {
    /**
     * Indicates that the image acquired is good.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_GOOD = 0,

    /**
     * Indicates that the fingerprint image is too noisy due to suspected or detected dirt on the sensor.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_IMAGER_DIRTY = 1,

    /**
     * Indicates that the fingerprint image is too noisy to process due to a detected condition.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_INSUFFICIENT = 2,

    /**
     * Indicates that only a partial fingerprint image is detected.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_PARTIAL = 3,

    /**
     * Indicates that the fingerprint image is incomplete due to quick motion.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_TOO_FAST = 4,

    /**
     * Indicates that the fingerprint image is unreadable due to lack of motion.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 8
     */
    FINGERPRINT_TIP_TOO_SLOW = 5,

    /**
     * Indicates that the finger is down.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT_TIP_FINGER_DOWN = 6,

    /**
     * Indicates that the finger is up.
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FINGERPRINT_TIP_FINGER_UP = 7
  }

  /**
   * Enumerates for constraint source types.
   *
   * @enum { number } ConstraintSourceType
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  enum ConstraintSourceType {
    /**
     * No constraints are set
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    CONSTRAINT_NOT_EXIST = 0,

    /**
     * Constraint is set by setOsAccountConstraints
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    CONSTRAINT_TYPE_BASE = 1,

    /**
     * Constraint is set by device owner
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    CONSTRAINT_TYPE_DEVICE_OWNER = 2,

    /**
     * Constraint is set by profile owner
     *
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    CONSTRAINT_TYPE_PROFILE_OWNER = 3
  }

  /**
   * Provides information about the constraint source type info of an os account.
   *
   * @interface ConstraintSourceTypeInfo
   * @syscap SystemCapability.Account.OsAccount
   * @systemapi Hide this for inner system use.
   * @since 9
   */
  interface ConstraintSourceTypeInfo {
    /**
     * Indicates the id of an os account who set the constraint.
     * When type is CONSTRAINT_NOT_EXIST or CONSTRAINT_TYPE_BASE, localId will be -1.
     *
     * @type { number }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    localId: number;

    /**
     * Indicates the source type of the constraint.
     *
     * @type { ConstraintSourceType }
     * @syscap SystemCapability.Account.OsAccount
     * @systemapi Hide this for inner system use.
     * @since 9
     */
    type: ConstraintSourceType;
  }
}

export default osAccount;