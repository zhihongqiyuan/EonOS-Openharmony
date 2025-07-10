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
struct v2DecoratorInitFromParent {
  build() {
    Column() {
      testParamChild()
      testChild({
        regular_value: "hello",
        local_value: "hello",
        provider_value: "hello",
        consumer_value: "hello"
      })
    }
  }
}

@ComponentV2
struct testParamChild {
  @Require @Param param_value: string = "hello"
  build() {}
}

@ComponentV2
struct testChild {
  regular_value: string = "hello"
  @Local local_value: string = "hello"
  @Provider() provider_value: string = "hello"
  @Consumer() consumer_value: string = "hello"
  build() {}
}