/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import path from 'path';
import fs from 'fs';
import {
  FILESINFO_TXT,
  MODULES_ABC,
  SOURCEMAPS
} from '../../pre_define';
import {
  mkDir,
  writeFileSync
} from '../../utils';
import {
  ES_ANNOTATIONS,
  SOURCEMAPS_JSON,
  WIDGETS_ABC,
  yellow,
  DECORATOR_WHITE_LIST,
  BYTECODE_OBFUSCATION_PROPERTY_WHITE_LIST,
  IDENTIFIER_CACHE,
  MEMBER_METHOD_CACHE
} from './common/ark_define';
import {
  ReservedNameInfo,
  separateUniversalReservedItem,
  UnobfuscationCollections,
} from 'arkguard';
import {
  SourceMapSegmentObj,
  decodeSourcemap,
  SourceMapLink,
  Source
} from 'arkguard/lib/utils/SourceMapMergingUtil';
import { MergedConfig } from './common/ob_config_resolver';
import { ModuleInfo } from './module/module_mode';
import { initArkConfig } from './common/process_ark_config';
import esInfo from 'arkguard/lib/configs/preset/es_reserved_properties.json';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from './logger';
import {
  ArkTSErrorDescription,
  ErrorCode
} from './error_code';
import { FILE_WHITE_LISTS } from 'arkguard/lib/utils/ProjectCollections';
import {
  FileKeepInfo,
  FileReservedInfo
} from 'arkguard/lib/utils/ProjectCollections';
import { SourceMapGenerator } from './generate_sourcemap';

const FILE_NAME_CACHE: string = 'FileNameCache';
const OBF_NAME_MAP: string = 'ObfNameMap';
const PROPERTY_CACHE: string = 'ProPertyCache';
const ENTRY_PACKAGE_INFO: string = 'entryPackageInfo';
const COMPILE_SDK_VERSION: string = 'compileSdkVersion';

export class BytecodeObfuscator {
  public static enable = false;
  private static instance: BytecodeObfuscator | undefined = undefined;
  public cmdArgs: string[];
  private projectConfig: Object;
  private logger: Object;
  private originDir: string;
  private obfDir: string;
  private obfuscationCacheDir: string;
  private bytecodeObfuscateConfig: BytecodeObfuscationConfig;
  private debugging: boolean;
  private enhanced: boolean;
  private arkConfig: ArkConfig;
  private configPath: string;
  private nameCachePath: string;
  private backupSourceMapPath: string;

  constructor(share: Object, arkProjectConfig: Object) {
    this.projectConfig = Object.assign(arkProjectConfig, share.projectConfig);
    const rollup = {
      share: share,
    };
    this.logger = CommonLogger.getInstance(rollup);
    this.obfuscationCacheDir = this.projectConfig.obfuscationOptions.obfuscationCacheDir;
    this.originDir = path.join(this.obfuscationCacheDir, 'origin');
    this.obfDir = path.join(this.obfuscationCacheDir, 'obf');
    this.configPath = path.join(this.obfuscationCacheDir, 'config.json');
    this.backupSourceMapPath = path.join(this.originDir, SOURCEMAPS_JSON);
    /**
     * When enhanced == true,
     * a bytecode har will be obfuscated when it is referenced by HAP,
     * but it is not obfuscated by default
     */
    this.enhanced = false;
    this.debugging = arkProjectConfig.obfuscationMergedObConfig?.options.bytecodeObf?.debugging;
    this.arkConfig = initArkConfig(this.projectConfig);
    this.cmdArgs = [];
  }

  public static cleanBcObfuscatorObject(): void {
    if (this.instance) {
      this.instance = undefined;
    }
    if (this.enable) {
      this.enable = null;
    }
  }

  static initForTest(share: Object): void {
    if (!share.arkProjectConfig.isBytecodeObfEnabled) {
      this.enable = false;
      return;
    }
    this.enable = true;
    BytecodeObfuscator.instance = new BytecodeObfuscator(share, share.arkProjectConfig);
    BytecodeObfuscator.instance.bytecodeObfuscateConfig =
      new BytecodeObfuscationConfig(share, share.arkProjectConfig);
  }

  static init(share: Object, arkProjectConfig: Object): void {
    if (!arkProjectConfig.isBytecodeObfEnabled) {
      this.enable = false;
      return;
    }
    this.enable = true;
    BytecodeObfuscator.instance = new BytecodeObfuscator(share, arkProjectConfig);
    if (!isVersionCompatible(
      BytecodeObfuscator.instance.projectConfig.compatibleSdkVersion,
      BytecodeObfuscator.instance.projectConfig.compatibleSdkVersionStage)) {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.BYTECODE_OBFUSCATION_UNSUPPORT_COMPATIBLESDKVERSION,
        ArkTSErrorDescription,
        `bytecodeObfuscation only can be used in the beta3 version of API 12 or higher version`,
        '',
        [
          'Check the compatibleSdkVersion and compatibleSdkVersionStage in build-profile.json',
          'if compatibleSdkVersion is set to API 12,then compatibleSdkVersionStage needs to be configured as beta3',
        ]
      );
      BytecodeObfuscator.instance.logger.printErrorAndExit(errInfo);
    }
    if (!fs.existsSync(BytecodeObfuscator.instance.originDir)) {
      mkDir(BytecodeObfuscator.instance.originDir);
    }
    if (!fs.existsSync(BytecodeObfuscator.instance.obfDir)) {
      mkDir(BytecodeObfuscator.instance.obfDir);
    }

    BytecodeObfuscator.instance.bytecodeObfuscateConfig =
      new BytecodeObfuscationConfig(share, arkProjectConfig);
    this.getInstance().getSystemApiConfigsByCache();
    this.getInstance().bytecodeObfuscateConfig.addReservedNames(collectReservedLangForProperty());
  }

  static getInstance(): BytecodeObfuscator {
    return BytecodeObfuscator.instance;
  }

  public removeStructProp(): void {
    if (!BytecodeObfuscator.enable ||
      !this.bytecodeObfuscateConfig.obfuscationRules.enableDecoratorObfuscation) {
      return;
    }

    const reservedNames = this.bytecodeObfuscateConfig.getReservedNames();

    for (const item of [...UnobfuscationCollections.reservedStruct]) {
      if (!reservedNames?.has(item)) {
        UnobfuscationCollections.reservedStruct.delete(item);
      }
    }
  }

  public execute(moduleInfos: Map<String, ModuleInfo>): void {
    if (!BytecodeObfuscator.enable) {
      return;
    }
    this.collectSkipModuleName();
    this.convertAbstractPathToRecordName(moduleInfos);
    this.collectWhiteList();
    const fileWhiteListsCachePath = path.join(this.obfuscationCacheDir, FILE_WHITE_LISTS);
    this.collectPropertyBySourceScan(fileWhiteListsCachePath);
    fs.writeFileSync(this.configPath, JSON.stringify(this.bytecodeObfuscateConfig, (key, value) => {
      if (value instanceof Set) {
        return Array.from(value);
      }
      return value;
    }, 2));
    this.generateObfCmd();
    this.createWarnings();
    const outPutABC: string = this.projectConfig.widgetCompile ? WIDGETS_ABC : MODULES_ABC;
    const abcPath = path.join(this.projectConfig.aceModuleBuild, outPutABC);
    //backup abc file
    fs.copyFileSync(abcPath, this.bytecodeObfuscateConfig.abcFilePath);
    const result = childProcess.spawnSync(this.arkConfig.bcObfuscatorPath, this.cmdArgs, {
      env: { ...process.env }
    });
    if (result.status !== 0) {
      const stderrString = result.stderr ? result.stderr.toString() : '';
      const errInfo: LogData = LogDataFactory.newInstanceFromBytecodeObfuscation(stderrString, result.status);
      BytecodeObfuscator.instance.logger.printErrorAndExit(errInfo);
    }
    fs.copyFileSync(this.bytecodeObfuscateConfig.obfAbcFilePath, abcPath);
    if (this.projectConfig.widgetCompile) {
      return;
    }
    this.updateAfterObfuscation();
  }

  private collectSkipModuleName(): void {
    const matchedPaths = new Set<string>();

    // collect keep bytecode har module pkgName
    const regex = /\/oh_modules\/([^/]+)$/;
    this.projectConfig.obfuscationMergedObConfig.keepSourceOfPaths.forEach(item => {
      const match = item.match(regex);
      if (match) {
        matchedPaths.add(match[1]);
      }
    });
    // collect remote har pkgName,which shouldn't to be obfuscated
    this.projectConfig.byteCodeHarInfo &&
      Object.entries(this.projectConfig.byteCodeHarInfo).forEach(([key, value]) => {
        if (!isVersionCompatible(value?.compatibleSdkVersion, null) || !this.enhanced || matchedPaths.has(key)) {
          this.bytecodeObfuscateConfig.addSkippedRemoteHarList(key);
          this.projectConfig.byteCodeHarToDependencyKeys?.get(key)?.forEach(dependentModule => {
            this.bytecodeObfuscateConfig.addSkippedRemoteHarList(dependentModule);
          });
        }
      });
  }

  public convertAbstractPathToRecordName(moduleInfos: Map<String, ModuleInfo>): void {
    if (this.bytecodeObfuscateConfig.obfuscationRules.keepOptions &&
      this.bytecodeObfuscateConfig.obfuscationRules.keepOptions.keepPaths.size > 0) {
      const convertedPath: Set<string> = new Set<string>();
      this.bytecodeObfuscateConfig.obfuscationRules.keepOptions.keepPaths.forEach(path => {
        if (moduleInfos.get(path)) {
          convertedPath.add(moduleInfos.get(path).recordName);
          this.bytecodeObfuscateConfig.removeKeepPaths(path);
        }
      });
      this.bytecodeObfuscateConfig.addKeepPaths(convertedPath);
    }
  }

  public generateObfCmd(): void {
    if (this.debugging) {
      this.cmdArgs.push('--debug');
      this.cmdArgs.push('--debug-file');
      const defaultDebugLogPath: string = path.join(this.obfuscationCacheDir, 'debug.log');
      if (!fs.existsSync(defaultDebugLogPath)) {
        writeFileSync(defaultDebugLogPath, '');
      }
      this.cmdArgs.push(defaultDebugLogPath);
    }
    this.cmdArgs.push(this.configPath);
  }

  private createWarnings(): void {
    this.logger.warn(
      yellow,
      'ArkTS:WARN When enabling bytecode obfuscation, the following exceptions may occur:\n' +
      '1. When enabling fileName, export, and property obfuscation, and a HAR is depended on by both HAP and HSP,\n' +
      'this scenario may result in the error:' +
      '   \"The requested module \'xxxx\' does not provide an export name \'x\' which is imported by \'xxxxx\'.\"'
    );

    if (this.enhanced) {
      this.logger.warn(
        yellow,
        'ArkTS:WARN When enabling the enhanced option, the following exceptions may occur:\n' +
        '1. When enabling export and property obfuscation, and a HAR is depended on by both HAP and HSP,\n' +
        '   this scenario may result in the error:' +
        '   \"The requested module \'xxxx\' does not provide an export name \'x\' which is imported by \'xxxxx\'.\"'
      );
    }
  }

  private updateAfterObfuscation(): void {
    let nameCache: Object;
    let sourceMap: Object;

    this.nameCachePath = this.bytecodeObfuscateConfig.obfuscationRules.printNameCache ||
      this.bytecodeObfuscateConfig.defaultNameCachePath;

    const sourceMapPath = path.join(this.projectConfig.cachePath, SOURCEMAPS);
    const souceMapJsonPath = path.join(this.projectConfig.cachePath, SOURCEMAPS_JSON);
    nameCache = JSON.parse(fs.readFileSync(this.nameCachePath, 'utf-8'));
    sourceMap = JSON.parse(fs.readFileSync(sourceMapPath, 'utf-8'));
    //copy origin sourceMaps for Incremental compilation
    if (fs.existsSync(souceMapJsonPath)) {
      fs.copyFileSync(souceMapJsonPath, this.backupSourceMapPath);
    }
    const nameMapping: Map<string, string> = generateMapping(nameCache);
    const sourceMapUpdated: Object = updateSourceMap(sourceMap, nameMapping);
    fs.writeFileSync(sourceMapPath, JSON.stringify(sourceMapUpdated, null, 2));
    fs.writeFileSync(souceMapJsonPath, SourceMapGenerator.getInstance().convertSourceMapToCache(sourceMapUpdated));
    const nameCacheUpdated: Object = this.bytecodeObfuscateConfig.obfuscationRules.compact ?
      nameCache : processNameCache(nameCache, sourceMap);
    fs.writeFileSync(this.nameCachePath, JSON.stringify(nameCacheUpdated, null, 2));
  }

  private collectWhiteList(): void {
    this.bytecodeObfuscateConfig.addReservedNames(ES_ANNOTATIONS);
    this.bytecodeObfuscateConfig.addReservedProperties(BYTECODE_OBFUSCATION_PROPERTY_WHITE_LIST);
  }

  public getBackupSourceMapPath(): string {
    return this ? this.backupSourceMapPath : '';
  }

  public setKeepSourceOfPaths(keepPath: Set<string>): void {
    this.bytecodeObfuscateConfig.addKeepPaths(keepPath);
  }

  public isDecoratorObfuscationEnabled(): boolean {
    return this.bytecodeObfuscateConfig?.obfuscationRules?.enableDecoratorObfuscation;
  }

  private getSystemApiConfigsByCache(): void {
    const uniqueNames = new Set([
      ...(UnobfuscationCollections.reservedSdkApiForLocal || []),
      ...(UnobfuscationCollections.reservedSdkApiForGlobal || []),
      ...(UnobfuscationCollections.reservedSdkApiForProp || [])
    ]);
    this.bytecodeObfuscateConfig.addReservedNames(uniqueNames);
  }

  collectPropertyBySourceScan(fileWhiteListsCachePath: string): void {
    const fileContent = fs.readFileSync(fileWhiteListsCachePath, 'utf8');
    const parsed: Object = JSON.parse(fileContent);
    const allValues: string[] = [];

    Object.values(parsed).forEach(fileData => {
      this.processFileKeepInfo(fileData.fileKeepInfo, allValues);
      this.processFileReservedInfo(fileData.fileReservedInfo, allValues);
      // only collect decorator when decorator Obfuscation is disabled
      if (!this.isDecoratorObfuscationEnabled()) {
        this.processDecoratorMap(fileData.bytecodeObfuscateKeepInfo?.decoratorMap);
      }
    });

    this.bytecodeObfuscateConfig.addReservedNames(allValues);
  }

  private processFileKeepInfo(fileKeepInfo: FileKeepInfo, allValues: string[]): void {
    if (!fileKeepInfo) {
      return;
    }

    let {
      keepSymbol,
      keepAsConsumer,
      structProperties,
      enumProperties,
      stringProperties,
      exported,
    } = fileKeepInfo;

    const rules = this.bytecodeObfuscateConfig.obfuscationRules;
    if (rules.enableDecoratorObfuscation) {
      const reservedNames = this.bytecodeObfuscateConfig.getReservedNames();
      structProperties = new Set(
        Array.from(structProperties || []).filter(item => reservedNames?.has(item))
      );
    }

    const arrayProperties = [
      keepSymbol?.propertyNames,
      keepSymbol?.globalNames,
      keepAsConsumer?.propertyNames,
      keepAsConsumer?.globalNames,
      exported?.propertyNames,
      exported?.globalNames,
      structProperties,
      enumProperties,
      stringProperties
    ];

    arrayProperties.forEach(arr => {
      if (arr) {
        allValues.push(...arr);
      }
    });
  }

  private processFileReservedInfo(fileReservedInfo: FileReservedInfo, allValues: string[]): void {
    if (!fileReservedInfo) {
      return;
    }

    const { enumProperties, propertyParams } = fileReservedInfo;

    [enumProperties, propertyParams].forEach(props => {
      if (props) {
        allValues.push(...props);
      }
    });
  }

  private processDecoratorMap(decoratorMap: Object): void {
    if (!decoratorMap) {
      return;
    }
    for (const [decoratorName, propertyList] of Object.entries(decoratorMap)) {
      const processedList = new Set(
        [...propertyList].flatMap(name =>
          name.includes('.') ? name.split('.') : [name]
        )
      );

      this.bytecodeObfuscateConfig.addReservedNames(processedList);
      processedList.forEach(name => {
        UnobfuscationCollections.reservedSdkApiForGlobal.add(name);
      });
    }
  }
}

export class BytecodeObfuscationConfig {
  abcFilePath: string;
  obfAbcFilePath: string;
  obfPaFilePath: string;
  compileSdkVersion: string;
  targetApiVersion: number;
  targetApiSubVersion: string;
  filesInfoPath: string;
  entryPackageInfo: string;
  sourceMapsPath: string;
  defaultNameCachePath: string;
  skippedRemoteHarList: Set<string>;
  useNormalizedOHMUrl: boolean;
  obfuscationRules: {
    disableObfuscation: boolean;
    compact: boolean;
    enableExportObfuscation: boolean;
    enableRemoveLog: boolean;
    enableDecoratorObfuscation: boolean;
    printNameCache: string;
    applyNameCache: string;
    reservedNames: Set<string>;
    propertyObfuscation: {
      enable: boolean;
      reservedProperties: Set<string>;
      universalReservedProperties: Set<string>;
    };
    toplevelObfuscation: {
      enable: boolean;
      reservedToplevelNames: Set<string>;
      universalReservedToplevelNames: Set<string>;
    };
    fileNameObfuscation: {
      enable: boolean;
      reservedFileNames: Set<string>;
      universalReservedFileNames: Set<string>;
      reservedRemoteHarPkgNames: Set<string>;
    };
    keepOptions: {
      enable: boolean;
      keepPaths: Set<string>;
    };
  };

  constructor(share: Object, arkProjectConfig: Object) {
    const mergedObConfig: MergedConfig = arkProjectConfig.obfuscationMergedObConfig;
    const obfuscationCacheDir: string = share.projectConfig.obfuscationOptions.obfuscationCacheDir;
    const originDir: string = path.join(obfuscationCacheDir, 'origin');
    const obfDir: string = path.join(obfuscationCacheDir, 'obf');
    const outPutABC: string = share.projectConfig.widgetCompile ? WIDGETS_ABC : MODULES_ABC;
    this.abcFilePath = path.join(originDir, outPutABC);
    this.obfAbcFilePath = path.join(obfDir, outPutABC);
    this.targetApiVersion = share.projectConfig.compatibleSdkVersion;
    this.targetApiSubVersion = share.projectConfig.compatibleSdkVersionStage;
    this.filesInfoPath = path.join(share.projectConfig.cachePath, FILESINFO_TXT);
    this.sourceMapsPath = path.join(share.projectConfig.cachePath, SOURCEMAPS);
    this.useNormalizedOHMUrl = share.projectConfig.useNormalizedOHMUrl;
    this.compileSdkVersion = share.projectConfig.etsLoaderVersion;
    this.entryPackageInfo = arkProjectConfig.entryPackageInfo;
    this.defaultNameCachePath = path.join(obfuscationCacheDir, 'nameCache.json');
    this.skippedRemoteHarList = new Set<string>();
    if (mergedObConfig.options.bytecodeObf.debugging) {
      const parsedPath = path.parse(this.obfAbcFilePath);
      this.obfPaFilePath = path.join(parsedPath.dir, `${parsedPath.name}.pa`);
    } else {
      this.obfPaFilePath = '';
    }
    const { options, reservedNames, reservedPropertyNames, universalReservedPropertyNames,
      reservedGlobalNames, universalReservedGlobalNames, reservedFileNames,
      keepSourceOfPaths } = mergedObConfig;
    const fileNameReservedInfo: ReservedNameInfo = processReservedFileNames(reservedFileNames);

    this.obfuscationRules = {
      disableObfuscation: false,
      compact: options.compact,
      enableExportObfuscation: options.enableExportObfuscation,
      enableRemoveLog: options.removeLog,
      enableDecoratorObfuscation: false,
      printNameCache: options.printNameCache,
      applyNameCache: options.applyNameCache,
      reservedNames: new Set<string>(reservedNames),
      propertyObfuscation: {
        enable: options.enablePropertyObfuscation,
        reservedProperties: new Set<string>(reservedPropertyNames),
        universalReservedProperties: new Set(
          universalReservedPropertyNames?.map(regexp => regexp.toString())
        )
      },
      toplevelObfuscation: {
        enable: options.enableToplevelObfuscation,
        reservedToplevelNames: new Set<string>(reservedGlobalNames),
        universalReservedToplevelNames: new Set(
          universalReservedGlobalNames?.map(regexp => regexp.toString()) || []
        )
      },
      fileNameObfuscation: {
        enable: options.enableFileNameObfuscation,
        reservedFileNames: new Set<string>(fileNameReservedInfo.specificReservedArray),
        universalReservedFileNames: new Set(
          fileNameReservedInfo.universalReservedArray?.map(regexp => regexp.toString()) || []
        )
        ,
        reservedRemoteHarPkgNames: new Set<string>()
      },
      keepOptions: {
        enable: keepSourceOfPaths.length > 0,
        keepPaths: new Set<string>(keepSourceOfPaths),
      }
    };
  }

  getReservedNames(): Set<string> {
    return this.obfuscationRules.reservedNames;
  }

  addToSet(set: Set<string>, values: string | string[] | Set<string>): void {
    if (typeof values === 'string') {
      set.add(values);
    } else {
      values.forEach(value => set.add(value));
    }
  }

  addReservedProperties(values: string | string[] | Set<string>): void {
    this.addToSet(this.obfuscationRules.propertyObfuscation.reservedProperties, values);
  }

  addReservedNames(values: string | string[] | Set<string>): void {
    this.addToSet(this.obfuscationRules.reservedNames, values);
  }

  addSkippedRemoteHarList(values: string | string[] | Set<string>): void {
    this.addToSet(this.skippedRemoteHarList, values);
  }

  addKeepPaths(values: string | string[] | Set<string>): void {
    this.addToSet(this.obfuscationRules.keepOptions.keepPaths, values);
  }

  removeKeepPaths(values: string | string[] | Set<string>): void {
    this.removeFromSet(this.obfuscationRules.keepOptions.keepPaths, values);
  }

  removeFromSet(
    set: Set<string>,
    values: string | string[] | Set<string>
  ): void {
    if (typeof values === 'string') {
      set.delete(values);
    } else {
      values.forEach((value) => set.delete(value));
    }
  }
}

function processReservedFileNames(reservedFileNames: string[]): ReservedNameInfo {
  const fileNameReservedInfo: ReservedNameInfo = separateUniversalReservedItem(reservedFileNames, false);
  fileNameReservedInfo.specificReservedArray = fileNameReservedInfo.specificReservedArray.map(fileName => {
    return fileName.replace(/^(?!\.)[^.]+\.[^.]+$/, match => {
      return match.replace(/\.[^.]+$/, '');
    });
  });
  return fileNameReservedInfo;
}

function collectReservedLangForProperty(): Set<string> {
  let languageSet: Set<string> = new Set<string>;
  for (const key of Object.keys(esInfo)) {
    const valueArray = esInfo[key];
    valueArray.forEach((element: string) => {
      languageSet.add(element);
    });
  }
  return languageSet;
}

function isVersionCompatible(
  compatibleSdkVersion: number | null | undefined,
  compatibleSdkVersionStage: string | null | undefined
): boolean {
  if (compatibleSdkVersion == null) {
    return false;
  }

  if (compatibleSdkVersion >= 13) {
    return true;
  }

  if (compatibleSdkVersion === 12) {
    return compatibleSdkVersionStage === 'beta3';
  }

  return false;
}

function generateMapping(nameCache: Object): Map<string, string> {
  const whiteList = new Set<string>([
    FILE_NAME_CACHE,
    OBF_NAME_MAP,
    PROPERTY_CACHE,
    ENTRY_PACKAGE_INFO,
    COMPILE_SDK_VERSION
  ]);
  const obfSourceFileMapping = new Map<string, string>();

  Object.entries(nameCache).forEach(([key, value]) => {
    if (!whiteList.has(key)) {
      const { ObfSourceFile: obfSourceFile, OriSourceFile: originSouceFile } = value;
      if (obfSourceFile && originSouceFile) {
        obfSourceFileMapping.set(originSouceFile, obfSourceFile);
      }
    }
  });
  return obfSourceFileMapping;
}

function updateSourceMap(sourceMap: Object, obfSourceFileMapping: Map<string, string>): Object {
  const newSourceMap: Object = {};
  Object.entries(sourceMap).forEach(([key, value]) => {
    const obfSourceKey = obfSourceFileMapping.get(key) || key;
    newSourceMap[obfSourceKey] = value;
  });
  return newSourceMap;
}

function processNameCache(nameCache: Object, sourceMap: Object): Object {
  const nameCacheUpdated = {};
  const entries = Object.entries(nameCache);
  for (let i = 0; i < entries.length; i++) {
    const [nameCacheKey, nameCacheValue] = entries[i];
    processNameCacheEntry(nameCacheKey, nameCacheValue, sourceMap, nameCacheUpdated);
  }
  return nameCacheUpdated;
}

function processNameCacheEntry(nameCacheKey: string, nameCacheValue: Object, sourceMap: Object, nameCacheUpdated: Object): void {
  if (!nameCacheValue.OriSourceFile) {
    nameCacheUpdated[nameCacheKey] = nameCacheValue;
    return;
  }
  const sourceMapKey = nameCacheValue.OriSourceFile;
  const sourcemap = sourceMap[sourceMapKey];
  if (!sourcemap) {
    return;
  }
  const sourceFileName = sourcemap.sources?.length === 1 ? sourcemap.sources[0] : '';
  const source = new Source(sourceFileName, null);
  const decodedSourceMap = decodeSourcemap(sourcemap);
  const sourceMapLink = new SourceMapLink(decodedSourceMap, [source]);

  if (!nameCacheUpdated[nameCacheKey]) {
    nameCacheUpdated[nameCacheKey] = {};
  }
  for (const [itemKey, itemValue] of Object.entries(nameCacheValue)) {
    if (itemKey !== IDENTIFIER_CACHE && itemKey !== MEMBER_METHOD_CACHE) {
      nameCacheUpdated[nameCacheKey][itemKey] = itemValue;
      continue;
    }
    processItemKey(nameCacheKey, itemKey, itemValue, nameCacheUpdated, sourceMapLink);
  }
}
/**

 * @param itemKey IdentifierCache || MemberMethodCache
 * @param itemValue 
 */
function processItemKey(nameCacheKey, itemKey, itemValue, nameCacheUpdated, sourceMapLink): void {
  if (!nameCacheUpdated[nameCacheKey][itemKey]) {
    nameCacheUpdated[nameCacheKey][itemKey] = {};
  }
  /**
   * key=>originName eg.#fun1:3:24
   * value => obfuscated Name
   */
  for (const [key, value] of Object.entries(itemValue)) {
    const [scopeName, oldStartLine, oldEndLine] = key.split(':');
    const startPosition: SourceMapSegmentObj = sourceMapLink.traceSegment(Number(oldStartLine) - 1, 0, '');
    const endPosition: SourceMapSegmentObj = sourceMapLink.traceSegment(
      Number(oldEndLine) - 1, 0, '');
    if (!startPosition || !endPosition) {
      nameCacheUpdated[nameCacheKey][itemKey][key] = value;
      continue;
    }
    const startLine = startPosition.line + 1;
    const endLine = endPosition.line + 1;
    const newKey = `${scopeName}:${startLine}:${endLine}`;
    nameCacheUpdated[nameCacheKey][itemKey][newKey] = value;
  }
}