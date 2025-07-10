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

let value1: number = 1;
assert.strictEqual(value1, 1);

function func1(): number {
  return value1;
}
assert.strictEqual(func1(), 1);


function func2(flag: boolean): number {
  let value2: number = 2;
  {
    let value3: number = 3;
    assert.strictEqual(value3, 3);
  }
  if (flag) {
    return value2;
  } else {
    // @ts-expect-error
    return value3; // This will cause an error as value3 is not defined in this scope
  }
}

assert.strictEqual(func2(true), 2);

try {
  func2(false);
} catch (e) {
  assert.strictEqual((e as ReferenceError).name, 'ReferenceError');
  assert.strictEqual((e as ReferenceError).message, 'value3 is not defined');
}

function func3(condition: boolean, value4: number): number {
  if (condition) {
    let value4: number = 100;
    return value4;
  }

  return value4;
}

assert.strictEqual(func3(false, 0), 0);
assert.strictEqual(func3(true, 0), 100);

function func4(): string {
  let func5: () => string;

  if (true) {
      let message: string = 'hello';
      func5 = function() {
          return message;
      }
  }

  return func5();
}

assert.strictEqual(func4(), 'hello');