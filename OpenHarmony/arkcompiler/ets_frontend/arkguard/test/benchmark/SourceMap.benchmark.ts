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


import type { RawSourceMap } from 'typescript';
import Benchmark from 'benchmark';
import {
  ArkObfuscator,
  renameIdentifierModule
} from '../../src/ArkObfuscator';
import {
  decodeSourcemap,
  mergeSourceMap,
  Source,
  SourceMapLink
} from '../../src/utils/SourceMapMergingUtil';
import {
  IDENTIFIER_CACHE,
  MEM_METHOD_CACHE,
} from '../../src/utils/NameCacheUtil';

const benchmarkSuite = new Benchmark.Suite;
const namePaddingSize: number = 56;
const dataPaddingSize: number = 16;

const previousMap = {
  version: 3,
  file: 'EntryAbility.ts',
  sourceRoot: '',
  sources: [ 'entry/src/main/ets/entryability/EntryAbility.ts' ],
  names: [],
  mappings: 'OAAO,SAAS;OACT,KAAK;YACL,MAAM;AAEb,MAAM,CAAC,OAAO,OAAO,YAAa,SAAQ,SAAS;IACjD,QAAQ,CAAC,IAAI,EAAE,WAAW;'
    + 'QACxB,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,kBAAkB,CAAC,CAAC;IAClE,CAAC;'
    + 'IAED,SAAS;QACP,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,mBAAmB,CAAC,CAAC;'
    + 'IACnE,CAAC;IAED,mBAAmB,CAAC,WAAW,EAAE,MAAM,CAAC,WAAW;QACjD,yDAAyD;'
    + 'QACzD,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,6BAA6B,CAAC,CAAC;'
    + 'QAE3E,WAAW,CAAC,WAAW,CAAC,aAAa,EAAE,CAAC,GAAG,EAAE,IAAI,EAAE,EAAE;YACnD,IAAI,GAAG,CAAC,IAAI,EAAE;'
    + 'gBACZ,KAAK,CAAC,KAAK,CAAC,MAAM,EAAE,SAAS,EAAE,+CAA+C,EAAE,IAAI,CAAC,SAAS,CAAC,GAAG,CAAC,IAAI,EAAE,CAAC,CAAC;'
    + 'gBAC3G,OAAO;aACR;YACD,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,oDAAoD,EAAE,IAAI,CAAC,SAAS,CAAC,IAAI,CAAC,IAAI,EAAE,CAAC,CAAC;'
    + 'QAClH,CAAC,CAAC,CAAC;IACL,CAAC;IAED,oBAAoB;QAClB,yDAAyD;QACzD,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,8BAA8B,CAAC,CAAC;'
    + 'IAC9E,CAAC;IAED,YAAY;QACV,oCAAoC;QACpC,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,sBAAsB,CAAC,CAAC;IACtE,CAAC;'
    + 'IAED,YAAY;QACV,iCAAiC;QACjC,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,sBAAsB,CAAC,CAAC;IACtE,CAAC;CACF',
  sourcesContent: undefined
};

const currentMap = {
  version: 3,
  file: 'EntryAbility.ts',
  sourceRoot: '',
  sources: [
    'entry/build/default/cache/default/default@CompileArkTS/esmodule/release/entry/src/main/ets/entryability/EntryAbility.ts'
  ],
  names: [],
  mappings: 'AAAA,OAAO,SAAS,MAAM,6BAA6B,CAAC;AACpD,OAAO,KAAK,MAAM,aAAa,CAAC;AAChC,OAAO,KAAK,MAAM,MAAM,cAAc,CAAC;'
    + 'AACvC,MAAM,CAAC,OAAO,OAAO,YAAa,SAAQ,SAAS;IAC/C,QAAQ,CAAC,CAAI,EAAE,CAAW;'
    + 'QACtB,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,kBAAkB,CAAC,CAAC;'
    + 'IACpE,CAAC;IACD,SAAS;QACL,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,mBAAmB,CAAC,CAAC;'
    + 'IACrE,CAAC;IACD,mBAAmB,CAAC,GAAa,MAAM,CAAC,WAAW;QAE/C,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,6BAA6B,CAAC,CAAC;'
    + 'QAC3E,EAAY,WAAW,CAAC,aAAa,EAAE,CAAC,CAAG,EAAE,CAAI,EAAE,EAAE;YACjD,IAAI,EAAI,IAAI,EAAE;'
    + 'gBACV,KAAK,CAAC,KAAK,CAAC,MAAM,EAAE,SAAS,EAAE,+CAA+C,EAAE,IAAI,CAAC,SAAS,GAAK,IAAI,EAAE,CAAC,CAAC;'
    + 'gBAC3G,OAAO;aACV;YACD,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,oDAAoD,EAAE,IAAI,CAAC,SAAS,GAAM,IAAI,EAAE,CAAC,CAAC;'
    + 'QACpH,CAAC,CAAC,CAAC;IACP,CAAC;IACD,oBAAoB;QAEhB,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,8BAA8B,CAAC,CAAC;'
    + 'IAChF,CAAC;IACD,YAAY;QAER,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,sBAAsB,CAAC,CAAC;IACxE,CAAC;IACD,YAAY;'
    + 'QAER,KAAK,CAAC,IAAI,CAAC,MAAM,EAAE,SAAS,EAAE,YAAY,EAAE,sBAAsB,CAAC,CAAC;IACxE,CAAC;CACJ',
  sourcesContent: undefined
};

renameIdentifierModule.nameCache = new Map([
  [
    'IdentifierCache',
    new Map([
      ['EntryAbility#onWindowStageCreate#windowStage', 'a'],
      ['EntryAbility#onWindowStageCreate#__function', 'b'],
      ['EntryAbility#onWindowStageCreate#$0#err', 'c'],
      ['EntryAbility#onWindowStageCreate#$0#data', 'd'],
      ['EntryAbility#onCreate#want', 'e'],
      ['EntryAbility#onCreate#launchParam', 'f']
    ])
  ],
  [
    'MemberMethodCache',
    new Map([
      ['onCreate:5:5:7:6', 'onCreate'],
      ['onDestroy:8:5:10:6', 'onDestroy'],
      ['onWindowStageCreate:11:5:20:6', 'onWindowStageCreate'],
      ['onWindowStageDestroy:21:5:23:6', 'onWindowStageDestroy'],
      ['onForeground:24:5:26:6', 'onForeground'],
      ['onBackground:27:5:29:6', 'onBackground']
    ])
  ]
]);

function alignColumn(name, ops, variance, runsSampled) {
  const namePadding = name.padEnd(namePaddingSize);
  const opsPadding = ops.padEnd(dataPaddingSize);
  const variancePadding = variance.padEnd(dataPaddingSize);
  const runsSampledPadding = runsSampled.toString().padEnd(dataPaddingSize);
  return `| ${namePadding} | ${opsPadding} | ${variancePadding} | ${runsSampledPadding} |`;
}

class ArkObfuscatorMock extends ArkObfuscator {
  convertLineBasedOnSourceMapMock(targetCache: string, sourceMapLink?: SourceMapLink): Map<string, string> {
      // Ignore `property is private and only accessible within class` error
      // @ts-ignore
      return this.convertLineBasedOnSourceMap(targetCache, sourceMapLink);
  }
};

describe('SourceMap Benchmark Test', function() {
  after(() => {
      benchmarkSuite.on('complete', function() {
        console.log('\n  Benchmark Summary Results:\n');
        console.log(' ', alignColumn('Test Name', 'Ops/Sec', 'Variance', 'Number of Runs'));
        console.log(' ',alignColumn('-'.repeat(namePaddingSize), '-'.repeat(dataPaddingSize),
                                '-'.repeat(dataPaddingSize), '-'.repeat(dataPaddingSize)));
        this.forEach(result => {
          console.log(' ', alignColumn(result.name, `${Math.round(result.hz)}`,
                      `${result.stats.rme.toFixed(2)}%`, result.stats.sample.length));
        });
        console.log('\n');
      });
  });

  it('run sourcemap benchmark test', function() {
      let decodedSourceMap;
      benchmarkSuite.add('decodeSourcemap', function() {
        decodedSourceMap = decodeSourcemap(previousMap as RawSourceMap);
      });

      benchmarkSuite.add('mergeSourceMap', function() {
        mergeSourceMap(previousMap as RawSourceMap, currentMap as RawSourceMap);
      });

      benchmarkSuite.add('convertLineBasedOnSourceMap', function() {
        const sourceFileName = previousMap.sources?.length === 1 ? previousMap.sources[0] : '';
        const source = new Source(sourceFileName, null);
        const sourceMapLink = new SourceMapLink(decodedSourceMap!, [source]);
        const arkObfuscator = new ArkObfuscatorMock();
        arkObfuscator.convertLineBasedOnSourceMapMock(IDENTIFIER_CACHE, sourceMapLink);
        arkObfuscator.convertLineBasedOnSourceMapMock(MEM_METHOD_CACHE, sourceMapLink);
      });

      benchmarkSuite.on('cycle', (event: any) => {
        console.log(' ', String(event.target));
      });

      benchmarkSuite.run({ 'async': true });
  });
});