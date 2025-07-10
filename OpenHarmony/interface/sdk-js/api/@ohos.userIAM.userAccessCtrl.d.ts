/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License"),
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
 * @kit UserAuthenticationKit
 */

import userAuth from '@ohos.userIAM.userAuth';

/**
 * User access control
 *
 * @namespace userAccessCtrl
 * @syscap SystemCapability.UserIAM.UserAuth.Core
 * @since 18
 */
declare namespace userAccessCtrl {
  /**
   * Verify the authentication token.
   *
   * @permission ohos.permission.USE_USER_ACCESS_MANAGER
   * @param { Uint8Array } authToken - AuthToken to be verified.
   * @param { number } allowableDuration - Allowable time interval from the authToken is issued till now in milliseconds.
   * @returns { Promise<AuthToken> } Returns parsed authToken.
   * @throws { BusinessError } 201 - Permission denied.
   * @throws { BusinessError } 202 - Not system application.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500015 - AuthToken integrity check failed.
   * @throws { BusinessError } 12500016 - AuthToken has expired.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  function verifyAuthToken(authToken: Uint8Array, allowableDuration: number): Promise<AuthToken>;

  /**
   * Authentication token.
   *
   * @typedef AuthToken
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  interface AuthToken {
    /**
     * Pass in challenge value.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    challenge: Uint8Array;

    /**
     * Trust level of authentication result.
     *
     * @type { userAuth.AuthTrustLevel }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    authTrustLevel: userAuth.AuthTrustLevel;

    /**
     * Credential type for authentication.
     *
     * @type { userAuth.UserAuthType }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    authType: userAuth.UserAuthType;

    /**
     * The type of authToken.
     *
     * @type { AuthTokenType }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    tokenType: AuthTokenType;

    /**
     * The user id of authToken.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    userId: number;

    /**
     * The time interval from the authToken is issued till now in milliseconds.
     *
     * @type { bigint }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    timeInterval: bigint;

    /**
     * The secure uid of authToken.
     *
     * @type { ?bigint }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    secureUid?: bigint;

    /**
     * The enrolled id of authToken.
     *
     * @type { ?bigint }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    enrolledId?: bigint;

    /**
     * The credential id of authToken.
     *
     * @type { ?bigint }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    credentialId?: bigint;
  }

  /**
   * The issued type for authToken.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 18
   */
  enum AuthTokenType {
    /**
     * AuthToken is issued locally.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    TOKEN_TYPE_LOCAL_AUTH = 0,

    /**
     * AuthToken is re-issued.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    TOKEN_TYPE_LOCAL_RESIGN = 1,

    /**
     * AuthToken is issued remotely.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    TOKEN_TYPE_COAUTH = 2
  }
}

export default userAccessCtrl;
