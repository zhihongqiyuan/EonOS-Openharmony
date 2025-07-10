/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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

/**
 * Want is the basic communication component of the system.
 *
 * @syscap SystemCapability.Ability.AbilityBase
 * @since 9
 */
/**
 * Want is the basic communication component of the system.
 *
 * @syscap SystemCapability.Ability.AbilityBase
 * @crossplatform
 * @since 10
 */
/**
 * Want is a carrier for information transfer between objects (application components).
 * Want can be used as a parameter of startAbility to specify a startup target and information that needs to be carried
 * during startup, for example, bundleName and abilityName, which respectively indicate the bundle name of the target
 * ability and the ability name in the bundle.
 * When UIAbilityA needs to start UIAbilityB and transfer some data to UIAbilityB, it can use Want a carrier to
 * transfer the data.
 *
 * @syscap SystemCapability.Ability.AbilityBase
 * @crossplatform
 * @atomicservice
 * @since 11
 */
export default class Want {
  /**
   * bundle name
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * bundle name
   *
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @since 10
   */
  /**
   * Bundle name of the ability.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  bundleName?: string;

  /**
   * ability name
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * ability name
   *
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @since 10
   */
  /**
   * Name of the ability.
   * If both bundleName and abilityName are specified in a Want object, the Want object can match a specific ability.
   * 
   * <p>**NOTE**:
   * <br>The value of abilityName must be unique in an application.
   * </p>
   * 
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  abilityName?: string;

  /**
   * device id
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * ID of the device running the ability.
   * 
   * <p>**NOTE**:
   * <br>If this field is unspecified, the local device is used.
   * </p>
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  deviceId?: string;

  /**
   * The description of a URI in a Want.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * Data carried.
   * This field is used together with type to specify the data type.
   * If uri is specified in a Want, the Want will match the specified URI information, including scheme,
   * schemeSpecificPart, authority, and path.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  uri?: string;

  /**
   * The description of the type in this Want.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * The description of the type in this Want.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  /**
   * MIME type, that is, the type of the file to open, for example, 'text/xml' and 'image/*'.
   * 
   * <p>**NOTE**:
   * <br>For details about the MIME type definition,
   * see {@link https://www.iana.org/assignments/media-types/media-types.xhtml?utm_source=ld246.com}.
   * </p>
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @atomicservice
   * @since 18
   */
  type?: string;

  /**
   * The options of the flags in this Want.
   *
   * @type { ?number }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * How the Want object will be handled. By default, a number is passed in.
   * For example, wantConstant.Flags.FLAG_ABILITY_CONTINUATION specifies whether to start the ability in cross-device
   * migration scenarios.
   *
   * @type { ?number }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  flags?: number;

  /**
   * The description of an action in an want.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * Action to take, such as viewing and sharing application details.
   * In implicit Want, you can define this field and use it together with uri or parameters to specify the operation
   * to be performed on the data.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  action?: string;

  /**
   * The description of the WantParams object in an Want
   *
   * @type { ?object }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * The description of the WantParams object in an Want
   *
   * @type { ?object }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @since 10
   */
  /**
   * List of parameters in the Want object.
   * 
   * <p>**NOTE**:
   * <br>1. The values of the following keys are assigned by the system. Manual settings do not take effect, since the
   * system automatically changes the values to the actual values during data transfer.
   * -ohos.aafwk.param.callerPid: PID of the caller. The value is a string.
   * -ohos.aafwk.param.callerBundleName: bundle name of the caller. The value is a string.
   * -ohos.aafwk.param.callerAbilityName: ability name of the caller. The value is a string.
   * -ohos.aafwk.param.callerNativeName: process name of the caller when the native method is called. The value is
   * a string.
   * -ohos.aafwk.param.callerAppId: appId of the caller. The value is a string.
   * -ohos.aafwk.param.callerAppIdentifier: appIdentifier of the caller. The value is a string.
   * -ohos.aafwk.param.callerToken: token of the caller. The value is a string.
   * -ohos.aafwk.param.callerUid: UID in BundleInfo, that is, the application's UID in the bundle information. The
   * value is a number.
   * -ohos.param.callerAppCloneIndex: clone index of the caller. The value is of the numeric type.
   * -component.startup.newRules: enabled status of the new control rule. The value is of the Boolean type.
   * -moduleName: module name of the caller. The value is a string.
   * -ability.params.backToOtherMissionStack: support for redirection back across mission stacks. The value is of
   * the Boolean type.
   * -ohos.ability.params.abilityRecoveryRestart: support for ability restart upon fault recovery. The value is of
   * the Boolean type.
   * -ohos.extra.param.key.contentTitle: title that can be shared by the atomic service. The value is a string.
   * -ohos.extra.param.key.shareAbstract: content that can be shared by the atomic service. The value is a string.
   * -ohos.extra.param.key.shareUrl: URL of the content that can be shared by the atomic service. The value is
   * a string.
   * -ohos.extra.param.key.supportContinuePageStack: support for migration of page stack information during
   * cross-device migration. The value is of the Boolean type. The default value is true, indicating that page stack
   * information is automatically migrated.
   * -ohos.extra.param.key.supportContinueSourceExit: support for application exit on the source device during
   * cross-device migration. The value is of the Boolean type. The default value is true, indicating that the
   * application on the source device automatically exits.
   * -ohos.extra.param.key.showMode: mode to show the atomic service startup. The value is an enumerated value of
   * wantConstant.ShowMode.
   * -ohos.dlp.params.sandbox: available only for DLP files. This key is involved only in system applications.
   * -ohos.dlp.params.bundleName: bundle name of DLP. The value is a string. This key is involved only in system
   * applications.
   * -ohos.dlp.params.moduleName: module name of DLP. The value is a string. This key is involved only in system
   * applications.
   * -ohos.dlp.params.abilityName: ability name of DLP. The value is a string. This key is involved only in system
   * applications.
   * -ohos.dlp.params.index: DLP index. The value is a number. This key is involved only in system applications.
   * -ohos.ability.params.asssertFaultSessionId: session ID of the fault assertion. The value is a string. This key
   * is involved only in system applications.
   * 
   * <br>2. The following keys are defined by the system, and their values need to be manually assigned.
   * -ability.params.stream: File URIs to be authorized to the target ability. The value is a file URI array of the
   * string type.
   * -ohos.extra.param.key.appCloneIndex: index of the application clone.
   * 
   * <br>3. In addition to the foregoing cases, applications may further agree on the key-value pairs to transfer.
   * </p>
   * 
   * <p>**NOTE**:
   * <br>For details about the constants of Params in want, see wantConstant.
   * <br>Note that a maximum of 200 KB data that can be transferred by using WantParams. If the data volume exceeds
   * 200 KB, transfer data in WriteRawDataBuffer or uri mode.
   * <br>The values of parameters must be of the following basic data types: String, Number, Boolean, Object,
   * undefined, and null. Functions in an object cannot be transferred.
   * </p>
   * 
   * @type { ?Record<string, Object> }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  parameters?: Record<string, Object>;

  /**
   * The description of a entities in a Want.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * Additional category information (such as browser and video player) of the ability.
   * It is a supplement to the action field for implicit Want. and is used to filter ability types.
   *
   * @type { ?Array<string> }
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 11
   */
  entities?: Array<string>;

  /**
   * The description of an module name in an want.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @since 9
   */
  /**
   * The description of an module name in an want.
   *
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @since 10
   */
  /**
   * Name of the module to which the ability belongs.
   * 
   * <p>**NOTE**:
   * <br>If the ability belongs to a HAR module, moduleName must be set to the name of the HAP or HSP module that
   * depends on this HAR.
   * </p>
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityBase
   * @crossplatform
   * @atomicservice
   * @since 11
   */
  moduleName?: string;

  /**
   * Want file descriptor (FD), which is used to identify the Want file opened.
   * You can obtain the FD from fs.open. When the FD is no longer needed, you must call fs.close to destroy the FD in a
   * timely manner to prevent FD leakage.
   *
   * @type { ?Record<string, number> }
   * @readonly
   * @syscap SystemCapability.Ability.AbilityBase
   * @atomicservice
   * @since 15
   */
  readonly fds?: Record<string, number>;
}
