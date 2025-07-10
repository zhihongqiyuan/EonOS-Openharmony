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

var v1 = function f() {
    let v1 = 'test';
    return v1;
}
v1();
assert(v1() === 'test');

var v2 = {
    c: function () {
        return v2;
    },
    d: function f() {
        return v2.c;
    }
}
v2.c();
v2.d();
assert(v2.c() === v2);
assert(v2.d()() === v2);


const {
    B = function () {
      let B = 'binding';
      return B;
    },
    C = function g() {
      let C = 'binding';
      return C;
    }
} = {B: undefined, C: ()=>{
  return 'test';
}}
B();
C();
assert(B() == 'binding');
assert(C() === 'test');

var x = function g() {
  return g;
}
var y = function g() {
  return y;
}
x();
y()();
assert(x() === x);
assert(y()() === y);

var z = function f(para: any, ...paras:any):any {
  return arguments;
}
z(1,2,3);
assert(z(1,2,3)['0'] === 1);
assert(z(1,2,3)['1'] === 2);
assert(z(1,2,3)['2'] === 3);