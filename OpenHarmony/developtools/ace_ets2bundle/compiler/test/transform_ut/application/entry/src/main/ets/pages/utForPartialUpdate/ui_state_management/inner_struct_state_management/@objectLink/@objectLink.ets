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
var nextID: number = 0;
@Observed
class Model {
  text : string = '';
  color: string = '#00ff00';
  constructor(t: string, c: string) {
    this.text = t;
    this.color = c;
  }
}

@Component
struct CustomText {
  @ObjectLink model: Model;
  build() {
    Row() {
      Text(this.model.text)
    }
  }
}

@Entry
@Component
struct Parent {
  nextId: number = 1;
  @State models : Model[] = [ new Model('0', '#ffffff'), new Model('1', '#fff456') ];
  build() {
    Column() {
      ForEach (this.models, (item) => {
          CustomText({model: item})
        },
        (item) => item.text
      )
    }
  }
}