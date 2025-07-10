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
@Builder
function testBuilder() {
  Text("testBuilder")
    .fontSize(30)
}

@Entry
@ComponentV2
struct V2ComponentMemberTypeCheck {
  regular_value = "hello"
  @Local local_value = "hello"
  @BuilderParam builder_value = testBuilder
  @Once @Param @Require param_value = "hello"
  @Param @Require param_value1 = "hello"
  @Once @Param param_value2 = "hello"
  @Param param_value3 = "hello"
  @Event event_value = "hello"
  @Provider() provide_value = "hello"
  @Consumer() consumer_value = "hello"

  build() {
    Text("V2Component member type check")
  }
}