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

class C {
  [1](){
    return 1;
  }
  [1?2:3](){
    return 2;
  }
  method1(){
    return 3;
  }
  #method2(){
    return 4;
  }
  method2(){
    return this.#method2();
  }
  $method3(){
    return 5;
  }
  _method4(){
    return 6;
  }
  __method5(){
    return 7;
  }
  * method6(){
    return 8;
  }
  async method7(){
    return 9;
  }
  async * method8(){
    return 10;
  }
}

let c = new C();
assert(c[1]() === 1);
assert(c[2]() === 2);
assert(c.method1() === 3);
assert(c.method2() === 4);
assert(c.$method3() === 5);
assert(c._method4() === 6);
assert(c.__method5() === 7);
assert(c.method6().next().value === 8);
async function f() {
  assert(await c.method7() === 9);
  assert((await c.method8().next()).value === 10);
}
f();
