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
function global() {
  Text('Global Builder')
}

@Entry
@Component
struct Index {
  judge: boolean = true
  @Builder inner(param: string) {
    Text('Inner Builder Text')
      .bindPopup(false, {
        onStateChange: (e) => {},
        builder: global()
      })
    Text('Inner Builder Text2')
      .bindPopup(false, {
        onStateChange: (e) => {},
        builder: this.judge ? global : undefined
      })
  }

  build() {
    Column() {
      Row()
        .bindMenu(this.inner("111"))
      Row()
        .bindMenu(this.judge ? this.inner("111") : global)
      Row()
        .onDragStart((event: DragEvent, extraParams: string) => {
          console.log('Text onDragStarts, ' + extraParams)
          return this.judge ? this.inner : global()
        })
      Row()
        .onDragStart((event: DragEvent, extraParams: string) => {
          console.log('Text onDragStarts, ' + extraParams)
          return {
            builder: this.judge ? this.inner() : undefined
          }
        })
      Text('Text')
        .bindPopup(false, {
          onStateChange: (e) => {},
          builder: undefined
        })
    }
  }
}