/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
import fs from "fs";
import * as ts from 'typescript';
import mocha from 'mocha';
import path from "path";
import sinon from 'sinon';

import {
  MergedConfig,
  ObConfigResolver,
  collectResevedFileNameInIDEConfig,
  getNameCacheByPath,
  getRelativeSourcePath,
  handleKeepFilesAndGetDependencies,
  setNewNameCache,
  setUnobfuscationNames,
  sourceFileDependencies,
  writeObfuscatedFile,
  writeObfuscationCaches,
  IDENTIFIER_CACHE,
  updateIncrementalCaches,
  readProjectCaches,
  getUpdatedFiles,
  obfuscationPreprocess,
  reObfuscate,
  printObfLogger
} from '../../../lib/fast_build/ark_compiler/common/ob_config_resolver';
import {
  OBFUSCATION_RULE_PATH,
  OBFUSCATION_RULE_TEMPLATE_PATH
} from '../mock/rollup_mock/path_config';
import { OBFUSCATION_TOOL } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { RELEASE } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import {
  ArkObfuscator,
  nameCacheMap,
  unobfuscationNamesObj,
  ProjectCollections,
  PerfMode,
  TimeAndMemTimeTracker,
  printerTimeAndMemDataConfig,
} from 'arkguard';
import * as arkUtils from '../../../lib/ark_utils'

const OBFUSCATE_TESTDATA_DIR = path.resolve(__dirname, '../../../test/ark_compiler_ut/testdata/obfuscation');

mocha.describe('test obfuscate config resolver api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
    let obfuscationContent = undefined;
    try {
      obfuscationContent = fs.readFileSync(OBFUSCATION_RULE_TEMPLATE_PATH, 'utf-8');
      obfuscationContent = obfuscationContent.replace('OBFUSCATE_TESTDATA_DIR', OBFUSCATE_TESTDATA_DIR);
      fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, obfuscationContent);
    } catch (err) {
      throw err;
    }
  });

  mocha.afterEach(() => {
    if (fs.existsSync(`${OBFUSCATION_RULE_PATH}`)) {
      fs.unlinkSync(`${OBFUSCATION_RULE_PATH}`);
    }
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test resolveDts', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [ OBFUSCATION_RULE_PATH ]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfig: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfig.resolveObfuscationConfigs();
    expect(mergedObConfig.options.enableToplevelObfuscation).to.be.true;
    expect(mergedObConfig.options.enablePropertyObfuscation).to.be.false;

    const reservedNames = mergedObConfig.reservedNames;
    expect(reservedNames.length == 4).to.be.true;
    expect(reservedNames.includes('matrix44')).to.be.true;
    expect(reservedNames.includes('TranslateOption2')).to.be.true;
    expect(reservedNames.includes('TestAdd')).to.be.true;
    expect(reservedNames.includes('TestProperty')).to.be.true;

    const reservedPropertyNames = mergedObConfig.reservedPropertyNames;
    expect(reservedPropertyNames.length == 4).to.be.true;
    expect(reservedPropertyNames.includes('matrix44')).to.be.true;
    expect(reservedPropertyNames.includes('TranslateOption2')).to.be.true;
    expect(reservedPropertyNames.includes('TestAdd')).to.be.true;
    expect(reservedPropertyNames.includes('TestProperty')).to.be.true;

    const reservedGlobalNames = mergedObConfig.reservedGlobalNames;
    expect(reservedGlobalNames.length == 4).to.be.true;
    expect(reservedGlobalNames.includes('matrix44')).to.be.true;
    expect(reservedGlobalNames.includes('TranslateOption2')).to.be.true;
    expect(reservedGlobalNames.includes('TestAdd')).to.be.true;
    expect(reservedGlobalNames.includes('TestProperty')).to.be.true;

    this.rollup.clearCache();
  });

  mocha.it('1-2: test resolveObfuscationConfigs: -enable-property-obfuscation', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enable-property-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    const obOptions: ObOptions = mergedObConfig.options;
    for (const [optionName, optionValue] of Object.entries(obOptions)) {
      if (optionName === 'enablePropertyObfuscation') {
        expect(optionValue).to.be.true;
      } else {
        if (typeof optionValue === 'boolean') {
          expect(optionValue).to.be.false;
        } else if (typeof optionValue === 'string') {
          expect(optionValue === '').to.be.true;
        }
      }
    }
  });

  mocha.it('1-3: test resolveObfuscationConfigs: -enable-property-obfuscation -enable-export-obfuscation', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enable-property-obfuscation\n-enable-export-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    const obOptions: ObOptions = mergedObConfig.options;
    for (const [optionName, optionValue] of Object.entries(obOptions)) {
      if (optionName === 'enablePropertyObfuscation' || optionName === 'enableExportObfuscation') {
        expect(optionValue).to.be.true;
      } else {
        if (typeof optionValue === 'boolean') {
          expect(optionValue).to.be.false;
        } else if (typeof optionValue === 'string') {
          expect(optionValue === '').to.be.true;
        }
      }
    }
  });

  mocha.it('1-3: test resolveObfuscationConfigs: -enable-property-obfuscation -enable-string-property-obfuscation', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enable-property-obfuscation\n-enable-string-property-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    const obOptions: ObOptions = mergedObConfig.options;
    for (const [optionName, optionValue] of Object.entries(obOptions)) {
      if (optionName === 'enablePropertyObfuscation' || optionName === 'enableStringPropertyObfuscation') {
        expect(optionValue).to.be.true;
      } else {
        if (typeof optionValue === 'boolean') {
          expect(optionValue).to.be.false;
        } else if (typeof optionValue === 'string') {
          expect(optionValue === '').to.be.true;
        }
      }
    }
  });

  mocha.it('1-4: test resolveObfuscationConfigs: -enable-property-obfuscation -enable-toplevel-obfuscation', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enable-property-obfuscation\n-enable-toplevel-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    const obOptions: ObOptions = mergedObConfig.options;
    for (const [optionName, optionValue] of Object.entries(obOptions)) {
      if (optionName === 'enablePropertyObfuscation' || optionName === 'enableToplevelObfuscation') {
        expect(optionValue).to.be.true;
      } else {
        if (typeof optionValue === 'boolean') {
          expect(optionValue).to.be.false;
        } else if (typeof optionValue === 'string') {
          expect(optionValue === '').to.be.true;
        }
      }
    }
  });

  mocha.it('1-5: test resolveObfuscationConfigs: enable all', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enable-property-obfuscation\n-enable-export-obfuscation\n-enable-filename-obfuscation\n'
                                + '-enable-string-property-obfuscation\n-enable-toplevel-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    const obOptions: ObOptions = mergedObConfig.options;
    expect(obOptions.disableObfuscation).to.be.false;
    expect(obOptions.enablePropertyObfuscation).to.be.true;
    expect(obOptions.enableStringPropertyObfuscation).to.be.true;
    expect(obOptions.enableToplevelObfuscation).to.be.true;
    expect(obOptions.enableFileNameObfuscation).to.be.true;
    expect(obOptions.enableExportObfuscation).to.be.true;
    expect(obOptions.removeComments).to.be.false;
    expect(obOptions.compact).to.be.false;
    expect(obOptions.removeLog).to.be.false;
  });

  describe('2: test collectResevedFileNameInIDEConfig', function() {
    let aceModuleJsonPath = '';
    let ohPackagePath = '';
    let projectConfig = {};
    let modulePathMap = {};
    let entryArray = [];
    mocha.before('init config', function () {
      aceModuleJsonPath = path.join(OBFUSCATE_TESTDATA_DIR, 'filename_obf/module.json');
      ohPackagePath = path.join(OBFUSCATE_TESTDATA_DIR, 'filename_obf/oh-package.json5');
      projectConfig = {
        aceModuleJsonPath: aceModuleJsonPath,
        projectPath: '/mnt/application/entry/src/main/ets',
        cachePath: '/mnt/application/entry/build/default/cache/default/default@HarCompileArkTs/esmodules/release',
        aceModuleBuild: '/mnt/application/entry/build/default/intermediates/loader_out',
        compileShared: false,
        compileHar: false,
        byteCodeHar: false,
      };
      modulePathMap = {
        'entry': '/mnt/application/entry',
        'harpackagename': '/mnt/application/harPackageName'
      };
      entryArray = [
        'entryability/EntryAbility',
        'pages/Index'
      ];
    });

    mocha.it('2-1: test collectResevedFileNameInIDEConfig in hsp module', function () {
      projectConfig.compileShared = true;
      projectConfig.compileHar = false;
      projectConfig.byteCodeHar = false;
      const acutualReservedFileNames: string[] = collectResevedFileNameInIDEConfig(ohPackagePath, projectConfig, modulePathMap, entryArray);
      const expectReservedFileNames = [
        'entryability',
        'EntryAbility',
        'pages',
        'Index',
        '',
        'mnt',
        'application',
        'entry',
        '',
        'mnt',
        'application',
        'harPackageName',
        'entry',
        'harpackagename',
        '.',
        'Index-oh-package.ets',
        '.',
        'Type-oh-package.ets',
        '..',
        '..',
        'Index2.ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'intermediates',
        'loader_out',
        'etsFortgz',
        '',
        'mnt',
        'application',
        'entry',
        'src',
        'main',
        'ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'cache',
        'default',
        'default@HarCompileArkTs',
        'esmodules',
        'release'
      ];
      expect(acutualReservedFileNames.toString() === expectReservedFileNames.toString()).to.be.true;
    });
  
    mocha.it('2-2: test collectResevedFileNameInIDEConfig in hap module', function () {
      projectConfig.compileShared = false;
      projectConfig.compileHar = false;
      projectConfig.byteCodeHar = false;
      const acutualReservedFileNames: string[] = collectResevedFileNameInIDEConfig(ohPackagePath, projectConfig, modulePathMap, entryArray);
      const expectReservedFileNames = [
        'entryability',
        'EntryAbility',
        'pages',
        'Index',
        '',
        'mnt',
        'application',
        'entry',
        '',
        'mnt',
        'application',
        'harPackageName',
        'entry',
        'harpackagename',
        '.',
        'Index-oh-package.ets',
        '.',
        'Type-oh-package.ets',
        '..',
        '..',
        'Index2.ets',
        '',
        'mnt',
        'application',
        'entry',
        'src',
        'main',
        'ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'cache',
        'default',
        'default@HarCompileArkTs',
        'esmodules',
        'release'
      ];
      expect(acutualReservedFileNames.toString() === expectReservedFileNames.toString()).to.be.true;
    });
  
    mocha.it('2-3: test collectResevedFileNameInIDEConfig in source har module', function () {
      projectConfig.compileShared = false;
      projectConfig.compileHar = true;
      projectConfig.byteCodeHar = false;
      const acutualReservedFileNames: string[] = collectResevedFileNameInIDEConfig(ohPackagePath, projectConfig, modulePathMap, entryArray);
      const expectReservedFileNames = [
        'entryability',
        'EntryAbility',
        'pages',
        'Index',
        '',
        'mnt',
        'application',
        'entry',
        '',
        'mnt',
        'application',
        'harPackageName',
        'entry',
        'harpackagename',
        '.',
        'Index-oh-package.ets',
        '.',
        'Type-oh-package.ets',
        '..',
        '..',
        'Index2.ets',
        '',
        'mnt',
        'application',
        'entry',
        'src',
        'main',
        'ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'cache',
        'default',
        'default@HarCompileArkTs',
        'esmodules',
        'release'
      ];
      expect(acutualReservedFileNames.toString() === expectReservedFileNames.toString()).to.be.true;
    });
  
    mocha.it('2-4: test collectResevedFileNameInIDEConfig in byte code har module', function () {
      projectConfig.compileShared = false;
      projectConfig.compileHar = true;
      projectConfig.byteCodeHar = true;
      const acutualReservedFileNames: string[] = collectResevedFileNameInIDEConfig(ohPackagePath, projectConfig, modulePathMap, entryArray);
      const expectReservedFileNames = [
        'entryability',
        'EntryAbility',
        'pages',
        'Index',
        '',
        'mnt',
        'application',
        'entry',
        '',
        'mnt',
        'application',
        'harPackageName',
        'entry',
        'harpackagename',
        '.',
        'Index-oh-package.ets',
        '.',
        'Type-oh-package.ets',
        '..',
        '..',
        'Index2.ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'intermediates',
        'loader_out',
        'etsFortgz',
        '',
        'mnt',
        'application',
        'entry',
        'src',
        'main',
        'ets',
        '',
        'mnt',
        'application',
        'entry',
        'build',
        'default',
        'cache',
        'default',
        'default@HarCompileArkTs',
        'esmodules',
        'release'
      ];
      expect(acutualReservedFileNames.toString() === expectReservedFileNames.toString()).to.be.true;
    });
  })

  mocha.it('4-1: test getRelativeSourcePath: filePath starts with projectRootPath', function () {
    const filePath = 'C:/projects/my-project/src/file.ts';
    const projectRootPath = 'C:/projects/my-project';
    const belongProjectPath = undefined;
    const relativePath = getRelativeSourcePath(filePath, projectRootPath, belongProjectPath);
    expect(relativePath).to.equal('src/file.ts');
  });

  mocha.it('4-2: test getRelativeSourcePath: filePath starts with belongProjectPath', function () {
    const filePath = 'C:/projects/another-project/src/file.ts';
    const projectRootPath = 'C:/projects/my-project';
    const belongProjectPath = 'C:/projects/another-project';
    const relativePath = getRelativeSourcePath(filePath, projectRootPath, belongProjectPath);
    expect(relativePath).to.equal('src/file.ts');
  });

  mocha.it('4-3: test getRelativeSourcePath: undefined projectRootPath', function () {
    const filePath = 'C:/projects/another-project/src/file.ts';
    const projectRootPath = undefined;
    const belongProjectPath = 'C:/projects/another-project';
    const relativePath = getRelativeSourcePath(filePath, projectRootPath, belongProjectPath);
    expect(relativePath).to.equal('src/file.ts');
  });

  mocha.it('4-4: test getRelativeSourcePath: undefined belongProjectPath ', function () {
    const filePath = 'C:/projects/my-project/src/file.ts';
    const projectRootPath = 'C:/projects/my-project';
    const belongProjectPath = undefined;
    const relativePath = getRelativeSourcePath(filePath, projectRootPath, belongProjectPath);
    expect(relativePath).to.equal('src/file.ts');
  });

  mocha.it('5-1: test getFileNamesForScanningWhitelist: -keep is empty ', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.projectRootPath = ''
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();
    mergedObConfig.keepSourceOfPaths = [];
    const keepFilesAndDependencies: Set<string> = handleKeepFilesAndGetDependencies(mergedObConfig,
      this.rollup.share.projectConfig.projectRootPath, new ArkObfuscator());
    expect(keepFilesAndDependencies.size === 0).to.be.true;
  });

  mocha.it('5-2: test getFileNamesForScanningWhitelist: unable export obfuscation ', function () {
    this.rollup.build(RELEASE);
    const logger: object = this.rollup.share.getLogger(OBFUSCATION_TOOL);
    const optionContent: string = '-enble-export-obfuscation';
    fs.writeFileSync(`${OBFUSCATION_RULE_PATH}`, optionContent);
    this.rollup.share.projectConfig.projectRootPath = ''
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATION_RULE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      }
    };
    
    const obConfigResolver: ObConfigResolver =  new ObConfigResolver(this.rollup.share.projectConfig, logger, true);
    const mergedObConfig: MergedConfig = obConfigResolver.resolveObfuscationConfigs();

    let testFile1: string = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project1/file1.js');
    let testFile2: string = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project1/file2.js');
    let testFile3: string = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project2/file3.js');
    let testFile4: string = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project2/file4.js');
    let testFile5: string = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project3/file5.js');

    mergedObConfig.keepSourceOfPaths = [
      testFile1,
      testFile3
    ];
    mergedObConfig.excludePathSet = new Set<string>([]);
    mergedObConfig.options.enableExportObfuscation = true;
    this.rollup.share.projectConfig.rootPathSet = new Set<string>([
      path.resolve(__dirname, '../testdata/obfuscation_keeptest/project1/'),
      path.resolve(__dirname, '../testdata/obfuscation_keeptest/project2/')
    ]);
    this.rollup.share.projectConfig.projectRootPath = path.resolve(__dirname, '../testdata/obfuscation_keeptest/project1/');

    const cache1: ts.ModeAwareCache<ts.ResolvedModuleFull | undefined> =
      ts.createModeAwareCache<ts.ResolvedModuleFull | undefined>();
    cache1.set('file1.ts', ts.ModuleKind.ESNext, { 
      resolvedFileName: testFile2, 
      isExternalLibraryImport: false, 
      extension: ts.Extension.Js
    });
    cache1.set('file2.ts', ts.ModuleKind.ESNext, { 
      resolvedFileName: testFile3, 
      isExternalLibraryImport: false, 
      extension: ts.Extension.Js
    });
    sourceFileDependencies.set(testFile1, cache1);

    const cache2: ts.ModeAwareCache<ts.ResolvedModuleFull | undefined> =
      ts.createModeAwareCache<ts.ResolvedModuleFull | undefined>();
    cache2.set('file4.ts', ts.ModuleKind.ESNext, { 
      resolvedFileName: testFile4, 
      isExternalLibraryImport: false, 
      extension: ts.Extension.Js
    });
    cache2.set('file5.ts', ts.ModuleKind.ESNext, { 
      resolvedFileName: testFile5, 
      isExternalLibraryImport: false, 
      extension: ts.Extension.Js
    });
    sourceFileDependencies.set(testFile3, cache2);

    let arkguardConfig: Object = {
      mNameObfuscation: {
        mEnable: true,
        mNameGeneratorType: 1,
        mRenameProperties: false,
        mReservedProperties: [],
        mTopLevel: false,
        mReservedToplevelNames:[],
      },
      mKeepFileSourceCode: {
        mKeepSourceOfPaths: [
          testFile1,
          testFile3
        ],
        mkeepFilesAndDependencies: []
      },
      mExportObfuscation: true,
      mPerformancePrinter: []
    };

    let arkObfuscator: ArkObfuscator = new ArkObfuscator();
    arkObfuscator.init(arkguardConfig);

    const keepFilesAndDependencies: Set<string> = handleKeepFilesAndGetDependencies(mergedObConfig,
      arkObfuscator, this.rollup.share.projectConfig);
    expect(keepFilesAndDependencies.has(testFile1)).to.be.true;
    expect(keepFilesAndDependencies.has(testFile2)).to.be.true;
    expect(keepFilesAndDependencies.has(testFile3)).to.be.true;
    expect(keepFilesAndDependencies.has(testFile4)).to.be.true;
    expect(keepFilesAndDependencies.has(testFile5)).to.be.false;
  });

  mocha.it('6-1: test getHistoryNameCache: isDeclaration false', function () {
    const moduleInfo = {
      content: 'module content',
      buildFilePath: 'build/path/to/file.ts',
      relativeSourceFilePath: 'src/file.ts',
      originSourceFilePath: 'C:/projects/my-project/src/file.ts'
    };
    nameCacheMap.set('src/file.ts', {
      [IDENTIFIER_CACHE]: {
        'identifier1:1': 'obfuscated1',
        identifier2: 'obfuscated2'
      }
    });
    const isDeclaration: boolean = false;
    const projectRootPath: string = 'C:/projects/my-project';
    const result: Map<string, string> = getNameCacheByPath(moduleInfo, isDeclaration, projectRootPath);
    nameCacheMap.clear();
    expect(result.size).to.equal(2);
    expect(result.get('identifier1')).to.equal('obfuscated1');
    expect(result.get('identifier2')).to.equal('obfuscated2');
  });

  mocha.it('6-2: test getHistoryNameCache: isDeclaration true', function () {
    const moduleInfo = {
      content: 'module content',
      buildFilePath: 'build/path/to/file.d.ts',
      relativeSourceFilePath: 'src/file.ts',
      originSourceFilePath: 'C:/projects/my-project/src/file.ts'
    };
    nameCacheMap.set('src/file.ts', {
      [IDENTIFIER_CACHE]: {
        'identifier1:1': 'obfuscated1',
        identifier2: 'obfuscated2'
      }
    });
    const isDeclaration: boolean = true;
    const projectRootPath: string = 'C:/projects/my-project';
    const result: Map<string, string> = getNameCacheByPath(moduleInfo, isDeclaration, projectRootPath);
    nameCacheMap.clear();
    expect(result.size).to.equal(2);
    expect(result.get('identifier1')).to.equal('obfuscated1');
    expect(result.get('identifier2')).to.equal('obfuscated2');
  });

  mocha.it('7-1: test setNewNameCache: isOhModule is false', function () {
    const moduleInfo = {
      content: 'module content',
      buildFilePath: 'build/path/to/file.ts',
      relativeSourceFilePath: 'src/file.ts',
      originSourceFilePath: 'C:/projects/my-project/src/file.ts'
    };
    nameCacheMap.set('src/file.ts', {
      [IDENTIFIER_CACHE]: {
        'identifier1:1': 'obfuscated1',
        identifier2: 'obfuscated2'
      }
    });
    const newNameCache = {
      [IDENTIFIER_CACHE]: {
        'identifier3:3': 'obfuscated3'
      }
    };
    const isDeclaration: boolean = false;
    const projectConfig: Object = {
      obfuscationMergedObConfig: { options: { enableFileNameObfuscation: true } },
      projectRootPath: '/projectRoot',
      packageDir: 'oh_modules',
      modulePathMap: {
        module1: '/externalModule'
      },
    };
    setNewNameCache(newNameCache, isDeclaration, moduleInfo, projectConfig);
    const result: string | {} = nameCacheMap.get(moduleInfo.relativeSourceFilePath);
    nameCacheMap.clear();

    expect(result[IDENTIFIER_CACHE]['identifier3:3'] === 'obfuscated3').to.be.true;
    expect(result.obfName === 'src/file.ts').to.be.false;
  });

  mocha.it('7-2: test setNewNameCache: isOhModule is true', function () {
    const moduleInfo = {
      content: 'module content',
      buildFilePath: 'build/path/to/file.ts',
      relativeSourceFilePath: 'src/file.ts',
      originSourceFilePath: '/projectRoot/oh_modules/src/file.ts'
    };
    nameCacheMap.set('src/file.ts', {
      [IDENTIFIER_CACHE]: {
        'identifier1:1': 'obfuscated1',
        identifier2: 'obfuscated2'
      }
    });
    const newNameCache = {
      [IDENTIFIER_CACHE]: {
        'identifier3:3': 'obfuscated3'
      }
    };
    const isDeclaration: boolean = false;
    const projectConfig: Object = {
      obfuscationMergedObConfig: { options: { enableFileNameObfuscation: true } },
      projectRootPath: '/projectRoot',
      packageDir: 'oh_modules'
    };
    setNewNameCache(newNameCache, isDeclaration, moduleInfo, projectConfig);
    const result: string | {} = nameCacheMap.get(moduleInfo.relativeSourceFilePath);
    nameCacheMap.clear();

    expect(result[IDENTIFIER_CACHE]['identifier3:3'] === 'obfuscated3').to.be.true;
    expect(result.obfName === 'src/file.ts').to.be.true;
  });

  mocha.it('8-1: test setUnobfuscationNames', function () {
    const relativeSourceFilePath: string = 'src/file.ts';
    const unobfuscationNameMap: Map<string, Set<string>> = new Map([
      ['key1', new Set(['value1', 'value2'])],
      ['key2', new Set(['value3'])]
    ]);
    const isDeclaration: boolean = false;

    setUnobfuscationNames(unobfuscationNameMap, relativeSourceFilePath, isDeclaration);

    expect(unobfuscationNamesObj[relativeSourceFilePath]).to.deep.equal({
      key1: ['value1', 'value2'],
      key2: ['value3']
    });
  });

  mocha.it('9-1: test writeFile', function () {
    const newFilePath: string = '../../test/ark_compiler_ut/testdata/writeFile.ts';
    const content: string = 'obfuscated code';

    writeObfuscatedFile(newFilePath, content);

    expect(fs.existsSync(newFilePath)).to.be.true;
    const fileContent: string = fs.readFileSync(newFilePath, 'utf-8');
    expect(fileContent).to.equal(content);
    fs.unlinkSync(newFilePath);
  });

  mocha.describe('10: test Obfuscation Functions', function () {
    let arkObfuscatorStub;
    let filePathManagerStub;
    let fileContentManagerStub;
    let projectWhiteListManagerStub;

    mocha.beforeEach(function () {
      filePathManagerStub = {
        getDeletedSourceFilePaths: sinon.stub(),
        createOrUpdateSourceFilePaths: sinon.stub(),
        addedSourceFilePaths: new Set(),
      };
      fileContentManagerStub = {
        deleteFileContent: sinon.stub(),
        readFileNamesMap: sinon.stub(),
        getSortedFiles: sinon.stub(),
        fileNamesMap: new Map(),
        readFileContent: sinon.stub(),
      };
      projectWhiteListManagerStub = {
        createOrUpdateWhiteListCaches: sinon.stub(),
        getShouldReObfuscate: sinon.stub(),
      };
      arkObfuscatorStub = {
        filePathManager: filePathManagerStub,
        fileContentManager: fileContentManagerStub,
        shouldReObfuscate: false,
        isIncremental: false,
      };
      ProjectCollections.initProjectWhiteListManager('', false, false);
      sinon.stub(ProjectCollections, 'projectWhiteListManager').value(projectWhiteListManagerStub);
    });

    mocha.afterEach(function () {
      sinon.restore();
    });

    mocha.describe('10-1: updateIncrementalCaches', function () {
      mocha.it('10-1-1: should update caches and set shouldReObfuscate flag if needed', function () {
        const deletedFilesSet = new Set(['file1.ts', 'file2.ts']);
        filePathManagerStub.getDeletedSourceFilePaths.returns(deletedFilesSet);
        projectWhiteListManagerStub.getShouldReObfuscate.returns(true);

        updateIncrementalCaches(arkObfuscatorStub);

        expect(projectWhiteListManagerStub.createOrUpdateWhiteListCaches.calledWith(deletedFilesSet)).to.be.true;
        expect(fileContentManagerStub.deleteFileContent.calledWith(deletedFilesSet)).to.be.true;
        expect(arkObfuscatorStub.shouldReObfuscate).to.be.true;
      });

      mocha.it('10-1-2: should not set shouldReObfuscate flag if not needed', function () {
        const deletedFilesSet = new Set(['file1.ts', 'file2.ts']);
        filePathManagerStub.getDeletedSourceFilePaths.returns(deletedFilesSet);
        projectWhiteListManagerStub.getShouldReObfuscate.returns(false);

        updateIncrementalCaches(arkObfuscatorStub);

        expect(arkObfuscatorStub.shouldReObfuscate).to.be.false;
      });
    });

    mocha.describe('10-2: readProjectCaches', function () {
      mocha.it('10-2-1: should update source file paths and read file names map if incremental', function () {
        const allFiles = new Set(['file1.ts', 'file2.ts']);
        arkObfuscatorStub.isIncremental = true;

        readProjectCaches(allFiles, arkObfuscatorStub);

        expect(filePathManagerStub.createOrUpdateSourceFilePaths.calledWith(allFiles)).to.be.true;
        expect(fileContentManagerStub.readFileNamesMap.calledOnce).to.be.true;
      });

      mocha.it('10-2-2: should not read file names map if not incremental', function () {
        const allFiles = new Set(['file1.ts', 'file2.ts']);
        arkObfuscatorStub.isIncremental = false;

        readProjectCaches(allFiles, arkObfuscatorStub);

        expect(fileContentManagerStub.readFileNamesMap.called).to.be.false;
      });
    });

    mocha.describe('10-3: getUpdatedFiles', function () {
      mocha.it('10-3-1: should return updated files if incremental', function () {
        const sourceProjectConfig = {
          arkObfuscator: {
            isIncremental: true,
            filePathManager: {
              addedSourceFilePaths: new Set(['file3.ts']),
            },
          },
        };
        const allSourceFilePaths = new Set(['file1.ts', 'file2.ts', 'file3.d.ts', 'file4.d.ets']);
        const moduleSourceFiles = [{ moduleId: 'file1.ts' }, { moduleId: 'file2.ts' }];
        const updatedFiles = getUpdatedFiles(sourceProjectConfig, allSourceFilePaths, moduleSourceFiles);
        expect(updatedFiles).to.deep.equal(new Set(['file1.ts', 'file2.ts', 'file3.ts', 'file3.d.ts', 'file4.d.ets']));
      });

      mocha.it('10-3-2: should return all source files if not incremental', function () {
        const sourceProjectConfig = {
          arkObfuscator: {
            isIncremental: false,
          },
        };
        const allSourceFilePaths = new Set(['file1.ts', 'file2.ts', 'file3.d.ts']);
        const moduleSourceFiles = [{ moduleId: 'file1.ts' }, { moduleId: 'file2.ts' }];
        const updatedFiles = getUpdatedFiles(sourceProjectConfig, allSourceFilePaths, moduleSourceFiles);
        expect(updatedFiles).to.deep.equal(allSourceFilePaths);
      });
    });

    mocha.describe('10-4: obfuscationPreprocess', function () {
      mocha.it('10-4-1: should call readProjectCaches and updateIncrementalCaches if arkObfuscator exists', function () {
        const sourceProjectConfig = {
          arkObfuscator: arkObfuscatorStub,
        };
        const allSourceFilePaths = new Set(['file1.ts', 'file2.ts']);
        const obfuscationConfig = {};
        const keepFilesAndDependencies = new Set();
        const moduleSourceFiles = [{ moduleId: 'file1.ts' }, { moduleId: 'file2.ts' }];

        obfuscationPreprocess(
          sourceProjectConfig,
          obfuscationConfig,
          allSourceFilePaths,
          keepFilesAndDependencies,
          moduleSourceFiles
        );

        expect(filePathManagerStub.createOrUpdateSourceFilePaths.calledWith(allSourceFilePaths)).to.be.true;
        expect(projectWhiteListManagerStub.createOrUpdateWhiteListCaches.called).to.be.true;
        expect(ProjectCollections.projectWhiteListManager).to.be.undefined;
      });

      mocha.it('10-4-2: should not call readProjectCaches if arkObfuscator does not exist', function () {
        const sourceProjectConfig = {};
        const allSourceFilePaths = new Set(['file1.ts', 'file2.ts']);
        const obfuscationConfig = {};
        const keepFilesAndDependencies = new Set();
        const moduleSourceFiles = [{ moduleId: 'file1.ts' }, { moduleId: 'file2.ts' }];

        obfuscationPreprocess(
          sourceProjectConfig,
          obfuscationConfig,
          allSourceFilePaths,
          keepFilesAndDependencies,
          moduleSourceFiles
        );

        expect(filePathManagerStub.createOrUpdateSourceFilePaths.called).to.be.false;
      });
    });

    mocha.describe('10-5: reObfuscate', function () {
      mocha.it('10-5-1: should update harFilesRecord if is declaration file', async function () {
        const harFilesRecord = new Map();
        const logger = {};
        const projectConfig = {};
        const sortedFiles = ['file1.d.ts', 'file2.d.ts'];
        const fileContentObj1 = {
          moduleInfo: {
            originSourceFilePath: 'file1.d.ts',
            buildFilePath: 'file1.d.ts',
            content: 'content1',
          },
          previousStageSourceMap: {},
        };
        const fileContentObj2 = {
          moduleInfo: {
            originSourceFilePath: 'file2.d.ts',
            buildFilePath: 'file2.d.ts',
            content: 'content2',
          },
          previousStageSourceMap: {},
        };
        fileContentManagerStub.getSortedFiles.returns(sortedFiles);
        fileContentManagerStub.fileNamesMap.set('file1.d.ts', 'file1.d.ts');
        fileContentManagerStub.fileNamesMap.set('file2.d.ts', 'file2.d.ts');
        fileContentManagerStub.readFileContent
          .withArgs('file1.d.ts').returns(fileContentObj1)
          .withArgs('file2.d.ts').returns(fileContentObj2);
        harFilesRecord.set('file2.d.ts',{});
        await reObfuscate(arkObfuscatorStub, harFilesRecord, printObfLogger, projectConfig);
        expect(harFilesRecord.has('file1.d.ts')).to.be.true;
        expect(harFilesRecord.get('file1.d.ts')).to.deep.equal({
          sourcePath: 'file1.d.ts',
          originalDeclarationCachePath: 'file1.d.ts',
          originalDeclarationContent: 'content1'
        });
        expect(harFilesRecord.has('file2.d.ts')).to.be.true;
        expect(harFilesRecord.get('file2.d.ts')).to.deep.equal({});
      });

      mocha.it('10-5-2: should writeArkguardObfuscatedSourceCode if is not declaraiont file', async function () {
        const harFilesRecord = new Map();
        const logger = {};
        const projectConfig = {};
        const sortedFiles = ['file1.ts', 'file2.ts'];
        const fileContentObj1 = {
          moduleInfo: {
            originSourceFilePath: 'file1.ts',
            buildFilePath: 'file1.ts',
            content: 'content1',
          },
          previousStageSourceMap: {},
        };
        const fileContentObj2 = {
          moduleInfo: {
            originSourceFilePath: 'file2.ts',
            buildFilePath: 'file2.ts',
            content: 'content2',
          },
          previousStageSourceMap: {},
        };
        let writeArkguardObfuscatedSourceCodeStub = sinon.stub(arkUtils, 'writeArkguardObfuscatedSourceCode');
        fileContentManagerStub.getSortedFiles.returns(sortedFiles);
        fileContentManagerStub.fileNamesMap.set('file1.ts', 'file1.ts');
        fileContentManagerStub.fileNamesMap.set('file2.ts', 'file2.ts');
        fileContentManagerStub.readFileContent
          .withArgs('file1.ts').returns(fileContentObj1)
          .withArgs('file2.ts').returns(fileContentObj2);
        await reObfuscate(arkObfuscatorStub, harFilesRecord, printObfLogger, projectConfig);
        expect(writeArkguardObfuscatedSourceCodeStub.calledTwice).to.be.true;
      });
    });
    
    mocha.describe('10-6: writeObfuscationCaches', function () {
      let testCacheDir: string = '';
      let timePerformanceFilePath: string = '';
      let memoryPerformanceFilePath: string = '';

      mocha.beforeEach(function () {
        printerTimeAndMemDataConfig.mTimeAndMemPrinter = true;
        testCacheDir = '../../test/ark_compiler_ut/testdata/';
        TimeAndMemTimeTracker.obfuscationCacheDir = testCacheDir;
        timePerformanceFilePath = path.join(testCacheDir, 'timePerformanceData.json');
        memoryPerformanceFilePath = path.join(testCacheDir, 'memoryPerformanceData.json');
        if (!fs.existsSync(testCacheDir)) {
          fs.mkdirSync(testCacheDir, { recursive: true });
        }
      });

      mocha.afterEach(function () {
        if (fs.existsSync(timePerformanceFilePath)) {
          fs.unlinkSync(timePerformanceFilePath);
        }
        if (fs.existsSync(memoryPerformanceFilePath)) {
          fs.unlinkSync(memoryPerformanceFilePath);
        }
        if (fs.existsSync(testCacheDir)) {
          fs.rmdirSync(testCacheDir, { recursive: true });
        }
      });

      mocha.it('10-6-1: should call enableTimeAndMemoryPrint when perf is ADVANCED', async function () {
        this.rollup.share.projectConfig.perf = PerfMode.ADVANCED;
        const sourceProjectConfig = Object.assign(this.rollup.share.arkProjectConfig, this.rollup.share.projectConfig);
        await writeObfuscationCaches(sourceProjectConfig, undefined);
        await new Promise(resolve => setTimeout(resolve, 100));
        expect(fs.existsSync(timePerformanceFilePath)).to.be.true;
        expect(fs.existsSync(memoryPerformanceFilePath)).to.be.true;
      });

      mocha.it('10-6-2: should not call enableTimeAndMemoryPrint when perf is not ADVANCED', async function () {
        this.rollup.share.projectConfig.perf = PerfMode.NORMAL;
        const sourceProjectConfig = Object.assign(this.rollup.share.arkProjectConfig, this.rollup.share.projectConfig);
        await writeObfuscationCaches(sourceProjectConfig, undefined);
        await new Promise(resolve => setTimeout(resolve, 100));
        expect(fs.existsSync(timePerformanceFilePath)).to.be.false;
        expect(fs.existsSync(memoryPerformanceFilePath)).to.be.false;
      });
    });
  });
});