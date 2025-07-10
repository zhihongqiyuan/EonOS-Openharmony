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
  build() {
    Column() { 
    }
  }
}

@Builder
function testBuilder() {
  Text("testBuilder")
}

@ComponentV2
struct ChildComponent {
  @Local static local_value: string = "hello"
  @Param static param_value: string = "hello"
  @Event static event_value: Function = () => {}
  @Provider() static provider_value: number = 0
  @Consumer("a") static consumer_value: boolean = true
  @BuilderParam static builder_value: Function = testBuilder

  @Monitor("local_value")
  static testMonitor() {}

  @Computed
  static get fullName() {
    return ChildComponent.param_value
  }

  build() {
    Column() {}
  }
}