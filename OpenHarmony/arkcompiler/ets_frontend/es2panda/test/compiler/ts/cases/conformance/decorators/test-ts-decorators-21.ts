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


function propDec(p: any) {
    return function (target: any, propName: any) { print("inner-propDec-", propName); }
}

function methodDec(p: any) {
    return function (target: any, methodName: any, desc: any) { print("inner-methodDec-", methodName); }
}

function paramDec(p: any) {
    return function (target: any, methodName: any, idx: any) { print("inner-paramDec-idx=", idx); }
}

class C1 {
    #x: number = 100;

    @propDec((c: C1) => print(c.#x))
    static f1 = "field1";

    @propDec((c: C1) => print(c.#x))
    f2 = "field2";

    @methodDec((c: C1) => print(c.#x))
    static m1(p: any) { }

    @methodDec((c: C1) => print(c.#x))
    m2(@paramDec((c: C1) => print(c.#x)) p: any) { }
}
