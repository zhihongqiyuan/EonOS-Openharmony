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
struct TextInputExample {
  @State text: string = ''

  build() {
    Column({ space: 20 }) {
      Text(this.text)
      TextInput({ text: $$this.text })
        .placeholderColor(Color.Grey)
        .placeholderFont({ size: 14, weight: 400 })
        .globalFancy1()
        .globalFancy()
        .caretColor(Color.Blue)
        .width(300)
    }.width('100%').height('100%').justifyContent(FlexAlign.Center)
  }

  @Styles globalFancy() {
    .width(150)
    .height(100)
    .backgroundColor(Color.Pink)
  }
}

@Styles function globalFancy1() {
  .width(150)
  .height(100)
  .backgroundColor(Color.Pink)
}