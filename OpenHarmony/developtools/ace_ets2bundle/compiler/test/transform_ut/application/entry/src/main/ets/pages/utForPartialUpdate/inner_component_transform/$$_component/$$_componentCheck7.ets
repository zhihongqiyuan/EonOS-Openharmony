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
function aa() {
  return 'red'
}

function dd() {
  return {
    ee: 30
  }
}

@Entry
@Component
struct Index {
  @State applyToAll: boolean = false;
  build() {
    Column() {
      if (1) {
        Row() {
          Checkbox()
            .select($$this.applyToAll)
            .shape(CheckBoxShape.ROUNDED_SQUARE)
            .selectedColor(xx())
            .unselectedColor(bb().cc)
            .mark({
              strokeColor: aa()
            })
            .globalFancy1()
            .globalFancy()
            .mark({
              strokeColor: 'red'
            })
            .width(dd().ee)
            .height(30)
        }
      }
    }
  }
  @Styles globalFancy() {
    .width(150)
    .height(100)
    .backgroundColor(Color.Pink)
  }
}
function xx() {
  return 'red';
}
function bb() {
  return {
    cc: 'red'
  }
}
@Styles function globalFancy1() {
  .width(150)
  .height(100)
  .backgroundColor(Color.Pink)
}