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

import * as fs from 'fs';
import * as path from 'path';
import cluster from 'cluster';
import process from 'process';
import os from 'os';
import events from 'events';
import Compiler from 'webpack/lib/Compiler';
import { logger } from './compile_info';
import * as childProcess from 'child_process';
import {
  toUnixPath,
  toHashData,
  mkdirsSync,
  nodeLargeOrEqualTargetVersion,
  removeDir,
  validateFilePathLength,
  unlinkSync,
  isPackageModulesFile,
  genTemporaryPath
} from './utils';
import {
  buildCachePath,
  genAbcFileName,
  genBuildPath,
  genMergeProtoFileName,
  genProtoFileName,
  getOhmUrlByFilepath,
  getPackageInfo,
  isEs2Abc,
  isTs2Abc,
  newSourceMaps,
  removeDuplicateInfo
} from './ark_utils';
import { projectConfig } from '../main';
import {
  ESMODULE,
  JSBUNDLE,
  NODE_MODULES,
  ES2ABC,
  EXTNAME_D_ETS,
  EXTNAME_D_TS,
  EXTNAME_ETS,
  EXTNAME_JS,
  EXTNAME_TS,
  EXTNAME_MJS,
  EXTNAME_CJS,
  EXTNAME_JSON,
  EXTNAME_JS_MAP,
  FAIL,
  MODULELIST_JSON,
  MODULES_ABC,
  PREBUILDINFO_JSON,
  SUCCESS,
  SOURCEMAPS_JSON,
  SOURCEMAPS,
  TEMPORARY,
  TS2ABC,
  PROTO_FILESINFO_TXT,
  NPMENTRIES_TXT,
  EXTNAME_PROTO_BIN,
  FILESINFO_TXT,
  MANAGE_WORKERS_SCRIPT,
  MAX_WORKER_NUMBER,
  GEN_ABC_SCRIPT,
  GEN_MODULE_ABC_SCRIPT,
  AOT_FULL,
  AOT_PARTIAL,
  PACKAGES
} from './pre_define';
import {
  generateMergedAbc,
  generateNpmEntriesInfo
} from './gen_merged_abc';
import {
  generateAot,
  FaultHandler
} from './gen_aot';

let output: string;
let isWin: boolean = false;
let isMac: boolean = false;
let isDebug: boolean = false;
let arkDir: string;
let nodeJs: string;

interface File {
  path: string,
  size: number,
  cacheOutputPath: string,
  sourceFile: string
}
let intermediateJsBundle: Array<File> = [];
let fileterIntermediateJsBundle: Array<File> = [];
let moduleInfos: Array<ModuleInfo> = [];
let filterModuleInfos: Array<ModuleInfo> = [];
let commonJsModuleInfos: Array<ModuleInfo> = [];
let ESMModuleInfos: Array<ModuleInfo> = [];
let entryInfos: Map<string, EntryInfo> = new Map<string, EntryInfo>();
let hashJsonObject = {};
let moduleHashJsonObject = {};
let buildPathInfo: string = '';
let buildMapFileList: Set<string> = new Set<string>();
let isHotReloadFirstBuild: boolean = true;
let protoFilePath: string = '';

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';
const blue = '\u001b[34m';
const hashFile: string = 'gen_hash.json';
const ARK: string = '/ark/';

export class ModuleInfo {
  filePath: string;
  tempFilePath: string;
  buildFilePath: string;
  abcFilePath: string;
  isCommonJs: boolean;
  recordName: string;
  sourceFile: string;
  packageName: string;

  constructor(filePath: string, tempFilePath: string, buildFilePath: string,
              abcFilePath: string, packageName: string, isCommonJs: boolean) {
    this.filePath = filePath;
    this.tempFilePath = tempFilePath;
    this.buildFilePath = buildFilePath;
    this.abcFilePath = abcFilePath;
    this.packageName = packageName;
    this.isCommonJs = isCommonJs;
    this.recordName = getOhmUrlByFilepath(filePath, projectConfig, logger);
    this.sourceFile = filePath.replace(projectConfig.projectRootPath + path.sep, '');
  }
}

export class EntryInfo {
  npmInfo: string;
  buildPath: string;
  entry: string;

  constructor(npmInfo: string, buildPath: string, entry: string) {
    this.npmInfo = npmInfo;
    this.buildPath = buildPath;
    this.entry = entry;
  }
}

export class GenAbcPlugin {
  constructor(output_, arkDir_, nodeJs_, isDebug_) {
    output = output_;
    arkDir = arkDir_;
    nodeJs = nodeJs_;
    isDebug = isDebug_;
  }
  apply(compiler: Compiler) {
    if (fs.existsSync(path.resolve(arkDir, 'build-win'))) {
      isWin = true;
    } else {
      if (fs.existsSync(path.resolve(arkDir, 'build-mac'))) {
        isMac = true;
      } else {
        if (!fs.existsSync(path.resolve(arkDir, 'build'))) {
          logger.error(red, 'ArkTS:ERROR find build fail', reset);
          process.exitCode = FAIL;
          return;
        }
      }
    }

    if (!checkNodeModules()) {
      process.exitCode = FAIL;
      return;
    }

    if (projectConfig.compileMode === ESMODULE) {
      if (projectConfig.cachePath && !projectConfig.xtsMode) {
        let cachedJson: Object = {};
        const cachePrebuildInfoPath: string = path.join(projectConfig.cachePath, PREBUILDINFO_JSON);
        validateFilePathLength(cachePrebuildInfoPath, logger);
        cachedJson.buildMode = projectConfig.buildArkMode;
        cachedJson.bundleName = projectConfig.bundleName;
        cachedJson.moduleName = projectConfig.moduleName;
        fs.writeFile(cachePrebuildInfoPath, JSON.stringify(cachedJson, null, 2), 'utf-8',
          (err) => {
            if (err) {
              logger.error(red, `ArkTS:ERROR Failed to write module build info.`, reset);
            }
          }
        );
      }

      // clear output dir
      removeDir(output);
      removeDir(projectConfig.nodeModulesPath);
    }

    if (projectConfig.compileMode === JSBUNDLE && process.env.minPlatformVersion) {
      if (projectConfig.cachePath && !projectConfig.xtsMode) {
        let cachedJson: Object = {};
        const cachePrebuildInfoPath: string = path.join(projectConfig.cachePath, PREBUILDINFO_JSON);
        validateFilePathLength(cachePrebuildInfoPath, logger);
        cachedJson.minAPIVersion = process.env.minPlatformVersion;
        fs.writeFile(cachePrebuildInfoPath, JSON.stringify(cachedJson, null, 2), 'utf-8',
          (err) => {
            if (err) {
              logger.error(red, `ArkTS:ERROR Failed to write bundle build info.`, reset);
            }
          }
        );
      }
    }

    // for preview mode max listeners
    events.EventEmitter.defaultMaxListeners = 100;

    compiler.hooks.compilation.tap('GenAbcPlugin', (compilation) => {
      if (projectConfig.compileMode === JSBUNDLE || projectConfig.compileMode === undefined) {
        return;
      }
      buildPathInfo = output;
      compilation.hooks.finishModules.tap('finishModules', handleFinishModules.bind(this));
    });

    compiler.hooks.compilation.tap('GenAbcPlugin', (compilation) => {
      compilation.hooks.processAssets.tap('processAssets', (assets) => {
        if (projectConfig.compileMode === JSBUNDLE || projectConfig.compileMode === undefined) {
          return;
        }
        let newAssets: Object = {};
        Object.keys(compilation.assets).forEach(key => {
          if (path.extname(key) !== EXTNAME_JS && path.extname(key) !== EXTNAME_JS_MAP) {
            newAssets[key] = assets[key];
          }
        });
        compilation.assets = newAssets;
      });
    });

    compiler.hooks.emit.tap('GenAbcPlugin', (compilation) => {
      if (projectConfig.compileMode === ESMODULE) {
        return;
      }
      Object.keys(compilation.assets).forEach(key => {
        // choose *.js
        if (output && path.extname(key) === EXTNAME_JS) {
          const newContent: string = compilation.assets[key].source();
          const keyPath: string = key.replace(/\.js$/, '.temp.js');
          writeFileSync(newContent, output, keyPath, key);
        }
      });
    });

    compiler.hooks.afterEmit.tap('GenAbcPluginMultiThread', () => {
      if (projectConfig.compileMode === ESMODULE) {
        return;
      }
      if (intermediateJsBundle.length === 0) {
        return;
      }
      buildPathInfo = output;
      if (isTs2Abc(projectConfig) || process.env.minPlatformVersion === '8') {
        invokeWorkersToGenAbc();
      } else if (isEs2Abc(projectConfig)) {
        generateAbcByEs2AbcOfBundleMode(intermediateJsBundle);
      } else {
        logger.error(red, `ArkTS:ERROR please set panda module`, reset);
      }
    });
  }
}

function clearGlobalInfo() {
  // fix bug of multi trigger
  if (process.env.watchMode !== 'true') {
    intermediateJsBundle = [];
    moduleInfos = [];
    entryInfos = new Map<string, EntryInfo>();
  }
  fileterIntermediateJsBundle = [];
  filterModuleInfos = [];
  commonJsModuleInfos = [];
  ESMModuleInfos = [];
  hashJsonObject = {};
  moduleHashJsonObject = {};
  buildMapFileList = new Set<string>();
}

function getEntryInfo(filePath: string, resourceResolveData: Object): string {
  if (!resourceResolveData.descriptionFilePath) {
    return '';
  }

  let isEntry: boolean = false;
  let mainFileds: Set<string> = getEntryCandidatesFromPackageJson(resourceResolveData);
  for (let value of mainFileds.values()) {
    if (toUnixPath(filePath) === value) {
      isEntry = true;
      break;
    }
  }
  const packageJsonPath: string = resourceResolveData.descriptionFilePath;
  let npmInfoPath: string = path.resolve(packageJsonPath, '..');

  let entry: string = toUnixPath(filePath.replace(npmInfoPath, ''));
  if (entry.startsWith('/')) {
    entry = entry.slice(1, entry.length);
  }

  const fakeEntryPath: string = path.resolve(npmInfoPath, 'fake.js');
  const tempFakeEntryPath: string = genTemporaryPath(fakeEntryPath, projectConfig.projectPath, process.env.cachePath,
    projectConfig, undefined);
  const buildFakeEntryPath: string = genBuildPath(fakeEntryPath, projectConfig.projectPath, projectConfig.buildPath,
    projectConfig);
  npmInfoPath = toUnixPath(path.resolve(tempFakeEntryPath, '..'));
  const buildNpmInfoPath: string = toUnixPath(path.resolve(buildFakeEntryPath, '..'));
  if (!entryInfos.has(npmInfoPath) && isEntry) {
    const entryInfo: EntryInfo = new EntryInfo(npmInfoPath, buildNpmInfoPath, entry);
    entryInfos.set(npmInfoPath, entryInfo);
  }

  return buildNpmInfoPath;
}

function getEntryCandidatesFromPackageJson(resourceResolveData: Object): Set<string> {
  let descriptionFileData: Object = resourceResolveData.descriptionFileData;
  let packagePath: string = path.resolve(resourceResolveData.descriptionFilePath, '..');
  let mainFileds: Set<string> = new Set<string>();
  if (descriptionFileData.browser) {
    if (typeof descriptionFileData.browser === 'string') {
      mainFileds.add(toUnixPath(path.join(packagePath, descriptionFileData.browser)));
    } else {
      Object.keys(descriptionFileData.browser).forEach(key => {
        if (typeof key === 'string' && typeof descriptionFileData.browser[key] === 'string') {
          mainFileds.add(toUnixPath(path.join(packagePath, descriptionFileData.browser[key])));
        }
      });
    }
  }
  if (descriptionFileData.module) {
    mainFileds.add(toUnixPath(path.join(packagePath, descriptionFileData.module)));
  }
  if (descriptionFileData.main) {
    mainFileds.add(toUnixPath(path.join(packagePath, descriptionFileData.main)));
  }
  if (mainFileds.size === 0) {
    mainFileds.add(toUnixPath(path.join(packagePath, 'index.js')));
    mainFileds.add(toUnixPath(path.join(packagePath, 'index.ets')));
    mainFileds.add(toUnixPath(path.join(packagePath, 'index.ts')));
  }

  return mainFileds;
}

function processNodeModulesFile(filePath: string, tempFilePath: string, buildFilePath: string,
  abcFilePath: string, nodeModulesFile: Array<string>, module: Object): void {
  let npmPkgPath: string = getEntryInfo(filePath, module.resourceResolveData);
  const buildNpmPkgPath: string = npmPkgPath.replace(toUnixPath(projectConfig.nodeModulesPath), '');
  const npmPkgName: string = toUnixPath(path.join(PACKAGES, buildNpmPkgPath)).replace(new RegExp(NODE_MODULES, 'g'), PACKAGES);

  const descriptionFileData: Object = module.resourceResolveData.descriptionFileData;
  if (descriptionFileData && descriptionFileData.type && descriptionFileData.type === 'module') {
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, npmPkgName, false);
    moduleInfos.push(tempModuleInfo);
    nodeModulesFile.push(tempFilePath);
  } else if (filePath.endsWith(EXTNAME_MJS)) {
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, npmPkgName, false);
    moduleInfos.push(tempModuleInfo);
    nodeModulesFile.push(tempFilePath);
  } else {
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, npmPkgName, true);
    moduleInfos.push(tempModuleInfo);
    nodeModulesFile.push(tempFilePath);
  }
  if (!filePath.endsWith(EXTNAME_JSON)) {
    buildMapFileList.add(toUnixPath(filePath.replace(projectConfig.projectRootPath + path.sep, '')));
  }
}

function processEtsModule(filePath: string, tempFilePath: string, buildFilePath: string, nodeModulesFile: Array<string>, module: Object): void {
  // skip declaration modules
  if (filePath.endsWith(EXTNAME_D_ETS)) {
    return;
  }
  if (projectConfig.processTs === true) {
    tempFilePath = tempFilePath.replace(/\.ets$/, EXTNAME_TS);
    buildFilePath = buildFilePath.replace(/\.ets$/, EXTNAME_TS);
  } else {
    tempFilePath = tempFilePath.replace(/\.ets$/, EXTNAME_JS);
    buildFilePath = buildFilePath.replace(/\.ets$/, EXTNAME_JS);
  }
  const abcFilePath: string = genAbcFileName(tempFilePath);
  if (isPackageModulesFile(filePath, projectConfig)) {
    processNodeModulesFile(filePath, tempFilePath, buildFilePath, abcFilePath, nodeModulesFile, module);
  } else {
    const moduleName: string = getPackageInfo(projectConfig.aceModuleJsonPath)[1];
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, moduleName, false);
    moduleInfos.push(tempModuleInfo);
  }
  buildMapFileList.add(toUnixPath(filePath.replace(projectConfig.projectRootPath + path.sep, '')));
}

function processTsModule(filePath: string, tempFilePath: string, buildFilePath: string, nodeModulesFile: Array<string>, module: Object): void {
  // skip declaration modules
  if (filePath.endsWith(EXTNAME_D_TS)) {
    return;
  }
  if (projectConfig.processTs === false) {
    tempFilePath = tempFilePath.replace(/\.ts$/, EXTNAME_JS);
    buildFilePath = buildFilePath.replace(/\.ts$/, EXTNAME_JS);
  }
  const abcFilePath: string = genAbcFileName(tempFilePath);
  if (isPackageModulesFile(filePath, projectConfig)) {
    processNodeModulesFile(filePath, tempFilePath, buildFilePath, abcFilePath, nodeModulesFile, module);
  } else {
    const moduleName: string = getPackageInfo(projectConfig.aceModuleJsonPath)[1];
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, moduleName, false);
    moduleInfos.push(tempModuleInfo);
  }
  buildMapFileList.add(toUnixPath(filePath.replace(projectConfig.projectRootPath + path.sep, '')));
}

function processJsModule(filePath: string, tempFilePath: string, buildFilePath: string, nodeModulesFile: Array<string>, module: Object): void {
  const parent: string = path.join(tempFilePath, '..');
  if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
    mkDir(parent);
  }
  if (filePath.endsWith(EXTNAME_MJS) || filePath.endsWith(EXTNAME_CJS)) {
    fs.copyFileSync(filePath, tempFilePath);
  }
  const abcFilePath: string = genAbcFileName(tempFilePath);
  if (isPackageModulesFile(filePath, projectConfig)) {
    processNodeModulesFile(filePath, tempFilePath, buildFilePath, abcFilePath, nodeModulesFile, module);
  } else {
    const moduleName: string = getPackageInfo(projectConfig.aceModuleJsonPath)[1];
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, moduleName, false);
    moduleInfos.push(tempModuleInfo);
  }
  buildMapFileList.add(toUnixPath(filePath.replace(projectConfig.projectRootPath + path.sep, '')));
}

function processJsonModule(filePath: string, tempFilePath: string, buildFilePath: string, nodeModulesFile: Array<string>, module: Object): void {
  const abcFilePath: string = 'NA';
  if (isPackageModulesFile(filePath, projectConfig)) {
    processNodeModulesFile(filePath, tempFilePath, buildFilePath, abcFilePath, nodeModulesFile, module);
  } else {
    const moduleName: string = getPackageInfo(projectConfig.aceModuleJsonPath)[1];
    const tempModuleInfo: ModuleInfo = new ModuleInfo(filePath, tempFilePath, buildFilePath, abcFilePath, moduleName, false);
    moduleInfos.push(tempModuleInfo);
  }
}

let cachedSourceMaps: Object;

function updateCachedSourceMaps(): void {
  const CACHED_SOURCEMAPS: string = path.join(process.env.cachePath, SOURCEMAPS_JSON);
  validateFilePathLength(CACHED_SOURCEMAPS, logger);
  if (!fs.existsSync(CACHED_SOURCEMAPS)) {
    cachedSourceMaps = {};
  } else {
    cachedSourceMaps = JSON.parse(fs.readFileSync(CACHED_SOURCEMAPS).toString());
  }
  Object.keys(newSourceMaps).forEach(key => {
    cachedSourceMaps[key] = newSourceMaps[key];
  });
}

function getCachedModuleList(): Array<string> {
  const CACHED_MODULELIST_FILE: string = path.join(process.env.cachePath, MODULELIST_JSON);
  validateFilePathLength(CACHED_MODULELIST_FILE, logger);
  if (!fs.existsSync(CACHED_MODULELIST_FILE)) {
    return [];
  }
  const data: Object = JSON.parse(fs.readFileSync(CACHED_MODULELIST_FILE).toString());
  const moduleList: Array<string> = data.list;
  return moduleList;
}

function updateCachedModuleList(moduleList: Array<string>): void {
  const CACHED_MODULELIST_FILE: string = path.join(process.env.cachePath, MODULELIST_JSON);
  validateFilePathLength(CACHED_MODULELIST_FILE, logger);
  const CACHED_SOURCEMAPS: string = path.join(process.env.cachePath, SOURCEMAPS_JSON);
  validateFilePathLength(CACHED_SOURCEMAPS, logger);
  let cachedJson: Object = {};
  cachedJson.list = moduleList;
  fs.writeFile(CACHED_MODULELIST_FILE, JSON.stringify(cachedJson, null, 2), 'utf-8',
    (err) => {
      if (err) {
        logger.error(red, `ArkTS:ERROR Failed to write module list.`, reset);
      }
    }
  );
  fs.writeFile(CACHED_SOURCEMAPS, JSON.stringify(cachedSourceMaps, null, 2), 'utf-8',
    (err) => {
      if (err) {
        logger.error(red, `ArkTS:ERROR Failed to write cache sourceMaps json.`, reset);
      }
    }
  );
}

function writeSourceMaps(): void {
  mkdirsSync(projectConfig.buildPath);
  let sourceMapFilePath: string = path.join(projectConfig.buildPath, SOURCEMAPS);
  validateFilePathLength(sourceMapFilePath, logger);
  fs.writeFile(sourceMapFilePath, JSON.stringify(cachedSourceMaps, null, 2), 'utf-8',
    (err) => {
      if (err) {
        logger.error(red, `ArkTS:ERROR Failed to write sourceMaps.`, reset);
      }
    }
  );
}

function eliminateUnusedFiles(moduleList: Array<string>): void {
  let cachedModuleList: Array<string> = getCachedModuleList();
  if (cachedModuleList.length !== 0) {
    let newCacheSourceMaps: Object = {};
    cachedModuleList.forEach((file) => {
      if (moduleList.includes(file)) {
        newCacheSourceMaps[file] = cachedSourceMaps[file];
      }
    });
    cachedSourceMaps = newCacheSourceMaps;
  }
}

function handleFullModuleFiles(modules, callback): void {
  const nodeModulesFile: Array<string> = [];
  modules.forEach(module => {
    if (module !== undefined && module.resourceResolveData !== undefined) {
      const filePath: string = module.resourceResolveData.path;
      let tempFilePath = genTemporaryPath(filePath, projectConfig.projectPath, process.env.cachePath,
        projectConfig, undefined);
      if (tempFilePath.length === 0) {
        return;
      }
      validateFilePathLength(tempFilePath, logger);
      let buildFilePath: string = genBuildPath(filePath, projectConfig.projectPath, projectConfig.buildPath,
        projectConfig);
      validateFilePathLength(buildFilePath, logger);
      tempFilePath = toUnixPath(tempFilePath);
      buildFilePath = toUnixPath(buildFilePath);

      switch (path.extname(filePath)) {
        case EXTNAME_ETS: {
          processEtsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, module);
          break;
        }
        case EXTNAME_TS: {
          processTsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, module);
          break;
        }
        case EXTNAME_JS:
        case EXTNAME_MJS:
        case EXTNAME_CJS: {
          processJsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, module);
          break;
        }
        case EXTNAME_JSON: {
          processJsonModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, module);
          break;
        }
        default: {
          logger.error(red, `ArkTS:ERROR Cannot find resolve this file path: ${filePath}`, reset);
          process.exitCode = FAIL;
        }
      }
    }
  });

  // for mergeabc source maps
  if (projectConfig.buildArkMode === 'debug') {
    const moduleList: Array<string> = Array.from(buildMapFileList);
    updateCachedSourceMaps();
    eliminateUnusedFiles(moduleList);
    updateCachedModuleList(moduleList);
    writeSourceMaps();
  }

  if (process.env.panda !== TS2ABC) {
    const outputABCPath: string = path.join(projectConfig.buildPath, MODULES_ABC);
    validateFilePathLength(outputABCPath, logger);
    generateMergedAbc(moduleInfos, entryInfos, outputABCPath);
    clearGlobalInfo();
  } else {
    invokeWorkersModuleToGenAbc(moduleInfos);
  }
}

function processEntryToGenAbc(entryInfos: Map<string, EntryInfo>): void {
  if (entryInfos.size <= 0) {
    return;
  }
  generateNpmEntriesInfo(entryInfos);
  const npmEntriesInfoPath: string = path.join(process.env.cachePath, NPMENTRIES_TXT);
  validateFilePathLength(npmEntriesInfoPath, logger);
  let npmEntriesProtoFileName: string = 'npm_entries' + EXTNAME_PROTO_BIN;
  const npmEntriesProtoFilePath: string = path.join(process.env.cachePath, 'protos', 'npm_entries', npmEntriesProtoFileName);
  validateFilePathLength(npmEntriesProtoFilePath, logger);
  mkdirsSync(path.dirname(npmEntriesProtoFilePath));
  let js2Abc: string = path.join(arkDir, 'build', 'bin', 'js2abc');
  if (isWin) {
    js2Abc = path.join(arkDir, 'build-win', 'bin', 'js2abc.exe');
  } else if (isMac) {
    js2Abc = path.join(arkDir, 'build-mac', 'bin', 'js2abc');
  }
  validateFilePathLength(js2Abc, logger);
  const singleCmd: string = `"${js2Abc}" --compile-npm-entries "${npmEntriesInfoPath}" "${npmEntriesProtoFilePath}`;
  try {
    childProcess.execSync(singleCmd);
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR Failed to generate npm proto file to abc, Error message: ${e}`, reset);
  }
}

function writeFileSync(inputString: string, buildPath: string, keyPath: string, jsBundleFile: string): void {
  let output = path.resolve(buildPath, keyPath);
  validateFilePathLength(output, logger);
  let parent: string = path.join(output, '..');
  if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
    mkDir(parent);
  }
  let cacheOutputPath: string = '';
  if (process.env.cachePath) {
    let buildDirArr: string[] = projectConfig.buildPath.split(path.sep);
    let abilityDir: string = buildDirArr[buildDirArr.length - 1];
    cacheOutputPath = path.join(process.env.cachePath, TEMPORARY, abilityDir, keyPath);
  } else {
    cacheOutputPath = output;
  }
  validateFilePathLength(cacheOutputPath, logger);
  parent = path.join(cacheOutputPath, '..');
  if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
    mkDir(parent);
  }
  fs.writeFileSync(cacheOutputPath, inputString);
  if (fs.existsSync(cacheOutputPath)) {
    const fileSize: number = fs.statSync(cacheOutputPath).size;
    let sourceFile: string = output.replace(/\.temp\.js$/, '_.js');
    if (projectConfig.projectRootPath) {
      sourceFile = toUnixPath(sourceFile.replace(projectConfig.projectRootPath + path.sep, ''));
    } else {
      sourceFile = toUnixPath(sourceFile);
    }
    output = toUnixPath(output);
    cacheOutputPath = toUnixPath(cacheOutputPath);

    intermediateJsBundle.push({path: output, size: fileSize, cacheOutputPath: cacheOutputPath, sourceFile: sourceFile});
  } else {
    logger.debug(red, `ArkTS:ERROR Failed to convert file ${jsBundleFile} to bin. ${output} is lost`, reset);
    process.exitCode = FAIL;
  }
}

function mkDir(path_: string): void {
  const parent: string = path.join(path_, '..');
  if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
    mkDir(parent);
  }
  fs.mkdirSync(path_);
}

function getSmallestSizeGroup(groupSize: Map<number, number>): any {
  const groupSizeArray: any = Array.from(groupSize);
  groupSizeArray.sort(function(g1, g2) {
    return g1[1] - g2[1]; // sort by size
  });
  return groupSizeArray[0][0];
}

function splitJsBundlesBySize(bundleArray: Array<File>, groupNumber: number): any {
  const result: any = [];
  if (bundleArray.length < groupNumber) {
    for (const value of bundleArray) {
      result.push([value]);
    }
    return result;
  }

  bundleArray.sort(function(f1: File, f2: File) {
    return f2.size - f1.size;
  });
  const groupFileSize = new Map();
  for (let i = 0; i < groupNumber; ++i) {
    result.push([]);
    groupFileSize.set(i, 0);
  }

  let index = 0;
  while (index < bundleArray.length) {
    const smallestGroup: any = getSmallestSizeGroup(groupFileSize);
    result[smallestGroup].push(bundleArray[index]);
    const sizeUpdate: any = groupFileSize.get(smallestGroup) + bundleArray[index].size;
    groupFileSize.set(smallestGroup, sizeUpdate);
    index++;
  }
  return result;
}

function invokeWorkersModuleToGenAbc(moduleInfos: Array<ModuleInfo>): void {
  invokeClusterModuleToAbc();
}

export function initAbcEnv() : string[] {
  let args: string[] = [];
  if (process.env.minPlatformVersion === '8') {
    process.env.panda = TS2ABC;
    let js2abc: string = path.join(arkDir, 'build', 'legacy_api8', 'src', 'index.js');
    if (isWin) {
      js2abc = path.join(arkDir, 'build-win', 'legacy_api8', 'src', 'index.js');
    } else if (isMac) {
      js2abc = path.join(arkDir, 'build-mac', 'legacy_api8', 'src', 'index.js');
    }
    validateFilePathLength(js2abc, logger);

    js2abc = '"' + js2abc + '"';
    args = [
      '--expose-gc',
      js2abc
    ];
    if (isDebug) {
      args.push('--debug');
    }
  } else if (process.env.panda === TS2ABC) {
    let js2abc: string = path.join(arkDir, 'build', 'src', 'index.js');
    if (isWin) {
      js2abc = path.join(arkDir, 'build-win', 'src', 'index.js');
    } else if (isMac) {
      js2abc = path.join(arkDir, 'build-mac', 'src', 'index.js');
    }
    validateFilePathLength(js2abc, logger);

    js2abc = '"' + js2abc + '"';
    args = [
      '--expose-gc',
      js2abc
    ];
    if (isDebug) {
      args.push('--debug');
    }
  } else if (process.env.panda === ES2ABC || process.env.panda === 'undefined' || process.env.panda === undefined) {
    let es2abc: string = path.join(arkDir, 'build', 'bin', 'es2abc');
    if (isWin) {
      es2abc = path.join(arkDir, 'build-win', 'bin', 'es2abc.exe');
    } else if (isMac) {
      es2abc = path.join(arkDir, 'build-mac', 'bin', 'es2abc');
    }
    validateFilePathLength(es2abc, logger);

    args = [
      '"' + es2abc + '"'
    ];
    if (isDebug) {
      args.push('--debug-info');
    }
    if (projectConfig.compileMode === ESMODULE) {
      args.push('--merge-abc');
    }
  } else {
    logger.error(red, `ArkTS:ERROR please set panda module`, reset);
  }

  return args;
}

function invokeClusterModuleToAbc(): void {
  if (process.env.watchMode === 'true') {
    process.exitCode = SUCCESS;
  }
  filterIntermediateModuleByHashJson(buildPathInfo, moduleInfos);
  const abcArgs: string[] = initAbcEnv();

  const splitedModules: any[] = splitModulesByNumber(filterModuleInfos, MAX_WORKER_NUMBER);
  let cmdPrefix: string = `${nodeJs} ${abcArgs.join(' ')}`;
  const workerNumber: number = MAX_WORKER_NUMBER < splitedModules.length ? MAX_WORKER_NUMBER : splitedModules.length;

  try {
    if (process.env.watchMode === 'true') {
      processWorkersOfPreviewMode(splitedModules, cmdPrefix, workerNumber);
    } else {
      processWorkersOfBuildMode(splitedModules, cmdPrefix, workerNumber);
    }
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR failed to generate abc. Error message: ${e}`, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.watchMode !== 'true') {
      process.exit(FAIL);
    }
  }
}

function splitModulesByNumber(moduleInfos: Array<ModuleInfo>, workerNumber: number): any[] {
  const result: any = [];
  if (moduleInfos.length < workerNumber) {
    for (const value of moduleInfos) {
      result.push([value]);
    }
    return result;
  }

  for (let i = 0; i < workerNumber; ++i) {
    result.push([]);
  }

  for (let i = 0; i < moduleInfos.length; i++) {
    const chunk = i % workerNumber;
    result[chunk].push(moduleInfos[i]);
  }

  return result;
}

function invokeWorkersToGenAbc(): void {
  if (process.env.watchMode === 'true') {
    process.exitCode = SUCCESS;
  }
  let cmdPrefix: string = '';

  const abcArgs: string[] = initAbcEnv();
  if (process.env.panda === TS2ABC) {
    cmdPrefix = `${nodeJs} ${abcArgs.join(' ')}`;
  } else {
    logger.error(red, `ArkTS:ERROR please set panda module`, reset);
  }

  filterIntermediateJsBundleByHashJson(buildPathInfo, intermediateJsBundle);
  const splitedBundles: any[] = splitJsBundlesBySize(fileterIntermediateJsBundle, MAX_WORKER_NUMBER);
  const workerNumber: number = MAX_WORKER_NUMBER < splitedBundles.length ? MAX_WORKER_NUMBER : splitedBundles.length;

  try {
    if (process.env.watchMode === 'true') {
      processWorkersOfPreviewMode(splitedBundles, cmdPrefix, workerNumber);
    } else {
      processWorkersOfBuildMode(splitedBundles, cmdPrefix, workerNumber);
    }
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR failed to generate abc. Error message: ${e}`, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.watchMode !== 'true') {
      process.exit(FAIL);
    }
  }
}

function filterIntermediateModuleByHashJson(buildPath: string, moduleInfos: Array<ModuleInfo>): void {
  const tempModuleInfos = Array<ModuleInfo>();
  moduleInfos.forEach((item) => {
    const check = tempModuleInfos.every((newItem) => {
      return item.tempFilePath !== newItem.tempFilePath;
    });
    if (check) {
      tempModuleInfos.push(item);
    }
  });
  moduleInfos = tempModuleInfos;

  for (let i = 0; i < moduleInfos.length; ++i) {
    filterModuleInfos.push(moduleInfos[i]);
  }
  const hashFilePath: string = genHashJsonPath(buildPath);
  if (hashFilePath.length === 0) {
    return;
  }
  const updateJsonObject: Object = {};
  let jsonObject: Object = {};
  let jsonFile: string = '';
  if (fs.existsSync(hashFilePath)) {
    jsonFile = fs.readFileSync(hashFilePath).toString();
    jsonObject = JSON.parse(jsonFile);
    filterModuleInfos = [];
    for (let i = 0; i < moduleInfos.length; ++i) {
      const input: string = moduleInfos[i].tempFilePath;
      let outputPath: string = genProtoFileName(moduleInfos[i].tempFilePath);
      if (!fs.existsSync(input)) {
        logger.debug(red, `ArkTS:ERROR ${input} is lost`, reset);
        process.exitCode = FAIL;
        break;
      }
      if (fs.existsSync(outputPath)) {
        const hashInputContentData: string = toHashData(input);
        const hashAbcContentData: string = toHashData(outputPath);
        if (jsonObject[input] === hashInputContentData && jsonObject[outputPath] === hashAbcContentData) {
          updateJsonObject[input] = hashInputContentData;
          updateJsonObject[outputPath] = hashAbcContentData;
        } else {
          filterModuleInfos.push(moduleInfos[i]);
        }
      } else {
        filterModuleInfos.push(moduleInfos[i]);
      }
    }
  }

  moduleHashJsonObject = updateJsonObject;
}

function writeModuleHashJson(): void {
  for (let i = 0; i < filterModuleInfos.length; ++i) {
    const input: string = filterModuleInfos[i].tempFilePath;
    let outputPath: string = genProtoFileName(filterModuleInfos[i].tempFilePath);
    if (!fs.existsSync(input) || !fs.existsSync(outputPath)) {
      logger.debug(red, `ArkTS:ERROR ${input} is lost`, reset);
      process.exitCode = FAIL;
      break;
    }
    const hashInputContentData: string = toHashData(input);
    const hashOutputContentData: string = toHashData(outputPath);
    moduleHashJsonObject[input] = hashInputContentData;
    moduleHashJsonObject[outputPath] = hashOutputContentData;
  }
  const hashFilePath: string = genHashJsonPath(buildPathInfo);
  if (hashFilePath.length === 0) {
    return;
  }
  // fix bug of multi trigger
  fs.writeFileSync(hashFilePath, JSON.stringify(moduleHashJsonObject));
}

function filterIntermediateJsBundleByHashJson(buildPath: string, inputPaths: File[]): void {
  inputPaths = removeDuplicateInfoOfBundleList(inputPaths);

  for (let i = 0; i < inputPaths.length; ++i) {
    fileterIntermediateJsBundle.push(inputPaths[i]);
  }
  const hashFilePath: string = genHashJsonPath(buildPath);
  if (hashFilePath.length === 0) {
    return;
  }
  const updateJsonObject: Object = {};
  let jsonObject: Object = {};
  let jsonFile: string = '';
  if (fs.existsSync(hashFilePath)) {
    jsonFile = fs.readFileSync(hashFilePath).toString();
    jsonObject = JSON.parse(jsonFile);
    fileterIntermediateJsBundle = [];
    for (let i = 0; i < inputPaths.length; ++i) {
      const cacheOutputPath: string = inputPaths[i].cacheOutputPath;
      const cacheAbcFilePath: string = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
      if (!fs.existsSync(cacheOutputPath)) {
        logger.debug(red, `ArkTS:ERROR ${cacheOutputPath} is lost`, reset);
        process.exitCode = FAIL;
        break;
      }
      if (fs.existsSync(cacheAbcFilePath)) {
        const hashInputContentData: string = toHashData(cacheOutputPath);
        const hashAbcContentData: string = toHashData(cacheAbcFilePath);
        if (jsonObject[cacheOutputPath] === hashInputContentData && jsonObject[cacheAbcFilePath] === hashAbcContentData) {
          updateJsonObject[cacheOutputPath] = hashInputContentData;
          updateJsonObject[cacheAbcFilePath] = hashAbcContentData;
        } else {
          fileterIntermediateJsBundle.push(inputPaths[i]);
        }
      } else {
        fileterIntermediateJsBundle.push(inputPaths[i]);
      }
    }
  }

  hashJsonObject = updateJsonObject;
}

function writeHashJson(): void {
  for (let i = 0; i < fileterIntermediateJsBundle.length; ++i) {
    const cacheOutputPath: string = fileterIntermediateJsBundle[i].cacheOutputPath;
    const cacheAbcFilePath: string = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
    if (!fs.existsSync(cacheOutputPath) || !fs.existsSync(cacheAbcFilePath)) {
      logger.debug(red, `ArkTS:ERROR ${cacheOutputPath} is lost`, reset);
      process.exitCode = FAIL;
      break;
    }
    const hashInputContentData: string = toHashData(cacheOutputPath);
    const hashAbcContentData: string = toHashData(cacheAbcFilePath);
    hashJsonObject[cacheOutputPath] = hashInputContentData;
    hashJsonObject[cacheAbcFilePath] = hashAbcContentData;
  }
  const hashFilePath: string = genHashJsonPath(buildPathInfo);
  if (hashFilePath.length === 0) {
    return;
  }
  fs.writeFileSync(hashFilePath, JSON.stringify(hashJsonObject));
}

function genHashJsonPath(buildPath: string): string {
  buildPath = toUnixPath(buildPath);
  if (process.env.cachePath) {
    if (!fs.existsSync(process.env.cachePath) || !fs.statSync(process.env.cachePath).isDirectory()) {
      logger.debug(red, `ArkTS:ERROR hash path does not exist`, reset);
      return '';
    }
    let buildDirArr: string[] = projectConfig.buildPath.split(path.sep);
    let abilityDir: string = buildDirArr[buildDirArr.length - 1];
    let hashJsonPath: string = path.join(process.env.cachePath, TEMPORARY, abilityDir, hashFile);
    validateFilePathLength(hashJsonPath, logger);
    mkdirsSync(path.dirname(hashJsonPath));
    return hashJsonPath;
  } else if (buildPath.indexOf(ARK) >= 0) {
    const dataTmps: string[] = buildPath.split(ARK);
    const hashPath: string = path.join(dataTmps[0], ARK);
    if (!fs.existsSync(hashPath) || !fs.statSync(hashPath).isDirectory()) {
      logger.debug(red, `ArkTS:ERROR hash path does not exist`, reset);
      return '';
    }
    let hashJsonPath: string = path.join(hashPath, hashFile);
    validateFilePathLength(hashJsonPath, logger);
    return hashJsonPath;
  } else {
    logger.debug(red, `ArkTS:ERROR not cache exist`, reset);
    return '';
  }
}

function checkNodeModules() {
  if (process.env.panda === TS2ABC) {
    let arkEntryPath: string = path.join(arkDir, 'build');
    if (isWin) {
      arkEntryPath = path.join(arkDir, 'build-win');
    } else if (isMac) {
      arkEntryPath = path.join(arkDir, 'build-mac');
    }
    let nodeModulesPath: string = path.join(arkEntryPath, NODE_MODULES);
    validateFilePathLength(nodeModulesPath, logger);
    if (!(fs.existsSync(nodeModulesPath) && fs.statSync(nodeModulesPath).isDirectory())) {
      logger.error(red, `ERROR: node_modules for ark compiler not found.
        Please make sure switch to non-root user before runing "npm install" for safity requirements and try re-run "npm install" under ${arkEntryPath}`, reset);
      return false;
    }
  }

  return true;
}

function copyFileCachePathToBuildPath() {
  for (let i = 0; i < intermediateJsBundle.length; ++i) {
    const abcFile: string = intermediateJsBundle[i].path.replace(/\.temp\.js$/, '.abc');
    const cacheOutputPath: string = intermediateJsBundle[i].cacheOutputPath;
    const cacheAbcFilePath: string = intermediateJsBundle[i].cacheOutputPath.replace(/\.temp\.js$/, '.abc');
    if (!fs.existsSync(cacheAbcFilePath)) {
      logger.debug(red, `ArkTS:ERROR ${cacheAbcFilePath} is lost`, reset);
      process.exitCode = FAIL;
      break;
    }
    let parent: string = path.join(abcFile, '..');
    if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
      mkDir(parent);
    }
    // for preview mode, cache path and old abc file both exist, should copy abc file for updating
    if (process.env.cachePath !== undefined) {
      fs.copyFileSync(cacheAbcFilePath, abcFile);
    }
    if (process.env.cachePath === undefined && fs.existsSync(cacheOutputPath)) {
      fs.unlinkSync(cacheOutputPath);
    }
  }
}

function processExtraAsset() {
  if (projectConfig.compileMode === JSBUNDLE || projectConfig.compileMode === undefined) {
    writeHashJson();
    copyFileCachePathToBuildPath();
  } else if (projectConfig.compileMode === ESMODULE) {
    processEntryToGenAbc(entryInfos);
    writeModuleHashJson();
    copyModuleFileCachePathToBuildPath();
    mergeProtoToAbc();
  }
  clearGlobalInfo();
}

function handleHotReloadChangedFiles() {
  if (!fs.existsSync(projectConfig.changedFileList)) {
    logger.debug(blue, `ArkTS: Cannot find file: ${projectConfig.changedFileList}, skip hot reload build`, reset);
    return;
  }

  let changedFileListJson: string = fs.readFileSync(projectConfig.changedFileList).toString();
  let changedFileList: Array<string> = JSON.parse(changedFileListJson).modifiedFiles;
  if (typeof(changedFileList) === 'undefined' || changedFileList.length === 0) {
    return;
  }

  let relativeProjectPath = projectConfig.projectPath.slice(projectConfig.projectRootPath.length + path.sep.length);
  const nodeModulesFile: Array<string> = [];
  let hotReloadSourceMap: Object = {};
  moduleInfos = [];

  for (let file of changedFileList) {
    let filePath: string = path.join(projectConfig.projectPath, file);
    validateFilePathLength(filePath, logger);
    let tempFilePath: string = genTemporaryPath(filePath, projectConfig.projectPath, process.env.cachePath,
      projectConfig, undefined);
    if (tempFilePath.length === 0) {
      return;
    }
    validateFilePathLength(tempFilePath, logger);
    let buildFilePath: string = '';
    tempFilePath = toUnixPath(tempFilePath);

    switch (path.extname(filePath)) {
      case EXTNAME_ETS: {
        processEtsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, undefined);
        break;
      }
      case EXTNAME_TS: {
        processTsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, undefined);
        break;
      }
      case EXTNAME_JS:
      case EXTNAME_MJS:
      case EXTNAME_CJS: {
        processJsModule(filePath, tempFilePath, buildFilePath, nodeModulesFile, undefined);
        break;
      }
      case EXTNAME_JSON: {
        logger.debug(blue, `ArkTS: json source file: ${filePath} changed, skip hot reload build`, reset);
        return;
      }
      default: {
        logger.debug(blue, `ArkTS:ERROR Cannot resolve file path: ${filePath}, stop hot reload build`, reset);
        return;
      }
    }

    let sourceMapPath: string = toUnixPath(path.join(relativeProjectPath, file));
    validateFilePathLength(sourceMapPath, logger);
    hotReloadSourceMap[sourceMapPath] = newSourceMaps[sourceMapPath];
  }

  if (!fs.existsSync(projectConfig.patchAbcPath)) {
    mkdirsSync(projectConfig.patchAbcPath);
  }

  const outputABCPath: string = path.join(projectConfig.patchAbcPath, MODULES_ABC);
  validateFilePathLength(outputABCPath, logger);
  generateMergedAbc(moduleInfos, entryInfos, outputABCPath);

  // write source maps
  let sourceMapFilePath: string = path.join(projectConfig.patchAbcPath, SOURCEMAPS);
  validateFilePathLength(sourceMapFilePath, logger);
  fs.writeFileSync(sourceMapFilePath,
                   JSON.stringify(hotReloadSourceMap, null, 2), 'utf-8');
}

function handleFinishModules(modules, callback) {
  if (projectConfig.hotReload && !isHotReloadFirstBuild) {
    handleHotReloadChangedFiles();
    return;
  }

  handleFullModuleFiles(modules, callback);

  if (projectConfig.hotReload) {
    isHotReloadFirstBuild = false;
  }
}

function copyModuleFileCachePathToBuildPath(): void {
  protoFilePath = path.join(path.join(process.env.cachePath, 'protos', PROTO_FILESINFO_TXT));
  validateFilePathLength(protoFilePath, logger);
  mkdirsSync(path.dirname(protoFilePath));
  let entriesInfo: string = '';
  moduleInfos = removeDuplicateInfo(moduleInfos);
  moduleInfos.sort((m1: ModuleInfo, m2: ModuleInfo) => {
    return m1.tempFilePath < m2.tempFilePath ? 1 : -1;
  });
  for (let i = 0; i < moduleInfos.length; ++i) {
    let protoTempPath: string = genProtoFileName(moduleInfos[i].tempFilePath);
    entriesInfo += `${toUnixPath(protoTempPath)}\n`;
  }
  if (entryInfos.size > 0) {
    let npmEntriesProtoFileName: string = 'npm_entries' + EXTNAME_PROTO_BIN;
    const npmEntriesProtoFilePath: string = path.join(process.env.cachePath, 'protos', 'npm_entries', npmEntriesProtoFileName);
    entriesInfo += `${toUnixPath(npmEntriesProtoFilePath)}\n`;
  }
  fs.writeFileSync(protoFilePath, entriesInfo, 'utf-8');
}

function mergeProtoToAbc(): void {
  let mergeAbc: string = path.join(arkDir, 'build', 'bin', 'merge_abc');
  if (isWin) {
    mergeAbc = path.join(arkDir, 'build-win', 'bin', 'merge_abc.exe');
  } else if (isMac) {
    mergeAbc = path.join(arkDir, 'build-mac', 'bin', 'merge_abc');
  }
  mkdirsSync(projectConfig.buildPath);
  const singleCmd: string = `"${mergeAbc}" --input "@${protoFilePath}" --outputFilePath "${projectConfig.buildPath}" --output ${MODULES_ABC} --suffix protoBin`;
  try {
    childProcess.execSync(singleCmd);
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR Failed to merge proto file to abc. Error message: ${e}`, reset);
  }
}

function generateAbcByEs2AbcOfBundleMode(inputPaths: File[]) {
  filterIntermediateJsBundleByHashJson(buildPathInfo, inputPaths);
  if (fileterIntermediateJsBundle.length === 0) {
    processExtraAsset();
    return;
  }
  let filesInfoPath = generateFileOfBundle(fileterIntermediateJsBundle);
  const fileThreads = os.cpus().length < 16 ? os.cpus().length : 16;
  let genAbcCmd: string =
      `${initAbcEnv().join(' ')} "@${filesInfoPath}" --file-threads "${fileThreads}"`;
  logger.debug('gen abc cmd is: ', genAbcCmd);
  try {
    if (process.env.watchMode === 'true') {
      childProcess.execSync(genAbcCmd);
      processExtraAsset();
    } else {
      const child = childProcess.exec(genAbcCmd);
      child.on('exit', (code: any) => {
        if (code === FAIL) {
          logger.debug(red, 'ArkTS:ERROR failed to execute es2abc', reset);
          process.exit(FAIL);
        }
        if (process.env.cachePath === undefined) {
          unlinkSync(filesInfoPath);
        }
        processExtraAsset();
      });

      child.on('error', (err: any) => {
        logger.debug(red, err.toString(), reset);
        process.exit(FAIL);
      });

      child.stderr.on('data', (data: any) => {
        logger.error(red, data.toString(), reset);
      });
    }
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR failed to generate abc with filesInfo ${filesInfoPath}. Error message: ${e} `, reset);
    process.env.abcCompileSuccess = 'false';
    if (process.env.watchMode !== 'true') {
      process.exit(FAIL);
    }
  } finally {
    if (process.env.watchMode === 'true') {
      if (process.env.cachePath === undefined) {
        unlinkSync(filesInfoPath);
      }
    }
  }
}

function generateFileOfBundle(inputPaths: File[]): string {
  let filesInfoPath: string = buildCachePath(FILESINFO_TXT, projectConfig, logger);
  inputPaths = removeDuplicateInfoOfBundleList(inputPaths);

  let filesInfo: string = '';
  inputPaths.forEach(info => {
    const cacheOutputPath: string = info.cacheOutputPath;
    const recordName: string = 'null_recordName';
    const moduleType: string = 'script';
    const sourceFile: string = info.sourceFile;
    const abcFilePath: string = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
    filesInfo += `${cacheOutputPath};${recordName};${moduleType};${sourceFile};${abcFilePath}\n`;
  });
  fs.writeFileSync(filesInfoPath, filesInfo, 'utf-8');

  return filesInfoPath;
}

function removeDuplicateInfoOfBundleList(inputPaths: File[]) {
  const tempInputPaths = Array<File>();
  inputPaths.forEach((item) => {
    const check = tempInputPaths.every((newItem) => {
      return item.path !== newItem.path;
    });
    if (check) {
      tempInputPaths.push(item);
    }
  });
  inputPaths = tempInputPaths;

  return inputPaths;
}

function processWorkersOfPreviewMode(splittedData: any, cmdPrefix: string, workerNumber: number) {
  let processEnv: any = Object.assign({}, process.env);
  let arkEnvParams: any = {
    'splittedData': JSON.stringify(splittedData),
    'cmdPrefix': cmdPrefix,
    'workerNumber': workerNumber.toString(),
  };
  if (projectConfig.compileMode === JSBUNDLE || projectConfig.compileMode === undefined) {
    arkEnvParams.mode = JSBUNDLE;
  } else if (projectConfig.compileMode === ESMODULE) {
    arkEnvParams.cachePath = process.env.cachePath;
    arkEnvParams.mode = ESMODULE;
  }
  processEnv.arkEnvParams = JSON.stringify(arkEnvParams);

  let genAbcCmd: string = `${nodeJs} "${path.resolve(__dirname, MANAGE_WORKERS_SCRIPT)}"`;
  childProcess.execSync(genAbcCmd, {env: processEnv});
  processExtraAsset();
}

function processWorkersOfBuildMode(splittedData: any, cmdPrefix: string, workerNumber: number) {
  const useNewApi: boolean = nodeLargeOrEqualTargetVersion(16);

  if (useNewApi && cluster.isPrimary || !useNewApi && cluster.isMaster) {
    let genAbcScript: string = GEN_ABC_SCRIPT;
    if (projectConfig.compileMode === ESMODULE) {
      genAbcScript = GEN_MODULE_ABC_SCRIPT;
    }
    if (useNewApi) {
      cluster.setupPrimary({
        exec: path.resolve(__dirname, genAbcScript)
      });
    } else {
      cluster.setupMaster({
        exec: path.resolve(__dirname, genAbcScript)
      });
    }

    for (let i = 0; i < workerNumber; ++i) {
      let workerData: any = {
        'inputs': JSON.stringify(splittedData[i]),
        'cmd': cmdPrefix
      };
      if (projectConfig.compileMode === ESMODULE) {
        let sn: number = i + 1;
        let workerFileName: string = `filesInfo_${sn}.txt`;
        workerData.workerFileName = workerFileName;
        workerData.cachePath = process.env.cachePath;
      }
      cluster.fork(workerData);
    }

    cluster.on('exit', (worker, code, signal) => {
      if (code === FAIL) {
        process.exitCode = FAIL;
      }
      logger.debug(`worker ${worker.process.pid} finished`);
    });

    process.on('exit', (code) => {
      if (process.exitCode !== FAIL && process.env.watchMode !== 'true') {
        processExtraAsset();
        if (projectConfig.compileMode === ESMODULE &&
          (projectConfig.anBuildMode === AOT_FULL || projectConfig.anBuildMode === AOT_PARTIAL)) {
          let faultHandler: FaultHandler = (error) => { logger.error(error); process.exit(FAIL); };
          let abcArgs: string[] = initAbcEnv();
          abcArgs.unshift(nodeJs);
          const appAbc: string = path.join(projectConfig.buildPath, MODULES_ABC);
          generateAot(arkDir, appAbc, projectConfig, logger, faultHandler);
        }
      }
    });
  }
}
