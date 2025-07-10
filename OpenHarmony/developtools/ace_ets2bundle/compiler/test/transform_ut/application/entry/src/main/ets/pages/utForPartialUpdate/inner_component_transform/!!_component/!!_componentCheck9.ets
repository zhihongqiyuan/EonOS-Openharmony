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

/*
 * This is a test case that demonstrates the use of the built-in components 'Refresh' and 'ListItem' with two-way sync binding functionality within if, else if, and else statements.
 */

@Entry
@ComponentV2
struct Index {
  @Local isRefreshing: boolean = true
  @Local num: 0 | 1 | 2 = 0
  @Local text: string = ''
  @Local arr: number[] = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
  @Local arrSelect: boolean[] = [true, false, true, false, true, false, true, false, true, false]
  controller: TextInputController = new TextInputController()

  build() {
    Row() {
      Column() {
        TextInput({ text: this.text!!, placeholder: 'input your word...', controller: this.controller })
        if (this.num === 0) {
          Refresh({refreshing: this.isRefreshing!!})
        } else if (this.num === 1) {
          Refresh({refreshing: this.isRefreshing!!})
        } else if (this.num === 2) {
          Refresh({refreshing: this.isRefreshing!!})
        } else if (this.num === 3) {
          Refresh({refreshing: this.isRefreshing!!})
        } else {
          Refresh({refreshing: this.isRefreshing!!})
        }
        List({ space: 20, initialIndex: 0 }) {
          ForEach(this.arr, (item: number, index: number) => {
            ListItem() {
              Text('' + item)
            }
            .selectable(true)
            .selected(this.arrSelect[index]!!)
          }, (item: string) => item)
        }
      }
    }
  }
}