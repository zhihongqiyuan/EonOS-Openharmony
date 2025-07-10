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
  ELMTID,
  FINALIZE_CONSTRUCTION
} from './pre_define';

import { 
  IFileLog, 
  LogInfo 
} from './utils';

class FileLog implements IFileLog {
  private _sourceFile: ts.SourceFile | undefined;
  private _errors: LogInfo[] = [];

  public get sourceFile(): ts.SourceFile | undefined {
    return this._sourceFile;
  }

  public set sourceFile(newValue: ts.SourceFile) {
    this._sourceFile = newValue;
  }

  public get errors(): LogInfo[] {
    return this._errors;
  }

  public set errors(newValue: LogInfo[]) {
    this._errors = newValue;
  }

  public cleanUp(): void {
    this._sourceFile = undefined;
    this._errors = [];
  }
}


function createParameterDeclaration(name: string): ts.ParameterDeclaration {
  let initializer: ts.Expression;
  if (name === ELMTID) {
    initializer = ts.factory.createPrefixUnaryExpression(
      ts.SyntaxKind.MinusToken, ts.factory.createNumericLiteral('1'));
  }
  return ts.factory.createParameterDeclaration(undefined, undefined,
    ts.factory.createIdentifier(name), undefined, undefined, initializer);
}

function createFinalizeConstruction(freezeParamNode: ts.Expression): ts.Statement {
  const params: ts.Expression[] = [];
  if (freezeParamNode) {
    params.push(freezeParamNode);
  }
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(),
      ts.factory.createIdentifier(FINALIZE_CONSTRUCTION)
    ),
    undefined,
    params
  ));
}

function createImportNodeForModuleInfo(): ts.ImportDeclaration {
  return ts.factory.createImportDeclaration(
    undefined, ts.factory.createImportClause(false, undefined,
      ts.factory.createNamedImports([ts.factory.createImportSpecifier(
        false, undefined, ts.factory.createIdentifier('__MODULE_NAME__')
      ), ts.factory.createImportSpecifier(false, undefined,
        ts.factory.createIdentifier('__BUNDLE_NAME__'))])
    ), ts.factory.createStringLiteral('ModuleInfo'), undefined
  );
}

export default {
  FileLog: FileLog,
  createParameterDeclaration: createParameterDeclaration,
  createFinalizeConstruction: createFinalizeConstruction,
  createImportNodeForModuleInfo: createImportNodeForModuleInfo
};
