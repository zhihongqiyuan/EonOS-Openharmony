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
 * @kit DeviceCertificateKit
 */

import type common from '@ohos.app.ability.common';

/**
 * OpenHarmony Universal CertificateManager
 *
 * @namespace certificateManagerDialog
 * @syscap SystemCapability.Security.CertificateManagerDialog
 * @stagemodelonly
 * @since 13
 */
declare namespace certificateManagerDialog {
  /**
   * Enum for result code
   *
   * @enum { number }
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 13
   */
  export enum CertificateDialogErrorCode {

    /**
     * Indicates that internal error.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 13
     */
    ERROR_GENERIC = 29700001,

    /**
     * Indicates the user cancels the operation.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 14
     */
    ERROR_OPERATION_CANCELED = 29700002,
    
    /**
     * Indicates that the user operation failed in the certificate manager dialog, such as install certificate failed.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 14
     */
    ERROR_OPERATION_FAILED = 29700003,

    /**
     * Indicates that the API is not supported on this device.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 14
     */
    ERROR_DEVICE_NOT_SUPPORTED = 29700004,

    /**
     * Indicates that the operation does not comply with the device security policy, such as the device does not allow users to manage the ca certificate of the global user..
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 18
     */
    ERROR_NOT_COMPLY_SECURITY_POLICY = 29700005,
  }


  /**
   * Enum for page type of certificate manager dialog
   *
   * @enum { number }
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 13
   */
  export enum CertificateDialogPageType {
    /**
     * Indicates the main entrance page.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 13
     */
    PAGE_MAIN = 1,

    /**
     * Indicates the CA certificate list page.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 13
     */
    PAGE_CA_CERTIFICATE = 2,
    
    /**
     * Indicates the Credential list page.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 13
     */
    PAGE_CREDENTIAL = 3,

    /**
     * Indicates the install certificate page.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 13
     */
    PAGE_INSTALL_CERTIFICATE = 4
  }

  /**
   * open certificate manager dialog and show the specified page.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context - Hap context information.
   * @param { CertificateDialogPageType } pageType - Indicates page type.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 - Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 13
   */
  function openCertificateManagerDialog(context: common.Context, pageType: CertificateDialogPageType): Promise<void>;



  /**
   * Enum for certificate type
   *
   * @enum { number }
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 14
   */
  export enum CertificateType {
    /**
     * Indicates CA certificate.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 14
     */
    CA_CERT = 1
  }

  /**
   * Enum for certificate scope
   *
   * @enum { number }
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 14
   */
  export enum CertificateScope {
    /**
     * Indicates the certificate scope is not specified and user can select the scope in the certificate install dialog.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 18
     */
    NOT_SPECIFIED = 0,

    /**
     * Indicates the certificate is for current user.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 14
     */
    CURRENT_USER = 1,

    /**
     * Indicates the certificate is for all users.
     *
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 18
     */
    GLOBAL_USER = 2,
  }

  /**
   * open install certificate dialog.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context - Hap context information
   * @param { CertificateType } certType - Indicates the type of certificate to be installed.
   * @param { CertificateScope } certScope - Indicates the user scope of certificate to be installed.
   * @param { Uint8Array } cert - Indicates the contents of the certificate file to be installed.
   * @returns { Promise<string> } The uri of installed certificate.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 - Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @throws { BusinessError } 29700002 - The user cancels the installation operation.
   * @throws { BusinessError } 29700003 - The user install certificate failed in the certificate manager dialog.
   * @throws { BusinessError } 29700004 - The API is not supported on this device.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 14
   */
  /**
   * open install certificate dialog.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context - Hap context information
   * @param { CertificateType } certType - Indicates the type of certificate to be installed.
   * @param { CertificateScope } certScope - Indicates the user scope of certificate to be installed.
   * @param { Uint8Array } cert - Indicates the contents of the certificate file to be installed.
   * @returns { Promise<string> } The uri of installed certificate.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 - Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @throws { BusinessError } 29700002 - The user cancels the installation operation.
   * @throws { BusinessError } 29700003 - The user install certificate failed in the certificate manager dialog, such as the certificate is in an invalid format.
   * @throws { BusinessError } 29700004 - The API is not supported on this device.
   * @throws { BusinessError } 29700005 - The operation does not comply with the device security policy, such as the device does not allow users to manage the ca certificate of the global user.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 18
   */
  function openInstallCertificateDialog(context: common.Context, certType: CertificateType, certScope: CertificateScope, cert: Uint8Array): Promise<string>;

  /**
   * open uninstall certificate dialog.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context - Hap context information
   * @param { CertificateType } certType - Indicates the type of certificate to be uninstalled.
   * @param { string } certUri - Indicates the certificate uri to be uninstalled.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 - Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @throws { BusinessError } 29700002 - The user cancels the uninstallation operation.
   * @throws { BusinessError } 29700003 - The user uninstall certificate failed in the certificate manager dialog, such as the certificate uri is not exist.
   * @throws { BusinessError } 29700004 - The API is not supported on this device.
   * @throws { BusinessError } 29700005 - The operation does not comply with the device security policy, such as the device does not allow users to manage the ca certificate of the global user.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 18
   */
  function openUninstallCertificateDialog(context: common.Context, certType: CertificateType, certUri: string): Promise<void>;

  /**
   * Provides the certificate dialog property.
   *
   * @typedef CertificateDialogProperty
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 18
   */
  export interface CertificateDialogProperty {
    /**
     * Indicates if the certificate dialog show install button.
     *
     * @type { boolean }
     * @syscap SystemCapability.Security.CertificateManagerDialog
     * @stagemodelonly
     * @since 18
     */
    showInstallButton: boolean;
  }

  /**
   * open certificate manager dialog and show the certificate detail page.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context - Hap context information
   * @param { Uint8Array } cert - Indicates the contents of the certificate file.
   * @param { CertificateDialogProperty } property - the certificate dialog property.
   * @returns { Promise<void> } The promise returned by the function.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
   * <br>2. Incorrect parameter types; 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 - Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @throws { BusinessError } 29700003 - Show the certificate detail dialog fail, such as the certificate is in an invalid format.
   * @throws { BusinessError } 29700004 - The API is not supported on this device.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 18
   */
  function openCertificateDetailDialog(context: common.Context, cert: Uint8Array, property: CertificateDialogProperty): Promise<void>;

  /**
   * Opens the authorization page of the certificate management dialog box. On the page displayed, the user can
   * authorize a certificate for the application.
   *
   * @permission ohos.permission.ACCESS_CERT_MANAGER
   * @param { common.Context } context Context of the HAP.
   * @returns { Promise<string> } Promise used to return the URI of the certificate authorized.
   * @throws { BusinessError } 201 - Permission verification failed. The application does not have the
   * permission required to call the API.
   * @throws { BusinessError } 401 - Invalid parameter. Possible causes: 1. A mandatory parameter is left
   * unspecified.
   * 2. Incorrect parameter type. 3. Parameter verification failed.
   * @throws { BusinessError } 29700001 Internal error. Possible causes: 1. IPC communication failed;
   * <br>2. Memory operation error; 3. File operation error.
   * @throws { BusinessError } 29700002 The user cancels the authorization.
   * @syscap SystemCapability.Security.CertificateManagerDialog
   * @stagemodelonly
   * @since 20
   */
  function openAuthorizeDialog(context: common.Context): Promise<string>;
}

export default certificateManagerDialog;
