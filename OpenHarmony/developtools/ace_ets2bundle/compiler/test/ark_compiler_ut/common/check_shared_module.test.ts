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
import ts from 'typescript';

import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { collectSharedModule } from '../../../lib/fast_build/ark_compiler/check_shared_module';
import { LogInfo } from '../../../lib/utils';

mocha.describe('test check_shared_module file api', function() {
  mocha.before(function() {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test collect shared module from empty file', function() {
    let sourceFile: ts.SourceFile;
    try {
      collectSharedModule('', '', sourceFile);
    } catch (e) {
      expect(false).to.be.true;
    }
  });
});
