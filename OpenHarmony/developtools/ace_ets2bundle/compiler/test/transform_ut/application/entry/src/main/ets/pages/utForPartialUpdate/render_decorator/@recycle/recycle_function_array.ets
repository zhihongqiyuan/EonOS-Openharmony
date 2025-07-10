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
struct RecycleFuncArrHomeComponent {
  @State value: number = 1
  build() {
    Column() {
      RecycleFuncArrChild({propvalue: this.value, linkvalue: this.value})
    }
  }
}

@Reusable
@Component
struct RecycleFuncArrChild {
  @State state_value: number = 1;
  reguar_value: string = "hello"
  build() {
    Column() {
      Circle()
        .onClick(() => {
          console.log("hello")
        })
        .strokeDashArray(["hello", this.reguar_value])
        .height(100)
      Circle()
        .strokeDashArray([this.state_value])
      Text("hello")
        .onClick(() => {
          console.log("hello")
        })
    }
  }
}