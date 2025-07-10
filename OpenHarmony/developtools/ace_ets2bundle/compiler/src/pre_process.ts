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

import {
  validateUISyntax,
  processSystemApi,
  ReplaceResult,
  sourceReplace,
  componentCollection
} from './validate_ui_syntax';
import {
  LogInfo,
  emitLogInfo,
  storedFileInfo
} from './utils';
import { BUILD_ON } from './pre_define';
import { parseVisual } from './process_visual';
import {
  CUSTOM_BUILDER_METHOD,
  GLOBAL_CUSTOM_BUILDER_METHOD,
  INNER_CUSTOM_BUILDER_METHOD,
  INNER_CUSTOM_LOCALBUILDER_METHOD
} from './component_map';

function preProcess(source: string): string {
  process.env.compiler = BUILD_ON;
  if (/\.ets$/.test(this.resourcePath)) {
    storedFileInfo.setCurrentArkTsFile();
    clearCollection();
    const result: ReplaceResult = sourceReplace(source, this.resourcePath);
    let newContent: string = result.content;
    const log: LogInfo[] = result.log.concat(validateUISyntax(source, newContent,
      this.resourcePath, this.resourceQuery));
    newContent = parseVisual(this.resourcePath, this.resourceQuery, newContent, log, source);
    if (log.length) {
      emitLogInfo(this, log);
    }
    return newContent;
  } else {
    return processSystemApi(source, false, this.resourcePath);
  }
}

function clearCollection(): void {
  componentCollection.customComponents.clear();
  CUSTOM_BUILDER_METHOD.clear();
  INNER_CUSTOM_LOCALBUILDER_METHOD.clear();
  GLOBAL_CUSTOM_BUILDER_METHOD.clear();
  INNER_CUSTOM_BUILDER_METHOD.clear();
}

module.exports = preProcess;
