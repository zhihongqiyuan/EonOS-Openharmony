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
  let friendA: { getX(o: A): number, setX(o: A, v: number): void };

  class A { 
    x: number;

    constructor (v: number) {
      this.x = v;
    }

    getX () {
      return this.x;
    }

    obj() {
      friendA = {
        getX(obj) { return obj.x },
        setX(obj, value) { obj.x = value }
      };
    }
  };

  class B {
    constructor(public a: A, private x01: number = 1, protected x02: string = '', readonly x03: number = 2) {
      const x = friendA.getX(a); // ok
      friendA.setX(a, x + 1); // ok
    }
  };

  const a = new A(41);
  a.obj();
  const b = new B(a);
  a.getX();
}