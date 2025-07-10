/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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
import fs from 'fs';
import JSON5 from 'json5';
import mocha from 'mocha';
import path from 'path';

mocha.describe('verify that the obfuscateWhiteList.json5 contains wildcards', () => {
  const OBFUSCATE_WHITELIST_DIR = path.resolve(__dirname, '../../');
  const arkUIWhitelistPath = path.join(OBFUSCATE_WHITELIST_DIR, './config/obfuscateWhiteList.json5');
  const arkUIWhitelist = JSON5.parse(fs.readFileSync(arkUIWhitelistPath, 'utf-8'));
  const wildcardsRegex = /[\*\?]/;

  mocha.it('should not contain wildcards(* or ?) in ReservedPropertyNames', () => {
    arkUIWhitelist.ReservedPropertyNames.forEach((name: string) => {
      expect(wildcardsRegex.test(name)).to.be.false;
    });  
  });  

  mocha.it('should not contain wildcards(* or ?) in OptimizedReservedPropertyNames', () => {
    arkUIWhitelist.OptimizedReservedPropertyNames.forEach((name: string) => {
      expect(wildcardsRegex.test(name)).to.be.false;
    });
  });
});
