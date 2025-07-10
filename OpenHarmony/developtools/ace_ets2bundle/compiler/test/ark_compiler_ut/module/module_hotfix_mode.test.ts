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

import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { ModuleHotfixMode } from '../../../lib/fast_build/ark_compiler/module/module_hotfix_mode';
import { RELEASE } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import {
  ES2ABC_PATH,
  DEBUG_INFO,
  BUILD_INFO,
  BUILD_NPM,
  MODULES_ABC,
  ENTRY_LIST,
  OUTPUT,
  FILE_THREADS,
  MERGE_ABC,
  TARGET_API_VERSION
} from '../mock/rollup_mock/path_config';
import { cpus } from '../utils/utils';

function checkCmdArgs(cmdArgs: Array<object>, compatibleSdkVersion: string) : void {
  const fileThreads: number = cpus();

  expect(cmdArgs[0].indexOf(BUILD_INFO) > 0).to.be.true;
  expect(cmdArgs[1] === ENTRY_LIST).to.be.true;
  expect(cmdArgs[2].indexOf(BUILD_NPM) > 0).to.be.true;
  expect(cmdArgs[3] === OUTPUT).to.be.true;
  expect(cmdArgs[4].indexOf(MODULES_ABC) > 0).to.be.true;
  expect(cmdArgs[5] === FILE_THREADS).to.be.true;
  expect(cmdArgs[6] === `\"${fileThreads}\"`).to.be.true;
  expect(cmdArgs[7].indexOf(compatibleSdkVersion) > 0).to.be.true;
  expect(cmdArgs[8] === MERGE_ABC).to.be.true;
}

mocha.describe('test module_hotfix_mode file api', function () {
  mocha.beforeEach(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.afterEach(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test generateEs2AbcCmdForHotfix under hot fix debug', function () {
    this.rollup.build();
    const moduleMode = new ModuleHotfixMode(this.rollup);
    moduleMode.generateEs2AbcCmdForHotfix();
    const compatibleSdkVersion = `${TARGET_API_VERSION}${this.rollup.share.projectConfig.compatibleSdkVersion}`;
    expect(moduleMode.cmdArgs[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    moduleMode.cmdArgs.shift();
    expect(moduleMode.cmdArgs[0] === DEBUG_INFO).to.be.true;
    moduleMode.cmdArgs.shift();
    checkCmdArgs(moduleMode.cmdArgs,compatibleSdkVersion);
  });

  mocha.it('1-2: test generateEs2AbcCmdForHotfix under hot fix release', function () {
    this.rollup.build(RELEASE);
    const moduleMode = new ModuleHotfixMode(this.rollup);
    moduleMode.generateEs2AbcCmdForHotfix();
    const compatibleSdkVersion = `${TARGET_API_VERSION}${this.rollup.share.projectConfig.compatibleSdkVersion}`;
    expect(moduleMode.cmdArgs[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    moduleMode.cmdArgs.shift();
    expect(moduleMode.cmdArgs[0] === DEBUG_INFO).to.be.false;
    checkCmdArgs(moduleMode.cmdArgs,compatibleSdkVersion);
  });
});