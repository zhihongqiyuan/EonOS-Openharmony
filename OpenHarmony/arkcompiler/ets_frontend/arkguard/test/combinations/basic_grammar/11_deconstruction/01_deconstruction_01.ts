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

//array
let a1=1;
let arr = [0,a1,3];
assert(a1 === 1)
assert(arr[0] === 0)
assert(arr[1] === 1)
assert(arr[2] === 3)
let [b1,c1,d] = arr;
assert(b1 === 0);
assert(c1 === 1);
assert(d === 3);
[,,] = [...arr];

//object
let obj = {a1:1, b1:a1+1, c1:3}
assert(obj.a1 === 1);
assert(obj.b1 === 2);
assert(obj.c1 === 3);
let {c1:b2,a1:c2,b1:d2}=obj;
assert(b2 === 3);
assert(c2 === 1);
assert(d2 === 2);

class C1 {
  a1:number;
  b1:number;
  c1:number;
  constructor([_a,_b,_c]:Array<number>) {
    this.a1 = _a;
    this.b1 = _b;
    this.c1 = _c;
  }
}
function f1([a1,b1,c1]: Array<number>, {a1:x,b1:y,c1:d}:C1) {
  a1;b1;c1;x;y;d;
  assert(a1 === 10);
  assert(b1 === 21);
  assert(c1 === 20);
  assert(x === 0);
  assert(y === 1);
  assert(d === 3);
}
f1([10,a1+20,20],new C1([...arr]))