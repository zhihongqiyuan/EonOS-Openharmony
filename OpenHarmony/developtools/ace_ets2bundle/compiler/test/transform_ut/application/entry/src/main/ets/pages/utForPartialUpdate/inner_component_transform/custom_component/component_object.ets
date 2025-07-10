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
interface optionsType {
    message: string;
}
let options: optionsType = {message: 'Hi'}
@Entry
@Component
struct Index {
  @State message1: string = 'hello'
  message2: string = 'Hi'
  build() {
    Row() {
      Child({options, message1:this.message1, message2: this.message2})
      Child2(options)
    }
  }
}

@Component
struct Child {
  private options: optionsType;
  @Link message1: string;
  @Prop message2: string;
  build() {
    Column() {
        Text(this.message1)
    }
  }
}

@Component
struct Child2 {
  private message: string;
  build() {
    Column() {
        Text(this.message)
    }
  }
}