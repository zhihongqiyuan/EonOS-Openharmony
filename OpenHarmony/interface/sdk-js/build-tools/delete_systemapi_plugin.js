/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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
const path = require('path');
const fs = require('fs');
const ts = require('typescript');

let sourceFile = null;
let lastNoteStr = '';
let lastNodeName = '';
const referencesMap = new Map();
const referencesModuleMap = new Map();
const kitFileNeedDeleteMap = new Map();
/**
 * @enum {string} references地址的切换类型
 */
const REFERENCE_TYPE = {
  TOLOCAL: 'toLocal',
  TORIGHTSDK: 'toRightSDK',
  TOSDK: 'toSDK',
};
const PATT = {
  GET_REFERENCE: /\/\/\/\s*<reference.*>/g,
  GET_REFERENCEURL: /\/\/\/\s*<reference\s*path=("|')(.*)("|')\s*\/>/g,
  REFERENCEURL_LOCAL: /(.\/)?(\S*)@internal\/component\/ets\/(\S*)/g,
  REFERENCEURL_RIGHTSDK: /(..\/)(\S*)build-tools\/ets-loader\/declarations\/(\S*)/g,
  REFERENCEURL_SDK: /(..\/)(\S*)component\/(\S*)/g,
};
function collectDeclaration(url) {
  // 入口
  try {
    const utPath = path.resolve(__dirname, url);
    const arktsPath = path.resolve(utPath, '../arkts');
    const kitPath = path.resolve(utPath, '../kits');
    const utFiles = [];
    readFile(utPath, utFiles); // 读取文件
    readFile(arktsPath, utFiles); // 读取文件
    tsTransform(utFiles, deleteSystemApi);
    tsTransformKitFile(kitPath);
  } catch (error) {
    console.error('DELETE_SYSTEM_PLUGIN ERROR: ', error);
  }
}

/**
 * 解析url目录下方的kit文件，删除对应systemapi
 * @param { string } kitPath kit文件路径
 */
function tsTransformKitFile(kitPath) {
  kitFileNeedDeleteMap.delete('');
  if (kitFileNeedDeleteMap.length === 0) {
    return;
  }
  const kitFiles = [];
  readFile(kitPath, kitFiles); // 读取文件
  kitFiles.forEach((kitFile) => {
    const kitName = processFileNameWithoutExt(kitFile).replace('@kit.', '');
    const content = fs.readFileSync(kitFile, 'utf-8');
    const fileName = processFileName(kitFile);
    let sourceFile = ts.createSourceFile(fileName, content, ts.ScriptTarget.ES2017, true);
    const sourceInfo = getKitNewSourceFile(sourceFile, kitName);
    if (isEmptyFile(sourceInfo.sourceFile)) {
      return;
    }
    const printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
    let result = printer.printNode(ts.EmitHint.Unspecified, sourceInfo.sourceFile, sourceFile);
    if (sourceInfo.copyrightMessage !== '') {
      result = sourceInfo.copyrightMessage + result;
    }
    writeFile(kitFile, result);
  });
}

/**
 * 处理kit中需要删除的节点，在其他文件被systemapi修饰的api
 * @param { ts.SourceFile } sourceFile
 * @param { string } kitName
 * @returns 删除完的节点，全部删除为空字符串
 */
function getKitNewSourceFile(sourceFile, kitName) {
  const newStatements = [];
  const needDeleteExportName = new Set();
  let copyrightMessage = '';
  // 初始化ts工厂
  const factory = ts.factory;
  sourceFile.statements.forEach((statement, index) => {
    if (ts.isImportDeclaration(statement)) {
      const newStatement = processKitImportDeclaration(statement, needDeleteExportName);
      if (newStatement) {
        newStatements.push(newStatement);
      } else if (index === 0) {
        copyrightMessage = sourceFile.getFullText().replace(sourceFile.getText(), '');
      }
    } else if (ts.isExportDeclaration(statement)) {
      const exportSpecifiers = statement.exportClause.elements.filter((item) => {
        return !needDeleteExportName.has(item.name.escapedText.toString());
      });
      if (exportSpecifiers.length !== 0) {
        statement.exportClause = factory.updateNamedExports(statement.exportClause, exportSpecifiers);
        newStatements.push(statement);
      }
    }
  });
  sourceFile = factory.updateSourceFile(sourceFile, newStatements);
  return { sourceFile, copyrightMessage };
}

/**
 * 根据节点和需要删除的节点数据生成新节点
 * @param { ts.ImportDeclaration } statement 需要处理的import节点
 * @param { Map} needDeleteMap 需要删除的节点数据
 * @param { Map} needDeleteExportName 需要删除的导出节点
 * @returns { ts.ImportDeclaration | undefined } 返回新的import节点，全部删除为undefined
 */
function processKitImportDeclaration(statement, needDeleteExportName) {
  // 初始化ts工厂
  const factory = ts.factory;
  const importClause = statement.importClause;
  if (!ts.isImportClause(importClause)) {
    return statement;
  }
  const importPath = statement.moduleSpecifier.text.replace('../', '');
  if (kitFileNeedDeleteMap === undefined || !kitFileNeedDeleteMap.has(importPath)) {
    const hasFilePath = hasFileByImportPath(importPath);
    return hasFilePath ? statement : undefined;
  }
  const currImportInfo = kitFileNeedDeleteMap.get(importPath);
  let defaultName = '';
  let importNodeNamedBindings = [];
  if (importClause.name) {
    if (currImportInfo.default === importClause.name.escapedText.toString()) {
      //import buffer from "../@ohos.buffer";
      needDeleteExportName.add(currImportInfo.default);
    } else {
      defaultName = importClause.name.escapedText.toString();
    }
  }
  const namedBindings = importClause.namedBindings;
  if (namedBindings !== undefined && ts.isNamedImports(namedBindings)) {
    const elements = namedBindings.elements;
    elements.forEach((element) => {
      const exportName = element.propertyName ?
        element.propertyName.escapedText.toString() :
        element.name.escapedText.toString();
      if (!currImportInfo.exportName.has(exportName)) {
        importNodeNamedBindings.push(factory.createImportSpecifier(false, element.propertyName, element.name));
      } else {
        needDeleteExportName.add(element.name.escapedText.toString());
      }
    });
  }
  if (defaultName !== '' || importNodeNamedBindings.length !== 0) {
    const newImportNode = factory.createImportDeclaration(
      undefined,
      factory.createImportClause(
        false,
        defaultName === '' ? undefined : factory.createIdentifier(defaultName),
        importNodeNamedBindings.length === 0 ? undefined : factory.createNamedImports(importNodeNamedBindings)
      ),
      statement.moduleSpecifier
    );
    return newImportNode;
  }
  return undefined;
}

/**
 * 判断文件路径对应的文件是否存在
 * @param {string} importPath kit文件import
 * @returns {boolean} importPath是否存在
 */
function hasFileByImportPath(importPath) {
  let fileDir = path.resolve(apiSourcePath);
  if (importPath.startsWith('@arkts')) {
    fileDir = path.resolve(apiSourcePath, '../arkts');
  }
  const flag = ['.d.ts', '.d.ets'].some(ext => {
    const filePath = path.resolve(fileDir, `${importPath}${ext}`);
    return fs.existsSync(filePath);
  });
  return flag;
}

/**
 * 统一处理文件名称，修改后缀等
 * @param {string} filePath 文件路径
 * @returns {string} filename 文件名称
 */
function processFileName(filePath) {
  return path
    .basename(filePath)
    .replace(/\.d\.ts$/g, '.ts')
    .replace(/\.d\.ets$/g, '.ets');
}

function processFileNameWithoutExt(filePath) {
  return path
    .basename(filePath)
    .replace(/\.d\.ts$/g, '')
    .replace(/\.d\.ets$/g, '')
    .replace(/\.ts$/g, '')
    .replace(/\.ets$/g, '');
}

/**
 * 遍历所有文件进行处理
 * @param {Array} utFiles 所有文件
 * @param {deleteSystemApi} callback 回调函数
 */
function tsTransform(utFiles, callback) {
  utFiles.forEach((url) => {
    const apiBaseName = path.basename(url);
    if (/\.json/.test(url) || apiBaseName === 'index-full.d.ts') {
      // 特殊类型文件处理
      const content = fs.readFileSync(url, 'utf-8');
      writeFile(url, content);
    } else if (/\.d\.ts/.test(apiBaseName) || /\.d\.ets/.test(apiBaseName)) {
      // dts文件处理
      let content = fs.readFileSync(url, 'utf-8'); // 文件内容
      const fileName = processFileName(url);
      let references = content.match(PATT.GET_REFERENCE);
      if (references) {
        referencesMap.set(url, { references: references });
        for (let index = 0; index < references.length; index++) {
          const item = references[index];
          content = content.replace(item, '');
        }
      }
      ts.transpileModule(content, {
        compilerOptions: {
          target: ts.ScriptTarget.ES2017,
        },
        fileName: fileName,
        transformers: { before: [callback(url)] },
      });
    }
  });
}
/**
 * 切换references或者references中path的格式
 * @param {string} references references或者references中的path
 * @param {REFERENCE_TYPE} reverse 切换类型
 * @returns {string}
 */
function referencesToOthers(references, type) {
  let referencesurl;
  let hasFullpatt = references.match(PATT.GET_REFERENCEURL);
  let isFullReferenceurl = hasFullpatt && hasFullpatt.length > 0;
  if (isFullReferenceurl) {
    referencesurl = RegExp.$2;
  } else {
    referencesurl = references;
  }
  let currentType = '';
  if (referencesurl.match(PATT.REFERENCEURL_LOCAL)) {
    currentType = REFERENCE_TYPE.TOLOCAL;
  } else if (referencesurl.match(PATT.REFERENCEURL_RIGHTSDK)) {
    currentType = REFERENCE_TYPE.TORIGHTSDK;
  } else if (referencesurl.match(PATT.REFERENCEURL_SDK)) {
    currentType = REFERENCE_TYPE.TOSDK;
  }
  if (currentType === '' || currentType === type) {
    return references;
  }
  let starturl = '';
  let fileName = '';
  switch (currentType) {
    case REFERENCE_TYPE.TOLOCAL:
      starturl = RegExp.$2;
      fileName = RegExp.$3;
      break;
    case REFERENCE_TYPE.TORIGHTSDK:
      starturl = RegExp.$2;
      fileName = RegExp.$3;
      break;
    case REFERENCE_TYPE.TOSDK:
      starturl = RegExp.$2;
      fileName = RegExp.$3;
      break;
    default:
      break;
  }
  let finallyurl;
  switch (type) {
    case REFERENCE_TYPE.TOLOCAL:
      finallyurl = `${starturl === '' ? './' : ''}${starturl}@internal/component/ets/${fileName}`;
      break;
    case REFERENCE_TYPE.TORIGHTSDK:
      finallyurl = `../${starturl}build-tools/ets-loader/declarations/${fileName}`;
      break;
    case REFERENCE_TYPE.TOSDK:
      finallyurl = `../${starturl}component/${fileName}`;
      break;
    default:
      break;
  }
  if (isFullReferenceurl) {
    finallyurl = `/// <reference path="${finallyurl}"/>`;
  }
  return finallyurl;
}

/**
 * 读取目录下所有文件
 * @param {string} dir 文件目录
 * @param {Array} utFiles 所有文件
 */
function readFile(dir, utFiles) {
  try {
    const files = fs.readdirSync(dir);
    files.forEach((element) => {
      const filePath = path.join(dir, element);
      const status = fs.statSync(filePath);
      if (status.isDirectory()) {
        readFile(filePath, utFiles);
      } else {
        utFiles.push(filePath);
      }
    });
  } catch (e) {
    console.log('ETS ERROR: ' + e);
  }
}

function writeFile(url, data, option) {
  if (fs.existsSync(outputPath)) {
    fs.rmdirSync(outputPath, { recursive: true });
  }
  const newFilePath = path.resolve(outputPath, path.relative(__dirname, url));
  fs.mkdir(path.dirname(newFilePath), { recursive: true }, (err) => {
    if (err) {
      console.log(`ERROR FOR CREATE PATH ${err}`);
    } else {
      if (data === '') {
        fs.rmSync(newFilePath);
        return;
      }
      fs.writeFileSync(newFilePath, data, option, (err) => {
        if (err) {
          console.log(`ERROR FOR CREATE FILE ${err}`);
        }
      });
    }
  });
}

const globalModules = new Map();

/**
 * 每个文件处理前回调函数第二个
 * @param {string} url 文件路径
 * @returns {Function}
 */
function formatImportDeclaration(url, copyrightMessage = '', isCopyrightDeleted = false) {
  return (context) => {
    const allIdentifierSet = new Set([]);
    return (node) => {
      sourceFile = node;
      collectAllIdentifier(node); // 获取所有标识符
      formatValue = formatAllNodes(url, node, allIdentifierSet); // 获取所有节点
      node = formatValue.node;
      const referencesMessage = formatValue.referencesMessage;
      if (formatValue.isCopyrightDeleted) {
        copyrightMessage = formatValue.copyrightMessage;
        isCopyrightDeleted = formatValue.isCopyrightDeleted;
      }
      if (!isEmptyFile(node)) {
        const printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
        let result = printer.printNode(ts.EmitHint.Unspecified, node, sourceFile);
        if (isCopyrightDeleted) {
          // 当第一个节点被删除时会同步删除整个文件jsdoc
          result = copyrightMessage + '\n' + result;
        }
        copyrightMessage = node.getFullText().replace(node.getText(), '');
        if (referencesMessage) {
          // 将references写入文件
          result =
            result.substring(0, copyrightMessage.length) +
            '\n' +
            referencesMessage +
            result.substring(copyrightMessage.length);
        }
        result = removeSystemapiDoc(result);
        writeFile(url, result);
      }
      return ts.factory.createSourceFile([], ts.SyntaxKind.EndOfFileToken, ts.NodeFlags.None);
    };
    function collectAllIdentifier(node) {
      if (ts.isSourceFile(node) && node.statements) {
        node.statements.forEach((stat) => {
          if (!ts.isImportDeclaration(stat)) {
            ts.visitEachChild(stat, collectAllNodes, context);
          }
        });
      }
    }
    function collectAllNodes(node) {
      if (ts.isIdentifier(node)) {
        allIdentifierSet.add(node.escapedText.toString());
      }
      return ts.visitEachChild(node, collectAllNodes, context);
    }
  };
}

function formatAllNodes(url, node, allIdentifierSet, copyrightMessage = '', isCopyrightDeleted = false) {
  let referencesMessage = '';
  let currReferencesModule = formatAllNodesReferences(url);
  if (ts.isSourceFile(node) && node.statements) {
    const newStatements = [];
    node.statements.forEach((statement) => {
      if (ts.isImportDeclaration(statement)) {
        const importInfo = formatAllNodesImportDeclaration(
          node,
          statement,
          url,
          currReferencesModule,
          allIdentifierSet
        );
        if (importInfo.statement) {
          newStatements.push(statement);
        } else if (importInfo.isCopyrightDeleted) {
          copyrightMessage = importInfo.copyrightMessage;
          isCopyrightDeleted = importInfo.isCopyrightDeleted;
        }
      } else if (ts.isStructDeclaration(statement)) {
        statement = ts.factory.updateStructDeclaration(
          statement,
          statement.modifiers,
          statement.name,
          statement.typeParameters,
          statement.heritageClauses,
          statement.members.slice(1)
        );
        newStatements.push(statement);
      } else {
        newStatements.push(statement);
      }
    });
    currReferencesModule.forEach((item) => {
      if (item.isUsed) {
        referencesMessage += item.reference + '\n';
      }
    });
    node = ts.factory.updateSourceFile(node, newStatements);
  }
  return { node, referencesMessage, copyrightMessage, isCopyrightDeleted };
}

function hasCopyright(node) {
  return /http\:\/\/www\.apache\.org\/licenses\/LICENSE\-2\.0/g.test(node.getFullText().replace(node.getText(), ''));
}

/**
 * 处理References节点
 * @param {ts.node} node 当前节点
 * @param {string} url 文件路径
 * @returns {Array} currReferencesModule 返回该文件的references数据
 */
function formatAllNodesReferences(url) {
  globalModules.clear();
  let currReferences = [];
  let currReferencesModule = [];
  if (referencesMap.has(url)) {
    currReferences = referencesMap.get(url);
    currReferencesModule = currReferences.references.map((element, index) => {
      element.match(PATT.GET_REFERENCEURL);
      let referencePath = RegExp.$2;
      referencePath = referencesToOthers(referencePath, REFERENCE_TYPE.TOLOCAL);
      let fullReferencePath = path.resolve(path.dirname(url), referencePath);
      let module = referencesModuleMap.get(fullReferencePath);
      for (const key in module) {
        if (Object.hasOwnProperty.call(module, key)) {
          globalModules.set(key, index);
        }
      }
      return { modules: module, fullReferencePath: fullReferencePath, reference: element, isUsed: false };
    });
  }
  return currReferencesModule;
}

/**
 * 处理Import节点 去除未使用、不存在、References中没有对应模块的导入
 * @param {ts.node} node 当前节点
 * @param {ts.ImportDeclaration} statement 导入节点
 * @param {string} url 文件路径
 * @param {string} url 文件路径
 * @param {Set} allIdentifierSet 该文件的所有Identifier关键字
 * @returns {{statement:ts.ImportDeclaration,copyrightMessage:string,isCopyrightDeleted:boolean}} statement 处理完成的导入节点、copyrightMessage
 */
function formatAllNodesImportDeclaration(node, statement, url, currReferencesModule, allIdentifierSet) {
  // 是import节点 import { AsyncCallback } from './@ohos.base';
  const clauseSet = new Set([]);
  if (statement.importClause && ts.isImportClause(statement.importClause)) {
    // 标识符
    const clauseNode = statement.importClause;
    if (!clauseNode.namedBindings && clauseNode.name && ts.isIdentifier(clauseNode.name)) {
      // 没有大括号的标识符
      clauseSet.add(clauseNode.name.escapedText.toString());
    } else if (
      clauseNode.namedBindings &&
      clauseNode.namedBindings.name &&
      ts.isIdentifier(clauseNode.namedBindings.name)
    ) {
      // 没有标识符 *号
      clauseSet.add(clauseNode.namedBindings.name.escapedText.toString());
    } else if (clauseNode.namedBindings && clauseNode.namedBindings.elements) {
      // 有花括号的标识符
      clauseNode.namedBindings.elements.forEach((ele) => {
        if (ele.name && ts.isIdentifier(ele.name)) {
          clauseSet.add(ele.name.escapedText.toString());
        }
      });
    }
  }
  const importSpecifier = statement.moduleSpecifier.getText().replace(/[\'\"]/g, '');
  const dtsImportSpecifierPath = path.resolve(url, `../${importSpecifier}.d.ts`); // import 文件路径判断
  const detsImportSpecifierPath = path.resolve(url, `../${importSpecifier}.d.ets`); // import 文件路径判断
  let hasImportSpecifierFile = fs.existsSync(dtsImportSpecifierPath) || fs.existsSync(detsImportSpecifierPath);
  let hasImportSpecifierInModules = globalModules.has(importSpecifier);
  if ((hasImportSpecifierFile || hasImportSpecifierInModules) && clauseSet.size > 0) {
    let currModule = [];
    if (hasImportSpecifierInModules) {
      let index = globalModules.get(importSpecifier);
      currModule = currReferencesModule[index].modules[importSpecifier];
    }
    const clasueCheckList = [];
    let exsitClauseSet = new Set([]);
    for (const clause of clauseSet) {
      let flag = allIdentifierSet.has(clause);
      if (hasImportSpecifierInModules) {
        flag = allIdentifierSet.has(clause) && currModule.includes(clause);
      }
      if (flag) {
        // 标识符使用到了当前import中的引用
        exsitClauseSet.add(clause);
        clasueCheckList.push('exist');
      } else {
        clasueCheckList.push('non-exist');
      }
    }
    let hasExsitStatus = false;
    let hasNonExsitStatus = false;
    clasueCheckList.forEach((ele) => {
      if (ele === 'exist') {
        hasExsitStatus = true;
      } else {
        hasNonExsitStatus = true;
      }
    });
    if (hasExsitStatus) {
      // 有使用到的标识符
      if (hasNonExsitStatus) {
        // 有没有使用到的标识符
        const newSpecifiers = [];
        statement.importClause.namedBindings.elements.forEach((element) => {
          if (exsitClauseSet.has(element.name.escapedText.toString())) {
            newSpecifiers.push(element);
          }
        });
        statement.importClause.namedBindings = ts.factory.updateNamedImports(
          statement.importClause.namedBindings,
          newSpecifiers
        );
      }
      if (hasImportSpecifierInModules) {
        let index = globalModules.get(importSpecifier);
        currReferencesModule[index].isUsed = true;
      }
      return { statement };
    } else if (hasCopyright(statement)) {
      return { copyrightMessage: node.getFullText().replace(node.getText(), ''), isCopyrightDeleted: true };
    }
  } else if (hasCopyright(statement)) {
    return { copyrightMessage: node.getFullText().replace(node.getText(), ''), isCopyrightDeleted: true };
  }
  return { statement: undefined, copyrightMessage: '', isCopyrightDeleted: false };
}

/**
 * 
 * 防止@file和@kit段注释丢失
 * @param {string} fileFullText 
 * @returns {string}
 * 
 */
function getFileAndKitComment(fileFullText) {
  let fileAndKitComment = '';
  let pattern = /\/\*\*\s*\*\s*@file[\s\S]*?@kit[\s\S]*?\*\//;
  let comment = fileFullText.match(pattern);
  if (comment) {
    fileAndKitComment = comment[0];
  }
  return fileAndKitComment;
}

/**
 * 处理最终结果中的systemapi
 * @param {string} result 
 */
function removeSystemapiDoc(result) {
  result.split;
  return result.replace(/\/\*\*[\s\S]*?\*\//g, (substring, p1) => {
    return /@systemapi/g.test(substring) ? '' : substring;
  });
}

/**
 * 每个文件处理前回调函数第一个
 * @callback deleteSystemApi
 * @param {string} url 文件路径
 * @returns {Function}
 */
function deleteSystemApi(url) {
  return (context) => {
    return (node) => {
      const fullText = String(node.getFullText());
      //获取文件头部的注释信息--这里可能会涉及到@file和@kit段注释丢失
      let fileAndKitComment = getFileAndKitComment(fullText);
      const copyrightMessage = fullText.replace(node.getText(), '').split(/\/\*\*/)[0] + fileAndKitComment + '\n';
      let kitName = '';
      if (fullText.match(/\@kit (.*)\r?\n/g)) {
        kitName = RegExp.$1.replace(/\s/g, '');
      }
      sourceFile = node;
      const deleteNode = processSourceFile(node, kitName); // 处理最外层节点
      node = processVisitEachChild(context, deleteNode.node);
      if (!isEmptyFile(node)) {
        const printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
        const result = printer.printNode(ts.EmitHint.Unspecified, node, sourceFile);
        if (referencesMap.has(url)) {
          resolveReferences(url);
        }
        const fileName = processFileName(url);
        ts.transpileModule(result, {
          compilerOptions: {
            target: ts.ScriptTarget.ES2017,
          },
          fileName: fileName,
          transformers: { before: [formatImportDeclaration(url, copyrightMessage, deleteNode.isCopyrightDeleted)] },
        });
      }
      return ts.factory.createSourceFile([], ts.SyntaxKind.EndOfFileToken, ts.NodeFlags.None);
    };
  };
}

exports.deleteSystemApi = deleteSystemApi;

/**
 * 遍历每个文件下的所有节点，然后删除节点
 * @param node
 * @returns
 */

/**
 * 处理最外层的节点看是否删除
 * @param node 解析过后的节点
 * @param kitName 当前文件kitName
 * @returns
 */
function processSourceFile(node, kitName) {
  let isCopyrightDeleted = false;
  const newStatements = [];
  const newStatementsWithoutExport = [];
  const deleteSystemApiSet = new Set();
  let needDeleteExport = {
    fileName: '',
    default: '',
    exportName: new Set(),
  };
  isCopyrightDeleted = addNewStatements(node, newStatements, deleteSystemApiSet, needDeleteExport);
  newStatements.forEach((statement) => {
    const names = getExportIdentifierName(statement);
    if (names.length === 0) {
      newStatementsWithoutExport.push(statement);
      return;
    }
    if (names.length === 1 && !deleteSystemApiSet.has(names[0])) {
      //exports.name = test;
      //export default test1
      //export {test1}
      newStatementsWithoutExport.push(statement);
      return;
    }
    processExportNode(statement, node, needDeleteExport, names, deleteSystemApiSet, newStatementsWithoutExport);
  });
  if (needDeleteExport.fileName !== '') {
    kitFileNeedDeleteMap.set(needDeleteExport.fileName, needDeleteExport);
  }
  return {
    node: ts.factory.updateSourceFile(node, newStatementsWithoutExport, node.isDeclarationFile, node.referencedFiles),
    isCopyrightDeleted,
  };
}

function processExportNode(statement, node, needDeleteExport, names, deleteSystemApiSet, newStatementsWithoutExport) {
  //删除export节点信息
  if (ts.isExportAssignment(statement)) {
    //export default abilityAccessCtrl;
    needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
    needDeleteExport.default = statement.expression.escapedText.toString();
  } else if (ts.isExportDeclaration(statement)) {
    //export {test1 as test,testa as test2}
    let needExport = false;
    const newSpecifiers = [];
    names.forEach((name, index) => {
      const exportSpecifier = statement.exportClause.elements[index];
      if (!deleteSystemApiSet.has(name)) {
        //未被删除的节点
        newSpecifiers.push(exportSpecifier);
        needExport = true;
      } else {
        //被删除的节点
        needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
        needDeleteExport.exportName.add(exportSpecifier.name.escapedText.toString());
      }
    });
    if (needExport) {
      statement.exportClause = ts.factory.updateNamedExports(statement.exportClause, newSpecifiers);
      newStatementsWithoutExport.push(statement);
    }
  }
}

function addNewStatements(node, newStatements, deleteSystemApiSet, needDeleteExport) {
  let isCopyrightDeleted = false;
  node.statements.forEach((statement, index) => {
    if (!isSystemapi(statement)) {
      newStatements.push(statement);
      return;
    }
    if (index === 0) {
      isCopyrightDeleted = true;
    }
    if (ts.isVariableStatement(statement)) {
      deleteSystemApiSet.add(variableStatementGetEscapedText(statement));
    } else if (
      ts.isModuleDeclaration(statement) ||
      ts.isInterfaceDeclaration(statement) ||
      ts.isClassDeclaration(statement) ||
      ts.isEnumDeclaration(statement) ||
      ts.isStructDeclaration(statement) ||
      ts.isTypeAliasDeclaration(statement)
    ) {
      if (statement && statement.name && statement.name.escapedText) {
        deleteSystemApiSet.add(statement.name.escapedText.toString());
      }
      setDeleteExport(statement, node, needDeleteExport, deleteSystemApiSet);
    } else if (ts.isExportAssignment(statement) || ts.isExportDeclaration(statement)) {
      setDeleteExport(statement, node, needDeleteExport, deleteSystemApiSet);
    }
  });

  return isCopyrightDeleted;
}

function setDeleteExport(statement, node, needDeleteExport, deleteSystemApiSet) {
  if (ts.isExportAssignment(statement) && deleteSystemApiSet.has(statement.expression.escapedText.toString())) {
    needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
    needDeleteExport.default = statement.expression.escapedText.toString();
  } else if (ts.isExportDeclaration(statement)) {
    needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
    statement.exportClause.elements.forEach((element) => {
      const exportName = element.propertyName ?
        element.propertyName.escapedText.toString() :
        element.name.escapedText.toString();
      if (deleteSystemApiSet.has(exportName)) {
        needDeleteExport.exportName.add(element.name.escapedText.toString());
      }
    });
  }
  //export namespace test {}
  const modifiers = statement.modifiers;
  if (modifiers === undefined) {
    return;
  }
  const exportFlag = modifiers.some((modifier) => modifier.kind === ts.SyntaxKind.ExportKeyword);
  const defaultFlag = modifiers.some((modifier) => modifier.kind === ts.SyntaxKind.DefaultKeyword);
  if (exportFlag && defaultFlag) {
    needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
    needDeleteExport.default = statement.name.escapedText.toString();
  } else if (exportFlag) {
    needDeleteExport.fileName = processFileNameWithoutExt(node.fileName);
    needDeleteExport.exportName.add(statement.name.escapedText.toString());
  }
}

/**
 * 获取export节点的名字，只获取第一个关键词
 * @param {ts.node} statement
 * @returns {Array<string>}
 */
function getExportIdentifierName(statement) {
  const names = [];
  if (ts.isExpressionStatement(statement)) {
    //exports.name = test;
    if (ts.isBinaryExpression(statement.expression)) {
      names.push(statement.expression.right.escapedText.toString());
    }
  } else if (ts.isExportAssignment(statement)) {
    //export default test1
    names.push(statement.expression.escapedText.toString());
  } else if (ts.isExportDeclaration(statement)) {
    //export {test1} 、export {test1 as test} 、export * from './featureability'
    const exportClause = statement.exportClause;
    if (exportClause) {
      const specifiers = exportClause.elements;
      specifiers.forEach((specifier) => {
        if (ts.isExportSpecifier(specifier)) {
          const name = specifier.propertyName ? specifier.propertyName : specifier.name;
          names.push(name.escapedText.toString());
        }
      });
    }
  }
  return names;
}

/**
 * 遍历处理tsnode节点
 * @param  context 解析过后的内容
 * @param  node 解析过后的节点
 * @returns ts.node
 */
function processVisitEachChild(context, node) {
  return ts.visitEachChild(node, processAllNodes, context); // 遍历所有子节点
  function processAllNodes(node) {
    if (ts.isInterfaceDeclaration(node)) {
      node = processInterfaceDeclaration(node);
    } else if (ts.isClassDeclaration(node)) {
      node = processClassDeclaration(node);
    } else if (ts.isModuleDeclaration(node) && node.body && ts.isModuleBlock(node.body)) {
      const newStatements = [];
      node.body.statements.forEach((statement) => {
        if (!isSystemapi(statement)) {
          newStatements.push(statement);
        }
      });
      const newModuleBody = ts.factory.updateModuleBlock(node.body, newStatements);
      node = ts.factory.updateModuleDeclaration(
        node,
        node.modifiers,
        node.name,
        newModuleBody
      );
    } else if (ts.isEnumDeclaration(node)) {
      node = processEnumDeclaration(node);
    } else if (ts.isStructDeclaration(node)) {
      node = processStructDeclaration(node);
    }
    return ts.visitEachChild(node, processAllNodes, context);
  }
}

/**
 * 处理interface子节点 
 */
function processInterfaceDeclaration(node) {
  const newMembers = [];
  node.members.forEach((member) => {
    if (!isSystemapi(member)) {
      newMembers.push(member);
    }
  });
  node = ts.factory.updateInterfaceDeclaration(
    node,
    node.modifiers,
    node.name,
    node.typeParameters,
    node.heritageClauses,
    newMembers
  );
  return node;
}

/**
 * 处理class子节点 
 */
function processClassDeclaration(node) {
  const newMembers = [];
  node.members.forEach((member) => {
    if (!isSystemapi(member)) {
      newMembers.push(member);
    }
  });
  node = ts.factory.updateClassDeclaration(
    node,
    node.modifiers,
    node.name,
    node.typeParameters,
    node.heritageClauses,
    newMembers
  );
  return node;
}

/**
 * 处理enum子节点 
 */
function processEnumDeclaration(node) {
  const newMembers = [];
  node.members.forEach((member) => {
    if (!isSystemapi(member)) {
      newMembers.push(member);
    }
  });
  node = ts.factory.updateEnumDeclaration(
    node,
    node.modifiers,
    node.name,
    newMembers
  );
  return node;
}

/**
 * 处理struct子节点 
 */
function processStructDeclaration(node) {
  const newMembers = [];
  node.members.forEach((member, index) => {
    if (index >= 1 && !isSystemapi(member)) {
      newMembers.push(member);
    }
  });
  node = ts.factory.updateStructDeclaration(
    node,
    node.modifiers,
    node.name,
    node.typeParameters,
    node.heritageClauses,
    newMembers
  );
  return node;
}

/**
 * 解析reference
 * @param {string} url reference文件地址
 */
function resolveReferences(url) {
  const obj = referencesMap.get(url);
  let references = obj.references;
  if (!references || references.length === 0) {
    return;
  }
  for (let index = 0; index < references.length; index++) {
    const element = references[index];
    element.match(PATT.GET_REFERENCEURL);
    let referencePath = RegExp.$2;
    referencePath = referencesToOthers(referencePath, REFERENCE_TYPE.TOLOCAL);
    let fullReferencePath = path.resolve(path.dirname(url), referencePath);
    if (fs.existsSync(fullReferencePath) && !referencesModuleMap.has(fullReferencePath)) {
      const content = fs.readFileSync(fullReferencePath, 'utf-8'); //文件内容
      const fileName = processFileName(fullReferencePath);
      ts.transpileModule(content, {
        compilerOptions: {
          target: ts.ScriptTarget.ES2017,
        },
        fileName: fileName,
        transformers: { before: [resolveCallback(fullReferencePath)] },
      });
    }
  }
}
function resolveCallback(url) {
  return (context) => {
    const allReferencesIdentifierSet = new Set([]);
    let allModule = {};
    return (node) => {
      const referenceSourceFile = node;
      node.statements.forEach((statement) => {
        if (
          ts.isModuleDeclaration(statement) &&
          statement.name &&
          ts.isStringLiteral(statement.name) &&
          statement.body &&
          ts.isModuleBlock(statement.body) &&
          !isSystemapi(statement)
        ) {
          ts.visitEachChild(statement, collectAllIdentifier, context);
          dealExternalStatements(referenceSourceFile);
          allModule[statement.name.text.toString()] = [...allReferencesIdentifierSet];
          allReferencesIdentifierSet.clear();
        }
      });
      referencesModuleMap.set(url, allModule);
      allModule = {};
      return node;
    };
    function dealExternalStatements(node) {
      node.statements.forEach((statement) => {
        let name = '';
        if (isSystemapi(statement)) {
          if (ts.isVariableStatement(statement)) {
            name = variableStatementGetEscapedText(statement);
          } else if (
            ts.isInterfaceDeclaration(statement) ||
            ts.isClassDeclaration(statement) ||
            ts.isEnumDeclaration(statement)
          ) {
            if (statement && statement.name && statement.name.escapedText) {
              name = statement.name.escapedText.toString();
            }
          }
          allReferencesIdentifierSet.delete(name);
        }
      });
    }
    function collectAllIdentifier(node) {
      if (isSystemapi(node)) {
        return node;
      }
      if (ts.isIdentifier(node)) {
        allReferencesIdentifierSet.add(node.escapedText.toString());
      }
      return ts.visitEachChild(node, collectAllIdentifier, context);
    }
  };
}

function variableStatementGetEscapedText(statement) {
  let name = '';
  if (
    statement &&
    statement.declarationList &&
    statement.declarationList.declarations &&
    statement.declarationList.declarations.length > 0 &&
    statement.declarationList.declarations[0].name &&
    statement.declarationList.declarations[0].name.escapedText
  ) {
    name = statement.declarationList.declarations[0].name.escapedText.toString();
  }
  return name;
}

function isSystemapi(node) {
  const notesContent = node.getFullText().replace(node.getText(), '').replace(/[\s]/g, '');
  const notesArr = notesContent.split(/\/\*\*/);
  const notesStr = notesArr[notesArr.length - 1];
  if (notesStr.length !== 0) {
    return /@systemapi/g.test(notesStr);
  }
  return false;
}

function isEmptyFile(node) {
  let isEmpty = true;
  if (ts.isSourceFile(node) && node.statements) {
    for (let i = 0; i < node.statements.length; i++) {
      const statement = node.statements[i];
      if (ts.isImportDeclaration(statement)) {
        continue;
      }
      isEmpty = false;
      break;
    }
  }
  return isEmpty;
}

const apiSourcePath = '../api';
const outputPath = path.resolve(__dirname, 'output');
collectDeclaration(apiSourcePath); //入口
