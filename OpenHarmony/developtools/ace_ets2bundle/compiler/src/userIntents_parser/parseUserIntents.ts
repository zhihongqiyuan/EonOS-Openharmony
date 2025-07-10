/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

import ts, { CatchClause, Declaration, Expression, TypeChecker, VariableDeclarationList } from 'typescript';
import {
  IntentEntryInfo,
  intentEntryInfoChecker,
  IntentLinkInfo,
  IntentLinkInfoChecker, intentMethodInfoChecker,
  LinkIntentParamMapping,
  IntentPageInfoChecker,
  ParamChecker
} from './intentType';
import {
  DECORATOR_DUPLICATE_INTENTNAME,
  DECORATOR_ILLEGAL_USE,
  DECORATOR_STATEMENT_ERROR,
  DISALLOWED_PARAM_ERROR,
  DYNAMIC_PARAM_ERROR,
  ENTRYPATH_ERROR,
  INCORRECT_PARAM_TYPE_ERROR,
  IntentLogger,
  INTERNAL_ERROR,
  INVALID_BASE_CLASS_ERROR, INVALID_PAGEPATH_ERROR,
  PARAM_CIRCULAR_REFERENCE_ERROR,
  REQUIRED_PARAM_DISMISS_ERROR,
  SCHEMA_ROOT_TYPE_MISMATCH_ERROR,
  SCHEMA_VALIDATE_ANY_OF_ERROR,
  SCHEMA_VALIDATE_ONE_OF_ERROR,
  SCHEMA_VALIDATE_REQUIRED_ERROR,
  SCHEMA_VALIDATE_TYPE_ERROR,
  UNSUPPORTED_PARSE_ERROR
} from './intentLogger';
import path from 'path';
import { getNormalizedOhmUrlByFilepath } from '../ark_utils';
import { globalModulePaths, projectConfig } from '../../main';
import fs from 'fs';
import { ProjectCollections } from 'arkguard';
import {
  COMPONENT_USER_INTENTS_DECORATOR,
  COMPONENT_USER_INTENTS_DECORATOR_ENTRY,
  COMPONENT_USER_INTENTS_DECORATOR_FUNCTION,
  COMPONENT_USER_INTENTS_DECORATOR_METHOD,
  COMPONENT_USER_INTENTS_DECORATOR_PAGE
} from '../pre_define';
import {
  CompileEvent,
  createAndStartEvent,
  stopEvent
} from '../performance';

type StaticValue = string | number | boolean | null | undefined | StaticValue[] | { [key: string]: StaticValue };

interface methodParametersInfo {
  functionName: string;
  parameters: Record<string, string>,
  args: ts.NodeArray<ts.Expression>;
}

class ParseIntent {
  constructor() {
    this.intentData = [];
    this.currentFilePath = '';
    this.heritageClassSet = new Set<string>();
    this.heritageClassSet.add('InsightIntentEntryExecutor_sdk');
    this.updatePageIntentObj = new Map();
  }

  checker: ts.TypeChecker;
  intentData: object[];
  currentFilePath: string;
  heritageClassSet: Set<string>;
  updatePageIntentObj: Map<string, object[]>;
  isUpdateCompile: boolean = false;
  isInitCache : boolean = false;

  hasDecorator(node: ts.Node, decorators: string[]): boolean {
    if (!node.modifiers) {
      return false;
    }
    return node.modifiers.some(decorator => {
      if (!ts.isDecorator(decorator)) {
        return false;
      }
      let decoratorName: string | undefined;
      if (ts.isCallExpression(decorator.expression)) {
        decoratorName = '@' + decorator.expression.expression.getText();
      }
      return decoratorName !== undefined && decorators.includes(decoratorName);
    });
  }

  detectInsightIntent(node: ts.ClassDeclaration, metaInfo: object, filePath: string, eventOrEventFactory: CompileEvent | undefined): ts.Node {
    const eventParseIntentTime: CompileEvent | undefined = createAndStartEvent(eventOrEventFactory, 'parseIntentTime');
    if (!this.isInitCache && projectConfig.cachePath) {
      const cacheSourceMapPath: string = path.join(projectConfig.cachePath, 'insight_compile_cache.json');
      this.isUpdateCompile = fs.existsSync(cacheSourceMapPath);
      this.isInitCache = true;
    }
    if (this.isUpdateCompile) {
      const pkgParams: object = {
        pkgName: metaInfo.pkgName,
        pkgPath: metaInfo.pkgPath
      };
      const Logger: IntentLogger = IntentLogger.getInstance();
      const recordName: string = getNormalizedOhmUrlByFilepath(filePath, projectConfig, Logger, pkgParams, null);
      if (!this.updatePageIntentObj.has(`@normalized:${recordName}`)) {
        this.updatePageIntentObj.set(`@normalized:${recordName}`, []);
      }
    }
    const definedDecorators: string[] = [COMPONENT_USER_INTENTS_DECORATOR, COMPONENT_USER_INTENTS_DECORATOR_ENTRY,
      COMPONENT_USER_INTENTS_DECORATOR_FUNCTION, COMPONENT_USER_INTENTS_DECORATOR_PAGE];
    if (ts.isClassDeclaration(node) && !this.hasDecorator(node, [COMPONENT_USER_INTENTS_DECORATOR_FUNCTION])) {
      node.members.forEach((member) => {
        if (ts.isMethodDeclaration(member) && this.hasModifier(member, ts.SyntaxKind.StaticKeyword) &&
          this.hasDecorator(member, [COMPONENT_USER_INTENTS_DECORATOR_METHOD])) {
          throw Error(`${DECORATOR_ILLEGAL_USE.toString()}, className: ${node.name.getText()}`);
        }
      });
    }
    if (this.hasDecorator(node, definedDecorators)) {
      const checker: TypeChecker = metaInfo.checker;
      this.handleIntent(node, checker, filePath, metaInfo);
      node = this.removeDecorator(node, definedDecorators.concat(COMPONENT_USER_INTENTS_DECORATOR_METHOD));
    }
    stopEvent(eventParseIntentTime);
    return node;
  }

  handleIntent(node: ts.ClassDeclaration, checker: ts.TypeChecker, filepath: string, metaInfo: Object = {}): void {
    this.checker = checker;
    this.currentFilePath = filepath;
    if (!filepath.endsWith('.ets')) {
      throw Error(`${ENTRYPATH_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
    const pkgParams: object = {
      pkgName: metaInfo.pkgName,
      pkgPath: metaInfo.pkgPath
    };
    node.modifiers.forEach(decorator => {
      const expr: ts.Expression = decorator.expression;
      if (!expr || !ts.isCallExpression(expr)) {
        return;
      }
      const argumentKind: ts.SyntaxKin | undefined = expr.arguments[0]?.kind;
      if (argumentKind && argumentKind === ts.SyntaxKind.NullKeyword || argumentKind && argumentKind === ts.SyntaxKind.UndefinedKeyword) {
        return;
      }
      const symbol: ts.Symbol = checker.getTypeAtLocation(decorator.expression.expression)?.getSymbol();
      const declarations: ts.Declaration[] | undefined = symbol?.getDeclarations();
      if (!declarations || declarations.length === 0) {
        return;
      }
      const decoratorSourceFile: string = declarations[0].getSourceFile().fileName;
      const isGlobalPath: boolean = globalModulePaths?.some(globalPath => {
        const normalizedParent: string = path.normalize(decoratorSourceFile).replace(/\\/g, '/');
        const normalizedGlobalPath: string = path.normalize(globalPath).replace(/\\/g, '/');
        return normalizedParent.startsWith(normalizedGlobalPath);
      });
      if (!isGlobalPath) {
        return;
      }
      const Logger: IntentLogger = IntentLogger.getInstance();
      const recordName: string = getNormalizedOhmUrlByFilepath(filepath, projectConfig, Logger, pkgParams, null);
      const intentObj: object = {
        'decoratorFile': `@normalized:${recordName}`,
        'decoratorClass': node.name.text
      };
      const originalDecorator: string = '@' + decorator.expression.expression.getText();
      if (originalDecorator === COMPONENT_USER_INTENTS_DECORATOR) {
        this.handleLinkDecorator(intentObj, node, decorator);
      } else if (originalDecorator === COMPONENT_USER_INTENTS_DECORATOR_ENTRY) {
        this.handleEntryDecorator(intentObj, node, decorator, pkgParams);
      } else if (originalDecorator === COMPONENT_USER_INTENTS_DECORATOR_FUNCTION) {
        this.handleMethodDecorator(intentObj, node, decorator);
      } else if (originalDecorator === COMPONENT_USER_INTENTS_DECORATOR_PAGE) {
        this.handlePageDecorator(intentObj, node, decorator, pkgParams);
      }
    });
  }

  handlePageDecorator(intentObj: object, node: ts.Node, decorator: ts.Decorator, pkgParams: object): void {
    const expr: ts.Expression = decorator.expression;
    if (ts.isCallExpression(expr)) {
      const args: ts.NodeArray<ts.Expression> = expr.arguments;
      Object.assign(intentObj, {
        'bundleName': projectConfig.bundleName,
        'moduleName': projectConfig.moduleName,
        'decoratorType': COMPONENT_USER_INTENTS_DECORATOR_PAGE
      });
      this.analyzeDecoratorArgs(args, intentObj, IntentPageInfoChecker);
      this.createObfuscation(node);
      this.transformPagePath(intentObj, pkgParams);
      if (this.isUpdateCompile) {
        this.updatePageIntentObj.get(intentObj.decoratorFile).push(intentObj);
      }
      this.intentData.push(intentObj);
    } else {
      throw Error(`${DECORATOR_STATEMENT_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  handleMethodDecorator(intentObj: object, node: ts.Node, decorator: ts.Decorator): void {
    const expr: ts.Expression = decorator.expression;
    if (ts.isCallExpression(expr)) {
      Object.assign(intentObj, {
        'bundleName': projectConfig.bundleName,
        'moduleName': projectConfig.moduleName,
        'decoratorType': COMPONENT_USER_INTENTS_DECORATOR_METHOD,
        'intentType': 'function'
      });

      interface methodParametersInfo {
        functionName: string,
        parameters: Record<string, string>,
        args: ts.NodeArray<ts.Expression>,
      }

      const methodParameters: methodParametersInfo[] = this.parseClassMethods(node, COMPONENT_USER_INTENTS_DECORATOR_METHOD);
      methodParameters.forEach(methodDecorator => {
        const functionName: string = methodDecorator.functionName;
        const methodArgs: ts.NodeArray<ts.Expression> = methodDecorator.args;
        const properties: Record<string, string> = methodDecorator.parameters;
        const functionParamList: Array<string> = Object.keys(properties);
        const methodObj: object = Object.assign({}, intentObj, {functionName, 'functionParamList': functionParamList});
        this.analyzeDecoratorArgs(methodArgs, methodObj, intentMethodInfoChecker);
        if (this.isUpdateCompile) {
          this.updatePageIntentObj.get(methodObj.decoratorFile).push(methodObj);
        }
        this.intentData.push(methodObj);
      });
      this.createObfuscation(node);
    } else {
      throw Error(`${DECORATOR_STATEMENT_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  handleLinkDecorator(intentObj: object, node: ts.Node, decorator: ts.Decorator): void {
    const expr: ts.Expression = decorator.expression;
    if (ts.isCallExpression(expr)) {
      const args: ts.NodeArray<ts.Expression> = expr.arguments;
      this.analyzeDecoratorArgs<IntentLinkInfo>(args, intentObj, IntentLinkInfoChecker);
      Object.assign(intentObj, {
        'bundleName': projectConfig.bundleName,
        'moduleName': projectConfig.moduleName,
        'decoratorType': COMPONENT_USER_INTENTS_DECORATOR
      });
      this.createObfuscation(node);
      if (this.isUpdateCompile) {
        this.updatePageIntentObj.get(intentObj.decoratorFile).push(intentObj);
      }
      this.intentData.push(intentObj);
    } else {
      throw Error(`${DECORATOR_STATEMENT_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  handleEntryDecorator(intentObj: object, node: ts.Node, decorator: ts.Decorator, pkgParams: object): void {
    const expr: ts.Expression = decorator.expression;
    if (ts.isCallExpression(expr)) {
      const args: ts.NodeArray<ts.Expression> = expr.arguments;
      Object.assign(intentObj, {
        'bundleName': projectConfig.bundleName,
        'moduleName': projectConfig.moduleName,
        'decoratorType': COMPONENT_USER_INTENTS_DECORATOR_ENTRY
      });
      this.analyzeDecoratorArgs<IntentEntryInfo>(args, intentObj, intentEntryInfoChecker);
      const properties: Record<string, string> = this.parseClassNode(node);
      this.schemaValidateSync(properties, intentObj.parameters);
      this.analyzeBaseClass(node, pkgParams, intentObj);
      this.createObfuscation(node);
      this.processExecuteModeParam(intentObj);
      if (this.isUpdateCompile) {
        this.updatePageIntentObj.get(intentObj.decoratorFile).push(intentObj);
      }
      this.intentData.push(intentObj);
    } else {
      throw Error(`${DECORATOR_STATEMENT_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  transformPagePath(intentObj: object, pkgParams: object): void {
    if (pkgParams.pkgPath) {
      const normalPagePath: string = path.join(pkgParams.pkgPath, 'src/main', intentObj.pagePath + '.ets');
      if (!fs.existsSync(normalPagePath)) {
        throw Error(`${INVALID_PAGEPATH_ERROR.toString()}, ${normalPagePath} does not exist, invalidDecoratorPath: ${this.currentFilePath}`);
      } else {
        const Logger: IntentLogger = IntentLogger.getInstance();
        intentObj.pagePath = '@normalized:' + getNormalizedOhmUrlByFilepath(normalPagePath, projectConfig, Logger, pkgParams, null);
      }
    }
  }

  analyzeBaseClass(node: ts.ClassDeclaration, pkgParams: object, intentObj: object): void {
    const interfaces: ts.ExpressionWithTypeArguments[] = [];
    node.heritageClauses?.forEach(clause => {
      if (clause.token === ts.SyntaxKind.ImplementsKeyword || clause.token === ts.SyntaxKind.ExtendsKeyword) {
        interfaces.push(...clause.types);
      }
    });
    if (interfaces.length > 0) {
      const parentNode: ts.ExpressionWithTypeArguments = interfaces[0];
      if (parentNode) {
        this.analyzeClassHeritage(parentNode, node, pkgParams, intentObj);
      }
    } else {
      throw Error(`${INVALID_BASE_CLASS_ERROR.toString()},
         The custom intent must be ultimately inherited to InsightIntentEntryExecutor, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  hasModifier(node: ts.Node, modifier: ts.SyntaxKind): boolean {
    return (node.modifiers || []).some(m => m.kind === modifier);
  }

  parseClassMethods(classNode: ts.ClassDeclaration, decoratorType: string): methodParametersInfo[] {
    const methodsArr: methodParametersInfo[] = [];
    for (const member of classNode.members) {
      if (!ts.isMethodDeclaration(member) || !this.hasModifier(member, ts.SyntaxKind.StaticKeyword)) {
        continue;
      }
      const decorator: boolean = member.modifiers?.find(m => {
        if (!ts.isDecorator(m)) {
          return false;
        }
        let decoratorName: string | undefined;
        if (ts.isCallExpression(m.expression)) {
          decoratorName = '@' + m.expression.expression.getText();
        }
        return decoratorName === decoratorType;
      });
      if (decorator && ts.isCallExpression(decorator.expression)) {
        const parameters: Record<string, string> = {};
        member.parameters.forEach(param => {
          const paramName: string = param.name.getText();
          parameters[paramName] = this.checker.typeToString(
            this.checker.getTypeAtLocation(param),
            param,
            ts.TypeFormatFlags.NoTruncation
          );
        });
        const obj: methodParametersInfo = {
          functionName: member.name.getText(),
          parameters: parameters,
          args: decorator.expression.arguments
        };
        methodsArr.push(obj);
      }
    }
    return methodsArr;
  }

  analyzeClassHeritage(
    parentNode: ts.ExpressionWithTypeArguments, node: ts.ClassDeclaration, pkgParams: object, intentObj: object
  ): void {
    const parentSymbol: ts.Symbol = this.checker.getTypeAtLocation(parentNode).getSymbol();
    let parentClassName: string;
    node.heritageClauses.forEach(clause => {
      if (clause.token === ts.SyntaxKind.ExtendsKeyword) {
        parentClassName = parentNode.expression.getText();
      } else if (clause.token === ts.SyntaxKind.ImplementsKeyword) {
        parentClassName = parentSymbol.getName();
      }
    });
    const parentFilePath: string = parentSymbol.getDeclarations()?.[0].getSourceFile().fileName;
    const logger: IntentLogger = IntentLogger.getInstance();
    const parentRecordName: string = getNormalizedOhmUrlByFilepath(parentFilePath, projectConfig, logger, pkgParams, null);
    const baseClassName: string = this.checker.getTypeAtLocation(node).getSymbol().getName();
    const baseFilePath: string = node.getSourceFile().fileName;
    const baseRecordName: string = getNormalizedOhmUrlByFilepath(baseFilePath, projectConfig, logger, pkgParams, null);
    const isGlobalPath: boolean = globalModulePaths?.some(globalPath => {
      const normalizedParent: string = path.normalize(parentFilePath).replace(/\\/g, '/');
      const normalizedGlobalPath: string = path.normalize(globalPath).replace(/\\/g, '/');
      return normalizedParent.startsWith(normalizedGlobalPath);
    });
    const recordPath: string = isGlobalPath ? `sdk` : `@normalized:${parentRecordName}`;
    if (isGlobalPath) {
      if (parentClassName !== 'InsightIntentEntryExecutor') {
        throw Error(`${INVALID_BASE_CLASS_ERROR.toString()},
         The custom intent must be ultimately inherited to InsightIntentEntryExecutor, invalidDecoratorPath: ${this.currentFilePath}`);
      }
    }
    this.heritageClassSet.add(baseClassName + '_' + `@normalized:${baseRecordName}`);
    this.collectClassInheritanceInfo(parentNode, intentObj, parentClassName, recordPath);
  }

  collectClassInheritanceInfo(
    parentNode: ts.ExpressionWithTypeArguments, intentObj: object, parentClassName: string, recordPath: string
  ): void {
    const ClassInheritanceInfo: object = {
      'parentClassName': parentClassName,
      'definitionFilePath': recordPath,
      'generics': []
    };
    if (parentNode.typeArguments?.length > 0) {
      parentNode.typeArguments.forEach((arg): void => {
        this.getInheritanceInfoByTypeNode(arg, ClassInheritanceInfo);
      });
    }
    Object.assign(intentObj, {
      'ClassInheritanceInfo': ClassInheritanceInfo
    });
  }

  getInheritanceInfoByTypeNode(arg: ts.TypeNode, ClassInheritanceInfo: object): void {
    const generic = {};
    const genericType: ts.Type = this.checker.getTypeAtLocation(arg);
    let genericName: string;
    let genericSource: string | undefined;
    let recordGenericSource : string;
    const genericSymbol: ts.Symbol | undefined = genericType.getSymbol();
    if (genericSymbol) {
      genericName = genericSymbol.getName();
      genericSource = genericSymbol.declarations?.[0]?.getSourceFile().fileName;
      recordGenericSource = path.relative(projectConfig.moduleName, genericSource).replace(/\\/g, '/');
    } else {
      genericName = this.checker.typeToString(genericType);
      const parentTypeNode: ts.Node = arg.parent;
      if (ts.isTypeReferenceNode(parentTypeNode)) {
        const contextualType: ts.Type = this.checker.getContextualType(parentTypeNode);
        const symbol: ts.Type = contextualType?.getSymbol();
        genericSource = symbol?.declarations?.[0]?.getSourceFile().fileName;
      }
      if (!genericSource && this.isPrimitiveType(genericType)) {
        recordGenericSource = 'lib.es5.d.ts';
      }
    }
    Object.assign(generic,
      {
        'typeName': genericName,
        'definitionFilePath': recordGenericSource
      });
    ClassInheritanceInfo.generics.push(generic);
  }

  isPrimitiveType(type: ts.Type): boolean {
    return (
      (type.flags & ts.TypeFlags.StringLike) ||
      (type.flags & ts.TypeFlags.NumberLike) ||
      (type.flags & ts.TypeFlags.BooleanLike)
    ) !== 0;
  }

  parseClassNode(node: ts.ClassDeclaration): Record<string, string> {
    const mergedObject: Record<string, string> = {};
    node.members
      .filter((member): member is ts.PropertyDeclaration => {
        return ts.isPropertyDeclaration(member) &&
          member.parent === node;
      })
      .forEach(propertyNode => {
        const propSymbol = this.checker.getSymbolAtLocation(propertyNode.name);
        if (!propSymbol) {
          return;
        }
        const objItem: Record<string, string> = this.processProperty(propSymbol);
        Object.assign(mergedObject, objItem);
      });
    return mergedObject;
  }

  processProperty(prop: ts.Symbol): Record<string, string> {
    const propType: ts.Type = this.checker.getTypeOfSymbol(prop);
    const {category} = this.getTypeCategory(propType);
    const obj: Record<string, string> = {};
    const propName: string = prop.getName();
    if (category === 'object') {
      obj[propName] = 'object';
    } else if (category === 'array') {
      obj[propName] = 'array';
    } else {
      obj[propName] = this.checker.typeToString(propType);
    }
    return obj;
  }

  getTypeCategory(type: ts.Type): { category: 'array' | 'object'; } {
    const flags: ts.TypeFlags = type.getFlags();

    const isPrimitive: boolean = !!(flags & ts.TypeFlags.StringLike) ||
      !!(flags & ts.TypeFlags.NumberLike) ||
      !!(flags & ts.TypeFlags.BooleanLike) ||
      !!(flags & ts.TypeFlags.Null) ||
      !!(flags & ts.TypeFlags.Undefined);

    let isArray: boolean;
    const symbol: ts.Symbol | undefined = type.getSymbol();
    if (symbol) {
      isArray = symbol.getName() === 'Array';
    } else {
      isArray = !!(flags & ts.TypeFlags.Object) &&
        !!(type as ts.ObjectType).objectFlags && ts.ObjectFlags.Reference &&
        ((type as ts.TypeReference).target.getSymbol()?.getName() === 'Array');
    }
    const isObject: boolean = !isPrimitive &&
      !isArray &&
      !!(flags & ts.TypeFlags.Object);
    let category: 'array' | 'object';
    if (isArray) {
      category = 'array';
    } else if (isObject) {
      category = 'object';
    }
    return {category};
  }

  removeDecorator(node: ts.ClassDeclaration, decoratorNames: string[]): ts.ClassDeclaration {
    const filteredModifiers: ts.ModifierLike[] = node.modifiers.filter(decorator => {
      if (!ts.isDecorator(decorator)) {
        return true;
      }
      let decoratorName: string | undefined;
      if (ts.isCallExpression(decorator.expression)) {
        decoratorName = '@' + decorator.expression.expression.getText();
      }
      return !decoratorNames.includes(decoratorName);
    });
    const updatedMembers: ts.ClassElement[] = node.members.map(member => {
      return this.reduceMembers(member);
    });
    return ts.factory.updateClassDeclaration(
      node,
      filteredModifiers,
      node.name,
      node.typeParameters,
      node.heritageClauses,
      ts.factory.createNodeArray(updatedMembers)
    );
  }

  reduceMembers(member : ts.ClassElement): ts.ClassElement {
    if (ts.isMethodDeclaration(member) && this.hasModifier(member, ts.SyntaxKind.StaticKeyword)) {
      const memberModifiers: ts.ModifierLike[] = (member.modifiers ?? []).filter(decorator => {
        if (!ts.isDecorator(decorator)) {
          return true;
        }
        let decoratorName: string | undefined;
        if (ts.isCallExpression(decorator.expression)) {
          decoratorName = '@' + decorator.expression.expression.getText();
        }
        return decoratorName !== COMPONENT_USER_INTENTS_DECORATOR_METHOD;
      });
      if (memberModifiers.length !== (member.modifiers?.length ?? 0)) {
        return ts.factory.updateMethodDeclaration(
          member,
          memberModifiers,
          member.asteriskToken,
          member.name,
          member.questionToken,
          member.typeParameters,
          member.parameters,
          member.type,
          member.body!
        );
      }
    }
    return member;
  }

  isSymbolConstant(symbol: ts.Symbol): boolean {
    const declaration: Declaration = symbol.valueDeclaration;

    if (!this.isConstVariable(declaration)) {
      return false;
    }
    const varDecl: ts.VariableDeclaration = declaration as ts.VariableDeclaration;
    const initializer: Expression = varDecl.initializer;
    return initializer ? this.isConstantExpression(initializer) : false;
  }

  isConstVariable(node: ts.Node | undefined): node is ts.VariableDeclaration {
    if (!node || !ts.isVariableDeclaration(node)) {
      return false;
    }

    const varList: VariableDeclarationList | CatchClause = node.parent;
    return !!varList && ts.isVariableDeclarationList(varList) &&
      (varList.flags & ts.NodeFlags.Const) !== 0;
  }

  isConstantExpression(node: ts.Node): boolean {
    let flag: boolean = true;
    if (ts.isLiteralExpression(node) || node.kind === ts.SyntaxKind.TrueKeyword || node.kind === ts.SyntaxKind.FalseKeyword) {
      flag = true;
    }

    if (ts.isIdentifier(node)) {
      const symbol: Symbol | undefined = this.checker.getSymbolAtLocation(node);
      flag = symbol ? this.isSymbolConstant(symbol) : false;
    }

    if (ts.isArrayLiteralExpression(node)) {
      flag = node.elements.every(element => this.isConstantExpression(element));
    }

    if (ts.isObjectLiteralExpression(node)) {
      flag = node.properties.every(property => {
        if (ts.isPropertyAssignment(property)) {
          const nameIsConst: boolean = !ts.isComputedPropertyName(property.name);
          return nameIsConst && this.isConstantExpression(property.initializer);
        }

        return false;
      });
    }

    if (ts.isCallExpression(node) && node.expression.getText() === '$r') {
      flag = node.arguments.every(node => {
        return ts.isStringLiteral(node);
      });
    }
    if (!flag) {
      throw Error(`${DYNAMIC_PARAM_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
    return flag;
  }

  validateRequiredIntentLinkInfo<T>(
    node: ts.ObjectLiteralExpression,
    paramCheckFields: ParamChecker<T>
  ): void {
    const existingParams: Set<keyof T> = new Set<keyof T>();
    const requiredFields: (keyof T)[] = paramCheckFields.requiredFields;
    const nestedCheckers: Map<string, ParamChecker<LinkIntentParamMapping>> = paramCheckFields.nestedCheckers;
    const allowedFields: Set<keyof T> = paramCheckFields.allowFields;
    const paramValidators: Record<keyof T, (v: ts.Expression) => boolean> = paramCheckFields.paramValidators;
    for (const prop of node.properties) {
      this.validateFields(prop, allowedFields, paramValidators);
      existingParams.add(prop.name.text);
      if (nestedCheckers && nestedCheckers.has(prop.name.text)) {
        this.validateSelfParamFields(prop, nestedCheckers);
      }
    }
    const missingFields: (keyof T)[] = requiredFields.filter(f => !existingParams.has(f));
    if (missingFields.length > 0) {
      throw Error(`${REQUIRED_PARAM_DISMISS_ERROR.toString()}, cause params: ${missingFields.join(', ')}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  validateSelfParamFields(prop: ts.Node, nestedCheckers: Map<string, ParamChecker<LinkIntentParamMapping>>): void {
    const checker: ParamChecker<LinkIntentParamMapping> = nestedCheckers.get(prop.name.text);
    if (ts.isArrayLiteralExpression(prop.initializer)) {
      prop.initializer.elements.every(elem => {
        if (ts.isIdentifier(elem)) {
          const symbol: ts.Symbol | undefined = this.checker.getSymbolAtLocation(elem);
          const declaration: ts.Declaration = symbol?.valueDeclaration;
          this.validateRequiredIntentLinkInfo<LinkIntentParamMapping>(declaration.initializer, checker);
        } else {
          this.validateRequiredIntentLinkInfo<LinkIntentParamMapping>(elem, checker);
        }
      });
    } else if (ts.isIdentifier(prop)) {
      const symbol: ts.Symbol | undefined = this.checker.getSymbolAtLocation(prop);
      const declaration: ts.Declaration = symbol?.valueDeclaration;
      this.validateRequiredIntentLinkInfo<LinkIntentParamMapping>(declaration.initializer, checker);
    } else {
      this.validateRequiredIntentLinkInfo<LinkIntentParamMapping>(prop, checker);
    }
  }

  validateFields<T>(
    prop: ts.Node, allowedFields: Set<keyof T>, paramValidators: Record<keyof T, (v: ts.Expression) => boolean>
  ): void {
    const paramName: keyof T = prop.name.text;
    if (ts.isPropertyAssignment(prop) && ts.isIdentifier(prop.name)) {
      if (!allowedFields.has(paramName)) {
        throw Error(`${DISALLOWED_PARAM_ERROR}, cause undeclared param: '${paramName.toString()}', invalidDecoratorPath: ${this.currentFilePath}`);
      }
      const validator: Function = paramValidators[paramName];
      if (ts.isIdentifier(prop.initializer)) {
        const symbol: ts.Symbol | undefined = this.checker.getSymbolAtLocation(prop.initializer);
        const declaration: ts.Declaration = symbol?.valueDeclaration;
        if (validator && !validator(declaration?.initializer)) {
          throw Error(`${INCORRECT_PARAM_TYPE_ERROR.toString()}, cause param: '${paramName.toString()}', invalidDecoratorPath: ${this.currentFilePath}`);
        }
      } else {
        if (validator && !validator(prop.initializer)) {
          throw Error(`${INCORRECT_PARAM_TYPE_ERROR.toString()}, cause param: '${paramName.toString()}', invalidDecoratorPath: ${this.currentFilePath}`);
        }
      }
    }
  }

  analyzeDecoratorArgs<T>(args: ts.NodeArray<ts.Expression>, intentObj: object, paramChecker: ParamChecker<T>): void {
    args.forEach(arg => {
      if (ts.isIdentifier(arg)) {
        const symbol: ts.Symbol | undefined = this.checker.getSymbolAtLocation(arg);
        const declaration: ts.Declaration = symbol?.valueDeclaration;
        this.validateRequiredIntentLinkInfo<T>(declaration.initializer, paramChecker);
      } else {
        this.validateRequiredIntentLinkInfo<T>(arg, paramChecker);
      }
      const res: StaticValue = this.parseStaticObject(arg);
      Object.assign(intentObj, res);
      this.collectSchemaInfo(intentObj);
    });
  }

  createObfuscation(classNode: ts.Node): void {
    ProjectCollections.projectWhiteListManager?.fileWhiteListInfo.fileKeepInfo.arkUIKeepInfo.globalNames.add(classNode.symbol.name);
    const isExported: boolean = classNode.modifiers?.some(mod => mod.kind === ts.SyntaxKind.ExportKeyword);
    if (isExported) {
      ProjectCollections.projectWhiteListManager?.fileWhiteListInfo.fileKeepInfo.arkUIKeepInfo.propertyNames.add(classNode.symbol.name);
    }
    classNode.members.forEach(member => {
      if (ts.isPropertyDeclaration(member) && member.name || ts.isFunctionDeclaration(member) || ts.isMethodDeclaration(member) ||
        ts.isGetAccessor(member) || ts.isSetAccessor(member)) {
        const propName: string = member.name.getText();
        ProjectCollections.projectWhiteListManager?.fileWhiteListInfo.fileKeepInfo.arkUIKeepInfo.propertyNames.add(propName);
      }
    });
  }

  parseStaticObject(node: ts.Node, visited: Set<ts.Node> = new Set()): StaticValue | undefined {
    if (visited.has(node)) {
      throw Error(`${PARAM_CIRCULAR_REFERENCE_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
    visited.add(node);
    if (ts.isStringLiteral(node)) {
      return node.text;
    }
    if (ts.isNumericLiteral(node)) {
      return parseFloat(node.text);
    }
    if (node.kind === ts.SyntaxKind.TrueKeyword) {
      return true;
    }
    if (node.kind === ts.SyntaxKind.FalseKeyword) {
      return false;
    }
    if (node.kind === ts.SyntaxKind.NullKeyword) {
      return null;
    }
    if (node.kind === ts.SyntaxKind.UndefinedKeyword) {
      return undefined;
    }
    if (ts.isIdentifier(node)) {
      const isStatic: boolean = this.isConstantExpression(node);
      if (isStatic) {
        const symbol: ts.Symbol | undefined = this.checker.getSymbolAtLocation(node);
        const declaration: ts.Declaration = symbol?.valueDeclaration;
        return this.parseStaticObject(declaration.initializer, visited);
      }
    }
    if (ts.isArrayLiteralExpression(node)) {
      return this.processArrayElements(node.elements);
    }
    if (ts.isObjectLiteralExpression(node)) {
      return this.processObjectElements(node);
    }
    if (ts.isCallExpression(node) && node.expression.getText() === '$r') {
      const isStatic: boolean = this.isConstantExpression(node);
      if (!isStatic) {
        return undefined;
      }
      return node.getText();
    }
    if (ts.isPropertyAccessExpression(node)) {
      return this.processEnumElement(node);
    }
    throw Error(`${UNSUPPORTED_PARSE_ERROR.toString()}, cause param: '${node.text}', invalidDecoratorPath: ${this.currentFilePath}`);
  }

  processEnumElement(node: ts.PropertyAccessExpression): number {
    const enumValue: string = node.getText();
    const executeModeEnum: Map<string, number> = new Map();
    executeModeEnum.set('insightIntent.ExecuteMode.UI_ABILITY_FOREGROUND', 0);
    executeModeEnum.set('insightIntent.ExecuteMode.UI_ABILITY_BACKGROUND', 1);
    executeModeEnum.set('insightIntent.ExecuteMode.UI_EXTENSION_ABILITY', 2);
    executeModeEnum.set('insightIntent.ExecuteMode.SERVICE_EXTENSION_ABILITY', 3);
    if (executeModeEnum.has(enumValue)) {
      return executeModeEnum.get(enumValue);
    } else {
      throw Error(`${UNSUPPORTED_PARSE_ERROR.toString()}, cause param: '${node.text}', invalidDecoratorPath: ${this.currentFilePath}`);
    }
  }

  processObjectElements(elements: ts.ObjectLiteralExpression): { [key: string]: StaticValue } {
    const obj: { [key: string]: StaticValue } = {};
    for (const prop of elements.properties) {
      if (ts.isPropertyAssignment(prop)) {
        const key: string = this.parsePropertyKey(prop.name);
        const value: StaticValue = this.parseStaticObject(prop.initializer);
        if (key !== undefined && value !== undefined) {
          obj[key] = value;
        }
      }

      if (ts.isSpreadAssignment(prop)) {
        const spreadObj: StaticValue = this.parseStaticObject(prop.expression);
        if (typeof spreadObj === 'object' && spreadObj !== null) {
          Object.assign(obj, spreadObj);
        }
      }
    }
    return obj;
  }

  processArrayElements(elements: readonly ts.Node[]): StaticValue[] {
    const parsedElements: StaticValue[] = [];

    elements.forEach((element) => {
      if (ts.isSpreadElement(element)) {
        const spreadValue: StaticValue = this.parseStaticObject(element.expression);
        if (Array.isArray(spreadValue)) {
          parsedElements.push(...spreadValue);
        }
      } else {
        const value: StaticValue = this.parseStaticObject(element);
        parsedElements.push(value);
      }
    });

    return parsedElements;
  }

  parsePropertyKey(node: ts.PropertyName): string | undefined {
    if (ts.isLiteralExpression(node)) {
      return node.text;
    }

    if (ts.isIdentifier(node)) {
      return node.text;
    }
    return undefined;
  }

  processExecuteModeParam(intentObj: object): void {
    if (intentObj.executeMode) {
      intentObj.executeMode.forEach((item: number, index: number) => {
        if (item === 0) {
          intentObj.executeMode[index] = 'foreground';
        }
        if (item === 1) {
          intentObj.executeMode[index] = 'background';
        }
        if (item === 2) {
          intentObj.executeMode[index] = 'uiextension';
        }
        if (item === 3) {
          intentObj.executeMode[index] = 'serviceextension';
        }
      });
    }
  }

  collectSchemaInfo(intentObj: object): void {
    if (intentObj.schema) {
      const schemaPath: string = path.join(
        __dirname, 'schema',
        `${intentObj.schema}_${intentObj.intentVersion}.json`
      );
      if (fs.existsSync(schemaPath)) {
        const schemaContent: string = fs.readFileSync(schemaPath, 'utf-8');
        const schemaObj: object = JSON.parse(schemaContent);
        intentObj.parameters = schemaObj.params;
        intentObj.llmDescription = schemaObj.llmDescription;
        intentObj.keywords = schemaObj.keywords;
        intentObj.intentName = schemaObj.intentName;
      }
    }
  }

  verifyInheritanceChain(): void {
    this.intentData.forEach((element): void => {
      if (element.ClassInheritanceInfo) {
        const parentClassName: string = element.ClassInheritanceInfo.parentClassName;
        const definitionFilePath: string = element.ClassInheritanceInfo.definitionFilePath;
        const verifiedString: string = parentClassName + '_' + definitionFilePath;
        if (!this.heritageClassSet.has(verifiedString)) {
          throw Error(`${INVALID_BASE_CLASS_ERROR.toString()}
          , Subclass must inherit from a class decorated with @InsightIntentEntry, invalidDecoratorPath: ${this.currentFilePath}\``);
        }
      }
    });
  }

  schemaValidationRequiredRule(schemaData: Record<string, string>, schemaObj: object): void {
    const reqData: Map<string, boolean> = new Map();
    schemaObj.required.forEach(key => reqData.set(key, true));
    if (schemaObj.properties) {
      const paramsSchema: object = schemaObj.properties;
      const keyArr: string[] = Object.keys(paramsSchema);
      keyArr.forEach(key => {
        if (!schemaData[key] && reqData.get(key)) {
          throw Error(`${SCHEMA_VALIDATE_REQUIRED_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
        }
      });
    }
  }

  schemaPropertiesValidation(schemaData: Record<string, string>, schemaObj: object): void {
    if (schemaObj.properties) {
      Object.entries(schemaObj.properties).forEach(([key, value]) => {
        if (schemaData[key] && value.type !== schemaData[key]) {
          throw Error(`${SCHEMA_VALIDATE_TYPE_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
        }
      });
    }
  }

  private schemaValidateRules(schemaData: Record<string, string>, schemaObj: object): void {
    const schemaKeys: string[] = Object.keys(schemaData);
    if (schemaObj.oneOf) {
      let count: number = 0;
      const requiredOne: string[][] = schemaObj.oneOf.map(item => item.required);
      requiredOne.forEach(val => {
        const isContain: boolean = val.every((item): boolean => {
          return schemaKeys.includes(item);
        });
        if (isContain) {
          count++;
        }
      });
      if (count !== 1) {
        throw Error(`${SCHEMA_VALIDATE_ONE_OF_ERROR.toString()} , invalidDecoratorPath: ${this.currentFilePath}`);
      }
    }
    if (schemaObj.anyOf) {
      let count: number = 0;
      const requiredAny: string[][] = schemaObj.anyOf.map(item => item.required);
      requiredAny.forEach(val => {
        const isContain: boolean = val.every((item): boolean => {
          return schemaKeys.includes(item);
        });
        if (isContain) {
          count++;
        }
      });
      if (count === 0) {
        throw Error(`${SCHEMA_VALIDATE_ANY_OF_ERROR.toString()} , invalidDecoratorPath: ${this.currentFilePath}`);
      }
    }
  }

  schemaValidateSync(schemaData: Record<string, string>, schemaObj: object): void {
    if (!schemaObj) {
      return;
    }
    if (schemaObj.additionalProperties === false) {
      this.schemaAdditionalPropertiesValidation(schemaData, schemaObj.properties);
    }
    if (schemaObj.items && schemaObj.items.type === 'array') {
      this.schemaValidateSync(schemaData, schemaObj.items.items);
    }
    if (schemaObj.type !== 'object') {
      throw Error(`${SCHEMA_ROOT_TYPE_MISMATCH_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
    }
    if (schemaObj.properties) {
      const items: string[] = Object.keys(schemaObj.properties);
      if (items.length === 1 && items[0].type === 'array') {
        this.schemaValidateSync(schemaData, items[0].items);
      } else {
        this.schemaPropertiesValidation(schemaData, schemaObj);
      }
    }
    if (schemaObj.required) {
      this.schemaValidationRequiredRule(schemaData, schemaObj);
    }
    this.schemaValidateRules(schemaData, schemaObj);
  }

  schemaAdditionalPropertiesValidation(schemaData, schemaProps): void {
    for (const key of Object.keys(schemaData)) {
      if (!schemaProps[key]) {
        throw Error(`${SCHEMA_VALIDATE_TYPE_ERROR.toString()}, invalidDecoratorPath: ${this.currentFilePath}`);
      }
    }
  }

  writeUserIntentJsonFile(): void {
    if (!projectConfig.aceProfilePath) {
      throw Error(`${INTERNAL_ERROR.toString()}, aceProfilePath not found, invalidDecoratorPath: ${this.currentFilePath}`);
    }
    const cacheSourceMapPath: string = path.join(projectConfig.aceProfilePath, 'insight_intent.json');
    const cachePath: string = path.join(projectConfig.cachePath, 'insight_compile_cache.json');
    if (!fs.existsSync(projectConfig.aceProfilePath)) {
      fs.mkdirSync(projectConfig.aceProfilePath, {recursive: true});
    }
    if (this.isUpdateCompile) {
      const cacheData: string = fs.readFileSync(cachePath, 'utf8');
      const cacheDataObj: object = JSON.parse(cacheData);
      const insightIntents: object[] = cacheDataObj.extractInsightIntents.filter(insightIntent => {
        return !this.updatePageIntentObj.has(insightIntent.decoratorFile);
      });
      this.updatePageIntentObj.forEach(insightIntent => {
        insightIntents.push(...insightIntent);
      });
      this.intentData = insightIntents;
    }
    let writeJsonData: object = {};
    if (fs.existsSync(cacheSourceMapPath)) {
      const originIntents: string = fs.readFileSync(cacheSourceMapPath, 'utf8');
      const jsonData: object = JSON.parse(originIntents);
      Object.assign(jsonData, {
        'extractInsightIntents': this.intentData
      });
      writeJsonData = jsonData;
    } else {
      Object.assign(writeJsonData, {
        'extractInsightIntents': this.intentData
      });
    }
    this.validateIntentIntentName(writeJsonData);
    if (this.intentData.length > 0) {
      fs.writeFileSync(cacheSourceMapPath, JSON.stringify(writeJsonData, null, 2), 'utf-8');
      fs.writeFileSync(cachePath, JSON.stringify({'extractInsightIntents': this.intentData}, null, 2), 'utf-8');
    } else if (fs.existsSync(cacheSourceMapPath)) {
      fs.unlinkSync(cacheSourceMapPath);
    }
    const normalizedPath: string = path.normalize(projectConfig.aceProfilePath);
    const fullPath: string = path.join(normalizedPath, '../../../module.json');
    if (fs.existsSync(fullPath)) {
      const rawData: string = fs.readFileSync(fullPath, 'utf8');
      const jsonData: object = JSON.parse(rawData);
      if (jsonData?.module) {
        jsonData.module.hasInsightIntent = this.intentData.length > 0 ? true : undefined;
      }
      const updatedJson: string = JSON.stringify(jsonData, null, 2);
      fs.writeFileSync(fullPath, updatedJson, 'utf8');
    }
  }

  validateIntentIntentName(writeJsonData: object): void {
    const duplicates = new Set<string>();
    writeJsonData.insightIntents?.forEach(insightIntent => {
      duplicates.add(insightIntent.intentName);
    });
    writeJsonData.extractInsightIntents.forEach(item => {
      if (duplicates.has(item.intentName)) {
        throw Error(`${DECORATOR_DUPLICATE_INTENTNAME.toString()}, value : ${item.intentName}`);
      } else {
        duplicates.add(item.intentName);
      }
    });
  }

  clear(): void {
    this.intentData = [];
    this.checker = null;
    this.currentFilePath = '';
    this.heritageClassSet = new Set<string>();
    this.heritageClassSet.add('InsightIntentEntryExecutor_sdk');
    this.isInitCache = false;
    this.isUpdateCompile = false;
    this.updatePageIntentObj = new Map();
  }
}

export default new ParseIntent();
