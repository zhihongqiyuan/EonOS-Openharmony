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

import assert from 'assert'
// Only use for testing importEqualsDeclaration in toplevel
import temp1 = require('fs');
export import temp2 = require('fs');
module X {
  export module Y {
    export interface Z {
      a:number;
    }
  }
  export interface Y {
    b:string;
  }
}
let a:X.Y.Z = {a:1}
assert(a.a === 1);
let b:X.Y = {b:"1"}
assert(b.b === "1");

module A {
  export module B {
    export class C {
      c: boolean = true;
    }
  }
}

var c: A.B.C = new A.B.C();
assert(c.c === true)

module M {
  export namespace N {
    export module M2 {
      export interface I {
        d: number;
      }
    }
  }
}
let d: M.N.M2.I = {d:2}
assert(d.d === 2)

type A = number;
declare const Q1:number;
declare namespace Q2 {
  export {A}
}
let e:Q2.A = 3;
assert(e ===3);

namespace ns1 {
  namespace ns2 {
      export var temp3 : string = "test-importEqualsDeclaration";
  }
  import temp4 = ns2.temp3;
  export import temp5 = ns2.temp3;
  assert(temp4 === "test-importEqualsDeclaration")
  assert(temp5 === "test-importEqualsDeclaration")
}

namespace ns1 {
  assert(temp5 === "test-importEqualsDeclaration")
}

assert(ns1.temp5 === "test-importEqualsDeclaration")
export {}