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

let labal1 = 1;
type labal1 = number;
let num=0;
labal1:
for (var i=0;i<10;i++){
  if (i>5) {
    for(let j in [1,2,3]) {
      assert(i===6)
      assert(j==="0")
      break labal1;
    }
  }
  assert(i<=5)
  if (i<5) continue labal1;
}
assert(i===6)

let labal2 = 2;
labal2:{
  let a=1;
  if (labal2>1) break labal2;
  labal2++;
}
assert(labal2 === 2)

let labal3 = true;
labal3:do {
  labal1++;
  labal3 = !labal3;
  continue labal3;
}while (labal1<10)
assert(labal1 === 10)
assert(labal3 === false)