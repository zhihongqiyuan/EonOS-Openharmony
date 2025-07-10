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
 * @file
 * @kit FormKit
 */

import type { AsyncCallback } from './@ohos.base';
import type Want from './@ohos.app.ability.Want';

/**
 * Interface of formAgent.
 *
 * @namespace formAgent
 * @syscap SystemCapability.Ability.Form
 * @systemapi
 * @since 11
 */
declare namespace formAgent {

  /**
   * Request to publish a form to the form host.
   *
   * @permission ohos.permission.AGENT_REQUIRE_FORM
   * @param { Want } want - The want of the form to publish.
   * @param { AsyncCallback<string> } callback - The callback is used to return the form id.
   * @throws { BusinessError } 202 - The application is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *     1.Mandatory parameters are left unspecified; 2.Incorrect parameter types; 3.Parameter verification failed.
   * @throws { BusinessError } 16500050 - An IPC connection error happened.
   * @throws { BusinessError } 16500100 - Failed to obtain the configuration information.
   * @throws { BusinessError } 16501000 - An internal functional error occurred.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 11
   */
  /**
   * Request to publish a form to the form host.
   *
   * @permission ohos.permission.AGENT_REQUIRE_FORM
   * @param { Want } want - The want of the form to publish.
   * @param { AsyncCallback<string> } callback - The callback is used to return the form id.
   * @throws { BusinessError } 202 - The application is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *     1.Mandatory parameters are left unspecified;2.Incorrect parameter types; 3.Parameter verification failed.
   * @throws { BusinessError } 16500050 - IPC connection error.
   * @throws { BusinessError } 16500100 - Failed to obtain the configuration information.
   * @throws { BusinessError } 16501000 - An internal functional error occurred.
   * @throws { BusinessError } 16501008 - Waiting for the form addition to the desktop timed out.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 12
   */
  function requestPublishForm(want: Want, callback: AsyncCallback<string>): void;

  /**
   * Request to publish a form to the form host.
   *
   * @permission ohos.permission.AGENT_REQUIRE_FORM
   * @param { Want } want - The want of the form to publish.
   * @returns { Promise<string> } Returns the form id.
   * @throws { BusinessError } 202 - The application is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *     1.Mandatory parameters are left unspecified; 2.Incorrect parameter types; 3.Parameter verification failed.
   * @throws { BusinessError } 16500050 - An IPC connection error happened.
   * @throws { BusinessError } 16500100 - Failed to obtain the configuration information.
   * @throws { BusinessError } 16501000 - An internal functional error occurred.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 11
   */
  /**
   * Request to publish a form to the form host.
   *
   * @permission ohos.permission.AGENT_REQUIRE_FORM
   * @param { Want } want - The want of the form to publish.
   * @returns { Promise<string> } Returns the form id.
   * @throws { BusinessError } 202 - The application is not a system application.
   * @throws { BusinessError } 401 - Parameter error. Possible causes:
   *     1.Mandatory parameters are left unspecified; 2.Incorrect parameter types; 3.Parameter verification failed.
   * @throws { BusinessError } 16500050 - IPC connection error.
   * @throws { BusinessError } 16500100 - Failed to obtain the configuration information.
   * @throws { BusinessError } 16501000 - An internal functional error occurred.
   * @throws { BusinessError } 16501008 - Waiting for the form addition to the desktop timed out.
   * @syscap SystemCapability.Ability.Form
   * @systemapi
   * @since 12
   */
  function requestPublishForm(want: Want): Promise<string>;
}
export default formAgent;
