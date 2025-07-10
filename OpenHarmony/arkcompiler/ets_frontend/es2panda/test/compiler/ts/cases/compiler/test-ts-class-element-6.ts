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


class A4 {
  public b = 1;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let a4 = new A4();

class B4 {
  private b = 1;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let b4 = new B4();

class C4 {
  protected b = 1;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let c4 = new C4();

class D4 {
  b = 1;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let d4 = new D4();
