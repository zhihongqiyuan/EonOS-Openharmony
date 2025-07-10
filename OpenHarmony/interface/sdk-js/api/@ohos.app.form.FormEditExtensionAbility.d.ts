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
 * @kit FormKit
 */
import UIExtensionAbility from './@ohos.app.ability.UIExtensionAbility';
import FormEditExtensionContext from './application/FormEditExtensionContext';

/**
 * The class of form edit extension ability.
 *
 * @extends UIExtensionAbility
 * @syscap SystemCapability.Ability.Form
 * @stagemodelonly
 * @since 18
 */
export default class FormEditExtensionAbility extends UIExtensionAbility {
/**
   * Indicates configuration infomation about an form edit extension ability context.
   *
   * @type { FormEditExtensionContext }
   * @syscap SystemCapability.Ability.Form
   * @stagemodelonly
   * @since 18
   */
	context: FormEditExtensionContext;
}
