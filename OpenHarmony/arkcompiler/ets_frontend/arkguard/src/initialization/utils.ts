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

import fs from 'fs';

import { DEBUG } from './CommonObject';

export function isDebug(projectConfig: any): boolean {
  return projectConfig.buildMode.toLowerCase() === DEBUG;
}

export function isFileExist(filePath: string): boolean {
  let exist = true;
  try {
    fs.accessSync(filePath, fs.constants.F_OK);
  } catch (err) {
    exist = !err;
  }
  return exist;
}

export function sortAndDeduplicateStringArr(arr: string[]) : string[] {
  if (arr.length === 0) {
    return arr;
  }

  arr.sort((a, b) => {
    return a.localeCompare(b);
  });

  let tmpArr: string[] = [arr[0]];
  for (let i = 1; i < arr.length; i++) {
    if (arr[i] !== arr[i - 1]) {
      tmpArr.push(arr[i]);
    }
  }
  return tmpArr;
}

export function mergeSet(set1: Set<string>, set2: Set<string>): Set<string> {
  if (set1.size > 0 && set2.size > 0) {
    return new Set([...set1, ...set2]);
  }
  if (set1.size > 0) {
    return set1;
  }
  return set2;
}

export function convertSetToArray(reservedSet: Set<string> | undefined): string[] {
  if (!reservedSet) {
    return [];
  }
  return Array.from(reservedSet);
}