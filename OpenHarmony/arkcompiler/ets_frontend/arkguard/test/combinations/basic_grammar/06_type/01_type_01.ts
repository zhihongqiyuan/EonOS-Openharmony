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
import assert from 'assert';

export type callback<T> = () => T;
export type CallbackArray<T extends callback<T>> = () => T;
type t = () => t;
let a: CallbackArray<() => t>;
a = () => a;
assert(a() === a);

let var1: number = 1;
typeof var1;
type t01 = typeof var1;
let a2: t01 = 1;
assert(a2 === 1);

let c: [string, number, boolean] = ["", 1, false];
assert(c[0] === "");
assert(c[1] === 1);
assert(c[2] === false);

type a = [number, string, ...number[]];

let temp1: number | string = 1;
assert(temp1 === 1);
let temp2: number & (string | number) = 1;
assert(temp2 === 1);
type temp7 = number;
type temp8 = string;
function foo<T>(param: T extends temp7 ? temp7 : temp8) {
  return param;
}
assert(foo<number>(1) === 1);

type X2<T> = T extends { a: infer U; b: infer U } ? U : never;
let x: X2<{ a: number; b: number }> = 1;
assert(x === 1);

let temp6: (string | number)[] = [1, 2];
assert(temp6[0] === 1);
assert(temp6[1] === 2);

interface Person {
  name: string;
  age: number;
}
type PersonKeys = keyof Person;
let b: PersonKeys = "name";
assert(b === "name");

type Foo<T extends any[]> = {
  [P in keyof T]: T[P];
};
let d: Foo<number[]> = [1];
assert(d[0] === 1);

let temp3: "cc" = "cc";
assert(temp3 === "cc");
let temp4: [prop1: string, prop2: number] = ["1", 2];
assert(temp4[0] === "1");
assert(temp4[1] === 2);

type T2 = {
  description: string;
  f1(para1: number): boolean;
  (para2: number): number;
};
const temp9: T2 = (para3: number) => para3;
temp9.description = "test";
temp9.f1 = (para4: number) => {
  return para4 > 0 ? true : false;
};
assert(temp9(100) === 100);
assert(temp9.description === "test");
assert(temp9.f1(-100) === false);

type T3 = (para5: number) => number;
const temp10: T3 = (para6: number) => para6;
assert(temp10(200) === 200);

// Different grammar scenarios of indexedAccessType
type T4 = {
  U: number;
  V: string;
  W: boolean;
};
export type T5 = { X1: T4 };

let temp5: T5["X1"]["U"] = 2;
assert(temp5 === 2);

let temp11: T4["U"] = 3;
let temp12: T4["V"] = "test";
let temp13: T4["W"] = false;
assert(temp11 === 3);
assert(temp12 === "test");
assert(temp13 === false);

let temp14: T4["U" | "V"] = 4;
let temp15: T4["U" | "V"] = "test";
assert(temp14 === 4);
assert(temp15 === "test");

let temp16: T5["X1"]["U" | "V"] = 5;
let temp17: T5["X1"]["U" | "V"] = "test";
assert(temp16 === 5);
assert(temp17 === "test");

let temp18: T4[keyof T5["X1"]] = 6;
assert(temp18 === 6);

const MyArray = [
  { n1: "Alice", m1: 15 },
  { n1: "Bob", m1: 23 },
  { n1: "Eve", m1: 38 },
];
type A1 = (typeof MyArray)[1]["m1"];
let temp19: A1 = 7;
assert(temp19 === 7);

type Tuple = [string, number];
let temp20: Tuple[0] = "test";
let temp21: Tuple[1] = 9;
assert(temp20 === "test");
assert(temp21 === 9);

// Defination of unionType
type U1 = "123" | "321";
type U2 = 1234 | 4321;
type U3 = "3124" | 4123;
type U4 = "U1234" | 2143;
type U5 = "U4213" | "U4132";
type U6 = "U3412" | "3421";

// need to add "1234" into whitelist when enable prop+strProp
let temp22: U1["1234"] = "1";
assert(temp22 === "1");
let temp23: U1[4321] = "2";
assert(temp23 === "2");
let temp24: U5[4321] = "3";
assert(temp24 === "3");
let temp25: U5["1234"] = "4";
assert(temp25 === "4");
let temp26: U6[4321] = "5";
assert(temp26 === "5");
let temp27: U6["1234"] = "6";
assert(temp27 === "6");

// Define properties in type individually
type nType1 = {
  prop3: number;
  prop4: number;
  "3214": number;
  1324: number;
  [2143]: number;
  ["2314"]: number;
};

// Define properties in type and access them using indexedAccessType
type nType2 = {
  prop5: number;
  prop6: number;
  "3412": number;
  ["2341"]: number;
  1432: number;
  [1423]: number;

  // need to add into whitelist when enable prop and prop+strProp
  1243: number;
  [2134]: number;

  // need to add into whitelist when enable prop+strProp
  "3142": number;
  ["2314"]: number;
};

let temp28: nType2["prop5"] = 1;
assert(temp28 === 1);
let temp29: nType2["prop6"] = 2;
assert(temp29 === 2);
let temp30: nType2["3412"] = 3;
assert(temp30 === 3);
let temp31: nType2["2341"] = 4;
assert(temp31 === 4);
let temp32: nType2["1432"] = 5;
assert(temp32 === 5);
let temp33: nType2["1423"] = 6;
assert(temp33 === 6);

// need to add into whitelist when enable prop and prop+strProp
let temp34: nType2[1243] = 7;
assert(temp34 === 7);
let temp35: nType2[2134] = 7;
assert(temp35 === 7);

// need to add into whitelist when enable prop+strProp
let temp36: nType2[3142] = 8;
assert(temp36 === 8);
let temp37: nType2[2314] = 9;
assert(temp37 === 9);
