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
  addLog,
  LogType,
  LogInfo
} from './utils';
import { ParentType } from './process_custom_component';
import constantDefine from './constant_define';

function checkLocalBuilderDecoratorCount(node: ts.Node, sourceFileNode: ts.SourceFile, checkDecoratorCount: number, log: LogInfo[]): void {
  if (checkDecoratorCount > 0) {
    const message: string = 'The member property or method can not be decorated by multiple decorators.';
    addLog(LogType.ERROR, message, node.getStart(), log, sourceFileNode, { code: '10905125' });
  }
}

function checkTwoWayComputed(node: ts.PropertyAccessExpression, symbol: ts.Symbol, log: LogInfo[]): void {
  if (symbol && symbol.declarations) {
    symbol.declarations.forEach((declaration: ts.Declaration) => {
      if (ts.isGetAccessor(declaration) && declaration.modifiers && 
        isTagWithDecorator(declaration.modifiers, constantDefine.COMPUTED)) {
        log.push({
          type: LogType.ERROR,
          message: `A property decorated by '${constantDefine.COMPUTED_DECORATOR}' cannot be used with two-bind syntax.`,
          pos: node.getStart(),
          code: '10905129'
        });
      }
    });
  }
}

function checkComputedGetter(symbol: ts.Symbol, declaration: ts.Declaration, log: LogInfo[]): void {
  if (ts.isSetAccessor(declaration) && declaration.name && ts.isIdentifier(declaration.name) &&
    symbol.escapedName.toString() === declaration.name.escapedText.toString()) {
    log.push({
      type: LogType.ERROR,
      message: `A property decorated by '${constantDefine.COMPUTED_DECORATOR}' cannot define a set method.`,
      pos: declaration.getStart(),
      code: '10905130'
    });
  }
}

function checkIfNeedDollarEvent(doubleExclamationCollection: string[], dollarPropertyCollection: string[], 
  node: ts.CallExpression, log: LogInfo[]): void {
  for (const item of doubleExclamationCollection) {
    if (dollarPropertyCollection.some((value: string) => value === '$' + item)) {
      log.push({
        type: LogType.ERROR,
        message: `When the two-way binding syntax is used, do not assign a value to '${constantDefine.EVENT_DECORATOR}'` +
          ` variable '${'$' + item}' because the framework generates the default assignment.`,
        pos: node.getStart(),
        code: '10905358'
      });
    }
  }
}

function checkIfAssignToStaticProps(node: ts.ObjectLiteralElementLike, propName: string,
  staticCollection: Set<string>, log: LogInfo[]): void {
  if (staticCollection.has(propName)) {
    log.push({
      type: LogType.WARN,
      message: `Static property '${propName}' can not be initialized through the component constructor.`,
      pos: node.getStart()
    });
  }
}

function checkNestedComponents(parentComponentType: ParentType, isRecycleChild: boolean, isReuseV2Child: boolean,
  node: ts.ExpressionStatement, log: LogInfo[]): void {
  if (parentComponentType === ParentType.NormalComponentV1 && isReuseV2Child) {
    log.push({
      type: LogType.ERROR,
      message: `A custom component decorated with '@Component' cannot contain child components decorated with '@ReusableV2'.`,
      pos: node.getStart(),
      code: '10905244'
    });
  }
  if (parentComponentType === ParentType.ReuseComponentV1 && isReuseV2Child) {
    log.push({
      type: LogType.ERROR,
      message: `A custom component decorated with '@Reusable' cannot contain child components decorated with '@ReusableV2'.`,
      pos: node.getStart(),
      code: '10905245'
    });
  }
  if (parentComponentType === ParentType.ReuseComponentV2 && isRecycleChild) {
    log.push({
      type: LogType.ERROR,
      message: `A custom component decorated with '@ReusableV2' cannot contain child components decorated with '@Reusable'.`,
      pos: node.getStart(),
      code: '10905246'
    });
  }
  if (parentComponentType === ParentType.NormalComponentV2 && isRecycleChild) {
    log.push({
      type: LogType.WARN,
      message: `When a custom component is decorated with '@ComponentV2' and contains a child decorated with '@Reusable', ` + 
        `the child component will not create.`,
      pos: node.getStart()
    });
  }
}

function checkIfReuseV2InRepeatTemplate(isInRepeatTemplate: boolean, isReuseV2Child: boolean,
  node: ts.ExpressionStatement, log: LogInfo[]): void {
  if (isInRepeatTemplate && isReuseV2Child) {
    log.push({
      type: LogType.ERROR,
      message: `The template attribute of the Repeat component cannot contain any custom component decorated with '@ReusableV2'.`,
      pos: node.getStart(),
      code: '10905247'
    });
  }
}

function checkUsageOfReuseAttribute(node: ts.CallExpression, isReusableV2NodeAttr: boolean, log: LogInfo[]): void {
  if (!isReusableV2NodeAttr) {
    log.push({
      type: LogType.ERROR,
      message: `The reuse attribute is only applicable to custom components decorated with both '@ComponentV2' and '@ReusableV2'.`,
      pos: node.getStart(),
      code: '10905248'
    });
  }
}

function checkUsageOfReuseIdAttribute(node: ts.CallExpression, isReusableV2NodeAttr: boolean, log: LogInfo[]): void {
  if (isReusableV2NodeAttr) {
    log.push({
      type: LogType.ERROR,
      message: `The reuseId attribute is not applicable to custom components decorated with both '@ComponentV2' and '@ReusableV2'.`,
      pos: node.getStart(),
      code: '10905249'
    });
  }
}

function isTagWithDecorator(node: ts.NodeArray<ts.ModifierLike>, decoratorName: string): boolean {
  return node.some((item: ts.Decorator) => ts.isDecorator(item) && 
    ts.isIdentifier(item.expression) && item.expression.escapedText.toString() === decoratorName);
}

export default {
  checkLocalBuilderDecoratorCount,
  checkTwoWayComputed,
  checkComputedGetter,
  checkIfNeedDollarEvent,
  checkIfAssignToStaticProps,
  checkNestedComponents,
  checkIfReuseV2InRepeatTemplate,
  checkUsageOfReuseAttribute,
  checkUsageOfReuseIdAttribute
};
