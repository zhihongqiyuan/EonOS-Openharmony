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

/*
   This test case is used to verify the presence of custom components decorated by @ReusbaleV2 in the template attribute of the Repeat component.
 */
@Entry
@ComponentV2
struct validateReusableV2InRepeat {
  @Local arr:string[] = ['1','2','3'];
  build() {
    Column() {
      List() {
        Repeat<string>(this.arr)
          .each((obj: RepeatItem<string>) => {})
          .virtualScroll()
          .template('temp', (obj: RepeatItem<string>) => { ListItem() {
            Column() { Row() { V2R() } } } })
      }
      Tabs() {
        Repeat<string>(this.arr)
          .each((obj: RepeatItem<string>) => {})
          .virtualScroll()
          .template('temp', (obj: RepeatItem<string>) => {
            Repeat<string>(this.arr)
              .each((obj: RepeatItem<string>) => {})
              .virtualScroll()
              .template('temp', (obj: RepeatItem<string>) => { TabContent() {
                Row() {
                  List() { ListItem() { V2R() } }
                  ForEach(this.arr,()=>{
                    V2R()
                    List() { ListItem() { V2R() } } })
                  if (this.arr.length === 1) {
                    V2R()
                  } else if (this.arr.length ===2) {
                    V2R()
                  } else {
                    if (this.arr.length) {
                      V2R()
                    }
                    V2R()
                  }}} })})}}
  }
}

@ReusableV2
@ComponentV2
export struct V2R {
  build() {}
}