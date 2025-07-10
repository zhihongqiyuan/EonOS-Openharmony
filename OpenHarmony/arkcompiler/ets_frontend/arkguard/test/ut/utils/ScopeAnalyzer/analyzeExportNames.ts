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

import {noSymolValue} from './analyzeFunctionLike';

const var1 = 10;
const var2 = "Hello, World!";
const var3 = [1, 2, 3, 4, 5];

function func1(): string {
  return "This is function 1";
}

function func2(name: string): string {
  return `This is function 2, hello ${name}`;
}

class MyClass {
  prop1: string;
  prop2: number;

  constructor(prop1: string, prop2: number) {
    this.prop1 = prop1;
    this.prop2 = prop2;
  }

  method1(): string {
    return `Method1: ${this.prop1}`;
  }
}

interface MyInterface {
  propA: number;
  propB: string;
}

export {var1, var2, var3, func1, func2, MyClass};

export {func1 as aliasFunc1, func2 as aliasFunc2};

export const inlineExports = {var1, func1};

export {MyInterface};
