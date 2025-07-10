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
  LogInfo,
  LogType,
  addLog,
  removeDecorator
} from './utils';
import {
  COMPONENT_CONSTRUCTOR_PARENT,
  COMPONENT_CONSTRUCTOR_PARAMS,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU,
  ELMTID,
  COMPONENT_PARAMS_LAMBDA_FUNCTION,
  CUSTOM_COMPONENT_EXTRAINFO,
  COMPONENT_CONSTRUCTOR_UNDEFINED,
  REUSABLE_V2_INNER_DECORATOR,
  REFLECT,
  DEFINE_PROPERTY,
  BASE_CLASS,
  PROTOTYPE,
  IS_REUSABLE_,
  GET_ATTRIBUTE,
} from './pre_define';
import constantDefine from './constant_define';
import createAstNodeUtils from './create_ast_node_utils';
import {
  updateHeritageClauses,
  processComponentMethod,
  BuildCount,
  addRerenderFunc,
  validateBuildMethodCount,
  getEntryNameFunction,
  FreezeParamType,
  decoratorAssignParams
} from './process_component_class';
import { isReuseInV2 } from './process_custom_component';
import { judgeBuilderParamAssignedByBuilder } from './process_component_member';
import {
  componentCollection,
  builderParamObjectCollection
} from './validate_ui_syntax';
import logMessageCollection from './log_message_collection';
import { globalProgram } from '../main';

export class ParamDecoratorInfo {
  initializer: ts.Expression;
  hasRequire: boolean = false;
}

export class StructInfo {
  isComponentV1: boolean = false;
  isComponentV2: boolean = false;
  isCustomDialog: boolean = false;
  isReusable: boolean = false;
  isReusableV2: boolean = false;
  structName: string = '';
  updatePropsDecoratorsV1: string[] = [];
  linkDecoratorsV1: string[] = [];
  paramDecoratorMap: Map<string, ParamDecoratorInfo> = new Map();
  eventDecoratorMap: Map<string, ts.PropertyDeclaration> = new Map();
  localDecoratorSet: Set<string> = new Set();
  providerDecoratorSet: Set<string> = new Set();
  consumerDecoratorSet: Set<string> = new Set();
  builderParamDecoratorSet: Set<string> = new Set();
  regularSet: Set<string> = new Set();
  propertiesMap: Map<string, ts.Expression> = new Map();
  staticPropertySet: Set<string> = new Set();
  computedDecoratorSet: Set<string> = new Set();
  monitorDecoratorSet: Set<string> = new Set();
}

const structMapInEts: Map<string, StructInfo> = new Map();

function getOrCreateStructInfo(key: string): StructInfo {
  let structInfo: StructInfo = structMapInEts.get(key);
  if (!structInfo) {
    structInfo = new StructInfo();
    structInfo.structName = key;
    structMapInEts.set(key, structInfo);
  }
  return structInfo;
}

/**
 * import * as a from './common'
 * a.struct()
 */
function getAliasStructInfo(node: ts.CallExpression): StructInfo {
  let aliasStructInfo: StructInfo;
  if (node.expression && structMapInEts.has(node.expression.getText())) {
    aliasStructInfo = structMapInEts.get(node.expression.getText());
  }
  return aliasStructInfo;
}

function processStructComponentV2(node: ts.StructDeclaration, log: LogInfo[],
  context: ts.TransformationContext, StateManagementV2: { hasReusableV2: boolean }): ts.ClassDeclaration {
  const isReusableV2: boolean = node.name && ts.isIdentifier(node.name) && isReuseInV2(node.name.getText());
  if (isReusableV2) {
    StateManagementV2.hasReusableV2 = true;
  }
  return ts.factory.createClassDeclaration(isReusableV2 ? 
    ts.concatenateDecoratorsAndModifiers(
      [ts.factory.createDecorator(ts.factory.createIdentifier(REUSABLE_V2_INNER_DECORATOR))], 
      ts.getModifiers(node)
    ) : 
    ts.getModifiers(node), 
    node.name,
    node.typeParameters, 
    updateHeritageClauses(node, log, true),
    processStructMembersV2(node, context, log)
  );
}

function createReusableV2ReflectFunction(): ts.FunctionDeclaration {
  const reflectStatement: ts.ExpressionStatement = ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(REFLECT), ts.factory.createIdentifier(DEFINE_PROPERTY)
      ), undefined,
      [
        ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(BASE_CLASS), ts.factory.createIdentifier(PROTOTYPE)
        ),
        ts.factory.createStringLiteral(IS_REUSABLE_),
        ts.factory.createObjectLiteralExpression([
          ts.factory.createPropertyAssignment(
            ts.factory.createIdentifier(GET_ATTRIBUTE),
            ts.factory.createArrowFunction(
              undefined,
              undefined,
              [],
              undefined,
              ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
              ts.factory.createTrue()
            )
          )], 
          false
        )
      ]
    )
  );
  const parameter: ts.ParameterDeclaration = ts.factory.createParameterDeclaration(
    undefined,
    undefined,
    ts.factory.createIdentifier(BASE_CLASS),
    undefined,
    undefined,
    undefined
  );
  return ts.factory.createFunctionDeclaration(
    undefined,
    undefined,
    ts.factory.createIdentifier(REUSABLE_V2_INNER_DECORATOR),
    undefined,
    [parameter],
    undefined,
    ts.factory.createBlock([reflectStatement])
  );
}

function processStructMembersV2(node: ts.StructDeclaration, context: ts.TransformationContext,
  log: LogInfo[]): ts.ClassElement[] {
  const structName: string = node.name.getText();
  const newMembers: ts.ClassElement[] = [];
  const buildCount: BuildCount = { count: 0 };
  const structInfo: StructInfo = getOrCreateStructInfo(structName);
  const addStatementsInConstructor: ts.Statement[] = [];
  const addStatementsInResetOnReuse: ts.Statement[] = [];
  const paramStatementsInStateVarsMethod: ts.Statement[] = [];
  const structDecorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  const freezeParam: FreezeParamType = { componentFreezeParam: undefined };
  decoratorAssignParams(structDecorators, context, freezeParam);
  traverseStructInfo(structInfo, addStatementsInConstructor, paramStatementsInStateVarsMethod, addStatementsInResetOnReuse);
  node.members.forEach((member: ts.ClassElement) => {
    if (ts.isGetAccessor(member) && member.modifiers?.some(isComputedDecorator) && member.name &&
      ts.isIdentifier(member.name)) {
      const symbol: ts.Symbol = globalProgram.checker?.getSymbolAtLocation(member.name);
      validateComputedGetter(symbol, log);
    }
    if (ts.isConstructorDeclaration(member)) {
      processStructConstructorV2(node.members, newMembers, addStatementsInConstructor, freezeParam);
      createResetStateVarsOnReuse(structInfo, newMembers, addStatementsInResetOnReuse);
      return;
    } else if (ts.isPropertyDeclaration(member)) {
      newMembers.push(processComponentProperty(member, structInfo, log));
      return;
    } else if (ts.isMethodDeclaration(member) && member.name) {
      const newMethodNode: ts.MethodDeclaration = processComponentMethod(member, context, log, buildCount);
      if (newMethodNode) {
        newMembers.push(newMethodNode);
      }
      return;
    }
    newMembers.push(member);
  });
  validateBuildMethodCount(buildCount, node.name, log);
  updateStateVarsMethodNode(paramStatementsInStateVarsMethod, newMembers);
  newMembers.push(addRerenderFunc([]));
  if (componentCollection.entryComponent === structName) {
    newMembers.push(getEntryNameFunction(componentCollection.entryComponent));
  }
  return newMembers;
}

function isComputedDecorator(decorator: ts.Decorator): boolean {
  return ts.isDecorator(decorator) && ts.isIdentifier(decorator.expression) &&
    decorator.expression.escapedText.toString() === constantDefine.COMPUTED;
}

function validateComputedGetter(symbol: ts.Symbol, log: LogInfo[]): void {
  if (symbol && symbol.declarations) {
    symbol.declarations.forEach((declaration: ts.Declaration) => {
      logMessageCollection.checkComputedGetter(symbol, declaration, log);
    });
  }
}

function traverseStructInfo(structInfo: StructInfo,
  addStatementsInConstructor: ts.Statement[], paramStatementsInStateVarsMethod: ts.Statement[],
  addStatementsInResetOnReuse: ts.Statement[]): void {
  const needInitFromParams: string[] = [...structInfo.builderParamDecoratorSet,
    ...structInfo.eventDecoratorMap.keys()];
  for (const property of structInfo.propertiesMap) {
    if (!structInfo.staticPropertySet.has(property[0])) {
      setPropertyStatement(structInfo, addStatementsInConstructor, property[0], property[1],
        needInitFromParams, addStatementsInResetOnReuse);
    }
  }
  for (const param of structInfo.paramDecoratorMap) {
    if (!structInfo.staticPropertySet.has(param[0])) {
      paramStatementsInStateVarsMethod.push(updateParamNode(param[0]));
    }
  }
}

function setPropertyStatement(structInfo: StructInfo, addStatementsInConstructor: ts.Statement[],
  propName: string, initializer: ts.Expression, needInitFromParams: string[], 
  addStatementsInResetOnReuse: ts.Statement[]): void {
  if (needInitFromParams.includes(propName)) {
    if (structInfo.eventDecoratorMap.has(propName)) {
      const eventDeclaration: ts.PropertyDeclaration = structInfo.eventDecoratorMap.get(propName);
      addStatementsInConstructor.push(
        createPropertyAssignNode(propName, initializer || getDefaultValueForEvent(eventDeclaration, false), true));
      addStatementsInResetOnReuse.push(
        createPropertyAssignNode(propName, initializer || getDefaultValueForEvent(eventDeclaration, true), true));
    } else {
      const builderParamExpression: ts.ExpressionStatement = createPropertyAssignNode(propName, initializer, true);
      addStatementsInConstructor.push(builderParamExpression);
      addStatementsInResetOnReuse.push(builderParamExpression);
    }
  } else if (structInfo.paramDecoratorMap.has(propName)) {
    const paramProperty: ParamDecoratorInfo = structInfo.paramDecoratorMap.get(propName);
    addStatementsInConstructor.push(createInitOrUpdateParam(propName, paramProperty.initializer, true));
    addStatementsInResetOnReuse.push(createInitOrUpdateParam(propName, paramProperty.initializer, false));
  } else if (structInfo.consumerDecoratorSet.has(propName)) {
    addStatementsInConstructor.push(createPropertyAssignNode(propName, initializer, false));
    addStatementsInResetOnReuse.push(createResetNode(propName, constantDefine.RESET_CONSUMER, initializer));
  } else if (structInfo.regularSet.has(propName)) {
    addStatementsInConstructor.push(createPropertyAssignNode(propName, initializer, false));
  } else if (structInfo.computedDecoratorSet.has(propName)) {
    addStatementsInResetOnReuse.push(createResetNode(propName, constantDefine.RESET_COMPUTED));
  } else {
    const propertyAssignNode: ts.ExpressionStatement = createPropertyAssignNode(propName, initializer, false);
    addStatementsInConstructor.push(propertyAssignNode);
    addStatementsInResetOnReuse.push(propertyAssignNode);
  }
}

function getDefaultValueForEvent(node: ts.PropertyDeclaration, isResetOnReuse: boolean = false): ts.Expression {
  let param: ts.NodeArray<ts.ParameterDeclaration>;
  if (node.type && ts.isFunctionTypeNode(node.type) && node.type.parameters && node.type.parameters.length) {
    param = node.type.parameters;
  }
  return ts.factory.createArrowFunction(undefined, undefined, isResetOnReuse && param ? param : [], undefined,
    ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
    ts.factory.createBlock([], false));
}

function createPropertyAssignNode(propName: string, initializer: ts.Expression,
  initFromParams: boolean): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(),
      ts.factory.createIdentifier(propName)
    ),
    ts.factory.createToken(ts.SyntaxKind.EqualsToken),
    setInitValue(propName, initializer, initFromParams)
  ));
}

function createResetNode(propName: string, type: string, initializer: ts.Expression = null): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(),
      ts.factory.createIdentifier(type)
    ),
    undefined,
    type === constantDefine.RESET_CONSUMER ? 
    [
      ts.factory.createStringLiteral(propName),
      initializer ? initializer : ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
    ] : [
      ts.factory.createStringLiteral(propName)
    ]
  ));
}

function processComponentProperty(member: ts.PropertyDeclaration, structInfo: StructInfo,
  log: LogInfo[]): ts.PropertyDeclaration {
  const propName: string = member.name.getText();
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(member);
  let initializer: ts.Expression;
  if (!structInfo.regularSet.has(propName) && !member.type) {
    checkV2ComponentMemberType(member.name, propName, log);
  }
  if (structInfo.staticPropertySet.has(propName)) {
    initializer = member.initializer;
  }
  if (structInfo.paramDecoratorMap.has(propName) && structInfo.builderParamDecoratorSet.has(propName)) {
    return processRequireBuilderParamProperty(member, decorators, initializer);
  }
  if (structInfo.paramDecoratorMap.has(propName)) {
    return processParamProperty(member, decorators, initializer);
  }
  if (structInfo.builderParamDecoratorSet.has(propName)) {
    return processBuilderParamProperty(member, log, decorators, initializer);
  }
  return ts.factory.updatePropertyDeclaration(member,
    ts.concatenateDecoratorsAndModifiers(decorators, ts.getModifiers(member)),
    member.name, member.questionToken, member.type, initializer);
}

function checkV2ComponentMemberType(node: ts.Node, propName: string, log: LogInfo[]): void {
  log.push({
    type: LogType.ERROR,
    message: `The property '${propName}' must specify a type.`,
    pos: node.getStart(),
    code: '10905328'
  });
}

function processParamProperty(member: ts.PropertyDeclaration,
  decorators: readonly ts.Decorator[], initializer: ts.Expression): ts.PropertyDeclaration {
  const newDecorators: readonly ts.Decorator[] = removeDecorator(decorators, constantDefine.REQUIRE);
  return ts.factory.updatePropertyDeclaration(member,
    ts.concatenateDecoratorsAndModifiers(newDecorators, ts.getModifiers(member)),
    member.name, member.questionToken, member.type, initializer);
}

function processRequireBuilderParamProperty(member: ts.PropertyDeclaration,
  decorators: readonly ts.Decorator[], initializer: ts.Expression): ts.PropertyDeclaration {
  const tempDecorators: readonly ts.Decorator[] = removeDecorator(decorators, constantDefine.REQUIRE);
  const newDecorators: readonly ts.Decorator[] = removeDecorator(tempDecorators, constantDefine.BUILDER_PARAM);
  return ts.factory.updatePropertyDeclaration(member,
    ts.concatenateDecoratorsAndModifiers(newDecorators, ts.getModifiers(member)),
    member.name, member.questionToken, member.type, initializer);
}

function processBuilderParamProperty(member: ts.PropertyDeclaration, log: LogInfo[],
  decorators: readonly ts.Decorator[], initializer: ts.Expression): ts.PropertyDeclaration {
  if (judgeBuilderParamAssignedByBuilder(member)) {
    log.push({
      type: LogType.ERROR,
      message: `'@BuilderParam' property can only initialized by '@Builder' function.`,
      pos: member.getStart(),
      code: '10905107'
    });
  }
  const newDecorators: readonly ts.Decorator[] = removeDecorator(decorators, constantDefine.BUILDER_PARAM);
  return ts.factory.updatePropertyDeclaration(member,
    ts.concatenateDecoratorsAndModifiers(newDecorators, ts.getModifiers(member)),
    member.name, member.questionToken, member.type, initializer);
}

function setInitValue(propName: string, initializer: ts.Expression,
  initFromParams: boolean): ts.Expression {
  let initNode: ts.Expression = initializer ||
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED);
  if (initFromParams) {
    initNode = createInitNode(propName, initNode);
  }
  return initNode;
}

function createInitNode(propName: string, defaultValue: ts.Expression): ts.Expression {
  return ts.factory.createConditionalExpression(
    ts.factory.createBinaryExpression(
      ts.factory.createStringLiteral(propName),
      ts.factory.createToken(ts.SyntaxKind.InKeyword),
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS)
    ),
    ts.factory.createToken(ts.SyntaxKind.QuestionToken),
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS),
      ts.factory.createIdentifier(propName)
    ),
    ts.factory.createToken(ts.SyntaxKind.ColonToken),
    defaultValue
  );
}

function parseComponentProperty(node: ts.StructDeclaration, structInfo: StructInfo, log: LogInfo[],
  sourceFileNode: ts.SourceFile): void {
  node.members.forEach((member: ts.ClassElement) => {
    if (ts.isPropertyDeclaration(member)) {
      const decorators: readonly ts.Decorator[] = ts.getAllDecorators(member);
      const modifiers: readonly ts.Modifier[] = ts.getModifiers(member);
      structInfo.propertiesMap.set(member.name.getText(), member.initializer);
      parsePropertyDecorator(member, decorators, structInfo, log, sourceFileNode);
      parsePropertyModifiers(member.name.getText(), structInfo, modifiers);
    } else if (ts.isGetAccessor(member)) {
      const decorators: readonly ts.Decorator[] = ts.getAllDecorators(member);
      parseGetAccessor(member, decorators, structInfo);
    } else if (ts.isMethodDeclaration(member)) {
      const decorators: readonly ts.Decorator[] = ts.getAllDecorators(member);
      parseMethodDeclaration(member, decorators, structInfo);
    }
  });
}

function parseGetAccessor(member: ts.GetAccessorDeclaration, decorators: readonly ts.Decorator[],
  structInfo: StructInfo): void {
  if (decorators.length && decorators.some((value: ts.Decorator) => getDecoratorName(value) === constantDefine.COMPUTED_DECORATOR)) {
    structInfo.propertiesMap.set(member.name.getText(), undefined);
    structInfo.computedDecoratorSet.add(member.name.getText());
  }
}

function parseMethodDeclaration(member: ts.MethodDeclaration, decorators: readonly ts.Decorator[],
  structInfo: StructInfo): void {
  if (decorators.length && decorators.some((value: ts.Decorator) => getDecoratorName(value) === constantDefine.MONITOR_DECORATOR)) {
    structInfo.monitorDecoratorSet.add(member.name.getText());
  }
}

function getDecoratorName(decorator: ts.Decorator): string {
  return decorator.getText().replace(/\([^\(\)]*\)/, '').trim();
}

function parsePropertyModifiers(propName: string, structInfo: StructInfo,
  modifiers: readonly ts.Modifier[]): void {
  if (modifiers && modifiers.length) {
    const isStatic: boolean = modifiers.some((item: ts.Modifier) => {
      return item.kind === ts.SyntaxKind.StaticKeyword;
    });
    if (isStatic) {
      structInfo.staticPropertySet.add(propName);
    }
  }
}

class PropertyDecorator {
  hasParam: boolean = false;
  hasRequire: boolean = false;
  hasOnce: boolean = false;
  hasEvent: boolean = false;
}

const decoratorsFunc: Record<string, Function> = {
  'Param': parseParamDecorator,
  'Event': parseEventDecorator,
  'Require': parseRequireDecorator,
  'Once': parseOnceDecorator,
  'Local': parseLocalDecorator,
  'BuilderParam': parseBuilderParamDecorator,
  'Provider': parseProviderDecorator,
  'Consumer': parseConsumerDecorator
};

function parsePropertyDecorator(member: ts.PropertyDeclaration, decorators: readonly ts.Decorator[],
  structInfo: StructInfo, log: LogInfo[], sourceFileNode: ts.SourceFile): void {
  const propertyDecorator: PropertyDecorator = new PropertyDecorator();
  let isRegular: boolean = true;
  for (let i = 0; i < decorators.length; i++) {
    const originalName: string = getDecoratorName(decorators[i]);
    const name: string = originalName.replace('@', '').trim();
    if (decoratorsFunc[name]) {
      decoratorsFunc[name](propertyDecorator, member, structInfo);
    }
    if (constantDefine.COMPONENT_MEMBER_DECORATOR_V2.includes(originalName) ||
      originalName === constantDefine.DECORATOR_BUILDER_PARAM) {
      isRegular = false;
    }
  }
  if (isRegular) {
    structInfo.regularSet.add(member.name.getText());
  }
  checkPropertyDecorator(propertyDecorator, member, log, sourceFileNode, structInfo);
}

function checkPropertyDecorator(propertyDecorator: PropertyDecorator,
  member: ts.PropertyDeclaration, log: LogInfo[], sourceFileNode: ts.SourceFile,
  structInfo: StructInfo): void {
  if (log && sourceFileNode) {
    checkParamDecorator(propertyDecorator, member, log, sourceFileNode, structInfo);
  }
}

function parseParamDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  propertyDecorator.hasParam = true;
  let paramDecoratorInfo: ParamDecoratorInfo = structInfo.paramDecoratorMap.get(member.name.getText());
  if (!paramDecoratorInfo) {
    paramDecoratorInfo = new ParamDecoratorInfo();
  }
  paramDecoratorInfo.initializer = member.initializer;
  structInfo.paramDecoratorMap.set(member.name.getText(), paramDecoratorInfo);
}

function parseEventDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  propertyDecorator.hasEvent = true;
  structInfo.eventDecoratorMap.set(member.name.getText(), member);
}

function parseRequireDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  propertyDecorator.hasRequire = true;
  let paramDecoratorInfo: ParamDecoratorInfo = structInfo.paramDecoratorMap.get(member.name.getText());
  if (!paramDecoratorInfo) {
    paramDecoratorInfo = new ParamDecoratorInfo();
  }
  paramDecoratorInfo.hasRequire = true;
  structInfo.paramDecoratorMap.set(member.name.getText(), paramDecoratorInfo);
}

function parseOnceDecorator(propertyDecorator: PropertyDecorator): void {
  propertyDecorator.hasOnce = true;
}

function parseLocalDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  structInfo.localDecoratorSet.add(member.name.getText());
}

function parseProviderDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  structInfo.providerDecoratorSet.add(member.name.getText());
}

function parseConsumerDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  structInfo.consumerDecoratorSet.add(member.name.getText());
}

function parseBuilderParamDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  structInfo: StructInfo): void {
  let builderParamSet: Set<string> = builderParamObjectCollection.get(structInfo.structName);
  if (!builderParamSet) {
    builderParamSet = new Set();
  }
  builderParamSet.add(member.name.getText());
  builderParamObjectCollection.set(structInfo.structName, builderParamSet);
  structInfo.builderParamDecoratorSet.add(member.name.getText());
}

function checkHasBuilderParamDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration,
  sourceFileNode: ts.SourceFile, structInfo: StructInfo): boolean {
  let checkResult: boolean = false;
  if (StructInfo) {
    checkResult = structInfo.builderParamDecoratorSet.has(member.name.getText());
  }
  return checkResult;
}

function checkParamDecorator(propertyDecorator: PropertyDecorator, member: ts.PropertyDeclaration, log: LogInfo[],
  sourceFileNode: ts.SourceFile, structInfo: StructInfo): void {
  if (propertyDecorator.hasParam && !member.initializer && !propertyDecorator.hasRequire) {
    const message: string = 'When a variable decorated with \'@Param\' is not assigned a default value, ' +
      'it must also be decorated with \'@Require\'.';
    addLog(LogType.ERROR, message, member.getStart(), log, sourceFileNode, { code: '10905327' });
  }
  if (propertyDecorator.hasOnce && !propertyDecorator.hasParam) {
    const message: string = 'When a variable decorated with \'@Once\', it must also be decorated with \'@Param\'.';
    addLog(LogType.ERROR, message, member.getStart(), log, sourceFileNode, { code: '10905326' });
  }
  if (propertyDecorator.hasRequire && !propertyDecorator.hasParam && !checkHasBuilderParamDecorator(propertyDecorator,
    member, sourceFileNode, structInfo)) {
    const message: string = 'In a struct decorated with \'@ComponentV2\', \'@Require\' can only be used with \'@Param\'.';
    addLog(LogType.ERROR, message, member.getStart(), log, sourceFileNode, { code: '10905325' });
  }
}

function processStructConstructorV2(members: ts.NodeArray<ts.ClassElement>, newMembers: ts.ClassElement[],
  paramStatements: ts.Statement[], freezeParam: FreezeParamType): void {
  const freezeParamNode: ts.Expression = freezeParam.componentFreezeParam ?
    freezeParam.componentFreezeParam : undefined;
  const constructorIndex: number = members.findIndex((item: ts.ClassElement) => {
    return ts.isConstructorDeclaration(item);
  });
  if (constructorIndex !== -1) {
    const constructorNode: ts.ConstructorDeclaration = members[constructorIndex] as ts.ConstructorDeclaration;
    newMembers.splice(constructorIndex, 0, ts.factory.updateConstructorDeclaration(constructorNode, ts.getModifiers(constructorNode),
      createConstructorParams(), updateConstructorBody(constructorNode.body, paramStatements, freezeParamNode)));
  }
}

function createConstructorParams(): ts.ParameterDeclaration[] {
  const paramNames: string[] = [COMPONENT_CONSTRUCTOR_PARENT, COMPONENT_CONSTRUCTOR_PARAMS,
    COMPONENT_CONSTRUCTOR_LOCALSTORAGE_PU, ELMTID, COMPONENT_PARAMS_LAMBDA_FUNCTION,
    CUSTOM_COMPONENT_EXTRAINFO];
  return paramNames.map((name: string) => {
    return createAstNodeUtils.createParameterDeclaration(name);
  });
}

function updateConstructorBody(node: ts.Block, paramStatements: ts.Statement[],
  freezeParamNode: ts.Expression): ts.Block {
  const body: ts.Statement[] = [createSuperV2()];
  if (node.statements) {
    body.push(...node.statements);
  }
  body.push(...paramStatements, createAstNodeUtils.createFinalizeConstruction(freezeParamNode));
  return ts.factory.createBlock(body, true);
}

function createResetStateVarsOnReuse(structInfo: StructInfo, newMembers: ts.ClassElement[], 
  addStatementsInResetOnReuse: ts.Statement[]): void {
  if (structInfo.monitorDecoratorSet.size) {
    addStatementsInResetOnReuse.push(generateResetMonitor());
  }
  const resetOnReuseNode: ts.MethodDeclaration = ts.factory.createMethodDeclaration(
    [ts.factory.createToken(ts.SyntaxKind.PublicKeyword)],
    undefined,
    ts.factory.createIdentifier(constantDefine.RESET_STATE_VARS_METHOD),
    undefined,
    undefined,
    [ts.factory.createParameterDeclaration(
      undefined,
      undefined,
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS),
      undefined,
      ts.factory.createTypeReferenceNode(
        ts.factory.createIdentifier(constantDefine.OBJECT_TYPE),
        undefined
      ),
      undefined
    )],
    ts.factory.createKeywordTypeNode(ts.SyntaxKind.VoidKeyword),
    ts.factory.createBlock(
      addStatementsInResetOnReuse,
      true
    )
  );
  newMembers.push(resetOnReuseNode);
}

function generateResetMonitor(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(),
      ts.factory.createIdentifier(constantDefine.RESET_MONITORS_ON_REUSE)
    ),
    undefined,
    []
  ));
}

function createSuperV2(): ts.Statement {
  const paramNames: string[] = [COMPONENT_CONSTRUCTOR_PARENT, ELMTID, CUSTOM_COMPONENT_EXTRAINFO];
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createSuper(), undefined, paramNames.map((name: string) => {
      return ts.factory.createIdentifier(name);
    })));
}

function createInitOrUpdateParam(propName: string, initializer: ts.Expression, isInit: boolean): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
      ts.factory.createIdentifier(isInit ? constantDefine.INIT_PARAM : constantDefine.RESET_PARAM)),
    undefined,
    [
      ts.factory.createStringLiteral(propName),
      ts.factory.createConditionalExpression(ts.factory.createParenthesizedExpression(
        ts.factory.createBinaryExpression(ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS),
          ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
          createParamBinaryNode(propName)
        )),
      ts.factory.createToken(ts.SyntaxKind.QuestionToken),
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS), ts.factory.createIdentifier(propName)
      ),
      ts.factory.createToken(ts.SyntaxKind.ColonToken),
      initializer || ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
      )
    ]));
}

function updateParamNode(propName: string): ts.IfStatement {
  return ts.factory.createIfStatement(createParamBinaryNode(propName),
    ts.factory.createBlock([
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
          ts.factory.createIdentifier(constantDefine.UPDATE_PARAM)),
        undefined,
        [
          ts.factory.createStringLiteral(propName),
          ts.factory.createPropertyAccessExpression(
            ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS), ts.factory.createIdentifier(propName)
          )
        ]))], true));
}

function createParamBinaryNode(propName: string): ts.BinaryExpression {
  return ts.factory.createBinaryExpression(
    ts.factory.createStringLiteral(propName),
    ts.factory.createToken(ts.SyntaxKind.InKeyword),
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS)
  );
}

function updateStateVarsMethodNode(paramStatements: ts.Statement[], newMembers: ts.ClassElement[]): void {
  if (paramStatements.length) {
    newMembers.push(ts.factory.createMethodDeclaration([ts.factory.createToken(ts.SyntaxKind.PublicKeyword)],
      undefined, ts.factory.createIdentifier(constantDefine.UPDATE_STATE_VARS), undefined, undefined,
      [createAstNodeUtils.createParameterDeclaration(COMPONENT_CONSTRUCTOR_PARAMS)], undefined,
      ts.factory.createBlock([emptyJudgeForParamsNode(), ...paramStatements], true)));
  }
}

function emptyJudgeForParamsNode(): ts.IfStatement {
  return ts.factory.createIfStatement(ts.factory.createBinaryExpression(
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARAMS),
    ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
  ), ts.factory.createBlock([ts.factory.createReturnStatement(undefined)], true), undefined);
}

function resetStructMapInEts(): void {
  structMapInEts.clear();
}

export default {
  getOrCreateStructInfo: getOrCreateStructInfo,
  processStructComponentV2: processStructComponentV2,
  parseComponentProperty: parseComponentProperty,
  resetStructMapInEts: resetStructMapInEts,
  getAliasStructInfo: getAliasStructInfo,
  createReusableV2ReflectFunction: createReusableV2ReflectFunction
};
