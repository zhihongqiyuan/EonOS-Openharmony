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

export const ENTRY_TXT: string = 'entry.txt';
export const FILESINFO_TXT: string = 'filesInfo.txt';
export const FILESINFO: string = 'filesInfo';
export const NPMENTRIES_TXT: string = 'npmEntries.txt';
export const MODULES_CACHE: string = 'modules.cache';
export const MODULES_ABC: string = 'modules.abc';
export const WIDGETS_ABC: string = 'widgets.abc';
export const MODULELIST_JSON: string = 'moduleList.json';
export const PREBUILDMODE_JSON: string = 'preBuildMode.json';
export const SOURCEMAPS_JSON: string = 'sourceMaps.json';
export const SOURCEMAPS: string = 'sourceMaps.map';
export const SYMBOLMAP: string = 'symbolMap.map';
export const PROTO_FILESINFO_TXT: string = 'protoFilesInfo.txt';
export const AOT_FULL: string = 'full';
export const AOT_TYPE: string = 'type';
export const AOT_PARTIAL: string = 'partial';
export const AOT_PROFILE_SUFFIX: string = '.ap';
export const NPM_ENTRIES_PROTO_BIN: string = 'npm_entries.protoBin';
export const PACKAGE_JSON: string = 'package.json';
export const FAKE_JS: string = 'fake.js';
export const COMPILE_CONTEXT_INFO_JSON: string = 'compileContextInfo.json';
export const PERFREPORT_JSON: string = 'perfReport.json';

export const ESMODULE: string = 'esmodule';
export const JSBUNDLE: string = 'jsbundle';
export const ARK: string = 'ark';
export const TEMPORARY: string = 'temporary';
export const MAIN: string = 'main';
export const AUXILIARY: string = 'auxiliary';
export const HAP_PACKAGE: string = '0';
export const PROJECT_PACKAGE: string = '1';
export const EXTNAME_ETS: string = '.ets';
export const EXTNAME_D_ETS: string = '.d.ets';
export const EXTNAME_JS: string = '.js';
export const EXTNAME_TS: string = '.ts';
export const EXTNAME_JS_MAP: string = '.js.map';
export const EXTNAME_TS_MAP: string = '.ts.map';
export const EXTNAME_MJS: string = '.mjs';
export const EXTNAME_CJS: string = '.cjs';
export const EXTNAME_D_TS: string = '.d.ts';
export const EXTNAME_ABC: string = '.abc';
export const EXTNAME_JSON: string = '.json';
export const EXTNAME_PROTO_BIN: string = '.protoBin';
export const PATCH_SYMBOL_TABLE: string = 'symbol.txt';
export const TEMP_JS: string = '.temp.js';
export const HASH_FILE_NAME: string = 'gen_hash.json';
export const EXTNAME_TXT: string = '.txt';
export const PROTOS: string = 'protos';

export const TS2ABC: string = 'ts2abc';
export const ES2ABC: string = 'es2abc';

export const JS: string = 'js';
export const TS: string = 'ts';
export const ETS: string = 'ets';

export const MAX_WORKER_NUMBER: number = 3;

export const GEN_ABC_SCRIPT: string = 'gen_abc.js';

export const NODE_MODULES: string = 'node_modules';
export const OH_MODULES: string = 'oh_modules';
export const PACKAGES: string = 'pkg_modules';
export const OHPM: string = 'ohpm';

export const TS_NOCHECK: string = '// @ts-nocheck';

export const WINDOWS: string = 'Windows_NT';
export const LINUX: string = 'Linux';
export const MAC: string = 'Darwin';

export const COMMONJS: string = 'commonjs';
export const ESM: string = 'esm';
export const SCRIPT: string = 'script';

export const SRC_MAIN: string = 'src/main';
export const GEN_ABC_PLUGIN_NAME: string = 'Gen_Abc_Plugin';
export const OBFUSCATION_TOOL: string = 'Obfuscation_Tool';
export const BYTECODE_OBFUSCATOR_INIT = 'Bytecode_obfuscator_init';

export const SUCCESS: number = 0;
export const FAIL: number = 1;

export const red: string = '\u001b[31m';
export const yellow: string = '\u001b[33m';
export const blue: string = '\u001b[34m';
export const reset: string = '\u001b[39m';

export const DEBUG: string = 'debug';
export const RELEASE: string = 'release';

export const TRUE: string = 'true';
export const FALSE: string = 'false';

export const IS_CACHE_INVALID: string = 'is_cache_invalid';
export const ARK_COMPILER_META_INFO: string = 'ark_compiler_meta_info';

// The following strings are used to specify 'ISendable' interface.
// 'ISendable' interface is in the 'lang' namespace of '@arkts.lang.d.ets' file.
export const ARKTS_LANG_D_ETS = '@arkts.lang.d.ets';
export const LANG_NAMESPACE = 'lang';
export const ISENDABLE_TYPE = 'ISendable';

export const USE_SHARED: string = 'use shared';
export const USE_SHARED_COMMENT: string = '// "use shared"';

export const SEPARATOR_BITWISE_AND: string = '&';
export const SEPARATOR_AT: string = '@';
export const SEPARATOR_SLASH: string = '/';

export const ES_ANNOTATIONS = [
  '_ESConcurrentModuleRequestsAnnotation',
  '_ESSlotNumberAnnotation',
  '_ESAnnotation'
];

export const DECORATOR_WHITE_LIST = [
  'Monitor',
  'Track',
  'Trace',
];

export const BYTECODE_OBFUSCATION_PROPERTY_WHITE_LIST = [
  'componentClass',
  'getReuseId',
  'resetStateVarsOnReuse'
];

export const IDENTIFIER_CACHE = 'IdentifierCache';
export const MEMBER_METHOD_CACHE = 'MemberMethodCache';