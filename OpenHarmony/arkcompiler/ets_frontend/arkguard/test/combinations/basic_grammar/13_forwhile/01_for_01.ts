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

let a=1;
for (let i1 = 0; i1<10;i1+=1)
  for (a;a<i1;a++) {
    let b=a;
    while (0<b) {
      b--;
    }
    assert(b===0)
  }
assert(a === 9)

let check = ["01", "11", "21", "02", "12", "22", "03", "13", "23"];
let num = 0;
for (const i2 of [1,2,3]){
  for(const i3 in [1,i2,3]) {
    assert(i3+i2 === check[num++]);
  }
}

check = ["xa", "i3a", "xb", "i3b"];
num = 0;
let o = {a:1,b:2}
for (var i3 in o){
  for (var i4 in {x:i3,i3:0}) {
    i4;
    assert(i4+i3 === check[num++]);
  }
}

do {
  a++;
  do {a++} while (10>a)
}while (a<10)
assert(a===11)

while(a>0){
  a--;
  while(0<a){a--}
}
assert(a===0)