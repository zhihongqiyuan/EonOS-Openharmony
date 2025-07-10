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

let friendA2: { getX2(o: number): number, setX2(o: number, v: number): void };

namespace ts {
  class A { 
    x: number;

    constructor (v: number) {
      this.x = v;
    }

    getX2 () {
      return this.x;
    }

    obj() {
      friendA2 = {
        getX2(obj):number { return obj },
        setX2(obj, value) { obj = value }
      };
    }
  };

  class B {
    constructor(public a: number, private x01: number = 1, protected x02: string = '', readonly x03: number = 2) {
      const x = friendA2.getX2(a); // ok
      friendA2.setX2(a, x + 1); // ok
    }
  };

  const a = new A(41);
  a.obj();
  let c = 1;
  const b = new B(c);
  a.getX2();
}