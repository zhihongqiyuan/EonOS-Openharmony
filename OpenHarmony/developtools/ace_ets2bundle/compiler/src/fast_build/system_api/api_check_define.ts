/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

export const PERMISSION_TAG_CHECK_NAME: string = 'permission';
export const PERMISSION_TAG_CHECK_ERROR: string = "To use this API, you need to apply for the permissions: $DT";
export const SYSTEM_API_TAG_CHECK_NAME: string = 'systemapi';
export const SYSTEM_API_TAG_CHECK_WARNING: string = "'{0}' is system api";
export const TEST_TAG_CHECK_NAME: string = 'test';
export const TEST_TAG_CHECK_ERROR: string = "'{0}' can only be used for testing directories ";
export const SYSCAP_TAG_CHECK_NAME: string = 'syscap';
export const SYSCAP_TAG_CHECK_WARNING: string = "The system capacity of this api '{0}' is not supported on all devices";
export const SYSCAP_TAG_CONDITION_CHECK_WARNING: string = 'The API is not supported on all devices. Use the canIUse condition to determine whether the API is supported.';
export const CANIUSE_FUNCTION_NAME: string = 'canIUse';
export const VERSION_CHECK_FUNCTION_NAME: string = 'isApiVersionGreaterOrEqual';
export const RUNTIME_OS_OH: string = 'OpenHarmony';
export const FORM_TAG_CHECK_NAME: string = 'form';
export const FORM_TAG_CHECK_ERROR: string = "'{0}' can't support form application.";
export const CROSSPLATFORM_TAG_CHECK_NAME: string = 'crossplatform';
export const CROSSPLATFORM_TAG_CHECK_ERROER: string = "'{0}' can't support crossplatform application.";
export const DEPRECATED_TAG_CHECK_NAME: string = 'deprecated';
export const DEPRECATED_TAG_CHECK_WARNING: string = "'{0}' has been deprecated.";
export const FA_TAG_CHECK_NAME: string = 'famodelonly';
export const FA_TAG_HUMP_CHECK_NAME: string = 'FAModelOnly';
export const FA_TAG_CHECK_ERROR: string = 'This API is used only in FA Mode, but the current Mode is Stage.';
export const STAGE_TAG_CHECK_NAME: string = 'stagemodelonly';
export const STAGE_TAG_HUMP_CHECK_NAME: string = 'StageModelOnly';
export const STAGE_TAG_CHECK_ERROR: string = 'This API is used only in Stage Mode, but the current Mode is FA.';
export const STAGE_COMPILE_MODE: string = 'moduleJson';
export const ATOMICSERVICE_BUNDLE_TYPE: string = 'atomicService';
export const ATOMICSERVICE_TAG_CHECK_NAME: string = 'atomicservice';
export const ATOMICSERVICE_TAG_CHECK_ERROER: string = "'{0}' can't support atomicservice application.";
export const SINCE_TAG_NAME: string = 'since';
export const SINCE_TAG_CHECK_ERROER: string = "The '{0}' API is supported since SDK version $SINCE1. However, the current compatible SDK version is $SINCE2.";
export const ATOMICSERVICE_TAG_CHECK_VERSION: number = 11;
export const FIND_MODULE_WARNING: string = "Cannot find name '{0}'.";

export const CONSTANT_STEP_0: number = 0;
export const CONSTANT_STEP_1: number = 1;
export const CONSTANT_STEP_2: number = 2;
export const CONSTANT_STEP_3: number = 3;

export const GLOBAL_DECLARE_WHITE_LIST: Set<string> = new Set(['Context', 'PointerStyle', 'PixelMap', 'UnifiedData',
  'Summary', 'UniformDataType', 'IntentionCode', 'NavDestinationInfo', 'UIContext', 'Resource', 'WebviewController']);
