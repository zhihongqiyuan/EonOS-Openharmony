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
  COMPONENT_NON_DECORATOR,
  COMPONENT_STATE_DECORATOR,
  COMPONENT_PROP_DECORATOR,
  COMPONENT_LINK_DECORATOR,
  COMPONENT_STORAGE_LINK_DECORATOR,
  COMPONENT_PROVIDE_DECORATOR,
  COMPONENT_OBJECT_LINK_DECORATOR,
  COMPONENT_CREATE_FUNCTION,
  COMPONENT_POP_FUNCTION,
  BASE_COMPONENT_NAME,
  CUSTOM_COMPONENT_EARLIER_CREATE_CHILD,
  COMPONENT_CONSTRUCTOR_UPDATE_PARAMS,
  CUSTOM_COMPONENT_FUNCTION_FIND_CHILD_BY_ID,
  COMPONENT_CONSTRUCTOR_UNDEFINED,
  CUSTOM_COMPONENT_NEEDS_UPDATE_FUNCTION,
  CUSTOM_COMPONENT_MARK_STATIC_FUNCTION,
  COMPONENT_COMMON,
  GENERATE_ID,
  ELMTID,
  STARTGETACCESSRECORDINGFOR,
  STOPGETACCESSRECORDING,
  BASE_COMPONENT_NAME_PU,
  OBSERVECOMPONENTCREATION,
  OBSERVECOMPONENTCREATION2,
  ISINITIALRENDER,
  UPDATE_STATE_VARS_OF_CHIND_BY_ELMTID,
  $$,
  COMPONENT_RECYCLE,
  COMPONENT_CREATE_RECYCLE,
  RECYCLE_NODE,
  ABOUT_TO_REUSE,
  COMPONENT_RERENDER_FUNCTION,
  OBSERVE_RECYCLE_COMPONENT_CREATION,
  FUNCTION,
  COMPONENT_IF_UNDEFINED,
  COMPONENT_PARAMS_LAMBDA_FUNCTION,
  COMPONENT_PARAMS_FUNCTION,
  COMPONENT_ABOUTTOREUSEINTERNAL_FUNCTION,
  NAME,
  COMPONENT_CALL,
  COMPONENT_CONSUME_DECORATOR,
  COMPONENT_STORAGE_PROP_DECORATOR,
  COMPONENT_LOCAL_STORAGE_PROP_DECORATOR,
  COMPONENT_LOCAL_STORAGE_LINK_DECORATOR,
  COMPONENTV2_LOCAL_DECORATOR,
  COMPONENTV2_CONSUMER_DECORATOR,
  COMPONENTV2_PROVIDER_DECORATOR
} from './pre_define';
import {
  stateCollection,
  linkCollection,
  propCollection,
  regularCollection,
  storagePropCollection,
  storageLinkCollection,
  provideCollection,
  consumeCollection,
  objectLinkCollection,
  isStaticViewCollection,
  builderParamObjectCollection,
  getLocalStorageCollection,
  builderParamInitialization,
  propInitialization,
  regularInitialization,
  stateInitialization,
  provideInitialization,
  privateCollection,
  regularStaticCollection,
  componentCollection,
  localStorageLinkCollection,
  localStoragePropCollection
} from './validate_ui_syntax';
import {
  PropMapManager,
  createViewCreate,
  createCustomComponentNewExpression,
  isLocalStorageParameter,
  isBasicType,
  isObservedV2
} from './process_component_member';
import {
  LogType,
  LogInfo,
  componentInfo,
  storedFileInfo
} from './utils';
import {
  bindComponentAttr,
  parentConditionalExpression,
  createComponentCreationStatement,
  createFunction,
  ComponentAttrInfo,
  ifRetakeId,
  transferBuilderCall,
  createCollectElmtIdNode,
  createViewStackProcessorStatement,
  BuilderParamsResult
} from './process_component_build';
import {
  partialUpdateConfig,
  projectConfig,
  globalProgram
} from '../main';
import {
  GLOBAL_CUSTOM_BUILDER_METHOD
} from './component_map';
import {
  createReference,
  isProperty
} from './process_component_class';
import processStructComponentV2, { StructInfo, ParamDecoratorInfo } from './process_struct_componentV2';
import constantDefine from './constant_define';
import createAstNodeUtils from './create_ast_node_utils';
import logMessageCollection from './log_message_collection';

let decoractorMap: Map<string, Map<string, Set<string>>>;

export function processCustomComponent(node: ts.ExpressionStatement, newStatements: ts.Statement[],
  log: LogInfo[], name: string, isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  idName: ts.Expression = undefined, builderParamsResult: BuilderParamsResult = null,
  isInRepeatTemplate: boolean = false): void {
  decoractorMap = new Map(
    [[COMPONENT_STATE_DECORATOR, stateCollection],
      [COMPONENT_LINK_DECORATOR, linkCollection],
      [COMPONENT_PROP_DECORATOR, propCollection],
      [COMPONENT_NON_DECORATOR, regularCollection],
      [COMPONENT_PROVIDE_DECORATOR, provideCollection],
      [COMPONENT_OBJECT_LINK_DECORATOR, objectLinkCollection]]);
  const componentNode: ts.CallExpression = getCustomComponentNode(node);
  if (componentNode) {
    const parentComponentType: ParentType = componentCollection.currentClassName ? 
    getParentComponentType(componentCollection.currentClassName) : getParentComponentType('');
    const isRecycleComponent: boolean = isRecycle(name);
    const isReuseComponentInV2: boolean = isReuseInV2(name);
    logMessageCollection.checkNestedComponents(parentComponentType, isRecycleComponent, isReuseComponentInV2, node, log);
    logMessageCollection.checkIfReuseV2InRepeatTemplate(isInRepeatTemplate, isReuseComponentInV2, node, log);
    const hasChainCall: boolean = componentNode.parent &&
      ts.isPropertyAccessExpression(componentNode.parent);
    let ischangeNode: boolean = false;
    let customComponentNewExpression: ts.NewExpression = createCustomComponentNewExpression(
      componentNode, name, isBuilder, isGlobalBuilder);
    let argumentsArray: ts.PropertyAssignment[];
    const componentAttrInfo: ComponentAttrInfo = { reuseId: null, hasIdAttr: false, attrCount: 0, reuse: '' };
    if (isHasChild(componentNode)) {
      // @ts-ignore
      argumentsArray = componentNode.arguments[0].properties.slice();
      argumentsArray.forEach((item: ts.PropertyAssignment, index: number) => {
        if (isToChange(item, name)) {
          ischangeNode = true;
          const propertyAssignmentNode: ts.PropertyAssignment = ts.factory.updatePropertyAssignment(
            item, item.name, changeNodeFromCallToArrow(item.initializer as ts.CallExpression));
          argumentsArray.splice(index, 1, propertyAssignmentNode);
        }
      });
      if (ischangeNode) {
        const newNode: ts.ExpressionStatement = ts.factory.updateExpressionStatement(node,
          ts.factory.createNewExpression(componentNode.expression, componentNode.typeArguments,
            [ts.factory.createObjectLiteralExpression(argumentsArray, true)]));
        customComponentNewExpression = createCustomComponentNewExpression(
          newNode.expression as ts.CallExpression, name, isBuilder);
      }
    }
    let judgeIdStart: number;
    if (partialUpdateConfig.partialUpdateMode && idName) {
      judgeIdStart = newStatements.length;
    }
    let needCommon: boolean = false;
    if (hasChainCall) {
      if (partialUpdateConfig.partialUpdateMode) {
        const commomComponentNode: ts.Statement[] = [ts.factory.createExpressionStatement(
          createFunction(ts.factory.createIdentifier(COMPONENT_COMMON),
            ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), null))];
        const immutableStatements: ts.Statement[] = [];
        bindComponentAttr(node, ts.factory.createIdentifier(COMPONENT_COMMON), commomComponentNode,
          log, true, false, immutableStatements, false, componentAttrInfo, isReuseComponentInV2);
        needCommon = commomComponentNode.length > 1 || immutableStatements.length > 0;
        if (componentAttrInfo.hasIdAttr && componentAttrInfo.attrCount === 1) {
          commomComponentNode[0] = createCommonIdAttrNode();
        }
        if (needCommon) {
          newStatements.push(createComponentCreationStatement(componentAttributes(COMPONENT_COMMON),
            commomComponentNode, COMPONENT_COMMON, isGlobalBuilder, false, undefined, immutableStatements,
            builderParamsResult, isRecycleComponent));
        }
      } else {
        newStatements.push(ts.factory.createExpressionStatement(
          createFunction(ts.factory.createIdentifier(COMPONENT_COMMON),
            ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), null)));
        bindComponentAttr(node, ts.factory.createIdentifier(COMPONENT_COMMON), newStatements, log);
      }
    }
    if (isRecycleComponent && partialUpdateConfig.partialUpdateMode) {
      newStatements.push(createRecycleComponent(isGlobalBuilder));
    }
    addCustomComponent(node, newStatements, customComponentNewExpression, log, name, componentNode,
      isBuilder, isGlobalBuilder, isRecycleComponent, componentAttrInfo, builderParamsResult, isReuseComponentInV2);
    if (hasChainCall && (!partialUpdateConfig.partialUpdateMode || needCommon)) {
      newStatements.push(ts.factory.createExpressionStatement(
        createFunction(ts.factory.createIdentifier(COMPONENT_COMMON),
          ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null)));
    }
    if (isRecycleComponent && partialUpdateConfig.partialUpdateMode) {
      newStatements.push(componentAttributes(COMPONENT_RECYCLE));
    }
    if (partialUpdateConfig.partialUpdateMode && idName) {
      newStatements.splice(judgeIdStart, newStatements.length - judgeIdStart,
        ifRetakeId(newStatements.slice(judgeIdStart), idName));
    }
  }
}

function createCommonIdAttrNode(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(
    createFunction(ts.factory.createIdentifier(COMPONENT_COMMON),
      ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION),
      // @ts-ignore
      [ts.factory.createTrue()]));
}

export function isRecycle(componentName: string): boolean {
  return storedFileInfo.getCurrentArkTsFile().recycleComponents.has(componentName);
}

export function isReuseInV2(componentName: string): boolean {
  return storedFileInfo.getCurrentArkTsFile().reuseComponentsV2.has(componentName);
}

function createRecycleComponent(isGlobalBuilder: boolean): ts.Statement {
  return createComponentCreationStatement(componentAttributes(COMPONENT_RECYCLE),
    [ts.factory.createExpressionStatement(
      createFunction(ts.factory.createIdentifier(COMPONENT_RECYCLE),
        ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), null))
    ], COMPONENT_RECYCLE, isGlobalBuilder);
}

function componentAttributes(componentName: string): ts.Statement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(componentName),
        ts.factory.createIdentifier(COMPONENT_POP_FUNCTION)
      ), undefined, []
    ));
}

function isHasChild(node: ts.CallExpression): boolean {
  return node.arguments && node.arguments[0] && ts.isObjectLiteralExpression(node.arguments[0]) &&
    node.arguments[0].properties && node.arguments[0].properties.length > 0;
}

function isToChange(item: ts.PropertyAssignment, name: string): boolean {
  const builderParamName: Set<string> = builderParamObjectCollection.get(name);
  if (item.initializer && ts.isCallExpression(item.initializer) && builderParamName &&
    builderParamName.has(item.name.getText()) &&
    !/\.(bind|call|apply)/.test(item.initializer.getText())) {
    return true;
  }
  return false;
}

function changeNodeFromCallToArrow(node: ts.CallExpression): ts.ConditionalExpression {
  let builderBindThis: ts.ExpressionStatement = ts.factory.createExpressionStatement(node);
  if (ts.isCallExpression(node) && node.expression && ts.isIdentifier(node.expression) &&
    GLOBAL_CUSTOM_BUILDER_METHOD.has(node.expression.escapedText.toString())) {
    builderBindThis = transferBuilderCall(ts.factory.createExpressionStatement(node), node.expression.escapedText.toString());
  }
  return changeNodeFromCallToArrowDetermine(node, builderBindThis);
}

function changeNodeFromCallToArrowDetermine(node: ts.CallExpression, builderBindThis: ts.ExpressionStatement): ts.ConditionalExpression {
  if (ts.isCallExpression(node)) {
    return ts.factory.createConditionalExpression(
      ts.factory.createBinaryExpression(
        ts.factory.createTypeOfExpression(node),
        ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
        ts.factory.createStringLiteral(FUNCTION)
      ),
      ts.factory.createToken(ts.SyntaxKind.QuestionToken),
      node,
      ts.factory.createToken(ts.SyntaxKind.ColonToken),
      ts.factory.createArrowFunction(
        undefined,
        undefined,
        [],
        undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock(
          [builderBindThis],
          true
        )
      )
    );
  }
  return undefined;
}

function addCustomComponent(node: ts.ExpressionStatement, newStatements: ts.Statement[],
  newNode: ts.NewExpression, log: LogInfo[], name: string, componentNode: ts.CallExpression,
  isBuilder: boolean, isGlobalBuilder: boolean, isRecycleComponent: boolean,
  componentAttrInfo: ComponentAttrInfo, builderParamsResult: BuilderParamsResult,
  isReuseComponentInV2: boolean): void {
  if (ts.isNewExpression(newNode)) {
    const propertyArray: ts.ObjectLiteralElementLike[] = [];
    validateCustomComponentPrams(componentNode, name, propertyArray, log, isBuilder);
    addCustomComponentStatements(node, newStatements, newNode, name, propertyArray, componentNode,
      isBuilder, isGlobalBuilder, isRecycleComponent, componentAttrInfo, builderParamsResult, log, isReuseComponentInV2);
  }
}

function addCustomComponentStatements(node: ts.ExpressionStatement, newStatements: ts.Statement[],
  newNode: ts.NewExpression, name: string, props: ts.ObjectLiteralElementLike[],
  componentNode: ts.CallExpression, isBuilder: boolean, isGlobalBuilder: boolean,
  isRecycleComponent: boolean, componentAttrInfo: ComponentAttrInfo,
  builderParamsResult: BuilderParamsResult, log: LogInfo[], isReuseComponentInV2: boolean): void {
  if (!partialUpdateConfig.partialUpdateMode) {
    const id: string = componentInfo.id.toString();
    newStatements.push(createFindChildById(id, name, isBuilder), createCustomComponentIfStatement(id,
      ts.factory.updateExpressionStatement(node, createViewCreate(newNode)),
      ts.factory.createObjectLiteralExpression(props, true), name));
  } else {
    newStatements.push(createCustomComponent(newNode, name, componentNode, isGlobalBuilder, isBuilder,
      isRecycleComponent, componentAttrInfo, builderParamsResult, log, isReuseComponentInV2));
  }
}

function createChildElmtId(node: ts.CallExpression, name: string, log: LogInfo[]): ts.PropertyAssignment[] {
  const childParam: ts.PropertyAssignment[] = [];
  const propsAndObjectLinks: string[] = [];
  if (propCollection.get(name)) {
    propsAndObjectLinks.push(...propCollection.get(name));
  }
  if (objectLinkCollection.get(name)) {
    propsAndObjectLinks.push(...objectLinkCollection.get(name));
  }
  if (projectConfig.optLazyForEach && storedFileInfo.processLazyForEach && stateCollection.get(name)) {
    propsAndObjectLinks.push(...stateCollection.get(name));
  }
  parseChildProperties(name, node, childParam, propsAndObjectLinks, log);
  return childParam;
}

class ChildAndParentComponentInfo {
  childStructInfo: StructInfo;
  parentStructInfo: StructInfo;
  paramDecoratorMap: Map<string, ParamDecoratorInfo>;
  updatePropsDecoratorsV2: string[];
  propsAndObjectLinks: string[];
  childName: string;
  forbiddenInitPropsV2: string[];
  updatePropsForV1Parent: string[];
  updatePropsForV2Parent: string[];
  constructor(childName: string, childNode: ts.CallExpression, propsAndObjectLinks: string[]) {
    this.childName = childName;
    this.propsAndObjectLinks = propsAndObjectLinks;
    this.childStructInfo = processStructComponentV2.getAliasStructInfo(childNode) ||
      processStructComponentV2.getOrCreateStructInfo(childName);
    this.paramDecoratorMap = this.childStructInfo.paramDecoratorMap;
    this.updatePropsDecoratorsV2 = [...this.childStructInfo.eventDecoratorMap.keys(), ...this.paramDecoratorMap.keys()];
    this.parentStructInfo = componentCollection.currentClassName ?
      processStructComponentV2.getOrCreateStructInfo(componentCollection.currentClassName) :
      new StructInfo();
    this.forbiddenInitPropsV2 = [...this.childStructInfo.localDecoratorSet,
      ...this.childStructInfo.providerDecoratorSet, ...this.childStructInfo.consumerDecoratorSet,
      ...this.childStructInfo.regularSet];
    this.updatePropsForV1Parent = getUpdatePropsForV1Parent();
    this.updatePropsForV2Parent = [...this.parentStructInfo.localDecoratorSet,
      ...this.parentStructInfo.paramDecoratorMap.keys(), ...this.parentStructInfo.providerDecoratorSet,
      ...this.parentStructInfo.consumerDecoratorSet];
  }
}

function getUpdatePropsForV1Parent(): string[] {
  const propertiesMapArr: Array<Map<string, Set<string>>> = [
    stateCollection, linkCollection, propCollection,
    provideCollection, consumeCollection, objectLinkCollection,
    storagePropCollection, storageLinkCollection
  ];
  const updatePropsForParent: string[] = [];
  if (componentCollection.currentClassName) {
    const localStorageSet: Set<string> = new Set();
    getLocalStorageCollection(componentCollection.currentClassName, localStorageSet);
    updatePropsForParent.push(...localStorageSet);
    propertiesMapArr.forEach((item: Map<string, Set<string>>) => {
      const value: Set<string> = item.get(componentCollection.currentClassName);
      if (value) {
        updatePropsForParent.push(...value);
      }
    });
  }
  return updatePropsForParent;
}

function parseChildProperties(childName: string, node: ts.CallExpression,
  childParam: ts.PropertyAssignment[], propsAndObjectLinks: string[], log: LogInfo[]): void {
  const childAndParentComponentInfo: ChildAndParentComponentInfo =
    new ChildAndParentComponentInfo(childName, node, propsAndObjectLinks);
  if (node.arguments[0].properties) {
    node.arguments[0].properties.forEach((item: ts.PropertyAssignment) => {
      if (ts.isIdentifier(item.name)) {
        const itemName: string = item.name.escapedText.toString();
        validateChildProperty(item, itemName, childParam, log, childAndParentComponentInfo);
      }
    });
  }
}

function getForbbidenInitPropsV2Type(itemName: string, info: ChildAndParentComponentInfo): string {
  let typeName: string = COMPONENT_NON_DECORATOR;
  if (info.childStructInfo.localDecoratorSet.has(itemName)) {
    typeName = COMPONENTV2_LOCAL_DECORATOR;
  } else if (info.childStructInfo.consumerDecoratorSet.has(itemName)) {
    typeName = COMPONENTV2_CONSUMER_DECORATOR;
  } else if (info.childStructInfo.providerDecoratorSet.has(itemName)) {
    typeName = COMPONENTV2_PROVIDER_DECORATOR;
  }
  return typeName;
}

function validateChildProperty(item: ts.PropertyAssignment, itemName: string,
  childParam: ts.PropertyAssignment[], log: LogInfo[], info: ChildAndParentComponentInfo): void {
  if (info.childStructInfo.isComponentV2) {
    if (info.forbiddenInitPropsV2.includes(itemName)) {
      const propType: string = getForbbidenInitPropsV2Type(itemName, info);
      log.push({
        type: LogType.ERROR,
        message: `The '${propType}' property '${itemName}' in the custom component '${info.childName}'` +
          ` cannot be initialized here (forbidden to specify).`,
        pos: item.getStart(),
        code: '10905324'
      });
      return;
    }
    if (info.paramDecoratorMap.has(itemName)) {
      childParam.push(item);
    }
    if (isForbiddenAssignToComponentV2(item, itemName, info)) {
      log.push({
        type: LogType.ERROR,
        message: `Property '${itemName}' in the '@ComponentV2' component '${info.childName}' is not allowed to be assigned value here.`,
        pos: item.getStart(),
        code: '10905323'
      });
    }
  } else {
    if (info.propsAndObjectLinks.includes(itemName)) {
      childParam.push(item);
    }
  }
  logMessageCollection.checkIfAssignToStaticProps(item, itemName, info.childStructInfo.staticPropertySet, log);
}

function isForbiddenTypeToComponentV1(type: ts.Type): boolean {
  // @ts-ignore
  if (type.types && type.types.length) {
    // @ts-ignore
    return !type.types.some((item: ts.Type) => {
      return !isForbiddenTypeToComponentV1(item);
    });
  }
  const allowedTypes: string[] = ['Set', 'Map', 'Date', 'Array'];
  const name: string = type?.getSymbol()?.getName();
  if (name && allowedTypes.includes(name)) {
    return true;
  }
  return false;
}

function isForbiddenAssignToComponentV2(item: ts.PropertyAssignment, itemName: string,
  info: ChildAndParentComponentInfo): boolean {
  if (!info.parentStructInfo.isComponentV2 && info.updatePropsDecoratorsV2.includes(itemName) &&
    isObervedProperty(item.initializer, info) && globalProgram.strictChecker) {
    const type: ts.Type = globalProgram.strictChecker.getTypeAtLocation(item.initializer);
    return !(isAllowedTypeToComponentV2(type) || isForbiddenTypeToComponentV1(type) || !(isObservedV2(type) || isFunctionType(type)));
  }
  return false;
}

// isFunctionType 
export function isFunctionType(type: ts.Type): boolean {
  if (type.types && type.types.length) {
    return !type.types.some((item: ts.Type) => {
      return !isFunctionType(item);
  });
  }
  const name: string = type?.getSymbol()?.getName();
  if (name && name === 'Function') {
    return true;
  }
  return false;
}

function isObervedProperty(value: ts.Expression, info: ChildAndParentComponentInfo,
  isV1Parent: boolean = true): boolean {
  if (value && ts.isPropertyAccessExpression(value) && value.expression.kind === ts.SyntaxKind.ThisKeyword &&
    ts.isIdentifier(value.name)) {
    const propertyName: string = value.name.escapedText.toString();
    return isV1Parent ? info.updatePropsForV1Parent.includes(propertyName) :
      info.updatePropsForV2Parent.includes(propertyName);
  }
  return false;
}

export function isAllowedTypeToComponentV2(type: ts.Type): boolean {
  if (type) {
    // @ts-ignore
    if (type.types && type.types.length) {
      // @ts-ignore
      return type.types.some((item: ts.Type) => {
        return isAllowedTypeToComponentV2(item);
      });
    }
    // string|number|boolean|enum|null|undefined
    if (isAllowedTypeForBasic(type.flags)) {
      return true;
    }
  }
  return false;
}

export function isAllowedTypeForBasic(flags: ts.TypeFlags): boolean {
  if (isBasicType(flags) || (flags & (ts.TypeFlags.Null | ts.TypeFlags.Undefined))) {
    return true;
  }
  return false;
}

function validateInitParam(childName: string, curChildProps: Set<string>,
  node: ts.CallExpression, log: LogInfo[], parentStructInfo: StructInfo): void {
  const childStructInfo: StructInfo = processStructComponentV2.getAliasStructInfo(node) ||
    processStructComponentV2.getOrCreateStructInfo(childName);
  const paramDecoratorMap: Map<string, ParamDecoratorInfo> = childStructInfo.paramDecoratorMap;
  if (childStructInfo.isComponentV2) {
    const needInitParam: string[] = [];
    for (const item of paramDecoratorMap) {
      if (item[1].hasRequire) {
        needInitParam.push(item[0]);
      }
    }
    needInitParam.forEach((paramName: string) => {
      if (!curChildProps.has(paramName)) {
        log.push({
          type: LogType.ERROR,
          message: `'@Require' decorated '${paramName}' must be initialized through the component constructor.`,
          pos: node.getStart(),
          code: '10905321'
        });
      }
    });
  } else if (parentStructInfo.isComponentV2 && childStructInfo.linkDecoratorsV1.length) {
    log.push({
      type: LogType.ERROR,
      message: `A V2 component cannot be used with any member property decorated by '@Link' in a V1 component.`,
      pos: node.getStart(),
      code: '10905213'
    });
  }
}

function createCustomComponent(newNode: ts.NewExpression, name: string, componentNode: ts.CallExpression,
  isGlobalBuilder: boolean, isBuilder: boolean, isRecycleComponent: boolean,
  componentAttrInfo: ComponentAttrInfo, builderParamsResult: BuilderParamsResult, log: LogInfo[], 
  isReuseComponentInV2:boolean): ts.Block {
  let componentParameter: ts.ObjectLiteralExpression;
  if (componentNode.arguments && componentNode.arguments.length) {
    componentParameter = ts.factory.createObjectLiteralExpression(createChildElmtId(componentNode, name, log), true);
  } else {
    componentParameter = ts.factory.createObjectLiteralExpression([], false);
  }
  const arrowArgArr: ts.ParameterDeclaration[] = [
    ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(ELMTID)
    ),
    ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(ISINITIALRENDER)
    )
  ];
  const arrowBolck: ts.Statement[] = [
    projectConfig.optLazyForEach && storedFileInfo.processLazyForEach ? createCollectElmtIdNode() : undefined,
    createIfCustomComponent(newNode, componentNode, componentParameter, name, isGlobalBuilder,
      isBuilder, isRecycleComponent, componentAttrInfo, log)
  ];
  if (isRecycleComponent) {
    arrowArgArr.push(ts.factory.createParameterDeclaration(
      undefined, undefined, ts.factory.createIdentifier(RECYCLE_NODE),
      undefined, undefined, ts.factory.createNull()
    ));
  } else if (partialUpdateConfig.optimizeComponent && isGlobalBuilder &&
    builderParamsResult && builderParamsResult.firstParam) {
    const paramName: ts.Identifier = builderParamsResult.firstParam.name as ts.Identifier;
    arrowArgArr.push(ts.factory.createParameterDeclaration(undefined, undefined,
      paramName, undefined, undefined, ts.factory.createIdentifier(`__${paramName.escapedText.toString()}__`)
    ));
  }
  if (isRecycleComponent || !partialUpdateConfig.optimizeComponent) {
    arrowBolck.unshift(createViewStackProcessorStatement(STARTGETACCESSRECORDINGFOR, ELMTID));
    arrowBolck.push(createViewStackProcessorStatement(STOPGETACCESSRECORDING));
  }
  const observeArgArr: ts.Node[] = [
    ts.factory.createArrowFunction(undefined, undefined, arrowArgArr, undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      ts.factory.createBlock(arrowBolck, true))
  ];
  if (isRecycleComponent) {
    componentAttrInfo.reuseId ? observeArgArr.unshift(componentAttrInfo.reuseId) :
      observeArgArr.unshift(ts.factory.createStringLiteral(name));
  } else if (partialUpdateConfig.optimizeComponent) {
    observeArgArr.push(componentPop(name));
  }
  const reuseOrCreateArgArr: ts.ObjectLiteralExpression[] = [ts.factory.createObjectLiteralExpression(
    generateReuseOrCreateArgArr(componentNode, componentAttrInfo, name, newNode), true)];
  return ts.factory.createBlock(
    [
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(isGlobalBuilder ?
          ts.factory.createParenthesizedExpression(parentConditionalExpression()) : ts.factory.createThis(),
        isRecycleComponent ?
          ts.factory.createIdentifier(OBSERVE_RECYCLE_COMPONENT_CREATION) :
          isReuseComponentInV2 ? ts.factory.createIdentifier(constantDefine.REUSE_OR_CREATE_METHOD) : 
            ts.factory.createIdentifier(partialUpdateConfig.optimizeComponent ?
              OBSERVECOMPONENTCREATION2 : OBSERVECOMPONENTCREATION)
        ),
        undefined, isReuseComponentInV2 ? reuseOrCreateArgArr as ts.Expression[] : observeArgArr as ts.Expression[]))
    ], 
    true
  );
}

function generateReuseOrCreateArgArr(componentNode: ts.CallExpression, componentAttrInfo: ComponentAttrInfo, 
  name: string, newNode: ts.NewExpression): ts.ObjectLiteralElementLike[] {
  const reuseParamsArr: ts.ObjectLiteralElementLike[] = [];
  if (componentNode.expression && ts.isIdentifier(componentNode.expression)) {
    reuseParamsArr.push(ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(constantDefine.COMPONENT_CLASS),
      componentNode.expression
    ));
  }
  reuseParamsArr.push(createReuseParameterArrowFunction(getArgumentsToPass(componentNode), constantDefine.GET_PARAMS, name));
  reuseParamsArr.push(createReuseParameterArrowFunction(
    [], constantDefine.GET_REUSE_ID, componentAttrInfo.reuse ? componentAttrInfo.reuse : name));
  if (newNode.arguments.length >= 6 && ts.isObjectLiteralExpression(newNode.arguments[5])) {
    reuseParamsArr.push(generateExtraInfo(true, newNode.arguments[5]));
  } else {
    reuseParamsArr.push(generateExtraInfo(false));
  }
  return reuseParamsArr;
}

function getArgumentsToPass(componentNode: ts.CallExpression): ts.NodeArray<ts.ObjectLiteralElementLike> | undefined[] {
  if (componentNode.arguments && componentNode.arguments.length === 1 && 
    ts.isObjectLiteralExpression(componentNode.arguments[0]) && componentNode.arguments[0].properties &&
    componentNode.arguments[0].properties.length) {
    return componentNode.arguments[0].properties;
  }
  return [];
}

function createReuseParameterArrowFunction(propertyArray: ts.NodeArray<ts.ObjectLiteralElementLike> | undefined[],
  identifierName: string, name: string): ts.PropertyAssignment {
  return ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier(identifierName),
    ts.factory.createArrowFunction(
      undefined,
      undefined,
      [],
      undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      identifierName === constantDefine.GET_REUSE_ID ? 
        ts.factory.createStringLiteral(name) :
        ts.factory.createParenthesizedExpression(ts.factory.createObjectLiteralExpression(
        propertyArray.length ? propertyArray : [],
        true
      ))
    )
  );
}

function generateExtraInfo(hasPositionInfo: boolean, positionInfo?: ts.ObjectLiteralExpression): ts.PropertyAssignment {
  return ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier(constantDefine.EXTRA_INFO),
    hasPositionInfo ? 
    positionInfo : 
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_UNDEFINED)
  );
}

function componentPop(name: string): ts.ObjectLiteralExpression {
  return ts.factory.createObjectLiteralExpression(
    [ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(NAME),
      ts.factory.createStringLiteral(name)
    )],
    false
  );
}

export function assignComponentParams(componentNode: ts.CallExpression,
  isBuilder: boolean = false): ts.VariableStatement {
  const isParamsLambda: boolean = true;
  const [keyArray, valueArray]: [ts.Node[], ts.Node[]] = splitComponentParams(componentNode, isBuilder, isParamsLambda);
  let integrateParams: boolean = false;
  if (!keyArray.length && componentNode.arguments && componentNode.arguments.length > 0 && componentNode.arguments[0]) {
    integrateParams = true;
  }
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList([ts.factory.createVariableDeclaration(
      ts.factory.createIdentifier(COMPONENT_PARAMS_LAMBDA_FUNCTION),
      undefined,
      undefined,
      ts.factory.createArrowFunction(
        undefined,
        undefined,
        [],
        undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock(
          [ts.factory.createReturnStatement(
            integrateParams ? paramsLambdaCallBack(componentNode) : ts.factory.createObjectLiteralExpression(
              reWriteComponentParams(keyArray, valueArray),
              true
            )
          )],
          true
        )
      )
    )],
    ts.NodeFlags.Let
    ));
}

function paramsLambdaCallBack(componentNode: ts.CallExpression): ts.Expression {
  if (partialUpdateConfig.partialUpdateMode && componentNode.arguments.length === 1 &&
    isLocalStorageParameter(componentNode)) {
    return ts.factory.createObjectLiteralExpression([], true);
  } else {
    return componentNode.arguments[0];
  }
}

function reWriteComponentParams(keyArray: ts.Node[], valueArray: ts.Node[]): (ts.PropertyAssignment |
  ts.ShorthandPropertyAssignment)[] {
  const returnProperties: (ts.PropertyAssignment | ts.ShorthandPropertyAssignment)[] = [];
  keyArray.forEach((item: ts.Identifier, index: number) => {
    if (!valueArray[index]) {
      returnProperties.push(ts.factory.createShorthandPropertyAssignment(
        item,
        undefined
      ));
    } else {
      returnProperties.push(ts.factory.createPropertyAssignment(
        item,
        valueArray[index] as ts.Identifier
      ));
    }
  });
  return returnProperties;
}

function splitComponentParams(componentNode: ts.CallExpression, isBuilder: boolean, isParamsLambda: boolean): [ts.Node[], ts.Node[]] {
  const keyArray: ts.Node[] = [];
  const valueArray: ts.Node[] = [];
  if (componentNode.arguments && componentNode.arguments.length > 0 &&
    ts.isObjectLiteralExpression(componentNode.arguments[0]) && componentNode.arguments[0].properties) {
    componentNode.arguments[0].properties.forEach((propertyItem: ts.PropertyAssignment) => {
      const newPropertyItem: ts.PropertyAssignment =
        isProperty(propertyItem) ? createReference(propertyItem, [], isBuilder, isParamsLambda) : propertyItem;
      keyArray.push(newPropertyItem.name);
      valueArray.push(newPropertyItem.initializer);
    });
  }
  return [keyArray, valueArray];
}

function createIfCustomComponent(newNode: ts.NewExpression, componentNode: ts.CallExpression,
  componentParameter: ts.ObjectLiteralExpression, name: string, isGlobalBuilder: boolean, isBuilder: boolean,
  isRecycleComponent: boolean, componentAttrInfo: ComponentAttrInfo, log: LogInfo[]): ts.IfStatement {
  return ts.factory.createIfStatement(
    ts.factory.createIdentifier(ISINITIALRENDER),
    ts.factory.createBlock(
      [componentParamDetachment(newNode, isRecycleComponent, name, log, componentNode),
        isRecycleComponent ? createNewRecycleComponent(newNode, componentNode, name, componentAttrInfo) :
          createNewComponent(COMPONENT_CALL, name, componentNode),
        assignComponentParams(componentNode, isBuilder),
        assignmentFunction(COMPONENT_CALL)
      ], true),
    ts.factory.createBlock(
      [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(isGlobalBuilder ?
          ts.factory.createParenthesizedExpression(parentConditionalExpression()) : ts.factory.createThis(),
        ts.factory.createIdentifier(UPDATE_STATE_VARS_OF_CHIND_BY_ELMTID)
        ), undefined,
        [ts.factory.createIdentifier(ELMTID), componentParameter]))], true)
  );
}

export function assignmentFunction(componeParamName: string): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(componeParamName),
      ts.factory.createIdentifier(COMPONENT_PARAMS_FUNCTION)
    ),
    ts.factory.createToken(ts.SyntaxKind.EqualsToken),
    ts.factory.createIdentifier(COMPONENT_PARAMS_LAMBDA_FUNCTION)
  ));
}

function traverseChildComponentArgs(childParam: ts.Expression[], name: string, log: LogInfo[],
  componentNode: ts.CallExpression): ts.Expression[] {
  const childStructInfo: StructInfo = processStructComponentV2.getAliasStructInfo(componentNode) ||
    processStructComponentV2.getOrCreateStructInfo(name);
  if (!childStructInfo.isComponentV2) {
    return childParam;
  }
  const objectLiteralIndex: number = 2;
  if (childParam.length > objectLiteralIndex && ts.isObjectLiteralExpression(childParam[1]) &&
    childParam[1].properties) {
    const newProperties: ts.PropertyAssignment[] = [];
    childParam[1].properties.forEach((item: ts.PropertyAssignment) => {
      if (item.name && ts.isIdentifier(item.name)) {
        const itemName: string = item.name.escapedText.toString();
        updatePropertyAssignment(newProperties, itemName, item, childStructInfo, log);
      }
    });
    if (newProperties.length) {
      return getNewArgsForCustomComponent(childParam, newProperties);
    }
  }
  return childParam;
}

function getNewArgsForCustomComponent(childParam: ts.Expression[],
  newProperties: ts.PropertyAssignment[]): ts.Expression[] {
  const newArr: ts.Expression[] = [];
  const newObjectLiteralNode: ts.ObjectLiteralExpression =
    ts.factory.updateObjectLiteralExpression(childParam[1], [...childParam[1].properties, ...newProperties]);
  childParam.forEach((item: ts.Expression, index: number) => {
    if (index === 1) {
      newArr.push(newObjectLiteralNode);
    } else {
      newArr.push(item);
    }
  });
  return newArr;
}

function updatePropertyAssignment(newProperties: ts.PropertyAssignment[],
  itemName: string, item: ts.PropertyAssignment, childStructInfo: StructInfo, log: LogInfo[]): void {
  if (isDoubleNonNullExpression(item.initializer)) {
    if (isLeftHandExpression(item.initializer.expression.expression)) {
      const result: Record<string, boolean> = { hasQuestionToken: false };
      traverseExpressionNode(item.initializer.expression.expression, result);
      if (result.hasQuestionToken) {
        log.push({
          type: LogType.ERROR,
          message: `The optional character can not be used in the initial value of property '${itemName}'.`,
          pos: item.getStart(),
          code: '10905320'
        });
        return;
      }
      if (childStructInfo.paramDecoratorMap.has(itemName) &&
        childStructInfo.eventDecoratorMap.has('$' + itemName)) {
        newProperties.push(createUpdateTwoWayNode(itemName, item.initializer.expression.expression));
        return;
      }
      log.push({
        type: LogType.ERROR,
        message: 'When the two-way binding syntax is used, ' +
          `the variable '${itemName}' must be decorated with '@Param', ` +
          `and the '@Event' variable '$` + `${itemName}' ` + `must be defined in the ${childStructInfo.structName}.`,
        pos: item.getStart(),
        code: '10905319'
      });
      return;
    }
    log.push({
      type: LogType.ERROR,
      message: 'When the two-way binding syntax is used, ' +
        `the initial value of property '${itemName}' must be a variable.`,
      pos: item.getStart(),
      code: '10905318'
    });
    return;
  }
}

function validateTwoWayComputed(node: ts.PropertyAccessExpression, log: LogInfo[]): void {
  if (ts.isPropertyAccessExpression(node) && node.expression && 
    node.expression.kind === ts.SyntaxKind.ThisKeyword && globalProgram.checker) {
    const symbol: ts.Symbol = globalProgram.checker.getSymbolAtLocation(node);
    logMessageCollection.checkTwoWayComputed(node, symbol, log);
  }
}

function createUpdateTwoWayNode(itemName: string, leftHandExpression: ts.Expression): ts.PropertyAssignment {
  return ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier('$' + itemName),
    ts.factory.createArrowFunction(undefined, undefined,
      [createAstNodeUtils.createParameterDeclaration('value')], undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      ts.factory.createBlock([
        ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
          leftHandExpression, ts.factory.createToken(ts.SyntaxKind.EqualsToken),
          ts.factory.createIdentifier('value')
        ))
      ], false)
    )
  );
}

function isDoubleNonNullExpression(node: ts.Expression): boolean {
  return node && ts.isNonNullExpression(node) && ts.isNonNullExpression(node.expression);
}

function isLeftHandExpression(node: ts.Expression): boolean {
  return node && (ts.isIdentifier(node) || ts.isPropertyAccessExpression(node));
}

function traverseExpressionNode(node: ts.Node, result: Record<string, boolean>): void {
  if (ts.isOptionalChain(node) && !ts.isNonNullExpression(node) && node.questionDotToken) {
    result.hasQuestionToken = true;
  }
  if (!result.hasQuestionToken) {
    ts.forEachChild(node, (child: ts.Node) => traverseExpressionNode(child, result));
  }
}

function componentParamDetachment(newNode: ts.NewExpression, isRecycleComponent: boolean,
  name: string, log: LogInfo[], componentNode: ts.CallExpression): ts.VariableStatement {
  const paramsArray: ts.Expression[] = newNode.arguments.length ? newNode.arguments : [];
  const updateNewNode = ts.factory.updateNewExpression(newNode, newNode.expression,
    newNode.typeArguments, traverseChildComponentArgs(paramsArray, name, log, componentNode));
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(COMPONENT_CALL),
        undefined,
        undefined,
        isRecycleComponent ? ts.factory.createConditionalExpression(
          ts.factory.createIdentifier(RECYCLE_NODE),
          ts.factory.createToken(ts.SyntaxKind.QuestionToken),
          ts.factory.createIdentifier(RECYCLE_NODE),
          ts.factory.createToken(ts.SyntaxKind.ColonToken),
          newNode) : updateNewNode
      )],
      ts.NodeFlags.Let
    ));
}

function createNewComponent(componeParamName: string, name: string,
  componentNode: ts.CallExpression): ts.Statement {
  const childStructInfo: StructInfo = processStructComponentV2.getAliasStructInfo(componentNode) ||
    processStructComponentV2.getOrCreateStructInfo(name);
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(
          childStructInfo.isComponentV2 ? constantDefine.STRUCT_PARENT : BASE_COMPONENT_NAME_PU),
        ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)
      ), undefined, [ts.factory.createIdentifier(componeParamName)]));
}

function createNewRecycleComponent(newNode: ts.NewExpression, componentNode: ts.CallExpression,
  name: string, componentAttrInfo: ComponentAttrInfo): ts.Statement {
  let argNode: ts.Expression[] = [];
  const componentParam: ts.PropertyAssignment[] = [];
  if (componentNode.arguments && componentNode.arguments.length > 0 &&
    ts.isObjectLiteralExpression(componentNode.arguments[0]) && componentNode.arguments[0].properties) {
    componentNode.arguments[0].properties.forEach((propertyItem: ts.PropertyAssignment) => {
      const newPropertyItem: ts.PropertyAssignment = isProperty(propertyItem) ?
        createReference(propertyItem, [], false, false, true) : propertyItem;
      componentParam.push(newPropertyItem);
    });
    argNode = [ts.factory.createObjectLiteralExpression(componentParam, false)];
  } else {
    argNode = [ts.factory.createObjectLiteralExpression([], false)];
  }
  const recycleNode: ts.CallExpression = ts.factory.createCallExpression(
    createRecyclePropertyNode(ABOUT_TO_REUSE), undefined, argNode);
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(BASE_COMPONENT_NAME_PU),
        ts.factory.createIdentifier(COMPONENT_CREATE_RECYCLE)
      ), undefined,
      [
        ts.factory.createIdentifier(COMPONENT_CALL),
        ts.factory.createBinaryExpression(
          ts.factory.createIdentifier(RECYCLE_NODE),
          ts.factory.createToken(ts.SyntaxKind.ExclamationEqualsEqualsToken),
          ts.factory.createNull()
        ),
        componentAttrInfo.reuseId ? componentAttrInfo.reuseId as ts.Expression :
          ts.factory.createStringLiteral(name),
        ts.factory.createArrowFunction(undefined, undefined, [], undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock([
            ts.factory.createIfStatement(
              ts.factory.createBinaryExpression(
                ts.factory.createIdentifier(RECYCLE_NODE),
                ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
                ts.factory.createBinaryExpression(
                  ts.factory.createTypeOfExpression(
                    createRecyclePropertyNode(COMPONENT_ABOUTTOREUSEINTERNAL_FUNCTION)),
                  ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
                  ts.factory.createStringLiteral(FUNCTION)
                )),
              ts.factory.createBlock([
                ts.factory.createExpressionStatement(ts.factory.createCallExpression(
                  createRecyclePropertyNode(COMPONENT_ABOUTTOREUSEINTERNAL_FUNCTION),
                  undefined,
                  []
                ))
              ], true),
              ts.factory.createBlock(
                [
                  ts.factory.createIfStatement(ts.factory.createBinaryExpression(
                    createRecyclePropertyNode(ABOUT_TO_REUSE), ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
                    ts.factory.createBinaryExpression(
                      ts.factory.createTypeOfExpression(createRecyclePropertyNode(ABOUT_TO_REUSE)),
                      ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
                      ts.factory.createStringLiteral(FUNCTION)
                    )),
                  ts.factory.createBlock([ts.factory.createExpressionStatement(recycleNode)], true)),
                  ts.factory.createExpressionStatement(ts.factory.createCallExpression(
                    createRecyclePropertyNode(COMPONENT_RERENDER_FUNCTION), undefined, []
                  ))
                ],
                true
              )
            )], true))
      ]));
}

function createRecyclePropertyNode(recycleFunctionName: string): ts.PropertyAccessExpression {
  return ts.factory.createPropertyAccessExpression(
    ts.factory.createIdentifier(RECYCLE_NODE), ts.factory.createIdentifier(recycleFunctionName));
}

function validateCustomComponentPrams(node: ts.CallExpression, name: string,
  props: ts.ObjectLiteralElementLike[], log: LogInfo[], isBuilder: boolean): void {
  const curChildProps: Set<string> = new Set([]);
  const nodeArguments: ts.NodeArray<ts.Expression> = node.arguments;
  const linkSet: Set<string> = getCollectionSet(name, linkCollection);
  if (nodeArguments && nodeArguments.length >= 1 &&
    ts.isObjectLiteralExpression(nodeArguments[0])) {
    const nodeArgument: ts.ObjectLiteralExpression = nodeArguments[0] as ts.ObjectLiteralExpression;
    const doubleExclamationCollection: string[] = [];
    const dollarPropertyCollection: string[] = [];
    nodeArgument.properties.forEach(item => {
      if (item.name && ts.isIdentifier(item.name)) {
        const propName: string = item.name.escapedText.toString();
        curChildProps.add(propName);
        logMessageCollection.checkIfAssignToStaticProps(item, propName, regularStaticCollection.get(name) || new Set(), log);
      }
      validateStateManagement(item, name, log, isBuilder, doubleExclamationCollection, dollarPropertyCollection);
      if (isNonThisProperty(item, linkSet)) {
        if (isToChange(item as ts.PropertyAssignment, name)) {
          item = ts.factory.updatePropertyAssignment(item as ts.PropertyAssignment,
            item.name, changeNodeFromCallToArrow(item.initializer));
        }
        props.push(item);
      }
    });
    logMessageCollection.checkIfNeedDollarEvent(doubleExclamationCollection, dollarPropertyCollection, node, log);
  }
  const parentStructInfo: StructInfo = componentCollection.currentClassName ?
    processStructComponentV2.getOrCreateStructInfo(componentCollection.currentClassName) :
    new StructInfo();
  validateInitDecorator(node, name, curChildProps, log);
  validateMandatoryToInitViaParam(node, name, curChildProps, log, parentStructInfo);
  validateInitParam(name, curChildProps, node, log, parentStructInfo);
}

function getCustomComponentNode(node: ts.ExpressionStatement): ts.CallExpression {
  let temp: any = node.expression;
  let child: any = null;
  let componentNode: any = null;
  while (temp) {
    if (ts.isIdentifier(temp)) {
      child = temp;
      break;
    }
    temp = temp.expression;
  }
  if (child) {
    let parent = child.parent;
    while (parent) {
      if (ts.isExpressionStatement(parent)) {
        break;
      }
      if (ts.isCallExpression(parent) || ts.isEtsComponentExpression(parent)) {
        componentNode = parent;
        break;
      }
      parent = parent.parent;
    }
  }
  return componentNode;
}

export enum ParentType {
  NormalComponentV1,
  NormalComponentV2,
  ReuseComponentV1,
  ReuseComponentV2,
  InvalidComponentType
}

function getParentComponentType(parentName: string): ParentType {
  const parentCustomComponentInfo: StructInfo = parentName ? 
    processStructComponentV2.getOrCreateStructInfo(parentName) : new StructInfo();
  let type: ParentType = ParentType.InvalidComponentType;
  if (parentCustomComponentInfo.isComponentV1 &&
    parentCustomComponentInfo.isReusable) {
    type = ParentType.ReuseComponentV1;
  } else if (parentCustomComponentInfo.isComponentV1 &&
    !parentCustomComponentInfo.isReusable) {
    type = ParentType.NormalComponentV1;
  } else if (parentCustomComponentInfo.isComponentV2 &&
    parentCustomComponentInfo.isReusableV2) {
    type = ParentType.ReuseComponentV2;
  } else if (parentCustomComponentInfo.isComponentV2 &&
    !parentCustomComponentInfo.isReusableV2) {
    type = ParentType.NormalComponentV2;
  }
  return type;
}

function getCollectionSet(name: string, collection: Map<string, Set<string>>): Set<string> {
  if (!collection) {
    return new Set([]);
  }
  return collection.get(name) || new Set([]);
}

function isThisProperty(node: ts.ObjectLiteralElementLike, propertySet: Set<string>): boolean {
  if (ts.isPropertyAssignment(node) && ts.isIdentifier(node.name) &&
    propertySet.has(node.name.escapedText.toString())) {
    return true;
  }
  return false;
}

function isNonThisProperty(node: ts.ObjectLiteralElementLike, propertySet: Set<string>): boolean {
  if (ts.isPropertyAssignment(node) && ts.isIdentifier(node.name) &&
    (node.initializer.escapedText && node.initializer.escapedText.includes('$') ||
    ts.isPropertyAccessExpression(node.initializer) && node.initializer.expression &&
    node.initializer.expression.kind === ts.SyntaxKind.ThisKeyword &&
    ts.isIdentifier(node.initializer.name) && node.initializer.name.escapedText.toString().includes('$'))) {
    return false;
  }
  if (ts.isPropertyAssignment(node) && ts.isIdentifier(node.name) &&
    !propertySet.has(node.name.escapedText.toString())) {
    return true;
  }
  return false;
}

function validateStateManagement(node: ts.ObjectLiteralElementLike, customComponentName: string, log: LogInfo[], 
  isBuilder: boolean, doubleExclamationCollection: string[], dollarPropertyCollection: string[]): void {
  validateForbiddenToInitViaParam(node, customComponentName, log);
  if (componentCollection.currentClassName) {
    const parentStructInfo: StructInfo =
      processStructComponentV2.getOrCreateStructInfo(componentCollection.currentClassName);
    if (parentStructInfo.isComponentV2) {
      if (ts.isPropertyAssignment(node)) {
        checkDoubleExclamationPropertyAssignment(node, log, doubleExclamationCollection);
        checkDollarPropertyAssignment(node, dollarPropertyCollection);
      }
      return;
    }
  }
  checkFromParentToChild(node, customComponentName, log, isBuilder);
}

function checkDoubleExclamationPropertyAssignment(node: ts.PropertyAssignment, log: LogInfo[], doubleExclamationCollection: string[]): void {
  if (node.initializer && isDoubleNonNullExpression(node.initializer)) {
    if (node.initializer.expression && node.initializer.expression.expression && 
      isLeftHandExpression(node.initializer.expression.expression)) {
        doubleExclamationCollection.push(node.name.getText());
        validateTwoWayComputed(node.initializer.expression.expression, log);
    }
  }
}

function checkDollarPropertyAssignment(node: ts.PropertyAssignment, dollarPropertyCollection: string[]): void {
  const regex = /^\$[a-zA-Z_][a-zA-Z0-9_]*$/;
  if (node.name && ts.isIdentifier(node.name) && regex.test(node.name.escapedText.toString())) {
    dollarPropertyCollection.push(node.name.escapedText.toString());
  }
}

function checkFromParentToChild(node: ts.ObjectLiteralElementLike, customComponentName: string,
  log: LogInfo[], isBuilder: boolean): void {
  let propertyName: string;
  if (ts.isIdentifier(node.name)) {
    propertyName = node.name.escapedText.toString();
  }
  const curPropertyKind: string = getPropertyDecoratorKind(propertyName, customComponentName);
  let parentPropertyName: string;
  if (curPropertyKind) {
    if (isInitFromParent(node)) {
      parentPropertyName =
        getParentPropertyName(node as ts.PropertyAssignment, curPropertyKind, log);
      let parentPropertyKind: string = PropMapManager.find(parentPropertyName);
      if (parentPropertyKind && !isCorrectInitFormParent(parentPropertyKind, curPropertyKind)) {
        validateIllegalInitFromParent(
          node, propertyName, curPropertyKind, parentPropertyName, parentPropertyKind, log);
      }
    } else if (isInitFromLocal(node) && ts.isPropertyAssignment(node) &&
      curPropertyKind !== COMPONENT_OBJECT_LINK_DECORATOR) {
      if (!isCorrectInitFormParent(COMPONENT_NON_DECORATOR, curPropertyKind)) {
        validateIllegalInitFromParent(node, propertyName, curPropertyKind,
          node.initializer.getText(), COMPONENT_NON_DECORATOR, log);
      }
    } else if (curPropertyKind === COMPONENT_OBJECT_LINK_DECORATOR && node.initializer &&
      (ts.isPropertyAccessExpression(node.initializer) ||
        ts.isElementAccessExpression(node.initializer) || ts.isIdentifier(node.initializer))) {
      return;
    } else {
      parentPropertyName =
        getParentPropertyName(node as ts.PropertyAssignment, curPropertyKind, log) || propertyName;
      const parentPropertyKind = COMPONENT_NON_DECORATOR;
      if (!isCorrectInitFormParent(parentPropertyKind, curPropertyKind)) {
        if (isBuilder && judgeStructAssignedDollar(node)) {
          log.push({
            type: LogType.WARN,
            message: `Unrecognized property '${parentPropertyName}', make sure it can be assigned to ` +
              `${curPropertyKind} property '${propertyName}' by yourself.`,
            // @ts-ignore
            pos: node.initializer ? node.initializer.getStart() : node.getStart()
          });
        } else {
          validateIllegalInitFromParent(
            node, propertyName, curPropertyKind, parentPropertyName, parentPropertyKind, log, LogType.WARN);
        }
      }
    }
  }
}

function judgeStructAssignedDollar(node: ts.ObjectLiteralElementLike): boolean {
  return partialUpdateConfig.partialUpdateMode && node.initializer &&
    ts.isPropertyAccessExpression(node.initializer) &&
    node.initializer.expression && ts.isIdentifier(node.initializer.expression) &&
    node.initializer.expression.escapedText.toString() === $$;
}

function isInitFromParent(node: ts.ObjectLiteralElementLike): boolean {
  if (ts.isPropertyAssignment(node) && node.initializer) {
    if (ts.isPropertyAccessExpression(node.initializer) && node.initializer.expression &&
    node.initializer.expression.kind === ts.SyntaxKind.ThisKeyword &&
    ts.isIdentifier(node.initializer.name)) {
      return true;
    } else if (ts.isIdentifier(node.initializer) &&
      matchStartWithDollar(node.initializer.getText())) {
      return true;
    }
  }
  return false;
}

function isInitFromLocal(node: ts.ObjectLiteralElementLike): boolean {
  if (ts.isPropertyAssignment(node) && ts.isIdentifier(node.initializer) &&
    !matchStartWithDollar(node.initializer.getText())) {
    return true;
  }
  return false;
}

function getParentPropertyName(node: ts.PropertyAssignment, curPropertyKind: string,
  log: LogInfo[]): string {
  const initExpression: ts.Expression = node.initializer;
  if (!initExpression) {
    return undefined;
  }
  let parentPropertyName: string = initExpression.getText();
  const symbol = globalProgram.checker?.getSymbolAtLocation(initExpression);
  if (curPropertyKind === COMPONENT_LINK_DECORATOR) {
    // @ts-ignore
    const initName: ts.Identifier = initExpression.name || initExpression;
    if (!symbol && hasDollar(initExpression)) {
      parentPropertyName = initName.getText().replace(/^\$/, '');
    } else {
      parentPropertyName = initName.getText();
    }
  } else {
    if (!symbol && hasDollar(initExpression)) {
      validateNonLinkWithDollar(node, log);
    }
    // @ts-ignore
    if (node.initializer && node.initializer.name) {
      parentPropertyName = node.initializer.name.getText();
    }
  }

  return parentPropertyName;
}

function isCorrectInitFormParent(parent: string, child: string): boolean {
  switch (child) {
    case COMPONENT_STATE_DECORATOR:
    case COMPONENT_PROP_DECORATOR:
    case COMPONENT_PROVIDE_DECORATOR:
      return true;
    case COMPONENT_NON_DECORATOR:
      if ([COMPONENT_NON_DECORATOR, COMPONENT_STATE_DECORATOR, COMPONENT_LINK_DECORATOR, COMPONENT_PROP_DECORATOR,
        COMPONENT_OBJECT_LINK_DECORATOR, COMPONENT_STORAGE_LINK_DECORATOR].includes(parent)) {
        return true;
      }
      break;
    case COMPONENT_LINK_DECORATOR:
    case COMPONENT_OBJECT_LINK_DECORATOR:
      return ![COMPONENT_NON_DECORATOR].includes(parent);
  }
  return false;
}

function getPropertyDecoratorKind(propertyName: string, customComponentName: string): string {
  for (const item of decoractorMap.entries()) {
    if (getCollectionSet(customComponentName, item[1]).has(propertyName)) {
      return item[0];
    }
  }
  return undefined;
}

function createFindChildById(id: string, name: string, isBuilder: boolean = false): ts.VariableStatement {
  return ts.factory.createVariableStatement(undefined, ts.factory.createVariableDeclarationList(
    [ts.factory.createVariableDeclaration(ts.factory.createIdentifier(
      `${CUSTOM_COMPONENT_EARLIER_CREATE_CHILD}${id}`), undefined, ts.factory.createTypeReferenceNode(
      ts.factory.createIdentifier(name)),
    ts.factory.createConditionalExpression(
      ts.factory.createParenthesizedExpression(
        ts.factory.createBinaryExpression(
          createConditionParent(isBuilder),
          ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
          ts.factory.createPropertyAccessExpression(
            createConditionParent(isBuilder),
            ts.factory.createIdentifier(CUSTOM_COMPONENT_FUNCTION_FIND_CHILD_BY_ID)
          ))), ts.factory.createToken(ts.SyntaxKind.QuestionToken),
      ts.factory.createAsExpression(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(createConditionParent(isBuilder),
          ts.factory.createIdentifier(`${CUSTOM_COMPONENT_FUNCTION_FIND_CHILD_BY_ID}`)), undefined,
        [isBuilder ? ts.factory.createCallExpression(ts.factory.createIdentifier(GENERATE_ID),
          undefined, []) : ts.factory.createStringLiteral(id)]),
      ts.factory.createTypeReferenceNode(ts.factory.createIdentifier(name))),
      ts.factory.createToken(ts.SyntaxKind.ColonToken),
      ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED)))], ts.NodeFlags.Let));
}

export function createConditionParent(isBuilder: boolean): ts.ParenthesizedExpression | ts.ThisExpression {
  return isBuilder ? ts.factory.createParenthesizedExpression(parentConditionalExpression()) : ts.factory.createThis();
}

function createCustomComponentIfStatement(id: string, node: ts.ExpressionStatement,
  newObjectLiteralExpression: ts.ObjectLiteralExpression, parentName: string): ts.IfStatement {
  const viewName: string = `${CUSTOM_COMPONENT_EARLIER_CREATE_CHILD}${id}`;
  return ts.factory.createIfStatement(ts.factory.createBinaryExpression(
    ts.factory.createIdentifier(viewName),
    ts.factory.createToken(ts.SyntaxKind.EqualsEqualsToken),
    ts.factory.createIdentifier(`${COMPONENT_CONSTRUCTOR_UNDEFINED}`)),
  ts.factory.createBlock([node], true),
  ts.factory.createBlock([ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(
      viewName), ts.factory.createIdentifier(
      `${COMPONENT_CONSTRUCTOR_UPDATE_PARAMS}`)), undefined, [newObjectLiteralExpression])),
  isStaticViewCollection.get(parentName) ? createStaticIf(viewName) : undefined,
  ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(`${BASE_COMPONENT_NAME}`),
      ts.factory.createIdentifier(`${COMPONENT_CREATE_FUNCTION}`)), undefined,
    [ts.factory.createIdentifier(viewName)]))], true));
}

function createStaticIf(name: string): ts.IfStatement {
  return ts.factory.createIfStatement(ts.factory.createPrefixUnaryExpression(
    ts.SyntaxKind.ExclamationToken, ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(name),
        ts.factory.createIdentifier(CUSTOM_COMPONENT_NEEDS_UPDATE_FUNCTION)), undefined, [])),
  ts.factory.createBlock([ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(name),
      ts.factory.createIdentifier(CUSTOM_COMPONENT_MARK_STATIC_FUNCTION)),
    undefined, []))], true), undefined);
}

function hasDollar(initExpression: ts.Expression): boolean {
  if (ts.isPropertyAccessExpression(initExpression) &&
    matchStartWithDollar(initExpression.name.getText())) {
    return true;
  } else if (ts.isIdentifier(initExpression) && matchStartWithDollar(initExpression.getText())) {
    return true;
  } else {
    return false;
  }
}

function matchStartWithDollar(name: string): boolean {
  return /^\$/.test(name);
}

function getForbbiddenToInitViaParamType(customComponentName: string,
  node: ts.Identifier): string {
  let propType: string = COMPONENT_CONSUME_DECORATOR;
  const propName: string = node.escapedText.toString();
  if (getCollectionSet(customComponentName, storageLinkCollection).has(propName)) {
    propType = COMPONENT_STORAGE_LINK_DECORATOR;
  } else if (getCollectionSet(customComponentName, storagePropCollection)) {
    propType = COMPONENT_STORAGE_PROP_DECORATOR;
  } else if (ifLocalStorageLink(customComponentName, propName)) {
    propType = COMPONENT_LOCAL_STORAGE_LINK_DECORATOR;
  } else if (ifLocalStorageProp(customComponentName, propName)) {
    propType = COMPONENT_LOCAL_STORAGE_PROP_DECORATOR;
  }
  return propType;
}

function ifLocalStorageProp(componentName: string, propName: string): boolean {
  if (!localStoragePropCollection.get(componentName).keys) {
    return false;
  } else {
    const collection: Set<string> = new Set();
    for (const key of localStoragePropCollection.get(componentName).keys()) {
      collection.add(key);
    }
    return collection.has(propName);
  }
}

function ifLocalStorageLink(componentName: string, propName: string): boolean {
  if (!localStorageLinkCollection.get(componentName).keys) {
    return false;
  } else {
    const collection: Set<string> = new Set();
    for (const key of localStorageLinkCollection.get(componentName).keys()) {
      collection.add(key);
    }
    return collection.has(propName);
  }
}

function validateForbiddenToInitViaParam(node: ts.ObjectLiteralElementLike,
  customComponentName: string, log: LogInfo[]): void {
  const forbiddenToInitViaParamSet: Set<string> = new Set([
    ...getCollectionSet(customComponentName, storageLinkCollection),
    ...getCollectionSet(customComponentName, storagePropCollection),
    ...getCollectionSet(customComponentName, consumeCollection)
  ]);
  const localStorageSet: Set<string> = new Set();
  getLocalStorageCollection(customComponentName, localStorageSet);
  if (isThisProperty(node, forbiddenToInitViaParamSet) || isThisProperty(node, localStorageSet)) {
    const nodeIdentifier: ts.Identifier = node.name as ts.Identifier;
    const propType: string = getForbbiddenToInitViaParamType(customComponentName, nodeIdentifier);
    log.push({
      type: LogType.ERROR,
      message: `The '${propType}' property '${node.name.getText()}' in the custom component '${customComponentName}'` +
        ` cannot be initialized here (forbidden to specify).`,
      pos: node.name.getStart(),
      code: '10905317'
    });
  }
}
  
function validateMandatoryToInitViaParam(node: ts.CallExpression, customComponentName: string,
  curChildProps: Set<string>, log: LogInfo[], parentStructInfo: StructInfo): void {
  let mandatoryToInitViaParamSet: Set<string>;
  if (projectConfig.compileMode === 'esmodule' && process.env.compileTool === 'rollup' && node.expression) {
    const overAll: string[] = [
      ...getCollectionSet(node.expression.getText(), storedFileInfo.overallObjectLinkCollection)];
    if (!parentStructInfo.isComponentV2) {
      overAll.unshift(...getCollectionSet(node.expression.getText(), storedFileInfo.overallLinkCollection));
    }
    mandatoryToInitViaParamSet = new Set(overAll);
    customComponentName = node.expression.getText();
  } else {
    const arr: string[] = [...getCollectionSet(customComponentName, objectLinkCollection)];
    if (!parentStructInfo.isComponentV2) {
      arr.unshift(...getCollectionSet(customComponentName, linkCollection));
    }
    mandatoryToInitViaParamSet = new Set(arr);
  }
  mandatoryToInitViaParamSet.forEach(item => {
    if (item && !curChildProps.has(item)) {
      log.push({
        type: LogType.ERROR,
        message: `The property '${item}' in the custom component '${customComponentName}'` +
          ` is missing (mandatory to specify).`,
        pos: node.getStart(),
        code: '10905316'
      });
    }
  });
}

function validateInitDecorator(node: ts.CallExpression, customComponentName: string,
  curChildProps: Set<string>, log: LogInfo[]): void {
  const mandatoryToInitViaParamSet: Set<string> = new Set([
    ...getCollectionSet(customComponentName, builderParamObjectCollection),
    ...getCollectionSet(customComponentName, propCollection),
    ...getCollectionSet(customComponentName, regularCollection),
    ...getCollectionSet(customComponentName, stateCollection),
    ...getCollectionSet(customComponentName, provideCollection)
  ]);
  const decoratorVariable: Set<string> = new Set([
    ...(builderParamInitialization.get(customComponentName) || []),
    ...(propInitialization.get(customComponentName) || []),
    ...(regularInitialization.get(customComponentName) || []),
    ...(stateInitialization.get(customComponentName) || []),
    ...(provideInitialization.get(customComponentName) || [])
  ]);
  mandatoryToInitViaParamSet.forEach((item: string) => {
    if (item && !curChildProps.has(item) && decoratorVariable && decoratorVariable.has(item)) {
      log.push({
        type: LogType.ERROR,
        message: `'@Require' decorated '${item}' must be initialized through the component constructor.`,
        pos: node.getStart(),
        code: '10905359'
      });
    }
  });
  const privateParamSet: Set<string> = privateCollection.get(customComponentName) || new Set([]);
  curChildProps.forEach((item: string) => {
    if (privateParamSet.has(item)) {
      log.push({
        type: LogType.WARN,
        message: `Property '${item}' is private and can not be initialized through the component constructor.`,
        pos: node.getStart()
      });
    }
  });
}

function validateIllegalInitFromParent(node: ts.ObjectLiteralElementLike, propertyName: string,
  curPropertyKind: string, parentPropertyName: string, parentPropertyKind: string,
  log: LogInfo[], inputType: LogType = undefined): void {
  let type: LogType = LogType.ERROR;
  if (inputType) {
    type = inputType;
  } else if ([COMPONENT_STATE_DECORATOR, COMPONENT_OBJECT_LINK_DECORATOR].includes(
    parentPropertyKind) && curPropertyKind === COMPONENT_OBJECT_LINK_DECORATOR) {
    type = LogType.WARN;
  }
  PropMapManager.reserveLog(parentPropertyName, parentPropertyKind, {
    type: type,
    message: `The '${parentPropertyKind}' property '${parentPropertyName}' cannot be assigned to ` +
      `the '${curPropertyKind}' property '${propertyName}'.`,
    // @ts-ignore
    pos: node.initializer ? node.initializer.getStart() : node.getStart(),
    code: type === LogType.ERROR ? '10905315' : undefined
  });
}

function validateNonLinkWithDollar(node: ts.PropertyAssignment, log: LogInfo[]): void {
  log.push({
    type: LogType.ERROR,
    message: `Property '${node.name.getText()}' cannot initialize` +
      ` using '$' to create a reference to a variable.`,
    pos: node.initializer.getStart(),
    code: '10905314'
  });
}
