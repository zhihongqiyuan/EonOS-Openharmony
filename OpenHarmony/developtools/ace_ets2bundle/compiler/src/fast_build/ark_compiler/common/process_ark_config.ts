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
import fs from 'fs';
import {
  ArkObfuscator,
  initObfuscationConfig,
  readProjectPropertiesByCollectedPaths,
  performancePrinter,
  EventList,
  blockPrinter,
  endFilesEvent,
  configurePerformancePrinter,
  startFilesEvent
} from 'arkguard';

import {
  TS2ABC,
  ESMODULE,
  NODE_MODULES,
  OH_MODULES,
  OBFUSCATION_TOOL
} from './ark_define';
import {
  isAotMode,
  isDebug,
  isBranchElimination
} from '../utils';
import {
  isHarmonyOs,
  isLinux,
  isMac,
  isWindows,
  toUnixPath
} from '../../../utils';
import { getArkBuildDir } from '../../../ark_utils';
import { checkAotConfig } from '../../../gen_aot';
import { projectConfig as mainProjectConfig } from '../../../../main';
import type { MergedConfig } from './ob_config_resolver';
import type { ReseverdSetForArkguard } from 'arkguard';
import { MemoryMonitor } from '../../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../../meomry_monitor/memory_define';
import { initObfLogger, printObfLogger } from './ob_config_resolver';
import { BytecodeObfuscator } from '../bytecode_obfuscator';

type ArkConfig = {
  arkRootPath: string;
  ts2abcPath: string;
  js2abcPath: string;
  mergeAbcPath: string;
  es2abcPath: string;
  aotCompilerPath: string;
  bcObfuscatorPath: string;
  nodePath: string;
  isDebug: boolean;
  isBranchElimination: boolean;
  optTryCatchFunc: boolean;
};

export function initArkConfig(projectConfig: Object): ArkConfig {
  let arkRootPath: string = path.join(__dirname, '..', '..', '..', '..', 'bin', 'ark');
  if (projectConfig.arkFrontendDir) {
    arkRootPath = projectConfig.arkFrontendDir;
  }
  let arkConfig: ArkConfig = {
    arkRootPath: '',
    ts2abcPath: '',
    js2abcPath: '',
    mergeAbcPath: '',
    es2abcPath: '',
    aotCompilerPath: '',
    bcObfuscatorPath: '',
    nodePath: '',
    isDebug: false,
    isBranchElimination: false,
    optTryCatchFunc: false
  };
  arkConfig.nodePath = 'node';
  if (projectConfig.nodeJs) {
    arkConfig.nodePath = projectConfig.nodePath;
  }
  arkConfig.isDebug = isDebug(projectConfig);
  arkConfig.isBranchElimination = isBranchElimination(projectConfig);
  arkConfig.optTryCatchFunc = mainProjectConfig.optTryCatchFunc;
  arkConfig.arkRootPath = arkRootPath;
  processPlatformInfo(arkConfig);
  processCompatibleVersion(projectConfig, arkConfig);
  return arkConfig;
}

export function initArkProjectConfig(share: Object): Object {
  let projectConfig: Object = share.projectConfig;
  let arkProjectConfig: Object = {};
  let entryPackageName: string = share.projectConfig.entryPackageName || '';
  let entryModuleVersion: string = share.projectConfig.entryModuleVersion || '';
  arkProjectConfig.entryPackageInfo = `${entryPackageName}|${entryModuleVersion}`;
  arkProjectConfig.projectRootPath = share.projectConfig.projectTopDir;
  if (projectConfig.aceBuildJson && fs.existsSync(projectConfig.aceBuildJson)) {
    const buildJsonInfo = JSON.parse(fs.readFileSync(projectConfig.aceBuildJson).toString());
    arkProjectConfig.projectRootPath = buildJsonInfo.projectRootPath;
    arkProjectConfig.modulePathMap = buildJsonInfo.modulePathMap;
    arkProjectConfig.isOhosTest = buildJsonInfo.isOhosTest;
    arkProjectConfig.arkRouterMap = buildJsonInfo.routerMap;
    // Collect bytecode har's declaration files entries include dynamic import and workers, use
    // by es2abc for dependency resolution.
    arkProjectConfig.declarationEntry = buildJsonInfo.declarationEntry;
    if (buildJsonInfo.patchConfig) {
      arkProjectConfig.oldMapFilePath = buildJsonInfo.patchConfig.oldMapFilePath;
    }
    if (checkAotConfig(projectConfig.compileMode, buildJsonInfo,
      (error: string) => { share.throwArkTsCompilerError(error) })) {
      arkProjectConfig.processTs = true;
      arkProjectConfig.pandaMode = TS2ABC;
      arkProjectConfig.anBuildOutPut = buildJsonInfo.anBuildOutPut;
      arkProjectConfig.anBuildMode = buildJsonInfo.anBuildMode;
      arkProjectConfig.apPath = buildJsonInfo.apPath;
    } else {
      arkProjectConfig.processTs = false;
      arkProjectConfig.pandaMode = buildJsonInfo.pandaMode;
    }

    if (projectConfig.compileMode === ESMODULE) {
      arkProjectConfig.nodeModulesPath = buildJsonInfo.nodeModulesPath;
      // harNameOhmMap stores har packages that are dependent for bytecode har when compile bytecode har.
      arkProjectConfig.harNameOhmMap = buildJsonInfo.harNameOhmMap;
      arkProjectConfig.hspNameOhmMap = buildJsonInfo.hspNameOhmMap;
      projectConfig.packageDir = buildJsonInfo.packageManagerType === 'ohpm' ? OH_MODULES : NODE_MODULES;
    }
    if (buildJsonInfo.dynamicImportLibInfo) {
      arkProjectConfig.dynamicImportLibInfo = buildJsonInfo.dynamicImportLibInfo;
    }
    if (buildJsonInfo.byteCodeHarInfo) {
      arkProjectConfig.byteCodeHarInfo = buildJsonInfo.byteCodeHarInfo;
    }
    // Currently, the IDE does not have this configuration option, and cacheBytecodeHar is true by default.
    arkProjectConfig.cacheBytecodeHar = !Object.prototype.hasOwnProperty.call(buildJsonInfo, 'cacheBytecodeHar') ||
      buildJsonInfo.cacheBytecodeHar;
  }
  if (projectConfig.aceManifestPath && fs.existsSync(projectConfig.aceManifestPath)) {
    const manifestJsonInfo = JSON.parse(fs.readFileSync(projectConfig.aceManifestPath).toString());
    if (manifestJsonInfo.minPlatformVersion) {
      arkProjectConfig.minPlatformVersion = manifestJsonInfo.minPlatformVersion;
    }
  }
  if (projectConfig.aceModuleJsonPath && fs.existsSync(projectConfig.aceModuleJsonPath)) {
    const moduleJsonInfo = JSON.parse(fs.readFileSync(projectConfig.aceModuleJsonPath).toString());
    if (moduleJsonInfo.app.minAPIVersion) {
      arkProjectConfig.minPlatformVersion = moduleJsonInfo.app.minAPIVersion;
    }
    if (moduleJsonInfo.module) {
      arkProjectConfig.moduleName = moduleJsonInfo.module.name;
    }
    if (moduleJsonInfo.app) {
      arkProjectConfig.bundleName = moduleJsonInfo.app.bundleName;
    }
  }

  // pass allowEtsAnnotations to arkProjectConfig
  arkProjectConfig.allowEtsAnnotations = mainProjectConfig.allowEtsAnnotations;

  // Hotreload attributes are initialized by arkui in main.js, just copy them.
  arkProjectConfig.hotReload = mainProjectConfig.hotReload;
  arkProjectConfig.coldReload = mainProjectConfig.coldReload;
  arkProjectConfig.isFirstBuild = mainProjectConfig.isFirstBuild;
  arkProjectConfig.patchAbcPath = mainProjectConfig.patchAbcPath;
  arkProjectConfig.changedFileList = mainProjectConfig.changedFileList;

  if (mainProjectConfig.es2abcCompileTsInAotMode || mainProjectConfig.es2abcCompileTsInNonAotMode) {
    arkProjectConfig.pandaMode = mainProjectConfig.pandaMode;
    arkProjectConfig.processTs = mainProjectConfig.processTs;
  }
  arkProjectConfig.compileMode = projectConfig.compileMode;
  arkProjectConfig.entryObj = mainProjectConfig.entryObj;
  arkProjectConfig.entryArrayForObf = mainProjectConfig.entryArrayForObf;
  arkProjectConfig.cardEntryObj = mainProjectConfig.cardEntryObj;
  if (mainProjectConfig.updateVersionInfo) {
    arkProjectConfig.updateVersionInfo = mainProjectConfig.updateVersionInfo;
  }
  if (!isDebug(projectConfig)) {
    arkProjectConfig.useTsHar = mainProjectConfig.useTsHar;
    // Configure the performance printer for obfuscation performance tracking
    configurePerformancePrinter(projectConfig.perf);
    startFilesEvent(EventList.OBFUSCATION_INITIALIZATION, performancePrinter.timeSumPrinter);
    const recordInfo = MemoryMonitor.recordStage(MemoryDefine.INIT_ARK_PROJECT_CONFIG);
    MemoryMonitor.stopRecordStage(recordInfo);
    initObfLogger(share);
    initObfuscationConfig(projectConfig, arkProjectConfig, printObfLogger);
    endFilesEvent(EventList.OBFUSCATION_INITIALIZATION, performancePrinter.timeSumPrinter);
    BytecodeObfuscator.init(share, arkProjectConfig);
  } else {
    // Set performance printer to undefined in case we cannot disable it without obfuscation initialization
    blockPrinter();
  }
  return arkProjectConfig;
}

function initTerserConfig(projectConfig: any, logger: any, mergedObConfig: MergedConfig, isHarCompiled: boolean): any {
  const isCompact = projectConfig.obfuscationOptions ? mergedObConfig.options.compact : isHarCompiled;
  const minifyOptions = {
    format: {
      beautify: !isCompact,
      indent_level: 2
    },
    compress: {
      join_vars: false,
      sequences: 0,
      directives: false,
      drop_console: mergedObConfig.options.removeLog
    },
    mangle: {
      reserved: mergedObConfig.reservedNames,
      toplevel: mergedObConfig.options.enableToplevelObfuscation
    }
  };
  const applyNameCache: string | undefined = mergedObConfig.options.applyNameCache;
  if (applyNameCache && applyNameCache.length > 0) {
    if (fs.existsSync(applyNameCache)) {
      minifyOptions.nameCache = JSON.parse(fs.readFileSync(applyNameCache, 'utf-8'));
    } else {
      logger.error(`ArkTS:ERROR Namecache file ${applyNameCache} does not exist`);
    }
  } else {
    if (projectConfig.obfuscationOptions && projectConfig.obfuscationOptions.obfuscationCacheDir) {
      const defaultNameCachePath: string = path.join(projectConfig.obfuscationOptions.obfuscationCacheDir, 'nameCache.json');
      if (fs.existsSync(defaultNameCachePath)) {
        minifyOptions.nameCache = JSON.parse(fs.readFileSync(defaultNameCachePath, 'utf-8'));
      } else {
        minifyOptions.nameCache = {};
      }
    }
  }

  if (mergedObConfig.options.enablePropertyObfuscation) {
    minifyOptions.mangle.properties = {
      reserved: mergedObConfig.reservedPropertyNames,
      keep_quoted: !mergedObConfig.options.enableStringPropertyObfuscation
    };
  }
  return minifyOptions;
}

// Scan the source code of project and libraries to collect whitelists.
export function readProjectAndLibsSource(
  allFiles: Set<string>,
  mergedObConfig: MergedConfig,
  arkObfuscator: ArkObfuscator,
  isHarCompiled: boolean,
  keepFilesAndDependencies: Set<string>
): void {
  if (mergedObConfig?.options === undefined || mergedObConfig.options.disableObfuscation || allFiles.size === 0) {
    return;
  }

  const obfOptions = mergedObConfig.options;
  const enableDecoratorScan =
    BytecodeObfuscator.enable && !BytecodeObfuscator.getInstance().isDecoratorObfuscationEnabled();
  let projectAndLibs: ReseverdSetForArkguard = readProjectPropertiesByCollectedPaths(allFiles,
    {
      mNameObfuscation: {
        mEnable: true,
        mReservedProperties: [],
        mRenameProperties: obfOptions.enablePropertyObfuscation,
        mKeepStringProperty: !obfOptions.enableStringPropertyObfuscation,
        mEnableAtKeep: obfOptions.enableAtKeep
      },
      mExportObfuscation: obfOptions.enableExportObfuscation,
      mKeepFileSourceCode: {
        mKeepSourceOfPaths: new Set(),
        mkeepFilesAndDependencies: keepFilesAndDependencies,
      }
    }, isHarCompiled,
    enableDecoratorScan);
  if (obfOptions.enablePropertyObfuscation) {
    arkObfuscator.addReservedSetForPropertyObf(projectAndLibs);
  }
  if (obfOptions.enableExportObfuscation) {
    arkObfuscator.addReservedSetForDefaultObf(projectAndLibs);
  }
  if (obfOptions.enableAtKeep) {
    // emit atKeep names and consumer configs
    arkObfuscator.obfConfigResolver.emitConsumerConfigFiles();
  }
}

function processPlatformInfo(arkConfig: ArkConfig): void {
  const arkPlatformPath: string = getArkBuildDir(arkConfig.arkRootPath);
  if (isWindows()) {
    arkConfig.es2abcPath = path.join(arkPlatformPath, 'bin', 'es2abc.exe');
    arkConfig.ts2abcPath = path.join(arkPlatformPath, 'src', 'index.js');
    arkConfig.mergeAbcPath = path.join(arkPlatformPath, 'bin', 'merge_abc.exe');
    arkConfig.js2abcPath = path.join(arkPlatformPath, 'bin', 'js2abc.exe');
    arkConfig.aotCompilerPath = path.join(arkPlatformPath, 'bin', 'ark_aot_compiler.exe');
    arkConfig.bcObfuscatorPath = path.join(arkPlatformPath, 'bin', 'panda_guard.exe');
    return;
  }
  if (isLinux() || isMac()) {
    arkConfig.es2abcPath = path.join(arkPlatformPath, 'bin', 'es2abc');
    arkConfig.ts2abcPath = path.join(arkPlatformPath, 'src', 'index.js');
    arkConfig.mergeAbcPath = path.join(arkPlatformPath, 'bin', 'merge_abc');
    arkConfig.js2abcPath = path.join(arkPlatformPath, 'bin', 'js2abc');
    arkConfig.aotCompilerPath = path.join(arkPlatformPath, 'bin', 'ark_aot_compiler');
    arkConfig.bcObfuscatorPath = path.join(arkPlatformPath, 'bin', 'panda_guard');
    return;
  }
  if (isHarmonyOs()) {
    arkConfig.es2abcPath = path.join(arkPlatformPath, 'bin', 'es2abc');
    return;
  }
}

function processCompatibleVersion(projectConfig: Object, arkConfig: ArkConfig): void {
  const platformPath: string = getArkBuildDir(arkConfig.arkRootPath);
  if (projectConfig.minPlatformVersion && projectConfig.minPlatformVersion.toString() === '8') {
    // use ts2abc to compile apps with 'CompatibleSdkVersion' set to 8
    arkConfig.ts2abcPath = path.join(platformPath, 'legacy_api8', 'src', 'index.js');
    projectConfig.pandaMode = TS2ABC;
  }
}

export const utProcessArkConfig = {
  processCompatibleVersion,
  initTerserConfig
};
