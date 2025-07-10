/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import path from 'path';
import fs from 'fs';
import {
  EXTNAME_ETS,
  EXTNAME_JS,
  EXTNAME_TS,
  EXTNAME_MJS,
  EXTNAME_CJS,
  SOURCEMAPS,
  SOURCEMAPS_JSON,
  yellow,
  reset
} from "./common/ark_define";
import {
  changeFileExtension,
  isCommonJsPluginVirtualFile,
  isCurrentProjectFiles,
  isDebug,
  shouldETSOrTSFileTransformToJS
} from "./utils";
import {
  toUnixPath,
  isPackageModulesFile,
  getProjectRootPath
} from "../../utils";
import {
  handleObfuscatedFilePath,
  mangleFilePath,
  enableObfuscateFileName
} from './common/ob_config_resolver';
import { MemoryMonitor } from '../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../meomry_monitor/memory_define';
import {
  ArkTSInternalErrorDescription,
  ErrorCode
} from './error_code';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from './logger';
import {
  createAndStartEvent,
  CompileEvent,
  stopEvent
} from '../../performance';
import { BytecodeObfuscator } from './bytecode_obfuscator';
import { createInterface } from 'readline';

let isShouldSourceMap: boolean = true;

export class SourceMapGenerator {
  private static instance: SourceMapGenerator | undefined = undefined;
  private static rollupObject: Object | undefined;

  private projectConfig: Object;
  private sourceMapPath: string;
  private sourceMapPathTmp: string;
  private cacheSourceMapPath: string;
  private triggerAsync: Object;
  private triggerEndSignal: Object;
  private sourceMaps: Object = {};
  private sourceMapKeys: Set<string> = new Set([]);
  private isNewSourceMap: boolean = true;
  private keyCache: Map<string, string> = new Map();
  private logger: CommonLogger;
  private isFirstAppend: boolean = true;
  private isCompileSingle: boolean = false;
  private originFd: number;
  private tempFd: number;

  public sourceMapKeyMappingForObf: Map<string, string> = new Map();

  constructor(rollupObject: Object) {
    this.projectConfig = Object.assign(rollupObject.share.arkProjectConfig, rollupObject.share.projectConfig);
    this.sourceMapPath = this.getSourceMapSavePath();
    this.sourceMapPathTmp = path.join(this.projectConfig.cachePath, SOURCEMAPS_JSON + '.tmp');
    this.cacheSourceMapPath = path.join(this.projectConfig.cachePath, SOURCEMAPS_JSON);
    this.triggerAsync = rollupObject.async;
    this.triggerEndSignal = rollupObject.signal;
    this.logger = CommonLogger.getInstance(rollupObject);
  }

  static init(rollupObject: Object): void {
    SourceMapGenerator.rollupObject = rollupObject;
    SourceMapGenerator.instance = new SourceMapGenerator(SourceMapGenerator.rollupObject);

    // adapt compatibility with hvigor
    if (!SourceMapGenerator.instance.projectConfig.entryPackageName ||
      !SourceMapGenerator.instance.projectConfig.entryModuleVersion) {
      SourceMapGenerator.instance.isNewSourceMap = false;
    }

    if ((SourceMapGenerator.instance.projectConfig.hotReload &&
      SourceMapGenerator.instance.projectConfig.watchMode !== 'true') ||
      (SourceMapGenerator.instance.projectConfig.coldReload)) {
      isShouldSourceMap = this.projectConfig.isFirstBuild;
    }

    SourceMapGenerator.instance.isCompileSingle = SourceMapGenerator.instance.isNewSourceMap &&
      SourceMapGenerator.instance.projectConfig.singleFileEmit && isShouldSourceMap;

    if (isShouldSourceMap && SourceMapGenerator.instance.isNewSourceMap) {
      if (fs.existsSync(SourceMapGenerator.instance.sourceMapPath)) {
        fs.unlinkSync(SourceMapGenerator.instance.sourceMapPath);
      }
      if (fs.existsSync(SourceMapGenerator.instance.sourceMapPathTmp)) {
        fs.unlinkSync(SourceMapGenerator.instance.sourceMapPathTmp);
      }

      const sourceMapPathDir: string = path.dirname(SourceMapGenerator.instance.sourceMapPath);
      if (!fs.existsSync(sourceMapPathDir)) {
        fs.mkdirSync(sourceMapPathDir, { recursive: true });
      }

      const sourceMapPathTmpDir: string = path.dirname(SourceMapGenerator.instance.sourceMapPathTmp);
      if (!fs.existsSync(sourceMapPathTmpDir)) {
        fs.mkdirSync(sourceMapPathTmpDir, { recursive: true });
      }
    }

    if (SourceMapGenerator.instance.projectConfig.hotReload) {
      isShouldSourceMap = false;
    }
  }

  static getInstance(): SourceMapGenerator {
    if (!SourceMapGenerator.instance) {
      SourceMapGenerator.instance = new SourceMapGenerator(SourceMapGenerator.rollupObject);
    }
    return SourceMapGenerator.instance;
  }

  //In window plateform, if receive path join by '/', should transform '/' to '\'
  private getAdaptedModuleId(moduleId: string): string {
    return moduleId.replace(/\//g, path.sep);
  }

  private getPkgInfoByModuleId(moduleId: string, shouldObfuscateFileName: boolean = false): Object {
    moduleId = this.getAdaptedModuleId(moduleId);

    const moduleInfo: Object = SourceMapGenerator.rollupObject.getModuleInfo(moduleId);
    if (!moduleInfo) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_GET_MODULE_INFO_FAILED,
        ArkTSInternalErrorDescription,
        `Failed to get ModuleInfo, moduleId: ${moduleId}`
      );
      this.logger.printErrorAndExit(errInfo);
    }
    const metaInfo: Object = moduleInfo['meta'];
    if (!metaInfo) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_MODULE_INFO_META,
        ArkTSInternalErrorDescription,
        `Failed to get ModuleInfo properties 'meta', moduleId: ${moduleId}`
      );
      this.logger.printErrorAndExit(errInfo);
    }
    const pkgPath = metaInfo['pkgPath'];
    if (!pkgPath) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_MODULE_INFO_META_PKG_PATH,
        ArkTSInternalErrorDescription,
        `Failed to get ModuleInfo properties 'meta.pkgPath', moduleId: ${moduleId}`
      );
      this.logger.printErrorAndExit(errInfo);
    }

    const dependencyPkgInfo = metaInfo['dependencyPkgInfo'];
    let middlePath = this.getIntermediateModuleId(moduleId, metaInfo).replace(pkgPath + path.sep, '');
    if (shouldObfuscateFileName) {
      middlePath = mangleFilePath(middlePath);
    }
    return {
      entry: {
        name: this.projectConfig.entryPackageName,
        version: this.projectConfig.entryModuleVersion
      },
      dependency: dependencyPkgInfo ? {
        name: dependencyPkgInfo['pkgName'],
        version: dependencyPkgInfo['pkgVersion']
      } : undefined,
      modulePath: toUnixPath(middlePath)
    };
  }

  public setNewSoureMaps(isNewSourceMap: boolean): void {
    this.isNewSourceMap = isNewSourceMap;
  }

  public isNewSourceMaps(): boolean {
    return this.isNewSourceMap;
  }

  //generate sourcemap key, notice: moduleId is absolute path
  public genKey(moduleId: string, shouldObfuscateFileName: boolean = false): string {
    moduleId = this.getAdaptedModuleId(moduleId);

    let key: string = this.keyCache.get(moduleId);
    if (key && !shouldObfuscateFileName) {
      return key;
    }
    const pkgInfo = this.getPkgInfoByModuleId(moduleId, shouldObfuscateFileName);
    if (pkgInfo.dependency) {
      key = `${pkgInfo.entry.name}|${pkgInfo.dependency.name}|${pkgInfo.dependency.version}|${pkgInfo.modulePath}`;
    } else {
      key = `${pkgInfo.entry.name}|${pkgInfo.entry.name}|${pkgInfo.entry.version}|${pkgInfo.modulePath}`;
    }
    if (key && !shouldObfuscateFileName) {
      this.keyCache.set(moduleId, key);
    }
    return key;
  }

  private getSourceMapSavePath(): string {
    if (this.projectConfig.compileHar && this.projectConfig.sourceMapDir && !this.projectConfig.byteCodeHar) {
      return path.join(this.projectConfig.sourceMapDir, SOURCEMAPS);
    }
    return isDebug(this.projectConfig) ? path.join(this.projectConfig.aceModuleBuild, SOURCEMAPS) :
      path.join(this.projectConfig.cachePath, SOURCEMAPS);
  }

  async *readLines(filePath: string): AsyncGenerator<string> {
    const fileStream = fs.createReadStream(filePath, { encoding: 'utf8' });
    const rl = createInterface({
      input: fileStream,
      crlfDelay: Infinity
    });

    try {
      for await (const line of rl) {
        yield line;
      }
    } finally {
      rl.close();
      fileStream.close();
    }
  }

  public formatOrigin(key: string, val: object): string {
    return `  "${key}": ${JSON.stringify(val, null, 2).replace(/\n/g, '\n  ')}`;
  }

  public formatTemp(key: string, val: object): string {
    return `{"key": "${key}", "val": ${JSON.stringify(val)}}`;
  }

  public writeOrigin(content: string): void {
    if (!this.originFd) {
      this.originFd = fs.openSync(this.sourceMapPath, 'a');
    }
    fs.appendFileSync(this.originFd, content, { encoding: 'utf8' });
  }

  public writeTemp(content: string): void {
    if (!this.tempFd) {
      this.tempFd = fs.openSync(this.sourceMapPathTmp, 'a');
    }
    fs.appendFileSync(this.tempFd, content, { encoding: 'utf8' });
  }

  public closeFd(): void {
    if (this.originFd) {
      fs.closeSync(this.originFd);
      this.originFd = undefined;
    }
    if (this.tempFd) {
      fs.closeSync(this.tempFd);
      this.tempFd = undefined;
    }
  }

  public convertSourceMapToCache(maps: Object): string {
    let isFirstLine: boolean = true;
    let cacheContent: string = '';
    Object.keys(maps).forEach(key => {
      let contentTmp: string = this.formatTemp(key, maps[key]);
      if (isFirstLine) {
        isFirstLine = false;
        cacheContent = contentTmp;
      } else {
        cacheContent += `\n${contentTmp}`;
      }
    });
    return cacheContent;
  }

  public writeModifiedSourceMapToFile(parentEvent: CompileEvent | undefined): void {
    const eventWriteCachedSourceMaps = createAndStartEvent(parentEvent, 'write source maps');
    Object.keys(this.sourceMaps).forEach(key => {
      let keyChanged: string = '';
      if (this.sourceMapKeyMappingForObf.has(key)) {
        keyChanged = this.sourceMapKeyMappingForObf.get(key);
      } else {
        keyChanged = key;
      }
      this.sourceMapKeys.add(keyChanged);

      let contentJson: string = this.formatOrigin(keyChanged, this.sourceMaps[key]);
      let contentTmp: string = this.formatTemp(keyChanged, this.sourceMaps[key]);
      if (this.isFirstAppend) {
        this.isFirstAppend = false;
        this.writeOrigin(`{\n${contentJson}`);
        this.writeTemp(`${contentTmp}`);
      } else {
        this.writeOrigin(`,\n${contentJson}`);
        this.writeTemp(`\n${contentTmp}`);
      }
    });
    this.sourceMaps = {};
    this.sourceMapKeyMappingForObf.clear();
    stopEvent(eventWriteCachedSourceMaps);
  }

  public async writeUsedAndUnmodifiedSourceMapToFile(parentEvent: CompileEvent | undefined): Promise<void> {
    const eventMergeCachedSourceMaps = createAndStartEvent(parentEvent, 'merge cached source maps');
    let cacheSourceMapInfo: Object = this.getCacheSourceMapInfo();
    if (!cacheSourceMapInfo.exist) {
      if (!this.isFirstAppend) {
        this.writeOrigin('\n}');
      }
      this.closeFd();
      fs.renameSync(this.sourceMapPathTmp, this.cacheSourceMapPath);
      stopEvent(eventMergeCachedSourceMaps);
      return;
    }

    await (async (): Promise<void> => {
      let compileFileList: Set<string> = this.getCompileFileList();
      for await (const line of this.readLines(cacheSourceMapInfo.path)) {
        if (line.trim() === '') {
          continue;
        }
        let smObj: Object = JSON.parse(line.trim());
        if (!compileFileList.has(smObj.key) || this.sourceMapKeys.has(smObj.key)) {
          // skip unuse or uncompile in cache
          continue;
        }
        this.writeOrigin(`,\n${this.formatOrigin(smObj.key, smObj.val)}`);
        this.writeTemp(`\n${this.formatTemp(smObj.key, smObj.val)}`);
      }
      if (!this.isFirstAppend) {
        this.writeOrigin('\n}');
      }
      this.closeFd();
      fs.unlinkSync(this.cacheSourceMapPath);
      fs.renameSync(this.sourceMapPathTmp, this.cacheSourceMapPath);
    })();
    stopEvent(eventMergeCachedSourceMaps);
  }

  public buildModuleSourceMapInfoSingle(parentEvent: CompileEvent | undefined): void {
    if (!this.isCompileSingle) {
      return;
    }
    if (Object.keys(this.sourceMaps).length === 0) {
      return;
    }
    this.writeModifiedSourceMapToFile(parentEvent);
  }

  public buildModuleSourceMapInfo(parentEvent: CompileEvent | undefined): void {
    if (this.isCompileSingle) {
      this.writeUsedAndUnmodifiedSourceMapToFile(parentEvent);
    } else {
      this.buildModuleSourceMapInfoAll(parentEvent);
    }
  }

  public buildModuleSourceMapInfoAll(parentEvent: CompileEvent | undefined): void {
    if (this.projectConfig.widgetCompile) {
      return;
    }

    const eventUpdateCachedSourceMaps = createAndStartEvent(parentEvent, 'update cached source maps');
    // If hap/hsp depends on bytecode har under debug mode, the source map of bytecode har need to be merged with
    // source map of hap/hsp.
    if (isDebug(this.projectConfig) && !this.projectConfig.byteCodeHar && !!this.projectConfig.byteCodeHarInfo) {
      Object.keys(this.projectConfig.byteCodeHarInfo).forEach((packageName) => {
        const sourceMapsPath = this.projectConfig.byteCodeHarInfo[packageName].sourceMapsPath;
        if (!sourceMapsPath && !!this.logger && !!this.logger.warn) {
          this.logger.warn(yellow, `ArkTS:WARN Property 'sourceMapsPath' not found in '${packageName}'.`, reset);
        }
        if (!!sourceMapsPath) {
          const bytecodeHarSourceMap = JSON.parse(fs.readFileSync(toUnixPath(sourceMapsPath)).toString());
          Object.assign(this.sourceMaps, bytecodeHarSourceMap);
        }
      });
    }

    if (this.isNewSourceMap) {
      this.writeModifiedSourceMapToFile(parentEvent);
      this.writeUsedAndUnmodifiedSourceMapToFile(parentEvent);
      return;
    }

    const updateSourceRecordInfo = MemoryMonitor.recordStage(MemoryDefine.UPDATE_SOURCE_MAPS);
    const cacheSourceMapObject: Object = this.updateCachedSourceMaps();
    MemoryMonitor.stopRecordStage(updateSourceRecordInfo);
    stopEvent(eventUpdateCachedSourceMaps);

    this.triggerAsync(() => {
      const eventWriteFile = createAndStartEvent(parentEvent, 'write source map (async)', true);
      fs.writeFile(this.sourceMapPath, JSON.stringify(cacheSourceMapObject, null, 2), 'utf-8', (err) => {
        if (err) {
          const errInfo: LogData = LogDataFactory.newInstance(
            ErrorCode.ETS2BUNDLE_INTERNAL_WRITE_SOURCE_MAP_FAILED,
            ArkTSInternalErrorDescription,
            `Failed to write sourceMaps. ${err.message}`,
            this.sourceMapPath
          );
          this.logger.printErrorAndExit(errInfo);
        }
        fs.copyFileSync(this.sourceMapPath, this.cacheSourceMapPath);
        stopEvent(eventWriteFile, true);
        this.triggerEndSignal();
      });
    });
  }

  public isEmptyFile(filePath: string): boolean {
    const stats = fs.statSync(filePath);
    return stats.size === 0;
  }

  public getCacheSourceMapInfo(): Object {
    let existCacheSourceMap: boolean = false;
    let cacheSourceMapPath: string = '';
    let cacheSourceMapPathTmp: string = '';
    /**
     * bytecode obfuscation requires that the input sourceMap must be unobfuscated,
     * the sourceMap will be saved in the cache directory before the first bytecode obfuscation,
     * and it will as the input for merging sourceMap during incremental compilation.
     */
    if (BytecodeObfuscator.enable) {
      cacheSourceMapPathTmp = BytecodeObfuscator.getInstance().getBackupSourceMapPath();
      if (fs.existsSync(cacheSourceMapPathTmp) && !this.isEmptyFile(cacheSourceMapPathTmp)) {
        existCacheSourceMap = true;
        cacheSourceMapPath = cacheSourceMapPathTmp;
      }
    }

    cacheSourceMapPathTmp = this.cacheSourceMapPath;
    if (!existCacheSourceMap && fs.existsSync(cacheSourceMapPathTmp) && !this.isEmptyFile(cacheSourceMapPathTmp)) {
      existCacheSourceMap = true;
      cacheSourceMapPath = cacheSourceMapPathTmp;
    }

    return { exist: existCacheSourceMap, path: cacheSourceMapPath };
  }

  //update cache sourcemap object
  public updateCachedSourceMaps(): Object {
    if (!this.isNewSourceMap) {
      this.modifySourceMapKeyToCachePath(this.sourceMaps);
    }

    let cacheSourceMapObject: Object = null;
    let cacheSourceMapInfo: Object = this.getCacheSourceMapInfo();
    if (!cacheSourceMapInfo.exist) {
      cacheSourceMapObject = this.sourceMaps;
    } else {
      cacheSourceMapObject = JSON.parse(fs.readFileSync(cacheSourceMapInfo.path).toString().trim());
      // remove unused source files's sourceMap
      let unusedFiles = [];
      let compileFileList: Set<string> = this.getCompileFileList();

      Object.keys(cacheSourceMapObject).forEach(key => {
        let newkeyOrOldCachePath = key;
        if (!this.isNewSourceMap) {
          newkeyOrOldCachePath = toUnixPath(path.join(this.projectConfig.projectRootPath, key));
        }
        if (!compileFileList.has(newkeyOrOldCachePath)) {
          unusedFiles.push(key);
        }
      });
      unusedFiles.forEach(file => {
        delete cacheSourceMapObject[file];
      })

      // update sourceMap
      Object.keys(this.sourceMaps).forEach(key => {
        cacheSourceMapObject[key] = this.sourceMaps[key];
      });
    }
    // update the key for filename obfuscation
    for (let [key, newKey] of this.sourceMapKeyMappingForObf) {
      this.updateSourceMapKeyWithObf(cacheSourceMapObject, key, newKey);
    }
    return cacheSourceMapObject;
  }

  public getCompileFileList(): Set<string> {
    let compileFileList: Set<string> = new Set();
    for (let moduleId of SourceMapGenerator.rollupObject.getModuleIds()) {
      // exclude .dts|.d.ets file
      if (isCommonJsPluginVirtualFile(moduleId) || !isCurrentProjectFiles(moduleId, this.projectConfig)) {
        continue;
      }

      if (this.isNewSourceMap) {
        const isPackageModules = isPackageModulesFile(moduleId, this.projectConfig);
        if (enableObfuscateFileName(isPackageModules, this.projectConfig)) {
          compileFileList.add(this.genKey(moduleId, true));
        } else {
          compileFileList.add(this.genKey(moduleId));
        }
        continue;
      }

      // adapt compatibilty with hvigor
      const projectRootPath = getProjectRootPath(moduleId, this.projectConfig, this.projectConfig?.rootPathSet);
      let cacheModuleId = this.getIntermediateModuleId(toUnixPath(moduleId))
        .replace(toUnixPath(projectRootPath), toUnixPath(this.projectConfig.cachePath));

      const isPackageModules = isPackageModulesFile(moduleId, this.projectConfig);
      if (enableObfuscateFileName(isPackageModules, this.projectConfig)) {
        compileFileList.add(mangleFilePath(cacheModuleId));
      } else {
        compileFileList.add(cacheModuleId);
      }
    }
    return compileFileList;
  }

  public getSourceMaps(): Object {
    return this.sourceMaps;
  }

  public getSourceMap(moduleId: string): Object {
    return this.getSpecifySourceMap(this.sourceMaps, moduleId);
  }

  //get specify sourcemap, allow receive param sourcemap
  public getSpecifySourceMap(specifySourceMap: Object, moduleId: string): Object {
    const key = this.isNewSourceMap ? this.genKey(moduleId) : moduleId;
    if (specifySourceMap && specifySourceMap[key]) {
      return specifySourceMap[key];
    }
    return undefined;
  }

  public updateSourceMap(moduleId: string, map: Object) {
    if (!this.sourceMaps) {
      this.sourceMaps = {};
    }
    this.updateSpecifySourceMap(this.sourceMaps, moduleId, map);
  }

  //update specify sourcemap, allow receive param sourcemap
  public updateSpecifySourceMap(specifySourceMap: Object, moduleId: string, sourceMap: Object) {
    const key = this.isNewSourceMap ? this.genKey(moduleId) : moduleId;
    specifySourceMap[key] = sourceMap;
  }

  public fillSourceMapPackageInfo(moduleId: string, sourcemap: Object) {
    if (!this.isNewSourceMap) {
      return;
    }

    const pkgInfo = this.getPkgInfoByModuleId(moduleId);
    sourcemap['entry-package-info'] = `${pkgInfo.entry.name}|${pkgInfo.entry.version}`;
    if (pkgInfo.dependency) {
      sourcemap['package-info'] = `${pkgInfo.dependency.name}|${pkgInfo.dependency.version}`;
    }
  }

  private getIntermediateModuleId(moduleId: string, metaInfo?: Object): string {
    let extName: string = "";
    switch (path.extname(moduleId)) {
      case EXTNAME_ETS: {
        extName = shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : EXTNAME_TS;
        break;
      }
      case EXTNAME_TS: {
        extName = shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : '';
        break;
      }
      case EXTNAME_JS:
      case EXTNAME_MJS:
      case EXTNAME_CJS: {
        extName = (moduleId.endsWith(EXTNAME_MJS) || moduleId.endsWith(EXTNAME_CJS)) ? EXTNAME_JS : '';
        break;
      }
      default:
        break;
    }
    if (extName.length !== 0) {
      return changeFileExtension(moduleId, extName);
    }
    return moduleId;
  }

  public setSourceMapPath(path: string): void {
    this.sourceMapPath = path;
  }

  public modifySourceMapKeyToCachePath(sourceMap: object): void {
    const projectConfig: object = this.projectConfig;

    // modify source map keys to keep IDE tools right
    const relativeCachePath: string = toUnixPath(projectConfig.cachePath.replace(
      projectConfig.projectRootPath + path.sep, ''));
    Object.keys(sourceMap).forEach(key => {
      let newKey: string = relativeCachePath + '/' + key;
      if (!newKey.endsWith(EXTNAME_JS)) {
        const moduleId: string = this.projectConfig.projectRootPath + path.sep + key;
        const extName: string = shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig) ? EXTNAME_JS : EXTNAME_TS;
        newKey = changeFileExtension(newKey, extName);
      }
      const isOhModules = key.startsWith('oh_modules');
      newKey = handleObfuscatedFilePath(newKey, isOhModules, this.projectConfig);
      sourceMap[newKey] = sourceMap[key];
      delete sourceMap[key];
    });
  }

  public static cleanSourceMapObject(): void {
    if (this.instance) {
      this.instance.closeFd();
      this.instance.keyCache.clear();
      this.instance.sourceMaps = undefined;
      this.instance = undefined;
    }
    if (this.rollupObject) {
      this.rollupObject = undefined;
    }
  }

  private updateSourceMapKeyWithObf(specifySourceMap: Object, key: string, newKey: string): void {
    if (!specifySourceMap.hasOwnProperty(key) || key === newKey) {
      return;
    }
    specifySourceMap[newKey] = specifySourceMap[key];
    delete specifySourceMap[key];
  }

  public saveKeyMappingForObfFileName(originalFilePath: string): void {
    this.sourceMapKeyMappingForObf.set(this.genKey(originalFilePath), this.genKey(originalFilePath, true));
  }

  //use by UT
  static initInstance(rollupObject: Object): SourceMapGenerator {
    if (!SourceMapGenerator.instance) {
      SourceMapGenerator.init(rollupObject);
    }
    return SourceMapGenerator.getInstance();
  }
}