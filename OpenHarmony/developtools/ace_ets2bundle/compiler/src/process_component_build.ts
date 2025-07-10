/*
 * Copyright (c) 2021-2024 Huawei Device Co., Ltd.
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
import path from 'path';
import fs from 'fs';
import {
  COMPONENT_RENDER_FUNCTION,
  COMPONENT_CREATE_FUNCTION,
  COMPONENT_POP_FUNCTION,
  COMPONENT_BUTTON,
  COMPONENT_CREATE_LABEL_FUNCTION,
  COMPONENT_CREATE_CHILD_FUNCTION,
  COMPONENT_FOREACH,
  COMPONENT_LAZYFOREACH,
  IS_RENDERING_IN_PROGRESS,
  FOREACH_OBSERVED_OBJECT,
  FOREACH_GET_RAW_OBJECT,
  COMPONENT_IF,
  COMPONENT_IF_BRANCH_ID_FUNCTION,
  COMPONENT_IF_UNDEFINED,
  ATTRIBUTE_ANIMATION,
  GLOBAL_CONTEXT,
  COMPONENT_GESTURE,
  COMPONENT_GESTURE_GROUP,
  GESTURE_ATTRIBUTE,
  PARALLEL_GESTURE_ATTRIBUTE,
  PRIORITY_GESTURE_ATTRIBUTE,
  GESTURE_ENUM_KEY,
  GESTURE_ENUM_VALUE_HIGH,
  GESTURE_ENUM_VALUE_LOW,
  GESTURE_ENUM_VALUE_PARALLEL,
  COMPONENT_TRANSITION_NAME,
  COMPONENT_DEBUGLINE_FUNCTION,
  ATTRIBUTE_STATESTYLES,
  THIS,
  VISUAL_STATE,
  VIEW_STACK_PROCESSOR,
  STYLE_ADD_DOUBLE_DOLLAR,
  STYLE_ADD_DOUBLE_EXCLAMATION,
  $$_VALUE,
  $$_CHANGE_EVENT,
  $$_THIS,
  $$_NEW_VALUE,
  $_VALUE,
  BUILDER_ATTR_NAME,
  BUILDER_ATTR_BIND,
  CUSTOM_DIALOG_CONTROLLER_BUILDER,
  BIND_DRAG_SET,
  BIND_POPUP_SET,
  BIND_POPUP,
  CUSTOM_COMPONENT_DEFAULT,
  $$,
  PROPERTIES_ADD_DOUBLE_DOLLAR,
  PROPERTIES_ADD_DOUBLE_EXCLAMATION,
  ATTRIBUTE_ID,
  RESOURCE,
  ISINITIALRENDER,
  ELMTID,
  VIEWSTACKPROCESSOR,
  STOPGETACCESSRECORDING,
  STARTGETACCESSRECORDINGFOR,
  OBSERVECOMPONENTCREATION,
  OBSERVECOMPONENTCREATION2,
  DEEPRENDERFUNCTION,
  ITEMCREATION,
  ITEMCREATION2,
  OBSERVEDDEEPRENDER,
  ItemComponents,
  FOREACHITEMGENFUNCTION,
  __LAZYFOREACHITEMGENFUNCTION,
  _ITEM,
  FOREACHITEMIDFUNC,
  __LAZYFOREACHITEMIDFUNC,
  FOREACHUPDATEFUNCTION,
  COMPONENT_INITIAL_RENDER_FUNCTION,
  COMPONENT_REPEAT,
  REPEAT_EACH,
  REPEAT_TEMPLATE,
  LIST_ITEM,
  IFELSEBRANCHUPDATEFUNCTION,
  CARD_ENABLE_COMPONENTS,
  CARD_LOG_TYPE_COMPONENTS,
  COMPONENT_CONSTRUCTOR_PARENT,
  RESOURCE_NAME_TYPE,
  XCOMPONENT_SINGLE_QUOTATION,
  XCOMPONENT_DOUBLE_QUOTATION,
  XCOMPONENTTYPE,
  XCOMPONENTTYPE_CONTAINER,
  BIND_OBJECT_PROPERTY,
  TRUE,
  FALSE,
  HEADER,
  INDICATORBUILDER,
  FOOTER,
  CALL,
  CREATE_BIND_COMPONENT,
  TabContentAndNavDestination,
  START,
  END,
  BUILDER_PARAM_PROXY,
  BUILDER_TYPE,
  CHECK_COMPONENT_EXTEND_DECORATOR,
  CHECK_COMPONENT_ANIMATABLE_EXTEND_DECORATOR,
  RECYCLE_REUSE_ID,
  UPDATE_FUNC_BY_ELMT_ID,
  CREATE_SET_METHOD,
  CAN_RETAKE,
  PREVIEW,
  IDS,
  PUSH,
  UPDATE_LAZY_FOREACH_ELEMENTS,
  INDEX,
  IS_INITIAL_ITEM,
  MY_IDS,
  WRAPBUILDER_BUILDERPROP,
  WRAPPEDBUILDER_CLASS,
  ALL_COMPONENTS,
  ATTRIBUTE_ATTRIBUTE_MODIFIER,
  ATTRIBUTE_CONTENT_MODIFIER,
  ATTRIBUTE_MENUITEM_CONTENT_MODIFIER,
  TITLE,
  PAGE_PATH,
  RESOURCE_NAME_MODULE,
  NAV_DESTINATION,
  NAVIGATION,
  CREATE_ROUTER_COMPONENT_COLLECT,
  NAV_PATH_STACK,
  IS_USER_CREATE_STACK,
  REUSE_ATTRIBUTE
} from './pre_define';
import {
  INNER_COMPONENT_NAMES,
  BUILDIN_CONTAINER_COMPONENT,
  BUILDIN_STYLE_NAMES,
  CUSTOM_BUILDER_METHOD,
  GESTURE_ATTRS,
  GESTURE_TYPE_NAMES,
  EXTEND_ATTRIBUTE,
  NO_DEBUG_LINE_COMPONENT,
  NEEDPOP_COMPONENT,
  INNER_STYLE_FUNCTION,
  GLOBAL_STYLE_FUNCTION,
  CUSTOM_BUILDER_PROPERTIES,
  CUSTOM_BUILDER_PROPERTIES_WITHOUTKEY,
  CUSTOM_BUILDER_CONSTRUCTORS,
  ID_ATTRS,
  SPECIFIC_PARENT_COMPONENT,
  STYLES_ATTRIBUTE,
  INNER_CUSTOM_LOCALBUILDER_METHOD,
  COMMON_ATTRS,
  EXT_WHITE_LIST
} from './component_map';
import {
  componentCollection,
  builderParamObjectCollection,
  checkAllNode,
  enumCollection
} from './validate_ui_syntax';
import {
  processCustomComponent,
  createConditionParent,
  isRecycle,
  isReuseInV2
} from './process_custom_component';
import {
  LogType,
  LogInfo,
  componentInfo,
  storedFileInfo
} from './utils';
import {
  globalProgram,
  partialUpdateConfig,
  projectConfig
} from '../main';
import {
  transformLog,
  contextGlobal,
  validatorCard,
  builderTypeParameter,
  resourceFileName
} from './process_ui_syntax';
import { regularCollection, getSymbolIfAliased } from './validate_ui_syntax';
import { contextStackPushOrPop } from './process_component_class';
import processStructComponentV2, { StructInfo } from './process_struct_componentV2';
import logMessageCollection from './log_message_collection';
import {
  concatenateEtsOptions,
  getExternalComponentPaths
} from './external_component_map';

export function processComponentBuild(node: ts.MethodDeclaration,
  log: LogInfo[]): ts.MethodDeclaration {
  let newNode: ts.MethodDeclaration;
  let renderNode: ts.Identifier;
  if (!partialUpdateConfig.partialUpdateMode) {
    renderNode = ts.factory.createIdentifier(COMPONENT_RENDER_FUNCTION);
  } else {
    renderNode = ts.factory.createIdentifier(COMPONENT_INITIAL_RENDER_FUNCTION);
  }
  if (node.body && node.body.statements && node.body.statements.length &&
    validateRootNode(node, log)) {
    const componentBlock: ts.Block = processComponentBlock(node.body, false, log);
    newNode = ts.factory.updateMethodDeclaration(node, ts.getModifiers(node),
      node.asteriskToken, renderNode, node.questionToken, node.typeParameters, node.parameters,
      node.type, componentBlock);
    if (partialUpdateConfig.partialUpdateMode && storedFileInfo.hasLocalBuilderInFile) {
      componentBlock.statements.unshift(contextStackPushOrPop(ts.factory.createIdentifier(PUSH), [ts.factory.createThis()]));
      componentBlock.statements.push(contextStackPushOrPop(ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), [])); 
    }
  } else {
    newNode = ts.factory.updateMethodDeclaration(node, ts.getModifiers(node),
      node.asteriskToken, renderNode, node.questionToken, node.typeParameters, node.parameters,
      node.type, node.body);
  }
  return newNode;
}

function createLazyForEachBlockNode(newStatements: ts.Statement[]): ts.IfStatement {
  return ts.factory.createIfStatement(
    ts.factory.createBinaryExpression(
      ts.factory.createBinaryExpression(
        ts.factory.createIdentifier(IS_INITIAL_ITEM),
        ts.factory.createToken(ts.SyntaxKind.EqualsEqualsEqualsToken),
        ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED)
      ),
      ts.factory.createToken(ts.SyntaxKind.BarBarToken),
      ts.factory.createIdentifier(IS_INITIAL_ITEM)
    ),
    ts.factory.createBlock(newStatements, true),
    ts.factory.createBlock([
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(UPDATE_LAZY_FOREACH_ELEMENTS)
        ), undefined, [
          ts.factory.createIdentifier(IDS),
          storedFileInfo.lazyForEachInfo.forEachParameters.name as ts.Identifier
        ]
      ))
    ], true)
  );
}

export type BuilderParamsResult = {
  firstParam: ts.ParameterDeclaration;
};

export function parseGlobalBuilderParams(parameters: ts.NodeArray<ts.ParameterDeclaration>,
  builderParamsResult: BuilderParamsResult) : void {
  if (partialUpdateConfig.partialUpdateMode && parameters.length && parameters.length === 1 &&
    ts.isIdentifier(parameters[0].name)) {
    builderParamsResult.firstParam = parameters[0];
  }
}

export function processComponentBlock(node: ts.Block, isLazy: boolean, log: LogInfo[],
  isTransition: boolean = false, isBuilder: boolean = false, parent: string = undefined,
  forEachParameters: ts.NodeArray<ts.ParameterDeclaration> = undefined,
  isGlobalBuilder: boolean = false, builderParamsResult: BuilderParamsResult = null,
  rootGlobalBuilder: boolean = false, isInRepeatTemplate: boolean = false): ts.Block {
  const newStatements: ts.Statement[] = [];
  processComponentChild(node, newStatements, log, {isAcceleratePreview: false, line: 0, column: 0, fileName: ''},
    isBuilder, parent, forEachParameters, isGlobalBuilder, isTransition, builderParamsResult, isInRepeatTemplate);
  if (isLazy && !partialUpdateConfig.partialUpdateMode) {
    newStatements.unshift(createRenderingInProgress(true));
  }
  if (isTransition) {
    if (!partialUpdateConfig.partialUpdateMode) {
      newStatements.unshift(ts.factory.createExpressionStatement(
        createFunction(ts.factory.createIdentifier(COMPONENT_TRANSITION_NAME),
          ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), null)));
    } else {
      newStatements.unshift(createComponentCreationStatement(ts.factory.createExpressionStatement(
        createFunction(ts.factory.createIdentifier(COMPONENT_TRANSITION_NAME),
          ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null)), [ts.factory.createExpressionStatement(
        createFunction(ts.factory.createIdentifier(COMPONENT_TRANSITION_NAME),
          ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), null))], COMPONENT_TRANSITION_NAME, false, isTransition));
    }
    newStatements.push(ts.factory.createExpressionStatement(
      createFunction(ts.factory.createIdentifier(COMPONENT_TRANSITION_NAME),
        ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null)));
  }
  if (isLazy && !partialUpdateConfig.partialUpdateMode) {
    newStatements.push(createRenderingInProgress(false));
  }
  if (rootGlobalBuilder && isGlobalBuilder && builderParamsResult && builderParamsResult.firstParam) {
    newStatements.unshift(forkBuilderParamNode(builderParamsResult.firstParam));
  }
  if (isLazy && projectConfig.optLazyForEach && storedFileInfo.processLazyForEach &&
    storedFileInfo.lazyForEachInfo.forEachParameters) {
    return ts.factory.updateBlock(node, [
      createMyIdsNode(),
      createLazyForEachBlockNode(newStatements),
      ts.factory.createReturnStatement(ts.factory.createIdentifier(MY_IDS))
    ]);
  }
  return ts.factory.updateBlock(node, newStatements);
}

function createMyIdsNode(): ts.Statement {
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(MY_IDS),
        undefined,
        undefined,
        ts.factory.createArrayLiteralExpression(
          [],
          false
        )
      )],
      ts.NodeFlags.Const
    )
  );
}

function visitComponent(node: ts.Node): void {
  if (storedFileInfo.lazyForEachInfo && !ts.isBlock(node)) {
    ts.forEachChild(node, (child: ts.Node) => {
      if (storedFileInfo.lazyForEachInfo.isDependItem) {
        return;
      }
      if (ts.isIdentifier(child)) {
        const symbol: ts.Symbol = globalProgram.checker.getSymbolAtLocation(child);
        if (symbol && symbol.valueDeclaration === storedFileInfo.lazyForEachInfo.forEachParameters) {
          storedFileInfo.lazyForEachInfo.isDependItem = true;
          return;
        }
      }
      visitComponent(child);
    });
  }
}

function forkBuilderParamNode(node: ts.ParameterDeclaration): ts.Statement {
  const paramNode: ts.Identifier = node.name as ts.Identifier;
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(`__${paramNode.escapedText.toString()}__`),
        undefined,
        undefined,
        paramNode
      )],
      ts.NodeFlags.Const
    )
  );
}

function validateRootNode(node: ts.MethodDeclaration, log: LogInfo[]): boolean {
  let isValid: boolean = false;
  if (node.body.statements.length === 1) {
    const statement: ts.Statement = node.body.statements[0];
    if (ts.isIfStatement(statement) || validateFirstNode(statement)) {
      isValid = true;
    }
  } else {
    isValid = false;
  }
  if (!isValid) {
    log.push({
      type: LogType.ERROR,
      message: `In an '@Entry' decorated component, the 'build' method can have only one root node,` +
        ` which must be a container component.`,
      pos: node.body.statements.pos,
      code: '10905210'
    });
  }
  return isValid;
}

function validateFirstNode(node: ts.Statement): boolean {
  const isEntryComponent: boolean =
    componentCollection.entryComponent === componentCollection.currentClassName;
  if (isEntryComponent && !validateContainerComponent(node)) {
    return false;
  }
  return true;
}

function validateContainerComponent(node: ts.Statement): boolean {
  if (ts.isExpressionStatement(node) && node.expression &&
    (ts.isEtsComponentExpression(node.expression) || ts.isCallExpression(node.expression))) {
    const nameResult: NameResult = { name: null, node: null, arguments: [] };
    validateEtsComponentNode(node.expression, nameResult);
    if (nameResult.name && checkContainer(nameResult.name, nameResult.node)) {
      return true;
    }
  }
  return false;
}

interface supplementType {
  isAcceleratePreview: boolean,
  line: number,
  column: number,
  fileName: string
}

let newsupplement: supplementType = {
  isAcceleratePreview: false,
  line: 0,
  column: 0,
  fileName: ''
};

type NameResult = {
  name: string,
  arguments: ts.NodeArray<ts.Expression> | [],
  node?: ts.Node
};

function validateEtsComponentNode(node: ts.CallExpression | ts.EtsComponentExpression, result?: NameResult): boolean {
  let childNode: ts.Node = node;
  result.name = null;
  while (ts.isCallExpression(childNode) && childNode.expression &&
    ts.isPropertyAccessExpression(childNode.expression) && childNode.expression.expression) {
    childNode = childNode.expression.expression;
  }
  if (ts.isEtsComponentExpression(childNode)) {
    if (ts.isIdentifier(childNode.expression)) {
      result.name = childNode.expression.getText();
      result.node = childNode;
      result.arguments = childNode.arguments || [];
    }
    return true;
  } else {
    return false;
  }
}

let sourceNode: ts.SourceFile;

export function processComponentChild(node: ts.Block | ts.SourceFile, newStatements: ts.Statement[],
  log: LogInfo[], supplement: supplementType = {isAcceleratePreview: false, line: 0, column: 0, fileName: ''},
  isBuilder: boolean = false, parent: string = undefined,
  forEachParameters: ts.NodeArray<ts.ParameterDeclaration> = undefined, isGlobalBuilder: boolean = false,
  isTransition: boolean = false, builderParamsResult: BuilderParamsResult = null,
  isInRepeatTemplate: boolean = false): void {
  if (supplement.isAcceleratePreview) {
    newsupplement = supplement;
    let compilerOptions = ts.readConfigFile(
      path.resolve(__dirname, '../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
    const componentPaths: string[] | undefined = getExternalComponentPaths();
    if (componentPaths) {
      for (const componentPath of componentPaths) {
        if (!fs.existsSync(componentPath)) {
          continue;
        }
        const externalCompilerOptions: ts.CompilerOptions = ts.readConfigFile(
          path.resolve(componentPath, 'externalconfig.json'), ts.sys.readFile
        ).config.compilerOptions;
        concatenateEtsOptions(compilerOptions, externalCompilerOptions);
      }
    }
    Object.assign(compilerOptions, {
      'sourceMap': false
    });
    sourceNode = ts.createSourceFile('', node.getText(), ts.ScriptTarget.Latest, true, ts.ScriptKind.ETS, compilerOptions);
  }
  if (node.statements.length) {
    // Save parent component
    const savedParent: string = parent;
    node.statements.forEach((item) => {
      if (ts.isExpressionStatement(item)) {
        assignParameter(forEachParameters, item);
        checkEtsComponent(item, log);
        const name: string = getName(item);
        if (CARD_ENABLE_COMPONENTS.has(name)) {
          validatorCard(log, CARD_LOG_TYPE_COMPONENTS, item.getStart(), name);
        }
        switch (getComponentType(item, log, name, parent, forEachParameters)) {
          case ComponentType.innerComponent: {
            const [etsExpression, idName]: [ts.EtsComponentExpression, ts.Expression] =
              checkEtsAndIdInIf(item, savedParent);
            if (ts.isIdentifier(etsExpression.expression)) {
              parent = etsExpression.expression.escapedText.toString();
            }
            processInnerComponent(item, newStatements, log, parent, isBuilder, isGlobalBuilder,
              isTransition, idName, savedParent, builderParamsResult, isInRepeatTemplate);
            break;
          }
          case ComponentType.customComponent: {
            const idName: ts.Expression = checkIdInIf(item, savedParent);
            parent = undefined;
            if (!newsupplement.isAcceleratePreview) {
              if (item.expression && ts.isEtsComponentExpression(item.expression) && item.expression.body) {
                const expressionResult: ts.ExpressionStatement =
                  processExpressionStatementChange(item, item.expression.body, log);
                if (expressionResult) {
                  item = expressionResult;
                }
              }
              processCustomComponent(item as ts.ExpressionStatement, newStatements, log, name,
                isBuilder, isGlobalBuilder, idName, builderParamsResult, isInRepeatTemplate);
            }
            break;
          }
          case ComponentType.forEachComponent:
            parent = undefined;
            if (!partialUpdateConfig.partialUpdateMode) {
              processForEachComponent(item, newStatements, log, isBuilder, isGlobalBuilder);
            } else {
              processForEachComponentNew(item, newStatements, log, name, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
            }
            break;
          case ComponentType.repeatComponent:
            parent = undefined;
            processRepeatComponent(item, newStatements, log, isBuilder, isGlobalBuilder, isTransition, builderParamsResult, isInRepeatTemplate);
            break;
          case ComponentType.customBuilderMethod:
            parent = undefined;
            if (partialUpdateConfig.partialUpdateMode) {
              newStatements.push(transferBuilderCall(item, name, isBuilder));
            } else {
              newStatements.push(addInnerBuilderParameter(item, isGlobalBuilder));
            }
            break;
          case ComponentType.builderParamMethod:
            parent = undefined;
            if (partialUpdateConfig.partialUpdateMode) {
              newStatements.push(transferBuilderCall(item, name, isBuilder));
            } else {
              newStatements.push(addInnerBuilderParameter(item));
            }
            break;
          case ComponentType.builderTypeFunction:
            parent = undefined;
            if (partialUpdateConfig.partialUpdateMode) {
              newStatements.push(transferBuilderCall(item, name, isBuilder));
            } else {
              newStatements.push(addInnerBuilderParameter(item));
            }
            break;
          case ComponentType.function:
            parent = undefined;
            newStatements.push(item);
            break;
        }
      } else if (ts.isIfStatement(item)) {
        assignParameter(forEachParameters, item);
        processIfStatement(item, newStatements, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
      } else if (!ts.isBlock(item)) {
        log.push({
          type: LogType.ERROR,  
          message: `Only UI component syntax can be written here.`,
          pos: item.getStart(),
          code: '10905209'
        });
      }
      storedFileInfo.lazyForEachInfo.isDependItem = false;
    });
  }
  if (supplement.isAcceleratePreview) {
    newsupplement = {
      isAcceleratePreview: false,
      line: 0,
      column: 0,
      fileName: ''
    };
  }
}

function assignParameter(forEachParameters: ts.NodeArray<ts.ParameterDeclaration>, item: ts.Node): void {
  if (partialUpdateConfig.partialUpdateMode && projectConfig.optLazyForEach &&
    storedFileInfo.processLazyForEach) {
    if (forEachParameters && forEachParameters[0]) {
      storedFileInfo.lazyForEachInfo.forEachParameters = forEachParameters[0];
    }
    if (storedFileInfo.lazyForEachInfo.forEachParameters) {
      visitComponent(item);
    }
  }
}

export function transferBuilderCall(node: ts.ExpressionStatement, name: string,
  isBuilder: boolean = false): ts.ExpressionStatement {
  if (node.expression && ts.isCallExpression(node.expression)) {
    let newNode: ts.Expression = builderCallNode(node.expression);
    newNode.expression.questionDotToken = node.expression.questionDotToken;
    if (node.expression.arguments && node.expression.arguments.length === 1 && ts.isObjectLiteralExpression(node.expression.arguments[0])) {
      return ts.factory.createExpressionStatement(ts.factory.updateCallExpression(
        node.expression,
        newNode,
        undefined,
        [ts.factory.createCallExpression(
          ts.factory.createIdentifier(BUILDER_PARAM_PROXY),
          undefined,
          [
            ts.factory.createStringLiteral(name),
            traverseBuilderParams(node.expression.arguments[0], isBuilder)
          ]
        )]
      ));
    } else {
      return ts.factory.createExpressionStatement(ts.factory.updateCallExpression(
        node.expression,
        newNode,
        undefined,
        !(projectConfig.optLazyForEach && (storedFileInfo.processLazyForEach &&
          storedFileInfo.lazyForEachInfo.forEachParameters || isBuilder)) ? node.expression.arguments :
          [...node.expression.arguments, ts.factory.createNull(), ts.factory.createIdentifier(MY_IDS)]
      ));
    }
  }
  return undefined;
}

function builderCallNode(node: ts.CallExpression): ts.Expression {
  let newNode: ts.Expression;
  if (node.expression && ts.isPropertyAccessExpression(node.expression) &&
    node.expression.questionDotToken && node.expression.questionDotToken.kind === ts.SyntaxKind.QuestionDotToken) {
    newNode = ts.factory.createCallChain(
      ts.factory.createPropertyAccessChain(
        node.expression,
        node.questionDotToken,
        ts.factory.createIdentifier(BUILDER_ATTR_BIND)
      ),
      undefined,
      undefined,
      [ts.factory.createThis()]
    );
  } else {
    newNode = ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        node.expression,
        ts.factory.createIdentifier(BUILDER_ATTR_BIND)
      ),
      undefined,
      [ts.factory.createThis()]
    );
  }
  return newNode;
}

function traverseBuilderParams(node: ts.ObjectLiteralExpression,
  isBuilder: boolean): ts.ObjectLiteralExpression {
  const properties: ts.ObjectLiteralElementLike[] = [];
  if (node.properties && node.properties.length) {
    node.properties.forEach(property => {
      if (ts.isPropertyAssignment(property) && property.initializer &&
        ts.isPropertyAccessExpression(property.initializer) && property.initializer.expression &&
        property.initializer.name && ts.isIdentifier(property.initializer.name)) {
        const name: string = property.initializer.name.escapedText.toString();
        if (!storedFileInfo.processGlobalBuilder && property.initializer.expression.kind === ts.SyntaxKind.ThisKeyword ||
          isBuilder && ts.isIdentifier(property.initializer.expression) &&
          property.initializer.expression.escapedText.toString() === $$) {
          const useThis: boolean = property.initializer.expression.kind === ts.SyntaxKind.ThisKeyword;
          addProperties(properties, property, name, isBuilder, useThis);
        } else {
          addBuilderParamsProperties(properties, property);
        }
      } else {
        addBuilderParamsProperties(properties, property);
      }
    });
  }
  return ts.factory.createObjectLiteralExpression(properties);
}

function addBuilderParamsProperties(properties: ts.ObjectLiteralElementLike[],
  property: ts.ObjectLiteralElementLike): void {
  const initializer: ts.Expression = ts.isShorthandPropertyAssignment(property) ?
    property.name : property.initializer;
  properties.push(ts.factory.createPropertyAssignment(
    property.name,
    ts.factory.createArrowFunction(
      undefined,
      undefined,
      [],
      undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      initializer
    )
  ));
}

function addProperties(properties: ts.ObjectLiteralElementLike[], property: ts.ObjectLiteralElementLike,
  name: string, isBuilder: boolean, useThis: boolean): void {
  properties.push(ts.factory.createPropertyAssignment(
    property.name,
    ts.factory.createArrowFunction(
      undefined,
      undefined,
      [],
      undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      ts.factory.createParenthesizedExpression(ts.factory.createConditionalExpression(
        ts.factory.createElementAccessExpression(
          (isBuilder && !useThis) ? ts.factory.createIdentifier($$) : ts.factory.createThis(),
          ts.factory.createStringLiteral('__' + name)
        ),
        ts.factory.createToken(ts.SyntaxKind.QuestionToken),
        ts.factory.createElementAccessExpression(
          (isBuilder && !useThis) ? ts.factory.createIdentifier($$) : ts.factory.createThis(),
          ts.factory.createStringLiteral('__' + name)
        ),
        ts.factory.createToken(ts.SyntaxKind.ColonToken),
        ts.factory.createElementAccessExpression(
          (isBuilder && !useThis) ? ts.factory.createIdentifier($$) : ts.factory.createThis(),
          ts.factory.createStringLiteral(name)
        )
      ))
    )
  ));
}

function addInnerBuilderParameter(node: ts.ExpressionStatement,
  isGlobalBuilder: boolean = false): ts.ExpressionStatement {
  if (node.expression && ts.isCallExpression(node.expression) && node.expression.arguments) {
    node.expression.arguments.push(isGlobalBuilder ? parentConditionalExpression() : ts.factory.createThis());
    return ts.factory.createExpressionStatement(ts.factory.updateCallExpression(node.expression,
      node.expression.expression, node.expression.typeArguments, node.expression.arguments));
  } else {
    return node;
  }
}

function processExpressionStatementChange(node: ts.ExpressionStatement, nextNode: ts.Block,
  log: LogInfo[]): ts.ExpressionStatement {
  let name: string;
  // @ts-ignore
  if (node.expression.expression && ts.isIdentifier(node.expression.expression)) {
    name = node.expression.expression.escapedText.toString();
  } else if (node.expression.expression && ts.isPropertyAccessExpression(node.expression.expression)) {
    name = node.expression.expression.getText();
  }
  if (builderParamObjectCollection.get(name) &&
    builderParamObjectCollection.get(name).size === 1) {
    return processBlockToExpression(node, nextNode, log, name, false);
  } else if (projectConfig.compileMode === 'esmodule' && process.env.compileTool === 'rollup' &&
    storedFileInfo.overallBuilderParamCollection.get(name) &&
    storedFileInfo.overallBuilderParamCollection.get(name).size === 1
  ) {
    return processBlockToExpression(node, nextNode, log, name, true);
  } else {
    log.push({
      type: LogType.ERROR,
      message: `In the trailing lambda case, '${name}' must have one and only one property decorated with ` +
        `'@BuilderParam', and its '@BuilderParam' expects no parameter.`,
      pos: node.getStart(),
      code: '10905102'
    });
    return null;
  }
}

function processBlockToExpression(node: ts.ExpressionStatement, nextNode: ts.Block,
  log: LogInfo[], name: string, isPropertyAccessExpressionNode: boolean): ts.ExpressionStatement {
  const childParam: string = isPropertyAccessExpressionNode ? [...storedFileInfo.overallBuilderParamCollection.get(name)].slice(-1)[0] :
    [...builderParamObjectCollection.get(name)].slice(-1)[0];
  const newBlock: ts.Block = processComponentBlock(nextNode, false, log);
  const arrowNode: ts.ArrowFunction = ts.factory.createArrowFunction(undefined, undefined,
    [], undefined, ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken), newBlock);
  const newPropertyAssignment:ts.PropertyAssignment = ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier(childParam), arrowNode);
  // @ts-ignore
  let argumentsArray: ts.ObjectLiteralExpression[] = node.expression.arguments;
  if (argumentsArray && !argumentsArray.length) {
    argumentsArray = [ts.factory.createObjectLiteralExpression([newPropertyAssignment], true)];
  } else if (ts.isObjectLiteralExpression(argumentsArray[0]) && argumentsArray.length === 1) {
    argumentsArray = [ts.factory.createObjectLiteralExpression(
      // @ts-ignore
      node.expression.arguments[0].properties.concat([newPropertyAssignment]), true)];
  } else if (ts.isObjectLiteralExpression(argumentsArray[0]) && argumentsArray.length === 2) {
    argumentsArray = [ts.factory.createObjectLiteralExpression(
      // @ts-ignore
      node.expression.arguments[0].properties.concat([newPropertyAssignment]), true), argumentsArray[1]];
  }
  const callNode: ts.CallExpression = ts.factory.updateCallExpression(
    // @ts-ignore
    node.expression, node.expression.expression, node.expression.expression.typeArguments,
    argumentsArray);
  // @ts-ignore
  node.expression.expression.parent = callNode;
  // @ts-ignore
  callNode.parent = node.expression.parent;
  node = ts.factory.updateExpressionStatement(node, callNode);
  return node;
}

type EtsComponentResult = {
  etsComponentNode: ts.EtsComponentExpression;
  hasAttr: boolean;
};
function parseEtsComponentExpression(node: ts.ExpressionStatement): EtsComponentResult {
  let etsComponentNode: ts.EtsComponentExpression;
  let hasAttr: boolean = false;
  let temp: any = node.expression;
  while (temp) {
    if (ts.isCallExpression(temp) && temp.expression &&
      ts.isPropertyAccessExpression(temp.expression)) {
      hasAttr = true;
    }
    if (ts.isEtsComponentExpression(temp)) {
      etsComponentNode = temp;
      break;
    }
    temp = temp.expression;
  }
  return { etsComponentNode: etsComponentNode, hasAttr: hasAttr };
}

export function createCollectElmtIdNode(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(MY_IDS),
      ts.factory.createIdentifier(PUSH)
    ),
    undefined,
    [ts.factory.createIdentifier(ELMTID)]
  ));
}

function processInnerComponent(node: ts.ExpressionStatement, innerCompStatements: ts.Statement[],
  log: LogInfo[], parent: string = undefined, isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  isTransition: boolean = false, idName: ts.Expression = undefined, savedParent: string = undefined,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): void {
  const newStatements: ts.Statement[] = [];
  const res: CreateResult = createComponent(node, COMPONENT_CREATE_FUNCTION);
  newStatements.push(res.newNode);
  const nameResult: NameResult = { name: null, arguments: [] };
  validateEtsComponentNode(node.expression as ts.EtsComponentExpression, nameResult);
  if (savedParent && nameResult.name) {
    checkNonspecificParents(node, nameResult.name, savedParent, log);
  }
  if (partialUpdateConfig.partialUpdateMode && ItemComponents.includes(nameResult.name)) {
    processItemComponent(node, nameResult, innerCompStatements, log, parent, isGlobalBuilder, idName, builderParamsResult, isInRepeatTemplate);
  } else if (partialUpdateConfig.partialUpdateMode && TabContentAndNavDestination.has(nameResult.name)) {
    processTabAndNav(node, innerCompStatements, nameResult, log, parent, isGlobalBuilder, idName, builderParamsResult, isInRepeatTemplate);
  } else {
    processNormalComponent(node, nameResult, innerCompStatements, log, parent, isBuilder, isGlobalBuilder,
      isTransition, idName, builderParamsResult, isInRepeatTemplate);
  }
}

function processNormalComponent(node: ts.ExpressionStatement, nameResult: NameResult,
  innerCompStatements: ts.Statement[], log: LogInfo[], parent: string = undefined, isBuilder: boolean = false,
  isGlobalBuilder: boolean = false, isTransition: boolean = false, idName: ts.Expression = undefined,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): void {
  const newStatements: ts.Statement[] = [];
  if (addElmtIdNode()) {
    newStatements.push(createCollectElmtIdNode());
  }
  const immutableStatements: ts.Statement[] = [];
  const res: CreateResult = createComponent(node, COMPONENT_CREATE_FUNCTION);
  newStatements.push(res.newNode);
  processDebug(node, nameResult, newStatements);
  const etsComponentResult: EtsComponentResult = parseEtsComponentExpression(node);
  const componentName: string = res.identifierNode.getText();
  let judgeIdStart: number;
  if (partialUpdateConfig.partialUpdateMode && idName) {
    judgeIdStart = innerCompStatements.length;
  }
  if (etsComponentResult.etsComponentNode.body && ts.isBlock(etsComponentResult.etsComponentNode.body)) {
    if (res.isButton) {
      checkButtonParamHasLabel(etsComponentResult.etsComponentNode, log);
      if (projectConfig.isPreview || projectConfig.enableDebugLine) {
        newStatements.splice(-2, 1, createComponent(node, COMPONENT_CREATE_CHILD_FUNCTION).newNode);
      } else {
        newStatements.splice(-1, 1, createComponent(node, COMPONENT_CREATE_CHILD_FUNCTION).newNode);
      }
    }
    if (etsComponentResult.hasAttr) {
      bindComponentAttr(node, res.identifierNode, newStatements, log, true, false, immutableStatements);
    }
    processInnerCompStatements(innerCompStatements, newStatements, node, isGlobalBuilder,
      isTransition, undefined, immutableStatements, componentName, builderParamsResult);
    storedFileInfo.lazyForEachInfo.isDependItem = false;
    processComponentChild(etsComponentResult.etsComponentNode.body, innerCompStatements, log,
      {isAcceleratePreview: false, line: 0, column: 0, fileName: ''}, isBuilder, parent, undefined,
      isGlobalBuilder, false, builderParamsResult, isInRepeatTemplate);
  } else {
    bindComponentAttr(node, res.identifierNode, newStatements, log, true, false, immutableStatements);
    processInnerCompStatements(innerCompStatements, newStatements, node, isGlobalBuilder,
      isTransition, undefined, immutableStatements, componentName, builderParamsResult);
  }
  if (res.isContainerComponent || res.needPop) {
    innerCompStatements.push(createComponent(node, COMPONENT_POP_FUNCTION).newNode);
  }
  if (partialUpdateConfig.partialUpdateMode && idName) {
    innerCompStatements.splice(judgeIdStart, innerCompStatements.length - judgeIdStart,
      ifRetakeId(innerCompStatements.slice(judgeIdStart), idName));
  }
}

export function ifRetakeId(blockContent: ts.Statement[], idName: ts.Expression): ts.IfStatement {
  return ts.factory.createIfStatement(
    ts.factory.createPrefixUnaryExpression(
      ts.SyntaxKind.ExclamationToken,
      ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createIdentifier(COMPONENT_IF),
          ts.factory.createIdentifier(CAN_RETAKE)
        ),
        undefined,
        [idName]
      )
    ),
    ts.factory.createBlock(
      blockContent,
      true
    ),
    undefined
  );
}

function processRepeatComponent(node: ts.ExpressionStatement, innerCompStatements: ts.Statement[],
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  isTransition: boolean = false, builderParamsResult: BuilderParamsResult = null,
  isInRepeatTemplate: boolean = false): void {
  const chainCallTransform: ts.CallExpression =
    recurseRepeatExpression(node.expression as ts.CallExpression, log, isBuilder, isGlobalBuilder,
      isTransition, isInRepeatTemplate) as ts.CallExpression;
  innerCompStatements.push(createComponentCreationStatement(node,
    [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        chainCallTransform,
        ts.factory.createIdentifier(COMPONENT_RENDER_FUNCTION)
      ),
      undefined,
      [ts.factory.createIdentifier(ISINITIALRENDER)]
    ))], COMPONENT_REPEAT, isGlobalBuilder, isTransition, undefined, null, builderParamsResult));
}

function recurseRepeatExpression(node: ts.CallExpression | ts.PropertyAccessExpression,
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false, isTransition: boolean = false, isInRepeatTemplate: boolean = false):
  ts.PropertyAccessExpression | ts.CallExpression {
  if (ts.isCallExpression(node) && node.expression && ts.isIdentifier(node.expression) &&
    node.expression.getText() === COMPONENT_REPEAT) {
    return ts.factory.createCallExpression(node.expression, node.typeArguments, [...node.arguments, ts.factory.createThis()]);
  } else if (ts.isPropertyAccessExpression(node)) {
    return ts.factory.updatePropertyAccessExpression(node,
      recurseRepeatExpression(node.expression, log, isBuilder, isGlobalBuilder, isTransition, isInRepeatTemplate), node.name);
  } else {
    let repeatPropArgs: ts.ArrowFunction[] = processRepeatAttributeArrowNode(node.arguments);
    storedFileInfo.processRepeat = true;
    repeatPropArgs = processRepeatPropWithChild(node, repeatPropArgs, log, isBuilder, isGlobalBuilder, isTransition, isInRepeatTemplate);
    storedFileInfo.processRepeat = false;
    return ts.factory.updateCallExpression(node,
      recurseRepeatExpression(node.expression as ts.PropertyAccessExpression, log, isBuilder,
        isGlobalBuilder, isTransition, isInRepeatTemplate) as ts.PropertyAccessExpression, undefined, repeatPropArgs);
  }
}

function processRepeatPropWithChild(node: ts.CallExpression, repeatPropArgs: ts.ArrowFunction[],
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false, isTransition: boolean = false,
  isInRepeatTemplate: boolean = false): ts.ArrowFunction[] {
  if (ts.isPropertyAccessExpression(node.expression) && ts.isIdentifier(node.expression.name) &&
    node.expression.name.getText() === REPEAT_EACH && repeatPropArgs.length > 0 && repeatPropArgs[0].body) {
    // transfer args for each property
    return [
      ts.factory.updateArrowFunction(repeatPropArgs[0], repeatPropArgs[0].modifiers, repeatPropArgs[0].typeParameters,
        repeatPropArgs[0].parameters, repeatPropArgs[0].type, repeatPropArgs[0].equalsGreaterThanToken,
        processComponentBlock(processRepeatCallBackBlock(repeatPropArgs[0]), false, log, isTransition,
          isBuilder, undefined, undefined, isGlobalBuilder, null, false, isInRepeatTemplate)),
      ...repeatPropArgs.slice(1)
    ];
  } else if (ts.isPropertyAccessExpression(node.expression) && ts.isIdentifier(node.expression.name) &&
    node.expression.name.getText() === REPEAT_TEMPLATE && repeatPropArgs.length > 1 && repeatPropArgs[1].body) {
    // transfer args for template property
    return [
      repeatPropArgs[0], ts.factory.updateArrowFunction(repeatPropArgs[1], repeatPropArgs[1].modifiers, repeatPropArgs[1].typeParameters,
        repeatPropArgs[1].parameters, repeatPropArgs[1].type, repeatPropArgs[1].equalsGreaterThanToken,
        processComponentBlock(processRepeatCallBackBlock(repeatPropArgs[1]), false, log, isTransition, isBuilder, undefined, undefined,
          isGlobalBuilder, null, false, true)),
      ...repeatPropArgs.slice(2)
    ];
  }
  return repeatPropArgs;
}

function processRepeatCallBackBlock(repeatPropArg: ts.ArrowFunction): ts.Block {
  if (ts.isBlock(repeatPropArg.body)) {
    return repeatPropArg.body;
  } else {
    return ts.factory.updateArrowFunction(repeatPropArg, ts.getModifiers(repeatPropArg), repeatPropArg.typeParameters,
      repeatPropArg.parameters, repeatPropArg.type, repeatPropArg.equalsGreaterThanToken,
      ts.factory.createBlock([ts.factory.createExpressionStatement(repeatPropArg.body)], true)).body as ts.Block;
  }
}

function processRepeatAttributeArrowNode(argumentsNode: ts.ArrowFunction[]): ts.ArrowFunction[] {
  for (let i = 0; i < argumentsNode.length; i++) {
    while (ts.isParenthesizedExpression(argumentsNode[i])) {
      if (ts.isArrowFunction(argumentsNode[i].expression)) {
        argumentsNode.splice(i, 1, argumentsNode[i].expression);
        break;
      } else {
        if (argumentsNode[i].expression) {
          argumentsNode[i] = argumentsNode[i].expression;
        } else {
          break;
        }
      }
    }
  }
  return argumentsNode;
}

function processDebug(node: ts.Statement, nameResult: NameResult, newStatements: ts.Statement[],
  getNode: boolean = false): ts.ExpressionStatement {
  if ((projectConfig.isPreview || projectConfig.enableDebugLine) && nameResult.name &&
    !NO_DEBUG_LINE_COMPONENT.has(nameResult.name)) {
    let posOfNode: ts.LineAndCharacter;
    let curFileName: string;
    let line: number = 1;
    let col: number = 1;
    if (sourceNode && newsupplement.isAcceleratePreview) {
      posOfNode = sourceNode.getLineAndCharacterOfPosition(getRealNodePos(node) - 22);
      curFileName = newsupplement.fileName;
      if (posOfNode.line === 0) {
        col = newsupplement.column - 1;
      }
      line = newsupplement.line;
    } else {
      posOfNode = transformLog.sourceFile.getLineAndCharacterOfPosition(getRealNodePos(node));
      curFileName = transformLog.sourceFile.fileName.replace(/\.ts$/, '');
    }
    let debugInfo: string;
    if (projectConfig.isPreview) {
      if (projectConfig.minAPIVersion >= 11) {
        debugInfo = `${path.relative(projectConfig.projectRootPath, curFileName).replace(/\\+/g, '/')}` +
          `(${posOfNode.line + line}:${posOfNode.character + col})`;
      } else {
        debugInfo = `${path.relative(projectConfig.projectPath, curFileName).replace(/\\+/g, '/')}` +
          `(${posOfNode.line + line}:${posOfNode.character + col})`;
      }
    } else if (projectConfig.enableDebugLine) {
      debugInfo = `${path.relative(projectConfig.projectRootPath, curFileName)}` +
        `(${posOfNode.line + line}:${posOfNode.character + col})`;
    }
    const debugNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(
      createFunction(ts.factory.createIdentifier(nameResult.name),
        ts.factory.createIdentifier(COMPONENT_DEBUGLINE_FUNCTION),
        createDebugLineArgs(debugInfo)));
    if (getNode) {
      return debugNode;
    }
    newStatements.push(debugNode);
  }
  return undefined;
}

function createDebugLineArgs(debugInfo: string): ts.NodeArray {
  const argsArr: ts.Node[] = [ts.factory.createStringLiteral(debugInfo)];
  const pkgName: string = storedFileInfo.getCurrentArkTsFile().pkgName;
  if (pkgName) {
    argsArr.push(ts.factory.createStringLiteral(pkgName));
  }
  return ts.factory.createNodeArray(argsArr);
}

function processInnerCompStatements(innerCompStatements: ts.Statement[],
  newStatements: ts.Statement[], node: ts.Statement, isGlobalBuilder: boolean, isTransition: boolean,
  nameResult: NameResult, immutableStatements: ts.Statement[], componentName: string,
  builderParamsResult: BuilderParamsResult): void {
  if (!partialUpdateConfig.partialUpdateMode) {
    innerCompStatements.push(...newStatements);
  } else {
    innerCompStatements.push(createComponentCreationStatement(node, newStatements, componentName,
      isGlobalBuilder, isTransition, nameResult, immutableStatements, builderParamsResult));
  }
}

function createComponentCreationArrowParams(isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult, isRecycleComponent: boolean = false): ts.ParameterDeclaration[] {
  const arrowNodes: ts.ParameterDeclaration[] = [
    ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(ELMTID), undefined, undefined, undefined),
    ts.factory.createParameterDeclaration(undefined, undefined,
      ts.factory.createIdentifier(ISINITIALRENDER), undefined, undefined, undefined)
  ];
  if (!isRecycleComponent && partialUpdateConfig.optimizeComponent && isGlobalBuilder &&
    builderParamsResult && builderParamsResult.firstParam) {
    const paramName: ts.Identifier = builderParamsResult.firstParam.name as ts.Identifier;
    arrowNodes.push(ts.factory.createParameterDeclaration(undefined, undefined,
      paramName, undefined, undefined, ts.factory.createIdentifier(`__${paramName.escapedText.toString()}__`)
    ));
  }
  return arrowNodes;
}

export function createComponentCreationStatement(node: ts.Statement, innerStatements: ts.Statement[],
  componentName: string, isGlobalBuilder: boolean = false, isTransition: boolean = false,
  nameResult: NameResult = undefined, immutableStatements: ts.Statement[] = null,
  builderParamsResult: BuilderParamsResult = null, isRecycleComponent: boolean = false): ts.Statement {
  const blockArr: ts.Statement[] = [...innerStatements];
  if (nameResult) {
    blockArr.push(processDebug(node, nameResult, innerStatements, true));
  }
  if (!isTransition) {
    createInitRenderStatement(node, immutableStatements, blockArr);
  }
  if (!partialUpdateConfig.optimizeComponent) {
    blockArr.unshift(createViewStackProcessorStatement(STARTGETACCESSRECORDINGFOR, ELMTID));
    blockArr.push(createViewStackProcessorStatement(STOPGETACCESSRECORDING));
  }
  const creationArgs: ts.Expression[] = [
    ts.factory.createArrowFunction(undefined, undefined,
      createComponentCreationArrowParams(isGlobalBuilder, builderParamsResult, isRecycleComponent),
      undefined, ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      ts.factory.createBlock(blockArr, true)
    )
  ];
  if (partialUpdateConfig.optimizeComponent) {
    creationArgs.push(isTransition ? ts.factory.createNull() :
      ts.factory.createIdentifier(componentName));
  }
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(createConditionParent(isGlobalBuilder),
        ts.factory.createIdentifier(partialUpdateConfig.optimizeComponent ?
          OBSERVECOMPONENTCREATION2 : OBSERVECOMPONENTCREATION)
      ), undefined, creationArgs)
  );
}

export function createViewStackProcessorStatement(propertyAccessName: string, elmtId?: string): ts.Statement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(VIEWSTACKPROCESSOR),
        ts.factory.createIdentifier(propertyAccessName)
      ),
      undefined,
      elmtId ? [ts.factory.createIdentifier(ELMTID)] : []
    )
  );
}

function createInitRenderStatement(node: ts.Statement,
  immutableStatements: ts.Statement[], blockArr: ts.Statement[]): void {
  if (partialUpdateConfig.optimizeComponent) {
    if (immutableStatements && immutableStatements.length) {
      blockArr.push(ts.factory.createIfStatement(
        ts.factory.createIdentifier(ISINITIALRENDER),
        ts.factory.createBlock(immutableStatements, true)
      ));
    }
  } else {
    blockArr.push(ts.factory.createIfStatement(
      ts.factory.createPrefixUnaryExpression(
        ts.SyntaxKind.ExclamationToken,
        ts.factory.createIdentifier(ISINITIALRENDER)
      ),
      ts.factory.createBlock(
        [
          ts.isExpressionStatement(node) ?
            createComponent(node, COMPONENT_POP_FUNCTION).newNode : createIfPop()
        ],
        true
      ),
      immutableStatements && immutableStatements.length ?
        ts.factory.createBlock(immutableStatements, true) : undefined
    ));
  }
}

function processItemComponent(node: ts.ExpressionStatement, nameResult: NameResult, innerCompStatements: ts.Statement[],
  log: LogInfo[], parent: string = undefined, isGlobalBuilder: boolean = false, idName: ts.Expression = undefined,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): void {
  const itemRenderInnerStatements: ts.Statement[] = [];
  const immutableStatements: ts.Statement[] = [];
  const deepItemRenderInnerStatements: ts.Statement[] = [];
  if (addElmtIdNode()) {
    itemRenderInnerStatements.push(createCollectElmtIdNode());
  }
  const res: CreateResult = createComponent(node, COMPONENT_CREATE_FUNCTION);
  const isLazyCreate: boolean = checkLazyCreate(node, nameResult);
  const itemCreateStatement: ts.Statement = createItemCreate(nameResult, isLazyCreate);
  itemRenderInnerStatements.push(itemCreateStatement);
  const etsComponentResult: EtsComponentResult = parseEtsComponentExpression(node);
  if (etsComponentResult.etsComponentNode.body && ts.isBlock(etsComponentResult.etsComponentNode.body)) {
    if (etsComponentResult.hasAttr) {
      bindComponentAttr(node, res.identifierNode, itemRenderInnerStatements, log, true, false, immutableStatements);
    }
    storedFileInfo.lazyForEachInfo.isDependItem = false;
    processComponentChild(etsComponentResult.etsComponentNode.body, deepItemRenderInnerStatements, log,
      {isAcceleratePreview: false, line: 0, column: 0, fileName: ''}, false, parent, undefined, isGlobalBuilder, false,
      builderParamsResult, isInRepeatTemplate);
  } else {
    bindComponentAttr(node, res.identifierNode, itemRenderInnerStatements, log, true, false, immutableStatements);
  }
  let generateItem: ts.IfStatement | ts.Block;
  if (idName) {
    generateItem = ifRetakeId([createItemBlock(
      node, itemRenderInnerStatements, deepItemRenderInnerStatements, nameResult, isLazyCreate,
      immutableStatements, isGlobalBuilder, builderParamsResult, itemCreateStatement)], idName);
  } else {
    generateItem = createItemBlock(
      node, itemRenderInnerStatements, deepItemRenderInnerStatements, nameResult, isLazyCreate,
      immutableStatements, isGlobalBuilder, builderParamsResult, itemCreateStatement);
  }
  innerCompStatements.push(generateItem);
}

function createItemCreate(nameResult: NameResult, isLazyCreate: boolean): ts.Statement {
  const itemCreateArgs: ts.Expression[] = [];
  if (isLazyCreate) {
    itemCreateArgs.push(ts.factory.createIdentifier(DEEPRENDERFUNCTION), ts.factory.createTrue());
  } else {
    itemCreateArgs.push(
      ts.factory.createArrowFunction(undefined, undefined, [], undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock([], false)),
      ts.factory.createFalse()
    );
  }
  itemCreateArgs.push(...nameResult.arguments);
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(nameResult.name),
      ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)), undefined, itemCreateArgs));
}

type ItemCreation = {
  creationArgs: ts.Expression[],
  creationName: string,
};

function getItemCreation(nameResult: NameResult): ItemCreation {
  const creationArgs: ts.Expression[] = [];
  let creationName: string = OBSERVECOMPONENTCREATION;
  if (partialUpdateConfig.optimizeComponent) {
    creationArgs.push(
      ts.factory.createIdentifier(ITEMCREATION2),
      ts.factory.createIdentifier(nameResult.name));
    creationName = OBSERVECOMPONENTCREATION2;
  } else {
    creationArgs.push(ts.factory.createIdentifier(ITEMCREATION));
  }
  return { creationArgs, creationName };
}

function createItemBlock(
  node: ts.ExpressionStatement,
  itemRenderInnerStatements: ts.Statement[],
  deepItemRenderInnerStatements: ts.Statement[],
  nameResult: NameResult, isLazyCreate: boolean,
  immutableStatements: ts.Statement[],
  isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult,
  itemCreateStatement: ts.Statement
): ts.Block {
  const blockNode: ts.Statement[] = [
    createItemCreation2(node, itemRenderInnerStatements, nameResult, immutableStatements,
      isGlobalBuilder, builderParamsResult)
  ];
  const itemCreation: ItemCreation = getItemCreation(nameResult);
  if (isLazyCreate) {
    blockNode.unshift(createItemCreation(node, isGlobalBuilder, builderParamsResult, itemCreateStatement));
    blockNode.push(
      createDeepRenderFunction(node, deepItemRenderInnerStatements, isGlobalBuilder, builderParamsResult),
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(itemCreation.creationName)
        ), undefined, itemCreation.creationArgs
      )),
      createComponent(node, COMPONENT_POP_FUNCTION).newNode
    );
  } else {
    if (!partialUpdateConfig.optimizeComponent) {
      blockNode.unshift(createItemCreation(node, isGlobalBuilder, builderParamsResult));
    }
    blockNode.push(
      createObservedDeepRender(node, deepItemRenderInnerStatements, itemCreation),
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createIdentifier(OBSERVEDDEEPRENDER), undefined, []))
    );
  }
  return ts.factory.createBlock(blockNode, true);
}

function checkLazyCreate(node: ts.ExpressionStatement, nameResult: NameResult): boolean {
  if (nameResult.name === LIST_ITEM) {
    if (nameResult.arguments.length && ts.isStringLiteral(nameResult.arguments[0]) &&
      nameResult.arguments[0].text === 'false') {
      return false;
    }
    if (storedFileInfo.processRepeat) {
      return false;
    }
    if (isLazyForEachChild(node)) {
      return false;
    }
    return true;
  }
  return false;
}

function createItemCreation(node: ts.ExpressionStatement, isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult, itemCreateStatement: ts.Statement = undefined): ts.VariableStatement {
  if (!partialUpdateConfig.optimizeComponent) {
    itemCreateStatement = ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createIdentifier(ITEMCREATION2),
      undefined, createItemCreationArgs(isGlobalBuilder, builderParamsResult)));
  }
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(ITEMCREATION), undefined, undefined,
        ts.factory.createArrowFunction(undefined, undefined,
          createComponentCreationArrowParams(isGlobalBuilder, builderParamsResult), undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock(
            [
              createViewStackProcessorStatement(STARTGETACCESSRECORDINGFOR, ELMTID),
              itemCreateStatement,
              ts.factory.createIfStatement(
                ts.factory.createPrefixUnaryExpression(
                  ts.SyntaxKind.ExclamationToken,
                  ts.factory.createIdentifier(ISINITIALRENDER)
                ),
                ts.factory.createBlock(
                  [createComponent(node, COMPONENT_POP_FUNCTION).newNode],
                  true
                )
              ),
              createViewStackProcessorStatement(STOPGETACCESSRECORDING)
            ],
            true
          )
        )
      )],
      ts.NodeFlags.Const
    )
  );
}

function createItemCreation2(
  node: ts.ExpressionStatement,
  itemRenderInnerStatements: ts.Statement[],
  nameResult: NameResult,
  immutableStatements: ts.Statement[],
  isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult
): ts.VariableStatement {
  const itemBlock: ts.Statement[] = [
    ...itemRenderInnerStatements,
    processDebug(node, nameResult, itemRenderInnerStatements, true)
  ];
  if (immutableStatements && immutableStatements.length) {
    itemBlock.push(ts.factory.createIfStatement(
      ts.factory.createIdentifier(ISINITIALRENDER),
      ts.factory.createBlock(immutableStatements, true)
    ));
  }
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(ITEMCREATION2), undefined, undefined,
        ts.factory.createArrowFunction(undefined, undefined,
          createComponentCreationArrowParams(isGlobalBuilder, builderParamsResult), undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock(itemBlock, true)
        )
      )],
      ts.NodeFlags.Const
    )
  );
}

function createItemCreationArgs(isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult): ts.Expression[] {
  const itemCreationArgs: ts.Expression[] = [
    ts.factory.createIdentifier(ELMTID), ts.factory.createIdentifier(ISINITIALRENDER)];
  if (partialUpdateConfig.optimizeComponent && isGlobalBuilder && builderParamsResult &&
    builderParamsResult.firstParam) {
    itemCreationArgs.push(builderParamsResult.firstParam.name as ts.Identifier);
  }
  return itemCreationArgs;
}

function createDeepRenderFunction(
  node: ts.ExpressionStatement,
  deepItemRenderInnerStatements: ts.Statement[],
  isGlobalBuilder: boolean,
  builderParamsResult: BuilderParamsResult
): ts.VariableStatement {
  const blockNode: ts.Statement[] = [
    ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createIdentifier(ITEMCREATION), undefined,
      createItemCreationArgs(isGlobalBuilder, builderParamsResult)
    )),
    ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(UPDATE_FUNC_BY_ELMT_ID)
        ),
        ts.factory.createIdentifier(CREATE_SET_METHOD)
      ), undefined,
      [ts.factory.createIdentifier(ELMTID), ts.factory.createIdentifier(ITEMCREATION)]
    )),
    ...deepItemRenderInnerStatements,
    createComponent(node, COMPONENT_POP_FUNCTION).newNode
  ];
  if (partialUpdateConfig.optimizeComponent) {
    blockNode.splice(1, 1);
  }
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(DEEPRENDERFUNCTION), undefined, undefined,
        ts.factory.createArrowFunction(undefined, undefined,
          createComponentCreationArrowParams(isGlobalBuilder, builderParamsResult), undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock(blockNode, true)
        )
      )],
      ts.NodeFlags.Const
    )
  );
}

function createObservedDeepRender(
  node: ts.ExpressionStatement,
  deepItemRenderInnerStatements: ts.Statement[],
  itemCreation: ItemCreation
): ts.VariableStatement {
  return ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(OBSERVEDDEEPRENDER),
        undefined,
        undefined,
        ts.factory.createArrowFunction(
          undefined,
          undefined,
          [],
          undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock(
            [
              ts.factory.createExpressionStatement(ts.factory.createCallExpression(
                ts.factory.createPropertyAccessExpression(
                  ts.factory.createThis(),
                  ts.factory.createIdentifier(itemCreation.creationName)
                ), undefined, itemCreation.creationArgs
              )),
              ...deepItemRenderInnerStatements,
              createComponent(node, COMPONENT_POP_FUNCTION).newNode
            ],
            true
          )
        )
      )],
      ts.NodeFlags.Const
    )
  );
}

function processTabAndNav(node: ts.ExpressionStatement, innerCompStatements: ts.Statement[],
  nameResult: NameResult, log: LogInfo[], parent: string = undefined, isGlobalBuilder: boolean = false,
  idName: ts.Expression = undefined, builderParamsResult: BuilderParamsResult = null,
  isInRepeatTemplate: boolean = false): void {
  const name: string = nameResult.name;
  const tabContentComp: ts.EtsComponentExpression = getEtsComponentExpression(node);
  const tabContentBody: ts.Block = tabContentComp.body;
  let tabContentCreation: ts.Statement;
  const tabContentPop: ts.Statement = ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(name),
      ts.factory.createIdentifier(COMPONENT_POP_FUNCTION)), undefined, []));
  const tabAttrs: ts.Statement[] = [];
  if (addElmtIdNode()) {
    tabAttrs.push(createCollectElmtIdNode());
  }
  const immutableStatements: ts.Statement[] = [];
  let judgeIdStart: number;
  if (idName) {
    judgeIdStart = innerCompStatements.length;
  }
  if (tabContentBody && tabContentBody.statements.length) {
    const newTabContentChildren: ts.Statement[] = [];
    processComponentChild(tabContentBody, newTabContentChildren, log, {isAcceleratePreview: false, line: 0, column: 0, fileName: ''},
      false, parent, undefined, isGlobalBuilder, false, builderParamsResult, isInRepeatTemplate);
    const navDestinationCallback: (ts.ArrowFunction | ts.NewExpression | ts.ObjectLiteralExpression)[] =
      [ts.factory.createArrowFunction(undefined, undefined, [], undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock([...newTabContentChildren], true))];
    if (name === NAV_DESTINATION) {
      navDestinationCallback.push(...navigationCreateParam(NAV_DESTINATION, COMPONENT_CREATE_FUNCTION, undefined, true));
    }
    if (equalToHiddenNavDes(name)) {
      navDestinationCallback.push(...navigationCreateParam(EXT_WHITE_LIST[1], COMPONENT_CREATE_FUNCTION, undefined, true));
    }
    tabContentCreation = ts.factory.createExpressionStatement(
      ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(name), ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)),
        undefined, navDestinationCallback));
    bindComponentAttr(node, ts.factory.createIdentifier(name), tabAttrs, log, true, false, immutableStatements);
    processInnerCompStatements(
      innerCompStatements, [tabContentCreation, ...tabAttrs], node, isGlobalBuilder, false,
      nameResult, immutableStatements, name, builderParamsResult);
    storedFileInfo.lazyForEachInfo.isDependItem = false;
  } else {
    tabContentCreation = ts.factory.createExpressionStatement(ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(name),
        ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)), undefined,
      name === NAV_DESTINATION ? navigationCreateParam(NAV_DESTINATION, COMPONENT_CREATE_FUNCTION) :
        (equalToHiddenNavDes(name) ? navigationCreateParam(EXT_WHITE_LIST[1], COMPONENT_CREATE_FUNCTION) : [])));
    bindComponentAttr(node, ts.factory.createIdentifier(name), tabAttrs, log, true, false, immutableStatements);
    processInnerCompStatements(
      innerCompStatements, [tabContentCreation, ...tabAttrs], node, isGlobalBuilder, false,
      nameResult, immutableStatements, name, builderParamsResult);
  }
  innerCompStatements.push(tabContentPop);
  if (idName) {
    innerCompStatements.splice(judgeIdStart, innerCompStatements.length - judgeIdStart,
      ifRetakeId(innerCompStatements.slice(judgeIdStart), idName));
  }
}

export function getRealNodePos(node: ts.Node): number {
  // @ts-ignore
  if (node.pos === -1 && node.expression) {
    // @ts-ignore
    return getRealNodePos(node.expression);
  } else {
    return node.getStart();
  }
}

function processForEachComponent(node: ts.ExpressionStatement, newStatements: ts.Statement[],
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false): void {
  const popNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(createFunction(
    // @ts-ignore
    node.expression.expression as ts.Identifier,
    ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null));
  if (ts.isCallExpression(node.expression)) {
    const propertyNode: ts.PropertyAccessExpression = ts.factory.createPropertyAccessExpression(
      node.expression.expression as ts.Identifier,
      ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)
    );
    const argumentsArray: ts.Expression[] = Array.from(node.expression.arguments);
    let arrayObserveredObject: ts.CallExpression;
    if (argumentsArray.length) {
      arrayObserveredObject = ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(ts.factory.createIdentifier(FOREACH_OBSERVED_OBJECT),
          ts.factory.createIdentifier(FOREACH_GET_RAW_OBJECT)), undefined, [argumentsArray[0]]);
    }
    argumentsArray.splice(0, 1, arrayObserveredObject);
    const newForEachArrowFunc: ts.ArrowFunction = processForEachFunctionBlock(node.expression);
    const newArrowNode: ts.ArrowFunction =
      processForEachBlock(node.expression, log, newForEachArrowFunc, isBuilder) as ts.ArrowFunction;
    if (newArrowNode) {
      argumentsArray.splice(1, 1, newArrowNode);
    }
    node = addForEachId(ts.factory.updateExpressionStatement(node, ts.factory.updateCallExpression(
      node.expression, propertyNode, node.expression.typeArguments, argumentsArray)), isGlobalBuilder);
  }
  newStatements.push(node, popNode);
}

function collectForEachAttribute(node: ts.ExpressionStatement,
  attributeList: ts.ExpressionStatement[], name: string): ts.ExpressionStatement {
  let tempNode = node.expression;
  while (tempNode && ts.isCallExpression(tempNode)) {
    if (tempNode.expression && ts.isPropertyAccessExpression(tempNode.expression)) {
      attributeList.unshift(generateForEachAttribute(tempNode, name));
    } else if (tempNode.expression && ts.isIdentifier(tempNode.expression)) {
      return ts.factory.updateExpressionStatement(node, tempNode);
    }
    tempNode = tempNode.expression?.expression;
  }
  return node;
}

function generateForEachAttribute(tempNode: ts.CallExpression, name: string): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(name),
      tempNode.expression.name
    ),
    undefined,
    tempNode.arguments
  ));
}

function processForEachComponentNew(node: ts.ExpressionStatement, newStatements: ts.Statement[],
  log: LogInfo[], name: string, isGlobalBuilder: boolean = false, builderParamsResult: BuilderParamsResult = null,
  isInRepeatTemplate: boolean = false): void {
  const attributeList: ts.ExpressionStatement[] = [];
  const newNode = collectForEachAttribute(node, attributeList, name);
  const newForEachStatements: ts.Statement[] = [];
  const popNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(createFunction(
    (newNode.expression as ts.CallExpression).expression as ts.Identifier,
    ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null));
  if (ts.isCallExpression(newNode.expression)) {
    if (checkForEachComponent(newNode)) {
      storedFileInfo.processForEach += 1;
    } else {
      storedFileInfo.processLazyForEach += 1;
    }
    const argumentsArray: ts.Expression[] = Array.from(newNode.expression.arguments);
    const propertyNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(
      ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
        newNode.expression.expression as ts.Identifier,
        ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)), undefined, []));
    const newForEachArrowFunc: ts.ArrowFunction = processForEachFunctionBlock(newNode.expression);
    const newArrowNode: ts.NodeArray<ts.Statement> =
      processForEachBlock(newNode.expression, log, newForEachArrowFunc, false, isGlobalBuilder,
        builderParamsResult, isInRepeatTemplate) as ts.NodeArray<ts.Statement>;
    const itemGenFunctionStatement: ts.VariableStatement = createItemGenFunctionStatement(newNode.expression, newArrowNode, newForEachArrowFunc);
    const itemIdFuncStatement: ts.VariableStatement = createItemIdFuncStatement(newNode.expression, argumentsArray);
    const updateFunctionStatement: ts.ExpressionStatement = createUpdateFunctionStatement(argumentsArray, newForEachArrowFunc, isGlobalBuilder);
    const lazyForEachStatement: ts.ExpressionStatement = createLazyForEachStatement(argumentsArray);
    if (newNode.expression.expression.getText() === COMPONENT_FOREACH) {
      newForEachStatements.push(propertyNode, ...attributeList, itemGenFunctionStatement, updateFunctionStatement);
      newStatements.push(createComponentCreationStatement(newNode, newForEachStatements, COMPONENT_FOREACH,
        isGlobalBuilder, false, undefined, null, builderParamsResult), popNode);
    } else {
      if (argumentsArray[2]) {
        newStatements.push(ts.factory.createBlock([itemGenFunctionStatement, itemIdFuncStatement, lazyForEachStatement,
          ...attributeList, popNode], true));
      } else {
        newStatements.push(ts.factory.createBlock([itemGenFunctionStatement, lazyForEachStatement, popNode, ...attributeList], true));
      }
    }
    if (checkForEachComponent(newNode)) {
      storedFileInfo.processForEach -= 1;
    } else {
      storedFileInfo.processLazyForEach -= 1;
    }
  }
}

function checkForEachComponent(node: ts.ExpressionStatement): boolean {
  return node.expression.expression && ts.isIdentifier(node.expression.expression) &&
    node.expression.expression.getText() === COMPONENT_FOREACH;
}

function createItemGenFunctionStatement(
  node: ts.CallExpression,
  newArrowNode: ts.NodeArray<ts.Statement>,
  newForEachArrowFunc: ts.ArrowFunction
): ts.VariableStatement {
  if (newForEachArrowFunc && ts.isArrowFunction(newForEachArrowFunc)) {
    return ts.factory.createVariableStatement(
      undefined,
      ts.factory.createVariableDeclarationList(
        [ts.factory.createVariableDeclaration(
          ts.factory.createIdentifier(node.expression.getText() === COMPONENT_FOREACH ?
            FOREACHITEMGENFUNCTION : __LAZYFOREACHITEMGENFUNCTION),
          undefined, undefined,
          ts.factory.createArrowFunction(
            undefined, undefined,
            newForEachArrowFunc.parameters && newForEachArrowFunc.parameters.length >= 1 ?
              getParameters(newForEachArrowFunc) : [],
            undefined, ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
            ts.factory.createBlock(
              newForEachArrowFunc.parameters && newForEachArrowFunc.parameters.length >= 1 ?
                isForEachItemGeneratorParam(newForEachArrowFunc, newArrowNode) :
                newArrowNode ? [...newArrowNode] : undefined,
              true
            )
          )
        )
        ],
        ts.NodeFlags.Const
      )
    );
  }
  return undefined;
}

function isForEachItemGeneratorParam(argumentsArray: ts.Expression, newArrowNode: ts.NodeArray<ts.Statement>): ts.Statement[] {
  const createVariableStatementNode: ts.Statement[] = [];
  createVariableStatementNode.push(ts.factory.createVariableStatement(
    undefined,
    ts.factory.createVariableDeclarationList(
      [ts.factory.createVariableDeclaration(
        ts.factory.createIdentifier(
          argumentsArray.parameters[0] && argumentsArray.parameters[0].name.getText()),
        undefined,
        undefined,
        ts.factory.createIdentifier(_ITEM)
      )],
      ts.NodeFlags.Const
    )
  ));
  if (newArrowNode) {
    createVariableStatementNode.push(...newArrowNode);
  }
  return createVariableStatementNode;
}

function getParameters(node: ts.ArrowFunction): ts.ParameterDeclaration[] {
  const parameterArr: ts.ParameterDeclaration[] = [
    ts.factory.createParameterDeclaration(
      undefined, undefined, ts.factory.createIdentifier(_ITEM))
  ];
  if (node.parameters && node.parameters.length > 1) {
    parameterArr.push(node.parameters[1]);
  }
  if (projectConfig.optLazyForEach && storedFileInfo.processLazyForEach) {
    if (node.parameters.length === 1) {
      parameterArr.push(ts.factory.createParameterDeclaration(
        undefined, undefined, ts.factory.createIdentifier(INDEX)));
    }
    parameterArr.push(
      ts.factory.createParameterDeclaration(
        undefined, undefined, ts.factory.createIdentifier(IS_INITIAL_ITEM)),
      ts.factory.createParameterDeclaration(
        undefined, undefined, ts.factory.createIdentifier(IDS))
    );
  }
  return parameterArr;
}

function createItemIdFuncStatement(
  node: ts.CallExpression,
  argumentsArray: ts.Expression[]
): ts.VariableStatement {
  if (argumentsArray[2]) {
    return ts.factory.createVariableStatement(
      undefined,
      ts.factory.createVariableDeclarationList(
        [ts.factory.createVariableDeclaration(
          ts.factory.createIdentifier(node.expression.getText() === COMPONENT_FOREACH ?
            FOREACHITEMIDFUNC : __LAZYFOREACHITEMIDFUNC), undefined, undefined,
          argumentsArray[2]
        )],
        ts.NodeFlags.Const
      )
    );
  }
  return undefined;
}

function createUpdateFunctionStatement(argumentsArray: ts.Expression[],
  newForEachArrowFunc: ts.ArrowFunction, isGlobalBuilder: boolean = false): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        isGlobalBuilder ? parentConditionalExpression() : ts.factory.createThis(),
        ts.factory.createIdentifier(FOREACHUPDATEFUNCTION)
      ),
      undefined,
      addForEachIdFuncParameter(argumentsArray, newForEachArrowFunc)
    )
  );
}

function addForEachIdFuncParameter(argumentsArray: ts.Expression[], newForEachArrowFunc: ts.ArrowFunction): ts.Expression[] {
  const addForEachIdFuncParameterArr: ts.Expression[] = [];
  addForEachIdFuncParameterArr.push(
    ts.factory.createIdentifier(ELMTID),
    argumentsArray[0],
    ts.factory.createIdentifier(FOREACHITEMGENFUNCTION)
  );
  // @ts-ignore
  if (newForEachArrowFunc && newForEachArrowFunc.parameters && newForEachArrowFunc.parameters[1]) {
    if (!argumentsArray[2]) {
      addForEachIdFuncParameterArr.push(...addForEachParameter(ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED), TRUE, FALSE));
    } else {
      // @ts-ignore
      argumentsArray[2].parameters && argumentsArray[2].parameters[1] ?
        addForEachIdFuncParameterArr.push(...addForEachParameter(argumentsArray[2], TRUE, TRUE)) :
        addForEachIdFuncParameterArr.push(...addForEachParameter(argumentsArray[2], TRUE, FALSE));
    }
  }
  // @ts-ignore
  if (newForEachArrowFunc && newForEachArrowFunc.parameters && newForEachArrowFunc.parameters.length < 2 && newForEachArrowFunc.parameters[0] &&
    argumentsArray && argumentsArray.length > 2 && argumentsArray[2]) {
    // @ts-ignore
    argumentsArray[2].parameters && argumentsArray[2].parameters[1] ?
      addForEachIdFuncParameterArr.push(...addForEachParameter(argumentsArray[2], FALSE, TRUE)) :
      addForEachIdFuncParameterArr.push(...addForEachParameter(argumentsArray[2], FALSE, FALSE));
  }
  return addForEachIdFuncParameterArr;
}

function addForEachParameter(forEachItemIdContent: ts.Expression, forEachItemGen: string, forEachItemId: string): ts.Expression[] {
  return [forEachItemIdContent, ts.factory.createIdentifier(forEachItemGen),
    ts.factory.createIdentifier(forEachItemId)];
}

function createLazyForEachStatement(argumentsArray: ts.Expression[]): ts.ExpressionStatement {
  const parameterList: ts.Expression[] = [
    ts.factory.createStringLiteral(componentInfo.id.toString()),
    ts.factory.createThis(),
    argumentsArray[0],
    ts.factory.createIdentifier(__LAZYFOREACHITEMGENFUNCTION)
  ];
  if (argumentsArray.length >= 3 && argumentsArray[2]) {
    parameterList.push(ts.factory.createIdentifier(__LAZYFOREACHITEMIDFUNC));
  }
  if (projectConfig.optLazyForEach) {
    parameterList.push(ts.factory.createTrue());
  }
  return ts.factory.createExpressionStatement(
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createIdentifier(COMPONENT_LAZYFOREACH),
        ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION)
      ),
      undefined,
      parameterList
    )
  );
}

function addForEachId(node: ts.ExpressionStatement, isGlobalBuilder: boolean = false): ts.ExpressionStatement {
  const forEachComponent: ts.CallExpression = node.expression as ts.CallExpression;
  return ts.factory.updateExpressionStatement(node, ts.factory.updateCallExpression(
    forEachComponent, forEachComponent.expression, forEachComponent.typeArguments,
    [ts.factory.createStringLiteral((++componentInfo.id).toString()),
      isGlobalBuilder ? parentConditionalExpression() : ts.factory.createThis(),
      ...forEachComponent.arguments]));
}

export function parentConditionalExpression(): ts.ConditionalExpression {
  return ts.factory.createConditionalExpression(
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARENT),
    ts.factory.createToken(ts.SyntaxKind.QuestionToken),
    ts.factory.createIdentifier(COMPONENT_CONSTRUCTOR_PARENT),
    ts.factory.createToken(ts.SyntaxKind.ColonToken),
    ts.factory.createThis());
}
function processForEachFunctionBlock(node: ts.CallExpression): ts.ArrowFunction {
  if (ts.isArrowFunction(node.arguments[1])) {
    return node.arguments[1];
  } else if (ts.isParenthesizedExpression(node.arguments[1]) && ts.isArrowFunction(node.arguments[1].expression)) {
    return node.arguments[1].expression;
  } else {
    return null;
  }
}
function processForEachBlock(node: ts.CallExpression, log: LogInfo[],
  arrowNode: ts.ArrowFunction, isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): ts.NodeArray<ts.Statement> | ts.ArrowFunction {
  if (node.arguments.length > 1 && ts.isArrowFunction(arrowNode)) {
    const isLazy: boolean = node.expression.getText() === COMPONENT_LAZYFOREACH;
    const body: ts.ConciseBody = arrowNode.body;
    if (!ts.isBlock(body)) {
      const statement: ts.Statement = ts.factory.createExpressionStatement(body);
      const blockNode: ts.Block = ts.factory.createBlock([statement], true);
      // @ts-ignore
      statement.parent = blockNode;
      if (!partialUpdateConfig.partialUpdateMode) {
        return ts.factory.updateArrowFunction(
          arrowNode, ts.getModifiers(arrowNode), arrowNode.typeParameters, arrowNode.parameters,
          arrowNode.type, arrowNode.equalsGreaterThanToken,
          processComponentBlock(blockNode, isLazy, log, false, false, undefined,
            arrowNode.parameters, isGlobalBuilder));
      } else {
        return processComponentBlock(blockNode, isLazy, log, false, false, undefined,
          arrowNode.parameters, isGlobalBuilder, builderParamsResult, false, isInRepeatTemplate).statements;
      }
    } else {
      if (!partialUpdateConfig.partialUpdateMode) {
        return ts.factory.updateArrowFunction(
          arrowNode, ts.getModifiers(arrowNode), arrowNode.typeParameters, arrowNode.parameters,
          arrowNode.type, arrowNode.equalsGreaterThanToken,
          processComponentBlock(body, isLazy, log, false, isBuilder, undefined, arrowNode.parameters));
      } else {
        return processComponentBlock(body, isLazy, log, false, false, undefined, arrowNode.parameters,
          isGlobalBuilder, builderParamsResult, false, isInRepeatTemplate).statements;
      }
    }
  }
  return null;
}

function createRenderingInProgress(isTrue: boolean): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createThis(),
      ts.factory.createIdentifier(IS_RENDERING_IN_PROGRESS)
    ),
    ts.factory.createToken(ts.SyntaxKind.EqualsToken),
    isTrue ? ts.factory.createTrue() : ts.factory.createFalse()
  ));
}

function addElmtIdNode(): boolean {
  return partialUpdateConfig.partialUpdateMode && projectConfig.optLazyForEach &&
    ((storedFileInfo.processLazyForEach && storedFileInfo.lazyForEachInfo.isDependItem) || storedFileInfo.processBuilder);
}

function processIfStatement(node: ts.IfStatement, newStatements: ts.Statement[],
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): void {
  const ifStatements: ts.Statement[] = [];
  if (addElmtIdNode()) {
    ifStatements.push(createCollectElmtIdNode());
    storedFileInfo.lazyForEachInfo.isDependItem = false;
  }
  const ifCreate: ts.ExpressionStatement = createIfCreate();
  const newIfNode: ts.IfStatement = processInnerIfStatement(node, 0, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
  ifStatements.push(ifCreate, newIfNode);
  const ifPop: ts.ExpressionStatement = createIfPop();
  if (!partialUpdateConfig.partialUpdateMode) {
    newStatements.push(ifCreate, newIfNode, ifPop);
  } else {
    newStatements.push(createComponentCreationStatement(node, ifStatements, COMPONENT_IF,
      isGlobalBuilder, false, undefined, null, builderParamsResult), ifPop);
  }
}

function processInnerIfStatement(node: ts.IfStatement, id: number, log: LogInfo[],
  isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): ts.IfStatement {
  if (ts.isIdentifier(node.expression) && node.expression.originalKeywordKind === undefined &&
    !node.expression.escapedText) {
    log.push({
      type: LogType.ERROR,
      message: 'Condition expression cannot be null in if statement.',
      pos: node.expression.getStart(),
      code: '10905208'
    });
    node = ts.factory.updateIfStatement(node, ts.factory.createIdentifier(COMPONENT_IF_UNDEFINED),
      node.thenStatement, node.elseStatement);
  }
  const newThenStatement: ts.Statement = processThenStatement(
    node.thenStatement, id, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
  const newElseStatement: ts.Statement = processElseStatement(
    node.elseStatement, id, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
  const newIfNode: ts.IfStatement = ts.factory.updateIfStatement(
    node, node.expression, newThenStatement, newElseStatement);
  return newIfNode;
}

function processThenStatement(thenStatement: ts.Statement, id: number,
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): ts.Statement {
  if (ts.isExpressionStatement(thenStatement) && ts.isIdentifier(thenStatement.expression) &&
    thenStatement.expression.originalKeywordKind === undefined &&
    !thenStatement.expression.escapedText) {
    log.push({
      type: LogType.ERROR,
      message: 'Then statement cannot be null in if statement.',
      pos: thenStatement.expression.getStart(),
      code: '10905207'
    });
  }
  if (thenStatement) {
    if (ts.isBlock(thenStatement)) {
      thenStatement = processIfBlock(thenStatement, id, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
    } else if (ts.isIfStatement(thenStatement)) {
      thenStatement = processInnerIfStatement(thenStatement, 0, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
      thenStatement = ts.factory.createBlock(
        partialUpdateConfig.partialUpdateMode ?
          [createIfCreate(), createIfBranchFunc(id, [thenStatement], isGlobalBuilder), createIfPop()] :
          [createIfCreate(), createIfBranchId(id), thenStatement, createIfPop()],
        true
      );
    } else {
      thenStatement = ts.factory.createBlock([thenStatement], true);
      thenStatement = processIfBlock(thenStatement as ts.Block, id, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
    }
  }
  return thenStatement;
}

function processElseStatement(elseStatement: ts.Statement, id: number,
  log: LogInfo[], isBuilder: boolean = false, isGlobalBuilder: boolean = false,
  builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): ts.Statement {
  if (elseStatement) {
    if (ts.isBlock(elseStatement)) {
      elseStatement = processIfBlock(elseStatement, id + 1, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
    } else if (ts.isIfStatement(elseStatement)) {
      elseStatement = processInnerIfStatement(elseStatement, id + 1, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
    } else {
      elseStatement = ts.factory.createBlock([elseStatement], true);
      elseStatement = processIfBlock(elseStatement as ts.Block, id + 1, log, isBuilder, isGlobalBuilder, builderParamsResult, isInRepeatTemplate);
    }
  } else if (partialUpdateConfig.partialUpdateMode) {
    elseStatement = ts.factory.createBlock([
      ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          ts.factory.createThis(),
          ts.factory.createIdentifier(IFELSEBRANCHUPDATEFUNCTION)
        ),
        undefined,
        [
          ts.factory.createNumericLiteral(++id),
          ts.factory.createArrowFunction(
            undefined,
            undefined,
            [],
            undefined,
            ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
            ts.factory.createBlock(
              [],
              true
            )
          )
        ]
      ))
    ], true);
  }
  return elseStatement;
}

function processIfBlock(block: ts.Block, id: number, log: LogInfo[], isBuilder: boolean = false,
  isGlobalBuilder: boolean = false, builderParamsResult: BuilderParamsResult = null, isInRepeatTemplate: boolean = false): ts.Block {
  return addIfBranchId(id, isGlobalBuilder,
    processComponentBlock(block, false, log, false, isBuilder, COMPONENT_IF, undefined, isGlobalBuilder, builderParamsResult, false, isInRepeatTemplate));
}

function addIfBranchId(id: number, isGlobalBuilder: boolean = false, container: ts.Block): ts.Block {
  let containerStatements: ts.Statement[];
  if (partialUpdateConfig.partialUpdateMode) {
    containerStatements = [createIfBranchFunc(id, [...container.statements], isGlobalBuilder)];
  } else {
    containerStatements = [createIfBranchId(id), ...container.statements];
  }
  return ts.factory.updateBlock(container, containerStatements);
}

function createIf(): ts.Identifier {
  return ts.factory.createIdentifier(COMPONENT_IF);
}

function createIfCreate(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(createFunction(createIf(),
    ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), ts.factory.createNodeArray([])));
}

function createIfPop(): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(createFunction(createIf(),
    ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null));
}

function createIfBranchId(id: number): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(createFunction(createIf(),
    ts.factory.createIdentifier(COMPONENT_IF_BRANCH_ID_FUNCTION),
    ts.factory.createNodeArray([ts.factory.createNumericLiteral(id)])));
}

function createIfBranchFunc(id: number, innerStatements: ts.Statement[],
  isGlobalBuilder: boolean = false): ts.ExpressionStatement {
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(ts.factory.createPropertyAccessExpression(
    isGlobalBuilder ? parentConditionalExpression() : ts.factory.createThis(),
    ts.factory.createIdentifier(IFELSEBRANCHUPDATEFUNCTION)), undefined,
  [ts.factory.createNumericLiteral(id), ts.factory.createArrowFunction(undefined, undefined, [], undefined,
    ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken), ts.factory.createBlock(innerStatements, true))]));
}

interface CreateResult {
  newNode: ts.ExpressionStatement;
  identifierNode: ts.Identifier;
  isContainerComponent: boolean;
  isButton: boolean;
  needPop: boolean;
}

function createComponent(node: ts.ExpressionStatement, type: string): CreateResult {
  const res: CreateResult = {
    newNode: node,
    identifierNode: null,
    isContainerComponent: false,
    isButton: false,
    needPop: false
  };
  let identifierNode: ts.Identifier = ts.factory.createIdentifier(type);
  let temp: any = node.expression;
  while (temp && !ts.isIdentifier(temp) && temp.expression) {
    temp = temp.expression;
  }
  if (temp && temp.parent && (ts.isCallExpression(temp.parent) ||
    ts.isEtsComponentExpression(temp.parent)) && ts.isIdentifier(temp)) {
    if (temp.getText() === COMPONENT_BUTTON && type !== COMPONENT_POP_FUNCTION) {
      res.isButton = true;
      identifierNode = type === COMPONENT_CREATE_CHILD_FUNCTION ?
        ts.factory.createIdentifier(COMPONENT_CREATE_CHILD_FUNCTION) :
        ts.factory.createIdentifier(COMPONENT_CREATE_LABEL_FUNCTION);
    }
    if (NEEDPOP_COMPONENT.has(temp.getText())) {
      res.needPop = true;
    }
    if (checkContainer(temp.getText(), temp.parent)) {
      res.isContainerComponent = true;
    }
    res.newNode = type === COMPONENT_POP_FUNCTION ?
      ts.factory.createExpressionStatement(createFunction(temp, identifierNode, null)) :
      ts.factory.createExpressionStatement(createFunction(temp, identifierNode, checkArguments(temp, type)));
    res.identifierNode = temp;
  }
  return res;
}

function checkArguments(temp: ts.Identifier, type: string): ts.Expression[] {
  const newArguments: ts.Expression[] = [];
  if (CUSTOM_BUILDER_CONSTRUCTORS.has(temp.escapedText.toString())) {
    temp.parent.arguments.forEach(argument => {
      if (ts.isConditionalExpression(argument)) {
        newArguments.push(processConditionalBuilder(argument, temp, type));
      } else if (isBuilderChangeNode(argument, temp, type)) {
        newArguments.push(parseBuilderNode(argument, type));
      } else {
        newArguments.push(argument);
      }
    });
    return newArguments;
  }
  return temp.getText() === 'XComponent' && type === COMPONENT_CREATE_FUNCTION &&
    projectConfig.moduleName && projectConfig.bundleName ?
    // @ts-ignore
    temp.parent.arguments.concat([
      ts.factory.createStringLiteral(`${projectConfig.bundleName}/${projectConfig.moduleName}`)
    ]) : temp.parent.arguments;
}

function checkContainer(name: string, node: ts.Node): boolean {
  return BUILDIN_CONTAINER_COMPONENT.has(name) && (name !== 'XComponent' ||
    (node && node.arguments && node.arguments.length &&
    ts.isObjectLiteralExpression(node.arguments[0]) && node.arguments[0].properties &&
    checkComponentType(node.arguments[0].properties)));
}

function checkComponentType(properties: ts.PropertyAssignment[]): boolean {
  let flag: boolean = false;
  properties.forEach(item => {
    if (isXComponentContainer(item)) {
      flag = true;
    }
  });
  return flag;
}

function isXComponentContainer(item: ts.PropertyAssignment): boolean {
  return item.name && ts.isIdentifier(item.name) && item.name.getText() === RESOURCE_NAME_TYPE &&
    item.initializer && ((ts.isStringLiteral(item.initializer) &&
    // value = 'component'
    (item.initializer.getText() === XCOMPONENT_SINGLE_QUOTATION ||
    item.initializer.getText() === XCOMPONENT_DOUBLE_QUOTATION)) ||
    // value = 1
    (ts.isNumericLiteral(item.initializer) && item.initializer.getText() === '1') ||
    // value = XComponentType.COMPONENT
    (ts.isPropertyAccessExpression(item.initializer) && item.initializer.expression &&
    ts.isIdentifier(item.initializer.expression) && item.initializer.name &&
    ts.isIdentifier(item.initializer.name) && item.initializer.expression.getText() === XCOMPONENTTYPE) &&
    item.initializer.name.getText() === XCOMPONENTTYPE_CONTAINER);
}

interface AnimationInfo {
  statement: ts.Statement,
  kind: boolean,
  hasAnimationAttr: boolean,
}

export interface ComponentAttrInfo {
  reuseId: ts.Node,
  hasIdAttr: boolean,
  attrCount: number,
  reuse: string,
}

export function bindComponentAttr(node: ts.ExpressionStatement, identifierNode: ts.Identifier,
  newStatements: ts.Statement[], log: LogInfo[], reverse: boolean = true,
  isStylesAttr: boolean = false, newImmutableStatements: ts.Statement[] = null,
  isStyleFunction: boolean = false, componentAttrInfo: ComponentAttrInfo = null,
  isReusableV2NodeAttr: boolean = false): void {
  if (!node) {
    return;
  }
  const isStylesUIComponent: boolean = validateStylesUIComponent(node, isStylesAttr);
  let temp = node.expression;
  const statements: ts.Statement[] = [];
  const immutableStatements: ts.Statement[] = [];
  const updateStatements: ts.Statement[] = [];
  const lastStatement: AnimationInfo = {
    statement: null,
    kind: false,
    hasAnimationAttr: false
  };
  const isRecycleComponent: boolean = isRecycle(componentCollection.currentClassName);
  const isReuseComponentInV2: boolean = isReuseInV2(componentCollection.currentClassName);
  if (ts.isPropertyAccessExpression(temp) || isStylesUIComponent) {
    log.push({
      type: isStylesUIComponent ? LogType.WARN : LogType.ERROR,
      message: `'${node.getText()}' does not meet UI component syntax.`,
      pos: node.getStart(),
      code: '10905206'
    });
  }
  while (temp && ts.isCallExpression(temp) && temp.expression) {
    let flag: boolean = false;
    if (temp.expression && (validatePropertyAccessExpressionWithCustomBuilder(temp.expression) ||
      validateIdentifierWithCustomBuilder(temp.expression))) {
      let propertyName: string = '';
      if (ts.isIdentifier(temp.expression)) {
        propertyName = temp.expression.escapedText.toString();
      } else if (ts.isPropertyAccessExpression(temp.expression)) {
        propertyName = temp.expression.name.escapedText.toString();
      }
      switch (true) {
        case BIND_POPUP_SET.has(propertyName):
          temp = processBindPopupBuilder(temp);
          break;
        case BIND_DRAG_SET.has(propertyName):
          temp = processDragStartBuilder(temp, propertyName);
          break;
        default:
          temp = processCustomBuilderProperty(temp, identifierNode, propertyName);
      }
    }
    if (ts.isPropertyAccessExpression(temp.expression) &&
      temp.expression.name && ts.isIdentifier(temp.expression.name) &&
      (!componentCollection.customComponents.has(temp.expression.name.getText()) || STYLES_ATTRIBUTE.has(temp.expression.name.getText()))) {
      parseRecycleId(temp, temp.expression.name, componentAttrInfo, log, isReusableV2NodeAttr);
      addComponentAttr(temp, temp.expression.name, lastStatement, statements, identifierNode, log,
        isStylesAttr, immutableStatements, updateStatements, newImmutableStatements,
        isRecycleComponent, isReuseComponentInV2, isStyleFunction);
      temp = temp.expression.expression;
      flag = true;
    } else if (ts.isIdentifier(temp.expression)) {
      if (!INNER_COMPONENT_NAMES.has(temp.expression.getText()) &&
        !GESTURE_TYPE_NAMES.has(temp.expression.getText()) &&
        !componentCollection.customComponents.has(temp.expression.getText())) {
        parseRecycleId(temp, temp.expression.name, componentAttrInfo, log, isReusableV2NodeAttr);
        addComponentAttr(temp, temp.expression, lastStatement, statements, identifierNode, log,
          isStylesAttr, immutableStatements, updateStatements, newImmutableStatements,
          isRecycleComponent, isReuseComponentInV2, isStyleFunction);
      }
      break;
    }
    if (!flag) {
      temp = temp.expression;
    }
  }
  if (lastStatement.statement && lastStatement.kind) {
    statements.push(lastStatement.statement);
  }
  if ((!isRecycleComponent && !isReuseComponentInV2) || lastStatement.hasAnimationAttr) {
    if (statements.length) {
      reverse ? newStatements.push(...statements.reverse()) : newStatements.push(...statements);
    }
  } else {
    if (updateStatements.length) {
      reverse ? newStatements.push(...updateStatements.reverse()) : newStatements.push(...updateStatements);
    }
    if (newImmutableStatements && immutableStatements.length) {
      reverse ? newImmutableStatements.push(...immutableStatements.reverse()) : newImmutableStatements.push(...immutableStatements);
    }
  }
}  

function validateStylesUIComponent(node: ts.ExpressionStatement, isStylesAttr: boolean): boolean {
  return (ts.isIfStatement(node) || ts.isSwitchStatement(node)) && isStylesAttr;
}

function parseRecycleId(node: ts.CallExpression, attr: ts.Identifier, componentAttrInfo: ComponentAttrInfo, log: LogInfo[],
  isReusableV2NodeAttr: boolean = false): void {
  if (componentAttrInfo) {
    const attrName: string = attr.escapedText.toString();
    if (attrName === RECYCLE_REUSE_ID) {
      logMessageCollection.checkUsageOfReuseIdAttribute(node, isReusableV2NodeAttr, log);
      componentAttrInfo.reuseId = node.arguments[0];
    } else if (attrName === ATTRIBUTE_ID) {
      componentAttrInfo.hasIdAttr = true;
    } else if (attrName === REUSE_ATTRIBUTE) {
      logMessageCollection.checkUsageOfReuseAttribute(node, isReusableV2NodeAttr, log);
      if (ts.isObjectLiteralExpression(node.arguments[0]) && !!getReuseIdInReuse(node.arguments[0])) {
        componentAttrInfo.reuse = getReuseIdInReuse(node.arguments[0]);
      } else {
        componentAttrInfo.reuse = '';
      }
    }
    componentAttrInfo.attrCount++;
  }
}

function getReuseIdInReuse(node: ts.ObjectLiteralExpression): string {
  let reuse: string = '';
  if (node.properties && node.properties.length) {
    node.properties.forEach((item: ts.ObjectLiteralElementLike) => {
      if (ts.isPropertyAssignment(item) && item.name && ts.isIdentifier(item.name) &&
        item.name.getText() === RECYCLE_REUSE_ID && item.initializer && 
        ts.isArrowFunction(item.initializer) && item.initializer.body &&
        ts.isStringLiteral(item.initializer.body)) {
        reuse = item.initializer.body.text;
      }
    });
  }
  return reuse;
}

function processCustomBuilderProperty(node: ts.CallExpression, identifierNode: ts.Identifier,
  propertyName: string): ts.CallExpression {
  const newArguments: ts.Expression[] = [];
  node.arguments.forEach((argument: ts.Expression | ts.Identifier, index: number) => {
    if (ts.isConditionalExpression(argument)) {
      newArguments.push(processConditionalBuilder(argument, identifierNode, propertyName));
    } else if (isBuilderChangeNode(argument, identifierNode, propertyName)) {
      newArguments.push(parseBuilderNode(argument, propertyName));
    } else {
      newArguments.push(argument);
    }
  });
  node = ts.factory.updateCallExpression(node, node.expression, node.typeArguments, newArguments);
  return node;
}

function isBuilderChangeNode(argument: ts.Node, identifierNode: ts.Identifier, propertyName: string): boolean {
  return ts.isPropertyAccessExpression(argument) && argument.name && ts.isIdentifier(argument.name) &&
    storedFileInfo.builderLikeCollection.has(argument.name.getText()) ||
    ts.isCallExpression(argument) && argument.expression && argument.expression.name &&
    ts.isIdentifier(argument.expression.name) &&
    storedFileInfo.builderLikeCollection.has(argument.expression.name.getText()) || ts.isIdentifier(argument) &&
    argument.escapedText && storedFileInfo.builderLikeCollection.has(argument.escapedText.toString()) ||
    ts.isObjectLiteralExpression(argument) && (BIND_OBJECT_PROPERTY.get(identifierNode.escapedText.toString()) &&
    BIND_OBJECT_PROPERTY.get(identifierNode.escapedText.toString()).has(propertyName) ||
    BIND_OBJECT_PROPERTY.get(ALL_COMPONENTS).has(propertyName)) ||
    ts.isCallExpression(argument) && argument.expression && ts.isIdentifier(argument.expression) &&
    storedFileInfo.builderLikeCollection.has(argument.expression.escapedText.toString()) ||
    isWrappedBuilder(argument as ts.PropertyAccessExpression) || isWrappedBuilderCallExpression(argument as ts.CallExpression);
}

export function isWrappedBuilder(node: ts.PropertyAccessExpression): boolean {
  const typeAtLocation = globalProgram.checker.getTypeAtLocation(node.expression);
  if (projectConfig.minAPIVersion >= 11 && ts.isPropertyAccessExpression(node) &&
    node.name && ts.isIdentifier(node.name) && node.name.escapedText.toString() === WRAPBUILDER_BUILDERPROP &&
    typeAtLocation && typeAtLocation.symbol && typeAtLocation.symbol.escapedName === WRAPPEDBUILDER_CLASS) {
    return true;
  }
  return false;
}

function isWrappedBuilderCallExpression(node: ts.CallExpression): boolean {
  if (projectConfig.minAPIVersion >= 11 && ts.isCallExpression(node) && node.expression &&
    isWrappedBuilder(node.expression as ts.PropertyAccessExpression)) {
    return true;
  }
  return false;
}

function parseBuilderNode(node: ts.Node, propertyName: string):
  ts.ObjectLiteralExpression | ts.CallExpression | ts.ArrowFunction {
  if (isWrappedBuilder(node as ts.PropertyAccessExpression) || isPropertyAccessExpressionNode(node)) {
    if (CUSTOM_BUILDER_PROPERTIES_WITHOUTKEY.has(propertyName)) {
      return processPropertyBuilderWithoutKey(node as ts.PropertyAccessExpression);
    } else {
      return processPropertyBuilder(node as ts.PropertyAccessExpression);
    }
  } else if (ts.isIdentifier(node) && storedFileInfo.builderLikeCollection.has(node.escapedText.toString())) {
    if (CUSTOM_BUILDER_PROPERTIES_WITHOUTKEY.has(propertyName)) {
      return processIdentifierBuilderWithoutKey(node);
    } else {
      return processIdentifierBuilder(node);
    }
  } else if (isWrappedBuilderCallExpression(node as ts.CallExpression) || ts.isCallExpression(node)) {
    if (CUSTOM_BUILDER_PROPERTIES_WITHOUTKEY.has(propertyName)) {
      return getParsedBuilderAttrArgumentWithParamsWithoutKey(node);
    } else {
      return getParsedBuilderAttrArgumentWithParams(node);
    }
  } else if (ts.isObjectLiteralExpression(node)) {
    return processObjectPropertyBuilder(node);
  }
  return undefined;
}

export function processObjectPropertyBuilder(node: ts.ObjectLiteralExpression): ts.ObjectLiteralExpression {
  const newProperties: ts.PropertyAssignment[] = [];
  node.properties.forEach((property: ts.PropertyAssignment) => {
    if (property.name && ts.isIdentifier(property.name) &&
      [CUSTOM_DIALOG_CONTROLLER_BUILDER, HEADER, INDICATORBUILDER, FOOTER, START, END, PREVIEW, TITLE].includes(
        property.name.escapedText.toString()) && property.initializer) {
      if (isPropertyAccessExpressionNode(property.initializer) || ts.isIdentifier(property.initializer) &&
        storedFileInfo.builderLikeCollection.has(property.initializer.escapedText.toString())) {
        newProperties.push(ts.factory.updatePropertyAssignment(property, property.name,
          ts.factory.createCallExpression(
            ts.factory.createPropertyAccessExpression(
              property.initializer,
              ts.factory.createIdentifier(BUILDER_ATTR_BIND)
            ),
            undefined,
            [ts.factory.createThis()]
          )));
      } else if (isGlobalBuilderCallExpressionNode(property.initializer) ||
        isInnerBuilderCallExpressionNode(property.initializer)) {
        newProperties.push(transformBuilderCallExpression(property));
      } else if (ts.isObjectLiteralExpression(property.initializer)) {
        newProperties.push(ts.factory.updatePropertyAssignment(property, property.name,
          processObjectPropertyBuilder(property.initializer)));
      } else {
        newProperties.push(property);
      }
    } else {
      newProperties.push(property);
    }
  });
  return ts.factory.updateObjectLiteralExpression(node, newProperties);
}

function transDoubleDollarInCustomBuilder(node: ts.CallExpression): ts.Expression[] {
  let name: string = '';
  if (node.expression && ts.isIdentifier(node.expression)) {
    name = node.expression.escapedText.toString();
  } else if (node.expression && ts.isPropertyAccessExpression(node.expression) &&
    node.expression.name && ts.isIdentifier(node.expression.name)) {
    name = node.expression.name.escapedText.toString();
  }
  if (node.arguments.length === 1 && ts.isObjectLiteralExpression(node.arguments[0])) {
    return [ts.factory.createCallExpression(
      ts.factory.createIdentifier(BUILDER_PARAM_PROXY),
      undefined,
      [
        ts.factory.createStringLiteral(name),
        traverseBuilderParams(node.arguments[0], storedFileInfo.processBuilder)
      ]
    )];
  } else {
    return node.arguments;
  }
}

function transformBuilderCallExpression(property: ts.PropertyAssignment): ts.PropertyAssignment {
  const newArguments: ts.Expression[] = transDoubleDollarInCustomBuilder(property.initializer as ts.CallExpression);
  return ts.factory.updatePropertyAssignment(property, property.name,
    ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        property.initializer.expression,
        ts.factory.createIdentifier(BUILDER_ATTR_BIND)
      ),
      undefined,
      [ts.factory.createThis(), ...(newArguments || [])]
    ));
}

function isInnerBuilderCallExpressionNode(node: ts.Node): boolean {
  return ts.isCallExpression(node) && node.expression && isPropertyAccessExpressionNode(node.expression);
}

function isGlobalBuilderCallExpressionNode(node: ts.Node): boolean {
  return ts.isCallExpression(node) && node.expression && ts.isIdentifier(node.expression) &&
    CUSTOM_BUILDER_METHOD.has(node.expression.escapedText.toString());
}

function isPropertyAccessExpressionNode(node: ts.Node): boolean {
  return ts.isPropertyAccessExpression(node) && node.expression &&
    node.expression.kind === ts.SyntaxKind.ThisKeyword && node.name && ts.isIdentifier(node.name) &&
    storedFileInfo.builderLikeCollection.has(node.name.escapedText.toString());
}

function processBindPopupBuilder(node: ts.CallExpression): ts.CallExpression {
  const newArguments: ts.Expression[] = [];
  node.arguments.forEach((argument: ts.ObjectLiteralExpression, index: number) => {
    if (index === 1 && ts.isObjectLiteralExpression(argument)) {
      // @ts-ignore
      newArguments.push(processBindPopupBuilderProperty(argument));
    } else {
      newArguments.push(argument);
    }
  });
  node = ts.factory.updateCallExpression(node, node.expression, node.typeArguments, newArguments);
  return node;
}

function processDragStartBuilder(node: ts.CallExpression, propertyName: string): ts.CallExpression {
  const newStatements: ts.Statement[] = [];
  if (isNodeFunction(node)) {
    // @ts-ignore
    for (let i = 0; i < node.arguments[0].body.statements.length; i++) {
      // @ts-ignore
      const statement: ts.Statement = node.arguments[0].body.statements[i];
      newStatements.push(checkStatement(statement, propertyName));
    }
    node = ts.factory.updateCallExpression(node, node.expression, node.typeArguments, [ts.factory.updateArrowFunction(
      // @ts-ignore
      node.arguments[0], undefined, undefined, node.arguments[0].parameters, node.arguments[0].type,
      // @ts-ignore
      node.arguments[0].equalsGreaterThanToken, ts.factory.updateBlock(node.arguments[0].body, newStatements))]);
  }
  return node;
}

function isNodeFunction(node: ts.CallExpression): boolean {
  return node.arguments && node.arguments.length && ts.isArrowFunction(node.arguments[0]) && node.arguments[0].body &&
    ts.isBlock(node.arguments[0].body);
}

function checkStatement(statement: ts.Statement, propertyName: string): ts.Statement {
  if (ts.isReturnStatement(statement)) {
    if (ts.isObjectLiteralExpression(statement.expression)) {
      const newProperties: ts.ObjectLiteralElementLike[] = [];
      for (let j = 0; j < statement.expression.properties.length; j++) {
        let property: ts.ObjectLiteralElementLike = statement.expression.properties[j];
        property = checkProperty(property, propertyName);
        newProperties.push(property);
      }
      return ts.factory.createReturnStatement(ts.factory.createObjectLiteralExpression(newProperties));
    } else {
      let initializer: ts.Expression = statement.expression;
      initializer = processInitializer(initializer, propertyName);
      return ts.factory.updateReturnStatement(statement, initializer);
    }
  } else {
    return statement;
  }
}

function checkProperty(property: ts.ObjectLiteralElementLike, propertyName: string): ts.ObjectLiteralElementLike {
  if (isPropertyFunction(property)) {
    let initializer: ts.Expression = property.initializer;
    initializer = processInitializer(initializer, propertyName);
    property = ts.factory.createPropertyAssignment(property.name, initializer);
  }
  return property;
}

function processInitializer(initializer: ts.Expression, propertyName: string): ts.Expression {
  if (initializer && ts.isConditionalExpression(initializer)) {
    return processConditionalBuilder(initializer, ts.factory.createIdentifier(CUSTOM_COMPONENT_DEFAULT),
      propertyName);
  } else if (isBuilderChangeNode(initializer, ts.factory.createIdentifier(CUSTOM_COMPONENT_DEFAULT),
    propertyName)) {
    return parseBuilderNode(initializer, propertyName);
  }
  return initializer;
}

function isPropertyFunction(property: ts.ObjectLiteralElementLike): boolean {
  return ts.isPropertyAssignment(property) && property.name && ts.isIdentifier(property.name) &&
    property.name.escapedText.toString() === BUILDER_ATTR_NAME;
}

function processBindPopupBuilderProperty(node: ts.ObjectLiteralExpression): ts.ObjectLiteralExpression {
  const newProperties: ts.PropertyAssignment[] = [];
  node.properties.forEach((property: ts.PropertyAssignment, index: number) => {
    if (property.name && ts.isIdentifier(property.name) && property.initializer &&
      property.name.escapedText.toString() === CUSTOM_DIALOG_CONTROLLER_BUILDER) {
      let initializer: ts.Expression = property.initializer;
      initializer = processInitializer(initializer, BIND_POPUP);
      newProperties.push(ts.factory.updatePropertyAssignment(property, property.name, initializer));
    } else {
      newProperties.push(property);
    }
  });
  return ts.factory.updateObjectLiteralExpression(node, newProperties);
}

function processConditionalBuilder(initializer: ts.ConditionalExpression, identifierNode: ts.Identifier,
  propertyName: string): ts.ConditionalExpression {
  let whenTrue: ts.Expression = initializer.whenTrue;
  let whenFalse: ts.Expression = initializer.whenFalse;
  if (isBuilderChangeNode(initializer.whenTrue, identifierNode, propertyName)) {
    whenTrue = parseBuilderNode(initializer.whenTrue, propertyName);
  }
  if (isBuilderChangeNode(initializer.whenFalse, identifierNode, propertyName)) {
    whenFalse = parseBuilderNode(initializer.whenFalse, propertyName);
  }
  return ts.factory.createConditionalExpression(
    initializer.condition,
    initializer.questionToken,
    whenTrue,
    initializer.colonToken,
    whenFalse
  );
}

function processPropertyBuilder(node: ts.PropertyAccessExpression): ts.ObjectLiteralExpression {
  return ts.factory.createObjectLiteralExpression([
    ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(BUILDER_ATTR_NAME),
      ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(
          node,
          ts.factory.createIdentifier(BUILDER_ATTR_BIND)
        ),
        undefined,
        [ts.factory.createThis()]
      )
    )
  ]);
}

function processPropertyBuilderWithoutKey(node: ts.PropertyAccessExpression): ts.CallExpression {
  return ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      node,
      ts.factory.createIdentifier(BUILDER_ATTR_BIND)
    ),
    undefined,
    [ts.factory.createThis()]
  );
}

function processIdentifierBuilder(node: ts.Identifier): ts.ObjectLiteralExpression {
  return ts.factory.createObjectLiteralExpression([
    ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(BUILDER_ATTR_NAME),
      ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(node, ts.factory.createIdentifier(BUILDER_ATTR_BIND)),
        undefined, [ts.factory.createThis()]
      )
    )
  ]);
}

function processIdentifierBuilderWithoutKey(node: ts.Identifier): ts.CallExpression {
  return ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(node, ts.factory.createIdentifier(BUILDER_ATTR_BIND)),
    undefined, [ts.factory.createThis()]
  );
}

function getParsedBuilderAttrArgumentWithParams(node: ts.CallExpression):
  ts.ObjectLiteralExpression {
  const newArguments: ts.Expression[] = transDoubleDollarInCustomBuilder(node);
  return ts.factory.createObjectLiteralExpression([
    ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(BUILDER_ATTR_NAME),
      ts.factory.createArrowFunction(
        undefined,
        undefined,
        [],
        undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock(
          [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
            ts.factory.createPropertyAccessExpression(node.expression, ts.factory.createIdentifier(CALL)
            ), undefined, [ts.factory.createThis(), ...newArguments]))],
          true
        )
      )
    )
  ]);
}

function getParsedBuilderAttrArgumentWithParamsWithoutKey(node: ts.CallExpression):
  ts.ArrowFunction {
  const newArguments: ts.Expression[] = transDoubleDollarInCustomBuilder(node);
  return ts.factory.createArrowFunction(
    undefined,
    undefined,
    [],
    undefined,
    ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
    ts.factory.createBlock(
      [ts.factory.createExpressionStatement(ts.factory.createCallExpression(
        ts.factory.createPropertyAccessExpression(node.expression, ts.factory.createIdentifier(CALL)
        ), undefined, [ts.factory.createThis(), ...newArguments]))],
      true
    )
  );
}

function validatePropertyAccessExpressionWithCustomBuilder(node: ts.Node): boolean {
  return ts.isPropertyAccessExpression(node) && node.name &&
    ts.isIdentifier(node.name) && CUSTOM_BUILDER_PROPERTIES.has(node.name.escapedText.toString());
}

function validateIdentifierWithCustomBuilder(node: ts.Node): boolean {
  return ts.isIdentifier(node) && CUSTOM_BUILDER_PROPERTIES.has(node.escapedText.toString());
}

function createArrowFunctionForDollar($$varExp: ts.Expression): ts.ArrowFunction {
  let varExp: ts.ConditionalExpression | ts.BinaryExpression;
  if (ts.isConditionalExpression($$varExp)) {
    varExp = ts.factory.updateConditionalExpression(
      $$varExp, $$varExp.condition, ts.factory.createToken(ts.SyntaxKind.QuestionToken), $$varExp.whenTrue,
      ts.factory.createToken(ts.SyntaxKind.ColonToken), ts.factory.createBinaryExpression(
        $$varExp.whenFalse, ts.factory.createToken(ts.SyntaxKind.EqualsToken), ts.factory.createIdentifier($$_NEW_VALUE))); 
  } else {
    varExp = ts.factory.createBinaryExpression(
      $$varExp, ts.factory.createToken(ts.SyntaxKind.EqualsToken), ts.factory.createIdentifier($$_NEW_VALUE)
    );
  }
    return ts.factory.createArrowFunction(
      undefined, undefined,
      [ts.factory.createParameterDeclaration(
        undefined, undefined,
        ts.factory.createIdentifier($$_NEW_VALUE),
        undefined, undefined, undefined
      )],
      undefined,
      ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
      ts.factory.createBlock(
        [ts.factory.createExpressionStatement(varExp)],
        false
      )
    );
}

function updateArgumentForExclamation(argument): ts.Expression {
  if (ts.isNonNullExpression(argument) && ts.isNonNullExpression(argument.expression)) {
    return argument.expression.expression;
  }
  return argument;
}

function updateArgumentForDollar(argument): ts.Expression {
  if (ts.isElementAccessExpression(argument)) {
    return ts.factory.updateElementAccessExpression(
      argument, updateArgumentForDollar(argument.expression), argument.argumentExpression);
  } else if (ts.isIdentifier(argument)) {
    if (argument.getText() === $$_THIS) {
      return ts.factory.createThis();
    } else if (argument.getText().match(/^\$\$(.|\n)+/)) {
      return ts.factory.createIdentifier(argument.getText().replace(/\$\$/, ''));
    }
  } else if (ts.isPropertyAccessExpression(argument)) {
    return ts.factory.updatePropertyAccessExpression(
      argument, updateArgumentForDollar(argument.expression), argument.name);
  }
  return argument;
}

function verifyComponentId(temp: any, node: ts.Identifier, propName: string,
  log: LogInfo[]): void {
  if (!newsupplement.isAcceleratePreview && propName === ATTRIBUTE_ID &&
    ts.isStringLiteral(temp.arguments[0])) {
    const id: string = temp.arguments[0].text;
    const posOfNode: ts.LineAndCharacter = transformLog.sourceFile
      .getLineAndCharacterOfPosition(getRealNodePos(node));
    const curFileName: string = transformLog.sourceFile.fileName.replace(/\.ts$/, '');
    const rPath: string = path.resolve(projectConfig.projectPath, curFileName)
      .replace(/\\+/g, '/');
    const rLine: number = posOfNode.line + 1;
    const rCol: number = posOfNode.character + 1;
    if (ID_ATTRS.has(id)) {
      const idInfo: Map<string, string | number> = ID_ATTRS.get(id);
      if (!(idInfo.get('path') === rPath &&
        idInfo.get('line') === rLine &&
        idInfo.get('col') === rCol)) {
        log.push({
          type: LogType.WARN,
          message: `The current component id "${id}" is duplicate with ` +
            `${idInfo.get('path')}:${idInfo.get('line')}:${idInfo.get('col')}.`,
          pos: node.pos
        });
      }
    } else {
      ID_ATTRS.set(id, new Map().set('path', rPath)
        .set('line', rLine)
        .set('col', rCol));
    }
  }
}

class StyleResult {
  doubleDollar: boolean = false;
  doubleExclamation: boolean = false;
}

function isDoubleBind(styleResult: StyleResult, isStylesAttr: boolean, identifierNode: ts.Identifier,
  propName: string, temp: any): boolean {
  if (isDoubleDollarToChange(isStylesAttr, identifierNode, propName, temp)) {
    styleResult.doubleDollar = true;
    return true;
  } else if (isDoubleExclamationToChange(isStylesAttr, identifierNode, propName, temp)) {
    styleResult.doubleExclamation = true;
    return true;
  }
  return false;
}

function addComponentAttr(temp, node: ts.Identifier, lastStatement,
  statements: ts.Statement[], identifierNode: ts.Identifier, log: LogInfo[],
  isStylesAttr: boolean, immutableStatements: ts.Statement[], updateStatements: ts.Statement[],
  newImmutableStatements: ts.Statement[] = null, isRecycleComponent: boolean = false,
  isReuseComponentInV2: boolean = false, isStyleFunction: boolean = false): void {
  const styleResult: StyleResult = new StyleResult();
  const propName: string = node.getText();
  verifyComponentId(temp, node, propName, log);
  const extendType: ExtendType = {type: ''};
  if (propName === ATTRIBUTE_ANIMATION) {
    const animationNullNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(
      createFunction(ts.factory.createIdentifier(GLOBAL_CONTEXT), node,
        // @ts-ignore
        [ts.factory.createNull()]));
    if (!lastStatement.statement) {
      if (!(temp.arguments.length === 1 &&
        temp.arguments[0].kind === ts.SyntaxKind.NullKeyword)) {
        statements.push(animationNullNode);
      }
    } else {
      statements.push(lastStatement.statement, animationNullNode);
    }
    lastStatement.statement = ts.factory.createExpressionStatement(createFunction(
      ts.factory.createIdentifier(GLOBAL_CONTEXT), node, temp.arguments));
    lastStatement.kind = false;
    lastStatement.hasAnimationAttr = true;
  } else if (GESTURE_ATTRS.has(propName)) {
    parseGesture(temp, propName, statements, log, updateStatements);
    lastStatement.kind = true;
  } else if (isExtendFunctionNode(identifierNode, propName, extendType)) {
    if (newsupplement.isAcceleratePreview) {
      log.push({
        type: LogType.ERROR,
        message: `Doesn't support '@Extend' function now.`,
        pos: temp.getStart(),
        code: '10906205'
      });
    }
    let functionName: string = '';
    if (extendType.type === CHECK_COMPONENT_EXTEND_DECORATOR) {
      functionName = `__${identifierNode.escapedText.toString()}__${propName}`;
    } else {
      functionName = propName;
    }
    const extendNode: ts.Statement = ts.factory.createExpressionStatement(
      ts.factory.createCallExpression(ts.factory.createIdentifier(functionName), undefined,
        extendType.type === CHECK_COMPONENT_EXTEND_DECORATOR ?
          temp.arguments :
          [
            ...temp.arguments, ts.factory.createIdentifier(ELMTID),
            ts.factory.createIdentifier(ISINITIALRENDER),
            ts.factory.createThis()
          ]
      ));
    statements.push(extendNode);
    updateStatements.push(extendNode);
    lastStatement.kind = true;
  } else if (propName === ATTRIBUTE_STATESTYLES) {
    if (temp.arguments.length === 1 && ts.isObjectLiteralExpression(temp.arguments[0])) {
      statements.push(createViewStackProcessor(temp, true));
      if (isRecycleComponent) {
        updateStatements.push(createViewStackProcessor(temp, true));
      }
      traverseStateStylesAttr(temp, statements, identifierNode, log, updateStatements,
        newImmutableStatements, isRecycleComponent);
      lastStatement.kind = true;
    } else {
      validateStateStyleSyntax(temp, log);
    }
  } else if (GLOBAL_STYLE_FUNCTION.has(propName) || INNER_STYLE_FUNCTION.has(propName)) {
    const styleBlock: ts.Block =
        INNER_STYLE_FUNCTION.get(propName) || GLOBAL_STYLE_FUNCTION.get(propName);
    if (styleBlock.statements.length > 0) {
      bindComponentAttr(styleBlock.statements[0] as ts.ExpressionStatement, identifierNode,
        statements, log, false, true, newImmutableStatements);
      if (isRecycleComponent) {
        bindComponentAttr(styleBlock.statements[0] as ts.ExpressionStatement, identifierNode,
          updateStatements, log, false, true, newImmutableStatements, true);
      }
    }
    lastStatement.kind = true;
  } else if (isDoubleBind(styleResult, isStylesAttr, identifierNode, propName, temp)) {
    const argumentsArr: ts.Expression[] = [];
    styleResult.doubleDollar ? classifyArgumentsNum(temp.arguments, argumentsArr, propName, identifierNode) :
      classifyArgumentsNumV2(temp.arguments, argumentsArr, propName, identifierNode);
    const doubleDollarNode: ts.Statement = ts.factory.createExpressionStatement(
      createFunction(identifierNode, node, argumentsArr));
    statements.push(doubleDollarNode);
    updateStatements.push(doubleDollarNode);
    lastStatement.kind = true;
  } else {
    temp = loopEtscomponent(temp, isStylesAttr);
    if (propName !== RECYCLE_REUSE_ID && propName !== REUSE_ATTRIBUTE) {
      let isAttributeModifier: boolean = false;
      if ([ATTRIBUTE_ATTRIBUTE_MODIFIER, ATTRIBUTE_CONTENT_MODIFIER,
        ATTRIBUTE_MENUITEM_CONTENT_MODIFIER].includes(propName)) {
        isAttributeModifier = true;
      }
      const attrStatement: ts.Statement = ts.factory.createExpressionStatement(
        createFunction(identifierNode, node, temp.arguments, isAttributeModifier));
      statements.push(attrStatement);
      if ((isRecycleComponent || isReuseComponentInV2) && (!isStylesAttr || isStyleFunction) &&
        !isGestureType(identifierNode) && filterRegularAttrNode(temp.arguments)) {
        immutableStatements.push(attrStatement);
      } else {
        updateStatements.push(attrStatement);
      }
    }
    lastStatement.kind = true;
  }
}

function isGestureType(node: ts.Identifier): boolean {
  return GESTURE_TYPE_NAMES.has(node.escapedText.toString());
}

function filterRegularAttrNode(argumentsNode: ts.NodeArray<ts.Expression>): boolean {
  return argumentsNode.every((argument: ts.Expression) => {
    return isRegularAttrNode(argument);
  });
}

type AttrResult = { isRegularNode: boolean };
function isRegularAttrNode(node: ts.Expression): boolean {
  if (ts.isObjectLiteralExpression(node)) {
    return node.properties.every((propNode: ts.PropertyAssignment) => {
      if (propNode.initializer) {
        return isRegularAttrNode(propNode.initializer);
      }
      return false;
    });
  }
  if (ts.isArrayLiteralExpression(node)) {
    return node.elements.every((child: ts.Expression) => {
      return isRegularAttrNode(child);
    });
  }
  // literal e.g. 'hello', 1, true, false, () => {}
  if (isLiteralNode(node)) {
    return true;
  }
  // enum e.g. Color.Red
  if (ts.isPropertyAccessExpression(node) && ts.isIdentifier(node.expression) &&
    ts.isIdentifier(node.name)) {
    if (enumCollection.has(node.expression.escapedText.toString())) {
      return true;
    }
    if (globalProgram.checker) {
      const type: ts.Type = globalProgram.checker.getTypeAtLocation(node);
      /* Enum */
      if (type.flags & (32 | 1024)) {
        return true;
      }
    }
    return false;
  }
  // regular variable, e.g. this.regularValue
  const result: AttrResult = { isRegularNode: false };
  if (ts.isPropertyAccessExpression(node)) {
    traversePropNode(node, result);
  }
  return result.isRegularNode || false;
}

function isLiteralNode(node: ts.Expression): boolean {
  return ts.isStringLiteral(node) || ts.isNumericLiteral(node) || ts.isArrowFunction(node) ||
    [ts.SyntaxKind.TrueKeyword, ts.SyntaxKind.FalseKeyword].includes(node.kind);
}

function isSimpleType(node: ts.PropertyAccessExpression): boolean {
  const symbol: ts.Symbol = getSymbolIfAliased(node);
  const simpleTypeCollection: ts.SyntaxKind[] = [ts.SyntaxKind.StringKeyword, ts.SyntaxKind.NumberKeyword, 
    ts.SyntaxKind.BooleanKeyword];
  if (symbol && symbol.declarations && symbol.declarations.length === 1 && ts.isPropertyDeclaration(symbol.declarations[0]) &&
    symbol.declarations[0].type && symbol.declarations[0].type.kind && simpleTypeCollection.includes(symbol.declarations[0].type.kind)) {
    return true;
  }
  return false;
}

function traversePropNode(node: ts.PropertyAccessExpression, result: AttrResult): void {
  const structInfo: StructInfo = processStructComponentV2.getOrCreateStructInfo(componentCollection.currentClassName);
  if (structInfo.isComponentV2 && node.expression.kind === ts.SyntaxKind.ThisKeyword && ts.isIdentifier(node.name) &&
    structInfo.regularSet.has(node.name.escapedText.toString()) && isSimpleType(node)) {
    result.isRegularNode = true;
    return;
  }
  if (!structInfo.isComponentV2 && node.expression.kind === ts.SyntaxKind.ThisKeyword && ts.isIdentifier(node.name) &&
    regularCollection.get(componentCollection.currentClassName).has(node.name.escapedText.toString())) {
    result.isRegularNode = true;
    return;
  }
  if (ts.isPropertyAccessExpression(node.expression)) {
    traversePropNode(node.expression, result);
  }
}

function isDoubleDollarToChange(isStylesAttr: boolean, identifierNode: ts.Identifier,
  propName: string, temp): boolean {
  return !isStylesAttr &&
    PROPERTIES_ADD_DOUBLE_DOLLAR.has(identifierNode.escapedText.toString()) &&
    PROPERTIES_ADD_DOUBLE_DOLLAR.get(identifierNode.escapedText.toString()).has(propName) ||
    STYLE_ADD_DOUBLE_DOLLAR.has(propName) && temp.arguments.length && temp.arguments[0] ?
    temp.arguments[0].getText().match(/^(?!\$\$\.)\$\$(.|\n)+/) !== null :
    false;
}

function isDoubleExclamationToChange(isStylesAttr: boolean, identifierNode: ts.Identifier, propName: string, temp: ts.CallExpression): boolean {
  return !isStylesAttr &&
    (STYLE_ADD_DOUBLE_EXCLAMATION.has(propName) && temp.arguments.length && temp.arguments[0] || 
    PROPERTIES_ADD_DOUBLE_EXCLAMATION.has(identifierNode.escapedText.toString()) &&
    PROPERTIES_ADD_DOUBLE_EXCLAMATION.get(identifierNode.escapedText.toString()).has(propName)) &&
    ts.isNonNullExpression(temp.arguments[0]) && ts.isNonNullExpression(temp.arguments[0].expression) &&
    !ts.isNonNullExpression(temp.arguments[0].expression.expression);
}

function isHaveDoubleDollar(param: ts.PropertyAssignment, name: string): boolean {
  return ts.isPropertyAssignment(param) && param.name && ts.isIdentifier(param.name) &&
    PROPERTIES_ADD_DOUBLE_DOLLAR.get(name).has(param.name.getText()) && param.initializer &&
    param.initializer.getText().match(/^(?!\$\$\.)\$\$(.|\n)+/) !== null;
}

function isHaveDoubleExclamation(param: ts.PropertyAssignment, name: string): boolean {
  return ts.isPropertyAssignment(param) && param.name && ts.isIdentifier(param.name) &&
    PROPERTIES_ADD_DOUBLE_EXCLAMATION.has(name) && PROPERTIES_ADD_DOUBLE_EXCLAMATION.get(name).has(param.name.getText()) &&
    param.initializer && ts.isNonNullExpression(param.initializer) && ts.isNonNullExpression(param.initializer.expression) &&
    !ts.isNonNullExpression(param.initializer.expression.expression);
}

function loopEtscomponent(node: any, isStylesAttr: boolean): ts.Node {
  node.arguments.forEach((item: ts.Node, index: number) => {
    if (ts.isEtsComponentExpression(item)) {
      node.arguments[index] = ts.factory.createCallExpression(
        item.expression, undefined, item.arguments);
    } else if ((ts.isCallExpression(item) || ts.isNewExpression(item)) &&
      !newsupplement.isAcceleratePreview) {
      node.arguments[index] = ts.visitEachChild(item,
        changeEtsComponentKind, contextGlobal);
    }
  });
  return node;
}

function changeEtsComponentKind(node: ts.Node): ts.Node {
  if (ts.isEtsComponentExpression(node)) {
    node.kind = 204;
    return node;
  }
  return ts.visitEachChild(node, changeEtsComponentKind, contextGlobal);
}

function classifyArgumentsNum(args, argumentsArr: ts.Expression[], propName: string,
  identifierNode: ts.Identifier): void {
  if (STYLE_ADD_DOUBLE_DOLLAR.has(propName) && args.length >= 2) {
    const varExp: ts.Expression = updateArgumentForDollar(args[0]);
    argumentsArr.push(generateObjectForDollar(varExp), ...args.slice(1));
  } else if (STYLE_ADD_DOUBLE_EXCLAMATION.has(propName) && args.length >= 2) {
    const varExp: ts.Expression = updateArgumentForExclamation(args[0]);
    argumentsArr.push(generateObjectForExclamation(varExp), ...args.slice(1));
  } else if (PROPERTIES_ADD_DOUBLE_DOLLAR.has(identifierNode.escapedText.toString()) && args.length === 1 &&
    PROPERTIES_ADD_DOUBLE_DOLLAR.get(identifierNode.escapedText.toString()).has(propName) ||
    STYLE_ADD_DOUBLE_DOLLAR.has(propName) && args.length === 1) {
    const varExp: ts.Expression = updateArgumentForDollar(args[0]);
    argumentsArr.push(varExp, createArrowFunctionForDollar(varExp));
  }
}

function classifyArgumentsNumV2(args: any, argumentsArr: ts.Expression[], propName: string, 
  identifierNode: ts.Identifier): void {
  const componentName: string = identifierNode.escapedText.toString();
  if (STYLE_ADD_DOUBLE_EXCLAMATION.has(propName) && args.length || 
  PROPERTIES_ADD_DOUBLE_EXCLAMATION.has(componentName) && args.length === 1 &&
  PROPERTIES_ADD_DOUBLE_EXCLAMATION.get(componentName).has(propName)) {
    const varExp: ts.Expression = updateArgumentForExclamation(args[0]);
    argumentsArr.push(generateObjectForExclamation(varExp), ...args.slice(1));
  }
}

function generateObjectForExclamation(varExp: ts.Expression): ts.ObjectLiteralExpression {
  return ts.factory.createObjectLiteralExpression([
      ts.factory.createPropertyAssignment(
        ts.factory.createIdentifier($$_VALUE),
        varExp),
      ts.factory.createPropertyAssignment(
        ts.factory.createIdentifier($_VALUE),
        ts.factory.createArrowFunction(
          undefined, undefined,
          [ts.factory.createParameterDeclaration(
            undefined, undefined,
            ts.factory.createIdentifier($$_NEW_VALUE),
            undefined, undefined, undefined
          )],
          undefined,
          ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
          ts.factory.createBlock(
            [ts.factory.createExpressionStatement(ts.factory.createBinaryExpression(
              varExp,
              ts.factory.createToken(ts.SyntaxKind.EqualsToken),
              ts.factory.createIdentifier($$_NEW_VALUE)
            ))], false)
        ))], false);
}

function generateObjectForDollar(varExp: ts.Expression): ts.ObjectLiteralExpression {
  return ts.factory.createObjectLiteralExpression(
    [
      ts.factory.createPropertyAssignment(
        ts.factory.createIdentifier($$_VALUE),
        varExp
      ),
      ts.factory.createPropertyAssignment(
        ts.factory.createIdentifier($$_CHANGE_EVENT),
        createArrowFunctionForDollar(varExp)
      )
    ],
    false
  );
}

function generateFunctionPropertyAssignmentForExclamation(name: string, varExp: ts.Expression): ts.PropertyAssignment {
  return ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier('$' + name),
    createArrowFunctionForDollar(varExp)
  );
}

function createViewStackProcessor(item, endViewStack: boolean): ts.ExpressionStatement {
  const argument: ts.StringLiteral[] = [];
  if (!endViewStack && item.name) {
    argument.push(ts.factory.createStringLiteral(item.name.getText()));
  }
  return ts.factory.createExpressionStatement(ts.factory.createCallExpression(
    ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(VIEW_STACK_PROCESSOR),
      ts.factory.createIdentifier(VISUAL_STATE)
    ),
    undefined,
    argument
  ));
}

function traverseStateStylesAttr(temp, statements: ts.Statement[],
  identifierNode: ts.Identifier, log: LogInfo[], updateStatements: ts.Statement[],
  newImmutableStatements: ts.Statement[] = null, isRecycleComponent: boolean = false): void {
  temp.arguments[0].properties.reverse().forEach((item: ts.PropertyAssignment) => {
    if (ts.isPropertyAccessExpression(item.initializer) &&
      item.initializer.expression.getText() === THIS &&
      INNER_STYLE_FUNCTION.get(item.initializer.name.getText())) {
      const name: string = item.initializer.name.getText();
      bindComponentAttr(INNER_STYLE_FUNCTION.get(name).statements[0] as ts.ExpressionStatement,
        identifierNode, statements, log, false, true, newImmutableStatements);
      if (isRecycleComponent) {
        bindComponentAttr(INNER_STYLE_FUNCTION.get(name).statements[0] as ts.ExpressionStatement,
          identifierNode, updateStatements, log, false, true, newImmutableStatements);
      }
    } else if (ts.isIdentifier(item.initializer) &&
      GLOBAL_STYLE_FUNCTION.get(item.initializer.getText())) {
      const name: string = item.initializer.getText();
      bindComponentAttr(GLOBAL_STYLE_FUNCTION.get(name).statements[0] as ts.ExpressionStatement,
        identifierNode, statements, log, false, true, newImmutableStatements);
      if (isRecycleComponent) {
        bindComponentAttr(GLOBAL_STYLE_FUNCTION.get(name).statements[0] as ts.ExpressionStatement,
          identifierNode, updateStatements, log, false, true, newImmutableStatements);
      }
    } else if (ts.isObjectLiteralExpression(item.initializer) &&
      item.initializer.properties.length === 1 &&
      ts.isPropertyAssignment(item.initializer.properties[0])) {
      bindComponentAttr(ts.factory.createExpressionStatement(
        item.initializer.properties[0].initializer), identifierNode, statements, log, false, true,
      newImmutableStatements);
      if (isRecycleComponent) {
        bindComponentAttr(ts.factory.createExpressionStatement(
          item.initializer.properties[0].initializer), identifierNode, updateStatements, log, false, true,
        newImmutableStatements);
      }
    } else {
      if (!(ts.isObjectLiteralExpression(item.initializer) && item.initializer.properties.length === 0)) {
        validateStateStyleSyntax(temp, log);
      }
    }
    if (item.name) {
      const viewNode: ts.Statement = createViewStackProcessor(item, false);
      statements.push(viewNode);
      if (isRecycleComponent) {
        updateStatements.push(viewNode);
      }
    }
  });
}

interface ExtendType {
  type: string
}

function isExtendFunctionNode(identifierNode: ts.Identifier, propName: string,
  extendType: ExtendType): boolean {
  const componentName: string = identifierNode.escapedText.toString();
  if (EXTEND_ATTRIBUTE.has(componentName) && [...EXTEND_ATTRIBUTE.get(componentName)].includes(propName)) {
    extendType.type = CHECK_COMPONENT_EXTEND_DECORATOR;
    return true;
  }
  const animatableExtendAttribute: Map<string, Set<string>> =
    storedFileInfo.getCurrentArkTsFile().animatableExtendAttribute;
  if (animatableExtendAttribute.has(componentName) &&
    [...animatableExtendAttribute.get(componentName)].includes(propName)) {
    extendType.type = CHECK_COMPONENT_ANIMATABLE_EXTEND_DECORATOR;
    return true;
  }
  return false;
}

const gestureMap: Map<string, string> = new Map([
  [PRIORITY_GESTURE_ATTRIBUTE, GESTURE_ENUM_VALUE_HIGH],
  [PARALLEL_GESTURE_ATTRIBUTE, GESTURE_ENUM_VALUE_PARALLEL],
  [GESTURE_ATTRIBUTE, GESTURE_ENUM_VALUE_LOW]
]);

function parseGesture(node: ts.CallExpression, propName: string, statements: ts.Statement[],
  log: LogInfo[], updateStatements: ts.Statement[]): void {
  const popNode: ts.Statement = ts.factory.createExpressionStatement(
    createFunction(ts.factory.createIdentifier(COMPONENT_GESTURE),
      ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null));
  statements.push(popNode);
  updateStatements.push(popNode);
  parseGestureInterface(node, statements, log, updateStatements);
  const argumentArr: ts.NodeArray<ts.PropertyAccessExpression> = ts.factory.createNodeArray(
    [ts.factory.createPropertyAccessExpression(
      ts.factory.createIdentifier(GESTURE_ENUM_KEY),
      ts.factory.createIdentifier(gestureMap.get(propName)))
    ]
  );
  if (node.arguments && node.arguments.length > 1 &&
    ts.isPropertyAccessExpression(node.arguments[1])) {
    // @ts-ignore
    argumentArr.push(node.arguments[1]);
  }
  const createNode: ts.Statement = ts.factory.createExpressionStatement(
    createFunction(ts.factory.createIdentifier(COMPONENT_GESTURE),
      ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), argumentArr));
  statements.push(createNode);
  updateStatements.push(createNode);
}

function processGestureType(node: ts.CallExpression, statements: ts.Statement[], log: LogInfo[],
  updateStatements: ts.Statement[], reverse: boolean = false): void {
  const newStatements: ts.Statement[] = [];
  const newNode: ts.ExpressionStatement = ts.factory.createExpressionStatement(node);
  let temp = node.expression;
  while (temp && !ts.isIdentifier(temp) && temp.expression) {
    temp = temp.expression;
  }
  if (temp && temp.parent && ts.isCallExpression(temp.parent) && ts.isIdentifier(temp) &&
    GESTURE_TYPE_NAMES.has(temp.escapedText.toString())) {
    newStatements.push(ts.factory.createExpressionStatement(
      createFunction(temp, ts.factory.createIdentifier(COMPONENT_POP_FUNCTION), null)));
    if (temp.escapedText.toString() === COMPONENT_GESTURE_GROUP) {
      const gestureStatements: ts.Statement[] = [];
      parseGestureInterface(temp.parent, gestureStatements, log, [], true);
      newStatements.push(...gestureStatements.reverse());
      bindComponentAttr(newNode, temp, newStatements, log, false);
      let argumentArr: ts.NodeArray<ts.Expression> = null;
      if (temp.parent.arguments && temp.parent.arguments.length) {
        // @ts-ignore
        argumentArr = ts.factory.createNodeArray([temp.parent.arguments[0]]);
      }
      newStatements.push(ts.factory.createExpressionStatement(
        createFunction(temp, ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), argumentArr)));
    } else {
      bindComponentAttr(newNode, temp, newStatements, log, false);
      newStatements.push(ts.factory.createExpressionStatement(
        createFunction(temp, ts.factory.createIdentifier(COMPONENT_CREATE_FUNCTION), temp.parent.arguments)));
    }
  }
  if (newStatements.length) {
    reverse ? statements.push(...newStatements.reverse()) : statements.push(...newStatements);
    reverse ? updateStatements.push(...newStatements.reverse()) : updateStatements.push(...newStatements);
  }
}

function parseGestureInterface(node: ts.CallExpression, statements: ts.Statement[], log: LogInfo[],
  updateStatements: ts.Statement[], reverse: boolean = false): void {
  if (node.arguments && node.arguments.length) {
    node.arguments.forEach((item: ts.Node) => {
      if (ts.isCallExpression(item)) {
        processGestureType(item, statements, log, updateStatements, reverse);
      }
    });
  }
}

export function getName(node: ts.ExpressionStatement | ts.Expression): string {
  // @ts-ignore
  let temp = node.expression;
  let name: string;
  while (temp) {
    if (ts.isIdentifier(temp) && temp.parent && (ts.isCallExpression(temp.parent) ||
      ts.isEtsComponentExpression(temp.parent))) {
      name = temp.escapedText.toString();
      break;
    } else if (ts.isPropertyAccessExpression(temp) && temp.name && ts.isIdentifier(temp.name) &&
      isCustomAttributes(temp)) {
      name = temp.name.escapedText.toString();
      break;
    }
    temp = temp.expression;
  }
  return name;
}

function isCustomAttributes(temp: ts.PropertyAccessExpression): boolean {
  if (temp.expression && temp.expression.getText() === THIS) {
    return true;
  } else if (temp.expression && ts.isIdentifier(temp.expression) && temp.expression.getText() === $$ &&
    builderTypeParameter.params.includes(temp.expression.getText())) {
    return true;
  } else {
    return !BUILDIN_STYLE_NAMES.has(temp.name.escapedText.toString());
  }
}

export function isAttributeNode(node: ts.ExpressionStatement): boolean {
  let temp: any = node.expression;
  let name: string;
  while (temp) {
    if (ts.isCallExpression(temp) && temp.expression && ts.isIdentifier(temp.expression)) {
      name = temp.expression.escapedText.toString();
      break;
    }
    temp = temp.expression;
  }
  return BUILDIN_STYLE_NAMES.has(name);
}

enum ComponentType {
  innerComponent,
  customComponent,
  forEachComponent,
  customBuilderMethod,
  builderParamMethod,
  function,
  builderTypeFunction,
  repeatComponent
}

function isEtsComponent(node: ts.ExpressionStatement): boolean {
  let isEtsComponent: boolean = false;
  let temp: any = node.expression;
  while (temp) {
    if (ts.isEtsComponentExpression(temp)) {
      isEtsComponent = true;
    }
    temp = temp.expression;
  }
  return isEtsComponent;
}

function isSomeName(forEachParameters: ts.NodeArray<ts.ParameterDeclaration>, name: string): boolean {
  return Array.isArray(forEachParameters) &&
    forEachParameters.some((item) => {
      return ts.isIdentifier(item.name) ? item.name.escapedText.toString() === name : false;
    });
}

function isParamFunction(node: ts.ExpressionStatement): boolean {
  return node.expression && ts.isCallExpression(node.expression) &&
    node.expression.expression && ts.isIdentifier(node.expression.expression);
}

function getComponentType(node: ts.ExpressionStatement, log: LogInfo[], name: string,
  parent: string, forEachParameters: ts.NodeArray<ts.ParameterDeclaration> = undefined): ComponentType {
  let isBuilderName: boolean = true;
  let isLocalBuilderName: boolean = true;
  if (forEachParameters && isSomeName(forEachParameters, name) && isParamFunction(node)) {
    isBuilderName = false;
    isLocalBuilderName = false;
  }
  if (isEtsComponent(node)) {
    if (componentCollection.customComponents.has(name)) {
      isCustomComponentAttributes(node, log);
      return ComponentType.customComponent;
    } else {
      return ComponentType.innerComponent;
    }
  } else if (!isPartMethod(node) && componentCollection.customComponents.has(name)) {
    isCustomComponentAttributes(node, log);
    return ComponentType.customComponent;
  } else if (name === COMPONENT_FOREACH || name === COMPONENT_LAZYFOREACH) {
    return ComponentType.forEachComponent;
  } else if (name === COMPONENT_REPEAT) {
    return ComponentType.repeatComponent;
  } else if (isLocalBuilderOrBuilderMethod(CUSTOM_BUILDER_METHOD, isBuilderName, name) || isWrappedBuilderExpression(node) ||
    isLocalBuilderOrBuilderMethod(INNER_CUSTOM_LOCALBUILDER_METHOD, isLocalBuilderName, name)) {
    return ComponentType.customBuilderMethod;
  } else if (builderParamObjectCollection.get(componentCollection.currentClassName) &&
    builderParamObjectCollection.get(componentCollection.currentClassName).has(name)) {
    return ComponentType.builderParamMethod;
  } else if (!partialUpdateConfig.builderCheck && builderTypeParameter.params.includes(name) &&
    judgeBuilderType(node)) {
    return ComponentType.builderTypeFunction;
  } else if ((['XComponent'].includes(parent) || CUSTOM_BUILDER_METHOD.has(parent)) &&
    ts.isCallExpression(node.expression) && ts.isIdentifier(node.expression.expression)) {
    return ComponentType.function;
  } else if (!isAttributeNode(node)) {
    log.push({
      type: LogType.ERROR,
      message: `'${node.getText()}' does not meet UI component syntax.`,
      pos: node.getStart(),
      code: '10905204'
    });
  }
  return null;
}

function isCustomComponentAttributes(node: ts.ExpressionStatement, log: LogInfo[]): void {
  if (node.expression && ts.isCallExpression(node.expression) && ts.isPropertyAccessExpression(node.expression.expression) &&
    !ts.isIdentifier(node.expression.expression.expression) && ts.isIdentifier(node.expression.expression.name) &&
    !COMMON_ATTRS.has(node.expression.expression.name.escapedText.toString())) {
      log.push({
        type: LogType.ERROR,
        message: `'${node.getText()}' does not meet UI component syntax.`,
        pos: node.getStart()
      });
  }
}

function isLocalBuilderOrBuilderMethod(LocalBuilderOrBuilderSet: Set<string>,
  isLocalBuilderOrBuilderName: boolean, name: string): boolean {
  return LocalBuilderOrBuilderSet.has(name) && isLocalBuilderOrBuilderName;
}

function isPartMethod(node: ts.ExpressionStatement): boolean {
  if (ts.isCallExpression(node.expression) && ts.isPropertyAccessExpression(node.expression.expression) &&
    node.expression.expression.expression && node.expression.expression.expression.kind &&
    node.expression.expression.expression.kind === ts.SyntaxKind.ThisKeyword) {
    return true;
  } else {
    return false;
  }
}

function isWrappedBuilderExpression(node: ts.ExpressionStatement): boolean {
  if (projectConfig.minAPIVersion >= 11 && node.expression &&
    isWrappedBuilderCallExpression(node.expression as ts.CallExpression)) {
    return true;
  }
  return false;
}

function judgeBuilderType(node: ts.ExpressionStatement): boolean {
  let checker: ts.TypeChecker;
  if (globalProgram.program) {
    checker = globalProgram.program.getTypeChecker();
  } else if (globalProgram.watchProgram) {
    checker = globalProgram.watchProgram.getCurrentProgram().getProgram().getTypeChecker();
  }
  if (node.expression && node.expression.expression && checker) {
    const type: ts.Type = checker.getTypeAtLocation(node.expression.expression);
    if (type && type.aliasSymbol && type.aliasSymbol.escapedName === BUILDER_TYPE) {
      return true;
    }
  }
  return false;
}

export function validateStateStyleSyntax(temp, log: LogInfo[]): void {
  log.push({
    type: LogType.ERROR,
    message: `'.stateStyles' doesn't conform standard.`,
    pos: temp.getStart(),
    code: '10905203'
  });
}

function getEtsComponentExpression(node:ts.ExpressionStatement): ts.EtsComponentExpression {
  let current = node.expression;
  while (current) {
    if (ts.isEtsComponentExpression(current)) {
      return current;
    }
    current = current.expression;
  }
  return null;
}

function checkEtsAndIdInIf(node:ts.ExpressionStatement, parent: string): [ts.EtsComponentExpression, ts.Expression] {
  let current = node.expression;
  let idName: ts.Expression;
  while (current) {
    if (ts.isEtsComponentExpression(current)) {
      break;
    }
    if (!idName && parent === COMPONENT_IF && ts.isPropertyAccessExpression(current) && current.name &&
      ts.isIdentifier(current.name) && current.name.escapedText.toString() === ATTRIBUTE_ID &&
      current.parent && current.parent.arguments && current.parent.arguments.length) {
      idName = current.parent.arguments[0];
    }
    current = current.expression;
  }
  return [current, idName];
}

function checkIdInIf(node:ts.ExpressionStatement, parent: string): ts.Expression {
  let current: any = node.expression;
  let idName: ts.Expression;
  while (current) {
    if (parent === COMPONENT_IF && ts.isPropertyAccessExpression(current) && current.name &&
      ts.isIdentifier(current.name) && current.name.escapedText.toString() === ATTRIBUTE_ID &&
      current.parent && current.parent.arguments && current.parent.arguments.length) {
      idName = current.parent.arguments[0];
      break;
    }
    current = current.expression;
  }
  return idName;
}

function checkEtsComponent(node: ts.ExpressionStatement, log: LogInfo[]): void {
  const etsComponentExpression: ts.EtsComponentExpression = getEtsComponentExpression(node);
  if (etsComponentExpression) {
    checkAllNode(
      etsComponentExpression,
      new Set([...INNER_COMPONENT_NAMES, ...componentCollection.customComponents]),
      transformLog.sourceFile,
      log
    );
  }
}

function checkButtonParamHasLabel(node: ts.EtsComponentExpression, log: LogInfo[]): void {
  if (node.arguments && node.arguments.length !== 0) {
    for (let i = 0; i < node.arguments.length; i++) {
      const argument: ts.Expression = node.arguments[i];
      if (ts.isStringLiteral(argument) || (ts.isCallExpression(argument) && ts.isIdentifier(argument.expression) &&
        (argument.expression.escapedText.toString() === RESOURCE))) {
        log.push({
          type: LogType.ERROR,
          message: 'The Button component with a label parameter can not have any child.',
          pos: node.getStart(),
          code: '10905202'
        });
        return;
      }
    }
  }
}

function isLazyForEachChild(node: ts.ExpressionStatement): boolean {
  let temp = node.parent;
  while (temp && !ts.isEtsComponentExpression(temp) && !ts.isCallExpression(temp)) {
    temp = temp.parent;
  }
  if (temp && temp.expression && (temp.expression as ts.Identifier).escapedText?.toString() === COMPONENT_LAZYFOREACH) {
    return true;
  }
  return false;
}

function processDollarEtsComponent(argumentsArr: ts.NodeArray<ts.Expression>, name: string): ts.Expression[] {
  const arr: ts.Expression[] = [];
  argumentsArr.forEach((item: ts.Expression, index: number) => {
    if (ts.isObjectLiteralExpression(item) && item.properties && item.properties.length) {
      const properties: ts.PropertyAssignment[] = [];
      item.properties.forEach((param: ts.PropertyAssignment, paramIndex: number) => {
        if (isHaveDoubleDollar(param, name)) {
          const varExp: ts.Expression = updateArgumentForDollar(param.initializer);
          properties.push(ts.factory.updatePropertyAssignment(param, param.name, generateObjectForDollar(varExp)));
        } else {
          properties.push(param);
        }
      });
      arr.push(ts.factory.updateObjectLiteralExpression(item, properties));
    } else {
      arr.push(item);
    }
  });
  return arr;
}

function processExclamationEtsComponent(argumentsArr: ts.NodeArray<ts.Expression>, name: string): ts.Expression[] {
  const arr: ts.Expression[] = [];
  argumentsArr.forEach((item: ts.Expression, index: number) => {
    if (ts.isObjectLiteralExpression(item) && item.properties && item.properties.length) {
      const properties: ts.PropertyAssignment[] = [];
      item.properties.forEach((param: ts.PropertyAssignment, paramIndex: number) => {
        if (isHaveDoubleExclamation(param, name) && param.initializer && param.name) {
          const varExp: ts.Expression = updateArgumentForExclamation(param.initializer);
          properties.push(ts.factory.updatePropertyAssignment(param, param.name, varExp));
          properties.push(generateFunctionPropertyAssignmentForExclamation(param.name.getText(), varExp));
        } else {
          properties.push(param);
        }
      });
      arr.push(ts.factory.updateObjectLiteralExpression(item, properties));
    } else {
      arr.push(item);
    }
  });
  return arr;
}

export function createFunction(node: ts.Identifier, attrNode: ts.Identifier,
  argumentsArr: ts.NodeArray<ts.Expression>, isAttributeModifier: boolean = false): ts.CallExpression {
    const compName: string = node.escapedText.toString();
    const type: string = attrNode.escapedText.toString();
  if (argumentsArr && argumentsArr.length) {
    if (type === COMPONENT_CREATE_FUNCTION && PROPERTIES_ADD_DOUBLE_DOLLAR.has(compName)) {
      // @ts-ignore
      argumentsArr = processDollarEtsComponent(argumentsArr, compName);
    }
    if (type === COMPONENT_CREATE_FUNCTION && PROPERTIES_ADD_DOUBLE_EXCLAMATION.has(compName)) {
      argumentsArr = processExclamationEtsComponent(argumentsArr, compName) as unknown as ts.NodeArray<ts.Expression>;
    }
    if (checkCreateArgumentBuilder(node, attrNode)) {
      argumentsArr = transformBuilder(argumentsArr);
    }
    if (((compName === NAVIGATION) || equalToHiddenNav(compName)) &&
      type === COMPONENT_CREATE_FUNCTION && partialUpdateConfig.partialUpdateMode) {
      // @ts-ignore
      argumentsArr = navigationCreateParam(compName, type, argumentsArr);
    }
  } else {
    // @ts-ignore
    argumentsArr = navigationCreateParam(compName, type);
  }
  return ts.factory.createCallExpression(
    isAttributeModifier ? ts.factory.createCallExpression(
      ts.factory.createPropertyAccessExpression(
        ts.factory.createPropertyAccessExpression(
          node,
          attrNode
        ),
        ts.factory.createIdentifier(BUILDER_ATTR_BIND)
      ),
      undefined,
      [ts.factory.createThis()]
    ) : 
      ts.factory.createPropertyAccessExpression(
        node,
        attrNode
      ),
    undefined,
    argumentsArr
  );
}

function navigationCreateParam(compName: string, type: string,
  argumentsArr: ts.NodeArray<ts.Expression> = undefined, isNavDestinationCallback: boolean = false):
  (ts.ObjectLiteralExpression | ts.NewExpression | ts.ArrowFunction)[] | [] {
  const navigationOrNavDestination: (ts.ObjectLiteralExpression | ts.NewExpression | ts.ArrowFunction)[] = [];
  const isCreate: boolean = type === COMPONENT_CREATE_FUNCTION;
  const partialUpdateMode: boolean = partialUpdateConfig.partialUpdateMode;
  let isHaveParam: boolean = true;
  if (argumentsArr && argumentsArr.length) {
    // @ts-ignore
    navigationOrNavDestination.push(...argumentsArr);
  } else if (partialUpdateMode && isCreate) {
    if ((compName === NAVIGATION) || equalToHiddenNav(compName)) {
      isHaveParam = false;
      navigationOrNavDestination.push(ts.factory.createNewExpression(
        ts.factory.createIdentifier(NAV_PATH_STACK), undefined, []
      ));
    } else if (((compName === NAV_DESTINATION) || equalToHiddenNavDes(compName)) &&
      !isNavDestinationCallback) {
      navigationOrNavDestination.push(ts.factory.createArrowFunction(
        undefined, undefined, [], undefined,
        ts.factory.createToken(ts.SyntaxKind.EqualsGreaterThanToken),
        ts.factory.createBlock(
          [],
          false
        )
      ));
    }
  }
  if (CREATE_ROUTER_COMPONENT_COLLECT.has(compName) && isCreate && partialUpdateMode) {
    navigationOrNavDestination.push(ts.factory.createObjectLiteralExpression(
      navigationOrNavDestinationCreateContent(compName, isHaveParam),
      false
    ));
  }
  return navigationOrNavDestination;
}

function navigationOrNavDestinationCreateContent(compName: string, isHaveParam: boolean): ts.PropertyAssignment[] {
  const navigationOrNavDestinationContent: ts.PropertyAssignment[] = [];
  navigationOrNavDestinationContent.push(ts.factory.createPropertyAssignment(
    ts.factory.createIdentifier(RESOURCE_NAME_MODULE),
    ts.factory.createStringLiteral(projectConfig.moduleName || '')
  ),
    ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(PAGE_PATH),
      ts.factory.createStringLiteral(
        projectConfig.compileHar ? '' :
          path.relative(projectConfig.projectRootPath || '', resourceFileName).replace(/\\/g, '/').replace(/\.ets$/, '')
      )
    ));
  if ((compName === NAVIGATION) || equalToHiddenNav(compName)) {
    navigationOrNavDestinationContent.push(ts.factory.createPropertyAssignment(
      ts.factory.createIdentifier(IS_USER_CREATE_STACK),
      isHaveParam ? ts.factory.createTrue() : ts.factory.createFalse()
    ));
  }
  return navigationOrNavDestinationContent;
}

function checkCreateArgumentBuilder(node: ts.Identifier, attrNode: ts.Identifier): boolean {
  if (attrNode.escapedText.toString() === COMPONENT_CREATE_FUNCTION &&
    CREATE_BIND_COMPONENT.has(node.escapedText.toString())) {
    return true;
  }
  return false;
}

function transformBuilder(argumentsArr: ts.NodeArray<ts.Expression>): ts.NodeArray<ts.Expression> {
  const newArguments: ts.Expression[] = [];
  argumentsArr.forEach((argument: ts.Expression) => {
    newArguments.push(parseCreateParameterBuilder(argument));
  });
  // @ts-ignore
  return newArguments;
}

function parseCreateParameterBuilder(argument: ts.Expression):ts.Expression {
  if (ts.isObjectLiteralExpression(argument)) {
    return processObjectPropertyBuilder(argument);
  } else {
    return argument;
  }
}

function checkNonspecificParents(node: ts.ExpressionStatement, name: string, savedParent: string, log: LogInfo[]): void {
  if (SPECIFIC_PARENT_COMPONENT.has(name)) {
    const specificParemtsSet: Set<string> = SPECIFIC_PARENT_COMPONENT.get(name);
    if (!specificParemtsSet.has(savedParent) && INNER_COMPONENT_NAMES.has(savedParent)) {
      const specificParentArray: string =
        Array.from(SPECIFIC_PARENT_COMPONENT.get(name)).join(',');
      log.push({
        type: LogType.ERROR,
        message: `The '${name}' component can only be nested in the '${specificParentArray}' parent component.`,
        pos: node.expression.getStart(),
        code: '10905201'
      });
    }
  }
}

function equalToHiddenNav(componentName: string): boolean {
  return (EXT_WHITE_LIST.length >= 2) && (componentName === EXT_WHITE_LIST[0]);
}

function equalToHiddenNavDes(componentName: string): boolean {
  return (EXT_WHITE_LIST.length >= 2) && (componentName === EXT_WHITE_LIST[1]);
}