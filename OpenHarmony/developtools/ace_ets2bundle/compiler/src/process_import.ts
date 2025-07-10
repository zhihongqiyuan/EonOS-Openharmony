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
import fs from 'fs';
import path from 'path';
import JSON5 from 'json5';

import {
  EXTNAME_ETS,
  EXTNAME_TS,
  INDEX_ETS,
  INDEX_TS,
  CUSTOM_COMPONENT_DEFAULT,
  CUSTOM_DECORATOR_NAME,
  COMPONENT_DECORATOR_ENTRY,
  COMPONENT_BUILDER_DECORATOR,
  DECORATOR_REUSEABLE,
  DECORATOR_REUSABLE_V2
} from './pre_define';
import {
  propertyCollection,
  linkCollection,
  componentCollection,
  preprocessExtend,
  preprocessNewExtend,
  processSystemApi,
  propCollection,
  isObservedClass,
  isCustomDialogClass,
  observedClassCollection,
  enumCollection,
  getComponentSet,
  IComponentSet,
  builderParamObjectCollection,
  stateCollection,
  regularCollection,
  storagePropCollection,
  storageLinkCollection,
  provideCollection,
  consumeCollection,
  objectLinkCollection,
  localStorageLinkCollection,
  localStoragePropCollection,
  builderParamInitialization,
  propInitialization,
  regularInitialization,
  stateInitialization,
  provideInitialization,
  privateCollection,
  regularStaticCollection
} from './validate_ui_syntax';
import {
  getExtensionIfUnfullySpecifiedFilepath,
  hasDecorator,
  LogInfo,
  LogType,
  storedFileInfo
} from './utils';
import {
  projectConfig,
  sdkConfigs,
  sdkConfigPrefix,
  globalProgram
} from '../main';
import {
  CUSTOM_BUILDER_METHOD,
  INNER_COMPONENT_NAMES,
  GLOBAL_CUSTOM_BUILDER_METHOD
} from './component_map';
import { 
  type ResolveModuleInfo,
  SOURCE_FILES
} from './ets_checker';
import { 
  getRealModulePath,
  validateModuleSpecifier
} from './fast_build/system_api/api_check_utils';
import processStructComponentV2, { StructInfo } from './process_struct_componentV2';

const IMPORT_FILE_ASTCACHE: Map<string, ts.SourceFile> =
  process.env.watchMode === 'true' ? new Map() : (SOURCE_FILES || new Map());

export default function processImport(node: ts.ImportDeclaration | ts.ImportEqualsDeclaration |
  ts.ExportDeclaration, pagesDir: string, log: LogInfo[], asName: Map<string, string> = new Map(),
  isEntryPage: boolean = true, pathCollection: Set<string> = new Set()): void {
  let filePath: string;
  let defaultName: string;
  if (ts.isImportDeclaration(node) || ts.isExportDeclaration(node)) {
    filePath = node.moduleSpecifier.getText().replace(/'|"/g, '');
    if (ts.isImportDeclaration(node) && node.importClause && node.importClause.name &&
      ts.isIdentifier(node.importClause.name)) {
      defaultName = node.importClause.name.escapedText.toString();
      if (isEntryPage) {
        asName.set(defaultName, defaultName);
      }
    }
    if (ts.isImportDeclaration(node) && node.importClause && node.importClause.namedBindings &&
      ts.isNamedImports(node.importClause.namedBindings) &&
      node.importClause.namedBindings.elements && isEntryPage) {
      validateModuleSpecifier(node.moduleSpecifier, log);
      node.importClause.namedBindings.elements.forEach(item => {
        if (item.name && ts.isIdentifier(item.name)) {
          validateModuleName(item.name, log);
          if (item.propertyName && ts.isIdentifier(item.propertyName) && asName) {
            asName.set(item.propertyName.escapedText.toString(), item.name.escapedText.toString());
          } else {
            asName.set(item.name.escapedText.toString(), item.name.escapedText.toString());
          }
        }
      });
    }
  } else {
    if (node.moduleReference && ts.isExternalModuleReference(node.moduleReference) &&
      node.moduleReference.expression && ts.isStringLiteral(node.moduleReference.expression)) {
      filePath = node.moduleReference.expression.text;
      defaultName = node.name.escapedText.toString();
      if (isEntryPage) {
        asName.set(defaultName, defaultName);
      }
    }
  }

  try {
    const fileResolvePath: string = getFileFullPath(filePath, pagesDir);
    if (fs.existsSync(fileResolvePath) && fs.statSync(fileResolvePath).isFile() &&
      !pathCollection.has(fileResolvePath)) {
      let sourceFile: ts.SourceFile;
      pathCollection.add(fileResolvePath);
      if (IMPORT_FILE_ASTCACHE.has(fileResolvePath)) {
        sourceFile = IMPORT_FILE_ASTCACHE.get(fileResolvePath);
      } else {
        sourceFile = generateSourceFileAST(fileResolvePath, filePath);
        IMPORT_FILE_ASTCACHE[fileResolvePath] = sourceFile;
      }
      visitAllNode(sourceFile, sourceFile, defaultName, asName, path.dirname(fileResolvePath), log,
        new Set(), new Set(), new Set(), new Map(), pathCollection, fileResolvePath, /\.d\.ets$/.test(fileResolvePath));
    }
  } catch (e) {
    // ignore
  }
}

function generateSourceFileAST(fileResolvePath: string, filePath: string): ts.SourceFile {
  const originContent: string = fs.readFileSync(fileResolvePath, { encoding: 'utf-8' });
  const content: string = path.extname(fileResolvePath) === EXTNAME_ETS ?
    preprocessNewExtend(preprocessExtend(processSystemApi(originContent))) : originContent;
  return ts.createSourceFile(fileResolvePath, content, ts.ScriptTarget.Latest, true, ts.ScriptKind.ETS);
}

type structDecoratorResult = {
  hasRecycle: boolean
};

const MODIFIER_LENGTH: number = 2;

function visitAllNode(node: ts.Node, sourceFile: ts.SourceFile, defaultNameFromParent: string,
  asNameFromParent: Map<string, string>, pagesDir: string, log: LogInfo[], entryCollection: Set<string>,
  exportCollection: Set<string>, defaultCollection: Set<string>, asExportCollection: Map<string, string>,
  pathCollection: Set<string>, fileResolvePath: string, isDETS: boolean): void {
  if (isObservedClass(node)) {
    collectSpecialFunctionNode(node as ts.ClassDeclaration, asNameFromParent, defaultNameFromParent, defaultCollection,
      asExportCollection, observedClassCollection);
    // @ts-ignore
    observedClassCollection.add(node.name.getText());
  }
  if (isCustomDialogClass(node)) {
    collectSpecialFunctionNode(node as ts.StructDeclaration, asNameFromParent, defaultNameFromParent, defaultCollection,
      asExportCollection, componentCollection.customDialogs);
    // @ts-ignore
    componentCollection.customDialogs.add(node.name.getText());
  }
  if (ts.isEnumDeclaration(node) && node.name) {
    enumCollection.add(node.name.getText());
  }
  const structDecorator: structDecoratorResult = { hasRecycle: false };
  if (ts.isStructDeclaration(node) && ts.isIdentifier(node.name) && isCustomComponent(node, structDecorator)) {
    addDependencies(node, defaultNameFromParent, asNameFromParent, isDETS, structDecorator);
    isExportEntry(node, log, entryCollection, exportCollection, defaultCollection, fileResolvePath, sourceFile);
    if (asExportCollection.has(node.name.getText())) {
      componentCollection.customComponents.add(asExportCollection.get(node.name.getText()));
      if (isDETS) {
        storedFileInfo.getCurrentArkTsFile().compFromDETS.add(asExportCollection.get(node.name.getText()));
      }
      if (structDecorator.hasRecycle) {
        storedFileInfo.getCurrentArkTsFile().recycleComponents.add(asExportCollection.get(node.name.getText()));
      }
    }
    const modifiers: readonly ts.Modifier[] =
      ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
    if (modifiers && modifiers.length >= MODIFIER_LENGTH && modifiers[0] &&
      modifiers[0].kind === ts.SyntaxKind.ExportKeyword && modifiers[1] &&
      modifiers[1].kind === ts.SyntaxKind.DefaultKeyword) {
      if (!defaultNameFromParent && hasCollection(node.name)) {
        addDefaultExport(node, isDETS, structDecorator);
      } else if (defaultNameFromParent && asNameFromParent.has(defaultNameFromParent)) {
        componentCollection.customComponents.add(asNameFromParent.get(defaultNameFromParent));
        if (isDETS) {
          storedFileInfo.getCurrentArkTsFile().compFromDETS.add(asNameFromParent.get(defaultNameFromParent));
        }
        if (structDecorator.hasRecycle) {
          storedFileInfo.getCurrentArkTsFile().recycleComponents.add(asNameFromParent.get(defaultNameFromParent));
        }
      }
    }
    if (defaultCollection.has(node.name.getText())) {
      componentCollection.customComponents.add(CUSTOM_COMPONENT_DEFAULT);
      if (isDETS) {
        storedFileInfo.getCurrentArkTsFile().compFromDETS.add(CUSTOM_COMPONENT_DEFAULT);
      }
      if (structDecorator.hasRecycle) {
        storedFileInfo.getCurrentArkTsFile().recycleComponents.add(CUSTOM_COMPONENT_DEFAULT);
      }
    }
  }
  if (ts.isFunctionDeclaration(node) && hasDecorator(node, COMPONENT_BUILDER_DECORATOR)) {
    collectSpecialFunctionNode(node, asNameFromParent, defaultNameFromParent, defaultCollection,
      asExportCollection, CUSTOM_BUILDER_METHOD);
    collectSpecialFunctionNode(node, asNameFromParent, defaultNameFromParent, defaultCollection,
      asExportCollection, GLOBAL_CUSTOM_BUILDER_METHOD);
  }
  if (ts.isExportAssignment(node) && node.expression && ts.isIdentifier(node.expression) &&
    hasCollection(node.expression)) {
    if (defaultNameFromParent) {
      const propertiesName: string = node.expression.escapedText.toString();
      setDependencies(defaultNameFromParent, undefined, linkCollection.get(propertiesName),
        propertyCollection.get(propertiesName), propCollection.get(propertiesName),
        builderParamObjectCollection.get(propertiesName), stateCollection.get(propertiesName),
        regularCollection.get(propertiesName), storagePropCollection.get(propertiesName),
        storageLinkCollection.get(propertiesName), provideCollection.get(propertiesName),
        consumeCollection.get(propertiesName), objectLinkCollection.get(propertiesName),
        localStorageLinkCollection.get(propertiesName), localStoragePropCollection.get(propertiesName),
        builderParamInitialization.get(propertiesName), propInitialization.get(propertiesName), isDETS,
        structDecorator);
    }
    addDefaultExport(node, isDETS, structDecorator);
  }
  if (ts.isExportAssignment(node) && node.expression && ts.isIdentifier(node.expression)) {
    if (defaultNameFromParent) {
      asNameFromParent.set(node.expression.getText(), asNameFromParent.get(defaultNameFromParent));
    }
    defaultCollection.add(node.expression.getText());
  }
  if (ts.isExportDeclaration(node) && node.exportClause &&
    ts.isNamedExports(node.exportClause) && node.exportClause.elements) {
    node.exportClause.elements.forEach(item => {
      if (process.env.watchMode === 'true') {
        exportCollection.add((item.propertyName ? item.propertyName : item.name).escapedText.toString());
      }
      if (item.name && ts.isIdentifier(item.name)) {
        if (!item.propertyName) {
          asExportCollection.set(item.name.escapedText.toString(), item.name.escapedText.toString());
        } else if (item.propertyName && ts.isIdentifier(item.propertyName)) {
          validateModuleName(item.name, log, sourceFile, fileResolvePath);
          if (hasCollection(item.propertyName)) {
            let asExportName: string = item.name.escapedText.toString();
            const asExportPropertyName: string = item.propertyName.escapedText.toString();
            if (asNameFromParent.has(asExportName)) {
              asExportName = asNameFromParent.get(asExportName);
            }
            setDependencies(asExportName, undefined, linkCollection.get(asExportPropertyName),
              propertyCollection.get(asExportPropertyName),
              propCollection.get(asExportPropertyName),
              builderParamObjectCollection.get(asExportPropertyName),
              stateCollection.get(asExportPropertyName), regularCollection.get(asExportPropertyName),
              storagePropCollection.get(asExportPropertyName), storageLinkCollection.get(asExportPropertyName),
              provideCollection.get(asExportPropertyName), consumeCollection.get(asExportPropertyName),
              objectLinkCollection.get(asExportPropertyName), localStorageLinkCollection.get(asExportPropertyName),
              localStoragePropCollection.get(asExportPropertyName), builderParamInitialization.get(asExportPropertyName),
              propInitialization.get(asExportPropertyName), isDETS, structDecorator);
          }
          asExportCollection.set(item.propertyName.escapedText.toString(), item.name.escapedText.toString());
        }
      }
      if (item.name && ts.isIdentifier(item.name) && asNameFromParent.has(item.name.escapedText.toString()) &&
        item.propertyName && ts.isIdentifier(item.propertyName)) {
        asNameFromParent.set(item.propertyName.escapedText.toString(),
          asNameFromParent.get(item.name.escapedText.toString()));
      }
    });
  }
  if (ts.isExportDeclaration(node) && node.moduleSpecifier &&
    ts.isStringLiteral(node.moduleSpecifier)) {
    if (process.env.watchMode === 'true' && node.exportClause && ts.isNamedExports(node.exportClause) &&
      node.exportClause.elements) {
      node.exportClause.elements.forEach(item => {
        exportCollection.add((item.propertyName ? item.propertyName : item.name).escapedText.toString());
        if (item.propertyName && ts.isIdentifier(item.propertyName) && item.name &&
          ts.isIdentifier(item.name) && asNameFromParent.has(item.name.escapedText.toString())) {
          asNameFromParent.set(item.propertyName.escapedText.toString(),
            asNameFromParent.get(item.name.escapedText.toString()));
          defaultCollection.add(item.name.escapedText.toString());
        }
      });
    }
    processImport(node, pagesDir, log, asNameFromParent, true, new Set(pathCollection));
  }
  if (ts.isImportDeclaration(node)) {
    if (node.importClause && node.importClause.name && ts.isIdentifier(node.importClause.name) &&
      asNameFromParent.has(node.importClause.name.getText())) {
      processImport(node, pagesDir, log, asNameFromParent, false, new Set(pathCollection));
    } else if (node.importClause && node.importClause.namedBindings &&
      ts.isNamedImports(node.importClause.namedBindings) && node.importClause.namedBindings.elements) {
      let nested: boolean = false;
      node.importClause.namedBindings.elements.forEach(item => {
        if (item.name && ts.isIdentifier(item.name) && asNameFromParent.has(item.name.escapedText.toString())) {
          nested = true;
          if (item.propertyName && ts.isIdentifier(item.propertyName)) {
            asNameFromParent.set(item.propertyName.escapedText.toString(),
              asNameFromParent.get(item.name.escapedText.toString()));
          }
        }
      });
      if (nested) {
        processImport(node, pagesDir, log, asNameFromParent, false, new Set(pathCollection));
      }
    }
  }
  node.getChildren().reverse().forEach((item: ts.Node) => visitAllNode(item, sourceFile,
    defaultNameFromParent, asNameFromParent, pagesDir, log, entryCollection, exportCollection,
    defaultCollection, asExportCollection, pathCollection, fileResolvePath, isDETS));
}

function collectSpecialFunctionNode(node: ts.FunctionDeclaration | ts.ClassDeclaration | ts.StructDeclaration,
  asNameFromParent: Map<string, string>, defaultNameFromParent: string, defaultCollection: Set<string>,
  asExportCollection: Map<string, string>, collection: Set<string>): void {
  const name: string = node.name.getText();
  const modifiers: readonly ts.Modifier[] = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
  if (asNameFromParent.has(name)) {
    collection.add(asNameFromParent.get(name));
  } else if (modifiers && modifiers.length >= 1 && modifiers[0] &&
    modifiers[0].kind === ts.SyntaxKind.ExportKeyword) {
    if (modifiers.length === 1) {
      collection.add(name);
    } else if (modifiers.length >= MODIFIER_LENGTH && modifiers[1] && modifiers[1].kind ===
      ts.SyntaxKind.DefaultKeyword) {
      collection.add(CUSTOM_COMPONENT_DEFAULT);
      if (defaultNameFromParent && asNameFromParent.has(defaultNameFromParent)) {
        collection.add(asNameFromParent.get(defaultNameFromParent));
      }
    }
  } else if (defaultCollection.has(name)) {
    collection.add(CUSTOM_COMPONENT_DEFAULT);
  } else if (asExportCollection.has(name)) {
    collection.add(asExportCollection.get(name));
  }
}

function isExportEntry(node: ts.StructDeclaration, log: LogInfo[], entryCollection: Set<string>,
  exportCollection: Set<string>, defaultCollection: Set<string>, fileResolvePath: string,
  sourceFile: ts.SourceFile): void {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (process.env.watchMode === 'true' && node && decorators) {
    let existExport: boolean = false;
    let existEntry: boolean = false;
    const modifiers: readonly ts.Modifier[] = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
    if (modifiers) {
      for (let i = 0; i < modifiers.length; i++) {
        if (modifiers[i].kind === ts.SyntaxKind.ExportKeyword) {
          existExport = true;
          break;
        }
      }
    }
    for (let i = 0; i < decorators.length; i++) {
      if (decorators[i].getText() === COMPONENT_DECORATOR_ENTRY) {
        entryCollection.add(node.name.escapedText.toString());
        existEntry = true;
        break;
      }
    }
  }
}

function addDependencies(node: ts.StructDeclaration, defaultNameFromParent: string,
  asNameFromParent: Map<string, string>, isDETS: boolean, structDecorator: structDecoratorResult): void {
  const componentName: string = node.name.getText();
  const ComponentSet: IComponentSet = getComponentSet(node, false);
  const modifiers: readonly ts.Modifier[] = ts.canHaveModifiers(node) ? ts.getModifiers(node) : undefined;
  if (defaultNameFromParent && modifiers && modifiers.length >= MODIFIER_LENGTH && modifiers[0] &&
    modifiers[1] && modifiers[0].kind === ts.SyntaxKind.ExportKeyword &&
    modifiers[1].kind === ts.SyntaxKind.DefaultKeyword) {
    setDependencies(defaultNameFromParent, undefined, ComponentSet.links, ComponentSet.properties,
      ComponentSet.props, ComponentSet.builderParams, ComponentSet.states, ComponentSet.regulars,
      ComponentSet.storageProps, ComponentSet.storageLinks, ComponentSet.provides,
      ComponentSet.consumes, ComponentSet.objectLinks, ComponentSet.localStorageLink,
      ComponentSet.localStorageProp, ComponentSet.builderParamData, ComponentSet.propData, isDETS,
      structDecorator);
  } else if (asNameFromParent.has(componentName)) {
    setDependencies(asNameFromParent.get(componentName), undefined, ComponentSet.links, ComponentSet.properties,
      ComponentSet.props, ComponentSet.builderParams, ComponentSet.states, ComponentSet.regulars,
      ComponentSet.storageProps, ComponentSet.storageLinks, ComponentSet.provides,
      ComponentSet.consumes, ComponentSet.objectLinks, ComponentSet.localStorageLink,
      ComponentSet.localStorageProp, ComponentSet.builderParamData, ComponentSet.propData, isDETS,
      structDecorator);
  } else {
    setDependencies(componentName, undefined, ComponentSet.links, ComponentSet.properties, ComponentSet.props,
      ComponentSet.builderParams, ComponentSet.states, ComponentSet.regulars,
      ComponentSet.storageProps, ComponentSet.storageLinks, ComponentSet.provides,
      ComponentSet.consumes, ComponentSet.objectLinks, ComponentSet.localStorageLink,
      ComponentSet.localStorageProp, ComponentSet.builderParamData, ComponentSet.propData, isDETS,
      structDecorator);
  }
}

function addDefaultExport(node: ts.StructDeclaration | ts.ExportAssignment, isDETS: boolean,
  structDecorator: structDecoratorResult): void {
  let name: string;
  if (ts.isStructDeclaration(node) && node.name && ts.isIdentifier(node.name)) {
    name = node.name.escapedText.toString();
  } else if (ts.isExportAssignment(node) && node.expression && ts.isIdentifier(node.expression)) {
    name = node.expression.escapedText.toString();
  } else {
    return;
  }
  setDependencies(CUSTOM_COMPONENT_DEFAULT, undefined,
    linkCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...linkCollection.get(CUSTOM_COMPONENT_DEFAULT), ...linkCollection.get(name)]) :
      linkCollection.get(name),
    propertyCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...propertyCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...propertyCollection.get(name)]) : propertyCollection.get(name),
    propCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...propCollection.get(CUSTOM_COMPONENT_DEFAULT), ...propCollection.get(name)]) :
      propCollection.get(name),
    builderParamObjectCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...builderParamObjectCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...builderParamObjectCollection.get(name)]) : builderParamObjectCollection.get(name),
    stateCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...stateCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...stateCollection.get(name)]) : stateCollection.get(name),
    regularCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...regularCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...regularCollection.get(name)]) : regularCollection.get(name),
    storagePropCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...storagePropCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...storagePropCollection.get(name)]) : storagePropCollection.get(name),
    storageLinkCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...storageLinkCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...storageLinkCollection.get(name)]) : storageLinkCollection.get(name),
    provideCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...provideCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...provideCollection.get(name)]) : provideCollection.get(name),
    consumeCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...consumeCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...consumeCollection.get(name)]) : consumeCollection.get(name),
    objectLinkCollection.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...objectLinkCollection.get(CUSTOM_COMPONENT_DEFAULT),
        ...objectLinkCollection.get(name)]) : objectLinkCollection.get(name),
    getNewLocalStorageMap(localStorageLinkCollection, name),
    getNewLocalStorageMap(localStoragePropCollection, name),
    builderParamInitialization.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...builderParamInitialization.get(CUSTOM_COMPONENT_DEFAULT),
        ...builderParamInitialization.get(name)]) : builderParamInitialization.get(name),
    propInitialization.has(CUSTOM_COMPONENT_DEFAULT) ?
      new Set([...propInitialization.get(CUSTOM_COMPONENT_DEFAULT),
        ...propInitialization.get(name)]) : propInitialization.get(name), isDETS,
    structDecorator
  );
}

function getNewLocalStorageMap(collection: Map<string, Map<string, Set<string>>>, name: string)
  : Map<string, Set<string>> {
  let localStorageLinkMap: Map<string, Set<string>> = new Map();
  if (collection.has(CUSTOM_COMPONENT_DEFAULT)) {
    const tempSet: Set<string> = new Set();
    if (collection.get(CUSTOM_COMPONENT_DEFAULT)) {
      for (const key of collection.get(CUSTOM_COMPONENT_DEFAULT).keys()) {
        tempSet.add(key);
      }
    }
    if (collection.get(name)) {
      for (const key of collection.get(name).keys()) {
        tempSet.add(key);
      }
    }
    localStorageLinkMap.set(name, tempSet);
  } else {
    localStorageLinkMap = collection.get(name);
  }
  return localStorageLinkMap;
}

function setDependencies(component: string, asComponentName: string, linkArray: Set<string>, propertyArray: Set<string>,
  propArray: Set<string>, builderParamArray: Set<string>, stateArray: Set<string>,
  regularArray: Set<string>, storagePropsArray: Set<string>, storageLinksArray: Set<string>,
  providesArray: Set<string>, consumesArray: Set<string>, objectLinksArray: Set<string>,
  localStorageLinkMap: Map<string, Set<string>>, localStoragePropMap: Map<string, Set<string>>,
  builderParamData: Set<string>, propData: Set<string>, isDETS: boolean,
  structDecorator: structDecoratorResult): void {
  if (asComponentName) {
    linkCollection.set(asComponentName, linkArray);
    storedFileInfo.overallLinkCollection.set(asComponentName, linkArray);
  } else if (!asComponentName && component) {
    linkCollection.set(component, linkArray);
    if (projectConfig.compileMode === 'esmodule' && process.env.compileTool === 'rollup') {
      storedFileInfo.overallLinkCollection.set(component, linkArray);
    }
  }
  propertyCollection.set(component, propertyArray);
  if (!propCollection.get(component)) {
    propCollection.set(component, propArray);
  }
  if (asComponentName) {
    storedFileInfo.overallBuilderParamCollection.set(asComponentName, builderParamArray);
  } else if (!asComponentName && component && projectConfig.compileMode === 'esmodule' &&
    process.env.compileTool === 'rollup') {
    storedFileInfo.overallBuilderParamCollection.set(component, builderParamArray);
  }
  builderParamObjectCollection.set(component, builderParamArray);
  componentCollection.customComponents.add(component);
  if (isDETS) {
    storedFileInfo.getCurrentArkTsFile().compFromDETS.add(component);
  }
  if (structDecorator.hasRecycle) {
    storedFileInfo.getCurrentArkTsFile().recycleComponents.add(component);
  }
  stateCollection.set(component, stateArray);
  regularCollection.set(component, regularArray);
  storagePropCollection.set(component, storagePropsArray);
  storageLinkCollection.set(component, storageLinksArray);
  provideCollection.set(component, providesArray);
  consumeCollection.set(component, consumesArray);
  if (asComponentName) {
    storedFileInfo.overallObjectLinkCollection.set(asComponentName, objectLinksArray);
  } else if (!asComponentName && component && projectConfig.compileMode === 'esmodule' &&
    process.env.compileTool === 'rollup') {
    storedFileInfo.overallObjectLinkCollection.set(component, objectLinksArray);
  }
  objectLinkCollection.set(component, objectLinksArray);
  localStorageLinkCollection.set(component, localStorageLinkMap);
  localStoragePropCollection.set(component, localStoragePropMap);
  if (!builderParamInitialization.get(component)) {
    builderParamInitialization.set(component, builderParamData);
  }
  if (!propInitialization.get(component)) {
    propInitialization.set(component, propData);
  }
}

function hasCollection(node: ts.Identifier): boolean {
  const name: string = node.escapedText.toString();
  return linkCollection.has(name) ||
    propCollection.has(name) ||
    propertyCollection.has(name) ||
    builderParamObjectCollection.has(name) ||
    stateCollection.has(name) ||
    regularCollection.has(name) ||
    storagePropCollection.has(name) ||
    storageLinkCollection.has(name) ||
    provideCollection.has(name) ||
    consumeCollection.has(name) ||
    objectLinkCollection.has(name) ||
    localStorageLinkCollection.has(name) ||
    localStoragePropCollection.has(name);
}

function isModule(filePath: string): boolean {
  return !/^(\.|\.\.)?\//.test(filePath) || filePath.indexOf(projectConfig.packageDir) > -1;
}

function isCustomComponent(node: ts.StructDeclaration, structDecorator: structDecoratorResult): boolean {
  let isComponent: boolean = false;
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  if (decorators && decorators.length) {
    for (let i = 0; i < decorators.length; ++i) {
      const decoratorName: ts.Expression = decorators[i].expression;
      if (ts.isIdentifier(decoratorName) || ts.isCallExpression(decoratorName)) {
        let name: string = '';
        if (ts.isCallExpression(decoratorName) && ts.isIdentifier(decoratorName.expression)) {
          name = decoratorName.expression.escapedText.toString();
        } else if (ts.isIdentifier(decoratorName)) {
          name = decoratorName.escapedText.toString();
        }
        if (CUSTOM_DECORATOR_NAME.has(name)) {
          isComponent = true;
        }
        if (name === DECORATOR_REUSEABLE) {
          structDecorator.hasRecycle = true;
        }
      }
    }
  }
  return isComponent;
}

let packageJsonEntry: string = '';

function isPackageJsonEntry(filePath: string): boolean {
  const packageJsonPath: string = path.join(filePath, projectConfig.packageJson);
  if (fs.existsSync(packageJsonPath)) {
    let entryTypes: string;
    let entryMain: string;
    try {
      const packageJson: Object =
        (projectConfig.packageManagerType === 'npm' ? JSON : JSON5).parse(fs.readFileSync(packageJsonPath).toString());
      entryTypes = packageJson.types;
      entryMain = packageJson.main;
    } catch (e) {
      return false;
    }
    if (entryExist(filePath, entryTypes)) {
      packageJsonEntry = path.resolve(filePath, entryTypes);
      return true;
    } else if (entryExist(filePath, entryMain)) {
      packageJsonEntry = path.resolve(filePath, entryMain);
      return true;
    }
  }
  return false;
}

function entryExist(filePath: string, entry: string): boolean {
  return typeof entry === 'string' && fs.existsSync(path.resolve(filePath, entry)) &&
    fs.statSync(path.resolve(filePath, entry)).isFile();
}

function getModuleFilePath(filePath: string): string {
  if (filePath && path.extname(filePath) !== EXTNAME_ETS && isModule(filePath)) {
    filePath += EXTNAME_ETS;
  }
  return filePath;
}

function getFileResolvePath(fileResolvePath: string, pagesDir: string, filePath: string,
  projectPath: string): string {
  const moduleFilePath: string = getModuleFilePath(filePath);
  const defaultModule: string = path.join(projectPath, moduleFilePath);
  if (fs.existsSync(defaultModule)) {
    return defaultModule;
  }
  let entryModule: string;
  let etsModule: string;
  if (new RegExp(`^@(${sdkConfigPrefix})\\.`).test(filePath.trim())) {
    for (let i = 0; i < sdkConfigs.length; i++) {
      const resolveModuleInfo: ResolveModuleInfo = getRealModulePath(sdkConfigs[i].apiPath, filePath, ['.d.ts', '.d.ets']);
      const systemModule: string = resolveModuleInfo.modulePath;
      if (fs.existsSync(systemModule)) {
        return systemModule;
      }
    }
  }
  if (!projectConfig.aceModuleJsonPath) {
    entryModule = path.join(projectPath, '../../../../../', moduleFilePath);
    etsModule = path.join(projectPath, '../../', moduleFilePath);
  } else {
    entryModule = path.join(projectPath, '../../../../', moduleFilePath);
    etsModule = path.join(projectPath, '../', moduleFilePath);
  }
  if (fs.existsSync(entryModule)) {
    return entryModule;
  }
  if (fs.existsSync(etsModule)) {
    return etsModule;
  }
  let curPageDir: string = pagesDir;
  while (!fs.existsSync(fileResolvePath)) {
    if (filePath.indexOf(projectConfig.packageDir) > -1 && /^(\.|\.\.)\//.test(filePath)) {
      fileResolvePath = path.join(curPageDir, filePath);
    } else {
      fileResolvePath = path.join(curPageDir, projectConfig.packageDir, filePath);
    }
    if (fs.existsSync(fileResolvePath + EXTNAME_ETS)) {
      fileResolvePath = fileResolvePath + EXTNAME_ETS;
    } else if (isPackageJsonEntry(fileResolvePath)) {
      fileResolvePath = packageJsonEntry;
      if (fs.statSync(fileResolvePath).isDirectory()) {
        if (fs.existsSync(path.join(fileResolvePath, INDEX_ETS))) {
          fileResolvePath = path.join(fileResolvePath, INDEX_ETS);
        } else if (fs.existsSync(path.join(fileResolvePath, INDEX_TS))) {
          fileResolvePath = path.join(fileResolvePath, INDEX_TS);
        }
      }
    } else if (fs.existsSync(path.join(fileResolvePath, INDEX_ETS))) {
      fileResolvePath = path.join(fileResolvePath, INDEX_ETS);
    } else if (fs.existsSync(path.join(fileResolvePath, INDEX_TS))) {
      fileResolvePath = path.join(fileResolvePath, INDEX_TS);
    }
    if (curPageDir === path.parse(curPageDir).root) {
      break;
    }
    curPageDir = path.dirname(curPageDir);
  }
  return fileResolvePath;
}

function getFileFullPath(filePath: string, pagesDir: string): string {
  if (filePath && path.extname(filePath) !== EXTNAME_ETS && path.extname(filePath) !== EXTNAME_TS &&
      !isModule(filePath)) {
    const dirIndexEtsPath: string = path.resolve(path.resolve(pagesDir, filePath), INDEX_ETS);
    const dirIndexTsPath: string = path.resolve(path.resolve(pagesDir, filePath), INDEX_TS);
    if (/^(\.|\.\.)\//.test(filePath) && !fs.existsSync(path.resolve(pagesDir, filePath + EXTNAME_ETS)) &&
      fs.existsSync(dirIndexEtsPath)) {
      filePath = dirIndexEtsPath;
    } else if (/^(\.|\.\.)\//.test(filePath) && !fs.existsSync(path.resolve(pagesDir, filePath + EXTNAME_TS)) &&
      fs.existsSync(dirIndexTsPath)) {
      filePath = dirIndexTsPath;
    } else {
      filePath += getExtensionIfUnfullySpecifiedFilepath(path.resolve(pagesDir, filePath));
    }
  }

  let fileResolvePath: string;
  if (/^(\.|\.\.)\//.test(filePath) && filePath.indexOf(projectConfig.packageDir) < 0) {
    fileResolvePath = path.resolve(pagesDir, filePath);
  } else if (/^\//.test(filePath) && filePath.indexOf(projectConfig.packageDir) < 0 ||
    fs.existsSync(filePath) && fs.statSync(filePath).isFile()) {
    fileResolvePath = filePath;
  } else {
    fileResolvePath = getFileResolvePath(fileResolvePath, pagesDir, filePath, projectConfig.projectPath);
  }

  return fileResolvePath;
}

function validateModuleName(moduleNode: ts.Identifier, log: LogInfo[], sourceFile?: ts.SourceFile,
  fileResolvePath?: string): void {
  const moduleName: string = moduleNode.escapedText.toString();
  if (INNER_COMPONENT_NAMES.has(moduleName)) {
    const error: LogInfo = {
      type: LogType.ERROR,
      message: `The module name '${moduleName}' can not be the same as the inner component name.`,
      pos: moduleNode.getStart(),
      code: '10905235'
    };
    if (sourceFile && fileResolvePath) {
      const posOfNode: ts.LineAndCharacter = sourceFile.getLineAndCharacterOfPosition(moduleNode.getStart());
      const line: number = posOfNode.line + 1;
      const column: number = posOfNode.character + 1;
      Object.assign(error, {
        fileName: fileResolvePath,
        line: line,
        column: column
      });
    }
    log.push(error);
  }
}

interface PageInfo {
  pageFile: string;
  setChildOnce: boolean;
}

export function processImportModule(node: ts.ImportDeclaration, pageFile: string, log: LogInfo[],
  share: object = null): void {
  let importSymbol: ts.Symbol;
  let realSymbol: ts.Symbol;
  let originNode: ts.Node;
  const pageInfo: PageInfo = { pageFile: pageFile, setChildOnce: false };
  validateModuleSpecifier(node.moduleSpecifier, log);

  // import xxx from 'xxx'
  if (node.importClause && node.importClause.name && ts.isIdentifier(node.importClause.name)) {
    getDefinedNode(importSymbol, realSymbol, originNode, node.importClause.name, pageInfo, share);
  }

  // import {xxx} from 'xxx'
  if (node.importClause && node.importClause.namedBindings &&
    ts.isNamedImports(node.importClause.namedBindings) &&
    node.importClause.namedBindings.elements) {
    node.importClause.namedBindings.elements.forEach((importSpecifier: ts.ImportSpecifier) => {
      if (ts.isImportSpecifier(importSpecifier) && importSpecifier.name && ts.isIdentifier(importSpecifier.name)) {
        getDefinedNode(importSymbol, realSymbol, originNode, importSpecifier.name, pageInfo, share);
      }
    });
  }

  // import * as xxx from 'xxx'
  if (node.importClause && node.importClause.namedBindings &&
    ts.isNamespaceImport(node.importClause.namedBindings) && node.importClause.namedBindings.name &&
    ts.isIdentifier(node.importClause.namedBindings.name)) {
    storedFileInfo.isAsPageImport = true;
    getDefinedNode(importSymbol, realSymbol, originNode, node.importClause.namedBindings.name, pageInfo, share);
  }
}

function getDefinedNode(importSymbol: ts.Symbol, realSymbol: ts.Symbol, originNode: ts.Node,
  usedNode: ts.Identifier, pageInfo: PageInfo, share: object = null): void {
  importSymbol = globalProgram.checker.getSymbolAtLocation(usedNode);
  if (importSymbol) {
    realSymbol = globalProgram.checker.getAliasedSymbol(importSymbol);
  } else {
    realSymbol = null;
  }
  if (realSymbol && realSymbol.declarations) {
    originNode = realSymbol.declarations[0];
  } else {
    originNode = null;
  }
  if (originNode) {
    if (ts.isSourceFile(originNode) && realSymbol.escapedName) {
      const escapedName: string = realSymbol.escapedName.toString().replace(/^("|')/, '').replace(/("|')$/, '');
      if (fs.existsSync(escapedName + '.ets') || fs.existsSync(escapedName + '.ts') &&
        realSymbol.exports && realSymbol.exports instanceof Map) {
        getIntegrationNodeInfo(originNode, usedNode, realSymbol.exports, pageInfo, share);
        return;
      }
    }
    processImportNode(originNode, usedNode, false, null, pageInfo, share);
  }
}

function getIntegrationNodeInfo(originNode: ts.Node, usedNode: ts.Identifier, exportsMap: ts.SymbolTable,
  pageInfo: PageInfo, share: object = null): void {
  for (const usedSymbol of exportsMap) {
    try {
      originNode = globalProgram.checker.getAliasedSymbol(usedSymbol[1]).declarations[0];
    } catch (e) {
      if (usedSymbol[1] && usedSymbol[1].declarations) {
        for (let i = 0; i < usedSymbol[1].declarations.length; i++) {
          originNode = usedSymbol[1].declarations[i];
          exportAllManage(originNode, usedNode, pageInfo, share);
        }
      }
    }
    processImportNode(originNode, usedNode, true, usedSymbol[0], pageInfo, share);
  }
}

// export * from 'xxx';
function exportAllManage(originNode: ts.Node, usedNode: ts.Identifier, pageInfo: PageInfo, share: object = null): void {
  let exportOriginNode: ts.Node;
  if (!originNode.exportClause && originNode.moduleSpecifier && ts.isStringLiteral(originNode.moduleSpecifier)) {
    const exportSymbol: ts.Symbol = globalProgram.checker.getSymbolAtLocation(originNode.moduleSpecifier);
    if (exportSymbol && exportSymbol.declarations) {
      exportOriginNode = exportSymbol.declarations[0];
    } else {
      exportOriginNode = null;
    }
    if (exportOriginNode) {
      if (ts.isSourceFile(exportOriginNode) && exportSymbol.escapedName) {
        const escapedName: string = exportSymbol.escapedName.toString().replace(/^("|')/, '').replace(/("|')$/, '');
        if (fs.existsSync(escapedName + '.ets') || fs.existsSync(escapedName + '.ts') &&
          exportSymbol.exports && exportSymbol.exports instanceof Map) {
          getIntegrationNodeInfo(originNode, usedNode, exportSymbol.exports, pageInfo, share);
          return;
        }
      }
    }
  }
}

function processImportNode(originNode: ts.Node, usedNode: ts.Identifier, importIntegration: boolean,
  usedPropName: string, pageInfo: PageInfo, share: object = null): void {
  const structDecorator: structDecoratorResult = { hasRecycle: false };
  let name: string;
  let asComponentName: string;
  if (importIntegration) {
    if (storedFileInfo.isAsPageImport) {
      asComponentName = usedNode.escapedText.toString() + '.' + usedPropName;
    }
    name = usedPropName;
  } else {
    name = usedNode.escapedText.toString();
  }
  let needCollection: boolean = true;
  const originFile: string = originNode.getSourceFile() ? originNode.getSourceFile().fileName : undefined;
  if (ts.isStructDeclaration(originNode) && ts.isIdentifier(originNode.name)) {
    parseComponentInImportNode(originNode, name, asComponentName, structDecorator, originFile);
  } else if (isObservedClass(originNode)) {
    observedClassCollection.add(name);
  } else if (ts.isFunctionDeclaration(originNode) && hasDecorator(originNode, COMPONENT_BUILDER_DECORATOR)) {
    CUSTOM_BUILDER_METHOD.add(name);
    GLOBAL_CUSTOM_BUILDER_METHOD.add(name);
  } else if (ts.isEnumDeclaration(originNode) && originNode.name) {
    enumCollection.add(name);
  } else {
    needCollection = false;
  }
  if (needCollection && pageInfo.pageFile && !pageInfo.setChildOnce && originFile) {
    const childFile: string = path.resolve(getRealPath(originFile) || originFile);
    const fileHash = share?.getHashByFilePath ? share.getHashByFilePath(childFile) : '';
    storedFileInfo.transformCacheFiles[pageInfo.pageFile].children.push({
      fileName: childFile,
      mtimeMs: fs.existsSync(childFile) ? fs.statSync(childFile).mtimeMs : 0,
      hash: fileHash
    });
    pageInfo.setChildOnce = true;
  }
}

function getRealPath(filePath: string): string {
  try {
    const newPath: string = fs.realpathSync.native(filePath);
    return newPath;
  } catch (err) {
    return undefined;
  }
}

function setComponentCollectionInfo(name: string, componentSet: IComponentSet, isDETS: boolean,
  structDecorator: structDecoratorResult, asComponentName: string): void {
  setDependencies(name, asComponentName, componentSet.links, componentSet.properties,
    componentSet.props, componentSet.builderParams, componentSet.states, componentSet.regulars,
    componentSet.storageProps, componentSet.storageLinks, componentSet.provides,
    componentSet.consumes, componentSet.objectLinks, componentSet.localStorageLink,
    componentSet.localStorageProp, componentSet.builderParamData, componentSet.propData, isDETS,
    structDecorator);
  regularInitialization.set(name, componentSet.regularInit);
  stateInitialization.set(name, componentSet.stateInit);
  provideInitialization.set(name, componentSet.provideInit);
  privateCollection.set(name, componentSet.privateCollection);
  regularStaticCollection.set(name, componentSet.regularStaticCollection);
  if (asComponentName) {
    const asComponentNameStructInfo: StructInfo =
      processStructComponentV2.getOrCreateStructInfo(asComponentName);
    asComponentNameStructInfo.updatePropsDecoratorsV1.push(
      ...componentSet.states, ...componentSet.props,
      ...componentSet.provides, ...componentSet.objectLinks
    );
    asComponentNameStructInfo.linkDecoratorsV1.push(...componentSet.links);
    return;
  }
  const nameStructInfo: StructInfo = processStructComponentV2.getOrCreateStructInfo(name);
  nameStructInfo.updatePropsDecoratorsV1.push(
    ...componentSet.states, ...componentSet.props,
    ...componentSet.provides, ...componentSet.objectLinks
  );
  nameStructInfo.linkDecoratorsV1.push(...componentSet.links);
}

function parseComponentInImportNode(originNode: ts.StructDeclaration, name: string,
  asComponentName: string, structDecorator: structDecoratorResult, originFile: string): void {
  componentCollection.customComponents.add(name);
  const structInfo: StructInfo = asComponentName ?
    processStructComponentV2.getOrCreateStructInfo(asComponentName) :
    processStructComponentV2.getOrCreateStructInfo(name);
  if (isComponentV2(originNode)) {
    parseComponentV2InImportNode(originNode, name, originFile, structInfo);
    return;
  }
  if (isCustomDialogClass(originNode)) {
    structInfo.isCustomDialog = true;
    componentCollection.customDialogs.add(name);
  }
  if (isCustomComponent(originNode, structDecorator)) {
    structInfo.isComponentV1 = true;
    let isDETS: boolean = false;
    const componentSet: IComponentSet = getComponentSet(originNode, false);
    while (originNode) {
      if (ts.isSourceFile(originNode) && /\.d\.ets$/.test(originNode.fileName)) {
        isDETS = true;
      }
      originNode = originNode.parent;
    }
    setComponentCollectionInfo(name, componentSet, isDETS, structDecorator, asComponentName);
  }
}

function parseComponentV2InImportNode(node: ts.StructDeclaration, name: string, originFile: string,
  structInfo: StructInfo): void {
  structInfo.isComponentV2 = true;
  const isDETS: boolean = originFile && /\.d\.ets$/.test(originFile);
  if (isDETS) {
    storedFileInfo.getCurrentArkTsFile().compFromDETS.add(name);
  }
  if (isReusableV2(node)) {
    storedFileInfo.getCurrentArkTsFile().reuseComponentsV2.add(name);
  }
  processStructComponentV2.parseComponentProperty(node, structInfo, null, null);
}

function isComponentV2(node: ts.StructDeclaration): boolean {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  return decorators.some((item: ts.Decorator) => {
    const name: string = item.getText().replace(/\([^\(\)]*\)/, '').replace('@', '').trim();
    return name === 'ComponentV2';
  });
}

function isReusableV2(node: ts.StructDeclaration): boolean {
  const decorators: readonly ts.Decorator[] = ts.getAllDecorators(node);
  return decorators.some((item: ts.Decorator) => {
    const name: string = item.getText().replace(/\([^\(\)]*\)/, '').replace('@', '').trim();
    return name === DECORATOR_REUSABLE_V2;
  });
}
