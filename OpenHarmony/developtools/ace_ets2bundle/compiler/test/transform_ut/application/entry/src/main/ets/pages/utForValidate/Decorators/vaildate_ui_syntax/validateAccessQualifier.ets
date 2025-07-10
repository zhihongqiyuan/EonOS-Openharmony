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
struct ValidateAccessQualifierHomeComponent {
  @State message: string = "hello"

  build() {
    Column() {
      TestAccessQualifier({
        regular_value: "hello",
        state_value: "hello",
        link_value: this.message,
        prop_value: "hello",
        value: "hello"
      })
    }
    .height(500)
  }
}

@Component
struct TestAccessQualifier {
  private regular_value: string = "hello"
  @State private state_value: string = "hello"
  @StorageLink("a") public storage_value: string = "hello"
  @Consume public consume_value: string
  @Link private link_value: string
  @Provide protected provide_value: string = "hello"
  @Require @Prop private prop_value: string = "hello"
  @Require private value: string = "hello"

  build() {}
}