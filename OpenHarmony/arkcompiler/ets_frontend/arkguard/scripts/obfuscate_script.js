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

const fs = require('fs');
const path = require('path');
const { exec } = require('child_process');
import { Extension } from '../src/common/type';

function obfuscateDirs(obfConfig, obfDir) {
  const command = `node --loader=ts-node/esm src/cli/SecHarmony.ts ${obfDir} --config-path ${obfConfig} --test-type grammar`;
  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`Error executing command: ${error.message}`);
      return;
    }
    if (stdout) {
      console.log('Debug info: ', stdout);
    }
  });
}

function traverseDirs(rootDirPath, configPath) {
  const currentEntries = fs.readdirSync(rootDirPath);
  let configFile = 'obfConfig.json';
  if (currentEntries.includes(configFile)) {
    configPath = rootDirPath;
  }

  const hasJsOrTsFiles = currentEntries.some(entry => {
    return entry.endsWith(Extension.TS) || entry.endsWith(Extension.JS) || entry.endsWith(Extension.DETS);
  });

  if (hasJsOrTsFiles) {
    obfuscateDirs(path.join(configPath, configFile), rootDirPath);
    return;
  }

  for (const currentEntry of currentEntries) {
    const currentPath = path.join(rootDirPath, currentEntry);
    if (fs.statSync(currentPath).isDirectory()) {
      traverseDirs(currentPath, configPath);
    }
  }
}

function run() {
  const testCasesRootDir = path.join(__dirname, '../test/grammar');
  traverseDirs(testCasesRootDir, testCasesRootDir);
}

function main() {
  run();
}

main();