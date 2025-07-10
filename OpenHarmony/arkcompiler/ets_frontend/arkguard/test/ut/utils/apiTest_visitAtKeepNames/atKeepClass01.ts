/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

// @KeepSymbol
class MyClass01 {
  static staticProperty01 = 1;

  property02: string;

  constructor(property02: string, public property03: number) {
    this.property02 = property02;
    this.property03 = 2;
  }

  myMethod01(): void{};

  static myStaticMethod(): void{};

  get myGetter(): string {
    return this.property02;
  }

  set mySetter(value: string) {
    this.property02 = value;
  }
}

// @KeepSymbol
export class MyClass02 {
  static staticProperty01 = 1;

  property03: string;
}

class MyClass03 {
  // @KeepSymbol
  constructor(){}
}

export class MyClass04 {
  // @KeepSymbol
  constructor(){}
}

class MyClass05 {
  // @KeepSymbol
  static staticProperty05_01 = 1;

  // @KeepSymbol
  property05_02: string;

  // @KeepSymbol
  constructor(property05_02: string, public property05_03: number) {
    this.property05_02 = property05_02;
    this.property05_03 = 2;
  }

  // @KeepSymbol
  myMethod05_01(): void{};

  // @KeepSymbol
  static myStaticMethod05(): void{};

  // @KeepSymbol
  get myGetter05(): string {
    return this.property05_02;
  }

  // @KeepSymbol
  set mySetter05(value: string) {
    this.property05_02 = value;
  }
}

export class MyClass06 {
  // @KeepSymbol
  property06: string;
}

export class MyClass07 {
  // not supported cases
  // @KeepSymbol @KeepSymbol
  property07_01: string;

  property07_02: string; // @KeepSymbol

  // @@KeepSymbol
  property07_03: string;

  //// @KeepSymbol
  property07_04: string;

  // cases will be collected
  // @KeepSymbol
  property07_05: string;

  //  @KeepSymbol
  property07_06: string;

  //    @KeepSymbol
  property07_07: string;

  //@KeepSymbol
  property07_08: string;
}

const myMethod: string = "myMethod";
export class MyClass08 {
  // @KeepSymbol
  'aa': 'bb';
  // @KeepSymbol
  11:11;
  // @KeepSymbol
  [myMethod](){}
}