/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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
  COMPONENT_CONSTRUCTOR_ID,
  COMPONENT_CONSTRUCTOR_PARENT,
  COMPONENT_CONSTRUCTOR_PARAMS,
  COMPONENT_CONSTRUCTOR_UPDATE_PARAMS,
  COMPONENT_CONSTRUCTOR_INITIAL_PARAMS,
  COMPONENT_WATCH_FUNCTION,
  BASE_COMPONENT_NAME,
  INTERFACE_NAME_SUFFIX,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE,
  BASE_COMPONENT_NAME_PU,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE_TYPE_PU,
  ELMTID,
  COMPONENT_PARAMS_LAMBDA_FUNCTION,
  COMPONENT_IF_UNDEFINED,
  CUSTOM_COMPONENT_EXTRAINFO
} from './pre_define';
import { partialUpdateConfig } from '../main';
import createAstNodeUtils from './create_ast_node_utils';

export function getInitConstructor(members: ts.NodeArray<ts.Node>, parentComponentName: ts.Identifier
): ts.ConstructorDeclaration {
  let ctorNode: any = members.find(item => {
    return ts.isConstructorDeclaration(item);
  });
  if (ctorNode) {
    ctorNode = updateConstructor(ctorNode, [], [], [], true);
  }
  return initConstructorParams(ctorNode, parentComponentName);
}

export function updateConstructor(ctorNode: ts.ConstructorDeclaration, para: ts.ParameterDeclaration[],
  addStatements: ts.Statement[], decoratorComponentParent: ts.IfStatement[], isSuper: boolean = false,
  isAdd: boolean = false, parentComponentName?: ts.Identifier): ts.ConstructorDeclaration {
  let modifyPara: ts.ParameterDeclaration[];
  if (para && para.length) {
    modifyPara = Array.from(ctorNode.parameters);
    if (modifyPara) {
      modifyPara.push(...para);
    }
  }
  let modifyBody: ts.Statement[];
  if (addStatements && addStatements.length && ctorNode) {
    modifyBody = Array.from(ctorNode.body.statements);
    if (modifyBody) {
      if (isSuper) {
        modifyBody.unshift(...addStatements);
        if (decoratorComponentParent && decoratorComponentParent.length) {
          modifyBody.push(...decoratorComponentParent);
        }
      } else {
        modifyBody.push(...addStatements);
      }
    }
  }
  if (ctorNode) {
    let ctorPara: ts.ParameterDeclaration[] | ts.NodeArray<ts.ParameterDeclaration> =
      modifyPara || ctorNode.parameters;
    if (isAdd) {
      ctorPara = addParamsType(ctorNode, modifyPara, parentComponentName);
    }
    ctorNode = ts.factory.updateConstructorDeclaration(ctorNode,
      ts.getModifiers(ctorNode), modifyPara || ctorNode.parameters,
      ts.factory.createBlock(modifyBody || ctorNode.body.statements, true));
  }
  return ctorNode;
}

function initConstructorParams(node: ts.ConstructorDeclaration, parentComponentName: ts.Identifier):
  ts.ConstructorDeclaration {
  if (!ts.isIdentifier(parentComponentName)) {
    return node;
  }
  const paramNames: Set<string> = !partialUpdateConfig.partialUpdateMode ? new Set([
    COMPONENT_CONSTRUCTOR_ID,
    COMPONENT_CONSTRUCTOR_PARENT,
    COMPONENT_CONSTRUCTOR_PARAMS,
    COMPONENT_CONSTRUCTOR_LOCALSTORAGE
  ]) : new Set([
    COMPONENT_CONSTRUCTOR_PARENT,
    COMPONENT_CONSTRUCTOR_PARAMS,
    COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU,
    ELMTID,
    COMPONENT_PARAMS_LAMBDA_FUNCTION,
    CUSTOM_COMPONENT_EXTRAINFO
  ]);
  const newParameters: ts.ParameterDeclaration[] = Array.from(node.parameters);
  if (newParameters.length !== 0) {
    // @ts-ignore
    newParameters.splice(0, newParameters.length);
  }
  paramNames.forEach((paramName: string) => {
    newParameters.push(ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(paramName), undefined, undefined,
      paramName === ELMTID || paramName === COMPONENT_PARAMS_LAMBDA_FUNCTION ? paramName === ELMTID ?
        ts.factory.createPrefixUnaryExpression(ts.SyntaxKind.MinusToken, ts.factory.createNumericLiteral('1')) :
        ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED) : undefined));
  });

  return ts.factory.updateConstructorDeclaration(node, ts.getModifiers(node), newParameters, node.body);
}

function addParamsType(ctorNode: ts.ConstructorDeclaration, modifyPara: ts.ParameterDeclaration[],
  parentComponentName: ts.Identifier): ts.ParameterDeclaration[] {
  const tsPara: ts.ParameterDeclaration[] | ts.NodeArray<ts.ParameterDeclaration> =
    modifyPara || ctorNode.parameters;
  const newTSPara: ts.ParameterDeclaration[] = [];
  tsPara.forEach((item) => {
    let parameter: ts.ParameterDeclaration = item;
    switch (item.name.escapedText) {
      case COMPONENT_CONSTRUCTOR_ID:
        parameter = ts.factory.updateParameterDeclaration(item, ts.getModifiers(item),
          item.dotDotDotToken, item.name, item.questionToken,
          ts.factory.createKeywordTypeNode(ts.SyntaxKind.StringKeyword), item.initializer);
        break;
      case COMPONENT_CONSTRUCTOR_PARENT:
        parameter = ts.factory.createParameterDeclaration(ts.getModifiers(item),
          item.dotDotDotToken, item.name, item.questionToken,
          ts.factory.createTypeReferenceNode(ts.factory.createIdentifier(
            !partialUpdateConfig.partialUpdateMode ? BASE_COMPONENT_NAME : BASE_COMPONENT_NAME_PU), undefined),
          item.initializer);
        break;
      case COMPONENT_CONSTRUCTOR_PARAMS:
        parameter = ts.factory.updateParameterDeclaration(item, ts.getModifiers(item),
          item.dotDotDotToken, item.name, item.questionToken,
          ts.factory.createTypeReferenceNode(ts.factory.createIdentifier(
            parentComponentName.getText() + INTERFACE_NAME_SUFFIX), undefined), item.initializer);
        break;
      case COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU:
        parameter = ts.factory.createParameterDeclaration(ts.getModifiers(item), item.dotDotDotToken,
          item.name, ts.factory.createToken(ts.SyntaxKind.QuestionToken), ts.factory.createTypeReferenceNode(
            ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_LOCALSTORAGE_TYPE_PU), undefined), item.initializer);
        break;
    }
    newTSPara.push(parameter);
  });
  return newTSPara;
}

export function addConstructor(ctorNode: any, watchMap: Map<string, ts.Node>,
  parentComponentName: ts.Identifier): ts.ConstructorDeclaration {
  const watchStatements: ts.ExpressionStatement[] = [];
  watchMap.forEach((value, key) => {
    const watchNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(
      ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(COMPONENT_WATCH_FUNCTION)
        ),
        undefined,
        [
          ts.factory.createStringLiteral(key),
          ts.isStringLiteral(value) ?
            ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
              ts.factory.createIdentifier(value.text)) : value as ts.PropertyAccessExpression
        ]
      ));
    watchStatements.push(watchNode);
  });
  const callSuperStatement: ts.Statement = createCallSuperStatement();
  const updateWithValueParamsStatement: ts.Statement = createUPdWithValStatement();
  const newBody: ts.Statement[] = [updateWithValueParamsStatement, ...watchStatements];
  if (partialUpdateConfig.partialUpdateMode) {
    newBody.push(createAstNodeUtils.createFinalizeConstruction());
  }
  return updateConstructor(updateConstructor(ctorNode, [], [callSuperStatement], [], true), [],
    newBody, [], false, true, parentComponentName);
}

function createCallSuperStatement(): ts.Statement {
  if (!partialUpdateConfig.partialUpdateMode) {
    return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createSuper(), undefined,
      [ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_ID),
        ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARENT),
        ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_LOCALSTORAGE)]));
  } else {
    return ts.factory.createExpressionStatement(
      ts.factory.createCallExpression(ts.factory.createSuper(), undefined,
        [ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARENT),
          ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU),
          ts.factory.createIdentifier(ELMTID),
          ts.factory.createIdentifier(CUSTOM_COMPONENT_EXTRAINFO)]));
  }
}

function createUPdWithValStatement(): ts.Statement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createThis(),
        ts.factory.createIdentifier(
          !partialUpdateConfig.partialUpdateMode ?
            COMPONENT_CONSTRUCTOR_UPDATE_PARAMS : COMPONENT_CONSTRUCTOR_INITIAL_PARAMS
        )
      ),
      undefined,
      [ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS)]
    )
  );
}
