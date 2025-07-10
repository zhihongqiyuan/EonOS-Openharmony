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
import cluster from 'cluster';
import childProcess from 'child_process';

import { CommonMode } from '../common/common_mode';
import {
  changeFileExtension,
  genCachePath,
  getEs2abcFileThreadNumber,
  genTemporaryModuleCacheDirectoryForBundle,
  isMasterOrPrimary,
  isSpecifiedExt,
  isDebug
} from '../utils';
import {
  ES2ABC,
  EXTNAME_ABC,
  EXTNAME_JS,
  FILESINFO_TXT,
  JSBUNDLE,
  MAX_WORKER_NUMBER,
  TEMP_JS,
  TS2ABC,
  red,
  blue,
  FAIL,
  SUCCESS,
  reset
} from '../common/ark_define';
import {
  mkDir,
  toHashData,
  toUnixPath,
  unlinkSync,
  validateFilePathLength
} from '../../../utils';
import {
  isEs2Abc,
  isTs2Abc
} from '../../../ark_utils';
import { 
  ArkTSErrorDescription, 
  ArkTSInternalErrorDescription, 
  ErrorCode
} from '../error_code';
import {
  LogData,
  LogDataFactory
} from '../logger';

interface File {
  filePath: string;
  cacheFilePath: string;
  sourceFile: string;
  size: number;
}

export class BundleMode extends CommonMode {
  intermediateJsBundle: Map<string, File>;
  filterIntermediateJsBundle: Array<File>;
  hashJsonObject: Object;
  filesInfoPath: string;

  constructor(rollupObject: Object, rollupBundleFileSet: Object) {
    super(rollupObject);
    this.intermediateJsBundle = new Map<string, File>();
    this.filterIntermediateJsBundle = [];
    this.hashJsonObject = {};
    this.filesInfoPath = '';
    this.prepareForCompilation(rollupObject, rollupBundleFileSet);
  }

  prepareForCompilation(rollupObject: Object, rollupBundleFileSet: Object): void {
    this.collectBundleFileList(rollupBundleFileSet);
    this.removeCacheInfo(rollupObject);
    this.filterBundleFileListWithHashJson();
  }

  collectBundleFileList(rollupBundleFileSet: Object): void {
    Object.keys(rollupBundleFileSet).forEach((fileName) => {
      // choose *.js
      if (this.projectConfig.aceModuleBuild && isSpecifiedExt(fileName, EXTNAME_JS)) {
        const tempFilePath: string = changeFileExtension(fileName, TEMP_JS);
        const outputPath: string = path.resolve(this.projectConfig.aceModuleBuild, tempFilePath);
        const cacheOutputPath: string = this.genCacheBundleFilePath(outputPath, tempFilePath);
        let rollupBundleSourceCode: string = '';
        if (rollupBundleFileSet[fileName].type === 'asset') {
          rollupBundleSourceCode = rollupBundleFileSet[fileName].source;
        } else if (rollupBundleFileSet[fileName].type === 'chunk') {
          rollupBundleSourceCode = rollupBundleFileSet[fileName].code;
        } else {
          const errInfo: LogData = LogDataFactory.newInstance(
            ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_SOURCE_CODE_FROM_SUMMARY,
            ArkTSInternalErrorDescription,
            `Failed to retrieve source code for ${fileName} from rollup file set.`
          );
          this.logger.printErrorAndExit(errInfo);
        }
        fs.writeFileSync(cacheOutputPath, rollupBundleSourceCode, 'utf-8');
        if (!fs.existsSync(cacheOutputPath)) {
          const errInfo: LogData = LogDataFactory.newInstance(
            ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GENERATE_CACHE_SOURCE_FILE,
            ArkTSInternalErrorDescription,
            `Failed to generate cached source file: ${fileName}.`
          );
          this.logger.printErrorAndExit(errInfo);
        }
        this.collectIntermediateJsBundle(outputPath, cacheOutputPath);
      }
    });
  }

  filterBundleFileListWithHashJson() {
    if (this.intermediateJsBundle.size === 0) {
      return;
    }
    if (!fs.existsSync(this.hashJsonFilePath) || this.hashJsonFilePath.length === 0) {
      this.intermediateJsBundle.forEach((value) => {
        this.filterIntermediateJsBundle.push(value);
      });
      return;
    }
    let updatedJsonObject: Object = {};
    let jsonObject: Object = {};
    let jsonFile: string = '';
    jsonFile = fs.readFileSync(this.hashJsonFilePath).toString();
    jsonObject = JSON.parse(jsonFile);
    this.filterIntermediateJsBundle = [];
    for (const value of this.intermediateJsBundle.values()) {
      const cacheFilePath: string = value.cacheFilePath;
      const cacheAbcFilePath: string = changeFileExtension(cacheFilePath, EXTNAME_ABC);
      if (!fs.existsSync(cacheFilePath)) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_PACKAGE_CACHE_IN_INCREMENTAL_BUILD,
          ArkTSInternalErrorDescription,
          `Failed to get bundle cached abc from ${cacheFilePath} in incremental build.`
        );
        this.logger.printErrorAndExit(errInfo);
      }
      if (fs.existsSync(cacheAbcFilePath)) {
        const hashCacheFileContentData: string = toHashData(cacheFilePath);
        const hashAbcContentData: string = toHashData(cacheAbcFilePath);
        if (jsonObject[cacheFilePath] === hashCacheFileContentData &&
          jsonObject[cacheAbcFilePath] === hashAbcContentData) {
          updatedJsonObject[cacheFilePath] = hashCacheFileContentData;
          updatedJsonObject[cacheAbcFilePath] = hashAbcContentData;
          continue;
        }
      }
      this.filterIntermediateJsBundle.push(value);
    }

    this.hashJsonObject = updatedJsonObject;
  }

  executeArkCompiler() {
    if (isEs2Abc(this.projectConfig)) {
      this.filesInfoPath = this.generateFileInfoOfBundle();
      this.generateEs2AbcCmd(this.filesInfoPath);
      this.executeEs2AbcCmd();
    } else if (isTs2Abc(this.projectConfig)) {
      const splittedBundles: any[] = this.getSplittedBundles();
      this.invokeTs2AbcWorkersToGenAbc(splittedBundles);
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

  afterCompilationProcess() {
    this.writeHashJson();
    this.copyFileFromCachePathToOutputPath();
    this.cleanTempCacheFiles();
  }

  private generateEs2AbcCmd(filesInfoPath: string) {
    const fileThreads: number = getEs2abcFileThreadNumber();
    this.cmdArgs.push(
      `"@${filesInfoPath}"`,
      '--file-threads',
      `"${fileThreads}"`,
      `"--target-api-version=${this.projectConfig.compatibleSdkVersion}"`,
      '--opt-try-catch-func=false'
    );
    if (this.projectConfig.compatibleSdkReleaseType) {
      this.cmdArgs.push(`"--target-api-sub-version=${this.projectConfig.compatibleSdkReleaseType}"`);
    }
  }

  private generateFileInfoOfBundle(): string {
    const filesInfoPath: string = genCachePath(FILESINFO_TXT, this.projectConfig, this.logger);
    let filesInfo: string = '';
    this.filterIntermediateJsBundle.forEach((info) => {
      const cacheFilePath: string = info.cacheFilePath;
      const recordName: string = 'null_recordName';
      const moduleType: string = 'script';
      // In release mode, there are '.temp.js' and '.js' file in cache path, no js file in output path.
      // In debug mode, '.temp.js' file is in cache path, and '.js' file is in output path.
      // '.temp.js' file is the input of es2abc, and should be uesd as sourceFile here. However，in debug mode ,
      // using '.temp.js' file as sourceFile needs IDE to adapt, so use '.js'  file in output path instead temporarily.
      const sourceFile: string = (isDebug(this.projectConfig) ? info.sourceFile.replace(/(.*)_/, '$1') :
        cacheFilePath).replace(toUnixPath(this.projectConfig.projectRootPath) + '/', '');
      const abcFilePath: string = changeFileExtension(cacheFilePath, EXTNAME_ABC);
      filesInfo += `${cacheFilePath};${recordName};${moduleType};${sourceFile};${abcFilePath}\n`;
    });
    fs.writeFileSync(filesInfoPath, filesInfo, 'utf-8');

    return filesInfoPath;
  }

  private executeEs2AbcCmd() {
    // collect data error from subprocess
    let logDataList: Object[] = [];
    let errMsg: string = '';
    const genAbcCmd: string = this.cmdArgs.join(' ');
    try {
      const child = this.triggerAsync(() => {
        return childProcess.exec(genAbcCmd, { windowsHide: true });
      });
      child.on('close', (code: number) => {
        if (code === SUCCESS) {
          this.afterCompilationProcess();
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

  private genCacheBundleFilePath(outputPath: string, tempFilePath: string): string {
    let cacheOutputPath: string = '';
    if (this.projectConfig.cachePath) {
      cacheOutputPath = path.join(genTemporaryModuleCacheDirectoryForBundle(this.projectConfig), tempFilePath);
    } else {
      cacheOutputPath = outputPath;
    }
    validateFilePathLength(cacheOutputPath, this.logger);
    const parentDir: string = path.join(cacheOutputPath, '..');
    if (!(fs.existsSync(parentDir) && fs.statSync(parentDir).isDirectory())) {
      mkDir(parentDir);
    }

    return cacheOutputPath;
  }

  private collectIntermediateJsBundle(filePath: string, cacheFilePath: string) {
    const fileSize: number = fs.statSync(cacheFilePath).size;
    let sourceFile: string = changeFileExtension(filePath, '_.js', TEMP_JS);
    if (!this.arkConfig.isDebug && this.projectConfig.projectRootPath) {
      sourceFile = sourceFile.replace(this.projectConfig.projectRootPath + path.sep, '');
    }

    filePath = toUnixPath(filePath);
    cacheFilePath = toUnixPath(cacheFilePath);
    sourceFile = toUnixPath(sourceFile);
    const bundleFile: File = {
      filePath: filePath,
      cacheFilePath: cacheFilePath,
      sourceFile: sourceFile,
      size: fileSize
    };
    this.intermediateJsBundle.set(filePath, bundleFile);
  }

  private getSplittedBundles(): any[] {
    const splittedBundles: any[] = this.splitJsBundlesBySize(this.filterIntermediateJsBundle, MAX_WORKER_NUMBER);
    return splittedBundles;
  }

  private invokeTs2AbcWorkersToGenAbc(splittedBundles) {
    if (isMasterOrPrimary()) {
      this.setupCluster(cluster);
      const workerNumber: number = splittedBundles.length < MAX_WORKER_NUMBER ? splittedBundles.length : MAX_WORKER_NUMBER;
      for (let i = 0; i < workerNumber; ++i) {
        const workerData: Object = {
          inputs: JSON.stringify(splittedBundles[i]),
          cmd: this.cmdArgs.join(' '),
          mode: JSBUNDLE
        };
        this.triggerAsync(() => {
          const worker: Object = cluster.fork(workerData);
          worker.on('message', (errorMsg) => {
            this.logger.error(red, errorMsg.data.toString(), reset);
            this.logger.throwArkTsCompilerError('ArkTS:ERROR Failed to execute ts2abc');
          });
        });
      }

      let workerCount: number = 0;
      cluster.on('exit', (worker, code, signal) => {
        if (code === FAIL) {
          this.logger.throwArkTsCompilerError('ArkTS:ERROR Failed to execute ts2abc, exit code non-zero');
        }
        workerCount++;
        if (workerCount === workerNumber) {
          this.afterCompilationProcess();
        }
        this.triggerEndSignal();
      });
    }
  }

  private getSmallestSizeGroup(groupSize: Map<number, number>): any {
    const groupSizeArray: any = Array.from(groupSize);
    groupSizeArray.sort(function(g1, g2) {
      return g1[1] - g2[1]; // sort by size
    });
    return groupSizeArray[0][0];
  }

  private splitJsBundlesBySize(bundleArray: Array<File>, groupNumber: number): any {
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
    const groupFileSize: any = new Map();
    for (let i = 0; i < groupNumber; ++i) {
      result.push([]);
      groupFileSize.set(i, 0);
    }

    let index: number = 0;
    while (index < bundleArray.length) {
      const smallestGroup: any = this.getSmallestSizeGroup(groupFileSize);
      result[smallestGroup].push(bundleArray[index]);
      const sizeUpdate: any = groupFileSize.get(smallestGroup) + bundleArray[index].size;
      groupFileSize.set(smallestGroup, sizeUpdate);
      index++;
    }
    return result;
  }

  private writeHashJson() {
    if (this.hashJsonFilePath.length === 0) {
      return;
    }

    for (let i = 0; i < this.filterIntermediateJsBundle.length; ++i) {
      const cacheFilePath: string = this.filterIntermediateJsBundle[i].cacheFilePath;
      const cacheAbcFilePath: string = changeFileExtension(cacheFilePath, EXTNAME_ABC);
      if (!fs.existsSync(cacheFilePath) || !fs.existsSync(cacheAbcFilePath)) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_HASH_JSON_FILE_GENERATION_MISSING_PATHS,
          ArkTSInternalErrorDescription,
          `During hash JSON file generation, ${cacheFilePath} or ${cacheAbcFilePath} is not found.`
        );
        this.logger.printErrorAndExit(errInfo);
      }
      const hashCacheFileContentData: string = toHashData(cacheFilePath);
      const hashCacheAbcContentData: string = toHashData(cacheAbcFilePath);
      this.hashJsonObject[cacheFilePath] = hashCacheFileContentData;
      this.hashJsonObject[cacheAbcFilePath] = hashCacheAbcContentData;
    }

    fs.writeFileSync(this.hashJsonFilePath, JSON.stringify(this.hashJsonObject), 'utf-8');
  }

  private copyFileFromCachePathToOutputPath() {
    for (const value of this.intermediateJsBundle.values()) {
      const abcFilePath: string = changeFileExtension(value.filePath, EXTNAME_ABC, TEMP_JS);
      const cacheAbcFilePath: string = changeFileExtension(value.cacheFilePath, EXTNAME_ABC);
      if (!fs.existsSync(cacheAbcFilePath)) {
        const errInfo: LogData = LogDataFactory.newInstance(
          ErrorCode.ETS2BUNDLE_INTERNAL_INCREMENTAL_BUILD_MISSING_CACHE_ABC_FILE_PATH,
          ArkTSInternalErrorDescription,
          `${cacheAbcFilePath} not found during incremental build.`
        );
        this.logger.printErrorAndExit(errInfo);
      }
      const parent: string = path.join(abcFilePath, '..');
      if (!(fs.existsSync(parent) && fs.statSync(parent).isDirectory())) {
        mkDir(parent);
      }
      // for preview mode, cache path and old abc file both exist, should copy abc file for updating
      if (this.projectConfig.cachePath !== undefined) {
        fs.copyFileSync(cacheAbcFilePath, abcFilePath);
      }
    }
  }

  private cleanTempCacheFiles() {
    // in xts mode, as cache path is not provided, cache files are located in output path, clear them
    if (this.projectConfig.cachePath !== undefined) {
      return;
    }

    for (const value of this.intermediateJsBundle.values()) {
      if (fs.existsSync(value.cacheFilePath)) {
        fs.unlinkSync(value.cacheFilePath);
      }
    }

    if (isEs2Abc(this.projectConfig) && fs.existsSync(this.filesInfoPath)) {
      unlinkSync(this.filesInfoPath);
    }
  }

  private removeCompilationCache(): void {
    if (fs.existsSync(this.hashJsonFilePath)) {
      fs.unlinkSync(this.hashJsonFilePath);
    }
  }
}
