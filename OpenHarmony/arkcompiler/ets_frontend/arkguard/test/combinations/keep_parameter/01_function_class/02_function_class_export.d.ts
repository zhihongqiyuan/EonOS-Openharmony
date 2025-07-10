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

export declare class A {
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

declare class B {
  constructor(keptPara1: number, keptPropertyPara2: number, keptPara3: (defaultKeptPara4: number) => string);
  field10?: (defaultKeptPara1: number) => void;
  method6(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): number;
  manualKeepMethod1(keptPara1: (defaultKeptPara2: number) => number, keptPara3: number): void;
}

export declare class manualKeepClassName {
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

export declare function foo1(keptPara1: number): void;
export declare function foo2(keptPara1: number, ...keptPara2: string[]): void;
export declare function foo3(keptPara1?: (defaultKeptPara2: number) => void): void;
declare function foo4(keptPara1: number, keptPara2?: (defaultKeptPara3: number) => void): void;
export declare function manualKeepFoo1(keptPara1: number, keptPara2?: (defaultKeptPara3: number) => void): void;

export declare let c: (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => void;
export declare let d: (defaultKeptPara1: number, defaultKeptPara2?: ((defaultKeptPara3: number) => void) | undefined) => void;
export declare function foo(keptPara1: number, objPara: {
  prop1: number;
  prop2: string;
}): void;

export declare let e: (defaultKeptPara1: number) => void;
export declare const sum: (defaultKeptPara1: number, defaultKeptPara2: number) => void;
export declare let myfun1: (defaultKeptPara1: (defaultKeptPara2: number) => number) => number;
export declare const sum2: (defaultKeptPara1: number, defaultKeptPara2: (defaultKeptPara3: number) => number, ...defaultKeptPara4: string[]) => void;
export declare let a: (defaultKeptPara1: (defaultKeptPara2: number) => number, ...defaultKeptPara3: string[]) => void;
export declare let b: (defaultKeptPara1: (defaultKeptPara2: number) => number, defaultKeptPara3?: ((defaultKeptPara4: number) => void) | undefined) => void;
export declare let f: (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => void;
export declare let g: (defaultKeptPara1: number, defaultKeptPara2?: ((defaultKeptPara3: number) => void) | undefined) => void;

export type typeFunc = (defaultKeptPara1: number, ...defaultKeptPara2: string[]) => {};
export type typeFunc2 = (defaultKeptPara1: number, defaultKeptPara2: (defaultKeptPara3: number) => number) => {};

export interface person<T1> {
  method<T2>(): void;
}
export declare class person2<T3> {
  method2<T4>(): void;
}
export declare function foo5<T5>(): void;
export { };