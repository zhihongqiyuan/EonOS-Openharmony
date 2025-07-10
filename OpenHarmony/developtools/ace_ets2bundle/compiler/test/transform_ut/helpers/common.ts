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
export const SDK_VERSION: number = 13;
export const ENTRY_PACKAGE_NAME_DEFAULT: string = 'entry';
export const ENTRY_MODULE_NAME_DEFAULT: string = 'entry';
export const PACKAGE_DIR_DEFAULT: string = 'oh_modules';
export const PACKAGE_JSON_DEFAULT: string = 'oh-package.json5';
export const PAGES_JSON_FILE_NAME_DEFAULT: string = 'main_pages.json';
export const PANDA_MODE_DEFAULT: string = 'es2abc';
export const MIN_API_VERSION_DEAFAULT: number = 50001013;

// irrelevant configs to prevent application from crashing
export const SDK_VERSION_STAGE: string = 'Canary1';
export const BUNDLE_NAME_DEFAULT: string = 'com.example.application';
export const ETS_LOADER_VERSION: string = '5.0.1.100';
export const OHPM: string = 'ohpm';
export const DEBUG: string = 'Debug';
export const RELEASE: string = 'release';
export const ENTRY_MODULE_VERSION_DEFAULT: string = '1.0.0';
export const MODULE_NAME_HASH_DEFAULT: string = '1043bfc77febe75fafec0c4309faccf1';
export const RESOURCE_TABLE_HASH_DEFAULT: string = '790527e39c8c2be7fbbc762f7966104e';
export const RUNTIME_OS_OPENHARMONY: string = 'HIDDEN';
export const DEVICE_TYPE: string = 'phone,tablet,2in1,wearable';
export const NODE_JS_PATH: string = '/usr/local/nodejs';
export const PORT_DEFAULT: string = '29900';
export const IS_CACHE_INVALID: string = 'is_cache_invalid';
export const ARK_COMPILER_META_INFO: string = 'ark_compiler_meta_info';
export const STRICT_MODE_DEFAULT: object = {
  caseSensitiveCheck: true,
	useNormalizedOHMUrl: true,
	noExternalImportByPath: true,
}
export const DEVICE_TYPES_DEFAULT = [
	"phone",
	"tablet",
	"2in1",
	"wearable"
]
export const PKG_JSON_FILE_HASH_DEFAULT: string = '14b2b93515e45a4ff785bb14c7b2aa25';
export const BUILD_ID_DEFAULT: string = '202410091727251800';
export const ARKUI_SUBSYSTEM_CODE: string = '109';
export const TSC_SYSTEM_CODE: string = '105';
export const ARKUI_ERROR_CODE_IN_TS_DIAGNOSTIC: string[] = [
	'10905128', '10905239', '10905238', '10905127', '10905353', '10905237', '10905236'
];