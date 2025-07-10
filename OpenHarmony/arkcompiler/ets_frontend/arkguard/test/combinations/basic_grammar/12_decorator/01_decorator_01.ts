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

let x:number = 1;
let y:string = "1";
let z:boolean  = true;

let classNum = 0;
let methosNum = 0;
let PropertyNum = 0;
let f1:ClassDecorator = () => {classNum++;};
let f2:MethodDecorator = () => {methosNum++;};
let f3:PropertyDecorator = () => {PropertyNum++;};

function f11(a:number):ClassDecorator {assert(a === 1); return f1}
function f21(b:string):MethodDecorator {assert(b === "1");return f2}
function f31(c:boolean):PropertyDecorator {assert(c === true);return f3}

type f1 = number;
type f2 = string;
type f3 = boolean;

@f1
@f11(x)
class C{
  @f2
  @f21(y)
  f01(){}
  @f2
  @f21(y)
  set a001(_a: number) {this.a01 = _a}
  @f21(y)
  @f2
  get a002():number {return this.a01}
  @f3
  @f31(z)
  a01:number=1;
}
assert(classNum === 2);
assert(methosNum === 6);
assert(PropertyNum === 2);
let c01 = new C();
assert(c01.a002 === 1);
c01.a001=2;
assert(c01.a01 === 2);
assert(c01.a002 as number === 2);