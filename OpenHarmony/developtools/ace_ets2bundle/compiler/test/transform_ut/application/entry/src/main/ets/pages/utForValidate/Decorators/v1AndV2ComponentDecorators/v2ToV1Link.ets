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
struct TestParent {
  @State state_value: string = "hello state_value"

  build() {
    Column() {
      V1()
      V1({})
    }
  }
}

@ComponentV2
struct V2 {
  @Local local_value: string = "hello local_value"
  build() {
    Column() {
      V1()
      V1({})
      V1({link_value: this.local_value})
      V1({link_value: "hello"})
    }
  }
}

@Component
struct V1 {
  @Link link_value: string
  build() {
    Text("hello V1")
  }
}