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
let a: string = "aaaaaaaaaa"
@Entry
@Component
struct RecycleReuseIdHomeComponent {
  @State state_value: string = "100%"
  @State value: number = 1
  build() {
    Column() {
      RecycleReuseIdChild({propvalue: this.value, linkvalue: this.value})
        .border({width: 3, color: Color.Red})
        .width(this.state_value)
        .reuseId("reuse_key")
      RecycleReuseIdChild({propvalue: this.value, linkvalue: this.value})
        .border({width: 3, color: Color.Red})
        .reuseId(this.state_value)
      RecycleReuseIdChild({propvalue: this.value, linkvalue: this.value})
        .reuseId("reuse_key11111111111")
      RecycleReuseIdChild({propvalue: this.value, linkvalue: this.value})
        .border({width: 3, color: Color.Red})
        .width(this.state_value)
      RecycleReuseIdChild({propvalue: this.value, linkvalue: this.value})
      .reuseId(a)
    }
  }
}

@Reusable
@Component
struct RecycleReuseIdChild {
  @Prop propvalue: number;
  @Link linkvalue: number;

  build() {
    Column() {
      
    }
  }
}