/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import * as fs from 'fs';
import * as path from 'path';
import * as os from 'os';
import * as childProcess from 'child_process';
import * as process from 'process';
import { logger } from './compile_info';
import { projectConfig } from '../main';
import {
  FAIL,
  FILESINFO_TXT,
  MODULES_CACHE,
  NPMENTRIES_TXT,
  NODE_MODULES,
  PACKAGES,
  PATCH_SYMBOL_TABLE,
  EXTNAME_PROTO_BIN
} from './pre_define';
import {
  EntryInfo,
  ModuleInfo,
  initAbcEnv
} from './gen_abc_plugin';
import {
  mkdirsSync,
  toUnixPath,
  validateFilePathLength
} from './utils';

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

function generateCompileFilesInfo(moduleInfos: Array<ModuleInfo>) {
  const tempModuleInfos: ModuleInfo[] = Array<ModuleInfo>();
  moduleInfos.forEach((item) => {
    let check: boolean = tempModuleInfos.every((newItem) => {
      return item.tempFilePath !== newItem.tempFilePath;
    });
    if (check) {
      tempModuleInfos.push(item);
    }
  });
  moduleInfos = tempModuleInfos;

  const filesInfoPath: string = path.join(process.env.cachePath, FILESINFO_TXT);
  validateFilePathLength(filesInfoPath, logger);
  let filesInfo: string = '';
  moduleInfos.forEach(info => {
    const moduleType: string = info.isCommonJs ? 'commonjs' : 'esm';
    const sourceFile: string = info.filePath.replace(projectConfig.projectRootPath + path.sep, '');
    filesInfo += `${info.tempFilePath};${info.recordName};${moduleType};${toUnixPath(sourceFile)};${info.packageName}\n`;
  });
  fs.writeFileSync(filesInfoPath, filesInfo, 'utf-8');
}

export function generateNpmEntriesInfo(entryInfos: Map<string, EntryInfo>) {
  const npmEntriesInfoPath: string = path.join(process.env.cachePath, NPMENTRIES_TXT);
  validateFilePathLength(npmEntriesInfoPath, logger);
  let entriesInfo: string = '';
  for (const value of entryInfos.values()) {
    const buildPath: string =
      value.buildPath.replace(toUnixPath(projectConfig.nodeModulesPath), '').replace(new RegExp(NODE_MODULES, 'g'), PACKAGES);
    const entryFile: string = toUnixPath(path.join(buildPath, value.entry));
    const entry: string = entryFile.substring(0, entryFile.lastIndexOf('.')).replace(new RegExp(NODE_MODULES, 'g'), PACKAGES);
    entriesInfo +=
      `${toUnixPath(path.join(PACKAGES, buildPath))}:${toUnixPath(path.join(PACKAGES, entry))}\n`;
  }
  fs.writeFileSync(npmEntriesInfoPath, entriesInfo, 'utf-8');
}

function generateAbcCacheFilesInfo(moduleInfos: Array<ModuleInfo>, npmEntriesInfoPath: string, cacheFilePath: string): void {
  let abcCacheFilesInfo: string = '';

  // generate source file cache
  moduleInfos.forEach((info) => {
    let tempFilePathWithoutExt: string = info.tempFilePath.substring(0, info.tempFilePath.lastIndexOf('.'));
    let abcCacheFilePath: string = tempFilePathWithoutExt + EXTNAME_PROTO_BIN;
    abcCacheFilesInfo += `${info.tempFilePath};${abcCacheFilePath}\n`;
  });

  // generate npm entries cache
  let npmEntriesCacheFileWithoutExt: string = npmEntriesInfoPath.substring(0, npmEntriesInfoPath.lastIndexOf('.'));
  let npmEntriesCacheFilePath: string = npmEntriesCacheFileWithoutExt + EXTNAME_PROTO_BIN;
  abcCacheFilesInfo += `${npmEntriesInfoPath};${npmEntriesCacheFilePath}\n`;

  fs.writeFileSync(cacheFilePath, abcCacheFilesInfo, 'utf-8');
}

export function generateMergedAbc(moduleInfos: Array<ModuleInfo>, entryInfos: Map<string, EntryInfo>, outputABCPath: string) {
  generateCompileFilesInfo(moduleInfos);
  generateNpmEntriesInfo(entryInfos);

  const filesInfoPath: string = path.join(process.env.cachePath, FILESINFO_TXT);
  const npmEntriesInfoPath: string = path.join(process.env.cachePath, NPMENTRIES_TXT);
  const cacheFilePath: string = path.join(process.env.cachePath, MODULES_CACHE);
  validateFilePathLength(cacheFilePath, logger);
  generateAbcCacheFilesInfo(moduleInfos, npmEntriesInfoPath, cacheFilePath);

  const fileThreads = os.cpus().length < 16 ? os.cpus().length : 16;
  mkdirsSync(projectConfig.buildPath);
  let genAbcCmd: string =
    `${initAbcEnv().join(' ')} "@${filesInfoPath}" --npm-module-entry-list "${npmEntriesInfoPath}" --output "${outputABCPath}" --file-threads "${fileThreads}"`;

  projectConfig.patch = projectConfig.patch || false;
  projectConfig.enableMap = projectConfig.enableMap || false;
  projectConfig.inOldSymbolTablePath = projectConfig.inOldSymbolTablePath || projectConfig.projectRootPath;

  if (projectConfig.patch) {
    let oldHapSymbolTable = path.join(projectConfig.inOldSymbolTablePath, PATCH_SYMBOL_TABLE);
    genAbcCmd += ` --input-symbol-table "${oldHapSymbolTable}" --generate-patch`;
  }

  if (!projectConfig.enableMap) {
    genAbcCmd += ` --cache-file "@${cacheFilePath}"`;
  } else {
    // when generating map, cache is forbiden to avoid uncomplete symbol table
    let oldHapSymbolTable = path.join(projectConfig.inOldSymbolTablePath, PATCH_SYMBOL_TABLE);
    genAbcCmd += ` --dump-symbol-table "${oldHapSymbolTable}"`;
  }

  logger.debug('gen abc cmd is: ', genAbcCmd);
  try {
    if (process.env.watchMode === 'true') {
      childProcess.execSync(genAbcCmd);
    } else {
      const child = childProcess.exec(genAbcCmd);
      child.on('exit', (code: any) => {
        if (code === 1) {
          logger.debug(red, 'ArkTS:ERROR failed to execute es2abc', reset);
          process.exit(FAIL);
        }
      });

      child.on('error', (err: any) => {
        logger.debug(red, err.toString(), reset);
        process.exit(FAIL);
      });

      child.stderr.on('data', (data: any) => {
        if (projectConfig.patch) {
          let patchErr :string[] =
            data.split(os.EOL).filter(line => line.includes('[Patch]') || line.includes('Error:'));
          logger.error(red, patchErr.join(os.EOL), reset);
        } else {
          logger.error(red, data.toString(), reset);
        }
      });
    }
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR failed to generate abc with filesInfo ${filesInfoPath}. Error message: ${e}`, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.watchMode !== 'true') {
      process.exit(FAIL);
    }
  }
}
