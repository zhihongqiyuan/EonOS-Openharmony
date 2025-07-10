/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import path from "path";
import fs from "fs";

import {
	ModuleInfo
} from './module';
import {
	Share
} from './share';
import {
	Cache
} from './cache';
import { 
	ARK_COMPILER_META_INFO, 
	BUNDLE_NAME_DEFAULT, 
	DEBUG, 
	ETS_LOADER_VERSION, 
	IS_CACHE_INVALID, 
	MIN_API_VERSION_DEAFAULT, 
	MODULE_NAME_HASH_DEFAULT, 
	PAGES_JSON_FILE_NAME_DEFAULT, 
	PANDA_MODE_DEFAULT, 
	RELEASE, 
	RUNTIME_OS_OPENHARMONY, 
	SDK_VERSION, 
	SDK_VERSION_STAGE 
} from './common';
import { 
	AN_BUILD_OUTPUT_PATH, 
	MODULE_ID_ROLLUP_PLACEHOLDER, 
	NODE_MODULES_PATH 
} from './pathConfig';
import { 
	ArkProjectConfig
} from './projectConfig';
import { 
	scanFiles 
} from './utils';

class RollUpPluginMock {
  cache: Cache;
  meta: object = { rollupVersion: '3.10.0', watchMode: false };
  moduleIds: object;
  share: Share;
  moduleInfos: Array<ModuleInfo>;

  private isPreview: boolean = false;

  constructor() {
    this.cache = new Cache();
  }

  public preConfig(buildMode: string = DEBUG) {
    this.share = new Share();
		this.share.projectConfig.setBuildMode(buildMode);
  }

  public build(projectRoot: string, testcase: string, pagePaths: string[] = [], buildMode: string = DEBUG) {
    this.isPreview = false;
    this.share = new Share();
		this.share.projectConfig.setBuildMode(buildMode);

    this.share.projectConfig.setPreview(this.isPreview);
    this.meta.watchMode = this.isPreview;

    this.doBuild(projectRoot, testcase, pagePaths);
  }

  public chooseTestData(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    if (!this.share) {
      throw new Error('Call build API first.');
    }
    this.doBuild(projectRoot, testcase, pagePaths);
  }

  public preview(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    this.isPreview = true;
    this.share = new Share();

    this.share.projectConfig.setPreview(this.isPreview);
    this.meta.watchMode = this.isPreview;

    this.doBuild(projectRoot, testcase, pagePaths);
  }

  public coldReload(projectRoot: string, testcase: string, pagePaths: string[] = [], buildMode: string = DEBUG) {
    this.isPreview = false;
    this.share = new Share();
		this.share.projectConfig.setBuildMode(buildMode)

    this.share.projectConfig.setPreview(this.isPreview);
    this.meta.watchMode = false;

    this.doBuild(projectRoot, testcase, pagePaths);
  }

  public hotReload(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    this.isPreview = false;
    this.share = new Share();

    this.share.projectConfig.setPreview(this.isPreview);
    this.meta.watchMode = this.isPreview;

    this.doBuild(projectRoot, testcase, pagePaths);
  }

  public useNormalizedOHMUrl() {
    this.share.projectConfig.useNormalizedOHMUrl = true;
  }

  public mockCompileContextInfo(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    this.share.projectConfig.mockCompileContextInfo(`${projectRoot}/${testcase}`, pagePaths);
    let entryObj: object = this.share.projectConfig.entryObj;
    if (!!this.share.projectConfig.widgetCompile) {
      entryObj = this.share.projectConfig.cardEntryObj
    }
    for(let entry in entryObj) {
      let filePath: string = entryObj[entry];
      let moduleInfo: ModuleInfo = new ModuleInfo(filePath, 'entry', `${projectRoot}/${testcase}/entry`);
      moduleInfo.meta.pkgName = 'entry';
      this.moduleInfos.push(moduleInfo);
    }
  }

  private doBuild(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    this.share.scan(projectRoot, testcase, pagePaths);
    this.load(pagePaths);

    // mock ets-loader build start
    this.share.arkProjectConfig = RollUpPluginMock.mockArkProjectConfig(projectRoot, testcase, this.isPreview);
    this.share.projectConfig.mockRollupShareBuild(`${projectRoot}/${testcase}`);
    this.mockCheckArkCompilerCacheInfo();
  }

  static mockArkProjectConfig(projectRoot: string, testcase: string, isPreview: boolean): ArkProjectConfig {
    const mode = isPreview ? '.preview' : 'build';
    const projectRootDir = projectRoot;
    const entryName = testcase;

    return {
      moduleName: `${entryName}`,
      bundleName: BUNDLE_NAME_DEFAULT,
      minAPIVersion: MIN_API_VERSION_DEAFAULT,
      pagesJsonFileName: PAGES_JSON_FILE_NAME_DEFAULT,
      projectRootPath: projectRootDir,
      modulePathMap: { entry: `${projectRootDir}/${entryName}` },
      es2abcCompileTsInAotMode: true,
      es2abcCompileTsInNonAotMode: false,
      processTs: true,
      pandaMode: PANDA_MODE_DEFAULT,
      anBuildOutPut: `${projectRootDir}/${entryName}/${mode}/${AN_BUILD_OUTPUT_PATH}`,
      anBuildMode: 'type',
      nodeModulesPath: `${projectRootDir}/${entryName}/${mode}/${NODE_MODULES_PATH}`,
      harNameOhmMap: {},
    }
  }

  private mockCheckArkCompilerCacheInfo(): void {
    const metaInfos = [
      SDK_VERSION,
      SDK_VERSION,
      SDK_VERSION_STAGE,
      RUNTIME_OS_OPENHARMONY,
      `/OpenHarmony/Sdk/${SDK_VERSION}/ets/build-tools/app`,
      ETS_LOADER_VERSION,
      RELEASE,
      BUNDLE_NAME_DEFAULT,
      MODULE_NAME_HASH_DEFAULT,
      'null_aotCompileMode',
      'null_apPath'
    ]
    const metaInfo = metaInfos.join(':');
    this.cache.set(IS_CACHE_INVALID, true);
    this.cache.set(ARK_COMPILER_META_INFO, metaInfo);
  }

  public addWatchFile() { }

  public async(func: Function) {
    if (func) {
      func();
    }
  }

  public block() { }

  public emitFile() { }

  public error() { }

  public getFileName() { }

  public getModuleIds(): IterableIterator<string> {
    return this.share.allFiles ? this.share.allFiles.values() : undefined;
  }

  public getModuleInfo(id: string): ModuleInfo | undefined {
    for (let i = 0; i < this.moduleInfos.length - 1; i++) {
      return this.moduleInfos.find(item => item.id === id);
    }
  }

  public getWatchFiles() { }

  public load(pagePaths: string[] = []) {
    // load project files list
    this.share.allFiles = new Set<string>();
    if (fs.existsSync(this.share.projectConfig.projectPath)) {
      scanFiles(this.share.projectConfig.projectPath, this.share.allFiles);
    } else {
      const tsFilePath = path.join(this.share.projectConfig.projectPath, '/entryability/EntryAbility.ts');
      const jsFilePath = path.join(this.share.projectConfig.projectPath, '/entryability/EntryAbility.js');
      // const etsFilePath = path.join(this.share.projectConfig.projectPath, '/pages/Index.ets');
      this.share.allFiles.add(tsFilePath);
      this.share.allFiles.add(jsFilePath);

      pagePaths.forEach((mainPage: string) => {
        const etsFilePath = path.join(this.share.projectConfig.projectPath, `/${mainPage}.ets`);
        this.share.allFiles.add(etsFilePath);
      })

      // this.share.allFiles.add(etsFilePath);
    }
    this.share.allFiles.add(MODULE_ID_ROLLUP_PLACEHOLDER);

    // load all files module info
    const allFiles = Array.from(this.share.allFiles);
    this.moduleInfos = new Array<ModuleInfo>();
    for (let i = 0; i < allFiles.length - 1; i++) {
      this.moduleInfos.push(new ModuleInfo(allFiles[i],
        this.share.projectConfig.entryModuleName,
        this.share.projectConfig.modulePath));
    }
  }

  public parse() { }

  public resolve() { }

  public setAssetSource() { }

  public signal() { }

  public warn() { }

  public clearCache() {
    this.cache.set(IS_CACHE_INVALID, false);
    this.cache.set(ARK_COMPILER_META_INFO, undefined);
  }
}

export {
	RollUpPluginMock
}