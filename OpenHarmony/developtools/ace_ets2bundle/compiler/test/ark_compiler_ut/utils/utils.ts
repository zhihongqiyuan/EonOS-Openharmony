/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use rollupObject file except in compliance with the License.
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

import os from 'os';
import fs from "fs";
import path from "path";
import * as ts from 'typescript';
import { PROJECT_ROOT } from "../mock/rollup_mock/path_config";
import { DEFAULT_PROJECT } from "../mock/rollup_mock/path_config";
import { 
  ArkTSErrorDescription,
  ErrorCode
} from '../../../lib/fast_build/ark_compiler/error_code';
import {
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';

export function cpus() {
  return os.cpus().length < 16 ? os.cpus().length : 16;
}

export function getProjectPath(name?: string) {
  return name ? `${PROJECT_ROOT}/${name}` : `${PROJECT_ROOT}/${DEFAULT_PROJECT}`;
}

export function scanFiles(filepath: string, fileList: Set<string>) {
  if (!fs.existsSync(filepath)) {
    return;
  }
  const files = fs.readdirSync(filepath);
  files.forEach((file) => {
    const child = path.join(filepath, file);
    const stat = fs.statSync(child);
    if (stat.isDirectory()) {
      scanFiles(child, fileList);
    } else {
      if (child.includes("mock")) {
        return;
      }
      fileList.add(child);
    }
  });
}
export function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

export function findImportSpecifier(node) {
  if (ts.isImportSpecifier(node)) {
    return node;
  }
  let result = null;
  let found = false;
  ts.forEachChild(node, child => {
    if (!found) {
      const potentialResult = findImportSpecifier(child);
      if (potentialResult !== null) {
        result = potentialResult;
        found = true;
      }
    }
  });
  return result;
}

export function createErrInfo(name: string): LogData {
  return LogDataFactory.newInstance(
    ErrorCode.ETS2BUNDLE_EXTERNAL_LAZY_IMPORT_RE_EXPORT_ERROR,
    ArkTSErrorDescription,
    `'${name}' of lazy-import is re-export`,
    '',
    ['Please make sure the namedBindings of lazy-import are not be re-exported.',
      'Please check whether the autoLazyImport switch is opened.']
  );
}
