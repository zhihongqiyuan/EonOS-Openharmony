/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import { ApiExtractor } from '../common/ApiExtractor';
import { FileUtils } from './FileUtils';
import {
  AtIntentCollections,
  AtKeepCollections,
  BytecodeObfuscationCollections,
  UnobfuscationCollections
} from './CommonCollections';
import * as crypto from 'crypto';
import * as ts from 'typescript';
import fs from 'fs';
import path from 'path';

export function addToSet<T>(targetSet: Set<T>, sourceSet: Set<T>): void {
  sourceSet.forEach((element) => targetSet.add(element));
}

export function arrayToSet<T>(array: T[]): Set<T> {
  return new Set(array);
}

export function setToArray<T>(set: Set<T>): T[] {
  return Array.from(set);
}

export function areSetsEqual<T>(setA: Set<T>, setB: Set<T>): boolean {
  if (setA.size !== setB.size) {
    return false;
  }
  for (const setItem of setA) {
    if (!setB.has(setItem)) {
      return false;
    }
  }
  return true;
}

export const SOURCE_FILE_PATHS: string = 'sourceFilePaths.cache';
export const TRANSFORMED_PATH: string = 'transformed';
export const FILE_NAMES_MAP: string = 'transformedFileNamesMap.json';
export const FILE_WHITE_LISTS: string = 'fileWhiteLists.json';
export const PROJECT_WHITE_LIST: string = 'projectWhiteList.json';

// this while list is only used for bytecode obfuscation
export const DECORATOR_WHITE_LIST = [
  'Monitor',
  'Track',
  'Trace',
  'AnimatableExtend'
];

export interface KeepInfo {
  propertyNames: Set<string>;
  globalNames: Set<string>;
}

/**
 * Informantion of build files
 */
export interface ModuleInfo {
  content: string;
  /**
   * The path in build cache dir
   */
  buildFilePath: string;
  /**
   * The `originSourceFilePath` relative to project root dir.
   */
  relativeSourceFilePath: string;
  /**
   * The origin source file path will be set with rollup moduleId when obfuscate intermediate js source code,
   * whereas be set with tsc node.fileName when obfuscate intermediate ts source code.
   */
  originSourceFilePath?: string;
  rollupModuleId?: string;
}

export interface FileContent {
  moduleInfo: ModuleInfo;
  previousStageSourceMap?: ts.RawSourceMap;
}

/**
 * We have these structure to store project white list and file white lists,
 * project white list is merged by each file white list.
 *
 * We have keepinfo and reservedinfo in white lists:
 *  KeepInfo: names that we cannot obfuscate or obfuscate as.
 *  ReservedInfo: names that we cannot obfuscate as.
 *
 * ProjectWhiteList
 * ├── projectKeepInfo: ProjectKeepInfo
 * │   ├── propertyNames: Set<string>
 * │   └── globalNames: Set<string>
 * └── projectReservedInfo: ProjectReservedInfo
 *     ├── enumProperties: Set<string>
 *     └── propertyParams: Set<string>
 * FileWhiteList
 * ├── fileKeepInfo: FileKeepInfo
 * │   ├── keepSymbol?: KeepInfo (optional)
 * │   │   ├── propertyNames: Set<string>
 * │   │   └── globalNames: Set<string>
 * │   ├── keepAsConsumer?: KeepInfo (optional)
 * │   │   ├── propertyNames: Set<string>
 * │   │   └── globalNames: Set<string>
 * │   ├── structProperties: Set<string>
 * │   ├── exported: KeepInfo
 * │   │   ├── propertyNames: Set<string>
 * │   │   └── globalNames: Set<string>
 * │   ├── enumProperties: Set<string>
 * │   └── stringProperties: Set<string>
 * │   └── arkUIKeepInfo: KeepInfo
 * │       ├── propertyNames: Set<string>
 * │       └── globalNames: Set<string>
 * └── bytecodeObfuscateKeepInfo: BytecodeObfuscateKeepInfo
 *     └── decoratorMap?: Map<string, string[]>
 * └── fileReservedInfo: FileReservedInfo
 *     ├── enumProperties: Set<string>
 *     └── propertyParams: Set<string>
 */
export interface FileKeepInfo {
  keepSymbol?: KeepInfo; // Names marked with "@KeepSymbol".
  keepAsConsumer?: KeepInfo; // Names marked with "@KeepAsConsumer".
  structProperties: Set<string>; // Struct properties collected from struct.
  exported: KeepInfo; // Exported names and properties.
  enumProperties: Set<string>; // Enum properties.
  stringProperties: Set<string>; // String properties.
  arkUIKeepInfo: KeepInfo; // Collecting classes and members
}

export interface FileReservedInfo {
  enumProperties: Set<string>; // Enum members, collected when has initializer.
  propertyParams: Set<string>; // Properties parameters in constructor.
}

export interface BytecodeObfuscateKeepInfo {
  decoratorMap?: Object; // collect DecoratorMap
}

export interface FileWhiteList {
  fileKeepInfo: FileKeepInfo;
  fileReservedInfo: FileReservedInfo;
  bytecodeObfuscateKeepInfo?: BytecodeObfuscateKeepInfo
}

export interface ProjectKeepInfo {
  propertyNames: Set<string>;
  globalNames: Set<string>;
}

export interface ProjectReservedInfo {
  enumProperties: Set<string>;
  propertyParams: Set<string>;
}

export interface ProjectWhiteList {
  projectKeepInfo: ProjectKeepInfo;
  projectReservedInfo: ProjectReservedInfo;
}

export interface ProjectWhiteListJsonData {
  projectKeepInfo: {
    propertyNames: Array<string>;
    globalNames: Array<string>;
  };
  projectReservedInfo: {
    enumProperties: Array<string>;
    propertyParams: Array<string>;
  };
}


// The object to manage project white lists, should be initialized in arkObfuscator
export let projectWhiteListManager: ProjectWhiteListManager | undefined;

// Initialize projectWhiteListManager
export function initProjectWhiteListManager(cachePath: string, isIncremental: boolean, enableAtKeep: boolean): void {
  projectWhiteListManager = new ProjectWhiteListManager(cachePath, isIncremental, enableAtKeep);
}

// Clear projectWhiteListManager
export function clearProjectWhiteListManager(): void {
  projectWhiteListManager = undefined;
}

/**
 * This class is used to manage project white lists.
 * Used to collect white list of each file and merge them into project white lists.
 */
export class ProjectWhiteListManager {
  // If atKeep is enabled
  private enableAtKeep: boolean;

  // Cache path for file white lists
  private fileWhiteListsCachePath: string;

  // Cache path for project white lists
  private projectWhiteListCachePath: string;

  // If it is incremental compliation
  private isIncremental: boolean = false;

  // If we should reObfuscate all files
  private shouldReObfuscate: boolean = false;

  // White lists of each file
  private fileWhiteListMap: Map<string, FileWhiteList>;

  // White lists for one file
  public fileWhiteListInfo: FileWhiteList | undefined;

  public getEnableAtKeep(): boolean {
    return this.enableAtKeep;
  }

  public getFileWhiteListsCachePath(): string {
    return this.fileWhiteListsCachePath;
  }

  public getProjectWhiteListCachePath(): string {
    return this.projectWhiteListCachePath;
  }

  public getIsIncremental(): boolean {
    return this.isIncremental;
  }

  public getShouldReObfuscate(): boolean {
    return this.shouldReObfuscate;
  }

  public getFileWhiteListMap(): Map<string, FileWhiteList> {
    return this.fileWhiteListMap;
  }

  constructor(cachePath: string, isIncremental: boolean, enableAtKeep: boolean) {
    this.fileWhiteListsCachePath = path.join(cachePath, FILE_WHITE_LISTS);
    this.projectWhiteListCachePath = path.join(cachePath, PROJECT_WHITE_LIST);
    this.isIncremental = isIncremental;
    this.enableAtKeep = enableAtKeep;
    this.fileWhiteListMap = new Map();
  }

  private createDefaultFileKeepInfo(): FileKeepInfo {
    return {
      structProperties: new Set<string>(),
      exported: {
        propertyNames: new Set<string>(),
        globalNames: new Set<string>(),
      },
      enumProperties: new Set<string>(),
      stringProperties: new Set<string>(),
      arkUIKeepInfo: {
        propertyNames: new Set<string>(),
        globalNames: new Set<string>(),
      },
    };
  }

  private createDefaultFileReservedInfo(): FileReservedInfo {
    return {
      enumProperties: new Set<string>(),
      propertyParams: new Set<string>(),
    };
  }

  // Create one fileWhilteList object, with keepSymbol & keepAsConsumer if atKeep is enabled
  public createFileWhiteList(): FileWhiteList {
    const fileKeepInfo = this.enableAtKeep
      ? {
          keepSymbol: {
            propertyNames: new Set<string>(),
            globalNames: new Set<string>(),
          },
          keepAsConsumer: {
            propertyNames: new Set<string>(),
            globalNames: new Set<string>(),
          },
          ...this.createDefaultFileKeepInfo(),
        }
      : this.createDefaultFileKeepInfo();
    return {
      fileKeepInfo,
      fileReservedInfo: this.createDefaultFileReservedInfo(),
    };
  }

  // Initialize current collector,
  // should be called before we collect fileWhiteLists.
  public setCurrentCollector(path: string): void {
    const unixPath = FileUtils.toUnixPath(path);
    let fileWhiteListInfo: FileWhiteList | undefined = this.fileWhiteListMap.get(unixPath);
    if (!fileWhiteListInfo) {
      fileWhiteListInfo = this.createFileWhiteList();
      this.fileWhiteListMap.set(unixPath, fileWhiteListInfo);
    }
    this.fileWhiteListInfo = fileWhiteListInfo;
  }

  private readFileWhiteLists(filePath: string): Map<string, FileWhiteList> {
    const fileContent = fs.readFileSync(filePath, 'utf8');
    const parsed: Object = JSON.parse(fileContent);

    const map = new Map<string, FileWhiteList>();
    for (const key in parsed) {
      if (Object.prototype.hasOwnProperty.call(parsed, key)) {
        const fileKeepInfo: FileKeepInfo = {
          keepSymbol: parsed[key].fileKeepInfo.keepSymbol
            ? {
                propertyNames: arrayToSet(parsed[key].fileKeepInfo.keepSymbol.propertyNames),
                globalNames: arrayToSet(parsed[key].fileKeepInfo.keepSymbol.globalNames),
              }
            : undefined,
          keepAsConsumer: parsed[key].fileKeepInfo.keepAsConsumer
            ? {
                propertyNames: arrayToSet(parsed[key].fileKeepInfo.keepAsConsumer.propertyNames),
                globalNames: arrayToSet(parsed[key].fileKeepInfo.keepAsConsumer.globalNames),
              }
            : undefined,
          structProperties: arrayToSet(parsed[key].fileKeepInfo.structProperties),
          exported: {
            propertyNames: arrayToSet(parsed[key].fileKeepInfo.exported.propertyNames),
            globalNames: arrayToSet(parsed[key].fileKeepInfo.exported.globalNames),
          },
          enumProperties: arrayToSet(parsed[key].fileKeepInfo.enumProperties),
          stringProperties: arrayToSet(parsed[key].fileKeepInfo.stringProperties),
          arkUIKeepInfo: {
            propertyNames: arrayToSet(parsed[key].fileKeepInfo.arkUIKeepInfo.propertyNames),
            globalNames: arrayToSet(parsed[key].fileKeepInfo.arkUIKeepInfo.globalNames),
          },
        };

        const fileReservedInfo: FileReservedInfo = {
          enumProperties: arrayToSet(parsed[key].fileReservedInfo.enumProperties),
          propertyParams: arrayToSet(parsed[key].fileReservedInfo.propertyParams),
        };
        const bytecodeObfuscateKeepInfo: BytecodeObfuscateKeepInfo = {
          decoratorMap: parsed[key].bytecodeObfuscateKeepInfo?.decoratorMap,
        };
        map.set(key, { fileKeepInfo, fileReservedInfo, bytecodeObfuscateKeepInfo });
      }
    }

    return map;
  }

  private writeFileWhiteLists(filePath: string, fileWhiteLists: Map<string, FileWhiteList>): void {
    const jsonData: Object = {};
    for (const [key, value] of fileWhiteLists) {
      jsonData[key] = {
        fileKeepInfo: {
          keepSymbol: value.fileKeepInfo.keepSymbol
            ? {
                propertyNames: setToArray(value.fileKeepInfo.keepSymbol.propertyNames),
                globalNames: setToArray(value.fileKeepInfo.keepSymbol.globalNames),
              }
            : undefined,
          keepAsConsumer: value.fileKeepInfo.keepAsConsumer
            ? {
                propertyNames: setToArray(value.fileKeepInfo.keepAsConsumer.propertyNames),
                globalNames: setToArray(value.fileKeepInfo.keepAsConsumer.globalNames),
              }
            : undefined,
          structProperties: setToArray(value.fileKeepInfo.structProperties),
          exported: {
            propertyNames: setToArray(value.fileKeepInfo.exported.propertyNames),
            globalNames: setToArray(value.fileKeepInfo.exported.globalNames),
          },
          enumProperties: setToArray(value.fileKeepInfo.enumProperties),
          stringProperties: setToArray(value.fileKeepInfo.stringProperties),
          arkUIKeepInfo: {
            propertyNames: setToArray(value.fileKeepInfo.arkUIKeepInfo.propertyNames),
            globalNames: setToArray(value.fileKeepInfo.arkUIKeepInfo.globalNames),
          },
        },
        fileReservedInfo: {
          enumProperties: setToArray(value.fileReservedInfo.enumProperties),
          propertyParams: setToArray(value.fileReservedInfo.propertyParams),
        },
      };
      if (value.bytecodeObfuscateKeepInfo?.decoratorMap) {
        jsonData[key].bytecodeObfuscateKeepInfo = {
          decoratorMap: value.bytecodeObfuscateKeepInfo.decoratorMap,
        };
      }
    }

    const jsonString = JSON.stringify(jsonData, null, 2);
    fs.writeFileSync(filePath, jsonString, 'utf8');
  }

  private readProjectWhiteList(filePath: string): ProjectWhiteList {
    const fileContent = fs.readFileSync(filePath, 'utf8');
    const parsed: ProjectWhiteListJsonData = JSON.parse(fileContent);

    const projectKeepInfo: ProjectKeepInfo = {
      propertyNames: arrayToSet(parsed.projectKeepInfo.propertyNames),
      globalNames: arrayToSet(parsed.projectKeepInfo.globalNames),
    };

    const projectReservedInfo: ProjectReservedInfo = {
      enumProperties: arrayToSet(parsed.projectReservedInfo.enumProperties),
      propertyParams: arrayToSet(parsed.projectReservedInfo.propertyParams),
    };

    return {
      projectKeepInfo,
      projectReservedInfo,
    };
  }

  private writeProjectWhiteList(filePath: string, projectWhiteList: ProjectWhiteList): void {
    const jsonData: ProjectWhiteListJsonData = {
      projectKeepInfo: {
        propertyNames: setToArray(projectWhiteList.projectKeepInfo.propertyNames),
        globalNames: setToArray(projectWhiteList.projectKeepInfo.globalNames),
      },
      projectReservedInfo: {
        enumProperties: setToArray(projectWhiteList.projectReservedInfo.enumProperties),
        propertyParams: setToArray(projectWhiteList.projectReservedInfo.propertyParams),
      },
    };

    const jsonString = JSON.stringify(jsonData, null, 2);
    fs.writeFileSync(filePath, jsonString, 'utf8');
  }

  public updateFileWhiteListMap(deletedFilePath?: Set<string>): void {
    const lastFileWhiteLists: Map<string, FileWhiteList> = this.readFileWhiteLists(this.fileWhiteListsCachePath);

    deletedFilePath?.forEach((path) => {
      lastFileWhiteLists.delete(path);
    });
    this.fileWhiteListMap.forEach((value, key) => {
      lastFileWhiteLists.set(key, value);
    });
    this.writeFileWhiteLists(this.fileWhiteListsCachePath, lastFileWhiteLists);
    this.fileWhiteListMap = lastFileWhiteLists;
  }

  public createProjectWhiteList(fileWhiteLists: Map<string, FileWhiteList>): ProjectWhiteList {
    const projectKeepInfo: ProjectKeepInfo = {
      propertyNames: new Set(),
      globalNames: new Set(),
    };

    const projectReservedInfo: ProjectReservedInfo = {
      enumProperties: new Set(),
      propertyParams: new Set(),
    };

    fileWhiteLists.forEach((fileWhiteList) => {
      // 1. Collect fileKeepInfo
      // Collect keepSymbol
      fileWhiteList.fileKeepInfo.keepSymbol?.globalNames.forEach((globalName) => {
        projectKeepInfo.globalNames.add(globalName);
      });
      fileWhiteList.fileKeepInfo.keepSymbol?.propertyNames.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect keepAsConsumer
      fileWhiteList.fileKeepInfo.keepAsConsumer?.globalNames.forEach((globalName) => {
        projectKeepInfo.globalNames.add(globalName);
      });
      fileWhiteList.fileKeepInfo.keepAsConsumer?.propertyNames.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect structProperties
      fileWhiteList.fileKeepInfo.structProperties.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect exportedNames
      fileWhiteList.fileKeepInfo.exported.globalNames.forEach((globalName) => {
        projectKeepInfo.globalNames.add(globalName);
      });
      fileWhiteList.fileKeepInfo.exported.propertyNames.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect enumProperties
      fileWhiteList.fileKeepInfo.enumProperties.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect stringProperties
      fileWhiteList.fileKeepInfo.stringProperties.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
      });

      // Collect arkUIKeepInfo
      fileWhiteList.fileKeepInfo.arkUIKeepInfo.globalNames.forEach((globalName) => {
        projectKeepInfo.globalNames.add(globalName);
        AtIntentCollections.globalNames.add(globalName);
      });
      fileWhiteList.fileKeepInfo.arkUIKeepInfo.propertyNames.forEach((propertyName) => {
        projectKeepInfo.propertyNames.add(propertyName);
        AtIntentCollections.propertyNames.add(propertyName);
      });

      // 2. Collect fileReservedInfo
      // Collect enumProperties
      fileWhiteList.fileReservedInfo.enumProperties.forEach((enumPropertyName) => {
        projectReservedInfo.enumProperties.add(enumPropertyName);
      });

      // Collect propertyParams
      fileWhiteList.fileReservedInfo.propertyParams.forEach((propertyParam) => {
        projectReservedInfo.propertyParams.add(propertyParam);
      });

      const decoratorMap = fileWhiteList.bytecodeObfuscateKeepInfo?.decoratorMap;
      for (const key in decoratorMap) {
        if (Object.prototype.hasOwnProperty.call(decoratorMap, key)) {
          decoratorMap[key]?.forEach(item => projectKeepInfo.globalNames.add(item));
        }
      }
    });

    const projectWhiteList = {
      projectKeepInfo: projectKeepInfo,
      projectReservedInfo: projectReservedInfo,
    };

    return projectWhiteList;
  }

  // Determine if the project's white list has been updated.
  private areProjectWhiteListsEqual(whiteList1: ProjectWhiteList, whiteList2: ProjectWhiteList): boolean {
    const projectKeepInfoEqual =
      areSetsEqual(whiteList1.projectKeepInfo.propertyNames, whiteList2.projectKeepInfo.propertyNames) &&
      areSetsEqual(whiteList1.projectKeepInfo.globalNames, whiteList2.projectKeepInfo.globalNames);

    if (!projectKeepInfoEqual) {
      return false;
    }

    const projectReservedInfoEqual =
      areSetsEqual(whiteList1.projectReservedInfo.enumProperties, whiteList2.projectReservedInfo.enumProperties) &&
      areSetsEqual(whiteList1.projectReservedInfo.propertyParams, whiteList2.projectReservedInfo.propertyParams);

    return projectReservedInfoEqual;
  }

  // We only scan updated files or newly created files in incremental compliation,
  // so we need to update the UnobfuscationCollections and reserved names use all file white lists.
  // This should be called in incremental compliation after we updated file white list.
  private updateUnobfuscationCollections(): void {
    this.fileWhiteListMap.forEach((fileWhiteList) => {
      if (this.enableAtKeep) {
        addToSet(AtKeepCollections.keepSymbol.propertyNames, fileWhiteList.fileKeepInfo.keepSymbol.propertyNames);
        addToSet(AtKeepCollections.keepSymbol.globalNames, fileWhiteList.fileKeepInfo.keepSymbol.globalNames);
        addToSet(AtKeepCollections.keepAsConsumer.propertyNames, fileWhiteList.fileKeepInfo.keepAsConsumer.propertyNames);
        addToSet(AtKeepCollections.keepAsConsumer.globalNames, fileWhiteList.fileKeepInfo.keepAsConsumer.globalNames);
      }
      addToSet(UnobfuscationCollections.reservedStruct, fileWhiteList.fileKeepInfo.structProperties);
      addToSet(UnobfuscationCollections.reservedEnum, fileWhiteList.fileKeepInfo.enumProperties);
      addToSet(UnobfuscationCollections.reservedExportName, fileWhiteList.fileKeepInfo.exported.globalNames);
      addToSet(UnobfuscationCollections.reservedExportNameAndProp, fileWhiteList.fileKeepInfo.exported.propertyNames);
      addToSet(UnobfuscationCollections.reservedStrProp, fileWhiteList.fileKeepInfo.stringProperties);
      addToSet(AtIntentCollections.propertyNames, fileWhiteList.fileKeepInfo.arkUIKeepInfo.propertyNames);
      addToSet(AtIntentCollections.globalNames, fileWhiteList.fileKeepInfo.arkUIKeepInfo.globalNames);
      addToSet(ApiExtractor.mConstructorPropertySet, fileWhiteList.fileReservedInfo.propertyParams);
      addToSet(ApiExtractor.mEnumMemberSet, fileWhiteList.fileReservedInfo.enumProperties);
      const decoratorMap = fileWhiteList.bytecodeObfuscateKeepInfo?.decoratorMap;
      for (const key in decoratorMap) {
        if (Object.prototype.hasOwnProperty.call(decoratorMap, key)) {
          decoratorMap[key]?.forEach(item => BytecodeObfuscationCollections.decoratorProp.add(item));
        }
      }
    });
  }

  private updateProjectWhiteList(): ProjectWhiteList {
    const lastProjectWhiteList: ProjectWhiteList = this.readProjectWhiteList(this.projectWhiteListCachePath);
    const newestProjectWhiteList: ProjectWhiteList = this.createProjectWhiteList(this.fileWhiteListMap);
    if (!this.areProjectWhiteListsEqual(lastProjectWhiteList, newestProjectWhiteList)) {
      this.writeProjectWhiteList(this.projectWhiteListCachePath, newestProjectWhiteList);
      this.shouldReObfuscate = true;
    }
    return newestProjectWhiteList;
  }

  public createOrUpdateWhiteListCaches(deletedFilePath?: Set<string>): void {
    let projectWhiteList: ProjectWhiteList;
    if (!this.isIncremental) {
      this.writeFileWhiteLists(this.fileWhiteListsCachePath, this.fileWhiteListMap);
      projectWhiteList = this.createProjectWhiteList(this.fileWhiteListMap);
      this.writeProjectWhiteList(this.projectWhiteListCachePath, projectWhiteList);
    } else {
      this.updateFileWhiteListMap(deletedFilePath);
      projectWhiteList = this.updateProjectWhiteList();
      this.updateUnobfuscationCollections();
    }
    this.fileWhiteListMap.clear();
  }
}

/**
 * This class is used to manage sourceFilePath.json file.
 * Will be created when initialize arkObfuscator.
 */
export class FilePathManager {
  // Stores all source files paths
  private sourceFilePaths: Set<string>;

  // Files deleted in incremental build
  private deletedSourceFilePaths: Set<string>;

  // Files added in incremental build
  private addedSourceFilePaths: Set<string>;

  // Cache path of sourceFilePaths.cache file
  private filePathsCache: string;

  public getSourceFilePaths(): Set<string> {
    return this.sourceFilePaths;
  }

  public getDeletedSourceFilePaths(): Set<string> {
    return this.deletedSourceFilePaths;
  }

  public getAddedSourceFilePaths(): Set<string> {
    return this.addedSourceFilePaths;
  }

  public getFilePathsCache(): string {
    return this.filePathsCache;
  }

  constructor(cachePath: string) {
    this.filePathsCache = path.join(cachePath, SOURCE_FILE_PATHS);
    this.sourceFilePaths = new Set();
    this.deletedSourceFilePaths = new Set();
    this.addedSourceFilePaths = new Set();
  }

  private setSourceFilePaths(sourceFilePaths: Set<string>): void {
    sourceFilePaths.forEach((filePath) => {
      if (!FileUtils.isReadableFile(filePath) || !ApiExtractor.isParsableFile(filePath)) {
        return;
      }
      this.sourceFilePaths.add(filePath);
    });
  }

  private writeSourceFilePaths(): void {
    const content = Array.from(this.sourceFilePaths).join('\n');
    FileUtils.writeFile(this.filePathsCache, content);
  }

  // Update sourceFilePaths.cache and get deleted file list and added file list
  private updateSourceFilePaths(): void {
    const cacheContent = FileUtils.readFile(this.filePathsCache).split('\n');
    const cacheSet = new Set(cacheContent);

    for (const path of cacheSet) {
      if (!this.sourceFilePaths.has(path)) {
        this.deletedSourceFilePaths.add(path);
      }
    }

    for (const path of this.sourceFilePaths) {
      if (!cacheSet.has(path)) {
        this.addedSourceFilePaths.add(path);
      }
    }

    this.writeSourceFilePaths();
  }

  // Create or update sourceFilePaths.cache
  public createOrUpdateSourceFilePaths(sourceFilePaths: Set<string>): void {
    this.setSourceFilePaths(sourceFilePaths);
    if (this.isIncremental()) {
      this.updateSourceFilePaths();
    } else {
      this.writeSourceFilePaths();
    }
  }

  public isIncremental(): boolean {
    return fs.existsSync(this.filePathsCache);
  }
}

/**
 * This class is used to manage transfromed file content.
 * Will be created when initialize arkObfuscator.
 */
export class FileContentManager {
  // Path that stores all transfromed file content
  private transformedFilesDir: string;

  // Path of fileNamesMap
  private fileNamesMapPath: string;

  // Stores [originpath -> transformed cache path]
  public fileNamesMap: Map<string, string>;

  // If it is incremental compilation
  private isIncremental: boolean;

  public getTransformedFilesDir(): string {
    return this.transformedFilesDir;
  }

  public getFileNamesMapPath(): string {
    return this.fileNamesMapPath;
  }

  public getIsIncremental(): boolean {
    return this.isIncremental;
  }

  constructor(cachePath: string, isIncremental: boolean) {
    this.transformedFilesDir = path.join(cachePath, TRANSFORMED_PATH);
    this.fileNamesMapPath = path.join(this.transformedFilesDir, FILE_NAMES_MAP);
    this.fileNamesMap = new Map();
    this.isIncremental = isIncremental;
    FileUtils.createDirectory(this.transformedFilesDir);
  }

  // Generate hash from filePah
  private generatePathHash(filePath: string): string {
    const hash = crypto.createHash('md5');
    hash.update(filePath);
    return hash.digest('hex').slice(0, 16);
  }

  // Generate new file name for transfromed sourcefiles.
  // Generated from origin filePath's hash & time stamp.
  private generateFileName(filePath: string): string {
    const hash = this.generatePathHash(filePath);
    const timestamp = Date.now().toString();
    return `${hash}_${timestamp}`;
  }

  // Delete a set of files, used in incremental compliation if we have deleted some files
  public deleteFileContent(deletedSourceFilePaths: Set<string>): void {
    deletedSourceFilePaths.forEach((filePath) => {
      this.deleteTransformedFile(filePath);
    });
  }

  // Delete transfromed file
  private deleteTransformedFile(deletedSourceFilePath: string): void {
    if (this.fileNamesMap.has(deletedSourceFilePath)) {
      const transformedFilePath: string = this.fileNamesMap.get(deletedSourceFilePath);
      this.fileNamesMap.delete(deletedSourceFilePath);
      FileUtils.deleteFile(transformedFilePath);
    }
  }

  public readFileNamesMap(): void {
    const jsonObject = FileUtils.readFileAsJson(this.fileNamesMapPath);
    this.fileNamesMap = new Map<string, string>(Object.entries(jsonObject));
  }

  public writeFileNamesMap(): void {
    const jsonObject = Object.fromEntries(this.fileNamesMap.entries());
    const jsonString = JSON.stringify(jsonObject, null, 2);
    FileUtils.writeFile(this.fileNamesMapPath, jsonString);
    this.fileNamesMap.clear();
  }

  public readFileContent(transformedFilePath: string): FileContent {
    const fileContentJson = FileUtils.readFile(transformedFilePath);
    const fileConstent: FileContent = JSON.parse(fileContentJson);
    return fileConstent;
  }

  private writeFileContent(filePath: string, fileContent: FileContent): void {
    const jsonString = JSON.stringify(fileContent, null, 2);
    FileUtils.writeFile(filePath, jsonString);
  }

  // Update file content for newly created files or updated files
  public updateFileContent(fileContent: FileContent): void {
    const originPath = FileUtils.toUnixPath(fileContent.moduleInfo.originSourceFilePath);
    if (this.isIncremental) {
      this.deleteTransformedFile(originPath);
    }
    const fileName = this.generateFileName(originPath);
    const transformedFilePath = path.join(this.transformedFilesDir, fileName);
    this.fileNamesMap.set(originPath, transformedFilePath);
    this.writeFileContent(transformedFilePath, fileContent);
  }

  public getSortedFiles(): string[] {
    return (this.fileNamesMap && Array.from(this.fileNamesMap.keys())).sort((a, b) => a.localeCompare(b)) || [];
  }
}
