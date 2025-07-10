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
class Model {
    value: string
    constructor(value: string) {
      this.value = value
    }
  }
  @Component
  struct MyComponent {
    @State title: Model = {value: 'Hello World'}
    @State count: number = 0
    private toggle: string = 'Hello World'
    private increaseBy: number = 1
  
    build() {
      Column() {
        Text('title.value: ' + this.title.value)
        Button() {
          Text('Click to change title')
        }.onClick(() => {
          this.title.value = (this.toggle == this.title.value) ? 'Hello World' : 'Hello UI'
        })
  
        Button() {
          Text('Click to increase count=' + this.count)
        }.onClick(() => {
          this.count += this.increaseBy
        })
      }
    }
  }
  @Entry
  @Component
  struct EntryComponent {
    build() {
      Column() {
        MyComponent({count: 1, increaseBy: 2})
        MyComponent({title: {value: 'Hello, World 2'}, count: 7})
      }
    }
  }