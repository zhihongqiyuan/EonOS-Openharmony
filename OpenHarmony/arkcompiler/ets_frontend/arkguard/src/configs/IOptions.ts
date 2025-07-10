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

import type {
  IFileNameObfuscationOption,
  IPrinterOption,
  INameObfuscationOption,
  IKeepSourcePathsAndDependency,
  IDeclarationCommentOption,
  IUnobfuscationOption
} from './INameObfuscationOption';

export interface IOptions {
  // Whether to generate compact code
  readonly mCompact?: boolean;

  // Whether to remove comments;
  readonly mRemoveComments?: boolean;

  // Whether to remove JSDoc comments;
  readonly mRemoveDeclarationComments?: IDeclarationCommentOption;

  // Whether to disable console output
  readonly mDisableConsole?: boolean;

  // Whether to do code simplification, includes variable declarations merging, expression merging...
  readonly mSimplify?: boolean;

  // Whether to do Name Obfuscation
  readonly mNameObfuscation?: INameObfuscationOption;

  mOutputDir?: string;

  readonly mOhSdkPath?: string;

  readonly mEnableSourceMap?: boolean;

  readonly mEnableNameCache?: boolean;

  readonly apiSavedDir?: string;

  readonly applyReservedNamePath?: string;

  readonly mRenameFileName?: IFileNameObfuscationOption;

  // Whether to obfuscate the names or properties of the exported content
  readonly mExportObfuscation?: boolean;

  // Time&Memory printer option
  readonly mPerformancePrinter?: IPrinterOption;

  // The code of the file is not obfuscated, except for file name obfuscation
  mKeepFileSourceCode?: IKeepSourcePathsAndDependency

  // Whether to print unobfuscation names and the print path.
  readonly mUnobfuscationOption?: IUnobfuscationOption;

  // Use optimized system whitelist
  readonly mStripLanguageDefaultWhitelist?: boolean;

  // Is Annotation allowed
  readonly mAllowEtsAnnotations?: boolean;
}
