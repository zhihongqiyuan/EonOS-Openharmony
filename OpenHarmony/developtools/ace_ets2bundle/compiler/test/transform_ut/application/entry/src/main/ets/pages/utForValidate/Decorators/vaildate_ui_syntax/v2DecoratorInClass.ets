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
struct Test {
  build() {}
}
class v2DecoratorInClass {
  @Local local_value: string = "hello"
  @Param param_value: string = "hello"
  @Param @Once param_once_value: string = "hello"
  @Once once_value: string = "hello"
  @Event event: () => void = () => {}
  @Provider() provider_value: string = "hello"
  @Consumer() consumer_value: string = "hello"
}