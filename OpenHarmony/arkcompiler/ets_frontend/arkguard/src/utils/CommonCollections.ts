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

import { KeepInfo } from './ProjectCollections';

// This records the collections related to property obfuscation.
export namespace PropCollections {
  // global mangled properties table used by all files in a project
  export let globalMangledTable: Map<string, string> = new Map();
  // used for property cache
  export let historyMangledTable: Map<string, string> = undefined;
  // the white list of property
  export let reservedProperties: Set<string> = new Set();
  export let universalReservedProperties: RegExp[] = [];
  // In incremental compilation, save the globally obfuscated names generated during the last compilation.
  // When generating new variable or property obfuscated names,
  // it is necessary to prevent duplication with the names in this set.
  export let globalMangledNamesInCache: Set<string> = new Set();

  // When the module is compiled, call this function to clear the collections associated with property obfuscation.
  export function clearPropsCollections(): void {
    globalMangledTable.clear();
    historyMangledTable?.clear();
    reservedProperties.clear();
    universalReservedProperties = [];
    globalMangledNamesInCache.clear();
  }
}

// This records the collections related to whitelists
export namespace UnobfuscationCollections {
  // whitelist
  export let reservedSdkApiForProp: Set<string> = new Set();
  export let reservedSdkApiForGlobal: Set<string> = new Set();
  export let reservedSdkApiForLocal: Set<string> = new Set();
  export let reservedStruct: Set<string> = new Set();
  export let reservedLangForProperty: Set<string> = new Set();
  // declare global {}
  // In the above syntax, 'global' is named '__global' in tsc.
  export let reservedLangForTopLevel: Set<string> = new Set(['__global', 'default']); // Will not add new elements anymore
  export let reservedExportName: Set<string> = new Set();
  export let reservedExportNameAndProp: Set<string> = new Set();
  export let reservedStrProp: Set<string> = new Set();
  export let reservedEnum: Set<string> = new Set();

  // The mapping between the unobfuscated names and their reasons.
  export let unobfuscatedPropMap: Map<string, Set<string>> = new Map();
  export let unobfuscatedNamesMap: Map<string, Set<string>> = new Map();

  // The mapping between wildcards and regular expressions
  export let reservedWildcardMap: Map<RegExp, string> = new Map();

  export function clear(): void {
    reservedSdkApiForProp.clear();
    reservedSdkApiForGlobal.clear();
    reservedSdkApiForLocal.clear();
    reservedStruct.clear();
    reservedLangForProperty.clear();
    reservedExportName.clear();
    reservedExportNameAndProp.clear();
    reservedStrProp.clear();
    reservedEnum.clear();
    unobfuscatedPropMap.clear();
    unobfuscatedNamesMap.clear();
    reservedWildcardMap.clear();
  }
}

export namespace LocalVariableCollections {
  export let reservedConfig: Set<string> = new Set(); // Obtain the name from the user-configured .d.ts file
  export let reservedLangForLocal: Set<string> = new Set(['this', '__global']); // Will not add new elements anymore

  export function clear(): void {
    reservedConfig.clear();
  }
}

export namespace AtKeepCollections {
  export let keepSymbol: KeepInfo = {
    propertyNames: new Set<string>(),
    globalNames: new Set<string>()
  };

  export let keepAsConsumer: KeepInfo = {
    propertyNames: new Set<string>(),
    globalNames: new Set<string>()
  };

  export function clear(): void {
    keepSymbol.globalNames.clear();
    keepSymbol.propertyNames.clear();
    keepAsConsumer.globalNames.clear();
    keepAsConsumer.propertyNames.clear();
  };
}

export namespace AtIntentCollections{
  export let propertyNames: Set<string> = new Set();
  export let globalNames: Set<string> = new Set();
  
  export function clear(): void{
    propertyNames.clear();
    globalNames.clear();
  }
}

export namespace BytecodeObfuscationCollections {
  export let decoratorProp: Set<string> = new Set();
  
  export function clear(): void {
    decoratorProp.clear();
  }
}