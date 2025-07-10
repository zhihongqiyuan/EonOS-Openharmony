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

import childProcess from 'child_process';
import fs from 'fs';
import path from 'path';
import cluster from 'cluster';

import {
  COMMONJS,
  COMPILE_CONTEXT_INFO_JSON,
  ESM,
  ESMODULE,
  EXTNAME_CJS,
  EXTNAME_ETS,
  EXTNAME_JS,
  EXTNAME_JSON,
  EXTNAME_MJS,
  EXTNAME_PROTO_BIN,
  EXTNAME_TS,
  EXTNAME_TXT,
  FAIL,
  SUCCESS,
  FILESINFO,
  FILESINFO_TXT,
  MAX_WORKER_NUMBER,
  MODULES_ABC,
  MODULES_CACHE,
  NPM_ENTRIES_PROTO_BIN,
  NPMENTRIES_TXT,
  OH_MODULES,
  PACKAGES,
  PROTO_FILESINFO_TXT,
  PROTOS,
  red,
  reset,
  SOURCEMAPS,
  SOURCEMAPS_JSON,
  WIDGETS_ABC,
  TS2ABC,
  ES2ABC,
  ETS,
  TS,
  JS,
  PERFREPORT_JSON
} from '../common/ark_define';
import {
  needAotCompiler,
  isMasterOrPrimary,
  isAotMode,
  isDebug
} from '../utils';
import { CommonMode } from '../common/common_mode';
import {
  handleObfuscatedFilePath,
  enableObfuscateFileName,
  enableObfuscatedFilePathConfig
} from '../common/ob_config_resolver';
import {
  changeFileExtension,
  getEs2abcFileThreadNumber,
  isCommonJsPluginVirtualFile,
  isCurrentProjectFiles,
  shouldETSOrTSFileTransformToJS
} from '../utils';
import {
  isPackageModulesFile,
  mkdirsSync,
  toUnixPath,
  toHashData,
  validateFilePathLength
} from '../../../utils';
import {
  getPackageInfo,
  getNormalizedOhmUrlByFilepath,
  getOhmUrlByFilepath,
  getOhmUrlByExternalPackage,
  isTs2Abc,
  isEs2Abc,
  transformOhmurlToPkgName,
  transformOhmurlToRecordName
} from '../../../ark_utils';
import {
  generateAot,
  FaultHandler
} from '../../../gen_aot';
import {
  NATIVE_MODULE
} from '../../../pre_define';
import {
  sharedModuleSet
} from '../check_shared_module';
import { SourceMapGenerator } from '../generate_sourcemap';
import { MemoryMonitor } from '../../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../../meomry_monitor/memory_define';
import {
  ArkTSInternalErrorDescription,
  ArkTSErrorDescription,
  ErrorCode
} from '../error_code';
import {
  LogData,
  LogDataFactory
} from '../logger';
import {
  CompileEvent,
  createAndStartEvent,
  ExternalEventType,
  isNeedPerformanceDotting,
  processExternalEvents,
  stopEvent
 } from '../../../performance';
import { BytecodeObfuscator } from '../bytecode_obfuscator';

export class ModuleInfo {
  filePath: string;
  cacheFilePath: string;
  recordName: string;
  isCommonJs: boolean;
  sourceFile: string;
  packageName: string;
  originSourceLang: string;

  constructor(filePath: string, cacheFilePath: string, isCommonJs: boolean, recordName: string, sourceFile: string,
    packageName: string, originSourceLang: string
  ) {
    this.filePath = filePath;
    this.cacheFilePath = cacheFilePath;
    this.recordName = recordName;
    this.isCommonJs = isCommonJs;
    this.sourceFile = sourceFile;
    this.packageName = packageName;
    this.originSourceLang = originSourceLang
  }
}

export class PackageEntryInfo {
  pkgEntryPath: string;
  pkgBuildPath: string;
  constructor(pkgEntryPath: string, pkgBuildPath: string) {
    this.pkgEntryPath = pkgEntryPath;
    this.pkgBuildPath = pkgBuildPath;
  }
}

export class ModuleMode extends CommonMode {
  moduleInfos: Map<String, ModuleInfo>;
  pkgEntryInfos: Map<String, PackageEntryInfo>;
  hashJsonObject: Object;
  filesInfoPath: string;
  npmEntriesInfoPath: string;
  moduleAbcPath: string;
  sourceMapPath: string;
  cacheFilePath: string;
  cacheSourceMapPath: string;
  workerNumber: number;
  npmEntriesProtoFilePath: string;
  protoFilePath: string;
  filterModuleInfos: Map<String, ModuleInfo>;
  symlinkMap: Object;
  useNormalizedOHMUrl: boolean;
  compileContextInfoPath: string;
  abcPaths: string[] = [];
  byteCodeHar: boolean;
  perfReportPath: string;

  constructor(rollupObject: Object) {
    super(rollupObject);
    this.moduleInfos = new Map<String, ModuleInfo>();
    this.pkgEntryInfos = new Map<String, PackageEntryInfo>();
    this.hashJsonObject = {};
    this.filesInfoPath = path.join(this.projectConfig.cachePath, FILESINFO_TXT);
    this.perfReportPath = path.join(this.projectConfig.cachePath, PERFREPORT_JSON);
    this.npmEntriesInfoPath = path.join(this.projectConfig.cachePath, NPMENTRIES_TXT);
    const outPutABC: string = this.projectConfig.widgetCompile ? WIDGETS_ABC : MODULES_ABC;
    this.moduleAbcPath = path.join(this.projectConfig.aceModuleBuild, outPutABC);
    this.sourceMapPath = this.arkConfig.isDebug ? path.join(this.projectConfig.aceModuleBuild, SOURCEMAPS) :
      path.join(this.projectConfig.cachePath, SOURCEMAPS);
    this.cacheFilePath = path.join(this.projectConfig.cachePath, MODULES_CACHE);
    this.cacheSourceMapPath = path.join(this.projectConfig.cachePath, SOURCEMAPS_JSON);
    this.workerNumber = MAX_WORKER_NUMBER;
    this.npmEntriesProtoFilePath = path.join(this.projectConfig.cachePath, PROTOS, NPM_ENTRIES_PROTO_BIN);
    this.protoFilePath = path.join(this.projectConfig.cachePath, PROTOS, PROTO_FILESINFO_TXT);
    this.hashJsonObject = {};
    this.filterModuleInfos = new Map<String, ModuleInfo>();
    this.symlinkMap = rollupObject.share.symlinkMap;
    this.useNormalizedOHMUrl = this.isUsingNormalizedOHMUrl();
    if (Object.prototype.hasOwnProperty.call(this.projectConfig, 'byteCodeHarInfo')) {
      let byteCodeHarInfo = this.projectConfig.byteCodeHarInfo;
      for (const packageName in byteCodeHarInfo) {
          const abcPath = toUnixPath(byteCodeHarInfo[packageName].abcPath);
          this.abcPaths.push(abcPath);
      }
    }
    this.byteCodeHar = !!this.projectConfig.byteCodeHar;
    if (this.useNormalizedOHMUrl) {
      this.compileContextInfoPath = this.generateCompileContextInfo(rollupObject);
    }
  }

  private generateCompileContextInfo(rollupObject: Object): string {
    let compileContextInfoPath: string = path.join(this.projectConfig.cachePath, COMPILE_CONTEXT_INFO_JSON);
    let compileContextInfo: Object = {};
    let hspPkgNames: Array<string> = [];
    for (const hspAliasName in this.projectConfig.hspNameOhmMap) {
      let hspPkgName: string = hspAliasName;
      if (this.projectConfig.dependencyAliasMap.has(hspAliasName)) {
        hspPkgName = this.projectConfig.dependencyAliasMap.get(hspAliasName);
      }
      hspPkgNames.push(toUnixPath(hspPkgName));
    }
    compileContextInfo.hspPkgNames = hspPkgNames;
    let compileEntries: Set<string> = new Set();
    let entryObj: Object = this.projectConfig.entryObj;
    if (!!this.projectConfig.widgetCompile) {
      entryObj = this.projectConfig.cardEntryObj;
    }
    for (const key in entryObj) {
      let moduleId: string = entryObj[key];
      let moduleInfo: Object = rollupObject.getModuleInfo(moduleId);
      if (!moduleInfo) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_MODULE_INFO_NOT_FOUND,
          ArkTSInternalErrorDescription,
          'Failed to find module info. ' +
          `Failed to find module info with '${moduleId}' from the context information.`
        );
        this.logger.printErrorAndExit(errInfo);
      }
      let metaInfo: Object = moduleInfo.meta;
      if (!metaInfo) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_META_INFO_NOT_FOUND,
          ArkTSInternalErrorDescription,
          'Failed to find meta info. ' +
          `Failed to find meta info with '${moduleId}' from the module info.`
        );
        this.logger.printErrorAndExit(errInfo);
      }
      const pkgParams = {
        pkgName: metaInfo.pkgName,
        pkgPath: metaInfo.pkgPath,
        isRecordName: true
      };
      let recordName: string = getNormalizedOhmUrlByFilepath(moduleId, this.projectConfig, this.logger, pkgParams,
        undefined);
      compileEntries.add(recordName);
    }
    this.collectDeclarationFilesEntry(compileEntries, hspPkgNames);
    compileContextInfo.compileEntries = Array.from(compileEntries);
    if (this.projectConfig.updateVersionInfo) {
      compileContextInfo.updateVersionInfo = this.projectConfig.updateVersionInfo;
    } else if (this.projectConfig.pkgContextInfo) {
      compileContextInfo.pkgContextInfo = this.projectConfig.pkgContextInfo;
    }
    // The bundleType is 'shared' in cross-app hsp.
    if (this.projectConfig.bundleType === 'shared') {
      compileContextInfo.needModifyRecord = true;
      compileContextInfo.bundleName = this.projectConfig.bundleName;
    }
    fs.writeFileSync(compileContextInfoPath, JSON.stringify(compileContextInfo), 'utf-8');
    return compileContextInfoPath;
  }

  private collectDeclarationFilesEntry(compileEntries: Set<string>, hspPkgNames: Array<string>): void {
    if (this.projectConfig.arkRouterMap) {
      // Collect bytecode har's declaration files entries in router map, use
      // by es2abc for dependency resolution.
      this.collectRouterMapEntries(compileEntries, hspPkgNames);
    }
    if (this.projectConfig.declarationEntry) {
      // Collect bytecode har's declaration files entries include dynamic import and workers, use
      // by es2abc for dependency resolution.
      this.projectConfig.declarationEntry.forEach((ohmurl) => {
        let pkgName: string = transformOhmurlToPkgName(ohmurl);
        if (!hspPkgNames.includes(pkgName)) {
          let recordName: string = transformOhmurlToRecordName(ohmurl);
          compileEntries.add(recordName);
        }
      });
    }
  }

  private collectRouterMapEntries(compileEntries: Set<string>, hspPkgNames: Array<string>): void {
    this.projectConfig.arkRouterMap.forEach((router) => {
      if (router.ohmurl) {
        let pkgName: string = transformOhmurlToPkgName(router.ohmurl);
        if (!hspPkgNames.includes(pkgName)) {
          let recordName: string = transformOhmurlToRecordName(router.ohmurl);
          compileEntries.add(recordName);
        }
      }
    });
  }

  prepareForCompilation(rollupObject: Object, parentEvent: CompileEvent): void {
    const eventPrepareForCompilation = createAndStartEvent(parentEvent, 'preparation for compilation');
    this.collectModuleFileList(rollupObject, rollupObject.getModuleIds());
    this.removeCacheInfo(rollupObject);
    stopEvent(eventPrepareForCompilation);
  }

  collectModuleFileList(module: Object, fileList: IterableIterator<string>): void {
    const recordInfo = MemoryMonitor.recordStage(MemoryDefine.PKG_ENTRY_INFOS_MODULE_INFOS);
    let moduleInfos: Map<String, ModuleInfo> = new Map<String, ModuleInfo>();
    let pkgEntryInfos: Map<String, PackageEntryInfo> = new Map<String, PackageEntryInfo>();
    for (const moduleId of fileList) {
      if (isCommonJsPluginVirtualFile(moduleId) || !isCurrentProjectFiles(moduleId, this.projectConfig)) {
        continue;
      }
      const moduleInfo: Object = module.getModuleInfo(moduleId);
      if (moduleInfo.meta.isNodeEntryFile && !this.useNormalizedOHMUrl) {
        this.getPackageEntryInfo(moduleId, moduleInfo.meta, pkgEntryInfos);
      }

      this.processModuleInfos(moduleId, moduleInfos, moduleInfo.meta);
    }
    if (!this.useNormalizedOHMUrl) {
      this.getDynamicImportEntryInfo(pkgEntryInfos);
    }
    this.getNativeModuleEntryInfo(pkgEntryInfos);
    this.moduleInfos = moduleInfos;
    this.pkgEntryInfos = pkgEntryInfos;
    MemoryMonitor.stopRecordStage(recordInfo);
  }

  private isUsingNormalizedOHMUrl(): boolean {
    return !!this.projectConfig.useNormalizedOHMUrl;
  }

  private updatePkgEntryInfos(pkgEntryInfos: Map<String, PackageEntryInfo>, key: String, value: PackageEntryInfo): void {
    if (!pkgEntryInfos.has(key)) {
      pkgEntryInfos.set(key, new PackageEntryInfo(key, value));
    }
  }

  private getDynamicImportEntryInfo(pkgEntryInfos: Map<String, PackageEntryInfo>): void {
    if (this.projectConfig.dynamicImportLibInfo) {
      const REG_LIB_SO: RegExp = /lib(.+)\.so/;
      for (const [pkgName, pkgInfo] of Object.entries(this.projectConfig.dynamicImportLibInfo)) {
        if (REG_LIB_SO.test(pkgName)) {
          let ohmurl: string = pkgName.replace(REG_LIB_SO, (_, libsoKey) => {
            return `@app.${this.projectConfig.bundleName}/${this.projectConfig.moduleName}/${libsoKey}`;
          });
          this.updatePkgEntryInfos(pkgEntryInfos, pkgName, ohmurl);
          continue;
        }
        let hspOhmurl: string | undefined = getOhmUrlByExternalPackage(pkgName, this.projectConfig, undefined, this.logger,
          this.useNormalizedOHMUrl);
        if (hspOhmurl !== undefined) {
          hspOhmurl = hspOhmurl.replace(/^@(\w+):(.*)/, '@$1.$2');
          this.updatePkgEntryInfos(pkgEntryInfos, pkgName, hspOhmurl);
          continue;
        }
        const entryFile: string = pkgInfo.entryFilePath;
        this.getPackageEntryInfo(entryFile, pkgInfo, pkgEntryInfos);
      }
    }
  }

  private getNativeModuleEntryInfo(pkgEntryInfos: Map<String, PackageEntryInfo>): void {
    for (const item of NATIVE_MODULE) {
      let key = '@' + item;
      this.updatePkgEntryInfos(pkgEntryInfos, key, '@native.' + item);
    }
  }

  private getPackageEntryInfo(filePath: string, metaInfo: Object, pkgEntryInfos: Map<String, PackageEntryInfo>): void {
    if (metaInfo.isLocalDependency) {
      const hostModulesInfo: Object = metaInfo.hostModulesInfo;
      const pkgBuildPath: string = getOhmUrlByFilepath(filePath, this.projectConfig, this.logger, metaInfo.moduleName);
      hostModulesInfo.forEach(hostModuleInfo => {
        const hostDependencyName: string = hostModuleInfo.hostDependencyName;
        const hostModuleName: string = hostModuleInfo.hostModuleName;
        const pkgEntryPath: string = toUnixPath(path.join(`${PACKAGES}@${hostModuleName}`, hostDependencyName));
        if (!pkgEntryInfos.has(pkgEntryPath)) {
          pkgEntryInfos.set(pkgEntryPath, new PackageEntryInfo(pkgEntryPath, pkgBuildPath));
        }
        this.updatePkgEntryInfos(pkgEntryInfos, hostDependencyName, `@bundle.${pkgBuildPath}`);
      });
      return;
    }

    if (!metaInfo.pkgPath) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_MODULE_INFO_META_PKG_PATH,
        ArkTSInternalErrorDescription,
        `Failed to get ModuleInfo properties 'meta.pkgPath', moduleId: ${filePath}`
      );
      this.logger.printErrorAndExit(errInfo);
    }
    const pkgPath: string = metaInfo.pkgPath;
    let originPkgEntryPath: string = toUnixPath(filePath.replace(pkgPath, ''));
    if (originPkgEntryPath.startsWith('/')) {
      originPkgEntryPath = originPkgEntryPath.slice(1, originPkgEntryPath.length);
    }
    const pkgEntryPath: string = toUnixPath(this.getPkgModulesFilePkgName(pkgPath));
    let pkgBuildPath: string = path.join(pkgEntryPath, originPkgEntryPath);
    pkgBuildPath = toUnixPath(pkgBuildPath.substring(0, pkgBuildPath.lastIndexOf('.')));
    if (!pkgEntryInfos.has(pkgEntryPath)) {
      pkgEntryInfos.set(pkgEntryPath, new PackageEntryInfo(pkgEntryPath, pkgBuildPath));
    }
    // create symlink path to actual path mapping in ohpm
    if (this.projectConfig.packageDir == OH_MODULES && this.symlinkMap) {
      const symlinkEntries: Object = Object.entries(this.symlinkMap);
      for (const [actualPath, symlinkPaths] of symlinkEntries) {
        if (actualPath === pkgPath) {
          (<string[]>symlinkPaths).forEach((symlink: string) => {
            const symlinkPkgEntryPath: string = toUnixPath(this.getPkgModulesFilePkgName(symlink));
            if (!pkgEntryInfos.has(symlinkPkgEntryPath)) {
              pkgEntryInfos.set(symlinkPkgEntryPath, new PackageEntryInfo(symlinkPkgEntryPath, pkgEntryPath));
            }
          });
          break;
        }
      }
    }
  }

  private processModuleInfos(moduleId: string, moduleInfos: Map<String, ModuleInfo>, metaInfo?: Object): void {
    switch (path.extname(moduleId)) {
      case EXTNAME_ETS: {
        const extName: string = shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : EXTNAME_TS;
        this.addModuleInfoItem(moduleId, false, extName, metaInfo, moduleInfos, ETS);
        break;
      }
      case EXTNAME_TS: {
        const extName: string = shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : '';
        this.addModuleInfoItem(moduleId, false, extName, metaInfo, moduleInfos, TS);
        break;
      }
      case EXTNAME_JS:
      case EXTNAME_MJS:
      case EXTNAME_CJS: {
        const extName: string = (moduleId.endsWith(EXTNAME_MJS) || moduleId.endsWith(EXTNAME_CJS)) ? EXTNAME_JS : '';
        const isCommonJS: boolean = metaInfo && metaInfo.commonjs && metaInfo.commonjs.isCommonJS;
        this.addModuleInfoItem(moduleId, isCommonJS, extName, metaInfo, moduleInfos, JS);
        break;
      }
      case EXTNAME_JSON: {
        this.addModuleInfoItem(moduleId, false, '', metaInfo, moduleInfos);
        break;
      }
      default:
        break;
    }
  }

  private handleFileNameObfuscationInModuleInfo(sourceMapGenerator: SourceMapGenerator, isPackageModules: boolean, originalFilePath: string, filePath: string,
    sourceFile: string) {
    if (!enableObfuscateFileName(isPackageModules, this.projectConfig)) {
      if (sourceMapGenerator.isNewSourceMaps()) {
        sourceFile = sourceMapGenerator.genKey(originalFilePath);
      }
      return {filePath: filePath, sourceFile: sourceFile};
    }

    // if release mode, enable obfuscation, enable filename obfuscation -> call mangleFilePath()
    filePath = handleObfuscatedFilePath(originalFilePath, isPackageModules, this.projectConfig);
    sourceFile = filePath.replace(toUnixPath(this.projectConfig.projectRootPath) + '/', '');

    if (sourceMapGenerator.isNewSourceMaps()) {
      sourceFile = sourceMapGenerator.genKey(originalFilePath); // If the file name is obfuscated, meta info cannot be found.
      if (!sourceMapGenerator.sourceMapKeyMappingForObf.get(sourceFile)) {
        sourceMapGenerator.saveKeyMappingForObfFileName(originalFilePath);
      }
      // If the file name is obfuscated, the sourceFile needs to be updated.
      sourceFile = sourceMapGenerator.sourceMapKeyMappingForObf.get(sourceFile);
    }
    return {filePath: filePath, sourceFile: sourceFile};
  }

  private addModuleInfoItem(originalFilePath: string, isCommonJs: boolean, extName: string,
    metaInfo: Object, moduleInfos: Map<String, ModuleInfo>, originSourceLang: string = ""): void {
    const sourceMapGenerator: SourceMapGenerator = SourceMapGenerator.getInstance();
    const isPackageModules = isPackageModulesFile(originalFilePath, this.projectConfig);
    let filePath: string = originalFilePath;
    let sourceFile: string = filePath.replace(this.projectConfig.projectRootPath + path.sep, '');
    const isObfuscateEnabled: boolean = enableObfuscatedFilePathConfig(isPackageModules, this.projectConfig);
    if (isObfuscateEnabled) {
      const filePathAndSourceFile = this.handleFileNameObfuscationInModuleInfo(sourceMapGenerator, isPackageModules, originalFilePath, filePath, sourceFile);
      filePath = filePathAndSourceFile.filePath;
      sourceFile = filePathAndSourceFile.sourceFile;
    } else {
      if (sourceMapGenerator.isNewSourceMaps()) {
        sourceFile = sourceMapGenerator.genKey(originalFilePath);
      }
    }

    let moduleName: string = metaInfo.moduleName;
    let recordName: string = '';
    let cacheFilePath: string =
      this.genFileCachePath(filePath, this.projectConfig.projectRootPath, this.projectConfig.cachePath, metaInfo);
    let packageName: string = '';

    if (this.useNormalizedOHMUrl) {
      packageName = metaInfo.pkgName;
      const pkgParams = {
        pkgName: packageName,
        pkgPath: metaInfo.pkgPath,
        isRecordName: true
      };
      recordName = getNormalizedOhmUrlByFilepath(filePath, this.projectConfig, this.logger, pkgParams, undefined);
    } else {
      recordName = getOhmUrlByFilepath(filePath, this.projectConfig, this.logger, moduleName);
      if (isPackageModules) {
        packageName = this.getPkgModulesFilePkgName(metaInfo.pkgPath);
      } else {
        packageName =
          metaInfo.isLocalDependency ? moduleName : getPackageInfo(this.projectConfig.aceModuleJsonPath)[1];
      }
    }

    if (extName.length !== 0) {
      cacheFilePath = changeFileExtension(cacheFilePath, extName);
    }

    cacheFilePath = toUnixPath(cacheFilePath);
    recordName = toUnixPath(recordName);
    packageName = toUnixPath(packageName);
    if (!sourceMapGenerator.isNewSourceMaps()) {
      sourceFile = cacheFilePath.replace(toUnixPath(this.projectConfig.projectRootPath) + '/', '');
    }
    filePath = toUnixPath(filePath);

    moduleInfos.set(filePath, new ModuleInfo(filePath, cacheFilePath, isCommonJs, recordName, sourceFile, packageName,
                                             originSourceLang));
  }

  generateEs2AbcCmd() {
    const fileThreads = getEs2abcFileThreadNumber();
    this.cmdArgs.push(`"@${this.filesInfoPath}"`);
    if (!this.byteCodeHar) {
      this.cmdArgs.push('--npm-module-entry-list');
      this.cmdArgs.push(`"${this.npmEntriesInfoPath}"`);
    }
    this.cmdArgs.push('--output');
    this.cmdArgs.push(`"${this.moduleAbcPath}"`);
    this.cmdArgs.push('--file-threads');
    this.cmdArgs.push(`"${fileThreads}"`);
    this.cmdArgs.push('--merge-abc');
    this.cmdArgs.push(`"--target-api-version=${this.projectConfig.compatibleSdkVersion}"`);
    if (this.projectConfig.compatibleSdkVersionStage) {
      this.cmdArgs.push(`"--target-api-sub-version=${this.projectConfig.compatibleSdkVersionStage}"`);
    }
    // when enable branch elimination and bytecode obfuscation will crash
    if (this.arkConfig.isBranchElimination && !BytecodeObfuscator.enable) {
      this.cmdArgs.push('--branch-elimination');
    }
    if (this.projectConfig.transformLib) {
      this.cmdArgs.push(`--transform-lib`);
      this.cmdArgs.push(`"${this.projectConfig.transformLib}"`);
    }
    if (this.compileContextInfoPath !== undefined) {
      this.cmdArgs.push(`--compile-context-info`);
      this.cmdArgs.push(`"${this.compileContextInfoPath}"`);
    }
    if (this.abcPaths.length > 0 && !this.byteCodeHar) {
      this.cmdArgs.push('--enable-abc-input');
      this.cmdArgs.push('--remove-redundant-file');
    }
    if (!this.arkConfig.optTryCatchFunc) {
      this.cmdArgs.push('--opt-try-catch-func=false');
    }
    if (this.projectConfig.allowEtsAnnotations) {
      this.cmdArgs.push('--enable-annotations');
    }
    if (isNeedPerformanceDotting(this.projectConfig)) {
      this.cmdArgs.push(`--perf-file=${this.perfReportPath}`);
    }
  }

  addCacheFileArgs() {
    this.cmdArgs.push('--cache-file');
    this.cmdArgs.push(`"@${this.cacheFilePath}"`);
  }

  private generateCompileFilesInfo(includeByteCodeHarInfo: boolean): void {
    let filesInfo: string = '';
    const cacheFilePathSet: Set<string> = new Set();
    this.moduleInfos.forEach((info) => {
      const moduleType: string = info.isCommonJs ? COMMONJS : ESM;
      const isSharedModule: boolean = sharedModuleSet.has(info.filePath);
      if (cacheFilePathSet.has(info.cacheFilePath)) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_EXTERNAL_DUPLICATE_FILE_NAMES_ERROR,
          ArkTSErrorDescription,
          `Failed to generate filesInfo.\n
          File ${info.filePath} with the same name in module ${info.packageName}.\n
          Please modify the names of these files with the same name in the source code.`
        );
        this.logger.printError(errInfo);
      }
      cacheFilePathSet.add(info.cacheFilePath);
      filesInfo += `${info.cacheFilePath};${info.recordName};${moduleType};${info.sourceFile};${info.packageName};` +
        `${isSharedModule};${info.originSourceLang}\n`;
    });
    if (includeByteCodeHarInfo) {
      Object.entries(this.projectConfig.byteCodeHarInfo).forEach(([pkgName, abcInfo]) => {
        // es2abc parses file path and pkgName according to the file extension .abc.
        // Accurate version replacement requires 'pkgName' to es2abc.
        const abcPath: string = toUnixPath(abcInfo.abcPath);
        filesInfo += `${abcPath};;;;${pkgName};\n`;
      });
    }

    fs.writeFileSync(this.filesInfoPath, filesInfo, 'utf-8');
  }

  private generateNpmEntriesInfo() {
    let entriesInfo: string = '';
    for (const value of this.pkgEntryInfos.values()) {
      entriesInfo += `${value.pkgEntryPath}:${value.pkgBuildPath}\n`;
    }
    fs.writeFileSync(this.npmEntriesInfoPath, entriesInfo, 'utf-8');
  }

  private generateAbcCacheFilesInfo(): void {
    let abcCacheFilesInfo: string = '';

    // generate source file cache
    this.moduleInfos.forEach((info) => {
      let abcCacheFilePath: string = changeFileExtension(info.cacheFilePath, EXTNAME_PROTO_BIN);
      abcCacheFilesInfo += `${info.cacheFilePath};${abcCacheFilePath}\n`;
    });

    // generate npm entries cache
    let npmEntriesCacheFilePath: string = changeFileExtension(this.npmEntriesInfoPath, EXTNAME_PROTO_BIN);
    abcCacheFilesInfo += `${this.npmEntriesInfoPath};${npmEntriesCacheFilePath}\n`;
    if (this.projectConfig.cacheBytecodeHar) {
      this.abcPaths.forEach((abcPath) => {
        let abcCacheFilePath: string = this.genAbcCacheFilePath(abcPath);
        mkdirsSync(path.dirname(abcCacheFilePath));
        abcCacheFilesInfo += `${abcPath};${abcCacheFilePath}\n`;
      });
    }
    fs.writeFileSync(this.cacheFilePath, abcCacheFilesInfo, 'utf-8');
  }

  // Generate cache file path for bytecode har
  private genAbcCacheFilePath(abcPath: string): string {
    /**
     * The projectTopDir is the path of the main project, the projectRootPath is the project path to which it belongs,
     * and the path of bytecode har is within the main project. Therefore, the projectTopDir is used to intercept the
     * relative path of bytecodehar.
     */
    let relativeAbcPath: string = abcPath.replace(toUnixPath(this.projectConfig.projectTopDir), '');
    let tempPath: string = path.join(this.projectConfig.cachePath, relativeAbcPath);
    return changeFileExtension(tempPath, EXTNAME_PROTO_BIN);
  }

  genDescriptionsForMergedEs2abc(includeByteCodeHarInfo: boolean, parentEvent: CompileEvent): void {
    const eventEenDescriptionsForMergedEs2abc = createAndStartEvent(parentEvent, 'generate descriptions for merged es2abc');
    this.generateCompileFilesInfo(includeByteCodeHarInfo);
    if (!this.byteCodeHar) {
      this.generateNpmEntriesInfo();
    }
    this.generateAbcCacheFilesInfo();
    stopEvent(eventEenDescriptionsForMergedEs2abc)
  }

  generateMergedAbcOfEs2Abc(parentEvent: CompileEvent): void {
    // collect data error from subprocess
    let logDataList: Object[] = [];
    let errMsg: string = '';
    const genAbcCmd: string = this.cmdArgs.join(' ');
    let eventGenAbc: CompileEvent;
    try {
      const child = this.triggerAsync(() => {
        eventGenAbc = createAndStartEvent(parentEvent, 'generate merged abc by es2abc (async)', true);
        return childProcess.exec(genAbcCmd, { windowsHide: true });
      });
      child.on('close', (code: any) => {
        if (code === SUCCESS) {
          stopEvent(eventGenAbc, true);
          this.processAotIfNeeded();
          processExternalEvents(this.projectConfig, ExternalEventType.ES2ABC, { parentEvent: eventGenAbc, filePath: this.perfReportPath });
          BytecodeObfuscator.enable && BytecodeObfuscator.getInstance().execute(this.moduleInfos);
          this.triggerEndSignal();
          return;
        }
        for (const logData of logDataList) {
          this.logger.printError(logData);
        }
        if (errMsg !== '') {
          this.logger.error(`Error Message: ${errMsg}`);
        }
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_EXTERNAL_ES2ABC_EXECUTION_FAILED,
          ArkTSErrorDescription,
          'Failed to execute es2abc.',
          '',
          ["Please refer to es2abc's error codes."]
        );
        this.logger.printErrorAndExit(errInfo);
      });

      child.on('error', (err: any) => {
        stopEvent(eventGenAbc, true);
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_ES2ABC_SUBPROCESS_START_FAILED,
          ArkTSInternalErrorDescription,
          `Failed to initialize or launch the es2abc process. ${err.toString()}`
        );
        this.logger.printErrorAndExit(errInfo);
      });

      child.stderr.on('data', (data: any) => {
        const logData = LogDataFactory.newInstanceFromEs2AbcError(data.toString());
        if (logData) {
          logDataList.push(logData);
        } else {
          errMsg += data.toString();
        }
      });

    } catch (e) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_EXECUTE_ES2ABC_WITH_ASYNC_HANDLER_FAILED,
        ArkTSInternalErrorDescription,
        `Failed to execute es2abc with async handler. ${e.toString()}`
      );
      this.logger.printErrorAndExit(errInfo);
    }
  }

  filterModulesByHashJson() {
    if (this.hashJsonFilePath.length === 0 || !fs.existsSync(this.hashJsonFilePath)) {
      for (const key of this.moduleInfos.keys()) {
        this.filterModuleInfos.set(key, this.moduleInfos.get(key));
      }
      return;
    }

    let updatedJsonObject: Object = {};
    let jsonObject: Object = {};
    let jsonFile: string = '';

    if (fs.existsSync(this.hashJsonFilePath)) {
      jsonFile = fs.readFileSync(this.hashJsonFilePath).toString();
      jsonObject = JSON.parse(jsonFile);
      this.filterModuleInfos = new Map<string, ModuleInfo>();
      for (const [key, value] of this.moduleInfos) {
        const cacheFilePath: string = value.cacheFilePath;
        const cacheProtoFilePath: string = changeFileExtension(cacheFilePath, EXTNAME_PROTO_BIN);
        if (!fs.existsSync(cacheFilePath)) {
          this.logger.throwArkTsCompilerError(
            `ArkTS:INTERNAL ERROR: Failed to get module cache abc from ${cacheFilePath} in incremental build.` +
            'Please try to rebuild the project.');
        }
        if (fs.existsSync(cacheProtoFilePath)) {
          const hashCacheFileContentData: string = toHashData(cacheFilePath);
          const hashProtoFileContentData: string = toHashData(cacheProtoFilePath);
          if (jsonObject[cacheFilePath] === hashCacheFileContentData &&
            jsonObject[cacheProtoFilePath] === hashProtoFileContentData) {
            updatedJsonObject[cacheFilePath] = cacheFilePath;
            updatedJsonObject[cacheProtoFilePath] = cacheProtoFilePath;
            continue;
          }
        }
        this.filterModuleInfos.set(key, value);
      }
    }

    this.hashJsonObject = updatedJsonObject;
  }

  getSplittedModulesByNumber() {
    const result: any = [];
    if (this.filterModuleInfos.size < this.workerNumber) {
      for (const value of this.filterModuleInfos.values()) {
        result.push([value]);
      }
      return result;
    }

    for (let i = 0; i < this.workerNumber; ++i) {
      result.push([]);
    }

    let pos: number = 0;
    for (const value of this.filterModuleInfos.values()) {
      const chunk = pos % this.workerNumber;
      result[chunk].push(value);
      pos++;
    }

    return result;
  }

  invokeTs2AbcWorkersToGenProto(splittedModules) {
    let ts2abcCmdArgs: string[] = this.cmdArgs.slice(0);
    ts2abcCmdArgs.push('--output-proto');
    ts2abcCmdArgs.push('--merge-abc');
    ts2abcCmdArgs.push('--input-file');
    if (isMasterOrPrimary()) {
      this.setupCluster(cluster);
      this.workerNumber = splittedModules.length;
      for (let i = 0; i < this.workerNumber; ++i) {
        const sn: number = i + 1;
        const workerFileName: string = `${FILESINFO}_${sn}${EXTNAME_TXT}`;
        const workerData: Object = {
          inputs: JSON.stringify(splittedModules[i]),
          cmd: ts2abcCmdArgs.join(' '),
          workerFileName: workerFileName,
          mode: ESMODULE,
          cachePath: this.projectConfig.cachePath
        };
        this.triggerAsync(() => {
          const worker: Object = cluster.fork(workerData);
          worker.on('message', (errorMsg) => {
            this.logger.error(red, errorMsg.data.toString(), reset);
            this.logger.throwArkTsCompilerError('ArkTS:ERROR Failed to execute ts2abc.');
          });
        });
      }
    }
  }

  processTs2abcWorkersToGenAbc() {
    this.generateNpmEntriesInfo();
    let workerCount: number = 0;
    if (isMasterOrPrimary()) {
      cluster.on('exit', (worker, code, signal) => {
        if (code === FAIL) {
          this.logger.throwArkTsCompilerError('ArkTS:ERROR Failed to execute ts2abc');
        }
        workerCount++;
        if (workerCount === this.workerNumber) {
          this.generateNpmEntryToGenProto();
          this.generateProtoFilesInfo();
          this.mergeProtoToAbc();
          this.processAotIfNeeded();
          this.afterCompilationProcess();
        }
        this.triggerEndSignal();
      });
      if (this.workerNumber === 0) {
        // process aot for no source file changed.
        this.processAotIfNeeded();
      }
    }
  }

  private processAotIfNeeded(): void {
    if (!needAotCompiler(this.projectConfig)) {
      return;
    }
    let faultHandler: FaultHandler = ((error: string) => { this.logger.throwArkTsCompilerError(error); });
    generateAot(this.arkConfig.arkRootPath, this.moduleAbcPath, this.projectConfig, this.logger, faultHandler);
  }

  private genFileCachePath(filePath: string, projectRootPath: string, cachePath: string, metaInfo: Object): string {
    filePath = toUnixPath(filePath);
    let sufStr: string = '';
    if (metaInfo) {
      if (metaInfo.isLocalDependency) {
        sufStr = filePath.replace(toUnixPath(metaInfo.belongModulePath), metaInfo.moduleName);
      } else {
        sufStr = filePath.replace(toUnixPath(metaInfo.belongProjectPath), '');
      }
    } else {
      sufStr = filePath.replace(toUnixPath(projectRootPath), '');
    }
    const output: string = path.join(cachePath, sufStr);
    return output;
  }

  private getPkgModulesFilePkgName(pkgPath: string) {
    pkgPath = toUnixPath(pkgPath);
    const packageDir: string = this.projectConfig.packageDir;
    const projectRootPath = toUnixPath(this.projectConfig.projectRootPath);
    const projectPkgModulesPath: string = toUnixPath(path.join(projectRootPath, packageDir));
    let pkgName: string = '';
    if (pkgPath.includes(projectPkgModulesPath)) {
      pkgName = path.join(PACKAGES, pkgPath.replace(projectPkgModulesPath, ''));
    } else {
      for (const key in this.projectConfig.modulePathMap) {
        const value: string = this.projectConfig.modulePathMap[key];
        const fakeModulePkgModulesPath: string = toUnixPath(path.resolve(value, packageDir));
        if (pkgPath.indexOf(fakeModulePkgModulesPath) !== -1) {
          const tempFilePath: string = pkgPath.replace(projectRootPath, '');
          pkgName = path.join(`${PACKAGES}@${key}`,
            tempFilePath.substring(tempFilePath.indexOf(packageDir) + packageDir.length + 1));
          break;
        }
      }
    }

    return pkgName.replace(new RegExp(packageDir, 'g'), PACKAGES);
  }

  private generateProtoFilesInfo() {
    validateFilePathLength(this.protoFilePath, this.logger);
    mkdirsSync(path.dirname(this.protoFilePath));
    let protoFilesInfo: string = '';
    const sortModuleInfos: Object = new Map([...this.moduleInfos].sort());
    for (const value of sortModuleInfos.values()) {
      const cacheProtoPath: string = changeFileExtension(value.cacheFilePath, EXTNAME_PROTO_BIN);
      protoFilesInfo += `${toUnixPath(cacheProtoPath)}\n`;
    }
    if (this.pkgEntryInfos.size > 0) {
      protoFilesInfo += `${toUnixPath(this.npmEntriesProtoFilePath)}\n`;
    }
    fs.writeFileSync(this.protoFilePath, protoFilesInfo, 'utf-8');
  }

  private mergeProtoToAbc() {
    mkdirsSync(this.projectConfig.aceModuleBuild);
    const cmd: string = `"${this.arkConfig.mergeAbcPath}" --input "@${this.protoFilePath}" --outputFilePath "${
      this.projectConfig.aceModuleBuild}" --output ${MODULES_ABC} --suffix protoBin`;
    try {
      childProcess.execSync(cmd, { windowsHide: true });
    } catch (e) {
      this.logger.throwArkTsCompilerError('ArkTS:INTERNAL ERROR: Failed to merge proto file to abc.\n' +
        'Error message:' + e.toString());
    }
  }

  private afterCompilationProcess() {
    this.writeHashJson();
  }

  private writeHashJson() {
    if (this.hashJsonFilePath.length === 0) {
      return;
    }

    for (const value of this.filterModuleInfos.values()) {
      const cacheFilePath: string = value.cacheFilePath;
      const cacheProtoFilePath: string = changeFileExtension(cacheFilePath, EXTNAME_PROTO_BIN);
      if (!fs.existsSync(cacheFilePath) || !fs.existsSync(cacheProtoFilePath)) {
        this.logger.throwArkTsCompilerError(
          `ArkTS:ERROR ${cacheFilePath} or  ${cacheProtoFilePath} is lost`
        );
      }
      const hashCacheFileContentData: string = toHashData(cacheFilePath);
      const hashCacheProtoContentData: string = toHashData(cacheProtoFilePath);
      this.hashJsonObject[cacheFilePath] = hashCacheFileContentData;
      this.hashJsonObject[cacheProtoFilePath] = hashCacheProtoContentData;
    }

    fs.writeFileSync(this.hashJsonFilePath, JSON.stringify(this.hashJsonObject));
  }

  private generateNpmEntryToGenProto() {
    if (this.pkgEntryInfos.size <= 0) {
      return;
    }
    mkdirsSync(path.dirname(this.npmEntriesProtoFilePath));
    const cmd: string = `"${this.arkConfig.js2abcPath}" --compile-npm-entries "${
      this.npmEntriesInfoPath}" "${this.npmEntriesProtoFilePath}"`;
    try {
      childProcess.execSync(cmd, { windowsHide: true });
    } catch (e) {
      this.logger.throwArkTsCompilerError('ArkTS:ERROR failed to generate npm proto file to abc. Error message: ' + e.toString());
    }
  }

  private removeCompilationCache(): void {
    if (isEs2Abc(this.projectConfig)) {
      this.removeEs2abcCompilationCache();
    } else if (isTs2Abc(this.projectConfig)) {
      this.removeTs2abcCompilationCache();
    } else {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
        ArkTSInternalErrorDescription,
        'Invalid compilation mode. ' +
        `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
      );
      this.logger.printErrorAndExit(errInfo);
    }
  }

  private removeEs2abcCompilationCache(): void {
    if (fs.existsSync(this.cacheFilePath)) {
      const data: string = fs.readFileSync(this.cacheFilePath, 'utf-8');
      const lines: string[] = data.split(/\r?\n/);
      lines.forEach(line => {
        const [, abcCacheFilePath]: string[] = line.split(';');
        if (fs.existsSync(abcCacheFilePath)) {
          fs.unlinkSync(abcCacheFilePath);
        }
      });
      fs.unlinkSync(this.cacheFilePath);
    }
  }

  private removeTs2abcCompilationCache(): void {
    if (fs.existsSync(this.hashJsonFilePath)) {
      fs.unlinkSync(this.hashJsonFilePath);
    }
    if (fs.existsSync(this.protoFilePath)) {
      const data: string = fs.readFileSync(this.protoFilePath, 'utf-8');
      const lines: string[] = data.split(/\r?\n/);
      lines.forEach(line => {
        const protoFilePath: string = line;
        if (fs.existsSync(protoFilePath)) {
          fs.unlinkSync(protoFilePath);
        }
      });
      fs.unlinkSync(this.protoFilePath);
    }
  }
}
