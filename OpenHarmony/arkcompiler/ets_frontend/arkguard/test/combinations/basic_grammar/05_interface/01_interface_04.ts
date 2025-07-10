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

interface Foo {
    method(a: number): string;
    optionalMethod?(a: number): string;
    property: string;
    optionalProperty: string;
}
  
class Foo {
    additionalProperty: string = '';
    additionalMethod(a: number): string {
      return this.method(0);
    }
}
class Bar extends Foo {
    method(a: number): string {
      return this.optionalProperty;
    }
}

let a = new Bar();
assert(a.method(1) === undefined)
assert(a.optionalMethod === undefined)
assert(a.property === undefined)
assert(a.optionalProperty === undefined)
assert(a.additionalProperty === '')
assert(a.additionalMethod(1) === undefined)