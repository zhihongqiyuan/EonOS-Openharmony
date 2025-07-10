/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
/**
 * @file Define capabilities to access app domain verification info.
 * @kit AbilityKit
 */

/**
 * This module provides app domain verification info.
 *
 * @namespace appDomainVerify
 * @syscap SystemCapability.BundleManager.AppDomainVerify
 * @systemapi
 * @stagemodelonly
 * @since 13
 */
declare namespace appDomainVerify {


  /**
   * query domains verify associated with bundleName.
   * @permission ohos.permission.GET_APP_DOMAIN_BUNDLE_INFO
   * @param { string } bundleName - app bundleName.
   * @returns { string[] } Result domains.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - System API accessed by non-system app.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 29900001 - Internal error.
   * @syscap SystemCapability.BundleManager.AppDomainVerify
   * @systemapi
   * @stagemodelonly
   * @since 13
   */
  function queryAssociatedDomains(bundleName: string): string[];

  /**
   * query bundleNames associated with domain.
   * @permission ohos.permission.GET_APP_DOMAIN_BUNDLE_INFO
   * @param { string } domain - Parameters related to the function.
   * @returns { string[] } Result bundleNames.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - System API accessed by non-system app.
   * @throws { BusinessError } 401 - Parameter error.
   * @throws { BusinessError } 29900001 - Internal error.
   * @syscap SystemCapability.BundleManager.AppDomainVerify
   * @systemapi
   * @stagemodelonly
   * @since 13
   */
  function queryAssociatedBundleNames(domain: string): string[];
}

export default appDomainVerify;

