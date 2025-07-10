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

// @Param base case

@Entry
@ComponentV2
struct Index {
  @Local count: number = 0;
  @Local message: string = "Hello";
  @Local flag: boolean = false;
  build() {
    Column() {
      Text("Local " + this.count)
      Text("Local " + this.message)
      Text("Local " + this.flag)
      Button("change Local")
        .onClick(() => {
          this.count++;
          this.message += " World";
          this.flag = !this.flag;
        })
      Child({
        count: this.count,
        message: this.message,
        flag: this.flag
      })
    }
  }
}
@ComponentV2
struct Child {
  @Require @Param count: number;
  @Require @Param message: string;
  @Require @Param flag: boolean;
  build() {
    Column() {
      Text("Param " + this.count)
      Text("Param " + this.message)
      Text("Param " + this.flag)
    }
  }
}