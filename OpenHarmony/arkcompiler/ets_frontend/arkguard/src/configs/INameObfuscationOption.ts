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

import type {NameGeneratorType} from '../generator/NameFactory';

export interface INameObfuscationOption {

  readonly mEnable: boolean;

  readonly mNameGeneratorType?: NameGeneratorType;

  readonly mRenameProperties: boolean;

  mReservedNames?: string[];

  mReservedProperties: string[];

  mUniversalReservedProperties?: RegExp[];

  readonly mDictionaryList?: string[];

  readonly mKeepStringProperty?: boolean;

  readonly mTopLevel?: boolean;

  readonly mKeepParameterNames?: boolean;

  mReservedToplevelNames?: string[];

  mUniversalReservedToplevelNames?: RegExp[];

  readonly mEnableAtKeep?: boolean;
}

export interface IFileNameObfuscationOption {

  readonly mEnable: boolean;

  readonly mNameGeneratorType: NameGeneratorType;

  mReservedFileNames: string[];

  mUniversalReservedFileNames?: RegExp[];

  readonly mOhmUrlStatus?: OhmUrlStatus;
}

export interface IDeclarationCommentOption {

  readonly mEnable: boolean,

  mReservedComments: string[],

  mUniversalReservedComments?: RegExp[]
}

export enum OhmUrlStatus {
  NONE,
  AT_BUNDLE,
  NORMALIZED
}

export interface IPrinterOption {

  //Print obfuscation time&memory usage of all files and obfuscation processes
  readonly mFilesPrinter?: boolean;

  //Print time&memory usage of a single file obfuscation in transform processes
  readonly mSingleFilePrinter?: boolean;

  //Print sum up time of transform processes during obfuscation
  readonly mSumPrinter?: boolean;

  //Output path of printer
  readonly mOutputPath?: string;
}

/**
 * It records which files cannot be obfuscated (except for file name obfuscation) and their dependent files.
 * The names and properties exported in the dependent files are put into the whitelist.
 */
export interface IKeepSourcePathsAndDependency {
  mKeepSourceOfPaths: Set<string>;
  mkeepFilesAndDependencies: Set<string>;
}

export interface IUnobfuscationOption {
  mPrintKeptNames: boolean;
  mPrintPath: string;
}
