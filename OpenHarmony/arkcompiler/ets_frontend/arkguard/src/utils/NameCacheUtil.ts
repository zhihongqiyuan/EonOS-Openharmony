/*
 * Copyright (c) 2023-2024 Huawei Device Co., Ltd.
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

import {FileUtils} from './FileUtils';

export const NAME_CACHE_SUFFIX: string = '.cache.json';
export const PROPERTY_CACHE_FILE: string = 'property.cache.json';
export const IDENTIFIER_CACHE: string = 'IdentifierCache';
export const MEM_METHOD_CACHE: string = 'MemberMethodCache';
export const spaceOfNameCache: number = 2;

export function writeCache(cache: Map<string, string> | undefined, destFileName: string): void {
  // convert map to json string
  if (!cache) {
    return;
  }

  const cacheString: string = JSON.stringify(Object.fromEntries(cache), null, spaceOfNameCache);
  FileUtils.writeFile(destFileName, cacheString);
}

export function readCache(filePath: string): Object | undefined {
  // read json string from file
  const cacheString: string = FileUtils.readFile(filePath);
  if (cacheString === undefined) {
    return undefined;
  }

  // get map from json string
  return JSON.parse(cacheString);
}

export function getMapFromJson(jsonObj: Object | undefined): Map<string, string> {
  if (jsonObj === undefined) {
    return new Map<string, string>();
  }

  return new Map<string, string>(Object.entries(jsonObj));
}

export function deleteLineInfoForNameString(historyNameCache: Map<string, string>, identifierCache: Object | undefined): void {
  if (identifierCache) {
    for (const [key, value] of Object.entries(identifierCache)) {
      let newKey = key.includes(':') ? key.split(':')[0] : key;
      historyNameCache.set(newKey, value as string);
    }
  }
}

// The original name of the member method is recorded during the identifier obfuscation.
// After the property obfuscation, it needs to be updated to the mangled name.
export function UpdateMemberMethodName(nameCache: Map<string, string | Map<string, string>>, globalMangledTable: Map<string, string>,
  classInfoInMemberMethodCache: Set<string>): void {
  let memberMethodCache: Map<string, string> = nameCache.get(MEM_METHOD_CACHE) as Map<string, string>;
  if (!memberMethodCache) {
    return;
  }
  // the valueName is the orignal name of member method.
  for (const [key, valueName] of memberMethodCache.entries()) {
    // It is used to prevent the class name from being updated incorrectly, since the obfuscated class name
    // is recorded during identifier obfuscation.
    if (classInfoInMemberMethodCache.has(key)) {
      continue;
    }
    const mangledName: string = globalMangledTable.get(valueName);
    if (mangledName) {
      memberMethodCache.set(key, mangledName);
    }
  }
}