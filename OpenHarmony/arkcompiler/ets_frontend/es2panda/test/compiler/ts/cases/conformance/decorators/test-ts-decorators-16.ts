/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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


function classDec() {
    print("classDec");
    return function (func: Function) { print("inner-classDec"); }
}

function propDec(p: any) {
    print("propDec-", p);
    return function (target: any, propName: any) { print("inner-propDec-", propName); }
}

function methodDec(p: any) {
    print("methodDec-", p);
    return function (target: any, methodName: any, desc: any) { print("inner-methodDec-", methodName); }
}

function paramDec(value: any) {
    print("paramDec-", value);
    return function (target: any, methodName: any, idx: any) { print("inner-paramDec-idx=", idx); }
}

@classDec()
class A {
    @propDec("prop1")
    prop1 = 1;

    constructor(@paramDec('start3') start: number, @paramDec('end3') end: number) { }

    @propDec("prop2")
    static prop2 = 2;

    @propDec("prop3")
    prop3 = 3;

    @methodDec("classMethod1")
    static classMethod1(@paramDec('start1') start: number, @paramDec('end1') end: number) {
        return {};
    }

    @methodDec("classMethod2")
    @methodDec("classMethod22")
    classMethod2(@paramDec('start2') @paramDec('start22') start: number, @paramDec('end2') end: number) {
        return {};
    }
}
