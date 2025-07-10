/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

import { BUILD_OFF, ESMODULE, JSBUNDLE } from './pre_define';
import {
  resetLog,
  transformLog
} from './process_ui_syntax';
import {
  propertyCollection,
  linkCollection,
  processSystemApi
} from './validate_ui_syntax';
import {
  emitLogInfo,
  componentInfo,
  generateSourceFilesInHar,
  getTransformLog
} from './utils';
import { generateSourceFilesToTemporary } from './ark_utils';
import { resetComponentCollection } from './validate_ui_syntax';
import { abilityConfig, projectConfig } from '../main';
import { logger } from './compile_info';
import processStructComponentV2 from './process_struct_componentV2';

module.exports = function resultProcess(source: string, map: any): void {
  process.env.compiler = BUILD_OFF;
  source = processSystemApi(source, true, this.resourcePath);
  if (/\.(ets|ts)$/.test(this.resourcePath)) {
    componentInfo.id = 0;
    propertyCollection.clear();
    linkCollection.clear();
    resetComponentCollection();
    processStructComponentV2.resetStructMapInEts();
    if (transformLog && transformLog.errors.length) {
      emitLogInfo(this, getTransformLog(transformLog));
      resetLog();
    }
  }
  if (projectConfig.compileMode === JSBUNDLE &&
    [abilityConfig.abilityEntryFile].concat(abilityConfig.projectAbilityPath).concat(abilityConfig.testRunnerFile).includes(this.resourcePath)) {
    source = source.replace(/exports\.default/, 'globalThis.exports.default');
  }

  if (projectConfig.compileMode === ESMODULE && projectConfig.processTs === false &&
    process.env.compilerType && process.env.compilerType === 'ark' && !projectConfig.compileHar) {
    generateSourceFilesToTemporary(this.resourcePath, source, map, projectConfig, logger);
  }

  if (projectConfig.compileHar) {
    generateSourceFilesInHar(this.resourcePath, source, '.js', projectConfig);
  }

  this.callback(null, source, map);
};
