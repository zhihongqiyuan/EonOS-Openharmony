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

declare global {

}
declare global {
  export module global1 {
    let val: number;
    type t = number;
  }
}
let a:global1.t = 1;
assert(a===1);

namespace ns {
  export module a {
    export function foo() {
      return 1;
    }
  }
  module b {

  }
}
assert(ns.a.foo() === 1);

export {}