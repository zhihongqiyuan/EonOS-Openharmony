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

import assert from 'assert';

// 不新增变量
let [PropDemo1, PropDemo2] = ['akria', 12];
let {...reset1} = {'name2': 'akria2', age2: 18};

assert(PropDemo1 === 'akria', 'success');
assert(PropDemo2 === 12, 'success');
assert(reset1.name2 === 'akria2', 'success');
assert(reset1.age2 === 18, 'success');

function funcLayer(){
  let [PropDemo3, PropDemo4] = ['akria3', 13];
  let {...reset2} = {'name2': 'akria4', age2: 18};

  assert(PropDemo3 === 'akria3', 'success');
  assert(PropDemo4 === 13, 'success');
  assert(reset2.name2 === 'akria4', 'success');
  assert(reset2.age2 === 18, 'success');
}
funcLayer();

// 新增变量
let { newName, ...reset3 } = {'newName': 'akria5', newAge: 18};
assert(newName === 'akria5', 'success');
assert(reset3.newAge === 18, 'success');

function foo() {
  return { Propx1: 1, Propy1: 2 };
}

const { Propx1, Propy1 } = foo();
assert(Propx1 === 1, 'success');
assert(Propy1 === 2, 'success');

let Propx2 =3;
let Propy2 =4;
const { Propx2: tempX2, Propy2: tempY2 } = {Propx2, Propy2}
assert(tempX2 === 3, 'success');
assert(tempY2 === 4, 'success');
