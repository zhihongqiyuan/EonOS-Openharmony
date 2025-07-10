/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import sinon from 'sinon';
import fs from 'fs';
import childProcess from 'child_process';
import cluster from 'cluster';
import path from 'path';

import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { BundleMode } from '../../../lib/fast_build/ark_compiler/bundle/bundle_mode';
import { changeFileExtension } from '../../../lib/fast_build/ark_compiler/utils';
import {
  DEBUG,
  ES2ABC,
  RELEASE,
  TEMPORARY,
  TS2ABC
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { toUnixPath } from '../../../lib/utils';
import { sleep } from "../utils/utils";
import {
  ArkTSErrorDescription,
  ArkTSInternalErrorDescription,
  ErrorCode
} from '../../../lib/fast_build/ark_compiler/error_code';
import { 
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';

mocha.describe('test bundle_mode file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test error message of executeArkCompiler', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
      ArkTSInternalErrorDescription,
      'Invalid compilation mode. ' + 
      `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
    );
    const bundleMode = new BundleMode(this.rollup, rollupBundleFileSet);
    bundleMode.projectConfig.pandaMode = 'invalid value';
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    bundleMode.executeArkCompiler();
    expect(stub.calledWith(errInfo)).to.be.true; 
    stub.restore();
  });

  mocha.it('1-2: test error message of executeArkCompiler without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
      ArkTSInternalErrorDescription,
      'Invalid compilation mode. ' + 
      `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode = new BundleMode(this.rollup, rollupBundleFileSet);
    bundleMode.projectConfig.pandaMode = 'invalid value';
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    bundleMode.executeArkCompiler();
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('2-1: test the error message of executeEs2AbcCmd handler error', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_EXECUTE_ES2ABC_WITH_ASYNC_HANDLER_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to execute es2abc with async handler. Error: Execution failed'
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').throws(new Error('Execution failed'));
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    try {
      bundleMode.executeEs2AbcCmd();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true; 
    triggerAsyncStub.restore();
    stub.restore();
  });

  mocha.it('2-2: test the error message of executeEs2AbcCmd throw error on failed code', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_ES2ABC_EXECUTION_FAILED,
      ArkTSErrorDescription,
      'Failed to execute es2abc.',
      '',
      ["Please refer to es2abc's error codes."]
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const child = childProcess.exec('false', { windowsHide: true });
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').returns(child);
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    const afterCompilationProcessStub = sinon.stub(bundleMode, 'afterCompilationProcess');
    bundleMode.executeEs2AbcCmd();
    await sleep(1000);
    expect(stub.calledWith(errInfo)).to.be.true;
    triggerAsyncStub.restore();
    stub.restore();
    afterCompilationProcessStub.restore();
  });

  mocha.it('2-3: test the error message of executeEs2AbcCmd throw error(Failed to startup es2abc)', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_ES2ABC_SUBPROCESS_START_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to initialize or launch the es2abc process. Error: test error'
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const child = {
      on: sinon.stub(),
      stderr: {
        on: sinon.stub(),
      },
    };
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').callsFake((callback) => {
      callback();
      return child;
    });
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    let errorEventCallback;
    child.on.callsFake((event, callback) => {
      if (event === 'error') {
        errorEventCallback = callback;
      }
    });
    bundleMode.executeEs2AbcCmd();
    if (errorEventCallback) {
      errorEventCallback(new Error('test error'));
    }
    await sleep(100);
    expect(stub.calledWith(errInfo)).to.be.true;
    triggerAsyncStub.restore();
    stub.restore();
  });

  mocha.it('2-4: test the error message of executeEs2AbcCmd handler error ' +
    'without getHvigorConsoleLogger', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_EXECUTE_ES2ABC_WITH_ASYNC_HANDLER_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to execute es2abc with async handler. Error: Execution failed'
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').throws(new Error('Execution failed'));
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    try {
      bundleMode.executeEs2AbcCmd();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true; 
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    triggerAsyncStub.restore();
    stub.restore();
  });

  mocha.it('2-5: test the error message of executeEs2AbcCmd throw error on failed code ' +
    'without getHvigorConsoleLogger', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_ES2ABC_EXECUTION_FAILED,
      ArkTSErrorDescription,
      'Failed to execute es2abc.',
      '',
      ["Please refer to es2abc's error codes."]
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const child = childProcess.exec('false', { windowsHide: true });
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').returns(child);
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    const afterCompilationProcessStub = sinon.stub(bundleMode, 'afterCompilationProcess');
    bundleMode.executeEs2AbcCmd();
    await sleep(1000);
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    triggerAsyncStub.restore();
    stub.restore();
    afterCompilationProcessStub.restore();
  });

  mocha.it('2-6: test the error message of executeEs2AbcCmd throw error(Failed to startup es2abc) ' +
    'without getHvigorConsoleLogger', async function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_ES2ABC_SUBPROCESS_START_FAILED,
      ArkTSInternalErrorDescription,
      'Failed to initialize or launch the es2abc process. Error: test error'
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const child = {
      on: sinon.stub(),
      stderr: {
        on: sinon.stub(),
      },
    };
    const triggerAsyncStub = sinon.stub(bundleMode, 'triggerAsync').callsFake((callback) => {
      callback();
      return child;
    });
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    let errorEventCallback;
    child.on.callsFake((event, callback) => {
      if (event === 'error') {
        errorEventCallback = callback;
      }
    });
    bundleMode.executeEs2AbcCmd();
    if (errorEventCallback) {
      errorEventCallback(new Error('test error'));
    }
    await sleep(100);
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    triggerAsyncStub.restore();
    stub.restore();
  });

  mocha.it('3-1: test the error message of collectBundleFileList(file.type is invalid value)', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': ''
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_SOURCE_CODE_FROM_SUMMARY,
      ArkTSInternalErrorDescription,
      'Failed to retrieve source code for test.js from rollup file set.'
    );
    const logger: CommonLogger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('3-1-1: test the error message of collectBundleFileList(file.type is invalid value)' +
    ' without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': ''
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_SOURCE_CODE_FROM_SUMMARY,
      ArkTSInternalErrorDescription,
      'Failed to retrieve source code for test.js from rollup file set.'
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger: CommonLogger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger, 'throwArkTsCompilerError');
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('3-2: test the error message of collectBundleFileList', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
          'type': 'asset',
          'source': 'test'
      }
    };
    const existsSyncStub = sinon.stub(fs, 'existsSync').callsFake((path) => {
      const pattern = /test\.temp\.js$/;
      if (pattern.test(path)) {
        return false;
      }
      return true;
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GENERATE_CACHE_SOURCE_FILE,
      ArkTSInternalErrorDescription,
      'Failed to generate cached source file: test.js.'
    );
    const logger: CommonLogger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    try {
      new BundleMode(this.rollup, rollupBundleFileSet);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    existsSyncStub.restore();
    stub.restore();
  });

  mocha.it('3-2-1: test the error message of collectBundleFileList without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
          'type': 'asset',
          'source': 'test'
      }
    };
    const existsSyncStub = sinon.stub(fs, 'existsSync').callsFake((path) => {
      const pattern = /test\.temp\.js$/;
      if (pattern.test(path)) {
        return false;
      }
      return true;
    });
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GENERATE_CACHE_SOURCE_FILE,
      ArkTSInternalErrorDescription,
      'Failed to generate cached source file: test.js.'
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger: CommonLogger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger, 'throwArkTsCompilerError');
    try {
      new BundleMode(this.rollup, rollupBundleFileSet);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    existsSyncStub.restore();
    stub.restore();
  });

  mocha.it('4-1: test the error message of filterBundleFileListWithHashJson', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_PACKAGE_CACHE_IN_INCREMENTAL_BUILD,
      ArkTSInternalErrorDescription,
      sinon.match.any
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const jsonData = JSON.stringify(rollupBundleFileSet, null, 2);
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    for (const value of bundleMode.intermediateJsBundle.values()) {
      fs.unlinkSync(value.cacheFilePath);
    }
    fs.writeFileSync(bundleMode.hashJsonFilePath, jsonData);
    bundleMode.filterBundleFileListWithHashJson();
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('4-1-1: test the error message of filterBundleFileListWithHashJson without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_RETRIEVE_PACKAGE_CACHE_IN_INCREMENTAL_BUILD,
      ArkTSInternalErrorDescription
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const jsonData = JSON.stringify(rollupBundleFileSet, null, 2);
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    for (const value of bundleMode.intermediateJsBundle.values()) {
      fs.unlinkSync(value.cacheFilePath);
    }
    fs.writeFileSync(bundleMode.hashJsonFilePath, jsonData);
    bundleMode.filterBundleFileListWithHashJson();
    expect(stub.calledWithMatch(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('5-1: test the error message of invokeTs2AbcWorkersToGenAbc(worker error)', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    const clusterStub = sinon.stub(cluster, 'fork');
    const fakeWorker = {
      on: sinon.stub()
    };
    clusterStub.returns(fakeWorker);
    const splittedBundles = bundleMode.getSplittedBundles()
    try {
      fakeWorker.on.withArgs('message').callsFake((event, callback) => {
        callback({ data: 'error' });
      });
      bundleMode.invokeTs2AbcWorkersToGenAbc(splittedBundles)
    } catch (e) {
    }
    expect(stub.calledWith('ArkTS:ERROR Failed to execute ts2abc')).to.be.true;
    clusterStub.restore();
    stub.restore();
  });

  mocha.it('6-1: test the error message of writeHashJson', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_HASH_JSON_FILE_GENERATION_MISSING_PATHS,
      ArkTSInternalErrorDescription,
      sinon.match.any
    );
    const bundleMode = new BundleMode(this.rollup, rollupBundleFileSet);
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    try {
      bundleMode.writeHashJson();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('6-2: test the error message of writeHashJson without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_HASH_JSON_FILE_GENERATION_MISSING_PATHS,
      ArkTSInternalErrorDescription
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode = new BundleMode(this.rollup, rollupBundleFileSet);
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    try {
      bundleMode.writeHashJson();
    } catch (e) {
    }
    expect(stub.calledWithMatch(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('7-1: test the error message of copyFileFromCachePathToOutputPath', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INCREMENTAL_BUILD_MISSING_CACHE_ABC_FILE_PATH,
      ArkTSInternalErrorDescription,
      sinon.match.any
    );
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const stub = sinon.stub(bundleMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    try {
      bundleMode.copyFileFromCachePathToOutputPath();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('7-2: test the error message of copyFileFromCachePathToOutputPath ' +
    'without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INCREMENTAL_BUILD_MISSING_CACHE_ABC_FILE_PATH,
      ArkTSInternalErrorDescription,
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const stub = sinon.stub(bundleMode.logger, 'throwArkTsCompilerError');
    try {
      bundleMode.copyFileFromCachePathToOutputPath();
    } catch (e) {
    }
    expect(stub.calledWithMatch(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('8-1: test sourceFile field of bundle mode in release', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.buildMode = RELEASE
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const filesinfo: string[] = fs.readFileSync(bundleMode.generateFileInfoOfBundle()).toString().split(";");
    // sourceFile is the 4th field in filesInfo
    const sourceFile: string = filesinfo[3];
    const relativeCachePath: string = toUnixPath(bundleMode.projectConfig.cachePath.replace(
      bundleMode.projectConfig.projectRootPath + path.sep, ''));
    const buildDirArr: string[] = bundleMode.projectConfig.aceModuleBuild.split(path.sep);
    const abilityDir: string = buildDirArr[buildDirArr.length - 1];

    expect(sourceFile === path.join(relativeCachePath, TEMPORARY, abilityDir, 'test.temp.js')).to.be.true;
  });

  mocha.it('9-1: test sourceFile field of bundle mode in debug', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.buildMode = DEBUG
    const rollupBundleFileSet: Object = {
      'test.js': {
        'type': 'asset',
        'source': 'test'
      }
    };
    const bundleMode =  new BundleMode(this.rollup, rollupBundleFileSet);
    const filesinfo: string[] = fs.readFileSync(bundleMode.generateFileInfoOfBundle()).toString().split(";");
    // sourceFile is the 4th field in filesInfo
    const sourceFile: string = filesinfo[3];
    const relativePath: string = toUnixPath(bundleMode.projectConfig.aceModuleBuild.replace(
      bundleMode.projectConfig.projectRootPath + path.sep, ''));
  
    expect(sourceFile === path.join(relativePath, 'test.js')).to.be.true;
  });
});