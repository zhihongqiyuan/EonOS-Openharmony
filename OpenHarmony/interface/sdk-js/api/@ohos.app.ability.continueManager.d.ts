/*
 * Copyright (c) 2025-2025 Huawei Device Co., Ltd.
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
 
import { AsyncCallback } from './@ohos.base';

/**
 * Providers methods for interacting with continue feature.
 * 
 * @namespace continueManager
 * @syscap SystemCapability.Ability.AbilityRuntime.Mission
 * @since 18
 */
declare namespace continueManager {
    /**
     * Register prepareContinue event, when the ability is configured with 'ContinueQuickStart' in the continueType, then can get the
     * result of LaunchReason.PREPARE_CONTINUATION.
     * 
     * @param { 'prepareContinue' } type - Registration Type, 'prepareContinue'.
     * @param { Context } context - the ability context.
     * @param { AsyncCallback<ContinueResultInfo> } callback - Used to handle ('prepareContinue') command.
     * @throws { BusinessError } 16300501 - the system ability work abnormally.
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @since 18
     */
    function on(type: 'prepareContinue', context: Context, callback: AsyncCallback<ContinueResultInfo>): void;

    /**
     * Unregister prepareContinue event.
     * 
     * @param { 'prepareContinue' } type - Registration Type, 'prepareContinue'.
     * @param { Context } context - the ability context.
     * @param { AsyncCallback<ContinueResultInfo> } callback - Used to handle ('prepareContinue') command.
     * @throws { BusinessError } 16300501 - the system ability work abnormally.
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @since 18
     */
    function off(type: 'prepareContinue', context: Context, callback?: AsyncCallback<ContinueResultInfo>): void;

    /**
     * Continue result info.
     * @interface ContinueEventInfo
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @since 18
     */
    interface ContinueResultInfo {
        /**
         * Continue state code.
         * @type { ContinueStateCode }
         * @syscap SystemCapability.Ability.AbilityRuntime.Mission
         * @since 18
         */
        resultState: ContinueStateCode;

        /**
         * Result info.
         * @type { ?string }
         * @syscap SystemCapability.Ability.AbilityRuntime.Mission
         * @since 18
         */
        resultInfo?: string;
    }

    /**
     * Continue state code.
     * @enum { number }
     * @syscap SystemCapability.Ability.AbilityRuntime.Mission
     * @since 18
     */
    enum ContinueStateCode {
        /**
         * Continue success
         * @syscap SystemCapability.Ability.AbilityRuntime.Mission 
         * @since 18
         */
        SUCCESS = 0,

        /**
         * System error
         * @syscap SystemCapability.Ability.AbilityRuntime.Mission
         * @since 18
         */
        SYSTEM_ERROR,
    }
}
export default continueManager;