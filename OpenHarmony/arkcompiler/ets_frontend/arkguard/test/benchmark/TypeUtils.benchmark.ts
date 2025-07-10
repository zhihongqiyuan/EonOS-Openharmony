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
import { createSourceFile, ScriptTarget, SourceFile } from 'typescript';
import { TypeUtils } from '../../src/utils/TypeUtils';

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

describe('TypeUtils Benchmark Test', function () {
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

  it('run createChecker benchmark test', function () {
    let fileContent = `
      import {var1} from 'dependency1';
      import {var2} from 'dependency2';
      import {var3} from 'dependency3';
      import {var4} from 'dependency4';
      import {var5} from 'dependency5';
      import {var6} from 'dependency6';
      import {var7} from 'dependency7';
      import {var8} from 'dependency8';
      import {var9} from 'dependency9';
      import {var10} from 'dependency10';
    `;
    const sourceFile: SourceFile = createSourceFile('demo.ts', fileContent, ScriptTarget.ES2015, true);

    benchmarkSuite.add('createChecker test', function () {
      let typeChecker = TypeUtils.createChecker(sourceFile);
    });

    benchmarkSuite.on('cycle', (event: any) => {
      console.log(' ', String(event.target));
    });

    benchmarkSuite.run({ async: true });
  });
});