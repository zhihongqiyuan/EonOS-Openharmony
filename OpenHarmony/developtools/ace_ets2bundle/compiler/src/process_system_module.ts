/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import {
  processSystemApi,
  collectImportNames
} from './validate_ui_syntax';
import { systemModules } from '../main';

module.exports = function processSystemModule(source: string): string {
  const REG_IMPORT: RegExp =
    /(import|export)\s+(.+)\s+from\s+['"](\S+)['"]|import\s+(.+)\s*=\s*require\(\s*['"](\S+)['"]\s*\)/g;
  source.replace(REG_IMPORT, (item, item1, item2, item3, item4, item5) => {
    const moduleRequest: string = item3 || item5;
    if (/^@(system|ohos)\./i.test(moduleRequest.trim())) {
      if (!systemModules.includes(moduleRequest.trim() + '.d.ts')) {
        const message: string =
          `Cannot find module '${moduleRequest}' or its corresponding type declarations.`;
        this.emitError(`BUILDERROR File: ${this.resourcePath}\n ${message}`);
      }
    }
    return item;
  });
  source = processSystemApi(source, false, this.resourcePath, true);
  collectImportNames(source, this.resourcePath);
  return source;
};
