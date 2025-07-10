/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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
 */

import { AsyncCallback } from './@ohos.base';
import { Callback } from './@ohos.base';
import Want from './@ohos.app.ability.Want';
import formInfo from './@ohos.app.form.formInfo';

/**
 * interface of formHost.
 *
 * @namespace formHost
 * @syscap SystemCapability.Ability.Form
 * @systemapi
 * @since 8
 * @deprecated since 9
 * @useinstead ohos.app.form.formHost/formHost
 */
declare namespace formHost {
  /**
   * Deletes an obtained form by its ID.
   * <p>After this method is called, the form won't be available for use by the application and the Form Manager
   * Service no longer keeps the cache information about the form.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the form ID
   * @param { AsyncCallback<void> } callback - Callback function, when the specified card is successfully deleted,
   *                                           the error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#deleteForm
   */
  function deleteForm(formId: string, callback: AsyncCallback<void>): void;

  /**
   * Deletes an obtained form by its ID.
   * <p>After this method is called, the form won't be available for use by the application and the Form Manager
   * Service no longer keeps the cache information about the form.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the form ID
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#deleteForm
   */
  function deleteForm(formId: string): Promise<void>;

  /**
   * Releases an obtained form by its ID.
   * <p>After this method is called, the form won't be available for use by the application, but the Form Manager
   * Service still keeps the cache information about the form, so that the application can quickly obtain it based on
   * the {@code formId}.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the form ID
   * @param { AsyncCallback<void> } callback - Callback function, when the specified card is successfully released,
   *                                           the error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#releaseForm
   */
  function releaseForm(formId: string, callback: AsyncCallback<void>): void;

  /**
   * Releases an obtained form by its ID.
   * <p>After this method is called, the form won't be available for use by the application, but the Form Manager
   * Service still keeps the cache information about the form, so that the application can quickly obtain it based on
   * the {@code formId}.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the form ID
   * @param { boolean } isReleaseCache - Indicates whether or not release cache
   * @param { AsyncCallback<void> } callback - Callback function, when the specified card is successfully released,
   *                                           the error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#releaseForm
   */
  function releaseForm(formId: string, isReleaseCache: boolean, callback: AsyncCallback<void>): void;

  /**
   * Releases an obtained form by its ID.
   * <p>After this method is called, the form won't be available for use by the application, but the Form Manager
   * Service still keeps the cache information about the form, so that the application can quickly obtain it based on
   * the {@code formId}.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the form ID
   * @param { boolean } [isReleaseCache] - Indicates whether or not release cache
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#releaseForm
   */
  function releaseForm(formId: string, isReleaseCache?: boolean): Promise<void>;

  /**
   * Requests for form update.
   * <p>This method must be called when the application has detected that a system setting item (such as the language,
   * resolution, or screen orientation) being listened for has changed. Upon receiving the update request, the form
   * provider automatically updates the form data (if there is any update) through the form framework, with the update
   * process being unperceivable by the application.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the ID of the form to update.
   * @param { AsyncCallback<void> } callback - Callback function, when the specified card is successfully requested,
   *                                           the error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#requestForm
   */
  function requestForm(formId: string, callback: AsyncCallback<void>): void;

  /**
   * Requests for form update.
   * <p>This method must be called when the application has detected that a system setting item (such as the language,
   * resolution, or screen orientation) being listened for has changed. Upon receiving the update request, the form
   * provider automatically updates the form data (if there is any update) through the form framework, with the update
   * process being unperceivable by the application.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the ID of the form to update.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#requestForm
   */
  function requestForm(formId: string): Promise<void>;

  /**
   * Converts a specified temporary form that has been obtained by the application into a normal form.
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the ID of the temporary form to convert.
   * @param { AsyncCallback<void> } callback - Callback function, when the specified temporary card is successfully
   *                                           converted to a regular card, the error is undefined, otherwise it is
   *                                           an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#castTempForm
   */
  function castTempForm(formId: string, callback: AsyncCallback<void>): void;

  /**
   * Converts a specified temporary form that has been obtained by the application into a normal form.
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { string } formId - Indicates the ID of the temporary form to convert.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#castTempForm
   */
  function castTempForm(formId: string): Promise<void>;

  /**
   * Sends a notification to the form framework to make the specified forms visible.
   * <p>After this method is successfully called, onVisibilityChange will be called to notify the form provider of
   * the form visibility change event.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made visible.
   * @param { AsyncCallback<void> } callback - Callback function, when the specified temporary card is successfully
   *                                           converted to a regular card, the error is undefined, otherwise it is
   *                                           an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyVisibleForms
   */
  function notifyVisibleForms(formIds: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Sends a notification to the form framework to make the specified forms visible.
   * <p>After this method is successfully called, onVisibilityChange will be called to notify the form provider of
   * the form visibility change event.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made visible.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyVisibleForms
   */
  function notifyVisibleForms(formIds: Array<string>): Promise<void>;

  /**
   * Sends a notification to the form framework to make the specified forms invisible.
   * <p>After this method is successfully called, onVisibilityChange will be called to notify the form provider of
   * the form visibility change event.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @param { AsyncCallback<void> } callback - Callback function, when sending a notification to the card framework to
   *                                           make the specified card invisible, the error is undefined, otherwise
   *                                           it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyInvisibleForms
   */
  function notifyInvisibleForms(formIds: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Sends a notification to the form framework to make the specified forms invisible.
   * <p>After this method is successfully called, onVisibilityChange will be called to notify the form provider of
   * the form visibility change event.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyInvisibleForms
   */
  function notifyInvisibleForms(formIds: Array<string>): Promise<void>;

  /**
   * Notify the form framework to make the specified forms updatable.
   * <p>You can use this method to set form refresh state to true, the form can receive new
   * update from service.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @param { AsyncCallback<void> } callback - function, when sending a notification to the card framework so that the
   *                                           specified card can be successfully updated, the error is undefined,
   *                                           otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#enableFormsUpdate
   */
  function enableFormsUpdate(formIds: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Notify the form framework to make the specified forms updatable.
   * <p>You can use this method to set form refresh state to true, the form can receive new
   * update from service.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#enableFormsUpdate
   */
  function enableFormsUpdate(formIds: Array<string>): Promise<void>;

  /**
   * Notify the form framework to make the specified forms non updatable.
   * <p>You can use this method to set form refresh state to false, the form do not receive
   * new update from service.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @param { AsyncCallback<void> } callback - Callback function, when sending a notification to the card framework to
   *                                           prevent the specified card from being successfully updated, the error
   *                                           is undefined, otherwise it is error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#disableFormsUpdate
   */
  function disableFormsUpdate(formIds: Array<string>, callback: AsyncCallback<void>): void;

  /**
   * Notify the form framework to make the specified forms non updatable.
   * <p>You can use this method to set form refresh state to false, the form do not receive
   * new update from service.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the IDs of the forms to be made invisible.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#disableFormsUpdate
   */
  function disableFormsUpdate(formIds: Array<string>): Promise<void>;

  /**
   * Checks whether the system is ready.
   * <p>You can use this method to obtain the system is ready.</p>
   *
   * @param { AsyncCallback<void> } callback - Callback function, when checking whether the system is ready for success,
   *                                           error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#isSystemReady
   */
  function isSystemReady(callback: AsyncCallback<void>): void;

  /**
   * Checks whether the system is ready.
   * <p>You can use this method to obtain the system is ready.</p>
   *
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#isSystemReady
   */
  function isSystemReady(): Promise<void>;

  /**
   * Obtains the FormInfo objects provided by all applications on the device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { AsyncCallback<Array<formInfo.FormInfo>> } callback - Callback function, when obtaining card information
   *                                                               provided by all applications on the device
   *                                                               successfully, the error is undefined, otherwise it
   *                                                               is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#getAllFormsInfo
   */
  function getAllFormsInfo(callback: AsyncCallback<Array<formInfo.FormInfo>>): void;

  /**
   * Obtains the FormInfo objects provided by all applications on the device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @returns { Promise<Array<formInfo.FormInfo>> } Returns the {@link FormInfo}
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#getAllFormsInfo
   */
  function getAllFormsInfo(): Promise<Array<formInfo.FormInfo>>;

  /**
   * Obtains the FormInfo objects provided by a specified application on the device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { AsyncCallback<Array<formInfo.FormInfo>> } callback - Callback function, when card information provided by
   *                                                               specified application on the device is successfully
   *                                                               obtained, error is undefined, otherwise it is
   *                                                               error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#getFormsInfo
   */
  function getFormsInfo(bundleName: string, callback: AsyncCallback<Array<formInfo.FormInfo>>): void;

  /**
   * Obtains the FormInfo objects provided by a specified application on the device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } moduleName - Indicates the module name of the application.
   * @param { AsyncCallback<Array<formInfo.FormInfo>> } callback - Callback function, when the card information provided
   *                                                               by the specified application on the device is
   *                                                               successfully obtained, the error is undefined,
   *                                                               otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#getFormsInfo
   */
  function getFormsInfo(
    bundleName: string,
    moduleName: string,
    callback: AsyncCallback<Array<formInfo.FormInfo>>
  ): void;

  /**
   * Obtains the FormInfo objects provided by a specified application on the device.
   *
   * @permission ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { string } bundleName - Indicates the bundle name of the application.
   * @param { string } [moduleName] - Indicates the module name of the application.
   * @returns { Promise<Array<formInfo.FormInfo>> } Returns the {@link FormInfo}
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#getFormsInfo
   */
  function getFormsInfo(bundleName: string, moduleName?: string): Promise<Array<formInfo.FormInfo>>;

  /**
   * Deletes invalid forms of the application in the Form Manager Service based on the list of.
   * <p>You can use this method to delete invalid forms of the application.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @param { AsyncCallback<number> } callback - Callback function, when deleting invalid cards from the application
   *                                             program based on the list is successful, error is undefined, and
   *                                             data is the number of deleted cards; Otherwise,
   *                                             it is the wrong object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#deleteInvalidForms
   */
  function deleteInvalidForms(formIds: Array<string>, callback: AsyncCallback<number>): void;

  /**
   * Deletes invalid forms of the application in the Form Manager Service based on the list of.
   * <p>You can use this method to delete invalid forms of the application.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @returns { Promise<number> } Returns the number of invalid forms deleted by the Form Manager Service
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#deleteInvalidForms
   */
  function deleteInvalidForms(formIds: Array<string>): Promise<number>;

  /**
   * Obtains the Form state.
   * <p>You can use this method to obtains the form state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Want } want - Indicates want of the form.
   * @param { AsyncCallback<formInfo.FormStateInfo> } callback - Callback function, when deleting invalid cards from
   *                                                             application program based on the list is successful,
   *                                                             error is undefined, and data is the number of deleted
   *                                                             cards;Otherwise, it is the wrong object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#acquireFormState
   */
  function acquireFormState(want: Want, callback: AsyncCallback<formInfo.FormStateInfo>): void;

  /**
   * Obtains the Form state.
   * <p>You can use this method to obtains the form state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM and ohos.permission.GET_BUNDLE_INFO_PRIVILEGED
   * @param { Want } want - Indicates want of the form.
   * @returns { Promise<formInfo.FormStateInfo> } Returns form state {@link FormStateInfo}
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#acquireFormState
   */
  function acquireFormState(want: Want): Promise<formInfo.FormStateInfo>;

  /**
   * Listens to the event of uninstall form.
   * <p>You can use this method to listen to the event of uninstall form.</p>
   *
   * @param { 'formUninstall' } type - Indicates event type.
   * @param { Callback<string> } callback - Callback function that returns the card identifier.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#on
   */
  function on(type: 'formUninstall', callback: Callback<string>): void;

  /**
   * Cancels listening to the event of uninstall form.
   * <p>You can use this method to cancel listening to the event of uninstall form.</p>
   *
   * @param { 'formUninstall' } type - Indicates event type.
   * @param { Callback<string> } [callback] - Callback function that returns the card identifier. By default, it
   *                                           indicates the cancellation of all registered event callbacks, which
   *                                           must be consistent with the corresponding
   *                                           callback for on('formUninstall').
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#off
   */
  function off(type: 'formUninstall', callback?: Callback<string>): void;

  /**
   * Notify form is Visible
   * <p>You can use this method to notify form visible state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @param { boolean } isVisible - Indicates whether visible.
   * @param { AsyncCallback<void> } callback - Callback function, when the notification card is visible and successful,
   *                                           the error is undefined,otherwise it is an error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyFormsVisible
   */
  function notifyFormsVisible(formIds: Array<string>, isVisible: boolean, callback: AsyncCallback<void>): void;

  /**
   * Notify form is Visible
   * <p>You can use this method to notify form visible state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @param { boolean } isVisible - Indicates whether visible.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyFormsVisible
   */
  function notifyFormsVisible(formIds: Array<string>, isVisible: boolean): Promise<void>;

  /**
   * Notify form enable update state.
   * <p>You can use this method to notify form enable update state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @param { boolean } isEnableUpdate - Indicates whether enable update.
   * @param { AsyncCallback<void> } callback - Callback function, when the notification card is successfully enabled
   *                                           for updating status, the error is undefined, otherwise is error object.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyFormsEnableUpdate
   */
  function notifyFormsEnableUpdate(
    formIds: Array<string>,
    isEnableUpdate: boolean,
    callback: AsyncCallback<void>
  ): void;

  /**
   * Notify form enable update state.
   * <p>You can use this method to notify form enable update state.</p>
   *
   * @permission ohos.permission.REQUIRE_FORM
   * @param { Array<string> } formIds - Indicates the specify form id.
   * @param { boolean } isEnableUpdate - Indicates whether enable update.
   * @returns { Promise<void> } The promise returned by the function.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.app.form.formHost/formHost#notifyFormsEnableUpdate
   */
  function notifyFormsEnableUpdate(formIds: Array<string>, isEnableUpdate: boolean): Promise<void>;
}
export default formHost;
