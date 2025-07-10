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

export class TestClass {
  prop1: string = "123";
  prop2: number = 123;
  objProp = {
    innerProp2: "123"
  }
}

let var1 = 1;
let var2 = 2;

export function foo() {
  return 1;
}

namespace ns {
  export let var3 = 1;
  export function nsFunction() {
    return 1;
  }
}

interface TestInterface {
  feature1: number;
  feature2: string;
}

class TestClass2 {
  prop4 = 123;
  propObj = {
    innerProp: "123"
  };
}

class TestClass3 {
  exportProp1 = "123";
  exportPropObj = {
    exportInnerProp: "234"
  };
}

export {var1, var2 as v2, ns as default, TestInterface, TestClass3 as t3};

export {outterElement1, outterElement2 as o2} from './filePath';