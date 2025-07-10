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

import {DependencyClass, DependencyFunctionReduce, DependencyInterface} from './depency-file1'
import {NoDependencyClass, NoDependencyFunction, NoDependencyInterface} from './no-depency-file1'
import assert from 'assert';

let tempIns = new DependencyClass();
let value1 = tempIns.depencyProp1;
let value2= tempIns.depencyProp2;
let methodAns = tempIns.depencyMethod1();
let tempAns = DependencyFunctionReduce(2,1);
assert(tempAns === 1, 'success');
type tempType = DependencyInterface;
let tempValue1: tempType = {depencyProp3: "hello", depencyProp4: 2};
const value3 = tempValue1.depencyProp3;
const value4 = tempValue1.depencyProp4;

let tempInsNoDepen = new NoDependencyClass();
let value1NoDepen = tempInsNoDepen.noDepencyProp1;
let value2NoDepen= tempInsNoDepen.noDepencyProp2;
let methodAnsNoDepen = tempInsNoDepen.noDepencyMethod1();
let tempAnsNoDepen = NoDependencyFunction();
type tempTypeNoDepen = NoDependencyInterface;
let tempValue1NoDepen: tempTypeNoDepen = {noDepencyProp3: "hello", noDepencyProp4: 2};
const value3NoDepen = tempValue1NoDepen.noDepencyProp3;
const value4NoDepen = tempValue1NoDepen.noDepencyProp4;

export class TempCountry1 {
  tempPropety1: string = 'us1';
  createTime1: number = 1999;
}

class TempCountry2 {
  tempPropety2: string = 'us2';
  createTime2: number = 2000;
}

export function TempFoo1(para1: string, para2: string): string {
  return para1.concat(para2);
}
const ans1 = TempFoo1('123','456');
assert(ans1 === '123456', 'success');
function TempFoo2(para3: string, para4: string): string {
  return para3 + para4;
}
TempFoo2('123','456');
const ans2 = TempFoo2('123','456');
assert(ans2 === '123456', 'success');