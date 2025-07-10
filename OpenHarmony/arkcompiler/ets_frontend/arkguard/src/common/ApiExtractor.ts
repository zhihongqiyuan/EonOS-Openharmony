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

import type {
  AnnotationDeclaration,
  ClassDeclaration,
  CommentRange,
  CompilerOptions,
  Decorator,
  ElementAccessExpression,
  EnumDeclaration,
  ExportDeclaration,
  Expression,
  FunctionDeclaration,
  InterfaceDeclaration,
  ModifiersArray,
  ModuleDeclaration,
  NamedDeclaration,
  Node,
  ParameterDeclaration,
  PropertyAccessExpression,
  SourceFile,
  TypeAliasDeclaration,
  VariableDeclaration,
  VariableStatement
} from 'typescript';

import {
  createSourceFile,
  ClassElement,
  forEachChild,
  getLeadingCommentRangesOfNode,
  isAnnotationDeclaration,
  isBinaryExpression,
  isClassDeclaration,
  isClassExpression,
  isExpressionStatement,
  isEnumDeclaration,
  isExportAssignment,
  isExportDeclaration,
  isExportSpecifier,
  isGetAccessor,
  isIdentifier,
  isInterfaceDeclaration,
  isModuleBlock,
  isObjectLiteralExpression,
  isParameterPropertyDeclaration,
  isStructDeclaration,
  isSourceFile,
  isTypeAliasDeclaration,
  isVariableDeclaration,
  isVariableStatement,
  isElementAccessExpression,
  isPropertyAccessExpression,
  isSetAccessor,
  isStringLiteral,
  ScriptTarget,
  SyntaxKind,
  sys,
  isConstructorDeclaration,
  getModifiers,
  isNamedExports,
  isNamespaceExport,
  isPropertyDeclaration,
  isPropertySignature,
  isMethodDeclaration,
  isMethodSignature,
  isPropertyAssignment,
  isEnumMember,
  isParameter,
  isTypeParameterDeclaration,
  isIndexedAccessTypeNode,
  Extension,
  isCallExpression,
  isDecorator
} from 'typescript';

import fs from 'fs';
import path from 'path';
import json5 from 'json5';

import {
  exportOriginalNameSet,
  getClassProperties,
  getElementAccessExpressionProperties,
  getEnumProperties,
  getIndexedAccessTypeProperties,
  getInterfaceProperties,
  getObjectExportNames,
  getObjectProperties,
  getTypeAliasProperties,
  isParameterPropertyModifier,
} from '../utils/OhsUtil';
import { scanProjectConfig } from './ApiReader';
import { enumPropsSet } from '../utils/OhsUtil';
import { FileUtils } from '../utils/FileUtils';
import { supportedParsingExtension } from './type';
import {
  addToSet,
  DECORATOR_WHITE_LIST,
  FileWhiteList, KeepInfo,
  projectWhiteListManager
} from '../utils/ProjectCollections';
import { AtKeepCollections, BytecodeObfuscationCollections, PropCollections } from '../utils/CommonCollections';
import { hasExportModifier } from '../utils/NodeUtils';

export namespace ApiExtractor {
  interface KeywordInfo {
    hasExport: boolean,
    hasDeclare: boolean
  }

  export enum ApiType {
    API,
    COMPONENT,
    PROJECT,
    CONSTRUCTOR_PROPERTY,
    KEEP_DTS
  }

  export enum AtKeepType {
    None,
    KeepSymbol,
    KeepAsConsumer
  }

  export enum WhiteListType {
    PropertyName,
    GlobalName,
  }

  type KeepTargetNode =
  | ClassDeclaration
  | InterfaceDeclaration
  | EnumDeclaration
  | FunctionDeclaration
  | ModuleDeclaration
  | VariableDeclaration
  | TypeAliasDeclaration
  | AnnotationDeclaration;

  const KEEP_SYMBOL = '//@KeepSymbol';
  const KEEP_AS_CONSUMER = '//@KeepAsConsumer';

  let mCurrentExportedPropertySet: Set<string> = new Set<string>();
  let mCurrentExportNameSet: Set<string> = new Set<string>();

  let decoratorMap: Map<string, Set<string>> = new Map<string, Set<string>>();

  let keepSymbolTemp: KeepInfo = {
    propertyNames: new Set<string>(),
    globalNames: new Set<string>(),
  };
  let keepAsConsumerTemp: KeepInfo = {
    propertyNames: new Set<string>(),
    globalNames: new Set<string>(),
  };

  function clearAtKeepTemp(): void {
    keepSymbolTemp.propertyNames.clear();
    keepSymbolTemp.globalNames.clear();
    keepAsConsumerTemp.propertyNames.clear();
    keepAsConsumerTemp.globalNames.clear();
  }

  export let mPropertySet: Set<string> = new Set<string>();
  export let mExportNames: Set<string> = new Set<string>();
  export let mConstructorPropertySet: Set<string> = new Set<string>();
  export let mEnumMemberSet: Set<string> = new Set<string>();
  export let mSystemExportSet: Set<string> = new Set<string>();
  /**
   * filter classes or interfaces with export, default, etc
   */
  const getKeyword = function (modifiers: ModifiersArray): KeywordInfo {
    if (modifiers === undefined) {
      return {hasExport: false, hasDeclare: false};
    }

    let hasExport: boolean = false;
    let hasDeclare: boolean = false;

    for (const modifier of modifiers) {
      if (modifier.kind === SyntaxKind.ExportKeyword) {
        hasExport = true;
      }

      if (modifier.kind === SyntaxKind.DeclareKeyword) {
        hasDeclare = true;
      }
    }

    return {hasExport: hasExport, hasDeclare: hasDeclare};
  };

  /**
   * get export name list
   * @param astNode
   */
  const visitExport = function (astNode, isSystemApi: boolean): void {
    /**
     * export = exportClass //collect exportClass
     *
     * function foo()
     * export default foo //collect foo
     */
    if (isExportAssignment(astNode)) {
      let nodeName = astNode.expression.getText();
      if (!mCurrentExportNameSet.has(nodeName)) {
        collectNodeName(nodeName);
      }
      return;
    }

    if (isExportDeclaration(astNode) && astNode.exportClause) {
      /**
       * export {name1, name2} //collect name1, name2
       * export {name1 as n1, name2} //collect n1, name2
       * export {name1 as default, name2, name3} //collect default, name2, name3
       */
      if (isNamedExports(astNode.exportClause)) {
        for (const element of astNode.exportClause.elements) {
          const exportElementName = element.name.getText();
          if (!mCurrentExportNameSet.has(exportElementName)) {
            collectNodeName(exportElementName);
          }
        }
      }

      /**
       * export * as name1 from 'file.ts' //collect name1
       */
      if (isNamespaceExport(astNode.exportClause)) {
        const exportElementName = astNode.exportClause.name.getText();
        if (!mCurrentExportNameSet.has(exportElementName)) {
          collectNodeName(exportElementName);
        }
      }

      /**
      * Other export syntax, which does not contain a name. such as:
      * export * from 'file.ts'
      */
      return;
    }

    let {hasExport, hasDeclare} = getKeyword(astNode.modifiers);
    if (!hasExport) {
      return;
    }

    if (astNode.name) {
      let nodeName = astNode.name.getText();
      if (!mCurrentExportNameSet.has(nodeName)) {
        collectNodeName(nodeName);
      }

      return;
    }

    if (hasDeclare && astNode.declarationList) {
      astNode.declarationList.declarations.forEach((declaration) => {
        const declarationName = declaration.name.getText();
        if (!mCurrentExportNameSet.has(declarationName)) {
          collectNodeName(declarationName);
        }
      });
    }
  };

  const isCollectedExportNames = function (astNode): boolean {
    if (astNode.name && !mCurrentExportNameSet.has(astNode.name.getText())) {
      return false;
    }

    if (astNode.name === undefined) {
      let {hasDeclare} = getKeyword(astNode.modifiers);
      if (hasDeclare && astNode.declarationList &&
        !mCurrentExportNameSet.has(astNode.declarationList.declarations[0].name.getText())) {
        return false;
      }
    }

    return true;
  };

  /**
   * used only in oh sdk api extract or api of xxx.d.ts declaration file
   * @param astNode
   */
  const visitChildNode = function (astNode, isSdkApi: boolean = false): void {
    if (!astNode) {
      return;
    }
    if (astNode.name !== undefined && !mCurrentExportedPropertySet.has(astNode.name.getText())) {
      const notAddParameter: boolean = scanProjectConfig.mStripSystemApiArgs && isSdkApi;
      if (!notAddParameter || (!isParameter(astNode) && !isTypeParameterDeclaration(astNode))) {
        const nameToAdd = isStringLiteral(astNode.name) ? astNode.name.text : astNode.name.getText();
        mCurrentExportedPropertySet.add(nameToAdd);
      }
    }

    astNode.forEachChild((childNode) => {
      visitChildNode(childNode, isSdkApi);
    });
  };

  // Collect constructor properties from all files.
  // To avoid generating the same name as the constructor property when obfuscating identifier names.
  const visitNodeForConstructorProperty = function (astNode): void {
    if (!astNode) {
      return;
    }

    if (isConstructorDeclaration(astNode)) {
      const visitParam = (param: ParameterDeclaration): void => {
        const modifiers = getModifiers(param);
        if (!modifiers || modifiers.length <= 0) {
          return;
        }

        const findRet = modifiers.find(modifier => isParameterPropertyModifier(modifier));
        if (!isIdentifier(param.name) || findRet === undefined) {
          return;
        }
        mConstructorPropertySet.add(param.name.getText());
        projectWhiteListManager?.fileWhiteListInfo?.fileReservedInfo.propertyParams.add(param.name.getText());
      };

      astNode?.parameters?.forEach((param) => {
        visitParam(param);
      });
    }

    astNode.forEachChild((childNode) => {
      visitNodeForConstructorProperty(childNode);
    });
  };
  /**
   * visit ast of a file and collect api list
   * used only in oh sdk api extract
   * @param astNode node of ast
   */
  const visitPropertyAndNameForSdk = function (astNode): void {
    if (!isCollectedExportNames(astNode)) {
      /**
       * Collects property names of elements that haven't been collected yet.
       * @param astNode elements of sourcefile
       */
      collectPropertyNames(astNode);
      return;
    }

    visitChildNode(astNode, true);
  };

  /**
   * commonjs exports extract
   * examples:
   * - exports.A = 1;
   * - exports.B = hello; // hello can be variable or class ...
   * - exports.C = {};
   * - exports.D = class {};
   * - exports.E = function () {}
   * - class F {}
   * - exports.F = F;
   * - module.exports = {G: {}};
   */
  const addCommonJsExports = function (astNode: Node, isRemoteHarOrSystemApi: boolean = false): void {
    if (!isExpressionStatement(astNode) || !astNode.expression) {
      return;
    }

    const expression = astNode.expression;
    if (!isBinaryExpression(expression)) {
      return;
    }

    const left = expression.left;
    if (!isElementAccessExpression(left) && !isPropertyAccessExpression(left)) {
      return;
    }

    if (!isModuleExports(left) || expression.operatorToken.kind !== SyntaxKind.EqualsToken) {
      return;
    }

    if (isElementAccessExpression(left)) {
      if (isStringLiteral(left.argumentExpression)) {
        /**
         * - module.exports['A'] = class {};
         * - module.exports['a'] = {};
         * - module.exports['a'] = A;
         */
        mCurrentExportedPropertySet.add(left.argumentExpression.text);
        mCurrentExportNameSet.add(left.argumentExpression.text);
      }
    }

    if (isPropertyAccessExpression(left)) {
      if (isIdentifier(left.name)) {
        /**
         * - module.exports.A = a;
         * - module.exports.A = {};
         * - module.exports.A = class {};
         */
        mCurrentExportedPropertySet.add(left.name.getText());
        mCurrentExportNameSet.add(left.name.getText());
      }
    }

    if (isIdentifier(expression.right)) {
      /**
       * module.exports.A = a;
       * exports.A = a;
       * module.exports = a;
       */
      let originalName = expression.right.getText();
      if (isRemoteHarOrSystemApi) {
        // To achieve compatibility changes, originalName is still collected into mCurrentExportNameSet
        // for both remoteHar and system API files.

        // NOTE: This logic will be optimized later to avoid collecting originalName into mCurrentExportNameSet under any circumstances.
        mCurrentExportNameSet.add(originalName);
      } else {
        exportOriginalNameSet.add(originalName);
      }
      return;
    }

    if (isClassDeclaration(expression.right) || isClassExpression(expression.right)) {
      /**
       * module.exports.A = class testClass {}
       * module.exports = class testClass {}
       * exports.A = class testClass {}
       * module.exports.A = class {}
       */
      getClassProperties(expression.right, mCurrentExportedPropertySet);
      return;
    }

    if (isObjectLiteralExpression(expression.right)) {
      /**
       * module.exports = {a, b, c};
       * module.exports.A = {a, b, c};
       * exports.A = {a, b, c}
       */
      getObjectProperties(expression.right, mCurrentExportedPropertySet);
      // module.exports = {a, b, c}, {a, b, c} as the export content of the module
      let defaultExport = left.expression.getText() === 'module';
      if (defaultExport) {
        getObjectExportNames(expression.right, mCurrentExportNameSet);
      }
      return;
    }

    return;
  };

  function isModuleExports(leftExpression: ElementAccessExpression | PropertyAccessExpression): boolean {
    let leftExpressionText = leftExpression.expression.getText();
    if (isPropertyAccessExpression(leftExpression.expression)) {
      /**
       * For example:
       * module.exports.a = A;
       * module.exports['a'] = A;
       */
      return leftExpressionText === 'module.exports';
    }
    if (isIdentifier(leftExpression.expression)) {
      if (leftExpressionText === 'module') {
        // module.exports = {A}, A as the export content of the module
        if (isPropertyAccessExpression(leftExpression) && leftExpression.name.getText() === 'exports') {
          return true;
        }
      }

      /**
       * For example:
       * exports.a = A;
       */
      return leftExpressionText === 'exports';
    }
    return false;
  };

  /**
   * extract project export name
   * - export {xxx, xxx};
   * - export {xxx as xx, xxx as xx};
   * - export default function/class/...{};
   * - export class xxx{}
   * - ...
   * @param astNode
   */
  const visitProjectExport = function (astNode, isRemoteHarFile: boolean): void {
    if (isExportAssignment(astNode)) {
      handleExportAssignment(astNode);
      return;
    }

    if (isExportDeclaration(astNode)) {
      handleExportDeclaration(astNode, isRemoteHarFile);
      return;
    }

    let {hasExport} = getKeyword(astNode.modifiers);
    if (!hasExport) {
      addCommonJsExports(astNode, isRemoteHarFile);
      forEachChild(astNode, node => visitProjectExport(node, isRemoteHarFile));
      return;
    }

    if (astNode.name) {
      if (!mCurrentExportNameSet.has(astNode.name.getText())) {
        mCurrentExportNameSet.add(astNode.name.getText());
        mCurrentExportedPropertySet.add(astNode.name.getText());
      }

      forEachChild(astNode, node => visitProjectExport(node, isRemoteHarFile));
      return;
    }

    if (isClassDeclaration(astNode)) {
      getClassProperties(astNode, mCurrentExportedPropertySet);
      return;
    }

    if (isVariableStatement(astNode)) {
      astNode.declarationList.forEachChild((child) => {
        if (isVariableDeclaration(child) && !mCurrentExportNameSet.has(child.name.getText())) {
          mCurrentExportNameSet.add(child.name.getText());
          mCurrentExportedPropertySet.add(child.name.getText());
        }
      });

      return;
    }

    forEachChild(astNode, node => visitProjectExport(node, isRemoteHarFile));
  };

  function handleExportAssignment(astNode): void {
    // let xxx; export default xxx = a;
    if (isBinaryExpression(astNode.expression)) {
      if (isObjectLiteralExpression(astNode.expression.right)) {
        getObjectProperties(astNode.expression.right, mCurrentExportedPropertySet);
        return;
      }

      if (isClassExpression(astNode.expression.right)) {
        getClassProperties(astNode.expression.right, mCurrentExportedPropertySet);
      }

      return;
    }

    // export = xxx; The xxx here can't be obfuscated
    // export default yyy; The yyy here can be obfuscated
    if (isIdentifier(astNode.expression)) {
      if (!mCurrentExportNameSet.has(astNode.expression.getText())) {
        mCurrentExportNameSet.add(astNode.expression.getText());
        mCurrentExportedPropertySet.add(astNode.expression.getText());
      }
      return;
    }

    if (isObjectLiteralExpression(astNode.expression)) {
      getObjectProperties(astNode.expression, mCurrentExportedPropertySet);
    }
  }

  function handleExportDeclaration(astNode: ExportDeclaration, isRemoteHarFile: boolean): void {
    if (astNode.exportClause) {
      if (astNode.exportClause.kind === SyntaxKind.NamedExports) {
        astNode.exportClause.forEachChild((child) => {
          if (!isExportSpecifier(child)) {
            return;
          }

          if (child.propertyName) {
            let originalName = child.propertyName.getText();
            if (isRemoteHarFile || astNode.moduleSpecifier) {
              // For the first condition, this ensures that for remoteHar files,
              // originalName is still collected into mCurrentExportNameSet to maintain compatibility.
              // NOTE: This specification needs to be revised to determine whether to add originalName
              // to mCurrentExportNameSet should be independent of whether it is in a remoteHar file.

              // The second condition indicates that for `export {A as B} from './filePath'` statements,
              // the original name (A) needs to be added to the export whitelist.
              mCurrentExportNameSet.add(originalName);
            } else {
              /**
               * In project source code:
               * class A {
               *   prop1 = 1;
               *   prop2 = 2;
               * }
               * export {A as B}; // collect A to ensure we can collect prop1 and prop2
               */
              exportOriginalNameSet.add(originalName);
            }
          }

          let exportName = child.name.getText();
          mCurrentExportedPropertySet.add(exportName);
          mCurrentExportNameSet.add(exportName);
        });
      }

      if (astNode.exportClause.kind === SyntaxKind.NamespaceExport) {
        mCurrentExportedPropertySet.add(astNode.exportClause.name.getText());
        return;
      }
    }
  }

  /**
   * extract the class, enum, and object properties of the export in the project before obfuscation
   * class A{};
   * export = A; need to be considered
   * export = namespace;
   * This statement also needs to determine whether there is an export in the namespace, and namespaces are also allowed in the namespace
   * @param astNode
   */
  const visitProjectNode = function (astNode): void {
    const currentPropsSet: Set<string> = new Set();
    let nodeName: string | undefined = astNode.name?.text;
    if ((isClassDeclaration(astNode) || isStructDeclaration(astNode))) {
      getClassProperties(astNode, currentPropsSet);
    } else if (isEnumDeclaration(astNode)) { // collect export enum structure properties
      getEnumProperties(astNode, currentPropsSet);
    } else if (isVariableDeclaration(astNode)) {
      if (astNode.initializer) {
        if (isObjectLiteralExpression(astNode.initializer)) {
          getObjectProperties(astNode.initializer, currentPropsSet);
        } else if (isClassExpression(astNode.initializer)) {
          getClassProperties(astNode.initializer, currentPropsSet);
        }
      }
      nodeName = astNode.name?.getText();
    } else if (isInterfaceDeclaration(astNode)) {
      getInterfaceProperties(astNode, currentPropsSet);
    } else if (isTypeAliasDeclaration(astNode)) {
      getTypeAliasProperties(astNode, currentPropsSet);
    } else if (isElementAccessExpression(astNode)) {
      getElementAccessExpressionProperties(astNode);
    } else if (isIndexedAccessTypeNode(astNode)) {
      getIndexedAccessTypeProperties(astNode);
    } else if (isObjectLiteralExpression(astNode)) {
      getObjectProperties(astNode, currentPropsSet);
    } else if (isClassExpression(astNode)) {
      getClassProperties(astNode, currentPropsSet);
    }

    addPropWhiteList(nodeName, astNode, currentPropsSet);

    forEachChild(astNode, visitProjectNode);
  };

  function addPropWhiteList(nodeName: string | undefined, astNode: Node, currentPropsSet: Set<string>): void {
    if (nodeName && (mCurrentExportNameSet.has(nodeName) || exportOriginalNameSet.has(nodeName))) {
      addElement(currentPropsSet);
    }

    if (scanProjectConfig.isHarCompiled && scanProjectConfig.mPropertyObfuscation && isEnumDeclaration(astNode)) {
      addEnumElement(currentPropsSet);
    }
  }

  function addElement(currentPropsSet: Set<string>): void {
    currentPropsSet.forEach((element: string) => {
      mCurrentExportedPropertySet.add(element);
    });
  }

  function addEnumElement(currentPropsSet: Set<string>): void {
    currentPropsSet.forEach((element: string) => {
      enumPropsSet.add(element);
      projectWhiteListManager?.fileWhiteListInfo?.fileKeepInfo.enumProperties.add(element);
    });
  }
  /**
   * parse file to api list and save to json object
   * @param fileName file name of api file
   * @param apiType
   * @private
   */
  const parseFile = function (fileName: string, apiType: ApiType): void {
    if (!FileUtils.isReadableFile(fileName) || !isParsableFile(fileName)) {
      return;
    }

    projectWhiteListManager?.setCurrentCollector(fileName);

    let sourceFile: SourceFile = createSourceFile(fileName, fs.readFileSync(fileName).toString(), ScriptTarget.ES2015, true, undefined, {
      etsAnnotationsEnable: true
    }, true);
    mCurrentExportedPropertySet.clear();

    collectWhiteListByApiType(sourceFile, apiType, fileName);

    // collect field decorated by UI
    if (scanProjectConfig.scanDecorator) {
      collectAndAddFieldDecorator(sourceFile);
    }

    // collect names marked with '// @KeepSymbol' or '// @KeepAsConsumer', only support .ts/.ets
    if (shouldCollectAtKeep(fileName)) {
      collectAndAddAtKeepNames(sourceFile);
    } 

    // collect origin source file white lists
    if (shouldCollectFileWhiteLists(apiType)) {
      collectFileWhiteLists();
    }

    // collect export names.
    mCurrentExportNameSet.forEach(item => mExportNames.add(item));
    mCurrentExportNameSet.clear();
    // collect export names and properties.
    mCurrentExportedPropertySet.forEach(item => mPropertySet.add(item));
    mCurrentExportedPropertySet.clear();
    exportOriginalNameSet.clear();
    decoratorMap.clear();
  };

  function shouldCollectAtKeep(fileName: string): boolean {
    return scanProjectConfig.mEnableAtKeep &&
      !(fileName.endsWith(Extension.Dts) || fileName.endsWith(Extension.Dets)) &&
      (fileName.endsWith(Extension.Ts) || fileName.endsWith(Extension.Ets));
  }

  function collectAndAddAtKeepNames(sourceFile: SourceFile): void {
    clearAtKeepTemp();
    collectNamesWithAtKeep(sourceFile, sourceFile);
    addToSet(AtKeepCollections.keepSymbol.globalNames, keepSymbolTemp.globalNames);
    addToSet(AtKeepCollections.keepSymbol.propertyNames, keepSymbolTemp.propertyNames);
    addToSet(AtKeepCollections.keepAsConsumer.globalNames, keepAsConsumerTemp.globalNames);
    addToSet(AtKeepCollections.keepAsConsumer.propertyNames, keepAsConsumerTemp.propertyNames);
  }

  function shouldCollectFileWhiteLists(apiType: ApiType): boolean {
    return apiType === ApiType.PROJECT || apiType === ApiType.CONSTRUCTOR_PROPERTY;
  }

  function collectFileWhiteLists(): void {
    const fileWhiteLists: FileWhiteList | undefined = projectWhiteListManager?.fileWhiteListInfo;
    if (!fileWhiteLists) {
      return;
    }

    if (scanProjectConfig.mPropertyObfuscation) {
      addToSet(fileWhiteLists.fileKeepInfo.exported.propertyNames, mCurrentExportedPropertySet);
      if (!scanProjectConfig.mKeepStringProperty) {
        fileWhiteLists.fileKeepInfo.stringProperties.clear();
      }
    } else {
      fileWhiteLists.fileKeepInfo.structProperties.clear();
      fileWhiteLists.fileKeepInfo.stringProperties.clear();
      fileWhiteLists.fileKeepInfo.enumProperties.clear();
    }
    if (scanProjectConfig.mExportObfuscation) {
      addToSet(fileWhiteLists.fileKeepInfo.exported.globalNames, mCurrentExportNameSet);
    }
    if (scanProjectConfig.mEnableAtKeep) {
      addToSet(fileWhiteLists.fileKeepInfo.keepSymbol.globalNames, keepSymbolTemp.globalNames);
      addToSet(fileWhiteLists.fileKeepInfo.keepSymbol.propertyNames, keepSymbolTemp.propertyNames);
      addToSet(fileWhiteLists.fileKeepInfo.keepAsConsumer.globalNames, keepAsConsumerTemp.globalNames);
      addToSet(fileWhiteLists.fileKeepInfo.keepAsConsumer.propertyNames, keepAsConsumerTemp.propertyNames);
    }
    if (scanProjectConfig.scanDecorator) {
      const convertedMap = new Map(
        Array.from(decoratorMap.entries()).map(([key, value]) => [key, Array.from(value)]));
      if (!fileWhiteLists.bytecodeObfuscateKeepInfo) {
        fileWhiteLists.bytecodeObfuscateKeepInfo = {};
      }
      fileWhiteLists.bytecodeObfuscateKeepInfo.decoratorMap = Object.fromEntries(convertedMap);
      const allProp = Array.from(convertedMap.values()).flat();
      allProp.forEach(value => BytecodeObfuscationCollections.decoratorProp.add(value));
    }
  }

  function collectAndAddFieldDecorator(sourceFile: SourceFile): void {
    visitDecorator(sourceFile);
  }

  function getDecorators(node: Node): Decorator[] {
    const decorators: Decorator[] = [];
    forEachChild(node, child => {
      if (isDecorator(child)) {
        decorators.push(child);
      }
    });
    return decorators;
  }

  function visitDecorator(node: Node): void {
    const decorators = getDecorators(node) || [];
    const propertyName = (node as NamedDeclaration).name?.getText();
    if (!propertyName) {
      forEachChild(node, visitDecorator);
      return;
    }

    decorators.forEach(decorator => {
      const expr = decorator.expression;
      const decoratorName = getDecoratorName(expr);
      if (!decoratorName || !isWhiteListedDecorator(decoratorName)) {
        return;
      }
      ensureEntry(decoratorMap, decoratorName, () => new Set<string>());
      const decoratorSet = decoratorMap.get(decoratorName)!;

      if (isCallExpression(expr)) {
        expr.arguments.forEach(arg => {
          const stripped = arg.getText().replace(/^['"]|['"]$/g, '').split('.');
          stripped.forEach(item => decoratorSet.add(item));
        });
      }
      decoratorSet.add(propertyName);

    });
    forEachChild(node, visitDecorator);
  }

  function collectWhiteListByApiType(sourceFile: SourceFile, apiType: ApiType, fileName: string): void {
    // get export name list
    switch (apiType) {
      case ApiType.COMPONENT:
        forEachChild(sourceFile, node => visitChildNode(node, true));
        break;
      case ApiType.KEEP_DTS:
        forEachChild(sourceFile, visitChildNode);
        break;
      case ApiType.API:
        mCurrentExportNameSet.clear();
        forEachChild(sourceFile, node => visitExport(node, true));
        mCurrentExportNameSet.forEach(item => mSystemExportSet.add(item));

        forEachChild(sourceFile, visitPropertyAndNameForSdk);
        mCurrentExportNameSet.clear();
        break;
      case ApiType.PROJECT:
        mCurrentExportNameSet.clear();
        if (fileName.endsWith('.d.ts') || fileName.endsWith('.d.ets')) {
          forEachChild(sourceFile, visitChildNode);
        }

        let isRemoteHarFile = isRemoteHar(fileName);
        forEachChild(sourceFile, node => visitProjectExport(node, isRemoteHarFile));
        forEachChild(sourceFile, visitProjectNode);
        collectEnumMembersOfFile(sourceFile);
        mCurrentExportedPropertySet = handleWhiteListWhenExportObfs(fileName, mCurrentExportedPropertySet);
        mCurrentExportNameSet = handleWhiteListWhenExportObfs(fileName, mCurrentExportNameSet);
        break;
      case ApiType.CONSTRUCTOR_PROPERTY:
        forEachChild(sourceFile, visitNodeForConstructorProperty);
        collectEnumMembersOfFile(sourceFile);
        break;
      default:
        break;
    }
  }

  function handleWhiteListWhenExportObfs(fileName: string, collectedExportNamesAndProperties: Set<string>): Set<string> {
    // If mExportObfuscation is not enabled, collect the export names and their properties into the whitelist.
    if (!scanProjectConfig.mExportObfuscation) {
      return collectedExportNamesAndProperties;
    }
    // If the current file is a keep file or its dependent file, collect the export names and their properties into the whitelist.
    if (scanProjectConfig.mkeepFilesAndDependencies?.has(fileName)) {
      return collectedExportNamesAndProperties;
    }
    // If it is a project source code file, the names and their properties of the export will not be collected.
    if (!isRemoteHar(fileName)) {
      collectedExportNamesAndProperties.clear();
      return collectedExportNamesAndProperties;
    }
    // If it is a third-party library file.
    return collectedExportNamesAndProperties;
  }

  const projectExtensions: string[] = ['.ets', '.ts', '.js'];
  const projectDependencyExtensions: string[] = ['.d.ets', '.d.ts', '.ets', '.ts', '.js'];
  const resolvedModules = new Set();

  function tryGetPackageID(filePath: string): string {
    const ohPackageJsonPath = path.join(filePath, 'oh-package.json5');
    let packgeNameAndVersion = '';
    if (fs.existsSync(ohPackageJsonPath)) {
      const ohPackageContent = json5.parse(fs.readFileSync(ohPackageJsonPath, 'utf-8'));
      packgeNameAndVersion = ohPackageContent.name + ohPackageContent.version;
    }
    return packgeNameAndVersion;
  }

  function traverseFilesInDir(apiPath: string, apiType: ApiType): void {
    let fileNames: string[] = fs.readdirSync(apiPath);
    for (let fileName of fileNames) {
      let filePath: string = path.join(apiPath, fileName);
      try {
        fs.accessSync(filePath, fs.constants.R_OK);
      } catch (err) {
        continue;
      }
      if (fs.statSync(filePath).isDirectory()) {
        const packgeNameAndVersion = tryGetPackageID(filePath);
        if (resolvedModules.has(packgeNameAndVersion)) {
          continue;
        }
        traverseApiFiles(filePath, apiType);
        packgeNameAndVersion.length > 0 && resolvedModules.add(packgeNameAndVersion);
        continue;
      }
      const suffix: string = path.extname(filePath);
      if ((apiType !== ApiType.PROJECT) && !projectDependencyExtensions.includes(suffix)) {
        continue;
      }

      if (apiType === ApiType.PROJECT && !projectExtensions.includes(suffix)) {
        continue;
      }
      parseFile(filePath, apiType);
    }
  }

  /**
   * traverse files of  api directory
   * @param apiPath api directory path
   * @param apiType
   * @private
   */
  export const traverseApiFiles = function (apiPath: string, apiType: ApiType): void {
    if (fs.statSync(apiPath).isDirectory()) {
      traverseFilesInDir(apiPath, apiType);
    } else {
      parseFile(apiPath, apiType);
    }
  };

  /**
   * desc: parse openHarmony sdk to get api list
   * @param version version of api, e.g. version 5.0.1.0 for api 9
   * @param sdkPath sdk real path of openHarmony
   * @param isEts true for ets, false for js
   * @param outputDir: sdk api output directory
   */
  export function parseOhSdk(sdkPath: string, version: string, isEts: boolean, outputDir: string): void {
    mPropertySet.clear();

    // visit api directory
    const apiPath: string = path.join(sdkPath, (isEts ? 'ets' : 'js'), version, 'api');
    traverseApiFiles(apiPath, ApiType.API);

    // visit component directory if ets
    if (isEts) {
      const componentPath: string = path.join(sdkPath, 'ets', version, 'component');
      traverseApiFiles(componentPath, ApiType.COMPONENT);
    }

    // visit the UI conversion API
    const uiConversionPath: string = path.join(sdkPath, (isEts ? 'ets' : 'js'), version,
      'build-tools', 'ets-loader', 'lib', 'pre_define.js');
    extractStringsFromFile(uiConversionPath);

    const reservedProperties: string[] = [...mPropertySet.values()];
    mPropertySet.clear();

    writeToFile(reservedProperties, path.join(outputDir, 'propertiesReserved.json'));
  }

  export function extractStringsFromFile(filePath: string): void {
    let collections: string[] = [];
    const fileContent = fs.readFileSync(filePath, 'utf-8');
    const regex = /"([^"]*)"/g;
    const matches = fileContent.match(regex);

    if (matches) {
      collections = matches.map(match => match.slice(1, -1));
    }

    collections.forEach(name => mPropertySet.add(name));
  }

  /**
   * save api json object to file
   * @private
   */
  export function writeToFile(reservedProperties: string[], outputPath: string): void {
    let str: string = JSON.stringify(reservedProperties, null, '\t');
    fs.writeFileSync(outputPath, str);
  }

  export function isRemoteHar(filePath: string): boolean {
    const realPath: string = sys.realpath(filePath);
    return isInOhModuleFile(realPath);
  }

  export function isInOhModuleFile(filePath: string): boolean {
    return filePath.indexOf('/oh_modules/') !== -1 || filePath.indexOf('\\oh_modules\\') !== -1;
  }

  export function isParsableFile(path: string): boolean {
    return supportedParsingExtension.some(extension => path.endsWith(extension));
  }

  /**
  * parse common project or file to extract exported api list
  * @return reserved api names
  */
  export function parseFileByPaths(projectPaths: Set<string>, scanningApiType: ApiType):
    {reservedExportPropertyAndName: Set<string> | undefined; reservedExportNames: Set<string> | undefined} {
    mPropertySet.clear();
    mExportNames.clear();
    projectPaths.forEach(path => {
      parseFile(path, scanningApiType);
    });
    let reservedExportPropertyAndName: Set<string>;
    let reservedExportNames: Set<string>;
    if (scanProjectConfig.mPropertyObfuscation) {
      reservedExportPropertyAndName = new Set(mPropertySet);
    }
    if (scanProjectConfig.mExportObfuscation) {
      reservedExportNames = new Set(mExportNames);
    }
    mPropertySet.clear();
    mExportNames.clear();
    return {
      reservedExportPropertyAndName: reservedExportPropertyAndName,
      reservedExportNames: reservedExportNames
    };
  }

  /**
   * Collect all property names in the AST.
   * @param astNode Nodes of the AST.
   */
  function collectPropertyNames(astNode: Node): void {
    visitElementsWithProperties(astNode);
  }

  /**
   * Visit elements that can contain properties.
   * @param node The current AST node.
   */
  function visitElementsWithProperties(node: Node): void {
    switch (node.kind) {
      case SyntaxKind.ClassDeclaration:
        forEachChild(node, visitClass);
        break;
      case SyntaxKind.InterfaceDeclaration:
      case SyntaxKind.TypeLiteral:
        forEachChild(node, visitInterfaceOrType);
        break;
      case SyntaxKind.EnumDeclaration:
        forEachChild(node, visitEnum);
        break;
      case SyntaxKind.ObjectLiteralExpression:
        forEachChild(node, visitObjectLiteral);
        break;
      case SyntaxKind.ModuleDeclaration:
        forEachChild(node, visitModule);
        break;
    }
    forEachChild(node, visitElementsWithProperties);
  }

  function visitClass(node: Node): void {
    if (isPropertyDeclaration(node) || isMethodDeclaration(node)) {
      if (isIdentifier(node.name)) {
        mCurrentExportedPropertySet.add(node.name.text);
      }
    }
    forEachChild(node, visitClass);
  }

  function visitInterfaceOrType(node: Node): void {
    if (isPropertySignature(node) || isMethodSignature(node)) {
      if (isIdentifier(node.name)) {
        mCurrentExportedPropertySet.add(node.name.text);
      }
    }
    forEachChild(node, visitInterfaceOrType);
  }

  function visitEnum(node: Node): void {
    if (isEnumMember(node) && isIdentifier(node.name)) {
      mCurrentExportedPropertySet.add(node.name.text);
    }
  }

  function visitObjectLiteral(node: Node): void {
    if (isPropertyAssignment(node)) {
      if (isIdentifier(node.name)) {
        mCurrentExportedPropertySet.add(node.name.text);
      }
    }
    forEachChild(node, visitObjectLiteral);
  }

  function visitModule(node: Node): void {
    forEachChild(node, visitElementsWithProperties);
  }

  function collectNodeName(name: string): void {
    mCurrentExportNameSet.add(name);
    mCurrentExportedPropertySet.add(name);
  }

  function containsIdentifier(node: Node, found: { value: boolean }): void {
    if (found.value) {
      return;
    }
    if (isIdentifier(node)) {
      found.value = true;
      return;
    }
    forEachChild(node, childNode => {
      containsIdentifier(childNode, found);
    });
  }

  function shouldCollectEnum(node: EnumDeclaration): boolean {
    const members = node.members;
    for (const member of members) {
      if (isEnumMember(member) && member.initializer) {
        const initializer = member.initializer;
        const found = { value: false };
        containsIdentifier(initializer, found);
        if (found.value) {
          return true;
        }
      }
    }
    return false;
  }

  function collectEnumMember(node: Node): void {
    if (isEnumMember(node) && isIdentifier(node.name)) {
      mEnumMemberSet.add(node.name.text);
      projectWhiteListManager?.fileWhiteListInfo?.fileReservedInfo.enumProperties.add(node.name.text);
    }
  }

  /**
   * Visit and collect enum members
   * @param node The current AST node.
   */
  function visitEnumMembers(node: Node): void {
    if (isEnumDeclaration(node)) {
      if (!shouldCollectEnum(node)) {
        return;
      }
      for (const member of node.members) {
        collectEnumMember(member);
      }
      return;
    }
    forEachChild(node, visitEnumMembers);
  }

  /**
   * Visit and collect enum members of non-js file
   * @param sourceFile The sourceFile to collect.
   */
  function collectEnumMembersOfFile(sourceFile: SourceFile): void {
    if (sourceFile.fileName.endsWith(Extension.Js)) {
      return;
    }
    forEachChild(sourceFile, visitEnumMembers);
  }

  function collectNamesWithAtKeep(node: Node, sourceFile: SourceFile): void {
    switch (node.kind) {
      case SyntaxKind.ClassDeclaration:
        collectClassDeclaration(node as ClassDeclaration, sourceFile);
        break;
      case SyntaxKind.InterfaceDeclaration:
        collectInterfaceDeclaration(node as InterfaceDeclaration, sourceFile);
        break;
      case SyntaxKind.EnumDeclaration:
        collectEnumDeclaration(node as EnumDeclaration, sourceFile);
        break;
      case SyntaxKind.FunctionDeclaration:
        collectFunctionDeclaration(node as FunctionDeclaration, sourceFile);
        break;
      case SyntaxKind.VariableStatement:
        collectVariableDeclararion(node as VariableStatement, sourceFile);
        break;
      case SyntaxKind.ModuleDeclaration:
        collectModuleDeclaration(node as ModuleDeclaration, sourceFile);
        break;
      case SyntaxKind.AnnotationDeclaration:
        collectAnnotationDeclaration(node as AnnotationDeclaration, sourceFile);
        break;
    }
    forEachChild(node, child => collectNamesWithAtKeep(child, sourceFile));
  }

  function collectClassDeclaration(node: ClassDeclaration, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);

    if (atKeepType === AtKeepType.KeepAsConsumer) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectClassDeclarationMembers(node, atKeepType);
    } else if (atKeepType === AtKeepType.KeepSymbol) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectClassDeclarationMembers(node, atKeepType);
      scanAndCollectClassDeclarationMembers(node, sourceFile, isToplevel, isExported);
    } else { // atKeepType === AtKeepType.None
      scanAndCollectClassDeclarationMembers(node, sourceFile, isToplevel, isExported);
    }
  }

  function collectClassDeclarationMembers(node: ClassDeclaration, atKeepType: AtKeepType): void {
    for (const member of node.members) {
      collectClassMemberNames(member, atKeepType);
      collectParameterPropertyNames(member, atKeepType);
    }
  }

  function collectClassMemberNames(member: ClassElement, atKeepType: AtKeepType): void {
    if (isPropertyDeclaration(member) || isMethodDeclaration(member) || isGetAccessor(member) || isSetAccessor(member)) {
      if (isIdentifier(member.name)) {
        collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
      }
    }
  }

  function collectParameterPropertyNames(member: ClassElement, atKeepType: AtKeepType): void {
    if (isConstructorDeclaration(member)) {
      member.parameters.forEach((param) => {
        if (isParameterPropertyDeclaration(param, member) && isIdentifier(param.name)) {
          collectAtKeepNamesByType(param.name.text, atKeepType, WhiteListType.PropertyName);
        }
      });
    }
  }

  function scanAndCollectClassDeclarationMembers(node: ClassDeclaration, sourceFile: SourceFile, isToplevel: boolean, isExported: boolean): void {
    let shouldKeepClassName: boolean = false;
    let atKeepTypeOfClass: AtKeepType = AtKeepType.KeepSymbol;
    for (const member of node.members) {
      const atKeepType: AtKeepType = getAtKeepType(member, sourceFile);
      if (atKeepType === AtKeepType.None) {
        continue;
      }
      if (atKeepType === AtKeepType.KeepAsConsumer) {
        atKeepTypeOfClass = AtKeepType.KeepAsConsumer;
      }
      if (isPropertyDeclaration(member) || isMethodDeclaration(member) || isGetAccessor(member) || isSetAccessor(member)) {
        if (isIdentifier(member.name)) {
          shouldKeepClassName = true;
          collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
        }
      }
      if (isConstructorDeclaration(member)) {
        shouldKeepClassName = true;
      }
    }
    if (shouldKeepClassName) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepTypeOfClass);
    }
  }

  function collectInterfaceDeclaration(node: InterfaceDeclaration, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);

    if (atKeepType === AtKeepType.KeepAsConsumer) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectInterfaceDeclarationMembers(node, atKeepType);
    } else if (atKeepType === AtKeepType.KeepSymbol) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectInterfaceDeclarationMembers(node, atKeepType);
      scanAndCollectInterfaceDeclarationMembers(node, sourceFile, isToplevel, isExported);
    } else { // atKeepType === AtKeepType.None
      scanAndCollectInterfaceDeclarationMembers(node, sourceFile, isToplevel, isExported);
    }
  }

  function collectInterfaceDeclarationMembers(node: InterfaceDeclaration, atKeepType: AtKeepType): void {
    for (const member of node.members) {
      if (isPropertySignature(member) || isMethodSignature(member)) {
        if (isIdentifier(member.name)) {
          collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
        }
      }
    }
  }

  function scanAndCollectInterfaceDeclarationMembers(node: InterfaceDeclaration, sourceFile: SourceFile, isToplevel: boolean, isExported: boolean): void {
    let shouldKeepInterfaceName: boolean = false;
    let atKeepTypeOfInterface: AtKeepType = AtKeepType.KeepSymbol;
    for (const member of node.members) {
      const atKeepType: AtKeepType = getAtKeepType(member, sourceFile);
      if (atKeepType === AtKeepType.None) {
        continue;
      }
      if (atKeepType === AtKeepType.KeepAsConsumer) {
        atKeepTypeOfInterface = AtKeepType.KeepAsConsumer;
      }
      if (isPropertySignature(member) || isMethodSignature(member)) {
        if (isIdentifier(member.name)) {
          shouldKeepInterfaceName = true;
          collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
        }
      }
    }
    if (shouldKeepInterfaceName) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepTypeOfInterface);
    }
  }

  function collectEnumDeclaration(node: EnumDeclaration, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);

    if (atKeepType === AtKeepType.KeepAsConsumer) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectEnumDeclarationMembers(node, atKeepType);
    } else if (atKeepType === AtKeepType.KeepSymbol) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
      collectEnumDeclarationMembers(node, atKeepType);
      scanAndCollectEnumDeclarationMembers(node, sourceFile, isToplevel, isExported);
    } else { // atKeepType === AtKeepType.None
      scanAndCollectEnumDeclarationMembers(node, sourceFile, isToplevel, isExported);
    }
  }

  function collectEnumDeclarationMembers(node: EnumDeclaration, atKeepType: AtKeepType): void {
    for (const member of node.members) {
      if (isEnumMember(member)) {
        if (isIdentifier(member.name)) {
          collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
        }
      }
    }
  }

  function scanAndCollectEnumDeclarationMembers(node: EnumDeclaration, sourceFile: SourceFile, isToplevel: boolean, isExported: boolean): void {
    let shouldKeepEnumName: boolean = false;
    let atKeepTypeOfEnum: AtKeepType = AtKeepType.KeepSymbol;
    for (const member of node.members) {
      const atKeepType: AtKeepType = getAtKeepType(member, sourceFile);
      if (atKeepType === AtKeepType.None) {
        continue;
      }
      if (atKeepType === AtKeepType.KeepAsConsumer) {
        atKeepTypeOfEnum = AtKeepType.KeepAsConsumer;
      }
      if (isEnumMember(member)) {
        if (isIdentifier(member.name)) {
          shouldKeepEnumName = true;
          collectAtKeepNamesByType(member.name.text, atKeepType, WhiteListType.PropertyName);
        }
      }
    }
    if (shouldKeepEnumName) {
      collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepTypeOfEnum);
    }
  }

  function collectFunctionDeclaration(node: FunctionDeclaration, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);
    collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
  }

  function collectVariableDeclararion(node: VariableStatement, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);
    node.declarationList.forEachChild((child) => {
      if (isVariableDeclaration(child)) {
        collectToplevelOrExportedNames(child, isToplevel, isExported, atKeepType);
      }
    });
  }

  function collectModuleDeclaration(node: ModuleDeclaration, sourceFile: SourceFile, atKeepTypeOuter?: AtKeepType): void {
    let atKeepType: AtKeepType;
    if (atKeepTypeOuter) {
      atKeepType = atKeepTypeOuter;
    } else {
      atKeepType = getAtKeepType(node, sourceFile);
    }
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);
    collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);

    if (atKeepType !== AtKeepType.None && isModuleBlock(node.body)) {
      node.body.statements.forEach((child) => {
        collectModuleChild(child, atKeepType, sourceFile);
      });
    }
  }

  function collectModuleChild(child: Node, atKeepType: AtKeepType, sourceFile: SourceFile): void {
    const isExportedChild: boolean = hasExportModifier(child);
    if (!isExportedChild) {
      return;
    }

    switch (child.kind) {
      case SyntaxKind.ClassDeclaration:
      case SyntaxKind.InterfaceDeclaration:
      case SyntaxKind.EnumDeclaration:
      case SyntaxKind.FunctionDeclaration:
      case SyntaxKind.TypeAliasDeclaration:
        collectToplevelOrExportedNames(child as KeepTargetNode, false, true, atKeepType);
        break;
      case SyntaxKind.ModuleDeclaration:
        collectToplevelOrExportedNames(child as ModuleDeclaration, false, true, atKeepType);
        collectModuleDeclaration(child as ModuleDeclaration, sourceFile, atKeepType);
        break;
      case SyntaxKind.VariableStatement:
        (child as VariableStatement).declarationList.forEachChild((variableDeclaration) => {
          if (isVariableDeclaration(variableDeclaration)) {
            collectToplevelOrExportedNames(variableDeclaration, false, true, atKeepType);
          }
        });
        break;
    }
  }

  function collectAnnotationDeclaration(node: AnnotationDeclaration, sourceFile: SourceFile): void {
    const atKeepType: AtKeepType = getAtKeepType(node, sourceFile);
    const isToplevel: boolean = isSourceFile(node.parent);
    const isExported: boolean = hasExportModifier(node);
    collectToplevelOrExportedNames(node, isToplevel, isExported, atKeepType);
  }

  function getAtKeepType(node: Node, sourceFile: SourceFile): AtKeepType {
    const ranges: CommentRange[] | undefined = getLeadingCommentRangesOfNode(node, sourceFile);
    let atKeepType: AtKeepType = AtKeepType.None;
    if (!ranges?.length) {
      return atKeepType;
    }
    for (const range of ranges) {
      if (range.kind !== SyntaxKind.SingleLineCommentTrivia) {
        continue;
      }
      const comment: string = sourceFile.text.slice(range.pos, range.end).replace(/\s+/g, '');
      if (comment === KEEP_AS_CONSUMER) {
        atKeepType = AtKeepType.KeepAsConsumer;
        return atKeepType;
      }
      if (comment === KEEP_SYMBOL) {
        atKeepType = AtKeepType.KeepSymbol;
      }
    }
    return atKeepType;
  }

  function collectToplevelOrExportedNames(node: KeepTargetNode, isToplevel: boolean, isExported: boolean, atKeepType: AtKeepType): void {
    if ((!isToplevel && !isExported) || !isIdentifier(node.name)) {
      return;
    }

    collectAtKeepNamesByType(node.name.text, atKeepType, WhiteListType.GlobalName);

    if (isExported) {
      collectAtKeepNamesByType(node.name.text, atKeepType, WhiteListType.PropertyName);
    }
  }

  function collectAtKeepNamesByType(name: string, atKeepType: AtKeepType, whiteListType: WhiteListType): void {
    if (atKeepType === AtKeepType.None) {
      return;
    }

    const targetCollection = atKeepType === AtKeepType.KeepSymbol
      ? keepSymbolTemp
      : keepAsConsumerTemp;

    updateKeepCollection(targetCollection, whiteListType, name);
  }

  function updateKeepCollection(collector: KeepInfo, whiteListType: WhiteListType, name: string): void {
    if (whiteListType === WhiteListType.PropertyName) {
      collector.propertyNames.add(name);
    } else {
      collector.globalNames.add(name);
    }
  }

  function ensureEntry<K, V>(map: Map<K, V>, key: K, createValue: () => V): void {
    if (!map.has(key)) {
      map.set(key, createValue());
    }
  }

  function isWhiteListedDecorator(name: string): boolean {
    return DECORATOR_WHITE_LIST.includes(name);
  }

  function getDecoratorName(expr: Expression): string | undefined {
    if (isCallExpression(expr)) {
      return expr.expression.getText();
    }
    if (isIdentifier(expr)) {
      return expr.text;
    }
    return undefined;
  }
}