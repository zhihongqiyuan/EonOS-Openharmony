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
import * as fs from 'fs';
import * as path from 'path';
import cluster from 'cluster';
import { logger } from './compile_info';
import {
  SUCCESS,
  FAIL
} from './pre_define';
import {
  toUnixPath
} from './utils';

const red: string = '\u001b[31m';
const reset: string = '\u001b[39m';

function js2abcByWorkers(jsonInput: string, cmd: string, workerFileName: string): Promise<void> {
  const inputPaths: any = JSON.parse(jsonInput);
  // cmd `${cmd} --input-file xx --output-proto --merge-abc`
  let filePath: string = path.join(process.env.cachePath, workerFileName);
  let content: string = '';
  for (let i = 0; i < inputPaths.length; ++i) {
    let info: any = inputPaths[i];
    const moduleType: string = info.isCommonJs ? 'commonjs' : 'esm';
    content +=
      `${info.tempFilePath};${info.recordName};${moduleType};${toUnixPath(info.sourceFile)};${info.packageName}`;
    if (i < inputPaths.length - 1) {
      content += '\n';
    }
  }
  fs.writeFileSync(filePath, content, 'utf-8');
  const singleCmd: string = `${cmd} --input-file "${filePath}" --output-proto --merge-abc`;
  logger.debug('gen abc cmd is: ', singleCmd);
  try {
    childProcess.execSync(singleCmd);
  } catch (e) {
    logger.debug(red, `ArkTS:ERROR Failed to convert file to proto `, reset);
    process.exit(FAIL);
  }

  return;
}

logger.debug('worker data is: ', JSON.stringify(process.env));
logger.debug('gen_abc isWorker is: ', cluster.isWorker);
if (cluster.isWorker && process.env.inputs !== undefined && process.env.cmd !== undefined &&
    process.env.workerFileName !== undefined && process.env.cachePath !== undefined) {
  logger.debug('==>worker #', cluster.worker.id, 'started!');
  js2abcByWorkers(process.env.inputs, process.env.cmd, process.env.workerFileName);
  process.exit(SUCCESS);
}
