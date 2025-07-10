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
import fs from 'fs';
import path from 'path';
import { expect } from 'chai';
import * as sinon from 'sinon';
import * as ts from 'typescript';

import { EXPECT_INDEX_ETS } from './mock/rollup_mock/path_config';
import RollUpPluginMock from './mock/rollup_mock/rollup_plugin_mock';
import {
  addLocalPackageSet,
  compilerOptions,
  localPackageSet,
  needReCheckForChangedDepUsers,
  resetEtsCheck,
  serviceChecker,
  getMaxFlowDepth,
  MAX_FLOW_DEPTH_DEFAULT_VALUE,
  fileCache,
  getFileContentWithHash
} from '../../lib/ets_checker';
import { TS_BUILD_INFO_SUFFIX } from '../../lib/pre_define';
import {
  globalProgram,
  projectConfig
} from '../../main';

mocha.describe('test ets_checker file api', function () {
    mocha.before(function () {
        this.rollup = new RollUpPluginMock();
    });

    mocha.after(() => {
        delete this.rollup;
        const cacheFile: string = path.resolve(projectConfig.cachePath, '../.ts_checker_cache');
        if (fs.existsSync(cacheFile)) {
            fs.unlinkSync(cacheFile);
        }
        const tsBuildInfoFilePath: string = path.resolve(projectConfig.cachePath, '..', TS_BUILD_INFO_SUFFIX);
        if (fs.existsSync(tsBuildInfoFilePath)) {
            fs.unlinkSync(tsBuildInfoFilePath);
        }
        const tsBuildInfoLinterFilePath: string = tsBuildInfoFilePath + '.linter';
        if (fs.existsSync(tsBuildInfoLinterFilePath)) {
            fs.unlinkSync(tsBuildInfoLinterFilePath);
        }
    });

    mocha.it('1-1: test addLocalPackageSet for original ohmurl', function () {
        this.rollup.build();
        const rollupObject = this.rollup;
        const rollupFileList = rollupObject.getModuleIds();
        projectConfig.useNormalizedOHMUrl = false;
        for (const moduleId of rollupFileList) {
            if (fs.existsSync(moduleId)) {
                addLocalPackageSet(moduleId, rollupObject);
            }
        }
        expect(localPackageSet.has('entry')).to.be.true;
        localPackageSet.clear();
    });

    mocha.it('1-2: test addLocalPackageSet for normalized ohmurl', function () {
        this.rollup.build();
        const rollupObject = this.rollup;
        const rollupFileList = rollupObject.getModuleIds();
        projectConfig.useNormalizedOHMUrl = true;
        for (const moduleId of rollupFileList) {
            const moduleInfo: Object = rollupObject.getModuleInfo(moduleId);
            if (moduleInfo) {
                const metaInfo: Object = moduleInfo.meta;
                metaInfo.pkgName = 'pkgname';
            }
            if (fs.existsSync(moduleId)) {
                addLocalPackageSet(moduleId, rollupObject);
            }
        }
        expect(localPackageSet.has('pkgname')).to.be.true;
    });

    mocha.it('1-3: test getOrCreateLanguageService when dependency in oh-package.json change', function () {
        this.timeout(10000);
        this.rollup.build();
        let rollupObject = this.rollup;
        expect(needReCheckForChangedDepUsers).to.be.false;

        interface MockCacheStore {
            service: Object | undefined;
            pkgJsonFileHash: string;
            targetESVersion: number;
        }
        const mockServiceCache = {
            service: undefined,
            pkgJsonFileHash: '9f07917d395682c73a90af8f5796a2c6',
            targetESVersion: 8
        }
        let mockCache = new Map<string, MockCacheStore>();
        mockCache.set('service', mockServiceCache);
        rollupObject.share.cache = mockCache;
        rollupObject.share.depInfo = {enableIncre: true};
        rollupObject.share.projectConfig.pkgJsonFileHash = '26bde0f30dda53b0afcbf39428ec9851';
        Object.assign(projectConfig, rollupObject.share.projectConfig);

        // The current hash and the hash in cache of the dependency differs, should recheck
        serviceChecker([EXPECT_INDEX_ETS], null, null, null, rollupObject.share);
        expect(needReCheckForChangedDepUsers).to.be.true;
        expect(globalProgram.program != null).to.be.true;

        resetEtsCheck();
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program == null).to.be.true;

        // The current hash and the hash in cache of the dependency are the same, no need to recheck
        serviceChecker([EXPECT_INDEX_ETS], null, null, null, rollupObject.share);
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program != null).to.be.true;

        resetEtsCheck();
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program == null).to.be.true;
    });

    mocha.it('1-4: test getOrCreateLanguageService when paths in compilerOptions change', function () {
        this.timeout(10000);
        this.rollup.build();
        let rollupObject = this.rollup;
        process.env.compileTool = 'rollup';

        Object.assign(projectConfig, rollupObject.share.projectConfig);
        serviceChecker([EXPECT_INDEX_ETS], null, null, null, rollupObject.share);
        expect(JSON.stringify(compilerOptions.paths) === '{"*":["*","../../../../*","../*"]}').to.be.true;
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program != null).to.be.true;
        expect(compilerOptions.skipPathsInKeyForCompilationSettings).to.be.true;

        resetEtsCheck();
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program == null).to.be.true;
        expect(compilerOptions.skipPathsInKeyForCompilationSettings).to.be.true;

        interface MockCacheStore {
            service: Object | undefined;
            pkgJsonFileHash: string;
            targetESVersion: number;
        }
        const mockServiceCache = {
            service: undefined,
            pkgJsonFileHash: '9f07917d395682c73a90af8f5796a2c6',
            targetESVersion: 8
        }
        let mockCache = new Map<string, MockCacheStore>();
        mockCache.set('service', mockServiceCache);
        rollupObject.share.cache = mockCache;
        rollupObject.share.depInfo = {enableIncre: true};
        rollupObject.share.projectConfig.pkgJsonFileHash = '26bde0f30dda53b0afcbf39428ec9851';

        // The current hash of the dependency differs, and the paths in compilerOptions will change since resolveModulePaths change
        const resolveModulePaths = ['../testdata/expect'];
        serviceChecker([EXPECT_INDEX_ETS], null, resolveModulePaths, null, rollupObject.share);
        expect(JSON.stringify(compilerOptions.paths) === '{"*":["*","../../../../../../../testdata/expect/*"]}').to.be.true;
        expect(needReCheckForChangedDepUsers).to.be.true;
        expect(globalProgram.program != null).to.be.true;
        expect(compilerOptions.skipPathsInKeyForCompilationSettings).to.be.true;

        resetEtsCheck();
        expect(needReCheckForChangedDepUsers).to.be.false;
        expect(globalProgram.program == null).to.be.true;
        expect(compilerOptions.skipPathsInKeyForCompilationSettings).to.be.true;
    });
    mocha.it('1-5: test GetEmitHost of program', function () {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testGetEmitHost.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        expect(program.getEmitHost()).to.not.be.undefined;
    });
});

mocha.describe('getMaxFlowDepth', () => { 
    mocha.it('1-1: test should return the default value when maxFlowDepth is undefined', () => {
        const result = getMaxFlowDepth();
        expect(result).to.equal(MAX_FLOW_DEPTH_DEFAULT_VALUE);
    });
  
    mocha.it('1-2: test should return the default value and log a warning when maxFlowDepth is less than the minimum valid value', () => {
        const invalidMaxFlowDepth = 1999;
        projectConfig.projectArkOption = {
            tscConfig: {
                maxFlowDepth: invalidMaxFlowDepth
          }
        }
        const result = getMaxFlowDepth();
        expect(result).to.equal(MAX_FLOW_DEPTH_DEFAULT_VALUE);
      });

    mocha.it('1-3: test should return the value of maxFlowDepth when it is 2000 within the valid range', () => {
        const validMaxFlowDepth = 2000;
        projectConfig.projectArkOption = {
            tscConfig: {
                maxFlowDepth: validMaxFlowDepth
            }
        }
        const result = getMaxFlowDepth();
        expect(result).to.equal(validMaxFlowDepth);
    });
  
    mocha.it('1-4: test should return the value of maxFlowDepth when it is 3000 within the valid range', () => {
        const validMaxFlowDepth = 3000;
        projectConfig.projectArkOption = {
            tscConfig: {
                maxFlowDepth: validMaxFlowDepth
            }
        }
        const result = getMaxFlowDepth();
        expect(result).to.equal(validMaxFlowDepth);
    });

    mocha.it('1-5: test should return the value of maxFlowDepth when it is 65535 within the valid range', () => {
        const validMaxFlowDepth = 65535;
        projectConfig.projectArkOption = {
            tscConfig: {
                maxFlowDepth: validMaxFlowDepth
            }
        }
        const result = getMaxFlowDepth();
        expect(result).to.equal(validMaxFlowDepth);
    });
  
    mocha.it('1-6: test should return the default value and log a warning when maxFlowDepth is greater than the maximum valid value', () => {
        const invalidMaxFlowDepth = 65536;
        projectConfig.projectArkOption = {
            tscConfig: {
                maxFlowDepth: invalidMaxFlowDepth
            }
        }
        const result = getMaxFlowDepth();
        expect(result).to.equal(MAX_FLOW_DEPTH_DEFAULT_VALUE);
    });
});

mocha.describe('setProgramSourceFiles', () => { 
    mocha.it('1-1: test add new sourcefile to program', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        const fileContent: string = `
            let a = 1;
        `;
        let newSourceFile: ts.SourceFile = ts.createSourceFile('demo.ts', fileContent, ts.ScriptTarget.ES2021, true);
        expect(program.getSourceFiles().includes(newSourceFile)).to.be.false;
        expect(program.getSourceFile('demo.ts')).to.be.undefined;
        const origrinFileSize: number = program.getSourceFiles().length;
        program.setProgramSourceFiles(newSourceFile);
        const newFileSize: number = program.getSourceFiles().length;
        expect(program.getSourceFiles().includes(newSourceFile)).to.be.true;
        expect(program.getSourceFile('demo.ts')).to.not.be.undefined;
        expect(origrinFileSize === (newFileSize - 1)).to.be.true;
    });
  
    mocha.it('1-2: test should do nothing when adding an existed sourcefile to the program', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        const fileContent1: string = `let a = 1;`;
        let newSourceFile1: ts.SourceFile = ts.createSourceFile('demo.ts', fileContent1, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile1);
        const origrinFileSize: number = program.getSourceFiles().length;
        expect(program.getSourceFiles().includes(newSourceFile1)).to.be.true;
        expect(program.getSourceFile('demo.ts').text).to.be.equal('let a = 1;');
        const fileContent2: string = `let b = 1;`;
        let newSourceFile2: ts.SourceFile = ts.createSourceFile('demo.ts', fileContent2, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile2);
        const newFileSize: number = program.getSourceFiles().length;
        expect(program.getSourceFiles().includes(newSourceFile2)).to.be.false;
        expect(program.getSourceFile('demo.ts').text).to.be.equal('let a = 1;');
        expect(origrinFileSize === newFileSize).to.be.true;
    });
});

mocha.describe('initProcessingFiles and getProcessingFiles', () => { 
    mocha.it('1-1: test should return undefined when processingDefaultFiles and processingOtherFiles is undefined', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        let processingFiles: ts.SourceFile[] | undefined = program.getProcessingFiles();
        expect(processingFiles === undefined).to.be.true;
    });
  
    mocha.it('1-2: test should return array when processingDefaultFiles and processingOtherFiles is not undefined', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        program.initProcessingFiles();
        let processingFiles: ts.SourceFile[] | undefined = program.getProcessingFiles();
        expect(processingFiles === undefined).to.be.false;
    });

});

mocha.describe('refreshTypeChecker', () => { 
    mocha.it('1-1: test should recreate typeChecker and linterTypeChecker', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        const fileContent: string = `
            let a = 1;
            let b = x; // Error!
        `;
        let newSourceFile: ts.SourceFile = ts.createSourceFile('demo.ts', fileContent, ts.ScriptTarget.ES2021, true);
        newSourceFile.originalFileName = newSourceFile.fileName;
        newSourceFile.resolvedPath = 'demo.ts';
        newSourceFile.path = 'demo.ts';
        program.processImportedModules(newSourceFile);
        program.setProgramSourceFiles(newSourceFile);
        program.refreshTypeChecker();
        const diagnostics: readonly ts.Diagnostic[] = program.getSemanticDiagnostics(newSourceFile);
        expect(diagnostics[0].messageText === `Cannot find name 'x'.`);
        expect(diagnostics.length === 1).to.be.true;
    });
});

mocha.describe('deleteProgramSourceFiles', () => { 
    mocha.it('1-1: test should delete when sourcefiles are exist in program', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        const fileContent1: string = `
            let a = 1;
        `;
        let newSourceFile1: ts.SourceFile = ts.createSourceFile('demo1.ts', fileContent1, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile1);
        expect(program.getSourceFiles().includes(newSourceFile1)).to.be.true;
        expect(program.getSourceFile('demo1.ts')).to.not.be.undefined;
        const fileContent2: string = `
            let b = 1;
        `;
        let newSourceFile2: ts.SourceFile = ts.createSourceFile('demo2.ts', fileContent2, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile2);
        expect(program.getSourceFiles().includes(newSourceFile2)).to.be.true;
        expect(program.getSourceFile('demo2.ts')).to.not.be.undefined;
        const origrinFileSize: number = program.getSourceFiles().length;
        const deleteFileNames: string[] = ['demo1.ts', 'demo2.ts'];
        program.deleteProgramSourceFiles(deleteFileNames);
        expect(program.getSourceFiles().includes(newSourceFile1)).to.be.false;
        expect(program.getSourceFile('demo1.ts')).to.be.undefined;
        expect(program.getSourceFiles().includes(newSourceFile2)).to.be.false;
        expect(program.getSourceFile('demo2.ts')).to.be.undefined;
        const newFileSize: number = program.getSourceFiles().length;
        expect(origrinFileSize === (newFileSize + 2)).to.be.true;
    });

    mocha.it('1-2: test should do nothing when sourcefiles are not exist in program', () => {
        const compilerOptions: ts.CompilerOptions = {
            target: ts.ScriptTarget.ES2021
        };
        const fileNames: string[] = ['../testdata/testfiles/testProgramSourceFiles.ts'];
        let program: ts.Program = ts.createProgram(fileNames, compilerOptions);
        const fileContent1: string = `
            let a = 1;
        `;
        let newSourceFile1: ts.SourceFile = ts.createSourceFile('demo1.ts', fileContent1, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile1);
        expect(program.getSourceFiles().includes(newSourceFile1)).to.be.true;
        expect(program.getSourceFile('demo1.ts')).to.not.be.undefined;
        const fileContent2: string = `
            let b = 1;
        `;
        let newSourceFile2: ts.SourceFile = ts.createSourceFile('demo2.ts', fileContent2, ts.ScriptTarget.ES2021, true);
        program.setProgramSourceFiles(newSourceFile2);
        expect(program.getSourceFiles().includes(newSourceFile2)).to.be.true;
        expect(program.getSourceFile('demo2.ts')).to.not.be.undefined;
        const origrinFileSize: number = program.getSourceFiles().length;
        const deleteFileNames: string[] = ['demo3.ts', 'demo4.ts'];
        program.deleteProgramSourceFiles(deleteFileNames);
        expect(program.getSourceFiles().includes(newSourceFile1)).to.be.true;
        expect(program.getSourceFile('demo1.ts')).to.not.be.undefined;
        expect(program.getSourceFiles().includes(newSourceFile2)).to.be.true;
        expect(program.getSourceFile('demo2.ts')).to.not.be.undefined;
        const newFileSize: number = program.getSourceFiles().length;
        expect(origrinFileSize === newFileSize).to.be.true;
    });
});

mocha.describe('optimize createProgram', () => {
    const testFileName = 'test.txt';
    const testFileContent = 'Hello, world!';
    let readFileSyncStub: sinon.SinonStub;

    mocha.beforeEach(() => {
      fileCache.clear();
      readFileSyncStub = sinon.stub(fs, 'readFileSync').returns(Buffer.from(testFileContent));
    });
  
    mocha.afterEach(() => {
      sinon.restore();
    });

    mocha.it('1-1: test should return file content when file exists ', () => {
      const result = getFileContentWithHash(testFileName);
      expect(result).to.equal(testFileContent);
    });

    mocha.it('1-2: test should cache the file content after first read', () => {
        // First call - should read from file system
        const firstResult = getFileContentWithHash(testFileName);
        expect(fileCache.has(testFileName)).to.be.true;
        expect(fileCache.get(testFileName)).to.equal(testFileContent);
        
        // Second call - should use cache
        readFileSyncStub.resetHistory();
        const secondResult = getFileContentWithHash(testFileName);
        expect(readFileSyncStub.called).to.be.false;
        expect(secondResult).to.equal(firstResult);
    });
});