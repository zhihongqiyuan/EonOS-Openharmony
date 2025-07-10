/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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
 * @kit UserAuthenticationKit
 */

import type { AsyncCallback } from './@ohos.base';

/**
 * User authentication
 *
 * @namespace userAuth
 * @syscap SystemCapability.UserIAM.UserAuth.Core
 * @since 6
 */
/**
 * The userAuth module provides APIs for user authentication, which applies to scenarios such as device unlocking,
 * payment, and application login.
 *
 * @namespace userAuth
 * @syscap SystemCapability.UserIAM.UserAuth.Core
 * @atomicservice
 * @since 12
 */
declare namespace userAuth {
  /**
   * Maximum reuse duration of the authentication result, in milliseconds. The value is 300000.
   *
   * @constant
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  const MAX_ALLOWABLE_REUSE_DURATION: 300000;

  /**
   * Enum for authentication result.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 6
   * @deprecated since 8
   * @useinstead ohos.userIAM.userAuth.ResultCode
   */
  export enum AuthenticationResult {
    /**
     * Indicates that the device does not support authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    NO_SUPPORT = -1,

    /**
     * Indicates that authentication is success.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    SUCCESS = 0,

    /**
     * Indicates the authenticator fails to identify user.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    COMPARE_FAILURE = 1,

    /**
     * Indicates that authentication has been canceled.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    CANCELED = 2,

    /**
     * Indicates that authentication has timed out.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    TIMEOUT = 3,

    /**
     * Indicates a failure to open the camera.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    CAMERA_FAIL = 4,

    /**
     * Indicates that the authentication task is busy. Wait for a few seconds and try again.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    BUSY = 5,

    /**
     * Indicates incorrect parameters.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    INVALID_PARAMETERS = 6,

    /**
     * Indicates that the authenticator is locked.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    LOCKED = 7,

    /**
     * Indicates that the user has not enrolled the authenticator.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    NOT_ENROLLED = 8,

    /**
     * Indicates other errors.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    GENERAL_ERROR = 100
  }

  /**
   * Auth types
   *
   * @typedef { 'ALL' | 'FACE_ONLY' }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 6
   * @deprecated since 8
   */
  type AuthType = 'ALL' | 'FACE_ONLY';

  /**
   * Secure levels
   *
   * @typedef { 'S1' | 'S2' | 'S3' | 'S4' }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 6
   * @deprecated since 8
   */
  type SecureLevel = 'S1' | 'S2' | 'S3' | 'S4';

  /**
   * Used to initiate authentication.
   *
   * @interface Authenticator
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 6
   * @deprecated since 8
   */
  interface Authenticator {
    /**
     * Execute authentication.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @param { AuthType } type - Indicates the authentication type.
     * @param { SecureLevel } level - Indicates the security level.
     * @param { AsyncCallback<number> } callback - Async callback of execute.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    execute(type: AuthType, level: SecureLevel, callback: AsyncCallback<number>): void;

    /**
     * Execute authentication.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @param { AuthType } type - Indicates the authentication type.
     * @param { SecureLevel } level - Indicates the security level.
     * @returns { Promise<number> }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 6
     * @deprecated since 8
     */
    execute(type: AuthType, level: SecureLevel): Promise<number>;
  }

  /**
   * Get Authenticator instance.
   *
   * @returns { Authenticator } Returns an Authenticator.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 6
   * @deprecated since 8
   */
  function getAuthenticator(): Authenticator;

  /**
   * User authentication.
   *
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.userIAM.userAuth.AuthInstance
   */
  class UserAuth {
    /**
     * Constructor to get the UserAuth class instance.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.getAuthInstance
     */
    constructor();

    /**
     * Get version information.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @returns { number } Returns version information.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.getVersion
     */
    getVersion(): number;

    /**
     * Check whether the authentication capability is available.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @param { UserAuthType } authType - Credential type for authentication.
     * @param { AuthTrustLevel } authTrustLevel - Trust level of authentication result.
     * @returns { number } Returns a check result, which is specified by getAvailableStatus, the value of number is related to the ResultCode enum, **201** is
     * check permission failed.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.getAvailableStatus
     */
    getAvailableStatus(authType: UserAuthType, authTrustLevel: AuthTrustLevel): number;

    /**
     * Executes authentication.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @param { Uint8Array } challenge - Pass in challenge value.
     * @param { UserAuthType } authType - Type of authentication.
     * @param { AuthTrustLevel } authTrustLevel - Trust level of authentication result.
     * @param { IUserAuthCallback } callback - Return result and acquireInfo through callback, the value of result code is related to the ResultCode enum,
     * **201** is check permission failed.
     * @returns { Uint8Array } Returns ContextId for cancel.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.AuthInstance.start
     */
    auth(
      challenge: Uint8Array,
      authType: UserAuthType,
      authTrustLevel: AuthTrustLevel,
      callback: IUserAuthCallback
    ): Uint8Array;

    /**
     * Cancel authentication with ContextID.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @param { Uint8Array } contextID - Cancel authentication and pass in ContextID.
     * @returns { number } Returns a number value indicating whether Cancel authentication was successful, the value of number is related to the ResultCode
     * enum, **201** is check permission failed.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.AuthInstance.cancel
     */
    cancelAuth(contextID: Uint8Array): number;
  }

  /**
   * Asynchronous callback of authentication operation.
   *
   * @interface IUserAuthCallback
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.userIAM.userAuth.AuthEvent
   */
  interface IUserAuthCallback {
    /**
     * The authentication result code is returned through the callback.
     * If the authentication is passed, the authentication token is returned in extraInfo,
     * If the authentication fails, the remaining authentication times are returned in extraInfo,
     * If the authentication executor is locked, the freezing time is returned in extraInfo.
     *
     * @type { function }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.AuthEvent.callback
     */
    onResult: (result: number, extraInfo: AuthResult) => void;

    /**
     * During an authentication, the TipsCode is returned through the callback.
     *
     * @type { ?function }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     * @useinstead ohos.userIAM.userAuth.AuthEvent.callback
     */
    onAcquireInfo?: (module: number, acquire: number, extraInfo: any) => void;
  }

  /**
   * Authentication result: authentication token, remaining authentication times, freezing time.
   *
   * @typedef AuthResult
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.userIAM.userAuth.AuthResultInfo
   */
  interface AuthResult {
    /**
     * The authentication result if the authentication is passed.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    token?: Uint8Array;

    /**
     * The remaining authentication times if the authentication fails.
     *
     * @type { ?number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    remainTimes?: number;

    /**
     * The freezing time if the authentication executor is locked.
     *
     * @type { ?number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    freezingTime?: number;
  }

  /**
   * Enum for operation result.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 9
   * @useinstead ohos.userIAM.userAuth.UserAuthResultCode
   */
  enum ResultCode {
    /**
     * Indicates that the result is success or ability is supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    SUCCESS = 0,

    /**
     * Indicates that authentication failed.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    FAIL = 1,

    /**
     * Indicates other errors.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    GENERAL_ERROR = 2,

    /**
     * Indicates that this operation has been canceled.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    CANCELED = 3,

    /**
     * Indicates that this operation has timed out.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    TIMEOUT = 4,

    /**
     * Indicates that this authentication type is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    TYPE_NOT_SUPPORT = 5,

    /**
     * Indicates that the authentication trust level is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    TRUST_LEVEL_NOT_SUPPORT = 6,

    /**
     * Indicates that the authentication task is busy. Wait for a few seconds and try again.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    BUSY = 7,

    /**
     * Indicates incorrect parameters.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    INVALID_PARAMETERS = 8,

    /**
     * Indicates that the authenticator is locked.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    LOCKED = 9,

    /**
     * Indicates that the user has not enrolled the authenticator.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 9
     */
    NOT_ENROLLED = 10
  }

  /**
   * The enumeration of prompt codes in the process of face authentication.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 11
   */
  enum FaceTips {
    /**
     * Indicates that the obtained facial image is too bright due to high illumination.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_BRIGHT = 1,

    /**
     * Indicates that the obtained facial image is too dark due to low illumination.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_DARK = 2,

    /**
     * Indicates that the face is too close to the device.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_CLOSE = 3,

    /**
     * Indicates that the face is too far away from the device.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_FAR = 4,

    /**
     * Indicates that the device is too high, and that only the upper part of the face is captured.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_HIGH = 5,

    /**
     * Indicates that the device is too low, and that only the lower part of the face is captured.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_LOW = 6,

    /**
     * Indicates that the device is deviated to the right, and that only the right part of the face is captured.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_RIGHT = 7,

    /**
     * Indicates that the device is deviated to the left, and that only the left part of the face is captured.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_LEFT = 8,

    /**
     * Indicates that the face moves too fast during facial information collection.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_TOO_MUCH_MOTION = 9,

    /**
     * Indicates that the face is not facing the device.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_POOR_GAZE = 10,

    /**
     * Indicates that no face is detected.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FACE_AUTH_TIP_NOT_DETECTED = 11
  }

  /**
   * The enumeration of prompt codes in the process of fingerprint authentication.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   * @deprecated since 11
   */
  enum FingerprintTips {
    /**
     * Indicates that the image acquired is good.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_GOOD = 0,

    /**
     * Indicates that the fingerprint image is too noisy due to suspected or detected dirt on sensor.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_DIRTY = 1,

    /**
     * Indicates that the fingerprint image is too noisy to process due to a detected condition.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_INSUFFICIENT = 2,

    /**
     * Indicates that only a partial fingerprint image is detected.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_PARTIAL = 3,

    /**
     * Indicates that the fingerprint image is incomplete due to quick motion.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_TOO_FAST = 4,

    /**
     * Indicates that the fingerprint image is unreadable due to lack of motion.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     * @deprecated since 11
     */
    FINGERPRINT_AUTH_TIP_TOO_SLOW = 5
  }

  /**
   * Credential type for authentication.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   */
  /**
   * Enumerates the identity authentication types.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  enum UserAuthType {
    /**
     * Authentication type pin.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * PIN authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    PIN = 1,

    /**
     * Authentication type face.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Facial authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    FACE = 2,

    /**
     * Authentication type fingerprint.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Fingerprint authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    FINGERPRINT = 4,

    /**
     * Privacy password authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 14
     */
    PRIVATE_PIN = 16
  }

  /**
   * Trust level of authentication results.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 8
   */
  /**
   * Enumerates the trust levels of the authentication result.
   * For details about typical scenarios and examples, see Principles for Classifying Biometric
   * Authentication Trust Levels.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  enum AuthTrustLevel {
    /**
     * Authentication result trusted level 1.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Authentication trust level 1. The authentication of this level can identify individual users and provides
     * limited liveness detection capabilities. It is applicable to scenarios such as service risk control and
     * access to common personal data.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    ATL1 = 10000,

    /**
     * Authentication result trusted level 2.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Authentication trust level 2. The authentication of this level can accurately identify individual users and
     * provides regular liveness detection capabilities. It is applicable to scenarios such as device unlocking
     * and application login.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    ATL2 = 20000,

    /**
     * Authentication result trusted level 3.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Authentication trust level 3. The authentication of this level can accurately identify individual users and
     * provides strong liveness detection capabilities. It is applicable to scenarios such as device unlocking.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    ATL3 = 30000,

    /**
     * Authentication result trusted level 4.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 8
     */
    /**
     * Authentication trust level 4. The authentication of this level can accurately identify individual users and
     * provides powerful liveness detection capabilities. It is applicable to scenarios such as small-amount payment.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    ATL4 = 40000
  }

  /**
   * Authentication events.
   *
   * @typedef { 'result' | 'tip' }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 11
   */
  type AuthEventKey = 'result' | 'tip';

  /**
   * Return information of Authentication events.
   *
   * @typedef { AuthResultInfo | TipInfo }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 11
   * @useinstead ohos.userIAM.userAuth.UserAuthResult
   */
  type EventInfo = AuthResultInfo | TipInfo;

  /**
   * Asynchronous callback of authentication event.
   *
   * @interface AuthEvent
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 11
   * @useinstead ohos.userIAM.userAuth.IAuthCallback
   */
  interface AuthEvent {
    /**
     * The authentication event callback.
     *
     * @param { EventInfo } result - Event info.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     * @useinstead ohos.userIAM.userAuth.IAuthCallback.onResult
     */
    callback(result: EventInfo): void;
  }

  /**
   * Authentication result information.
   *
   * @typedef AuthResultInfo
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 11
   */
  interface AuthResultInfo {
    /**
     * The authentication result.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    result: number;

    /**
     * The authentication token if the authentication is passed.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    token?: Uint8Array;

    /**
     * The remaining authentication attempts if the authentication fails.
     *
     * @type { ?number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    remainAttempts?: number;

    /**
     * The lockout duration if the authentication executor is locked.
     *
     * @type { ?number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    lockoutDuration?: number;
  }

  /**
   * Authentication tip info.
   *
   * @typedef TipInfo
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 11
   */
  interface TipInfo {
    /**
     * The authentication module of sending tip information.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    module: number;

    /**
     * Tip information, used to prompt the business to perform some operations.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 11
     */
    tip: number;
  }

  /**
   * Authentication instance, used to initiate a complete authentication.
   *
   * @interface AuthInstance
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 10
   * @useinstead ohos.userIAM.userAuth.UserAuthInstance
   */
  interface AuthInstance {
    /**
     * Turn on authentication event listening.
     *
     * @throws { BusinessError } 401 - Incorrect parameters.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 10
     */
    on: (name: AuthEventKey, callback: AuthEvent) => void;

    /**
     * Turn off authentication event listening.
     *
     * @throws { BusinessError } 401 - Incorrect parameters.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 10
     */
    off: (name: AuthEventKey) => void;

    /**
     * Start this authentication, an instance can only perform authentication once.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @type { function }
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters.
     * @throws { BusinessError } 12500001 - Authentication failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @throws { BusinessError } 12500003 - The operation is canceled.
     * @throws { BusinessError } 12500004 - The operation is time-out.
     * @throws { BusinessError } 12500005 - The authentication type is not supported.
     * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
     * @throws { BusinessError } 12500007 - The authentication task is busy.
     * @throws { BusinessError } 12500009 - The authenticator is locked.
     * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 10
     */
    start: () => void;

    /**
     * Cancel this authentication.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @type { function }
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     * @deprecated since 10
     */
    cancel: () => void;
  }

  /**
   * Check whether the authentication capability is available.
   *
   * @permission ohos.permission.ACCESS_BIOMETRIC
   * @param { UserAuthType } authType - Credential type for authentication.
   * @param { AuthTrustLevel } authTrustLevel - Trust level of authentication result.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
   * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   */
  /**
   * Checks whether the specified authentication capability is supported.
   *
   * <p><strong>NOTE</strong>:
   * <br>Error code 12500005 is returned if the authentication executor is not registered and the specified
   * authentication capability is not supported.
   * <br>Error code 12500006 is returned if the authentication executor has been registered, the authentication
   * functionality is not disabled, but the authentication trust level is lower than that specified by the service.
   * <br>Error code 12500010 is returned if the authentication executor has been registered, the authentication
   * functionality is not disabled, but the user has not enrolled credential.
   * <br>Error code 12500013 is returned if the authentication executor has been registered, the authentication
   * functionality is not disabled, but the password has expired.
   * <br>If getAvailableStatus is called to check whether lock screen password authentication at ATL4 is supported
   * for a user who has enrolled a 4-digit PIN as the lock screen password (the authentication trust level is ATL3),
   * error code 12500010 will be returned.
   * </p>
   *
   * @permission ohos.permission.ACCESS_BIOMETRIC
   * @param { UserAuthType } authType - Authentication type. PIN is supported since API version 11.
   * @param { AuthTrustLevel } authTrustLevel - Authentication trust level.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
   * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
   * @throws { BusinessError } 12500013 - Operation failed because of PIN expired.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  function getAvailableStatus(authType: UserAuthType, authTrustLevel: AuthTrustLevel): void;

  /**
   * Represents the state of a credential enrolled.
   *
   * @typedef EnrolledState
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface EnrolledState {
    /**
     * Credential digest, which is randomly generated when a credential is added.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    credentialDigest: number;

    /**
     * Number of enrolled credentials.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    credentialCount: number;
  }

  /**
   * Obtains the credential state.
   *
   * @permission ohos.permission.ACCESS_BIOMETRIC
   * @param { UserAuthType } authType - Authentication type.
   * @returns { EnrolledState } Credential state obtained if the operation is successful.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  function getEnrolledState(authType: UserAuthType): EnrolledState;

  /**
   * Get Authentication instance.
   *
   * @param { Uint8Array } challenge - Pass in challenge value.
   * @param { UserAuthType } authType - Credential type for authentication.
   * @param { AuthTrustLevel } authTrustLevel - Trust level of authentication result.
   * @returns { AuthInstance } Returns an authentication instance.
   * @throws { BusinessError } 401 - Incorrect parameters.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   * @deprecated since 10
   * @useinstead ohos.userIAM.userAuth.getUserAuthInstance
   */
  function getAuthInstance(challenge: Uint8Array, authType: UserAuthType, authTrustLevel: AuthTrustLevel): AuthInstance;

  /**
   * Enumerates the window types of the authentication widget.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  enum WindowModeType {
    /**
     * Window mode type is dialog box.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    DIALOG_BOX = 1,

    /**
     * Window mode type is full screen.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    FULLSCREEN = 2
  }

  /**
   * Enumerates the modes for reusing authentication results.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  enum ReuseMode {
    /**
     * The device unlock authentication result can be reused within the validity period if the authentication type
     * matches any of the authentication types specified for this authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    AUTH_TYPE_RELEVANT = 1,

    /**
     * The device unlock authentication result can be reused within the validity period regardless of the
     * authentication type.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    AUTH_TYPE_IRRELEVANT = 2,

    /**
     * Any identity authentication result (including device unlock authentication result) can be reused within the
     * validity period if the authentication type matches any of the authentication types specified for
     * this authentication.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 14
     */
    CALLER_IRRELEVANT_AUTH_TYPE_RELEVANT = 3,

    /**
     * Any identity authentication result (including device unlock authentication result) can be reused within the
     * validity period regardless of the authentication type.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 14
     */
    CALLER_IRRELEVANT_AUTH_TYPE_IRRELEVANT = 4
  }

  /**
   * Represents information about the authentication result reuse.
   *
   * <p><strong>NOTE</strong>:
   * <br>If the credential changes within the reuse duration after a successful identity authentication
   * (including device unlock authentication), the authentication result can still be reused and the actual
   * EnrolledState is returned in the authentication result.
   * <br>If the credential used for the previous authentication has been deleted when the authentication
   * result is used:
   * 1.If the deleted credential is face or fingerprint, the authentication result can still be reused, but
   * credentialCount and credentialDigest in the EnrolledState returned are both 0.
   * 2.If the deleted credential is a lock screen password, the reuse will fail.
   * </p>
   *
   * @typedef ReuseUnlockResult
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface ReuseUnlockResult {
    /**
     * Authentication result reuse mode.
     *
     * @type { ReuseMode }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    reuseMode: ReuseMode;

    /**
     * Period for which the authentication result can be reused. The value must be greater than 0 and less
     * than MAX_ALLOWABLE_REUSE_DURATION.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    reuseDuration: number;
  }

  /**
   * Auth parameter.
   *
   * @typedef AuthParam
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Defines the user authentication parameters.
   *
   * @typedef AuthParam
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface AuthParam {
    /**
     * Pass in challenge value.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Random challenge value, which can be used to prevent replay attacks.
     * It cannot exceed 32 bytes and can be passed in Uint8Array([]) format.
     *
     * @type { Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    challenge: Uint8Array;

    /**
     * Credential type for authentication.
     *
     * @type { UserAuthType[] }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Authentication type list, which specifies the types of authentication provided on the user authentication page.
     *
     * @type { UserAuthType[] }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    authType: UserAuthType[];

    /**
     * Trust level of authentication result.
     *
     * @type { AuthTrustLevel }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Authentication trust level. For details, see Principles for Classifying Biometric Authentication Trust Levels.
     *
     * @type { AuthTrustLevel }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    authTrustLevel: AuthTrustLevel;

    /**
     * Information about the authentication result reuse.
     *
     * @type { ?ReuseUnlockResult }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    reuseUnlockResult?: ReuseUnlockResult;

    /**
     * ID of the user to be authenticated.
     *
     * @type { ?number }
     * @default The ID of the current user. The value is a positive integer greater than or equal to 0.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    userId?: number;
  }

  /**
   * Auth widget parameter.
   *
   * @typedef WidgetParam
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Represents the information presented on the user authentication page.
   *
   * @typedef WidgetParam
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface WidgetParam {
    /**
     * Title of widget.
     *
     * @type { string }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Title of the user authentication page. It cannot exceed 500 characters.
     *
     * @type { string }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    title: string;

    /**
     * The description text of navigation button.
     *
     * @type { ?string }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Text on the navigation button. It cannot exceed 60 characters. It is supported in single fingerprint or facial
     * authentication before API version 18. Since API version 18, it is also supported in combined facial and
     * fingerprint authentication.
     *
     * @type { ?string }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    navigationButtonText?: string;

    /**
     * Display format of the user authentication page.
     *
     * @type { ?WindowModeType }
     * @default WindowModeType.DIALOG_BOX
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    windowMode?: WindowModeType;

    /**
     * Whether to display the authentication dialog box in modal application mode. This mode is applicable only
     * to 2-in-1 devices. If this mode is not used or other types of devices are used, the authentication dialog
     * box is displayed in modal system mode.
     *
     * @type { ?Context }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 18
     */
    uiContext?: Context;
  }

  /**
   * Authentication result: authentication token, credential type for authentication succeed.
   *
   * @typedef UserAuthResult
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Represents the user authentication result. If the authentication is successful, the authentication type and token
   * information are returned.
   *
   * @typedef UserAuthResult
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface UserAuthResult {
    /**
     * The authentication result.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * User authentication result. If the authentication is successful, SUCCESS is returned. Otherwise, an error code
     * is returned. For details, see UserAuthResultCode.
     *
     * @type { number }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    result: number;

    /**
     * The authentication result if the authentication is passed.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Authentication token information.
     *
     * @type { ?Uint8Array }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    token?: Uint8Array;

    /**
     * Credential type for authentication succeed.
     *
     * @type { ?UserAuthType }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Authentication type.
     *
     * @type { ?UserAuthType }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    authType?: UserAuthType;

    /**
     * Credential state.
     *
     * @type { ?EnrolledState }
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    enrolledState?: EnrolledState;
  }

  /**
   * Asynchronous callback of authentication operation.
   *
   * @interface IAuthCallback
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Provides callbacks to return the authentication result.
   *
   * @interface IAuthCallback
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface IAuthCallback {
    /**
     * The authentication result code is returned through the callback.
     * If the authentication is passed, the authentication token is returned in extraInfo.
     *
     * @param { UserAuthResult } result - Authentication result information.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Called to return the authentication result. If the authentication is successful,
     * UserAuthResult contains the token information.
     *
     * @param { UserAuthResult } result - Authentication result information.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    onResult(result: UserAuthResult): void;
  }

  /**
   * User authentication instance, used to initiate a complete authentication.
   *
   * @interface UserAuthInstance
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Provides APIs for user authentication. The user authentication widget is supported. Before using the APIs
   * of UserAuthInstance, you must obtain a UserAuthInstance instance by using getUserAuthInstance.
   *
   * @interface UserAuthInstance
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  interface UserAuthInstance {
    /**
     * Turn on widget authentication result event listening.
     *
     * @param { 'result' } type - Indicates the type of event.
     * @param { IAuthCallback } callback - Indicates the listener.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Subscribes to the user authentication result.
     *
     * @param { 'result' } type - Event type. The value is result, which indicates the authentication result.
     * @param { IAuthCallback } callback - Callback used to return the user authentication result.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    on(type: 'result', callback: IAuthCallback): void;

    /**
     * Turn off widget authentication result event listening.
     *
     * @param { 'result' } type - Indicates the type of event.
     * @param { IAuthCallback } callback - Indicates the listener.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Unsubscribes from the user authentication result.
     *
     * <p><strong>NOTE</strong>:
     * <br>The UserAuthInstance instance used to invoke this API must be the one used to subscribe to the event.
     * </p>
     *
     * @param { 'result' } type - Event type. The value is result, which indicates the authentication result.
     * @param { IAuthCallback } callback - Callback to unregister.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    off(type: 'result', callback?: IAuthCallback): void;

    /**
     * Start this authentication, an instance can only perform authentication once.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Incorrect parameter types.
     * @throws { BusinessError } 12500001 - Authentication failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @throws { BusinessError } 12500003 - Authentication canceled.
     * @throws { BusinessError } 12500004 - Authentication timeout.
     * @throws { BusinessError } 12500005 - The authentication type is not supported.
     * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
     * @throws { BusinessError } 12500007 - Authentication service is busy.
     * @throws { BusinessError } 12500009 - Authentication is locked out.
     * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
     * @throws { BusinessError } 12500011 - Switched to the custom authentication process.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Start this authentication, an instance can only perform authentication once.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Incorrect parameter types.
     * @throws { BusinessError } 12500001 - Authentication failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @throws { BusinessError } 12500003 - Authentication canceled.
     * @throws { BusinessError } 12500004 - Authentication timeout.
     * @throws { BusinessError } 12500005 - The authentication type is not supported.
     * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
     * @throws { BusinessError } 12500007 - Authentication service is busy.
     * @throws { BusinessError } 12500009 - Authentication is locked out.
     * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
     * @throws { BusinessError } 12500011 - Switched to the custom authentication process.
     * @throws { BusinessError } 12500013 - Operation failed because of PIN expired.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    /**
     * Starts authentication.
     *
     * <p><strong>NOTE</strong>:
     * <br>Each UserAuthInstance can be used for authentication only once.
     * <br>Starting from API version 20, only system applications can apply for the
     * ohos.permission.USER_AUTH_FROM_BACKGROUND permission.
     * </p>
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC or ohos.permission.USER_AUTH_FROM_BACKGROUND
     * @throws { BusinessError } 201 - Permission verification failed. Possible causes:
     * <br>1. No permission to access biometric.
     * <br>2. No permission to start authentication from background.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Incorrect parameter types.
     * @throws { BusinessError } 12500001 - Authentication failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @throws { BusinessError } 12500003 - Authentication canceled.
     * @throws { BusinessError } 12500004 - Authentication timeout.
     * @throws { BusinessError } 12500005 - The authentication type is not supported.
     * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
     * @throws { BusinessError } 12500007 - Authentication service is busy.
     * @throws { BusinessError } 12500009 - Authentication is locked out.
     * @throws { BusinessError } 12500010 - The type of credential has not been enrolled.
     * @throws { BusinessError } 12500011 - Switched to the custom authentication process.
     * @throws { BusinessError } 12500013 - Operation failed because of PIN expired.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 20
     */
    start(): void;

    /**
     * Cancel this authentication.
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Incorrect parameter types.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * Cancels this authentication.
     *
     * <p><strong>NOTE</strong>:
     * <br>UserAuthInstance must be the instance being authenticated.
     * </p>
     *
     * @permission ohos.permission.ACCESS_BIOMETRIC
     * @throws { BusinessError } 201 - Permission verification failed.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Incorrect parameter types.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    cancel(): void;
  }

  /**
   * Get user authentication instance with widget.
   *
   * @param { AuthParam } authParam - Auth parameter.
   * @param { WidgetParam } widgetParam - Widget parameter.
   * @returns { UserAuthInstance } Returns an authentication instance with widget.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 10
   */
  /**
   * Obtains a UserAuthInstance instance for user authentication. The user authentication widget is also supported.
   *
   * <p><strong>NOTE</strong>:
   * <br>Each UserAuthInstance can be used for authentication only once.
   * </p>
   *
   * @param { AuthParam } authParam - Auth parameter.
   * @param { WidgetParam } widgetParam - Widget parameter.
   * @returns { UserAuthInstance } Returns an authentication instance with widget.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 12500002 - General operation error.
   * @throws { BusinessError } 12500005 - The authentication type is not supported.
   * @throws { BusinessError } 12500006 - The authentication trust level is not supported.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  function getUserAuthInstance(authParam: AuthParam, widgetParam: WidgetParam): UserAuthInstance;

  /**
   * Defines the type of the user authentication notification.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  enum NoticeType {
    /**
     * Notification from the user authentication widget.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    WIDGET_NOTICE = 1
  }

  /**
   * Sends a notification from the user authentication widget.
   *
   * @permission ohos.permission.SUPPORT_USER_AUTH
   * @param { NoticeType } noticeType - Notification type.
   * @param { string } eventData - Event data. The data cannot exceed 65536 bytes.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * <br>3. Parameter verification failed.
   * @throws { BusinessError } 12500002 - General operation error.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function sendNotice(noticeType: NoticeType, eventData: string): void;

  /**
   * Enum for operation result.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @since 9
   */
  /**
   * Enumerates the authentication result codes.
   *
   * @enum { number }
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @atomicservice
   * @since 12
   */
  enum UserAuthResultCode {
    /**
     * Indicates that the result is success or ability is supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication is successful.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    SUCCESS = 12500000,

    /**
     * Indicates that the authentication result is failed.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication failed.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    FAIL = 12500001,

    /**
     * Indicates other errors.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * A general operation error occurred.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    GENERAL_ERROR = 12500002,

    /**
     * Indicates that this operation is canceled.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication is canceled.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    CANCELED = 12500003,

    /**
     * Indicates that this operation is time-out.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication has timed out.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    TIMEOUT = 12500004,

    /**
     * Indicates that this authentication type is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication type is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    TYPE_NOT_SUPPORT = 12500005,

    /**
     * Indicates that the authentication trust level is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication trust level is not supported.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    TRUST_LEVEL_NOT_SUPPORT = 12500006,

    /**
     * Indicates that the authentication task is busy. Wait for a few seconds and try again.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The system does not respond.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    BUSY = 12500007,

    /**
     * Indicates that the authenticator is locked.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The authentication executor is locked.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    LOCKED = 12500009,

    /**
     * Indicates that the user has not enrolled the authenticator.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 9
     */
    /**
     * The user has not enrolled the specified system identity authentication credential.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    NOT_ENROLLED = 12500010,

    /**
     * Indicates that this operation is canceled from widget's navigation button.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @since 10
     */
    /**
     * The user cancels the system authentication and selects a custom authentication of the application.
     * The caller needs to launch the custom authentication page.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    CANCELED_FROM_WIDGET = 12500011,

    /**
     * The authentication failed because the lock screen password has expired.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @atomicservice
     * @since 12
     */
    PIN_EXPIRED = 12500013,

    /**
     * The AuthToken is invalid.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    AUTH_TOKEN_CHECK_FAILED = 12500015,

    /**
     * The interval between the AuthToken issuance time and the AuthToken verification time exceeds
     * the maximum validity period.
     *
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 18
     */
    AUTH_TOKEN_EXPIRED = 12500016
  }

  /**
   * Provides APIs for managing the user authentication widget. You can use the APIs to register the user
   * authentication widget with UserAuthWidgetMgr for management and scheduling.
   *
   * @interface UserAuthWidgetMgr
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface UserAuthWidgetMgr {
    /**
     * Subscribes to commands from the user authentication framework for the user authentication widget.
     *
     * @param { 'command' } type - Event type. The vlaue is command, which indicates the command sent from the user
     * authentication framework to the user authentication widget.
     * @param { IAuthWidgetCallback } callback - Callback used to return the command from the user authentication
     * framework to the user authentication widget.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    on(type: 'command', callback: IAuthWidgetCallback): void;

    /**
     * Unsubscribes from commands sent from the user authentication framework.
     *
     * @param { 'command' } type - Event type. The value is command, which indicates the command sent from the user
     * authentication framework to the user authentication widget.
     * @param { IAuthWidgetCallback } callback - Callback to unregister.
     * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
     * <br>1. Mandatory parameters are left unspecified.
     * <br>2. Incorrect parameter types.
     * <br>3. Parameter verification failed.
     * @throws { BusinessError } 12500002 - General operation error.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    off(type: 'command', callback?: IAuthWidgetCallback): void;
  }

  /**
   * Obtains a UserAuthWidgetMgr instance for user authentication.
   *
   * <p><strong>NOTE</strong>:
   * <br>A UserAuthInstance instance can be used for an authentication only once.
   * </p>
   *
   * @permission ohos.permission.SUPPORT_USER_AUTH
   * @param { number } version - Version of the user authentication widget.
   * @returns { UserAuthWidgetMgr } UserAuthWidgetMgr instance obtained.
   * @throws { BusinessError } 201 - Permission verification failed.
   * @throws { BusinessError } 202 - The caller is not a system application.
   * @throws { BusinessError } 401 - Incorrect parameters. Possible causes:
   * <br>1. Mandatory parameters are left unspecified.
   * <br>2. Incorrect parameter types.
   * @throws { BusinessError } 12500002 - General operation error.
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  function getUserAuthWidgetMgr(version: number): UserAuthWidgetMgr;

  /**
   * Provides the callback for returning the commands sent from the user authentication framework
   * to the user authentication widget.
   *
   * @interface IAuthWidgetCallback
   * @syscap SystemCapability.UserIAM.UserAuth.Core
   * @systemapi Hide this for inner system use.
   * @since 10
   */
  interface IAuthWidgetCallback {
    /**
     * Called to return the command sent from the user authentication framework to the user authentication widget.
     *
     * @param { string } cmdData - Command sent from the user authentication framework to the user
     * authentication widget.
     * @syscap SystemCapability.UserIAM.UserAuth.Core
     * @systemapi Hide this for inner system use.
     * @since 10
     */
    sendCommand(cmdData: string): void;
  }
}

export default userAuth;
