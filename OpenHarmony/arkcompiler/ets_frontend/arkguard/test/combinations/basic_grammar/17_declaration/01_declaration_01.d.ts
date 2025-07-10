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


declare let a1:number;
declare var a2:string;
declare const a3:boolean;
export const a4=1;

declare class C1 {
  f1:number;
  f2():boolean;
  f3:()=>{}
}

declare interface I {
  f1:number;
  f2():I;
  f3:()=>{}
}

declare type t = number | string | t[];

declare function f1(a:C1, b:{a:E2}):{a:t}

declare enum E1{a,b,c}
declare const enum E2{e,f,g,c}

declare namespace ns {
  let a1:number;
  var a2:string;
  const a3:boolean;
  export const a4=1;

  class C1 {
    f1:number;
    f2():boolean;
    f3:()=>{};
  }

  interface I{
    f1:number;
    f2():ns.I;
    f3:()=>{}
  }

  type t = number | string | t[];
  function f1(a:ns.C1, b:{a:ns.E2}):{a:ns.t}

  enum E1{a=1,b,c}
  const enum E2{e=2,f,g,c}
}