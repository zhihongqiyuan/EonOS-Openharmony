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
 * This is a test case about the use of "!!" two-way sync binding in the component 'SideBarContainer'.
 */

@Entry
@ComponentV2
struct SideBarContainerExample {
  @Local arr: number[] = [1, 2, 3]
  @Local current: number = 1
  @Local showSideBar: boolean = true
  @Local isOn1: boolean = true
  @Local isOn2: boolean = true

  build() {
    SideBarContainer(SideBarContainerType.Embed) {
      Column() {
        ForEach(this.arr, (item: number) => {
          Column({ space: 5 }) {
            Text("Index0" + item)
              .fontColor(this.current === item ? '#0A59F7' : '#999')
          }
          .onClick(() => {
            this.current = item
          })
        }, (item: string) => item)
      }

      Column() {
        Text('SideBarContainer content text1').fontSize(25)
        Text('SideBarContainer content text2').fontSize(25)
        Toggle({ type: ToggleType.Switch, isOn: this.isOn1!! })
        Toggle({ type: ToggleType.Switch, isOn: this.isOn2 })
      }
    }
    .showSideBar(this.showSideBar!!)
  }
}