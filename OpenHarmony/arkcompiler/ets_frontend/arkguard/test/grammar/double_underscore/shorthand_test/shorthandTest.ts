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

let __prop1 = "123";
let __prop2 = 456;
let __prop3 = true;
let ___prop4 = 123;

const __method1 = () => {};

const __method2 = (param: string) => {
  return `Hello, ${param}`;
};

const __nestedObj = {
  __nestedProp: "nested",
  __nestedMethod() {
    return "This is a method inside a nested object";
  }
};

const __array = [1, 2, 3, 4];

const props = {
  __prop1,
  __prop2,
  __prop3,
  __method1,
  __method2,
  __nestedObj,
  __array,
  ___prop4
};