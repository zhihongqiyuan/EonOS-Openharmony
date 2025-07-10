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

interface a0 {
    (): string;
    (a:number, b:number, c?:string): number;

    new ():string;
    new (s:string):any;

    [n: number]: ()=>string;
    [s: string]: any;

    p1: any;
    p2: string;
    p3?: any;
    p4?: number;
    p5: (s: number)=>string;

    f1():any;
    f2?():any;
    f3(a: string):number;
    f4?(s: number):string;
}

interface a1 {
    (a:number, b:number, c?:string): number;
}
let b1: a1 = (a:number,b:number,c?:string) => 1
assert(b1(1,1) === 1)

interface a2 {
    new (s:string):any;
}
let b2: a2 = class {
    a: string;
    constructor(a: string) {
        this.a = a;
    }
}
let b3 = new b2("test");
assert(b3.a = "test");

interface a3 {
    p1: any;
    p2: string;
    p3?: any;
    p4?: number;
    p5: (s: number)=>string;
}
let b4: a3 = {
    p1: 1,
    p2: "1",
    p5: (a:number) => a.toString(),
}
assert(b4.p1 === 1);
assert(b4.p2 === "1");
assert(b4.p3 === undefined);
assert(b4.p4 === undefined);
assert(b4.p5(1) === "1");


interface a4 {
    f1():any;
    f2?():any;
    f3(a: string):number;
    f4?(s: number):string;
}
let b5: a4 = {
    f1: ()=>1,
    f3: (a:string)=>1,
    f4: (a:number) => a.toString(),
}
assert(b5.f1() === 1);
assert(b5.f2 === undefined);
assert(b5.f3("1") === 1);
assert(b5.f4!(2) === "2");