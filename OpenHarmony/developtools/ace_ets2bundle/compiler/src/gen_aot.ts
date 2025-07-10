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

import * as childProcess from 'child_process';
import * as process from 'process';
import * as fs from 'fs';
import * as path from 'path';
import * as os from 'os';
import {
  MODULES_ABC,
  TEMPORARY,
  AOT_FULL,
  AOT_TYPE,
  AOT_PARTIAL,
  TS2ABC
} from './pre_define';
import {
  isWindows,
  mkdirsSync,
  toUnixPath,
  validateFilePathLength,
  validateFilePathLengths
} from './utils';
import {
  getArkBuildDir,
  getBuildBinDir
} from './ark_utils';

const hostToolKeyWords: string = '[HostTool] ';
const logLevelIndex: number = 4;

export interface FaultHandler {
  (error: string): void
}

function checkAotPartialConfig(compileMode: string, buildJsonInfo: any, faultHandler: FaultHandler): boolean {
  if (buildJsonInfo.anBuildMode !== AOT_PARTIAL && !buildJsonInfo.apPath) {
    // no AOT's partial related configuration is hit, pass the configuration to next compile mode
    return false;
  }
  // Aot compiler's partial mode.
  return true;
}

function checkAotFullConfig(compileMode: string, buildJsonInfo: any, faultHandler: FaultHandler): boolean {
  if (buildJsonInfo.anBuildMode !== AOT_FULL) {
    // no AOT's full related configuration is hit, pass the configuration to next compile mode
    return false;
  }
  // Aot compiler's full mode.
  return true;
}

function checkAotTypeConfig(compileMode: string, buildJsonInfo: any, faultHandler: FaultHandler): boolean {
  if (buildJsonInfo.anBuildMode !== AOT_TYPE) {
    // no AOT's type related configuration is hit, pass the configuration to next compile mode
    return false;
  }
  // Aot compiler's type mode.
  return true;
}

/**
 * Check if the AOT related configuration is hit
 * @param compileMode CompileMode for the project, which can be jsbundle or esmodule
 * @param buildJsonInfo buildJsonInfo which parsed from projectConfig.aceBuildJson
 * @param faultHandler faultHandler for illegal AOT configuration
 * @returns {Boolean} false means no AOT related configuration found, else return true
 * @api private
 */
export function checkAotConfig(compileMode: string, buildJsonInfo: any, faultHandler: FaultHandler): boolean {
  return checkAotTypeConfig(compileMode, buildJsonInfo, faultHandler) ||
    checkAotFullConfig(compileMode, buildJsonInfo, faultHandler) ||
    checkAotPartialConfig(compileMode, buildJsonInfo, faultHandler);
}

export function generateAot(arkDir: string, appAbc: string,
                            projectConfig: any, logger: any, faultHandler: FaultHandler): void {
  let aotCompiler: string = path.join(getBuildBinDir(arkDir), isWindows() ? 'ark_aot_compiler.exe' : 'ark_aot_compiler');
  const appAot: string = path.join(projectConfig.anBuildOutPut, projectConfig.moduleName);

  if (!validateFilePathLengths([aotCompiler, appAbc, appAot], logger)) {
    faultHandler('ArkTS:ERROR GenerateAot failed. Invalid file path.');
  }
  if (!fs.existsSync(appAbc)) {
    faultHandler(`ArkTS:ERROR GenerateAot failed. AppAbc not found in "${appAbc}"`);
  }
  const singleCmdPrefix: string = `"${aotCompiler}" ` +
    `--aot-file="${appAot}" --compiler-target-triple=aarch64-unknown-linux-gnu `;
  let singleCmd: string = '';
  if (projectConfig.anBuildMode === AOT_FULL) {
    singleCmd = singleCmdPrefix + ` "${appAbc}"`;
  } else if (projectConfig.anBuildMode === AOT_PARTIAL) {
    const profile: string = projectConfig.apPath;
    if (!validateFilePathLength(profile, logger)) {
      faultHandler('ArkTS:ERROR GenerateAot failed. Invalid profile file path.');
    }
    if (!fs.existsSync(profile)) {
      faultHandler(`ArkTS:ERROR GenerateAot failed. Partial mode lost profile in "${profile}"`);
    }
    singleCmd = singleCmdPrefix + ` --enable-pgo-profiler=true --compiler-pgo-profiler-path="${profile}" "${appAbc}"`;
  } else {
    faultHandler(`ArkTS:ERROR GenerateAot failed. unknown anBuildMode: ${projectConfig.anBuildMode}`);
  }
  try {
    logger.debug(`generateAot cmd: ${singleCmd}`);
    mkdirsSync(projectConfig.anBuildOutPut);
    fs.closeSync(fs.openSync(appAot + '.an', 'w'));
    fs.closeSync(fs.openSync(appAot + '.ai', 'w'));
  } catch (e) {
    // Extract HostTool log information from hilog, which outputs to stdout.
    let outStream: Buffer = e.stdout;
    outStream.toString().split(os.EOL).forEach((stdLog: string) => {
      if (!stdLog.includes(hostToolKeyWords)) {
        return;
      }
      let logHeader: string = '';
      let logContent: string = '';
      [logHeader, logContent] = stdLog.split(hostToolKeyWords);
      if (!logHeader && !logContent) {
        return;
      }
      let logLevel: string = logHeader.trim().split(/\s+/)[logLevelIndex];
      if (logLevel === 'F' || logLevel === 'E') {
        logger.warn(`ArkTS:WARN ${logContent}`);
      }
    });
    logger.warn(`ArkTS:WARN ${e}`);
    logger.warn(`ArkTS:WARN Failed to generate aot file, the module may run with an interpreter`);
  }
}
