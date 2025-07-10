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
import fs from 'fs';
import path from 'path';

const {
  projectConfig,
  loadEntryObj,
  abilityConfig,
  initBuildInfo,
  readWorkerFile,
  loadWorker,
  readPatchConfig,
  loadModuleInfo
} = require('../../../main.js');

export let workerFile = null;
export function getEntryObj() {
  loadEntryObj(projectConfig);
  initBuildInfo();
  readPatchConfig();
  loadModuleInfo(projectConfig);
  workerFile = readWorkerFile();
  if (!projectConfig.isPreview) {
    loadWorker(projectConfig, workerFile);
  }
  projectConfig.entryObj = Object.keys(projectConfig.entryObj).reduce((newEntry, key) => {
    const newKey: string = key.replace(/^\.\//, '');
    newEntry[newKey] = projectConfig.entryObj[key].replace('?entry', '');
    return newEntry;
  }, {});
}

export function setCopyPluginConfig(projectConfig: any, appResource: string) {
  const copyPluginPattrens: object[] = [];
  const BUILD_SHARE_PATH: string = '../share';
  copyPluginPattrens.push({
    src: [
      path.resolve(projectConfig.projectPath, '**/*'),
      '!**/*.ets',
      '!**/*.ts',
      '!**/*.js',
      `!${projectConfig.buildPath}`
    ],
    dest: projectConfig.buildPath
  });
  const sharePath: string = path.resolve(projectConfig.projectPath, BUILD_SHARE_PATH);
  if (fs.existsSync(sharePath)) {
    copyPluginPattrens.push({
      src: [
        path.resolve(projectConfig.projectPath, BUILD_SHARE_PATH, '**/*'),
        '!**/*.ets',
        '!**/*.ts',
        '!**/*.js'
      ],
      dest: path.resolve(projectConfig.buildPath, BUILD_SHARE_PATH),
    });
  }
  if (abilityConfig.abilityType === 'page') {
    if (fs.existsSync(projectConfig.manifestFilePath)) {
      copyPluginPattrens.push({
        src: projectConfig.manifestFilePath,
        dest: projectConfig.buildPath
      });
    } else if (fs.existsSync(projectConfig.aceConfigPath)) {
      copyPluginPattrens.push({
        src: projectConfig.aceConfigPath,
        dest: projectConfig.buildPath
      });
    }
  }
  if (appResource && fs.existsSync(appResource) && !projectConfig.xtsMode &&
    projectConfig.isPreview) {
    copyPluginPattrens.push({
      src: path.resolve(__dirname, appResource),
      dest: path.resolve(__dirname, projectConfig.cachePath)
    });
  }
  return copyPluginPattrens;
}
