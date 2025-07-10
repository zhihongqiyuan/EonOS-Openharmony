/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

if (typeof (print) !== 'function') {
  print = function (msg) {
    console.log(msg);
  }
}

function fn(title) {
  return new Promise((resolve, reject) => {
    resolve(title);
  }).then((res) => {
    print(res);
  });
}

class Test {
  gen() {
    fn("#1");
    fn("#2");
    fn("#3");
  }
}

let test = new Test();
let g = test.gen();

print('end');
