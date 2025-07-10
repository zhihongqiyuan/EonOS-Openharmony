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
import * as ts from 'typescript';
import { projectConfig } from '../main';
import { toUnixPath } from './utils';
import {
  ERROR_DESCRIPTION,
  LINTER_SUBSYSTEM_CODE,
  ERROR_TYPE_CODE,
  EXTENSION_CODE,
  HvigorErrorInfo,
} from './hvigor_error_code/hvigor_error_info';

const arkTSDir: string = 'ArkTS';
const arkTSLinterOutputFileName: string = 'ArkTSLinter_output.json';
const spaceNumBeforeJsonLine = 2;
const complementSize: number = 3;
const complementCode: string = '0';

interface OutputInfo {
  categoryInfo: string | undefined;
  fileName: string | undefined;
  line: number | undefined;
  character: number | undefined;
  messageText: string | ts.DiagnosticMessageChain;
}

export enum ArkTSLinterMode {
  NOT_USE = 0,
  COMPATIBLE_MODE = 1,
  STANDARD_MODE = 2
}

export enum ArkTSVersion {
  ArkTS_1_0,
  ArkTS_1_1,
}

export type ProcessDiagnosticsFunc = (diagnostics: ts.Diagnostic, errorCodeLogger?: Object | undefined) => void;

function getArkTSVersionString(arkTSVersion: ArkTSVersion): string {
  return arkTSVersion === ArkTSVersion.ArkTS_1_0 ? 'ArkTS_1_0' : 'ArkTS_1_1';
}

export function doArkTSLinter(arkTSVersion: ArkTSVersion, arkTSMode: ArkTSLinterMode,
  builderProgram: ts.BuilderProgram, printDiagnostic: ProcessDiagnosticsFunc, shouldWriteFile: boolean = true,
  buildInfoWriteFile?: ts.WriteFileCallback, errorCodeLogger?: Object | undefined): ts.Diagnostic[] {
  if (arkTSMode === ArkTSLinterMode.NOT_USE) {
    return [];
  }

  let diagnostics: ts.Diagnostic[] = [];
  const isOldVersion = arkTSVersion === ArkTSVersion.ArkTS_1_0;
  if (isOldVersion) {
    diagnostics = ts.ArkTSLinter_1_0.runArkTSLinter(builderProgram, /*srcFile*/ undefined, buildInfoWriteFile,
      getArkTSVersionString(arkTSVersion));
  } else {
    diagnostics = ts.ArkTSLinter_1_1.runArkTSLinter(builderProgram, /*srcFile*/ undefined, buildInfoWriteFile,
      getArkTSVersionString(arkTSVersion));
  }

  removeOutputFile();
  if (diagnostics.length === 0) {
    return [];
  }

  if (arkTSMode === ArkTSLinterMode.COMPATIBLE_MODE) {
    processArkTSLinterReportAsWarning(diagnostics, printDiagnostic, shouldWriteFile);
  } else {
    processArkTSLinterReportAsError(diagnostics, printDiagnostic, isOldVersion ? undefined : errorCodeLogger);
  }

  return diagnostics;
}

function processArkTSLinterReportAsError(diagnostics: ts.Diagnostic[], printDiagnostic: ProcessDiagnosticsFunc, errorCodeLogger?: Object | undefined): void {
  diagnostics.forEach((diagnostic: ts.Diagnostic) => {
    printDiagnostic(diagnostic, errorCodeLogger);
  });
  printArkTSLinterFAQ(diagnostics, printDiagnostic);
}

function processArkTSLinterReportAsWarning(diagnostics: ts.Diagnostic[], printDiagnostic: ProcessDiagnosticsFunc,
  shouldWriteFile: boolean): void {
  const filePath = shouldWriteFile ? writeOutputFile(diagnostics) : undefined;
  if (filePath === undefined) {
    diagnostics.forEach((diagnostic: ts.Diagnostic) => {
      const originalCategory = diagnostic.category;
      diagnostic.category = ts.DiagnosticCategory.Warning;
      printDiagnostic(diagnostic);
      diagnostic.category = originalCategory;
    });
    printArkTSLinterFAQ(diagnostics, printDiagnostic);
    return;
  }
  const logMessage = `Has ${diagnostics.length} ArkTS Linter Error. You can get the output in ${filePath}`;
  const arkTSDiagnostic: ts.Diagnostic = {
    file: undefined,
    start: undefined,
    length: undefined,
    messageText: logMessage,
    category: ts.DiagnosticCategory.Warning,
    code: -1,
    reportsUnnecessary: undefined,
    reportsDeprecated: undefined
  };
  printDiagnostic(arkTSDiagnostic);

  printArkTSLinterFAQ(diagnostics, printDiagnostic);
}

function writeOutputFile(diagnostics: ts.Diagnostic[]): string | undefined {
  let filePath: string = toUnixPath(projectConfig.cachePath);
  if (!fs.existsSync(filePath)) {
    return undefined;
  }
  filePath = toUnixPath(path.join(filePath, arkTSDir));
  if (!fs.existsSync(filePath)) {
    fs.mkdirSync(filePath);
  }
  filePath = toUnixPath((path.join(filePath, arkTSLinterOutputFileName)));
  const outputInfo: OutputInfo[] = [];
  diagnostics.forEach((diagnostic: ts.Diagnostic) => {
    const { line, character }: ts.LineAndCharacter =
      diagnostic.file.getLineAndCharacterOfPosition(diagnostic.start!);
    outputInfo.push({
      categoryInfo: diagnostic.category === ts.DiagnosticCategory.Error ? 'Error' : 'Warning',
      fileName: diagnostic.file?.fileName,
      line: line + 1,
      character: character + 1,
      messageText: diagnostic.messageText
    });
  });
  let output: string | undefined = filePath;
  try {
    fs.writeFileSync(filePath, JSON.stringify(outputInfo, undefined, spaceNumBeforeJsonLine));
  } catch {
    output = undefined;
  }
  return output;
}

function removeOutputFile(): void {
  let filePath: string = toUnixPath(projectConfig.cachePath);
  if (!fs.existsSync(filePath)) {
    return;
  }
  filePath = toUnixPath(path.join(filePath, arkTSDir));
  if (!fs.existsSync(filePath)) {
    return;
  }
  filePath = toUnixPath((path.join(filePath, arkTSLinterOutputFileName)));
  if (fs.existsSync(filePath)) {
    fs.rmSync(filePath);
  }
}

function printArkTSLinterFAQ(diagnostics: ts.Diagnostic[], printDiagnostic: ProcessDiagnosticsFunc): void {
  if (diagnostics === undefined || diagnostics.length === undefined || diagnostics.length <= 0) {
    return;
  }

  const logMessageFAQ = 'For details about ArkTS syntax errors, see FAQs';
  const arkTSFAQDiagnostic: ts.Diagnostic = {
    file: undefined,
    start: undefined,
    length: undefined,
    messageText: logMessageFAQ,
    category: ts.DiagnosticCategory.Warning,
    code: -1,
    reportsUnnecessary: undefined,
    reportsDeprecated: undefined
  };
  printDiagnostic(arkTSFAQDiagnostic);
}

export function transfromErrorCode(code: number, positionMessage: string, message: string): HvigorErrorInfo {
  return new HvigorErrorInfo()
    .setCode(formatNumber(code))
    .setDescription(ERROR_DESCRIPTION)
    .setCause(message)
    .setPosition(positionMessage)
    .setSolutions([]);
}

function formatNumber(num: number): string {
  // The minimum code in strict mode starts from 1000
  const extendedCode = num > 1000 ? EXTENSION_CODE : num.toString().padStart(complementSize, complementCode);
  return LINTER_SUBSYSTEM_CODE + ERROR_TYPE_CODE + extendedCode;
}
