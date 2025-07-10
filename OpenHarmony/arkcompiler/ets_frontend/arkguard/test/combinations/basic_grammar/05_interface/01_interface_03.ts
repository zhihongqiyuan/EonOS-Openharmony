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

interface I1 {
    i1P1: number;
    i1P2(): void;
}

let a1: I1 = {
  i1P1: 1,
  i1P2: ()=>{},
}

assert(a1.i1P1 === 1)
assert(a1.i1P2() === undefined)

interface I2 extends I1 {
  i2P1: string;
}

let a2: I2 = {
  i1P1: 1,
  i1P2: ()=>{},
  i2P1: "1",
}

assert(a2.i1P1 === 1)
assert(a2.i1P2() === undefined)
assert(a2.i2P1 === "1")
  
class C1 {
  public foo(x: any) { return x;}
  private x = 1;
}
let c1 = new C1();
assert(c1.foo(10) === 10)

interface I3 extends C1 {
  other(x: any): any;
}

  
class C2 {
  public foo(x: any) {return x;}
  private x = 1;
}
let c2 = new C2();
assert(c2.foo(2) === 2)

class D {
  public foo1(x: any) {return x;}
  private x1=1;
}
let d = new D();
assert(d.foo1(3) === 3)

class e {
  public foo2(x: any) {return x;}
  private x2=1;
}
let E = new e();
assert(E.foo2(4) === 4)

interface I4 extends C2,D,e {
  other(x:any): any;
}