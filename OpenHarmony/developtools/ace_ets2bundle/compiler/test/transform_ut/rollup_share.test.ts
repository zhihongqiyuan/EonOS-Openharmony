/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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
import path from 'path';
import mocha from 'mocha';
import { expect } from 'chai';
import { RollUpPluginMock } from './helpers/mockRollupContext';

const PROJECT_ROOT = path.resolve(__dirname, '../../test/transform_ut');
const DEFAULT_PROJECT: string = 'application';

mocha.describe('test rollup mock', function () {
	mocha.before(function () {
		this.rollup = new RollUpPluginMock();
	});

	mocha.after(() => {
		delete this.rollup;
	});

	mocha.it('1-1: test rollup projectConfig', function () {
		this.rollup.preview(PROJECT_ROOT, DEFAULT_PROJECT);

		expect(this.rollup.share.projectConfig !== null).to.be.true;
	});
});