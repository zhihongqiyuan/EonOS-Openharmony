/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

namespace ts {
  let friendA1: { getX(o: A1): number, setX(o: A1, m: number): void };
  class A1 { 
    x: number;

    constructor (m: number) {
      this.x = m;
    }

    getX () {
      return this.x;
    }

    obj() {
      friendA1 = {
        getX(obj) { return obj.x },
        setX(obj, value) { obj.x = value }
      };
    }
  };

  class B1 {
    constructor(a: A1) {
      const x = friendA1.getX(a); // ok
      friendA1.setX(a, x + 1); // ok
    }
  };

  const a = new A1(41);
  a.obj();
  const b = new B1(a);
  a.getX();
}