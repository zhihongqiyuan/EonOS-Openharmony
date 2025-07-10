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
  COMPONENT_STATE_DECORATOR,
  COMPONENT_PROVIDE_DECORATOR,
  COMPONENT_LINK_DECORATOR,
  COMPONENT_PROP_DECORATOR,
  COMPONENT_STORAGE_LINK_DECORATOR,
  COMPONENT_STORAGE_PROP_DECORATOR,
  COMPONENT_OBJECT_LINK_DECORATOR,
  COMPONENT_CONSUME_DECORATOR,
  SYNCHED_PROPERTY_NESED_OBJECT,
  SYNCHED_PROPERTY_SIMPLE_TWO_WAY,
  SYNCHED_PROPERTY_SIMPLE_ONE_WAY,
  OBSERVED_PROPERTY_OBJECT,
  OBSERVED_PROPERTY_SIMPLE,
  COMPONENT_BUILD_FUNCTION,
  BASE_COMPONENT_NAME,
  CREATE_CONSTRUCTOR_PARAMS,
  COMPONENT_CONSTRUCTOR_UPDATE_PARAMS,
  COMPONENT_CONSTRUCTOR_INITIAL_PARAMS,
  COMPONENT_CONSTRUCTOR_PURGE_VARIABLE_DEP,
  COMPONENT_CONSTRUCTOR_DELETE_PARAMS,
  COMPONENT_DECORATOR_PREVIEW,
  CREATE_CONSTRUCTOR_SUBSCRIBER_MANAGER,
  ABOUT_TO_BE_DELETE_FUNCTION_ID,
  ABOUT_TO_BE_DELETE_FUNCTION_ID__,
  CREATE_CONSTRUCTOR_GET_FUNCTION,
  CREATE_CONSTRUCTOR_DELETE_FUNCTION,
  FOREACH_OBSERVED_OBJECT,
  FOREACH_GET_RAW_OBJECT,
  COMPONENT_BUILDER_DECORATOR,
  COMPONENT_TRANSITION_FUNCTION,
  COMPONENT_CREATE_FUNCTION,
  GEOMETRY_VIEW,
  COMPONENT_STYLES_DECORATOR,
  STYLES,
  INTERFACE_NAME_SUFFIX,
  OBSERVED_PROPERTY_ABSTRACT,
  COMPONENT_LOCAL_STORAGE_LINK_DECORATOR,
  COMPONENT_LOCAL_STORAGE_PROP_DECORATOR,
  COMPONENT_CONSTRUCTOR_LOCALSTORAGE,
  COMPONENT_SET_AND_LINK,
  COMPONENT_SET_AND_PROP,
  COMPONENT_CONSTRUCTOR_UNDEFINED,
  CUSTOM_COMPONENT,
  COMPONENT_CONSTRUCTOR_PARENT,
  NULL,
  INNER_COMPONENT_MEMBER_DECORATORS,
  COMPONENT_RERENDER_FUNCTION,
  RMELMTID,
  ABOUTTOBEDELETEDINTERNAL,
  UPDATEDIRTYELEMENTS,
  BASE_COMPONENT_NAME_PU,
  OBSERVED_PROPERTY_SIMPLE_PU,
  OBSERVED_PROPERTY_OBJECT_PU,
  SYNCHED_PROPERTY_SIMPLE_TWO_WAY_PU,
  SYNCHED_PROPERTY_SIMPLE_ONE_WAY_PU,
  SYNCHED_PROPERTY_NESED_OBJECT_PU,
  OBSERVED_PROPERTY_ABSTRACT_PU,
  CREATE_LOCAL_STORAGE_LINK,
  CREATE_LOCAL_STORAGE_PROP,
  COMPONENT_UPDATE_STATE_VARS,
  COMPONENT_WATCH_DECORATOR,
  $$,
  COMPONENT_UPDATE_ELMT_ID,
  OLD_ELMT_ID,
  NEW_ELMT_ID,
  UPDATE_RECYCLE_ELMT_ID,
  GET_ENTRYNAME,
  COMPONENT_PARAMS_FUNCTION,
  FUNCTION,
  COMPONENT_PARAMS_LAMBDA_FUNCTION,
  DECORATOR_COMPONENT_FREEZEWHENINACTIVE,
  INIT_ALLOW_COMPONENT_FREEZE,
  FINALIZE_CONSTRUCTION,
  PROTOTYPE,
  REFLECT,
  CREATE_SET_METHOD,
  COMPONENT_REQUIRE_DECORATOR,
  CONTEXT_STACK,
  COMPONENT_IF_UNDEFINED,
  COMPONENT_POP_FUNCTION,
  PUSH,
  PUV2_VIEW_BASE,
  COMPONENT_LOCAL_BUILDER_DECORATOR,
  DECORATOR_REUSEABLE
} from './pre_define';
import {
  BUILDIN_STYLE_NAMES,
  CUSTOM_BUILDER_METHOD,
  INNER_STYLE_FUNCTION,
  INTERFACE_NODE_SET,
  STYLES_ATTRIBUTE,
  INNER_CUSTOM_BUILDER_METHOD
} from './component_map';
import {
  componentCollection,
  linkCollection,
  localStorageLinkCollection,
  localStoragePropCollection,
  builderParamObjectCollection,
  methodDecoratorCollect
} from './validate_ui_syntax';
import {
  addConstructor,
  getInitConstructor,
  updateConstructor
} from './process_component_constructor';
import {
  ControllerType,
  processMemberVariableDecorators,
  UpdateResult,
  stateObjectCollection,
  PropMapManager,
  decoratorParamSet,
  isSimpleType,
  isSingleKey,
  findDecoratorIndex
} from './process_component_member';
import {
  processComponentBuild,
  processComponentBlock
} from './process_component_build';
import { isRecycle } from './process_custom_component';
import {
  LogType,
  LogInfo,
  hasDecorator,
  getPossibleBuilderTypeParameter,
  storedFileInfo,
  removeDecorator
} from './utils';
import {
  partialUpdateConfig,
  projectConfig
} from '../main';
import {
  builderTypeParameter,
  initializeMYIDS,
  globalBuilderParamAssignment
} from './process_ui_syntax';
import constantDefine from './constant_define';
import processStructComponentV2, { StructInfo } from './process_struct_componentV2';

export function processComponentClass(node: ts.StructDeclaration, context: ts.TransformationContext,
  log: LogInfo[], program: ts.Program): ts.ClassDeclaration {
  const decoratorNode: readonly ts.Decorator[] = ts.getAllDecorators(node);
  const memberNode: ts.ClassElement[] =
    processMembers(node.members, node.name, context, decoratorNode, log, program, checkPreview(node));
  return ts.factory.createClassDeclaration(
    ts.getModifiers(node), 
    node.name,
    node.typeParameters, 
    updateHeritageClauses(node, log), 
    memberNode
  );
}

function checkPreview(node: ts.StructDeclaration): boolean {
  let hasPreview: boolean = false;
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (node && decorators) {
    for (let i = 0; i < decorators.length; i++) {
      const name: string = decorators[i].getText().replace(/\([^\(\)]*\)/, '').trim();
      if (name === COMPONENT_DECORATOR_PREVIEW) {
        hasPreview = true;
        break;
      }
    }
  }
  return hasPreview;
}

export type BuildCount = {
  count: number;
};
export type FreezeParamType = {
  componentFreezeParam: ts.Expression;
};
function processMembers(members: ts.NodeArray<ts.ClassElement>, parentComponentName: ts.Identifier,
  context: ts.TransformationContext, decoratorNode: readonly ts.Decorator[], log: LogInfo[],
  program: ts.Program, hasPreview: boolean): ts.ClassElement[] {
  const buildCount: BuildCount = { count: 0 };
  let ctorNode: any = getInitConstructor(members, parentComponentName);
  const newMembers: ts.ClassElement[] = [];
  const watchMap: Map<string, ts.Node> = new Map();
  const updateParamsStatements: ts.Statement[] = [];
  const stateVarsStatements: ts.Statement[] = [];
  const purgeVariableDepStatements: ts.Statement[] = [];
  const rerenderStatements: ts.Statement[] = [];
  const deleteParamsStatements: ts.PropertyDeclaration[] = [];
  const checkController: ControllerType = { hasController: !componentCollection.customDialogs.has(parentComponentName.getText()), 
    unassignedControllerSet: new Set() };
  const interfaceNode = ts.factory.createInterfaceDeclaration(undefined,
    parentComponentName.getText() + INTERFACE_NAME_SUFFIX, undefined, undefined, []);
  members.forEach((item: ts.MethodDeclaration) => {
    if (hasDecorator(item, COMPONENT_STYLES_DECORATOR)) {
      methodDecoratorCollect(item);
    }
  });
  members.forEach((item: ts.ClassElement) => {
    let updateItem: ts.ClassElement;
    if (ts.isPropertyDeclaration(item)) {
      if (isStaticProperty(item)) {
        newMembers.push(item);
        validateDecorators(item, log);
      } else {
        addPropertyMember(item, newMembers, program, parentComponentName.getText(), log);
        const result: UpdateResult = processMemberVariableDecorators(parentComponentName, item,
          ctorNode, watchMap, checkController, log, program, context, hasPreview, interfaceNode);
        if (result.isItemUpdate()) {
          updateItem = result.getProperity();
        } else {
          updateItem = item;
        }
        if (result.getVariableGet()) {
          newMembers.push(result.getVariableGet());
        }
        if (result.getVariableSet()) {
          newMembers.push(result.getVariableSet());
        }
        if (result.isCtorUpdate()) {
          ctorNode = result.getCtor();
        }
        if (result.getUpdateParams()) {
          updateParamsStatements.push(result.getUpdateParams());
        }
        if (result.getStateVarsParams()) {
          stateVarsStatements.push(result.getStateVarsParams());
        }
        if (result.isDeleteParams()) {
          deleteParamsStatements.push(item);
        }
        if (result.getControllerSet()) {
          newMembers.push(result.getControllerSet());
        }
        processPropertyUnchanged(result, purgeVariableDepStatements);
      }
    }
    if (ts.isMethodDeclaration(item) && item.name) {
      updateItem =
        processComponentMethod(item, context, log, buildCount);
    }
    if (updateItem) {
      newMembers.push(updateItem);
    }
  });
  INTERFACE_NODE_SET.add(interfaceNode);
  validateBuildMethodCount(buildCount, parentComponentName, log);
  validateHasControllerAndControllerCount(parentComponentName, checkController, log);
  if (storedFileInfo.getCurrentArkTsFile().recycleComponents.has(parentComponentName.getText())) {
    newMembers.unshift(addDeleteParamsFunc(deleteParamsStatements, true));
  }
  newMembers.unshift(addDeleteParamsFunc(deleteParamsStatements));
  addIntoNewMembers(newMembers, parentComponentName, updateParamsStatements,
    purgeVariableDepStatements, rerenderStatements, stateVarsStatements);
  if (partialUpdateConfig.partialUpdateMode) {
    const creezeParam: FreezeParamType = {
      componentFreezeParam: undefined
    };
    const isFreezeComponent: boolean = decoratorAssignParams(decoratorNode, context, creezeParam);
    ctorNode = updateConstructor(ctorNode, [], assignParams(parentComponentName.getText()),
      isFreezeComponent ? decoratorComponentParam(creezeParam) : [], true);
  }
  newMembers.unshift(addConstructor(ctorNode, watchMap, parentComponentName));
  if (componentCollection.entryComponent === parentComponentName.escapedText.toString() &&
    partialUpdateConfig.partialUpdateMode && projectConfig.minAPIVersion > 10) {
    newMembers.push(getEntryNameFunction(componentCollection.entryComponent));
  }
  log.push(...Array.from(PropMapManager.logInfoMap.values()).flat());
  PropMapManager.reset();
  return newMembers;
}

export function decoratorAssignParams(decoratorNode: readonly ts.Decorator[], context: ts.TransformationContext,
  creezeParam: FreezeParamType): boolean {
  if (decoratorNode && Array.isArray(decoratorNode) && decoratorNode.length) {
    return decoratorNode.some((item: ts.Decorator) => {
      if (isFreezeComponents(item, context, creezeParam)) {
        return true;
      } else {
        return false;
      }
    });
  } else {
    return false;
  }
}

function isFreezeComponents(decorator: ts.Decorator, context: ts.TransformationContext,
  creezeParam: FreezeParamType): boolean {
  let isComponentAssignParent: boolean = false;
  ts.visitNode(decorator, visitComponentParament);
  function visitComponentParament(decorator: ts.Node): ts.Node {
    if (ts.isPropertyAssignment(decorator) && decorator.name && decorator.name.text &&
      decorator.name.text.toString() === DECORATOR_COMPONENT_FREEZEWHENINACTIVE) {
      isComponentAssignParent = true;
      creezeParam.componentFreezeParam = decorator.initializer;
      return decorator;
    }
    return ts.visitEachChild(decorator, visitComponentParament, context);
  }
  return isComponentAssignParent;
}

export function getEntryNameFunction(entryName: string): ts.MethodDeclaration {
  return ts.factory.createMethodDeclaration(
    [ts.factory.createToken(ts.SyntaxKind.StaticKeyword)],
    undefined,
    ts.factory.createIdentifier(GET_ENTRYNAME),
    undefined,
    undefined,
    [],
    ts.factory.createKeywordTypeNode(ts.SyntaxKind.StringKeyword),
    ts.factory.createBlock(
      [ts.factory.createReturnStatement(ts.factory.createStringLiteral(entryName))],
      true
    )
  );
}

function assignParams(parentComponentName: string): ts.Statement[] {
  return [ts.factory.createIfStatement(
    ts.factory.createBinaryExpression(
      ts.factory.createTypeOfExpression(ts.factory.createIdentifier(COMPONENT_PARAMS_LAMBDA_FUNCTION)),
      ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
      ts.factory.createStringLiteral(FUNCTION)
    ),
    ts.factory.createBlock(
      [ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(COMPONENT_PARAMS_FUNCTION)
        ),
        ts.factory.createToken(ts.SyntaxKind.EqualsToken),
        ts.factory.createIdentifier(COMPONENT_PARAMS_LAMBDA_FUNCTION)
      ))],
      true
    )
  )];
}

function decoratorComponentParam(freezeParam: FreezeParamType): ts.IfStatement[] {
  return [ts.factory.createIfStatement(
    ts.factory.createBinaryExpression(
      ts.factory.createElementAccessExpression(
        ts.factory.createSuper(),
        ts.factory.createStringLiteral(INIT_ALLOW_COMPONENT_FREEZE)
      ),
      ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
      ts.factory.createBinaryExpression(
        ts.factory.createTypeOfExpression(ts.factory.createElementAccessExpression(
          ts.factory.createSuper(),
          ts.factory.createStringLiteral(INIT_ALLOW_COMPONENT_FREEZE)
        )),
        ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
        ts.factory.createStringLiteral(FUNCTION)
      )
    ),
    ts.factory.createBlock(
      [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createElementAccessExpression(
          ts.factory.createSuper(),
          ts.factory.createStringLiteral(INIT_ALLOW_COMPONENT_FREEZE)
        ),
        undefined,
        [freezeParam.componentFreezeParam]
      ))],
      true
    ),
    undefined
  )];
}

function isStaticProperty(property: ts.PropertyDeclaration): boolean {
  const modifiers: readonly ts.Modifier[] =
    ts.canHaveModifiers(property) ? ts.getModifiers(property) : undefined;
  return modifiers && modifiers.length && modifiers.some(modifier => {
    return modifier.kind === ts.SyntaxKind.StaticKeyword;
  });
}

function validateDecorators(item: ts.ClassElement, log: LogInfo[]): void {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(item);
  if (decorators && decorators.length) {
    decorators.map((decorator: ts.Decorator) => {
      const decoratorName: string = decorator.getText();
      if (INNER_COMPONENT_MEMBER_DECORATORS.has(decoratorName)) {
        log.push({
          type: LogType.ERROR,
          message: `The static variable of struct cannot be used together with built-in decorators.`,
          pos: item.getStart(),
          code: '10905313'
        });
      }
    });
  }
}

function processPropertyUnchanged(
  result: UpdateResult,
  purgeVariableDepStatements: ts.Statement[]
): void {
  if (partialUpdateConfig.partialUpdateMode) {
    if (result.getPurgeVariableDepStatement()) {
      purgeVariableDepStatements.push(result.getPurgeVariableDepStatement());
    }
  }
}

function addIntoNewMembers(
  newMembers: ts.ClassElement[],
  parentComponentName: ts.Identifier,
  updateParamsStatements: ts.Statement[],
  purgeVariableDepStatements: ts.Statement[],
  rerenderStatements: ts.Statement[],
  stateVarsStatements: ts.Statement[]
): void {
  if (partialUpdateConfig.partialUpdateMode) {
    newMembers.unshift(
      addInitialParamsFunc(updateParamsStatements, parentComponentName),
      addUpdateStateVarsFunc(stateVarsStatements, parentComponentName),
      addPurgeVariableDepFunc(purgeVariableDepStatements)
    );
    newMembers.push(addRerenderFunc(rerenderStatements));
  } else {
    newMembers.unshift(addUpdateParamsFunc(updateParamsStatements, parentComponentName));
  }
}

export function isRegularProperty(decorators: readonly ts.Decorator[]): boolean {
  if (decorators && decorators.length) {
    if (decorators.length === 1 && decorators[0].getText() === COMPONENT_REQUIRE_DECORATOR) {
      return true;
    }
    return false;
  }
  return true;
}

function addPropertyMember(item: ts.ClassElement, newMembers: ts.ClassElement[],
  program: ts.Program, parentComponentName: string, log: LogInfo[]): void {
  const propertyItem: ts.PropertyDeclaration = item as ts.PropertyDeclaration;
  let decoratorName: string;
  let updatePropertyItem: ts.PropertyDeclaration;
  const type: ts.TypeNode = propertyItem.type;
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(propertyItem);
  if (isRegularProperty(decorators)) {
    updatePropertyItem = createPropertyDeclaration(propertyItem, type, true);
    newMembers.push(updatePropertyItem);
  } else {
    for (let i = 0; i < decorators.length; i++) {
      let newType: ts.TypeNode;
      decoratorName = decorators[i].getText().replace(/\(.*\)$/, '').trim();
      let isLocalStorage: boolean = false;
      if (!partialUpdateConfig.partialUpdateMode) {
        newType = createTypeReference(decoratorName, type, log, program);
      } else {
        newType = createTypeReferencePU(decoratorName, type, log, program);
      }
      if (
        decoratorName === COMPONENT_LOCAL_STORAGE_LINK_DECORATOR ||
        decoratorName === COMPONENT_LOCAL_STORAGE_PROP_DECORATOR
      ) {
        isLocalStorage = true;
      }
      const newUpdatePropertyItem = createPropertyDeclaration(
        propertyItem, newType, false, isLocalStorage, parentComponentName);
      if (!updatePropertyItem) {
        updatePropertyItem = newUpdatePropertyItem;
      } else if (INNER_COMPONENT_MEMBER_DECORATORS.has(decoratorName) &&
        ![COMPONENT_WATCH_DECORATOR, COMPONENT_REQUIRE_DECORATOR].includes(decoratorName)) {
        updatePropertyItem = newUpdatePropertyItem;
      }
    }
    if (updatePropertyItem) {
      newMembers.push(updatePropertyItem);
    }
  }
}

function createPropertyDeclaration(propertyItem: ts.PropertyDeclaration, newType: ts.TypeNode | undefined,
  normalVar: boolean, isLocalStorage: boolean = false, parentComponentName: string = null): ts.PropertyDeclaration {
  if (typeof newType === undefined) {
    return undefined;
  }
  let prefix: string = '';
  if (!normalVar) {
    prefix = '__';
  }
  const privateM: ts.ModifierToken<ts.SyntaxKind.PrivateKeyword> =
    ts.factory.createModifier(ts.SyntaxKind.PrivateKeyword);
  const modifiers: readonly ts.Modifier[] =
    ts.canHaveModifiers(propertyItem) ? ts.getModifiers(propertyItem) : undefined;
  return ts.factory.updatePropertyDeclaration(propertyItem,
    ts.concatenateDecoratorsAndModifiers(undefined, modifiers || [privateM]), prefix + propertyItem.name.getText(),
    propertyItem.questionToken, newType, isLocalStorage ?
      createLocalStroageCallExpression(propertyItem, propertyItem.name.getText(),
        parentComponentName) : undefined);
}

function createLocalStroageCallExpression(node: ts.PropertyDeclaration, name: string,
  parentComponentName: string): ts.CallExpression {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (isSingleKey(node)) {
    const localStorageLink: Set<string> = localStorageLinkCollection.get(parentComponentName).get(name);
    const localStorageProp: Set<string> = localStoragePropCollection.get(parentComponentName).get(name);
    let localFuncName: string;
    const index: number = findDecoratorIndex(decorators,
      [COMPONENT_LOCAL_STORAGE_LINK_DECORATOR, COMPONENT_LOCAL_STORAGE_PROP_DECORATOR]);
    const localValue: ts.Expression[] = [
      decorators[index].expression.arguments[0],
      node.initializer ? node.initializer : ts.factory.createNumericLiteral(COMPONENT_CONSTRUCTOR_UNDEFINED),
      ts.factory.createThis(),
      ts.factory.createStringLiteral(name || COMPONENT_CONSTRUCTOR_UNDEFINED)
    ];
    if (!partialUpdateConfig.partialUpdateMode) {
      localFuncName = localStorageLink && !localStorageProp ? COMPONENT_SET_AND_LINK :
        COMPONENT_SET_AND_PROP;
    } else {
      localFuncName = localStorageLink && !localStorageProp ? CREATE_LOCAL_STORAGE_LINK :
        CREATE_LOCAL_STORAGE_PROP;
      localValue.splice(-2, 1);
    }
    return ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        !partialUpdateConfig.partialUpdateMode ?
          ts.factory.createPropertyAccessExpression(
            ts.factory.createThis(),
            ts.factory.createIdentifier(`${COMPONENT_CONSTRUCTOR_LOCALSTORAGE}_`)
          ) : ts.factory.createThis(),
        ts.factory.createIdentifier(localFuncName)
      ),
      [node.type],
      localValue
    );
  }
  return undefined;
}
export interface builderConditionType {
  isBuilder: boolean,
  isLocalBuilder: boolean
}
export function processComponentMethod(node: ts.MethodDeclaration, context: ts.TransformationContext,
  log: LogInfo[], buildCount: BuildCount): ts.MethodDeclaration {
  let updateItem: ts.MethodDeclaration = node;
  const name: string = node.name.getText();
  const customBuilder: ts.Decorator[] = [];
  const builderCondition: builderConditionType = {
    isBuilder: false,
    isLocalBuilder: false
  };
  if (builderParamObjectCollection.get(componentCollection.currentClassName)) {
    storedFileInfo.builderLikeCollection =
      new Set([...builderParamObjectCollection.get(componentCollection.currentClassName), ...CUSTOM_BUILDER_METHOD]);
  } else {
    storedFileInfo.builderLikeCollection = CUSTOM_BUILDER_METHOD;
  }
  if (name === COMPONENT_BUILD_FUNCTION) {
    storedFileInfo.processBuilder = false;
    storedFileInfo.processGlobalBuilder = false;
    buildCount.count = buildCount.count + 1;
    if (node.parameters.length) {
      log.push({
        type: LogType.ERROR,
        message: `The 'build' method can not have arguments.`,
        pos: node.getStart(),
        code: '10905106'
      });
    }
    const buildNode: ts.MethodDeclaration = processComponentBuild(node, log);
    updateItem = processBuildMember(buildNode, context, log);
  } else if (node.body && ts.isBlock(node.body)) {
    if (name === COMPONENT_TRANSITION_FUNCTION) {
      updateItem = ts.factory.updateMethodDeclaration(node, ts.getModifiers(node),
        node.asteriskToken, node.name, node.questionToken, node.typeParameters, node.parameters,
        node.type, processComponentBlock(node.body, false, log, true));
    } else if (isBuilderOrLocalBuilder(node, builderCondition, customBuilder)) {
      storedFileInfo.processBuilder = true;
      storedFileInfo.processGlobalBuilder = false;
      if (builderCondition.isLocalBuilder) {
        storedFileInfo.processLocalBuilder = true;
      }
      CUSTOM_BUILDER_METHOD.add(name);
      INNER_CUSTOM_BUILDER_METHOD.add(name);
      builderTypeParameter.params = getPossibleBuilderTypeParameter(node.parameters);
      const parameters: ts.NodeArray<ts.ParameterDeclaration> = ts.factory.createNodeArray(Array.from(node.parameters));
      if (!builderCondition.isLocalBuilder) {
        parameters.push(createParentParameter());
      }
      if (projectConfig.optLazyForEach) {
        parameters.push(initializeMYIDS());
      }
      const modifiers = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
      const componentBlock: ts.Block = processComponentBlock(node.body, false, log, false, true);
      if (partialUpdateConfig.partialUpdateMode && builderCondition.isLocalBuilder &&
        node.body.statements.length) {
        componentBlock.statements.unshift(globalBuilderParamAssignment());
      }
      let builderNode: ts.MethodDeclaration | ts.PropertyDeclaration = ts.factory.updateMethodDeclaration(node,
        ts.concatenateDecoratorsAndModifiers(removeDecorator(customBuilder, 'Builder'), modifiers),
        node.asteriskToken, node.name, node.questionToken, node.typeParameters,
        parameters, node.type, componentBlock);
      builderTypeParameter.params = [];
      updateItem = processBuildMember(builderNode, context, log, true);
      if (builderCondition.isLocalBuilder) {
        checkDecoratorMethod(node, modifiers, log);
        updateItem = localBuilderNode(node, updateItem.body);
      }
      storedFileInfo.processBuilder = false;
      storedFileInfo.processLocalBuilder = false;
    } else if (hasDecorator(node, COMPONENT_STYLES_DECORATOR)) {
      if (node.parameters && node.parameters.length === 0) {
        if (ts.isBlock(node.body) && node.body.statements && node.body.statements.length) {
          INNER_STYLE_FUNCTION.set(name, node.body);
          STYLES_ATTRIBUTE.add(name);
          BUILDIN_STYLE_NAMES.add(name);
          decoratorParamSet.add(STYLES);
        }
      } else {
        log.push({
          type: LogType.ERROR,
          message: `'@Styles' decorated functions and methods cannot have arguments.`,
          pos: node.getStart(),
          code: '10905105'
        });
      }
      return undefined;
    }
  }
  return updateItem;
}

function checkDecoratorMethod(node: ts.MethodDeclaration, modifiers: readonly ts.Modifier[], log: LogInfo[]): void {
  if (modifiers && modifiers.length) {
    for (let i = 0; i < modifiers.length; i++) {
      if (modifiers[i].kind && modifiers[i].kind === ts.SyntaxKind.StaticKeyword) {
        log.push({
          type: LogType.ERROR,
          message: `Static methods in custom components cannot be decorated by '@LocalBuilder'.`,
          pos: node.getStart(),
          code: '10905104'
        });
        return;
      }
    }
  }
}

export function isBuilderOrLocalBuilder(node: ts.MethodDeclaration | ts.FunctionDeclaration, builderCondition: builderConditionType,
  customBuilder: ts.Decorator[] = undefined): boolean {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (decorators && decorators.length) {
    for (let i = 0; i < decorators.length; i++) {
      const originalDecortor: string = decorators[i].getText().replace(/\(.*\)$/, '').replace(/\s*/g, '').trim();
      if ([COMPONENT_LOCAL_BUILDER_DECORATOR, COMPONENT_BUILDER_DECORATOR].includes(originalDecortor)) {
        if (originalDecortor === COMPONENT_BUILDER_DECORATOR) {
          builderCondition.isBuilder = true;
          if (customBuilder) {
            customBuilder.push(...decorators.slice(i + 1), ...decorators.slice(0, i));
          }
        } else {
          builderCondition.isLocalBuilder = true;
        }
        return true;
      }
    }
  }
  return false;
}

function localBuilderNode(node: ts.MethodDeclaration, componentBlock: ts.Block): ts.PropertyDeclaration {
  return ts.factory.createPropertyDeclaration(
    undefined, node.name, undefined, undefined,
    ts.factory.createArrowFunction(
      undefined, undefined, node.parameters ? node.parameters : [], undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      componentBlock
    )
  );
}

export function createParentParameter(): ts.ParameterDeclaration {
  return ts.factory.createParameterDeclaration(undefined, undefined,
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARENT), undefined, undefined,
    ts.factory.createIdentifier(NULL));
}

export function processBuildMember(node: ts.MethodDeclaration | ts.FunctionDeclaration, context: ts.TransformationContext,
  log: LogInfo[], isBuilder = false): ts.MethodDeclaration | ts.FunctionDeclaration {
  return ts.visitNode(node, visitBuild);
  function visitBuild(node: ts.Node): ts.Node {
    if (isGeometryView(node)) {
      node = processGeometryView(node as ts.ExpressionStatement, log);
    }
    if (isProperty(node)) {
      node = createReference(node as ts.PropertyAssignment, log, isBuilder);
    }
    if (isNeedGetRawObject(node)) {
      return ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(FOREACH_OBSERVED_OBJECT),
        ts.factory.createIdentifier(FOREACH_GET_RAW_OBJECT)), undefined, [node]);
    }
    return ts.visitEachChild(node, visitBuild, context);
  }
}

function checkStateName(node: ts.PropertyAccessExpression): string {
  if (node.expression && !node.expression.expression && node.name && ts.isIdentifier(node.name)) {
    return node.name.escapedText.toString();
  }
  return null;
}

// EnableV2Compatibility MakeV1Observed Do not generate ObservedObject GetRawObject.
function isNeedGetRawObject(node: ts.Node): boolean {
  return !isInComponentV2Context() && !isEnableV2CompatibilityOrMakeV1Observed(node) && ts.isPropertyAccessExpression(node) &&
    ts.isIdentifier(node.name) && stateObjectCollection.has(checkStateName(node)) && node.parent && ts.isCallExpression(node.parent) &&
    ts.isPropertyAccessExpression(node.parent.expression) && node !== node.parent.expression &&
    node.parent.expression.name.escapedText.toString() !== FOREACH_GET_RAW_OBJECT;
}

function isEnableV2CompatibilityOrMakeV1Observed(node: ts.Node): boolean {
  if (node.parent && ts.isCallExpression(node.parent) && ts.isPropertyAccessExpression(node.parent.expression) &&
    ts.isIdentifier(node.parent.expression.name) &&
    ['enableV2Compatibility', 'makeV1Observed'].includes(node.parent.expression.name.escapedText.toString())) {
    return true;
  } else {
    return false;
  }
}

function isInComponentV2Context(): boolean {
  if (componentCollection.currentClassName) {
    const parentStructInfo: StructInfo =
      processStructComponentV2.getOrCreateStructInfo(componentCollection.currentClassName);
    return parentStructInfo.isComponentV2;
  }
  return false;
}

function isGeometryView(node: ts.Node): boolean {
  if (ts.isExpressionStatement(node) && ts.isCallExpression(node.expression)) {
    const call: ts.CallExpression = node.expression;
    const exp: ts.Expression = call.expression;
    const args: ts.NodeArray<ts.Expression> = call.arguments;
    if (ts.isPropertyAccessExpression(exp) && ts.isIdentifier(exp.expression) &&
      exp.expression.escapedText.toString() === GEOMETRY_VIEW && ts.isIdentifier(exp.name) &&
      exp.name.escapedText.toString() === COMPONENT_CREATE_FUNCTION && args && args.length === 1 &&
      (ts.isArrowFunction(args[0]) || ts.isFunctionExpression(args[0]))) {
      return true;
    }
  }
  return false;
}

function processGeometryView(node: ts.ExpressionStatement,
  log: LogInfo[]): ts.ExpressionStatement {
  const exp: ts.CallExpression = node.expression as ts.CallExpression;
  const arg: ts.ArrowFunction | ts.FunctionExpression =
    exp.arguments[0] as ts.ArrowFunction | ts.FunctionExpression;
  return ts.factory.updateExpressionStatement(node, ts.factory.updateCallExpression(exp,
    exp.expression, undefined, [ts.factory.createArrowFunction(undefined, undefined, arg.parameters,
      undefined, ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      getGeometryReaderFunctionBlock(arg, log))]));
}

function getGeometryReaderFunctionBlock(node: ts.ArrowFunction | ts.FunctionExpression,
  log: LogInfo[]): ts.Block {
  let blockNode: ts.Block;
  if (ts.isBlock(node.body)) {
    blockNode = node.body;
  } else if (ts.isArrowFunction(node) && ts.isCallExpression(node.body)) {
    blockNode = ts.factory.createBlock([ts.factory.createExpressionStatement(node.body)]);
  }
  return processComponentBlock(blockNode, false, log);
}

export function updateHeritageClauses(node: ts.StructDeclaration, log: LogInfo[],
  isComponentV2: boolean = false): ts.NodeArray<ts.HeritageClause> {
  if (node.heritageClauses && !checkHeritageClauses(node)) {
    log.push({
      type: LogType.ERROR,
      message: 'Structs are not allowed to inherit from classes or implement interfaces.',
      pos: node.heritageClauses.pos,
      code: '10905212'
    });
  }
  const result: ts.HeritageClause[] = [];
  const heritageClause: ts.HeritageClause = createHeritageClause(isComponentV2);
  result.push(heritageClause);
  return ts.factory.createNodeArray(result);
}

function checkHeritageClauses(node: ts.StructDeclaration): boolean {
  if (node.heritageClauses.length === 1 && node.heritageClauses[0].types &&
    node.heritageClauses[0].types.length === 1) {
    const expressionNode: ts.ExpressionWithTypeArguments = node.heritageClauses[0].types[0];
    if (expressionNode.expression && ts.isIdentifier(expressionNode.expression) &&
      expressionNode.expression.escapedText.toString() === CUSTOM_COMPONENT) {
      return true;
    }
  }
  return false;
}

export function isProperty(node: ts.Node): Boolean {
  if (judgmentParentType(node)) {
    if (node.parent.parent.expression && ts.isIdentifier(node.parent.parent.expression) &&
      !BUILDIN_STYLE_NAMES.has(node.parent.parent.expression.escapedText.toString()) &&
      componentCollection.customComponents.has(
        node.parent.parent.expression.escapedText.toString())) {
      return true;
    } else if (ts.isPropertyAccessExpression(node.parent.parent.expression) &&
      ts.isIdentifier(node.parent.parent.expression.expression) &&
      componentCollection.customComponents.has(
        node.parent.parent.expression.name.escapedText.toString())) {
      return true;
    }
  }
  return false;
}

function judgmentParentType(node: ts.Node): boolean {
  return ts.isPropertyAssignment(node) && node.name && ts.isIdentifier(node.name) &&
    node.parent && ts.isObjectLiteralExpression(node.parent) && node.parent.parent &&
    (ts.isCallExpression(node.parent.parent) || ts.isEtsComponentExpression(node.parent.parent));
}

export function createReference(node: ts.PropertyAssignment, log: LogInfo[], isBuilder = false,
  isParamsLambda: boolean = false, isRecycleComponent: boolean = false): ts.PropertyAssignment {
  const linkParentComponent: string[] = getParentNode(node, linkCollection).slice(1);
  const propertyName: ts.Identifier = node.name as ts.Identifier;
  let initText: string;
  const LINK_REG: RegExp = /^\$/g;
  if (isRecycleComponent && ts.isShorthandPropertyAssignment(node)) {
    return node;
  }
  const initExpression: ts.Expression = node.initializer;
  let is$$: boolean = false;
  if (ts.isIdentifier(initExpression) &&
    initExpression.escapedText.toString().match(LINK_REG)) {
    initText = initExpression.escapedText.toString().replace(LINK_REG, '');
  } else if (ts.isPropertyAccessExpression(initExpression) && initExpression.expression &&
    initExpression.expression.kind === ts.SyntaxKind.ThisKeyword &&
    ts.isIdentifier(initExpression.name) && initExpression.name.escapedText.toString().match(LINK_REG)) {
    initText = initExpression.name.escapedText.toString().replace(LINK_REG, '');
  } else if (isBuilder && ts.isPropertyAccessExpression(initExpression) && initExpression.expression &&
    ts.isIdentifier(initExpression.expression) && initExpression.expression.escapedText.toString() === $$ &&
    ts.isIdentifier(initExpression.name) && linkParentComponent.includes(propertyName.escapedText.toString())) {
    is$$ = true;
    initText = initExpression.name.escapedText.toString();
  } else if (isMatchInitExpression(initExpression) &&
    linkParentComponent.includes(propertyName.escapedText.toString())) {
    initText = initExpression.name.escapedText.toString().replace(LINK_REG, '');
  }
  if (initText) {
    node = addDoubleUnderline(node, propertyName, initText, is$$, isParamsLambda, isRecycleComponent);
  }
  return node;
}

function isMatchInitExpression(initExpression: ts.Expression): boolean {
  return ts.isPropertyAccessExpression(initExpression) &&
    initExpression.expression &&
    initExpression.expression.kind === ts.SyntaxKind.ThisKeyword &&
    ts.isIdentifier(initExpression.name);
}

function addDoubleUnderline(node: ts.PropertyAssignment, propertyName: ts.Identifier,
  initText: string, is$$ = false, isParamsLambda: boolean, isRecycleComponent: boolean): ts.PropertyAssignment {
  return ts.factory.updatePropertyAssignment(node, propertyName,
    ts.factory.createPropertyAccessExpression(
      is$$ && partialUpdateConfig.partialUpdateMode ? ts.factory.createIdentifier($$) : ts.factory.createThis(),
      isParamsLambda || isRecycleComponent ? ts.factory.createIdentifier(initText) : ts.factory.createIdentifier(`__${initText}`)));
}

function getParentNode(node: ts.PropertyAssignment, collection: Map<string, Set<string>>): string[] {
  const grandparentNode: ts.NewExpression = node.parent.parent as ts.NewExpression;
  const grandparentExpression: ts.Identifier | ts.PropertyAccessExpression =
    grandparentNode.expression as ts.Identifier | ts.PropertyAccessExpression;
  let parentComponent: Set<string> = new Set();
  let grandparentName: string;
  if (ts.isIdentifier(grandparentExpression)) {
    grandparentName = grandparentExpression.escapedText.toString();
    parentComponent = collection.get(grandparentName);
  } else if (ts.isPropertyAccessExpression(grandparentExpression)) {
    if (storedFileInfo.isAsPageImport) {
      grandparentName = grandparentExpression.getText();
    } else {
      grandparentName = grandparentExpression.name.escapedText.toString();
    }
    parentComponent = collection.get(grandparentName);
  } else {
    // ignore
  }
  if (!parentComponent) {
    parentComponent = new Set();
  }
  return [grandparentName, ...parentComponent];
}

function addUpdateParamsFunc(statements: ts.Statement[], parentComponentName: ts.Identifier):
  ts.MethodDeclaration {
  return createParamsInitBlock(COMPONENT_CONSTRUCTOR_UPDATE_PARAMS, statements, parentComponentName);
}

function addInitialParamsFunc(statements: ts.Statement[], parentComponentName: ts.Identifier): ts.MethodDeclaration {
  return createParamsInitBlock(COMPONENT_CONSTRUCTOR_INITIAL_PARAMS, statements, parentComponentName);
}

function addUpdateStateVarsFunc(statements: ts.Statement[], parentComponentName: ts.Identifier): ts.MethodDeclaration {
  return createParamsInitBlock(COMPONENT_UPDATE_STATE_VARS, statements, parentComponentName);
}

function addPurgeVariableDepFunc(statements: ts.Statement[]): ts.MethodDeclaration {
  return ts.factory.createMethodDeclaration(
    undefined, undefined,
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PURGE_VARIABLE_DEP),
    undefined, undefined, [ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(RMELMTID), undefined, undefined, undefined)], undefined,
    ts.factory.createBlock(statements, true));
}

function addDeleteParamsFunc(statements: ts.PropertyDeclaration[],
  updateRecyle: boolean = false): ts.MethodDeclaration {
  const deleteStatements: ts.ExpressionStatement[] = [];
  const updateStatements: ts.ExpressionStatement[] = [];
  statements.forEach((statement: ts.PropertyDeclaration) => {
    const name: ts.Identifier = statement.name as ts.Identifier;
    let paramsStatement: ts.ExpressionStatement;
    const decorators: readonly ts.Decorator[] = ts.getAllDecorators(statement);
    const isRegular: boolean = isRegularProperty(decorators);
    if (!partialUpdateConfig.partialUpdateMode || !isRegular) {
      paramsStatement = createParamsWithUnderlineStatement(name);
    }
    if (partialUpdateConfig.partialUpdateMode && !isRegular) {
      updateStatements.push(createElmtIdWithUnderlineStatement(name));
    }
    deleteStatements.push(paramsStatement);
  });
  if (partialUpdateConfig.partialUpdateMode && updateRecyle) {
    return createRecycleElmt(updateStatements);
  }
  const defaultStatement: ts.ExpressionStatement =
    ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(CREATE_CONSTRUCTOR_SUBSCRIBER_MANAGER),
          ts.factory.createIdentifier(CREATE_CONSTRUCTOR_GET_FUNCTION)), undefined, []),
        ts.factory.createIdentifier(CREATE_CONSTRUCTOR_DELETE_FUNCTION)),
      undefined, [ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
        ts.factory.createThis(), ts.factory.createIdentifier(
          !partialUpdateConfig.partialUpdateMode ?
            ABOUT_TO_BE_DELETE_FUNCTION_ID : ABOUT_TO_BE_DELETE_FUNCTION_ID__)),
      undefined, [])]));
  deleteStatements.push(defaultStatement);
  if (partialUpdateConfig.partialUpdateMode) {
    const aboutToBeDeletedInternalStatement: ts.ExpressionStatement = createDeletedInternalStatement();
    deleteStatements.push(aboutToBeDeletedInternalStatement);
  }
  const deleteParamsMethod: ts.MethodDeclaration =
    createParamsInitBlock(COMPONENT_CONSTRUCTOR_DELETE_PARAMS, deleteStatements);
  return deleteParamsMethod;
}

function createRecycleElmt(statements: ts.Statement[]): ts.MethodDeclaration {
  return ts.factory.createMethodDeclaration(undefined, undefined,
    ts.factory.createIdentifier(UPDATE_RECYCLE_ELMT_ID), undefined, undefined, [
      ts.factory.createParameterDeclaration(undefined, undefined,
        ts.factory.createIdentifier(OLD_ELMT_ID)),
      ts.factory.createParameterDeclaration(undefined, undefined,
        ts.factory.createIdentifier(NEW_ELMT_ID))
    ], undefined, ts.factory.createBlock(statements, true));
}

function createParamsWithUnderlineStatement(name: ts.Identifier): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
      ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
        ts.factory.createIdentifier(`__${name.escapedText.toString()}`)),
      ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_DELETE_PARAMS)), undefined, []));
}

function createElmtIdWithUnderlineStatement(name: ts.Identifier): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
      ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
        ts.factory.createIdentifier(`__${name.escapedText.toString()}`)),
      ts.factory.createIdentifier(COMPONENT_UPDATE_ELMT_ID)), undefined, [
      ts.factory.createIdentifier(OLD_ELMT_ID), ts.factory.createIdentifier(NEW_ELMT_ID)
    ]));
}

function createDeletedInternalStatement(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createThis(),
      ts.factory.createIdentifier(ABOUTTOBEDELETEDINTERNAL)), undefined, []));
}

export function addRerenderFunc(statements: ts.Statement[]): ts.MethodDeclaration {
  const updateDirtyElementStatement: ts.Statement = ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(), ts.factory.createIdentifier(UPDATEDIRTYELEMENTS)), undefined, []));
  statements.push(updateDirtyElementStatement);
  if (storedFileInfo.hasLocalBuilderInFile) {
    statements.unshift(contextStackPushOrPop(ts.factory.createIdentifier(PUSH), [ts.factory.createThis()]));
    statements.push(contextStackPushOrPop(ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), []));
  }
  return ts.factory.createMethodDeclaration(undefined, undefined,
    ts.factory.createIdentifier(COMPONENT_RERENDER_FUNCTION), undefined, undefined, [], undefined,
    ts.factory.createBlock(statements, true));
}

function createParamsInitBlock(express: string, statements: ts.Statement[],
  parentComponentName?: ts.Identifier): ts.MethodDeclaration {
  const methodDeclaration: ts.MethodDeclaration = ts.factory.createMethodDeclaration(
    undefined, undefined, ts.factory.createIdentifier(express), undefined, undefined,
    [ts.factory.createParameterDeclaration(undefined, undefined,
      express === COMPONENT_CONSTRUCTOR_DELETE_PARAMS ? undefined :
        ts.factory.createIdentifier(CREATE_CONSTRUCTOR_PARAMS), undefined,
      express === COMPONENT_CONSTRUCTOR_DELETE_PARAMS ? undefined :
        ts.factory.createTypeReferenceNode(
          ts.factory.createIdentifier(parentComponentName.getText() + INTERFACE_NAME_SUFFIX), undefined),
      undefined)], undefined, ts.factory.createBlock(statements, true));
  return methodDeclaration;
}

export function validateBuildMethodCount(buildCount: BuildCount, parentComponentName: ts.Identifier,
  log: LogInfo[]): void {
  if (buildCount.count !== 1) {
    log.push({
      type: LogType.ERROR,
      message: `The struct '${parentComponentName.getText()}' must have at least and at most one 'build' method.`,
      pos: parentComponentName.getStart(),
      code: '10905103',
      solutions: [`A structurally modified page must have at least one and no more than one 'build' method.`]
    });
  }
}

function validateHasControllerAndControllerCount(componentName: ts.Identifier, checkController: ControllerType,
  log: LogInfo[]): void {
  if (!checkController.hasController) {
    log.push({
      type: LogType.ERROR,
      message: `The '@CustomDialog' decorated custom component must contain a property of the CustomDialogController type.`,
      pos: componentName.pos,
      code: '10905211'
    });
  }
  if (checkController.unassignedControllerSet.size >= 2) {
    log.push({
      type: LogType.WARN,
      message: 'A @CustomDialog component can only have one uninitialized CustomDialogController.',
      pos: componentName.pos
    });
  }
}

function createHeritageClause(isComponentV2: boolean = false): ts.HeritageClause {
  if (partialUpdateConfig.partialUpdateMode) {
    return ts.factory.createHeritageClause(
      ts.SyntaxKind.ExtendsKeyword,
      [ts.factory.createExpressionWithTypeArguments(
        ts.factory.createIdentifier(isComponentV2 ? constantDefine.STRUCT_PARENT : BASE_COMPONENT_NAME_PU),
        [])]
    );
  }
  return ts.factory.createHeritageClause(
    ts.SyntaxKind.ExtendsKeyword,
    [ts.factory.createExpressionWithTypeArguments(ts.factory.createIdentifier(BASE_COMPONENT_NAME), [])]
  );
}

function createTypeReference(decoratorName: string, type: ts.TypeNode, log: LogInfo[],
  program: ts.Program): ts.TypeNode {
  let newType: ts.TypeNode;
  switch (decoratorName) {
    case COMPONENT_STATE_DECORATOR:
    case COMPONENT_PROVIDE_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        isSimpleType(type, program, log) ?
          OBSERVED_PROPERTY_SIMPLE :
          OBSERVED_PROPERTY_OBJECT,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_LINK_DECORATOR:
    case COMPONENT_CONSUME_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        isSimpleType(type, program, log) ?
          SYNCHED_PROPERTY_SIMPLE_TWO_WAY :
          SYNCHED_PROPERTY_SIMPLE_ONE_WAY,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_PROP_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        SYNCHED_PROPERTY_SIMPLE_ONE_WAY,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_OBJECT_LINK_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        SYNCHED_PROPERTY_NESED_OBJECT,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_STORAGE_PROP_DECORATOR:
    case COMPONENT_STORAGE_LINK_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(OBSERVED_PROPERTY_ABSTRACT, [
        type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)
      ]);
      break;
    case COMPONENT_LOCAL_STORAGE_LINK_DECORATOR:
    case COMPONENT_LOCAL_STORAGE_PROP_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(OBSERVED_PROPERTY_ABSTRACT, [
        type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)
      ]);
      break;
  }
  return newType;
}

function createTypeReferencePU(decoratorName: string, type: ts.TypeNode, log: LogInfo[],
  program: ts.Program): ts.TypeNode {
  let newType: ts.TypeNode;
  switch (decoratorName) {
    case COMPONENT_STATE_DECORATOR:
    case COMPONENT_PROVIDE_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        isSimpleType(type, program, log) ?
          OBSERVED_PROPERTY_SIMPLE_PU :
          OBSERVED_PROPERTY_OBJECT_PU,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_LINK_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        isSimpleType(type, program, log) ?
          SYNCHED_PROPERTY_SIMPLE_TWO_WAY_PU :
          SYNCHED_PROPERTY_SIMPLE_ONE_WAY_PU,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_PROP_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        SYNCHED_PROPERTY_SIMPLE_ONE_WAY_PU,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_OBJECT_LINK_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(
        SYNCHED_PROPERTY_NESED_OBJECT_PU,
        [type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)]
      );
      break;
    case COMPONENT_CONSUME_DECORATOR:
    case COMPONENT_STORAGE_PROP_DECORATOR:
    case COMPONENT_STORAGE_LINK_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(OBSERVED_PROPERTY_ABSTRACT_PU, [
        type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)
      ]);
      break;
    case COMPONENT_LOCAL_STORAGE_LINK_DECORATOR:
    case COMPONENT_LOCAL_STORAGE_PROP_DECORATOR:
      newType = ts.factory.createTypeReferenceNode(OBSERVED_PROPERTY_ABSTRACT_PU, [
        type || ts.factory.createKeywordTypeNode(ts.SyntaxKind.AnyKeyword)
      ]);
      break;
  }
  return newType;
}

export function checkFinalizeConstruction(): ts.Statement {
  return ts.factory.createIfStatement(
    ts.factory.createPrefixUnaryExpression(ts.SyntaxKind.ExclamationToken,
      ts.factory.createParenthesizedExpression(ts.factory.createBinaryExpression(
        ts.factory.createStringLiteral(FINALIZE_CONSTRUCTION),
        ts.factory.createToken(ts.SyntaxKind.InKeyword), ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(BASE_COMPONENT_NAME_PU), ts.factory.createIdentifier(PROTOTYPE))))
    ),
    ts.factory.createBlock(
      [
        ts.factory.createExpressionStatement(ts.factory.createCallExpression(
          ts.factory.createPropertyAccessExpression(
            ts.factory.createIdentifier(REFLECT), ts.factory.createIdentifier(CREATE_SET_METHOD)
          ), undefined,
          [
            ts.factory.createPropertyAccessExpression(
              ts.factory.createIdentifier(BASE_COMPONENT_NAME_PU), ts.factory.createIdentifier(PROTOTYPE)
            ),
            ts.factory.createStringLiteral(FINALIZE_CONSTRUCTION),
            ts.factory.createArrowFunction(undefined, undefined, [], undefined,
              ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
              ts.factory.createBlock([], false)
            )
          ]
        ))
      ], true), undefined);
}

export function checkContextStack(): ts.Statement {
  return ts.factory.createIfStatement(
    ts.factory.createBinaryExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(PUV2_VIEW_BASE),
        ts.factory.createIdentifier(CONTEXT_STACK)
      ),
      ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
      ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED)
    ),
    ts.factory.createBlock(
      [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(REFLECT),
          ts.factory.createIdentifier(CREATE_SET_METHOD)
        ),
        undefined,
        [
          ts.factory.createIdentifier(PUV2_VIEW_BASE),
          ts.factory.createStringLiteral(CONTEXT_STACK),
          ts.factory.createArrayLiteralExpression(
            [],
            false
          )
        ]
      ))],
      true
    ),
    undefined
  );
}

export function contextStackPushOrPop(pushOrPop: ts.Identifier, param: ts.ThisExpression[] | []): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(PUV2_VIEW_BASE),
      ts.factory.createIdentifier(CONTEXT_STACK)
    ),
    ts.factory.createToken(ts.SyntaxKind.AmpersandAmpersandToken),
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(PUV2_VIEW_BASE),
          ts.factory.createIdentifier(CONTEXT_STACK)
        ),
        pushOrPop
      ),
      undefined,
      param
    )
  ));
}
