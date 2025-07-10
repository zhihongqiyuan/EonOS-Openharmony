/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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
 * @kit AbilityKit
 */

import { AsyncCallback } from './@ohos.base';

/**
 * Support install, upgrade, remove and recover bundles on the devices.
 *
 * @namespace installer
 * @syscap SystemCapability.BundleManager.BundleFramework.Core
 * @systemapi
 * @since 9
 */
declare namespace installer {
  /**
   * Obtains the interface used to install bundle.
   *
   * @param { AsyncCallback<BundleInstaller> } callback - The callback of BundleInstaller object.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Incorrect parameter types.
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  function getBundleInstaller(callback: AsyncCallback<BundleInstaller>): void;

  /**
   * Obtains the interface used to install bundle.
   *
   * @returns { Promise<BundleInstaller> } BundleInstaller object.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  function getBundleInstaller(): Promise<BundleInstaller>;

  /**
   * Obtains the interface used to install bundle.
   *
   * @returns { BundleInstaller } BundleInstaller object.
   * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 10
   */
  function getBundleInstallerSync(): BundleInstaller;

  /**
   * Bundle installer interface, include install uninstall recover.
   *
   * @interface BundleInstaller
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  interface BundleInstaller {
    /**
     * Install haps for an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the hap of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing haps result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
     /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
	/**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 17
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700076 - Failed to install the HAP or HSP because the app distribution type is not allowed.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 18
     */
    install(hapFilePaths: Array<string>, installParam: InstallParam, callback: AsyncCallback<void>): void;

    /**
     * Install haps for an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the hap of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing haps result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
     /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
	/**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 17
     */
    /**
     * Install HAPs for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - The callback of installing HAPs result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700076 - Failed to install the HAP or HSP because the app distribution type is not allowed.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 18
     */
    install(hapFilePaths: Array<string>, callback: AsyncCallback<void>): void;

    /**
     * Install haps for an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the hap of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
    /**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } [installParam] - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } [installParam] - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
	/**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } [installParam] - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 17
     */
    /**
     * Install haps for an application.
     * To install a non-enterprise application, you must have the permission ohos.permission.INSTALL_BUNDLE.
     * To install an enterprise application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_BUNDLE.
     * To install an enterprise administrator application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE.
     * To install an enterprise normal application, you must have the permission ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE.
     * To install an internal beta application, you must have the permission ohos.permission.INSTALL_INTERNALTESTING_BUNDLE.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE or ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } [installParam] - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_BUNDLE' or
     * 'ohos.permission.INSTALL_ENTERPRISE_MDM_BUNDLE' or 'ohos.permission.INSTALL_ENTERPRISE_NORMAL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature; 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700031 - Failed to install the HAP because the overlay check of the HAP is failed.
     * @throws { BusinessError } 17700036 - Failed to install the HSP because lacks appropriate permissions.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     *  (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700052 - Failed to install the HAP because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700054 - Failed to install the HAP because the HAP requests wrong permissions.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because the device has been controlled.
     * @throws { BusinessError } 17700066 - Failed to install the HAP because installing the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700076 - Failed to install the HAP or HSP because the app distribution type is not allowed.
     * @throws { BusinessError } 17700077 - Failed to install the HAP and restore to preinstalled bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 18
     */
    install(hapFilePaths: Array<string>, installParam?: InstallParam): Promise<void>;

    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.UNINSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is pre-installed bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall because enterprise device management disallow uninstall.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstallation.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700062 - Failed to uninstall the app because the app is locked.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    uninstall(bundleName: string, installParam: InstallParam, callback: AsyncCallback<void>): void;

    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.UNINSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is pre-installed bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall because enterprise device management disallow uninstall.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling application result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    uninstall(bundleName: string, callback: AsyncCallback<void>): void;

    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.UNINSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is pre-installed bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle which cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall because enterprise device management disallow uninstall.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
     /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    /**
     * Uninstall an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled.
     * @param { InstallParam } [installParam] - Indicates other parameters required for the uninstallation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700040 - The specified bundle is a shared bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700045 - Failed to uninstall the HAP because uninstall is not allowed by the enterprise device management.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700062 - Failed to uninstall the app because the app is locked.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    uninstall(bundleName: string, installParam?: InstallParam): Promise<void>;

    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because this application is prohibited
     * <br>from being installed on this device or by specified users.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 14
     */
    recover(bundleName: string, installParam: InstallParam, callback: AsyncCallback<void>): void;

    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { AsyncCallback<void> } callback - The callback of recovering application result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because this application is prohibited
     * <br>from being installed on this device or by specified users.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 14
     */
    recover(bundleName: string, callback: AsyncCallback<void>): void;

    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    /**
     * Recover an application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.RECOVER_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be recovered.
     * @param { InstallParam } installParam - Indicates other parameters required for the recover.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE' or 'ohos.permission.RECOVER_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because this application is prohibited
     * <br>from being installed on this device or by specified users.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 14
     */
    recover(bundleName: string, installParam?: InstallParam): Promise<void>;

    /**
     * Uninstall a shared bundle.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { UninstallParam } uninstallParam - Indicates parameters required for the uninstall.
     * @param { AsyncCallback<void> } callback - The callback of uninstalling shared bundle result.
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700037 - The version of shared bundle is dependent on other applications.
     * @throws { BusinessError } 17700038 - The specified shared bundle does not exist.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    uninstall(uninstallParam: UninstallParam, callback: AsyncCallback<void>): void;

    /**
     * Uninstall a shared bundle.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { UninstallParam } uninstallParam - Indicates parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700020 - The specified bundle is a pre-installed bundle and cannot be uninstalled.
     * @throws { BusinessError } 17700037 - The version of shared bundle is dependent on other applications.
     * @throws { BusinessError } 17700038 - The specified shared bundle does not exist.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    uninstall(uninstallParam: UninstallParam): Promise<void>;

    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - Indicates the callback used to return the HAP installation result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @param { AsyncCallback<void> } callback - Indicates the callback used to return the HAP installation result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    updateBundleForSelf(hapFilePaths: Array<string>, installParam: InstallParam, callback: AsyncCallback<void>): void;

    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - Indicates the callback used to return the HAP installation result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { AsyncCallback<void> } callback - Indicates the callback used to return the HAP installation result.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    updateBundleForSelf(hapFilePaths: Array<string>, callback: AsyncCallback<void>): void;

    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    /**
     * Updates this enterprise application.
     *
     * @permission ohos.permission.INSTALL_SELF_BUNDLE
     * @param { Array<string> } hapFilePaths - Indicates the path where the HAP of the application is stored.
     * @param { InstallParam } installParam - Indicates other parameters required for the installation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_SELF_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified;
     * 2. Incorrect parameter types; 3. Parameter hapFiles is needed for code signature;
     * 4. The size of specifiedDistributionType is greater than 128; 5. The size of additionalInfo is greater than 3000.
     * @throws { BusinessError } 17700004 - The specified user ID is not found.
     * @throws { BusinessError } 17700010 - Failed to install the HAP because the HAP fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the HAP because the HAP signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the HAP because the HAP path is invalid or the HAP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the HAPs because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the HAP because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the HAP since the version of the HAP to install is too early.
     * @throws { BusinessError } 17700018 - Failed to install because the dependent module does not exist.
     * @throws { BusinessError } 17700039 - Failed to install because disallow install a shared bundle by hapFilePaths.
     * @throws { BusinessError } 17700041 - Failed to install because enterprise device management disallow install.
     * @throws { BusinessError } 17700042 - Failed to install the HAP because of incorrect URI in the data proxy.
     * @throws { BusinessError } 17700043 - Failed to install the HAP because of low APL in the non-system data proxy
     * (required APL: system_basic or system_core).
     * @throws { BusinessError } 17700044 - Failed to install the HAP because the isolationMode configured is not supported.
     * @throws { BusinessError } 17700047 - Failed to install the HAP because the VersionCode to be updated is not greater than the current VersionCode.
     * @throws { BusinessError } 17700048 - Failed to install the HAP because the code signature verification is failed.
     * @throws { BusinessError } 17700049 - Failed to install the HAP because the bundleName is different from the bundleName of the caller application.
     * @throws { BusinessError } 17700050 - Failed to install the HAP because enterprise normal/MDM bundle cannot be installed on non-enterprise device.
     * @throws { BusinessError } 17700051 - Failed to install the HAP because the distribution type of caller application is not enterprise_mdm.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    updateBundleForSelf(hapFilePaths: Array<string>, installParam?: InstallParam): Promise<void>;

    /**
     * Uninstall updates for a pre-installed application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled updates.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700045 - Failed to uninstall because enterprise device management disallow uninstall.
     * @throws { BusinessError } 17700057 - Failed to uninstall updates because the HAP is not pre-installed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Uninstall updates for a pre-installed application.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of the application to be uninstalled updates.
     * @param { InstallParam } installParam - Indicates other parameters required for the uninstall.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Permission verification failed. The application does not have the permission required to call the API.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700045 - Failed to uninstall because enterprise device management disallow uninstall.
     * @throws { BusinessError } 17700057 - Failed to uninstall updates because the HAP is not pre-installed.
     * @throws { BusinessError } 17700060 - The specified application cannot be uninstalled.
     * @throws { BusinessError } 17700067 - Failed to uninstall the HAP because uninstalling the native package failed.
     * @throws { BusinessError } 17700073 - Failed to install the HAP because an application with the same
     * <br>bundle name but different signature information exists on the device.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 13
     */
    uninstallUpdates(bundleName: string, installParam?: InstallParam): Promise<void>;

    /**
     * Add extend resources.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundleName.
     * @param { Array<string> } filePaths - Indicates the file path for extend resources.
     * @returns { Promise<void> } Returns addExtResource result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName is not found.
     * @throws { BusinessError } 17700301 - AddExtResource failed due to parse file failed.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
    */
    addExtResource(bundleName: string, filePaths: Array<string>): Promise<void>;

    /**
     * Remove extend resources.
     *
     * @permission ohos.permission.INSTALL_BUNDLE or ohos.permission.UNINSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundleName.
     * @param { Array<string> } moduleNames - Indicates the moduleNames for extend resources.
     * @returns { Promise<void> } Returns removeExtResource result.
     * @throws { BusinessError } 201 - Permission denied.
     * @throws { BusinessError } 202 - Permission denied, non-system app called system api.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName is not found.
     * @throws { BusinessError } 17700302 - RemoveExtResource failed due to module does not exist.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    removeExtResource(bundleName: string, moduleNames: Array<string>): Promise<void>;

    /**
     * Create clone instance for an application.
     *
     * @permission ohos.permission.INSTALL_CLONE_BUNDLE
     * @param { string } bundleName - Indicates the path where the HAP of the application is stored.
     * @param { CreateAppCloneParam } [createAppCloneParam] Indicates the optional params of create clone app.
     * @returns { Promise<number> } Return the appIndex of the clone application.
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_CLONE_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700061 - The appIndex is not in valid range or already exists.
     * @throws { BusinessError } 17700069 - The app does not support the creation of an appClone instance.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
     createAppClone(bundleName: string, createAppCloneParam?: CreateAppCloneParam): Promise<number>;

    /**
     * Destroy clone instance for an application.
     *
     * @permission ohos.permission.UNINSTALL_CLONE_BUNDLE
     * @param { string } bundleName - Indicates the path where the HAP of the application is stored.
     * @param { number } appIndex - Indicates the clone application's index.
     * @param { number } [userId] - userId Indicates the user ID.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.UNINSTALL_CLONE_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700061 - AppIndex not in valid range.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
     destroyAppClone(bundleName: string, appIndex: number, userId?: number): Promise<void>;

    /**
     * Destroy clone instance for an application.
     *
     * @permission ohos.permission.UNINSTALL_CLONE_BUNDLE
     * @param { string } bundleName - Indicates the bundleName of clone app.
     * @param { number } appIndex - Indicates the clone application's index.
     * @param { DestroyAppCloneParam } [destroyAppCloneParam] - Indicates other parameters required for the uninstallation.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.UNINSTALL_CLONE_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700061 - AppIndex not in valid range.
     * @throws { BusinessError } 17700062 - Failed to uninstall the app because the app is locked.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
     destroyAppClone(bundleName: string, appIndex: number, destroyAppCloneParam?: DestroyAppCloneParam): Promise<void>;

    /**
     * Install application by bundle name with specified user.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of application.
     * @param { number } [userId] - userId Indicates the user ID.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700071 - It is not allowed to install the enterprise bundle.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    /**
     * Install application by bundle name with specified user.
     *
     * @permission ohos.permission.INSTALL_BUNDLE
     * @param { string } bundleName - Indicates the bundle name of application.
     * @param { number } [userId] - userId Indicates the user ID.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 401 - Parameter error. Possible causes: 1. Mandatory parameters are left unspecified; 2. Incorrect parameter types.
     * @throws { BusinessError } 17700001 - The specified bundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700071 - It is not allowed to install the enterprise bundle.
     * @throws { BusinessError } 17700058 - Failed to install the HAP because this application is prohibited
     * <br>from being installed on this device or by specified users.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 14
     */
     installPreexistingApp(bundleName: string, userId?: number): Promise<void>;

     /**
     * Install plugin for host application.
     *
     * @permission ohos.permission.INSTALL_PLUGIN_BUNDLE
     * @param { string } hostBundleName - Indicates the bundle name of host application.
     * @param { Array<string> } pluginFilePaths - Indicates the file paths of plugin.
     * @param { PluginParam } [pluginParam] - Indicates parameters required for install plugin.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.INSTALL_PLUGIN_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 17700001 - The specified hostBundleName cannot be found or the bundle is not installed by the specified user.
     * @throws { BusinessError } 17700004 - The userId is invalid.
     * @throws { BusinessError } 17700010 - Failed to install the plugin because the plugin fails to be parsed.
     * @throws { BusinessError } 17700011 - Failed to install the plugin because the plugin signature fails to be verified.
     * @throws { BusinessError } 17700012 - Failed to install the plugin because the HSP path is invalid or the HSP is too large.
     * @throws { BusinessError } 17700015 - Failed to install the plugin because they have different configuration information.
     * @throws { BusinessError } 17700016 - Failed to install the plugin because of insufficient system disk space.
     * @throws { BusinessError } 17700017 - Failed to install the plugin since the version of the plugin to install is too early.
     * @throws { BusinessError } 17700048 - Failed to install the plugin because the code signature verification is failed.
     * @throws { BusinessError } 17700052 - Failed to install the plugin because debug bundle cannot be installed under non-developer mode.
     * @throws { BusinessError } 17700073 - Failed to install the plugin because a plugin with the same
     * <br>bundle name but different signature information exists on the device.
     * @throws { BusinessError } 17700087 - Failed to install the plugin because the current device does not support plugin.
     * @throws { BusinessError } 17700088 - Failed to install the plugin because the host application lacks ohos.permission.kernel.SUPPORT_PLUGIN.
     * @throws { BusinessError } 17700089 - Failed to install the plugin because the plugin id fails to be parsed.
     * @throws { BusinessError } 17700090 - Failed to install the plugin because the plugin id fails to be verified.
     * @throws { BusinessError } 17700091 - Failed to install the plugin because the plugin name is same as host bundle name.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 19
     */
     installPlugin(hostBundleName: string, pluginFilePaths: Array<string>, pluginParam?: PluginParam): Promise<void>;

     /**
     * Uninstall plugin for host application.
     *
     * @permission ohos.permission.UNINSTALL_PLUGIN_BUNDLE
     * @param { string } hostBundleName - Indicates the bundle name of host application.
     * @param { string } pluginBundleName - Indicates the bundle name of plugin application.
     * @param { PluginParam } [pluginParam] - Indicates parameters required for uninstall plugin.
     * @returns { Promise<void> }
     * @throws { BusinessError } 201 - Calling interface without permission 'ohos.permission.UNINSTALL_PLUGIN_BUNDLE'.
     * @throws { BusinessError } 202 - Permission verification failed. A non-system application calls a system API.
     * @throws { BusinessError } 17700001 - The specified bundle name is not found.
     * @throws { BusinessError } 17700004 - The user id is invalid.
     * @throws { BusinessError } 17700092 - Failed to uninstall the plugin because the specified plugin is not found.
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 19
     */
     uninstallPlugin(hostBundleName: string, pluginBundleName: string, pluginParam?: PluginParam): Promise<void>;
  }

  /**
   * Provides parameters required for hashParam.
   *
   * @typedef HashParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  export interface HashParam {
    /**
     * Indicates the moduleName
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    moduleName: string;

    /**
     * Indicates the hash value
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    hashValue: string;
  }

  /**
   * Provides parameters required for VerifyCodeParam.
   *
   * @typedef VerifyCodeParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 10
   * @deprecated since 11
   */
  export interface VerifyCodeParam {
    /**
     * Indicates the moduleName which hopes to be processed with code signature.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     * @deprecated since 11
     */
    moduleName: string;

    /**
     * Indicates the path where the code signature file of the corresponding HAP is stored.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     * @deprecated since 11
     */
    signatureFilePath: string;
  }

  /**
   * Provides parameters required for PGOParam.
   *
   * @typedef PGOParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 11
   */
  export interface PGOParam {
    /**
     * Indicates the moduleName that need to store profile-guided optimization(PGO) file.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
    moduleName: string;

    /**
     * Indicates the path where the profile-guided optimization(PGO) file is stored.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
    pgoFilePath: string;
  }

  /**
   * Provides parameters required for Parameters.
   *
   * @typedef Parameters
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 15
   */
  export interface Parameters {
    /**
     * Indicates key.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    key: string;

    /**
     * Indicates value.
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    value: string;
  }

  /**
   * Provides parameters required for installing or uninstalling an application.
   *
   * @typedef InstallParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 9
   */
  export interface InstallParam {
    /**
     * Indicates the user id
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    userId?: number;

    /**
     * Indicates the installation type. The value 0x00 means normal installation, 0x01 means overwrite installation, and 0x10 means installation-free.
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    installFlag?: number;

    /**
     * Indicates whether the param has data
     *
     * @type { ?boolean }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    isKeepData?: boolean;

    /**
     * Indicates the hash params
     *
     * @type { ?Array<HashParam> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    hashParams?: Array<HashParam>;

    /**
     * Indicates the deadline of the crowdtesting bundle
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 9
     */
    crowdtestDeadline?: number;

    /**
     * Indicates the shared bundle dir paths.
     *
     * @type { ?Array<String> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    sharedBundleDirPaths?: Array<String>;

    /**
     * Indicates the distribution type specified during bundle installation.
     *
     * @type { ?string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    specifiedDistributionType?: string;

    /**
     * Indicates the additional information during bundle installation.
     *
     * @type { ?string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    additionalInfo?: string;

    /**
     * Indicates the verification code param.
     *
     * @type { ?Array<VerifyCodeParam> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     * @deprecated since 11
     */
    verifyCodeParams?: Array<VerifyCodeParam>;

    /**
     * Indicates the profile-guided optimization(PGO) params.
     *
     * @type { ?Array<PGOParam> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 11
     */
    pgoParams?: Array<PGOParam>;

    /**
     * Indicates parameters.
     *
     * @type { ?Array<Parameters> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    parameters?: Array<Parameters>;
  }

  /**
   * Provides parameters required for uninstalling shared bundle.
   *
   * @typedef UninstallParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 10
   */
  export interface UninstallParam {
    /**
     * Indicates the shared bundle name
     *
     * @type { string }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    bundleName: string;

    /**
     * Indicates the shared version code. If default, indicates that all version sharing bundles are uninstalled
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 10
     */
    versionCode?: number;
  }

  /**
   * Provides parameters required for creating clone app.
   *
   * @typedef CreateAppCloneParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 12
   */
   export interface CreateAppCloneParam {
    /**
     * Indicates the user id
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    userId?: number;
    /**
     * Indicates the appIndex of MultiApp
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 12
     */
    appIndex?: number;
  }

  /**
   * Provides parameters required for destroy clone app.
   *
   * @typedef DestroyAppCloneParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 15
   */
  export interface DestroyAppCloneParam {
    /**
     * Indicates the user id
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    userId?: number;
    /**
     * Indicates parameters.
     *
     * @type { ?Array<Parameters> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 15
     */
    parameters?: Array<Parameters>;
  }

  /**
   * Provides parameters required for installing a plugin or uninstalling a plugin
   *
   * @typedef PluginParam
   * @syscap SystemCapability.BundleManager.BundleFramework.Core
   * @systemapi
   * @since 19
   */
  export interface PluginParam {
    /**
     * Indicates the user id.
     *
     * @type { ?number }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 19
     */
    userId?: number;

    /**
     * Indicates parameters.
     *
     * @type { ?Array<Parameters> }
     * @syscap SystemCapability.BundleManager.BundleFramework.Core
     * @systemapi
     * @since 19
     */
    parameters?: Array<Parameters>;
  }
}

export default installer;
