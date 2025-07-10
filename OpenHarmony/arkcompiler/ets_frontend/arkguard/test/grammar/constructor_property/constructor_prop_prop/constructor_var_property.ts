/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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
namespace ts {
  class X {
    n: string = "";
    constructor(s:string) {
      this.n = s;
    }
    method(){
      return (this.n);
    }
  }
  let name1 = new X("global");

  class A {
    name0:string = '';
    constructor(name2:string, public name3: X) {
      name3.method();
    }
  }
  let a = new A('aa',new X("param"));
  assert(a.name3.n === 'param', 'success');
  assert(name1.n === 'global', 'success');
}