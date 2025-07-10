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
import ts from 'typescript';
import fs from 'fs';

import { SourceMapGenerator } from './fast_build/ark_compiler/generate_sourcemap';
import {
  EXTNAME_TS,
  EXTNAME_ETS,
} from './pre_define';
import {
  genTemporaryPath,
  mkdirsSync,
  toUnixPath,
} from './utils';
import {
  newSourceMaps as webpackNewSourceMaps,
  transformModuleSpecifier,
  writeObfuscatedSourceCode,
} from './ark_utils';
import { processSystemApi } from './validate_ui_syntax';
import { isDebug } from './fast_build/ark_compiler/utils';
import { getRelativeSourcePath } from './fast_build/ark_compiler/common/ob_config_resolver';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent,
 } from './performance';

export const SRC_MAIN: string = 'src/main';

export async function writeFileSyncByNode(node: ts.SourceFile, projectConfig: Object, metaInfo: Object, moduleId?: string,
  parentEvent?: CompileEvent, logger?: Object): Promise<void> {
  const eventWriteFileSyncByNode = createAndStartEvent(parentEvent, 'write file sync by node');
  const eventGenContentAndSourceMapInfo = createAndStartEvent(eventWriteFileSyncByNode, 'generate content and source map information');
  const mixedInfo: { content: string, sourceMapJson: ts.RawSourceMap } = genContentAndSourceMapInfo(node, moduleId, projectConfig, metaInfo);
  const sourceMapGenerator = SourceMapGenerator.getInstance();
  stopEvent(eventGenContentAndSourceMapInfo);

  /**
   * In the following situation:
   * A typescript source file whose name is 'Test.ts', which is used via `import xxx for 'test'` in another source file.

   * The value of "node.fileName" consists of "test.ts", which does not correspond with the source file's actual name and would lead to a compilation error.
   * The value of moduleId is same as the actual file name, so it would be used here for locating the target source file.

   * Note: current realization is related to the moduleId mechanism in the rollup framework, which is needed to be reconsidered to improve the code robustness.
   * In the current realization, when moduleId mechanism is changed, there would be a compilation error.
   */
  let temporaryFile: string = genTemporaryPath(moduleId ? moduleId : node.fileName, projectConfig.projectPath, process.env.cachePath,
    projectConfig, metaInfo);
  if (temporaryFile.length === 0) {
    return;
  }
  if (temporaryFile.endsWith(EXTNAME_ETS)) {
    temporaryFile = temporaryFile.replace(/\.ets$/, EXTNAME_TS);
  }
  let relativeFilePath = getRelativeSourcePath(moduleId ? moduleId : node.fileName, projectConfig.projectRootPath, metaInfo?.belongProjectPath);
  let sourceMaps: Object;
  if (process.env.compileTool === 'rollup') {
    const key = sourceMapGenerator.isNewSourceMaps() ? moduleId! : relativeFilePath;
    sourceMapGenerator.fillSourceMapPackageInfo(moduleId!, mixedInfo.sourceMapJson);
    sourceMapGenerator.updateSourceMap(key, mixedInfo.sourceMapJson);
    sourceMaps = sourceMapGenerator.getSourceMaps();
  } else {
    webpackNewSourceMaps[relativeFilePath] = mixedInfo.sourceMapJson;
    sourceMaps = webpackNewSourceMaps;
  }
  if (!isDebug(projectConfig)) {
    const eventWriteObfuscatedSourceCode = createAndStartEvent(eventWriteFileSyncByNode, 'write obfuscated source code');
    await writeObfuscatedSourceCode({
        content: mixedInfo.content,
        buildFilePath: temporaryFile,
        relativeSourceFilePath: relativeFilePath,
        originSourceFilePath: node.fileName,
        rollupModuleId: moduleId ? moduleId : undefined
      }, logger, projectConfig, sourceMaps);
    stopEvent(eventWriteObfuscatedSourceCode);
    return;
  }
  mkdirsSync(path.dirname(temporaryFile));
  fs.writeFileSync(temporaryFile, mixedInfo.content);
  stopEvent(eventWriteFileSyncByNode);
}

function genContentAndSourceMapInfo(node: ts.SourceFile, moduleId: string | undefined, projectConfig: Object, metaInfo: Object): Object {
  const printer: ts.Printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
  const options: ts.CompilerOptions = {
    sourceMap: true
  };
  const mapOpions: Object = {
    sourceMap: true,
    inlineSourceMap: false,
    inlineSources: false,
    sourceRoot: '',
    mapRoot: '',
    extendedDiagnostics: false
  };
  const host: ts.CompilerHost = ts.createCompilerHost(options);
  const fileName: string = moduleId ? moduleId : node.fileName;
  // @ts-ignore
  const sourceMapGenerator: ts.SourceMapGenerator = ts.createSourceMapGenerator(
    host,
    // @ts-ignore
    ts.getBaseFileName(fileName),
    '',
    '',
    mapOpions
  );
  // @ts-ignore
  const writer: ts.EmitTextWriter = ts.createTextWriter(
    // @ts-ignore
    ts.getNewLineCharacter({ newLine: ts.NewLineKind.LineFeed, removeComments: false }));
  printer.writeFile(node, writer, sourceMapGenerator);
  const sourceMapJson: ts.RawSourceMap = sourceMapGenerator.toJSON();
  sourceMapJson.sources = [
    toUnixPath(fileName).startsWith(toUnixPath(projectConfig.projectRootPath)) ?
    toUnixPath(fileName).replace(toUnixPath(projectConfig.projectRootPath) + '/', '') :
    toUnixPath(fileName).replace(toUnixPath(metaInfo.belongProjectPath) + '/', '')
  ];
  let content: string = writer.getText();
  if (process.env.compileTool !== 'rollup') {
    content = transformModuleSpecifier(fileName, processSystemApi(content, true), projectConfig);
  }

  return {
    content: content,
    sourceMapJson: sourceMapJson
  };
}
