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

declare namespace ns2 {
  export class A {
    keptPropertyPara2: B;
    static method1(keptPara1: number, ...keptPara2: string[]): void;
    method2(keptPara1: number, keptPara2: (defaultKeptPara3: number) => string, para4?: number): void;
    method3(keptPara5?: number, ...keptPara6: number[]): void;
    constructor(keptPara1: number, keptPropertyPara2: number, keptPara3: (defaultKeptPara4: number) => string);
    field1?: (defaultKeptPara1: number) => void;
    field2: (defaultKeptPara2: number) => number;
    method4(keptPara1: (defaultKeptPara2: string) => number, keptPara2: { key1: number, key2: string }): void;
    method5(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): number;
    manualKeepMethod1(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): number;
  }

  class B {
    constructor(keptPara1: number, keptPropertyPara2: number, keptPara3: (defaultKeptPara4: number) => string);
    field6?: (defaultKeptPara1: number) => void;
    method10(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): number;
    manualKeepMethod1(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): void;
  }

  export class manualKeepClassName {
    constructor(keptPara1: number, keptPropertyPara2: number, keptPara3: (defaultKeptPara4: number) => string);
    field5?: (defaultKeptPara1: number) => void;
    method9(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): number;
    manualKeepMethod1(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): void;
  }

  export interface C {
    method6(defaultKeptPara1: number, ...defaultKeptPara2: string[]): void;
    constructor(defaultKeptPara1: number): void;
    field3: (defaultKeptPara1: number, ...defaultKeptPara2: number[]) => void;
    field4: (defaultKeptPara1: number, defaultKeptPara2: (defaultKeptPara3: string) => number) => string;
    method7(defaultKeptPara1: (defaultKeptPara2: string) => number, defaultKeptPara3: { key1: number, key2: string }): void;
    method8(defaultKeptPara1: (defaultKeptPara2: number) => number, defaultKeptPara3: number): void;
    manualKeepMethod1(defaultKeptPara1: (defaultKeptPara2: number) => number, defaultKeptPara3: number): void;
  }

  export function foo1(keptPara1: number): void;
  export function foo2(keptPara1: number, ...keptPara2: string[]): void;
  export function foo3(keptPara1?: (defaultKeptPara2: number) => void): void;
  function foo4(keptPara1: number, keptPara2?: (defaultKeptPara3: number) => void): void;
  export function manualKeepFoo1(keptPara1: number, keptPara2?: (defaultKeptPara3: number) => void): void;

  export let c: (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => void;
  export let d: (defaultKeptPara1: number, defaultKeptPara2?: ((defaultKeptPara3: number) => void) | undefined) => void;
  export function foo(keptPara1: number, obj: {
    prop1: number;
    prop2: string;
  }): void;

  export let e: (defaultKeptPara1: number) => void;
  export const sum: (defaultKeptPara1: number, defaultKeptPara2: number) => void;
  export let myfun1: (defaultKeptPara1: (defaultKeptPara2: number) => number) => number;
  export const sum2: (defaultKeptPara1: number, defaultKeptPara2: (defaultKeptPara3: number) => number, ...defaultKeptPara4: string[]) => void;
  export let a: (defaultKeptPara1: (defaultKeptPara2: number) => number, ...defaultKeptPara3: string[]) => void;
  export let b: (defaultKeptPara1: (defaultKeptPara2: number) => number, defaultKeptPara3?: ((defaultKeptPara4: number) => void) | undefined) => void;
  export let f: (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => void;
  export let g: (defaultKeptPara1: number, defaultKeptPara2?: ((defaultKeptPara3: number) => void) | undefined) => void;

  export type typeFunc = (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => {};
  export type typeFunc2 = (defaultKeptPara1: number, defaultKeptPara2: (defaultKeptPara3: number) => number) => {};

  export interface person<T1> {
    method<T2>(): void;
  }
  export class person2<T3> {
    method2<T4>(): void;
  }
  export function foo5<T5>(): void;
  export { };
}

declare const simpleType: 1;
declare class classProperties {
  static method1(keptPara1: number, ...keptPara2: string[]): void;
  "method2"(keptPara1: number): void;
  ["method3"](keptPara1: number): void;
  [111](keptPara1: number): void;
  [simpleType](keptPara1: number): number;
  222(keptPara1: number): void;
}