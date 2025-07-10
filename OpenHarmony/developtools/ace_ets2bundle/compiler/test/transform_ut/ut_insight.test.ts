/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
import fs from 'fs';
import mocha from 'mocha';
import sinon from 'sinon';
import { expect } from 'chai';

import {
  	BUILD_ON,
} from '../../lib/pre_define';
import {
	resetComponentCollection,
  	componentCollection
} from '../../lib/validate_ui_syntax';
import { 
	transformLog 
} from '../../lib/process_ui_syntax';
import {
  	componentInfo,
  	resetUtils,
  	storedFileInfo
} from '../../lib/utils';
import {
	partialUpdateConfig, 
	projectConfig, 
	resetGlobalProgram, 
	resetMain, 
	resources 
} from '../../main';
import { 
	etsChecker 
} from '../../lib/fast_build/ets_ui/rollup-plugin-ets-checker';
import { 
	etsTransform 
} from '../../lib/fast_build/ets_ui/rollup-plugin-ets-typescript';
import processStructComponentV2 from '../../lib/process_struct_componentV2';
import { 
	RollUpPluginMock 
} from './helpers/mockRollupContext';
import { 
	PartialUpdateConfig, 
	ProjectConfig 
} from './helpers/projectConfig';
import { 
	CACHE_PATH,
	UT_PAGES 
} from './helpers/insightUTPath';
import { 
	parseCode, 
	sourceReplace 
} from './helpers/parser';

const PROJECT_ROOT: string = path.resolve(__dirname, '../../test/transform_ut');
const DEFAULT_PROJECT: string = 'application';
const TEST_CASES_PATH: string = path.resolve(PROJECT_ROOT, DEFAULT_PROJECT, 'entry/src/main/ets/pages');
const OUTPUTS_PATH: string = path.resolve(PROJECT_ROOT, DEFAULT_PROJECT, 'entry/build', CACHE_PATH, 'entry/src/main/ets/pages');
const MAIN_PAGES: string[] = UT_PAGES.map((p) => `pages/utForInsightDecorator/${p}`);

mocha.describe('test InsightDecorator UT testcases', function () {
	this.timeout(10000);

	mocha.before(function () {
		resetUtils();
		resetGlobalProgram();
		resetMain();
		this.rollup = new RollUpPluginMock();
		this.rollup.build(PROJECT_ROOT, DEFAULT_PROJECT, MAIN_PAGES);

		this.globalProjectConfig = new ProjectConfig();
		this.globalProjectConfig.setPreview(false);
		this.globalProjectConfig.setIgnoreWarning(true);
		this.globalProjectConfig.scan(PROJECT_ROOT, DEFAULT_PROJECT, MAIN_PAGES);
		this.globalProjectConfig.mockCompileContextInfo(`${PROJECT_ROOT}/${DEFAULT_PROJECT}`, MAIN_PAGES);
		this.globalProjectConfig.concat(RollUpPluginMock.mockArkProjectConfig(PROJECT_ROOT, DEFAULT_PROJECT, false));

		this.rollup.share.projectConfig.concat(this.globalProjectConfig);
		Object.assign(projectConfig, this.globalProjectConfig);

		this.globalPartialUpdateConfig = new PartialUpdateConfig();
		this.globalPartialUpdateConfig.mockDisableArkTSLinter();

		Object.assign(partialUpdateConfig, this.globalPartialUpdateConfig);

		this.etsCheckerPlugin = etsChecker();
		this.etsTransformPlugin = etsTransform();

		// disable writing to local files
		sinon.stub(fs, 'writeSync');

		// run etsChecker once
		const buildStart = this.etsCheckerPlugin.buildStart.bind(this.rollup);
		buildStart();
	});

	mocha.after(() => {
		this.rollup?.share?.flushLogger();
		delete this.rollup;
		delete this.globalProjectConfig;
		delete this.globalPartialUpdateConfig;
		delete this.etsCheckerPlugin;
		delete this.etsTransformPlugin;

		resetUtils();
		resetGlobalProgram();
		resetMain();
		sinon.restore();
	});

	mocha.beforeEach(function () {
		resources.app["media"] = {icon:16777222};
		resources.app["font"] = {song:16777223};

		process.env.rawFileResource = './';
		process.env.compileMode = 'moduleJson';
		process.env.compiler = BUILD_ON;
		process.env.compileTool = 'rollup';

		transformLog.errors = [];
		componentInfo.id = 0;
		componentCollection.customComponents.clear();
		resetComponentCollection();
		storedFileInfo.setCurrentArkTsFile();
	});

	mocha.afterEach(function () {
		processStructComponentV2.resetStructMapInEts();
	});

	UT_PAGES.forEach((utPage, index) => {
		mocha.it(`1-${index + 1}: test ${utPage}`, function (done) {
			const sourceFilePath: string = path.resolve(TEST_CASES_PATH, `utForInsightDecorator/${utPage}.ets`);
			const sourceCode: string = fs.readFileSync(sourceFilePath, 'utf-8');
			
			const targetFilePath: string = path.resolve(OUTPUTS_PATH, `utForInsightDecorator/${utPage}.js.sample`);
			const targetCode: string = fs.readFileSync(targetFilePath, 'utf-8');

			storedFileInfo.addFileCacheInfo(sourceFilePath);

			const transform = this.etsTransformPlugin.transform.bind(this.rollup);

			transform(sourceReplace(sourceCode), sourceFilePath)
				.then(res => {
					expect(parseCode(res.code)).eql(parseCode(targetCode));
					done();
				})
				.catch(err => done(err));
		});
	});
});
