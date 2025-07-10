/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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

// This test case is used to verify @Local object.
@ObservedV2
class Info {
  @Trace name: string;
  @Trace age: number;
  constructor(name: string, age: number) {
    this.name = name;
    this.age = age;
  }
}
@Entry
@ComponentV2
struct Index {
  info: Info = new Info("Tom", 25);
  @Local localInfo: Info = new Info("Tom", 25);
  build() {
    Column() {
      Text(`info: ${this.info.name}-${this.info.age}`) // Text1
      Text(`localInfo: ${this.localInfo.name}-${this.localInfo.age}`) // Text2
      Button("change info&localInfo")
        .onClick(() => {
          this.info = new Info("Lucy", 18); // Text1不会刷新
          this.localInfo = new Info("Lucy", 18); // Text2会刷新
      })
    }
  }
}
