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

describe('RenameIdentifier Benchmark Test', function() {
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

  it('run RenameIdentifier benchmark test', function () {
    let fileContent = `
      let var1 = 1;\nlet var2 = 1;\nlet var3 = 1;\nlet var4 = 1;\nlet var5 = 1;\nlet var6 = 1;\nlet var7 = 1;\n
      let var8 = 1;\nlet var9 = 1;\nlet var10 = 1;\nlet var11 = 1;\nlet var12 = 1;\nlet var13 = 1;\nlet var14 = 1;\n
      let var15 = 1;\nlet var16 = 1;\nlet var17 = 1;\nlet var18 = 1;\nlet var19 = 1;\nlet var20 = 1;\nlet var21 = 1;\n
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
        mRenameProperties: false,
        mKeepStringProperty: false,
        mTopLevel: true,
        mReservedProperties: [],
      },
      mEnableSourceMap: false,
      mEnableNameCache: false,
    };
    let transformer: TransformerFactory<Node> = transformerPlugin.createTransformerFactory(option);
    const sourceFile: SourceFile = createSourceFile('demo.ts', fileContent, ScriptTarget.ES2015, true);

    benchmarkSuite.add('identifier obfuscation test', function () {
      let transformed = transform(sourceFile, [transformer]);
    });

    benchmarkSuite.on('cycle', (event: any) => {
      console.log(' ', String(event.target));
    });

    benchmarkSuite.run({ async: true });
  });
});