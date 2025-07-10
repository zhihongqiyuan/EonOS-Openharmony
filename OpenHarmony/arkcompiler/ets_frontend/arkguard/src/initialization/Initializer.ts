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

import { ArkObfuscator, blockPrinter, renameIdentifierModule } from '../ArkObfuscator';
import { collectResevedFileNameInIDEConfig, MergedConfig, ObConfigResolver, readNameCache } from './ConfigResolver';
import { type IOptions } from '../configs/IOptions';
import type { HvigorErrorInfo } from '../common/type';
import { blockTimeAndMemPrinter, getObfuscationCacheDir } from '../utils/PrinterTimeAndMemUtils';

// Record all unobfuscated properties and reasons.
export const historyUnobfuscatedPropMap: Map<string, string[]> = new Map<string, string[]>();
// Record all files and the unobfuscated names and reasons in the files.
export const historyAllUnobfuscatedNamesMap: Map<string, Object> = new Map<string, Object>();

// Clear the map after one compilation is completed.
export function clearHistoryUnobfuscatedMap(): void {
  historyUnobfuscatedPropMap.clear();
  historyAllUnobfuscatedNamesMap.clear();
}

export const printerConfig = {
  // Print obfuscation time&memory usage of all files and obfuscation processes
  mFilesPrinter: false,
  // Print time&memory usage of a single file obfuscation in transform processes
  mSingleFilePrinter: false,
  // Print sum up time of transform processes during obfuscation
  mSumPrinter: false,
  // Output path of printer
  mOutputPath: '',
};

export const printerTimeAndMemDataConfig = {
  // The switch for printing obfuscation performance data and memory data
  mTimeAndMemPrinter: false,
  // Print more obfuscation time data to obtain more detailed time performance data
  mMoreTimePrint: false,
};


export function initObfuscationConfig(projectConfig: any, arkProjectConfig: any, printObfLogger: Function): void {
  const obConfig: ObConfigResolver = new ObConfigResolver(projectConfig, printObfLogger, true);
  let mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
  if (arkProjectConfig.allowEtsAnnotations) {
    mergedObConfig.options.enableEtsAnnotation = arkProjectConfig.allowEtsAnnotations;
  }
  const isHarCompiled: boolean = projectConfig.compileHar;
  if (mergedObConfig.options.disableObfuscation) {
    blockPrinter();
    blockTimeAndMemPrinter();
    return;
  }
  getObfuscationCacheDir(projectConfig);
  /**
   * Only one of bytecode obfuscation and source code obfuscation will take effect.
   * Enabling bytecode obfuscation is to obfuscate declaration files when compile bytecode har.
   */
  const isBytecodeObfEnabled = mergedObConfig.options.bytecodeObf.enable;
  arkProjectConfig.isBytecodeObfEnabled = isBytecodeObfEnabled;
  arkProjectConfig.isArkguardEnabled = !isBytecodeObfEnabled;

  if (mergedObConfig.options.enableFileNameObfuscation) {
    const ohPackagePath = path.join(projectConfig.modulePath, 'oh-package.json5');
    const entryArray = arkProjectConfig.entryArrayForObf;
    const reservedFileNamesInIDEconfig = collectResevedFileNameInIDEConfig(
      ohPackagePath,
      projectConfig,
      arkProjectConfig.modulePathMap,
      entryArray,
    );
    mergedObConfig.reservedFileNames.push(...reservedFileNamesInIDEconfig);
  }
  arkProjectConfig.obfuscationMergedObConfig = mergedObConfig;

  arkProjectConfig.arkObfuscator = initArkGuardConfig(
    projectConfig.obfuscationOptions?.obfuscationCacheDir,
    printObfLogger,
    mergedObConfig,
    isHarCompiled,
  );
  arkProjectConfig.arkObfuscator.obfConfigResolver = obConfig;
}

function initArkGuardConfig(
  obfuscationCacheDir: string | undefined,
  printObfLogger: Function,
  mergedObConfig: MergedConfig,
  isHarCompiled: boolean,
): ArkObfuscator {
  const arkguardConfig: IOptions = {
    mCompact: mergedObConfig.options.compact,
    mDisableConsole: mergedObConfig.options.removeLog,
    mSimplify: false,
    mRemoveComments: true,
    mNameObfuscation: {
      mEnable: true,
      mNameGeneratorType: 1,
      mReservedNames: mergedObConfig.reservedNames,
      mRenameProperties: mergedObConfig.options.enablePropertyObfuscation,
      mReservedProperties: mergedObConfig.reservedPropertyNames,
      mKeepStringProperty: !mergedObConfig.options.enableStringPropertyObfuscation,
      mTopLevel: mergedObConfig.options.enableToplevelObfuscation,
      mReservedToplevelNames: mergedObConfig.reservedGlobalNames,
      mKeepParameterNames: mergedObConfig.options.keepParameterNames,
      mUniversalReservedProperties: mergedObConfig.universalReservedPropertyNames,
      mUniversalReservedToplevelNames: mergedObConfig.universalReservedGlobalNames,
      mEnableAtKeep: mergedObConfig.options.enableAtKeep
    },
    mUnobfuscationOption: {
      mPrintKeptNames: mergedObConfig.options.printKeptNames,
      mPrintPath: mergedObConfig.options.printKeptNamesPath
    },
    mRemoveDeclarationComments: {
      mEnable: mergedObConfig.options.removeComments,
      mReservedComments: mergedObConfig.keepComments,
    },
    mEnableSourceMap: true,
    mEnableNameCache: true,
    mRenameFileName: {
      mEnable: mergedObConfig.options.enableFileNameObfuscation,
      mNameGeneratorType: 1,
      mReservedFileNames: mergedObConfig.reservedFileNames,
    },
    mExportObfuscation: mergedObConfig.options.enableExportObfuscation,
    mPerformancePrinter: printerConfig,
    mKeepFileSourceCode: {
      mKeepSourceOfPaths: new Set(),
      mkeepFilesAndDependencies: new Set(),
    },
    mStripLanguageDefaultWhitelist: mergedObConfig.options.stripLanguageDefault,
    mAllowEtsAnnotations: mergedObConfig.options.enableEtsAnnotation,
  };

  const arkObfuscator: ArkObfuscator = new ArkObfuscator();
  arkObfuscator.init(arkguardConfig, obfuscationCacheDir);
  if (mergedObConfig.options.applyNameCache && mergedObConfig.options.applyNameCache.length > 0) {
    readNameCache(mergedObConfig.options.applyNameCache, printObfLogger);
  } else {
    if (obfuscationCacheDir) {
      const defaultNameCachePath: string = path.join(obfuscationCacheDir, 'nameCache.json');
      if (fs.existsSync(defaultNameCachePath)) {
        readNameCache(defaultNameCachePath, printObfLogger);
      }
    }
  }
  if (mergedObConfig.options.printKeptNames && obfuscationCacheDir) {
    const defaultUnobfuscationPath: string = path.join(obfuscationCacheDir, 'keptNames.json');
    if (fs.existsSync(defaultUnobfuscationPath)) {
      readUnobfuscationContentCache(defaultUnobfuscationPath, printObfLogger);
    }
  }
  return arkObfuscator;
}

function readUnobfuscationContentCache(defaultUnobfuscationPath: string, printObfLogger: Function): void {
  try {
    const unobfuscationContent = fs.readFileSync(defaultUnobfuscationPath, 'utf-8');
    const unobfuscationObj: {
      keptReasons: Object;
      keptNames: {
        property: Object;
        [key: string]: Object;
      };
    } = JSON.parse(unobfuscationContent);

    Object.keys(unobfuscationObj.keptNames.property).forEach((key) => {
      historyUnobfuscatedPropMap.set(key, unobfuscationObj.keptNames.property[key]);
    });

    const { property, ...rest } = unobfuscationObj.keptNames;
    Object.keys(rest).forEach((key) => {
      historyAllUnobfuscatedNamesMap.set(key, rest[key]);
    });
  } catch (err) {
    const errorInfo: string = `Failed to open ${defaultUnobfuscationPath}. Error message: ${err}`;
    const errorCodeInfo: HvigorErrorInfo = {
      code: '10804003',
      description: 'ArkTS compiler Error',
      cause: `Failed to open ${defaultUnobfuscationPath}. Error message: ${err}`,
      position: defaultUnobfuscationPath,
      solutions: [`Please check ${defaultUnobfuscationPath} as error message suggested.`],
    };
    printObfLogger(errorInfo, errorCodeInfo, 'error');
  }
}
