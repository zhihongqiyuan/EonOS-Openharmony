/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

import Benchmark from 'benchmark';
import { IOptions } from '../../src/configs/IOptions';
import { createSourceFile, Node, ScriptTarget, SourceFile, transform, TransformerFactory } from 'typescript';
import { transformerPlugin } from '../../src/transformers/rename/RenameIdentifierTransformer';

const benchmarkSuite = new Benchmark.Suite;
const namePaddingSize: number = 56;
const dataPaddingSize: number = 16;

function alignColumn(name, ops, variance, runsSampled) {
  const namePadding = name.padEnd(namePaddingSize);
  const opsPadding = ops.padEnd(dataPaddingSize);
  const variancePadding = variance.padEnd(dataPaddingSize);
  const runsSampledPadding = runsSampled.toString().padEnd(dataPaddingSize);
  return `| ${namePadding} | ${opsPadding} | ${variancePadding} | ${runsSampledPadding} |`;
}

describe('RenameProperty Benchmark Test', function() {
  after(() => {
    benchmarkSuite.on('complete', function () {
      console.log('\n  Benchmark Summary Results:\n');
      console.log(' ', alignColumn('Test Name', 'Ops/Sec', 'Variance', 'Number of Runs'));
      console.log(
        ' ',
        alignColumn(
          '-'.repeat(namePaddingSize),
          '-'.repeat(dataPaddingSize),
          '-'.repeat(dataPaddingSize),
          '-'.repeat(dataPaddingSize),
        ),
      );
      this.forEach((result) => {
        console.log(
          ' ',
          alignColumn(
            result.name,
            `${Math.round(result.hz)}`,
            `${result.stats.rme.toFixed(2)}%`,
            result.stats.sample.length,
          ),
        );
      });
      console.log('\n');
    });
  });

  it('run RenameProperty benchmark test', function () {
    let fileContent = `
      class TestClass{
        prop1 = 1; prop2 = 2; prop3 = 3; prop4 = 4; prop5 = 5;
        prop6 = 6; prop7 = 7; prop8 = 8; prop9 = 9; prop10 = 10;
        prop11 = 11; prop12 = 12; prop13 = 13; prop14 = 14; prop15 = 15;
        prop16 = 16; prop17 = 17; prop18 = 18; prop19 = 19; prop20 = 20;
      }
    `;
    let option: IOptions = {
      mCompact: false,
      mRemoveComments: false,
      mOutputDir: '',
      mDisableConsole: false,
      mSimplify: false,
      mNameObfuscation: {
        mEnable: true,
        mNameGeneratorType: 1,
        mDictionaryList: [],
        mRenameProperties: true,
        mKeepStringProperty: false,
        mTopLevel: false,
        mReservedProperties: [],
      },
      mEnableSourceMap: false,
      mEnableNameCache: false,
    };
    let transformer: TransformerFactory<Node> = transformerPlugin.createTransformerFactory(option);
    const sourceFile: SourceFile = createSourceFile('demo.ts', fileContent, ScriptTarget.ES2015, true);
    
    benchmarkSuite.add('property obfuscation test', function () {
      let transformed = transform(sourceFile, [transformer]);
    });

    benchmarkSuite.on('cycle', (event: any) => {
      console.log(' ', String(event.target));
    });

    benchmarkSuite.run({ async: true });
  });
});