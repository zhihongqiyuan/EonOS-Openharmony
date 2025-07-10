/*
 * Copyright (c) 2023-2025 Huawei Device Co., Ltd.
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

import ts from 'typescript';
import path from 'path';
import fs from 'fs';
import { createFilter } from '@rollup/pluginutils';
import MagicString from 'magic-string';
import nodeEvents from 'node:events';

import {
  LogInfo,
  componentInfo,
  emitLogInfo,
  getTransformLog,
  genTemporaryPath,
  writeFileSync,
  getAllComponentsOrModules,
  writeCollectionFile,
  storedFileInfo,
  fileInfo,
  resourcesRawfile,
  differenceResourcesRawfile,
  CacheFile,
  genLoaderOutPathOfHar,
  harFilesRecord,
  resetUtils,
  getResolveModules,
  toUnixPath,
  getBelongModuleInfo
} from '../../utils';
import {
  preprocessExtend,
  preprocessNewExtend,
  validateUISyntax,
  propertyCollection,
  linkCollection,
  resetComponentCollection,
  componentCollection,
  resetValidateUiSyntax
} from '../../validate_ui_syntax';
import {
  processUISyntax,
  resetLog,
  transformLog,
  resetProcessUiSyntax
} from '../../process_ui_syntax';
import {
  projectConfig,
  abilityPagesFullPath,
  globalProgram,
  resetMain,
  globalModulePaths
} from '../../../main';
import {
  appComponentCollection,
  compilerOptions as etsCheckerCompilerOptions,
  resolveModuleNames,
  resolveTypeReferenceDirectives,
  resetEtsCheck,
  collectAllFiles,
  allModuleIds,
  resetEtsCheckTypeScript,
  maxMemoryInServiceChecker
} from '../../ets_checker';
import {
  CUSTOM_BUILDER_METHOD,
  GLOBAL_CUSTOM_BUILDER_METHOD,
  INNER_CUSTOM_BUILDER_METHOD,
  resetComponentMap,
  INNER_CUSTOM_LOCALBUILDER_METHOD,
  EXTEND_ATTRIBUTE,
  resetExtContent
} from '../../component_map';
import {
  kitTransformLog,
  processKitImport,
  checkHasKeepTs,
  resetKitImportLog
} from '../../process_kit_import';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent,
  getHookEventFactory
} from '../../performance';
import { resetProcessComponentMember } from '../../process_component_member';
import {
  collectReservedNameForObf,
  mangleFilePath,
  resetObfuscation
} from '../ark_compiler/common/ob_config_resolver';
import {
  LazyImportOptions,
  reExportNoCheckMode
} from '../../process_lazy_import';
import arkoalaProgramTransform, { ArkoalaPluginOptions } from './arkoala-plugin';
import processStructComponentV2 from '../../process_struct_componentV2';
import { shouldETSOrTSFileTransformToJSWithoutRemove } from '../ark_compiler/utils';
import { MemoryMonitor } from '../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../meomry_monitor/memory_define';
import { ModuleSourceFile } from '../ark_compiler/module/module_source_file';
import { ARKUI_SUBSYSTEM_CODE } from '../../../lib/hvigor_error_code/hvigor_error_info';
import { ProjectCollections } from 'arkguard';
import parseIntent from '../../userIntents_parser/parseUserIntents';
import { concatenateEtsOptions, getExternalComponentPaths } from '../../external_component_map';

let switchTsAst: boolean = true;

const filter: any = createFilter(/(?<!\.d)\.(ets|ts)$/);

const shouldEmitJsFlagMap: Map<string, boolean> = new Map<string, boolean>();
let shouldDisableCache: boolean = false;
interface ShouldEnableDebugLineType {
  enableDebugLine: boolean;
}

export const ShouldEnableDebugLine: ShouldEnableDebugLineType = {
  enableDebugLine: false
};

let disableCacheOptions = {
  bundleName: 'default',
  entryModuleName: 'default',
  runtimeOS: 'default',
  resourceTableHash: 'default',
  etsLoaderVersion: 'default'
};

export function etsTransform() {
  const allFilesInHar: Map<string, string> = new Map();
  let cacheFile: { [fileName: string]: CacheFile };
  return {
    name: 'etsTransform',
    transform: transform,
    moduleParsed(moduleInfo: moduleInfoType): void {
      if (this.share.projectConfig.singleFileEmit && this.share.projectConfig.needCoverageInsert) {
        //ts coverage instrumentation
        this.share.sourceFile = ModuleSourceFile.getSourceFileById(moduleInfo.id);
      }
    },
    buildStart() {
      const hookEventFactory: CompileEvent = getHookEventFactory(this.share, 'etsTransform', 'buildStart');
      const eventEtsTransformBuildStart = createAndStartEvent(hookEventFactory, 'etsTransformBuildStart');
      judgeCacheShouldDisabled.call(this);
      if (process.env.compileMode === 'moduleJson') {
        cacheFile = this.cache.get('transformCacheFiles');
        storedFileInfo.addGlobalCacheInfo(this.cache.get('resourceListCacheInfo'),
          this.cache.get('resourceToFileCacheInfo'), cacheFile);
        if (this.cache.get('lastResourcesArr')) {
          storedFileInfo.lastResourcesSet = new Set([...this.cache.get('lastResourcesArr')]);
        }
        if (process.env.rawFileResource) {
          resourcesRawfile(process.env.rawFileResource, storedFileInfo.resourcesArr);
          this.share.rawfilechanged = differenceResourcesRawfile(storedFileInfo.lastResourcesSet, storedFileInfo.resourcesArr);
        }
      }
      if (!!this.cache.get('enableDebugLine') !== projectConfig.enableDebugLine) {
        ShouldEnableDebugLine.enableDebugLine = true;
      }
      // Initialize the baseline of trggering GC by the maxMemory at the end of serviceChecker. 
      // In etsTransform plugin, if the memory after transforming a file exceeds the baseline,
      // trigger GC and update a new baseline.
      ts.MemoryUtils.initializeBaseMemory(maxMemoryInServiceChecker);
      stopEvent(eventEtsTransformBuildStart);
    },
    load(id: string) {
      let fileCacheInfo: fileInfo;
      const hookEventFactory: CompileEvent = getHookEventFactory(this.share, 'etsTransform', 'load');
      const eventEtsTransformLoad = createAndStartEvent(hookEventFactory, 'etsTransformLoad');
      if (this.cache.get('fileCacheInfo')) {
        fileCacheInfo = this.cache.get('fileCacheInfo')[path.resolve(id)];
      }
      // Exclude Component Preview page
      if (projectConfig.isPreview && !projectConfig.checkEntry && id.match(/(?<!\.d)\.(ets)$/)) {
        abilityPagesFullPath.add(path.resolve(id).toLowerCase());
        storedFileInfo.judgeShouldHaveEntryFiles(abilityPagesFullPath);
      }
      storedFileInfo.addFileCacheInfo(path.resolve(id), fileCacheInfo);
      storedFileInfo.setCurrentArkTsFile();
      stopEvent(eventEtsTransformLoad);
    },
    shouldInvalidCache(options) {
      const fileName: string = path.resolve(options.id);
      let shouldDisable: boolean = shouldDisableCache || disableNonEntryFileCache(fileName) || ShouldEnableDebugLine.enableDebugLine;
      if (process.env.compileMode === 'moduleJson') {
        shouldDisable = shouldDisable || storedFileInfo.shouldInvalidFiles.has(fileName) || this.share.rawfilechanged;
        if (cacheFile && cacheFile[fileName] && cacheFile[fileName].children.length) {
          for (let child of cacheFile[fileName].children) {
            const newTimeMs: number = fs.existsSync(child.fileName) ? fs.statSync(child.fileName).mtimeMs : -1;
            const fileHash: string = this.share?.getHashByFilePath ? this.share?.getHashByFilePath(child.fileName) : '';
            if (this.share?.getHashByFilePath && (fileHash !== child.hash || fileHash === '')) {
              shouldDisable = true;
              break;
            } else if (!(this.share?.getHashByFilePath) && newTimeMs !== child.mtimeMs) {
              shouldDisable = true;
              break;
            }
          }
        }
      }
      // If a file import an const enum object from other changed file, this file also need to be transformed.
      shouldDisable = shouldDisable || checkRelateToConstEnum(options.id);
      if (!shouldDisable) {
        storedFileInfo.collectCachedFiles(fileName);
      }
      return shouldDisable;
    },
    buildEnd(): void {
      const fileToDelete: string[] = Array.from(CreateProgramMoment.deleteFileCollect);
      globalProgram.program.deleteProgramSourceFiles(fileToDelete);
      CreateProgramMoment.resetDeleteFiles();
      if (process.env.watchMode !== 'true' && !projectConfig.hotReload && !projectConfig.isPreview) {
        resetEtsCheckTypeScript();
      }
    },
    afterBuildEnd() {
      const hookEventFactory: CompileEvent = getHookEventFactory(this.share, 'etsTransform', 'afterBuildEnd');
      const eventEtsTransformAfterBuildEnd = createAndStartEvent(hookEventFactory, 'etsTransformafterBuildEnd');
      if (parseIntent.intentData.length > 0 || parseIntent.isUpdateCompile) {
        parseIntent.verifyInheritanceChain();
        parseIntent.writeUserIntentJsonFile();
      }
      // Copy the cache files in the compileArkTS directory to the loader_out directory
      if (projectConfig.compileHar && !projectConfig.byteCodeHar) {
        for (let moduleInfoId of allModuleIds.keys()) {
          const moduleInfo: Object = this.getModuleInfo(moduleInfoId);
          if (!moduleInfo && !moduleInfoId.match(/\.d\.e?ts$/)) {
            continue;
          }
          if (moduleInfoId && !moduleInfoId.match(new RegExp(projectConfig.packageDir)) &&
            !moduleInfoId.startsWith('\x00') &&
            path.resolve(moduleInfoId).startsWith(projectConfig.moduleRootPath + path.sep)) {
            let filePath: string = moduleInfoId;
            let metaInfo: Object;
            if (filePath.match(/\.d\.e?ts$/)) {
              metaInfo = getBelongModuleInfo(moduleInfoId, projectConfig.modulePathMap, projectConfig.projectRootPath);
            } else {
              metaInfo = moduleInfo.meta;
            }
            if (this.share.arkProjectConfig?.obfuscationMergedObConfig?.options?.enableFileNameObfuscation) {
              filePath = mangleFilePath(filePath);
            }

            const cacheFilePath: string = genTemporaryPath(filePath, projectConfig.moduleRootPath,
              process.env.cachePath, projectConfig, metaInfo);
            const buildFilePath: string = genTemporaryPath(filePath, projectConfig.moduleRootPath,
              projectConfig.buildPath, projectConfig, metaInfo, true);
            const recordInfo = MemoryMonitor.recordStage(MemoryDefine.SET_INCREMENTAL_FILE_IN_HAR);
            if (filePath.match(/\.e?ts$/)) {
              setIncrementalFileInHar(cacheFilePath, buildFilePath, allFilesInHar);
            } else {
              allFilesInHar.set(cacheFilePath, buildFilePath);
            }
            MemoryMonitor.stopRecordStage(recordInfo);
          }
        }

        allFilesInHar.forEach((buildFilePath, cacheFilePath) => {
          // if the ts or ets file code only contain interface, it doesn't have js file.
          if (fs.existsSync(cacheFilePath)) {
            const sourceCode: string = fs.readFileSync(cacheFilePath, 'utf-8');
            writeFileSync(buildFilePath, sourceCode);
          }
        });
      }
      shouldDisableCache = false;
      this.cache.set('disableCacheOptions', disableCacheOptions);
      this.cache.set('lastResourcesArr', [...storedFileInfo.resourcesArr]);
      if (projectConfig.enableDebugLine) {
        this.cache.set('enableDebugLine', true);
      } else {
        this.cache.set('enableDebugLine', false);
      }
      storedFileInfo.clearCollectedInfo(this.cache);
      this.cache.set('transformCacheFiles', storedFileInfo.transformCacheFiles);
      stopEvent(eventEtsTransformAfterBuildEnd);
    },
    cleanUp(): void {
      resetMain();
      resetComponentMap();
      resetEtsCheck();
      resetEtsTransform();
      resetProcessComponentMember();
      resetProcessUiSyntax();
      resetUtils();
      resetValidateUiSyntax();
      resetObfuscation();
      parseIntent.clear();
      resetExtContent();
    }
  };
}

export function shouldEmitJsFlagById(id: string): boolean {
  return shouldEmitJsFlagMap.get(id);
}

// If a ArkTS file don't have @Entry decorator but it is entry file this time
function disableNonEntryFileCache(filePath: string): boolean {
  return storedFileInfo.buildStart && filePath.match(/(?<!\.d)\.(ets)$/) &&
    !storedFileInfo.wholeFileInfo[filePath].hasEntry &&
    storedFileInfo.shouldHaveEntry.includes(filePath);
}

function judgeCacheShouldDisabled(): void {
  for (const key in disableCacheOptions) {
    if (this.cache.get('disableCacheOptions') && this.share &&
      this.share.projectConfig && this.share.projectConfig[key] &&
      this.cache.get('disableCacheOptions')[key] !== this.share.projectConfig[key]) {
      if (key === 'resourceTableHash' && process.env.compileMode === 'moduleJson') {
        storedFileInfo.resourceTableChanged = true;
      } else if (!!projectConfig.uiTransformOptimization && key === 'bundleName' &&
        process.env.compileMode === 'moduleJson') {
        shouldDisableCache = false;
      } else if (!shouldDisableCache) {
        shouldDisableCache = true;
      }
    }
    if (this.share && this.share.projectConfig && this.share.projectConfig[key]) {
      disableCacheOptions[key] = this.share.projectConfig[key];
    }
    storedFileInfo.judgeShouldHaveEntryFiles(abilityPagesFullPath);
  }
}

interface EmitResult {
  outputText: string,
  sourceMapText: string,
}

export function getCompilerHost(): ts.CompilerHost {
  const compilerHost: ts.CompilerHost = ts.createCompilerHost(etsCheckerCompilerOptions);
  compilerHost.writeFile = (): void => {};
  compilerHost.resolveModuleNames = resolveModuleNames;
  compilerHost.getCurrentDirectory = (): string => process.cwd();
  compilerHost.getDefaultLibFileName = (options: ts.CompilerOptions): string => ts.getDefaultLibFilePath(options);
  compilerHost.resolveTypeReferenceDirectives = resolveTypeReferenceDirectives;
  return compilerHost;
}

let compilerHost: ts.CompilerHost = null;

if (!compilerHost) {
  compilerHost = getCompilerHost();
}


const arkoalaTsProgramCache: WeakMap<ts.Program, ts.Program> = new WeakMap();

function getArkoalaTsProgram(program: ts.Program): ts.Program {
  let extendedProgram = arkoalaTsProgramCache.get(program);
  if (!extendedProgram) {
    const pluginOptions: ArkoalaPluginOptions = {};
    // This is a stub for the interface generated by ts-patch.
    // Probably we can use the reported diagnostics in the output
    const pluginTransformExtras: Object = {
      diagnostics: [],
      addDiagnostic(): number {return 0},
      removeDiagnostic(): void {},
      ts: ts,
      library: 'typescript',
    };
    extendedProgram = arkoalaProgramTransform(program, compilerHost, pluginOptions, pluginTransformExtras);
    arkoalaTsProgramCache.set(program, extendedProgram);
  }
  return extendedProgram;
}

async function transform(code: string, id: string) {
  const hookEventFactory: CompileEvent = getHookEventFactory(this.share, 'etsTransform', 'transform');
  if (!filter(id)) {
    return null;
  }
  const recordInfo = MemoryMonitor.recordStage(MemoryDefine.STORED_FILE_INFO_TRANSFORM);
  storedFileInfo.collectTransformedFiles(path.resolve(id));
  MemoryMonitor.stopRecordStage(recordInfo);
  const logger = this.share.getLogger('etsTransform');
  const hvigorLogger = this.share.getHvigorConsoleLogger?.(ARKUI_SUBSYSTEM_CODE);

  if (projectConfig.compileMode !== 'esmodule') {
    const eventEtsTransformForJsbundle = createAndStartEvent(hookEventFactory, 'transform for jsbundle');
    let compilerOptions = ts.readConfigFile(
      path.resolve(__dirname, '../../../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
    const componentPaths: string[] | undefined = getExternalComponentPaths();
    if (componentPaths) {
      for (const componentPath of componentPaths) {
        if (!fs.existsSync(componentPath)) {
          continue;
        }
        const externalCompilerOptions: ts.CompilerOptions = ts.readConfigFile(
          path.resolve(componentPath, 'externalconfig.json'), ts.sys.readFile
        ).config.compilerOptions;
        concatenateEtsOptions(compilerOptions, externalCompilerOptions);
      }
    }
    compilerOptions.moduleResolution = 'nodenext';
    compilerOptions.module = 'es2020';
    const newContent: string = jsBundlePreProcess(code, id, this.getModuleInfo(id).isEntry, logger, hvigorLogger);
    const metaInfo: Object = this.getModuleInfo(id).metaInfo || {};
    metaInfo.checker = globalProgram.program.getTypeChecker();
    const result: ts.TranspileOutput = ts.transpileModule(newContent, {
      compilerOptions: compilerOptions,
      fileName: id,
      transformers: { before: [processUISyntax(null, false, null, id, metaInfo)] }
    });

    resetCollection();
    if (transformLog && transformLog.errors.length && !projectConfig.ignoreWarning) {
      emitLogInfo(logger, getTransformLog(transformLog), true, id, hvigorLogger);
      resetLog();
    }

    stopEvent(eventEtsTransformForJsbundle);
    return {
      code: result.outputText,
      map: result.sourceMapText ? JSON.parse(result.sourceMapText) : new MagicString(code).generateMap()
    };
  }

  const eventEtsTransformForEsmodule = createAndStartEvent(hookEventFactory, 'transform for esmodule');
  let tsProgram: ts.Program = globalProgram.program;
  let targetSourceFile: ts.SourceFile | undefined = tsProgram.getSourceFile(id);

  // createProgram from the file which does not have corresponding ast from ets-checker's program
  // by those following cases:
  // 1. .ets/.ts imported by .js file with tsc's `allowJS` option is false.
  // 2. .ets/.ts imported by .js file with same name '.d.ts' file which is prior to .js by tsc default resolving
  if (!targetSourceFile) {
    await processNoTargetSourceFile(id, code, eventEtsTransformForEsmodule);
	const recordInfo = MemoryMonitor.recordStage(MemoryDefine.GLOBAL_PROGRAM_GET_CHECKER);
    // init TypeChecker to run binding
    globalProgram.checker = tsProgram.getTypeChecker();
    globalProgram.strictChecker = tsProgram.getLinterTypeChecker();
    MemoryMonitor.stopRecordStage(recordInfo);
    targetSourceFile = tsProgram.getSourceFile(id)!;
    storedFileInfo.reUseProgram = false;
    collectAllFiles(tsProgram);
  } else {
    if (!storedFileInfo.reUseProgram) {
	  const recordInfo = MemoryMonitor.recordStage(MemoryDefine.GLOBAL_PROGRAM_GET_CHECKER);
      globalProgram.checker = globalProgram.program.getTypeChecker();
      globalProgram.strictChecker = globalProgram.program.getLinterTypeChecker();
      MemoryMonitor.stopRecordStage(recordInfo);
    }
    storedFileInfo.reUseProgram = true;
  }
  setPkgNameForFile(this.getModuleInfo(id));
  const eventValidateEts = createAndStartEvent(eventEtsTransformForEsmodule, 'validateEts');
  validateEts(code, id, this.getModuleInfo(id).isEntry, logger, targetSourceFile, hvigorLogger);
  stopEvent(eventValidateEts);
  const emitResult: EmitResult = { outputText: '', sourceMapText: '' };
  const writeFile: ts.WriteFileCallback = (fileName: string, data: string) => {
    if (/.map$/.test(fileName)) {
      emitResult.sourceMapText = data;
    } else {
      emitResult.outputText = data;
    }
  };

  // close `noEmit` to make invoking emit() effective.
  tsProgram.getCompilerOptions().noEmit = false;
  const metaInfo: Object = this.getModuleInfo(id).meta;
  const lazyImportOptions: LazyImportOptions = {
    autoLazyImport: this.share.projectConfig?.autoLazyImport ?? false,
    reExportCheckMode: this.share.projectConfig?.reExportCheckMode ?? reExportNoCheckMode
  };
  // use `try finally` to restore `noEmit` when error thrown by `processUISyntax` in preview mode
  const eventShouldEmitJs = createAndStartEvent(eventEtsTransformForEsmodule, 'shouldEmitJs');
  const shouldEmitJsFlag: boolean = getShouldEmitJs(projectConfig.shouldEmitJs, targetSourceFile, id);
  shouldEmitJsFlagMap.set(id, shouldEmitJsFlag);
  stopEvent(eventShouldEmitJs);
  let transformResult: ts.TransformationResult<ts.SourceFile> = null;
  ProjectCollections.projectWhiteListManager?.setCurrentCollector(id);
  try {
    const eventTsProgramEmit = createAndStartEvent(eventEtsTransformForEsmodule, 'tsProgramEmit');
    const recordInfo = MemoryMonitor.recordStage(MemoryDefine.GLOBAL_PROGRAM_UI_KIT);
    metaInfo.checker = tsProgram.getTypeChecker();
    if (projectConfig.useArkoala) {
      tsProgram = getArkoalaTsProgram(tsProgram);
      targetSourceFile = tsProgram.getSourceFile(id);
    }
    if (shouldEmitJsFlag) {
      const eventEmit = createAndStartEvent(eventTsProgramEmit, 'emit');
      const uiKitrecordInfo = MemoryMonitor.recordStage(MemoryDefine.GLOBAL_PROGRAM_UI_KIT);
      tsProgram.emit(targetSourceFile, writeFile, undefined, undefined,
        {
          before: [
            processUISyntax(null, false, eventEmit, id, this.share, metaInfo),
            processKitImport(id, metaInfo, eventEmit, true, lazyImportOptions),
            collectReservedNameForObf(this.share.arkProjectConfig?.obfuscationMergedObConfig,
              shouldETSOrTSFileTransformToJSWithoutRemove(id, projectConfig, metaInfo))
          ]
        }
      );
      MemoryMonitor.stopRecordStage(uiKitrecordInfo);
      stopEvent(eventEmit);
    } else {
      const uiKitrecordInfo = MemoryMonitor.recordStage(MemoryDefine.GLOBAL_PROGRAM_UI_KIT);
      const eventTransformNodes = createAndStartEvent(eventTsProgramEmit, 'transformNodes');
      const emitResolver: ts.EmitResolver = globalProgram.checker.getEmitResolver(outFile(tsProgram.getCompilerOptions()) ?
        undefined : targetSourceFile, undefined);
      metaInfo.checker = tsProgram.getTypeChecker();
      transformResult = ts.transformNodes(emitResolver, tsProgram.getEmitHost?.(), ts.factory,
        tsProgram.getCompilerOptions(), [targetSourceFile],
        [processUISyntax(null, false, eventTransformNodes, id, this.share, metaInfo),
        processKitImport(id, metaInfo, eventTransformNodes, false, lazyImportOptions),
        collectReservedNameForObf(this.share.arkProjectConfig?.obfuscationMergedObConfig,
          shouldETSOrTSFileTransformToJSWithoutRemove(id, projectConfig, metaInfo))], false);
      stopEvent(eventTransformNodes);
      MemoryMonitor.stopRecordStage(uiKitrecordInfo);
    }
    stopEvent(eventTsProgramEmit);
    MemoryMonitor.stopRecordStage(recordInfo);
  } finally {
    // restore `noEmit` to prevent tsc's watchService emitting automatically.
    tsProgram.getCompilerOptions().noEmit = true;
  }

  ts.MemoryUtils.tryGC();
  resetCollection();
  processStructComponentV2.resetStructMapInEts();
  if (((transformLog && transformLog.errors.length) || (kitTransformLog && kitTransformLog.errors.length)) &&
    !projectConfig.ignoreWarning) {
    emitLogInfo(logger, getTransformLog(kitTransformLog), true, id);
    emitLogInfo(logger, getTransformLog(transformLog), true, id, hvigorLogger);
    resetLog();
    resetKitImportLog();
  }

  const result = shouldEmitJsFlag ? {
    code: emitResult.outputText,
    // Use magicString to generate sourceMap because of Typescript do not emit sourceMap in some cases
    map: emitResult.sourceMapText ? JSON.parse(emitResult.sourceMapText) : new MagicString(code).generateMap(),
    meta: {
      shouldEmitJs: true
    }
  } : switchTsAst ? {
    meta: {
      tsAst: transformResult.transformed[0]
    }
  } : printSourceFile(transformResult.transformed[0], eventEtsTransformForEsmodule);

  stopEvent(eventEtsTransformForEsmodule);
  return result;
}

async function processNoTargetSourceFile(id: string, code: string, parentEvent?: CompileEvent): Promise<void> {
  const eventNoSourceFileRebuildProgram = createAndStartEvent(parentEvent, 'noSourceFileRebuildProgram');
  if (storedFileInfo.isFirstBuild && storedFileInfo.changeFiles.length) {
    storedFileInfo.newTsProgram = ts.createProgram(storedFileInfo.changeFiles, etsCheckerCompilerOptions, compilerHost);
    storedFileInfo.isFirstBuild = false;
  }
  await CreateProgramMoment.block(id, code);
  CreateProgramMoment.release(id);
  globalProgram.program.initProcessingFiles();
  for (const root of CreateProgramMoment.getRoots(id, code)) {
    if (!globalProgram.program.getSourceFile(root.id)) {
      const newSourceFile: ts.SourceFile = ts.createSourceFile(root.id, root.code, etsCheckerCompilerOptions.target,
        true, undefined, etsCheckerCompilerOptions);
      newSourceFile.originalFileName = newSourceFile.fileName;
      newSourceFile.resolvePath = root.id;
      newSourceFile.path = root.id;
      globalProgram.program.processImportedModules(newSourceFile, true);
      globalProgram.program.setProgramSourceFiles(newSourceFile);
      CreateProgramMoment.deleteFileCollect.add(newSourceFile.fileName);
    }
  }
  const processingFiles: ts.SourceFile[] = globalProgram.program.getProcessingFiles();
  if (processingFiles) {
    processingFiles.forEach(file => {
      if (!globalProgram.program.getSourceFiles().includes(file.fileName)) {
        CreateProgramMoment.deleteFileCollect.add(file.fileName);
      }
      globalProgram.program.setProgramSourceFiles(file);
    });
  }
  globalProgram.program.refreshTypeChecker();
  stopEvent(eventNoSourceFileRebuildProgram);
}

function printSourceFile(sourceFile: ts.SourceFile, parentEvent: CompileEvent): string | null {
  if (sourceFile) {
    const eventPrintNode = createAndStartEvent(parentEvent, 'printNode');
    const printer: ts.Printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
    const sourceCode: string = printer.printNode(ts.EmitHint.Unspecified, sourceFile, sourceFile);
    stopEvent(eventPrintNode);
    return sourceCode;
  }
  return null;
}

function outFile(options: ts.CompilerOptions): string {
  return options.outFile || options.out;
}

function getShouldEmitJs(shouldEmitJs: boolean, targetSourceFile: ts.SourceFile, id: string): boolean {
  let shouldEmitJsFlag: boolean = true;
  let hasKeepTs: boolean = false;
  if (!projectConfig.processTs) {
    return shouldEmitJsFlag;
  }
  if (projectConfig.complieHar) {
    if (!projectConfig.UseTsHar && !projectConfig.byteCodeHar) {
      return shouldEmitJsFlag;
    }
  } else {
    hasKeepTs = checkHasKeepTs(targetSourceFile);
  }
  // FA model/code coverage instrumentation/default situation
  // These three situations require calling the emit interface, while in other cases 'shouldEmitJs' be false.
  // The 'shouldEmitJS' variable is obtained through 'this.share.sprojectConfig'.
  if (shouldEmitJs !== undefined) {
    // ark es2abc
    shouldEmitJsFlag = shouldEmitJs || ts.hasTsNoCheckOrTsIgnoreFlag(targetSourceFile) && !hasKeepTs;
  }
  return shouldEmitJsFlag;
}

function setPkgNameForFile(moduleInfo: Object): void {
  if (moduleInfo && moduleInfo.meta && moduleInfo.meta.pkgName) {
    storedFileInfo.getCurrentArkTsFile().pkgName = moduleInfo.meta.pkgName;
  }
}

function validateEts(code: string, id: string, isEntry: boolean, logger: Object, sourceFile: ts.SourceFile, 
  hvigorLogger: Object | undefined = undefined): void {
  if (/\.ets$/.test(id)) {
    clearCollection();
    const fileQuery: string = isEntry && !abilityPagesFullPath.has(path.resolve(id).toLowerCase()) ? '?entry' : '';
    const log: LogInfo[] = validateUISyntax(code, code, id, fileQuery, sourceFile);
    if (log.length && !projectConfig.ignoreWarning) {
      emitLogInfo(logger, log, true, id, hvigorLogger);
    }
  }
}

function jsBundlePreProcess(code: string, id: string, isEntry: boolean, logger: Object, 
  hvigorLogger: Object | undefined = undefined): string {
  if (/\.ets$/.test(id)) {
    clearCollection();
    let content = preprocessExtend(code);
    content = preprocessNewExtend(content);
    const fileQuery: string = isEntry && !abilityPagesFullPath.has(path.resolve(id).toLowerCase()) ? '?entry' : '';
    const log: LogInfo[] = validateUISyntax(code, content, id, fileQuery);
    if (log.length && !projectConfig.ignoreWarning) {
      emitLogInfo(logger, log, true, id, hvigorLogger);
    }
    return content;
  }
  return code;
}

function clearCollection(): void {
  componentCollection.customComponents.clear();
  CUSTOM_BUILDER_METHOD.clear();
  INNER_CUSTOM_LOCALBUILDER_METHOD.clear();
  GLOBAL_CUSTOM_BUILDER_METHOD.clear();
  INNER_CUSTOM_BUILDER_METHOD.clear();
  storedFileInfo.getCurrentArkTsFile().compFromDETS.clear();
  storedFileInfo.getCurrentArkTsFile().recycleComponents.clear();
}

function resetCollection() {
  componentInfo.id = 0;
  propertyCollection.clear();
  linkCollection.clear();
  EXTEND_ATTRIBUTE.clear();
  storedFileInfo.getCurrentArkTsFile().reuseComponentsV2.clear();
  resetComponentCollection();
  storedFileInfo.hasLocalBuilderInFile = false;
}

function resetEtsTransform(): void {
  ShouldEnableDebugLine.enableDebugLine = false;
  projectConfig.ignoreWarning = false;
  projectConfig.widgetCompile = false;
  compilerHost = null;
  disableCacheOptions = {
    bundleName: 'default',
    entryModuleName: 'default',
    runtimeOS: 'default',
    resourceTableHash: 'default',
    etsLoaderVersion: 'default'
  };
}

function findArkoalaRoot(): string {
  let arkoalaSdkRoot: string;
  if (process.env.ARKOALA_SDK_ROOT) {
    arkoalaSdkRoot = process.env.ARKOALA_SDK_ROOT;
    if (!isDir(arkoalaSdkRoot)) {
      throw new Error('Arkoala SDK not found in ' + arkoalaSdkRoot);
    }
  } else {
    const arkoalaPossiblePaths: string[] = globalModulePaths.map(dir => path.join(dir, '../../arkoala'));
    arkoalaSdkRoot = arkoalaPossiblePaths.find(possibleRootDir => isDir(possibleRootDir)) ?? '';
    if (!arkoalaSdkRoot) {
      throw new Error('Arkoala SDK not found in ' + arkoalaPossiblePaths.join(';'));
    }
  }

  return arkoalaSdkRoot;
}

function isDir(filePath: string): boolean {
  try {
    let stat: fs.Stats = fs.statSync(filePath);
    return stat.isDirectory();
  } catch (e) {
    return false;
  }
}

function setIncrementalFileInHar(cacheFilePath: string, buildFilePath: string, allFilesInHar: Map<string, string>): void {
  if (cacheFilePath.match(/\.d.e?ts$/)) {
    allFilesInHar.set(cacheFilePath, buildFilePath);
    return;
  }
  let extName = projectConfig.useTsHar ? '.ts' : '.js';
  allFilesInHar.set(cacheFilePath.replace(/\.ets$/, '.d.ets').replace(/\.ts$/, '.d.ts'),
    buildFilePath.replace(/\.ets$/, '.d.ets').replace(/\.ts$/, '.d.ts'));
  allFilesInHar.set(cacheFilePath.replace(/\.e?ts$/, extName), buildFilePath.replace(/\.e?ts$/, extName));
}

function checkRelateToConstEnum(id: string): boolean {
  let tsProgram: ts.Program = globalProgram.builderProgram;
  let targetSourceFile: ts.SourceFile | undefined = tsProgram ? tsProgram.getSourceFile(id) : undefined;
  if (!targetSourceFile) {
    return false;
  }
  if (!tsProgram.isFileUpdateInConstEnumCache) {
    return false;
  }
  return tsProgram.isFileUpdateInConstEnumCache(targetSourceFile);
}

interface moduleInfoType {
  id: string;
};

interface optionsType {
  id: string;
};

interface newSourceFileType {
  id: string;
  code: string;
};

class CreateProgramMoment {
  static transFileCollect: Set<string> = new Set();
  static awaitFileCollect: Set<string> = new Set();
  static deleteFileCollect: Set<string> = new Set();
  static moduleParsedFileCollect: Set<string> = new Set();
  static promise: Promise<void> = undefined;
  static emitter = undefined;
  static roots: Map<string, string> = new Map();

  static init(): void {
    if (CreateProgramMoment.promise) {
      return;
    }
    const commonDtsPath: string = path.resolve(__dirname, '../../../declarations/common.d.ts');
    CreateProgramMoment.roots.set(commonDtsPath, fs.readFileSync(commonDtsPath, 'utf-8'));
    CreateProgramMoment.emitter = new nodeEvents.EventEmitter();
    CreateProgramMoment.promise = new Promise<void>(resolve => {
      CreateProgramMoment.emitter.on('checkPrefCreateProgramId', () => {
        if (CreateProgramMoment.awaitFileCollect.size + CreateProgramMoment.moduleParsedFileCollect.size ===
          CreateProgramMoment.transFileCollect.size) {
          resolve();
        }
      });
    });
  }

  static getPlugin() {
    return {
      name: 'createProgramPlugin',
      load: {
        order: 'pre',
        handler(id: string): void {
          CreateProgramMoment.transFileCollect.add(id);
        }
      },

      moduleParsed(moduleInfo: moduleInfoType): void {
        CreateProgramMoment.moduleParsedFileCollect.add(moduleInfo.id);
        CreateProgramMoment.emitter?.emit('checkPrefCreateProgramId');
      },
      cleanUp(): void {
        shouldEmitJsFlagMap.clear();
        CreateProgramMoment.reset();
      }
    };
  }

  static async block(id: string, code: string): Promise<void> {
    CreateProgramMoment.init();
    CreateProgramMoment.awaitFileCollect.add(id);
    CreateProgramMoment.roots.set(id, code);
    CreateProgramMoment.emitter.emit('checkPrefCreateProgramId');
    return CreateProgramMoment.promise;
  }

  static release(id: string): void {
    CreateProgramMoment.awaitFileCollect.delete(id);
  }

  static reset(): void {
    CreateProgramMoment.transFileCollect.clear();
    CreateProgramMoment.awaitFileCollect.clear();
    CreateProgramMoment.moduleParsedFileCollect.clear();
    CreateProgramMoment.promise = undefined;
    CreateProgramMoment.emitter = undefined;
    CreateProgramMoment.roots.clear();
  }

  static resetDeleteFiles(): void {
    CreateProgramMoment.deleteFileCollect.clear();
  }

  static getRoots(id: string, code: string): newSourceFileType[] {
    const res: newSourceFileType[] = [];
    for (const [id, code] of CreateProgramMoment.roots) {
      res.push({id, code});
    }
    CreateProgramMoment.promise = undefined;
    CreateProgramMoment.emitter = undefined;
    CreateProgramMoment.roots.clear();
    if (res.length === 0) {
      return [{id, code}];
    }
    return res;
  }
}

exports.createProgramPlugin = CreateProgramMoment.getPlugin;
