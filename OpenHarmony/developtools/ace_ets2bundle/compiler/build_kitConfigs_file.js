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

const fs = require('fs');
const path = require('path');
const ts = require('typescript');
const SECOND_PARAM = 2;
const THIRD_PARAM = 3;
const FOURTH_PARAM = 4;
const FIFTH_PARAM = 5;
const systemModules = [];

function generateKitConfig(kitFilePath, output, apiFilePath, arktsFilePath) {
  readSystemApis(apiFilePath, systemModules);
  readSystemApis(arktsFilePath, systemModules);
  const kitFiles = [];
  readFile(kitFilePath, kitFiles);
  if (fs.existsSync(output)) {
    removeDir(output);
  }
  mkDir(output);
  kitFiles.forEach((item) => {
    let content = fs.readFileSync(item, 'utf8');
    const outputPath = path.resolve(output, path.basename(item).replace('.d.ts', '.json'));
    const symbol = {};
    const kitSourceFile = ts.createSourceFile(item, content, ts.ScriptTarget.Latest, true, ts.ScriptKind.TS);
    if (kitSourceFile.statements && kitSourceFile.statements.length > 0) {
      kitSourceFile.statements.forEach(statement => {
        getImportDeclarationInfo(statement, symbol);
      });
    }
    const result = {
      'symbols': symbol
    };
    const STANDARD_INDENT = 2;
    createKitConfigs(outputPath, JSON.stringify(result, null, STANDARD_INDENT));
  });
}

function getImportDeclarationInfo(statement, symbol) {
  if (!ts.isImportDeclaration(statement)) {
    return;
  }
  let source = '';
  if (statement.moduleSpecifier && ts.isStringLiteral(statement.moduleSpecifier)) {
    source = statement.moduleSpecifier.getText().replace(/('|")*/g, '');
    for (let i = 0; i < systemModules.length; i++) {
      const moduleName = systemModules[i];
      if (moduleName.replace(/(\.d\.ts|\.d\.ets)$/, '') === source) {
        source = moduleName;
        break;
      }
    }
  }
  if (statement.importClause) {
    const clause = statement.importClause;
    if (clause.name) {
      addSymbol(symbol, clause.name.getText(), source, 'default');
    }
    if (clause.namedBindings) {
      const binding = clause.namedBindings;
      if (ts.isNamespaceImport(binding) && binding.name) {
        addSymbol(symbol, binding.name.getText(), source, 'default');
      } else if (ts.isNamedImports(binding) && binding.elements && binding.elements.length > 0) {
        processNamedImports(binding.elements, symbol, source);
      }
    }
  }
}

function processNamedImports(elements, symbol, source) {
  elements.forEach(element => {
    if (ts.isImportSpecifier(element)) {
      const name = element.name.getText();
      const bindingsName = element.propertyName ? element.propertyName.getText() : name;
      addSymbol(symbol, name, source, bindingsName);
    }
  });
}

function addSymbol(symbol, name, source, bindings) {
  symbol[name] = {
    source: source,
    bindings: bindings
  };
}

function readFile(dir, fileDir) {
  const files = fs.readdirSync(dir);
  files.forEach((element) => {
    const filePath = path.join(dir, element);
    const status = fs.statSync(filePath);
    if (status.isDirectory()) {
      readFile(filePath, fileDir);
    } else {
      fileDir.push(filePath);
    }
  });
}

function readSystemApis(dir, fileDir) {
  const files = fs.readdirSync(dir);
  files.forEach(file => {
    const filePath = path.join(dir, file);
    const status = fs.statSync(filePath);
    if (!status.isDirectory()) {
      fileDir.push(file);
    }
  });
}

function mkDir(filePath) {
  const parent = path.join(filePath, '..');
  if (!(fs.existsSync(parent) && !fs.statSync(parent).isFile())) {
    mkDir(parent);
  }
  fs.mkdirSync(filePath);
}

function removeDir(path) {
  if (fs.existsSync(path)) {
    fs.rmdirSync(path, {recursive: true});
  }
}

function createKitConfigs(fileName, content) {
  fs.writeFile(fileName, content, err => {
    if (err) {
      console.error(err);
      return;
    }
  });
}

generateKitConfig(process.argv[SECOND_PARAM], process.argv[THIRD_PARAM], process.argv[FOURTH_PARAM], process.argv[FIFTH_PARAM]);
