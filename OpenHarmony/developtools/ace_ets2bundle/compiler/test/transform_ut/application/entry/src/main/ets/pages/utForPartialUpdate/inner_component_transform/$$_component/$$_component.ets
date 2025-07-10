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
const value5: boolean[] = [true, false]
let value6: {item1: boolean} = {item1: true}
let isCountDown: boolean = false

@Entry
@Component
struct HomeComponent {
  private value1: string = "hello world 1"
  private value2: string = "hello world 2"
  private value3: string = "hello world 3"
  private value4: boolean = false
  private count: number = 1000;
  myTimeController: TextTimerController  = new TextTimerController();
  @State format: string = "hh:mm:ss:ms"

  build() {
    Column() {
      Row() {
        Text(this.value1)
        Radio({value: "Radio", group: "1"})
          .checked($$this.value4)
      }
      Row() {
        Button() {
          Text(this.value1)
            .bindPopup($$value5[0], {message: "This is $$ for Array"})
        }
        .bindPopup($$this.value4, {message: "This is $$ for regular"})
        .width(100)
        .height(20)
        Text(this.value2)
          .fontSize(100)
          .bindPopup($$value6.item1, {message: "This is $$ for Obj"})
        Text(this.value3)
        Radio({value: "Radio", group: "1"})
          .checked($$value5[0])
      }
      .width(20)
      Row(){
        TextTimer({controller: this.myTimeController, isCountDown: $$isCountDown, count: $$this.count})
          .format($$this.format)
        Button("start")
          .onClick(()=>{
            this.myTimeController.start();
          })
        Button("pause")
          .onClick(()=>{
            this.myTimeController.pause();
          })
        Button("reset")
          .onClick(()=>{
            this.myTimeController.reset();
          })
      }
    }
    .height(500)
  }
}