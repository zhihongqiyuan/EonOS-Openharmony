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
import path from "path";
import fs from "fs";
import sinon from 'sinon';
import childProcess from 'child_process';

import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import {
  generateAot,
} from '../../../lib/gen_aot';
import { ModuleMode } from '../../../lib/fast_build/ark_compiler/module/module_mode';
import { TEMPORARY } from '../../../lib/pre_define';

mocha.describe('test gen_aot file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test the error message of generateAot (full)', function () {
    this.rollup.build();
    const moduleMode = new ModuleMode(this.rollup);
    const loggerStub = sinon.stub(moduleMode.logger, 'debug');
    const stub = sinon.stub(moduleMode.logger, 'throwArkTsCompilerError');
    const faultHandler = ((error: string) => { moduleMode.logger.throwArkTsCompilerError(error); })
    try {
      generateAot(moduleMode.arkConfig.arkRootPath, moduleMode.moduleAbcPath,
        moduleMode.projectConfig, moduleMode.logger, faultHandler);
    } catch (e) {
    }
    expect(stub.calledWithMatch('ArkTS:ERROR GenerateAot failed. AppAbc not found in ')).to.be.true;
    expect(stub.calledWithMatch('ArkTS:ERROR GenerateAot failed. unknown anBuildMode: ')).to.be.true;
    loggerStub.restore();
    stub.restore();
  });

  mocha.it('1-2: test the error message of generateAot (partial)', function () {
    this.rollup.build();
    const moduleMode = new ModuleMode(this.rollup);
    const loggerStub = sinon.stub(moduleMode.logger, 'debug');
    const stub = sinon.stub(moduleMode.logger, 'throwArkTsCompilerError');
    const faultHandler = ((error: string) => { moduleMode.logger.throwArkTsCompilerError(error); });
    moduleMode.projectConfig.anBuildMode = 'partial';
    moduleMode.projectConfig.apPath = '';
    try {
      generateAot(moduleMode.arkConfig.arkRootPath, moduleMode.moduleAbcPath,
        moduleMode.projectConfig, moduleMode.logger, faultHandler);
    } catch (e) {
    }
    expect(stub.calledWith('ArkTS:ERROR GenerateAot failed. Invalid profile file path.')).to.be.true;
    expect(stub.calledWithMatch('ArkTS:ERROR GenerateAot failed. Partial mode lost profile in "')).to.be.true;
    loggerStub.restore();
    stub.restore();
  });
});