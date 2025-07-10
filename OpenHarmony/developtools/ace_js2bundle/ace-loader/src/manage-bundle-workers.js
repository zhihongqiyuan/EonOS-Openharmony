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

import * as path from "path";
import cluster from "cluster";
import process from "process";
const genAbcScript = "gen-abc.js";
const FAIL = 1;

if (process.env['arkEnvParams'] === undefined) {
  process.exit(FAIL);
}

let arkEnvParams = JSON.parse(process.env['arkEnvParams']);
if (arkEnvParams['workerNumber'] !== undefined &&
  arkEnvParams['splittedData'] !== undefined &&
  arkEnvParams['cmdPrefix'] !== undefined) {
  const clusterNewApiVersion = 16;
  const currentNodeVersion = parseInt(process.version.split(".")[0]);
  const useNewApi = currentNodeVersion >= clusterNewApiVersion;

  let workerNumber = parseInt(arkEnvParams['workerNumber']);
  let splittedData = JSON.parse(arkEnvParams['splittedData']);
  let cmdPrefix = arkEnvParams['cmdPrefix'];

  if ((useNewApi && cluster.isPrimary) || (!useNewApi && cluster.isMaster)) {
    if (useNewApi) {
      cluster.setupPrimary({
        exec: path.resolve(__dirname, genAbcScript),
      });
    } else {
      cluster.setupMaster({
        exec: path.resolve(__dirname, genAbcScript),
      });
    }

    for (let i = 0; i < workerNumber; ++i) {
      const workerData = {
        inputs: JSON.stringify(splittedData[i]),
        cmd: cmdPrefix,
      };
      cluster.fork(workerData);
    }
    cluster.on("exit", (worker, code, signal) => {
      if (code === FAIL) {
        process.exit(FAIL);
      }
    });
  }
}
