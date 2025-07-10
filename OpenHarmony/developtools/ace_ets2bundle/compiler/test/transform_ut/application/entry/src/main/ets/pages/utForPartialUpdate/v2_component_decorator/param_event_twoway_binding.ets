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
@ComponentV2
struct HomeComponent {
  @Local local_value: string = "Foo"

  build() {
    Column() {
      TestParam({
        paramValue: this.local_value!!,
        paramValue1: "hello HomeComponent"
      })
      TestParam1({
        paramValue: "hello HomeComponent",
        myEvent: (newVal: string) => { this.local_value = newVal },
        myEvent1: (newVal: string) => this.local_value = newVal
      })
    }
  }
}

@ComponentV2
struct TestParam {
  // Two way binding
  @Param paramValue: string = "hello TestParam"
  @Event $paramValue: (value: string) => void = (value: string) => {}
  @Require @Param paramValue1: string
  
  build() {}
}

@ComponentV2
struct TestParam1 {
  @Param paramValue: string = "hello TestParam1"
  @Event myEvent: (newVal: number) => void = (newVal: number) => {}
  @Event myEvent1: (newVal: number) => void
  
  build() {}
}