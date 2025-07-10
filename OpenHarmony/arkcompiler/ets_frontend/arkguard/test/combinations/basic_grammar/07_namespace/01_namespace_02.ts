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

type A1 = string;
type B = number;
declare namespace NS1 {
  export {NS2, A1}
}
declare namespace NS2 {
  export {NS1, B}
}
export {}

declare const try1: NS1.A1;
declare const try2: NS2.B;
declare const try3: NS1.NS2.B;
declare const try4: NS2.NS1.A1;
declare const try5: NS1.NS2.NS1.A1;
declare const try6: NS2.NS1.NS2.B;

let a1: NS1.A1 = "a";
let a2: NS2.B = 1;
let a3: NS1.NS2.B = 2;
let a4: NS2.NS1.A1 = "b";
let a5: NS1.NS2.NS1.A1 = "c";
let a6: NS2.NS1.NS2.B = 3;
assert(a1 === "a");
assert(a2 === 1);
assert(a3 === 2);
assert(a4 === "b");
assert(a5 === "c");
assert(a6 === 3);


declare namespace ns1 {
  function foo1(): void;
  let val: number;
}
declare module ns2 {
  function foo1(): void;
  let val: number;
}
declare module ns3 {
  export let val: alias;
  type alias = number;
  export {};
}
