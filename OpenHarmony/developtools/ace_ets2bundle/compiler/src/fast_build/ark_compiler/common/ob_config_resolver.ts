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

import fs from 'fs';
import path from 'path';
import type * as ts from 'typescript';
import {
  ApiExtractor,
  clearGlobalCaches,
  clearNameCache,
  deleteLineInfoForNameString,
  endFilesEvent,
  endSingleFileEvent,
  enableTimeAndMemoryPrint,
  EventList,
  getRelativeSourcePath,
  handleUniversalPathInObf,
  mangleFilePath,
  MemoryUtils,
  nameCacheMap,
  performancePrinter,
  printTimeSumData,
  printTimeSumInfo,
  PerfMode,
  ProjectCollections,
  startFilesEvent,
  startSingleFileEvent,
  unobfuscationNamesObj,
  writeObfuscationNameCache,
  writeUnobfuscationContent,
} from 'arkguard';
import type {
  ArkObfuscator,
  HvigorErrorInfo
} from 'arkguard';

import { GeneratedFileInHar, harFilesRecord, isPackageModulesFile, mkdirsSync, toUnixPath } from '../../../utils';
import { allSourceFilePaths, collectAllFiles, localPackageSet } from '../../../ets_checker';
import { isCurrentProjectFiles } from '../utils';
import { sourceFileBelongProject } from '../module/module_source_file';
import {
  compileToolIsRollUp,
  mangleDeclarationFileName,
  ModuleInfo,
  writeArkguardObfuscatedSourceCode
} from '../../../ark_utils';
import { OBFUSCATION_TOOL, red, yellow } from './ark_define';
import { logger } from '../../../compile_info';
import { MergedConfig } from '../common/ob_config_resolver';
import { ModuleSourceFile } from '../module/module_source_file';
import { readProjectAndLibsSource } from './process_ark_config';
import { CommonLogger } from '../logger';
import { MemoryMonitor } from '../../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../../meomry_monitor/memory_define';
import { ESMODULE } from '../../../pre_define';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent
} from '../../../performance';
import { BytecodeObfuscator } from '../bytecode_obfuscator';

export {
  collectResevedFileNameInIDEConfig, // For running unit test.
  collectReservedNameForObf,
  enableObfuscatedFilePathConfig,
  enableObfuscateFileName,
  generateConsumerObConfigFile,
  getRelativeSourcePath,
  handleObfuscatedFilePath,
  handleUniversalPathInObf,
  mangleFilePath,
  MergedConfig,
  nameCacheMap,
  ObConfigResolver,
  writeObfuscationNameCache,
  writeUnobfuscationContent,
} from 'arkguard';

export function resetObfuscation(): void {
  clearGlobalCaches();
  sourceFileBelongProject.clear();
  ApiExtractor.mPropertySet?.clear();
  ApiExtractor.mSystemExportSet?.clear();
  localPackageSet?.clear();
}

/**
 * dependencies of sourceFiles
 */
export const sourceFileDependencies: Map<string, ts.ModeAwareCache<ts.ResolvedModuleFull | undefined>> = new Map();

/**
 * Identifier cache name
 */
export const IDENTIFIER_CACHE: string = 'IdentifierCache';

/**
 * Subsystem Number For Obfuscation
 */
export const OBF_ERR_CODE = '108';

/**
 * Logger for obfuscation
 */
export let obfLogger: Object = undefined;

enum LoggerLevel {
  WARN = 'warn',
  ERROR = 'error'
}

export function initObfLogger(share: Object): void {
  if (share) {
    obfLogger = share.getHvigorConsoleLogger ? share.getHvigorConsoleLogger(OBF_ERR_CODE) : share.getLogger(OBFUSCATION_TOOL);
    return;
  }
  obfLogger = logger;
}

export function printObfLogger(errorInfo: string, errorCodeInfo: HvigorErrorInfo | string, level: LoggerLevel): void {
  if (obfLogger.printError) {
    switch (level) {
      case LoggerLevel.WARN:
        obfLogger.printWarn(errorCodeInfo);
        break;
      case LoggerLevel.ERROR: 
        obfLogger.printError(errorCodeInfo);
        break;
      default:
        break;
    }
    return;
  }

  switch (level) {
    case LoggerLevel.WARN:
      obfLogger.warn(yellow, errorInfo);
      break;
    case LoggerLevel.ERROR:
      obfLogger.error(red, errorInfo);
      break;
    default:
      return;
  }
}

// Collect all keep files. If the path configured by the developer is a folder, all files in the compilation will be used to match this folder.
function collectAllKeepFiles(startPaths: string[], excludePathSet: Set<string>): Set<string> {
  const allKeepFiles: Set<string> = new Set();
  const keepFolders: string[] = [];
  startPaths.forEach(filePath => {
    if (excludePathSet.has(filePath)) {
      return;
    }
    if (fs.statSync(filePath).isDirectory()) {
      keepFolders.push(filePath);
    } else {
      allKeepFiles.add(filePath);
    }
  });
  if (keepFolders.length === 0) {
    return allKeepFiles;
  }

  allSourceFilePaths.forEach(filePath => {
    if (keepFolders.some(folderPath => filePath.startsWith(folderPath)) && !excludePathSet.has(filePath)) {
      allKeepFiles.add(filePath);
    }
  });
  return allKeepFiles;
}

// Collect all keep files and then collect their dependency files.
export function handleKeepFilesAndGetDependencies(mergedObConfig: MergedConfig, arkObfuscator: ArkObfuscator,
  projectConfig: Object): Set<string> {
  if (mergedObConfig === undefined || mergedObConfig.keepSourceOfPaths.length === 0) {
    sourceFileDependencies.clear();
    return new Set<string>();
  }
  const keepPaths = mergedObConfig.keepSourceOfPaths;
  const excludePaths = mergedObConfig.excludePathSet;
  let allKeepFiles: Set<string> = collectAllKeepFiles(keepPaths, excludePaths);
  arkObfuscator.setKeepSourceOfPaths(allKeepFiles);
  if (BytecodeObfuscator.enable) {
    BytecodeObfuscator.getInstance().setKeepSourceOfPaths(allKeepFiles);
  }
  const keepFilesAndDependencies: Set<string> = getFileNamesForScanningWhitelist(mergedObConfig, allKeepFiles, projectConfig);
  sourceFileDependencies.clear();
  return keepFilesAndDependencies;
}

/**
 * Use tsc's dependency collection to collect the dependency files of the keep files.
 * Risk: The files resolved by typescript are different from the files resolved by rollup. For example, the two entry files have different priorities.
 * Tsc looks for files in the types field in oh-packagek.json5 first, and rollup looks for files in the main field.
 */
function getFileNamesForScanningWhitelist(mergedObConfig: MergedConfig, allKeepFiles: Set<string>,
  projectConfig: Object): Set<string> {
  const keepFilesAndDependencies: Set<string> = new Set<string>();
  if (!mergedObConfig.options.enableExportObfuscation) {
    return keepFilesAndDependencies;
  }
  let stack: string[] = Array.from(allKeepFiles);
  while (stack.length > 0) {
    const filePath: string = toUnixPath(stack.pop());
    if (keepFilesAndDependencies.has(filePath)) {
      continue;
    }

    keepFilesAndDependencies.add(filePath);
    const dependentModules: ts.ModeAwareCache<ts.ResolvedModuleFull | undefined> = sourceFileDependencies.get(filePath);
    dependentModules?.forEach(resolvedModule => {
      if (!resolvedModule) {
        // For `import moduleName form 'xx.so'`, when the xx.so cannot be resolved, dependentModules is [null]
        return;
      }
      let curDependencyPath: string = toUnixPath(resolvedModule.resolvedFileName);
      // resolvedModule can record system Api declaration files and ignore them
      if (isCurrentProjectFiles(curDependencyPath, projectConfig)) {
        stack.push(curDependencyPath);
      }
    });
  }
  return keepFilesAndDependencies;
}

/**
 * Get namecache by path
 *
 * If it is a declaration file, retrieves the corresponding source file's obfuscation results
 * Or retrieves obfuscation results from full compilation run
 */
export function getNameCacheByPath(
  moduleInfo: ModuleInfo,
  isDeclaration: boolean,
  projectRootPath: string | undefined
): Map<string, string> {
  let historyNameCache = new Map<string, string>();
  let nameCachePath = moduleInfo.relativeSourceFilePath;
  if (isDeclaration) {
    nameCachePath = getRelativeSourcePath(
      moduleInfo.originSourceFilePath,
      projectRootPath,
      sourceFileBelongProject.get(toUnixPath(moduleInfo.originSourceFilePath))
    );
  }
  if (nameCacheMap) {
    let identifierCache = nameCacheMap.get(nameCachePath)?.[IDENTIFIER_CACHE];
    deleteLineInfoForNameString(historyNameCache, identifierCache);
  }
  return historyNameCache;
}

/**
 * Set newly updated namecache for project source files
 */
export function setNewNameCache(
  newNameCache: Object,
  isDeclaration: boolean,
  moduleInfo: ModuleInfo,
  projectConfig: Object
): void {
  if (newNameCache && !isDeclaration) {
    let obfName: string = moduleInfo.relativeSourceFilePath;
    let isOhModule: boolean = isPackageModulesFile(moduleInfo.originSourceFilePath, projectConfig);
    if (projectConfig.obfuscationMergedObConfig?.options.enableFileNameObfuscation && !isOhModule) {
      obfName = mangleFilePath(moduleInfo.relativeSourceFilePath);
    }
    newNameCache.obfName = obfName;
    nameCacheMap.set(moduleInfo.relativeSourceFilePath, newNameCache);
  }
}

/**
 * Set unobfuscation list after obfuscation
 */
export function setUnobfuscationNames(
  unobfuscationNameMap: Map<string, Set<string>> | undefined,
  relativeSourceFilePath: string,
  isDeclaration: boolean
): void {
  if (unobfuscationNameMap && !isDeclaration) {
    let arrayObject: Record<string, string[]> = {};
    // The type of unobfuscationNameMap's value is Set, convert Set to Array.
    unobfuscationNameMap.forEach((value: Set<string>, key: string) => {
      let array: string[] = Array.from(value);
      arrayObject[key] = array;
    });
    unobfuscationNamesObj[relativeSourceFilePath] = arrayObject;
  }
}

/**
 * Write out obfuscated files
 */
export function writeObfuscatedFile(newFilePath: string, content: string): void {
  startSingleFileEvent(EventList.WRITE_FILE, performancePrinter.timeSumPrinter);
  mkdirsSync(path.dirname(newFilePath));
  fs.writeFileSync(newFilePath, content);
  endSingleFileEvent(EventList.WRITE_FILE, performancePrinter.timeSumPrinter, false, true);
}

// create or update incremental caches, also set the shouldReObfuscate flag if needed
export function updateIncrementalCaches(arkObfuscator: ArkObfuscator): void {
  if (arkObfuscator.filePathManager) {
    const deletedFilesSet: Set<string> = arkObfuscator.filePathManager.getDeletedSourceFilePaths();
    ProjectCollections.projectWhiteListManager.createOrUpdateWhiteListCaches(deletedFilesSet);
    arkObfuscator.fileContentManager.deleteFileContent(deletedFilesSet);
    arkObfuscator.shouldReObfuscate = ProjectCollections.projectWhiteListManager.getShouldReObfuscate();
  }
}

// Scan all files of project and create or update cache for it
export function readProjectCaches(allFiles: Set<string>, arkObfuscator: ArkObfuscator): void {
  arkObfuscator.filePathManager?.createOrUpdateSourceFilePaths(allFiles);

  // read fileContent caches if is increamental
  if (arkObfuscator.isIncremental) {
    arkObfuscator.fileContentManager.readFileNamesMap();
  }
}

export function getUpdatedFiles(
  sourceProjectConfig: Object,
  allSourceFilePaths: Set<string>,
  moduleSourceFiles: ModuleSourceFile[]
): Set<string> {
  let updatedFiles: Set<string> = new Set();
  if (sourceProjectConfig.arkObfuscator?.isIncremental) {
    moduleSourceFiles.forEach((sourceFile) => {
      updatedFiles.add(toUnixPath(sourceFile.moduleId));
    });
    sourceProjectConfig.arkObfuscator?.filePathManager.addedSourceFilePaths.forEach((path: string) => {
      updatedFiles.add(path);
    });
    // Add declaration files written by users
    allSourceFilePaths.forEach((path: string) => {
      if (path.endsWith('.d.ts') || path.endsWith('.d.ets')) {
        updatedFiles.add(path);
      }
    });
  } else {
    updatedFiles = allSourceFilePaths;
  }
  return updatedFiles;
}

/**
 * Preprocess of obfuscation:
 * 1. collect whileLists for each file.
 * 2. create or update incremental caches
 */
export function obfuscationPreprocess(
  sourceProjectConfig: Object,
  obfuscationConfig: MergedConfig,
  allSourceFilePaths: Set<string>,
  keepFilesAndDependencies: Set<string>,
  moduleSourceFiles: ModuleSourceFile[]
): void {
  if (sourceProjectConfig.arkObfuscator) {
    readProjectCaches(allSourceFilePaths, sourceProjectConfig.arkObfuscator);

    const updatedFiles = getUpdatedFiles(sourceProjectConfig, allSourceFilePaths, moduleSourceFiles);

    readProjectAndLibsSource(
      updatedFiles,
      obfuscationConfig,
      sourceProjectConfig.arkObfuscator,
      sourceProjectConfig.compileHar,
      keepFilesAndDependencies
    );

    updateIncrementalCaches(sourceProjectConfig.arkObfuscator);
    if (BytecodeObfuscator.enable) {
      BytecodeObfuscator.getInstance().removeStructProp();
    }
    ProjectCollections.clearProjectWhiteListManager();
  }
}

/**
 * Reobfuscate all files if needed.
 */
export async function reObfuscate(
  arkObfuscator: ArkObfuscator,
  harFilesRecord: Map<string, GeneratedFileInHar>,
  logger: Function,
  projectConfig: Object
): Promise<void> {
  // name cache cannot be used since we need to reObfuscate all files
  clearNameCache();
  const sortedFiles: string[] = arkObfuscator.fileContentManager.getSortedFiles();
  for (const originFilePath of sortedFiles) {
    const transformedFilePath: string = arkObfuscator.fileContentManager.fileNamesMap.get(originFilePath);
    const fileContentObj: ProjectCollections.FileContent = arkObfuscator.fileContentManager.readFileContent(transformedFilePath);
    const originSourceFilePath: string = toUnixPath(fileContentObj.moduleInfo.originSourceFilePath);
    const isOriginalDeclaration: boolean = (/\.d\.e?ts$/).test(originSourceFilePath);
    if (isOriginalDeclaration) {
      if (!harFilesRecord.has(originSourceFilePath)) {
        const genFilesInHar: GeneratedFileInHar = {
          sourcePath: originSourceFilePath,
          originalDeclarationCachePath: fileContentObj.moduleInfo.buildFilePath,
          originalDeclarationContent: fileContentObj.moduleInfo.content
        };
        harFilesRecord.set(originSourceFilePath, genFilesInHar);
      }
      continue;
    }
    MemoryUtils.tryGC();
    await writeArkguardObfuscatedSourceCode(fileContentObj.moduleInfo, logger, projectConfig, fileContentObj.previousStageSourceMap);
    MemoryUtils.tryGC();
  }
}

/**
 * Include collect file, resolve denpendency, read source
 */
export function collectSourcesWhiteList(rollupObject: Object, allSourceFilePaths: Set<string>, sourceProjectConfig: Object,
  sourceFiles: ModuleSourceFile[]
): void {
  collectAllFiles(undefined, rollupObject.getModuleIds(), rollupObject);
  startFilesEvent(EventList.SCAN_SOURCEFILES, performancePrinter.timeSumPrinter);
  const recordInfo = MemoryMonitor.recordStage(MemoryDefine.SCAN_SOURCEFILES);
  if (compileToolIsRollUp()) {
    const obfuscationConfig = sourceProjectConfig.obfuscationMergedObConfig;
    handleUniversalPathInObf(obfuscationConfig, allSourceFilePaths);
    const keepFilesAndDependencies = handleKeepFilesAndGetDependencies(
      obfuscationConfig,
      sourceProjectConfig.arkObfuscator,
      sourceProjectConfig
    );
    obfuscationPreprocess(
      sourceProjectConfig,
      obfuscationConfig,
      allSourceFilePaths,
      keepFilesAndDependencies,
      sourceFiles
    );
  }
  MemoryMonitor.stopRecordStage(recordInfo);
  endFilesEvent(EventList.SCAN_SOURCEFILES, performancePrinter.timeSumPrinter);
}

/**
 * Handle post obfuscation tasks:
 * 1.ReObfuscate all files if whitelists changed in incremental build.
 * 2.Obfuscate declaration files.
 * 3.Write fileNamesMap.
 */
export async function handlePostObfuscationTasks(
  sourceProjectConfig: Object,
  projectConfig: Object,
  rollupObject: Object,
  logger: Function
): Promise<void> {
  const arkObfuscator = sourceProjectConfig.arkObfuscator;

  if (arkObfuscator?.shouldReObfuscate) {
    await reObfuscate(arkObfuscator, harFilesRecord, logger, projectConfig);
    arkObfuscator.shouldReObfuscate = false;
  }

  if (compileToolIsRollUp() && rollupObject.share.arkProjectConfig.compileMode === ESMODULE) {
    await mangleDeclarationFileName(logger, rollupObject.share.arkProjectConfig, sourceFileBelongProject);
  }

  if (arkObfuscator?.fileContentManager) {
    arkObfuscator.fileContentManager.writeFileNamesMap();
  }

  printTimeSumInfo('All files obfuscation:');
  printTimeSumData();
  endFilesEvent(EventList.ALL_FILES_OBFUSCATION);
}

/**
 * Write obfuscation caches if needed
 */
export function writeObfuscationCaches(sourceProjectConfig: Object, parentEvent: CompileEvent): void {
  const eventObfuscatedCode = createAndStartEvent(parentEvent, 'write obfuscation name cache');

  const needToWriteCache = compileToolIsRollUp() && sourceProjectConfig.arkObfuscator && sourceProjectConfig.obfuscationOptions;
  const isWidgetCompile = sourceProjectConfig.widgetCompile;

  if (needToWriteCache) {
    writeObfuscationNameCache(
      sourceProjectConfig,
      sourceProjectConfig.entryPackageInfo,
      sourceProjectConfig.obfuscationOptions.obfuscationCacheDir,
      sourceProjectConfig.obfuscationMergedObConfig.options?.printNameCache
    );
  }

  if (needToWriteCache && !isWidgetCompile) {
    writeUnobfuscationContent(sourceProjectConfig);
  }

  stopEvent(eventObfuscatedCode);
  
  if (sourceProjectConfig.perf === PerfMode.ADVANCED) {
    enableTimeAndMemoryPrint();
  }
}