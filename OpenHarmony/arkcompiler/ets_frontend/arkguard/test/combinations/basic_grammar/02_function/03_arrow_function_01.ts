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

let a = 1;
let b = 2;
let c = 3;
let x1 = ()=>{};
assert(x1() === undefined);
let x2 = () => 1?2:3;
assert(x2() === 2);
let d = () => a?b:c;
assert(d() === 2);
let x3 = ([]) => 1?2:3;
assert(x3([]) === 2);
let e = ([]) => a?b:c;
assert(e([]) === 2);
let x4 = (...a:any):any => a;
assert(x4(1,2,3)[0] === 1);
let x5 = ()=> {1?2:3};
assert(x5() === undefined);
let f = ()=> {a?b:c};
assert(f() === undefined);
let x6 = ([]) => {1?2:3};
assert(x6([]) === undefined);
let g = ([]) => {a?b:c};
assert(g([]) === undefined);