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
import { SUPER_ARGS, COMPONENT_SENDABLE_DECORATOR } from './pre_define';

function transformOptionalMemberForSendable(node: ts.PropertyDeclaration): ts.PropertyDeclaration {
  let updatedTypeNode: ts.TypeNode = node.type;

  if (ts.isUnionTypeNode(updatedTypeNode)) {
    if (!updatedTypeNode.types.find(type => type.kind === ts.SyntaxKind.UndefinedKeyword)) {
      updatedTypeNode = ts.factory.createUnionTypeNode([
        ...updatedTypeNode.types,
        ts.factory.createKeywordTypeNode(ts.SyntaxKind.UndefinedKeyword)
      ]);
    }
  } else {
    updatedTypeNode = ts.factory.createUnionTypeNode([
      updatedTypeNode,
      ts.factory.createKeywordTypeNode(ts.SyntaxKind.UndefinedKeyword)
    ]);
  }

  return ts.factory.createPropertyDeclaration(
    node.modifiers,
    node.name,
    undefined,
    updatedTypeNode,
    node.initializer ? node.initializer : ts.factory.createIdentifier('undefined')
  );
}

function removeSendableDecorator(modifiers: ts.NodeArray<ts.ModifierLike>): ts.NodeArray<ts.ModifierLike> {
  return ts.factory.createNodeArray(
    modifiers.filter(decorator => {
      const originalDecortor: string = decorator.getText().replace(/\(.*\)$/, '').trim();
      return originalDecortor !== COMPONENT_SENDABLE_DECORATOR;
    })
  );
}

function updateSendableConstructor(constructor: ts.ConstructorDeclaration): ts.ConstructorDeclaration {
  // Skip the overloaded signature of the constructor
  if (constructor.body === undefined) {
    return constructor;
  }
  const statementArray: ts.Statement[] = [
    ts.factory.createExpressionStatement(ts.factory.createStringLiteral('use sendable')),
    ...constructor.body.statements
  ];

  return ts.factory.updateConstructorDeclaration(
    constructor,
    constructor.modifiers,
    constructor.parameters,
    ts.factory.updateBlock(constructor.body, statementArray)
  );
}

function addConstructorForSendableClass(
  members: ts.NodeArray<ts.ClassElement>,
  needSuper: boolean): ts.NodeArray<ts.ClassElement> {
  const params: ts.ParameterDeclaration[] = [];
  const constructorStatements: ts.Statement[] = [
    ts.factory.createExpressionStatement(ts.factory.createStringLiteral('use sendable'))
  ];
  if (needSuper) {
    constructorStatements.push(
      ts.factory.createExpressionStatement(
        ts.factory.createCallExpression(
          ts.factory.createSuper(), undefined, [ts.factory.createSpreadElement(ts.factory.createIdentifier(SUPER_ARGS))])
      )
    );
    params.push(
      ts.factory.createParameterDeclaration(
        undefined,
        ts.factory.createToken(ts.SyntaxKind.DotDotDotToken),
        ts.factory.createIdentifier(SUPER_ARGS),
        undefined,
        undefined,
        undefined)
    );
  }
  const constructor: ts.ConstructorDeclaration = ts.factory.createConstructorDeclaration(
    undefined,
    params,
    ts.factory.createBlock(constructorStatements, true)
  );

  return ts.factory.createNodeArray([constructor, ...(members || [])]);
}

export function processSendableClass(node: ts.ClassDeclaration): ts.ClassDeclaration {
  let hasConstructor = false;
  let updatedMembers: ts.NodeArray<ts.ClassElement> = node.members;
  let updatedModifiers: ts.NodeArray<ts.ModifierLike> = removeSendableDecorator(node.modifiers);
  let needSuper: boolean =
    node.heritageClauses?.some(clause => clause.token === ts.SyntaxKind.ExtendsKeyword) || false;

  for (const member of node.members) {
    if (ts.isPropertyDeclaration(member) && member.questionToken) {
      const propertyDecl: ts.PropertyDeclaration = member as ts.PropertyDeclaration;
      const updatedPropertyDecl: ts.PropertyDeclaration = transformOptionalMemberForSendable(member);
      updatedMembers = ts.factory.createNodeArray(
        updatedMembers.map(member => (member === propertyDecl ? updatedPropertyDecl : member))
      );
    }
    if (ts.isConstructorDeclaration(member)) {
      hasConstructor = true;
      const constructor: ts.ConstructorDeclaration = member as ts.ConstructorDeclaration;
      updatedMembers = ts.factory.createNodeArray(
        updatedMembers.map(member => (member === constructor ? updateSendableConstructor(constructor) : member))
      );
    }
  }

  if (!hasConstructor) {
    updatedMembers = addConstructorForSendableClass(updatedMembers, needSuper);
  }

  node = ts.factory.updateClassDeclaration(node, updatedModifiers, node.name, node.typeParameters,
    node.heritageClauses, updatedMembers);

  return node;
}

export function processSendableFunction(node: ts.FunctionDeclaration): ts.FunctionDeclaration {
  if (node.body) {
    const statementArray: ts.Statement[] =
      [ts.factory.createExpressionStatement(ts.factory.createStringLiteral('use sendable')),
        ...node.body.statements];
    return ts.factory.updateFunctionDeclaration(node, ts.getModifiers(node), node.asteriskToken, node.name,
      node.typeParameters, node.parameters, node.type, ts.factory.updateBlock(node.body, statementArray));
  }
  return ts.factory.createFunctionDeclaration(undefined, node.asteriskToken, node.name,
    node.typeParameters, node.parameters, node.type, node.body);
}

export function processSendableType(node: ts.TypeAliasDeclaration): ts.TypeAliasDeclaration {
  return ts.factory.createTypeAliasDeclaration(undefined, node.name, node.typeParameters, node.type);
}
