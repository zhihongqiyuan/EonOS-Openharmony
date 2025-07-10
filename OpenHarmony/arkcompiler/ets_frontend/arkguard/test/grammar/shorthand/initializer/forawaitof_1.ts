/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
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

import assert from 'assert';

namespace A {
  let xFnexp, fnexp;
  const f1 = 0;
  const f2 = 0;
  async function fn() {
    for await ({ xFnexp = f1, fnexp = f2 } of [{ xFnexp, fnexp }]) {
      assert.strictEqual(xFnexp, f1);
      assert.strictEqual(fnexp, f2);
    }
  }
}

class TestClass {
  xFnexp: string = "123";
}