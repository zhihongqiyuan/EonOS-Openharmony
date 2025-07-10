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
import sinon from 'sinon';

import {
  ES2ABC,
  RELEASE,
  OH_MODULES
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { TS2ABC } from '../../../lib/pre_define';
import CommonModeMock from '../mock/class_mock/common_mode_mock';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import {
  ES2ABC_PATH,
  TS2ABC_PATH
} from '../mock/rollup_mock/path_config';
import {
  CMD_DEBUG_INFO,
  NODE,
  EXPOSE_GC,
  DEBUG,
  MODULES
} from '../mock/rollup_mock/common';
import {
  ArkTSInternalErrorDescription,
  ArkTSErrorDescription,
  ErrorCode,
} from '../../../lib/fast_build/ark_compiler/error_code';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';

mocha.describe('test common_mode file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1-1: test initCmdEnv under build debug: projectConfig.pandaMode is Ts2Abc', function () {
    this.rollup.build();
    const modeMock = new CommonModeMock(this.rollup);
    modeMock.projectConfig.pandaMode = TS2ABC;
    modeMock.projectConfig.packageDir = OH_MODULES;
    const args: string[] = modeMock.checkInitCmdEnv();
    expect(args.length === 5).to.be.true;
    expect(args[0] === NODE).to.be.true;
    expect(args[1].indexOf(EXPOSE_GC) > 0).to.be.true;
    expect(args[2].indexOf(TS2ABC_PATH) > 0).to.be.true;
    expect(args[3].indexOf(DEBUG) > 0).to.be.true;
    expect(args[4].indexOf(MODULES) > 0).to.be.true;
  });

  mocha.it('1-1-2: test initCmdEnv under build debug: projectConfig.pandaMode is Es2Abc', function () {
    this.rollup.build();
    const modeMock = new CommonModeMock(this.rollup);
    const args: string[] = modeMock.checkInitCmdEnv();
    expect(args.length === 2).to.be.true;
    expect(args[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(args[1] === CMD_DEBUG_INFO).to.be.true;
  });

  mocha.it('1-1-3: test initCmdEnv under build debug: projectConfig.pandaMode is invalid value', function () {
    this.rollup.build();
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
      ArkTSInternalErrorDescription,
      'Invalid compilation mode. ' + 
      `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
    );
    const modeMock = new CommonModeMock(this.rollup);
    const stub = sinon.stub(modeMock.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    modeMock.projectConfig.pandaMode = 'invalid value';
    try {
      modeMock.checkInitCmdEnv();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('1-1-4: test initCmdEnv under build debug: projectConfig.pandaMode is invalid value ' +
    'without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
      ArkTSInternalErrorDescription,
      'Invalid compilation mode. ' + 
      `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const modeMock = new CommonModeMock(this.rollup);
    const stub = sinon.stub(modeMock.logger, 'throwArkTsCompilerError');
    modeMock.projectConfig.pandaMode = 'invalid value';
    try {
      modeMock.checkInitCmdEnv();
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('1-2: test initCmdEnv under build release', function () {
    this.rollup.build(RELEASE);
    const modeMock = new CommonModeMock(this.rollup);
    const args: string[] = modeMock.checkInitCmdEnv();
    expect(args.length === 1).to.be.true;
    expect(args[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(args[0] === CMD_DEBUG_INFO).to.be.false;
  });

  mocha.it('1-3: test initCmdEnv under preview debug', function () {
    this.rollup.preview();
    const modeMock = new CommonModeMock(this.rollup);
    const args: string[] = modeMock.checkInitCmdEnv();
    expect(args.length === 2).to.be.true;
    expect(args[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(args[1] === CMD_DEBUG_INFO).to.be.true;
  });

  mocha.it('1-4: test initCmdEnv under hot reload debug', function () {
    this.rollup.hotReload();
    const modeMock = new CommonModeMock(this.rollup);
    const args: string[] = modeMock.checkInitCmdEnv();
    expect(args.length === 2).to.be.true;
    expect(args[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(args[1] === CMD_DEBUG_INFO).to.be.true;
  });
});