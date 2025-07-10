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
struct Index {
  @State WIDTH_AND_HEIGHT: Array<{ w: number, h: number }> = [
    { w: 10, h: 10 },
    { w: 20, h: 20 },
    { w: 30, h: 30 },
    { w: 40, h: 40 },
    { w: 50, h: 50 }
  ]
  build() {
    Row() {
      Column() {
        ForEach(this.WIDTH_AND_HEIGHT, ({ w, h }) => {
          Button()
            .width(w)
            .height(h)
        }, item => item.toString())
      }
      .width('100%')
    }
    .height('100%')
  }
}