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

import {
  ArkObfuscator,
  ObfuscationResultType,
  PropCollections,
  performancePrinter,
  renameIdentifierModule
} from './ArkObfuscator';
import { readProjectProperties } from './common/ApiReaderForTest';
import { FileUtils } from './utils/FileUtils';
import {
  EventList,
  endFilesEvent,
  endSingleFileEvent,
  printTimeSumData,
  printTimeSumInfo,
  startFilesEvent,
  startSingleFileEvent,
} from './utils/PrinterUtils';
import { handleReservedConfig } from './utils/TransformUtil';
import {
  IDENTIFIER_CACHE,
  NAME_CACHE_SUFFIX,
  PROPERTY_CACHE_FILE,
  deleteLineInfoForNameString,
  getMapFromJson,
  readCache,
  writeCache
} from './utils/NameCacheUtil';

import * as fs from 'fs';
import path from 'path';
import ingoreTest262List from './configs/ingoreFilenameList/ingoreTest262List.json';
import ingoreCompilerTestList from './configs/ingoreFilenameList/ingoreCompilerTestList.json';
import { UnobfuscationCollections } from './utils/CommonCollections';
import { unobfuscationNamesObj } from './initialization/CommonObject';
import { printUnobfuscationReasons } from './initialization/ConfigResolver';
import { mergeSet, convertSetToArray } from './initialization/utils';

import type { IOptions } from './configs/IOptions';

const JSON_TEXT_INDENT_LENGTH: number = 2;

interface OutPathObj {
  outputPath: string;
  relativePath: string;
}

export class ArkObfuscatorForTest extends ArkObfuscator {
  // A list of source file path
  private readonly mSourceFiles: string[];

  // Path of obfuscation configuration file.
  private readonly mConfigPath: string;

  private mTestType: string | undefined = undefined;

  constructor(sourceFiles?: string[], configPath?: string) {
    super();
    this.mSourceFiles = sourceFiles;
    this.mConfigPath = configPath;
  }

  public get configPath(): string {
    return this.mConfigPath;
  }

  public setTestType(testType: string | undefined): void {
    this.mTestType = testType;
  }

  /**
   * init ArkObfuscator according to user config
   * should be called after constructor
   */
  public init(config: IOptions | undefined): boolean {
    if (!config) {
        console.error('obfuscation config file is not found and no given config.');
        return false;
    }

    handleReservedConfig(config, 'mNameObfuscation', 'mReservedProperties', 'mUniversalReservedProperties');
    handleReservedConfig(config, 'mNameObfuscation', 'mReservedToplevelNames', 'mUniversalReservedToplevelNames');
    return super.init(config);
  }

  /**
   * Obfuscate all the source files.
   */
  public async obfuscateFiles(): Promise<void> {
    if (!path.isAbsolute(this.mCustomProfiles.mOutputDir)) {
      this.mCustomProfiles.mOutputDir = path.join(path.dirname(this.mConfigPath), this.mCustomProfiles.mOutputDir);
    }

    startFilesEvent(EventList.ALL_FILES_OBFUSCATION);
    readProjectProperties(this.mSourceFiles, structuredClone(this.mCustomProfiles), this);
    const propertyCachePath = path.join(this.mCustomProfiles.mOutputDir, 
                                        path.basename(this.mSourceFiles[0])); // Get dir name
    this.readPropertyCache(propertyCachePath);

    // support directory and file obfuscate
    for (const sourcePath of this.mSourceFiles) {
      if (!fs.existsSync(sourcePath)) {
        console.error(`File ${FileUtils.getFileName(sourcePath)} is not found.`);
        return;
      }

      if (fs.lstatSync(sourcePath).isFile()) {
        await this.obfuscateFile(sourcePath, this.mCustomProfiles.mOutputDir);
        continue;
      }

      const dirPrefix: string = FileUtils.getPrefix(sourcePath);
      await this.obfuscateDir(sourcePath, dirPrefix);
    }

    if (this.mCustomProfiles.mUnobfuscationOption?.mPrintKeptNames) {
      const dir = path.dirname(this.mSourceFiles[0]).replace('grammar', 'local');
      const basename = path.basename(this.mSourceFiles[0]);
      let printKeptNamesPath = path.join(dir, basename, '/keptNames.unobf.json');
      let printWhitelistPath = path.join(dir, basename, '/whitelist.unobf.json');
      this.writeUnobfuscationContentForTest(printKeptNamesPath, printWhitelistPath);
    }

    this.producePropertyCache(propertyCachePath);
    printTimeSumInfo('All files obfuscation:');
    printTimeSumData();
    endFilesEvent(EventList.ALL_FILES_OBFUSCATION);
  }

  private writeUnobfuscationContentForTest(printKeptNamesPath: string, printWhitelistPath: string): void {
    printUnobfuscationReasons('', printKeptNamesPath);
    this.printWhitelist(this.mCustomProfiles, printWhitelistPath);
  }

  private printWhitelist(obfuscationOptions: IOptions, printPath: string): void {
    const nameOption = obfuscationOptions.mNameObfuscation;
    const enableToplevel = nameOption.mTopLevel;
    const enableProperty = nameOption.mRenameProperties;
    const enableStringProp = !nameOption.mKeepStringProperty;
    const enableExport = obfuscationOptions.mExportObfuscation;
    const reservedConfToplevelArrary = nameOption.mReservedToplevelNames ?? [];
    const reservedConfPropertyArray = nameOption.mReservedProperties ?? [];

    let whitelistObj = {
      lang: [],
      conf: [],
      struct: [],
      exported: [],
      strProp: []
    };
  
    if (enableExport || enableProperty) {
      const languageSet = mergeSet(UnobfuscationCollections.reservedLangForProperty, UnobfuscationCollections.reservedLangForTopLevel);
      whitelistObj.lang = convertSetToArray(languageSet);
      const strutSet = UnobfuscationCollections.reservedStruct;
      whitelistObj.struct = convertSetToArray(strutSet);
      const exportSet = mergeSet(UnobfuscationCollections.reservedExportName, UnobfuscationCollections.reservedExportNameAndProp);
      whitelistObj.exported = convertSetToArray(exportSet);
      if (!enableStringProp) {
        const stringSet = UnobfuscationCollections.reservedStrProp;
        whitelistObj.strProp = convertSetToArray(stringSet);
      }
    }
  
    const hasPropertyConfig = enableProperty && reservedConfPropertyArray?.length > 0;
    const hasTopLevelConfig = enableToplevel && reservedConfToplevelArrary?.length > 0;
    if (hasPropertyConfig) {
      // if -enable-property-obfuscation and -enable-toplevel-obfuscation,
      // the mReservedToplevelNames has already been merged into the mReservedToplevelNames.
      whitelistObj.conf.push(...reservedConfPropertyArray);
      this.handleUniversalReservedList(nameOption.mUniversalReservedProperties, whitelistObj.conf);
    } else if (hasTopLevelConfig) {
      whitelistObj.conf.push(...reservedConfToplevelArrary);
      this.handleUniversalReservedList(nameOption.mUniversalReservedToplevelNames, whitelistObj.conf);
    }
  
    let whitelistContent = JSON.stringify(whitelistObj, null, 2);
    if (!fs.existsSync(path.dirname(printPath))) {
      fs.mkdirSync(path.dirname(printPath), { recursive: true });
    }
    fs.writeFileSync(printPath, whitelistContent);
  }

  private handleUniversalReservedList(universalList: RegExp[] | undefined, configArray: string[]): void {
    if (universalList?.length > 0) {
      universalList.forEach((value) => {
        const originalString = UnobfuscationCollections.reservedWildcardMap.get(value);
        if (originalString) {
          configArray.push(originalString);
        }
      });
    }
  }

  /**
   * obfuscate directory
   * @private
   */
  private async obfuscateDir(dirName: string, dirPrefix: string): Promise<void> {
    const currentDir: string = FileUtils.getPathWithoutPrefix(dirName, dirPrefix);
    let newDir: string = this.mCustomProfiles.mOutputDir;
    // there is no need to create directory because the directory names will be obfuscated.
    if (!this.mCustomProfiles.mRenameFileName?.mEnable) {
      newDir = path.join(this.mCustomProfiles.mOutputDir, currentDir);
    }

    const fileNames: string[] = fs.readdirSync(dirName);
    for (let fileName of fileNames) {
      const filePath: string = path.join(dirName, fileName);
      if (fs.lstatSync(filePath).isFile()) {
        await this.obfuscateFile(filePath, newDir);
        continue;
      }

      await this.obfuscateDir(filePath, dirPrefix);
    }
  }

  /**
   * Obfuscate single source file with path provided
   *
   * @param sourceFilePath single source file path
   * @param outputDir
   */
  public async obfuscateFile(sourceFilePath: string, outputDir: string): Promise<void> {
    const fileName: string = FileUtils.getFileName(sourceFilePath);
    const config = this.mCustomProfiles;
    if (this.isObfsIgnoreFile(fileName)) {
      fs.mkdirSync(outputDir, { recursive: true });
      fs.copyFileSync(sourceFilePath, path.join(outputDir, fileName));
      return;
    }

    // To skip the path where 262 and compiler test will fail.
    if (this.shouldIgnoreFile(sourceFilePath)) {
      return;
    }

    // Add the whitelist of file name obfuscation for ut.
    if (config.mRenameFileName?.mEnable) {
      const reservedArray = config.mRenameFileName.mReservedFileNames;
      FileUtils.collectPathReservedString(this.mConfigPath, reservedArray);
    }
    let content: string = FileUtils.readFile(sourceFilePath);
    this.readNameCache(sourceFilePath, outputDir);
    startFilesEvent(sourceFilePath);
    let filePath = { buildFilePath: sourceFilePath, relativeFilePath: sourceFilePath };
    startSingleFileEvent(EventList.OBFUSCATE, performancePrinter.timeSumPrinter, sourceFilePath);
    const mixedInfo: ObfuscationResultType = await this.obfuscate(content, filePath);
    endSingleFileEvent(EventList.OBFUSCATE, performancePrinter.timeSumPrinter);
    endFilesEvent(sourceFilePath, undefined, true);

    if (this.mWriteOriginalFile && mixedInfo) {
      // Write the obfuscated content directly to orignal file.
      fs.writeFileSync(sourceFilePath, mixedInfo.content);
      return;
    }
    if (outputDir && mixedInfo) {
      const outputPathObj: OutPathObj = this.getOutputPath(sourceFilePath, mixedInfo);
      this.writeContent(outputPathObj.outputPath, outputPathObj.relativePath, mixedInfo);
    }
  }

  private getOutputPath(sourceFilePath: string, mixedInfo: ObfuscationResultType): OutPathObj {
    const config = this.mCustomProfiles;
    if (this.mTestType === 'grammar') {
      const testCasesRootPath = path.join(__dirname, '../', 'test/grammar');
      let relativePath = '';
      if (config.mRenameFileName?.mEnable && mixedInfo.filePath) {
        relativePath = mixedInfo.filePath.replace(testCasesRootPath, '');
      } else {
        relativePath = sourceFilePath.replace(testCasesRootPath, '');
      }
      const resultPath = path.join(config.mOutputDir, relativePath);
      return {outputPath: resultPath, relativePath: relativePath};
    } else if (this.mTestType === 'combinations') {
      const outputDir = this.mCustomProfiles.mOutputDir;
      const directory = outputDir.substring(0, outputDir.lastIndexOf('/') + 1);
      const sourceBaseDir = directory.replace('local/combinations', 'combinations');
      const relativePath = sourceFilePath.replace(sourceBaseDir, '');
      const resultPath = path.join(this.mCustomProfiles.mOutputDir, relativePath);
      return {outputPath: resultPath, relativePath: relativePath};
    } else {
      throw new Error('Please select a test type');
    }
  }

  private writeContent(outputPath: string, relativePath: string, mixedInfo: ObfuscationResultType): void {
    if (!fs.existsSync(path.dirname(outputPath))) {
      fs.mkdirSync(path.dirname(outputPath), { recursive: true });
    }

    fs.writeFileSync(outputPath, mixedInfo.content);

    if (this.mCustomProfiles.mEnableSourceMap && mixedInfo.sourceMap) {
      fs.writeFileSync(path.join(outputPath + '.map'),
        JSON.stringify(mixedInfo.sourceMap, null, JSON_TEXT_INDENT_LENGTH));
    }

    if (this.mCustomProfiles.mEnableNameCache && this.mCustomProfiles.mEnableNameCache) {
      this.produceNameCache(mixedInfo.nameCache, outputPath);
    }

    if (mixedInfo.unobfuscationNameMap) {
      this.loadunobfuscationNameMap(mixedInfo, relativePath);
    }
  }

  private loadunobfuscationNameMap(mixedInfo: ObfuscationResultType, relativePath: string): void {
    let arrayObject: Record<string, string[]> = {};
    // The type of unobfuscationNameMap's value is Set, convert Set to Array.
    mixedInfo.unobfuscationNameMap.forEach((value: Set<string>, key: string) => {
      let array: string[] = Array.from(value);
      arrayObject[key] = array;
    });
    unobfuscationNamesObj[relativePath] = arrayObject;
  }

  private shouldIgnoreFile(sourceFilePath: string): boolean {
    const isIgnored = (path: string, ignoreList: string[]): boolean => ignoreList.includes(path);

    // 1: Relative path of the first-level directory after '.local'
    const compilerTestFilename = this.getPathAfterDirectory(sourceFilePath, '.local', 1);
    if (isIgnored(compilerTestFilename, ingoreCompilerTestList)) {
      return true;
    }

    // 2: Relative path of the second-level directory after 'test262'
    const test262Filename = this.getPathAfterDirectory(sourceFilePath, 'test262', 2);
    return isIgnored(test262Filename, ingoreTest262List);
  }

  private getPathAfterDirectory(fullPath: string, directory: string, level: number): string {  
    const pathParts = fullPath.split('/');  
    const dataIndex = pathParts.indexOf(directory);  
    // -1: The directory name does not exist in the absolute path
    const targetIndex = dataIndex !== -1 ? dataIndex + level : -1;

    if (targetIndex < pathParts.length) {
        return pathParts.slice(targetIndex).join('/');
    }

    return fullPath;
  }

  private produceNameCache(namecache: { [k: string]: string | {} }, resultPath: string): void {
    const nameCachePath: string = resultPath + NAME_CACHE_SUFFIX;
    fs.writeFileSync(nameCachePath, JSON.stringify(namecache, null, JSON_TEXT_INDENT_LENGTH));
  }

  private readNameCache(sourceFile: string, outputDir: string): void {
    if (!this.mCustomProfiles.mNameObfuscation?.mEnable || !this.mCustomProfiles.mEnableNameCache) {
      return;
    }

    const nameCachePath: string = path.join(outputDir, FileUtils.getFileName(sourceFile) + NAME_CACHE_SUFFIX);
    const nameCache: Object = readCache(nameCachePath);
    let historyNameCache = new Map<string, string>();
    let identifierCache = nameCache ? Reflect.get(nameCache, IDENTIFIER_CACHE) : undefined;
    deleteLineInfoForNameString(historyNameCache, identifierCache);

    renameIdentifierModule.historyNameCache = historyNameCache;
  }

  private producePropertyCache(outputDir: string): void {
    if (this.mCustomProfiles.mNameObfuscation &&
      this.mCustomProfiles.mNameObfuscation.mRenameProperties &&
      this.mCustomProfiles.mEnableNameCache) {
      const propertyCachePath: string = path.join(outputDir, PROPERTY_CACHE_FILE);
      writeCache(PropCollections.globalMangledTable, propertyCachePath);
    }
  }

  private readPropertyCache(outputDir: string): void {
    if (!this.mCustomProfiles.mNameObfuscation?.mRenameProperties || !this.mCustomProfiles.mEnableNameCache) {
      return;
    }

    const propertyCachePath: string = path.join(outputDir, PROPERTY_CACHE_FILE);
    const propertyCache: Object = readCache(propertyCachePath);
    if (!propertyCache) {
      return;
    }

    PropCollections.historyMangledTable = getMapFromJson(propertyCache);
  }
}