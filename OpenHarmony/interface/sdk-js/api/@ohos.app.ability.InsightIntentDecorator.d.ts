/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
 * @kit AbilityKit
 */

import insightIntent from './@ohos.app.ability.insightIntent';

/**
 * Declare interface of IntentDecoratorInfo.
 *
 * @interface IntentDecoratorInfo
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface IntentDecoratorInfo {
  /**
   * The intent name.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  intentName: string;

  /**
   * The intent domain.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  domain: string;

  /**
   * The intent version.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  intentVersion: string;

  /**
   * The display name of intent.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  displayName: string;

  /**
   * The display description of intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  displayDescription?: string;

  /**
   * The schema of intent, indicates a standard intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  schema?: string;

  /**
   * The icon of intent, the string type indicates an online resource, and the Resource type indicates a local resource.
   * The value of Resource type must be a literal.
   *
   * @type { ?ResourceStr }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  icon?: ResourceStr;

  /**
   * The large language model description of intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  llmDescription?: string;

  /**
   * The search keywords of intent.
   *
   * @type { ?string[] }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  keywords?: string[];

  /**
   * The parameters of intent.
   *
   * @type { ?Record<string, Object> }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  parameters?: Record<string, Object>;

  /**
   * The type definition of the result returned by intent call.
   *
   * @type { ?Record<string, Object> }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  result?: Record<string, Object>;

  /**
   * The example of the intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  example?: string;
}

/**
 * Declare interface of LinkIntentDecoratorInfo.
 *
 * @extends IntentDecoratorInfo
 * @interface LinkIntentDecoratorInfo
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface LinkIntentDecoratorInfo extends IntentDecoratorInfo {
  /**
   * The uri of a link.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  uri: string;

  /**
   * The parameters mapping of a link.
   *
   * @type { ?LinkIntentParamMapping[] }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  paramMappings?: LinkIntentParamMapping[];
}

/**
 * Enum definition of the paramCategory {@link #LinkIntentParamMapping#paramCategory},
 * paramCategory is an attribute of LinkIntentParamMapping and
 * used in InsightIntentLink {@link #InsightIntentLink}.
 *
 * @enum { string }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare enum LinkParamCategory {
  /**
   * The parameter will added to the end of link uri.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  LINK = 'link',

  /**
   * The parameter will transferred to the application as parameters of want.
   *
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  WANT = 'want',
}

/**
 * Declare interface of LinkIntentParamMapping.
 *
 * @interface LinkIntentParamMapping
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface LinkIntentParamMapping {
  /**
   * The parameter name.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  paramName: string;

  /**
   * The parameter mapping name.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  paramMappingName?: string;

  /**
   * The parameter category.
   *
   * @type { ?LinkParamCategory }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  paramCategory?: LinkParamCategory;
}

/**
 * Define InsightIntentLink.
 *
 * @type { ((intentInfo: LinkIntentDecoratorInfo) => ClassDecorator) }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
export declare const InsightIntentLink: ((intentInfo: LinkIntentDecoratorInfo) => ClassDecorator);

/**
 * Declare interface of PageIntentDecoratorInfo.
 *
 * @extends IntentDecoratorInfo
 * @interface PageIntentDecoratorInfo
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface PageIntentDecoratorInfo extends IntentDecoratorInfo {
  /**
   * The uiability name bound to the intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  uiAbility?: string;

  /**
   * The page path bound to the intent.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  pagePath: string;

  /**
   * The navigation Id bound to the intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  navigationId?: string;

  /**
   * The navigation destination name bound to the intent.
   *
   * @type { ?string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  navDestinationName?: string;
}

/**
 * Define InsightIntentPage.
 *
 * @type { ((intentInfo: PageIntentDecoratorInfo) => ClassDecorator) }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
export declare const InsightIntentPage: ((intentInfo: PageIntentDecoratorInfo) => ClassDecorator);

/**
 * Declare interface of FunctionIntentDecoratorInfo.
 *
 * @extends IntentDecoratorInfo
 * @interface FunctionIntentDecoratorInfo
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface FunctionIntentDecoratorInfo extends IntentDecoratorInfo {}

/**
 * Define InsightIntentFunctionMethod.
 *
 * @type { ((intentInfo: FunctionIntentDecoratorInfo) => MethodDecorator) }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
export declare const InsightIntentFunctionMethod: ((intentInfo: FunctionIntentDecoratorInfo) => MethodDecorator);

/**
 * Define InsightIntentFunction.
 *
 * @type { (() => ClassDecorator) }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
export declare const InsightIntentFunction: (() => ClassDecorator);

/**
 * Declare interface of EntryIntentDecoratorInfo.
 *
 * @extends IntentDecoratorInfo
 * @interface EntryIntentDecoratorInfo
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
declare interface EntryIntentDecoratorInfo extends IntentDecoratorInfo {
  /**
   * The ability name bound to the intent.
   *
   * @type { string }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  abilityName: string;

  /**
   * The execute mode of the intent.
   * For UIAbility, the parameter can be set to insightIntent.ExecuteMode.UI_ABILITY_FOREGROUND or
   * insightIntent.ExecuteMode.UI_ABILITY_BACKGROUND or both of them.
   *
   * @type { insightIntent.ExecuteMode[] }
   * @syscap SystemCapability.Ability.AbilityRuntime.Core
   * @stagemodelonly
   * @atomicservice
   * @since 20
   */
  executeMode: insightIntent.ExecuteMode[];
}

/**
 * Define InsightIntentEntry.
 *
 * @type { ((intentInfo: EntryIntentDecoratorInfo) => ClassDecorator) }
 * @syscap SystemCapability.Ability.AbilityRuntime.Core
 * @stagemodelonly
 * @atomicservice
 * @since 20
 */
export declare const InsightIntentEntry: ((intentInfo: EntryIntentDecoratorInfo) => ClassDecorator);
