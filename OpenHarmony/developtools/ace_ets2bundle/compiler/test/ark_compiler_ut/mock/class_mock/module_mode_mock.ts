/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import fs from 'fs';
import path from 'path';
import {
  COMMONJS,
  ESM,
  EXTNAME_PROTO_BIN,
  EXTNAME_JS,
  EXTNAME_TS,
  EXTNAME_ETS,
  EXTNAME_MJS,
  EXTNAME_CJS
} from '../../../../lib/fast_build/ark_compiler/common/ark_define';
import {
  ModuleMode,
  PackageEntryInfo
} from '../../../../lib/fast_build/ark_compiler/module/module_mode';
import { getNormalizedOhmUrlByFilepath } from '../../../../lib/ark_utils';
import {
  changeFileExtension,
  compilingEtsOrTsFiles,
  shouldETSOrTSFileTransformToJS
} from '../../../../lib/fast_build/ark_compiler/utils';
import { toUnixPath } from '../../../../lib/utils';
import { META } from '../rollup_mock/common';
import { sharedModuleSet } from '../../../../lib/fast_build/ark_compiler/check_shared_module';
import { SourceMapGenerator } from '../../../../lib/fast_build/ark_compiler/generate_sourcemap';
import { CompileEvent } from '../../../../lib/performance';
class ModuleModeMock extends ModuleMode {
  collectModuleFileListMock(rollupObject: object) {
    const fileList = Array.from(rollupObject.getModuleIds());
    this.collectModuleFileList(rollupObject, fileList);
  }

  addModuleInfoItemMock(rollupObject: object) {
    const mockFileList = rollupObject.getModuleIds();
    for (const filePath of mockFileList) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_ETS) || filePath.endsWith(EXTNAME_JS)) {
        const moduleInfo: object = rollupObject.getModuleInfo(filePath);
        const metaInfo: object = moduleInfo[META];
        compilingEtsOrTsFiles.push(filePath);
        this.processModuleInfos(filePath, this.moduleInfos, metaInfo);
      }
    }
  }

  addSourceMapMock(rollupObject: object, sourceMapGenerator: SourceMapGenerator) {
    for (const filePath of rollupObject.getModuleIds()) {
      
      const isValidSuffix: boolean =
        filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_ETS) || filePath.endsWith(EXTNAME_JS);
      if (!isValidSuffix)
        continue;
      const moduleInfo: object = rollupObject.getModuleInfo(filePath);
      const metaInfo: object = moduleInfo[META];
      compilingEtsOrTsFiles.push(filePath);
      if (sourceMapGenerator.isNewSourceMaps()) {
        sourceMapGenerator.updateSourceMap(filePath, {});
      } else {
        let filePathCache: string = this.genFileCachePath(
          filePath, this.projectConfig.projectRootPath, this.projectConfig.cachePath);
        const extName: string = this.getExtName(filePath, metaInfo);
        if (extName) {
          filePathCache = changeFileExtension(filePathCache, this.getExtName(filePath, metaInfo));
        }
        sourceMapGenerator.updateSourceMap(
          filePathCache.replace(this.projectConfig.projectRootPath + path.sep, ''), {});
      }
    }
  }

  static getModuleInfosAndSourceMapMock(rollupObject: object, sourceMapGenerator: SourceMapGenerator) {
    const moduleMode = new ModuleModeMock(rollupObject);
    moduleMode.addSourceMapMock(rollupObject, sourceMapGenerator);
    moduleMode.addModuleInfoItemMock(rollupObject);
    return { moduleInfos: moduleMode.moduleInfos, sourceMap: sourceMapGenerator.getSourceMaps() };
  }

  getExtName(moduleId: string, metaInfo: object): string {
    switch (path.extname(moduleId)) {
      case EXTNAME_ETS: {
        return shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : EXTNAME_TS;
      }
      case EXTNAME_TS: {
        return shouldETSOrTSFileTransformToJS(moduleId, this.projectConfig, metaInfo) ? EXTNAME_JS : '';
      }
      case EXTNAME_JS:
      case EXTNAME_MJS:
      case EXTNAME_CJS: {
        return (moduleId.endsWith(EXTNAME_MJS) || moduleId.endsWith(EXTNAME_CJS)) ? EXTNAME_JS : '';
      }
      default:
        return ''
    }
  }

  generateCompileFilesInfoMock(includeByteCodeHarInfo: boolean) {
    this.generateCompileFilesInfo(includeByteCodeHarInfo);
  }

  generateNpmEntriesInfoMock() {
    this.generateNpmEntriesInfo();
  }

  generateAbcCacheFilesInfoMock() {
    this.generateAbcCacheFilesInfo();
  }

  generateCompileContextInfoMock(rollupObject: object): void {
      this.compileContextInfoPath = this.generateCompileContextInfo(rollupObject);
  }

  checkGenerateCompileContextInfo(rollupObject: object): boolean {
    const cacheCompileContextInfo = fs.readFileSync(this.compileContextInfoPath, 'utf-8');

    let compileContextInfo: Object = {};
    let hspPkgNames: Array<string> = [];
    for (const hspName in rollupObject.share.projectConfig.hspNameOhmMap) {
      let hspPkgName: string = hspName;
      if (rollupObject.share.projectConfig.dependencyAliasMap.has(hspName)) {
        hspPkgName = rollupObject.share.projectConfig.dependencyAliasMap.get(hspName);
      }
      hspPkgNames.push(toUnixPath(hspPkgName));
    }
    compileContextInfo.hspPkgNames = hspPkgNames;
    let compileEntries: Set<string> = new Set();
    let entryObj: Object = this.projectConfig.entryObj;
    if (this.projectConfig.widgetCompile) {
      entryObj = this.projectConfig.cardEntryObj;
    }
    for (const key in entryObj) {
      let moduleId: string = entryObj[key];
      let moduleInfo: Object = rollupObject.getModuleInfo(moduleId);
      if (!moduleInfo) {
        this.throwArkTsCompilerError(red, `ArkTS:INTERNAL ERROR: Failed to find module info.\n` +
          `Error Message: Failed to find module info with '${moduleId}' from the context information.`, reset);
      }
      let metaInfo: Object = moduleInfo.meta;
      if (!metaInfo) {
        this.throwArkTsCompilerError(red, `ArkTS:INTERNAL ERROR: Failed to find meta info.\n` +
          `Error Message: Failed to find meta info with '${moduleId}' from the module info.`, reset);
      }
      const pkgParams = {
        pkgName: metaInfo.pkgName,
        pkgPath: metaInfo.pkgPath,
        isRecordName: true
      };
      let recordName: string = getNormalizedOhmUrlByFilepath(moduleId, rollupObject.share.projectConfig,
        rollupObject.share.logger, pkgParams, undefined);
      compileEntries.add(recordName);
    }
    this.collectDeclarationFilesEntry(compileEntries, hspPkgNames);
    compileContextInfo.compileEntries = Array.from(compileEntries);
    if (rollupObject.share.projectConfig.updateVersionInfo) {
      compileContextInfo.updateVersionInfo = this.projectConfig.updateVersionInfo;
    } else if (rollupObject.share.projectConfig.pkgContextInfo) {
      compileContextInfo.pkgContextInfo = this.projectConfig.pkgContextInfo;
    }
    if (this.projectConfig.bundleType === 'shared') {
      compileContextInfo.needModifyRecord = true;
      compileContextInfo.bundleName = this.projectConfig.bundleName;
    }
    if (JSON.stringify(compileContextInfo) === cacheCompileContextInfo) {
      return true;
    }
    return false;
  }

  checkGenerateCompileFilesInfo(includeByteCodeHarInfo: boolean): boolean {
    let mockfilesInfo: string = '';
    const filesInfo = fs.readFileSync(this.filesInfoPath, 'utf-8');
    this.moduleInfos.forEach((info) => {
      const moduleType: string = info.isCommonJs ? COMMONJS : ESM;
      const isSharedModule: boolean = sharedModuleSet.has(info.filePath);
      mockfilesInfo +=
        `${info.cacheFilePath};${info.recordName};${moduleType};${info.sourceFile};${info.packageName};` +
        `${isSharedModule};${info.originSourceLang}\n`;
    });
    if (includeByteCodeHarInfo) {
      Object.entries(this.projectConfig.byteCodeHarInfo).forEach(([pkgName, abcInfo]) => {  
        const abcPath: string = toUnixPath(abcInfo.abcPath);
        mockfilesInfo += `${abcPath};;;;${pkgName};\n`; 
      });
    }
    if (filesInfo === mockfilesInfo) {
      return true;
    }
    return false;
  }

  checkGenerateNpmEntriesInfo(): boolean {
    let mockentriesInfo: string = '';
    const filesInfo = fs.readFileSync(this.npmEntriesInfoPath, 'utf-8');
    for (const value of this.pkgEntryInfos.values()) {
      mockentriesInfo += `${value.pkgEntryPath}:${value.pkgBuildPath}\n`;
    }
    if (filesInfo === mockentriesInfo) {
      return true;
    }
    return false;
  }

  checkGenerateAbcCacheFilesInfo(): boolean {
    let mockabcCacheFilesInfo: string = '';
    const filesInfo = fs.readFileSync(this.cacheFilePath, 'utf-8');
    this.moduleInfos.forEach((info) => {
      const abcCacheFilePath: string = changeFileExtension(info.cacheFilePath, EXTNAME_PROTO_BIN);
      mockabcCacheFilesInfo += `${info.cacheFilePath};${abcCacheFilePath}\n`;
    });

    const npmEntriesCacheFilePath: string = changeFileExtension(this.npmEntriesInfoPath, EXTNAME_PROTO_BIN);
    mockabcCacheFilesInfo += `${this.npmEntriesInfoPath};${npmEntriesCacheFilePath}\n`;
    this.abcPaths.forEach((abcPath) => {
      let abcCacheFilePath: string = changeFileExtension(abcPath, EXTNAME_PROTO_BIN);
      mockabcCacheFilesInfo += `${abcPath};${abcCacheFilePath}\n`;
    });
    if (filesInfo === mockabcCacheFilesInfo) {
      return true;
    }
    return false;
  }

  checkGetPackageEntryInfo(rollup: object, isTestErrorLog: boolean = false) {
    this.pkgEntryInfos = new Map<String, PackageEntryInfo>();
    const mockFileList = rollup.getModuleIds();
    for (const filePath of mockFileList) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_ETS) || filePath.endsWith(EXTNAME_JS)) {
        const moduleInfos = rollup.getModuleInfo(filePath);
        moduleInfos.setIsLocalDependency(false);
        moduleInfos.setIsNodeEntryFile(true);
        const metaInfo: object = moduleInfos[META];
        if (isTestErrorLog) {
          metaInfo['pkgPath'] = '';
        }
        this.getPackageEntryInfo(filePath, metaInfo, this.pkgEntryInfos);
      }
    }
  }

  updateCachedSourceMapsMock(sourceMapGenerator: Object) {
    sourceMapGenerator.updateCachedSourceMaps();
  }

  buildModuleSourceMapInfoMock(sourceMapGenerator: Object) {
    sourceMapGenerator.buildModuleSourceMapInfo();
  }

  checkModuleSourceMapInfoMock(): boolean {
    let readSourceMap: string = '';
    if (fs.existsSync(this.sourceMapPath)) {
      readSourceMap = fs.readFileSync(this.sourceMapPath, 'utf-8');
    }
    let readCacheSourceMap: string = '';
    if (fs.existsSync(this.cacheSourceMapPath)) {
      readCacheSourceMap = fs.readFileSync(this.cacheSourceMapPath, 'utf-8');
    }

    if (readSourceMap.length == 0 && readCacheSourceMap.length == 0) {
      return true;
    } else {
      const sourcemapObj: Object = JSON.parse(readSourceMap);
      const cacheArrayStr: string = '[' + readCacheSourceMap.replace(/\n/g, ',') + ']';
      const cacheArray: Array<Object> = JSON.parse(cacheArrayStr);
      for (const item of cacheArray) {
        if (!sourcemapObj.hasOwnProperty(item.key)) {
          return false;
        }
        if (JSON.stringify(sourcemapObj[item.key]) != JSON.stringify(item.val)) {
          return false;
        }
      }
      return true;
    }
  }

  generateMergedAbcOfEs2AbcMock(parentEvent: CompileEvent) {
    this.generateMergedAbcOfEs2Abc(parentEvent)
  }

  filterModulesByHashJsonMock() {
    this.filterModulesByHashJson();
  }

  invokeTs2AbcWorkersToGenProtoMock(splittedModules: Object) {
    this.invokeTs2AbcWorkersToGenProto(splittedModules)
  }

  
}

export default ModuleModeMock;