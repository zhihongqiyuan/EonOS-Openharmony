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

// @KeepAsConsumer
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

// @KeepAsConsumer
export class MyClass02 {
  static staticProperty01 = 1;

  property03: string;
}

class MyClass03 {
  // @KeepAsConsumer
  constructor(){}
}

export class MyClass04 {
  // @KeepAsConsumer
  constructor(){}
}

class MyClass05 {
  // @KeepAsConsumer
  static staticProperty05_01 = 1;

  // @KeepAsConsumer
  property05_02: string;

  // @KeepAsConsumer
  constructor(property05_02: string, public property05_03: number) {
    this.property05_02 = property05_02;
    this.property05_03 = 2;
  }

  // @KeepAsConsumer
  myMethod05_01(): void{};

  // @KeepAsConsumer
  static myStaticMethod05(): void{};

  // @KeepAsConsumer
  get myGetter05(): string {
    return this.property05_02;
  }

  // @KeepAsConsumer
  set mySetter05(value: string) {
    this.property05_02 = value;
  }
}

export class MyClass06 {
  // @KeepAsConsumer
  property06: string;
}