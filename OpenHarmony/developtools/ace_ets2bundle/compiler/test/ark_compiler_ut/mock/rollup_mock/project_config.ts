/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use rollupObject file except in compliance with the License.
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

import { 
  DEFAULT_PROJECT,
  PROJECT_ROOT
} from "./path_config";
import {
  SDK_VERSION,
  BUNDLE_NAME_DEFAULT,
  ETS_LOADER_VERSION,
  ENTRY_MODULE_NAME_DEFAULT,
  ENTRY_PACKAGE_NAME_DEFAULT,
  RUNTIME_OS_OPENHARMONY,
  MODULE_NAME_HASH_DEFAULT,
  RESOURCE_TABLE_HASH_DEFAULT,
  DEVICE_TYPE,
  NODE_JS_PATH,
  PORT_DEFAULT,
  ENTRY_MODULE_VERSION_DEFAULT,
  SDK_VERSION_STAGE
} from "./common";
import {
  ESMODULE,
  OHPM,
  RELEASE
} from "../../../../lib/fast_build/ark_compiler/common/ark_define";
import EntryAbility from "../../testdata/expect/expect_EntryAbility";

interface IArkProjectConfig {
  projectRootPath: string,
  modulePathMap: object,
  isOhosTest: object,
  oldMapFilePath?: object,
  processTs: boolean,
  pandaMode: string,
  anBuildOutPut?: string,
  anBuildMode?: object,
  apPath?: object,
  nodeModulesPath?: object,
  harNameOhmMap: object,
  minPlatformVersion: number,
  moduleName: string,
  bundleName: string,
  hotReload: object,
  patchAbcPath: object,
  changedFileList: object,
  compileMode: string,
  processTs: boolean
}

class ProjectConfig {
  compileMode: string = ESMODULE;
  packageManagerType: string = OHPM;
  compileSdkVersion: number = SDK_VERSION;
  compatibleSdkVersion: number = SDK_VERSION;
  compatibleSdkVersionStage: string = SDK_VERSION_STAGE;
  bundleName: string = BUNDLE_NAME_DEFAULT;
  etsLoaderVersion: string = ETS_LOADER_VERSION;
  etsLoaderReleaseType: string = RELEASE;
  entryModuleName: string = ENTRY_MODULE_NAME_DEFAULT;
  entryPackageName: string = ENTRY_PACKAGE_NAME_DEFAULT;
  entryModuleVersion: string = ENTRY_MODULE_VERSION_DEFAULT;
  allModuleNameHash: string = MODULE_NAME_HASH_DEFAULT;
  resourceTableHash: string = RESOURCE_TABLE_HASH_DEFAULT;
  runtimeOS: string = RUNTIME_OS_OPENHARMONY;
  sdkInfo: string = `true:${SDK_VERSION}:${ETS_LOADER_VERSION}:${RELEASE}`;

  watchMode: string;
  isPreview: boolean;
  isLocalTest: boolean;
  buildMode: string;
  localPropertiesPath: string;
  aceProfilePath: string;
  etsLoaderPath: string;
  modulePath: string;
  needCoverageInsert: boolean;
  projectTopDir: string;
  apPath: string;
  aceModuleJsonPath: string;
  appResource: string;
  aceModuleRoot: string;
  aceSuperVisualPath: string;
  aceBuildJson: string;
  cachePath: string;
  aceModuleBuild: string;
  patchAbcPath: string;
  DynamicImportpath: string;
  supportChunks: boolean;
  projectPath: string;
  resolveModulePaths: Array<string>;
  compileHar: boolean;
  compileShared: boolean;
  moduleRootPath: object;
  buildPath: string;
  rootPathSet: Object;

  deviceType?: string;
  checkEntry?: string;
  Path?: string;
  note?: string;
  hapMode?: string;
  img2bin?: string;
  projectProfilePath?: string;
  logLevel?: string;
  stageRouterConfig?: Array<object>;
  port?: string;
  aceSoPath?: string;
  mockParams?: object;
  projectRootPath: string;
  pkgContextInfo: object;
  useNormalizedOHMUrl: boolean = false;
  dependencyAliasMap: Map<string, string>;
  byteCodeHarInfo: object;
  entryObj: object;
  harNameOhmMap: object;
  hspNameOhmMap: object;
  cardEntryObj: object;
  widgetCompile: boolean;
  arkRouterMap: Array<object>;
  declarationEntry: Array<string>;

  constructor(buildMode: string) {
    this.watchMode = 'false';
    this.isPreview = false;
    this.isLocalTest = false;
    this.buildMode = buildMode;
    this.needCoverageInsert = false;
    this.supportChunks = true;
    this.compileHar = false;
    this.compileShared = false;
    this.harNameOhmMap = {};
    this.hspNameOhmMap = {};
    this.arkRouterMap = [];
    this.declarationEntry = [];
  }

  public scan(testcase: string) {
    this.initPath(`${PROJECT_ROOT}/${testcase}`);
  }

  public setPreview(isPreview: boolean) {
    this.isPreview = isPreview;
    this.watchMode = String(isPreview);
  }

  public setCompilerVersion(version: number) {
    this.compileSdkVersion = version;
    this.compatibleSdkVersion = version;
  }

  public setMockParams(params: object) {
    this.mockParams = params;
  }

  public mockCompileContextInfo() {
    this.entryObj = {
      'entryAbility/EntryAbility': `${PROJECT_ROOT}/entry/src/main/ets/entryability/EntryAbility.ets`,
      'page/Intex': `${PROJECT_ROOT}/entry/src/main/ets/page/Index.ets`,
      'entryformability/EntryFormAbility': `${PROJECT_ROOT}/entry/src/main/ets/entryformability/EntryFormAbility.ets`,
      'ets/widget/page/WidgetCard': `${PROJECT_ROOT}/entry/src/main/ets/widget/page/WidgetCard.ets`
    }
    this.cardEntryObj = {
      'entryformability/EntryFormAbility': `${PROJECT_ROOT}/entry/src/main/ets/entryformability/EntryFormAbility.ets`,
      'ets/widget/page/WidgetCard': `${PROJECT_ROOT}/entry/src/main/ets/widget/page/WidgetCard.ets`
    }
    this.projectRootPath = PROJECT_ROOT;
    this.pkgContextInfo = {
      'entry': {
        'packageName': 'entry',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      },
      'har': {
        'packageName': 'har',
        'bundleName': '',
        'moduleName': 'har',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      },
      'hsp': {
        'packageName': 'hsp',
        'bundleName': '',
        'moduleName': 'hsp',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      },
    };
    this.harNameOhmMap = {
      'hspAlias': '@normalized:N&&&hsp/Index&1.0.0'
    }
    this.dependencyAliasMap = new Map([
      ['hspAlias', 'hsp']
    ]);
  }

  public mockBytecodeHarInfo() {
    this.byteCodeHarInfo = {
      "har": {
        "abcPath": "",
        "sourceMapsPath": `${PROJECT_ROOT}/${DEFAULT_PROJECT}/bytecodeHarSourceMap.json`,
      }
    }
  }

  private initPath(proPath: string) {
    // build and preview
    let mode = this.isPreview ? 'preview' : this.isLocalTest ? '.test' : 'build';
    this.localPropertiesPath = `${proPath}/local.properties`;
    this.aceProfilePath = `${proPath}/${this.entryModuleName}/${mode}/res/default/resources/base/profile`;
    this.etsLoaderPath = `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/ets/build-tools/app`;
    this.modulePath = `${proPath}/${this.entryModuleName}`;
    this.projectTopDir = `${proPath}`;
    this.apPath = '';
    this.aceModuleJsonPath = `${proPath}/${this.entryModuleName}/${mode}/module.json`;
    this.appResource = `${proPath}/${this.entryModuleName}/${mode}/res/default/ResourceTable.txt`;
    this.aceModuleRoot = `${proPath}/${this.entryModuleName}/src`;
    this.aceSuperVisualPath = `${proPath}/${this.entryModuleName}/src/main/supervisual`;
    this.aceBuildJson = `${proPath}/${this.entryModuleName}/${mode}/loader.json`;
    this.cachePath = `${proPath}/${this.entryModuleName}/${mode}`;
    this.aceModuleBuild = `${proPath}/${this.entryModuleName}/${mode}`;
    this.projectPath = `${proPath}/${this.entryModuleName}/src`;
    this.moduleRootPath = undefined;
    this.buildPath = `${proPath}/${this.entryModuleName}/${mode}`;
    this.patchAbcPath = `${proPath}/${this.entryModuleName}/${mode}/hotReload`;
    this.DynamicImportpath = `${this.modulePath}/DynamicImport.ets`;
    this.projectRootPath = `${proPath}`;

    if (this.isPreview) {
      this.previewUniqueConfig();
    }
  }

  private previewUniqueConfig() {
    this.deviceType = DEVICE_TYPE;
    this.checkEntry = 'true';
    this.Path = NODE_JS_PATH;
    this.note = 'false';
    this.hapMode = 'false';
    this.img2bin = 'true';
    this.projectProfilePath = `${this.projectTopDir}/build-profile.json5`;
    this.logLevel = '3';
    this.stageRouterConfig = [];
    this.port = PORT_DEFAULT;
    this.aceSoPath = `${this.projectTopDir}/entry/preview/cache/nativeDependencies.txt`;
  }
}

export { ProjectConfig, IArkProjectConfig }
