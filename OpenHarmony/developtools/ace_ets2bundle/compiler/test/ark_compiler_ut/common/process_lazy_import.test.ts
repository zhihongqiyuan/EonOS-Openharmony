/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use rollupObject file except in compliance with the License.
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

import mocha from 'mocha';
import path from 'path';
import * as ts from 'typescript';
import { expect } from 'chai';

import {
  processKitImport
} from '../../../lib/process_kit_import';
import {
  processJsCodeLazyImport,
  reExportNoCheckMode,
  reExportCheckLog,
  resetReExportCheckLog
} from '../../../lib/process_lazy_import';
import { RELEASE } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { ModuleSourceFile } from '../../../lib/fast_build/ark_compiler/module/module_source_file';
import { projectConfig } from '../../../main';
import { createErrInfo } from '../utils/utils';
import RollUpPluginMock from './../mock/rollup_mock/rollup_plugin_mock';

const TRANSFORM_LAZY_IMPORT_CODE: string =
`
  import { test } from "./test";
  import { test1 as t } from "./test1";
  import { Ability } from "@kit.AbilityKit";
  import { featureAbility as k } from "@kit.AbilityKit";
  import { AbilityConstant, abilityAccessCtrl } from "@kit.AbilityKit";
  import test2 from "./test2";
  import type { testType } from "./testType";
  import * as ns3 from "./test3";
  import test4, { test5 } from "./test4";
  import test6, * as ns6 from "./test6";
  import type dt from "./test7";
  let a: testType = test + t + test2 + testType + ns3.b + test4 + test5 + test6 + ns6.b + dt;
  let localAbility = new Ability();
  let localFeatureAbility = new k();
  let localAbilityConstant = new AbilityConstant();
  let localAbilityAccessCtrl = new abilityAccessCtrl();
`

const TRANSFORM_LAZY_IMPORT_CODE_EXPECT: string =
'import lazy { test } from "./test";\n' +
'import lazy { test1 as t } from "./test1";\n' +
'import lazy Ability from "@ohos.app.ability.Ability";\n' +
'import lazy k from "@ohos.ability.featureAbility";\n' +
'import lazy AbilityConstant from "@ohos.app.ability.AbilityConstant";\n' +
'import lazy abilityAccessCtrl from "@ohos.abilityAccessCtrl";\n' +
'import lazy test2 from "./test2";\n' +
'import type { testType } from "./testType";\n' +
'import * as ns3 from "./test3";\n' +
'import lazy test4, { test5 } from "./test4";\n' +
'import test6, * as ns6 from "./test6";\n' +
'import type dt from "./test7";\n' +
'let a: testType = test + t + test2 + testType + ns3.b + test4 + test5 + test6 + ns6.b + dt;\n' +
'let localAbility = new Ability();\n' +
'let localFeatureAbility = new k();\n' +
'let localAbilityConstant = new AbilityConstant();\n' +
'let localAbilityAccessCtrl = new abilityAccessCtrl();\n';

const LAZY_IMPORT_RE_EXPORT_ERROR_TS: string =
'import lazy { e1 } from "./test1";\n' +
'import lazy { e2, e3 as a3 } from "./test1";\n' +
'import lazy d1 from "./test1";\n' +
'import lazy d2, { e4 as a4, e5 } from "./test2";\n' +
'import lazy { componentUtils } from "@kit.ArkUI";\n' +
'import lazy { dragController as k1, uiObserver } from "@kit.ArkUI";\n' +
'import lazy { lazy } from "./test3"\n' +
'import lazy { e6, type type1 } from "./testType1"\n' +
'import lazy d3, { type type2 } from "./testType2"\n' +
'export { e1 };\n' +
'export { e2, a3 };\n' +
'export { d1, d2, a4, e5, componentUtils, k1, uiObserver, lazy, e6, type1, d3, type2 };';

const LAZY_IMPORT_RE_EXPORT_ERROR_JS: string =
'import lazy { e1 } from "./test1";\n' +
'import lazy { e2, e3 as a3 } from "./test1";\n' +
'import lazy d1 from "./test1";\n' +
'import lazy d2, { e4 as a4, e5 } from "./test3";\n' +
'import lazy { componentUtils } from "@kit.ArkUI";\n' +
'import lazy { dragController as k1, uiObserver } from "@kit.ArkUI";\n' +
'import lazy { lazy } from "./test3"\n' +
'export { e1 };\n' +
'export { e2, a3 };\n' +
'export { d1, d2, a4, e5, componentUtils, k1, uiObserver, lazy };';

const compilerOptions = ts.readConfigFile(
  path.resolve(__dirname, '../../../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
compilerOptions['moduleResolution'] = 'nodenext';
compilerOptions['module'] = 'es2020';

mocha.describe('process Lazy Imports tests', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test transformLazyImport (ts.sourceFile): perform lazy conversion', function () {
    projectConfig.processTs = true;
    ts.transpileModule(TRANSFORM_LAZY_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: 'test.ets',
      transformers: {
        before: [
          processKitImport('test.ets', undefined, undefined, true, { autoLazyImport: true, reExportCheckMode: reExportNoCheckMode })
        ]
      }
    });
    const sourceFile: ts.SourceFile = ModuleSourceFile.getSourceFiles().find(element => element.moduleId === 'test.ets');
    const printer: ts.Printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
    // @ts-ignore
    const writer: ts.EmitTextWriter = ts.createTextWriter(
      // @ts-ignore
      ts.getNewLineCharacter({ newLine: ts.NewLineKind.LineFeed, removeComments: false }));
    printer.writeFile(sourceFile.source, writer, undefined);
    expect(writer.getText() === TRANSFORM_LAZY_IMPORT_CODE_EXPECT).to.be.true;
    projectConfig.processTs = false;
  });

  mocha.it('1-2: test transformLazyImport (ts.sourceFile): no lazy conversion', function () {
    const code: string = `
    import lazy { test } from "./test";
    import lazy { test1 as t } from "./test1";
    import test2 from "./test2";
    import * as test3 from "./test3";
    import test4, { test5 } from "./test4";
    import type { testType } from "./testType";
    import "test6";
    let a: testType = test + t + test2 + test3.b + test4 + test5;
    `;
    projectConfig.processTs = true;
    ts.transpileModule(code, {
      compilerOptions: compilerOptions,
      fileName: 'no.ets',
      transformers: {
        before: [
          processKitImport('no.ets', undefined, undefined, true, { autoLazyImport: false, reExportCheckMode: reExportNoCheckMode })
        ]
      }
    });
    const sourceFile: ts.SourceFile = ModuleSourceFile.getSourceFiles().find(element => element.moduleId === 'no.ets');
    const printer: ts.Printer = ts.createPrinter({ newLine: ts.NewLineKind.LineFeed });
    // @ts-ignore
    const writer: ts.EmitTextWriter = ts.createTextWriter(
      // @ts-ignore
      ts.getNewLineCharacter({ newLine: ts.NewLineKind.LineFeed, removeComments: false }));
    printer.writeFile(sourceFile.source, writer, undefined);
    const expectCode: string = 'import lazy { test } from "./test";\n' +
    'import lazy { test1 as t } from "./test1";\n' +
    'import test2 from "./test2";\n' +
    'import * as test3 from "./test3";\n' +
    'import test4, { test5 } from "./test4";\n' +
    'import type { testType } from "./testType";\n' +
    'import "test6";\n' +
    'let a: testType = test + t + test2 + test3.b + test4 + test5;\n';
    expect(writer.getText() === expectCode).to.be.true;
  });

  mocha.it('1-3: test transformLazyImport (js code): perform lazy conversion', function () {
    const code: string = `
    import { test } from "./test";
    import { test1 as t } from "./test1";
    import test2 from "./test2";
    import * as test3 from "./test3";
    import test4, { test5 } from "./test4";
    import "test6";
    const a = "a" + test + t + test2 + test3.b + test4 + test5;
    `;
    const expectCode: string = 'import lazy { test } from "./test";\n' +
    'import lazy { test1 as t } from "./test1";\n' +
    'import lazy test2 from "./test2";\n' +
    'import * as test3 from "./test3";\n' +
    'import lazy test4, { test5 } from "./test4";\n' +
    'import "test6";\n' +
    'const a = "a" + test + t + test2 + test3.b + test4 + test5;\n';
    const result: string = processJsCodeLazyImport('index.js', code, true, reExportNoCheckMode);
    expect(result === expectCode).to.be.true;
  });

  mocha.it('1-4: test transformLazyImport (js code): no lazy conversion', function () {
    const code: string = 'import lazy { test } from "./test";\n' +
    'import lazy { test1 as t } from "./test1";\n' +
    'import { t } from "./t";\n' +
    'import { t1 as t0 } from "./t1";\n' +
    'import test2 from "./test2";\n' +
    'import * as test3 from "./test3";\n' +
    'import test4, { test5 } from "./test4";\n' +
    'import "test6";\n';
    const expectCode: string = 'import lazy { test } from "./test";\n' +
    'import lazy { test1 as t } from "./test1";\n' +
    'import { t } from "./t";\n' +
    'import { t1 as t0 } from "./t1";\n' +
    'import test2 from "./test2";\n' +
    'import * as test3 from "./test3";\n' +
    'import test4, { test5 } from "./test4";\n' +
    'import "test6";\n';
    const result: string = processJsCodeLazyImport('index.js', code, false, reExportNoCheckMode);
    expect(result === expectCode).to.be.true;
  });

  mocha.it('2-1: the error message of lazy-import re-export (ts.sourceFile)', function () {
    projectConfig.processTs = true;
    ts.transpileModule(LAZY_IMPORT_RE_EXPORT_ERROR_TS, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: {
        before: [
          processKitImport('kitTest.ts', undefined, undefined, true, { autoLazyImport: false, reExportCheckMode: 'strict' })
        ]
      }
    });
    const errorKeys = [
      'e1', 'e2', 'a3', 'd1', 'd2', 'a4', 'e5',
      'componentUtils', 'k1', 'uiObserver', 'lazy',
      'e6', 'type1', 'd3', 'type2'
    ];
    for (const key of errorKeys) {
      const errInfo = createErrInfo(key);
      const hasError = reExportCheckLog.errors.some(error =>
        error.message.includes(errInfo.toString())
      );
      expect(hasError).to.be.true;
    }
    resetReExportCheckLog();
    projectConfig.processTs = false;
  });

  mocha.it('2-2: the error message of lazy-import re-export with the name lazy (ts.sourceFile)', function () {
    projectConfig.processTs = true;
    const code: string = `
    import lazy lazy from "./test";
    export { lazy };
    `;
    ts.transpileModule(code, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: {
        before: [
          processKitImport('kitTest.ts', undefined, undefined, true, { autoLazyImport: false, reExportCheckMode: 'strict' })
        ]
      }
    });
    const errInfo = createErrInfo('lazy');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
    projectConfig.processTs = false;
  });

  mocha.it('2-3: the warn message of lazy-import re-export (ts.sourceFile)', function () {
    projectConfig.processTs = true;
    const code: string = `
    import lazy { y1 } from "./test";
    export { y1 };
    `;
    ts.transpileModule(code, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: {
        before: [
          processKitImport('kitTest.ts', undefined, undefined, true, { autoLazyImport: false, reExportCheckMode: 'compatible' })
        ]
      }
    });
    const errInfo = createErrInfo('y1');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
    projectConfig.processTs = false;
  });

  mocha.it('2-4: the error message of lazy-import re-export when the autoLazyImport is true (ts.sourceFile)', function () {
    projectConfig.processTs = true;
    const code: string = `
    import { a } from "./test";
    export { a };
    `;
    ts.transpileModule(code, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: {
        before: [
          processKitImport('kitTest.ts', undefined, undefined, true, { autoLazyImport: true, reExportCheckMode: 'compatible' })
        ]
      }
    });
    const errInfo = createErrInfo('a');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
    projectConfig.processTs = false;
  });

  mocha.it('2-5 test the error message of lazy-import re-export (js code)', function () {
    this.rollup.build(RELEASE);
    processJsCodeLazyImport('test.js', LAZY_IMPORT_RE_EXPORT_ERROR_JS, false, 'strict');
    const errorKeys = [
      'e1', 'e2', 'a3', 'd1', 'd2', 'a4', 'e5',
      'componentUtils', 'k1', 'uiObserver', 'lazy'
    ];
    errorKeys.forEach(key => {
      const errInfo = createErrInfo(key);
      const hasError = reExportCheckLog.errors.some(error =>
        error.message.includes(errInfo.toString())
      );
      expect(hasError).to.be.true;
    });
    resetReExportCheckLog();
  });

  mocha.it('2-6: test the error message of lazy-import re-export with the name lazy (js code)', function () {
    this.rollup.build(RELEASE);
    const code: string = `
    import lazy lazy from "./test";
    export { lazy };
    `;
    processJsCodeLazyImport('test.js', code, false, 'strict');
    const errInfo = createErrInfo('lazy');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
  });

  mocha.it('2-7: test the warn message of lazy-import re-export (js code)', function () {
    this.rollup.build(RELEASE);
    const code: string = `
    import lazy { y1 } from "./test";
    export { y1 };
    `;
    processJsCodeLazyImport('test.js', code, false, 'compatible');
    const errInfo = createErrInfo('y1');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
  });

  mocha.it('2-8: test the error message of lazy-import re-export when the autoLazyImport is true (js code)', function () {
    this.rollup.build(RELEASE);
    const code: string = `
    import { a } from "./test";
    export { a };
    `;
    processJsCodeLazyImport('test.js', code, true, 'strict');
    const errInfo = createErrInfo('a');
    const hasError = reExportCheckLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
    resetReExportCheckLog();
  });

  mocha.it('2-9: test processJsCodeLazyImport(autoLazyImport is false and reExportCheckMode is strict): no re-export', function () {
    const code: string = 'import lazy { y1 } from "./test";';
    const expectCode: string = 'import lazy { y1 } from "./test";';
    const result: string = processJsCodeLazyImport('index.js', code, false, 'strict');
    expect(result === expectCode).to.be.true;
  });

  mocha.it('2-10: test processJsCodeLazyImport(autoLazyImport is true and reExportCheckMode is strict): no re-export', function () {
    const code: string = 'import { y1 } from "./test";';
    const expectCode: string = 'import lazy { y1 } from "./test";\n';
    const result: string = processJsCodeLazyImport('index.js', code, true, 'strict');
    expect(result === expectCode).to.be.true;
  });
});