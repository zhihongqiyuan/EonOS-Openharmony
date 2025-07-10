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


declare var v1: string;
declare let v2: string;
declare const v3: string;

declare function AssertType(a: string, b: number): void;
declare function print(arg:any, arg1?:any):string;

declare class A1 {
    static a: number;
    b: string;
    func1(): void;
    func2(p: string): number;
}
declare abstract class A2 { }

declare type t = 1;
var v4: t;

declare module m { export var a; };

declare namespace ns { export var a; };

declare enum E { a = 1 };

declare interface I { a: string };
var v5: I;

print("HelloWorld");
