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
import fs from 'fs';
import mocha from 'mocha';
import sinon from 'sinon';
import { expect } from 'chai';

import {
	BUILD_ON,
	JSBUNDLE,
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
import main, {
	partialUpdateConfig, 
	projectConfig, 
	readAppResource, 
	resetGlobalProgram, 
	resetMain, 
	resources, 
	sdkConfigs,
	systemModules
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
	UT_VALIDATE_PAGES,
	UT_VALIDATE_PAGES_JSBUNDLE,
	UT_VALIDATE_PAGES_PREVIEW
} from './helpers/pathConfig';
import { 
	parseFileNameFromPath, 
	parseLog, 
	processExecInStr, 
	sourceReplace 
} from './helpers/parser';
import { 
	scanFileNames
} from './helpers/utils';
import { 
	Logger,
	LogInfo 
} from './helpers/logger';
import { 
	ARKUI_SUBSYSTEM_CODE, 
	TSC_SYSTEM_CODE 
} from './helpers/common';

const PROJECT_ROOT: string = path.resolve(__dirname, '../../test/transform_ut');
const DEFAULT_PROJECT: string = 'application';
const TEST_CASES_PATH: string = path.resolve(PROJECT_ROOT, DEFAULT_PROJECT, 'entry/src/main/ets/pages');
const SYS_CONFIG_PATH: string = path.resolve(PROJECT_ROOT, DEFAULT_PROJECT, 'entry/src/main/ets/test/common');
const ERROR_COLLECTION_PATH: string = path.resolve(__dirname, '../../test/transform_ut_error.json');
const MAIN_PAGES: string[] = [
	...UT_VALIDATE_PAGES,
	...UT_VALIDATE_PAGES_PREVIEW,
	...UT_VALIDATE_PAGES_JSBUNDLE
].map((p) => `pages/utForValidate/${p}`);

const systemModuleSet: Set<string> = new Set();
scanFileNames(SYS_CONFIG_PATH, systemModuleSet);

mocha.describe('test UT for validate testcases [non-preview mode]', function () {
  	this.timeout(10000);

	mocha.before(function () {
		resetUtils();
		resetGlobalProgram();
		resetMain();
		this.rollup = new RollUpPluginMock();
		this.rollup.build(PROJECT_ROOT, DEFAULT_PROJECT, MAIN_PAGES);

		// enable logger for etsTransform roll-up plugin
		this.rollup.share.flushLogger();
		this.rollup.share.setEnableLogger(true);
		this.rollup.share.setAllowedLoggerPrefix(['etsTransform', TSC_SYSTEM_CODE, ARKUI_SUBSYSTEM_CODE]);

		this.globalProjectConfig = new ProjectConfig();
		this.globalProjectConfig.setPreview(false);
		this.globalProjectConfig.setIgnoreWarning(true);
		this.globalProjectConfig.scan(PROJECT_ROOT, DEFAULT_PROJECT, MAIN_PAGES);
		this.globalProjectConfig.mockCompileContextInfo(`${PROJECT_ROOT}/${DEFAULT_PROJECT}`, MAIN_PAGES);
		this.globalProjectConfig.concat(RollUpPluginMock.mockArkProjectConfig(PROJECT_ROOT, DEFAULT_PROJECT, false));

		this.rollup.share.projectConfig.concat(this.globalProjectConfig);
		Object.assign(projectConfig, this.globalProjectConfig);
		readAppResource(projectConfig.appResource);

		this.globalPartialUpdateConfig = new PartialUpdateConfig();
		this.globalPartialUpdateConfig.setPartialUpdateMode(true);
		this.globalPartialUpdateConfig.mockDisableArkTSLinter();
		Object.assign(partialUpdateConfig, this.globalPartialUpdateConfig);

		Object.assign(main, { 
			sdkConfigs: [
				...sdkConfigs
					.filter((sdkConfig) => !sdkConfig['apiPath'].includes(SYS_CONFIG_PATH))
					.map((sdkConfig) => {
						sdkConfig['apiPath'].push(SYS_CONFIG_PATH);
						return sdkConfig;
					}
				),
			],
			systemModules: [...systemModules, ...systemModuleSet]
		});

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

		// disable ignoreWarning to get log info
		Object.assign(projectConfig, { ignoreWarning: false });
	});

	mocha.afterEach(function () {
		this.rollup?.share?.flushLogger();
		processStructComponentV2.resetStructMapInEts();
	});

	[...UT_VALIDATE_PAGES, ...UT_VALIDATE_PAGES_JSBUNDLE].forEach((utPage, index) => {
		mocha.it(`1-${index + 1}: test ${utPage}`, function (done) {
			if (UT_VALIDATE_PAGES_JSBUNDLE.includes(utPage)) {
				Object.assign(projectConfig, { compileMode: JSBUNDLE });
			}

			const sourceFilePath: string = path.resolve(TEST_CASES_PATH, `utForValidate/${utPage}.ets`);
			const sourceCode: string = fs.readFileSync(sourceFilePath, 'utf-8');

			storedFileInfo.addFileCacheInfo(sourceFilePath);

			const transform = this.etsTransformPlugin.transform.bind(this.rollup);
			const errorCollection: object = JSON.parse(fs.readFileSync(ERROR_COLLECTION_PATH, 'utf-8'));
			const errorKey: string = parseFileNameFromPath(sourceFilePath);
			const errorVals: object = errorCollection[errorKey] ?? {};

			const expectResults: LogInfo[] = Array.isArray(errorVals) ? errorVals : [errorVals];

			let expectErrorInfos: LogInfo[] = [];
			let expectWarnInfos: LogInfo[] = [];
			expectResults.filter(e => e.type).forEach((e) => {
				if (e.type === 'ERROR') {
					expectErrorInfos.push({ ...e, message: processExecInStr(e.message.trim()) });
				} else {
					expectWarnInfos.push({ ...e, message: processExecInStr(e.message.trim()) });
				}
			});

			transform(sourceReplace(sourceCode), sourceFilePath)
				.then(_ => {
					const logger: Logger = Logger.mergeLoggers(
						'etsTransform', 
						TSC_SYSTEM_CODE, 
						ARKUI_SUBSYSTEM_CODE
					);

					const errorInfos: LogInfo[] = logger.getErrorInfos();
					const warnInfos: LogInfo[] = logger.getWarnInfos();

					expect(errorInfos.length === expectErrorInfos.length).to.be.true;
					expect(warnInfos.length === expectWarnInfos.length).to.be.true;

					let _findIndex: number = 0;
					let _sumCount: number = expectErrorInfos.length;
					errorInfos.forEach((err) => {
						const message: string = parseLog(err.cause ?? err.message);
						const findIndex: number = expectErrorInfos
							.slice(_findIndex)
							.findIndex((value) => value.message === message);
						expect(findIndex !== -1).to.be.true;
						if (err.code) {
							expect(expectErrorInfos[_findIndex + findIndex].code === err.code).to.be.true;
							expect(expectErrorInfos[_findIndex + findIndex].solutions ?? []).to.deep.equal(err.solutions ?? []);
						}
						_sumCount -= 1;
						_findIndex = findIndex + 1;
					});
					expect(_sumCount === 0).to.be.true;
					
					_findIndex = 0;
					_sumCount = expectWarnInfos.length;
					warnInfos.forEach((err) => {
						const message: string = parseLog(err.cause ?? err.message);
						const findIndex: number = expectWarnInfos
							.slice(_findIndex)
							.findIndex((value) => value.message === message);
						expect(findIndex !== -1).to.be.true;
						_sumCount -= 1;
						_findIndex = findIndex + 1;
					});
					expect(_sumCount === 0).to.be.true;

					done();
				})
				.catch(err => done(err));
		});
	});
});