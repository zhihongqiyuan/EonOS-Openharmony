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
struct v1DecoratorInComponentV2 {
  @State state_value: string = "hello"
  @Prop prop_value: string = "hello"
  @Link link_value: string
  @Provide provide_value: string = "hello"
  @Consume consumer_value: string
  @Watch("aa") watch_value: string = "hello"
  @State @Watch("aa") watch_value1: string = "hello"
  @StorageLink("b") storageLink_value: string = "hello"
  @StorageProp("b") storageProp_value: string = "hello"
  @LocalStorageLink("b") localStorageLink_value: string = "hello"
  @LocalStorageProp("b") localStorageProp_value: string = "hello"
  @ObjectLink objectLink_value: AA

  aa() {}

  build() {
    Column() {}
  }
}

@Observed
class AA {}