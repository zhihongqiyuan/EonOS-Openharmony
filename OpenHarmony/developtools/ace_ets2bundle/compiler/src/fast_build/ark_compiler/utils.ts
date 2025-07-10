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

import cluster from 'cluster';
import fs from 'fs';
import path from 'path';
import ts from 'typescript';
import os from 'os';
import sourceMap from 'source-map';

import {
  DEBUG,
  ESMODULE,
  EXTNAME_ETS,
  EXTNAME_JS,
  EXTNAME_TS,
  EXTNAME_JSON,
  EXTNAME_CJS,
  EXTNAME_MJS,
  TEMPORARY
} from './common/ark_define';
import {
  nodeLargeOrEqualTargetVersion,
  genTemporaryPath,
  mkdirsSync,
  validateFilePathLength,
  toUnixPath,
  isPackageModulesFile,
  isFileInProject
} from '../../utils';
import {
  tryMangleFileName,
  writeObfuscatedSourceCode,
} from '../../ark_utils';
import { AOT_FULL, AOT_PARTIAL, AOT_TYPE } from '../../pre_define';
import { SourceMapGenerator } from './generate_sourcemap';
import {
  handleObfuscatedFilePath,
  enableObfuscateFileName,
  enableObfuscatedFilePathConfig,
  getRelativeSourcePath
} from './common/ob_config_resolver';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent
} from '../../performance';
import { BytecodeObfuscator } from './bytecode_obfuscator';

export let hasTsNoCheckOrTsIgnoreFiles: string[] = [];
export let compilingEtsOrTsFiles: string[] = [];

export function cleanUpFilesList(): void {
  hasTsNoCheckOrTsIgnoreFiles = [];
  compilingEtsOrTsFiles = [];
}

export function needAotCompiler(projectConfig: Object): boolean {
  return projectConfig.compileMode === ESMODULE && (projectConfig.anBuildMode === AOT_FULL ||
    projectConfig.anBuildMode === AOT_PARTIAL);
}

export function isAotMode(projectConfig: Object): boolean {
  return projectConfig.compileMode === ESMODULE && (projectConfig.anBuildMode === AOT_FULL ||
    projectConfig.anBuildMode === AOT_PARTIAL || projectConfig.anBuildMode === AOT_TYPE);
}

export function isDebug(projectConfig: Object): boolean {
  return projectConfig.buildMode.toLowerCase() === DEBUG;
}

export function isBranchElimination(projectConfig: Object): boolean {
  return projectConfig.branchElimination;
}

export function isMasterOrPrimary() {
  return ((nodeLargeOrEqualTargetVersion(16) && cluster.isPrimary) ||
    (!nodeLargeOrEqualTargetVersion(16) && cluster.isMaster));
}

export function changeFileExtension(file: string, targetExt: string, originExt = ''): string {
  let currentExt = originExt.length === 0 ? path.extname(file) : originExt;
  let fileWithoutExt = file.substring(0, file.lastIndexOf(currentExt));
  return fileWithoutExt + targetExt;
}

function removeCacheFile(cacheFilePath: string, ext: string): void {
  let filePath = toUnixPath(changeFileExtension(cacheFilePath, ext));
  if (fs.existsSync(filePath)) {
    fs.rmSync(filePath);
  }
}

export function shouldETSOrTSFileTransformToJS(filePath: string, projectConfig: Object, metaInfo?: Object): boolean {
  let cacheFilePath: string = genTemporaryPath(filePath, projectConfig.projectPath, projectConfig.cachePath,
    projectConfig, metaInfo);

  if (!projectConfig.processTs) {
    removeCacheFile(cacheFilePath, EXTNAME_TS);
    return true;
  }

  if (compilingEtsOrTsFiles.indexOf(filePath) !== -1) {
    // file involves in compilation
    const hasTsNoCheckOrTsIgnore = hasTsNoCheckOrTsIgnoreFiles.indexOf(filePath) !== -1;
    // Remove cacheFile whose extension is different the target file
    removeCacheFile(cacheFilePath, hasTsNoCheckOrTsIgnore ? EXTNAME_TS : EXTNAME_JS);
    return hasTsNoCheckOrTsIgnore;
  }

  cacheFilePath = updateCacheFilePathIfEnableObuscatedFilePath(filePath, cacheFilePath, projectConfig);
  cacheFilePath = toUnixPath(changeFileExtension(cacheFilePath, EXTNAME_JS));
  return fs.existsSync(cacheFilePath);
}

// This API is used exclusively to determine whether a file needs to be converted into a JS file without removing the cached files,
// which differs from API 'shouldETSOrTSFileTransformToJS'.
export function shouldETSOrTSFileTransformToJSWithoutRemove(filePath: string, projectConfig: Object, metaInfo?: Object): boolean {
  if (!projectConfig.processTs) {
    return true;
  }

  if (compilingEtsOrTsFiles.indexOf(filePath) !== -1) {
    // file involves in compilation
    return hasTsNoCheckOrTsIgnoreFiles.indexOf(filePath) !== -1;
  }

  let cacheFilePath: string = genTemporaryPath(filePath, projectConfig.projectPath, projectConfig.cachePath,
    projectConfig, metaInfo);
  cacheFilePath = updateCacheFilePathIfEnableObuscatedFilePath(filePath, cacheFilePath, projectConfig);
  cacheFilePath = toUnixPath(changeFileExtension(cacheFilePath, EXTNAME_JS));
  return fs.existsSync(cacheFilePath);
}

function updateCacheFilePathIfEnableObuscatedFilePath(filePath: string, cacheFilePath: string, projectConfig: Object): string {
  const isPackageModules = isPackageModulesFile(filePath, projectConfig);
  if (enableObfuscatedFilePathConfig(isPackageModules, projectConfig) && enableObfuscateFileName(isPackageModules, projectConfig)) {
    return handleObfuscatedFilePath(cacheFilePath, isPackageModules, projectConfig);
  }
  return cacheFilePath;
}

export async function writeFileContentToTempDir(id: string, content: string, projectConfig: Object,
  logger: Object, parentEvent: CompileEvent, metaInfo: Object): Promise<void> {
  if (isCommonJsPluginVirtualFile(id)) {
    return;
  }

  if (!isCurrentProjectFiles(id, projectConfig)) {
    return;
  }

  let filePath: string;
  if (projectConfig.compileHar) {
    // compileShared: compile shared har of project
    filePath = genTemporaryPath(id,
      projectConfig.compileShared ? projectConfig.projectRootPath : projectConfig.moduleRootPath,
      projectConfig.compileShared ? path.resolve(projectConfig.aceModuleBuild, '../etsFortgz') : projectConfig.cachePath,
      projectConfig, metaInfo, projectConfig.compileShared);
  } else {
    filePath = genTemporaryPath(id, projectConfig.projectPath, projectConfig.cachePath, projectConfig, metaInfo);
  }

  const eventWriteFileContent = createAndStartEvent(parentEvent, 'write file content');
  switch (path.extname(id)) {
    case EXTNAME_ETS:
    case EXTNAME_TS:
    case EXTNAME_JS:
    case EXTNAME_MJS:
    case EXTNAME_CJS:
      await writeFileContent(id, filePath, content, projectConfig, logger, metaInfo);
      break;
    case EXTNAME_JSON:
      const newFilePath: string = tryMangleFileName(filePath, projectConfig, id, projectConfig.isBytecodeObfEnabled);
      mkdirsSync(path.dirname(newFilePath));
      fs.writeFileSync(newFilePath, content ?? '');
      break;
    default:
      break;
  }
  stopEvent(eventWriteFileContent);
}

async function writeFileContent(sourceFilePath: string, filePath: string, content: string,
  projectConfig: Object, logger: Object, metaInfo?: Object): Promise<void> {
  if (!isSpecifiedExt(sourceFilePath, EXTNAME_JS)) {
    filePath = changeFileExtension(filePath, EXTNAME_JS);
  }

  if (!isDebug(projectConfig)) {
    const relativeSourceFilePath: string = getRelativeSourcePath(sourceFilePath, projectConfig.projectRootPath,
      metaInfo?.belongProjectPath);
    await writeObfuscatedSourceCode({content: content, buildFilePath: filePath,
      relativeSourceFilePath: relativeSourceFilePath, originSourceFilePath: sourceFilePath, rollupModuleId: sourceFilePath},
      logger, projectConfig, SourceMapGenerator.getInstance().getSourceMaps());
    return;
  }
  mkdirsSync(path.dirname(filePath));
  fs.writeFileSync(filePath, content, 'utf-8');
}

export function getEs2abcFileThreadNumber(): number {
  const fileThreads: number = os.cpus().length < 16 ? os.cpus().length : 16;
  return fileThreads;
}

export function isCommonJsPluginVirtualFile(filePath: string): boolean {
  // rollup uses commonjs plugin to handle commonjs files,
  // which will automatic generate files like 'jsfile.js?commonjs-exports'
  return filePath.includes('\x00');
}

export function isCurrentProjectFiles(filePath: string, projectConfig: Object): boolean {
  if (projectConfig.rootPathSet) {
    for (const projectRootPath of projectConfig.rootPathSet) {
      if (isFileInProject(filePath, projectRootPath)) {
        return true;
      }
    }
    return false;
  } 
  return isFileInProject(filePath, projectConfig.projectRootPath);
}

export function genTemporaryModuleCacheDirectoryForBundle(projectConfig: Object): string {
  const buildDirArr: string[] = projectConfig.aceModuleBuild.split(path.sep);
  const abilityDir: string = buildDirArr[buildDirArr.length - 1];
  const temporaryModuleCacheDirPath: string = path.join(projectConfig.cachePath, TEMPORARY, abilityDir);
  mkdirsSync(temporaryModuleCacheDirPath);

  return temporaryModuleCacheDirPath;
}

export function isSpecifiedExt(filePath: string, fileExtendName: string) {
  return path.extname(filePath) === fileExtendName;
}

export function genCachePath(tailName: string, projectConfig: Object, logger: Object): string {
  const pathName: string = projectConfig.cachePath !== undefined ?
    path.join(projectConfig.cachePath, TEMPORARY, tailName) : path.join(projectConfig.aceModuleBuild, tailName);
  mkdirsSync(path.dirname(pathName));

  validateFilePathLength(pathName, logger);
  return pathName;
}

export function isTsOrEtsSourceFile(file: string): boolean {
  return /(?<!\.d)\.[e]?ts$/.test(file);
}

export function isJsSourceFile(file: string): boolean {
  return /\.[cm]?js$/.test(file);
}

export function isJsonSourceFile(file: string): boolean {
  return /\.json$/.test(file);
}

export async function updateSourceMap(originMap: sourceMap.RawSourceMap, newMap: sourceMap.RawSourceMap): Promise<any> {
  if (!originMap) {
    return newMap;
  }
  if (!newMap) {
    return originMap;
  }
  const originConsumer: sourceMap.SourceMapConsumer = await new sourceMap.SourceMapConsumer(originMap);
  const newConsumer: sourceMap.SourceMapConsumer = await new sourceMap.SourceMapConsumer(newMap);
  const newMappingList: sourceMap.MappingItem[] = [];
  newConsumer.eachMapping((mapping: sourceMap.MappingItem) => {
    if (mapping.originalLine == null) {
      return;
    }
    const originalPos =
      originConsumer.originalPositionFor({ line: mapping.originalLine, column: mapping.originalColumn });
    if (originalPos.source == null) {
      return;
    }
    mapping.originalLine = originalPos.line;
    mapping.originalColumn = originalPos.column;
    newMappingList.push(mapping);
  });
  const updatedGenerator: sourceMap.SourceMapGenerator = sourceMap.SourceMapGenerator.fromSourceMap(newConsumer);
  updatedGenerator._file = originMap.file;
  updatedGenerator._mappings._array = newMappingList;
  return JSON.parse(updatedGenerator.toString());
}

export function hasArkDecorator(node: ts.MethodDeclaration | ts.FunctionDeclaration |
  ts.StructDeclaration | ts.ClassDeclaration | ts.TypeAliasDeclaration, decortorName: string): boolean {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (decorators && decorators.length) {
    for (let i = 0; i < decorators.length; i++) {
      const originalDecortor: string = decorators[i].getText().replace(/\(.*\)$/, '').replace(/\s*/g, '').trim();
      return originalDecortor === decortorName;
    }
  }
  return false;
}

export const utUtils = {
  writeFileContent
};