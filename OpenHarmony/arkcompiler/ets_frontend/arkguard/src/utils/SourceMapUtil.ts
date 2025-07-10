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

import {
  createCompilerHost,
  createSourceMapGenerator,
} from 'typescript';

import type {
  CompilerHost,
  CompilerOptions,
  EmitHost,
  SourceMapGenerator,
  SourceMapGeneratorOptions,
} from 'typescript';

/**
 * create sourcemap generator use api of typescript
 * @param sourceFile: file path of source code
 */
export function getSourceMapGenerator(sourceFile: string): SourceMapGenerator {
  let compilerOptions: CompilerOptions = {};
  let compilerHost: CompilerHost = createCompilerHost(compilerOptions);

  function getCanonicalFileName(fileName: string): string {
    return compilerHost.getCanonicalFileName(fileName);
  }

  const currentDirectory: string = compilerHost.getCurrentDirectory();

  let host: EmitHost = {
    getSourceFileFromReference: undefined,
    redirectTargetsMap: undefined,
    fileExists(path: string): boolean {
      return false;
    },
    isEmitBlocked(emitFileName: string): boolean {
      return false;
    },
    useCaseSensitiveFileNames(): boolean {
      return false;
    },
    getPrependNodes: undefined,
    getCanonicalFileName: getCanonicalFileName,
    getCommonSourceDirectory: undefined,
    getCompilerOptions: undefined,
    getCurrentDirectory: () => currentDirectory,
    getNewLine: undefined,
    getSourceFile: undefined,
    getSourceFileByPath: undefined,
    getSourceFiles: undefined,
    getLibFileFromReference: undefined,
    isSourceFileFromExternalLibrary: undefined,
    getResolvedProjectReferenceToRedirect: undefined,
    getProjectReferenceRedirect: undefined,
    isSourceOfProjectReferenceRedirect: undefined,
    writeFile: undefined
  };

  const generatorOptions: SourceMapGeneratorOptions = {extendedDiagnostics: false};
  return createSourceMapGenerator(host, sourceFile, currentDirectory, currentDirectory, generatorOptions);
}
