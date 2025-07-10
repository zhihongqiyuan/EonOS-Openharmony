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

import { writeFileSyncByString } from './ark_utils';
import { projectConfig } from '../main';
import {
  ESMODULE,
  ARK
} from './pre_define';
import { logger } from './compile_info';

module.exports = function processSourcefile(source: string): string {
  if (projectConfig.compileMode === ESMODULE && process.env.compilerType &&
      process.env.compilerType === ARK) {
    writeFileSyncByString(this.resourcePath, source, projectConfig, logger);
  }
  return source;
};
