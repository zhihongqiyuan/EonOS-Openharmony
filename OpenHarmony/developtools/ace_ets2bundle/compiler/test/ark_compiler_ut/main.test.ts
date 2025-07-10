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
import { expect } from 'chai';

import { setEntryArrayForObf, projectConfig } from '../../main';

mocha.describe('test main file api', function () {
    mocha.it('1-1: test setEntryArrayForObf', function () {
        const entryFile1 = 'pages/mainPage';
        const entryFile2 = 'worker';
        const entryFile3 = 'Worker';
        const entryFile4 = 'index';
        const entryFile5 = '';
        projectConfig.entryArrayForObf = [];
        setEntryArrayForObf(entryFile1);
        setEntryArrayForObf(entryFile2, entryFile3);
        setEntryArrayForObf(entryFile4);
        setEntryArrayForObf(entryFile5);
        expect(projectConfig.entryArrayForObf[0]).to.equal('pages/mainPage');
        expect(projectConfig.entryArrayForObf[1]).to.equal('worker/Worker');
        expect(projectConfig.entryArrayForObf[2]).to.equal('index');
        expect(projectConfig.entryArrayForObf[3]).to.equal('');
    });
});