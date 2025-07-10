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
struct V2ToV2ComponentValidate {
  build() {}
}

class A {
  name: string
  constructor(name: string) {
    this.name = name
  }
}

enum MemberType {
  first = 1
}

@Observed
class TestObserved {

}
type DummyNewType = Set<number> | Set<string>
@ComponentV2
struct TestV2Parent1 {
  string_value: string = "hello"
  @Param string_value1: string | Set<string> = "hello"
  regular_value: Set<string> = new Set()
  @Local enum_value1: MemberType = MemberType.first
  @Provider() local_value: TestObserved
  @Consumer() enum_value: Set<number> | MemberType = MemberType.first
  @Event func_value1: Function = () => {}
  @Event func_value2: () => void  = () => {}

  @Local set_value: Set<number> | Set<string> = new Set<number>()
  @Local set_value_alias: DummyNewType = new Set<string>()
  build() {
    Column() {
      TestV1Child1({
        string_value: this.string_value,
        string_value1: this.string_value1,
        regular_value: this.regular_value,
        enum_value1: this.enum_value1,
        objectLink_value: this.local_value,
        enum_value: this.enum_value,
        set_value: this.set_value,
        set_value_alias: this.set_value_alias,
        func_value1: this.func_value1,
        func_value2: this.func_value2
      })
    }
  }
}

@Component
struct TestV1Child1 {
  @State string_value: string = "hello"
  @State string_value1: string | Set<string> = "hello"
  @Prop regular_value: Set<string> = new Set()
  @Link enum_value1: MemberType
  @Link enum_value: Set<number> | MemberType
  @Provide set_value: Set<number> | Set<string> = new Set<string>()
  @State set_value_alias: DummyNewType = new Set<string>()
  @ObjectLink objectLink_value: TestObserved
  @Prop func_value1: Function
  @BuilderParam func_value2: () => void

  build() {

  }
}

@ComponentV2
struct TestV2Parent {
  @Local set_value: Set<string> = new Set()
  @Param map_value: Map<string, string> = new Map();
  @Param @Once date_value: Date = new Date()
  @Require @Param arr_value: Array<string> = new Array()
  @Param arr_value1: Array<A> = new Array()
  @Provider() arr_value2: Array<string> = new Array()
  @Consumer() arr_value3: Array<MemberType> = new Array()
  build() {
    Column() {
      TestV1Child({
        set_value: this.set_value,
        map_value: this.map_value,
        date_value: this.date_value,
        arr_value: this.arr_value,
        arr_value1: this.arr_value1,
        arr_value2: this.arr_value2,
        arr_value3: this.arr_value3
      })
    }
  }
}

@Component
struct TestV1Child {
  @State set_value: Set<string> = new Set()
  @Prop map_value: Map<string, string> = new Map();
  @Link date_value: Date
  @Provide arr_value: Array<string> = new Array()
  @State arr_value1: Array<A> = new Array()
  @State arr_value2: Array<string> = new Array()
  @State arr_value3: Array<MemberType> = new Array()
  build() {

  }
}