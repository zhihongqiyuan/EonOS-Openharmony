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

import path from 'path';

import {
  shouldWriteChangedList,
  writeFileSync,
  getHotReloadFiles,
  storedFileInfo,
  resourcesRawfile,
  differenceResourcesRawfile
} from '../../utils';
import {
  projectConfig,
  readAppResource,
  resources
} from '../../../main';
import {
  incrementWatchFile,
  hotReloadSupportFiles
} from '../../ets_checker';
import { ShouldEnableDebugLine } from '../ets_ui/rollup-plugin-ets-typescript';

export function watchChangeFiles() {
  function addFileToCache(this: any, key: string, id: string) {
    let modifiedFiles: string[] = [];
    if (!projectConfig.hotReload && this.cache && this.cache.has(key)) {
      modifiedFiles = this.cache.get(key);
      modifiedFiles.push(id);
    } else {
      modifiedFiles.push(id);
    }
    this.cache.set(key, modifiedFiles);
  }
  return {
    name: 'watchChangedFiles',
    watchChange(id: string, change: {event: 'create' | 'update' | 'delete'}): void {
      if (change.event === 'update') {
        addFileToCache.call(this, 'watchModifiedFiles', id);
      }
      if (['create', 'delete'].includes(change.event)) {
        addFileToCache.call(this, 'watchRemovedFiles', id);
      }
      if (path.resolve(id) === path.resolve(process.env.appResource) && process.env.compileMode === 'moduleJson') {
        storedFileInfo.resourceTableChanged = true;
        storedFileInfo.resourceList.clear();
        resources.app = {};
        readAppResource(process.env.appResource);
        if (process.env.rawFileResource) {
          resourcesRawfile(process.env.rawFileResource, storedFileInfo.resourcesArr);
          this.share.rawfilechanged = differenceResourcesRawfile(storedFileInfo.lastResourcesSet, storedFileInfo.resourcesArr);
        }
      }
      ShouldEnableDebugLine.enableDebugLine = false;
    },
    beforeBuild() {
      this.cache.set('watchChangedFilesCache', 'watchChangedFiles');
      const watchModifiedFiles: string[] = this.cache.get('watchModifiedFiles') || [];
      const watchRemovedFiles: string[] = this.cache.get('watchRemovedFiles') || [];
      if (!projectConfig.removeChangedFileListInSdk && shouldWriteChangedList(watchModifiedFiles, watchRemovedFiles)) {
        writeFileSync(projectConfig.changedFileList, JSON.stringify(
          getHotReloadFiles(watchModifiedFiles, watchRemovedFiles, hotReloadSupportFiles)));
      }
      incrementWatchFile(watchModifiedFiles, watchRemovedFiles);
      if (this.cache.get('lastWatchResourcesArr')) {
        storedFileInfo.lastResourcesSet = new Set([...this.cache.get('lastWatchResourcesArr')]);
      }
    }
  };
}
