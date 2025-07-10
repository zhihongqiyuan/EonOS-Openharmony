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

export declare namespace ns1 {
  export class TestClass1 {
      prop1: ns2.TestClass2;
      prop2: TestInterface1;
  }
  interface TestInterface1 {
      prop3: number;
  }
  export {};
}

declare namespace ns2 {
  class TestClass2 {
      prop4: TestInterface2;
  }
  interface TestInterface2 {
      prop5: number;
  }
}
export {};