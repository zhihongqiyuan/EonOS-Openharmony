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
import { 
	ESMODULE,
  THIS
} from '../../../lib/pre_define';

import {
  SDK_VERSION,
  SDK_VERSION_STAGE,
  BUNDLE_NAME_DEFAULT,
  ETS_LOADER_VERSION,
  OHPM,
  RELEASE,
  DEBUG,
  ENTRY_MODULE_NAME_DEFAULT,
  ENTRY_PACKAGE_NAME_DEFAULT,
  ENTRY_MODULE_VERSION_DEFAULT,
  MODULE_NAME_HASH_DEFAULT,
  RESOURCE_TABLE_HASH_DEFAULT,
  RUNTIME_OS_OPENHARMONY,
  DEVICE_TYPE,
  NODE_JS_PATH,
  PORT_DEFAULT,
  PACKAGE_DIR_DEFAULT,
  PACKAGE_JSON_DEFAULT,
  PAGES_JSON_FILE_NAME_DEFAULT,
  PANDA_MODE_DEFAULT,
  MIN_API_VERSION_DEAFAULT,
  STRICT_MODE_DEFAULT,
  DEVICE_TYPES_DEFAULT,
  PKG_JSON_FILE_HASH_DEFAULT,
  BUILD_ID_DEFAULT,
} from './common';
import { 
  ACE_PROFILE_PATH, 
  CACHE_PATH, 
  LOADER_PATH, 
  MAIN_PATH, 
  PREVIEW_CACHE_PATH, 
  PREVIEW_MAIN_PATH, 
  PROJECT_PATH_HASH_DEFAULT, 
  RES_PATH 
} from './pathConfig';

interface ConfigPermission {
	requestPermissions?: Array<{ name: string }>;
	definePermissions?: Array<{ name: string }>;
}

class ProjectConfig {
	deviceTypesMessage?: string;
	deviceTypes?: string[];
	externalApiPaths?: string[];
	syscapIntersectionSet?: Set<string>;
	syscapUnionSet?: Set<string>;
	permission?: ConfigPermission;
	requestPermissions?: string[];
	definePermissions?: string[];
	integratedHsp: boolean;
	resetBundleName: boolean;
	isCrossplatform: boolean;
	tsImportSendable?: boolean;
	xtsMode: boolean;
	hotReload?: object;
	testFrameworkPar?: object;
	targetName?: string;
	isOhosTest?: object;
	packageDir: string = PACKAGE_DIR_DEFAULT;
  packageJson: string = PACKAGE_JSON_DEFAULT;
	modulePathMap: object;
	enableDebugLine: boolean;
	byteCodeHar?: boolean;
	moduleName: string = ENTRY_MODULE_NAME_DEFAULT;
	pagesJsonFileName: string = PAGES_JSON_FILE_NAME_DEFAULT;
	ignoreWarning?: boolean;
	shouldEmitJs?: boolean;
	useArkoala: boolean;
  entryArrayForObf?: string[];
  cardObj?: object;
  manifestFilePath: string;
  hashProjectPath: string = PROJECT_PATH_HASH_DEFAULT;
  compilerTypes: string[];
  splitCommon: boolean;
  bundleType: string;
  optLazyForEach: boolean;
  hspResourcesMap: boolean;
  useTsHar: boolean;
  optTryCatchFunc: boolean;
  otherCompileFiles: object;
  intentEntry: string[];
  es2abcCompileTsInAotMode: boolean;
  es2abcCompileTsInNonAotMode: boolean;
  processTs: boolean;
  pandaMode: string = PANDA_MODE_DEFAULT;
  minAPIVersion: number = MIN_API_VERSION_DEAFAULT;
  anBuildOutPut?: string;
  anBuildMode?: object;
  nodeModulesPath?: object;
  minPlatformVersion?: number;
  changedFileList?: object;
  moduleType?: string;
  perf?: number;
  debugLine?: boolean;
  aotCompileMode?: string;
  buildGeneratedProfilePath?: string;
  apiVersion: number = SDK_VERSION;
  needCompleteSourcesMap: boolean = false;
  isFaMode: boolean = false;
  strictMode?: object;
  buildDir?: string;
  ohPackagePathMap?: object;
  branchElimination?: boolean;
  caseSensitiveCheck?: boolean;
  depName2RootPath?: Map<string, string>;
  useNativeResolver?: boolean;
  resolveComflictMode?: boolean;
  rawFileResource?: string;
  pkgNameToPkgBriefInfo?: object;
  projectModel?: object;
  pkgJsonFileHash: string = PKG_JSON_FILE_HASH_DEFAULT;
  allModulePaths?: string[];
  routerMap?: object;
  compileBlockPkg?: string[];
  overallProjectPaths?: string[];
  level?: object;
  buildId?: string;
  globalModulePaths?: string[];
  compileEnv?: string;

	// copied from ark_compiler_ut
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
  sdkInfo: string = `false:${SDK_VERSION}:${ETS_LOADER_VERSION}:${RELEASE}`;

  watchMode: string;
  isPreview: boolean;
  isLocalTest: boolean;
  buildMode: string = DEBUG;
  localPropertiesPath?: string;
  aceProfilePath: string;
  etsLoaderPath?: string;
  modulePath?: string;
  needCoverageInsert: boolean;
  projectTopDir?: string;
  apPath?: string;
  aceModuleJsonPath: string;
  appResource?: string;
  aceModuleRoot?: string;
  aceSuperVisualPath: string;
  aceBuildJson: string;
  cachePath: string;
  aceModuleBuild: string;
  patchAbcPath?: string;
  DynamicImportpath?: string;
  supportChunks: boolean;
  projectPath: string;
  resolveModulePaths?: Array<string>;
  compileHar: boolean;
  compileShared: boolean;
  moduleRootPath?: object;
  buildPath: string;
  rootPathSet?: Object;

  deviceType?: string;
  checkEntry?: string;
  Path?: string;
  note?: string;
  hapMode?: string;
  img2bin?: string;
  projectProfilePath?: string;
  logLevel?: string;
  stageRouterConfig?: object;
  port?: string;
  aceSoPath?: string;
  mockParams?: object;
  projectRootPath: string;
  pkgContextInfo?: object;
  useNormalizedOHMUrl: boolean = false;
  dependencyAliasMap?: Map<string, string>;
  byteCodeHarInfo?: object;
  entryObj?: object;
  harNameOhmMap: object;
  hspNameOhmMap: object;
  cardEntryObj?: object;
  widgetCompile?: boolean;
  arkRouterMap: Array<object>;
  declarationEntry: Array<string>;

  constructor() {
    this.watchMode = 'false';
    this.isPreview = false;
    this.isLocalTest = false;
    this.needCoverageInsert = false;
    this.supportChunks = true;
    this.compileHar = false;
    this.compileShared = false;
    this.xtsMode = false;
    this.splitCommon = false;
    this.isCrossplatform = false;
    this.enableDebugLine = false;
    this.bundleType = '';
    this.optLazyForEach = false;
    this.hspResourcesMap = true;
    this.useArkoala = false;
    this.resetBundleName = false;
    this.integratedHsp = false;
    this.useTsHar = false;
    this.optTryCatchFunc = true;
    this.es2abcCompileTsInAotMode = true;
    this.es2abcCompileTsInNonAotMode = false;
    this.processTs = true;

    this.harNameOhmMap = {};
    this.hspNameOhmMap = {};
    this.arkRouterMap = [];
    this.declarationEntry = [];
    this.compilerTypes = [];
    this.otherCompileFiles = {}
    this.intentEntry = [];
  }

  public setIgnoreWarning(ignoreWarning: boolean) {
    this.ignoreWarning = ignoreWarning;
  }

  public setPreview(isPreview: boolean) {
    this.isPreview = isPreview;
    this.watchMode = String(isPreview);
  }

  public setBuildMode(buildMode: string) {
    this.buildMode = buildMode;
  }

  public setCompilerVersion(version: number) {
    this.compileSdkVersion = version;
    this.compatibleSdkVersion = version;
  }

  public scan(projectRoot: string, testcase: string, pagePaths: string[] = []) {
    this.initPath(`${projectRoot}/${testcase}`, pagePaths);
  }

  public concat(other: typeof ProjectConfig | ArkProjectConfig) {
    const filteredEntries = Object.entries(other).reduce((acc, [key, value]) => {
      if (value !== undefined || this[key] === undefined) {
        acc.push([key, value]);
      }
      return acc;
    }, []);

    Object.assign(this, Object.fromEntries(filteredEntries));

  }

	private initPath(projectPath: string, pagePaths: string[] = []) {
		let mode = this.isPreview ? '.preview' : this.isLocalTest ? '.test' : 'build';

    this.projectPath = `${projectPath}/${this.entryModuleName}/src/main/ets`;
    this.buildPath = this.isPreview 
      ? `${projectPath}/${this.entryModuleName}/${mode}/${PREVIEW_MAIN_PATH}` 
      : `${projectPath}/${this.entryModuleName}/${mode}/${MAIN_PATH}`;
    this.aceModuleBuild = this.isPreview 
      ? `${projectPath}/${this.entryModuleName}/${mode}/${PREVIEW_MAIN_PATH}` 
      : `${projectPath}/${this.entryModuleName}/${mode}/${MAIN_PATH}`;
    this.manifestFilePath = `${projectPath}/${this.entryModuleName}/src/main/ets/manifest.json`;
    this.aceProfilePath = `${projectPath}/${this.entryModuleName}/${mode}/${ACE_PROFILE_PATH}`;
    this.aceModuleJsonPath = `${projectPath}/${this.entryModuleName}/${mode}/${RES_PATH}/module.json`;
    this.aceSuperVisualPath = `${projectPath}/${this.entryModuleName}/src/main/supervisual`;
    this.aceBuildJson = `${projectPath}/${this.entryModuleName}/${mode}/${LOADER_PATH}/loader.json`;
    this.cachePath = this.isPreview 
      ? `${projectPath}/${this.entryModuleName}/${mode}/${PREVIEW_CACHE_PATH}`
      : `${projectPath}/${this.entryModuleName}/${mode}/${CACHE_PATH}`;
    this.projectRootPath = `${projectPath}`;

    if (this.isPreview) {
      this.previewUniqueConfig(pagePaths);
    }
	}

  private previewUniqueConfig(pagePaths: string[] = []) {
    this.deviceType = DEVICE_TYPE;
    this.checkEntry = 'true';
    this.Path = NODE_JS_PATH;
    this.note = 'false';
    this.hapMode = 'false';
    this.img2bin = 'true';
    this.projectProfilePath = `${this.projectRootPath}/build-profile.json5`;
    this.logLevel = '3';
    this.stageRouterConfig = {
      'contents': [
        "{\"module\":{\"pages\":\"$profile:main_pages\",\"name\":\"entry\"}}",
        JSON.stringify({ 'src': pagePaths })
      ],
      'paths': [
        `${this.projectRootPath}/${this.entryModuleName}/.preview/${RES_PATH}/module.json`,
        `${this.projectRootPath}/${this.entryModuleName}/.preview/${RES_PATH}/resources/base/profile/main_pages.json`
      ]
    };
    this.port = PORT_DEFAULT;
    this.aceSoPath = `${this.projectRootPath}/${this.entryModuleName}/.preview/cache/nativeDependencies.txt`;
  }

  public mockCompileContextInfo(projectRoot: string, pagePaths: string[] = []) {
    this.entryObj = {
      'entryAbility/EntryAbility': `${projectRoot}/entry/src/main/ets/entryability/EntryAbility.ets`,
      'entryformability/EntryFormAbility': `${projectRoot}/entry/src/main/ets/entryformability/EntryFormAbility.ets`,
    };

    this.entryArrayForObf = [
      "./entryability/EntryAbility",
      "./entryformability/EntryFormAbility",
    ];

    pagePaths.forEach((mainPage: string) => {
      this.entryObj[mainPage] = `${projectRoot}/entry/src/main/ets/${mainPage}.ets`;
      this.entryArrayForObf.push(mainPage);
    })

    this.cardObj = {};
    this.cardEntryObj = {};
    this.projectRootPath = projectRoot;
  }

  public mockRollupShareBuild(projectRoot: string) {
    let mode = this.isPreview ? '.preview' : this.isLocalTest ? '.test' : 'build';

    this.moduleType = "entry";
    this.perf = 0;
    this.targetName = ".default";
    this.localPropertiesPath = `${projectRoot}/local.properties`;
    this.isOhosTest = false;
    this.etsLoaderPath = `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/openharmony/ets/build-tools/ets-loader`;
    this.modulePath = `${projectRoot}/${this.entryModuleName}`;
    this.testFrameworkPar = {};
    this.debugLine = false;
    this.projectTopDir = `${projectRoot}`;
    this.aotCompileMode = 'type';
    this.apPath = `${projectRoot}/${this.entryModuleName}/modules.ap`;
    this.externalApiPaths = [`/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/hms/ets`]
    this.buildGeneratedProfilePath = `${projectRoot}/${this.entryModuleName}/${mode}/default/generated/profile/default`;
    this.bundleType = 'app';
    this.strictMode = STRICT_MODE_DEFAULT;
    this.buildDir = `${projectRoot}/${this.entryModuleName}/${mode}`;
    this.deviceTypes = DEVICE_TYPES_DEFAULT;
    this.useNormalizedOHMUrl = true;
    this.pkgContextInfo = {
      '@ohos/hypium': {
        'packageName': '@ohos/hypium',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.19',
        'entryPath': 'index.js',
        'isSO': false,
        'dependencyAlias': ''
      },
      '@ohos/hamock': {
        'packageName': '@ohos/hamock',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.1-rc2',
        'entryPath': 'index.ets',
        'isSO': false,
        'dependencyAlias': ''
      },
      'entry': {
        'packageName': 'entry',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': 'src/main',
        'isSO': false,
        'dependencyAlias': ''
      }
    };
    this.ohPackagePathMap = {};
    this.dependencyAliasMap = new Map();
    this.permission = {};
    this.integratedHsp = false;
    this.branchElimination = false;
    this.caseSensitiveCheck = true;
    this.tsImportSendable = false;
    this.resolveComflictMode = true;
    this.depName2RootPath = new Map();
    this.rootPathSet = new Set([`${projectRoot}`]);
    this.useNativeResolver = true;
    this.shouldEmitJs = true;
    this.appResource = `${projectRoot}/${this.entryModuleName}/${mode}/${RES_PATH}/ResourceTable.txt`;
    this.rawFileResource = `${projectRoot}/${this.entryModuleName}/${mode}/${RES_PATH}/resources/rawfile`;
    this.aceModuleRoot = `${projectRoot}/${this.entryModuleName}/src/main/ets`;
    this.pkgNameToPkgBriefInfo = {
      'entry': {
        'pkgRoot': `${projectRoot}/${this.entryModuleName}`,
        'sourceRoots': [
          'src/main',
          `${mode}/default/generated/profile/default`
        ],
        'pkgName': 'entry'
      }
    };
    this.projectModel = {
      [`${projectRoot}/${this.entryModuleName}/src/main`]: {
        'moduleName': this.entryModuleName,
        'modulePkgPath': `${projectRoot}/${this.entryModuleName}`,
        'belongProjectPath': projectRoot
      },
      [`${projectRoot}/${this.entryModuleName}/${mode}/default/intermediates/src/ohosTest/ets`]: {
        'moduleName': 'entry_test',
        'modulePkgPath': `${projectRoot}/${this.entryModuleName}`,
        'belongProjectPath': projectRoot
      },
      [`${projectRoot}/${this.entryModuleName}/src/ohosTest`]: {
        'moduleName': 'entry_test',
        'modulePkgPath': `${projectRoot}/${this.entryModuleName}`,
        'belongProjectPath': projectRoot
      },
      [`${projectRoot}/${this.entryModuleName}`]: {
        'moduleName': this.entryModuleName,
        'modulePkgPath': `${projectRoot}/${this.entryModuleName}`,
        'belongProjectPath': projectRoot
      },
      [projectRoot]: {
        'moduleName': 'application',
        'modulePkgPath': projectRoot,
        'belongProjectPath': projectRoot
      }
    };
    this.allModulePaths = [`${projectRoot}/${this.entryModuleName}`];
    this.routerMap = {};
    this.compileBlockPkg = [];
    this.overallProjectPaths = [projectRoot];
    this.mockParams = {
      'decorator': '@MockSetup',
      'packageName': '@ohos/hamock',
      'etsSourceRootPath': 'ets/main/ets',
      'mockConfigPath': `${projectRoot}/${this.entryModuleName}/src/mock/mock-config.json5`,
      'mockConfigKey2ModuleInfo': {},
      'isMockConfigChanged': this.isPreview
    };
    this.level = {
      'level': 20000,
      'levelStr': 'INFO',
      'colour': 'green'
    };
    this.globalModulePaths = [
      `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/openharmony/ets/api`,
      `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/openharmony/ets/arkts`,
      `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/hms/ets/api`,
      `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/hms/ets/kits`,
      `/${this.runtimeOS}/Sdk/${this.compileSdkVersion}/openharmony/ets/api`
    ]
    this.compileEnv = [
      this.compileSdkVersion,
      this.compatibleSdkVersion,
      this.etsLoaderPath,
      this.isLocalTest,
      this.entryModuleName,
      `${this.cachePath}/compiler.cache`
    ].map((c) => c.toString()).join('#')
    this.resolveModulePaths = [
      `${projectRoot}/${this.entryModuleName}/src/main`,
      `${projectRoot}/${this.entryModuleName}/${mode}/default/generated/profile/default`,
      `${projectRoot}/${this.entryModuleName}/oh_modules`,
      `${projectRoot}/oh_modules`,
    ]

    if (!this.isPreview) {
      this.buildId = BUILD_ID_DEFAULT
    }
  }
}

class PartialUpdateConfig {
  partialUpdateMode: boolean = false;
  builderCheck: boolean = true;
  executeArkTSLinter: boolean = true;
  standardArkTSLinter: boolean = true;
  optimizeComponent: boolean = true;
  arkTSVersion: string = undefined;
  skipTscOhModuleCheck: boolean = false;
  skipArkTSStaticBlocksCheck: boolean = false;

  constructor() {
    this.partialUpdateMode = false;
    this.builderCheck = true;
    this.executeArkTSLinter = true;
    this.standardArkTSLinter = true;
    this.optimizeComponent = true;
    this.arkTSVersion = undefined;
    this.skipTscOhModuleCheck = false;
    this.skipArkTSStaticBlocksCheck = false;
  }

  public setPartialUpdateMode(newVal: boolean) {
    this.partialUpdateMode = newVal;
  }

  public mockDisableArkTSLinter() {
    this.executeArkTSLinter = false;
    this.standardArkTSLinter = false;
  }
}

interface ArkProjectConfig {
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
  es2abcCompileTsInAotMode: boolean,
  es2abcCompileTsInNonAotMode: boolean,
  minAPIVersion: number,
  pagesJsonFileName: string,
}

export {
	ArkProjectConfig,
	ProjectConfig,
  PartialUpdateConfig
}