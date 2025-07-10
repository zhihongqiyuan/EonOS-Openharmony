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
import mocha from 'mocha';
import fs from 'fs';
import path from 'path';
import sinon from 'sinon';
import os from "os";

import {
  OBFUSCATION_TOOL,
  ESMODULE,
  RELEASE,
  TS2ABC
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import {
  NODE,
  BUNDLE_NAME_DEFAULT,
  ENTRY_MODULE_NAME_DEFAULT,
  NODE_JS_PATH,
  LOADER_AOTMODE
} from '../mock/rollup_mock/common';
import {
  ES2ABC_PATH,
  TS2ABC_PATH,
  MERGERABC_PATH,
  JS2ABC_PATH,
  AOTCOMPILER_PATH,
  WIN_ES2ABC_PATH,
  WIN_TS2ABC_PATH,
  WIN_MERGERABC_PATH,
  WIN_JS2ABC_PATH,
  WIN_AOTCOMPILER_PATH,
  MAC_ES2ABC_PATH,
  MAC_TS2ABC_PATH,
  MAC_MERGERABC_PATH,
  MAC_JS2ABC_PATH,
  MAC_AOTCOMPILER_PATH,
  ARKROOT_PATH,
  ARKCONFIG_TS2ABC_PATH,
  WIN_BC_OBFUSCATOR_PATH,
  BC_OBFUSCATOR_PATH,
  MAC_BC_OBFUSCATOR_PATH
} from '../mock/rollup_mock/path_config';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import {
  initArkConfig,
  initArkProjectConfig,
  utProcessArkConfig,
  readProjectAndLibsSource
} from '../../../lib/fast_build/ark_compiler/common/process_ark_config';
import {
  ObConfigResolver,
  MergedConfig
} from '../../../lib/fast_build/ark_compiler/common/ob_config_resolver';
import {
  ArkObfuscator,
  PerfMode,
  TimeAndMemTimeTracker,
  performanceTimeAndMemPrinter,
  printerTimeAndMemDataConfig,
} from 'arkguard';
import { UnobfuscationCollections, AtKeepCollections } from 'arkguard/lib/utils/CommonCollections';

const WINDOWS: string = 'Windows_NT';
const LINUX: string = 'Linux';
const MAC: string = 'Darwin';
const HARMONYOS: string = 'HarmonyOS';

mocha.describe('test process_ark_config file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test initArkConfig under build debug', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.arkFrontendDir = this.rollup.share.projectConfig.projectTopDir;
    this.rollup.share.projectConfig.nodeJs = true;
    this.rollup.share.projectConfig.nodePath = NODE_JS_PATH;
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);

    expect(arkConfig.nodePath === NODE_JS_PATH).to.be.true;
    expect(arkConfig.es2abcPath.indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.mergeAbcPath.indexOf(MERGERABC_PATH) > 0).to.be.true;
    expect(arkConfig.js2abcPath.indexOf(JS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.aotCompilerPath.indexOf(AOTCOMPILER_PATH) > 0).to.be.true;
    expect(arkConfig.isDebug === true).to.be.true;
    expect(arkConfig.arkRootPath === this.rollup.share.projectConfig.arkFrontendDir).to.be.true;
  });

  mocha.it('1-2: test initArkConfig under build release', function () {
    this.rollup.build(RELEASE);
    this.rollup.share.projectConfig.arkFrontendDir = this.rollup.share.projectConfig.projectTopDir;
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);

    expect(arkConfig.nodePath === NODE).to.be.true;
    expect(arkConfig.es2abcPath.indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.mergeAbcPath.indexOf(MERGERABC_PATH) > 0).to.be.true;
    expect(arkConfig.js2abcPath.indexOf(JS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.aotCompilerPath.indexOf(AOTCOMPILER_PATH) > 0).to.be.true;
    expect(arkConfig.isDebug === false).to.be.true;
    expect(arkConfig.arkRootPath === this.rollup.share.projectConfig.arkFrontendDir).to.be.true;
  });

  mocha.it('1-3: test initArkConfig under preview debug', function () {
    this.rollup.preview();
    this.rollup.share.projectConfig.arkFrontendDir = this.rollup.share.projectConfig.projectTopDir;
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);

    expect(arkConfig.nodePath === NODE).to.be.true;
    expect(arkConfig.es2abcPath.indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.mergeAbcPath.indexOf(MERGERABC_PATH) > 0).to.be.true;
    expect(arkConfig.js2abcPath.indexOf(JS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.aotCompilerPath.indexOf(AOTCOMPILER_PATH) > 0).to.be.true;
    expect(arkConfig.isDebug === true).to.be.true;
    expect(arkConfig.arkRootPath === this.rollup.share.projectConfig.arkFrontendDir).to.be.true;
  });

  mocha.it('1-4: test initArkConfig under hot reload debug', function () {
    this.rollup.hotReload();
    this.rollup.share.projectConfig.arkFrontendDir = this.rollup.share.projectConfig.projectTopDir;
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);

    expect(arkConfig.nodePath === NODE).to.be.true;
    expect(arkConfig.es2abcPath.indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.mergeAbcPath.indexOf(MERGERABC_PATH) > 0).to.be.true;
    expect(arkConfig.js2abcPath.indexOf(JS2ABC_PATH) > 0).to.be.true;
    expect(arkConfig.aotCompilerPath.indexOf(AOTCOMPILER_PATH) > 0).to.be.true;
    expect(arkConfig.isDebug === true).to.be.true;
    expect(arkConfig.arkRootPath === this.rollup.share.projectConfig.arkFrontendDir).to.be.true;
  });

  mocha.it('2-1-1: test initArkProjectConfig under build debug: moduleJsonInfo exists', function () {
    this.rollup.build();
    const arkConfig = initArkProjectConfig(this.rollup.share);
    const buildJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceBuildJson).toString());
    const moduleJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceModuleJsonPath).toString());

    expect(arkConfig.nodeModulesPath === buildJsonInfo.nodeModulesPath).to.be.true;
    expect(arkConfig.minPlatformVersion === moduleJsonInfo.app.minAPIVersion).to.be.true;
    expect(arkConfig.processTs === false).to.be.true;
    expect(arkConfig.moduleName === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.bundleName === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.compileMode === ESMODULE).to.be.true;
  });

  mocha.it('2-1-2: test initArkProjectConfig under build debug: buildJsonInfo.patchConfig is true', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.aceBuildJson =
      `${this.rollup.share.projectConfig.aceModuleBuild}/${LOADER_AOTMODE}`;
    const arkConfig = initArkProjectConfig(this.rollup.share);
    const buildJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceBuildJson).toString());

    expect(arkConfig.oldMapFilePath === buildJsonInfo.patchConfig.oldMapFilePath).to.be.true;
    expect(arkConfig.pandaMode === TS2ABC).to.be.true;
    expect(arkConfig.processTs === true).to.be.true;
    expect(arkConfig.anBuildOutPut === buildJsonInfo.anBuildOutPut).to.be.true;
    expect(arkConfig.anBuildMode === buildJsonInfo.anBuildMode).to.be.true;
    expect(arkConfig.apPath === buildJsonInfo.apPath).to.be.true;
    expect(arkConfig.moduleName === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.bundleName === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.compileMode === ESMODULE).to.be.true;
  });

  mocha.it('2-2: test initArkProjectConfig under build release', function () {
    this.rollup.build(RELEASE);
    const arkConfig = initArkProjectConfig(this.rollup.share);
    const buildJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceBuildJson).toString());
    const moduleJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceModuleJsonPath).toString());

    expect(arkConfig.nodeModulesPath === buildJsonInfo.nodeModulesPath).to.be.true;
    expect(arkConfig.minPlatformVersion === moduleJsonInfo.app.minAPIVersion).to.be.true;
    expect(arkConfig.processTs === false).to.be.true;
    expect(arkConfig.moduleName === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.bundleName === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.compileMode === ESMODULE).to.be.true;
  });

  mocha.it('2-3: test initArkProjectConfig under preview debug', function () {
    this.rollup.preview();
    const arkConfig = initArkProjectConfig(this.rollup.share);
    const buildJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceBuildJson).toString());
    const moduleJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceModuleJsonPath).toString());

    expect(arkConfig.nodeModulesPath === buildJsonInfo.nodeModulesPath).to.be.true;
    expect(arkConfig.minPlatformVersion === moduleJsonInfo.app.minAPIVersion).to.be.true;
    expect(arkConfig.processTs === false).to.be.true;
    expect(arkConfig.moduleName === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.bundleName === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.compileMode === ESMODULE).to.be.true;
  });

  mocha.it('2-4: test initArkProjectConfig under hot reload debug', function () {
    this.rollup.hotReload();
    const arkConfig = initArkProjectConfig(this.rollup.share);
    const buildJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceBuildJson).toString());
    const moduleJsonInfo =
      JSON.parse(fs.readFileSync(this.rollup.share.projectConfig.aceModuleJsonPath).toString());

    expect(arkConfig.nodeModulesPath === buildJsonInfo.nodeModulesPath).to.be.true;
    expect(arkConfig.minPlatformVersion === moduleJsonInfo.app.minAPIVersion).to.be.true;
    expect(arkConfig.processTs === false).to.be.true;
    expect(arkConfig.moduleName === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.bundleName === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(arkConfig.compileMode === ESMODULE).to.be.true;
  });

  mocha.it('2-5: test initArkProjectConfig under build debug: perf is not ADVANCED', function () {
    performanceTimeAndMemPrinter.singleFilePrinter = new TimeAndMemTimeTracker();
    performanceTimeAndMemPrinter.filesPrinter = new TimeAndMemTimeTracker();
    this.rollup.build(RELEASE);
    this.rollup.share.projectConfig.perf = PerfMode.NORMAL;
    initArkProjectConfig(this.rollup.share);

    expect(performanceTimeAndMemPrinter.filesPrinter).to.be.undefined;
    expect(performanceTimeAndMemPrinter.singleFilePrinter).to.be.undefined;
  });

  mocha.it('2-6: test initArkProjectConfig under build debug: perf is ADVANCED', function () {
    printerTimeAndMemDataConfig.mTimeAndMemPrinter = false;
    performanceTimeAndMemPrinter.singleFilePrinter = undefined;
    performanceTimeAndMemPrinter.filesPrinter = undefined;

    this.rollup.build(RELEASE);
    this.rollup.share.projectConfig.perf = PerfMode.ADVANCED;
    initArkProjectConfig(this.rollup.share);

    expect(printerTimeAndMemDataConfig.mTimeAndMemPrinter).to.be.true;
    expect(performanceTimeAndMemPrinter.filesPrinter).to.not.be.undefined;
    expect(performanceTimeAndMemPrinter.singleFilePrinter).to.not.be.undefined;
  });

  mocha.it('3-1: test initTerserConfig under build debug', function () {
    this.rollup.build();
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const obConfig: ObConfigResolver = new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
    const isHarCompiled: boolean = this.rollup.share.projectConfig.compileHar;
    const minifyOptions =
      utProcessArkConfig.initTerserConfig(this.rollup.share.projectConfig, logger, mergedObConfig, isHarCompiled);

    expect(minifyOptions.format.beautify === true).to.be.true;
    expect(minifyOptions.format.indent_level === 2).to.be.true;
    expect(minifyOptions.compress.join_vars === false).to.be.true;
    expect(minifyOptions.compress.sequences === 0).to.be.true;
    expect(minifyOptions.compress.directives === false).to.be.true;
    expect(minifyOptions.compress.drop_console === false).to.be.true;
    expect(minifyOptions.mangle.toplevel === false).to.be.true;
  });

  mocha.it('3-2: test initTerserConfig under build release', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const obConfig: ObConfigResolver = new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
    const isHarCompiled: boolean = this.rollup.share.projectConfig.compileHar;
    const minifyOptions =
      utProcessArkConfig.initTerserConfig(this.rollup.share.projectConfig, logger, mergedObConfig, isHarCompiled);

    expect(minifyOptions.format.beautify === true).to.be.true;
    expect(minifyOptions.format.indent_level === 2).to.be.true;
    expect(minifyOptions.compress.join_vars === false).to.be.true;
    expect(minifyOptions.compress.sequences === 0).to.be.true;
    expect(minifyOptions.compress.directives === false).to.be.true;
    expect(minifyOptions.compress.drop_console === false).to.be.true;
    expect(minifyOptions.mangle.toplevel === false).to.be.true;
  });

  mocha.it('3-3: test initTerserConfig under preview debug', function () {
    this.rollup.preview();
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const obConfig: ObConfigResolver = new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
    const isHarCompiled: boolean = this.rollup.share.projectConfig.compileHar;
    const minifyOptions =
      utProcessArkConfig.initTerserConfig(this.rollup.share.projectConfig, logger, mergedObConfig, isHarCompiled);

    expect(minifyOptions.format.beautify === true).to.be.true;
    expect(minifyOptions.format.indent_level === 2).to.be.true;
    expect(minifyOptions.compress.join_vars === false).to.be.true;
    expect(minifyOptions.compress.sequences === 0).to.be.true;
    expect(minifyOptions.compress.directives === false).to.be.true;
    expect(minifyOptions.compress.drop_console === false).to.be.true;
    expect(minifyOptions.mangle.toplevel === false).to.be.true;
  });

  mocha.it('3-4: test initTerserConfig under hot reload debug', function () {
    this.rollup.hotReload();
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const obConfig: ObConfigResolver = new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
    const isHarCompiled: boolean = this.rollup.share.projectConfig.compileHar;
    const minifyOptions =
      utProcessArkConfig.initTerserConfig(this.rollup.share.projectConfig, logger, mergedObConfig, isHarCompiled);

    expect(minifyOptions.format.beautify === true).to.be.true;
    expect(minifyOptions.format.indent_level === 2).to.be.true;
    expect(minifyOptions.compress.join_vars === false).to.be.true;
    expect(minifyOptions.compress.sequences === 0).to.be.true;
    expect(minifyOptions.compress.directives === false).to.be.true;
    expect(minifyOptions.compress.drop_console === false).to.be.true;
    expect(minifyOptions.mangle.toplevel === false).to.be.true;
  });

  mocha.it('4-1: test processCompatibleVersion under build debug', function () {
    this.rollup.build();
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === undefined).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;

    this.rollup.share.projectConfig.minPlatformVersion = 8;
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === TS2ABC).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(ARKCONFIG_TS2ABC_PATH) > 0).to.be.true;
  });

  mocha.it('4-2: test processCompatibleVersion under build release', function () {
    this.rollup.build(RELEASE);
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === undefined).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;

    this.rollup.share.projectConfig.minPlatformVersion = 8;
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === TS2ABC).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(ARKCONFIG_TS2ABC_PATH) > 0).to.be.true;
  });

  mocha.it('4-3: test processCompatibleVersion under preview debug', function () {
    this.rollup.preview();
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === undefined).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;

    this.rollup.share.projectConfig.minPlatformVersion = 8;
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === TS2ABC).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(ARKCONFIG_TS2ABC_PATH) > 0).to.be.true;
  });

  mocha.it('4-4: test processCompatibleVersion under hot reload debug', function () {
    this.rollup.hotReload();
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === undefined).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(TS2ABC_PATH) > 0).to.be.true;

    this.rollup.share.projectConfig.minPlatformVersion = 8;
    utProcessArkConfig.processCompatibleVersion(this.rollup.share.projectConfig, arkConfig);
    expect(this.rollup.share.projectConfig.pandaMode === TS2ABC).to.be.true;
    expect(arkConfig.ts2abcPath.indexOf(ARKCONFIG_TS2ABC_PATH) > 0).to.be.true;
  });

  mocha.describe('5: test readProjectAndLibsSource api', function () {
    let mergedObConfig: MergedConfig = {};
    let keepFilesAndDependencies: Set<string> = new Set();
    const targetFile: string = path.join(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation/third_package/oh_modules/Index.ts');
    const allFiles: Set<string> = new Set([targetFile]);
    let arkguardConfig: Object = {};
    const languageWhiteListNum = 7900;
    mocha.before(function () {
      this.rollup = new RollUpPluginMock();
      mergedObConfig = {
        options: {
          disableObfuscation: false,
          enablePropertyObfuscation: false,
          enableStringPropertyObfuscation: false,
          enableToplevelObfuscation: false,
          enableFileNameObfuscation: false,
          enableExportObfuscation: true,
          removeComments: false,
          compact:false,
          removeLog:  false,
          printNameCache: '',
          applyNameCache: ''
        }
      };
      arkguardConfig = {
        mNameObfuscation: {
          mEnable: true,
          mNameGeneratorType: 1,
          mRenameProperties: false,
          mReservedProperties: [],
          mTopLevel: false,
          mReservedToplevelNames:[],
        },
        mExportObfuscation: true,
        mPerformancePrinter: []
      };
    });

    mocha.it('5-1: test readProjectAndLibsSource with export obfuscation', function () {
      let arkObfuscator: ArkObfuscator = new ArkObfuscator();
      arkObfuscator.init(arkguardConfig);
      readProjectAndLibsSource(allFiles, mergedObConfig, arkObfuscator, false, keepFilesAndDependencies);

      expect(UnobfuscationCollections.reservedExportName.has('foo1')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo2')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('ts')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo5')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo6')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.size === 5).to.be.true;
      expect(UnobfuscationCollections.reservedLangForProperty.size > languageWhiteListNum).to.be.true;
      UnobfuscationCollections.clear();
    });

    mocha.it('5-2: test readProjectAndLibsSource with export & toplevel obfuscation', function () {
      let arkObfuscator: ArkObfuscator = new ArkObfuscator();
      arkguardConfig.mNameObfuscation.mTopLevel = true;
      arkObfuscator.init(arkguardConfig);
      mergedObConfig.options.enableToplevelObfuscation = true;
      readProjectAndLibsSource(allFiles, mergedObConfig, arkObfuscator, false, keepFilesAndDependencies);

      expect(UnobfuscationCollections.reservedExportName.has('foo1')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo2')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('ts')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo5')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo6')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.size === 5).to.be.true;
      expect(UnobfuscationCollections.reservedLangForProperty.size > languageWhiteListNum).to.be.true;
      UnobfuscationCollections.clear();
    });

    mocha.it('5-3: test readProjectAndLibsSource with property obfuscation', function () {
      arkguardConfig.mNameObfuscation.mRenameProperties = true;
      arkguardConfig.mExportObfuscation = false;
      arkguardConfig.mNameObfuscation.mTopLevel = false;
      let arkObfuscator: ArkObfuscator = new ArkObfuscator();
      arkObfuscator.init(arkguardConfig);
      mergedObConfig.options.enablePropertyObfuscation = true;
      mergedObConfig.options.enableExportObfuscation = false;
      mergedObConfig.options.enableToplevelObfuscation = false;
      readProjectAndLibsSource(allFiles, mergedObConfig, arkObfuscator, false, keepFilesAndDependencies);

      expect(UnobfuscationCollections.reservedExportName.size === 0).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo1')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo2')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('ts')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo5')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo6')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('prop21')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.size === 6).to.be.true;
      UnobfuscationCollections.clear();
    });

    mocha.it('5-4: test readProjectAndLibsSource with export & property & toplevel obfuscation', function () {
      arkguardConfig.mNameObfuscation.mRenameProperties = true;
      arkguardConfig.mExportObfuscation = true;
      arkguardConfig.mNameObfuscation.mTopLevel = true;
      let arkObfuscator: ArkObfuscator = new ArkObfuscator();
      arkObfuscator.init(arkguardConfig);
      mergedObConfig.options.enablePropertyObfuscation = true;
      mergedObConfig.options.enableToplevelObfuscation = true;
      mergedObConfig.options.enableExportObfuscation = true;
      readProjectAndLibsSource(allFiles, mergedObConfig, arkObfuscator, false, keepFilesAndDependencies);

      expect(UnobfuscationCollections.reservedExportName.has('foo1')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo2')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('ts')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo5')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.has('foo6')).to.be.true;
      expect(UnobfuscationCollections.reservedExportName.size === 5).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo1')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo2')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('ts')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo5')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('foo6')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.has('prop21')).to.be.true;
      expect(UnobfuscationCollections.reservedExportNameAndProp.size === 6).to.be.true;
      UnobfuscationCollections.clear();
    });

    mocha.it('5-5: test readProjectAndLibsSource with enableAtKeep', function () {
      this.rollup.build(RELEASE);
      arkguardConfig.mNameObfuscation.mEnableAtKeep = true;
      const cachePath: string = path.join(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation/');
      const rulesPath: string = path.join(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation/third_package/oh_modules/obfuscation-rules.txt');
      const consumerRulesPath: string = path.join(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation/third_package/oh_modules/consumer-rules.txt');
      const atKeepExportedPath: string = path.join(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation/third_package/oh_modules/exported-rules.txt');
      this.rollup.share.projectConfig.obfuscationOptions = {
        'selfConfig': {
          'ruleOptions': {
            'enable': true,
            'rules': [rulesPath]
          },
          'consumerRules': [consumerRulesPath],
        },
        'dependencies': {
          'libraries': [],
          'hars': []
        },
        obfuscationCacheDir: './test/testData/cache',
        exportRulePath: atKeepExportedPath,
        sdkApis: []
      };
      this.rollup.share.projectConfig.compileHar = true;
      const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
      const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
      obConfigResolver.resolveObfuscationConfigs();
      let arkObfuscator: ArkObfuscator = new ArkObfuscator();
      arkObfuscator.init(arkguardConfig, cachePath);
      arkObfuscator.obfConfigResolver = obConfigResolver;
      mergedObConfig.options.enableAtKeep = true;
      readProjectAndLibsSource(allFiles, mergedObConfig, arkObfuscator, false, keepFilesAndDependencies);
      expect(AtKeepCollections.keepSymbol.globalNames.has('foo2')).to.be.true;
      expect(AtKeepCollections.keepSymbol.propertyNames.has('foo2')).to.be.true;
      expect(AtKeepCollections.keepSymbol.propertyNames.has('prop21')).to.be.true;
      expect(AtKeepCollections.keepAsConsumer.globalNames.has('foo3')).to.be.true;
      expect(AtKeepCollections.keepAsConsumer.propertyNames.has('foo3')).to.be.false;
      const exportedContent = fs.readFileSync(atKeepExportedPath, 'utf-8');
      expect(exportedContent).to.include('-enable-property-obfuscation');
      expect(exportedContent).to.include('-keep-global-name');
      expect(exportedContent).to.include('foo3');
      expect(exportedContent).to.include('globalName00');
      expect(exportedContent).to.include('-keep-property-name');
      expect(exportedContent).to.include('propertyName00');
      expect(exportedContent).not.to.include('foo2');
      expect(exportedContent).not.to.include('prop21');
    });
  });

  mocha.it('6-1: test processPlatformInfo on windows', function () {
    this.rollup.build();
    const stub = sinon.stub(os, 'type').returns(WINDOWS);
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    const expectEs2abcPath = path.join(ARKROOT_PATH, WIN_ES2ABC_PATH);
    const expectTs2abcPath = path.join(ARKROOT_PATH, WIN_TS2ABC_PATH);
    const expectMergeAbcPath = path.join(ARKROOT_PATH, WIN_MERGERABC_PATH);
    const expectJs2abcPath = path.join(ARKROOT_PATH, WIN_JS2ABC_PATH);
    const expectAotCompilerPath = path.join(ARKROOT_PATH, WIN_AOTCOMPILER_PATH);
    const expectBcObfuscatorPath = path.join(ARKROOT_PATH, WIN_BC_OBFUSCATOR_PATH);
    expect(arkConfig.es2abcPath === expectEs2abcPath).to.be.true;
    expect(arkConfig.ts2abcPath === expectTs2abcPath).to.be.true;
    expect(arkConfig.mergeAbcPath === expectMergeAbcPath).to.be.true;
    expect(arkConfig.js2abcPath === expectJs2abcPath).to.be.true;
    expect(arkConfig.aotCompilerPath === expectAotCompilerPath).to.be.true;
    expect(arkConfig.bcObfuscatorPath === expectBcObfuscatorPath).to.be.true;
    stub.restore();
  });

  mocha.it('6-2: test processPlatformInfo on linux', function () {
    this.rollup.build();
    const stub = sinon.stub(os, 'type').returns(LINUX);
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    const expectEs2abcPath = path.join(ARKROOT_PATH, ES2ABC_PATH);
    const expectTs2abcPath = path.join(ARKROOT_PATH, TS2ABC_PATH);
    const expectMergeAbcPath = path.join(ARKROOT_PATH, MERGERABC_PATH);
    const expectJs2abcPath = path.join(ARKROOT_PATH, JS2ABC_PATH);
    const expectAotCompilerPath = path.join(ARKROOT_PATH, AOTCOMPILER_PATH);
    const expectBcObfuscatorPath = path.join(ARKROOT_PATH, BC_OBFUSCATOR_PATH);
    expect(arkConfig.es2abcPath === expectEs2abcPath).to.be.true;
    expect(arkConfig.ts2abcPath === expectTs2abcPath).to.be.true;
    expect(arkConfig.mergeAbcPath === expectMergeAbcPath).to.be.true;
    expect(arkConfig.js2abcPath === expectJs2abcPath).to.be.true;
    expect(arkConfig.aotCompilerPath === expectAotCompilerPath).to.be.true;
    expect(arkConfig.bcObfuscatorPath === expectBcObfuscatorPath).to.be.true;
    stub.restore();
  });

  mocha.it('6-3: test processPlatformInfo on mac', function () {
    this.rollup.build();
    const stub = sinon.stub(os, 'type').returns(MAC);
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    const expectEs2abcPath = path.join(ARKROOT_PATH, MAC_ES2ABC_PATH);
    const expectTs2abcPath = path.join(ARKROOT_PATH, MAC_TS2ABC_PATH);
    const expectMergeAbcPath = path.join(ARKROOT_PATH, MAC_MERGERABC_PATH);
    const expectJs2abcPath = path.join(ARKROOT_PATH, MAC_JS2ABC_PATH);
    const expectAotCompilerPath = path.join(ARKROOT_PATH, MAC_AOTCOMPILER_PATH);
    const expectBcObfuscatorPath = path.join(ARKROOT_PATH, MAC_BC_OBFUSCATOR_PATH);
    expect(arkConfig.es2abcPath === expectEs2abcPath).to.be.true;
    expect(arkConfig.ts2abcPath === expectTs2abcPath).to.be.true;
    expect(arkConfig.mergeAbcPath === expectMergeAbcPath).to.be.true;
    expect(arkConfig.js2abcPath === expectJs2abcPath).to.be.true;
    expect(arkConfig.aotCompilerPath === expectAotCompilerPath).to.be.true;
    expect(arkConfig.bcObfuscatorPath === expectBcObfuscatorPath).to.be.true;
    stub.restore();
  });

  mocha.it('6-4: test processPlatformInfo on harmonyos', function () {
    this.rollup.build();
    const stub = sinon.stub(os, 'type').returns(HARMONYOS);
    const arkConfig = initArkConfig(this.rollup.share.projectConfig);
    const expectEs2abcPath = path.join(ARKROOT_PATH, ES2ABC_PATH);
    expect(arkConfig.es2abcPath === expectEs2abcPath).to.be.true;
    stub.restore();
  });
});