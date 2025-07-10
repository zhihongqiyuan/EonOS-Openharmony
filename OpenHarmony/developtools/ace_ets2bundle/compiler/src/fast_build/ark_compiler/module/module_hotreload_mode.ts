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

import fs from 'fs';
import path from 'path';

import { ModuleMode } from './module_mode';
import {
  blue,
  reset,
  MODULES_ABC,
  SOURCEMAPS,
  SYMBOLMAP
} from '../common/ark_define';
import { isJsonSourceFile } from '../utils';
import {
  mkdirsSync,
  toUnixPath,
  validateFilePathLength
} from '../../../utils';
import { SourceMapGenerator } from '../generate_sourcemap';
import { 
  ArkTSInternalErrorDescription,
  ErrorCode 
} from '../error_code';
import {
  LogData,
  LogDataFactory
} from '../logger';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent
} from '../../../performance';

let isFirstBuild: boolean = true;

export class ModuleHotreloadMode extends ModuleMode {
  symbolMapFilePath: string;
  constructor(rollupObject: Object) {
    super(rollupObject);
    if (this.projectConfig.oldMapFilePath) {
      this.symbolMapFilePath = path.join(this.projectConfig.oldMapFilePath, SYMBOLMAP);
    } else {
      const errInfo: LogData = LogDataFactory.newInstance(
        ErrorCode.ETS2BUNDLE_INTERNAL_HOT_RELOAD_FAILED_INCORRECT_SYMBOL_MAP_CONFIG,
        ArkTSInternalErrorDescription,
        'Hot Reload failed, symbolMap file is not correctly configured.'
      );
      this.logger.printErrorAndExit(errInfo);
    }
  }

  generateAbc(rollupObject: Object, parentEvent: CompileEvent): void {
    // To support hotreload of multiple HSP modules, rollup no longer runs in watch mode. 
    // In this case isFirstBuild needs to be passed in by hvigor, because if multiple HSP 
    // module build tasks are running in the same worker in the IDE, the isFirstBuild of 
    // the later build task will be overwritten by the first build task to false.
    if (rollupObject.share.projectConfig.watchMode !== 'true') {
      isFirstBuild = this.projectConfig.isFirstBuild;
    }
    if (isFirstBuild) {
      this.compileAllFiles(rollupObject, parentEvent);
      if (rollupObject.share.projectConfig.watchMode === 'true') {
        isFirstBuild = false;
      }
    } else {
      this.compileChangeListFiles(rollupObject, parentEvent);
    }
  }

  addHotReloadArgs(): void {
    if (isFirstBuild) {
      this.cmdArgs.push('--dump-symbol-table');
      this.cmdArgs.push(`"${this.symbolMapFilePath}"`);
      return;
    }
    this.addCacheFileArgs();
    this.cmdArgs.push('--input-symbol-table');
    this.cmdArgs.push(`"${this.symbolMapFilePath}"`);
    this.cmdArgs.push('--hot-reload');
  }

  private compileAllFiles(rollupObject: Object, parentEvent: CompileEvent): void {
    this.prepareForCompilation(rollupObject, parentEvent);
    SourceMapGenerator.getInstance().buildModuleSourceMapInfo(parentEvent);
    this.generateAbcByEs2abc(parentEvent, !!this.projectConfig.byteCodeHarInfo);
  }

  private compileChangeListFiles(rollupObject: Object, parentEvent: CompileEvent): void {
    if (!fs.existsSync(this.projectConfig.changedFileList)) {
      this.logger.debug(blue, `ArkTS: Cannot find file: ${
        this.projectConfig.changedFileList}, skip hot reload build`, reset);
      return;
    }

    const changedFileListJson: string = fs.readFileSync(this.projectConfig.changedFileList).toString();
    const {
      changedFileListVersion,
      changedFileList
    } = this.parseChangedFileListJson(changedFileListJson);
    if (typeof changedFileList === 'undefined' || changedFileList.length === 0) {
      this.logger.debug(blue, `ArkTS: No changed files found, skip hot reload build`, reset);
      return;
    }

    let needHotreloadBuild: boolean = true;
    let changedFileListInAbsolutePath: Array<string> = changedFileList.map((file) => {
      if (isJsonSourceFile(file)) {
        this.logger.debug(blue, `ARKTS: json source file: ${file} changed, skip hot reload build!`, reset);
        needHotreloadBuild = false;
      }
      return changedFileListVersion === 'v1' ? path.join(this.projectConfig.projectPath, file) : file;
    });

    if (!needHotreloadBuild) {
      return;
    }

    // The process of '--remove-redundant-file' use filesInfos.txt and compileContextInfo.json to get compile list.
    // In non first build hot reload, filesInfos.txt only contian modified files, could cause loss of compile list.
    // When useNormalizedOHMUrl is true, project could import from bytecode har, it will cause crash.
    // So recreate the entry list for compileContextInfo.json in non first build.
    if (this.useNormalizedOHMUrl) {
      this.projectConfig.entryObj = changedFileListInAbsolutePath;
      this.compileContextInfoPath = this.generateCompileContextInfo(rollupObject);
    }

    const eventCollectModuleFileList = createAndStartEvent(parentEvent, 'collect module file list');
    this.collectModuleFileList(rollupObject, changedFileListInAbsolutePath[Symbol.iterator]());
    stopEvent(eventCollectModuleFileList);

    if (!fs.existsSync(this.projectConfig.patchAbcPath)) {
      mkdirsSync(this.projectConfig.patchAbcPath);
    }

    this.updateSourceMapFromFileList(
      SourceMapGenerator.getInstance().isNewSourceMaps() ? changedFileListInAbsolutePath : changedFileList,
      parentEvent);
    const outputABCPath: string = path.join(this.projectConfig.patchAbcPath, MODULES_ABC);
    validateFilePathLength(outputABCPath, this.logger);
    this.moduleAbcPath = outputABCPath;
    // During incremental compilation, the bytecode har path must be blocked from being written to filesInfo.
    this.generateAbcByEs2abc(parentEvent, false);
  }

  private parseChangedFileListJson(changedFileListJson: string): object {
    const changedFileList = JSON.parse(changedFileListJson);
    if (Object.prototype.hasOwnProperty.call(changedFileList, 'modifiedFilesV2')) {
      return {
        'changedFileListVersion': 'v2',
        'changedFileList': changedFileList.modifiedFilesV2
          .filter(file => Object.prototype.hasOwnProperty.call(file, 'filePath'))
          .map(file => file.filePath)
      };
    } else if (Object.prototype.hasOwnProperty.call(changedFileList, 'modifiedFiles')) {
      return {
        'changedFileListVersion': 'v1',
        'changedFileList': changedFileList.modifiedFiles
      };
    } else {
      return {
        'changedFileListVersion': '',
        'changedFileList': []
      };
    }
  }

  private updateSourceMapFromFileList(fileList: Array<string>, parentEvent: CompileEvent): void {
    const eventUpdateSourceMapFromFileList = createAndStartEvent(parentEvent, 'update source map from file list');
    const sourceMapGenerator = SourceMapGenerator.getInstance();
    const relativeProjectPath: string = this.projectConfig.projectPath.slice(
      this.projectConfig.projectRootPath.length + path.sep.length);
    let hotReloadSourceMap: Object = {};
    for (const file of fileList) {
      if (sourceMapGenerator.isNewSourceMaps()) {
        validateFilePathLength(file, this.logger);
        hotReloadSourceMap[sourceMapGenerator.genKey(file)] = sourceMapGenerator.getSourceMap(file);
      } else {
        const sourceMapPath: string = toUnixPath(path.join(relativeProjectPath, file));
        validateFilePathLength(sourceMapPath, this.logger);
        hotReloadSourceMap[sourceMapPath] = sourceMapGenerator.getSourceMap(sourceMapPath);
      }
    }
    if (!sourceMapGenerator.isNewSourceMaps()) {
      sourceMapGenerator.modifySourceMapKeyToCachePath(hotReloadSourceMap);
    }
    const sourceMapFilePath: string = path.join(this.projectConfig.patchAbcPath, SOURCEMAPS);
    validateFilePathLength(sourceMapFilePath, this.logger);
    fs.writeFileSync(sourceMapFilePath,
      JSON.stringify(hotReloadSourceMap, null, 2), 'utf-8');
    stopEvent(eventUpdateSourceMapFromFileList);
  }

  private generateAbcByEs2abc(parentEvent: CompileEvent, includeByteCodeHarInfo: boolean): void {
    this.generateEs2AbcCmd();
    this.addHotReloadArgs();
    this.genDescriptionsForMergedEs2abc(includeByteCodeHarInfo, parentEvent);
    this.generateMergedAbcOfEs2Abc(parentEvent);
  }
}
