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

import {
  ESMODULE,
  GEN_ABC_PLUGIN_NAME
} from './common/ark_define';
import { ModuleSourceFile } from './module/module_source_file';
import {
  isAotMode,
  isCommonJsPluginVirtualFile,
  isDebug,
  isJsonSourceFile,
  isJsSourceFile,
  isTsOrEtsSourceFile,
  shouldETSOrTSFileTransformToJS
} from './utils';
import {
  toUnixPath,
  emitLogInfo,
  getTransformLog
} from '../../utils';
import {
  resetReExportCheckLog,
  reExportNoCheckMode,
  processJsCodeLazyImport,
  reExportCheckLog,
  LazyImportOptions
} from '../../process_lazy_import';
import { SourceMapGenerator } from './generate_sourcemap';
import { MemoryMonitor } from '../meomry_monitor/rollup-plugin-memory-monitor';
import { MemoryDefine } from '../meomry_monitor/memory_define';
import {
  getHookEventFactory,
  createAndStartEvent,
  stopEvent,
  CompileEvent
} from '../../performance';

/**
 * rollup transform hook
 * @param {string} code: transformed source code of an input file
 * @param {string} id: absolute path of an input file
 */
export function transformForModule(code: string, id: string): string {
  const hookEventFactory: CompileEvent | undefined = getHookEventFactory(this.share, 'genAbc', 'transform');
  const eventTransformForModule = createAndStartEvent(hookEventFactory, 'transform for module');
  const {
    autoLazyImport,
    reExportCheckMode
  }: LazyImportOptions = {
    autoLazyImport: this.share.projectConfig?.autoLazyImport ?? false,
    reExportCheckMode: this.share.projectConfig?.reExportCheckMode ?? reExportNoCheckMode
  };
  if (this.share.projectConfig.compileMode === ESMODULE) {
    const metaInfo: Object = this.getModuleInfo(id).meta;
    const projectConfig: Object = Object.assign(this.share.arkProjectConfig, this.share.projectConfig);
    if (isTsOrEtsSourceFile(id) && shouldETSOrTSFileTransformToJS(id, projectConfig, metaInfo)) {
      preserveSourceMap(id, this.getCombinedSourcemap(), projectConfig, metaInfo, eventTransformForModule);
      // when ets/ts -> js, we need to convert lazy-import based on the js code generated after tsc conversion
      code = processJsCodeLazyImport(id, code, autoLazyImport, reExportCheckMode);
      const newSourceFileRecordInfo = MemoryMonitor.recordStage(MemoryDefine.MODULE_SOURCE_FILE_NEW_SOURCE_FILE);
      ModuleSourceFile.newSourceFile(id, code, metaInfo, projectConfig.singleFileEmit);
      MemoryMonitor.stopRecordStage(newSourceFileRecordInfo);
    }

    if (isJsSourceFile(id) || isJsonSourceFile(id)) {
      let code: string = this.getModuleInfo(id).originalCode;
      if (isJsSourceFile(id)) {
        code = processJsCodeLazyImport(id, code, autoLazyImport, reExportCheckMode);
        if (projectConfig.compatibleSdkVersion <= 10) {
          const transformedResult: object = transformJsByBabelPlugin(code, eventTransformForModule);
          code = transformedResult.code;
          preserveSourceMap(id, transformedResult.map, projectConfig, metaInfo, eventTransformForModule);
        } else {
          // preserve sourceMap of js file without transforming
          preserveSourceMap(id, this.getCombinedSourcemap(), projectConfig, metaInfo, eventTransformForModule);
        }
      }
      const recordInfo = MemoryMonitor.recordStage(MemoryDefine.MODULE_SOURCE_FILE_NEW_SOURCE_FILE);
      ModuleSourceFile.newSourceFile(id, code, metaInfo, projectConfig.singleFileEmit);
      MemoryMonitor.stopRecordStage(recordInfo);
    }
  }
  stopEvent(eventTransformForModule);

  if (reExportCheckLog && reExportCheckLog.errors.length &&
    reExportCheckMode !== reExportNoCheckMode && !this.share.projectConfig.ignoreWarning) {
    emitLogInfo(this.share.getLogger(GEN_ABC_PLUGIN_NAME), [...getTransformLog(reExportCheckLog)], true, id);
    resetReExportCheckLog();
  }

  // if we perform lazy-import conversion, we need to return the converted js code
  return code;
}

function preserveSourceMap(sourceFilePath: string, sourcemap: Object, projectConfig: Object, metaInfo: Object, parentEvent: CompileEvent): void {
  if (isCommonJsPluginVirtualFile(sourceFilePath)) {
    // skip automatic generated files like 'jsfile.js?commonjs-exports'
    return;
  }

  const eventAddSourceMapInfo = createAndStartEvent(parentEvent, 'preserve source map for ts/ets files');
  const relativeSourceFilePath = sourceFilePath.startsWith(projectConfig.projectRootPath) ?
    toUnixPath(sourceFilePath.replace(projectConfig.projectRootPath + path.sep, '')) :
    toUnixPath(sourceFilePath.replace(metaInfo.belongProjectPath + path.sep, ''));
  sourcemap.sources = [relativeSourceFilePath];
  sourcemap.file = path.basename(relativeSourceFilePath);
  sourcemap.sourcesContent && delete sourcemap.sourcesContent;
  const sourceMapGenerator = SourceMapGenerator.getInstance();
  const key = sourceMapGenerator.isNewSourceMaps() ? sourceFilePath : relativeSourceFilePath;
  sourceMapGenerator.fillSourceMapPackageInfo(sourceFilePath, sourcemap);
  sourceMapGenerator.updateSourceMap(key, sourcemap);
  stopEvent(eventAddSourceMapInfo);
}

function transformJsByBabelPlugin(code: string, parentEvent: CompileEvent): Object {
  const eventTransformByBabel = createAndStartEvent(parentEvent, 'transform Js by babel plugin');
  const transformed: Object = require('@babel/core').transformSync(code,
    {
      plugins: [
        [require("@babel/plugin-proposal-class-properties"), { "loose": true }]
      ],
      compact: false,
      sourceMaps: true
    }
  );
  stopEvent(eventTransformByBabel);
  return transformed;
}