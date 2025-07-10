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

import type * as ts from 'typescript'
import { reset, yellow } from '../ark_compiler/common/ark_define'

export interface ArkoalaPluginOptions {
  /**
   * Filter program source files which must be trnsformed with Arkoala plugins
   * 
   * @param fileName Normalize source file path
   * @returns true if the given file must be transformed
   */
  filter?: (fileName: string) => boolean;
  /**
   * Specify the root directory from which router path must be resolved
   * @default "."
   */
  routerRootDir?: string;
}

// This is an implementation stub, it should be replaced with arkoala-plugin.js from the Arkoala repo.
export default function arkoalaProgramTransform(
  program: ts.Program,
  compilerHost: ts.CompilerHost | undefined,
  options: ArkoalaPluginOptions,
  extras: Object,
): ts.Program {
  let [,,,] = [compilerHost, options, extras]
  console.warn(`${yellow}WARN: Arkoala plugin is missing in the current SDK. Source transformation will not be performed.${reset}`)
  return program
}
