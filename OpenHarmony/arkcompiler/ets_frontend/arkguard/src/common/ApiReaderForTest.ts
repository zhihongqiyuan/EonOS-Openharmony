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

import { ArkObfuscator } from '../ArkObfuscator';
import { IOptions } from '../configs/IOptions';
import { ReseverdSetForArkguard, readProjectPropertiesByCollectedPaths } from './ApiReader';
import fs from 'fs';
import path from 'path';

/**
 * Recursive retrieval of all subfile paths within a folder
 * @param dir - Folder path to traverse
 * @returns returns an array of all subfile paths
 */
function getAllFiles(inputPaths: string[]): Set<string> {
  let results: Set<string> = new Set();
  for (let inputPath of inputPaths) {
    const stat = fs.statSync(inputPath);
    if (stat.isDirectory()) {
      getAllFilesInDirRecursively(inputPath, results);
    } else if (stat.isFile()) {
      results.add(inputPath);
    }
  }

  return results;
}

function getAllFilesInDirRecursively(dir: string, results: Set<string>): void {
  const list = fs.readdirSync(dir);
  list.forEach((file) => {
    const filePath = path.join(dir, file);
    const stat = fs.statSync(filePath);

    if (stat && stat.isDirectory()) {
      getAllFilesInDirRecursively(filePath, results);
    } else {
      results.add(filePath);
    }
  });
}

/**
 * read project reserved properties for UT
 * @param projectPaths can be dir or file
 * @param customProfiles
 */
export function readProjectProperties(projectPaths: string[], customProfiles: IOptions, arkObfuscator: ArkObfuscator): void {
  const allPaths: Set<string> = getAllFiles(projectPaths);
  let projectAndLibs: ReseverdSetForArkguard = readProjectPropertiesByCollectedPaths(allPaths, customProfiles, false);
  if (customProfiles.mNameObfuscation.mRenameProperties) {
    arkObfuscator.addReservedSetForPropertyObf(projectAndLibs);
  }
  if (customProfiles.mExportObfuscation) {
    arkObfuscator.addReservedSetForDefaultObf(projectAndLibs);
  }
}