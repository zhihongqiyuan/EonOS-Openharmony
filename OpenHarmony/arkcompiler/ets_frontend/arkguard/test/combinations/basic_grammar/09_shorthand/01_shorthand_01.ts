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

let name1 = 'hello'
let info1 = {name1}
info1.name1
assert(name1 === 'hello');
assert(info1.name1 === 'hello');

let name2 = 'hello'
let info2 = {name: name1, name2: name1}
info2.name
assert(name2 === 'hello');
assert(info2.name === 'hello');
assert(info2.name2 === 'hello');

let __name3 = 'hello'
let info3 = {__name3}
info3.__name3
assert(__name3 === 'hello');
assert(info3.__name3 === 'hello');

let ___name3 = 'hello'
let info4 = {___name3}
info4.___name3
assert(___name3 === 'hello');
assert(info4.___name3 === 'hello');

let _name3 = 'hello'
let info5 = {_name3}
info5._name3
assert(_name3 === 'hello');
assert(info5._name3 === 'hello');