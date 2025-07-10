/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
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
@Entry
@Component
struct Property_Observe_Validate {
  build() {

  }
}

@ObservedV2
class ObservedV2_A {
  name: string = ""
  constructor(name: string) {
    this.name = name
  }
}

@Observed
class Observed_B {

}

@ObservedV2
class ObservedV2_C {
  name: string = ""
  constructor(name: string) {
    this.name = name
  }
}

@Observed
class Observed_F {
  value: boolean
}

namespace TestNameSpace {
  @ObservedV2
  export class ObservedV2_D {

  }
  @Observed
  export class Observed_E {
    value: number
  }
}

type v2AliasType = ObservedV2_A
type v2AliasType1 = ObservedV2_A | Observed_B
type v2AliasType2 = ObservedV2_A | ObservedV2_C

@Component
struct TestV1Parent {
  // build ok
  regular_value: ObservedV2_A = new ObservedV2_A("hello")
  @State state_value6: Observed_B = new Observed_B()
  @State state_value7: Observed_B | Observed_F | TestNameSpace.Observed_E = new Observed_F()

  @State state_value: ObservedV2_A = new ObservedV2_A("hello")
  @State state_value1: TestNameSpace.ObservedV2_D = new TestNameSpace.ObservedV2_D()
  @State state_value2: v2AliasType = new ObservedV2_A("hello")
  @State state_value3: v2AliasType1 = new ObservedV2_A("hello")
  @State state_value5: v2AliasType2 = new ObservedV2_C("hello")
  @Prop prop_value: ObservedV2_A = new ObservedV2_A("hello")
  @Link link_value: ObservedV2_A
  @Provide provide_value: ObservedV2_A = new ObservedV2_A("hello")
  @Consume consume_value: ObservedV2_A
  @StorageLink("a") storage_link_value: ObservedV2_A = new ObservedV2_A("hello")
  @StorageProp("b") storage_prop_value: ObservedV2_A = new ObservedV2_A("hello")
  @LocalStorageLink("c") local_storage_link_value: ObservedV2_A = new ObservedV2_A("hello")
  @LocalStorageProp("c") local_storage_prop_value: ObservedV2_A = new ObservedV2_A("hello")

  build() {

  }
}

type v1AliasType = Observed_B
type v1AliasType1 = ObservedV2_A | Observed_B
type v1AliasType2 = Observed_B | Observed_F

@ComponentV2
struct TestV2Parent {
  // build ok
  regular_value: Observed_B = new Observed_B();
  @Local local_value7: ObservedV2_A = new ObservedV2_A("");
  @Local local_value8: ObservedV2_A | ObservedV2_C | TestNameSpace.ObservedV2_D = new ObservedV2_A("");
  @Local local_value9: TestNameSpace.ObservedV2_D = new TestNameSpace.ObservedV2_D()

  @Local local_value: Observed_B = new Observed_B()
  @Local local_value1: v1AliasType = new Observed_B()
  @Local local_value2: v1AliasType1 = new Observed_B()
  @Local local_value3: v1AliasType2 = new Observed_F()
  @Local local_value5: ObservedV2_A | Observed_B = new Observed_B()
  @Local local_value6: TestNameSpace.Observed_E = new TestNameSpace.Observed_E()
  @Param @Require param_value1: Observed_B = new Observed_B()
  @Once @Param param_value2: Observed_B = new Observed_B()
  @Param param_value3: Observed_B = new Observed_B()
  @Event event_value: Observed_B = new Observed_B()
  @Provider() provide_value: Observed_B = new Observed_B()
  @Consumer() consumer_value: Observed_B = new Observed_B()
  

  build() {

  }
}