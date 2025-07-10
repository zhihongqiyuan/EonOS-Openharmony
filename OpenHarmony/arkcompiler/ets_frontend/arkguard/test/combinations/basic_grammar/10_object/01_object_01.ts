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

const s01:unique symbol = Symbol();
class C01 {
  a01?: number;
  b01?: string;
  c01?: boolean;
  1?: number;
  "x"?: number;
  [s01]?: number;
}

let x01 = 10;
let y01 = {a01:20};
let s02:symbol;
namespace z01 {
  export let a01 = 30;
}
z01.a01;
let a01 = {
  a01:1,
  b01:"12",
  1:1,
  "2":"2",
  [3]:3,
  ["4"]:"4",
  [s02 = Symbol()]:5,
  [s01]:6,
  ["1"+"2"]:7,
  [1+2+3]:8,
  [x01]:9,
  [x01+3]:10,
  [y01.a01]:11,
  [z01.a01]:12,
  x01:()=>{ return 13},
  await:()=>14
}
assert(a01.a01 === 1);
assert(a01.b01 === '12');
assert(a01[1] === 1);
assert(a01["2"] === "2");
assert(a01[3] === 3);
assert(a01["4"] === "4");
assert(a01[s02] === 5);
assert(a01[s01] === 6);
assert(a01["12"] === 7);
assert(a01[6] === 8);
assert(a01[10] === 9);
assert(a01[13] === 10);
assert(a01[20] === 11);
assert(a01[30] === 12);
assert(a01.x01() === 13);
assert(a01.await() === 14);


function g01({b01,...a01}:{b01:number, c01:string}) {
  return b01+a01.c01;
}
g01({b01:1, c01:"213"});
assert(g01({b01:1, c01:"213"}) === "1213")

function f01(x01:C01) {
  assert(x01.a01 === 1);
  assert(x01.b01 === "1");
  assert(x01.c01 === true);
  assert(x01[1] === 1);
  assert(x01["x"] === 2);
  assert(x01[s01] === 3);
}
f01({a01:1, b01:"1", c01:true, 1:1, "x":2, [s01]:3})
