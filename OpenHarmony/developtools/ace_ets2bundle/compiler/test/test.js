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

const ts = require('typescript');
const path = require('path');
const chai = require('chai');
const mocha = require('mocha');
const fs = require('fs');
const expect = chai.expect;
const {
  processUISyntax,
  transformLog
} = require('../lib/process_ui_syntax');
const {
  validateUISyntax,
  preprocessExtend,
  resetComponentCollection,
  componentCollection
} = require('../lib/validate_ui_syntax');
const {
  componentInfo,
  readFile,
  storedFileInfo
} = require('../lib/utils');
const {
  BUILD_ON,
  OHOS_PLUGIN,
  NATIVE_MODULE,
  SYSTEM_PLUGIN
} = require('../lib/pre_define');
const {
  projectConfig,
  resources
} = require('../main');
const processStructComponentV2 = require('../lib/process_struct_componentV2');
const { getCompilerHost } = require('../lib/fast_build/ets_ui/rollup-plugin-ets-typescript');
const { processKitImport } = require('../lib/process_kit_import');
const { ModuleSourceFile } = require('../lib/fast_build/ark_compiler/module/module_source_file');

projectConfig.projectPath = path.resolve(process.cwd());

function processModule(compilerOptions, etsFilePath) {
  const compilerHost = getCompilerHost();
  const tsProgram = ts.createProgram([etsFilePath], compilerOptions, compilerHost);
  tsProgram.emit(tsProgram.getSourceFile(etsFilePath), undefined, undefined, undefined,
    {
      before: [
        processUISyntax(null, true),
        processKitImport(etsFilePath, undefined, undefined, false)
      ]
    }
  );
}

function processIntermediateTS(content, filePath, afterProcess) {
  projectConfig.processTs = true;
  const etsCheckerCompilerOptions = require('../lib/ets_checker').compilerOptions;
  etsCheckerCompilerOptions.lib = ["lib.es2021.d.ts"];
  if (content.etsAnnotationsEnable) {
    Object.assign(etsCheckerCompilerOptions, {
      'etsAnnotationsEnable': true
    });
  }
  const etsFilePath = filePath.replace('.ts', '.ets')
  fs.writeFileSync(etsFilePath, afterProcess.content);
  processModule(etsCheckerCompilerOptions, etsFilePath);
  sourceFile = ModuleSourceFile.getSourceFiles().find((element) => element.moduleId.includes(etsFilePath));
  result = generateIntermediateContent(sourceFile);
  fs.unlinkSync(etsFilePath);
  projectConfig.processTs = false;
  return result
}

function expectActual(name, filePath, checkError = false) {
  transformLog.errors = [];
  resources.app["media"] = {icon:16777222};
  resources.app["font"] = {song:16777223};
  process.env.rawFileResource = './';
  process.env.compileMode = 'moduleJson';
  const content = require(filePath);
  const source = content.source;
  process.env.compiler = BUILD_ON;
  componentInfo.id = 0;
  componentCollection.customComponents.clear();
  resetComponentCollection();
  storedFileInfo.setCurrentArkTsFile();
  const afterProcess = sourceReplace(source);
  if (checkError) {
    transformLog.errors.push(...validateUISyntax(source, afterProcess.content, `${name}.ets`, "?entry"));
  } else {
    validateUISyntax(source, afterProcess.content, `${name}.ets`);
  }
  const compilerOptions = ts.readConfigFile(
    path.resolve(__dirname, '../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
  Object.assign(compilerOptions, {
    'sourceMap': false
  });
  let actualResult = '';
  if (content.emitIntermediateTS) {
    actualResult = processIntermediateTS(content, filePath, afterProcess);
  } else {
    const result = ts.transpileModule(afterProcess.content, {
      compilerOptions: compilerOptions,
      fileName: `${name}.ets`,
      transformers: { before: [processUISyntax(null, true)] }
    });
    actualResult = result.outputText;
  }
  processStructComponentV2.default.resetStructMapInEts();
  expect(actualResult).eql(content.expectResult);
}

function generateIntermediateContent(sourceFile) {
  const printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
  const writer = ts.createTextWriter(
    ts.getNewLineCharacter({ newLine: ts.NewLineKind.LineFeed, removeComments: false }));
  printer.writeFile(sourceFile.source, writer, undefined);
  return writer.getText();
}

mocha.describe('compiler', () => {
  let utPath = path.resolve(__dirname, './ut');
  const utFiles = [];
  readFile(utPath, utFiles);
  utFiles.forEach((item) => {
    const fileName = path.basename(item, '.ts');
    mocha.it(fileName, () => {
      expectActual(fileName, item);
    });
  });
});

function sourceReplace(source) {
  let content = source;
  const log = [];
  content = preprocessExtend(content);
  content = processSystemApi(content);
  return {
    content: content,
    log: log
  };
}

function processSystemApi(content) {
  const REG_SYSTEM = 
    /import\s+(.+)\s+from\s+['"]@(system|ohos)\.(\S+)['"]|import\s+(.+)\s*=\s*require\(\s*['"]@(system|ohos)\.(\S+)['"]\s*\)/g;
  const REG_LIB_SO =
    /import\s+(.+)\s+from\s+['"]lib(\S+)\.so['"]|import\s+(.+)\s*=\s*require\(\s*['"]lib(\S+)\.so['"]\s*\)/g;
  const newContent = content.replace(REG_LIB_SO, (_, item1, item2, item3, item4) => {
    const libSoValue = item1 || item3;
    const libSoKey = item2 || item4;
    return `var ${libSoValue} = globalThis.requireNapi("${libSoKey}", true);`;
  }).replace(REG_SYSTEM, (item, item1, item2, item3, item4, item5, item6, item7) => {
    let moduleType = item2 || item5;
    let systemKey = item3 || item6;
    let systemValue = item1 || item4;
    if (NATIVE_MODULE.has(`${moduleType}.${systemKey}`)) {
      item = `var ${systemValue} = globalThis.requireNativeModule('${moduleType}.${systemKey}')`;
    } else if (moduleType === SYSTEM_PLUGIN) {
      item = `var ${systemValue} = isSystemplugin('${systemKey}', '${SYSTEM_PLUGIN}') ? ` +
        `globalThis.systemplugin.${systemKey} : globalThis.requireNapi('${systemKey}')`;
    } else if (moduleType === OHOS_PLUGIN) {
      item = `var ${systemValue} = globalThis.requireNapi('${systemKey}') || ` +
        `(isSystemplugin('${systemKey}', '${OHOS_PLUGIN}') ? ` +
        `globalThis.ohosplugin.${systemKey} : isSystemplugin('${systemKey}', '${SYSTEM_PLUGIN}') ` +
        `? globalThis.systemplugin.${systemKey} : undefined)`;
    }
    return item;
  });
  return newContent;
}
