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

import * as path from 'path';
import cluster from 'cluster';
import process from 'process';
import {
  ESMODULE,
  FAIL,
  GEN_ABC_SCRIPT,
  GEN_MODULE_ABC_SCRIPT,
  JSBUNDLE
} from './pre_define';

if (process.env.arkEnvParams === undefined) {
  process.exit(FAIL);
}

let arkEnvParams = JSON.parse(process.env.arkEnvParams);
if (arkEnvParams.mode !== JSBUNDLE && arkEnvParams.mode !== ESMODULE) {
  process.exit(FAIL);
}

if (arkEnvParams.workerNumber !== undefined &&
  arkEnvParams.splittedData !== undefined &&
  arkEnvParams.cmdPrefix !== undefined) {
  let workerNumber: number = parseInt(arkEnvParams.workerNumber);
  let splittedData: Object = JSON.parse(arkEnvParams.splittedData);
  let cmdPrefix: string = arkEnvParams.cmdPrefix;

  const clusterNewApiVersion: number = 16;
  const currentNodeVersion: number = parseInt(process.version.split('.')[0]);
  const useNewApi: boolean = currentNodeVersion >= clusterNewApiVersion;

  if ((useNewApi && cluster.isPrimary) || (!useNewApi && cluster.isMaster)) {
    let genAbcScript: string = GEN_ABC_SCRIPT;
    if (arkEnvParams.mode === ESMODULE) {
      genAbcScript = GEN_MODULE_ABC_SCRIPT;
    }
    if (useNewApi) {
      cluster.setupPrimary({
        exec: path.resolve(__dirname, genAbcScript)
      });
    } else {
      cluster.setupMaster({
        exec: path.resolve(__dirname, genAbcScript)
      });
    }

    for (let i = 0; i < workerNumber; ++i) {
      let workerData: Object = {
        'inputs': JSON.stringify(splittedData[i]),
        'cmd': cmdPrefix
      };
      if (arkEnvParams.mode === ESMODULE) {
        let sn: number = i + 1;
        let workerFileName: string = `filesInfo_${sn}.txt`;
        workerData.workerFileName = workerFileName;
        workerData.cachePath = arkEnvParams.cachePath;
      }
      cluster.fork(workerData);
    }

    cluster.on('exit', (worker, code, signal) => {
      if (code === FAIL) {
        process.exit(FAIL);
      }
    });
  }
}
