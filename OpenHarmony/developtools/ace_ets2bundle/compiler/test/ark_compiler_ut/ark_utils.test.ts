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

// Execute this file first to avoid circular dependency problems

import { expect } from 'chai';
import mocha from 'mocha';
import fs, { unwatchFile } from "fs";
import path from "path";
import MagicString from 'magic-string';
import sinon from 'sinon';

import {
  getBuildModeInLowerCase,
  getPackageInfo,
  genSourceMapFileName,
  isOhModules,
  isEs2Abc,
  writeArkguardObfuscatedSourceCode,
  writeMinimizedSourceCode,
  tryMangleFileName,
  getPreviousStageSourceMap,
  collectObfuscationFileContent
} from '../../lib/ark_utils';
import {
  DEBUG,
  RELEASE,
  OH_MODULES,
  EXTNAME_TS,
  EXTNAME_JS,
  EXTNAME_ETS,
  OBFUSCATION_TOOL
} from '../../lib/fast_build/ark_compiler/common/ark_define';
import RollUpPluginMock from './mock/rollup_mock/rollup_plugin_mock';
import {
  BUNDLE_NAME_DEFAULT,
  ENTRY_MODULE_NAME_DEFAULT,
  EXTNAME_MAP,
  ENTRYABILITY_JS
} from './mock/rollup_mock/common';
import projectConfig from './utils/processProjectConfig';
import {
  ES2ABC,
  TS2ABC
} from '../../lib/pre_define';
import { changeFileExtension } from '../../lib/fast_build/ark_compiler/utils';
import ModuleSourceFileMock from './mock/class_mock/module_source_files_mock';
import {
  genTemporaryPath,
  toUnixPath
} from '../../lib/utils';
import {
  ObConfigResolver,
  MergedConfig,
  obfLogger
} from '../../lib/fast_build/ark_compiler/common/ob_config_resolver';
import {
  utProcessArkConfig
} from '../../lib/fast_build/ark_compiler/common/process_ark_config';
import { ModuleSourceFile } from '../../lib/fast_build/ark_compiler/module/module_source_file';
import { PROJECT_ROOT, TERSER_PROCESSED_EXPECTED_CODE } from './mock/rollup_mock/path_config';
import { GEN_ABC_PLUGIN_NAME } from '../../lib/fast_build/ark_compiler/common/ark_define';
import { SourceMapGenerator } from '../../lib/fast_build/ark_compiler/generate_sourcemap';
import { ArkObfuscator } from 'arkguard';
import {
  ArkTSInternalErrorDescription,
  ErrorCode
} from '../../lib/fast_build/ark_compiler/error_code';
import { 
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../lib/fast_build/ark_compiler/logger';
import { initObfLogger, printObfLogger } from '../../lib/fast_build/ark_compiler/common/ob_config_resolver';
import { getLogger } from 'log4js';
import { createErrInfo } from './utils/utils';

mocha.describe('test ark_utils file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test getBuildModeInLowerCase under build debug', function () {
    this.rollup.build();
    const buildMode = getBuildModeInLowerCase(this.rollup.share.projectConfig);
    expect(buildMode === DEBUG).to.be.true;
  });

  mocha.it('1-2: test getBuildModeInLowerCase under build release', function () {
    this.rollup.build(RELEASE);
    const buildMode = getBuildModeInLowerCase(this.rollup.share.projectConfig);
    expect(buildMode === RELEASE).to.be.true;
  });

  mocha.it('1-3: test getBuildModeInLowerCase under preview debug', function () {
    this.rollup.preview();
    const buildMode = getBuildModeInLowerCase(this.rollup.share.projectConfig);
    expect(buildMode === DEBUG).to.be.true;
  });

  mocha.it('1-4: test getBuildModeInLowerCase under hot reload debug', function () {
    this.rollup.hotReload();
    const buildMode = getBuildModeInLowerCase(this.rollup.share.projectConfig);
    expect(buildMode === DEBUG).to.be.true;
  });

  mocha.it('2-1: test getPackageInfo under build debug', function () {
    this.rollup.build();
    const returnInfo = getPackageInfo(this.rollup.share.projectConfig.aceModuleJsonPath);
    expect(returnInfo[0] === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(returnInfo[1] === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
  });

  mocha.it('2-2: test getPackageInfo under build release', function () {
    this.rollup.build(RELEASE);
    const returnInfo = getPackageInfo(this.rollup.share.projectConfig.aceModuleJsonPath);
    expect(returnInfo[0] === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(returnInfo[1] === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
  });

  mocha.it('2-3: test getPackageInfo under preview debug', function () {
    this.rollup.preview();
    const returnInfo = getPackageInfo(this.rollup.share.projectConfig.aceModuleJsonPath);
    expect(returnInfo[0] === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(returnInfo[1] === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
  });

  mocha.it('2-4: test getPackageInfo under hot reload debug', function () {
    this.rollup.hotReload();
    const returnInfo = getPackageInfo(this.rollup.share.projectConfig.aceModuleJsonPath);
    expect(returnInfo[0] === BUNDLE_NAME_DEFAULT).to.be.true;
    expect(returnInfo[1] === ENTRY_MODULE_NAME_DEFAULT).to.be.true;
  });

  mocha.it('3-1: test genSourceMapFileName under build debug', function () {
    this.rollup.build();
    for (const filePath of this.rollup.share.allFiles) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_JS)) {
        const originPath = genSourceMapFileName(filePath);
        const expectedPath = `${filePath}${EXTNAME_MAP}`;
        expect(originPath === expectedPath).to.be.true;
      } else if (filePath.endsWith(EXTNAME_ETS)) {
        const originPath = genSourceMapFileName(filePath);
        expect(originPath === filePath).to.be.true;
      }
    }
  });

  mocha.it('3-2: test genSourceMapFileName under build release', function () {
    this.rollup.build(RELEASE);
    for (const filePath of this.rollup.share.allFiles) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_JS)) {
        const originPath = genSourceMapFileName(filePath);
        const expectedPath = `${filePath}${EXTNAME_MAP}`;
        expect(originPath === expectedPath).to.be.true;
      }
    }
  });

  mocha.it('3-3: test genSourceMapFileName under preview debug', function () {
    this.rollup.preview();
    for (const filePath of this.rollup.share.allFiles) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_JS)) {
        const originPath = genSourceMapFileName(filePath);
        const expectedPath = `${filePath}${EXTNAME_MAP}`;
        expect(originPath === expectedPath).to.be.true;
      }
    }
  });

  mocha.it('3-4: test genSourceMapFileName under hot reload debug', function () {
    this.rollup.hotReload();
    for (const filePath of this.rollup.share.allFiles) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_JS)) {
        const originPath = genSourceMapFileName(filePath);
        const expectedPath = `${filePath}${EXTNAME_MAP}`;
        expect(originPath === expectedPath).to.be.true;
      }
    }
  });

  mocha.it('4-1: test isOhModules under build debug', function () {
    this.rollup.build();
    const returnInfo = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfo === false).to.be.true;
    this.rollup.share.projectConfig.packageDir = OH_MODULES;
    const returnInfoOh = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfoOh).to.be.true;
  });

  mocha.it('4-2: test isOhModules under build release', function () {
    this.rollup.build(RELEASE);
    const returnInfo = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfo === false).to.be.true;
    this.rollup.share.projectConfig.packageDir = OH_MODULES;
    const returnInfoOh = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfoOh).to.be.true;
  });

  mocha.it('4-3: test isOhModules under preview debug', function () {
    this.rollup.preview();
    const returnInfo = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfo === false).to.be.true;
    this.rollup.share.projectConfig.packageDir = OH_MODULES;
    const returnInfoOh = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfoOh).to.be.true;
  });

  mocha.it('4-4: test isOhModules under hot reload debug', function () {
    this.rollup.hotReload();
    const returnInfo = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfo === false).to.be.true;
    this.rollup.share.projectConfig.packageDir = OH_MODULES;
    const returnInfoOh = isOhModules(this.rollup.share.projectConfig);
    expect(returnInfoOh).to.be.true;
  });

  mocha.it('4-5: test isOhModules under hot fix debug', function () {
    projectConfig.buildMode = DEBUG;
    const returnInfo = isOhModules(projectConfig);
    expect(returnInfo).to.be.true;
  });

  mocha.it('4-6: test isOhModules under hot fix release', function () {
    projectConfig.buildMode = RELEASE;
    const returnInfo = isOhModules(projectConfig);
    expect(returnInfo).to.be.true;
  });

  mocha.it('5-1: test isEs2Abc under build debug', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfo).to.be.true;

    this.rollup.share.projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoTS2ABC === false).to.be.true;

    this.rollup.share.projectConfig.pandaMode = "undefined";
    const returnInfoUndef = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoUndef).to.be.true;

    this.rollup.share.projectConfig.pandaMode = undefined;
    const returnInfoUndefined = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoUndefined).to.be.true;
  });

  mocha.it('5-2: test isEs2Abc under build release', function () {
    this.rollup.build(RELEASE);
    this.rollup.share.projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfo).to.be.true;

    this.rollup.share.projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoTS2ABC).to.be.false;
  });

  mocha.it('5-3: test isEs2Abc under preview debug', function () {
    this.rollup.preview();
    this.rollup.share.projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfo).to.be.true;

    this.rollup.share.projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoTS2ABC).to.be.false;
  });

  mocha.it('5-4: test isEs2Abc under hot reload debug', function () {
    this.rollup.hotReload();
    this.rollup.share.projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfo).to.be.true;

    this.rollup.share.projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(this.rollup.share.projectConfig);
    expect(returnInfoTS2ABC).to.be.false;
  });

  mocha.it('5-5: test isEs2Abc under hot fix debug', function () {
    projectConfig.buildMode = DEBUG;
    projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(projectConfig);
    expect(returnInfo).to.be.true;

    projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(projectConfig);
    expect(returnInfoTS2ABC).to.be.false;
  });

  mocha.it('5-6: test isEs2Abc under hot fix release', function () {
    projectConfig.buildMode = RELEASE;
    projectConfig.pandaMode = ES2ABC;
    const returnInfo = isEs2Abc(projectConfig);
    expect(returnInfo).to.be.true;

    projectConfig.pandaMode = TS2ABC;
    const returnInfoTS2ABC = isEs2Abc(projectConfig);
    expect(returnInfoTS2ABC).to.be.false;
  });

  mocha.it('6-1: test the error message of writeArkguardObfuscatedSourceCode', async function () {
    this.rollup.build(RELEASE);
    SourceMapGenerator.initInstance(this.rollup);
    this.rollup.share.getHvigorConsoleLogger = undefined;
    initObfLogger(this.rollup.share);
    const stub = sinon.stub(obfLogger, 'error');
    const red: string = '\x1B[31m';
    try {
      await writeArkguardObfuscatedSourceCode(
        {content: undefined, buildFilePath: '', relativeSourceFilePath: '', originSourceFilePath: ''},
        printObfLogger, this.rollup.share.projectConfig, {});
    } catch (e) {
    }
    expect(stub.calledWith(red,
      `ArkTS:INTERNAL ERROR: Failed to obfuscate file '' with arkguard. TypeError: Cannot read properties of undefined (reading 'obfuscate')`
    )).to.be.true;
    stub.restore();
    SourceMapGenerator.cleanSourceMapObject();
  });
  
  mocha.it('6-2: test the error message of writeArkguardObfuscatedSourceCode' +
    'with getHvigorConsoleLogger', async function () {
    this.rollup.build(RELEASE);
    SourceMapGenerator.initInstance(this.rollup);
    class customShare {
      public getHvigorConsoleLogger(prefix: string) {
       const logger = hvigorLogger.getLogger(prefix);
       return logger;
     }
    }
    class hvigorLogger {
      mSubSystem: string = '';
      constructor(subSystem: string) {
        this.mSubSystem = subSystem;
      }
    
      public printError(errInfo: Object) {
      }

      public static getLogger(subSystem): hvigorLogger {
        return new hvigorLogger(subSystem);
      }
    }
    initObfLogger(new customShare());
    const stub = sinon.stub(obfLogger, 'printError');
    try {
      await writeArkguardObfuscatedSourceCode(
        {content: undefined, buildFilePath: '', relativeSourceFilePath: '', originSourceFilePath: ''},
        printObfLogger, this.rollup.share.projectConfig, {});
    } catch (e) {
    }
    const info = {
        code: '10810001',
        description: 'ArkTS compiler Error',
        cause: "ArkTS:INTERNAL ERROR: Failed to obfuscate file '' with arkguard. TypeError: Cannot read properties of undefined (reading 'obfuscate')",
        position: '',
        solutions: [ 'Please modify the code based on the error information.' ]
    };
    expect(stub.calledWith(info)).to.be.true;
    stub.restore();
    SourceMapGenerator.cleanSourceMapObject();
  });

  mocha.it('7-1: test the error message of writeMinimizedSourceCode', async function () {
    this.rollup.build(RELEASE);
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_SOURCE_CODE_OBFUSCATION_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to obfuscate source code for filePath'
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    try {
      await writeMinimizedSourceCode(undefined, 'filePath', logger);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true; 
    stub.restore();
  });

  mocha.it('7-2: test the error message of writeMinimizedSourceCode without getHvigorConsoleLogger', async function () {
    this.rollup.build(RELEASE);
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_SOURCE_CODE_OBFUSCATION_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to obfuscate source code for filePath'
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.logger, 'error');
    try {
      await writeMinimizedSourceCode(undefined, 'filePath', logger);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('8-1: test tryMangleFileName when obfuscation is disabled', async function () {
    const filePath = '/mnt/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/release/src/main/ets/entryability/EntryAbility.ts';
    const originalFilePath = '/mnt/application/entry/src/main/ets/entryability/EntryAbility.ets';
    const projectConfig = {
      projectRootPath: '/mnt/application',
      packageDir: 'oh_modules',
      modulePathMap: {
        entry: '/mnt/application/entry'
      },
      obfuscationMergedObConfig: {
        options: {
          enableFileNameObfuscation: false
        }
      }
    }
    const result = tryMangleFileName(filePath, projectConfig, originalFilePath);
    expect(result === filePath).to.be.true;
  });

  mocha.it('8-2: test tryMangleFileName when obfuscation is enabled', async function () {
    const filePath = '/mnt/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/release/src/main/ets/entryability/EntryAbility.ts';
    const originalFilePath = '/mnt/application/entry/src/main/ets/entryability/EntryAbility.ets';
    const newFilePath = '/mnt/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/release/src/main/ets/a/b.ts';
    const projectConfig = {
      projectRootPath: '/mnt/application',
      packageDir: 'oh_modules',
      modulePathMap: {
        entry: '/mnt/application/entry'
      },
      obfuscationMergedObConfig: {
        options: {
          enableFileNameObfuscation: true
        }
      }
    }

    const printerConfig = {
      //Print obfuscation time&memory usage of all files and obfuscation processes
      mFilesPrinter: false,
      //Print time&memory usage of a single file obfuscation in transform processes
      mSingleFilePrinter: false,
      //Print sum up time of transform processes during obfuscation
      mSumPrinter: false,
      //Output path of printer
      mOutputPath: "" 
    }

    const arkguardConfig = {
      mRenameFileName: {
        mEnable: true,
        mNameGeneratorType: 1,
        mReservedFileNames: [
          'entry',
          'mnt',
          'application',
          'entry',
          'src',
          'main',
          'ets',
          'build',
          'default',
          'cache',
          'default',
          'default@CompileArkTS',
          'esmodule',
          'release'
        ],
      },
      mPerformancePrinter: printerConfig
    }

    let arkObfuscator: ArkObfuscator = new ArkObfuscator();
    arkObfuscator.init(arkguardConfig);
    const content = `function foo() {}`
    const obfuscateResult = arkObfuscator.obfuscate(content, filePath, undefined);
    obfuscateResult.then(result => {
      const afterObfuscateFilePath = result.filePath;
      expect(afterObfuscateFilePath === newFilePath).to.be.true;
    })

    const result = tryMangleFileName(filePath, projectConfig, originalFilePath);
    expect(result === newFilePath).to.be.true;
  });

  mocha.it('8-3: test tryMangleFileName when bytecode obfuscation is enabled', async function () {
    const filePath = '/mnt/application/entry/build/default/cache/default/default@CompileArkTS/esmodule/release/src/main/ets/entryability/EntryAbility.ts';
    const originalFilePath = '/mnt/application/entry/src/main/ets/entryability/EntryAbility.ets';
    const projectConfig = {
      projectRootPath: '/mnt/application',
      packageDir: 'oh_modules',
      modulePathMap: {
        entry: '/mnt/application/entry'
      },
      obfuscationMergedObConfig: {
        options: {
          enableFileNameObfuscation: true
        }
      },
      isArkguardEnabled: false,
      isBytecodeObfEnabled: true
    }
    const result = tryMangleFileName(filePath, projectConfig, originalFilePath, projectConfig.isBytecodeObfEnabled);
    expect(result === filePath).to.be.true;
    projectConfig.isBytecodeObfEnabled = false
  });

  mocha.it('9-1: test writeArkguardObfuscatedSourceCode when obfuscation is enabled', async function () {
    this.rollup.build(RELEASE);
    const logger = this.rollup.share.getLogger(GEN_ABC_PLUGIN_NAME);
    const arkguardConfig = {
      mCompact: false,
      mDisableConsole: false,
      mSimplify: false,
      mRemoveComments: true,
      mNameObfuscation: {
        mEnable: true,
        mNameGeneratorType: 1,
        mReservedNames: [],
        mRenameProperties: false,
        mReservedProperties: [],
        mKeepStringProperty: true,
        mTopLevel: false,
        mReservedToplevelNames: [],
        mUniversalReservedProperties: [],
        mUniversalReservedToplevelNames: [],
      },
      mRemoveDeclarationComments: {
        mEnable: true,
        mReservedComments: []
      },
      mEnableSourceMap: true,
      mEnableNameCache: true,
      mRenameFileName: {
        mEnable: false,
        mNameGeneratorType: 1,
        mReservedFileNames: [],
      },
      mExportObfuscation: false,
      mPerformancePrinter: {
        mFilesPrinter: false,
        mSingleFilePrinter: false,
        mSumPrinter: false,
        mOutputPath: ""
      },
      mKeepFileSourceCode: {
        mKeepSourceOfPaths: new Set(),
        mkeepFilesAndDependencies: new Set(),
      },
    };

    const sourceMapGenerator: SourceMapGenerator = SourceMapGenerator.initInstance(this.rollup);
    sourceMapGenerator.setNewSoureMaps(false);
    const arkObfuscator: ArkObfuscator = new ArkObfuscator();
    arkObfuscator.init(arkguardConfig);

    const projectConfig = {
      projectRootPath: PROJECT_ROOT,
      arkObfuscator: arkObfuscator,
      packageDir: 'oh_modules',
      localPackageSet: new Set(),
      useTsHar: false,
      useNormalized: false
    };

    const sourceFileName = 'sourceFile.ts';
    const originalSourceFilePath = path.join(__dirname, '../../test/ark_compiler_ut/testdata/har_obfuscation', sourceFileName);
    const moduleInfo = {
      content: fs.readFileSync(originalSourceFilePath, 'utf-8'),
      buildFilePath: `${PROJECT_ROOT}/har_obfuscation/build/default/cache/${sourceFileName}`,
      relativeSourceFilePath: `har_obfuscation/${sourceFileName}`,
      originSourceFilePath: originalSourceFilePath,
      rollupModuleId: originalSourceFilePath
    };

    try {
      await writeArkguardObfuscatedSourceCode(moduleInfo, logger, projectConfig, undefined);
    } catch (e) {
    }
    const expectedResult = `export function add(d: number, e: number): number {
    return d + e;
}
export function findElement<a>(b: a[], c: (item: a) => boolean): a | undefined {
    return b.find(c);
}
`;
    const result = fs.readFileSync(moduleInfo.buildFilePath, 'utf-8');
    expect(result === expectedResult).to.be.true;

    const declFileName = 'sourceFile.d.ts';
    const originalDeclFilePath = path.join(__dirname, '../../test/ark_compiler_ut/testdata/har_obfuscation/', declFileName);

    const declModuleInfo = {
      content: fs.readFileSync(originalDeclFilePath, 'utf-8'),
      buildFilePath: `${PROJECT_ROOT}/har_obfuscation/build/default/cache/${declFileName}`,
      relativeSourceFilePath: `har_obfuscation/build/default/cache/${declFileName}`,
      originSourceFilePath: originalSourceFilePath,
      rollupModuleId: originalSourceFilePath
    };

    try {
      await writeArkguardObfuscatedSourceCode(declModuleInfo, logger, projectConfig, undefined);
    } catch (e) {
    }

    const expectedDeclResult = `export declare function add(d: number, e: number): number;
export declare function findElement<a>(b: a[], c: (item: a) => boolean): a | undefined;
`;
    const declResult = fs.readFileSync(declModuleInfo.buildFilePath, 'utf-8');
    expect(declResult === expectedDeclResult).to.be.true;
  });

  mocha.describe('10: test getPreviousStageSourceMap', function() {
    let sourceMapGeneratorStub;
    mocha.beforeEach(function () {
      sourceMapGeneratorStub = sinon.stub(SourceMapGenerator, 'getInstance');
    });
  
    mocha.afterEach(() => {
      sinon.restore();
    });

    mocha.it('10-1: should return undefined if relativeSourceFilePath is empty', async function () {
      const moduleInfo = {
        buildFilePath: 'file.js',
        relativeSourceFilePath: '',
        rollupModuleId: 'moduleId'
      };
      const result = getPreviousStageSourceMap(moduleInfo);
      expect(result).to.be.undefined;
    });

    mocha.it('10-2: should return undefined if the file is a declaration file', function() {
      const moduleInfo = {
        buildFilePath: 'file.d.ts',
        relativeSourceFilePath: 'sourceFile.ts',
        rollupModuleId: 'moduleId'
      };
      const result = getPreviousStageSourceMap(moduleInfo);
      expect(result).to.be.undefined;
    });

    mocha.it('10-3: should call getSpecifySourceMap with rollupModuleId if isNewSourceMaps is true', function() {
      const fakeSourceMap = {};
      const instanceStub = {
        isNewSourceMaps: sinon.stub().returns(true),
        getSpecifySourceMap: sinon.stub().returns(fakeSourceMap)
      };
      sourceMapGeneratorStub.returns(instanceStub);
  
      const moduleInfo = {
        buildFilePath: 'file.js',
        relativeSourceFilePath: 'sourceFile.ts',
        rollupModuleId: 'moduleId'
      };
      const result = getPreviousStageSourceMap(moduleInfo);
  
      expect(instanceStub.getSpecifySourceMap.calledWith({}, 'moduleId')).to.be.true;
      expect(result).to.equal(fakeSourceMap);
    });

    mocha.it('10-4: should call getSpecifySourceMap with relativeSourceFilePath if isNewSourceMaps is false', function() {
      const fakeSourceMap = {};
      const instanceStub = {
        isNewSourceMaps: sinon.stub().returns(false),
        getSpecifySourceMap: sinon.stub().returns(fakeSourceMap)
      };
      sourceMapGeneratorStub.returns(instanceStub);
  
      const moduleInfo = {
        buildFilePath: 'file.js',
        relativeSourceFilePath: 'sourceFile.ts',
        rollupModuleId: 'moduleId'
      };
      const result = getPreviousStageSourceMap(moduleInfo);
  
      expect(instanceStub.getSpecifySourceMap.calledWith({}, 'sourceFile.ts')).to.be.true;
      expect(result).to.equal(fakeSourceMap);
    });
  })
  mocha.describe('11: test collectObfuscationFileContent', function () {
    let fileContentManagerStub;
  
    mocha.beforeEach(function () {
      fileContentManagerStub = {
        updateFileContent: sinon.stub(),
      };
    });
  
    mocha.afterEach(function () {
      sinon.restore();
    });
    
    mocha.it('11-1: should not call updateFileContent if filePathManager is not defined', function () {
      const moduleInfo = {
        buildFilePath: 'file.js',
        originSourceFilePath: 'sourceFile.ts',
      };
      const projectConfig = {
        arkObfuscator: {},
      };
      const previousStageSourceMap = {};
  
      collectObfuscationFileContent(moduleInfo, projectConfig, previousStageSourceMap);
  
      expect(fileContentManagerStub.updateFileContent.called).to.be.false;
    });
  
    mocha.it('11-2: should not call updateFileContent if the file is a declaration file and not an original declaration file', function () {
      const moduleInfo = {
        buildFilePath: 'file.d.ts',
        originSourceFilePath: 'sourceFile.ts',
      };
      const projectConfig = {
        arkObfuscator: {
          filePathManager: {},
          fileContentManager: fileContentManagerStub,
        },
      };
      const previousStageSourceMap = {};
  
      collectObfuscationFileContent(moduleInfo, projectConfig, previousStageSourceMap);
  
      expect(fileContentManagerStub.updateFileContent.called).to.be.false;
    });
  
    mocha.it('11-3: should call updateFileContent if the file is not a declaration file', function () {
      const moduleInfo = {
        buildFilePath: 'file.js',
        originSourceFilePath: 'sourceFile.ts',
      };
      const projectConfig = {
        arkObfuscator: {
          filePathManager: {},
          fileContentManager: fileContentManagerStub,
        },
      };
      const previousStageSourceMap = {};
  
      collectObfuscationFileContent(moduleInfo, projectConfig, previousStageSourceMap);
  
      expect(fileContentManagerStub.updateFileContent.calledOnce).to.be.true;
      expect(fileContentManagerStub.updateFileContent.calledWith({
        moduleInfo: moduleInfo,
        previousStageSourceMap: previousStageSourceMap,
      })).to.be.true;
    });
  
    mocha.it('11-4: should call updateFileContent if the file is an original declaration file', function () {
      const moduleInfo = {
        buildFilePath: 'file.d.ts',
        originSourceFilePath: 'file.d.ts',
      };
      const projectConfig = {
        arkObfuscator: {
          filePathManager: {},
          fileContentManager: fileContentManagerStub,
        },
      };
      const previousStageSourceMap = {};
  
      collectObfuscationFileContent(moduleInfo, projectConfig, previousStageSourceMap);
  
      expect(fileContentManagerStub.updateFileContent.calledOnce).to.be.true;
      expect(fileContentManagerStub.updateFileContent.calledWith({
        moduleInfo: moduleInfo,
        previousStageSourceMap: previousStageSourceMap,
      })).to.be.true;
    });
  });
});