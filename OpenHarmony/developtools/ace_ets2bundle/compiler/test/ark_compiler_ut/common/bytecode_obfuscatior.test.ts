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

import mocha from 'mocha';
import { expect } from 'chai';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { RELEASE } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { BytecodeObfuscator } from '../../../lib/fast_build/ark_compiler/bytecode_obfuscator';
import { OBFUSCATE_RULE_BYTECODE_OBFUSCATION_ENABLE_PATH } from '../mock/rollup_mock/path_config';
import { ModuleInfo } from '../../../lib/fast_build/ark_compiler/module/module_mode';

mocha.describe('test bytecodeObfuscator file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
    this.rollup.build(RELEASE);
    this.rollup.share.projectConfig.compatibleSdkVersion=14;
    this.rollup.share.projectConfig.obfuscationOptions = {
      'selfConfig': {
        'ruleOptions': {
          'enable': true,
          'rules': [OBFUSCATE_RULE_BYTECODE_OBFUSCATION_ENABLE_PATH]
        },
        'consumerRules': [],
      },
      'dependencies': {
        'libraries': [],
        'hars': []
      },
      obfuscationCacheDir:""
    };
    this.rollup.share.arkProjectConfig.obfuscationMergedObConfig={
      options:{
        bytecodeObf: { enable: true, enhanced: false, debugging: false },
        disableObfuscation: false,
        enablePropertyObfuscation: true,
        enableStringPropertyObfuscation: false,
        enableToplevelObfuscation: true,
        enableFileNameObfuscation: true,
        enableExportObfuscation: true,
        printKeptNames: false,
        removeComments: false,
        compact: false,
        removeLog: false,
        printNameCache: '',
        printKeptNamesPath: '',
        applyNameCache: ''
      },
      reservedPropertyNames: [],
      reservedGlobalNames: [],
      reservedNames: [],
      reservedFileNames: [],
      keepComments: [],
      keepSourceOfPaths: [],
      universalReservedPropertyNames: [],
      universalReservedGlobalNames: [],
      keepUniversalPaths: [],
      excludeUniversalPaths: [],
      excludePathSet: new Set<string>()
    }
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test initBytecodeObfuscator when obfuscation disabled', function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled=true;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=false;
    BytecodeObfuscator.initForTest(this.rollup.share);
    expect(BytecodeObfuscator.enable === false).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });

  mocha.it('1-2: test init when obfuscation is arkguard', function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled=true;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=false;
    BytecodeObfuscator.initForTest(this.rollup.share);
    expect(BytecodeObfuscator.enable === false).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });

  mocha.it('1-3: test init when obfuscation is bytecode obfuscation', function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled=false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=true;
    BytecodeObfuscator.initForTest(this.rollup.share);
    expect(BytecodeObfuscator.enable === true).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });
  
  mocha.it('2-1: test collectSkipModuleName add skip module pkgName correctly with keep bytecode har', function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled=false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=true;
    this.rollup.share.arkProjectConfig.obfuscationMergedObConfig.keepSourceOfPaths.push(
      'xxxx/xxxx/kohtpunnbppmmzjb0tw2ry4=/oh_modules/har',
      'xxxx/xxxx/kohtpunnbppmmzjb0tw2ry4=/oh_modules/har1'
    );
    this.rollup.share.projectConfig.byteCodeHarInfo={
      'har':{
        "compatibleSdkVersion":"14",
        "abcPath":"adsasd"
      },
    }
    BytecodeObfuscator.initForTest(this.rollup.share)
    BytecodeObfuscator.getInstance().collectSkipModuleName();
    expect(BytecodeObfuscator.getInstance().bytecodeObfuscateConfig.skippedRemoteHarList.has('har')).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });

  mocha.it('2-2: test collectSkipModuleName add skip module pkgName correctly with bytecode har', function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled=false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=true;
    this.rollup.share.projectConfig.byteCodeHarInfo={
      'Har_version_11':{
        "compatibleSdkVersion":"11",
      },
      'Har_version_14':{
        "compatibleSdkVersion":"14",
      }
    }
    BytecodeObfuscator.initForTest(this.rollup.share)
    BytecodeObfuscator.getInstance().collectSkipModuleName();
    expect(BytecodeObfuscator.getInstance().bytecodeObfuscateConfig.skippedRemoteHarList.has('Har_version_11')).to.be.true;
    expect(BytecodeObfuscator.getInstance().bytecodeObfuscateConfig.skippedRemoteHarList.has('Har_version_14')).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });
  

  mocha.it('3-1: test generateObfCmd add config path correctly',function(){
    this.rollup.share.arkProjectConfig.isArkguardEnabled=false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=true;
    BytecodeObfuscator.initForTest(this.rollup.share);
    expect(BytecodeObfuscator.enable === true).to.be.true;
    BytecodeObfuscator.getInstance().generateObfCmd();
    expect(BytecodeObfuscator.getInstance().cmdArgs.includes('config.json')).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });

  mocha.it('3-2: test generateObfCmd add debug correctly',function(){
    this.rollup.share.arkProjectConfig.isArkguardEnabled=false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled=true;
    this.rollup.share.arkProjectConfig.obfuscationMergedObConfig.options.bytecodeObf.debugging=true;
    BytecodeObfuscator.initForTest(this.rollup.share);
    expect(BytecodeObfuscator.enable === true).to.be.true;
    BytecodeObfuscator.getInstance().generateObfCmd();
    expect(BytecodeObfuscator.getInstance().cmdArgs.length==4).to.be.true;
    expect(BytecodeObfuscator.getInstance().cmdArgs.includes('--debug')).to.be.true;
    expect(BytecodeObfuscator.getInstance().cmdArgs.includes('--debug-file')).to.be.true;
    expect(BytecodeObfuscator.getInstance().cmdArgs.includes('debug.log')).to.be.true;
    expect(BytecodeObfuscator.getInstance().cmdArgs.includes('config.json')).to.be.true;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });

    mocha.it("4-1: test convertAbstractPathToRecordName successfully and remove originPath",function () {
    this.rollup.share.arkProjectConfig.isArkguardEnabled = false;
    this.rollup.share.arkProjectConfig.isBytecodeObfEnabled = true;
    this.rollup.share.arkProjectConfig.obfuscationMergedObConfig.options.bytecodeObf.debugging = true;
    const originPath = 'ProblemReproduction/oh_modules/.ohpm/reflect-metadata@0.2.1/oh_modules/reflect-metadata/Reflect.js';
    const recordName = '&reflect-meatadata|0.2.1|Reflect.js';
    this.rollup.share.arkProjectConfig.obfuscationMergedObConfig.keepSourceOfPaths = [originPath];
    BytecodeObfuscator.initForTest(this.rollup.share);
    let moduleInfos: Map<String, ModuleInfo> = new Map();
    moduleInfos.set(
      originPath,
      new ModuleInfo(
        originPath,
        'oh_modules/reflect-metadata/Reflect.js',
        true,
        '&reflect-meatadata|0.2.1|Reflect.js',
        'Reflect.js',
        'reflect-metadata',
        'js'
      )
    );
    BytecodeObfuscator.getInstance().convertAbstractPathToRecordName(moduleInfos);
    const keepPaths =
      BytecodeObfuscator.getInstance().bytecodeObfuscateConfig.obfuscationRules
        .keepOptions.keepPaths;
    expect(keepPaths.has(recordName)).to.be.true;
    expect(keepPaths.has(originPath)).to.be.false;
    BytecodeObfuscator.cleanBcObfuscatorObject();
  });
});