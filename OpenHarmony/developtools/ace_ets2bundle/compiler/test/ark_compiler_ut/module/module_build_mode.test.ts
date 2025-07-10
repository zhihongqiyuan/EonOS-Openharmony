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

import {
  ES2ABC,
  TS2ABC
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { ModuleBuildMode } from '../../../lib/fast_build/ark_compiler/module/module_build_mode';
import {
  ArkTSInternalErrorDescription,
  ErrorCode
} from '../../../lib/fast_build/ark_compiler/error_code';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';

mocha.describe('test module_build_mode file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test the error message of ModuleBuildMode executeArkCompiler', function () {
    this.rollup.build();
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_INVALID_COMPILE_MODE,
      ArkTSInternalErrorDescription,
      'Invalid compilation mode. ' +
      `ProjectConfig.pandaMode should be either ${TS2ABC} or ${ES2ABC}.`
    );
    const moduleBuildMode = new ModuleBuildMode(this.rollup);
    moduleBuildMode.projectConfig.pandaMode = 'invalid value';
    const stub = sinon.stub(moduleBuildMode.logger.getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    moduleBuildMode.executeArkCompiler();
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('1-2: test the error message of ModuleBuildMode executeArkCompiler ' +
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
    const moduleBuildMode = new ModuleBuildMode(this.rollup);
    moduleBuildMode.projectConfig.pandaMode = 'invalid value';
    const stub = sinon.stub(moduleBuildMode.logger, 'throwArkTsCompilerError');
    moduleBuildMode.executeArkCompiler();
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });
});