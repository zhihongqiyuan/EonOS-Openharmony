/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import { expect } from 'chai';
import fs from 'fs';
import mocha from 'mocha';
import path from 'path';
import * as ts from 'typescript';

import {
  processKitImport,
  kitTransformLog,
  KitInfo
} from '../../../lib/process_kit_import';
import { findImportSpecifier } from '../utils/utils';
import { 
  ArkTSErrorDescription,
  ErrorCode
} from '../../../lib/fast_build/ark_compiler/error_code';
import {
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';
import { etsLoaderErrorReferences } from '../../../lib/fast_build/ark_compiler/url_config.json';

const KIT_IMPORT_CODE: string =
`
import { Ability, featureAbility, ErrorCode } from "@kit.AbilityKit";
let localAbility = new Ability();
let localFeature = new featureAbility();
let noError = ErrorCode.NO_ERROR;
`

const KIT_IMPORT_CODE_EXPECT: string =
'import Ability from "@ohos.app.ability.Ability";\n' +
'import featureAbility from "@ohos.ability.featureAbility";\n'+
'import { ErrorCode } from "@ohos.ability.errorCode";\n'+
'let localAbility = new Ability();\n'+
'let localFeature = new featureAbility();\n'+
'let noError = ErrorCode.NO_ERROR;\n'+
'//# sourceMappingURL=kitTest.js.map'

const KIT_EXPORT_CODE: string =
`
export { Ability, featureAbility, ErrorCode } from "@kit.AbilityKit";
`

const KIT_EXPORT_CODE_EXPECT: string =
'export { default as Ability } from "@ohos.app.ability.Ability";\n' +
'export { default as featureAbility } from "@ohos.ability.featureAbility";\n'+
'export { ErrorCode } from "@ohos.ability.errorCode";\n'+
'//# sourceMappingURL=kitTest.js.map'

const KIT_STAR_EXPORT_CODE: string =
`
export * from "@kit.AudioKit";
`

const KIT_STAR_EXPORT_CODE_EXPECT: string =
'export * from "@ohos.multimedia.audio";\n' +
'export * from "@ohos.multimedia.audioHaptic";\n'+
'export * from "@ohos.multimedia.systemSoundManager";\n'+
'export * from "@ohos.multimedia.avVolumePanel";\n' +
'//# sourceMappingURL=kitTest.js.map'

const KIT_IMPORT_ERROR_CODE: string =
'import { Ability } from "@kit.Kit";'

const KIT_UNUSED_TYPE_IMPROT_CODE: string = 
'import { BusinessError } from "@kit.BasicServicesKit";'

const KIT_UNUSED_TYPE_IMPROT_CODE_EXPECT: string =
'export {};\n' +
'//# sourceMappingURL=kitTest.js.map'

const KIT_USED_TYPE_IMPROT_CODE: string = 
'import { BusinessError } from "@kit.BasicServicesKit";\n' +
'let e: BusinessError = undefined';

const KIT_USED_TYPE_IMPROT_CODE_EXPECT: string =
'let e = undefined;\n' +
'export {};\n' +
'//# sourceMappingURL=kitTest.js.map'

const KIT_UNUSED_VALUE_IMPORT_CODE: string =
'import { appAccount } from "@kit.BasicServicesKit";'

const KIT_UNUSED_VALUE_IMPROT_CODE_EXPECT: string =
'export {};\n' +
'//# sourceMappingURL=kitTest.js.map'

const KIT_USED_VALUE_IMPORT_CODE: string =
'import { appAccount } from "@kit.BasicServicesKit";\n' +
'appAccount.createAppAccountManager();';

const KIT_USED_VALUE_IMPROT_CODE_EXPECT: string =
'import appAccount from "@ohos.account.appAccount";\n' +
'appAccount.createAppAccountManager();\n' +
'//# sourceMappingURL=kitTest.js.map'

const KIT_EMPTY_IMPORT_CODE: string =
'import { appAccount } from "@kit.BasicServicesKit";\n' +
'import "@kit.ArkUI";\n' +
'appAccount.createAppAccountManager();';

const KIT_NAMESPACE_IMPORT_CODE: string =
'import * as ArkUI "@kit.ArkUI";\n' +
'ArkUI.AlertDialog;';

const KIT_LAZY_IMPORT_CODE: string =
'import { test } from "./test";\n' +
'import lazy { appAccount } from "@kit.BasicServicesKit";\n' +
'import lazy { lang } from "@kit.ArkTS";\n' +
'import lazy { socket, VpnExtensionContext } from "@kit.NetworkKit";\n' +
'import lazy { UIAbility as x1 } from "@kit.AbilityKit";\n' +
'import lazy buffer from "@kit.ArkTest";\n' +
'test;\n' +
'appAccount.createAppAccountManager();\n' +
'type ISendable = lang.ISendable;\n' +
'socket.sppCloseServerSocket(1);\n' +
'new VpnExtensionContext();\n' +
'export { x1 };\n' +
'const buf = new buffer();';

const KIT_LAZY_IMPORT_CODE_EXPECT: string =
'import { test } from "./test";\n' +
'import lazy appAccount from "@ohos.account.appAccount";\n' +
'import lazy socket from "@ohos.net.socket";\n' +
'import lazy { VpnExtensionContext } from "@ohos.app.ability.VpnExtensionAbility";\n' +
'import lazy x1 from "@ohos.app.ability.UIAbility";\n' +
'import lazy buffer from "@ohos.buffer";\n' +
'test;\n' +
'appAccount.createAppAccountManager();\n' +
'socket.sppCloseServerSocket(1);\n' +
'new VpnExtensionContext();\n' +
'export { x1 };\n' +
'const buf = new buffer();\n' +
'//# sourceMappingURL=kitTest.js.map';

const KIT_LAZY_IMPORT_ERROR_CODE: string =
'import lazy Animator from "@kit.ArkUI";\n' +
'new Animator();'

const SINGLE_DEFAULT_BINDINGS_IMPORT_CODE: string =
'import buffer from "@kit.ArkTest";\n' +
'const buf = new buffer()';

const SINGLE_DEFAULT_BINDINGS_IMPORT_CODE_EXPECT: string =
'import buffer from "@ohos.buffer";\n' +
'const buf = new buffer();\n' +
'//# sourceMappingURL=kitTest.js.map'

const DEFAULT_BINDINGS_IMPORT_AFTER_NORMAL_KIT_CODE: string =
'import { Ability } from "@kit.AbilityKit";\n' +
'import buffer from "@kit.ArkTest";\n' +
'let localAbility = new Ability();\n' +
'const buf = new buffer()';

const DEFAULT_BINDINGS_IMPORT_AFTER_NORMAL_KIT_CODE_EXPECT: string =
'import Ability from "@ohos.app.ability.Ability";\n' +
'import buffer from "@ohos.buffer";\n' +
'let localAbility = new Ability();\n' +
'const buf = new buffer();\n' +
'//# sourceMappingURL=kitTest.js.map'

const DEFAULT_BINDINGS_IMPORT_BEFORE_NORMAL_KIT_CODE: string =
'import buffer from "@kit.ArkTest";\n' +
'import { Ability } from "@kit.AbilityKit";\n' +
'let localAbility = new Ability();\n' +
'const buf = new buffer()';

const DEFAULT_BINDINGS_IMPORT_AFTER_BEFORE_KIT_CODE_EXPECT: string =
'import buffer from "@ohos.buffer";\n' +
'import Ability from "@ohos.app.ability.Ability";\n' +
'let localAbility = new Ability();\n' +
'const buf = new buffer();\n' +
'//# sourceMappingURL=kitTest.js.map'

const DEFAULT_BINDINGS_IMPORT_WITH_NORMAL_KIT_CODE: string =
'import { Ability } from "@kit.AbilityKit";\n' +
'import buffer, {convertxml, process} from "@kit.ArkTest";\n' +
'let localAbility = new Ability();\n' +
'const localconvertxml = new convertxml();\n' +
'const localprocess = new process();\n' +
'const buf = new buffer()'

const DEFAULT_BINDINGS_IMPORT_WITH_NORMAL_KIT_CODE_EXPECT: string =
'import Ability from "@ohos.app.ability.Ability";\n' +
'import convertxml from "@ohos.convertxml";\n' +
'import process from "@ohos.process";\n' +
'import buffer from "@ohos.buffer";\n' +
'let localAbility = new Ability();\n' +
'const localconvertxml = new convertxml();\n' +
'const localprocess = new process();\n' +
'const buf = new buffer();\n' +
'//# sourceMappingURL=kitTest.js.map'

const SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE: string =
'import NoExist from "@kit.ArkTest";\n'

const SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE_EXPECT: string =
'export {};\n' +
'//# sourceMappingURL=kitTest.js.map'

const ARK_TEST_KIT: Object = {
  symbols: {
    "default": {
      "source": "@ohos.buffer.d.ts",
      "bindings": "default"
    },
    "convertxml": {
      "source": "@ohos.convertxml.d.ts",
      "bindings": "default"
    },
    "process": {
      "source": "@ohos.process.d.ts",
      "bindings": "default"
    }
  }
}

const ARK_TEST_NO_DEFAULT_KIT: Object = {
  symbols: {
    "convertxml": {
      "source": "@ohos.convertxml.d.ts",
      "bindings": "default"
    },
    "process": {
      "source": "@ohos.process.d.ts",
      "bindings": "default"
    }
  }
}

const compilerOptions = ts.readConfigFile(
  path.resolve(__dirname, '../../../tsconfig.json'), ts.sys.readFile).config.compilerOptions;
compilerOptions['moduleResolution'] = 'nodenext';
compilerOptions['module'] = 'es2020';

// !! The Kit transform result would be changed once the kit config file has updated.
mocha.describe('process Kit Imports tests', function () {
  mocha.it('1-1: process specifier imports', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_IMPORT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-2: process specifier exports', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_EXPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_EXPORT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-3: process star export', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_STAR_EXPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_STAR_EXPORT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-4: process unused type import', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_UNUSED_TYPE_IMPROT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_UNUSED_TYPE_IMPROT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-5 process used type import', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_USED_TYPE_IMPROT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_USED_TYPE_IMPROT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-6 process unused value import', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_UNUSED_VALUE_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_UNUSED_VALUE_IMPROT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-7 process used value import', function () {
    const result: ts.TranspileOutput = ts.transpileModule(KIT_USED_VALUE_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_USED_VALUE_IMPROT_CODE_EXPECT).to.be.true;
  });

  mocha.it('1-8 process used lazy import', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(KIT_LAZY_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == KIT_LAZY_IMPORT_CODE_EXPECT).to.be.true;
  });

  mocha.it('2-1 the error message of processKitImport', function () {
    ts.transpileModule(KIT_IMPORT_ERROR_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_KIT_CONFIG_FILE_NOT_FOUND,
      ArkTSErrorDescription,
      "Kit '@kit.Kit' has no corresponding config file in ArkTS SDK.",
      '',
      [
        "Please make sure the Kit apis are consistent with SDK and there's no local modification on Kit apis.",
        `For more details on Kit apis, please refer to ${etsLoaderErrorReferences.harmonyOSReferencesAPI}.`
      ]
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
  });

  mocha.it('2-2 the error message of test specifiers in newSpecificerInfo', function () {
    const symbols = {
      'test': ''
    }
    const sourceCode = 'import { test } from "my-module";';
    const sourceFile = ts.createSourceFile(
        "tempFile.ts",
        sourceCode,
        ts.ScriptTarget.Latest,
        true
    );
    const kitNode = findImportSpecifier(sourceFile);
    const kitInfo = new KitInfo(kitNode, symbols);
    kitInfo.newSpecificerInfo('', 'test', undefined);
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_IMPORT_NAME_NOT_EXPORTED_FROM_KIT,
      ArkTSErrorDescription,
      "'test' is not exported from Kit '@kit.ArkTest'.",
      '',
      [
        "Please make sure the Kit apis are consistent with SDK and there's no local modification on Kit apis.",
        `For more details on Kit apis, please refer to ${etsLoaderErrorReferences.harmonyOSReferencesAPI}.`
      ]
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
  });

  mocha.it('2-3 the error message of EmptyImportKitInfo', function () {
    ts.transpileModule(KIT_EMPTY_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_EMPTY_IMPORT_NOT_ALLOWED_WITH_KIT,
      ArkTSErrorDescription,
      "Can not use empty import(side-effect import) statement with Kit '@kit.ArkUI'.",
      '',
      ['Please specify imported symbols explicitly. ' + 
       'For example, import "@kit.ArkUI"; -> import { lang } from "@kit.ArkUI";']
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
  });

  mocha.it('2-4 the error message of default specifiers in newSpecificerInfo', function () {
    ts.transpileModule(KIT_LAZY_IMPORT_ERROR_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_IMPORT_NAME_NOT_EXPORTED_FROM_KIT,
      ArkTSErrorDescription,
      "'default' is not exported from Kit '@kit.ArkUI'.",
      '',
      [
        "Please make sure the Kit apis are consistent with SDK and there's no local modification on Kit apis.",
        `For more details on Kit apis, please refer to ${etsLoaderErrorReferences.harmonyOSReferencesAPI}.`
      ]
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
  });

  mocha.it('2-5 the error message of NameSpaceKitInfo', function () {
    ts.transpileModule(KIT_NAMESPACE_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_KIT_NAMESPACE_IMPORT_EXPORT_NOT_SUPPORTED,
      ArkTSErrorDescription,
      'Namespace import or export of Kit is not supported currently.',
      '',
      ['Please namespace import or export of Kit replace it with named import or export instead. ' + 
       'For example, import * as ArkTS from "@kit.ArkUI"; -> import { AlertDialog } from "@kit.ArkUI";']
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())
    );
    expect(hasError).to.be.true;
  });

  mocha.it('2-6 the error message of validateImportingETSDeclarationSymbol', function () {
    const symbols = {
      "test": {
        "source": "@ohos.test.d.ets",
        "bindings": "default"
      }
    };
    const sourceCode = 'import { test } from "my-module";';
    const sourceFile = ts.createSourceFile(
      "tempFile.ts",
      sourceCode,
      ts.ScriptTarget.Latest,
      true
    );
    const kitNode = findImportSpecifier(sourceFile);
    const kitInfo = new KitInfo(kitNode, symbols);
    compilerOptions['tsImportSendableEnable'] = false;
    kitInfo.newSpecificerInfo('', 'test', findImportSpecifier(sourceFile));
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_IDENTIFIER_IMPORT_NOT_ALLOWED_IN_TS_FILE,
      ArkTSErrorDescription,
      "Identifier 'test' comes from '@ohos.test.d.ets' " + 
      'which can not be imported in .ts file.',
      '',
      ["Please remove the import statement or change the file extension to .ets."]
    );
    const hasError = kitTransformLog.errors.some(error =>
      error.message.includes(errInfo.toString())

    );
    expect(hasError).to.be.true;
  });

  mocha.it('3-1 process single default-bindings import', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(SINGLE_DEFAULT_BINDINGS_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == SINGLE_DEFAULT_BINDINGS_IMPORT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });

  mocha.it('3-2 process default-bindings import after normal kit', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(DEFAULT_BINDINGS_IMPORT_AFTER_NORMAL_KIT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == DEFAULT_BINDINGS_IMPORT_AFTER_NORMAL_KIT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });

  mocha.it('3-3 process default-bindings import before normal kit', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(DEFAULT_BINDINGS_IMPORT_BEFORE_NORMAL_KIT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == DEFAULT_BINDINGS_IMPORT_AFTER_BEFORE_KIT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });

  mocha.it('3-4 process default-bindings import with specifiers', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(DEFAULT_BINDINGS_IMPORT_WITH_NORMAL_KIT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == DEFAULT_BINDINGS_IMPORT_WITH_NORMAL_KIT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });

  mocha.it('3-5 process single default-bindings import, but no use', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });

  mocha.it('3-6 process single default-bindings import, but kit no default export', function () {
    const ARK_TEST_KIT_JSON = '@kit.ArkTest.json';
    const KIT_CONFIGS = 'kit_configs';

    const arkTestKitConfig: string = path.resolve(__dirname, `../../../${KIT_CONFIGS}/${ARK_TEST_KIT_JSON}`);
    fs.writeFileSync(arkTestKitConfig, JSON.stringify(ARK_TEST_NO_DEFAULT_KIT));

    const result: ts.TranspileOutput = ts.transpileModule(SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE, {
      compilerOptions: compilerOptions,
      fileName: "kitTest.ts",
      transformers: { before: [ processKitImport() ] }
    });
    expect(result.outputText == SINGLE_DEFAULT_BINDINGS_NO_DEFAULT_IMPORT_CODE_EXPECT).to.be.true;
    fs.unlinkSync(arkTestKitConfig);
  });
});
