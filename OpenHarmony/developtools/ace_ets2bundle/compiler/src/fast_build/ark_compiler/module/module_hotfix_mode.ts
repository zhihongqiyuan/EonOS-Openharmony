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

import path from 'path';

import { ModuleMode } from './module_mode';
import { PATCH_SYMBOL_TABLE } from '../common/ark_define';
import { getEs2abcFileThreadNumber } from '../utils';
import { SourceMapGenerator } from '../generate_sourcemap';
import { CompileEvent } from '../../../performance';

export class ModuleHotfixMode extends ModuleMode {
  patch: boolean;
  inOldSymbolTablePath: string;
  enableMap: boolean;

  constructor(rollupObject: Object) {
    super(rollupObject);
    this.patch = false;
    this.inOldSymbolTablePath = '';
    this.enableMap = false;
  }

  generateAbc(rollupObject: Object, parentEvent: CompileEvent): void {
    this.patch = this.projectConfig.patch || false;
    this.inOldSymbolTablePath = this.projectConfig.inOldSymbolTablePath || this.projectConfig.projectRootPath;
    this.enableMap = this.projectConfig.enableMap || false;
    this.prepareForCompilation(rollupObject, parentEvent);
    SourceMapGenerator.getInstance().buildModuleSourceMapInfo(parentEvent);

    this.generateEs2AbcCmdForHotfix();
    this.genDescriptionsForMergedEs2abc(!!this.projectConfig.byteCodeHarInfo, parentEvent);
    this.generateMergedAbcOfEs2Abc(parentEvent);
  }

  private generateEs2AbcCmdForHotfix() {
    const fileThreads = getEs2abcFileThreadNumber();
    this.cmdArgs.push(`"@${this.filesInfoPath}"`);
    this.cmdArgs.push('--npm-module-entry-list');
    this.cmdArgs.push(`"${this.npmEntriesInfoPath}"`);
    this.cmdArgs.push('--output');
    this.cmdArgs.push(`"${this.moduleAbcPath}"`);
    this.cmdArgs.push('--file-threads');
    this.cmdArgs.push(`"${fileThreads}"`);
    this.cmdArgs.push(`"--target-api-version=${this.projectConfig.compatibleSdkVersion}"`);

    if (this.projectConfig.patch) {
      const oldHapSymbolTable: string = path.join(this.projectConfig.inOldSymbolTablePath, PATCH_SYMBOL_TABLE);
      this.cmdArgs.push('--input-symbol-table');
      this.cmdArgs.push(`"${oldHapSymbolTable}"`);
      this.cmdArgs.push('--generate-patch');
    }

    if (this.projectConfig.enableMap) {
      // when generating map, cache is forbiden to avoid uncomplete symbol table
      const oldHapSymbolTable: string = path.join(this.projectConfig.inOldSymbolTablePath, PATCH_SYMBOL_TABLE);
      this.cmdArgs.push('--dump-symbol-table');
      this.cmdArgs.push(`"${oldHapSymbolTable}"`);
    }

    this.cmdArgs.push('--merge-abc');
  }
}
