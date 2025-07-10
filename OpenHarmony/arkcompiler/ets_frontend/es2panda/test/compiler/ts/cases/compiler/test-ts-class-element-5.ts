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


class A3 {
  public b;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let a3 = new A3();

class B3 {
  private b;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let b3 = new B3();

class C3 {
  protected b;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let c3 = new C3();

class D3 {
  b;
  #c = 2;
  constructor() {
    print("b" in this);
  }
}
let d3 = new D3();
