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

import { expect } from 'chai';
import mocha from 'mocha';
import sinon from 'sinon';

import {
  getOhmUrlByFilepath,
  getOhmUrlByExternalPackage,
  getOhmUrlBySystemApiOrLibRequest,
  getNormalizedOhmUrlByFilepath,
  getNormalizedOhmUrlByAliasName,
  getNormalizedOhmUrlByModuleRequest
} from '../../../../lib/ark_utils';
import { PACKAGES } from '../../../../lib/pre_define';
import projectConfig from '../../utils/processProjectConfig';
import { projectConfig as mainProjectConfig } from '../../../../main';
import RollUpPluginMock from '../../mock/rollup_mock/rollup_plugin_mock';
import { GEN_ABC_PLUGIN_NAME } from '../../../../lib/fast_build/ark_compiler/common/ark_define';
import { ModuleSourceFile } from '../../../../lib/fast_build/ark_compiler/module/module_source_file';
import {
  ArkTSErrorDescription,
  ArkTSInternalErrorDescription,
  ErrorCode
} from '../../../../lib/fast_build/ark_compiler/error_code';
import {
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../../../lib/fast_build/ark_compiler/logger';

const PRVIEW_MOCK_CONFIG : Object = {
  // system api mock
  "@ohos.bluetooth": {
    "source": "src/main/mock/ohos/bluetooth.mock.ts"
  },
  // local function mock
  "./src/main/ets/calc": {
    "source": "src/main/mock/module/calc.mock.ts"
  },
  // ohpm dependency mock
  "lib": {
    "source": "src/main/mock/module/bigInt.mock.ts"
  },
  // native mock
  "libentry.so": {
    "source": "src/main/mock/native/libentry.mock.ts"
  }
}

const MOCK_CONFIG_FILEPATH = {
  'lib': `${projectConfig.projectRootPath}/oh_modules/lib/dist/index.js`,
  './src/main/ets/calc': `${projectConfig.projectRootPath}/entry/src/main/ets/calc.ets`,
}

mocha.describe('generate ohmUrl', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('nested src main ets|js in filePath', function () {
    const filePath: string = `${projectConfig.projectRootPath}/entry/src/main/ets/feature/src/main/js/`
      + `subfeature/src/main/ets/pages/test.ts`;
    const moduleName: string = 'entry';
    const moduleNamespace: string = 'library';
    let ohmUrl_1 = getOhmUrlByFilepath(filePath, projectConfig, undefined, moduleName);
    let ohmUrl_2 = getOhmUrlByFilepath(filePath, projectConfig, undefined, moduleNamespace);
    let expected_1 = 'UtTestApplication/entry/ets/feature/src/main/js/subfeature/src/main/ets/pages/test';
    let expected_2 = 'UtTestApplication/entry@library/ets/feature/src/main/js/subfeature/src/main/ets/pages/test';
    expect(ohmUrl_1 == expected_1).to.be.true;
    expect(ohmUrl_2 == expected_2).to.be.true;
  });

  mocha.it('nested src ohosTest ets|js in filePath', function () {
    const filePath: string = `${projectConfig.projectRootPath}/entry/src/ohosTest/ets/feature/src/main/js/`
      + `subfeature/src/main/ets/pages/test.ts`;
    const moduleName: string = 'entry';
    const moduleNamespace: string = 'library';
    let ohmUrl_1 = getOhmUrlByFilepath(filePath, projectConfig, undefined, moduleName);
    let ohmUrl_2 = getOhmUrlByFilepath(filePath, projectConfig, undefined, moduleNamespace);
    let expected_1 = 'UtTestApplication/entry/ets/feature/src/main/js/subfeature/src/main/ets/pages/test';
    let expected_2 = 'UtTestApplication/entry@library/ets/feature/src/main/js/subfeature/src/main/ets/pages/test';
    expect(ohmUrl_1 == expected_1).to.be.true;
    expect(ohmUrl_2 == expected_2).to.be.true;
  });

  mocha.it('system builtins & app builtins', function () {
    mainProjectConfig.bundleName = 'UtTestApplication';
    mainProjectConfig.moduleName = 'entry';
    const systemModuleRequest: string = '@system.app';
    const ohosModuleRequest: string = '@ohos.hilog';
    const appSoModuleRequest: string = 'libapplication.so';
    const systemOhmUrl: string = getOhmUrlBySystemApiOrLibRequest(systemModuleRequest);
    const ohosOhmUrl: string = getOhmUrlBySystemApiOrLibRequest(ohosModuleRequest);
    const appOhmUrl: string = getOhmUrlBySystemApiOrLibRequest(appSoModuleRequest);
    const expectedSystemOhmUrl: string = '@native:system.app';
    const expectedOhosOhmUrl: string = '@ohos:hilog';
    const expectedappOhmUrl: string = '@app:UtTestApplication/entry/application';
    expect(systemOhmUrl == expectedSystemOhmUrl).to.be.true;
    expect(ohosOhmUrl == expectedOhosOhmUrl).to.be.true;
    expect(appOhmUrl == expectedappOhmUrl).to.be.true;
  });

  mocha.it('shared library', function () {
    this.rollup.build();
    const sharedLibraryPackageName: string = "@ohos/sharedLibrary";
    const sharedLibraryPackageNameSlashes: string = "@ohos/library///";
    const sharedLibraryPage: string = "@ohos/sharedLibrary/src/main/ets/pages/page1";
    const errorSharedLibrary: string = "@ohos/staticLibrary";
    const sharedLibraryModuleRequest = "@ohos/library////\\\\/";
    const sharedLibraryModuleRequestByPath = "@ohos/library/src/main/ets////";
    const sharedLibraryPackageNameOhmUrl: string = getOhmUrlByExternalPackage(sharedLibraryPackageName, projectConfig, this.rollup);
    const sharedLibraryPackageNameSlashesOhmUrl: string = getOhmUrlByExternalPackage(sharedLibraryPackageNameSlashes, projectConfig, this.rollup, ModuleSourceFile.logger, true);
    const sharedLibraryPageOhmUrl: string = getOhmUrlByExternalPackage(sharedLibraryPage, projectConfig, this.rollup);
    const errorSharedLibraryOhmUrl = getOhmUrlByExternalPackage(errorSharedLibrary, projectConfig, this.rollup);
    const sharedLibraryModuleRequestOhmUrl = getOhmUrlByExternalPackage(sharedLibraryModuleRequest, projectConfig, this.rollup, ModuleSourceFile.logger, true);
    const sharedLibraryModuleRequestByPathOhmUrl = getOhmUrlByExternalPackage(sharedLibraryModuleRequestByPath, projectConfig, this.rollup, ModuleSourceFile.logger, true);
    const expectedSharedLibraryOhmUrl: string = "@bundle:UtTestApplication/sharedLibrary/ets/index";
    const expectedSharedLibrarySlashesOhmUrl: string = "@normalized:N&&&@ohos/library/Index&1.0.0";
    const expectedSharedLibraryPageOhmUrl: string = "@bundle:UtTestApplication/sharedLibrary/ets/pages/page1";
    const expectedErrorSharedLibraryOhmUrl = undefined;
    const expectsharedLibraryModuleRequestOhmUrl = "@normalized:N&&&@ohos/library/Index&1.0.0";
    const expectsharedLibraryModuleRequestByPathOhmUrl = "@normalized:N&&&@ohos/library/src/main/ets/Index&1.0.0";
    expect(sharedLibraryPackageNameOhmUrl == expectedSharedLibraryOhmUrl).to.be.true;
    expect(sharedLibraryPackageNameSlashesOhmUrl == expectedSharedLibrarySlashesOhmUrl).to.be.true;
    expect(sharedLibraryPageOhmUrl == expectedSharedLibraryPageOhmUrl).to.be.true;
    expect(errorSharedLibraryOhmUrl == expectedErrorSharedLibraryOhmUrl).to.be.true;
    expect(sharedLibraryModuleRequestOhmUrl == expectsharedLibraryModuleRequestOhmUrl).to.be.true;
    expect(sharedLibraryModuleRequestByPathOhmUrl == expectsharedLibraryModuleRequestByPathOhmUrl).to.be.true;

  });

  mocha.it('project module', function () {
    const filePath: string = `${projectConfig.projectRootPath}/entry/src/main/ets/pages/test.ts`;
    const harFilePath = `${projectConfig.projectRootPath}/library/src/main/ets/pages/test.ts`;
    const moduleName: string = 'entry';
    const moduleNamespace: string = 'library';
    const ohmUrl = getOhmUrlByFilepath(filePath, projectConfig, undefined, moduleName);
    const harOhmUrl = getOhmUrlByFilepath(harFilePath, projectConfig, undefined, moduleNamespace);
    const expected = 'UtTestApplication/entry/ets/pages/test';
    const harOhmUrlExpected = 'UtTestApplication/entry@library/ets/pages/test';
    expect(ohmUrl == expected).to.be.true;
    expect(harOhmUrl == harOhmUrlExpected).to.be.true;
  });

  mocha.it('thirdParty module', function () {
    const moduleLevelPkgPath = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const projectLevelPkgPath = `${projectConfig.projectRootPath}/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const moduleLevelPkgOhmUrl = getOhmUrlByFilepath(moduleLevelPkgPath, projectConfig, undefined, undefined);
    const projectLevelPkgOhmUrl = getOhmUrlByFilepath(projectLevelPkgPath, projectConfig, undefined, undefined);
    const moduleLevelPkgOhmUrlExpected = `${PACKAGES}@${moduleName}/json5/dist/index`;
    const projectLevelPkgOhmUrlExpected = `${PACKAGES}/json5/dist/index`;
    expect(moduleLevelPkgOhmUrl == moduleLevelPkgOhmUrlExpected).to.be.true;
    expect(projectLevelPkgOhmUrl == projectLevelPkgOhmUrlExpected).to.be.true;
  });

  mocha.it('static library entry', function () {
    const staticLibraryEntry = `${projectConfig.projectRootPath}/library/index.ets`;
    const moduleNamespace: string = 'library';
    const staticLibraryEntryOhmUrl =
      getOhmUrlByFilepath(staticLibraryEntry, projectConfig, undefined, moduleNamespace);
    const staticLibraryEntryOhmUrlExpected = 'UtTestApplication/entry@library/index';
    expect(staticLibraryEntryOhmUrl == staticLibraryEntryOhmUrlExpected).to.be.true;
  });

  mocha.it('ohosTest module', function () {
    const ohosTestfilePath = `${projectConfig.projectRootPath}/entry/src/ohosTest/ets/pages/test.ts`;
    const moduleName: string = 'entry';
    const ohmUrl = getOhmUrlByFilepath(ohosTestfilePath, projectConfig, undefined, moduleName);
    const expected = 'UtTestApplication/entry/ets/pages/test';
    expect(ohmUrl == expected).to.be.true;
  });

  mocha.it('the error message of processPackageDir', function () {
    this.rollup.build();
    projectConfig.modulePathMap = {};
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. ' +
      'Failed to get a resolved OhmUrl for "/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    getOhmUrlByFilepath(filePath, projectConfig, logger, moduleName, importerFile);
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('the error message of processPackageDir without getHvigorConsoleLogger', function () {
    this.rollup.build();
    projectConfig.modulePathMap = {};
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. ' +
      'Failed to get a resolved OhmUrl for "/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.logger, 'error');
    getOhmUrlByFilepath(filePath, projectConfig, logger, moduleName, importerFile);
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('the error message of processPackageDir(packageDir is invalid value)', function () {
    this.rollup.build();
    projectConfig.packageDir = undefined;
    projectConfig.modulePathMap = {};
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. Failed to get a resolved OhmUrl for ' +
      '"/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    getOhmUrlByFilepath(filePath, projectConfig, logger, moduleName, importerFile);
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('the error message of processPackageDir(packageDir is invalid value) ' +
    'without getHvigorConsoleLogger', function () {
    this.rollup.build();
    projectConfig.packageDir = undefined;
    projectConfig.modulePathMap = {};
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. Failed to get a resolved OhmUrl for ' +
      '"/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.logger, 'error');
    getOhmUrlByFilepath(filePath, projectConfig, logger, moduleName, importerFile);
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('NormalizedOHMUrl inter-app hsp self import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghsp': {
        'packageName': 'pkghsp',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/hsp/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghsp',
        pkgPath: '/testHsp/hsp'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHsp/hsp/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghsp/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&pkghsp/src/main/ets/utils/Calc&';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl inter-app hsp others import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghsp': {
        'packageName': 'pkghsp',
        'bundleName': 'com.test.testHsp',
        'moduleName': 'hsp',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    this.rollup.share.projectConfig.dependencyAliasMap = new Map([
      ['pkghsp_alias', 'pkghsp']
    ]);
    this.rollup.share.projectConfig.harNameOhmMap = {
      'pkghsp_alias': '@bundle:com.test.testHsp/src/main/ets/utils/Calc'
    }
    const filePath: string = 'pkghsp/src/main/ets/utils/Calc';
    const indexFilePath: string = 'pkghsp_alias';
    const importerFile: string = '/testHap/entry/src/main/ets/pages/index.ets'
    const importByPkgName = 'pkghsp_alias';
    const standardImportPath: string = 'pkghsp_alias/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&hsp&com.test.testHsp&pkghsp/Index&';
    const standardImportPathNormalizedOhmUrl: string =
      '@normalized:N&hsp&com.test.testHsp&pkghsp/src/main/ets/utils/Calc&';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl in-app hsp self import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghsp': {
        'packageName': 'pkghsp',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/hsp/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghsp',
        pkgPath: '/testHsp/hsp'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHsp/hsp/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghsp/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&&pkghsp/src/main/ets/utils/Calc&';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl in-app hsp others import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghsp': {
        'packageName': 'pkghsp',
        'bundleName': '',
        'moduleName': 'hsp',
        'version': '',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    this.rollup.share.projectConfig.dependencyAliasMap = new Map([
      ['pkghsp_alias', 'pkghsp']
    ]);
    this.rollup.share.projectConfig.harNameOhmMap = {
      'pkghsp_alias': '@bundle:com.test.testHap/src/main/ets/utils/Calc'
    }
    const filePath: string = 'pkghsp_alias/src/main/ets/utils/Calc';
    const indexFilePath: string = 'pkghsp_alias';

    const importerFile: string = '/testHap/entry/src/main/ets/pages/index.ets'
    const importByPkgName = 'pkghsp_alias';
    const standardImportPath: string = 'pkghsp_alias/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&hsp&&pkghsp/Index&';
    const standardImportPathNormalizedOhmUrl: string = '@normalized:N&hsp&&pkghsp/src/main/ets/utils/Calc&';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl hap self import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'entry': {
        'packageName': 'entry',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': false
      }
    }
    const filePath: string = '/testHap/entry/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'entry',
        pkgPath: '/testHap/entry'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHap/entry/src/main/ets/pages/index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'entry/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&&entry/src/main/ets/utils/Calc&';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl source code har self import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHar/har/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghar',
        pkgPath: '/testHar/har'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHar/har/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl source code har others import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHar/har/src/main/ets/utils/Calc.ets';
    const indexFilePath: string = '/testHar/har/Index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: 'pkghar',
          pkgPath: '/testHar/har'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHar/entry/src/main/ets/pages/Index.ets'
    const importByPkgName = 'pkghar';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&&pkghar/Index&1.0.1';
    const standardImportPathNormalizedOhmUrl: string = '@normalized:N&&&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl source code har self import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/har/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghar',
        pkgPath: '/testHsp/har'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHsp/har/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl source code har others import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/har/src/main/ets/utils/Calc.ets';
    const indexFilePath: string = '/testHsp/har/Index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: 'pkghar',
          pkgPath: '/testHsp/har'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHsp/hsp/src/main/ets/pages/Index.ets'
    const importByPkgName = 'pkghar';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&pkghar/Index&1.0.1';
    const standardImportPathNormalizedOhmUrl: string =
      '@normalized:N&&com.test.testHsp&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl product har self import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHar/har/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghar',
        pkgPath: '/testHar/har'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHar/har/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl product har others import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHap/oh_modules/.ohpm/pkghar@test=/oh_modules/pkghar/src/main/ets/utils/Calc.ets';
    const indexFilePath: string = '/testHap/oh_modules/.ohpm/pkghar@test=/oh_modules/pkghar/Index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: 'pkghar',
          pkgPath: '/testHap/oh_modules/.ohpm/pkghar@test=/oh_modules/pkghar'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHar/entry/src/main/ets/pages/index.ets'
    const importByPkgName = 'pkghar';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&&pkghar/Index&1.0.1';
    const standardImportPathNormalizedOhmUrl: string = '@normalized:N&&&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl remote source code har self import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/har/src/main/ets/utils/Calc.ets';
    const moduleInfo = {
      id: filePath,
      meta: {
        pkgName: 'pkghar',
        pkgPath: '/testHsp/har'
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);
    const importerFile: string = '/testHsp/har/src/main/ets/pages/Index.ets'
    const relativePath: string = '../utils/Calc';
    const etsBasedAbsolutePath: string = 'ets/utils/Calc';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const relativePathOhmUrl: string = moduleSourceFile.getOhmUrl(this.rollup, relativePath, filePath, importerFile);
    const etsBasedAbsolutePathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, etsBasedAbsolutePath, filePath,
      importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(relativePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(etsBasedAbsolutePathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl remote source code har others import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'pkghar': {
        'packageName': 'pkghar',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '1.0.1',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const filePath: string = '/testHsp/har/src/main/ets/utils/Calc.ets';
    const indexFilePath: string = '/testHsp/har/Index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: 'pkghar',
          pkgPath: '/testHsp/har'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHsp/hsp/src/main/ets/pages/Index.ets'
    const importByPkgName = 'pkghar';
    const standardImportPath: string = 'pkghar/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&pkghar/Index&1.0.1';
    const standardImportPathNormalizedOhmUrl: string =
      '@normalized:N&&com.test.testHsp&pkghar/src/main/ets/utils/Calc&1.0.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl native so others import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'libproduct.so': {
        'packageName': 'libproduct.so',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': true
      }
    }
    const importerFile: string = '/testHap/hsp/src/main/ets/pages/Index.ets'
    const moduleRequest = 'libproduct.so';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const moduleRequestOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, undefined, importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:Y&&&libproduct.so&';
    expect(moduleRequestOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl native so others import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'libproduct.so': {
        'packageName': 'libproduct.so',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': true
      }
    }
    const importerFile: string = '/testHsp/hsp/src/main/ets/pages/Index.ets'
    const moduleRequest = 'libproduct.so';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const moduleRequestOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, undefined, importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:Y&&com.test.testHsp&libproduct.so&';
    expect(moduleRequestOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl native so others import (source code har)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'libhar.so': {
        'packageName': 'libhar.so',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': true
      }
    }
    const importerFile: string = '/testHap/har/src/main/ets/pages/Index.ets'
    const moduleRequest = 'libhar.so';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const moduleRequestOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, undefined, importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:Y&&&libhar.so&';
    expect(moduleRequestOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl native so others import (product har)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'libhar.so': {
        'packageName': 'libhar.so',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': true
      }
    }
    const importerFile: string =
      '/testHap/oh_modules/.ohpm/pkghar@test+har=/oh_modules/pkghar/src/main/ets/pages/Index.ets';
    const moduleRequest = 'libhar.so';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const moduleRequestOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, undefined, importerFile);
    const expectedNormalizedOhmUrl: string = '@normalized:Y&&&libhar.so&';
    expect(moduleRequestOhmUrl == expectedNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl ohpm package others import (hap/in-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      '@ohos/Test': {
        'packageName': '@ohos/Test',
        'bundleName': '',
        'moduleName': '',
        'version': '2.3.1',
        'entryPath': 'index.ets',
        'isSO': false
      }
    }
    const filePath: string =
      '/testHap/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test/src/main/ets/utils/Calc.ets'
    const indexFilePath: string = '/testHap/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test/index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: '@ohos/Test',
          pkgPath: '/testHap/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHap/entry/src/main/ets/pages/index.ets'
    const importByPkgName = '@ohos/Test';
    const standardImportPath: string = '@ohos/Test/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&&@ohos/Test/index&2.3.1';
    const standardImportPathNormalizedOhmUrl: string = '@normalized:N&&&@ohos/Test/src/main/ets/utils/Calc&2.3.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('NormalizedOHMUrl ohpm package others import (inter-app hsp)', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      '@ohos/Test': {
        'packageName': '@ohos/Test',
        'bundleName': 'com.test.testHsp',
        'moduleName': '',
        'version': '2.3.1',
        'entryPath': 'index.ets',
        'isSO': false
      }
    }
    const filePath: string =
      '/testHsp/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test/src/main/ets/utils/Calc.ets'
    const indexFilePath: string = '/testHsp/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test/index.ets';
    for (let file of [filePath, indexFilePath]) {
      const moduleInfo = {
        id: file,
        meta: {
          pkgName: '@ohos/Test',
          pkgPath: '/testHsp/oh_modules/.ohpm/@ohos+test@2.3.1/oh_modules/@ohos/test'
        }
      }
      this.rollup.moduleInfos.push(moduleInfo);
    }
    const importerFile: string = '/testHsp/entry/src/main/ets/pages/index.ets'
    const importByPkgName = '@ohos/Test';
    const standardImportPath: string = '@ohos/Test/src/main/ets/utils/Calc';
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&com.test.testHsp&@ohos/Test/index&2.3.1';
    const standardImportPathNormalizedOhmUrl: string =
      '@normalized:N&&com.test.testHsp&@ohos/Test/src/main/ets/utils/Calc&2.3.1';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
  });

  mocha.it('the error message of getNormalizedOhmUrlByFilepath', function () {
    this.rollup.build();
    const pkgParams = {
      pkgName: 'json5',
      pkgPath: `${projectConfig.projectRootPath}/entry/oh_modules/json5`,
      isRecordName: false
    };
    projectConfig.pkgContextInfo = {
      'json5': undefined
    };
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. ' +
      'Failed to get a resolved OhmUrl for "/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the "json5" module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    try {
      getNormalizedOhmUrlByFilepath(filePath, projectConfig, logger, pkgParams, importerFile);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('the error message of getNormalizedOhmUrlByFilepath without getHvigorConsoleLogger', function () {
    this.rollup.build();
    const pkgParams = {
      pkgName: 'json5',
      pkgPath: `${projectConfig.projectRootPath}/entry/oh_modules/json5`,
      isRecordName: false
    };
    projectConfig.pkgContextInfo = {
      'json5': undefined
    };
    const filePath: string = `${projectConfig.projectRootPath}/entry/oh_modules/json5/dist/index.js`;
    const moduleName: string = 'entry';
    const importerFile: string = 'importTest.ts';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_EXTERNAL_FAILED_TO_RESOLVE_OHM_URL,
      ArkTSErrorDescription,
      'Failed to resolve OhmUrl. ' +
      'Failed to get a resolved OhmUrl for "/testProjectRootPath/entry/oh_modules/json5/dist/index.js" imported by "importTest.ts".',
      '',
      ['Check whether the "json5" module which /testProjectRootPath/entry/oh_modules/json5/dist/index.js belongs to is correctly configured.',
       'Check the corresponding file name is correct(including case-sensitivity).']
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger = CommonLogger.getInstance(this.rollup);
    const stub = sinon.stub(logger.logger, 'error');
    try {
      getNormalizedOhmUrlByFilepath(filePath, projectConfig, logger, pkgParams, importerFile);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('transform mockConfigInfo', function () {
    this.rollup.preview();
    ModuleSourceFile.mockConfigInfo = PRVIEW_MOCK_CONFIG;
    this.rollup.share.projectConfig.modulePath = `${projectConfig.projectRootPath}/entry`;
    this.rollup.share.projectConfig.mockParams = {
      etsSourceRootPath: 'src/main/ets',
      mockConfigPath: `${projectConfig.projectRootPath}/entry/src/mock/mock-config.json5`
    }
    this.rollup.share.projectConfig.entryModuleName = 'entry';
    const importerFile: string = `${projectConfig.projectRootPath}/entry/src/main/ets/pages/index.ets`;
    const moduleInfo = {
      id: importerFile,
      meta: {
        moduleName: 'entry',
      }
    };
    this.rollup.moduleInfos.push(moduleInfo);
    for (let moduleRequest in PRVIEW_MOCK_CONFIG) {
      let mockPath = PRVIEW_MOCK_CONFIG[moduleRequest]
      let filePath: string;
      if (Object.prototype.hasOwnProperty.call(MOCK_CONFIG_FILEPATH, moduleRequest)) {
        filePath = MOCK_CONFIG_FILEPATH[moduleRequest];
        const moduleInfo = {
          id: filePath,
          meta: {
            moduleName: moduleRequest === 'lib' ? 'lib' : 'entry',
            pkgName: moduleRequest === 'lib' ? 'lib' : 'entry',
            pkgPath: moduleRequest === 'lib' ? `${projectConfig.projectRootPath}/oh_modules/lib` :
              `${projectConfig.projectRootPath}/entry`
          }
        };
        this.rollup.moduleInfos.push(moduleInfo);
      }
      const moduleSourceFile: string = new ModuleSourceFile();
      ModuleSourceFile.initPluginEnv(this.rollup);
      ModuleSourceFile.setProcessMock(this.rollup);
      moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, filePath, importerFile);
    }
    const expectMockConfig: Object = {
      '@ohos:bluetooth': {
        source: '@bundle:com.example.app/entry/mock/ohos/bluetooth.mock'
      },
      '@bundle:com.example.app/entry/ets/calc': {
        source: '@bundle:com.example.app/entry/mock/module/calc.mock'
      },
      '@bundle:/testProjectRootPath/oh_modules/lib/dist/index.js': {
        source: '@bundle:com.example.app/entry/mock/module/bigInt.mock'
      },
      '@app:UtTestApplication/entry/entry': {
        source: '@bundle:com.example.app/entry/mock/native/libentry.mock'
      }
    };
    expect(ModuleSourceFile.newMockConfigInfo.toString() === expectMockConfig.toString()).to.be.true;
    ModuleSourceFile.cleanUpObjects();
  });

  mocha.it('NormalizedOHMUrl transform mockConfigInfo', function () {
    this.rollup.preview();
    this.rollup.useNormalizedOHMUrl();
    this.rollup.share.projectConfig.pkgContextInfo = {
      'entry': {
        'packageName': 'entry',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': 'index.ets',
        'isSO': false
      },
      'lib': {
        'packageName': 'lib',
        'bundleName': '',
        'moduleName': 'lib',
        'version': '',
        'entryPath': 'index.ets',
        'isSO': false
      },
      'libentry.so': {
        'packageName': 'libentry.so',
        'bundleName': '',
        'moduleName': '',
        'version': '',
        'entryPath': '',
        'isSO': true
      }
    };
    ModuleSourceFile.mockConfigInfo = PRVIEW_MOCK_CONFIG;
    this.rollup.share.projectConfig.modulePath = `${projectConfig.projectRootPath}/entry`;
    this.rollup.share.projectConfig.mockParams = {
      etsSourceRootPath: 'src/main/ets',
      mockConfigPath: `${projectConfig.projectRootPath}/entry/src/mock/mock-config.json5`
    }
    this.rollup.share.projectConfig.entryModuleName = 'entry';

    const importerFile: string = `${projectConfig.projectRootPath}/entry/src/main/ets/pages/index.ets`;
    const moduleInfo = {
      id: importerFile,
      meta: {
        moduleName: 'entry',
        pkgName: 'entry',
        pkgPath: `${projectConfig.projectRootPath}/entry`
      }
    }
    this.rollup.moduleInfos.push(moduleInfo);

    for (let moduleRequest in PRVIEW_MOCK_CONFIG) {
      let mockPath = PRVIEW_MOCK_CONFIG[moduleRequest]
      let filePath: string;
      if (Object.prototype.hasOwnProperty.call(MOCK_CONFIG_FILEPATH, moduleRequest)) {
        filePath = MOCK_CONFIG_FILEPATH[moduleRequest];
        const moduleInfo = {
          id: filePath,
          meta: {
            moduleName: moduleRequest === 'lib' ? 'lib' : 'entry',
            pkgName: moduleRequest === 'lib' ? 'lib' : 'entry',
            pkgPath: moduleRequest === 'lib' ? `${projectConfig.projectRootPath}/oh_modules/lib` :
              `${projectConfig.projectRootPath}/entry`
          }
        }
        this.rollup.moduleInfos.push(moduleInfo);
      }
      const moduleSourceFile: string = new ModuleSourceFile();
      ModuleSourceFile.initPluginEnv(this.rollup);
      ModuleSourceFile.setProcessMock(this.rollup);
      moduleSourceFile.getOhmUrl(this.rollup, moduleRequest, filePath, importerFile);
    }
    const expectMockConfig = {
      '@ohos:bluetooth': {
        source: '@normalized:N&&&entry/src/main/mock/ohos/bluetooth.mock&'
      },
      '@normalized:N&&&entry/src/main/ets/calc&': {
        source: '@normalized:N&&&entry/src/main/mock/module/calc.mock&'
      },
      '@normalized:N&lib&&lib/dist/index&': {
        source: '@normalized:N&&&entry/src/main/mock/module/bigInt.mock&'
      },
      '@normalized:Y&&&libentry.so&': {
        source: '@normalized:N&&&entry/src/main/mock/native/libentry.mock&'
      }
    };
    expect(ModuleSourceFile.newMockConfigInfo.toString() === expectMockConfig.toString()).to.be.true;
    ModuleSourceFile.cleanUpObjects();
  });

  mocha.it('NormalizedOHMUrl bytecode har import', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'bytecode_har': {
        'packageName': 'bytecode_har',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      },
      'bytecode_alias_oh': {
        'packageName': 'bytecode_alias_oh',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    this.rollup.share.projectConfig.dependencyAliasMap = new Map([
      ['bytecode_alias', 'bytecode_har', 'bytecode_alias_oh']
    ]);
    this.rollup.share.projectConfig.byteCodeHarInfo = {
      'bytecode_alias': {
        'abcPath':'D:\\projectPath\\bytecode_har\\modules.abc'
      },
      'bytecode_alias_oh': {
        'abcPath':'D:\\projectPath\\bytecode_alias_oh\\modules.abc'
      }
    }
    const filePath: string = 'bytecode_alias/src/main/ets/utils/Calc';
    const indexFilePath: string = 'bytecode_alias';

    const importerFile: string = '/testHap/entry/src/main/ets/pages/index.ets'
    const importByPkgName = 'bytecode_alias';
    const standardImportPath: string = 'bytecode_alias/src/main/ets/utils/Calc';
    const importByPkgNameSlashes = 'bytecode_alias///';
    const importByPkgNameSlashesOh = 'bytecode_alias_oh///';
    const importModuleRequets = 'bytecode_alias_oh///\\\/'
    const moduleSourceFile: string = new ModuleSourceFile();
    ModuleSourceFile.initPluginEnv(this.rollup);
    const importByPkgNameOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, importByPkgName, indexFilePath, importerFile);
    const standardImportPathOhmUrl = moduleSourceFile.getOhmUrl(this.rollup, standardImportPath, filePath,
      importerFile);
    const importByPkgNameOhmUrlSlashes = moduleSourceFile.getOhmUrl(this.rollup, importByPkgNameSlashes, indexFilePath, importerFile);
    const importByPkgNameOhmUrlSlashesOh = moduleSourceFile.getOhmUrl(this.rollup, importByPkgNameSlashesOh, indexFilePath, importerFile);
    const importModuleRequetsOhmUrlSlashesOh = moduleSourceFile.getOhmUrl(this.rollup, importModuleRequets, indexFilePath, importerFile);
    const importByPkgNameNormalizedOhmUrl: string = '@normalized:N&&&bytecode_har/Index&1.0.0';
    const standardImportPathNormalizedOhmUrl: string = '@normalized:N&&&bytecode_har/src/main/ets/utils/Calc&1.0.0';
    const importByPkgNameNormalizedOhmUrlSlashes: string = '@normalized:N&&&bytecode_har/Index&1.0.0';
    const importByPkgNameNormalizedOhmUrlSlashesOh: string = '@normalized:N&&&bytecode_alias_oh/Index&1.0.0';
    const importModuleRequetsNormalizedOhmUrlSlashesOh: string = '@normalized:N&&&bytecode_alias_oh/Index&1.0.0';
    expect(importByPkgNameOhmUrl == importByPkgNameNormalizedOhmUrl).to.be.true;
    expect(standardImportPathOhmUrl == standardImportPathNormalizedOhmUrl).to.be.true;
    expect(importByPkgNameOhmUrlSlashes == importByPkgNameNormalizedOhmUrlSlashes).to.be.true;
    expect(importByPkgNameOhmUrlSlashesOh == importByPkgNameNormalizedOhmUrlSlashesOh).to.be.true;
    expect(importModuleRequetsOhmUrlSlashesOh == importModuleRequetsNormalizedOhmUrlSlashesOh).to.be.true;
  });

  mocha.it('useNormalizedOHMUrl app builtins error message', function () {
    this.rollup.build();
    this.rollup.useNormalizedOHMUrl();
    this.rollup.share.projectConfig.pkgContextInfo = {};
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_PKG_CONTENT_INFO,
      ArkTSInternalErrorDescription,
      "Can not get pkgContextInfo of package 'libapplication.so' which being imported by " +
      "'/testHap/oh_modules/.ohpm/pkghar@test+har=/oh_modules/pkghar/src/main/ets/pages/Index.ets'"
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const loggerStub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    const importerFile: string =
      '/testHap/oh_modules/.ohpm/pkghar@test+har=/oh_modules/pkghar/src/main/ets/pages/Index.ets';
    const appSoModuleRequest: string = 'libapplication.so';
    try {
      getOhmUrlBySystemApiOrLibRequest(appSoModuleRequest, this.rollup.share.projectConfig, logger,
        importerFile, true);
    } catch (e) {
    }
    expect(loggerStub.calledWith(errInfo)).to.be.true;
    loggerStub.restore();
  });

  mocha.it('useNormalizedOHMUrl app builtins error message without getHvigorConsoleLogger', function () {
    this.rollup.build();
    this.rollup.useNormalizedOHMUrl();
    this.rollup.share.projectConfig.pkgContextInfo = {};
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_UNABLE_TO_GET_PKG_CONTENT_INFO,
      ArkTSInternalErrorDescription,
      "Can not get pkgContextInfo of package 'libapplication.so' which being imported by " +
      "'/testHap/oh_modules/.ohpm/pkghar@test+har=/oh_modules/pkghar/src/main/ets/pages/Index.ets'"
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const logger = CommonLogger.getInstance(this.rollup);
    const loggerStub = sinon.stub(logger.logger, 'error');
    const importerFile: string =
      '/testHap/oh_modules/.ohpm/pkghar@test+har=/oh_modules/pkghar/src/main/ets/pages/Index.ets';
    const appSoModuleRequest: string = 'libapplication.so';
    try {
      getOhmUrlBySystemApiOrLibRequest(appSoModuleRequest, this.rollup.share.projectConfig, logger,
        importerFile, true);
    } catch (e) {
    }
    expect(loggerStub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    loggerStub.restore();
  });

  mocha.it('the error message of getNormalizedOhmUrlByModuleRequest', function () {
    const moduleInfoByModuleRequest = {
      normalizedPath: "bytecode_module/Index",
      packageName: "bytecode_module"
    };
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'bytecode_module1': {
        'packageName': 'bytecode_module1',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_NOT_FOUND_IN_CONTEXT_INFO,
      ArkTSInternalErrorDescription,
      "Failed to find package 'bytecode_module'. Failed to obtain package 'bytecode_module' " +
      "from the package context information."
    );
    const logger = CommonLogger.getInstance(this.rollup);
    const loggerStub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');
    try {
      delete this.rollup.share.projectConfig.pkgContextInfo['bytecode_module'];
      getNormalizedOhmUrlByModuleRequest(moduleInfoByModuleRequest, this.rollup.share.projectConfig, logger);
    } catch (e) {
    }
    expect(loggerStub.getCall(0).calledWithMatch(errInfo)).to.be.true;
    loggerStub.restore();
  });

  mocha.it('the error message of getNormalizedOhmUrlByAliasName', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'bytecode_har': {
        'packageName': 'bytecode_har',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    this.rollup.share.projectConfig.dependencyAliasMap = new Map([
      ['bytecode_alias', 'bytecode_har']
    ]);
    this.rollup.share.projectConfig.byteCodeHarInfo = {
      'bytecode_alias': {
        'abcPath': 'D:\\projectPath\\bytecode_har\\modules.abc'
      }
    }
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_ENTRY_FILE_NOT_FOUND,
      ArkTSInternalErrorDescription,
      "Failed to find entry file of 'bytecode_har'. Failed to obtain the entry file " +
      "information of 'bytecode_har' from the package context information."
    );
    const aliasPkgName = 'bytecode_alias';
    const pkgName = 'bytecode_har';
    const logger = CommonLogger.getInstance(this.rollup);
    const loggerStub = sinon.stub(logger.getLoggerFromErrorCode(errInfo.code), 'printError');

    try {
      delete this.rollup.share.projectConfig.pkgContextInfo['bytecode_har']['entryPath'];
      getNormalizedOhmUrlByAliasName(aliasPkgName, this.rollup.share.projectConfig, logger);
    } catch (e) {
    }
    expect(loggerStub.getCall(0).calledWith(errInfo)).to.be.true;

    try {
      delete this.rollup.share.projectConfig.pkgContextInfo['bytecode_har'];
      getNormalizedOhmUrlByAliasName(aliasPkgName, this.rollup.share.projectConfig, logger);
    } catch (e) {
    }
    const errInfo1: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_NOT_FOUND_IN_CONTEXT_INFO,
      ArkTSInternalErrorDescription,
      "Failed to find package 'bytecode_har'. Failed to obtain package 'bytecode_har' " +
      "from the package context information."
    );
    expect(loggerStub.getCall(1).calledWithMatch(errInfo1)).to.be.true;

    loggerStub.restore();
  });

  mocha.it('the error message of getNormalizedOhmUrlByAliasName without getHvigorConsoleLogger', function () {
    this.rollup.build();
    this.rollup.share.projectConfig.useNormalizedOHMUrl = true;
    this.rollup.share.projectConfig.pkgContextInfo = {
      'bytecode_har': {
        'packageName': 'bytecode_har',
        'bundleName': '',
        'moduleName': '',
        'version': '1.0.0',
        'entryPath': 'Index.ets',
        'isSO': false
      }
    }
    this.rollup.share.projectConfig.dependencyAliasMap = new Map([
      ['bytecode_alias', 'bytecode_har']
    ]);
    this.rollup.share.projectConfig.byteCodeHarInfo = {
      'bytecode_alias': {
        'abcPath': 'D:\\projectPath\\bytecode_har\\modules.abc'
      }
    }
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_ENTRY_FILE_NOT_FOUND,
      ArkTSInternalErrorDescription,
      "Failed to find entry file of 'bytecode_har'. Failed to obtain the entry file " +
      "information of 'bytecode_har' from the package context information."
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const aliasPkgName = 'bytecode_alias';
    const pkgName = 'bytecode_har';
    const logger = CommonLogger.getInstance(this.rollup);
    const loggerStub = sinon.stub(logger.logger, 'error');

    try {
      delete this.rollup.share.projectConfig.pkgContextInfo['bytecode_har']['entryPath'];
      getNormalizedOhmUrlByAliasName(aliasPkgName, this.rollup.share.projectConfig, logger);
    } catch (e) {
    }
    expect(loggerStub.getCall(0).calledWith(errInfo.toString())).to.be.true;

    try {
      delete this.rollup.share.projectConfig.pkgContextInfo['bytecode_har'];
      getNormalizedOhmUrlByAliasName(aliasPkgName, this.rollup.share.projectConfig, logger);
    } catch (e) {
    }
    const errInfo1: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_PACKAGE_NOT_FOUND_IN_CONTEXT_INFO,
      ArkTSInternalErrorDescription,
      "Failed to find package 'bytecode_har'. Failed to obtain package 'bytecode_har' " +
      "from the package context information."
    );
    expect(loggerStub.getCall(1).calledWithMatch(errInfo1.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    loggerStub.restore();
  });
});