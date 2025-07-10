#!/usr/bin/env node

/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

import {program} from 'commander';
import * as path from 'path';

import { ArkObfuscatorForTest } from '../ArkObfuscatorForTest';
import { performancePrinter } from '../ArkObfuscator';
import { EventList, endFilesEvent, startFilesEvent } from '../utils/PrinterUtils';
import { UnitTestUtil } from '../utils/UnitTestUtil';
import { FileUtils } from '../utils/FileUtils';

import type { IOptions } from '../configs/IOptions';

/**
 * Main Entry of Obfuscation in
 */
const minParametersNum: number = 3;
(function (): void {
  if (process.argv.length < minParametersNum) {
    console.error('Too few input parameters.');
    console.error('Usage: SecHarmony [input files] [options]');
    return;
  }

  initOptionsSetting();
  let configPath: string = program.opts()?.configPath;
  configPath = path.resolve(configPath);
  let fileList: Array<string> = [];
  program.args.forEach((value) => {
    const resolved: string = path.resolve(value);
    fileList.push(resolved);
  });

  let obfuscator: ArkObfuscatorForTest = new ArkObfuscatorForTest(fileList, configPath);
  startFilesEvent(EventList.OBFUSCATION_INITIALIZATION, performancePrinter.timeSumPrinter);
  const config: IOptions = FileUtils.readFileAsJson(configPath);
  const initSuccess: boolean = obfuscator.init(config);
  let inplace: boolean = program.opts()?.inplace;
  if (inplace) {
    obfuscator.setWriteOriginalFile(true);
  }

  let testType: string | undefined = program.opts()?.testType;
  obfuscator.setTestType(testType);

  UnitTestUtil.initKeepPathConfig(obfuscator.customProfiles, obfuscator.configPath);
  endFilesEvent(EventList.OBFUSCATION_INITIALIZATION, performancePrinter.timeSumPrinter);
  if (!initSuccess) {
    console.error('init from config file error.');
    return;
  }
  obfuscator.obfuscateFiles();
})();

function initOptionsSetting(): void {
  program.name('SecHarmony')
    .version('1.0.0')
    .description('A tool to obfuscate open harmony application written by Javascript or Typescript.')
    .usage('Usage: SecHarmony [input files] [options]')
    .option('-v, --version', 'show version information.')
    .option('--inplace', 'write the ofuscated content to the original file')
    .option('--test-type <type>', 'Indicates the type of test case, must be either "grammar" or "combinations"')
    .option('-cp, --config-path <dir>', 'obfuscation configuration for open harmony application.')
    .parse();
}
