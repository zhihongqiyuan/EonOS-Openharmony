/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
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

import * as childProcess from 'child_process';
import * as process from 'process';
import cluster from 'cluster';
import { logger } from './compile_info';
import {
  SUCCESS,
  FAIL
} from './pre_define';

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

function js2abcByWorkers(jsonInput: string, cmd: string): Promise<void> {
  const inputPaths: any = JSON.parse(jsonInput);
  for (let i = 0; i < inputPaths.length; ++i) {
    const input: string = inputPaths[i].path.replace(/\.temp\.js$/, '_.js');
    const cacheOutputPath: string = inputPaths[i].cacheOutputPath;
    const cacheAbcFilePath: string = cacheOutputPath.replace(/\.temp\.js$/, '.abc');
    const sourceFile: string = inputPaths[i].sourceFile;
    const singleCmd: string = `${cmd} "${cacheOutputPath}" -o "${cacheAbcFilePath}" --source-file "${sourceFile}"`;
    logger.debug('gen abc cmd is: ', singleCmd, ' ,file size is:', inputPaths[i].size, ' byte');
    try {
      childProcess.execSync(singleCmd);
    } catch (e) {
      logger.debug(red, `ArkTS:ERROR Failed to convert file ${input} to abc. Error message: ${e} `, reset);
      process.exit(FAIL);
    }
  }

  return;
}

logger.debug('worker data is: ', JSON.stringify(process.env));
logger.debug('gen_abc isWorker is: ', cluster.isWorker);
if (cluster.isWorker && process.env.inputs !== undefined && process.env.cmd !== undefined) {
  logger.debug('==>worker #', cluster.worker.id, 'started!');
  js2abcByWorkers(process.env.inputs, process.env.cmd);
  process.exit(SUCCESS);
}
