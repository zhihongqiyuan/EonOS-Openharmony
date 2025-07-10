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

class A5 {
  prop_5 = 5;
  constructor(public para1: number, private para2: string, protected para3: boolean, readonly para4: number, para5: string) {
    para5 = para5 + 1;
    let temp1 = para1;
    let temp2 = para2;
    let temp3 = para3;
    let temp4 = para4;
    this.prop_5 = para4;
  }
  private myMethod(methodPara1: string, methodPara2: number): void {
  }
}

function myFunction(funcPara1: number, funcPara2: number): number {
  return funcPara1 + funcPara2;
}