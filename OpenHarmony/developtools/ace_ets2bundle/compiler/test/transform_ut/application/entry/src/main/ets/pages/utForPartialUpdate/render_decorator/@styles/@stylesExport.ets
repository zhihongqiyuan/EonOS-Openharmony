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
@Styles function overallStyles() {

}

@Styles function overallStylesAttr() {
  .backgroundColor(Color.Red)
}
@Entry
@Component
struct MyComponent {
  @Styles partStyles() {

  }

  @Styles partStylesAttr() {
    .backgroundColor(Color.Red)
  }
  build() {
    Column() {
      Text('Hello world - 0')
        .partStyles()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: this.partStyles
        })
      Text('Hello world - 1')
        .partStylesAttr()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: this.partStylesAttr
        })
      Text('Hello world - 4')
        .overallStyles()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: overallStyles
        })
      Text('Hello world - 6')
        .overallStylesAttr()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: overallStylesAttr
        })
      Text('Hello world - 8')
        .lastPartStyles()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: this.lastPartStyles
        })
      Text('Hello world - 9')
        .lastPartStylesAttr()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: this.lastPartStylesAttr
        })
      Text('Hello world - 12')
        .lastOverallStyles()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: lastOverallStyles
        })
      Text('Hello world - 13')
        .lastOverallStylesAttr()
        .stateStyles({
          focused:{
            .backgroundColor("#ffffeef0")
          },
          pressed: {
            .backgroundColor("#ff707070")
          },
          normal: lastOverallStylesAttr
        })
    }
  }
  @Styles lastPartStyles() {

  }

  @Styles lastPartStylesAttr() {
    .backgroundColor(Color.Red)
  }
}

@Styles function lastOverallStyles() {

}

@Styles function lastOverallStylesAttr() {
  .backgroundColor(Color.Red)
}
