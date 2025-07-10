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
import path from 'path';
import chai from 'chai';
import {
  describe,
  it
} from 'mocha';
const expect = chai.expect;

import {
  validateUISyntax,
  resetComponentCollection,
  sourceReplace,
  componentCollection
} from '../lib/validate_ui_syntax';
import { processUISyntax } from '../lib/process_ui_syntax';
import {
  componentInfo,
  readFile
} from '../lib/utils';
import { BUILD_ON } from '../lib/pre_define';

function expectActual(name: string, filePath: string) {
  const content: any = require(filePath);
  const source: string = content.source;
  process.env.compiler = BUILD_ON;
  const afterProcess = sourceReplace(source);
  validateUISyntax(source, afterProcess, `${name}.ts`);
  const result: ts.TranspileOutput = ts.transpileModule(afterProcess, {
    compilerOptions: {
      "target": ts.ScriptTarget.ES2021
    },
    fileName: `${name}.ts`,
    transformers: { before: [processUISyntax(null, true)] }
  });
  componentInfo.id = 0;
  componentCollection.customComponents.clear();
  resetComponentCollection();
  expect(result.outputText).eql(content.expectResult);
}

describe('compiler', () => {
  const utPath: string = path.resolve(__dirname, './ut');
  const utFiles: string[] = [];
  readFile(utPath, utFiles);
  utFiles.forEach((item) => {
    const fileName: string = path.basename(item, '.ts');
    it(fileName, () => {
      expectActual(fileName, item);
    })
  })
})
