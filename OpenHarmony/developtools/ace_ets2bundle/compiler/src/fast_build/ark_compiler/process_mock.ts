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

import ts from 'typescript';
import { projectConfig } from '../../../main';
import { DECORATOR_SUFFIX } from '../../pre_define';

export function disableMockDecorator(node: ts.Decorator): boolean {
  if (!shouldDisableMockDecorator()) {
    return false;
  }

  let parent: ts.Node = node.parent;
  switch (parent.kind) {
    case ts.SyntaxKind.Parameter: {
      ts.factory.updateParameterDeclaration(<ts.ParameterDeclaration>parent,
        ts.getModifiers((<ts.ParameterDeclaration>parent)),
        (<ts.ParameterDeclaration>parent).dotDotDotToken,
        (<ts.ParameterDeclaration>parent).name,
        (<ts.ParameterDeclaration>parent).questionToken,
        (<ts.ParameterDeclaration>parent).type,
        (<ts.ParameterDeclaration>parent).initializer);
      break;
    }
    case ts.SyntaxKind.MethodDeclaration: {
      ts.factory.updateMethodDeclaration(<ts.MethodDeclaration>parent,
        ts.getModifiers((<ts.MethodDeclaration>parent)),
        (<ts.MethodDeclaration>parent).asteriskToken,
        (<ts.MethodDeclaration>parent).name,
        (<ts.MethodDeclaration>parent).questionToken,
        (<ts.MethodDeclaration>parent).typeParameters,
        (<ts.MethodDeclaration>parent).parameters,
        (<ts.MethodDeclaration>parent).type,
        (<ts.MethodDeclaration>parent).body);
      break;
    }
    case ts.SyntaxKind.Constructor: {
      ts.factory.updateConstructorDeclaration(<ts.ConstructorDeclaration>parent,
        ts.getModifiers((<ts.ConstructorDeclaration>parent)),
        (<ts.ConstructorDeclaration>parent).parameters,
        (<ts.ConstructorDeclaration>parent).body);
      break;
    }
    case ts.SyntaxKind.GetAccessor: {
      ts.factory.updateGetAccessorDeclaration(<ts.GetAccessorDeclaration>parent,
        ts.getModifiers((<ts.GetAccessorDeclaration>parent)),
        (<ts.GetAccessorDeclaration>parent).name,
        (<ts.GetAccessorDeclaration>parent).parameters,
        (<ts.GetAccessorDeclaration>parent).type,
        (<ts.GetAccessorDeclaration>parent).body);
      break;
    }
    case ts.SyntaxKind.SetAccessor: {
      ts.factory.updateSetAccessorDeclaration(<ts.SetAccessorDeclaration>parent,
        ts.getModifiers((<ts.SetAccessorDeclaration>parent)),
        (<ts.SetAccessorDeclaration>parent).name,
        (<ts.SetAccessorDeclaration>parent).parameters,
        (<ts.SetAccessorDeclaration>parent).body);
      break;
    }
    case ts.SyntaxKind.PropertyDeclaration: {
      if ((<ts.PropertyDeclaration>parent).questionToken) {
        ts.factory.updatePropertyDeclaration(<ts.PropertyDeclaration>parent,
          ts.getModifiers((<ts.PropertyDeclaration>parent)),
          (<ts.PropertyDeclaration>parent).name,
          (<ts.PropertyDeclaration>parent).questionToken,
          (<ts.PropertyDeclaration>parent).type,
          (<ts.PropertyDeclaration>parent).initializer);
      } else if ((<ts.PropertyDeclaration>parent).exclamationToken) {
        ts.factory.updatePropertyDeclaration(<ts.PropertyDeclaration>parent,
          ts.getModifiers((<ts.PropertyDeclaration>parent)),
          (<ts.PropertyDeclaration>parent).name,
          (<ts.PropertyDeclaration>parent).exclamationToken,
          (<ts.PropertyDeclaration>parent).type,
          (<ts.PropertyDeclaration>parent).initializer);
      } else {
        ts.factory.updatePropertyDeclaration(<ts.PropertyDeclaration>parent,
          ts.getModifiers((<ts.PropertyDeclaration>parent)),
          (<ts.PropertyDeclaration>parent).name,
          undefined,
          (<ts.PropertyDeclaration>parent).type,
          (<ts.PropertyDeclaration>parent).initializer);
      }
      break;
    }
    case ts.SyntaxKind.ClassDeclaration: {
      ts.factory.updateClassDeclaration(<ts.ClassDeclaration>parent,
        ts.getModifiers((<ts.ClassDeclaration>parent)),
        (<ts.ClassDeclaration>parent).name,
        (<ts.ClassDeclaration>parent).typeParameters,
        (<ts.ClassDeclaration>parent).heritageClauses,
        (<ts.ClassDeclaration>parent).members);
      break;
    }
    default: {
      break;
    }
  }
  return true;
}

function removeMockDecorator(decs: readonly ts.Decorator[]): ts.Decorator[] {
  let res: ts.Decorator[];
  for (let dec of decs) {
    if (!isMockDecorator(dec)) {
      res.push(dec);
    }
  }

  return res;
}

export function isMockDecorator(dec: ts.Decorator): boolean {
  let decObj = dec.expression;
  if (!ts.isIdentifier(decObj)) {
    return false;
  }

  if (projectConfig.mockParams) {
    let mockDecorator: string = projectConfig.mockParams.decorator.replace(DECORATOR_SUFFIX, '');
    return ((<ts.Identifier>decObj).escapedText.toString() === mockDecorator);
  }

  return false;
}

function shouldDisableMockDecorator(): boolean {
  // mock decorator only takes effect under preview mode, should be removed otherswise
  if (projectConfig.isPreview) {
    return false;
  }

  // mockParams = {
  //   "decorator": "name of mock decorator",
  //   "packageName": "name of mock package",
  //   "etsSourceRootPath": "path of ets source root",
  //   "mockConfigPath": "path of mock configuration file"
  // }
  return (projectConfig.mockParams && projectConfig.mockParams.decorator && projectConfig.mockParams.packageName) ?
    true : false;
}

export const ORIGIN_EXTENTION: string = '.origin';