/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
 * @file Provides the capability of integrating advertising services with vendors
 * @kit AdsKit
 */

import type advertising from './@ohos.advertising';

/**
 * Provides the capability of integrating advertising services with vendors.
 * @syscap SystemCapability.Advertising.Ads
 * @systemapi
 * @since 11
 */
export default class AdsServiceExtensionAbility {

  /**
   * Called when media application starts to load ad.
   * @param { advertising.AdRequestParams } adParam - Indicates the parameters in the request.
   * @param { advertising.AdOptions } adOptions - Indicates the ad options.
   * @param { RespCallback } respCallback - The response callback.
   * @syscap SystemCapability.Advertising.Ads
   * @systemapi
   * @since 11
   */
  onLoadAd(adParam: advertising.AdRequestParams, adOptions: advertising.AdOptions, respCallback: RespCallback);

  /**
   * Called when media application starts to load ad with multi-slots.
   * @param { advertising.AdRequestParams[] } adParams - Indicates the parameters in the request.
   * @param { advertising.AdOptions } adOptions - Indicates the ad options.
   * @param { RespCallback } respCallback - The response callback.
   * @syscap SystemCapability.Advertising.Ads
   * @systemapi
   * @since 11
   */
  onLoadAdWithMultiSlots(adParams: advertising.AdRequestParams[], adOptions: advertising.AdOptions, respCallback: RespCallback);
}

/**
 * Defines the callback of loading ad.
 * @typedef RespCallback
 * @syscap SystemCapability.Advertising.Ads
 * @since 11
 */
export interface RespCallback {
  /**
   * Defines the callback data.
   * @param { Map<string, Array<advertising.Advertisement>> } respData
   * @syscap SystemCapability.Advertising.Ads
   * @since 11
   */
  (respData: Map<string, Array<advertising.Advertisement>>): void;
}



