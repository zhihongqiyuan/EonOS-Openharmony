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

import ts from 'typescript';

import {
  USE_SHARED,
  USE_SHARED_COMMENT
} from './common/ark_define';
import {
  GeneratedFileInHar,
  harFilesRecord,
  toUnixPath
} from '../../utils';
import {
  projectConfig
} from '../../../main';

export const sharedModuleSet: Set<string> = new Set();

export function collectSharedModule(source: string, filePath: string, sourceFile: ts.SourceFile | null): void {
  if (!sourceFile) {
    sourceFile = ts.createSourceFile(filePath, source, ts.ScriptTarget.Latest, true, ts.ScriptKind.ETS);
  }

  // "use shared" will only be effective when used after imports, before other statements
  for (const statement of sourceFile.statements) {
    if (ts.isImportDeclaration(statement)) {
      continue;
    }
    if (ts.isExpressionStatement(statement) && ts.isStringLiteral(statement.expression) &&
      statement.expression.text === USE_SHARED) {
      sharedModuleSet.add(toUnixPath(filePath));
      processDeclarationFile(filePath);
    }
    return;
  }
}

// For hsp and close-source har, we need add comment '// use shared' for shared modules to inform developers.
function processDeclarationFile(filePath: string): void {
  if (!projectConfig.compileHar && !projectConfig.compileShared) {
    return;
  }

  const generatedDetsFilesInHar: GeneratedFileInHar = harFilesRecord.get(toUnixPath(filePath));
  if (generatedDetsFilesInHar) {
    generatedDetsFilesInHar.originalDeclarationContent = USE_SHARED_COMMENT + '\n' +
      generatedDetsFilesInHar.originalDeclarationContent;
  }
}

export function cleanSharedModuleSet(): void {
  sharedModuleSet.clear();
}
