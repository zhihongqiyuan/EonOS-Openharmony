/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the 'License');
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an 'AS IS' BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import assert from 'assert';

const var1: string = 'A';
assert.strictEqual(var1, 'A');

try {
  // @ts-expect-error
  var1 = 'B';
} catch (e) {
  assert.strictEqual((e as TypeError).name, 'TypeError');
  assert.strictEqual((e as TypeError).message, 'Assignment to constant variable.');
}

const var2: { name1: string, version1: string } = {
  name1: 'arkguard',
  version1: '1.0.0'
}

assert.strictEqual(var2.name1, 'arkguard');
assert.strictEqual(var2.version1, '1.0.0');

try {
  // @ts-expect-error
  var2 = 'another';
} catch (e) {
  assert.strictEqual((e as TypeError).name, 'TypeError');
  assert.strictEqual((e as TypeError).message, 'Assignment to constant variable.');
}

var2.version1 = '1.0.1'
assert.strictEqual(var2.name1, 'arkguard');
assert.strictEqual(var2.version1, '1.0.1');

const {name1: name2, version1: version2}: {name1: string, version1: string} = var2;
assert.strictEqual(name2, 'arkguard');
assert.strictEqual(version2, '1.0.1');