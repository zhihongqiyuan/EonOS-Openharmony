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

import {
  checkArkCompilerCacheInfo,
  utCache
} from '../../../lib/fast_build/ark_compiler/cache';
import {
  IS_CACHE_INVALID,
  ARK_COMPILER_META_INFO,
  RELEASE
} from '../../../lib/fast_build/ark_compiler/common/ark_define';
import RollUpPluginMock from '../mock/rollup_mock/rollup_plugin_mock';
import { SDK_VERSION_MOCK } from '../mock/rollup_mock/common';

mocha.describe('test cache file api', function () {
  mocha.before(function () {
    this.rollup = new RollUpPluginMock();
  });

  mocha.after(() => {
    delete this.rollup;
  });

  mocha.it('1-1: test checkArkCompilerCacheInfo under build debug', function () {
    this.rollup.build();
    this.rollup.clearCache();
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;

    this.rollup.share.projectConfig.setCompilerVersion(SDK_VERSION_MOCK);
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;
  });

  mocha.it('1-2: test checkArkCompilerCacheInfo under build release', function () {
    this.rollup.build(RELEASE);
    this.rollup.clearCache();
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;

    this.rollup.share.projectConfig.setCompilerVersion(SDK_VERSION_MOCK);
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;
  });

  mocha.it('1-3: test checkArkCompilerCacheInfo under preview debug', function () {
    this.rollup.preview();
    this.rollup.clearCache();
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;

    this.rollup.share.projectConfig.setCompilerVersion(SDK_VERSION_MOCK);
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;

    this.rollup.mockProjectConfigMockParams();
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;
  });

  mocha.it('1-4: test checkArkCompilerCacheInfo under hot reload debug', function () {
    this.rollup.hotReload();
    this.rollup.clearCache();

    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;

    this.rollup.share.projectConfig.setCompilerVersion(SDK_VERSION_MOCK);
    checkArkCompilerCacheInfo(this.rollup);
    expect(this.rollup.cache.get(ARK_COMPILER_META_INFO).length > 0).to.be.true;
    expect(this.rollup.cache.get(IS_CACHE_INVALID) === true).to.be.true;
  });

  mocha.it('2-1: test getMetaInfo under build debug', function () {
    this.rollup.build();
    const metaInfoArr = JSON.stringify(this.rollup.cache.get(ARK_COMPILER_META_INFO));
    const returnInfo = JSON.stringify(utCache.getMetaInfo(this.rollup.share.projectConfig));
    expect(metaInfoArr === returnInfo).to.be.true;
  });

  mocha.it('2-2: test getMetaInfo under build release', function () {
    this.rollup.build(RELEASE);
    const metaInfoArr = JSON.stringify(this.rollup.cache.get(ARK_COMPILER_META_INFO));
    const returnInfo = JSON.stringify(utCache.getMetaInfo(this.rollup.share.projectConfig));
    expect(metaInfoArr === returnInfo).to.be.true;
  });

  mocha.it('2-3: test getMetaInfo under preview debug', function () {
    this.rollup.preview();
    const metaInfoArr = JSON.stringify(this.rollup.cache.get(ARK_COMPILER_META_INFO));
    const returnInfo = JSON.stringify(utCache.getMetaInfo(this.rollup.share.projectConfig));
    expect(metaInfoArr === returnInfo).to.be.true;
  });

  mocha.it('2-4: test getMetaInfo under hot reload debug', function () {
    this.rollup.hotReload();
    const metaInfoArr = JSON.stringify(this.rollup.cache.get(ARK_COMPILER_META_INFO));
    const returnInfo = JSON.stringify(utCache.getMetaInfo(this.rollup.share.projectConfig));
    expect(metaInfoArr === returnInfo).to.be.true;
  });
});