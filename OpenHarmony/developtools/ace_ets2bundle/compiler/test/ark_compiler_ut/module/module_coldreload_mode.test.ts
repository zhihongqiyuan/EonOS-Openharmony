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
import mocha from 'mocha';
import path from "path";
import fs from "fs";
import sinon from 'sinon';

import {
  ENTRYABILITY_TS_PATH_DEFAULT,
  ENTRYABILITY_JS_PATH_DEFAULT,
  INDEX_ETS_PATH_DEFAULT
} from '../mock/rollup_mock/common';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { ModuleColdreloadMode } from '../../../lib/fast_build/ark_compiler/module/module_coldreload_mode';
import { RELEASE } from '../../../lib/fast_build/ark_compiler/common/ark_define';
import { toUnixPath } from '../../../lib/utils';
import {
  SOURCEMAPS,
  EXTNAME_TS,
  EXTNAME_ETS
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import {
  ES2ABC_PATH,
  SYMBOLMAP_MAP,
  DEFAULT_ETS,
  DEBUG_INFO,
  SIMBOL_TABLE
} from '../mock/rollup_mock/path_config';
import {
  ENTRYABILITY_TS_PATH,
  INDEX_ETS_PATH,
  FILE,
  SOURCE
} from '../mock/rollup_mock/common';
import { SourceMapGenerator } from '../../../lib/fast_build/ark_compiler/generate_sourcemap';
import {
  ArkTSInternalErrorDescription,
  ErrorCode
} from '../../../lib/fast_build/ark_compiler/error_code';
import { 
  CommonLogger,
  LogData,
  LogDataFactory
} from '../../../lib/fast_build/ark_compiler/logger';

mocha.describe('test module_coldreload_mode file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test the error message of the ModuleColdreloadMode constructor', function () {
    this.rollup.coldReload(RELEASE);
    this.rollup.share.projectConfig.oldMapFilePath = '';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_COLD_RELOAD_FAILED_INCORRECT_SYMBOL_MAP_CONFIG,
      ArkTSInternalErrorDescription,
      'Coldreload failed, symbolMap file is not correctly configured.'
    );
    const stub = sinon.stub(CommonLogger.getInstance(this.rollup).getLoggerFromErrorCode(errInfo.code), 'printErrorAndExit');
    try {
      new ModuleColdreloadMode(this.rollup);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo)).to.be.true;
    stub.restore();
  });

  mocha.it('1-1-1: test the error message of the ModuleColdreloadMode constructor ' +
    'without getHvigorConsoleLogger', function () {
    this.rollup.coldReload(RELEASE);
    this.rollup.share.projectConfig.oldMapFilePath = '';
    const errInfo: LogData = LogDataFactory.newInstance(
      ErrorCode.ETS2BUNDLE_INTERNAL_COLD_RELOAD_FAILED_INCORRECT_SYMBOL_MAP_CONFIG,
      ArkTSInternalErrorDescription,
      'Coldreload failed, symbolMap file is not correctly configured.'
    );
    CommonLogger.destroyInstance();
    const getHvigorConsoleLogger = this.rollup.share.getHvigorConsoleLogger;
    this.rollup.share.getHvigorConsoleLogger = undefined;
    const stub = sinon.stub(CommonLogger.getInstance(this.rollup), 'throwArkTsCompilerError');
    try {
      new ModuleColdreloadMode(this.rollup);
    } catch (e) {
    }
    expect(stub.calledWith(errInfo.toString())).to.be.true;
    CommonLogger.destroyInstance();
    this.rollup.share.getHvigorConsoleLogger = getHvigorConsoleLogger;
    stub.restore();
  });

  mocha.it('1-2: test addColdReloadArgs under cold reload debug', function () {
    this.rollup.coldReload();
    this.rollup.share.projectConfig.oldMapFilePath = DEFAULT_ETS;
    const moduleMode = new ModuleColdreloadMode(this.rollup);
    moduleMode.addColdReloadArgs();
    expect(moduleMode.cmdArgs[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(moduleMode.cmdArgs[1] === DEBUG_INFO).to.be.true;
    expect(moduleMode.cmdArgs[2] === SIMBOL_TABLE).to.be.true;
    expect(moduleMode.cmdArgs[3].indexOf(SYMBOLMAP_MAP) > 0).to.be.true;
  });

  mocha.it('1-3: test addColdReloadArgs under cold reload release', function () {
    this.rollup.coldReload(RELEASE);
    this.rollup.share.projectConfig.oldMapFilePath = DEFAULT_ETS;
    const moduleMode = new ModuleColdreloadMode(this.rollup);
    moduleMode.addColdReloadArgs();
    expect(moduleMode.cmdArgs[0].indexOf(ES2ABC_PATH) > 0).to.be.true;
    expect(moduleMode.cmdArgs[1] === DEBUG_INFO).to.be.false;
    expect(moduleMode.cmdArgs[1] === SIMBOL_TABLE).to.be.true;
    expect(moduleMode.cmdArgs[2].indexOf(SYMBOLMAP_MAP) > 0).to.be.true;
  });

  mocha.it('1-4: test updateSourceMapFromFileList under cold reload debug', function () {
    this.rollup.coldReload();
    this.rollup.share.projectConfig.oldMapFilePath = DEFAULT_ETS;
    const moduleMode = new ModuleColdreloadMode(this.rollup);
    const fileList = this.rollup.getModuleIds();
    const sourceMapGenerator: SourceMapGenerator = SourceMapGenerator.initInstance(this.rollup);

    for (const filePath of fileList) {
      if (filePath.endsWith(EXTNAME_TS) || filePath.endsWith(EXTNAME_ETS)) {
        const sourceMap: Map<string, string[]> = new Map<string, string[]>();
        const relativeSourceFilePath =
          toUnixPath(filePath.replace(this.rollup.share.projectConfig.projectTopDir + path.sep, ''));
        sourceMap[FILE] = path.basename(relativeSourceFilePath);
        sourceMap[SOURCE] = [relativeSourceFilePath];
        sourceMapGenerator.updateSourceMap(filePath, sourceMap);
      }
    }
    const fileListArray: Array<string> = [
      path.join(this.rollup.share.projectConfig.modulePath, ENTRYABILITY_TS_PATH_DEFAULT),
      path.join(this.rollup.share.projectConfig.modulePath, INDEX_ETS_PATH_DEFAULT)
    ];
    moduleMode.updateSourceMapFromFileList(fileListArray);
    const sourceMapFilePath: string = path.join(this.rollup.share.projectConfig.patchAbcPath, SOURCEMAPS);
    if (sourceMapFilePath && fs.existsSync(sourceMapFilePath)) {
      const testObject = fs.readFileSync(sourceMapFilePath).toString();
      expect(testObject.indexOf(ENTRYABILITY_TS_PATH_DEFAULT) > 0 ||
        testObject.indexOf(ENTRYABILITY_JS_PATH_DEFAULT) > 0 ||
        testObject.indexOf(INDEX_ETS_PATH_DEFAULT) > 0).to.be.true;
    }

    let newSourceMaps = sourceMapGenerator.getSourceMaps();
    for (const key of Object.keys(newSourceMaps)) {
      delete newSourceMaps[key];
    }
    SourceMapGenerator.cleanSourceMapObject();
  });
});