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

import fs from 'fs';
import path from 'path';
import JSON5 from 'json5';

import type * as ts from 'typescript';

import { FileUtils } from '../utils/FileUtils';
import {
  type ReservedNameInfo,
  ApiExtractor,
  containWildcards,
  EventList,
  getMapFromJson,
  performancePrinter,
  PropCollections,
  renameFileNameModule,
  separateUniversalReservedItem,
  wildcardTransformer,
  ArkObfuscator,
} from '../ArkObfuscator';

import { isDebug, isFileExist, sortAndDeduplicateStringArr, mergeSet, convertSetToArray } from './utils';
import { nameCacheMap, yellow, unobfuscationNamesObj } from './CommonObject';
import { clearHistoryUnobfuscatedMap, historyAllUnobfuscatedNamesMap, historyUnobfuscatedPropMap } from './Initializer';
import { AtIntentCollections, AtKeepCollections, LocalVariableCollections, UnobfuscationCollections } from '../utils/CommonCollections';
import { INameObfuscationOption } from '../configs/INameObfuscationOption';
import { WhitelistType } from '../utils/TransformUtil';
import { endFilesEvent, startFilesEvent } from '../utils/PrinterUtils';
import { initScanProjectConfigByMergeConfig, scanProjectConfig, resetScanProjectConfig } from '../common/ApiReader';
import { MemoryDottingDefine } from '../utils/MemoryDottingDefine';
import type { HvigorErrorInfo } from '../common/type';
import { addToSet, KeepInfo } from '../utils/ProjectCollections';

enum OptionType {
  NONE,
  KEEP,
  KEEP_DTS,
  KEEP_GLOBAL_NAME,
  KEEP_PROPERTY_NAME,
  KEEP_FILE_NAME,
  KEEP_COMMENTS,
  DISABLE_OBFUSCATION,
  ENABLE_PROPERTY_OBFUSCATION,
  ENABLE_STRING_PROPERTY_OBFUSCATION,
  ENABLE_TOPLEVEL_OBFUSCATION,
  ENABLE_FILENAME_OBFUSCATION,
  ENABLE_EXPORT_OBFUSCATION,
  ENABLE_LIB_OBFUSCATION_OPTIONS,
  ENABLE_ATKEEP,
  COMPACT,
  REMOVE_LOG,
  REMOVE_COMMENTS,
  PRINT_NAMECACHE,
  PRINT_KEPT_NAMES,
  APPLY_NAMECACHE,
  EXTRA_OPTIONS,
  STRIP_LANGUAGE_DEFAULT,
  STRIP_SYSTEM_API_ARGS,
  KEEP_PARAMETER_NAMES,
  ENABLE_BYTECODE_OBFUSCATION,
  ENABLE_BYTECODE_OBFUSCATION_DEBUGGING,
  ENABLE_BYTECODE_OBFUSCATION_ENHANCED,
  ENABLE_BYTECODE_OBFUSCATION_ARKUI
}
export { OptionType as OptionTypeForTest };

type SystemApiContent = {
  ReservedPropertyNames?: string[];
  ReservedGlobalNames?: string[];
  ReservedLocalNames?: string[];
};

interface BytecodeObf {
  enable: boolean;
  enhanced: boolean;
  debugging: boolean;
  obfArkUI: boolean;
}

class ObOptions {
  bytecodeObf: BytecodeObf = {
    enable: false,
    enhanced: false,
    debugging: false,
    obfArkUI: false
  };
  disableObfuscation: boolean = false;
  enablePropertyObfuscation: boolean = false;
  enableStringPropertyObfuscation: boolean = false;
  enableToplevelObfuscation: boolean = false;
  enableFileNameObfuscation: boolean = false;
  enableExportObfuscation: boolean = false;
  enableLibObfuscationOptions: boolean = false;
  enableAtKeep: boolean = false;
  enableEtsAnnotation: boolean = false;
  printKeptNames: boolean = false;
  removeComments: boolean = false;
  compact: boolean = false;
  removeLog: boolean = false;
  printNameCache: string = '';
  printKeptNamesPath: string = '';
  applyNameCache: string = '';
  stripLanguageDefault: boolean = false;
  stripSystemApiArgs: boolean = false;
  keepParameterNames: boolean = false;

  mergeObOptions(other: ObOptions): void {
    this.disableObfuscation = this.disableObfuscation || other.disableObfuscation;
    this.enablePropertyObfuscation = this.enablePropertyObfuscation || other.enablePropertyObfuscation;
    this.enableToplevelObfuscation = this.enableToplevelObfuscation || other.enableToplevelObfuscation;
    this.enableStringPropertyObfuscation =
      this.enableStringPropertyObfuscation || other.enableStringPropertyObfuscation;
    this.bytecodeObf.obfArkUI = this.bytecodeObf?.obfArkUI || other.bytecodeObf?.obfArkUI;
    this.removeComments = this.removeComments || other.removeComments;
    this.compact = this.compact || other.compact;
    this.removeLog = this.removeLog || other.removeLog;
    this.enableFileNameObfuscation = this.enableFileNameObfuscation || other.enableFileNameObfuscation;
    this.enableExportObfuscation = this.enableExportObfuscation || other.enableExportObfuscation;
    this.stripLanguageDefault = this.stripLanguageDefault || other.stripLanguageDefault;
    this.stripSystemApiArgs = this.stripSystemApiArgs || other.stripSystemApiArgs;
    this.keepParameterNames = this.keepParameterNames || other.keepParameterNames;
    this.enableAtKeep = this.enableAtKeep || other.enableAtKeep;

    if (other.printNameCache.length > 0) {
      this.printNameCache = other.printNameCache;
    }
    if (other.printKeptNamesPath.length > 0) {
      this.printKeptNamesPath = other.printKeptNamesPath;
    }
    if (other.applyNameCache.length > 0) {
      this.applyNameCache = other.applyNameCache;
    }
  }
}
export const ObOptionsForTest = ObOptions;

export class MergedConfig {
  options: ObOptions = new ObOptions();
  reservedPropertyNames: string[] = [];
  reservedGlobalNames: string[] = [];
  reservedNames: string[] = [];
  reservedFileNames: string[] = [];
  keepComments: string[] = [];
  keepSourceOfPaths: string[] = []; // The file path or folder path configured by the developer.
  universalReservedPropertyNames: RegExp[] = []; // Support reserved property names contain wildcards.
  universalReservedGlobalNames: RegExp[] = []; // Support reserved global names contain wildcards.
  keepUniversalPaths: RegExp[] = []; // Support reserved paths contain wildcards.
  excludeUniversalPaths: RegExp[] = []; // Support excluded paths contain wildcards.
  excludePathSet: Set<string> = new Set();

  mergeKeepOptions(other: MergedConfig): void {
    this.reservedPropertyNames.push(...other.reservedPropertyNames);
    this.reservedGlobalNames.push(...other.reservedGlobalNames);
    this.reservedFileNames.push(...other.reservedFileNames);
    this.keepComments.push(...other.keepComments);
    this.keepSourceOfPaths.push(...other.keepSourceOfPaths);
    this.keepUniversalPaths.push(...other.keepUniversalPaths);
    this.excludeUniversalPaths.push(...other.excludeUniversalPaths);
    other.excludePathSet.forEach((excludePath) => {
      this.excludePathSet.add(excludePath);
    });
  }

  mergeAllRules(other: MergedConfig): void {
    this.options.mergeObOptions(other.options);
    this.mergeKeepOptions(other);
  }

  sortAndDeduplicate(): void {
    this.reservedPropertyNames = sortAndDeduplicateStringArr(this.reservedPropertyNames);
    this.reservedGlobalNames = sortAndDeduplicateStringArr(this.reservedGlobalNames);
    this.reservedFileNames = sortAndDeduplicateStringArr(this.reservedFileNames);
    this.keepComments = sortAndDeduplicateStringArr(this.keepComments);
    this.keepSourceOfPaths = sortAndDeduplicateStringArr(this.keepSourceOfPaths);
  }

  serializeMergedConfig(): string {
    let resultStr: string = '';
    const keys = Object.keys(this.options);
    for (const key of keys) {
      // skip the export of some switches.
      if (this.options[key] === true && ObConfigResolver.exportedSwitchMap.has(String(key))) {
        resultStr += ObConfigResolver.exportedSwitchMap.get(String(key)) + '\n';
      }
    }

    if (this.options.bytecodeObf?.obfArkUI) {
      resultStr += ObConfigResolver.ENABLE_BYTECODE_OBFUSCATION_ARKUI + '\n';
    }
    if (this.reservedGlobalNames.length > 0) {
      resultStr += ObConfigResolver.KEEP_GLOBAL_NAME + '\n';
      this.reservedGlobalNames.forEach((item) => {
        resultStr += item + '\n';
      });
    }
    if (this.reservedPropertyNames.length > 0) {
      resultStr += ObConfigResolver.KEEP_PROPERTY_NAME + '\n';
      this.reservedPropertyNames.forEach((item) => {
        resultStr += item + '\n';
      });
    }
    return resultStr;
  }
}

export class ObConfigResolver {
  sourceObConfig: any;
  printObfLogger: Function;
  isHarCompiled: boolean | undefined;
  isHspCompiled: boolean | undefined;
  isTerser: boolean;
  needConsumerConfigs: boolean = false;
  dependencyConfigs: MergedConfig;

  constructor(projectConfig: any, printObfLogger: Function, isTerser?: boolean) {
    this.sourceObConfig = projectConfig.obfuscationOptions;
    this.printObfLogger = printObfLogger;
    this.isHarCompiled = projectConfig.compileHar;
    this.isHspCompiled = projectConfig.compileShared;
    this.isTerser = isTerser;
  }

  public resolveObfuscationConfigs(): MergedConfig {
    let sourceObConfig = this.sourceObConfig;
    if (!sourceObConfig) {
      return new MergedConfig();
    }
    startFilesEvent(EventList.RESOLVE_OBFUSCATION_CONFIGS);
    let enableObfuscation: boolean = sourceObConfig.selfConfig.ruleOptions.enable;

    let selfConfig: MergedConfig = new MergedConfig();
    if (enableObfuscation) {
      this.getSelfConfigs(selfConfig);
      enableObfuscation = !selfConfig.options.disableObfuscation;
    } else {
      selfConfig.options.disableObfuscation = true;
    }

    this.needConsumerConfigs = (this.isHarCompiled || this.isHspCompiled) &&
      sourceObConfig.selfConfig.consumerRules &&
      sourceObConfig.selfConfig.consumerRules.length > 0;
    let needDependencyConfigs: boolean = enableObfuscation || this.needConsumerConfigs;

    this.dependencyConfigs = new MergedConfig();
    const dependencyMaxLength: number = Math.max(
      sourceObConfig.dependencies.libraries.length,
      sourceObConfig.dependencies.hars.length,
      sourceObConfig.dependencies.hsps?.length ?? 0,
      sourceObConfig.dependencies.hspLibraries?.length ?? 0
    );
    if (needDependencyConfigs && dependencyMaxLength > 0) {
      this.dependencyConfigs = new MergedConfig();
      this.getDependencyConfigs(sourceObConfig, this.dependencyConfigs);
      enableObfuscation = enableObfuscation && !this.dependencyConfigs.options.disableObfuscation;
    }
    const mergedConfigs: MergedConfig = this.getMergedConfigs(selfConfig, this.dependencyConfigs);
    this.handleReservedArray(mergedConfigs);
    endFilesEvent(EventList.RESOLVE_OBFUSCATION_CONFIGS);
   
    /**
     * Bytecode obfuscate mode:
     * temporary variable or in non-top-level scope is obfuscated to be added to the obfuscate name set,
     * All names that appear in the confused name collection will be obfuscated later
     * So when a developer-defined name has the same name as systemApi
     * Without a whitelist, all the same names would be obfuscated, leading to devastating errors
     * so in order to work properly, Bytecode obfuscate enables the obfuscate function and requires whitelist
     */
    let needKeepSystemApi =
      enableObfuscation &&
      (mergedConfigs.options.enablePropertyObfuscation ||
        (mergedConfigs.options.enableExportObfuscation && mergedConfigs.options.enableToplevelObfuscation) ||
        mergedConfigs.options.bytecodeObf.enable); 

    if (needKeepSystemApi && sourceObConfig.obfuscationCacheDir) {
      const systemApiCachePath: string = path.join(sourceObConfig.obfuscationCacheDir, 'systemApiCache.json');
      if (isFileExist(systemApiCachePath)) {
        startFilesEvent(EventList.GET_SYSTEM_API_CONFIGS_BY_CACHE);
        this.getSystemApiConfigsByCache(systemApiCachePath);
        endFilesEvent(EventList.GET_SYSTEM_API_CONFIGS_BY_CACHE);
      } else {
        const recordInfo = ArkObfuscator.recordStage(MemoryDottingDefine.SCAN_SYS_API);
        startFilesEvent(EventList.SCAN_SYSTEMAPI, performancePrinter.timeSumPrinter);
        this.collectSystemApiWhitelist(mergedConfigs, systemApiCachePath);
        endFilesEvent(EventList.SCAN_SYSTEMAPI, performancePrinter.timeSumPrinter);
        ArkObfuscator.stopRecordStage(recordInfo);
      }
    }

    // when atKeep is enabled, we can not emit here since we need to collect names marked with atKeep
    if (!mergedConfigs.options.enableAtKeep) {
      this.emitConsumerConfigFiles();
    }
    return mergedConfigs;
  }

  public emitConsumerConfigFiles(): void {
    if (this.needConsumerConfigs) {
      let selfConsumerConfig = new MergedConfig();
      startFilesEvent(EventList.GEN_CONSUMER_CONFIG);
      this.getSelfConsumerConfig(selfConsumerConfig);
      this.genConsumerConfigFiles(this.sourceObConfig, selfConsumerConfig, this.dependencyConfigs);
      endFilesEvent(EventList.GEN_CONSUMER_CONFIG);
    }
  }

  private getSelfConfigs(selfConfigs: MergedConfig): void {
    if (this.sourceObConfig.selfConfig.ruleOptions.rules) {
      const configPaths: string[] = this.sourceObConfig.selfConfig.ruleOptions.rules;
      for (const path of configPaths) {
        this.getConfigByPath(path, selfConfigs);
      }
    }
  }

  public getSelfConfigsForTest(selfConfigs: MergedConfig): void {
    return this.getSelfConfigs(selfConfigs);
  }

  private getConfigByPath(path: string, configs: MergedConfig): void {
    let fileContent = undefined;
    try {
      fileContent = fs.readFileSync(path, 'utf-8');
    } catch (err) {
      const errorInfo = `Failed to open ${path}. Error message: ${err}`;
      const errorCodeInfo: HvigorErrorInfo = {
        code: '10804001',
        description: 'ArkTS compiler Error',
        cause: `Failed to open obfuscation config file from ${path}. Error message: ${err}`,
        position: path,
        solutions: [`Please check whether ${path} exists.`],
      };
      this.printObfLogger(errorInfo, errorCodeInfo, 'error');
    }
    this.handleConfigContent(fileContent, configs, path);
  }

  public getConfigByPathForTest(path: string, configs: MergedConfig): void {
    return this.getConfigByPath(path, configs);
  }

  private handleReservedArray(mergedConfigs: MergedConfig): void {
    const shouldPrintKeptNames = mergedConfigs.options.printKeptNames;
    if (mergedConfigs.options.enablePropertyObfuscation && mergedConfigs.reservedPropertyNames) {
      const propertyReservedInfo: ReservedNameInfo =
        separateUniversalReservedItem(mergedConfigs.reservedPropertyNames, shouldPrintKeptNames);
      mergedConfigs.universalReservedPropertyNames = propertyReservedInfo.universalReservedArray;
      mergedConfigs.reservedPropertyNames = propertyReservedInfo.specificReservedArray;
    }

    if (mergedConfigs.options.enableToplevelObfuscation && mergedConfigs.reservedGlobalNames) {
      const globalReservedInfo: ReservedNameInfo =
        separateUniversalReservedItem(mergedConfigs.reservedGlobalNames, shouldPrintKeptNames);
      mergedConfigs.universalReservedGlobalNames = globalReservedInfo.universalReservedArray;
      mergedConfigs.reservedGlobalNames = globalReservedInfo.specificReservedArray;
    }
  }

  public handleReservedArrayForTest(mergedConfigs: MergedConfig): void {
    return this.handleReservedArray(mergedConfigs);
  }

  // obfuscation options
  static readonly KEEP = '-keep';
  static readonly KEEP_DTS = '-keep-dts';
  static readonly KEEP_GLOBAL_NAME = '-keep-global-name';
  static readonly KEEP_PROPERTY_NAME = '-keep-property-name';
  static readonly KEEP_FILE_NAME = '-keep-file-name';
  static readonly KEEP_COMMENTS = '-keep-comments';
  static readonly DISABLE_OBFUSCATION = '-disable-obfuscation';
  static readonly ENABLE_PROPERTY_OBFUSCATION = '-enable-property-obfuscation';
  static readonly ENABLE_STRING_PROPERTY_OBFUSCATION = '-enable-string-property-obfuscation';
  static readonly ENABLE_TOPLEVEL_OBFUSCATION = '-enable-toplevel-obfuscation';
  static readonly ENABLE_FILENAME_OBFUSCATION = '-enable-filename-obfuscation';
  static readonly ENABLE_EXPORT_OBFUSCATION = '-enable-export-obfuscation';
  static readonly ENABLE_LIB_OBFUSCATION_OPTIONS = '-enable-lib-obfuscation-options';
  static readonly ENABLE_ATKEEP = '-use-keep-in-source';
  static readonly REMOVE_COMMENTS = '-remove-comments';
  static readonly COMPACT = '-compact';
  static readonly REMOVE_LOG = '-remove-log';
  static readonly PRINT_NAMECACHE = '-print-namecache';
  static readonly PRINT_KEPT_NAMES = '-print-kept-names';
  static readonly APPLY_NAMECACHE = '-apply-namecache';
  // obfuscation options for bytecode obfuscation
  static readonly ENABLE_BYTECODE_OBFUSCATION = '-enable-bytecode-obfuscation';
  static readonly ENABLE_BYTECODE_OBFUSCATION_DEBUGGING = '-enable-bytecode-obfuscation-debugging';
  static readonly ENABLE_BYTECODE_OBFUSCATION_ENHANCED = '-enable-bytecode-obfuscation-enhanced';
  static readonly ENABLE_BYTECODE_OBFUSCATION_ARKUI = '-enable-bytecode-obfuscation-arkui';
  static readonly EXTRA_OPTIONS = '-extra-options';
  static readonly STRIP_LANGUAGE_DEFAULT = 'strip-language-default';
  static readonly STRIP_SYSTEM_API_ARGS = 'strip-system-api-args';
  static readonly KEEP_PARAMETER_NAMES = '-keep-parameter-names';

  // renameFileName, printNameCache, applyNameCache, removeComments and keepComments won't be reserved in obfuscation.txt file.
  static exportedSwitchMap: Map<string, string> = new Map([
    ['enablePropertyObfuscation', ObConfigResolver.ENABLE_PROPERTY_OBFUSCATION],
    ['enableStringPropertyObfuscation', ObConfigResolver.ENABLE_STRING_PROPERTY_OBFUSCATION],
    ['enableToplevelObfuscation', ObConfigResolver.ENABLE_TOPLEVEL_OBFUSCATION],
    ['compact', ObConfigResolver.COMPACT],
    ['removeLog', ObConfigResolver.REMOVE_LOG],
  ]);

  private getTokenType(token: string): OptionType {
    switch (token) {
      case ObConfigResolver.ENABLE_BYTECODE_OBFUSCATION:
        return OptionType.ENABLE_BYTECODE_OBFUSCATION;
      case ObConfigResolver.ENABLE_BYTECODE_OBFUSCATION_DEBUGGING:
        return OptionType.ENABLE_BYTECODE_OBFUSCATION_DEBUGGING;
      case ObConfigResolver.ENABLE_BYTECODE_OBFUSCATION_ENHANCED:
        return OptionType.ENABLE_BYTECODE_OBFUSCATION_ENHANCED;
      case ObConfigResolver.ENABLE_BYTECODE_OBFUSCATION_ARKUI:
        return OptionType.ENABLE_BYTECODE_OBFUSCATION_ARKUI;
      case ObConfigResolver.KEEP_DTS:
        return OptionType.KEEP_DTS;
      case ObConfigResolver.KEEP_GLOBAL_NAME:
        return OptionType.KEEP_GLOBAL_NAME;
      case ObConfigResolver.KEEP_PROPERTY_NAME:
        return OptionType.KEEP_PROPERTY_NAME;
      case ObConfigResolver.KEEP_FILE_NAME:
        return OptionType.KEEP_FILE_NAME;
      case ObConfigResolver.KEEP_COMMENTS:
        return OptionType.KEEP_COMMENTS;
      case ObConfigResolver.DISABLE_OBFUSCATION:
        return OptionType.DISABLE_OBFUSCATION;
      case ObConfigResolver.ENABLE_PROPERTY_OBFUSCATION:
        return OptionType.ENABLE_PROPERTY_OBFUSCATION;
      case ObConfigResolver.ENABLE_STRING_PROPERTY_OBFUSCATION:
        return OptionType.ENABLE_STRING_PROPERTY_OBFUSCATION;
      case ObConfigResolver.ENABLE_TOPLEVEL_OBFUSCATION:
        return OptionType.ENABLE_TOPLEVEL_OBFUSCATION;
      case ObConfigResolver.ENABLE_FILENAME_OBFUSCATION:
        return OptionType.ENABLE_FILENAME_OBFUSCATION;
      case ObConfigResolver.ENABLE_EXPORT_OBFUSCATION:
        return OptionType.ENABLE_EXPORT_OBFUSCATION;
      case ObConfigResolver.ENABLE_LIB_OBFUSCATION_OPTIONS:
        return OptionType.ENABLE_LIB_OBFUSCATION_OPTIONS;
      case ObConfigResolver.ENABLE_ATKEEP:
        return OptionType.ENABLE_ATKEEP;
      case ObConfigResolver.REMOVE_COMMENTS:
        return OptionType.REMOVE_COMMENTS;
      case ObConfigResolver.COMPACT:
        return OptionType.COMPACT;
      case ObConfigResolver.REMOVE_LOG:
        return OptionType.REMOVE_LOG;
      case ObConfigResolver.PRINT_NAMECACHE:
        return OptionType.PRINT_NAMECACHE;
      case ObConfigResolver.PRINT_KEPT_NAMES:
        return OptionType.PRINT_KEPT_NAMES;
      case ObConfigResolver.APPLY_NAMECACHE:
        return OptionType.APPLY_NAMECACHE;
      case ObConfigResolver.KEEP:
        return OptionType.KEEP;
      case ObConfigResolver.EXTRA_OPTIONS:
        return OptionType.EXTRA_OPTIONS;
      case ObConfigResolver.STRIP_LANGUAGE_DEFAULT:
        return OptionType.STRIP_LANGUAGE_DEFAULT;
      case ObConfigResolver.STRIP_SYSTEM_API_ARGS:
        return OptionType.STRIP_SYSTEM_API_ARGS;
      case ObConfigResolver.KEEP_PARAMETER_NAMES:
        return OptionType.KEEP_PARAMETER_NAMES;
      default:
        return OptionType.NONE;
    }
  }

  public getTokenTypeForTest(token: string): OptionType {
    return this.getTokenType(token);
  }

  private handleConfigContent(data: string, configs: MergedConfig, configPath: string): void {
    data = this.removeComments(data);
    const tokens = data.split(/[',', '\t', ' ', '\n', '\r\n']/).filter((item) => item !== '');
    let type: OptionType = OptionType.NONE;
    let extraOptionType: OptionType = OptionType.NONE;
    let tokenType: OptionType;
    let dtsFilePaths: string[] = [];
    let keepConfigs: string[] = [];
    for (let i = 0; i < tokens.length; i++) {
      const token = tokens[i];
      tokenType = this.getTokenType(token);
      // handle switches cases
      switch (tokenType) {
        case OptionType.ENABLE_BYTECODE_OBFUSCATION: {
          configs.options.bytecodeObf.enable = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_BYTECODE_OBFUSCATION_DEBUGGING: {
          configs.options.bytecodeObf.debugging = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_BYTECODE_OBFUSCATION_ENHANCED: {
          configs.options.bytecodeObf.enhanced = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_BYTECODE_OBFUSCATION_ARKUI: {
          configs.options.bytecodeObf.obfArkUI = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.DISABLE_OBFUSCATION: {
          configs.options.disableObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_PROPERTY_OBFUSCATION: {
          configs.options.enablePropertyObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_STRING_PROPERTY_OBFUSCATION: {
          configs.options.enableStringPropertyObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_TOPLEVEL_OBFUSCATION: {
          configs.options.enableToplevelObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.REMOVE_COMMENTS: {
          configs.options.removeComments = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_FILENAME_OBFUSCATION: {
          configs.options.enableFileNameObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_EXPORT_OBFUSCATION: {
          configs.options.enableExportObfuscation = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_LIB_OBFUSCATION_OPTIONS: {
          configs.options.enableLibObfuscationOptions = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.ENABLE_ATKEEP: {
          configs.options.enableAtKeep = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.COMPACT: {
          configs.options.compact = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.REMOVE_LOG: {
          configs.options.removeLog = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.EXTRA_OPTIONS: {
          extraOptionType = tokenType;
          continue;
        }
        case OptionType.PRINT_KEPT_NAMES: {
          configs.options.printKeptNames = true;
          type = tokenType;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.KEEP_PARAMETER_NAMES: {
          configs.options.keepParameterNames = true;
          extraOptionType = OptionType.NONE;
          continue;
        }
        case OptionType.KEEP:
        case OptionType.KEEP_DTS:
        case OptionType.KEEP_GLOBAL_NAME:
        case OptionType.KEEP_PROPERTY_NAME:
        case OptionType.KEEP_FILE_NAME:
        case OptionType.KEEP_COMMENTS:
        case OptionType.PRINT_NAMECACHE:
        case OptionType.APPLY_NAMECACHE:
          type = tokenType;
          extraOptionType = OptionType.NONE;
          continue;
        case OptionType.NONE:
          extraOptionType = OptionType.NONE;
        default: {
          // fall-through
        }
      }
      const matchedExtraOptions: boolean = this.isMatchedExtraOptions(extraOptionType, tokenType, configs);
      if (matchedExtraOptions) {
        continue;
      }
      // handle 'keep' options and 'namecache' options
      switch (type) {
        case OptionType.KEEP: {
          keepConfigs.push(token);
          continue;
        }
        case OptionType.KEEP_DTS: {
          dtsFilePaths.push(token);
          continue;
        }
        case OptionType.KEEP_GLOBAL_NAME: {
          configs.reservedGlobalNames.push(token);
          continue;
        }
        case OptionType.KEEP_PROPERTY_NAME: {
          configs.reservedPropertyNames.push(token);
          continue;
        }
        case OptionType.KEEP_FILE_NAME: {
          configs.reservedFileNames.push(token);
          continue;
        }
        case OptionType.KEEP_COMMENTS: {
          configs.keepComments.push(token);
          continue;
        }
        case OptionType.PRINT_NAMECACHE: {
          configs.options.printNameCache = this.resolvePath(configPath, token);
          type = OptionType.NONE;
          continue;
        }
        case OptionType.PRINT_KEPT_NAMES: {
          configs.options.printKeptNamesPath = this.resolvePath(configPath, token);
          type = OptionType.NONE;
          continue;
        }
        case OptionType.APPLY_NAMECACHE: {
          const absNameCachePath: string = this.resolvePath(configPath, token);
          this.determineNameCachePath(absNameCachePath, configPath);
          configs.options.applyNameCache = absNameCachePath;
          type = OptionType.NONE;
          continue;
        }
        default:
          continue;
      }
    }

    this.resolveDts(dtsFilePaths, configs);
    this.resolveKeepConfig(keepConfigs, configs, configPath);
  }

  public isMatchedExtraOptions(extraOptionType: OptionType, tokenType: OptionType, configs: MergedConfig): boolean {
      if (extraOptionType !== OptionType.EXTRA_OPTIONS) {
          return false;
      }
      switch (tokenType) {
        case OptionType.STRIP_LANGUAGE_DEFAULT: {
          configs.options.stripLanguageDefault = true;
          return true;
        }
        case OptionType.STRIP_SYSTEM_API_ARGS: {
          configs.options.stripSystemApiArgs = true;
          return true;
        }
      }
      extraOptionType = OptionType.NONE;
      return false;
  }
  
  public handleConfigContentForTest(data: string, configs: MergedConfig, configPath: string): void {
    return this.handleConfigContent(data, configs, configPath);
  }
  // get absolute path
  private resolvePath(configPath: string, token: string): string {
    if (path.isAbsolute(token)) {
      return token;
    }
    const configDirectory = path.dirname(configPath);
    return path.resolve(configDirectory, token);
  }

  public resolvePathForTest(configPath: string, token: string): string {
    return this.resolvePath(configPath, token);
  }

  // get names in .d.ts files and add them into reserved list
  private resolveDts(dtsFilePaths: string[], configs: MergedConfig): void {
    ApiExtractor.mPropertySet.clear();
    dtsFilePaths.forEach((token) => {
      ApiExtractor.traverseApiFiles(token, ApiExtractor.ApiType.KEEP_DTS);
    });
    configs.reservedNames = configs.reservedNames.concat([...ApiExtractor.mPropertySet]);
    configs.reservedPropertyNames = configs.reservedPropertyNames.concat([...ApiExtractor.mPropertySet]);
    configs.reservedGlobalNames = configs.reservedGlobalNames.concat([...ApiExtractor.mPropertySet]);
    ApiExtractor.mPropertySet.clear();
  }

  public resolveKeepConfig(keepConfigs: string[], configs: MergedConfig, configPath: string): void {
    for (let keepPath of keepConfigs) {
      let tempAbsPath: string;
      const isExclude: boolean = keepPath.startsWith('!');
      // 1: remove '!'
      tempAbsPath = FileUtils.getAbsPathBaseConfigPath(configPath, isExclude ? keepPath.substring(1) : keepPath);

      // contains '*', '?'
      if (containWildcards(tempAbsPath)) {
        const regexPattern = wildcardTransformer(tempAbsPath, true);
        const regexOperator = new RegExp(`^${regexPattern}$`);
        if (isExclude) {
          // start with '!'
          configs.excludeUniversalPaths.push(regexOperator);
        } else {
          configs.keepUniversalPaths.push(regexOperator);
        }
        continue;
      }

      if (isExclude) {
        // exclude specific path
        configs.excludePathSet.add(tempAbsPath);
        continue;
      }

      if (!fs.existsSync(tempAbsPath)) {
        const warnInfo: string = `ArkTS: The path of obfuscation \'-keep\' configuration does not exist: ${keepPath}`;
        this.printObfLogger(warnInfo, warnInfo, 'warn');
        continue;
      }
      tempAbsPath = fs.realpathSync(tempAbsPath);
      configs.keepSourceOfPaths.push(FileUtils.toUnixPath(tempAbsPath));
    }
  }

  // the content from '#' to '\n' are comments
  private removeComments(data: string): string {
    const commentStart = '#';
    const commentEnd = '\n';
    let tmpStr = '';
    let isInComments = false;
    for (let i = 0; i < data.length; i++) {
      if (isInComments) {
        isInComments = data[i] !== commentEnd;
      } else if (data[i] !== commentStart) {
        tmpStr += data[i];
      } else {
        isInComments = true;
      }
    }
    return tmpStr;
  }

  /**
   * arkguardConfigs includes the API directorys.
   * component directory and pre_define.js file path needs to be concatenated
   * @param config
   */
  private collectSystemApiWhitelist(config: MergedConfig, systemApiCachePath: string): void {
    ApiExtractor.mPropertySet.clear();
    ApiExtractor.mSystemExportSet.clear();
    initScanProjectConfigByMergeConfig(config);
    const sdkApis: string[] = sortAndDeduplicateStringArr(this.sourceObConfig.sdkApis);
    let existPreDefineFilePath: string = '';
    let existArkUIWhitelistPath: string = '';
    const scannedRootFolder: Set<string> = new Set();
    for (let apiPath of sdkApis) {
      this.collectSdkApiWhitelist(apiPath, scannedRootFolder);
      const preDefineFilePath: string = path.join(apiPath, '../build-tools/ets-loader/lib/pre_define.js');
      if (fs.existsSync(preDefineFilePath)) {
        existPreDefineFilePath = preDefineFilePath;
      }
      const arkUIWhitelistPath: string = path.join(apiPath, '../build-tools/ets-loader/obfuscateWhiteList.json5');
      if (fs.existsSync(arkUIWhitelistPath)) {
        existArkUIWhitelistPath = arkUIWhitelistPath;
      }
    }
    const arkUIReservedPropertyNames: string[] = this.collectUIApiWhitelist(existPreDefineFilePath, existArkUIWhitelistPath, config);
    let systemApiContent: SystemApiContent = {};
    if (config.options.enablePropertyObfuscation || config.options.bytecodeObf?.enable) {
      if (!config.options.stripSystemApiArgs) {
        UnobfuscationCollections.reservedSdkApiForLocal = new Set(ApiExtractor.mPropertySet);
        systemApiContent.ReservedLocalNames = Array.from(ApiExtractor.mPropertySet);
      }
      const savedNameAndPropertySet = new Set([...ApiExtractor.mPropertySet, ...arkUIReservedPropertyNames]);
      UnobfuscationCollections.reservedSdkApiForProp = savedNameAndPropertySet;
      systemApiContent.ReservedPropertyNames = Array.from(savedNameAndPropertySet);
    }
    if ((config.options.enableToplevelObfuscation && config.options.enableExportObfuscation) ||
      config.options.bytecodeObf?.enable) {
      const savedExportNamesSet = new Set(ApiExtractor.mSystemExportSet);
      UnobfuscationCollections.reservedSdkApiForGlobal = savedExportNamesSet;
      systemApiContent.ReservedGlobalNames = Array.from(savedExportNamesSet);
    }

    if (!fs.existsSync(path.dirname(systemApiCachePath))) {
      fs.mkdirSync(path.dirname(systemApiCachePath), { recursive: true });
    }
    fs.writeFileSync(systemApiCachePath, JSON.stringify(systemApiContent, null, 2));
    ApiExtractor.mPropertySet.clear();
    ApiExtractor.mSystemExportSet.clear();
    resetScanProjectConfig();
  }

  public collectSystemApiWhitelistForTest(config: MergedConfig, systemApiCachePath: string): void {
    return this.collectSystemApiWhitelist(config, systemApiCachePath);
  }

  private collectSdkApiWhitelist(sdkApiPath: string, scannedRootFolder: Set<string>): void {
    ApiExtractor.traverseApiFiles(sdkApiPath, ApiExtractor.ApiType.API);
    const componentPath: string = path.join(sdkApiPath, '../component');
    if (!scannedRootFolder.has(componentPath) && fs.existsSync(componentPath)) {
      scannedRootFolder.add(componentPath);
      ApiExtractor.traverseApiFiles(componentPath, ApiExtractor.ApiType.COMPONENT);
    }
  }

  private collectPreDefineFile(uiApiPath: string): void {
    ApiExtractor.extractStringsFromFile(uiApiPath);
  }

  private collectUIApiWhitelist(preDefineFilePath: string, arkUIWhitelistPath: string, config: MergedConfig): string[] {
    interface ArkUIWhitelist {
      ReservedPropertyNames: string[],
      OptimizedReservedPropertyNames: string[]
    }
    // OptimizedReservedPropertyNames saves accurate list of UI API that need to be kept
    let arkUIWhitelist: ArkUIWhitelist = { ReservedPropertyNames: [], OptimizedReservedPropertyNames: [] };
    if (fs.existsSync(arkUIWhitelistPath)) {
      arkUIWhitelist = JSON5.parse(fs.readFileSync(arkUIWhitelistPath, 'utf-8'));
    }
    // if enable -extra-options strip-system-api-args, use OptimizedReservedPropertyNames in arkUIWhitelist, and not scan pre_define.js
    let arkUIReservedPropertyNames: string[] = [...arkUIWhitelist.ReservedPropertyNames, ...arkUIWhitelist.OptimizedReservedPropertyNames];
    if (!config.options.stripSystemApiArgs) {
      if (fs.existsSync(preDefineFilePath)) {
        this.collectPreDefineFile(preDefineFilePath);
      }
      arkUIReservedPropertyNames = [...arkUIWhitelist.ReservedPropertyNames];
    }
    return arkUIReservedPropertyNames;
  }

  private getDependencyConfigs(sourceObConfig: SourceObConfig, dependencyConfigs: MergedConfig): void {
    for (const lib of sourceObConfig.dependencies.libraries || []) {
      if (lib.consumerRules && lib.consumerRules.length > 0) {
        this.mergeDependencyConfigsByPath(lib.consumerRules, dependencyConfigs);
      }
    }

    for (const lib of sourceObConfig.dependencies.hspLibraries || []) {
      if (lib.consumerRules && lib.consumerRules.length > 0) {
        this.mergeDependencyConfigsByPath(lib.consumerRules, dependencyConfigs);
      }
    }

    if (
      sourceObConfig.dependencies &&
      sourceObConfig.dependencies.hars &&
      sourceObConfig.dependencies.hars.length > 0
    ) {
      this.mergeDependencyConfigsByPath(sourceObConfig.dependencies.hars, dependencyConfigs);
    }

    if (
      sourceObConfig.dependencies &&
      sourceObConfig.dependencies.hsps &&
      sourceObConfig.dependencies.hsps.length > 0
    ) {
      this.mergeDependencyConfigsByPath(sourceObConfig.dependencies.hsps, dependencyConfigs);
    }
  }

  private mergeDependencyConfigsByPath(paths: string[], dependencyConfigs: MergedConfig): void {
    for (const path of paths) {
      const thisConfig = new MergedConfig();
      this.getConfigByPath(path, thisConfig);
      dependencyConfigs.mergeAllRules(thisConfig);
    }
  }

  public getDependencyConfigsForTest(sourceObConfig: SourceObConfig, dependencyConfigs: MergedConfig): void {
    return this.getDependencyConfigs(sourceObConfig, dependencyConfigs);
  }

  private getSystemApiConfigsByCache(systemApiCachePath: string): void {
    let systemApiContent: {
      ReservedPropertyNames?: string[];
      ReservedGlobalNames?: string[];
      ReservedLocalNames?: string[];
    } = JSON.parse(fs.readFileSync(systemApiCachePath, 'utf-8'));
    if (systemApiContent.ReservedPropertyNames) {
      UnobfuscationCollections.reservedSdkApiForProp = new Set(systemApiContent.ReservedPropertyNames);
    }
    if (systemApiContent.ReservedGlobalNames) {
      UnobfuscationCollections.reservedSdkApiForGlobal = new Set(systemApiContent.ReservedGlobalNames);
    }
    if (systemApiContent.ReservedLocalNames) {
      UnobfuscationCollections.reservedSdkApiForLocal = new Set(systemApiContent.ReservedLocalNames);
    }
  }

  public getSystemApiConfigsByCacheForTest(systemApiCachePath: string): void {
    return this.getSystemApiConfigsByCache(systemApiCachePath);
  }

  private getSelfConsumerConfig(selfConsumerConfig: MergedConfig): void {
    for (const path of this.sourceObConfig.selfConfig.consumerRules) {
      this.getConfigByPath(path, selfConsumerConfig);
    }
  }

  public getSelfConsumerConfigForTest(selfConsumerConfig: MergedConfig): void {
    return this.getSelfConsumerConfig(selfConsumerConfig);
  }

  private getMergedConfigs(selfConfigs: MergedConfig, dependencyConfigs: MergedConfig): MergedConfig {
    if (dependencyConfigs) {
      if (selfConfigs.options.enableLibObfuscationOptions) {
        selfConfigs.mergeAllRules(dependencyConfigs);
      } else {
        selfConfigs.mergeKeepOptions(dependencyConfigs);
      }
    }
    selfConfigs.sortAndDeduplicate();
    return selfConfigs;
  }

  public getMergedConfigsForTest(selfConfigs: MergedConfig, dependencyConfigs: MergedConfig): MergedConfig {
    return this.getMergedConfigs(selfConfigs, dependencyConfigs);
  }

  private genConsumerConfigFiles(
    sourceObConfig: SourceObConfig,
    selfConsumerConfig: MergedConfig,
    dependencyConfigs: MergedConfig,
  ): void {
    if (this.isHarCompiled) {
      selfConsumerConfig.mergeAllRules(dependencyConfigs);
    }
    this.addKeepConsumer(selfConsumerConfig, AtKeepCollections.keepAsConsumer);
    selfConsumerConfig.sortAndDeduplicate();
    this.writeConsumerConfigFile(selfConsumerConfig, sourceObConfig.exportRulePath);
  }

  private addKeepConsumer(selfConsumerConfig: MergedConfig, keepAsConsumer: KeepInfo): void {
    keepAsConsumer.propertyNames.forEach((propertyName) => {
      selfConsumerConfig.reservedPropertyNames.push(propertyName);
    });
    keepAsConsumer.globalNames.forEach((globalName) =>{
      selfConsumerConfig.reservedGlobalNames.push(globalName);
    });
  }

  public genConsumerConfigFilesForTest(
    sourceObConfig: SourceObConfig,
    selfConsumerConfig: MergedConfig,
    dependencyConfigs: MergedConfig,
  ): void {
    return this.genConsumerConfigFiles(sourceObConfig, selfConsumerConfig, dependencyConfigs);
  }

  public writeConsumerConfigFile(selfConsumerConfig: MergedConfig, outpath: string): void {
    const configContent: string = selfConsumerConfig.serializeMergedConfig();
    fs.writeFileSync(outpath, configContent);
  }

  private determineNameCachePath(nameCachePath: string, configPath: string): void {
    if (!fs.existsSync(nameCachePath)) {
      const errorInfo: string = `The applied namecache file '${nameCachePath}' configured by '${configPath}' does not exist.`;
      const errorCodeInfo: HvigorErrorInfo = {
        code: '10804004',
        description: 'ArkTS compiler Error',
        cause: `The applied namecache file '${nameCachePath}' configured by '${configPath}' does not exist.`,
        position: configPath,
        solutions: [`Please check ${configPath} and make sure the file configured by -apply-namecache exists`],
      };
      this.printObfLogger(errorInfo, errorCodeInfo, 'error');
    }
  }
}

/**
 * Collect reserved file name configured in oh-package.json5 and module.json5.
 * @param ohPackagePath The 'main' and 'types' fileds in oh-package.json5 need to be reserved.
 * @param projectConfig Several paths or file contents in projectconfig need to be reserved.
 *   1: module.json's 'srcEntry' field
 *   2: projectPath: /library/src/main/ets
 *   3: cachePath: /library/build/default/cache/default/default@HarCompileArkTs/esmodules/release
 *      target reserved path: /library/build/default/cache/default/default@HarCompileArkTs/esmodules/release/src/main/ets
 *   4: aceModuleBuild/etsFortgz directory: /library/build/default/intermediates/loader_out/etsFortgz
 *      If compile the hsp module, the declaration file will be written to the 'aceModuleBuild/etsFortgz' directory.
 * @param modulePathMap packageName of local har package should be reserved as it is a fixed part of ohmUrl.
 *   example: modulePathMap: { packageName: path }
 * @returns reservedFileNames
 */
export function collectResevedFileNameInIDEConfig(
  ohPackagePath: string,
  projectConfig: any,
  modulePathMap: Object | undefined,
  entryArray: Array<string> | undefined,
): string[] {
  const reservedFileNames: string[] = [];
  const moduleJsonPath: string = projectConfig.aceModuleJsonPath;
  const projectPath: string = projectConfig.projectPath;
  const cachePath: string = projectConfig.cachePath;

  if (entryArray) {
    entryArray.forEach((element) => {
      FileUtils.collectPathReservedString(element, reservedFileNames);
    });
  }

  if (modulePathMap) {
    const modulePaths = Object.values(modulePathMap);
    const moduleNames = Object.keys(modulePathMap);
    modulePaths.forEach((val) => {
      FileUtils.collectPathReservedString(val, reservedFileNames);
    });
    moduleNames.forEach((val) => {
      FileUtils.collectPathReservedString(val, reservedFileNames);
    });
  }
  if (fs.existsSync(ohPackagePath)) {
    const ohPackageContent = JSON5.parse(fs.readFileSync(ohPackagePath, 'utf-8'));
    ohPackageContent.main && FileUtils.collectPathReservedString(ohPackageContent.main, reservedFileNames);
    ohPackageContent.types && FileUtils.collectPathReservedString(ohPackageContent.types, reservedFileNames);
  }

  if (fs.existsSync(moduleJsonPath)) {
    const moduleJsonContent = JSON5.parse(fs.readFileSync(moduleJsonPath, 'utf-8'));
    moduleJsonContent.module?.srcEntry &&
      FileUtils.collectPathReservedString(moduleJsonContent.module?.srcEntry, reservedFileNames);
  }

  if (projectConfig.compileShared || projectConfig.byteCodeHar) {
    FileUtils.collectPathReservedString(projectConfig.aceModuleBuild, reservedFileNames);
    reservedFileNames.push('etsFortgz');
  }

  FileUtils.collectPathReservedString(projectPath, reservedFileNames);
  FileUtils.collectPathReservedString(cachePath, reservedFileNames);
  return reservedFileNames;
}

export function readNameCache(nameCachePath: string, printObfLogger: Function): void {
  try {
    const fileContent = fs.readFileSync(nameCachePath, 'utf-8');
    const nameCache: {
      compileSdkVersion?: string;
      [key: string]: Object;
      PropertyCache?: Object;
      FileNameCache?: Object;
    } = JSON.parse(fileContent);
    if (nameCache.PropertyCache) {
      PropCollections.historyMangledTable = getMapFromJson(nameCache.PropertyCache);
    }
    if (nameCache.FileNameCache) {
      renameFileNameModule.historyFileNameMangledTable = getMapFromJson(nameCache.FileNameCache);
    }

    const { compileSdkVersion, PropertyCache, FileNameCache, ...rest } = nameCache;
    Object.keys(rest).forEach((key) => {
      nameCacheMap.set(key, rest[key]);
    });
  } catch (err) {
    const errorInfo: string = `Failed to open ${nameCachePath}. Error message: ${err}`;
    const errorCodeInfo: HvigorErrorInfo = {
      code: '10804002',
      description: 'ArkTS compiler Error',
      cause: `Failed to open namecache file from ${nameCachePath}, Error message: ${err}`,
      position: nameCachePath,
      solutions: [`Please check ${nameCachePath} as error message suggested.`],
    };
    printObfLogger(errorInfo, errorCodeInfo, 'error');
  }
}

// Clear name caches, used when we need to reobfuscate all files
export function clearNameCache(): void {
  PropCollections.historyMangledTable?.clear();
  nameCacheMap?.clear();
  clearHistoryUnobfuscatedMap();
}

/**
 * collect the reserved or excluded paths containing wildcards
 */
export function handleUniversalPathInObf(mergedObConfig: MergedConfig, allSourceFilePaths: Set<string>): void {
  if (
    !mergedObConfig ||
    (mergedObConfig.keepUniversalPaths.length === 0 && mergedObConfig.excludeUniversalPaths.length === 0)
  ) {
    return;
  }
  for (const realFilePath of allSourceFilePaths) {
    let isReserved = false;
    for (const universalPath of mergedObConfig.keepUniversalPaths) {
      if (universalPath.test(realFilePath)) {
        isReserved = true;
        break;
      }
    }
    for (const excludePath of mergedObConfig.excludeUniversalPaths) {
      if (excludePath.test(realFilePath)) {
        isReserved = false;
        mergedObConfig.excludePathSet.add(realFilePath);
        break;
      }
    }
    if (isReserved) {
      mergedObConfig.keepSourceOfPaths.push(realFilePath);
    }
  }
}

export function getArkguardNameCache(
  enablePropertyObfuscation: boolean,
  enableFileNameObfuscation: boolean,
  enableExportObfuscation: boolean,
  sdkVersion: string,
  entryPackageInfo: string,
): string {
  let writeContent: string = '';
  let nameCacheCollection: {
    compileSdkVersion?: string;
    PropertyCache?: Object;
    FileNameCache?: Object;
    entryPackageInfo?: string;
  } = Object.fromEntries(nameCacheMap.entries());
  nameCacheCollection.compileSdkVersion = sdkVersion;
  nameCacheCollection.entryPackageInfo = entryPackageInfo;

  if (enablePropertyObfuscation || enableExportObfuscation) {
    const mergedPropertyNameCache: Map<string, string> = new Map();
    fillNameCache(PropCollections.historyMangledTable, mergedPropertyNameCache);
    fillNameCache(PropCollections.globalMangledTable, mergedPropertyNameCache);
    nameCacheCollection.PropertyCache = Object.fromEntries(mergedPropertyNameCache);
  }

  if (enableFileNameObfuscation) {
    const mergedFileNameCache: Map<string, string> = new Map();
    fillNameCache(renameFileNameModule.historyFileNameMangledTable, mergedFileNameCache);
    fillNameCache(renameFileNameModule.globalFileNameMangledTable, mergedFileNameCache);
    nameCacheCollection.FileNameCache = Object.fromEntries(mergedFileNameCache);
  }

  writeContent += JSON.stringify(nameCacheCollection, null, 2);
  return writeContent;
}

// export fillNameCache function
export function fillNameCache(table: Map<string, string>, nameCache: Map<string, string>): void {
  if (table) {
    for (const [key, value] of table.entries()) {
      nameCache.set(key, value);
    }
  }
  return;
}

export function writeObfuscationNameCache(
  projectConfig: any,
  entryPackageInfo: string,
  obfuscationCacheDir?: string,
  printNameCache?: string,
): void {
  if (!projectConfig.arkObfuscator) {
    return;
  }
  let options = projectConfig.obfuscationMergedObConfig.options;
  let writeContent = getArkguardNameCache(
    options.enablePropertyObfuscation,
    options.enableFileNameObfuscation,
    options.enableExportObfuscation,
    projectConfig.etsLoaderVersion,
    entryPackageInfo,
  );
  if (obfuscationCacheDir && obfuscationCacheDir.length > 0) {
    const defaultNameCachePath: string = path.join(obfuscationCacheDir, 'nameCache.json');
    if (!fs.existsSync(path.dirname(defaultNameCachePath))) {
      fs.mkdirSync(path.dirname(defaultNameCachePath), { recursive: true });
    }
    fs.writeFileSync(defaultNameCachePath, writeContent);
  }
  if (printNameCache && printNameCache.length > 0) {
    fs.writeFileSync(printNameCache, writeContent);
  }
}

// Print unobfuscation names, reasons and whitelist, if -print-kept-names is enabled.
export function writeUnobfuscationContent(projectConfig: any): void {
  let obfuscationOptions = projectConfig.obfuscationMergedObConfig.options;
  let unobfuscationOptions = projectConfig.arkObfuscator.mCustomProfiles.mUnobfuscationOption;
  let nameOptions = projectConfig.arkObfuscator.mCustomProfiles.mNameObfuscation;
  if (!unobfuscationOptions.mPrintKeptNames) {
    return;
  }

  let configPath = unobfuscationOptions.mPrintPath;
  let printDir = projectConfig.obfuscationOptions.obfuscationCacheDir;
  let printUnobfPath = path.join(printDir, 'keptNames.json');
  printUnobfuscationReasons(configPath, printUnobfPath);
  let printWhitelistPath = path.join(printDir, 'whitelist.json');
  printWhitelist(obfuscationOptions, nameOptions, printWhitelistPath);
}

// Merge similar whitelists and output according to whether the corresponding options are enabled.
export function printWhitelist(obfuscationOptions: ObOptions, nameOptions: INameObfuscationOption, defaultPath: string): void {
  const enableToplevel = obfuscationOptions.enableToplevelObfuscation;
  const enableProperty = obfuscationOptions.enablePropertyObfuscation;
  const enableStringProp = obfuscationOptions.enableStringPropertyObfuscation;
  const enableExport = obfuscationOptions.enableExportObfuscation;
  const enableAtKeep = obfuscationOptions.enableAtKeep;
  const reservedConfToplevelArrary = nameOptions.mReservedToplevelNames ?? [];
  const reservedConfPropertyArray = nameOptions.mReservedProperties ?? [];
  let whitelistObj = {
    lang: [],
    conf: [],
    struct: [],
    exported: [],
    strProp: [],
    enum: []
  };

  let languareSet: Set<string>;
  if (enableProperty) {
    languareSet = mergeSet(UnobfuscationCollections.reservedLangForProperty, LocalVariableCollections.reservedLangForLocal);
  } else {
    languareSet = LocalVariableCollections.reservedLangForLocal;
  }
  whitelistObj.lang = convertSetToArray(languareSet);

  let structSet: Set<string>;
  if (enableProperty) {
    structSet = UnobfuscationCollections.reservedStruct;
  }
  whitelistObj.struct = convertSetToArray(structSet);

  let exportedSet: Set<string>;
  if (enableProperty) {
    exportedSet = UnobfuscationCollections.reservedExportNameAndProp;
  } else if (enableExport) {
    exportedSet = UnobfuscationCollections.reservedExportName;
  }
  whitelistObj.exported = convertSetToArray(exportedSet);

  let stringSet: Set<string>;
  if (enableProperty && !enableStringProp) {
    stringSet = UnobfuscationCollections.reservedStrProp;
  }
  whitelistObj.strProp = convertSetToArray(stringSet);

  whitelistObj.conf = convertSetToArray(LocalVariableCollections.reservedConfig);
  const hasPropertyConfig = enableProperty && reservedConfPropertyArray?.length > 0;
  const hasTopLevelConfig = enableToplevel && reservedConfToplevelArrary?.length > 0;
  if (hasPropertyConfig) {
    whitelistObj.conf.push(...reservedConfPropertyArray);
    handleUniversalReservedList(nameOptions.mUniversalReservedProperties, whitelistObj.conf);
  }
  if (hasTopLevelConfig) {
    whitelistObj.conf.push(...reservedConfToplevelArrary);
    handleUniversalReservedList(nameOptions.mUniversalReservedToplevelNames, whitelistObj.conf);
  }
  if (enableAtKeep) {
    let atKeepSet: Set<string> = new Set();
    addToSet(atKeepSet, AtKeepCollections.keepAsConsumer.globalNames);
    addToSet(atKeepSet, AtKeepCollections.keepAsConsumer.propertyNames);
    addToSet(atKeepSet, AtKeepCollections.keepSymbol.globalNames);
    addToSet(atKeepSet, AtKeepCollections.keepSymbol.propertyNames);
    whitelistObj.conf.push(...atKeepSet);
  }

  let atIndentSet: Set<string> = new Set();
  addToSet(atIndentSet, AtIntentCollections.propertyNames);
  addToSet(atIndentSet, AtIntentCollections.globalNames);
  whitelistObj.conf.push(...atIndentSet);

  let enumSet: Set<string>;
  if (enableProperty) {
    enumSet = UnobfuscationCollections.reservedEnum;
  }
  whitelistObj.enum = convertSetToArray(enumSet);

  let whitelistContent = JSON.stringify(whitelistObj, null, 2); // 2: indentation
  if (!fs.existsSync(path.dirname(defaultPath))) {
    fs.mkdirSync(path.dirname(defaultPath), { recursive: true });
  }
  fs.writeFileSync(defaultPath, whitelistContent);
}

function handleUniversalReservedList(universalList: RegExp[] | undefined, configArray: string[]): void {
  if (universalList?.length > 0) {
    universalList.forEach((value) => {
      const originalString = UnobfuscationCollections.reservedWildcardMap.get(value);
      if (originalString) {
        configArray.push(originalString);
      }
    });
  }
}

// Merge KeptReasons and KeptNames and output
export function printUnobfuscationReasons(configPath: string, defaultPath: string): void {
  let property: Record<string, string[]> = {};
  let unobfuscationObj = { keptReasons: {}, keptNames: { property } };
  type WhitelistObject = {
    [key in WhitelistType]: string;
  };
  let keptReasons: WhitelistObject = {
    sdk: 'same as the system api names',
    lang: 'same as the language keywords',
    conf: 'same as the user-configured kept name',
    struct: 'same as the ArkUI struct property',
    strProp: 'same as the string property',
    exported: 'same as the exported names and properties',
    enum: 'same as the members in the enum'
  };
  unobfuscationObj.keptReasons = keptReasons;

  if (historyUnobfuscatedPropMap.size === 0) {
    // Full compilation or there is no cache after the previous compilation.
    UnobfuscationCollections.unobfuscatedPropMap.forEach((value: Set<string>, key: string) => {
      let array: string[] = Array.from(value);
      unobfuscationObj.keptNames.property[key] = array;
    });
  } else {
    // Retrieve the cache from 'historyUnobfuscatedPropMap' after the previous compilation.
    UnobfuscationCollections.unobfuscatedPropMap.forEach((value: Set<string>, key: string) => {
      let array: string[] = Array.from(value);
      historyUnobfuscatedPropMap.set(key, array);
    });
    historyUnobfuscatedPropMap.forEach((value: string[], key: string) => {
      unobfuscationObj.keptNames.property[key] = value;
    });
  }

  if (historyAllUnobfuscatedNamesMap.size === 0) {
    // Full compilation or there is no cache after the previous compilation.
    Object.assign(unobfuscationObj.keptNames, unobfuscationNamesObj);
  } else {
    // Retrieve the cache from 'historyAllUnobfuscatedNamesMap' after the previous compilation.
    let historyAllUnobfuscatedNamesObj = Object.fromEntries(historyAllUnobfuscatedNamesMap);
    Object.keys(unobfuscationNamesObj).forEach(key => {
      historyAllUnobfuscatedNamesObj[key] = unobfuscationNamesObj[key];
    });
    Object.assign(unobfuscationObj.keptNames, historyAllUnobfuscatedNamesObj);
  }

  let unobfuscationContent = JSON.stringify(unobfuscationObj, null, 2);
  if (!fs.existsSync(path.dirname(defaultPath))) {
    fs.mkdirSync(path.dirname(defaultPath), { recursive: true });
  }
  fs.writeFileSync(defaultPath, unobfuscationContent);

  if (!fs.existsSync(path.dirname(configPath))) {
    fs.mkdirSync(path.dirname(configPath), { recursive: true });
  }
  if (configPath) {
    fs.copyFileSync(defaultPath, configPath);
  }
}


export function generateConsumerObConfigFile(obfuscationOptions: SourceObConfig, printObfLogger: Function): void {
  const projectConfig = { obfuscationOptions, compileHar: true };
  const obConfig: ObConfigResolver = new ObConfigResolver(projectConfig, printObfLogger);
  obConfig.resolveObfuscationConfigs();
}

export function mangleFilePath(originalPath: string): string {
  const mangledFilePath = renameFileNameModule.getMangleCompletePath(originalPath);
  return mangledFilePath;
}

export function enableObfuscatedFilePathConfig(isPackageModules: boolean, projectConfig: any): boolean {
  const isDebugMode = isDebug(projectConfig);
  const hasObfuscationConfig = projectConfig.obfuscationMergedObConfig;
  if (isDebugMode || !hasObfuscationConfig) {
    return false;
  }
  const disableObfuscation = hasObfuscationConfig.options.disableObfuscation;
  const enableFileNameObfuscation = hasObfuscationConfig.options.enableFileNameObfuscation;
  const enableBytecodeObfuscation = hasObfuscationConfig.options.bytecodeObf?.enable;
  if (disableObfuscation || !enableFileNameObfuscation || enableBytecodeObfuscation) {
    return false;
  }
  return true;
}

export function handleObfuscatedFilePath(filePath: string, isPackageModules: boolean, projectConfig: Object): string {
  if (!enableObfuscatedFilePathConfig(isPackageModules, projectConfig)) {
    return filePath;
  }
  // Do not obfuscate the file path in dir oh_modules.
  if (!isPackageModules) {
    return mangleFilePath(filePath);
  }
  // When open the config 'enableFileNameObfuscation', keeping all paths in unix format.
  return FileUtils.toUnixPath(filePath);
}

export function enableObfuscateFileName(isPackageModules: boolean, projectConfig: Object): boolean {
  if (!enableObfuscatedFilePathConfig(isPackageModules, projectConfig)) {
    return false;
  }

  // Do not obfuscate the file path in dir oh_modules.
  if (!isPackageModules) {
    return true;
  }
  // When open the config 'enableFileNameObfuscation', keeping all paths in unix format.
  return false;
}

/**
 * Get the relative path relative to the project based on the file's associated project
 */
export function getRelativeSourcePath(
  filePath: string,
  projectRootPath: string | undefined,
  belongProjectPath: string | undefined,
): string {
  filePath = FileUtils.toUnixPath(filePath);

  if (projectRootPath) {
    projectRootPath = FileUtils.toUnixPath(projectRootPath);
  }

  if (belongProjectPath) {
    belongProjectPath = FileUtils.toUnixPath(belongProjectPath);
  }

  let relativeFilePath: string = filePath;
  if (projectRootPath && filePath.startsWith(projectRootPath)) {
    relativeFilePath = filePath.replace(projectRootPath + '/', '');
  } else if (belongProjectPath) {
    relativeFilePath = filePath.replace(belongProjectPath + '/', '');
  }

  return relativeFilePath;
}

/**
 * 'rootAbsPath\\sdk\\default\\openharmony\\ets\\api',
 * 'rootAbsPath\\sdk\\default\\openharmony\\ets\\arkts',
 * 'rootAbsPath\\sdk\\default\\openharmony\\ets\\kits',
 * 'rootAbsPath\\sdk\\default\\hms\\ets\\api',
 * 'rootAbsPath\\sdk\\default\\hms\\ets\\kits',
 * 'rootAbsPath\\sdk\\default\\openharmony\\ets\\api'
 * obfuscationCacheDir: rootAbsPath\\moduleName\\build\\default\\cache\\default\\default@CompileArkTS\\esmodule\\release\\obfuscation
 * exportRulePath: rootAbsPath\\moduleName\\build\\default\\intermediates\\obfuscation\\default\\obfuscation.txt
 * dependencies: { libraries: [], hars: [], hsps: [], hspLibraries: [] }
 */

export interface SourceObConfig {
  selfConfig: Obfuscation;
  sdkApis: string[];
  obfuscationCacheDir: string;
  exportRulePath: string;
  dependencies: ObfuscationDependencies;
}

export interface Obfuscation {
  ruleOptions?: RuleOptions; 
  consumerRules?: string[]; // absolute path of consumer-rules.txt
  consumerFiles?: string | string[]; // relative path of consumer-rules.txt
  libDir?: string; // actual path of local module
}

export interface RuleOptions {
  enable?: boolean;
  files?: string | string[]; // should be relative path of obfuscation-rules.txt, but undefined now
  rules?: string[]; // absolute path of obfuscation-rules.txt
}

export interface ObfuscationDependencies {
  libraries: Obfuscation[];
  hars: string[];
  hsps?: string[];
  hspLibraries?: Obfuscation[];
}
