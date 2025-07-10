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

import {
  Identifier,
  isComputedPropertyName,
  isConstructorDeclaration,
  isFunctionDeclaration,
  isFunctionExpression,
  isIdentifier,
  isMethodDeclaration,
  isNumericLiteral,
  isParameter,
  isStringLiteralLike,
  isTypeParameterDeclaration,
} from 'typescript';
import type {
  Declaration,
  Node,
  NumericLiteral,
  PropertyName,
  SourceFile,
  StringLiteral,
  StringLiteralLike,
  Symbol,
  TypeChecker,
} from 'typescript';
import type { INameObfuscationOption } from '../configs/INameObfuscationOption';
import { NodeUtils } from './NodeUtils';
import { isReservedProperty } from './TransformUtil';
import type { MangledSymbolInfo } from '../common/type';
import { nodeSymbolMap } from './ScopeAnalyzer';

// Determine that the current file is a declaration file and that the option to keep parameters is enabled
export function shouldKeepCurFileParamerters(sourceFile: SourceFile, config: INameObfuscationOption): boolean {
  return !!(sourceFile.isDeclarationFile && config.mKeepParameterNames);
}

// Determine whether the current parameter name needs to be obfuscated.
export function shouldKeepParameter(decl: Declaration | undefined, config: INameObfuscationOption, mangledSymbolNames: Map<Symbol, MangledSymbolInfo>,
  checker: TypeChecker): boolean {
  if (!decl || (!isParameter(decl) && !isTypeParameterDeclaration(decl)) || !isIdentifier(decl.name) || !decl.parent) {
    return false;
  }
  const parentNode: Node = decl.parent;

  if ((isFunctionDeclaration(parentNode) || isFunctionExpression(parentNode))) {
    const functionName: Identifier | undefined = parentNode.name;
    return !!functionName && !isObfuscatedName(functionName, mangledSymbolNames, checker);
  }

  if (isMethodDeclaration(parentNode)) {
    if (!config.mRenameProperties) {
      return true;
    }
    let methodName: Identifier | NumericLiteral | StringLiteralLike | undefined = getPropertyNameOfMethod(parentNode.name);
    return !!methodName && NodeUtils.isPropertyNameType(methodName) && isReservedProperty(methodName.text);
  }

  if (isConstructorDeclaration(parentNode)) {
    const className: Identifier | undefined = parentNode.parent.name;
    return !!className && !isObfuscatedName(className, mangledSymbolNames, checker);
  }

  return false;
}

function isObfuscatedName(name: Identifier, mangledSymbolNames: Map<Symbol, MangledSymbolInfo>, checker: TypeChecker): boolean {
  const sym: Symbol | undefined = NodeUtils.findSymbolOfIdentifier(checker, name, nodeSymbolMap);
  const symbolInfo: MangledSymbolInfo | undefined = mangledSymbolNames.get(sym);
  if (symbolInfo && symbolInfo.mangledName !== name.text) {
    return true;
  }
  return false;
}

function getPropertyNameOfMethod(propertyName: PropertyName): Identifier | NumericLiteral | StringLiteralLike | undefined {
  if (isComputedPropertyName(propertyName)) {
    if (isStringLiteralLike(propertyName.expression) || isNumericLiteral(propertyName.expression) || isIdentifier(propertyName.expression)) {
      return propertyName.expression;
    }
    return undefined;
  }
  return propertyName as Identifier | NumericLiteral | StringLiteral;
}